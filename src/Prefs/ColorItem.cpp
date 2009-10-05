
#include <ListItem.h>
#include <GraphicsDefs.h>
#include <Message.h>
#include <View.h>
#include <CheckBox.h>

#include <string.h>
#include <stdlib.h>

#include "../../common/basics.h"
#include "../MessageView.h"
#include "../colors.h"

#include "ColorItem.h"
#include "ColorView.h"
#include "ColorItem.fdh"

#define COLOR_ITEM_HEIGHT		32


ColorItem::ColorItem(BRect frame, const char *text, rgb_color fg, rgb_color bg, \
					bool boldstate, bool UsesBothColors, int index, ColorView *c_view)
	: MessageView(frame, "", B_FOLLOW_ALL, B_WILL_DRAW),
	  fText(NULL),
	  fBoldState(boldstate),
	  fIndex(index),
	  leftColor(fg),
	  rightColor(bg),
	  fColorsCombined(UsesBothColors)
{
	fText = strdup(text);

	fParentColorView = c_view;
	fTarget = c_view->Looper();
	
	fColorsCombined = !UsesBothColors;
	leftColor = fg;
	rightColor = bg;

	// create "Bold" checkbox
	BRect rc(Bounds());
	rc.left = rc.right - 23;
	rc.top += 4;
	
	// presence of bold has nothing logically to do with whether colors are
	// combined or not, but it happens to be that the colors for which bold doesn't
	// apply are the same as for which the colors are combined.
	if (!fColorsCombined)
	{
		chkBold = new BCheckBox(rc, "", "", NULL);
		chkBold->SetValue(boldstate);
		
		BMessage *msg = new BMessage(M_CI_BOLD_CLICKED);
		msg->AddInt32("index", fIndex);
		chkBold->SetMessage(msg);
		chkBold->SetTarget(fTarget);
		
		AddChild(chkBold);
	}
}

ColorItem::~ColorItem()
{
	free(fText);
}

void ColorItem::UpdateData(rgb_color new_fg, rgb_color new_bg, bool NewBoldState)
{
	if (fBoldState != NewBoldState)
	{
		fBoldState = NewBoldState;
		chkBold->SetValue(fBoldState);
	}

	if (new_fg != leftColor || new_bg != rightColor)
	{
		leftColor = new_fg;
		rightColor = new_bg;
		
		Invalidate();
	}
}

/*
void c------------------------------() {}
*/

void ColorItem::MouseDown(BPoint where)
{
BMessage *msg;
BRect rc;
uint32 buttons;

	int MessageToSend = 0;
	
	if (fColorsCombined)
	{
		BRect rc(GetLeftSwatchRect());
		BRect rc2(GetRightSwatchRect());
		rc.right = rc2.right;
		
		if (rc.Contains(where))
			MessageToSend = M_CI_FG_CLICKED;
	}
	else
	{
		rc = GetLeftSwatchRect();
		
		if (rc.Contains(where))
		{
			MessageToSend = M_CI_FG_CLICKED;
		}
		else
		{
			rc = GetRightSwatchRect();
			
			if (rc.Contains(where))
				MessageToSend = M_CI_BG_CLICKED;
		}
	}
	
	if (MessageToSend)
	{		
		if (IsControlClick())
		{
			if (MessageToSend == M_CI_FG_CLICKED)
				MessageToSend = M_CI_CONTROL_CLICK_FG;
			else
				MessageToSend = M_CI_CONTROL_CLICK_BG;
		}
		else
		{
			GetMouse(&where, &buttons);
			
			if (buttons & B_SECONDARY_MOUSE_BUTTON)
			{
				fParentColorView->SetPickedUpColor(this, (MessageToSend == M_CI_BG_CLICKED));
				return;
			}
		}
		
		msg = new BMessage(MessageToSend);
		msg->AddInt32("index", fIndex);
		fTarget->PostMessage(msg);
	}
	else
	{
		MessageView::MouseDown(where);
	}
}


bool ColorItem::IsControlClick()
{
	return (IsCtrlDown() || IsAltDown() || IsShiftDown());
}


/*
void c------------------------------() {}
*/

void ColorItem::Draw(BRect updateRect)
{
BRect rc;

	// draw text
	font_height fheight;
	BFont font;
	GetFont(&font);
	font.GetHeight(&fheight);
	
	SetHighColor(0, 0, 0);
	SetLowColor(ViewColor());
	
	MovePenTo(8, (HEIGHTOF(Bounds()) / 2) + fheight.descent + 1);
	DrawString(fText);
	
	// get whether we should be highlighting because of
	// the "color pickup" feature
	bool highlight_fg = false;
	bool highlight_bg = false;
	bool IsBGFlag;
	
	if (fParentColorView->GetPickedUpColor(&IsBGFlag) == this)
	{
		if (IsBGFlag)
			highlight_bg = true;
		else
			highlight_fg = true;
	}
	
	// draw fg swatch or combined swatch
	rc = GetLeftSwatchRect();
	
	if (fColorsCombined)
		rc.right = GetRightSwatchRect().right;
	
	SetHighColor(0, 0, 0);
	StrokeRect(rc);
	
	SetHighColor(leftColor);
	rc.InsetBy(1, 1);
	FillRect(rc);
	
	if (highlight_fg)
		DrawHighlight(rc);
	
	// draw right swatch
	if (!fColorsCombined)
	{
		rc = GetRightSwatchRect();
		
		SetHighColor(0, 0, 0);
		StrokeRect(rc);
		
		SetHighColor(rightColor);
		rc.InsetBy(1, 1);
		FillRect(rc);
		
		if (highlight_bg)
			DrawHighlight(rc);
	}
}

void ColorItem::DrawHighlight(BRect rc)
{
static const rgb_color highlight_color = { 0, 255, 255 };

	SetHighColor(highlight_color);
	StrokeRect(rc);
	
	rc.InsetBy(1, 1);
	SetHighColor(0, 0, 0);
	StrokeRect(rc);
}

BRect ColorItem::GetRightSwatchRect()
{
	BRect rc(Bounds());
	rc.left = rc.right - HEIGHTOF(rc);
	rc.OffsetBy(-30, -1);
	rc.InsetBy(5, 4);

	return rc;
}

BRect ColorItem::GetLeftSwatchRect()
{
	BRect rc(GetRightSwatchRect());
	rc.OffsetBy(-32, 0);
	
	return rc;
}



