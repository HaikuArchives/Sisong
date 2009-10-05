
#include "Preflet.h"
#include "GeneralPreflet.h"

struct
{
	const char *text;
	bool *setting_target;
}
boolean_settings[] =
{
	"Auto-indent on block open", &editor.settings.smart_indent_on_open,
	"Auto-indent on block close", &editor.settings.smart_indent_on_close,
	"Do not auto-indent from baselevel", &editor.settings.no_smart_open_at_baselevel,
	"Smart indenting for \"switch\" statement", &editor.settings.language_aware_indent,

	"Draw Tab Lines", &editor.settings.DrawTabLines,
	"Do Brace Matching", &editor.settings.DoBraceMatching,
	"Disable Lexer", &editor.settings.DisableLexer,
	"Use I-Beam cursor", &editor.settings.use_ibeam_cursor,
	//"Swap Control & Alt", &editor.settings.swap_ctrl_and_alt,
	"Show Build Help", &editor.settings.ShowBuildHelp,
	"Notify if update available", &editor.settings.CheckForUpdate,
	
	NULL, NULL
};
	
//	"Tab Width", &editor.settings.tab_width,
//	Use Syntax Highlighting with these extensions (* for all)	
	

GeneralPreflet::GeneralPreflet(PrefsWindow *parent)
	: Preflet(parent)
{
int i;
	
	BRect rc(16, 16, Bounds().Width() - 2, 24);
	
	for(i=0;boolean_settings[i].setting_target;i++)
	{
		fCheckBox[i] = new BCheckBox(rc, "", boolean_settings[i].text, new BMessage(1));
		
		BMessage *msg = new BMessage(M_CHECKBOX_CLICKED);
		msg->AddInt32("which", i);
		
		fCheckBox[i]->SetMessage(msg);
		fCheckBox[i]->SetTarget(Looper());
		
		AddChild(fCheckBox[i]);
		
		rc.OffsetBy(0, (i==3)?45:25);
		//else
		//	rc.OffsetBy(0, 35);
	}
}

void GeneralPreflet::ReloadSettings()
{
	for(int i=0;boolean_settings[i].setting_target;i++)
		fCheckBox[i]->SetValue(*boolean_settings[i].setting_target);
}


void GeneralPreflet::MessageReceived(BMessage *msg)
{
	if (msg->what == M_CHECKBOX_CLICKED)
	{
		int32 which;
		if (msg->FindInt32("which", &which) == B_OK)
		{
			bool value = fCheckBox[which]->Value();
			*boolean_settings[which].setting_target = value;
			
			fParent->SettingsChanged();
		}
	}
	
	MessageView::MessageReceived(msg);
}





