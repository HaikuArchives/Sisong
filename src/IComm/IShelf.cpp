
/*
	the new replicant shelf for "server IComm"
*/

#include "../editor.h"

#include "IComm.h"
#include "IShelf.h"
#include "IShelf.fdh"


IShelfView::IShelfView(BRect frame, uint32 resizingMode)
	: MessageView(frame, "IShelf", resizingMode, 0)
{
	// create the IShelf
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	fShelf = new IShelf(this);

	// create the BMessenger which can be used by the replicants
	// to talk back to us.
	fServerLink = new BMessenger(NULL, Looper());
	fServerLink->SendMessage(MIC_GET_CURRENT_LINE);
}

IShelfView::~IShelfView()
{
	delete fServerLink;
}


void IShelfView::MessageReceived(BMessage *msg)
{
EditView *ev = editor.curev;
BMessage reply;

	stat("IShelfView::MessageReceived %08x", msg->what);
	
	if (ev)
	{
		switch(msg->what)
		{
			case MIC_GET_CURRENT_LINE:
			{
				stat("IShelfView got MIC_GET_CURRENT_LINE");
				reply.AddInt32("reply", ev->cursor.y);
				msg->SendReply(&reply);
			}
			break;
			
			case MIC_GET_CURRENT_COL:
			{
				stat("IShelfView got MIC_GET_CURRENT_COL");
				reply.AddInt32("reply", ev->cursor.x);
				msg->SendReply(&reply);
			}
			break;
		}
	}
	else
	{
		stat(" -- ignoring message, because editor.curev is NULL");
	}
	
	BView::MessageReceived(msg);
}


/*
void c------------------------------() {}
*/

IShelf::IShelf(IShelfView *target)
	: BShelf(target),
	  fTarget(target)
{
	
}


bool IShelf::CanAcceptReplicantView(BRect destRect, BView *view, BMessage *archive) const
{
	stat("CARV!");
	// temporarily add the replicant as a child of the view
	fTarget->AddChild(view);
	
	stat("%08x", fTarget->fServerLink);
	
	// tell it who we are
	BMessenger msgr(view, NULL);
	BMessage initMessage(MIC_HELLO);
	initMessage.AddMessenger("ServerLink", *fTarget->fServerLink);
	
	msgr.SendMessage(&initMessage, (BHandler *)NULL, (bigtime_t)500);
	
	
	
	fTarget->RemoveChild(view);
	return true;
}

























