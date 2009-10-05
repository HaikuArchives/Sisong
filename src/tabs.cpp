
#include <OS.h>
#include <View.h>
#include <String.h>

#include "editor.h"
#include "tabs.fdh"

const rgb_color color_tabbar_bg = { 0xf0, 0xf0, 0xf0 };
#define ICON_W			0//20
#define ICON_H			20
#define TEXT_SPACING	16

#define M_SCROLL_LEFT	'SCRL'
#define M_SCROLL_RIGHT	'SCRR'


CTabBar::CTabBar(BRect frame, uint32 resizingMode)
	: MessageView(frame, "TabView", resizingMode, B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE),
	  firsttab(NULL),
	  lasttab(NULL),
	  curtab(NULL),
	  fNumTabs(0),
	  _cantswapto(NULL),
	  fDragging(false),
	  fScrollAmt(0),
	  fBackgroundColor(color_tabbar_bg)
{
	// set font and get font size info
	fTabFont = new BFont(be_plain_font);
	fTabFont->SetSize(12);
	SetFont(fTabFont);
	
	font_height fh;
	fTabFont->GetHeight(&fh);
	fFontAscent = (int)ceilf(fh.ascent);
	
	SetViewColor(B_TRANSPARENT_COLOR);
	
	// create the buttons for scrolling when there are too many tabs to display	
	const int kButtonWidth = 24;
	
	BRect rc(0, 0, kButtonWidth-1, frame.Height());
	fScrollLeftButton = new BButton(rc, "", "<", new BMessage(M_SCROLL_LEFT));
	rc.OffsetBy(kButtonWidth, 0);
	fScrollRightButton = new BButton(rc, "", ">", new BMessage(M_SCROLL_RIGHT));
	
	rc.Set(Bounds().right-kButtonWidth*2, Bounds().top, Bounds().right, Bounds().bottom);
	fScrollButtonsView = new BView(rc, "SBtnView", B_FOLLOW_TOP|B_FOLLOW_RIGHT, 0);
	fScrollButtonsView->SetViewColor(ViewColor());
	
	AddChild(fScrollButtonsView);
	
	fScrollLeftButton->SetTarget(Looper());
	fScrollRightButton->SetTarget(Looper());
	fScrollButtonsView->AddChild(fScrollLeftButton);
	fScrollButtonsView->AddChild(fScrollRightButton);
	
	// hide the scroll buttons until needed
	fScrollButtonsVisible = true;
	SetScrollButtonsVisible(false);
	fScrollAmt = 0;
	fUserFreeRoam = false;
}

void CTabBar::SetScrollButtonsVisible(bool enable)
{
	if (enable == fScrollButtonsVisible) return;
	
	if (!enable)
	{
		fScrollButtonsView->MoveBy(0, 100);
		fScrollButtonsVisible = false;
	}
	else
	{
		fScrollButtonsView->MoveBy(0, -100);
		fScrollButtonsVisible = true;
	}
}


CTabBar::~CTabBar()
{
	delete fTabFont;
	
	TCTabItem *tab = firsttab, *next;
	while(tab)
	{
		next = tab->next;
		delete tab;
		tab = next;
	}
}

void CTabBar::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_SCROLL_LEFT:
			fUserFreeRoam = true;
			DoScrollLeft();
		break;
		
		case M_SCROLL_RIGHT:
			fUserFreeRoam = true;
			DoScrollRight();
		break;
		
		default:
			MessageView::MessageReceived(msg);
	}
}

void CTabBar::Draw(BRect updateRect)
{
	redraw(true);
}

