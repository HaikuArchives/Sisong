
#define LN_WIDTH			62		// line-numbers bar
#define SPACER_WIDTH		16		// spacer between line-numbers and document

#define SBAR_SIZE			14		// width/height of vertical/horizontal scrollbars
#define STATUS_HEIGHT		16		// height of status bar
#define MENU_HEIGHT			19		// height of menu bar
#define FL_WIDTH			170		// width of Function List

#define CMDP_WIDTH			LN_WIDTH// width of command preview pane

#define TOPVIEW_HEIGHT		(MENU_HEIGHT + TAB_HEIGHT)


class testview;
class CEditArea;
class DocScrollBar;
class PopupPane;
class SearchResultsPane;

// the main editor window
class CMainWindow : public BWindow
{
public:
	CMainWindow(BRect frame);
	~CMainWindow();

	// -- controls --
	struct		// top section
	{
		BView *topview;
			MainMenuBar *menubar;
			CTabBar *tabbar;
	} top;

	struct		// main area
	{
		CEditArea *editarea;
	} main;

	struct		// popup panes
	{
		PopupPane *pane;
			SearchResultsPane *searchresults;
			CompilePane *compile;
			BuildHelpPane *buildhelp;
	} popup;


	bool IsMenuCommand(unsigned int code);
	void ProcessMenuCommand(unsigned int code, BMessage *msg=NULL);

	virtual void MessageReceived(BMessage *msg);
	virtual void DispatchMessage(BMessage *message, BHandler *handler);
	virtual void ProcessRefs(BMessage *message);

	virtual void WindowActivated(bool active);
	virtual bool QuitRequested();

	void UpdateWindowTitle();

	CViewTimer *cursor_timer;
	bool fDoingInstantQuit;
	bool fWindowIsForeground;
	bool fClosing;
};

/*
void c------------------------------() {}
*/



