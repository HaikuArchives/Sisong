
/*
well
this is a long comment
please get rid of it
please please
just a test
*/

#include "editor.h"
#include "PopupPane.fdh"

#define RESIZE_GRABBER_HEIGHT	23

#define CLOSEBTN_WIDTH		12
#define CLOSEBTN_HEIGHT		12

static rgb_color lightColor = { 240, 240, 240 };
static rgb_color darkColor = { 200, 200, 200 };


PopupPane::PopupPane()
		: BView(BRect(0, 0, 150, 150), "popup_pane", B_FOLLOW_LEFT_RIGHT|B_FOLLOW_BOTTOM, 0)
{
	contents = NULL;
	open = false;
	
	BRect rc(Bounds());
	rc.bottom = RESIZE_GRABBER_HEIGHT-1;
	resizer = new ResizeBar(rc, this, B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP);
	AddChild(resizer);
	
	rc = Bounds();
	rc.top = RESIZE_GRABBER_HEIGHT;
	containerView = new BView(rc, "containerView", B_FOLLOW_ALL, 0);
	containerView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	AddChild(containerView);
}

PopupPane::~PopupPane()
{
}

// set the height of a popup pane (using the "y2-y1" definition of height).
// called by the resize grabby.
void PopupPane::SetHeight(int newCYExtent)
{
CEditArea *editarea = MainWindow->main.editarea;

	if (newCYExtent < RESIZE_GRABBER_HEIGHT + 8)
		newCYExtent = RESIZE_GRABBER_HEIGHT + 8;
	
	// keep pane from getting too small
	int delta = (int)Frame().Height() - newCYExtent;
	if (delta == 0) return;
	
	// keep edit area from getting too small
	int nh = (int)editarea->Bounds().Height() + delta;
	if (nh < 32)
	{
		delta += (32 - nh);
	}
	
	editarea->ResizeBy(0, delta);
	MoveBy(0, delta);
	ResizeBy(0, -delta);
}

/*
void c------------------------------() {}
*/

// pop open the pane, showing whatever contents were set last.
void PopupPane::Open()
{
	if (open) return;
	open = true;
	
	LockWindow();
	
	CEditArea *editarea = MainWindow->main.editarea;
	BRect eabounds(editarea->Bounds());
	
	// get desired height of pane
	int halfheight = (int)(HEIGHTOF(eabounds) / 2);
	int defaultheight = (int)(HEIGHTOF(eabounds) / 2.5);
	int height = settings->GetInt("PopupPaneHeight", defaultheight);
	if (height > halfheight) height = halfheight;
	
	// shrink edit area so there's room. note that these functions use
	// the "creative" concept of width/height (x2-x1 instead of (x2-x1)+1).
	editarea->ResizeTo(eabounds.Width(), \
						eabounds.Height() - height);
	
	// add popup pane as child of MainWindow (seems must do this before resizing,
	// or our child views won't resize)
	MainWindow->AddChild(this);
	
	// add contents to pane
	if (contents)
	{
		contents->PopupOpening();
		containerView->AddChild(contents);
	}
	
	// move popup pane into position and size to fit
	BRect eaframe(editarea->Frame());
	BRect rc(0, eaframe.bottom+1, eaframe.right, MainWindow->Bounds().bottom);
	
	MoveTo(rc.left, rc.top);
	ResizeTo(rc.Width(), rc.Height());
	
	// resize contents to match container view. theoretically this would happen
	// automatically, but if the content view has children, they may not be resized
	// properly unless we say so explicitly.
	if (contents)
	{
		BRect cvrc(containerView->Bounds());
		contents->MoveTo(0, 0);
		contents->ResizeTo(cvrc.Width(), cvrc.Height());
	}
	
	contents->MakeFocus();
	UnlockWindow();
}

// set the contents of the pane as given, then pop it open.
void PopupPane::Open(const char *title, PopupContents *contents)
{
	SetContents(title, contents);
	Open();
}