// redraw the tab bar
void CTabBar::redraw(bool recalcPositions = true)
{
	if (!app_running) return;
	if (!curtab || !firsttab) return;
	LockLooper();
	
	if (recalcPositions)
		RecalcTabPositions();
	
	// if currently scrolled, but some tabs have been closed and now they would fit
	// within the total width, turn off the scroll buttons.
	if (fScrollAmt)
	{
		int tabs_width = (lasttab->rightedge - firsttab->leftedge) + 1;
		if (tabs_width < WIDTHOF(Bounds()))
		{
			ScrollTabsBy(-fScrollAmt, false);
		}
	}
	
	// show/hide scroll buttons as needed
	if (fScrollAmt != 0 || \
		(lasttab->rightedge >= Bounds().right && fNumTabs > 1))
	{
		SetScrollButtonsVisible(true);
		int buttons_left = (int)fScrollButtonsView->Frame().left;
		
		// set button(s) disabled if no more scrolling to be done
		fScrollLeftButton->SetEnabled((firsttab->leftedge < 0));
		fScrollRightButton->SetEnabled((lasttab->rightedge >= buttons_left));
		
		EnsureCurtabVisible();
	}
	else
	{
		SetScrollButtonsVisible(false);
	}
	
	// draw all the tabs
	TCTabItem *tab = firsttab;
	while(tab)
	{
		tab->drawItem();
		tab = tab->next;
	}		
	
	// clear any empty space unused by tabs
	SetHighColor(color_tabbar_bg);
	FillRect(BRect(lasttab->rightedge+1, 0, Bounds().right, TAB_HEIGHT));
	UnlockLooper();
}


void CTabBar::RecalcTabPositions()
{
int x = fScrollAmt;
TCTabItem *tab = firsttab;

	while(tab)
	{
		tab->width = tab->GetWidth();
		tab->leftedge = x;
		tab->rightedge = (x + (tab->width - 1));
		
		x += tab->width;
		tab = tab->next;
	}
}

/*
void c------------------------------() {}
*/

void CTabBar::DoScrollRight(bool doRedraw=true)
{
TCTabItem *tab;
	// find the first tab whose left edge is > 0, and move it so that
	// it is exactly at 0.
	for(tab=firsttab; tab; tab=tab->next)
	{
		if (tab->leftedge > 0)
		{
			ScrollTabsBy(-tab->leftedge, doRedraw);
			break;
		}
	}
}

void CTabBar::DoScrollLeft(bool doRedraw=true)
{
TCTabItem *tab;
	// going backwards, find the first tab whose left edge < 0, and scroll so that
	// it is exactly at 0.
	for(tab=lasttab; tab; tab=tab->prev)
	{
		if (tab->leftedge < 0)
		{
			// we want to increase the number, but it is negative, so negate it again
			ScrollTabsBy(-tab->leftedge, doRedraw);
			break;
		}
	}
}

// adjust X position of all tabs by "offs"
void CTabBar::ScrollTabsBy(int offs, bool doRedraw=true)
{
	for(TCTabItem *tab=firsttab; tab; tab=tab->next)
	{
		tab->leftedge += offs;
		tab->rightedge += offs;
	}
	
	fScrollAmt += offs;
	
	if (doRedraw)
		redraw(false);
}


// if the current tab is scrolled off the edge, makes it visible and redraws
// the tab bar. true is returned if the tabs were scrolled.
bool CTabBar::EnsureCurtabVisible(bool doRedraw)
{
	if (!curtab) return false;
	if (fUserFreeRoam) return false;
	
	if (curtab->leftedge < 0)
	{
		ScrollTabsBy(-curtab->leftedge, doRedraw);
		return true;
	}
	else if (fScrollAmt || fScrollButtonsVisible)
	{
		if (curtab->rightedge >= (int)fScrollButtonsView->Frame().left)
		{
			while(curtab->rightedge >= (int)fScrollButtonsView->Frame().left)
				DoScrollRight(false);
			
			if (doRedraw)
				redraw(false);
			
			return true;
		}
	}

	return false;
}


// convert an X coordinate within the tab bar to a TCTabItem
TCTabItem *CTabBar::XToTab(int x)
{
	for(TCTabItem *tab=firsttab; tab; tab=tab->next)
	{
		if (x >= tab->leftedge && x <= tab->rightedge)
			return tab;
	}
	
	return NULL;
}

/*
void c------------------------------() {}
*/

