

typedef struct CheckboxPrefletData
{
	const char *text;		// text to display next to the checkbox
	bool *target;			// pointer to "editor.settings" flag it controls
	bool *depends;			// an optional flag which controls whether this box is enabled
	bool depends_xor;		// value to xor with value of dim_if to determine visibility
};


class CheckboxPreflet : public Preflet
{
public:
	CheckboxPreflet(PrefsWindow *parent, CheckboxPrefletData *data);
	void MessageReceived(BMessage *msg);
	void ReloadSettings();
	
private:
	CheckboxPrefletData *fData;
	BCheckBox *fCheckBox[32];
	int fNumCheckBoxes;
};
