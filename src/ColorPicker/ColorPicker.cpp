
#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Message.h>
#include <Button.h>

#include "ColorPicker.h"

#define M_COLOR_CHANGED		'COLR'
#define M_OK				'OK!!'
#define M_REVERT			'REVT'

class Swatch : public BView
{
public:
	Swatch(BRect rc)
		: BView(rc, "", 0, B_WILL_DRAW)
	{ }
	
	void Draw(BRect updateRect)
	{
		BRect rc(Bounds());
		rc.InsetBy(1, 1);
		
		SetHighColor(color);
		FillRect(rc);
		
		SetHighColor(0, 0, 0);
		StrokeRect(Bounds());
	}
	
	void SetColor(rgb_color color)
	{
		this->color = color;
		Invalidate();
	}
	
private:
	rgb_color color;
};


ColorPickerWindow::ColorPickerWindow(BMessage *msg, BLooper *target, rgb_color initial_color)
	: SubWindow(BRect(0, 0, 500, 160), "Color Picker"),
	  fTarget(target),
	  msg_to_deliver(msg),
	  orgcolor(initial_color)
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	SetFlags(Flags() | B_NOT_RESIZABLE | B_NOT_MINIMIZABLE | B_NOT_ZOOMABLE);
	
	// create color control
	ColorCtrl = new BColorControl(BPoint(8, 8), B_CELLS_32x8, 12, "", new BMessage(M_COLOR_CHANGED));
	ColorCtrl->SetValue(initial_color);
	bgview->AddChild(ColorCtrl);
	
	// create swatch
	BRect rc(Bounds());
	rc.InsetBy(9, 9);
	rc.top = rc.bottom - 26;
	rc.right = rc.left + 12*32;
	rc.OffsetBy(0, -2);
	rc.right -= ((rc.right - rc.left)+1) / 2;
	swatch = new Swatch(rc);
	((Swatch *)swatch)->SetColor(initial_color);
	bgview->AddChild(swatch);
	
	BButton *btn;
	rc = Bounds();
	rc.OffsetBy(-10, -10);
	rc.left = rc.right - 96;
	rc.top = rc.bottom - 28;
	btn = new BButton(rc, "", "OK", new BMessage(M_OK));
	bgview->AddChild(btn);
	
	rc.OffsetBy(-106, 0);
	btn = new BButton(rc, "", "Revert", new BMessage(M_REVERT));
	bgview->AddChild(btn);	
	
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	Show();
}

ColorPickerWindow::~ColorPickerWindow()
{
	if (msg_to_deliver)
		delete msg_to_deliver;
}

void ColorPickerWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_COLOR_CHANGED:
		{
			rgb_color newColor = ColorCtrl->ValueAsColor();
			
			((Swatch *)swatch)->SetColor(newColor);
			
			if (fTarget && msg_to_deliver)
			{
				BMessage *msg = new BMessage(*msg_to_deliver);
				msg->AddInt8("red", newColor.red);
				msg->AddInt8("green", newColor.green);
				msg->AddInt8("blue", newColor.blue);
				
				fTarget->PostMessage(msg);
			}
		}
		break;
		
		case M_REVERT:
		{
			ColorCtrl->SetValue(orgcolor);
			BMessage msg = BMessage(M_COLOR_CHANGED);
			MessageReceived(&msg);
		}
		break;
		
		case M_OK:
			Quit();
		break;
		
		default:
			SubWindow::MessageReceived(msg);
	}	
}

