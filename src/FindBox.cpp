
#include "editor.h"
#include <cctype>	// "isalnum"
#include <Screen.h>
#include "FindBox.fdh"

#define BOX_WIDTH	505
#define BOX_HEIGHT	225
static BRect box_size(0, 0, BOX_WIDTH, BOX_HEIGHT);
static CFindBox *CurrentFindBox = NULL;
static BFilePanel *BrowsePanel = NULL;

#define M_FIND_NEXT				'FINa'
#define M_FIND_ALL				'FINb'
#define M_FIND_ALL_IN_ALL		'FINc'
#define M_FIND_IN_FILES			'FINd'

#define M_REPLACE				'FINe'
#define M_REPLACE_ALL			'FINf'
#define M_REPLACE_ALL_IN_ALL	'FINg'

#define M_CLOSE					'FINh'
#define M_BROWSE_FOR_FOLDER		'FINj'

#define NUM_TABS				3


CFindBox::CFindBox(int initialMode)
	: BWindow(box_size, "Search", B_TITLED_WINDOW,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_ASYNCHRONOUS_CONTROLS)
{
	// don't open more than one find box at a time
	if (CurrentFindBox)
	{
		CenterWindow(MainWindow, CurrentFindBox);
		CurrentFindBox->Show();
		CurrentFindBox->Activate();
		delete this;
		return;
	}
	
	CurrentFindBox = this;
	CenterWindow(MainWindow, this);
	
	// create tabs and views
	BRect r(Bounds());
	
	tabview = new FBTabView(r, "tab_view");
	tabview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	r = tabview->Bounds();
	r.InsetBy(5, 5);
	r.top += tabview->TabHeight();
	
	FBView::InitTabData();
	
	views[0] = new FBView(Bounds(), FINDBOX_FIND);
	views[1] = new FBView(Bounds(), FINDBOX_REPLACE);
	views[2] = new FBView(Bounds(), FINDBOX_FIND_FILES);
	
	tabview->AddTab(views[0], new BTab);
	tabview->AddTab(views[1], new BTab);
	tabview->AddTab(views[2], new BTab);
	
	AddChild(tabview);
	tabview->Select(initialMode);
	
	// if there is a selection, make that the default thing to search for, else,
	// use current word if any, else load the last word from the config
	if (editor.curev->selection.present)
	{
		int x1, y1, x2, y2;
		BString *linestr;
		clLine *line;
		
		// if selection is <= 1 line high, use whole selection, else, just
		// use word at start of selection.
		GetSelectionExtents(editor.curev, &x1, &y1, &x2, &y2);
		line = editor.curev->GetLineHandle(y1);
		linestr = line->GetLineAsString();
		
		if (y1 != y2)
			line->GetWordExtent(x1, &x1, &x2);
		
		DeleteAfter(linestr, x2);
		DeleteBefore(linestr, x1);
		
		GetCurrentView()->txtFindWhat->SetText(linestr->String());
		delete linestr;
	}
	else
	{	// try current word
		BString *str = editor.curev->curline->GetLineAsString();
		int x1, x2;
		editor.curev->GetCurrentWordExtent(&x1, &x2);
		DeleteAfter(str, x2);
		DeleteBefore(str, x1);
		
		// if word is no good (whitespace), load from prefs instead. because of the way
		// the word scanner works, if the first char is whitespace, it's all whitespace.
		char ch = str->String()[0];
		if (ch==TAB || ch==' ' || !ch)
			GetCurrentView()->txtFindWhat->SetText(settings->GetString("FBfindwhat", ""));
		else
			GetCurrentView()->txtFindWhat->SetText(str->String());
		
		delete str;
	}
	
	views[FINDBOX_REPLACE]->txtReplaceWith->SetText(settings->GetString("FBreplacewith", ""));
	
	// load checkbox settings
	GetCurrentView()->chkWholeWord->SetValue(settings->GetInt("FBWholeWordOnly", 0));
	GetCurrentView()->chkCaseSensitive->SetValue(settings->GetInt("FBCaseSensitive", 0));
	GetCurrentView()->chkBackwards->SetValue(settings->GetInt("FBBackwards", 0));
	views[FINDBOX_FIND_FILES]->chkRecursive->SetValue(settings->GetInt("FBrecursive", 1));
	views[FINDBOX_FIND_FILES]->chkIgnoreHiddenFolders->SetValue(settings->GetInt("FBignoredot", 1));
	
	GetCurrentView()->txtFindWhat->MakeFocus();
	GetCurrentView()->txtFindWhat->TextView()->Select(0, 99999);
	GetCurrentView()->DefButton->MakeDefault(true);
	Show();
}

CFindBox::~CFindBox()
{
	settings->SetString("FBfindwhat", GetCurrentView()->txtFindWhat->Text());
	settings->SetString("FBreplacewith", views[FINDBOX_REPLACE]->txtReplaceWith->Text());
	
	settings->SetInt("FBWholeWordOnly", GetCurrentView()->chkWholeWord->Value());
	settings->SetInt("FBCaseSensitive", GetCurrentView()->chkCaseSensitive->Value());
	settings->SetInt("FBBackwards", GetCurrentView()->chkBackwards->Value());
	settings->SetInt("FBrecursive", views[FINDBOX_FIND_FILES]->chkRecursive->Value());
	settings->SetInt("FBignoredot", views[FINDBOX_FIND_FILES]->chkIgnoreHiddenFolders->Value());
	
	CurrentFindBox = NULL;
	if (BrowsePanel)
	{
		delete BrowsePanel;
		BrowsePanel = NULL;
	}
}

