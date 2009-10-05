
#ifndef _SUBWINDOW_H
#define _SUBWINDOW_H

class SubWindow : public BWindow
{
public:
	SubWindow(BRect frame, const char *title);
	virtual void DispatchMessage(BMessage *message, BHandler *handler);
	
protected:
	BView *bgview;
};


#endif
