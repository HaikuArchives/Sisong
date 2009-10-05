
#include <View.h>
#include <stdio.h>

#include "TitleView.h"
#include "FileItem.h"		// SIZE_TAB_OFFS

// some of this drawing stuff taken from Haiku source tree in the real BFilePanel...thanks
static rgb_color sTitleBackground;
static rgb_color sDarkTitleBackground;
static rgb_color sShineColor;
static rgb_color sLightShadowColor;
static rgb_color sShadowColor;
static rgb_color sDarkShadowColor;


TitleView::TitleView(BRect frame, uint32 resizingMode)
	: BView(frame, "TitleView", resizingMode, B_WILL_DRAW)
{
	SetViewColor(B_TRANSPARENT_COLOR);

	sTitleBackground = tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), 0.88f);
	sDarkTitleBackground = tint_color(sTitleBackground, B_DARKEN_1_TINT);
	sShineColor = tint_color(sTitleBackground, B_LIGHTEN_MAX_TINT);
	sLightShadowColor = tint_color(sTitleBackground, B_DARKEN_2_TINT);
	sShadowColor = tint_color(sTitleBackground, B_DARKEN_4_TINT);
	sDarkShadowColor = tint_color(sShadowColor, B_DARKEN_2_TINT);
	
	BFont font(be_plain_font);
	font.SetSize(9);
	SetFont(&font);
}

void TitleView::Draw(BRect updateRect)
{
	BRect bounds(Bounds());
	
	SetHighColor(sTitleBackground);
	FillRect(bounds);
	
	SetHighColor(tint_color(sTitleBackground, 1.03f));
	BRect rc(bounds);
	rc.top = rc.bottom - 6;
	FillRect(rc);
	
	// draw bottom border
	BeginLineArray(6);
	AddLine(bounds.LeftTop(), bounds.RightTop(), sShadowColor);
	AddLine(bounds.LeftBottom(), bounds.RightBottom(), sShadowColor);
	// draw lighter gray and white inset lines
	bounds.InsetBy(0, 1);
	//AddLine(bounds.LeftBottom(), bounds.RightBottom(), sLightShadowColor);
	AddLine(bounds.LeftTop(), bounds.RightTop(), sShineColor);
	
	AddLine(bounds.LeftTop(), bounds.LeftBottom(), sLightShadowColor);
	AddLine(bounds.RightTop(), bounds.RightBottom(), sLightShadowColor);
	
	EndLineArray();
	
	// seperators
	SetHighColor(sLightShadowColor);
	StrokeLine(BPoint(27, bounds.top), BPoint(27, bounds.bottom));
	StrokeLine(BPoint(bounds.right - SIZE_TAB_OFFS - 18, bounds.top),
				BPoint(bounds.right - SIZE_TAB_OFFS - 18, bounds.bottom));

	// draw text
	SetHighColor(sDarkShadowColor);
	SetLowColor(sTitleBackground);
	DrawString("Name", BPoint(33, 12));
	DrawString("Size", BPoint(bounds.right - SIZE_TAB_OFFS - 12, 12));
}