void CTabBar::MouseDown(BPoint where)
{
int x = (int)where.x;
TCTabItem *tab;
uint32 buttons;
BPoint pos;

	LockLooper();	// needed for GetMouse
	GetMouse(&pos, &buttons);

	tab = XToTab(x);
	if (tab)
	{		
		if (buttons == B_PRIMARY_MOUSE_BUTTON)
		{	// switch to tab, or rearranging tabs
			if (tab != curtab)
				tab->setActive();
			else
			{
				fUserFreeRoam = false;
				EnsureCurtabVisible();
			}
			
			SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);
			fDragging = true;
			_cantswapto = NULL;
		}
		else if (buttons == B_SECONDARY_MOUSE_BUTTON)
		{	// right-click; close tab
			ConfirmCloseEditView(tab->ev);
		}		
	}
	else if (buttons == B_SECONDARY_MOUSE_BUTTON)
	{	// right click in unused tab area
		SetActiveTab(CreateEditView(NULL));
	}
	
	UnlockLooper();
}

void CTabBar::MouseMoved(BPoint where, uint32 code, const BMessage *msg)
{
	if (!fDragging) return;	
	if (!curtab) return;
	LockLooper();
	
	// allow repositioning of tabs via drag n' drop
	int x = (int)where.x;
	TCTabItem *tab = XToTab(x);	
	
	if (tab && tab != _cantswapto)
	{
		if (tab != curtab)
		{
			// prevents rapid switching back and forth when tabs are different sizes
			_cantswapto = tab;
			
			SwapTabs(tab, curtab);
			RecalcTabPositions();
			
			if ((tab->next == curtab || tab->prev == curtab) && \
				EnsureCurtabVisible() == false)
			{
				tab->drawItem();
				curtab->drawItem();
			}
			else
			{
				redraw();
			}
		}
		else
		{
			_cantswapto = NULL;
		}
	}
	else if (_cantswapto && _cantswapto != curtab)
	{
		int x1 = _cantswapto->leftedge;
		int x2 = _cantswapto->rightedge;
		if (curtab->leftedge < _cantswapto->leftedge)
		{
			if (x > x2 - ((x2 - x1) / 3))
				_cantswapto = NULL;
		}
		else
		{
			if (x < x1 + ((x2 - x1) / 3))
				_cantswapto = NULL;
		}
	}
	
	UnlockLooper();
}

void CTabBar::MouseUp(BPoint where)
{
	fDragging = false;
}


// swap the relative positions of two tabs on the bar by fiddling
// with their linked-listed pointers
void CTabBar::SwapTabs(TCTabItem *t1, TCTabItem *t2)
{
TCTabItem *temp;
	
	// first, swap first and last pointers if one or the other is first or last
	if (firsttab==t1)		firsttab = t2;
	else if (firsttab==t2) firsttab = t1;
	
	if (lasttab==t1)		lasttab = t2;
	else if (lasttab==t2)	lasttab = t1;
	
	// ok, hang on...
	if (t2->next==t1)
	{
		// next pointers
		if (t2->prev) t2->prev->next = t1;
		t2->next = t1->next;
		t1->next = t2;
		
		// prev pointers
		t1->prev = t2->prev;
		t2->prev = t1;
		if (t2->next) t2->next->prev = t2;
	}
	else if (t1->next==t2)
	{
		// next pointers
		if (t1->prev) t1->prev->next = t2;		// Barnie's previous, Alfred, points next to Charlie
		t1->next = t2->next;					// Barnie's next is now Dennis, Charlie's old next.
		t2->next = t1;							// Charlie's next is now Barnie
		
		// prev pointers
		t2->prev = t1->prev;					// Charlie points to Alfred, Barnie's prev.
		t1->prev = t2;							// Barnie points to Charlie.
		if (t1->next) t1->next->prev = t1;		// Dennis, Charlie's old Next, now points to Barnie
	}
	else		// tabs are not neighbors at all
	{
		// next pointers
		if (t1->prev) t1->prev->next = t2;		// Barnie's prev Alfred points to Dennis now
		if (t2->prev) t2->prev->next = t1;		// Dennis's prev Charlie points to Barnie now.
		
		temp = t1->next;
		t1->next = t2->next;
		t2->next = temp;
		
		// prev pointers
		temp = t1->prev;
		t1->prev = t2->prev;
		t2->prev = temp;		
		
		// think t1_next and t2_next...since they're swapped now
		// t2 is actually t1 and vice versa
		if (t2->next) t2->next->prev = t2;		// Barnie's old Next Charlie, points at Dennis.
		if (t1->next) t1->next->prev = t1;		// Dennis's old Next Eager points to Barnie.		
	}
}


/*
void c------------------------------() {}
*/

