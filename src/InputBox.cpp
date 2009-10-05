
#include <stdio.h>
#include <stdlib.h>

#include <TextControl.h>
#include <Button.h>
#include <String.h>
#include <Window.h>
#include <View.h>

#include "../common/basics.h"

#include "InputBox.h"
#include "InputBox.fdh"

void CenterWindow(BWindow *parent, BWindow *child, bool at_bottom=false);

#define M_OK		'MOK!'
#define M_CANCEL	'MCAN'


BString *InputBox::Go(BWindow *parent, const char *title, const char *prompt, const char *initialValue)
{
InputBox *box;
BString *retval;

	box = new InputBox(parent, title, prompt, initialValue);
	
	stat("InputBox::Go(): entering snooze loop");
	for(;;)
	{
		snooze(10 * 1000);
		if (box->fBoxDone)
		{
			break;
		}
	}
	
	if (box->fCanceled)
		retval = NULL;
	else
	{
		retval = new BString(box->txtPrompt->Text());
	}
	
	box->LockLooper();
	box->Quit();
	return retval;
}


InputBox::InputBox(BWindow *parent, const char *title, const char *prompt, const char *initialValue)
	: BWindow(BRect(0, 0, 400, 80), title,
	B_MODAL_WINDOW, B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_NOT_MOVABLE|B_ASYNCHRONOUS_CONTROLS
					|B_WILL_ACCEPT_FIRST_CLICK)
{
	CenterWindow(parent, this);
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	backview = new BView(Bounds(), "", B_FOLLOW_ALL, 0);
	backview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(backview);
	
	txtPrompt = new BTextControl(BRect(40, 13, 380, 13+21), "", prompt, initialValue, NULL);
	txtPrompt->SetDivider(140);
	backview->AddChild(txtPrompt);
	
	BRect rc(Bounds());
	BButton *OKButton;
	
	rc.InsetBy(10, 10);
	rc.left = rc.right - 96;
	rc.top = rc.bottom - 24;
	OKButton = new BButton(rc, "", "OK", new BMessage(M_OK));
	backview->AddChild(OKButton);
	SetDefaultButton(OKButton);
	
	rc.right = rc.left - 10;
	rc.left = rc.right - 96;
	backview->AddChild(new BButton(rc, "", "Cancel", new BMessage(M_CANCEL)));
	
	txtPrompt->MakeFocus();
	
	fCanceled = true;		// assume yes, in case window is closed
	fBoxDone = false;
	Show();
}

void InputBox::DispatchMessage(BMessage *msg, BHandler *handler)
{
	switch(msg->what)
	{
		case B_KEY_DOWN:
		{
			const char *bytes;
			
			if (msg->FindString("bytes", &bytes) == B_OK)
			{
				if (*bytes == B_ESCAPE)
				{
					fCanceled = true;
					fBoxDone = true;
					Quit();
				}
			}
		}
		break;
	}
	
	BWindow::DispatchMessage(msg, handler);
}

void InputBox::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_OK:
			fCanceled = false;
			fBoxDone = true;
		break;
		
		case M_CANCEL:
			fCanceled = true;
			fBoxDone = true;
		break;
		
		default:
			BWindow::MessageReceived(msg);
		break;
	}
}

bool InputBox::QuitRequested()
{
	fBoxDone = true;
	return false;
}