// set the contents and title of the popup pane.
void PopupPane::SetContents(const char *title, PopupContents *newContents)
{
	// set new contents
	if (contents != newContents)
	{
		if (open)
		{
			if (contents)
			{
				contents->PopupClosing();
				containerView->RemoveChild(contents);
			}
			
			if (newContents)
			{
				newContents->PopupOpening();
				containerView->AddChild(newContents);
			}
			
			// resize contents to match container view
			if (newContents)
			{
				BRect cvrc(containerView->Bounds());
				newContents->MoveTo(0, 0);
				newContents->ResizeTo(cvrc.Width(), cvrc.Height());
			}
		}
		
		// resize contents to fit
		contents = newContents;
	}
	
	// set new title
	resizer->SetTitle(title);
}

void PopupPane::SetTitle(const char *newTitle)
{
	resizer->SetTitle(newTitle);
}

// close the popup pane and returns the edit area to full height.
// the contents are orphaned.
void PopupPane::Close()
{
CEditArea *editarea = MainWindow->main.editarea;

	if (open)
	{
		settings->SetInt("PopupPaneHeight", (int)HEIGHTOF(Bounds()));
		open = false;
		
		// orphan our contents
		if (contents)
		{
			contents->PopupClosing();
			containerView->RemoveChild(contents);
		}
		
		// remove ourselves from window
		MainWindow->RemoveChild(this);
		
		// reset height of edit pane
		BRect eabounds(editarea->Frame());
		eabounds.bottom = MainWindow->Bounds().bottom;
		
		editarea->ResizeTo(eabounds.Width(), eabounds.Height());
		MainView->MakeFocus();
		
		// bit of a hack: since console is closed now make sure the
		// "Force Show Console" item is unchecked
		MainWindow->top.menubar->ShowConsoleItem->SetMarked(false);
	}
}

// remove the contents from the pane, so that there is nothing in it at all.
// the contents are only orphaned, not deleted.
void PopupPane::RemoveContents()
{
	if (contents)
	{
		if (open)
		{
			contents->PopupClosing();
			containerView->RemoveChild(contents);
		}
		
		contents = NULL;
	}
}

bool PopupPane::IsOpen()
{
	return open;
}

/*
void c------------------------------() {}
*/

