
#ifndef _QLISTVIEW_H
#define _QLISTVIEW_H

#include <ListView.h>
#include <ListItem.h>
#include <ScrollView.h>
#include "MessageView.h"

/*
	quick listview setter-up with scrollbar
*/

class QListView : public MessageView
{
public:
	QListView(BRect frame, uint32 resizingMode)
		: MessageView(frame, "QListView", resizingMode, 0)
	{
		// create list
		BRect rc(Bounds());
		rc.InsetBy(2, 2);
		rc.right -= B_V_SCROLL_BAR_WIDTH;
		
		fListView = new BListView(rc, "list", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
		
		// create scrollview
		fScrollView = new BScrollView("sv", fListView, B_FOLLOW_ALL, 0, false, true);
		AddChild(fScrollView);
	}

	inline BListView *ListView() { return fListView; }
	inline BScrollView *ScrollView() { return fScrollView; }

	inline bool AddItem(BListItem *item) { return fListView->AddItem(item); }
	inline bool AddItem(BListItem *item, int index) { return fListView->AddItem(item, index); }
	inline BListItem *ItemAt(int index) { return fListView->ItemAt(index); }
	inline void RemoveItem(int index) { fListView->RemoveItem(index); }
	inline void MakeEmpty() { fListView->MakeEmpty(); }

	void Clear()
	{
		int i, count = fListView->CountItems();
		for(i=0;i<count;i++)
			delete fListView->ItemAt(i);
		fListView->MakeEmpty();
	}
	//inline void SetInvocationMessage(

private:
	BListView *fListView;
	BScrollView *fScrollView;
};

#endif
