

class MiscPreflet : public Preflet
{
public:
	MiscPreflet(PrefsWindow *parent);
	void MessageReceived(BMessage *msg);
	void ReloadSettings();
	
private:
	BCheckBox *fCheckBox[32];
	int fNumCheckBoxes;
};
