

class ColorItem : public BListItem
{
public:
	ColorItem(const char *text, rgb_color fg, rgb_color bg, bool boldstate, char applic);
	~ColorItem();

	virtual	void DrawItem(BView *owner, BRect frame, bool complete=false);
	virtual	void Update(BView* owner, const BFont* font);

private:
	char *fText;
	rgb_color fgColor, bgColor;
	bool fBoldState;
	unsigned char fApplic;
	
	float fBaselineOffset;
};
