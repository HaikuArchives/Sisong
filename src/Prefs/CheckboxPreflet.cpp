
/*
	a preflet which shows a series of checkboxes which control boolean
	flags in editor.settings, used by several different panels.
*/
#include "Preflet.h"
#include "CheckboxPreflet.h"

CheckboxPreflet::CheckboxPreflet(PrefsWindow *parent, CheckboxPrefletData *data)
	: Preflet(parent)
{
int i;

	fData = data;
	fNumCheckBoxes = 0;
	BRect rc(16, 16, Bounds().Width() - 2, 24);
	
	for(i=0;data[i].text;i++)
	{
		if (data[i].text[0])
		{
			fCheckBox[i] = new BCheckBox(rc, "", data[i].text, new BMessage(1));
			
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

void CheckboxPreflet::PrefletOpening()
{
	fHaveAlertedAboutRestart = false;
}

void CheckboxPreflet::ReloadSettings()
{
int i;

	for(i=0;i<fNumCheckBoxes;i++)
	{
		if (fCheckBox[i])
		{
			fCheckBox[i]->SetValue(*fData[i].target);
			
			bool enable = true;
			if (fData[i].depends)
				enable = *fData[i].depends ^ fData[i].depends_xor;
			
			fCheckBox[i]->SetEnabled(enable);
		}
	}
}



void CheckboxPreflet::MessageReceived(BMessage *msg)
{
	if (msg->what == M_CHECKBOX_CLICKED)
	{
		int32 which;
		if (msg->FindInt32("which", &which) == B_OK)
		{
			bool value = fCheckBox[which]->Value();
			*fData[which].target = value;
			
			if (fData[which].flags & CPF_REQUIRES_RESTART)
			{
				if (!fHaveAlertedAboutRestart)
				{
					fHaveAlertedAboutRestart = true;
					(new BAlert("", "Please restart Sisong to apply this setting.", "OK"))->Go();
				}
			}
			
			fParent->SettingsChanged();
			ReloadSettings();
		}
	}
	
	MessageView::MessageReceived(msg);
}




