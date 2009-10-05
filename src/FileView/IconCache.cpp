
/*
	Caches file-type icons for MIME types.
	Need this because it takes some time to look up and copy the icon
	which otherwise makes directory navigation quite slow.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <File.h>
#include <NodeInfo.h>
#include <Mime.h>
#include <Bitmap.h>

#include "IconCache.h"


IconCache::IconCache()
{
	tree = QSInit();

	CacheAllocSize = INITIAL_CACHE_SIZE;
	nIconsCached = 0;
	
	icon_cache = (BBitmap **)malloc(INITIAL_CACHE_SIZE * sizeof(BBitmap *));
	
	// cache directory icon separately just to avoid the QSLookup's and for convenience,
	// this isn't actually necessary, you can look up a directory mime just like a file mime.
	fDirectoryIcon = new BBitmap(BRect(0, 0, 15, 15), B_RGBA32);
	
	BMimeType mime("application/x-vnd.Be-directory");
	mime.GetIcon(fDirectoryIcon, B_MINI_ICON);
}

IconCache::~IconCache()
{
	QSClose(tree);
	
	for(int i=0;i<nIconsCached;i++)
		delete icon_cache[i];
	
	free(icon_cache);
	delete fDirectoryIcon;
}

/*
void c------------------------------() {}
*/

// give it a MIME type, it returns a BBitmap containing the 16x16 icon
// for that type. Simple.
BBitmap *IconCache::GetIconForMimeType(const char *mimeType)
{
int index;
BBitmap *bitmap;

	// is it already cached?
	index = QSLookup(tree, mimeType);
	if (index != -1)
	{
		bitmap = icon_cache[index];
	}
	else
	{
		// fetch the icon
		bitmap = new BBitmap(BRect(0, 0, 15, 15), B_RGBA32);
		
		BMimeType mime(mimeType);
		mime.GetIcon(bitmap, B_MINI_ICON);
		
		// add the icon into the cache
		if (nIconsCached >= CacheAllocSize)
		{
			CacheAllocSize += INITIAL_CACHE_SIZE;
			icon_cache = (BBitmap **)realloc(icon_cache, CacheAllocSize * sizeof(BBitmap *));
		}
		
		icon_cache[nIconsCached] = bitmap;
		
		QSAddString(tree, mimeType, nIconsCached);
		nIconsCached++;
	}
	
	return bitmap;
}


// returns the default icon for the directory mime type, "application/x-vnd.Be-directory".
BBitmap *IconCache::GetDirectoryIcon()
{
	return fDirectoryIcon;
}




