
#ifndef __EDITOR_H
#define __EDITOR_H

#include <OS.h>
#include <Application.h>
#include <String.h>
#include <List.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <StopWatch.h>
#include <interface/Alert.h>
#include <interface/Font.h>
#include <interface/ScrollBar.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <interface/StringView.h>
#include <interface/TextControl.h>
#include <interface/TabView.h>
#include <interface/Button.h>
#include <interface/CheckBox.h>
#include <Path.h>
#include <FilePanel.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <FindDirectory.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>		// min/max

#include "../common/basics.h"
#include "../common/smal.h"
int min(int a, int b);
int max(int a, int b);
void CenterWindow(BWindow *parent, BWindow *child, bool at_bottom=false);
const char *GetFileSpec(const char *file_and_path);
bool strbegin(const char *bigstr, const char *smallstr);
void Unimplemented(void);

class EditView;
extern int ignore_scrollbars;	//weee! i'm crazy edd and i love hacks!!! ba la la la!

#define APPLICATION_NAME		"Sisong"
#define APPLICATION_SIGNATURE	"application/x-vnd.KT-Sisong"

#include "SubWindow.h"
#include "QListView.h"
#include "testview.h"
#include "CList.h"
#include "config.h"
#include "colors.h"
#include "messages.h"
#include "lexer.h"
#include "line.h"
#include "DocPoint.h"
#include "FindBox.h"
#include "selection.h"
#include "undo.h"
#include "bmatch.h"
#include "clipboard.h"
#include "cmdseq.h"
#include "edit_cursor.h"
#include "edit_mouse.h"
#include "edit.h"
#include "tabs.h"
#include "draw.h"
#include "menu.h"
#include "main.h"
#include "projects.h"
#include "InputBox.h"
#include "PopupPane.h"
#include "SearchResults.h"
#include "CompilePane.h"
#include "BuildHelpPane.h"
#include "FunctionList.h"
#include "EditArea.h"
#include "MainWindow.h"
#include "ColoredStringItem.h"

extern CMainWindow *MainWindow;
extern CEditPane *MainView;
extern DocScrollBar *scrHorizontal, *scrVertical;
extern CTabBar *TabBar;
extern CFunctionList *FunctionList;
extern CFontDrawer *FontDrawer;
extern LNPanel *ln_panel;
extern CProjectManager ProjectManager;

struct EditorData
{
	// EditViews for each open document
	EditView *curev;				// pointer to current open document
	BList *DocList;					// list of all open documents
	int NextUntitledID;				// next number for untitled documents
	int NextDocID;					// next DocID value (unique ID for each document opened in a session)
	
	// information about the window
	int width, height;				// width and height in chars
	int pxwidth, pxheight;			// size of editor pane in pixels
	int font_width, font_height;	// size of a char in px
	char PartialLineAtBottom;		// 1 if bottom line is partially cut off
	char HozBarVisible;				// 1 if horizontal scrollbar is visible
	
	// backbuffer which attempts to store the contents of the current line (only)
	OffscreenBuffer *curline_bb;
	// which line is currently stored in curline_bb, -1 if none
	int bbed_line;
	
	// true if the cursor is in Overwrite mode, false if the cursor is in Insert mode
	bool InOverwriteMode;
	
	// last filename opened or saved, used for making guesses about
	// default directory for Open/Save box, when the path isn't known for sure.
	char last_filepath_reference[MAXPATHLEN];
	
	/*
	  NOTE TO SELF:
	    Don't add anything in here that is not REALLY a user-settable preference
	    from the prefs panel. Otherwise, it can break the Revert functionality
	    of the prefs panel (because it does a memcmp() on the whole structure).
	*/
	struct
	{
		int tab_width;
		int font_size;
		
		bool smart_indent_on_open;
		bool smart_indent_on_close;
		bool no_smart_open_at_baselevel;
		bool language_aware_indent;
		bool use_ibeam_cursor;
		bool esc_quits_immediately;
		//bool swap_ctrl_and_alt;
		
		bool DrawTabLines;
		bool DoBraceMatching;
		bool DisableLexer;
		bool ShowBuildHelp;
		bool CheckForUpdate;
		
		bool FixIndentationGaps;
		bool TrimTrailingOnSave;
		bool TTExceptBlankLines;
		bool WarnHaikuGuidelines;
		bool EnableAutoSaver;
		
		struct
		{
			bool JumpToErrors;
			bool NoJumpToWarning;
			bool NoJumpToWarningAtAll;
		} build;
		
		// F-key shortcuts: maps F1-F12 to associated menu commands,
		// or 0 if nothing set
		#define NUM_F_KEYS			12
		unsigned int fkey_mapping[NUM_F_KEYS];
	} settings;
	
	struct
	{
		unsigned int keystrokes_typed;
		unsigned int CRs_typed;
		unsigned int mouse_clicks;
		
		unsigned int days_used;
		unsigned int hours_used;
		unsigned int minutes_used;
		unsigned int seconds_used;
		unsigned int us_used;
		bigtime_t LastUsageUpdateTime;
	} stats;

	char testmode;
};

extern EditorData editor;
extern bool app_running;

#define KEY_MOUSEWHEEL_UP	-100
#define KEY_MOUSEWHEEL_DOWN	-101

#define TAB				9
#define TAB_WIDTH		4



#endif
