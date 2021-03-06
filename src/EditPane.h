
// main editor pane
class CEditPane : public BView
{
public:
	CEditPane(BRect frame, uint32 resizingMode);
	~CEditPane();
	
	virtual void Draw(BRect updateRect);
	virtual void FrameResized(float width, float height);
	
	virtual void MouseDown(BPoint where);
	virtual void MouseMoved(BPoint where, uint32 code, const BMessage *msg);
	virtual void MouseUp(BPoint where);
	
	void ClearBelow(int y);
	void SetFontSize(int newsize);
	
	CFlashingCursor cursor;

private:
	bool mouse_down;
};
