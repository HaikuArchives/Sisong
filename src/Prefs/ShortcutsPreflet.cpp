
#include "Preflet.h"
#include <PopUpMenu.h>
#include <MenuField.h>

#include "ShortcutsPreflet.h"
#include "ShortcutsPreflet.fdh"

#define M_MENUITEM_CHANGED		'MIch'


ShortcutsPreflet::ShortcutsPreflet(PrefsWindow *parent)
	: Preflet(parent),
	  fParent(parent)
{
int i;
BRect rc;

	rc.top = 10;
	
	// compute maximum spacing we can have and still fit all the keys in
	float avail_space = HEIGHTOF(Bounds()) - (rc.top * 2);
	int spacing = (int)(avail_space / NUM_F_KEYS);
	
	rc.bottom = rc.top + (spacing - 1);

	// generate a configurator for each F key
	for(i=0;i<NUM_F_KEYS;i++)
	{
		rc.left = 8+8;
		rc.right = 32+8;
		
		char text[8];
		sprintf(text, "F%d", i+1);
		BStringView *sv = new BStringView(rc, "", text);
		sv->SetAlignment(B_ALIGN_RIGHT);
		AddChild(sv);
		
		rc.left = rc.right + 17;
		rc.right = Bounds().right - 60;
		
		fMenu[i] = new BPopUpMenu(text);
		
		fMenuField[i] = new BMenuField(rc, "fkey config", NULL, fMenu[i], true);
		AddChild(fMenuField[i]);
		
		rc.OffsetBy(0, spacing);
	}
	
}


// calls "func()" repeatedly, passing it each menu item in turn from the menu bar.
// it passes a NULL item before each menu change. it removes seperators from the
// menus. if func() returns false the iteration stops.
void ShortcutsPreflet::DoForEachMenuItem(bool (*func)(BMenuItem *item, void *data1, void *data2, void *data3), void *data1, void *data2, void *data3)
{
BMenuBar *mainbar = MainWindow->top.menubar->bar;

	// iterate over items available on main menu and add as a possible shortcut
	int i, j, k, icount, jcount, kcount;
	
	icount = mainbar->CountItems();
	for(i=0;i<icount;i++)
	{
		// get "File", "Edit" etc
		BMenu *menua = mainbar->ItemAt(i)->Submenu();
		
		if (!func(NULL, data1, data2, data3))
			return;
		
		// iterate over each item in the submenu
		jcount = menua->CountItems();
		
		const char *CurrentMainMenu = mainbar->ItemAt(i)->Label();
		bool IsSettingsMenu = (strcmp(CurrentMainMenu, "Settings") == 0);
		if (IsSettingsMenu)
			jcount = 2;
		
		for(j=0;j<jcount;j++)
		{
			BMenuItem *item = menua->ItemAt(j);
			const char *label = item->Label();
			
			if (label[0])	// remove separators from the list
			{
				// if it has a submenu...
				BMenu *submenu2 = item->Submenu();
				if (submenu2)
				{
					// ... include Load/Save Layout items, but not items under Projects
					if (strcmp(CurrentMainMenu, "Projects") != 0)
					{
						kcount = submenu2->CountItems();
						for(k=0;k<kcount;k++)
						{
							BMenuItem *item = submenu2->ItemAt(k);
							
							if (!func(item, data1, data2, data3))
								return;
						}
					}
				}
				else
				{	// else, just pass it to delegate
					if (!func(item, data1, data2, data3))
						return;
				}
			}
		}
	}
}


/*
void c------------------------------() {}
*/

void ShortcutsPreflet::ReloadSettings()
{
int i, j, count;

	// clear all menus and repopulate
	for(i=0;i<NUM_F_KEYS;i++)
	{
		count = fMenu[i]->CountItems();
		for(j=count-1;j>=0;j--)
			delete fMenu[i]->RemoveItem(j);
		
		PopulateMenu(i);
	}
}

