
#include "editor.h"
#include "selection.h"

// opens a selection anchored at the current cursor position.
void selection_create(EditView *ev)
{
	ev->selection.anchor.SetToCursor(ev);
	ev->selection.extent.SetToCursor(ev);
	rd_invalidate_line(ev, ev->cursor.y);

	ev->selection.present = true;
}

// extends an existing selection to the cursor position.
void selection_extend(EditView *ev)
{
int before_x1, before_x2, after_x1, after_x2;
int before_y1, before_y2, after_y1, after_y2;

	// obtain original range of selected lines for redraw invalidation
	GetSelectionExtents(ev, &before_x1, &before_y1, &before_x2, &before_y2);

	ev->selection.extent.SetToCursor(ev);

	// selected some, then moved back so selection is no longer visible.
	// kill the selection.
	if (ev->selection.anchor == ev->selection.extent)
	{
		selection_drop(ev);
		rd_invalidate_range(ev, before_y1, before_y2);
		return;
	}

	// invalidate the intersection of the two ranges:
	// invalidate all lines which are in BEFORE but not AFTER,
	// and invalidate all lines which are in AFTER but not BEFORE.
	GetSelectionExtents(ev, &after_x1, &after_y1, &after_x2, &after_y2);

	if (before_y1 > after_y1) SWAP(before_y1, after_y1);
	if (before_y2 > after_y2) SWAP(before_y2, after_y2);

	if (before_y1 != after_y1) rd_invalidate_range(ev, before_y1, after_y1);
	if (before_y2 != after_y2) rd_invalidate_range(ev, before_y2, after_y2);
	//if (after_y1 == after_y2) rd_invalidate_line(ev, after_y1);
	//if (before_y1 == before_y2) rd_invalidate_line(ev, before_x1);
	if (before_y1==after_y1 && before_x1 != after_x1) rd_invalidate_line(ev, before_y1);
	if (before_y2==after_y2 && before_x2 != after_x2) rd_invalidate_line(ev, before_y2);
}

// removes the selection.
void selection_drop(EditView *ev)
{
	if (ev->selection.present)
	{
		rd_invalidate_selection(ev);
		ev->selection.present = false;
	}
}

// returns true if a given point is within the selection.
char selection_IsPointWithin(EditView *ev, int lineno, int x)
{
int start_x, start_y;
int end_x, end_y;

	if (!ev->selection.present) return 0;

	// get start and end points for selection
	GetSelectionExtents(ev, &start_x, &start_y, &end_x, &end_y);

	// check if the point is within the selection
	if (lineno < start_y) return 0;
	if (lineno > end_y) return 0;

	if (lineno == start_y)
	{
		if (x < start_x) return 0;
	}
	if (lineno == end_y)
	{
		if (x > end_x) return 0;
	}

	return 1;
}


void GetSelectionExtents(EditView *ev, int *startx, int *starty, int *endx, int *endy)
{
DocPoint *anchor, *extent;
DocPoint end;
int dummy;

	if (!startx) startx = &dummy;
	if (!starty) starty = &dummy;
	if (!endx) endx = &dummy;
	if (!endy) endy = &dummy;

	if (!ev->selection.present)
	{
		*startx = *endx = *starty = *endy = 0;
		return;
	}

	anchor = &ev->selection.anchor;
	extent = &ev->selection.extent;

	if (*extent > *anchor)
	{	// "right-leaning" selection
		*startx = anchor->x;
		*starty = anchor->y;

		end = *extent;
		end.Decrement();

		*endx = end.x;
		*endy = end.y;
	}
	else
	{	// "left-leaning" selection
		*startx = extent->x;
		*starty = extent->y;

		end = *anchor;
		end.Decrement();

		*endx = end.x;
		*endy = end.y;
	}
}

void SetSelectionExtents(EditView *ev, int x1, int y1, int x2, int y2)
{
	ev->selection.anchor.x = x1;
	ev->selection.anchor.y = y1;
	ev->selection.anchor.line = ev->GetLineHandle(y1);

	ev->selection.extent.x = x2;
	ev->selection.extent.y = y2;
	ev->selection.extent.line = ev->GetLineHandle(y2);

	ev->selection.extent.Increment();
}

/*
void c------------------------------() {}
*/

// select the entire line the cursor is currently on
void selection_SelectCurrentLine(EditView *ev)
{
	ev->selection.present = true;

	ev->selection.anchor.x = 0;
	ev->selection.anchor.y = ev->cursor.y;
	ev->selection.anchor.line = ev->curline;

	ev->selection.extent.x = ev->curline->GetLength() + 1;
	ev->selection.extent.y = ev->cursor.y;
	ev->selection.extent.line = ev->curline;

	rd_invalidate_current_line(ev);
}

// select the word the cursor is currently within.
void selection_SelectCurrentWord(EditView *ev)
{
	ev->selection.present = true;

	int x1, x2;
	ev->GetCurrentWordExtent(&x1, &x2);
	// because extent is > than anchor
	if (++x2 > ev->curline->GetLength())
		x2 = ev->curline->GetLength();

	ev->selection.anchor.x = x1;
	ev->selection.extent.x = x2;
	ev->selection.anchor.y = ev->cursor.y;
	ev->selection.extent.y = ev->cursor.y;
	ev->selection.anchor.line = ev->curline;
	ev->selection.extent.line = ev->curline;

	rd_invalidate_current_line(ev);
}

// select the entire document
void selection_SelectAll(EditView *ev)
{
	ev->selection.present = true;

	ev->selection.anchor.x = 0;
	ev->selection.anchor.y = 0;
	ev->selection.anchor.line = ev->firstline;

	ev->selection.extent.y = (ev->nlines - 1);
	ev->selection.extent.line = ev->lastline;
	ev->selection.extent.x = ev->lastline->GetLength();

	rd_invalidate_all(ev);
}

void selection_select_range(EditView *ev, DocPoint *start, DocPoint *end)
{
	ev->selection.present = true;

	ev->selection.anchor.x = start->x;
	ev->selection.anchor.y = start->y;
	ev->selection.anchor.line = start->line;

	ev->selection.extent.x = end->x;
	ev->selection.extent.y = end->y;
	ev->selection.extent.line = end->line;
	ev->selection.extent.Increment();

	rd_invalidate_range(ev, start->y, end->y);
}

// expand the selection to encompass full lines
void selection_SelectFullLines(EditView *ev)
{
int x1, y1, x2, y2;

	GetSelectionExtents(ev, &x1, &y1, &x2, &y2);

	x1 = 0;
	x2 = (ev->GetLineHandle(y2))->GetLength();

	SetSelectionExtents(ev, x1, y1, x2, y2);
}


