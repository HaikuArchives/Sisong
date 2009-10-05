
#ifndef _FILEITEM_H
#define _FILEITEM_H

#include <ListItem.h>
#include <GraphicsDefs.h>
#include <Bitmap.h>

#include "IconCache.h"

class FileItem : public BListItem
{
public:
	FileItem(const char *itemPath, IconCache *iconCache, bool is_directory);
	~FileItem();

	virtual	void DrawItem(BView *owner, BRect frame, bool complete=false);
	virtual	void SetText(const char* text);
	const char *Text() const;
	
	virtual	void Update(BView* owner, const BFont* font);
	
	char fItemPath[MAXPATHLEN];
	bool fIsDirectory;
	
private:
	void SetSizeText(const char *fname);
	
	char *fText;
	float fBaselineOffset;
	BBitmap *fIcon;
	
	char fFileSize[80];
};

#define SIZE_TAB_OFFS	78

#endif
