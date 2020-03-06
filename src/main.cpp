
#include "editor.h"
#include "main.fdh"
#include <Screen.h>

EditorData editor;
CProjectManager ProjectManager;

CMainWindow *MainWindow = NULL;
CEditPane *MainView = NULL;
CFunctionList *FunctionList = NULL;
DocScrollBar *scrHorizontal = NULL;
DocScrollBar *scrVertical = NULL;
LNPanel *ln_panel = NULL;

CFontDrawer *FontDrawer = NULL;
BFilePanel *FilePanel = NULL;
CTabBar *TabBar = NULL;

bool app_running = false;


int main(int argc, char **argv)
{
	//smal_init();
	memset(&editor, 0, sizeof(editor));

	char lfn[80];
	if (argv[0][0] == '/')
	{
		sprintf(lfn, "%s.log", argv[0]);
	}
	else
	{
		const char *myname = strrchr(argv[0], '/');
		if (!myname) myname = argv[0]; else myname++;
		sprintf(lfn, "/boot/dev/sisong/%s.log", myname);
	}
	SetLogfileName(lfn);

	settings = Config::load();
	LoadEditorSettings();

	if (argc >= 2)
	{
		if (!strcmp(argv[1], "-test"))
		{
			editor.settings.testmode = 1;
			editor.settings.esc_quits_immediately = 1;
		}
		else if (!strcmp(argv[1], "-big"))
		{
			editor.settings.testmode = 2;
		}
	}

	editor.stats.LastUsageUpdateTime = system_time();

	lexer_init();

	EApp *app = new(EApp);
	app->Run();

    lexer_close();

	SaveEditorSettings();
	Config::save(settings);

	delete settings;
    delete app;

    return 0;//smal_cleanup();
}

/*
void c----------------------------() {}
*/

EApp::EApp()
	: BApplication(APPLICATION_SIGNATURE)
{
	BRect rc;

	if (editor.settings.testmode == 1)
		rc.Set(383, 68, 383+900, 68+800+5);
	else if (editor.settings.testmode == 2)
		rc.Set(225, 26, 1440, 1024);
	else
	{
		// get a good default window size for first-time open
		BScreen screen;
		BRect defaultrect(screen.Frame());

		// on 4:3 fullscreen the default size looks squished so they
		// need a square inset
		float aspect_ratio = (screen.Frame().Width()+1) / (screen.Frame().Height()+1);

		if (aspect_ratio >= 1.50f)	// widescreen
			defaultrect.InsetBy(200, 50);
		else						// fullscreen
			defaultrect.InsetBy(50, 50);

		// now try to load previous position from config file, which will
		// override the default if successful.
		rc.left = settings->GetInt("window_left", (int)defaultrect.left);
		rc.top = settings->GetInt("window_top", (int)defaultrect.top);
		rc.right = settings->GetInt("window_right", (int)defaultrect.right);
		rc.bottom = settings->GetInt("window_bottom", (int)defaultrect.bottom);
	}

	MainWindow = new CMainWindow(rc);

	// create backbuffer
	editor.curline_bb = new OffscreenBuffer(4096, 100);
	editor.curline_bb->Lock();
	editor.curline_bb->SetFont(FontDrawer->font);
	editor.curline_bb->Unlock();
	editor.bbed_line = -1;

	// setup global convenience pointers
	MainView = MainWindow->main.editarea->editpane;
	ln_panel = MainWindow->main.editarea->ln;
	FunctionList = MainWindow->main.editarea->functionlist;
	scrHorizontal = MainWindow->main.editarea->HScrollbar;
	scrVertical = MainWindow->main.editarea->VScrollbar;
	TabBar = MainWindow->top.tabbar;

	// calc initial sizing data for edit area
	MainView->LockLooper();
	rc = MainView->Bounds();
	MainView->FrameResized(WIDTHOF(rc), HEIGHTOF(rc));
	MainView->UnlockLooper();

	// initilize menus
	ProjectManager.UpdateProjectsMenu();

	// create initial document view
	editor.DocList = new CList;

	editor.curev = CreateEditView("/boot/dev/sisong/src/borkme");
	if (!editor.curev) editor.curev = CreateEditView(NULL);
	MainWindow->UpdateWindowTitle();

	app_running = true;
	MainWindow->LockLooper();
	{
		MainWindow->main.editarea->editpane->Invalidate();
		MainWindow->main.editarea->ln->Invalidate();
		MainWindow->top.tabbar->Invalidate();

		FunctionList->ScanAll();
	}

	MainWindow->UnlockLooper();
}

EApp::~EApp()
{
	app_running = false;

	delete FontDrawer;
	FontDrawer = NULL;

	delete editor.curline_bb;
	editor.curline_bb = NULL;

	delete editor.DocList;
	editor.DocList = NULL;
}

void EApp::RefsReceived(BMessage *message)
{
	// processing refs directly here caused crashing issue...
	MainWindow->PostMessage(message);
}

void EApp::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		default:
			BApplication::MessageReceived(msg);
		break;
	}
}

/*
void c------------------------------() {}
*/

// stuff the "editor.settings" structure in and out of the "settings" class
// TODO: find a way to consolidate these two different settings methods

