
#ifndef _FILEVIEW_H
#define _FILEVIEW_H

#include "TitleView.h"
#include "IconCache.h"

class FileListView;
#define M_PATH_CHANGED		'PTHc'


class FileView : public BView
{
public:
	FileView(BRect frame, const char *initialDir=NULL, uint32 resizingMode=B_FOLLOW_ALL);
	
	void SetPath(const char *newPath);
	void GetPath(BPath *path_out);
	void Update();
	void SelectItem(const char *path);
	void ItemInvoked(const char *path, bool is_directory);

private:
	void Clear();
	void AddVolumeIcons();
	
	char fCurDir[MAXPATHLEN];
	char fFileFilter[MAXPATHLEN];
	
	FileListView *fListView;
	BScrollView *fScrollView;
	TitleView *fTitleView;
	
	IconCache fIconCache;
};



class FileListView : public BListView
{
public:
	FileListView(BRect frame, FileView *parent, uint32 resizingMode)
		: BListView(frame, "filelist", B_SINGLE_SELECTION_LIST, resizingMode),
		  fileview(parent)
	{ }
	
	virtual void MouseDown(BPoint where);
	
private:
	FileView *fileview;
};


#endif
