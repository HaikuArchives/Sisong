
#include "Preflet.h"
#include "MiscPreflet.h"

static struct
{
	const char *text;
	bool *setting_target;
}
boolean_settings[] =
{
	"Fix gaps in indentation when opening", &editor.settings.FixIndentationGaps,
	"Remove trailing whitespace when saving", &editor.settings.RemoveTrailingWhitespace,
	"", NULL,
	"Warn if code doesn't match Haiku Coding Guidelines", &editor.settings.WarnHaikuGuidelines,
	
	NULL, NULL
};
	

MiscPreflet::MiscPreflet(PrefsWindow *parent)
	: Preflet(parent)
{
int i;

	fNumCheckBoxes = 0;
	BRect rc(16, 16, Bounds().Width() - 2, 24);
	
	for(i=0;boolean_settings[i].text;i++)
	{
		if (boolean_settings[i].text[0])
		{
			fCheckBox[i] = new BCheckBox(rc, "", \
								boolean_settings[i].text, new BMessage(1));
			
			BMessage *msg = new BMessage(M_CHECKBOX_CLICKED);
			msg->AddInt32("which", i);
			
			fCheckBox[i]->SetMessage(msg);
			fCheckBox[i]->SetTarget(Looper());
			
			AddChild(fCheckBox[i]);
		}
		else
		{
			fCheckBox[i] = NULL;
		}
		
		rc.OffsetBy(0, 25);
		fNumCheckBoxes++;
	}
}

void MiscPreflet::ReloadSettings()
{
int i;
	for(i=0;i<fNumCheckBoxes;i++)
	{
		if (fCheckBox[i])
			fCheckBox[i]->SetValue(*boolean_settings[i].setting_target);
	}
}


void MiscPreflet::MessageReceived(BMessage *msg)
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




