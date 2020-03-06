
#include <stdio.h>
#include <string.h>

#include <Application.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <MenuBar.h>
#include <View.h>
#include <Bitmap.h>
#include <Path.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <Volume.h>
#include <VolumeRoster.h>
#include <Node.h>
#include <NodeInfo.h>
#include <ListView.h>

#include "../../common/basics.h"

#include "DirMenuItem.h"
#include "DirMenu.h"
#include "../FileView/IconCache.h"
#include "../FileView/FileView.h"

#include "DirMenu.fdh"

DirMenu::DirMenu(BRect frame, BPath *initialPath, uint32 resizingMode)
	: BView(frame, "DirMenu", 0, resizingMode)
{
	if (initialPath)
		fCurPath = new BPath(*initialPath);
	else
	{
		char cp[MAXPATHLEN];
		getcwd(cp, sizeof(cp));
		fCurPath = new BPath(cp);
	}
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	fMenu = new BPopUpMenu("directories");
	
	fMenuField = new BMenuField(Bounds(), "DirMenuField", "", fMenu);
	fMenuField->MenuBar()->SetFont(be_plain_font);
	fMenuField->SetDivider(0);
	
	//fMenuField->MenuBar()->RemoveItem((int32)0);
	fMenuBar = fMenuField->MenuBar();
	
	// replace the default item the MenuField's menubar with a DirMenuItem
	// so that the drive icon is visible even when the dirmenu is closed.
	fSuperMenu = new DirMenuItem(fMenu);
	
	fMenuBar->RemoveItem((int32)0);
	fMenuBar->AddItem(fSuperMenu);

	// populate
	Populate();	
	AddChild(fMenuField);
}

DirMenu::~DirMenu()
{
	delete fCurPath;
}

/*
void c------------------------------() {}
*/

// attempts to determine which BVolume has "path" as it's root directory
bool VolumeFromPath(BVolume *vol_out, const char *path)
{
BVolumeRoster vroster;
BVolume volume;
BDirectory root;
BEntry eroot;
BPath proot;

	while(vroster.GetNextVolume(&volume) == B_OK)
	{
		volume.GetRootDirectory(&root);
		root.GetEntry(&eroot);
		eroot.GetPath(&proot);
		
		if (!strcmp(proot.Path(), path))
		{
			*vol_out = volume;
			return true;
		}
	}
	
	return false;
}


// remove all old entries from menu
void DirMenu::Clear()
{
int i, count = fMenu->CountItems();
	for(i=count-1;i>=0;i--)
	{
		delete fMenu->RemoveItem(i);
	}
}

// add an item at the beginning of the menu
void DirMenu::AddItem(const char *path, const char *text)
{
BBitmap *icon = new BBitmap(BRect(0, 0, 15, 15), B_RGBA32);
BDirectory dir = BDirectory(path);

	// for root directories show drive volume icon and change text to show volume name
	if (dir.IsRootDirectory())
	{
		BVolume volume;
		char volname[MAXPATHLEN];
		
		if (VolumeFromPath(&volume, path))
		{
			volume.GetIcon(icon, B_MINI_ICON);
			volume.GetName(volname);
			text = volname;
		}
	}
	else
	{
		// get tracker icon used for this folder
		BNode node(path);
		BNodeInfo ninfo(&node);
		ninfo.GetTrackerIcon(icon, B_MINI_ICON);
	}
	
	BMessage *msg = new BMessage(M_PATH_CHANGED);
	msg->AddString("path", path);
	
	fMenu->AddItem(new DirMenuItem(text, icon, &dir, msg), 0);
}

void DirMenu::Populate()
{
	Clear();

	// get desktop path
	char desktopPath[MAXPATHLEN];
	find_directory(B_DESKTOP_DIRECTORY, 0, true, desktopPath, sizeof(desktopPath));
	
	// walk up path and add each entry...unless we are at desktop
	if (strcmp(fCurPath->Path(), desktopPath) != 0)
	{
		BPath path(*fCurPath);
		for(;;)
		{
			const char *name = strrchr(path.Path(), '/');
			if (name) name++; else name = path.Path();
			
			AddItem(path.Path(), name);
			
			if (path.GetParent(&path) != B_OK || !strcmp(path.Path(), "/"))
				break;
		}
	}
	
	// add desktop item
	AddItem(desktopPath, "Desktop");
	
	// select last item
	DirMenuItem *lastItem = (DirMenuItem *)fMenu->ItemAt(fMenu->CountItems() - 1);
	if (lastItem)
	{
		lastItem->SetMarked(true);
	}
	
	// set the icon of the supermenu (shown when menu is closed)
	// to match that of the selected item
	BBitmap *super_icon = new BBitmap(BRect(0, 0, 15, 15), B_RGBA32);
	lastItem->GetIcon(super_icon);
	fSuperMenu->SetIcon(super_icon);
}


void DirMenu::SetPath(const char *path)
{
	fCurPath->SetTo(path);
	Populate();
}


