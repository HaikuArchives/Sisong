
// routines for drawing and flashing the cursor
// this handles the actual display on-screen of the cursor

#include "editor.h"
#include "draw_cursor.fdh"


#define CURSOR_W		(fThick ? editor.font_width : 1)
#define CURSOR_H		(editor.font_height)

#define THINFLASHRATE	5
#define THICKFLASHRATE	3

CFlashingCursor::CFlashingCursor(BView *parentView) :
	_cx(5),
	_cy(5),
	view(parentView),
	fActive(true),
	flashstate(0),
	visible(0),
	fThick(false),
	fFlashRate(THINFLASHRATE)
{ }

/*
void c------------------------------() {}
*/

void CFlashingCursor::tick()
{
	if (!fActive)
		return;
	
	if (++timer >= fFlashRate)
	{
		timer = 0;
		
		if (!flashstate)
		{
			flashstate = 1;
			draw();
		}
		else
		{
			flashstate = 0;
			erase();
		}
	}
}

// enable or disable the flashing for when the window
// becomes active or inactive.
void CFlashingCursor::EnableFlashing(bool newActive)
{
	if (fActive != newActive)
	{
		fActive = newActive;
		
		if (!newActive)
		{
			if (flashstate)
				erase();
		}
		else
		{
			bump();
			draw();
		}
	}
}

// turn on or off "thick" cursor (used for Overwrite mode)
void CFlashingCursor::SetThick(bool enabled)
{
bool fWasVisible;

	if (enabled != fThick)
	{
		fWasVisible = visible;
		if (visible) erase();
		
		fThick = enabled;
		fFlashRate = enabled ? THICKFLASHRATE : THINFLASHRATE;
		
		if (fWasVisible) draw();
	}
}

// draws the cursor
void CFlashingCursor::draw()
{
int x, y;

	if (!fActive) return;
	if (_cy < 0 || _cy >= editor.height) return;
	
	visible = 1;

	//stat("draw[%d, %d]", _cx, _cy);

	x = GET_CURSOR_PX(_cx);
	y = GET_CURSOR_PY(_cy);
	
	// draw the cursor
	LockWindow();
	
	MainView->SetHighColor(GetEditColor(COLOR_CURSOR));
	
	if (fThick)
		MainView->StrokeRect(BRect(x, y, x + (CURSOR_W - 1), y + (CURSOR_H - 1)));
	else
		MainView->StrokeLine(BPoint(x, y), BPoint(x, y + (CURSOR_H - 1)));
	
	UnlockWindow();
}

// erases the cursor
void CFlashingCursor::erase()
{
	erase(_cx, _cy);
}

void CFlashingCursor::erase(int cx, int cy)
{
int x, y, line_number;
EditView *ev = editor.curev;

	visible = 0;
	if (cy < 0 || cy >= editor.height) return;
	//stat("erase[%d, %d]", cx, cy);
	
	x = GET_CURSOR_PX(cx);
	y = GET_CURSOR_PY(cy);
	
	// restore the image behind the cursor
	LockWindow();
	line_number = (ev->scroll.y + cy);
	
	if (line_number >= ev->nlines)
	{
		MainView->SetLowColor(GetEditBGColor(COLOR_TEXT));
		MainView->FillRect(BRect(x, y, x + (CURSOR_W - 1), y + (CURSOR_H - 1)), B_SOLID_LOW);
	}
	else
	{
		editor.curline_bb->Lock();
		
		if (line_number != editor.bbed_line)
		{
			clLine *line = ev->GetLineHandle(line_number);
			if (line)
			{
				if (!line->lexresult.points) lexer_update_line(line);
				UpdateLine(editor.curline_bb, ev, line, 0, line_number);
				editor.bbed_line = line_number;
			}
			else
			{
				staterr("CFlashingCursor::erase: could not obtain line handle to update BB");
			}
		}
		
		BRect source(x, 0, x + (CURSOR_W - 1), CURSOR_H-1);
		BRect dest(x, y, x + (CURSOR_W - 1), y + (CURSOR_H-1));
		
		editor.curline_bb->BlitTo(view, source, dest);
		editor.curline_bb->Unlock();
	}

	UnlockWindow();
}

// sets the x & y position of the cursor, given in chars.
void CFlashingCursor::move(int x, int y)
{
	this->_cx = x;
	this->_cy = y;
}

// "bump" the cursor (reset it's blink timer, so that it is shown for a full cycle).
void CFlashingCursor::bump()
{
	this->flashstate = 1;
	this->timer = 0;
}
