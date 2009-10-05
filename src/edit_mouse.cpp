
#include "editor.h"
#include "edit_mouse.fdh"

void EditView::MouseDown(int x, int y)
{
EditView *ev = this;

	ev->CancelCommandSeq();

	// shift-clicking to open a selection
	if (IsShiftDown())
	{
		this->mouse.lx = -1;
		this->mouse.ly = -1;
		MouseDrag(x, y);
		return;
	}

	if (PixelToCharCoords(ev, &x, &y)) return;
	MainView->SetMouseEventMask(B_POINTER_EVENTS, B_LOCK_WINDOW_FOCUS);

	// move mouse, drop selection if present
	ev->cursor.set_mode(CM_FREE);
	
	selection_drop(ev);
	ev->cursor.move(x, y);
	
	// detect if this is a double- or triple-click
	if (x==ev->mouse.lx && y==ev->mouse.ly && (timer() - ev->mouse.lastclicktime < 400))
	{
		ev->mouse.clickcount++;
		
		if (ev->mouse.clickcount==1)		// double-click
		{
			selection_SelectCurrentWord(ev);
		}
		else if (ev->mouse.clickcount==2)	// triple-click
		{
			selection_SelectCurrentLine(ev);
			ev->cursor.move(0, ev->cursor.y+1);
		}
		else if (ev->mouse.clickcount==3)	// quadruple-click
		{
			selection_SelectAll(ev);
		}
		else
		{
			ev->mouse.clickcount = 0;
		}
	}
	else
	{
		ev->mouse.clickcount = 0;
		editor.stats.mouse_clicks++;
	}
	
	ev->mouse.lastclicktime = timer();
	ev->mouse.lx = x;
	ev->mouse.ly = y;
	
	ev->RedrawView();
}

void EditView::MouseDrag(int x, int y)
{
EditView *ev = this;

	if (PixelToCharCoords(ev, &x, &y))
		return;
	
	// mouse still on same character as before?
	if (x==ev->mouse.lx && y==ev->mouse.ly)
		return;

	ev->cursor.set_mode(CM_FREE);	
	
	if (!ev->selection.present)
		selection_create(ev);
	
	ev->cursor.move(x, y);
	ev->ExtendSel();
	
	ev->RedrawView();
	
	ev->mouse.lx = x;
	ev->mouse.ly = y;
}

void EditView::MouseUp()
{
}


/*
void c------------------------------() {}
*/

// converts the given pixel position to a character position within the document
char PixelToCharCoords(EditView *ev, int *x_inout, int *y_inout)
{
int x, y, cx, cy, ccx;
clLine *line;

	// read input coords
	x = *x_inout;
	y = *y_inout;
	
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	
	// get Y coordinate and line
	cy = (y / editor.font_height);
	cy += ev->scroll.y;
	if (cy < 0 || cy > ev->nlines)
	{
		ev->RedrawView();	// bumps the cursor
		return 1;
	}
	
	line = ev->GetLineHandle(cy);

	// add in hoz scrolling, pixels to chars
	x += ev->xscroll;	
	cx = (x / editor.font_width);
	
	// screen char coordinates aren't necessarily char line coordinates because
	// tabs are wider than a single char but only count as one character in the line.
	// convert our screen coords to line coords.
	ccx = line->ScreenCoordToCharCoord(cx);
	int line_length = line->GetLength();
	if (ccx >= line_length) ccx = line_length-1;
	
	// get half the width of the clicked char in px
	char ch = line->GetCharAtIndex(ccx);
	int halfchwidth = (ch==TAB) ? (TAB_WIDTH * editor.font_width)/2 : editor.font_width/2;
	
	// get start of character, then, how far in cursor was
	int stofch = line->CharCoordToScreenCoord(ccx);
	int cfarin = (x - (stofch * editor.font_width));
	
	// if clicked more than halfway past width of char, they probably actually wanted next char
	if (cfarin >= halfchwidth) ccx++;	
	
	// we outta here. write output coords.
	*x_inout = ccx;
	*y_inout = cy;
	
	return 0;
}
