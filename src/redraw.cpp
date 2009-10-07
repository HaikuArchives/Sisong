
#include "editor.h"
#include "redraw.fdh"

// only needs to be as big as maximum number of lines visible at once
static char dirty_bits[1000];
static char fullredraw;
static char dirty_bits_present;

// clear the dirty bits, should be done after a screen update is finished
void rd_clear_dirty_bits(EditView *ev)
{
	if (dirty_bits_present)
	{
		memset(dirty_bits, 0, editor.height);
		fullredraw = 0;
		dirty_bits_present = 0;
	}
}

// mark the entire screen for redraw.
void rd_invalidate_all(EditView *ev)
{
	//staterr("rd_invalidate_all...");
	fullredraw = 1;
	dirty_bits_present = 1;
}

// mark the given line number for redraw
void rd_invalidate_line(EditView *ev, int y)
{
	y -= ev->scroll.y;
	if (y >= 0 && y < editor.height)
	{
		//staterr("invalidated line %d, Y coord %d", y+ev->scroll.y, y);
		dirty_bits[y] = 1;
		dirty_bits_present = 1;
		return;
	}
}

// invalidate the given range of line numbers, inclusive.
void rd_invalidate_range(EditView *ev, int y1, int y2)
{
int i;

	if (y1 > y2)
		SWAP(y1, y2);

	//staterr("invalidating lines %d - %d", y1, y2);
	y1 -= ev->scroll.y;
	y2 -= ev->scroll.y;

	if (y1 >= editor.height) return;
	if (y2 < 0) return;

	if (y1 < 0) y1 = 0;
	if (y2 >= editor.height) y2 = (editor.height - 1);

	//staterr("<after adjustment: [%d-%d]>", y1, y2);
	for(i=y1;i<=y2;i++)
		dirty_bits[i] = 1;

	dirty_bits_present = 1;
}

// invalidates the given range of line numbers, exclusive.
// (lines y1 and y2 are not redrawn, only the lines between them).
void rd_invalidate_range_exclusive(EditView *ev, int y1, int y2)
{
	y1++;
	y2--;

	if (y1 <= y2)
		rd_invalidate_range(ev, y1, y2);
}


// mark the line the cursor is on invalid
void rd_invalidate_current_line(EditView *ev)
{
	rd_invalidate_line(ev, ev->cursor.y);
}

// mark the current line and all lines below it invalid
void rd_invalidate_below(EditView *ev, int y)
{
	rd_invalidate_range(ev, y, 0x7fffffff);
}

// mark the current line and all lines above it invalid
void rd_invalidate_above(EditView *ev, int y)
{
	rd_invalidate_range(ev, 0, y);
}

// invalidate all lines which are selected
void rd_invalidate_selection(EditView *ev)
{
int y1, y2;

	GetSelectionExtents(ev, NULL, &y1, NULL, &y2);
	rd_invalidate_range(ev, y1, y2);
}


// returns true if a given Y position on screen is dirty
char rd_is_line_dirty(int y)
{
	if (fullredraw) return 1;
	return dirty_bits[y];
}

// returns true if the current redraw is a "full" redraw
char rd_fullredraw()
{
	return fullredraw;
}

