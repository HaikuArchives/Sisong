
/*
	constructs the main editing area, including line number view,
	scrollbars, command preview window, etc.
*/

#include "editor.h"
#include "EditArea.fdh"


CEditArea::CEditArea(BRect frame, uint32 resizingMode)
	: BView(frame, "edit_area", resizingMode, 0)
{
BRect bo(Bounds());
BRect mainbo;
BRect rc;

	/* initilize */
	SetViewColor(B_TRANSPARENT_COLOR);
	
	mainbo = bo;
	mainbo.bottom -= SBAR_SIZE;
	mainbo.right -= (SBAR_SIZE + FL_WIDTH);
	
	/* create controls */
	int spacer_width = editor.settings.ShowSpacer ? SPACER_WIDTH : 1;
	int ln_width = editor.settings.ShowLineNumbers ? LN_WIDTH : 0;
	
	int area_lrmode = B_FOLLOW_LEFT;
	int fl_lrmode = B_FOLLOW_RIGHT;
	
	if (editor.settings.FunctionListOnLeft)
	{
		mainbo.OffsetBy(FL_WIDTH, 0);
		SWAP(area_lrmode, fl_lrmode);
	}
	

	// Line Numbers view
	rc = mainbo;
	rc.right = rc.left + (ln_width - 1);
	ln = new LNPanel(rc, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM);
	AddChild(ln);
	
	// Spacer
	rc = mainbo;
	rc.left = mainbo.left + ln_width;
	rc.right = rc.left + (spacer_width - 1);
	spacer = new CSpacerView(rc, B_FOLLOW_LEFT | B_FOLLOW_TOP_BOTTOM);
	AddChild(spacer);
	
	// Command Preview panel
	rc.left = mainbo.left;
	rc.top = rc.bottom + 1;
	rc.bottom = bo.bottom;
	rc.right = rc.left+ln_width-1;
	cmd_preview = new BStringView(rc, "cmdp", "", B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	cmd_preview->SetAlignment(B_ALIGN_RIGHT);
	AddChild(cmd_preview);
	
	// Horizontal Scrollbar
	rc.left = rc.right + 1;
	rc.right = mainbo.right;
	HScrollbar = new DocScrollBar(rc, B_FOLLOW_LEFT | B_FOLLOW_BOTTOM, B_HORIZONTAL);
	AddChild(HScrollbar);
	
	// Vertical Scrollbar
	rc.left = rc.right + 1;
	rc.right = mainbo.right + SBAR_SIZE;
	rc.bottom = rc.top - 1;
	rc.top = 0;
	VScrollbar = new DocScrollBar(rc, B_FOLLOW_RIGHT | B_FOLLOW_TOP_BOTTOM, B_VERTICAL);
	AddChild(VScrollbar);
	
	// Main Edit Area
	rc.right = rc.left - 1;
	rc.left = mainbo.left + (spacer_width + ln_width);
	editpane = new CEditPane(rc, B_FOLLOW_ALL);
	AddChild(editpane);
	
	// Corner square between H and V scrollbars
	rc.top = rc.bottom + 1;
	rc.left = rc.right + 1;
	rc.right = mainbo.right + SBAR_SIZE;
	rc.bottom = bo.bottom;
	ScrollbarCorner = new BView(rc, "corner", B_FOLLOW_BOTTOM | B_FOLLOW_RIGHT, 0);
	AddChild(ScrollbarCorner);
	
	// function list
	if (editor.settings.FunctionListOnLeft)
	{
		rc = Bounds();
		rc.right = FL_WIDTH - 1;
	}
	else
	{
		rc = bo;
		rc.left = mainbo.right + SBAR_SIZE + 1;
	}
	
	functionlist = new CFunctionList(rc, fl_lrmode | B_FOLLOW_TOP_BOTTOM);
	AddChild(functionlist);

	editpane->MakeFocus();
}

CEditArea::~CEditArea()
{
	
}

void CEditArea::SetFontSize(int newsize)
{
	LockWindow();	
	
	editpane->SetFontSize(newsize);
	ln->SetFontSize(newsize);
	
	if (MainWindow->popup.searchresults)
		MainWindow->popup.searchresults->ChangeFontSize(newsize);
	
	if (MainWindow->popup.compile)
		MainWindow->popup.compile->ChangeFontSize(newsize);
	
	if (editor.curev)
		editor.curev->FullRedrawView();
	
	editor.settings.font_size = newsize;
	UnlockWindow();
}

/*
void c------------------------------() {}
*/
int ignore_scrollbars = 0;

void DocScrollBar::ValueChanged(float fNewValue)
{
int newValue = (int)fNewValue;

	if (ignore_scrollbars || !editor.curev)
		return;
	
	if (Orientation() == B_HORIZONTAL)
	{
		int cur_x_scroll = editor.curev->xscroll;
		float scroll_min, scroll_max;
		GetRange(&scroll_min, &scroll_max);
		if (cur_x_scroll > scroll_max) cur_x_scroll = (int)scroll_max;
		
		if (cur_x_scroll != newValue)
		{
			//stat("HScrollbar change: newValue %d, current scroll %d", newValue, editor.curev->xscroll);
			editor.curev->SetXScroll(newValue);
			editor.curev->RedrawView();
		}
	}
	else
	{
		if (newValue != editor.curev->scroll.y)
		{
			//stat("VScrollbar change: newvalue=%d, editor=%d", newValue, editor.curev->scroll.y);
			editor.curev->SetVerticalScroll(newValue);
			editor.curev->RedrawView();
		}
	}
}



