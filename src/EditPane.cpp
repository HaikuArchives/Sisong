
/*
	the main edit pane, consisting of only the actual editing region itself,
	not the scrollbars, line number view, etc.
*/

#include "editor.h"
#include "EditPane.fdh"


CEditPane::CEditPane(BRect frame, uint32 resizingMode)
			: BView(frame, "EDIT_PANE!", resizingMode,
			  B_WILL_DRAW | B_FRAME_EVENTS | B_FULL_UPDATE_ON_RESIZE),
			  cursor(this)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	mouse_down = false;
	
	if (!FontDrawer)
		FontDrawer = new CFontDrawer(editor.settings.font_size);
	
	SetFontSize(editor.settings.font_size);
	SetFont(FontDrawer->font);
}

CEditPane::~CEditPane()
{
	delete FontDrawer;
	FontDrawer = NULL;
}

void CEditPane::SetFontSize(int new_size)
{
	FontDrawer->SetSize(new_size);
	SetFont(FontDrawer->font);

	editor.font_width = FontDrawer->fontwidth;
	editor.font_height = FontDrawer->fontheight;
	
	// need to recalculate # of visible lines, etc
	FrameResized(0, 0);
}

/*
void c------------------------------() {}
*/

void CEditPane::Draw(BRect updateRect)
{
	if (editor.curev)
		editor.curev->FullRedrawView();
}

void CEditPane::FrameResized(float untrustworthy_width, \
							 float untrustworthy_height)
{	
	// set new pixel size.
	// don't trust the width x height arguments we were passed,
	// they sometimes lie!
	editor.pxwidth = (int)WIDTHOF(Bounds());
	editor.pxheight = (int)HEIGHTOF(Bounds());
	if (editor.pxwidth < 1) editor.pxwidth = 1;
	if (editor.pxheight < 1) editor.pxheight = 1;

	// calc size in chars
	editor.width = (editor.pxwidth / editor.font_width);
	editor.height = (editor.pxheight / editor.font_height);
	if (editor.pxwidth % editor.font_width) editor.width++;
	
	// set whether or not the line at the bottom is partly cut-off
	editor.PartialLineAtBottom = (editor.pxheight % editor.font_height) != 0;
	if (editor.PartialLineAtBottom) editor.height++;
	
	// set up scroll bars
	if (scrHorizontal && scrVertical)
	{
		scrHorizontal->SetSteps(editor.font_width, editor.font_width*10);
		scrVertical->SetSteps(1, editor.height - 2);
	}
	
	if (editor.curev)
	{
		editor.curev->UpdateVertScrollBarRange(true);
		editor.curev->UpdateHozScrollBarRange(true);
	}
	
	Invalidate();
	//stat("FrameResized to %dx%d: edit pane now %dx%d chars", editor.pxwidth, editor.pxheight, editor.width, editor.height);
}

/*
void c------------------------------() {}
*/

void CEditPane::MouseDown(BPoint where)
{
	MakeFocus();
	mouse_down = true;
	
	// transfer mouse click to editor backend
	if (editor.curev)
		editor.curev->MouseDown(where.x, where.y);
}

void CEditPane::MouseMoved(BPoint where, uint32 code, const BMessage *msg)
{
	if (editor.settings.use_ibeam_cursor)
	{
		if (code == B_ENTERED_VIEW)
		{
			be_app->SetCursor(B_CURSOR_I_BEAM);
		}
		else if (code == B_EXITED_VIEW)
		{
			be_app->SetCursor(B_CURSOR_SYSTEM_DEFAULT);
		}
	}

	if (mouse_down && editor.curev)
	{
		editor.curev->MouseDrag(where.x, where.y);
	}	
}

void CEditPane::MouseUp(BPoint where)
{
	mouse_down = false;
	
	if (editor.curev)
		editor.curev->MouseUp();
}

