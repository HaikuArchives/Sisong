
#include "MessageView.h"
#include "MessageView.fdh"

/*
	a BView that creates it's own BLooper for receiving messages.
	this allows the BView to SetTarget it's controls before the BView is added
	to a window (otherwise, the SetTarget call will fail).
*/


MessageView::MessageView(BRect frame, const char *name, uint32 resizingMode, uint32 flags)
	: BView(frame, name, resizingMode, flags)
{
	_init();
}


MessageView::MessageView(BMessage *archive)
	: BView(archive)
{
	_init();
}


void MessageView::_init()
{
	fLooper = new MessageViewLooper(this);
	fLooper->Run();
	
	fMessenger = new BMessenger(NULL, fLooper);
}


MessageView::~MessageView()
{
	if (fMessenger) {
		delete fMessenger;
		fMessenger = NULL;
	}
	
	if (fLooper) {
		fLooper->Lock();
		fLooper->Quit();
		fLooper = NULL;
	}
}

/*
void c------------------------------() {}
*/

BLooper *MessageView::Looper()
{
	return fLooper;
}

BMessenger *MessageView::Messenger()
{
	return fMessenger;
}

/*
void c------------------------------() {}
*/

MessageViewLooper::MessageViewLooper(MessageView *assoc_view)
	: BLooper(),
	  fAssocView(assoc_view)
{ }


void MessageViewLooper::DispatchMessage(BMessage *msg, BHandler *target)
{
	if (fAssocView && fAssocView->LockLooper()) {
		fAssocView->MessageReceived(msg);
		fAssocView->UnlockLooper();
	}
	
	BLooper::DispatchMessage(msg, target);
}