void LoadEditorSettings()
{
bool firsttime;

	firsttime = settings->GetInt("ES_FirstTime", 1);
	if (firsttime)
		settings->SetInt("ES_FirstTime", 0);

	editor.settings.font_size = settings->GetInt("font_size", 16);
	editor.settings.tab_width = settings->GetInt("tab_width", 4);

	editor.settings.smart_indent_on_open = settings->GetInt("indent_open", 1);
	editor.settings.smart_indent_on_close = settings->GetInt("indent_close", 1);
	editor.settings.no_smart_open_at_baselevel = settings->GetInt("noindent_bl", 1);
	editor.settings.language_aware_indent = settings->GetInt("la_indent", 0);
	editor.settings.DrawTabLines = settings->GetInt("show_tab_lines", 1);
	editor.settings.DoBraceMatching = settings->GetInt("do_brace_match", 1);
	editor.settings.DisableLexer = settings->GetInt("disable_lexer", 0);
	editor.settings.ShowBuildHelp = settings->GetInt("show_build_help", 1);
	editor.settings.CheckForUpdate = settings->GetInt("CheckForUpdate", 1);

	editor.settings.use_ibeam_cursor = settings->GetInt("use_ibeam", 0);
	//editor.settings.swap_ctrl_and_alt = settings->GetInt("swap_ctrl_and_alt", 0);

	editor.settings.FixIndentationGaps = settings->GetInt("FixIndentationGaps", 1);
	editor.settings.TrimTrailingOnSave = settings->GetInt("TrimTrailingOnSave", 1);
	editor.settings.WarnHaikuGuidelines = settings->GetInt("WarnHaikuGuidelines", 0);
	editor.settings.EnableAutoSaver = settings->GetInt("EnableAutoSaver", 1);

	editor.settings.build.JumpToErrors = settings->GetInt("JumpToErrors", 1);
	editor.settings.build.NoJumpToWarning = settings->GetInt("NoJumpToWarning", 0);

	editor.stats.keystrokes_typed = settings->GetInt("keystrokes_typed", 0);
	editor.stats.mouse_clicks = settings->GetInt("mouse_clicks", 0);
	editor.stats.CRs_typed = settings->GetInt("CRs_typed", 0);
	editor.stats.days_used = settings->GetInt("days_used", 0);
	editor.stats.hours_used = settings->GetInt("hours_used", 0);
	editor.stats.minutes_used = settings->GetInt("minutes_used", 0);
	editor.stats.seconds_used = settings->GetInt("seconds_used", 0);
	editor.stats.us_used = settings->GetInt("us_used", 0);

	// load f keys
	for(int i=0;i<NUM_F_KEYS;i++)
	{
		char fk[16];
		sprintf(fk, "F%d", i+1);
		editor.settings.fkey_mapping[i] = settings->GetInt(fk, 0);
	}

	// set f-key defaults on first-time load
	if (firsttime)
	{
		editor.settings.fkey_mapping[3-1] = M_SEARCH_FIND_NEXT;
		editor.settings.fkey_mapping[5-1] = M_RUN_RUN;
		editor.settings.fkey_mapping[7-1] = M_RUN_BUILD_NO_RUN;
		editor.settings.fkey_mapping[10-1] = M_PROJECT_NEW;
	}
}

void SaveEditorSettings()
{
	settings->SetInt("font_size", editor.settings.font_size);
	settings->SetInt("tab_width", editor.settings.tab_width);

	settings->SetInt("indent_open", editor.settings.smart_indent_on_open);
	settings->SetInt("indent_close", editor.settings.smart_indent_on_close);
	settings->SetInt("noident_bl", editor.settings.no_smart_open_at_baselevel);
	settings->SetInt("la_indent", editor.settings.language_aware_indent);
	settings->SetInt("show_tab_lines", editor.settings.DrawTabLines);
	settings->SetInt("do_brace_match", editor.settings.DoBraceMatching);
	settings->SetInt("disable_lexer", editor.settings.DisableLexer);
	settings->SetInt("show_build_help", editor.settings.ShowBuildHelp);
	settings->SetInt("CheckForUpdate", editor.settings.CheckForUpdate);

	settings->SetInt("use_ibeam", editor.settings.use_ibeam_cursor);
	//settings->SetInt("swap_ctrl_and_alt", editor.settings.swap_ctrl_and_alt);

	settings->SetInt("FixIndentationGaps", editor.settings.FixIndentationGaps);
	settings->SetInt("TrimTrailingOnSave", editor.settings.TrimTrailingOnSave);
	settings->SetInt("WarnHaikuGuidelines", editor.settings.WarnHaikuGuidelines);
	settings->SetInt("EnableAutoSaver", editor.settings.EnableAutoSaver);

	settings->SetInt("JumpToErrors", editor.settings.build.JumpToErrors);
	settings->SetInt("NoJumpToWarning", editor.settings.build.NoJumpToWarning);

	settings->SetInt("keystrokes_typed", editor.stats.keystrokes_typed);
	settings->SetInt("mouse_clicks", editor.stats.mouse_clicks);
	settings->SetInt("CRs_typed", editor.stats.CRs_typed);
	settings->SetInt("days_used", editor.stats.days_used);
	settings->SetInt("hours_used", editor.stats.hours_used);
	settings->SetInt("minutes_used", editor.stats.minutes_used);
	settings->SetInt("seconds_used", editor.stats.seconds_used);
	settings->SetInt("us_used", editor.stats.us_used);

	for(int i=0;i<NUM_F_KEYS;i++)
	{
		char fk[16];
		sprintf(fk, "F%d", i+1);
		settings->SetInt(fk, editor.settings.fkey_mapping[i]);
	}
}

/*
void c----------------------------() {}
*/

void LockWindow()
{
	MainWindow->LockLooper();
}

void UnlockWindow()
{
	MainWindow->UnlockLooper();
}


