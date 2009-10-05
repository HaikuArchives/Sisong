
#include "editor.h"
#include "DocPoint.fdh"

DocPoint::DocPoint()
{
	this->line = NULL;
	this->x = this->y = 0;
}

DocPoint::DocPoint(DocPoint *d)
{
	this->x = d->x;
	this->y = d->y;
	this->line = d->line;
}

DocPoint::DocPoint(EditView *ev, int x, int y)
{
	Set(ev, x, y);
}

DocPoint::DocPoint(EditView *ev, int x, int y, clLine *line)
{
	Set(ev, x, y, line);
}

bool DocPoint::IsValid()
{
	return (this->line != NULL);
}

void DocPoint::Print()
{
	if (IsValid())
		stat("line %d, x=%d", y, x);
	else
		stat("<invalid DocPoint>");
}

/*
void c------------------------------() {}
*/

void DocPoint::Set(EditView *ev, int x, int y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (y >= ev->nlines) y = (ev->nlines - 1);
	
	this->x = x;
	this->y = y;
	this->line = ev->GetLineHandle(y);
}

void DocPoint::Set(EditView *ev, int x, int y, clLine *line)
{
	this->x = x;
	this->y = y;
	this->line = line;
}

void DocPoint::SetToCursor(EditView *ev)
{
	x = ev->cursor.x;
	y = ev->cursor.y;
	line = ev->curline;
}

void DocPoint::SetToStart(EditView *ev)
{
	Set(ev, 0, 0);
}

void DocPoint::SetToEnd(EditView *ev)
{
	clLine *lastline = ev->lastline;
	if (lastline)
	{
		int llen = lastline->GetLength();
		Set(ev, llen, ev->nlines-1);
	}
}

/*
void c------------------------------() {}
*/

// moves the given docpoint forward one char.
// if the docpoint goes past the end of the line,
// moves it to the beginning of the next line.
void DocPoint::Increment()
{
int line_length;

	line_length = line->GetLength();
	
	if (x >= line_length)
	{
		if (line->next)
		{
			line = line->next;
			y++;
			x = 0;
		}
		
		return;
	}
	
	x++;
}


// moves the given docpoint back one char.
// if the docpoint goes past the beginning of the line,
// moves it to the end of the previous line.
void DocPoint::Decrement()
{
	if (x > 0)
	{
		x--;
		return;
	}
	
	// at beginning of document, can't go back any more
	if (!line->prev) return;
	
	y--;
	line = line->prev;
	x = line->GetLength();
}


void DocPoint::IncrementBy(int count)
{
	while(count--)
		Increment();
}


void DocPoint::DecrementBy(int count)
{
	while(count--)
		Decrement();
}

/*
void c------------------------------() {}
*/

// returns true if the two points are equal.
bool DocPoint::operator==(const DocPoint &other) const
{
	return (this->y == other.y && this->x == other.x);
}

// returns true if the docpoint is greater than (further down in the document)
// than the given "other".
bool DocPoint::operator>(const DocPoint &other) const
{
	if (this->y >= other.y)
	{
		if (this->y == other.y)
		{
			return (this->x > other.x);
		}
		else
		{
			return true;
		}
	}
	
	return false;
}

bool DocPoint::operator>=(const DocPoint &other) const
{
	if (this->y >= other.y)
	{
		if (this->y == other.y)
		{
			return (this->x >= other.x);
		}
		else
		{
			return true;
		}
	}
	
	return false;
}

// returns true if this docpoint is further up in the document than the
// given "other".
bool DocPoint::operator<(const DocPoint &other) const
{
	if (this->y <= other.y)
	{
		if (this->y == other.y)
		{
			return (this->x < other.x);
		}
		else
		{
			return true;
		}
	}
	
	return false;
}

bool DocPoint::operator<=(const DocPoint &other) const
{
	if (this->y <= other.y)
	{
		if (this->y == other.y)
		{
			return (this->x <= other.x);
		}
		else
		{
			return true;
		}
	}
	
	return false;
}


