
#ifndef _COLORITEM_H
#define _COLORITEM_H

#include "../MessageView.h"

#define M_CI_BOLD_CLICKED	'C!CK'
#define M_CI_FG_CLICKED		'C!FG'
#define M_CI_BG_CLICKED		'C!BG'
#define M_CI_CONTROL_CLICK_FG	'C!C1'
#define M_CI_CONTROL_CLICK_BG	'C!C2'

class ColorView;

class ColorItem : public MessageView
{
public:
	ColorItem(BRect frame, const char *text, rgb_color fg, rgb_color bg, \
			bool boldstate, bool UsesBothColors, int index, ColorView *c_view);
	~ColorItem();
	
	void UpdateData(rgb_color new_fg, rgb_color new_bg, bool NewBoldState);
	
	virtual void Draw(BRect updateRect);
	virtual void MouseDown(BPoint where);
	
	BCheckBox *chkBold;

private:
	BRect GetLeftSwatchRect();
	BRect GetRightSwatchRect();
	void DrawHighlight(BRect rc);
	bool IsControlClick();

	rgb_color leftColor, rightColor;
	bool fColorsCombined;	// ... "I am Captain Planet!"
	bool fBoldState;

	char *fText;
	
	BLooper *fTarget;
	ColorView *fParentColorView;
	int fIndex;
};

#endif