// simple override for the tabview, all it does is ensure that
// the Find What box is focused when the tabs are switched.
void FBTabView::MouseDown(BPoint where)
{
FBView *newview, *oldview;
int OldSelection, NewSelection;
int i;

	OldSelection = BTabView::Selection();
	
	// eliminates flicker
	for(i=0;i<NUM_TABS;i++)
	{
		if (i == OldSelection) continue;
		
		CurrentFindBox->views[i]->chkWholeWord->SetValue(0);
		CurrentFindBox->views[i]->chkCaseSensitive->SetValue(0);
		CurrentFindBox->views[i]->txtFindWhat->SetText("");
	}
	
	BTabView::MouseDown(where);
	NewSelection = BTabView::Selection();
	
	newview = CurrentFindBox->views[NewSelection];
	oldview = CurrentFindBox->views[OldSelection];
	
	if (NewSelection != OldSelection && CurrentFindBox && \
		(NewSelection >= 0 && NewSelection < NUM_TABS))
	{
		// transfer contents of "find what" box
		newview->txtFindWhat->SetText(oldview->txtFindWhat->Text());
		
		// set "find what" box to active focus and select contents
		newview->txtFindWhat->MakeFocus();
		newview->txtFindWhat->TextView()->Select(0, strlen(newview->txtFindWhat->Text()));
		
		// transfer values of checkboxes from old pane to new pane
		newview->chkWholeWord->SetValue(oldview->chkWholeWord->Value());
		newview->chkCaseSensitive->SetValue(oldview->chkCaseSensitive->Value());
		newview->chkBackwards->SetValue(oldview->chkBackwards->Value());
		
		// set default button
		newview->DefButton->MakeDefault(true);
	}
}

// handle key shortcuts
void CFindBox::DispatchMessage(BMessage *msg, BHandler *target)
{
	if (msg->what == B_KEY_DOWN)
	{
		const char *bytes;
		if (msg->FindString("bytes", &bytes) == B_OK)
		{
			switch(*bytes)
			{
				case 27:
					PostMessage(B_QUIT_REQUESTED);
					return;
				
				//case 'a': case 'A':
				case 1:	// ALT+A
					if (tabview->Selection()==FINDBOX_REPLACE)
					{
						PostMessage(M_REPLACE_ALL);
						return;
					}
				break;
				
				//case 'f': case 'F':
				case 6:	// ALT+F
					switch(tabview->Selection())
					{
						case FINDBOX_FIND:
							PostMessage(M_FIND_NEXT);
							return;
						case FINDBOX_FIND_FILES:
							PostMessage(M_FIND_IN_FILES);
							return;
					}
				break;
			}
		}
	}
	
	BWindow::DispatchMessage(msg, target);
}

FBView *CFindBox::GetCurrentView()
{
int index = tabview->Selection();

	if (index >= 0 && index < NUM_TABS)
		return views[index];
	
	// failsafe
	staterr("CFindBox error: focus tab out of range: %d!", index);
	return views[0];
}

CFindBox *GetCurrentFindBox()
{
	return CurrentFindBox;
}

/*
void c------------------------------() {}
*/

struct
{
	char *name;
	
	char *button_text[3];
	int button_msg[3];
} tabdata[NUM_TABS];

void FBView::InitTabData()
{
	tabdata[FINDBOX_FIND].name = "Find";
	tabdata[FINDBOX_FIND].button_text[0] = "Find";
	tabdata[FINDBOX_FIND].button_text[1] = "Find All";
	tabdata[FINDBOX_FIND].button_text[2] = "Find in All Opened";
	tabdata[FINDBOX_FIND].button_msg[0] = M_FIND_NEXT;
	tabdata[FINDBOX_FIND].button_msg[1] = M_FIND_ALL;
	tabdata[FINDBOX_FIND].button_msg[2] = M_FIND_ALL_IN_ALL;
	
	tabdata[FINDBOX_REPLACE].name = "Replace";
	tabdata[FINDBOX_REPLACE].button_text[0] = "Replace";
	tabdata[FINDBOX_REPLACE].button_text[1] = "Replace All";
	tabdata[FINDBOX_REPLACE].button_text[2] = "Replace All in All";
	tabdata[FINDBOX_REPLACE].button_msg[0] = M_REPLACE;
	tabdata[FINDBOX_REPLACE].button_msg[1] = M_REPLACE_ALL;
	tabdata[FINDBOX_REPLACE].button_msg[2] = M_REPLACE_ALL_IN_ALL;
	
	tabdata[FINDBOX_FIND_FILES].name = "Find in Files";
	tabdata[FINDBOX_FIND_FILES].button_text[0] = "Find them All";
	tabdata[FINDBOX_FIND_FILES].button_text[1] = NULL;
	tabdata[FINDBOX_FIND_FILES].button_text[2] = NULL;
	tabdata[FINDBOX_FIND_FILES].button_msg[0] = M_FIND_IN_FILES;

}

