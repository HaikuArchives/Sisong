
class PopupContents;
class ResizeBar;
class CCloseButton;


class PopupPane : public BView
{
public:
	PopupPane();
	~PopupPane();
	
	void Open();
	void Open(const char *title, PopupContents *contents);
	void Close();
	bool IsOpen();
	void SetContents(const char *title, PopupContents *newContents);
	void SetTitle(const char *newTitle);
	void RemoveContents();
	
	friend class ResizeBar;
	
private:
	void SetHeight(int newCYExtent);
	
	BView *containerView;
	ResizeBar *resizer;
	
	PopupContents *contents;
	bool open;
};

// contents of the popup pane
class PopupContents : public BView
{
public:
	PopupContents()
		: BView(BRect(0,0,100,100), "popup container view", B_FOLLOW_ALL, \
		B_FULL_UPDATE_ON_RESIZE)
	{
		SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	}
	
	// can be implemented by derived classes to receive notification when the
	// popup pane is closing or they are about to be replaced by another pane
	virtual void PopupClosing() { }
	virtual void PopupOpening() { }
};


class ResizeBar : public BView
{
public:
	ResizeBar(BRect frame, PopupPane *parent, uint32 resizingMode);
	~ResizeBar();
	void SetTitle(const char *newTitle);
	
	virtual void Draw(BRect region);
	virtual void MouseDown(BPoint where);
	virtual void MouseMoved(BPoint where, uint32 code, const BMessage *msg);
	virtual void MouseUp(BPoint where);

private:
	PopupPane *parent;
	void GetScreenMouse(BPoint *where);
	bool dragging;
	BPoint drag_origin;
	int original_height;
	char *title;
};

class CCloseButton : public BView
{
public:
	CCloseButton(BRect frame, BMessage *msg, BLooper *target, uint32 resizingMode);
	~CCloseButton();
	
	virtual void Draw(BRect region);
	virtual void MouseDown(BPoint where);
	virtual void MouseUp(BPoint where);
	
private:
	BMessage *invokeMsg;
	BLooper *invokeTarget;
	
	bool pressed;
};
