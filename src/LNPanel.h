
// affects only the maximum height of window, not maximum height of document
#define MAX_LN_NUMBERS		1000
class LNPanel;

// an individual line number
class LNItem
{
public:
	void SetNumber(LNPanel *parent, int new_no);
	void DrawItem(LNPanel *parent, int y);
	
private:
	char StringToDraw[10];
	int draw_x;
	
};

// the line number view
class LNPanel : public BView
{
public:
	LNPanel(BRect frame, uint32 resizingMode);
	~LNPanel();
	virtual void Draw(BRect updateRect);
	friend class LNItem;
	
	void SetLineNumber(int y, int newNumber);
	void SetNumVisibleLines(int count);
	void Redraw(void);
	void RedrawIfNeeded(void);
	
	void SetFontSize(int newsize);
	
private:
	CFontDrawer *font_drawer;
	LNItem lineItems[MAX_LN_NUMBERS];
	int numLinesShown;
	bool redraw_needed;
	int fFace;
};

