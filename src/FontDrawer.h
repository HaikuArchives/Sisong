
class CFontDrawer
{
public:
	CFontDrawer(int font_size);
	~CFontDrawer();
	
	void SetSize(int newsize);
	void SetColors(BView *view, rgb_color fg, rgb_color bg);
	void SetFace(BView *view, int newface);
	int GetStringWidth(char *string);
	void DrawString(BView *view, char *string, int x, int y);
	void DrawString(BView *view, char *string, int x, int y, int len);
	
	int fontheight, fontwidth;
	BFont *font;

private:
	int _ascent;
	int _descent;
	int _face;
	BView *_lastview;
};


