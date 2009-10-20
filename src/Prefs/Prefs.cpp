
#include "../editor.h"
#include "Prefs.h"

#include "Preflet.h"
#include "AboutPreflet.h"
#include "CheckboxPreflet.h"
#include "ColorsPreflet.h"
#include "ShortcutsPreflet.h"
#include "StatsPreflet.h"

#include "Prefs.fdh"

#define M_PREFS_SELECT		'PSel'
#define M_REVERT_CLICKED	'RVRT'
#define M_OK_CLICKED		'OK!!'

PrefsWindow *CurrentPrefsWindow = NULL;

// prefs data for checkboxes (move this somewhere else maybe)
static CheckboxPrefletData GeneralPanelData[] =
{
	"Auto-indent on block open", &editor.settings.smart_indent_on_open, NULL, 0,
	"Auto-outdent on block close", &editor.settings.smart_indent_on_close, NULL, 0,
	"Do not auto-indent from baselevel", &editor.settings.no_smart_open_at_baselevel, &editor.settings.smart_indent_on_open, 0,
	"Smart auto-indenting for \"switch\" statement", &editor.settings.language_aware_indent, &editor.settings.smart_indent_on_open, 0,
	"", NULL, NULL, 0,
	"Draw Tab Lines", &editor.settings.DrawTabLines, NULL, 0,
	"Do Brace Matching", &editor.settings.DoBraceMatching, NULL, 0,
	"Disable Lexer", &editor.settings.DisableLexer, NULL, 0,
	"Use I-Beam cursor", &editor.settings.use_ibeam_cursor, NULL, 0,
	"Show Build Help", &editor.settings.ShowBuildHelp, NULL, 0,
	"Notify if update available", &editor.settings.CheckForUpdate, NULL, 0,
	
	NULL, NULL, NULL, 0
};

static CheckboxPrefletData MiscPanelData[] =
{
	"Fix gaps in indentation when opening", &editor.settings.FixIndentationGaps, NULL, 0,
	"Trim trailing whitespace when saving", &editor.settings.TrimTrailingOnSave, NULL, 0,
	"... but leave lines which are ONLY whitespace alone", &editor.settings.TTExceptBlankLines, &editor.settings.TrimTrailingOnSave, 0,
	"Periodically auto-save to /boot/var/tmp/Sisong", &editor.settings.EnableAutoSaver, NULL, 0,
	"", NULL, NULL, 0,
	"Show guideline at 80 chars", &editor.settings.ShowCol80Guideline, NULL, 0,
	//"Warn if code doesn't match Haiku Coding Guidelines", &editor.settings.WarnHaikuGuidelines, NULL, 0,
	
	NULL, NULL, NULL, 0
};

static CheckboxPrefletData BuildPanelData[] =
{
	"Auto-jump to first error/warning", &editor.settings.build.JumpToErrors, NULL, 0,
	"Favor errors over warnings when auto-jumping", &editor.settings.build.NoJumpToWarning, &editor.settings.build.JumpToErrors, 0,
	"Don't auto-jump to warnings at all", &editor.settings.build.NoJumpToWarningAtAll, &editor.settings.build.JumpToErrors, 0,
	
	 NULL, NULL, NULL, 0
};



