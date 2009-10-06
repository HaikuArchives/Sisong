

class AboutPreflet : public Preflet
{
public:
	AboutPreflet(PrefsWindow *parent);
	~AboutPreflet();

	virtual void PrefletClosing();
	virtual void ReloadSettings();
	virtual void MessageReceived(BMessage *msg);

private:
	void MakeStrings(const char *list[], int y_start, int spacing, BFont *font);
	void Animate();

	CViewTimer *fTimer;
	BStringView *LogoChars[10];
	int rotate_frame;
};
