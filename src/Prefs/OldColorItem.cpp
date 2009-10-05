
#include <ListItem.h>
#include <GraphicsDefs.h>
#include <Message.h>
#include <View.h>

#include <string.h>
#include <stdlib.h>

#include "ColorItem.h"
#include "../../common/basics.h"
#include "../colors.h"

#include "ColorItem.fdh"

#define COLOR_ITEM_HEIGHT		32


ColorItem::ColorItem(const char *text, \
					rgb_color fg, rgb_color bg, bool boldstate, char applic)
	: BListItem(0, true),
	fText(NULL),
	fBaselineOffset(0),
	fgColor(fg),
	bgColor(bg),
	fBoldState(boldstate),
	fApplic(applic)
{
	fText = strdup(text);
}

ColorItem::~ColorItem()
{
	free(fText);
}


/*
void c------------------------------() {}
*/

void ColorItem::DrawItem(BView *owner, BRect frame, bool complete)
{
rgb_color oldHighColor = owner->HighColor();
rgb_color oldLowColor = owner->LowColor();
const static rgb_color color_black = { 0, 0, 0 };
	owner->SetHighColor(!IsSelected() ? color_black : owner->ViewColor());
	owner->SetLowColor(IsSelected() ? color_black : owner->ViewColor());
	
	//if (IsSelected() || complete)
		owner->FillRect(frame, B_SOLID_LOW);

	owner->MovePenTo(frame.left+8, frame.top+(fBaselineOffset-1));
	owner->DrawString(fText);

	// draw swatches
	BRect rc(frame);
	rc.left = rc.right - HEIGHTOF(frame);
	rc.OffsetBy(-23, -1);
	rc.InsetBy(5, 4);
	
	if (fApplic & BG_APPLICABLE)
	{
		owner->SetHighColor(0, 0, 0);
		owner->StrokeRect(rc);
		
		owner->SetHighColor(bgColor);
		rc.InsetBy(1, 1);
		owner->FillRect(rc);
		rc.InsetBy(-1, -1);
	}
	
	if (fApplic & FG_APPLICABLE)
	{
		rc.OffsetBy(-32, 0);
		
		owner->SetHighColor(0, 0, 0);
		owner->StrokeRect(rc);
		owner->SetHighColor(fgColor);
		rc.InsetBy(1, 1);
		owner->FillRect(rc);
	}

	owner->SetHighColor(oldHighColor);
	owner->SetLowColor(oldLowColor);
}


void ColorItem::Update(BView *owner, const BFont *font)
{
	if (fText)
		SetWidth(font->StringWidth(fText));

	font_height fheight;
	font->GetHeight(&fheight);

	float height = ceilf(fheight.ascent) + ceilf(fheight.descent)
		+ ceilf(fheight.leading) + 1;

	if (height < COLOR_ITEM_HEIGHT) height = COLOR_ITEM_HEIGHT;
	fBaselineOffset = (height / 2) + fheight.descent + 1;
	
	SetHeight(height);
}