// add a tab to the bar. the tab will correspond to the given editview.
void CTabBar::AddTab(EditView *ev)
{
TCTabItem *tab = new TCTabItem(this);

	tab->ev = ev;
	tab->isdirty = false;
	
	tab->prev = lasttab;
	tab->next = NULL;
	if (lasttab) lasttab->next = tab; else firsttab = tab;
	lasttab = tab;

	fNumTabs++;
	
	if (!curtab)
	{
		fUserFreeRoam = false;
		curtab = tab;
		MainWindow->UpdateWindowTitle();
	}
	
	redraw();	// handles RecalcTabPositions for us
}

// remove a tab from the bar given it's EditView.
void CTabBar::RemoveTab(EditView *ev)
{
TCTabItem *tab;

	if (tab = TabFromEV(ev))
	{
		if (tab == curtab)
		{
			if (tab->next)
			{
				tab->next->setActive();
			}
			else if (tab->prev)
			{
				tab->prev->setActive();
			}
			else
			{
				curtab = NULL;
				editor.curev = NULL;
			}
		}
		
		if (tab->next) tab->next->prev = tab->prev;
		if (tab->prev) tab->prev->next = tab->next;
		if (tab == firsttab) firsttab = firsttab->next;
		if (tab == lasttab) lasttab = lasttab->prev;
		delete tab;
		
		fNumTabs--;
		redraw();	// handles RecalcTabPositions implicitly
	}
}

// returns the object of the currently active tab
EditView *CTabBar::GetActiveTab()
{
	return curtab ? curtab->ev : NULL;
}

int CTabBar::GetTabCount()
{
	return fNumTabs;
}

// sets the active tab to the one which is holding document "ev".
void CTabBar::SetActiveTab(EditView *ev)
{
TCTabItem *tab;

	if (ev == NULL)	// used by editor_close_all
	{
		curtab = NULL;
		editor.curev = NULL;
	}
	else if (tab = TabFromEV(ev))
	{
		tab->setActive();
	}
}

void CTabBar::SwitchToNextTab()
{
TCTabItem *tab = curtab;

	if (tab == lasttab)
		tab = firsttab;
	else
		tab = tab->next;
	
	tab->setActive();
}

void CTabBar::SwitchToPrevTab()
{
TCTabItem *tab = curtab;

	if (tab == firsttab)
		tab = lasttab;
	else
		tab = tab->prev;
	
	tab->setActive();
}

void CTabBar::SetDirtyState(EditView *ev, bool newState)
{
TCTabItem *tab = TabFromEV(ev);

	if (tab && tab->isdirty != newState)
	{
		tab->isdirty = newState;
		tab->drawItem();
	}
}

TCTabItem *CTabBar::TabFromEV(EditView *ev)
{
TCTabItem *tab = firsttab;

	while(tab)
	{
		if (tab->ev == ev) return tab;
		tab = tab->next;
	}
	
	return NULL;
}

/*
void c------------------------------() {}
*/

TCTabItem::TCTabItem(CTabBar *tv)
{
	parent = tv;
}

BString *TCTabItem::GetCaption()
{
	if (ev)
	{
		return new BString(GetFileSpec(ev->filename));
	}
	else
	{
		char str[80];
		sprintf(str, "<tab 0x%06x>", this);
		return new BString(str);
	}
}

// returns the width of the tab
int TCTabItem::GetWidth(void)
{
BString *caption = this->GetCaption();
int cx;

	cx = (int)parent->fTabFont->StringWidth(caption->String());
	delete caption;
	
	return (cx + TEXT_SPACING + ICON_W + 1);
}

// returns true if this tab is the active one
bool TCTabItem::IsActive()
{
	return (parent->curtab == this);
}

// set the given tab active
void TCTabItem::setActive()
{
	if (parent->curtab != this || editor.curev != this->ev)
	{
		TCTabItem *oldtab = parent->curtab;
		parent->curtab = this;
		
		parent->fUserFreeRoam = false;
		if (!parent->EnsureCurtabVisible())
		{
			oldtab->drawItem();
			this->drawItem();
		}
		
		// refresh the new document
		editor.curev = this->ev;
		MainWindow->UpdateWindowTitle();
		editor.curev->FullRedrawView();
		FunctionList->ScanAll();
	}
}

