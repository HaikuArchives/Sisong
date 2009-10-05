
#include <stdio.h>

#include <Application.h>
#include <String.h>
#include <List.h>
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Directory.h>
#include <VolumeRoster.h>
#include <Volume.h>
#include <interface/Font.h>
#include <interface/ListView.h>
#include <interface/ScrollView.h>
#include <FindDirectory.h>
#include <Path.h>

#include "../../common/basics.h"

#include "FileItem.h"
#include "FileView.h"
#include "FileView.fdh"

#define TITLEVIEW_HEIGHT			17
void maxcpy(char *dst, const char *src, int size);


FileView::FileView(BRect frame, const char *initialDir, uint32 resizingMode)
	: BView(frame, "FileView", resizingMode, B_FULL_UPDATE_ON_RESIZE)
{
	// create list
	BRect lvrect(Bounds());
	lvrect.InsetBy(1, 1);
	lvrect.right -= B_V_SCROLL_BAR_WIDTH;
	lvrect.top += TITLEVIEW_HEIGHT;

	fListView = new FileListView(lvrect, this, B_FOLLOW_ALL);
	fListView->SetFont(be_plain_font);
	
	// create scrollview
	fScrollView = new BScrollView("scrollview", fListView, B_FOLLOW_ALL, 0, false, true);
	AddChild(fScrollView);
	
	// create titleview
	fTitleView = new TitleView(BRect(0, 0, Bounds().right, TITLEVIEW_HEIGHT-1), B_FOLLOW_LEFT_RIGHT);
	AddChild(fTitleView);
	
	
	if (!initialDir)
		getcwd(fCurDir, sizeof(fCurDir));
	else
		maxcpy(fCurDir, initialDir, sizeof(fCurDir));
	
	strcpy(fFileFilter, "*");
	Update();
}

void FileView::SetPath(const char *newPath)
{
	maxcpy(fCurDir, newPath, sizeof(fCurDir)-1);
	Update();
}

void FileView::GetPath(BPath *path_out)
{
	path_out->SetTo(fCurDir);
}

void FileView::SelectItem(const char *path)
{
int i, count;
	
	count = fListView->CountItems();
	for(i=0;i<count;i++)
	{
		FileItem *item = (FileItem *)fListView->ItemAt(i);
		
		if (!strcmp(item->fItemPath, path))
		{
			fListView->Select(i);
			fListView->ScrollToSelection();
			fListView->MakeFocus();
			break;
		}
	}
}

void FileView::Update()
{
BList files, dirs;
int i, count;
char *itemPath;

	stat("updating from path %s", fCurDir);
	Clear();
	
	// if this is the desktop folder, add in the volumes at the top
	char desktop_path[MAXPATHLEN];
	find_directory(B_DESKTOP_DIRECTORY, 0, true, desktop_path, sizeof(desktop_path));
	if (!strcmp(fCurDir, desktop_path))
	{
		AddVolumeIcons();
	}

	if (GetDirectoryContents(fCurDir, fFileFilter, &files, &dirs))
	{
		FreeBList(&files);
		FreeBList(&dirs);
		return;
	}
	
	// add dirs
	count = dirs.CountItems();
	for(i=0;i<count;i++)
	{
		itemPath = (char *)dirs.ItemAt(i);
		fListView->AddItem(new FileItem(itemPath, &fIconCache, true));
	}
	
	// add files
	count = files.CountItems();
	for(i=0;i<count;i++)
	{
		itemPath = (char *)files.ItemAt(i);
		fListView->AddItem(new FileItem(itemPath, &fIconCache, false));
	}
	
	
	FreeBList(&files);
	FreeBList(&dirs);
}

void FileView::AddVolumeIcons()
{
BVolumeRoster vroster;
BVolume volume;
BDirectory root;
BEntry eroot;
BPath proot;
char name[MAXPATHLEN];

	while(vroster.GetNextVolume(&volume) == B_OK)
	{
		volume.GetName(name);
		if (name[0])
		{
			volume.GetRootDirectory(&root);
			root.GetEntry(&eroot);
			eroot.GetPath(&proot);
			
			fListView->AddItem(new FileItem(proot.Path(), &fIconCache, true));
		}
	}
}


void FileView::Clear()
{
int i, count = fListView->CountItems();

	for(i=0;i<count;i++)
		delete fListView->ItemAt(i);
	
	fListView->MakeEmpty();
}


void FileView::ItemInvoked(const char *path, bool is_directory)
{	
	if (is_directory)
	{
		BMessage *msg = new BMessage(M_PATH_CHANGED);
		msg->AddString("path", path);
		
		Window()->PostMessage(msg);
		//maxcpy(fCurDir, path, sizeof(fCurDir));
		//Update();
	}
}

/*
void c------------------------------() {}
*/

void FileListView::MouseDown(BPoint where)
{
static bigtime_t lasttime = 0;
static int32 lastindex = -1;
int index = -1;
bigtime_t now;

	BListView::MouseDown(where);
	
	// get index
	int i, count = CountItems();
	for(i=0;i<count;i++)
	{
		if ((FileItem *)ItemAt(i)->IsSelected())
		{
			index = i; break;
		}
	}
	
	if (index != lastindex)
		lasttime = 0;	// sabatoge double click if item no. changed
	
	// check for double click; chintzy
	now = system_time();
	if (now - lasttime < 265000)
	{
		FileItem *item = (FileItem *)ItemAt(index);
		if (item)
			fileview->ItemInvoked(item->fItemPath, item->fIsDirectory);
	}

	lastindex = index;
	lasttime = now;
}




