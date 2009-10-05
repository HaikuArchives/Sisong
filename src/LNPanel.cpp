
#include "editor.h"
#include "LNPanel.fdh"

/***********************************************
*  THE											*
*   LINE NUMBERS								*
*     VIEW										*
************************************************/

LNPanel::LNPanel(BRect frame, uint32 resizingMode)
            : BView(frame, "LineNumberView",
			resizingMode, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
{
	font_drawer = new CFontDrawer(editor.settings.font_size);
	SetFont(font_drawer->font);
	
	SetViewColor(B_TRANSPARENT_COLOR);
	
	fFace = -1;	// be sure to update it first time
	
	numLinesShown = 0;
	redraw_needed = true;
}

LNPanel::~LNPanel()
{
	delete font_drawer;
}

void LNPanel::SetFontSize(int new_size)
{
	font_drawer->SetSize(new_size);
	SetFont(font_drawer->font);
}

/*
void c------------------------------() {}
*/

void LNPanel::Draw(BRect updateRect)
{
	Redraw();
}

void LNPanel::Redraw()
{
int i, y;

	if (LockLooper())
	{
		// check if Bold option has changed in prefs panel
		int face = GetColorBoldState(COLOR_LINENUM) ? B_BOLD_FACE : B_REGULAR_FACE;
		if (face != fFace)
		{
			font_drawer->SetFace(this, face);
			fFace = face;
		}
		
		y = 0;
		for(i=0;i<numLinesShown;i++)
		{
			lineItems[i].DrawItem(this, y);
			y += font_drawer->fontheight;
		}
		
		// clear unused area if lines don't cover whole height
		BRect unusedrect(Bounds());	
		unusedrect.top = y;
		
		if (unusedrect.top <= unusedrect.bottom)
		{
			SetLowColor(GetEditBGColor(COLOR_LINENUM));
			FillRect(unusedrect, B_SOLID_LOW);
		}
		
		UnlockLooper();
		redraw_needed = false;
		//stat("LNPANEL redrawn @ nls = %d, Height = %.2f, clear from %d-%d", numLinesShown, Bounds().Height(), (int)unusedrect.top, (int)unusedrect.bottom);
	}
}

void LNPanel::RedrawIfNeeded()
{
	if (redraw_needed)
		Redraw();
}

// change line number at row Y to read line number 'newNumber'.
void LNPanel::SetLineNumber(int y, int newNumber)
{
	if (y < 0 || y >= MAX_LN_NUMBERS)
		return;
	
	lineItems[y].SetNumber(this, newNumber);
	redraw_needed = true;
	// needed to fix a bug where the numbers would occasionally not be redrawn
	// after the compile panel was closed
	Invalidate();
}

void LNPanel::SetNumVisibleLines(int count)
{
	if (count >= MAX_LN_NUMBERS)
		count = MAX_LN_NUMBERS-1;
	
	if (count != numLinesShown)
	{
		numLinesShown = count;
		redraw_needed = true;
	}
}

/***********************************************
*  EACH	INDIVIDUAL								*
*    LINE NUMBER (PRIVATE)						*
************************************************/

void LNItem::SetNumber(LNPanel *parent, int new_no)
{
	// build string to draw
	sprintf(StringToDraw, "%d", new_no);
	
	// get X coordinate that aligns the number with right edge
	parent->LockLooper();
	
	draw_x = ((int)(parent->Bounds().right - parent->Bounds().left) -
		parent->font_drawer->GetStringWidth(StringToDraw));
	
	parent->UnlockLooper();
	
	draw_x -= 6;
}

void LNItem::DrawItem(LNPanel *parent, int y)
{
	// set colors
	parent->SetLowColor(GetEditBGColor(COLOR_LINENUM));
	parent->SetHighColor(GetEditFGColor(COLOR_LINENUM));

	// clear the area we're about to redraw
	BRect r(parent->Bounds());
	r.top = y;
	r.bottom = (y + parent->font_drawer->fontheight);
	parent->FillRect(r, B_SOLID_LOW);
	
	// draw the string
	parent->font_drawer->DrawString(parent, StringToDraw, draw_x, y);
}

