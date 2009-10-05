

class StatsPreflet : public Preflet
{
public:
	StatsPreflet(PrefsWindow *parent);
	~StatsPreflet();
	virtual void MessageReceived(BMessage *msg);
	virtual void ReloadSettings();
	virtual void PrefletClosing();
	
private:
	CViewTimer *fTimer;
	PrefsWindow *prefsWin;
	
	BStringView *fKeystrokesTyped;
	BStringView *fMouseClicks;
	BStringView *fCRsTyped;
	BTextView *fTimeUsed;
};