FBView::FBView(BRect frame, int fbmode)
		: BView(frame, tabdata[fbmode].name, B_FOLLOW_ALL, B_WILL_DRAW)
{
int checkbox_y = 53;

	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	// -- text boxes --
	
	// Find What
	txtFindWhat = new BTextControl(BRect(40, 13, 335, 13+21), "",
								"Find what:", "", NULL);
	
	txtFindWhat->SetDivider(95);
	AddChild(txtFindWhat);
	
	if (fbmode == FINDBOX_REPLACE)
	{
		txtReplaceWith = new BTextControl(BRect(40, 13+31, 335, 13+31+21), "",
								"Replace with:", "", NULL);
		
		txtReplaceWith->SetDivider(95);
		AddChild(txtReplaceWith);
		
		checkbox_y += 32;
	}
	else
	if (fbmode == FINDBOX_FIND_FILES)
	{
		txtFilter = new BTextControl(BRect(40, 13+31, 335, 13+31+21), "",
								"File filter:", "", NULL);
		txtFolder = new BTextControl(BRect(40, 13+31+31, 305, 13+31+31+21), "",
								"In folder:", "", NULL);
		
		txtFolder->SetDivider(95);
		txtFilter->SetDivider(95);
		AddChild(txtFilter);
		AddChild(txtFolder);
		
		AddChild(new BButton(BRect(310, 13+31+31, 335, 13+31+31+21), "", \
								"...", new BMessage(M_BROWSE_FOR_FOLDER)));
		
		checkbox_y += 32+32;
		
		// set default folder to that of current file
		char *defaultFolder = RemoveFileSpec(editor.curev->filename);
		txtFolder->SetText(defaultFolder);
		frees(defaultFolder);
		
		// set default filter to all files with same extension as current one
		char *ptr = strrchr(editor.curev->filename, '.');
		if (ptr)
		{
			BString filter("*.");
			filter.Append(ptr+1);
			txtFilter->SetText(filter.String());
		}
		else
		{
			txtFilter->SetText("*");
		}
	}
	
	// -- buttons --
	DefButton = new BButton(BRect(348, 13, 490, 13+21+((fbmode==FINDBOX_FIND_FILES)?20:0)), "",
		tabdata[fbmode].button_text[0], new BMessage(tabdata[fbmode].button_msg[0]));
	
	AddChild(DefButton);
	
	if (tabdata[fbmode].button_text[1])
	{
		AddChild(new BButton(BRect(348, 13+31, 490, 13+31+38), "",
			tabdata[fbmode].button_text[1], new BMessage(tabdata[fbmode].button_msg[1])));
		
		AddChild(new BButton(BRect(348, 13+31+38+5, 490, 13+31+41+5+41), "",
			tabdata[fbmode].button_text[2], new BMessage(tabdata[fbmode].button_msg[2])));
	}
	
	AddChild(new BButton(BRect(348, 13+71+21+31+15, 490, 13+71+21+31+41+10), "",
						"Close", new BMessage(M_CLOSE)));
	
	// -- check boxes --
	
	if (fbmode == FINDBOX_FIND_FILES)
	{
		const int kht = 22;
		
		chkRecursive = new BCheckBox(BRect(134, checkbox_y-15, 340, checkbox_y-15+(kht-1)), "",
								"Recursive", NULL);
		AddChild(chkRecursive);
		
		chkIgnoreHiddenFolders = new BCheckBox(BRect(134, kht+checkbox_y-15, 340, kht+checkbox_y-15+20), "",
								"Ignore \".\" hidden folders", NULL);
		AddChild(chkIgnoreHiddenFolders);
		
		checkbox_y += 29;
	}
	else
	{
		chkRecursive = NULL;
		chkIgnoreHiddenFolders = NULL;
	}
	
	chkWholeWord = new BCheckBox(BRect(134, checkbox_y, 340, checkbox_y+20), "",
							"Match whole word only", NULL);
	
	chkCaseSensitive = new BCheckBox(BRect(134, checkbox_y+23, 340, checkbox_y+23+20), "",
							"Match case", NULL);
	
	AddChild(chkWholeWord);
	AddChild(chkCaseSensitive);
	
	chkBackwards = new BCheckBox(BRect(134, checkbox_y+35+23, 340, checkbox_y+35+23+20), "",
				"Search backwards", NULL);
	
	if (fbmode != FINDBOX_FIND_FILES)
		AddChild(chkBackwards);
	
	this->fbmode = fbmode;
}

FBView::~FBView()
{
	// on the Find Files tab this checkbox is created for consistency but never
	// actually added to the window, so we have to delete it manually
	if (fbmode == FINDBOX_FIND_FILES);
		delete chkBackwards;
}

/*
void c------------------------------() {}
*/

