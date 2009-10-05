

class DirMenuItem : public BMenuItem
{
public:
	DirMenuItem(const char *text, BBitmap *icon, BDirectory *dir, \
				BMessage *msg, char shortcut = 0, uint32 modifiers = 0);
	DirMenuItem(BMenu *submenu);
	~DirMenuItem();
	
	void DrawContent();
	void GetContentSize(float *width, float *height);
	void SetIcon(BBitmap *newIcon);
	void GetIcon(BBitmap *iconOut);
	
private:
	BBitmap *fIcon;
	float fHeightDelta;
	
};


