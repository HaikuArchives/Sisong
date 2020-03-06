
#include <stdio.h>

#include <Application.h>
#include <PopUpMenu.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <MenuBar.h>
#include <View.h>
#include <Bitmap.h>
#include <Path.h>
#include <FindDirectory.h>
#include <Volume.h>
#include <VolumeRoster.h>

#include "../../common/basics.h"

#include "DirMenuItem.h"
#include "DirMenuItem.fdh"

#include <stdlib.h>


DirMenuItem::DirMenuItem(const char *text, BBitmap *icon, BDirectory *dir, \
					BMessage *msg, char shortcut, uint32 modifiers)
	: BMenuItem(text, msg, shortcut, modifiers),
	  fIcon(icon),
	  fHeightDelta(0)
{
}

DirMenuItem::DirMenuItem(BMenu *submenu)
	: BMenuItem(submenu, NULL),
	  fIcon(NULL),
	  fHeightDelta(0)
{
}


DirMenuItem::~DirMenuItem()
{
	if (fIcon)
		delete fIcon;
}


/*
void c------------------------------() {}
*/

void DirMenuItem::SetIcon(BBitmap *newIcon)
{
	if (fIcon)
		delete fIcon;
	
	fIcon = newIcon;
}

void DirMenuItem::GetIcon(BBitmap *iconOut)
{
	if (fIcon)
		*iconOut = *fIcon;
}


void DirMenuItem::GetContentSize(float *width, float *height)
{
	BMenuItem::GetContentSize(width, height);

	fHeightDelta = 15 - *height;
	if (*height < 15)
		*height = 15;

	*width += 28;
}


void DirMenuItem::DrawContent()
{
	BPoint drawPoint(ContentLocation());
	drawPoint.x += 20;
	if (fHeightDelta > 0)
		drawPoint.y += ceil(fHeightDelta / 2);
	Menu()->MovePenTo(drawPoint);
	BMenuItem::DrawContent();

	BPoint where(ContentLocation());
	float deltaHeight = fHeightDelta < 0 ? -fHeightDelta : 0;
	where.y += ceil(deltaHeight / 2) - 1;

	if (fIcon)
	{
		Menu()->PushState();
		
		BPoint where(ContentLocation());
		float deltaHeight = fHeightDelta < 0 ? -fHeightDelta : 0;
		where.y += ceil(deltaHeight / 2);
		
		if (IsEnabled())
			Menu()->SetDrawingMode(B_OP_ALPHA);
		else
		{
			Menu()->SetDrawingMode(B_OP_ALPHA);
			Menu()->SetHighColor(0, 0, 0, 64);
			Menu()->SetBlendingMode(B_CONSTANT_ALPHA, B_ALPHA_OVERLAY);
		}
		
		Menu()->DrawBitmapAsync(fIcon, where);
		Menu()->PopState();
	}
	
}