char *CFindBox::GetSearchSettings(int *options)
{
const char *search_term;
FBView *view = GetCurrentView();

	search_term = view->txtFindWhat->Text();
	if (!search_term || *search_term == '\0')
	{
		LockLooper();
		view->txtFindWhat->MakeFocus();
		UnlockLooper();
		return NULL;
	}
	
	// setup options
	*options = 0;
	
	if (view->chkWholeWord->Value() == B_CONTROL_ON)
		*options |= FINDF_WHOLE_WORD;
	
	if (view->chkCaseSensitive->Value() == B_CONTROL_ON)
		*options |= FINDF_CASE_SENSITIVE;
	
	if (view->fbmode != FINDBOX_FIND_FILES)
	{
		if (view->chkBackwards->Value() == B_CONTROL_ON)
			*options |= FINDF_BACKWARDS;
	}
	
	if (view->chkRecursive)
	{
		if (view->chkRecursive->Value() == B_CONTROL_ON)
			*options |= FINDF_RECURSIVE;
	}
	
	if (view->chkIgnoreHiddenFolders)
	{
		if (view->chkIgnoreHiddenFolders->Value() == B_CONTROL_ON)
			*options |= FINDF_IGNORE_DOT;
	}
	
	return smal_strdup(search_term);
}

void CFindBox::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_FIND_NEXT:
		case M_FIND_ALL:
		case M_FIND_ALL_IN_ALL:
		case M_FIND_IN_FILES:
		case M_REPLACE:
		case M_REPLACE_ALL:
		case M_REPLACE_ALL_IN_ALL:
		{
			// ping pong the message off the main window thread.
			// the main window just calls HandleMessageFromMainThread right back.
			// this is an admitted hack so that the main thread does the search
			// instead of ours. But makes things simpler. If you don't do this,
			// it crashes sometimes, and I think it's because of threading issues.
			// But adding code to safely support multiple threads accessing the document
			// at once would be a needless complexity hike.
			msg->AddInt32("old_what", msg->what);
			msg->what = M_FINDBOX_PINGPONG;
			
			MainWindow->PostMessage(msg);
		}
		break;
		
		case M_BROWSE_FOR_FOLDER:
		{
			if (BrowsePanel) delete BrowsePanel;
			
			BrowsePanel = new BFilePanel(B_OPEN_PANEL, NULL, NULL, B_DIRECTORY_NODE);
			BrowsePanel->SetTarget(this);
			BrowsePanel->Show();
		}
		break;
		
		case B_REFS_RECEIVED:
			ProcessBrowseForFolderResults(msg);
		break;
		
		case M_CLOSE:
			PostMessage(B_QUIT_REQUESTED);
		break;
		
		default:
			BWindow::MessageReceived(msg);
	}
}

// "ping ponged" message handler (see comment above)
void CFindBox::HandleSearchRequest(BMessage *msg)
{
char *search_string;
char *replace_string;
int options;

	search_string = GetSearchSettings(&options);
	if (!search_string) return;
	
	// put the search-results pane into the popup pane and clear any prior results.
	// note that we hold off on opening the popup pane until any results are
	// actually found.
	if (msg->what == M_FIND_NEXT)
	{
		MainWindow->popup.pane->Close();
	}
	else
	{
		BString title;
		
		MainWindow->popup.searchresults->Clear();
		MainWindow->popup.searchresults->SetSearchTerm(search_string);
		
		MainWindow->popup.searchresults->GetCaptionForTitlebar(&title);
		
		MainWindow->popup.pane->SetContents(title.String(), \
											MainWindow->popup.searchresults);
	}
	
	switch(msg->what)
	{
		case M_FIND_NEXT:
		case M_FIND_ALL:
		case M_FIND_ALL_IN_ALL:
		case M_FIND_IN_FILES:
		{
			int count = 9;	// just anything nonzero
			bool quit = true;
			
			// save last search term for "F3"
			if (editor.curev->search.lastSearch) frees(editor.curev->search.lastSearch);
			editor.curev->search.lastSearch = smal_strdup(search_string);
			editor.curev->search.lastOptions = options;
			
			switch(msg->what)
			{
				case M_FIND_NEXT:
					if (DoFindNext(search_string, options) != FN_HIT_FOUND)
						quit = false;
				break;
				
				case M_FIND_ALL:
					count = DoFindAll(editor.curev, search_string, options, true);
				break;
				
				case M_FIND_ALL_IN_ALL:
					count = DoFindAllInAll(search_string, options);
				break;
				
				case M_FIND_IN_FILES:
				{
					const char *filter = GetCurrentView()->txtFilter->Text();
					const char *folder = GetCurrentView()->txtFolder->Text();
					bool recursive = (options & FINDF_RECURSIVE) ? true:false;
					
					// set filter to wildcard if it was empty
					if (!filter[0])
					{
						filter = "*";
						GetCurrentView()->txtFilter->SetText(filter);
					}
					
					count = DoFindInFiles(search_string, folder, filter, options, recursive);
				}
				break;
			}
			
			if (!count)
			{
				(new BAlert("", "No matches found.", "OK"))->Go();
				quit = false;
			}
			
			if (quit)
				PostMessage(B_QUIT_REQUESTED);
			else
			{
				LockLooper();
				GetCurrentView()->txtFindWhat->TextView()->Select(0, 99999);
				UnlockLooper();
			}
			
			frees(search_string);
		}
		break;
		
		// ---------------------------------------
		
		case M_REPLACE:
		case M_REPLACE_ALL:
		case M_REPLACE_ALL_IN_ALL:
		{
			int count = 0;
			bool canceled = false;
			
			replace_string = smal_strdup(GetCurrentView()->txtReplaceWith->Text());
			
			if (editor.curev->search.lastSearch) frees(editor.curev->search.lastSearch);
			editor.curev->search.lastSearch = smal_strdup(search_string);
			editor.curev->search.lastOptions = options;
			
			switch(msg->what)
			{
				case M_REPLACE:
				{
					count = DoInteractiveReplace(editor.curev, search_string, \
										replace_string, options, &canceled);
				}
				break;
				
				case M_REPLACE_ALL:
					count = DoReplaceAll(editor.curev, search_string, \
									replace_string, options);
				break;
				
				case M_REPLACE_ALL_IN_ALL:
					count = DoReplaceAllInAll(search_string, replace_string, \
											options);
				break;
			}
			
			if (!count)
			{
				if (canceled)
					Show();
				else
					(new BAlert("", "No matches found.", "OK"))->Go();
			}
			else
			{
				PostMessage(B_QUIT_REQUESTED);
				
				if (msg->what == M_REPLACE_ALL ||
					msg->what == M_REPLACE_ALL_IN_ALL)
				{
					// update function list immediately, so if they replaced function
					// names the changes are visible while the alert is up
					//FunctionList->ScanIfNeeded();					
					
					char msg[800];
					sprintf(msg, "%d instance%s were replaced.", count, (count>1)?"s":"");
					
					(new BAlert("", msg, "OK"))->Go();
				}
			}
			
			frees(search_string);
			frees(replace_string);
		}
		break;
		
		// ---------------------------------------
	}
}

