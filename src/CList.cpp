
#include <OS.h>
#include <List.h>
#include "CList.h"

CList::CList() { }

CList::CList(void *FreeVia)
{
	FreeFunc = (void (*)(void *))FreeVia;
}

void *CList::pop()
{
int lastitem = CountItems() - 1;
	if (lastitem < 0) return NULL;
	
	return RemoveItem(lastitem);
}

void CList::free()
{
	if (FreeFunc)
	{
		int nitems = CountItems();
		for(int i=0;i<nitems;i++)
		{
			void *item = ItemAt(i);
			(*FreeFunc)(item);
		}
	}
	
	MakeEmpty();
}
