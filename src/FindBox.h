
// values for mode parameter of constructor
#define FINDBOX_FIND		0
#define FINDBOX_REPLACE		1
#define FINDBOX_FIND_FILES	2

// DoFindNext options flags
#define FINDF_WHOLE_WORD		0x01
#define FINDF_CASE_SENSITIVE	0x02
#define FINDF_RECURSIVE			0x04
#define FINDF_FORCE_Y			0x08
#define FINDF_Y_AT_BOTTOM		0x10
#define FINDF_BACKWARDS			0x20	// find previous instead of find next
#define FINDF_IGNORE_DOT		0x40	// ignore "." hidden folders

// return values from DoFindNext
#define FN_HIT_FOUND			0
#define FN_NO_HITS				1
#define FN_NO_ADDITIONAL_HITS	2

// return values from SelectHit
#define SH_OK					0
#define SH_ALREADY_SELECTED		1

// the view that contains the controls, there is one for each tab
class FBView : public BView
{
	FBView(BRect frame, int initialMode);
	~FBView();
	static void InitTabData();

	friend class CFindBox;
	friend class FBTabView;

private:
	BTextControl *txtFindWhat;
	BTextControl *txtReplaceWith;
	BTextControl *txtFolder;
	BTextControl *txtFilter;
	BButton *DefButton;

	BCheckBox *chkWholeWord;
	BCheckBox *chkCaseSensitive;
	BCheckBox *chkBackwards;
	BCheckBox *chkRecursive;
	BCheckBox *chkIgnoreHiddenFolders;

	int fbmode;
};

// simple override for tabview control
class FBTabView : public BTabView
{
public:
	FBTabView(BRect frame, char *name)
		: BTabView(frame, name) { }

	virtual void MouseDown(BPoint where);
};

// the find box window itself
class CFindBox : public BWindow
{
public:
	CFindBox(int mode);
	~CFindBox();
	virtual void MessageReceived(BMessage *msg);
	virtual void DispatchMessage(BMessage *msg, BHandler *target);

	void HandleSearchRequest(BMessage *msg);
	void ProcessBrowseForFolderResults(BMessage *message);

	friend class FBView;
	friend class FBTabView;

private:
	char *GetSearchSettings(int *options);
	FBView *GetCurrentView();

	BTabView *tabview;
	FBView *views[3];
};


typedef struct SearchData
{
	char *lastSearch;
	char *lastReplaceWith;
	int lastOptions;
};