// draws the tab, returning the width of the tab.
int TCTabItem::drawItem()
{
const int x = leftedge;
const int TabWidth = width;

	if (!app_running) return 0;
	parent->LockLooper();
	
	static const rgb_color Active_BG = { 0xf0, 0xf0, 0xf0 };
	static const rgb_color Active_Text = { 0, 0, 0 };
	static const rgb_color Inactive_BG = { 0xc0, 0xc0, 0xc0 };
	static const rgb_color Inactive_Text = { 0x80, 0x80, 0x80 };
	static const rgb_color White_FF = { 0xff, 0xff, 0xff };
	static const rgb_color White_FB = { 0xfb, 0xfb, 0xfb };
	static const rgb_color White_F0 = { 0xf0, 0xf0, 0xf0 };
	static const rgb_color Grey = { 0xc8, 0xc8, 0xc8 };
	static const rgb_color Orange = { 0xfa, 0xaa, 0x3c };
	static const rgb_color Red = { 0xca, 0x10, 0x10 };
	static const rgb_color DkRed = { 0xaa, 0x10, 0x10 };
	
	rgb_color ColorTabBG;
	rgb_color ColorTabText;
	
	int TabRight = x + (TabWidth - 1);
	int TabTop;
	int x1 = x + 1;
	int x2 = TabRight - 1;
	int icon_left;
	int TextOffset;

	if (this->IsActive())
	{
		ColorTabBG = Active_BG;
		ColorTabText = !isdirty ? Active_Text : Red;
		TabTop = 2;
		TextOffset = 4;
		icon_left = ICON_W;
		
		QFillRect(x1, 0, x2, 1, White_FF);
		QFillRect(x1, 2, x2, 2, White_FB);
		QFillRect(x1, 2, x2, 5, Orange);
		QFillRect(x1, 6, x2, TAB_HEIGHT, ColorTabBG);
		// right side, w/ shadow
		QFillRect(TabRight, TabTop, TabRight, TAB_HEIGHT-3, Grey);
		QFillRect(TabRight, TAB_HEIGHT-3, TabRight, TAB_HEIGHT, color_tabbar_bg);
	}
	else
	{
		ColorTabBG = Inactive_BG;
		ColorTabText = !isdirty ? Inactive_Text : DkRed;
		TabTop = 3;
		TextOffset = 3;
		icon_left = 0;
		
		QFillRect(x1, 0, x2, 1, color_tabbar_bg);
		QFillRect(x1, 2, x2, 2, White_FF);
		QFillRect(x1, 3, x2, 3, White_FB);
		QFillRect(x1, 4, x2, TAB_HEIGHT-5, ColorTabBG);
		QFillRect(x1, TAB_HEIGHT-4, x2, TAB_HEIGHT-3, White_F0);
		QFillRect(x1, TAB_HEIGHT-2, x2, TAB_HEIGHT, White_FF);
		// right side
		QFillRect(TabRight, TabTop, TabRight, TAB_HEIGHT, White_F0);
	}
	
	// draw spacer at top and left side
	QFillRect(x, 0, x, TabTop-1, color_tabbar_bg);
	QFillRect(TabRight, 0, TabRight, TabTop-1, color_tabbar_bg);
	
	QFillRect(x, TabTop, x, TAB_HEIGHT, White_F0);

	// draw icon
	//int icon_top = tab->isdirty ? ICON_H : 0;
	//BitBlt(hdc, x+5, TabTop+1, ICON_W, ICON_H, diskicon->hdc, icon_left, icon_top, SRCCOPY);
	
	// text
	BString *caption = this->GetCaption();
	
	parent->SetHighColor(ColorTabText);
	parent->SetLowColor(ColorTabBG);
	
	BPoint pt(x+ICON_W+(TEXT_SPACING/2)+TextOffset, TabTop + parent->fFontAscent + 5);
	parent->DrawString(caption->String(), pt);
	delete caption;
	
	parent->UnlockLooper();
	return TabWidth;
}

void TCTabItem::QFillRect(int x1, int y1, int x2, int y2, rgb_color color)
{
	parent->SetHighColor(color);
	parent->FillRect(BRect(x1, y1, x2, y2), B_SOLID_HIGH);
}

