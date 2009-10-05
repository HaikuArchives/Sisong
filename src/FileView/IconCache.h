
#ifndef _ICONCACHE_H
#define _ICONCACHE_H

#include "QuickSearch.h"

#define INITIAL_CACHE_SIZE		32

class IconCache
{
public:
	IconCache();
	~IconCache();
	
	BBitmap *GetIconForMimeType(const char *mimeType);
	BBitmap *GetDirectoryIcon();
	
private:
	QSTree *tree;
	
	BBitmap *fDirectoryIcon;
	BBitmap **icon_cache;
	int nIconsCached;
	int CacheAllocSize;
};

#endif
