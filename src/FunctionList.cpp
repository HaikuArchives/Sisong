
#include "editor.h"
#include "FunctionList.fdh"
#include <cctype>


CFunctionList::CFunctionList(BRect frame, uint32 resizingMode)
		: BView(frame, "Function List", resizingMode, B_WILL_DRAW)
{
	// create the BListView which will contain the list of functions.
	// because the BScrollView will not shrink it's target, we have to
	// compute the width of the scrollbars to be added by the BScrollView
	// and reserve space for them within the panel.
	BRect rc(Bounds());
	rc.right -= B_V_SCROLL_BAR_WIDTH;
	rc.left += 4;
	rc.top += 2;
	rc.bottom -= 1;
	rc.right -= 1;
	list = new BListView(rc, "functionlist_listview");
	//list->SetResizingMode(B_FOLLOW_ALL);

	BFont fnt(be_plain_font);
	fnt.SetSize(11);
	list->SetFont(&fnt);

	list->SetInvocationMessage(new BMessage(M_FUNCTIONLIST_INVOKE));

	// create scrollview
	sv = new BScrollView("functionlist_scrollview", list, B_FOLLOW_ALL, \
						0, false, true);

	// adopt our child scrollview
	AddChild(sv);
}

CFunctionList::~CFunctionList()
{
	RemoveChild(sv);
	sv->RemoveChild(list);

	delete list;
	delete sv;

	LineNumbers.MakeEmpty();
}

void CFunctionList::SetHeight(int height)
{
	sv->ResizeTo(FL_WIDTH-3, height);
	list->ResizeTo(FL_WIDTH-B_V_SCROLL_BAR_WIDTH, height);
}

/*
void c----------------------------() {}
*/

void CFunctionList::ScanIfNeeded()
{
	if (!UpToDate)
	{
		ScanAll();
	}
}

void CFunctionList::ScanAll()
{
	if (!editor.curev) return;
	Scan(0, editor.curev->nlines - 1);
}

void CFunctionList::Scan(int startline, int endline)
{
clLine *line, *lastline;
int y;

	if (!editor.curev) return;

	// sanity checks
	if (endline < startline)
	{
		staterr("Invalid FL scan: %d-%d", startline, endline);
		return;
	}

	if (startline < 0) startline = 0;
	if (startline >= editor.curev->nlines) endline = (editor.curev->nlines - 1);

	if (endline < 0) endline = 0;
	if (endline >= editor.curev->nlines) endline = (editor.curev->nlines - 1);

	// initilize pointers
	//stat(" ** FL Scan Begin %d-%d", startline, endline);
	NewResults.MakeEmpty();

	line = editor.curev->GetLineHandle(startline);
	lastline = editor.curev->GetLineHandle(endline);
	y = startline;

	// initiate scan
	while(line)
	{
		ScanLine(line, y++);

		if (line==lastline) break;
		line = line->next;
	}

	// update the list with the new results set
	ApplyNewResults();
}