// handles user's selection from the "Browse for Folder" panel
// on Find in Files tab
void CFindBox::ProcessBrowseForFolderResults(BMessage *msg)
{
entry_ref ref;

	if (msg->FindRef("refs", &ref) == B_OK)
	{
		BEntry entry(&ref, true);
		BPath path;
		
		entry.GetPath(&path);
		
		BTextControl *txtFolder = GetCurrentView()->txtFolder;
		if (txtFolder)
			txtFolder->SetText(path.Path());
	}
}

/*
void c------------------------------() {}
*/

// runs a find/replace operation for "search_string" replacing with
// "replace_string", and offers a confirmation for each item.
int DoInteractiveReplace(EditView *ev, const char *search_string, \
						const char *replace_string, int options, bool *canceled)
{
DocPoint startpoint, seekpoint;
DocPoint hit_start;
bool wrapped = false;
bool firsthit = true;
int count = 0;
bool reverse = (options & FINDF_BACKWARDS);

	if (canceled) *canceled = false;
	
	BeginUndoGroup(ev, UG_NO_AFFECT_CURSOR);
	selection_drop(ev);
	
	startpoint.SetToCursor(ev);
	seekpoint = startpoint;
	
	if (ev != editor.curev)
		TabBar->SetActiveTab(ev);
	
	rept
	{
		if (!reverse)
			hit_start = find_next_hit(ev, seekpoint, search_string, options);
		else
			hit_start = find_prev_hit(ev, seekpoint, search_string, options);
		
		// wrap around when we get to end
		if (!hit_start.IsValid())
		{
			if (!wrapped)
			{
				if (!reverse)
					seekpoint.SetToStart(ev);
				else
					seekpoint.SetToEnd(ev);
				
				wrapped = true;
				continue;
			}
			else
			{
				break;
			}
		}
		
		// stop once we've been around the whole document, even
		// if more hits were found (they were already marked NO).
		if (wrapped)
		{
			if (!reverse && hit_start >= startpoint)
				break;
			if (reverse && hit_start <= startpoint)
				break;
		}
		
		seekpoint = hit_start;
		// prevents finding string again if replace string begins with search string
		if (reverse)
			seekpoint.DecrementBy(2);
		
		// now that we know we have at least 1 hit, hide the Find/Replace box
		if (firsthit)
		{
			CFindBox *fb = GetCurrentFindBox();
			if (fb) fb->Hide();
			firsthit = false;
		}
		
		// select the hit and offer to replace it
		SelectHit(search_string, hit_start, options|FINDF_FORCE_Y|FINDF_Y_AT_BOTTOM);
		
		BAlert *prompt = new BAlert("", "Replace this occurrance?", "Cancel", "No", "Yes");
		SetPromptPos(prompt, ev, hit_start.y);
		
		switch(prompt->Go())
		{
			case 0:	// Cancel
				if (canceled) *canceled = true;
				goto done;
			
			case 1: // No
				seekpoint.IncrementBy(strlen(search_string));
			break;
			
			case 2: // Yes
				selection_drop(ev);
				
				editor.curev->action_delete_right(hit_start.x, hit_start.y, strlen(search_string));
				editor.curev->action_insert_string(hit_start.x, hit_start.y, replace_string, NULL, NULL);
				
				editor.curev->RedrawView();
				seekpoint.IncrementBy(strlen(replace_string));
			break;
		}
		
		count++;
	}

done: ;
	EndUndoGroup(ev);
	
	// hack--noticed cursor wasn't always immediately visible after replace box
	// went away; don't want to mess with it at the moment but this fixes it for now.
	MainView->cursor.EnableFlashing(true);
	MainView->cursor.bump();
	editor.curev->FullRedrawView();
	
	return count;
}

