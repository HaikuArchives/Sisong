
// based on original BStringItem code from Haiku project
// original is (c)Haiku Inc., released under MIT license
// modified DrawItem to draw the items in editor's color settings

#include "ColoredStringItem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Message.h>
#include <View.h>

#include "colors.h"
#include "ColoredStringItem.fdh"


ColoredStringItem::ColoredStringItem(const char *text, \
							rgb_color fg, rgb_color bg, rgb_color bg_selected)
	: BListItem(0, true),
	fText(NULL),
	fBaselineOffset(0)
{	
	this->fg = fg;
	this->bg = bg;
	this->bg_selected = bg_selected;
	
	SetText(text);
}


ColoredStringItem::~ColoredStringItem()
{
	free(fText);
}


/*
void c------------------------------() {}
*/

void ColoredStringItem::DrawItem(BView *owner, BRect frame, bool complete)
{
	if (fText == NULL)
		return;

	rgb_color oldHighColor = owner->HighColor();
	rgb_color oldLowColor = owner->LowColor();

	owner->SetLowColor(IsSelected() ? bg_selected : bg);
	owner->SetHighColor(fg);
	
	if (IsSelected() || complete)
		owner->FillRect(frame, B_SOLID_LOW);

	owner->MovePenTo(frame.left+2, frame.top+(fBaselineOffset-1));
	owner->DrawString(fText);

	owner->SetHighColor(oldHighColor);
	owner->SetLowColor(oldLowColor);
}


void ColoredStringItem::SetText(const char *text)
{
	if (fText) free(fText);
	
	if (text)
		fText = strdup(text);
	else
		fText = NULL;
}


const char *ColoredStringItem::Text() const
{
	return fText;
}


void ColoredStringItem::SetColor(rgb_color newColor)
{
	this->fg = newColor;
}
void ColoredStringItem::SetBackgroundColor(rgb_color newColor)
{
	this->bg = newColor;
}
void ColoredStringItem::SetSelectionColor(rgb_color newColor)
{
	this->bg_selected = newColor;
}


void ColoredStringItem::Update(BView *owner, const BFont *font)
{
	if (fText)
		SetWidth(font->StringWidth(fText));

	font_height fheight;
	font->GetHeight(&fheight);

	fBaselineOffset = 2 + ceilf(fheight.ascent + fheight.leading / 2);

	SetHeight(ceilf(fheight.ascent) + ceilf(fheight.descent)
		+ ceilf(fheight.leading) + 1);
}


status_t ColoredStringItem::Perform(perform_code d, void *arg)
{
	return BListItem::Perform(d, arg);
}