// generates the menu items for an f-key menu
void ShortcutsPreflet::PopulateMenu(int fkeyindex)
{
int32 counter = 0;

	// add the "do nothing" menu item
	BMenuItem *item = new BMenuItem("-", new BMessage(M_MENUITEM_CHANGED));
	item->SetTarget(Looper());
	fMenu[fkeyindex]->AddItem(item);
	
	// add all items
	DoForEachMenuItem(do_populate, this, (void *)(size_t)fkeyindex, &counter);
	
	// set the current setting active
	int activeindex = GetIndexForMessage(editor.settings.fkey_mapping[fkeyindex]);
	BMenuItem *active = fMenu[fkeyindex]->ItemAt(activeindex);
	if (active)
		active->SetMarked(true);
}

bool do_populate(BMenuItem *item, void *This, void *FKEYIndex, void *Counter)
{
ShortcutsPreflet *preflet = (ShortcutsPreflet *)This;
int fkeyindex = (int)(size_t)FKEYIndex;
int32 *counter = (int32 *)Counter;
BPopUpMenu *menu = preflet->fMenu[fkeyindex];
	
	if (item)
	{
		const char *label = item->Label();
		char buffer[800];
		
		// slight hacks to fixup names
		if (!strcmp(label, "New..."))
		{
			label = "New Project...";
		}
		else if (strbegin(label, "Position"))
		{
			strcpy(buffer, "Load ");
			strcpy(&buffer[5], label);
			label = buffer;
		}
		
		BMessage *msg = new BMessage(M_MENUITEM_CHANGED);
		msg->AddInt32("messageindex", *counter);
		msg->AddInt32("fkeyindex", fkeyindex);
		
		BMenuItem *newitem = new BMenuItem(label, msg);
		
		newitem->SetTarget(preflet->Looper());
		
		menu->AddItem(newitem);
	}
	else
	{
		menu->AddSeparatorItem();
	}

	(*counter)++;
	return true;
}

/*
void c------------------------------() {}
*/

// converts an index in the popups menus into a what code that triggers
// that menu action.
int ShortcutsPreflet::GetMessageForIndex(int messageindex)
{
int Counter = 0;
int Output = 0;

	if (messageindex == 0)	// "Does Nothing" option
	{
		return 0;
	}
	
	DoForEachMenuItem(do_msg2index, &Counter, (void *)(size_t)messageindex, &Output);
	return Output;
}

bool do_msg2index(BMenuItem *item, void *Counter, void *WantedIndex, void *Output)
{
int *counter = (int *)Counter;
int wanted_index = (int)(size_t)WantedIndex;
	
	if (*counter == wanted_index)
	{
		if (item)
		{
			BMessage *msg = item->Message();
			
			if (msg)
				*((int *)Output) = msg->what;
		}
		
		return false;
	}
	
	*counter = *counter + 1;
	return true;
}


// converts a what code into an index in the popups menus.
int ShortcutsPreflet::GetIndexForMessage(int what_code)
{
int Counter = 1;
int Output = 0;

	if (what_code == 0)	// "Does Nothing" option
	{
		return 0;
	}
	
	DoForEachMenuItem(do_index2msg, &Counter, (void *)(size_t)what_code, &Output);
	return Output;
}

bool do_index2msg(BMenuItem *item, void *Counter, void *WantedCode, void *Output)
{
int *counter = (int *)Counter;
int what_code = (int)(size_t)WantedCode;
	
	if (item && item->Message() && item->Message()->what == what_code)
	{
		*((int *)Output) = *counter;
		return false;
	}
	
	*counter = *counter + 1;
	return true;
}


/*
void c------------------------------() {}
*/

void ShortcutsPreflet::MessageReceived(BMessage *msg)
{
	if (msg->what == M_MENUITEM_CHANGED)
	{
		stat("MI_CH!");
		int32 fkeyindex = 0, messageindex = 0;
		
		msg->FindInt32("fkeyindex", &fkeyindex);
		msg->FindInt32("messageindex", &messageindex);
		
		// convert messageindex into an actual message
		int whatcode = GetMessageForIndex(messageindex);
		
		// set the function key to use that message
		if (fkeyindex >= 0 && fkeyindex < NUM_F_KEYS)
			editor.settings.fkey_mapping[fkeyindex] = whatcode;
		
		fParent->SettingsChanged();
	}
	else
	{
		Preflet::MessageReceived(msg);
	}
}