// moves the "replace this?" prompt to try to keep it out of the
// way of the selected occurrance, which is on line "y" of document "ev".
static void SetPromptPos(BAlert *prompt, EditView *ev, int y)
{
	// get which screen column that line is at
	y -= ev->scroll.y;
	// convert to pixels
	y *= editor.font_height;
	// get the approximate actual screen position by adding in the
	// Y coord of the window.
	y += (int)MainWindow->Frame().top;
	
	// get the current and flipped (at bottom) possible Y positions
	// of the dialog prompt
	int top_y = (int)(MainWindow->Frame().top + \
		((MainWindow->Frame().bottom - MainWindow->Frame().top) / 4));
	
	int bottom_y = (int)(MainWindow->Frame().bottom - \
		((MainWindow->Frame().bottom - MainWindow->Frame().top) / 3));
	
	// pick whichever puts the dialog furthest from the selected line
	int dist_top = abs(y - top_y);
	int dist_bottom = abs(y - bottom_y);
	
	y = (dist_top > dist_bottom) ? top_y : bottom_y;
	
	// center prompt horizontally within window
	int x = (int)(MainWindow->Frame().left + (WIDTHOF(MainWindow->Frame()) / 2));
		x -= (int)WIDTHOF(prompt->Frame()) / 2;
	
	prompt->MoveTo(x, y);
}


// replaces all instances of "search_string" with "replace_string",
// following the rules in "options", and does not ask for confirmation.
// the number of matches found in returned.
int DoReplaceAll(EditView *ev, const char *search_string, \
				const char *replace_string, int options)
{
DocPoint hit_start, seekpoint;
int ss_length = strlen(search_string);
int replace_length = strlen(replace_string);
int count = 0;

	BeginUndoGroup(ev, UG_NO_AFFECT_CURSOR);
	selection_drop(ev);
	
	// for "Replace All" we can ignore forwards/backwards flag, because
	// we are replacing them all anyway.
	seekpoint.Set(ev, 0, 0);
	
	rept
	{
		hit_start = find_next_hit(ev, seekpoint, search_string, options);
		if (!hit_start.IsValid()) break;
		
		ev->action_delete_right(hit_start.x, hit_start.y, ss_length);
		ev->action_insert_string(hit_start.x, hit_start.y, replace_string, NULL, NULL);
		
		// must do this after replace so line wrap works
		seekpoint = hit_start;
		seekpoint.IncrementBy(replace_length);
		
		count++;
	}
	
	EndUndoGroup(ev);
	
	if (ev == editor.curev)
	{
		UpdateCursorPos(ev);
		ev->RedrawView();
	}
	
	return count;
}

// runs DoReplaceAll for every open document
int DoReplaceAllInAll(const char *search_string, const char *replace_string, \
					int options)
{
EditView *ev;
int count = 0;
int i;

	for(i=0;;i++)
	{
		ev = (EditView *)editor.DocList->ItemAt(i);
		if (!ev) break;
		
		count += DoReplaceAll(ev, search_string, replace_string, options);
	}
	
	return count;
}

/*
void c------------------------------() {}
*/

// finds and selects the next hit in the document.
// can also find previous hit, if FINDF_BACKWARDS option is passed.
// returns one of the following result codes:
//	FN_HIT_FOUND: a hit was found and selected.
//	FN_NO_HITS:	  no matches were found in the current document.
//	FN_NO_ADDITIONAL_HITS:	a single hit was found, but it is already selected.
int DoFindNext(const char *search_string, int options)
{
DocPoint start, hit_start;
bool wrapped = false;
bool reverse = (options & FINDF_BACKWARDS);

	// find the next hit
	start.SetToCursor(editor.curev);

wrap: ;
	if (!reverse)
	{
		start.Increment();
		hit_start = find_next_hit(editor.curev, start, search_string, options);
	}
	else
	{
		start.Decrement();
		hit_start = find_prev_hit(editor.curev, start, search_string, options);
	}
	
	if (!hit_start.IsValid())	// no more hits? wrap around to start and try again
	{
		if (wrapped)
		{	// already wrapped, there are just plain no matches
			(new BAlert("", "No matches found.", "OK"))->Go();
			return FN_NO_HITS;
		}
		else
		{
			// wrap around to beginning (or end) of document
			if (!reverse)
				start.SetToStart(editor.curev);
			else
				start.SetToEnd(editor.curev);
			
			wrapped = true;
			goto wrap;
		}
	}
	
	if (SelectHit(search_string, hit_start, options) == SH_ALREADY_SELECTED)
	{
		(new BAlert("", "No additional matches found.", "OK"))->Go();
		return FN_NO_ADDITIONAL_HITS;
	}
	
	return FN_HIT_FOUND;
}

// selects a given hit "search_string" beginning at "hit_start"
// and makes it visible onscreen.
// if the hit is already selected, returns SH_ALREADY_SELECTED.
int SelectHit(const char *search_string, DocPoint hit_start, int options)
{
DocPoint hit_end;

	// select the hit
	hit_end = hit_start;
	hit_end.IncrementBy(strlen(search_string) - 1);
	
	// detect if hit is already selected.
	if (editor.curev->selection.present)
	{
		int x1, y1, x2, y2;
		GetSelectionExtents(editor.curev, &x1, &y1, &x2, &y2);
		
		if (x1==hit_start.x && \
			y1==hit_start.y && \
			x2==hit_end.x && \
			y2==hit_end.y)
		{
			return SH_ALREADY_SELECTED;
		}
		
		// else, drop selection in preparation for a new one
		selection_drop(editor.curev);
	}
	
	editor.curev->cursor.move(hit_start.x, hit_start.y);
	selection_select_range(editor.curev, &hit_start, &hit_end);
	
	// this option affects whether we want to scroll the screen,
	// if the line is already visible. in case of find box we do,
	// in case of find next we don't.
	int seekmode = BV_SPECIFIC_Y;
	if (options & FINDF_FORCE_Y) seekmode = BV_FORCE_SPECIFIC_Y;
	
	int target_y = 8;
	if (options & FINDF_Y_AT_BOTTOM) target_y = (editor.height - 8);
	
	editor.curev->BringLineIntoView(hit_start.y, seekmode, target_y);
	editor.curev->RedrawView();
	
	return SH_OK;
}

