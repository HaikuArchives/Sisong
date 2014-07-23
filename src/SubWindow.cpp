
#include <Window.h>
#include <View.h>
#include <Message.h>

#include "SubWindow.h"
#include "misc2.h"

extern BWindow *MainWindow;


SubWindow::SubWindow(BRect frame, const char *title)
	: BWindow(frame, title, B_TITLED_WINDOW, 
	B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE)
{
	CenterWindow(MainWindow, this);
	
	bgview = new BView(Bounds(), "background_view", B_FOLLOW_ALL, 0);
	bgview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(bgview);
}


void SubWindow::DispatchMessage(BMessage *msg, BHandler *handler)
{
	switch(msg->what)
	{
		case B_KEY_DOWN:
		{
			const char *bytes;
			if (msg->FindString("bytes", &bytes) == B_OK)
			{
				if (*bytes == B_ESCAPE)
					Quit();
			}
		}
		break;
	}
	
	BWindow::DispatchMessage(msg, handler);
}

/*
void c------------------------------() {}
*/