ResizeBar::ResizeBar(BRect frame, PopupPane *parent, uint32 resizingMode)
		: BView(frame, "resize_bar", resizingMode, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
{
	dragging = false;
	this->parent = parent;
	
	this->title = NULL;
	SetTitle("Popup Pane");
	
	int x = (int)Bounds().right - CLOSEBTN_WIDTH - 1;
	int y = (int)Bounds().bottom - CLOSEBTN_HEIGHT - 4;
	BRect closebtn_rect(x, y, x+(CLOSEBTN_WIDTH-1), y+(CLOSEBTN_HEIGHT-1));
	
	AddChild(new CCloseButton(closebtn_rect,
							new BMessage(M_POPUPPANE_CLOSE), MainWindow, \
							B_FOLLOW_BOTTOM | B_FOLLOW_RIGHT));
	
	SetViewColor(lightColor);
}

ResizeBar::~ResizeBar()
{
	if (title)
		frees(title);
}

void ResizeBar::SetTitle(const char *newTitle)
{
	if (!newTitle) newTitle = "";
	
	if (title) frees(title);
	title = smal_strdup(newTitle);
	
	if (parent->IsOpen())
	{
		LockLooper();
		Invalidate();
		UnlockLooper();
	}
}

void ResizeBar::Draw(BRect region)
{
BRect rc(Bounds());
int x1, y1, x2, y2;
char *drawTitle;
BFont font;

	x1 = (int)rc.left; x2 = (int)rc.right;
	y1 = (int)rc.top; y2 = (int)rc.bottom;
	
	// draw title (caption)
	SetHighColor(0, 0, 0);
	SetLowColor(lightColor);
	
	int text_y = (y2 - 6);
	int grablines_end = x2 - CLOSEBTN_WIDTH - 4;
	
	MovePenTo(3, text_y);
	
	// limit title text width: suboptimal, but this shouldn't happen often
	drawTitle = strdup(title);
	GetFont(&font);
	while(font.StringWidth(drawTitle)+20 >= grablines_end)
	{
		int len = strlen(drawTitle);
		if (len == 0) break;
		drawTitle[len - 1] = 0;
	}
	
	DrawString(drawTitle);
	free(drawTitle);
	
	int grablines_start = (int)PenLocation().x + 4;
	
	// draw grabber lines
	SetHighColor(darkColor);
	for(int count=0;count<4;count++)
	{
		StrokeLine(BPoint(grablines_start, text_y), BPoint(grablines_end, text_y));
		text_y -= 3;
	}
	
	// outline top and bottom with dark color
	StrokeLine(BPoint(x1, y1), BPoint(x2, y1));
	StrokeLine(BPoint(x1, y2), BPoint(x2, y2));
}

void ResizeBar::MouseDown(BPoint where)
{
	GetScreenMouse(&drag_origin);
	original_height = (int)parent->Frame().Height();
	dragging = true;
	
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}

void ResizeBar::MouseMoved(BPoint where, uint32 code, const BMessage *msg)
{
BPoint pos;
int delta, newHeight;

	if (dragging)
	{
		GetScreenMouse(&pos);
		
		delta = (int)(drag_origin.y - pos.y);
		newHeight = (original_height + delta);
		
		parent->SetHeight(newHeight);
	}
}

void ResizeBar::MouseUp(BPoint where)
{
	dragging = false;
}


void ResizeBar::GetScreenMouse(BPoint *pt)
{
uint32 buttons;

	LockLooper();
	
	GetMouse(pt, &buttons);
	ConvertToScreen(pt);
	
	UnlockLooper();
}

/*
void c------------------------------() {}
*/

static rgb_color btnHilite = { 255, 255, 255 };
static rgb_color btnBody = { 236, 233, 216 };
static rgb_color btnShadow = { 173, 167, 151 };
static rgb_color btnShadowDeep = { 114, 111, 102 };

CCloseButton::CCloseButton(BRect frame, BMessage *msg, BLooper *target, uint32 resizingMode)
		: BView(frame, "closebutton", resizingMode, B_WILL_DRAW)
{
	invokeMsg = msg;
	invokeTarget = target;
	pressed = false;
	
	SetViewColor(btnBody);
}

CCloseButton::~CCloseButton()
{
	if (invokeMsg)
		delete invokeMsg;
}

void CCloseButton::Draw(BRect region)
{
	int x1, y1, x2, y2;
	x1 = (int)Bounds().left;
	y1 = (int)Bounds().top;
	x2 = (int)Bounds().right;
	y2 = (int)Bounds().bottom;
	
	if (!pressed)
	{
		SetHighColor(btnHilite);
		StrokeLine(BPoint(x1, y1), BPoint(x2-1, y1));
		StrokeLine(BPoint(x1, y1+1), BPoint(x1, y2-1));
		
		SetHighColor(btnShadowDeep);
		StrokeLine(BPoint(x2, y1), BPoint(x2, y2));
		StrokeLine(BPoint(x1, y2), BPoint(x2-1, y2));
		
		SetHighColor(btnShadow);
		StrokeLine(BPoint(x2-1, y1+1), BPoint(x2-1, y2-1));
		StrokeLine(BPoint(x1+1, y2-1), BPoint(x2-2, y2-1));
	}
	else
	{
		SetHighColor(btnShadowDeep);
		StrokeLine(BPoint(x1, y1), BPoint(x2-1, y1));
		StrokeLine(BPoint(x1, y1+1), BPoint(x1, y2-1));
		
		SetHighColor(btnHilite);
		StrokeLine(BPoint(x2, y1), BPoint(x2, y2));
		StrokeLine(BPoint(x1, y2), BPoint(x2-1, y2));
		
		// recess the "X"
		y2++;
		y1++;
	}
	
	// draw "X"
	SetHighColor(0,0,0);
	StrokeLine(BPoint(x1+3, y1+3), BPoint(x2-3, y2-4));
	StrokeLine(BPoint(x1+3, y2-4), BPoint(x2-3, y1+3));
}

void CCloseButton::MouseDown(BPoint where)
{
	pressed = true;
	Invalidate();
	SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
}

void CCloseButton::MouseUp(BPoint where)
{
	pressed = false;
	Invalidate();
	
	if (Bounds().Contains(where))
	{
		if (invokeMsg && invokeTarget)
			invokeTarget->PostMessage(invokeMsg);
	}
}