/*
void c------------------------------() {}
*/

// runs DoFindAll on every open document
int DoFindAllInAll(const char *search_string, int options)
{
EditView *ev;
bool clear_window_first = true;
int count = 0;
int i;

	for(i=0;;i++)
	{
		ev = (EditView *)editor.DocList->ItemAt(i);
		if (!ev) break;
		
		count += DoFindAll(ev, search_string, options, clear_window_first);
		clear_window_first = false;
	}
	
	return count;
}

// opens the search results pane, finds all matches in the document,
// and adds them to the search results list.
int DoFindAll(EditView *ev, const char *search_string, int options, \
				bool clear_window_first)
{
DocPoint hit_start, seekpoint;
int ss_length = strlen(search_string);
int count = 0;

	seekpoint.Set(ev, 0, 0);
	
	rept
	{
		hit_start = find_next_hit(ev, seekpoint, search_string, options);
		if (!hit_start.IsValid()) break;
		
		BString *linestr = hit_start.line->GetLineAsString();
		
		AddSearchResult(ev->filename, hit_start.x, hit_start.y, \
						linestr->String(), search_string);
		
		delete linestr;
		count++;
		
		// advance cursor past hit and get ready to search again
		seekpoint = hit_start;
		seekpoint.IncrementBy(ss_length);
	}
	
	return count;
}

// adds a hit to the search results window.
//	filename: the filename of the file the hit was found in
//	x/y:	  the column and line number the hit was found on (line number is zero-based)
//	line:	  the contents of the line the hit was found in
// search_string: the search term that was being searched for
void AddSearchResult(const char *filename, int x, int y, const char *linestr, \
					const char *search_string)
{
stSearchResult *result;

	// pop open search results if not already visible
	if (!MainWindow->popup.pane->IsOpen())
		MainWindow->popup.pane->Open();
	
	// generate the result structure
	result = (stSearchResult *)smal(sizeof(*result));
	
	result->x = x;
	result->lineNumber = y;
	result->filename = smal_strdup(filename);
	
	// create the line string:
	//	* remove leading whitespace
	//	* change tabs to spaces
	char *sptr = (char *)linestr;
	
	while(*sptr==' ' || *sptr==9) sptr++;
	sptr = smal_strdup(sptr);
	
	char *sptr2 = sptr;
	while(*sptr2)
	{
		if (*sptr2==9) *sptr2 = ' ';
		sptr2++;
	}
	
	result->lineString = sptr;
	MainWindow->popup.searchresults->AddResult(result, true);
}

/*
void c------------------------------() {}
*/

// runs a Find in Files operation and returns the number of hits found.
// if any hits are found, the search results pane is opened, and the results
// are displayed in it.
int DoFindInFiles(const char *search_string, const char *folder, \
					const char *filter, int options, bool recursive)
{
BList files, dirs;
char *fname;
int i;
int count = 0;
bool ignore_dot = (options & FINDF_IGNORE_DOT);

	//stat("DoFindInFiles: '%s'; '%s' in '%s', recursive=%d", search_string, filter, folder, recursive);
	
	// get a list of all files and directories in "folder".
	// for each file, run a SearchInFile operation
	// for each directory, call ourselves recursively, if "recursive" is true
	if (recursive)
		GetDirectoryContents(folder, filter, &files, &dirs);
	else
		GetDirectoryContents(folder, filter, &files, NULL);
	
	// search all files in the directory
	i = 0;
	while((fname = (char *)files.ItemAt(i++)))
	{
		count += SearchInFile(fname, search_string, options);
		frees(fname);
	}
	
	// search all directories recursively
	if (recursive)
	{
		char *dir;
		i = 0;
		
		while((dir = (char *)dirs.ItemAt(i++)))
		{
			// TODO: this depends on the paths returned by GetDirectoryContents
			// not having the terminating '/', which is currently the case, but seems
			// a risky assumption.
			if (GetFileSpec(dir)[0] != '.' || !ignore_dot)
			{
				count += DoFindInFiles(search_string, dir, filter, options, true);
			}
			
			frees(dir);
		}
	}
	
	return count;
}

// searches for "search_string" in "filename" using "options",
// and returns the number of matches found.
int SearchInFile(const char *filename, const char *search_string, int options)
{
char *(*StringScanner)(const char *haystack, const char *needle, int needle_length);
int needle_length = strlen(search_string);
FILE *fp;
char line[8000];
char *seek, *hit;
int lineNo = 0, x;
int count = 0;

	fp = fopen(filename, "rb");
	if (!fp) return 0;
	
	StringScanner = GetStringScanner(options);
	
	while(!feof(fp))
	{
		fgetline(fp, line, sizeof(line));
		
		// scan the line for hits
		seek = line;
		rept
		{
			hit = (*StringScanner)(seek, search_string, needle_length);
			if (!hit) break;
			
			x = (hit - line);
			AddSearchResult(filename, x, lineNo, line, search_string);
			
			count++;
			seek = (hit + needle_length);
		}
		
		lineNo++;
	}
	
	fclose(fp);
	return count;
}