void CFunctionList::ScanLine(clLine *line, int lineNumber)
{
BString *bsLine;
const char *line_str;
const char *fnstart, *fnend, *ptr;
char *funcname;
char *funcname_with_space = NULL;
FLResult *result;

	bsLine = line->GetLineAsString();
	line_str = bsLine->String();

	if (line_str[0] == '\t') goto not_a_match;
	if (line_str[0] == ' ') goto not_a_match;
	if (line_str[0] == '#') goto not_a_match;

	// "pragma mark" support used in Haiku sources
	if (line_str[0] == '/')
	{
		int start = bsLine->Length() - 1;

		while((line_str[start]==TAB || line_str[start]==' ') &&
			start > 0) { start--; }

		start -= 13;	// 13 = length of string - 1

		if (start >= 0)
		{
			if (strbegin(&line_str[start], "#pragma mark -"))
			{
				// yeah, it's cheeesy
				funcname_with_space = smal_strdup(" c----------------------------");
				goto pragma_mark;
			}
		}

		// if we didn't find a pragma, then there's probably no function where the
		// line begins with a '/', so call it a no-match.
		goto not_a_match;
	}

	// if line ends in a semicolon, it's out of the running immediately.
	fnend = line_str + (bsLine->Length() - 1);
	rept
	{
		if (fnend <= line_str) goto not_a_match;
		if (*fnend != 9 && *fnend != ' ') break;
		fnend--;
	}

	if (*fnend == ';') goto not_a_match;
	// exclude initilizer lists in C++ code, but include functions
	// which are split onto multiple lines without a "\"
	if (*fnend == ',')
	{
		if (fnend != line_str && *(fnend-1) == ')')
			goto not_a_match;
	}

	// look for the opening paren of a function def
	fnend = strchr(line_str, '(');
	if (!fnend) goto not_a_match;

	// move backwards from paren until we hit something other than whitespace
	rept
	{
		fnend--;
		if (fnend <= line_str) goto not_a_match;
		if (*fnend != ' ' && *fnend != 9) { fnend++; break; }
	}

	// get the function name.
	// move backwards and search for start of func name.
	fnstart = fnend;
	rept
	{
		fnstart--;
		if (fnstart < line_str) { fnstart = line_str; break; }

		if (*fnstart == '\t' ||
			*fnstart == ' ' ||
			*fnstart == '*' ||
			*fnstart == ':')
		{
			fnstart++;
			break;
		}
	}

	if (fnstart == fnend) goto not_a_match;

	// grab function name: it lies between fnstart and fnend
	{
		int fn_len = (fnend - fnstart);
		funcname_with_space = (char *)smal(fn_len + 2);
		*funcname_with_space = ' ';
		funcname = (funcname_with_space + 1);
		funcname[fn_len] = 0;
		memcpy(funcname, fnstart, fn_len);
	}

	// filter out reserved keywords which are obviously false positives
	if (!strcmp(funcname, "if")) goto not_a_match;
	if (!strcmp(funcname, "for")) goto not_a_match;
	if (!strcmp(funcname, "while")) goto not_a_match;
	if (!strcmp(funcname, "until")) goto not_a_match;
	if (!strcmp(funcname, "switch")) goto not_a_match;
	if (!strcmp(funcname, "rept")) goto not_a_match;
	if (!strcmp(funcname, "//")) goto not_a_match;
	if (!strcmp(funcname, "/*")) goto not_a_match;
	if (!strcmp(funcname, "catch")) goto not_a_match;

	// filter out lines that are commented out with a line-comment
	ptr = strstr(line_str, "//");
	if (ptr && ptr < fnstart) goto not_a_match;

	// checked earlier, but may not have been caught if there is a line-comment
	// on same line.
	if (strchr(funcname, ';')) goto not_a_match;

pragma_mark: ;
	// add the hit to the list of results
	result = (FLResult *)smal(sizeof(FLResult));

	result->text = new BStringItem(funcname_with_space);
	result->lineNumber = lineNumber;

	NewResults.AddItem((void *)result);

not_a_match: ;
	if (funcname_with_space) frees(funcname_with_space);
	delete bsLine;
	return;
}

// apply the new result set to the onscreen list
void CFunctionList::ApplyNewResults()
{
int i, count;
FLResult *result;

	count = NewResults.CountItems();

	list->MakeEmpty();
	LineNumbers.MakeEmpty();

	for(i=0;i<count;i++)
	{
		result = (FLResult *)NewResults.ItemAt(i);

		list->AddItem((BStringItem *)result->text);
		LineNumbers.AddItem((void *)(size_t)result->lineNumber);

		frees(result);
	}

	NewResults.MakeEmpty();
	UpdateSelectionHighlight();

	UpToDate = true;
}

/*
void c----------------------------() {}
*/

void CFunctionList::UpdateSelectionHighlight()
{
int i, y;
int lno;

	// take the Y at halfway down the screen.
	// find first function which is before that, if any.
	int lastItem = LineNumbers.CountItems() - 1;
	// no functions in list?
	if (lastItem < 0)
	{
		list->DeselectAll();
		return;
	}

	// get target scroll Y
	y = editor.curev->scroll.y + (editor.height / 2);

	if (y < 0) y = 0;
	if (y >= editor.curev->nlines) y = (editor.curev->nlines - 1);

	// start scanning
	for(i=lastItem;i>=0;i--)
	{
		lno = (int)(size_t)LineNumbers.ItemAt(i);

		if (lno <= y)
		{
			list->Select(i);
			return;
		}
	}

	list->DeselectAll();
}

// called by the edit_actions anytime the document is about to be modified.
// it resets the timer which causes update of the function list if the
// user doesn't type anything for a moment.
void CFunctionList::ResetTimer()
{
	Timer = 0;
	UpToDate = false;
}

// called every 100ms.
void CFunctionList::TimerTick()
{
	if (UpToDate) return;

	if (++Timer >= 10)	// 10 * 100 = 1 second
	{
		ScanAll();
	}
}


// ping-ponged back from the main window when it receives
// an invocation message from the scroll view.
void CFunctionList::JumpToIndex(int index)
{
int target_line;

	if (index < 0 || index >= LineNumbers.CountItems()) return;

	target_line = (int)(size_t)LineNumbers.ItemAt(index);
	target_line -= 2;
	if (target_line < 0) target_line = 0;

	editor.curev->SetXScroll(0);
	editor.curev->SetVerticalScroll(target_line);
	editor.curev->FullRedrawView();

	// hack in case UpdateSelectionHighlight doesn't agree the
	// function we just clicked is the current one.
	list->Select(index);

	MainView->MakeFocus();
}



