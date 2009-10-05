
#define TAB_HEIGHT			27

class TCTabItem;


class CTabBar : public MessageView
{
public:
	CTabBar(BRect frame, uint32 resizingMode);
	~CTabBar();
	void Draw(BRect updateRect);
	void MessageReceived(BMessage *msg);
	void MouseDown(BPoint where);
	void MouseMoved(BPoint where, uint32 code, const BMessage *msg);
	void MouseUp(BPoint where);
	
	void redraw(bool recalcPositions=true);
	void AddTab(EditView *ev);
	void RemoveTab(EditView *ev);
	void SetActiveTab(EditView *ev);
	void SwitchToNextTab();
	void SwitchToPrevTab();
	void SetDirtyState(EditView *ev, bool newState);
	EditView *GetActiveTab();
	int GetTabCount();
	
	rgb_color fBackgroundColor;
	friend class TCTabItem;
	
private:
	void RecalcTabPositions();
	void SwapTabs(TCTabItem *t1, TCTabItem *t2);
	void DoScrollLeft(bool doRedraw=true);
	void DoScrollRight(bool doRedraw=true);
	void ScrollTabsBy(int offs, bool doRedraw=true);
	bool EnsureCurtabVisible(bool doRedraw=true);
	TCTabItem *XToTab(int x);
	TCTabItem *TabFromEV(EditView *ev);
	
	void SetScrollButtonsVisible(bool enable);
	BView *fScrollButtonsView;
	bool fScrollButtonsVisible;
	BButton *fScrollLeftButton;
	BButton *fScrollRightButton;
	int fScrollAmt;
	bool fUserFreeRoam;
	
	TCTabItem *firsttab, *lasttab;
	TCTabItem *curtab;
	TCTabItem *_cantswapto;

	int fNumTabs;
	
	BFont *fTabFont;
	int fFontAscent;
	//stImage *_diskicon;

	bool fDragging;
};


class TCTabItem
{
public:
	TCTabItem(CTabBar *tv);
	
	TCTabItem *next, *prev;
	EditView *ev;			// the document associated with this tab
	char isdirty;			// 1 if "dirty" (red) version of icon is shown
	int leftedge;			// X coordinate of left edge of tab
	int rightedge;			// X coordinate of right edge
	int width;				// width of tab
	
	CTabBar *parent;
	
	bool IsActive();
	void setActive();
	BString *GetCaption();
	int GetWidth();
	int drawItem();

private:
	void QFillRect(int x1, int y1, int x2, int y2, rgb_color color);
};