PrefsWindow::PrefsWindow()
	: SubWindow(BRect(0, 0, 520, 350), "Settings")
{
	BRect rc(Bounds());
	rc.InsetBy(1, 1);
	rc.right = 128;
	
	// allow only one
	if (CurrentPrefsWindow)
	{
		CurrentPrefsWindow->LockLooper();
		CurrentPrefsWindow->Quit();
	}
	CurrentPrefsWindow = this;
	
	//SetFeel(B_FLOATING_APP_WINDOW_FEEL);
	//SetFlags(Flags() | B_WILL_ACCEPT_FIRST_CLICK);
	
	// create list view
	fListView = new BListView(rc, "prefslist", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	fListView->SetSelectionMessage(new BMessage(M_PREFS_SELECT));
	
	// create scrollview
	fScrollView = new BScrollView("scrollview", fListView, B_FOLLOW_ALL, 0, false, true);
	bgview->AddChild(fScrollView);
	
	// container view
	rc = Bounds();
	rc.left = fScrollView->Bounds().right + 1;
	rc.bottom -= 38;
	fContainer = new BView(rc, "Container", B_FOLLOW_LEFT_RIGHT, 0);
	fContainer->SetViewColor(bgview->ViewColor());
	bgview->AddChild(fContainer);
	
	// buttons
	rc = Bounds();
	rc.InsetBy(8, 8);
	rc.left = rc.right - 100;
	rc.top = rc.bottom - 24;
	bgview->AddChild(new BButton(rc, "", "OK", new BMessage(M_OK_CLICKED)));
	
	rc.OffsetBy(-110, 0);
	/*rc.left = (fContainer->Bounds().Width() / 2);
	rc.left += fScrollView->Bounds().right;
	rc.right = rc.left;
	rc.left -= 50;
	rc.right += 50;*/
	cmdRevert = new BButton(rc, "", "Revert", new BMessage(M_REVERT_CLICKED));
	bgview->AddChild(cmdRevert);
	
	// initilize "Revert"
	fRevertBuffer = (char *)smal(sizeof(editor.settings));
	memcpy(fRevertBuffer, &editor.settings, sizeof(editor.settings));
	cmdRevert->SetEnabled(false);
	
	// add all preflets to list
	AddPreflet(" About", new AboutPreflet(this));
	AddPreflet(" Editor", new CheckboxPreflet(this, GeneralPanelData));
	AddPreflet(" Font/Colors", new ColorsPreflet(this));
	AddPreflet(" Shortcuts", new ShortcutsPreflet(this));
	AddPreflet(" Stats", new StatsPreflet(this));
	AddPreflet(" Build", new CheckboxPreflet(this, BuildPanelData));
	AddPreflet(" Misc", new CheckboxPreflet(this, MiscPanelData));
	
	fContainerContents = NULL;
	
	int index = settings->GetInt("LastSelectedPrefsPanel", 0);
	if (index < 0 && index > 4) index = 0;
	fListView->Select(index);
	
	Show();
}

void PrefsWindow::AddPreflet(const char *name, BView *preflet)
{
	fListView->AddItem(new BStringItem(name));
	fViewList.AddItem(preflet);
}

// notification from a preflet that settings have changed.
//
// redraw the document and update the enabled status of the Revert button,
// based on whether there are any changes to revert
void PrefsWindow::SettingsChanged()
{
	editor.curev->FullRedrawView();
	
	int revert_avail = 0;
	
	if (fContainerContents)
		revert_avail = fContainerContents->HaveSpecialRevert();
	
	if (!revert_avail)
		revert_avail = memcmp(fRevertBuffer, &editor.settings, sizeof(editor.settings));
	
	cmdRevert->SetEnabled(revert_avail ? B_CONTROL_ON : B_CONTROL_OFF);
}


PrefsWindow::~PrefsWindow()
{
	if (fContainerContents)
	{
		fContainerContents->PrefletClosing();
		
		fContainer->RemoveChild(fContainerContents);
		fContainerContents = NULL;
	}
	
	// clean up all preflet views
	int i, count = fViewList.CountItems();
	for(i=0;i<count;i++)
		delete (BView *)fViewList.ItemAt(i);
	fViewList.MakeEmpty();
	
	frees(fRevertBuffer);
	fRevertBuffer = NULL;
	
	if (this == CurrentPrefsWindow)
		CurrentPrefsWindow = NULL;
}


bool IsPrefsWindowOpenAndActive()
{
	return (CurrentPrefsWindow != NULL) && CurrentPrefsWindow->IsActive();
}

/*
void c------------------------------() {}
*/


void PrefsWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case M_PREFS_SELECT:
		{
			int32 index;
			
			if (message->FindInt32("index", &index) == B_OK)
			{
				if (fContainerContents)
				{
					fContainerContents->PrefletClosing();
					fContainer->RemoveChild(fContainerContents);
				}
				
				fContainerContents = (Preflet *)fViewList.ItemAt(index);
				
				if (fContainerContents)
				{
					fContainerContents->ReloadSettings();
					fContainer->AddChild(fContainerContents);
				}
				
				settings->SetInt("LastSelectedPrefsPanel", index);
			}
		}
		break;
		
		case M_OK_CLICKED:
		{
			//Config::save(settings);
			Quit();
		}
		break;
		
		case M_REVERT_CLICKED:
		{
			if (fRevertBuffer)
			{
				memcpy(&editor.settings, fRevertBuffer, sizeof(editor.settings));
				
				if (fContainerContents)
				{
					fContainerContents->DoSpecialRevert();
					fContainerContents->ReloadSettings();
				}
				
				cmdRevert->SetEnabled(false);
				editor.curev->FullRedrawView();
			}
		}
		break;
		
		default:
			BWindow::MessageReceived(message);
		break;
	}
}




