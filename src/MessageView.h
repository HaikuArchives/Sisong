
#ifndef _MESSAGEVIEW_H
#define _MESSAGEVIEW_H

#include <View.h>
#include <Rect.h>
#include <Message.h>
#include <Messenger.h>
#include <Looper.h>

class MessageViewLooper;


class MessageView : public BView
{
public:
	MessageView(BRect frame, const char *name, uint32 resizingMode, uint32 flags);
	MessageView(BMessage *archive);
	virtual ~MessageView();
	
	virtual void MessageReceived(BMessage *msg)
	{ BView::MessageReceived(msg); }
	
	BLooper *Looper();
	BMessenger *Messenger();
	
	
private:
	MessageViewLooper *fLooper;
	BMessenger *fMessenger;
	
	void _init();
	
};


class MessageViewLooper : public BLooper
{
public:
	MessageViewLooper(MessageView *assoc_view);
	void DispatchMessage(BMessage *msg, BHandler *target);

private:
	MessageView *fAssocView;
};

#endif
