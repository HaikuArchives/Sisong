
#include "editor.h"
#include "edit_cursor.fdh"

/*
	left
	right
	up
	down
	pgup
	pgdn
	to_eol
	to_start
*/

void EditCursor::left()
{	
	if (x > 0)
	{
		x--;
	}
	else if (y > 0)
	{
		ev->curline = ev->curline->prev;
		y--;
		x = ev->curline->GetLength();
	}
	
	UpdateCursorPos(ev);
}


void EditCursor::right()
{	
	if (x < ev->curline->GetLength())
	{
		x++;
	}
	else if (ev->curline->next)
	{
		x = 0; y++;
		ev->curline = ev->curline->next;
	}
	
	UpdateCursorPos(ev);
}

void EditCursor::up()
{	
	if (ev->cursor.y)
	{
		set_mode(CM_WANT_SCREEN_COORD);
		
		ev->curline = ev->curline->prev;
		ev->cursor.y--;
		
		UpdateCursorPos(ev);
	}
}

// down moves the cursor on the next line.
// the cursor enters a mode where it will try to the same apparent screen
// position it was at when it left the current line.
void EditCursor::down()
{
	if (ev->curline->next)
	{
		set_mode(CM_WANT_SCREEN_COORD);
		
		ev->curline = ev->curline->next;
		ev->cursor.y++;
		
		UpdateCursorPos(ev);
	}
}

void EditCursor::pgdn()
{
int pagesize, y;

	// bring bottom line to top
	pagesize = (editor.height - 1);
	ev->scroll_down(pagesize);
	
	// cursor tries to stay at same screen position
	y = (ev->cursor.y + pagesize);
	
	if (y < ev->nlines)
	{
		set_mode(CM_WANT_SCREEN_COORD);
		move(0, y);
		UpdateCursorPos(ev);
	}
	else
	{
		move(0, (ev->nlines - 1));
	}	
}

void EditCursor::pgup()
{
int pagesize, y;

	// bring top line to bottom
	pagesize = (editor.height - 1);
	ev->scroll_up(pagesize);
	
	// cursor tries to stay at same screen position
	y = (ev->cursor.y - pagesize);
	
	if (y >= 0)
	{
		set_mode(CM_WANT_SCREEN_COORD);
		move(ev->cursor.x, y);
	}
	else
	{
		move(0, 0);
	}
	
	UpdateCursorPos(ev);
}

void EditCursor::to_eol()
{
	x = ev->curline->GetLength();
	UpdateCursorPos(ev);
}

void EditCursor::to_home()
{
	x = 0;
	UpdateCursorPos(ev);
}

/*
void c------------------------------() {}
*/

// jump the editing cursor to a specified line and column.
void EditCursor::move(int x, int y)
{
	if (y < 0) y = 0;
	if (y >= ev->nlines) y = ev->nlines-1;	

	ev->cursor.x = x;

	// GetLineHandle may use curline as optimization, so curline MUST be set first.
	if (y != ev->cursor.y)
	{
		ev->curline = ev->GetLineHandle(y);
		ev->cursor.y = y;
	}
	
	UpdateCursorPos(ev);
}


void EditCursor::set_mode(int newmode)
{
	if (xseekmode==CM_FREE || newmode==CM_FREE)
	{
		xseekmode = newmode;
		xseekcoord = screen_x;
	}
}

const char *EditCursor::DescribeMode()
{
	switch(this->xseekmode)
	{
		case CM_FREE: return "CM_FREE";
		case CM_WANT_SCREEN_COORD: return "CM_WANT_SCREEN_COORD";
		case CM_WANT_EOL: return "CM_WANT_EOL";
		default: return "<unknown cursor mode>";
	}
}

/*
void c------------------------------() {}
*/

void UpdateCursorPos(EditView *ev)
{
int line_length = ev->curline->GetLength();

	// handle special "x-seek" modes
	switch(ev->cursor.xseekmode)
	{
		// try to go to the same screen X position we were at when we started
		// seeking, but don't go past the end of the line.
		case CM_WANT_SCREEN_COORD:
			ev->cursor.x = ev->curline->ScreenCoordToCharCoord(ev->cursor.xseekcoord);
		break;
		
		// always go to the end of the current line
		case CM_WANT_EOL:
			ev->cursor.x = line_length;
		break;
	}
	
	if (ev->cursor.x > line_length)
		ev->cursor.x = line_length;
	
	ev->cursor.screen_x = ev->curline->CharCoordToScreenCoord(ev->cursor.x);
	ev->cursor.screen_y = (ev->cursor.y - ev->scroll.y);
}

