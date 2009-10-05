
// based on original BStringItem code from Haiku project
// original is (c)Haiku Inc., released under MIT license
// modified DrawItem to draw the items in editor's color settings

#include "FileItem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Message.h>
#include <View.h>
#include <File.h>
#include <NodeInfo.h>
#include <Mime.h>

void maxcpy(char *dst, const char *src, int size);

static const rgb_color color_background = { 0xff, 0xff, 0xff };
static const rgb_color color_text 		= { 0x00, 0x00, 0x00 };

#define ICON_SIZE		16
#define BORDER_LEFT		20

#define MINHEIGHT		(ICON_SIZE+4)


FileItem::FileItem(const char *itemPath, IconCache *iconCache, bool is_directory)
	: BListItem(0, true),
	fText(NULL),
	fBaselineOffset(0)
{
	if (is_directory)
	{
		fIcon = iconCache->GetDirectoryIcon();
		strcpy(fFileSize, "-");
	}
	else
	{	// get mime type for file
		char mimeType[B_MIME_TYPE_LENGTH];
		BFile file;
		BNodeInfo nodeinfo;
		
		mimeType[0] = 0;
		
		if (file.SetTo(itemPath, B_READ_ONLY) == B_OK)
		{
			if (nodeinfo.SetTo(&file) == B_OK)
			{
				nodeinfo.GetType(mimeType);
			}
		}
		
		if (!mimeType[0])
			strcpy(mimeType, "text/plain");
		
		fIcon = iconCache->GetIconForMimeType(mimeType);
		
		SetSizeText(itemPath);
	}
	
	
	// set path and directory flag
	maxcpy(this->fItemPath, itemPath, sizeof(this->fItemPath));
	fIsDirectory = is_directory;

	// set text
	char *ptr = strrchr(itemPath, '/');
	if (ptr) itemPath = ptr+1;

	SetText(itemPath);
}


FileItem::~FileItem()
{
	free(fText);
}


/*
void c------------------------------() {}
*/

static void DimmedIconBlitter(BView *view, BBitmap *bitmap, BPoint where)
{
	if (bitmap->ColorSpace() == B_RGBA32) {
		rgb_color oldHighColor = view->HighColor();
		view->SetHighColor(255, 0, 0, 128);
		view->SetLowColor(255,0,0);
		view->SetDrawingMode(B_OP_ALPHA);
		view->SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
		view->DrawBitmap(bitmap, where);
		view->SetHighColor(oldHighColor);
	} else {
		view->SetDrawingMode(B_OP_BLEND);
		view->DrawBitmap(bitmap, where);
	}
	view->SetDrawingMode(B_OP_COPY);
}

void FileItem::DrawItem(BView *owner, BRect frame, bool complete)
{
rgb_color oldHighColor = owner->HighColor();
rgb_color oldLowColor = owner->LowColor();


	if (complete)
	{
		owner->SetLowColor(color_background);
		owner->FillRect(frame, B_SOLID_LOW);
	}

	BPoint iconPos(frame.left+BORDER_LEFT, frame.top+3);
	
	if (!IsSelected())
	{
		// draw icon
		owner->SetDrawingMode(B_OP_OVER);
		owner->DrawBitmap(fIcon, iconPos);
		owner->SetDrawingMode(B_OP_COPY);
		
		owner->SetHighColor(color_text);
		owner->SetLowColor(color_background);
	}
	else
	{
		// draw dimmed icon
		DimmedIconBlitter(owner, fIcon, iconPos);
		
		owner->SetHighColor(color_background);
		owner->SetLowColor(color_text);
	}

	// draw text and selection rect if present
	int x = (int)(frame.left+BORDER_LEFT+6+ICON_SIZE);
	int y = (int)(frame.top+fBaselineOffset+1);
	
	BFont font;
	owner->GetFont(&font);
	
	owner->FillRect(BRect(x-1, frame.top+3, x+font.StringWidth(fText)+1, frame.bottom-1), B_SOLID_LOW);

	owner->MovePenTo(x, y);
	owner->DrawString(fText);

	// draw file size
	if (IsSelected())
	{
		owner->SetHighColor(color_text);
		owner->SetLowColor(color_background);
	}
	
	owner->MovePenTo(frame.right - SIZE_TAB_OFFS, y);
	owner->DrawString(fFileSize);
	

	owner->SetHighColor(oldHighColor);
	owner->SetLowColor(oldLowColor);
}



void FileItem::SetText(const char *text)
{
	if (fText) free(fText);
	
	if (text)
		fText = strdup(text);
	else
		fText = NULL;
}


const char *FileItem::Text() const
{
	return fText;
}


void FileItem::Update(BView *owner, const BFont *font)
{
	if (fText)
		SetWidth(font->StringWidth(fText));

	font_height fheight;
	font->GetHeight(&fheight);

	fBaselineOffset = 2 + ceilf(fheight.ascent + fheight.leading / 2);

	float height = ceilf(fheight.ascent) + ceilf(fheight.descent)
		+ ceilf(fheight.leading) + 4;
	if (height < MINHEIGHT)
		height = MINHEIGHT;
	SetHeight(height);
}

/*
void c------------------------------() {}
*/


void FileItem::SetSizeText(const char *fname)
{
FILE *fp;
unsigned int fileSize;

	// get file size
	fp = fopen(fname, "rb");
	if (!fp)
	{
		strcpy(fFileSize, "-");
		return;
	}
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fclose(fp);
	
	#define GIGABYTE		1073741824
	#define MEGABYTE		1048576
	#define KILOBYTE		1024
	double fs = fileSize;
	
	if (fileSize >= GIGABYTE)
	{
		fs /= GIGABYTE;
		sprintf(fFileSize, "%.2f GB", fs);
	}
	else if (fileSize >= MEGABYTE)
	{
		fs /= MEGABYTE;
		sprintf(fFileSize, "%.2f MB", fs);
	}
	else if (fileSize >= KILOBYTE)
	{
		fs /= KILOBYTE;
		sprintf(fFileSize, "%.2f KB", fs);
	}
	else
	{
		sprintf(fFileSize, "%d bytes", fileSize);
	}
}