/*
void c------------------------------() {}
*/

// find the next instance of "search_string", beginning in document
// "ev" at position "start".
// if no hits are found, returns a DocPoint whose IsValid()
// function returns false.
DocPoint find_next_hit(EditView *ev, DocPoint start, \
				const char *search_string, uint options)
{
clLine *line;
BString *bstr;
const char *str, *hit;
char *(*StringScanner)(const char *haystack, const char *needle, int needle_length);
int needle_length;
int y;

	//stat("searching for '%s' beginning at [%d,%d]...", search_string, start.x, start.y);
	
	// select which string-searching function to call based on options
	StringScanner = GetStringScanner(options);
	
	// fetch contents of the first line
	line = start.line;
	y = start.y;
	
	bstr = line->GetLineAsString();
	DeleteBefore(bstr, start.x);
	
	needle_length = strlen(search_string);
	
	rept
	{
		str = bstr->String();
		//stat("line %d: '%s'", y, str);
		hit = (*StringScanner)(str, search_string, needle_length);
		
		if (hit)
		{
			int x = (hit - str);
			if (y == start.y) x += start.x;
			return DocPoint(ev, x, y, line);
		}
		
		line = line->next;
		if (!line) break;
		
		y++;
		
		delete bstr;
		bstr = line->GetLineAsString();
	}
	
	// return an invalid DocPoint to signal no hits found
	return DocPoint();
}

// find the previous instance of "search_string", beginning in document
// "ev" at position "start".
// if no hits are found, returns a DocPoint whose IsValid() function
// returns false.
DocPoint find_prev_hit(EditView *ev, DocPoint start, \
				const char *search_string, uint options)
{
clLine *line;
BString *bstr;
const char *str, *hit;
char *(*StringScanner)(const char *haystack, const char *needle, int needle_length);
int needle_length;
int y;

	// select which string-searching function to call based on options
	StringScanner = GetStringScanner(options);
	
	// fetch contents of the first line
	line = start.line;
	y = start.y;
	
	bstr = line->GetLineAsString();
	DeleteAfter(bstr, start.x);
	
	needle_length = strlen(search_string);
	
	rept
	{
		str = bstr->String();
		
		// are there any hits on this line?
		hit = (*StringScanner)(str, search_string, needle_length);
		if (hit)
		{
			// yes! find the LAST hit on the line, since we are searching backwards
			const char *lasthit = hit;
			rept
			{
				lasthit += needle_length;
				
				lasthit = (*StringScanner)(lasthit, search_string, needle_length);
				if (lasthit)
					hit = lasthit;
				else
					break;
			}
			
			return DocPoint(ev, (hit - str), y, line);
		}
		
		line = line->prev;
		if (!line) break;
		
		y--;
		
		delete bstr;
		bstr = line->GetLineAsString();
	}
	
	// return an invalid DocPoint to signal no hits found
	return DocPoint();
}


// given a options set, returns the function to use for searching
char *(*GetStringScanner(int options))(const char *, const char *, int)
{
	if (options & FINDF_WHOLE_WORD)
	{
		if (options & FINDF_CASE_SENSITIVE)
		{
			return wholeword_strstr;
		}
		else
		{
			return wholeword_strcasestr;
		}
	}
	else
	{
		if (options & FINDF_CASE_SENSITIVE)
		{
			return normal_strstr;
		}
		else
		{
			return normal_strcasestr;
		}
	}
}

char *normal_strstr(const char *haystack, const char *needle, int needle_length)
{
	return strstr(haystack, needle);
}

char *normal_strcasestr(const char *haystack, const char *needle, int needle_length)
{
	return strcasestr(haystack, needle);
}

// works like strstr, but only finds whole words
char *wholeword_strstr(const char *haystack, const char *needle, int needle_length)
{
	return wholeword_common(haystack, needle, needle_length, strstr);
}

// works like strcasestr, but only finds whole words
char *wholeword_strcasestr(const char *haystack, const char *needle, int needle_length)
{
	return wholeword_common(haystack, needle, needle_length, strcasestr);
}

char *wholeword_common(const char *haystack, \
						const char *needle, \
						int needle_length, \
						char *(*StringScanner)(const char *haystack, \
												const char *needle))
{
const char *initial_haystack;
char *hit;
char ch;

	initial_haystack = haystack;
	
	rept
	{
		hit = (*StringScanner)(haystack, needle);
		if (!hit) return NULL;
		
		// check that char before hit is not alphanumeric
		if (hit > initial_haystack)
		{
			ch = *(hit - 1);
			if (isalnum(ch) || ch=='_')
			{
				haystack = (hit + needle_length);
				continue;
			}
		}
		
		// check that char after hit is not alphanumeric
		ch = *(hit + needle_length);
		if (isalnum(ch) || ch=='_')
		{
			haystack = (hit + needle_length);
			continue;
		}
		
		return hit;
	}
}

