
#ifndef _ISHELF_H
#define _ISHELF_H

#include <Shelf.h>

class IShelfView;

class IShelf : public BShelf
{
public:
	IShelf(IShelfView *target);
	bool CanAcceptReplicantView(BRect destRect, BView *view, BMessage *archive) const;

private:
	IShelfView *fTarget;
	
};


// this is what you actually instantiated
class IShelfView : public MessageView
{
public:
	IShelfView(BRect frame, uint32 resizingMode);
	~IShelfView();
	
	void InitAfterAdoption();
	void MessageReceived(BMessage *msg);


private:
	friend class IShelf;
	
	BShelf *fShelf;
	BMessenger *fServerLink;

};

#endif
