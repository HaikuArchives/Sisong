

class GeneralPreflet : public Preflet
{
public:
	GeneralPreflet(PrefsWindow *parent);
	virtual void MessageReceived(BMessage *msg);
	
	virtual void ReloadSettings();
	
private:
	BCheckBox *fCheckBox[32];
	
};
