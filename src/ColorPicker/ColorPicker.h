
#include <ColorControl.h>
#include "../SubWindow.h"

class ColorPickerWindow : public SubWindow
{
public:
	ColorPickerWindow(BMessage *msg, BLooper *target, rgb_color initial_color);
	~ColorPickerWindow();
	
	virtual void MessageReceived(BMessage *msg);
	
private:
	BLooper *fTarget;
	BMessage *msg_to_deliver;
	
	rgb_color orgcolor;
	
	BColorControl *ColorCtrl;
	BView *swatch;
};
