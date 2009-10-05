

class ShortcutsPreflet : public Preflet
{
public:
	ShortcutsPreflet(PrefsWindow *parent);

	BPopUpMenu *fMenu[NUM_F_KEYS];
	BMenuField *fMenuField[NUM_F_KEYS];

	virtual void ReloadSettings();
	
private:
	void DoForEachMenuItem(bool (*func)(BMenuItem *item, void *data1, void *data2, void *data3), void *data1, void *data2, void *data3);
	
	void PopulateMenu(int fkeyindex);
	int GetMessageForIndex(int index);
	int GetIndexForMessage(int what_code);
	
	virtual void MessageReceived(BMessage *msg);
	
	PrefsWindow *fParent;
};
