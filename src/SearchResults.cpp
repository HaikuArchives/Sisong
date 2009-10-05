
#include "editor.h"
#include "SearchResults.fdh"

#define BOX_WIDTH	870
#define BOX_HEIGHT	280
static BRect box_size(0, 0, BOX_WIDTH, BOX_HEIGHT);


SearchResultsPane::SearchResultsPane()
{
	this->search_term = smal_strdup("");
	
	// create list
	BRect lvrect(Bounds());
	lvrect.InsetBy(2, 2);
	lvrect.right -= B_V_SCROLL_BAR_WIDTH;

	OnscreenList = new BListView(lvrect, "search_list", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);

	BFont font(be_fixed_font);
	font.SetSize(editor.settings.font_size);
	OnscreenList->SetFont(&font);
	
	OnscreenList->SetSelectionMessage(new BMessage(M_SEARCHRESULTS_INVOKE));
	
	// create scrollview
	ScrollView = new BScrollView("ScrollView", OnscreenList, \
							B_FOLLOW_ALL, 0, false, true);
	AddChild(ScrollView);
}

SearchResultsPane::~SearchResultsPane()
{
	// free memory used by listentries
	Clear();
	
	if (search_term)
		frees(search_term);
}

void SearchResultsPane::ChangeFontSize(int new_point_size)
{
	BFont font(be_fixed_font);
	font.SetSize(new_point_size);
	OnscreenList->SetFont(&font);
	OnscreenList->Invalidate();
}

void SearchResultsPane::PopupOpening()
{
	OnscreenList->SetViewColor(GetEditBGColor(COLOR_TEXT));
}

// call this to inform the search results pane what the search term is that
// is being searched for. this is used to calculate the length of the selection
// when clicking on a result, and also affects the text shown in the titlebar.
void SearchResultsPane::SetSearchTerm(const char *newTerm)
{
	if (search_term) frees(search_term);
	search_term = smal_strdup(newTerm);
}

// fills a BString containing the text to display on the popup-pane's titlebar.
void SearchResultsPane::GetCaptionForTitlebar(BString *title)
{
	if (search_term && search_term[0])
	{
		*title = "Search Results: '";
		title->Append(search_term);
		title->Append("'");
	}
	else
	{
		*title = "Search Results";
	}
}

/*
void c------------------------------() {}
*/

void SearchResultsPane::ItemClicked(int index)
{
stSearchResult *result;

	result = (stSearchResult *)ResultsList.ItemAt(index);
	if (!result) return;
	
	DoFileOpenAtLine(result->filename, result->lineNumber, \
					result->x, result->x + strlen(search_term) - 1);
}


void SearchResultsPane::AddResult(stSearchResult *result,
									bool include_filename = true)
{
BString str;
char line_no[800];
ColoredStringItem *item;

	sprintf(line_no, "%d", result->lineNumber+1);
	
	if (include_filename && result->filename)
	{
		#define MAX_FN_LENGTH		20
		// trim pathname from filename
		char *filename = strrchr(result->filename, '/');
		filename ? filename++ : (filename = result->filename);
		// limit filename length to a reasonable value
		int fn_x = strlen(filename) - MAX_FN_LENGTH;
		if (fn_x <= 0) fn_x = 0; else str.Append("...");
		str.Append(&filename[fn_x]);
		
		str.Append("(");
		str.Append(line_no);
		str.Append("): ");
	}
	else
	{
		str.Append(line_no);
		str.Append(": ");
	}
	
	str.Append(result->lineString);
	
	bool locked = LockLooper();
	
	item = new ColoredStringItem(str.String(), GetEditFGColor(COLOR_TEXT), \
							GetEditBGColor(COLOR_TEXT), GetEditBGColor(COLOR_SELECTION));
	OnscreenList->AddItem(item);
	
	ResultsList.AddItem((void *)result);
	
	if (locked)
		UnlockLooper();
}

void SearchResultsPane::Clear()
{
int i;
BListItem *item;
stSearchResult *result;
	
	i = 0;
	while((item = (BListItem *)OnscreenList->ItemAt(i++)))
		delete item;

	i = 0;
	while((result = (stSearchResult *)ResultsList.ItemAt(i++)))
	{
		if (result->filename) frees(result->filename);
		if (result->lineString) frees(result->lineString);
		frees(result);
	}
	
	OnscreenList->MakeEmpty();
	ResultsList.MakeEmpty();
}

