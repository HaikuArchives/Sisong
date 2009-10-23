
#include "editor.h"
#include "line.fdh"

// initial size of the gap
#define GAP_SIZE		32

// retrieves the current length of the gap
#define GetGapLength(line)	((line->gap_end - line->gap_start) + 1)

// create a new line.
clLine::clLine(const char *initial_string)
{
int strlength;

	strlength = strlen(initial_string);
	
	length = (strlength + GAP_SIZE);
	text = (char *)smal(length + 1);
	
	gap_start = strlength;
	gap_end = (length - 1);
	
	memcpy(text, initial_string, strlength);

	next = prev = NULL;
	memset(&lexresult, 0, sizeof(lexresult));
}

clLine::~clLine()
{
	if (this->text) frees(this->text);
	FreeLexResult(&this->lexresult);
}

// returns the total length of text on the line (not including the gap).
int clLine::GetLength()
{
	return (this->length - GetGapLength(this));
}

void clLine::SetContents(const char *newContents)
{
	int strlength = strlen(newContents);
	
	this->length = (strlength + GAP_SIZE);
	this->text = (char *)resmal(this->text, length + 1);
	
	this->gap_start = strlength;
	this->gap_end = (length - 1);
	
	memcpy(this->text, newContents, strlength);
}

/*
void c------------------------------() {}
*/

// reinitilize the gap in a gap buffer (grow it back to the initial size).
static void grow_gap(clLine *line)
{
int grow_amt;

	// determine how much we will have to increase the gap
	grow_amt = GAP_SIZE - GetGapLength(line);
	
	// allocate a bigger buffer
	line->text = (char *)resmal(line->text, line->length + grow_amt + 1);
	
	// move the right-hand half of the text forward to make room for the new gap
	memmove(&line->text[line->gap_start + GAP_SIZE],
			&line->text[line->gap_end + 1],
			line->length - (line->gap_end + 1));
	
	line->gap_end += grow_amt;
	line->length += grow_amt;
}

// insert a character at the cursor position and advance the insertion point
void clLine::insert_char(char ch)
{
	this->text[this->gap_start] = ch;
	this->gap_start++;
	
	// grow gap if it gets filled up
	if (this->gap_start > this->gap_end)
	{
		grow_gap(this);
	}
}

// insert a string at the cursor position
void clLine::insert_string(char *str)
{
int str_length;
int insert_pos;

	str_length = strlen(str);
	insert_pos = this->gap_start;
	
	// make sure there is enough room in the gap
	this->gap_start += str_length;
	
	if (this->gap_start > this->gap_end)
		grow_gap(this);
	
	// copy the string into new larger gap
	memcpy(&text[insert_pos], str, str_length);
}

// backward DEL (backspace)
void clLine::delete_left()
{
	if (this->gap_start==0)
	{
		errorblip();
		return;
	}
	
	this->gap_start--;
}

// forward DEL
void clLine::delete_right()
{
	if (this->gap_end >= this->length)
	{
		errorblip();
		return;
	}
	
	this->gap_end++;
}

// delete chars between x1 and x2 from line
void clLine::delete_range(int x1, int x2)
{
	if (x2 >= x1)
	{
		set_insertion_point(x1);
		this->gap_end += (x2 - x1) + 1;
	}
}

// delete all chars after index from line (trim line at index).
void clLine::DeleteAfterIndex(int index)
{
	if (index >= GetLength()) return;
	
	set_insertion_point(index);
	this->gap_end = (this->length - 1);
}

// returns character number "index" from line.
char clLine::GetCharAtIndex(int index)
{
	if (index >= this->gap_start)
		index += GetGapLength(this);
	
	if (index >= this->length)
		return 0;
	
	return text[index];
}

// write the entire contents of the line to the given buffer.
// you must check first that the buffer is big enough, and
// a terminating null is NOT written.
void clLine::GetLineToBuffer(char *buffer)
{
	memcpy(buffer, this->text, this->gap_start);
	memcpy(buffer + this->gap_start, &this->text[this->gap_end+1], (this->length - (this->gap_end + 1)));
}

// return the contents of the given line as a String object
BString *clLine::GetLineAsString()
{
BString *string = new BString;

	string->SetTo(this->text, this->gap_start);
	string->Append(&this->text[this->gap_end+1], (this->length - (this->gap_end + 1)));
	
	return string;
}

// returns partial contents of the given line as a String object.
// start & end are inclusive indexes.
// -1 means "all of line before or after".
BString *clLine::GetPartialLine(int start, int end)
{
BString *string = GetLineAsString();

	if (start > 0) DeleteBefore(string, start);
	
	if (end != -1)
	{
		DeleteAfter(string, end);
	}
	
	return string;
}

/*
void c------------------------------() {}
*/

// returns the current insertion point
int clLine::get_insertion_point()
{
	return gap_start;
}

// sets the insertion point
void clLine::set_insertion_point(int newip)
{
	if (this->gap_start != newip)
	{
		int offset = (newip - this->gap_start);
		
		if (offset > 0)
		{
			MoveGapRight(offset);
		}
		else
		{
			MoveGapLeft(-offset);
		}
	}
}

// move the insertion point (gap) a given number of chars to the right.
void clLine::MoveGapRight(int nchars)
{
	// cursor at end of buffer?
	if (this->gap_start + GetGapLength(this) + (nchars - 1) >= this->length)
	{
		// do a partial move to end of buffer if possible
		if ((nchars - 1) > 0)
			MoveGapLeft(nchars - 1);
		else
			errorblip();
		
		return;
	}
	
	// move characters just after end of gap to start of gap.
	memmove(&this->text[gap_start], &text[gap_end+1], nchars);
	
	this->gap_start += nchars;
	this->gap_end += nchars;
}

// move the insertion point (gap) a given number of chars to the left.
void clLine::MoveGapLeft(int nchars)
{
	// cursor at start of buffer?
	if (this->gap_start <= (nchars - 1))
	{
		if (nchars > 1)
			MoveGapLeft(nchars - 1);
		else
			errorblip();
		
		return;
	}
	
	// move character just before start of gap to end of gap.
	this->gap_start -= nchars;
	this->gap_end -= nchars;
	
	memmove(&text[gap_end+1], &text[gap_start], nchars);
}

/*
void c------------------------------() {}
*/

// returns the width of the string in chars, expanding tabs,
// of the given line's string up to the given character index (exclusive).
int clLine::CharCoordToScreenCoord(int index)
{
int i, width;

	for(i=width=0;i<index;i++)
	{
		switch(GetCharAtIndex(i))
		{
			case TAB:
				width += TAB_WIDTH;
				width -= (width % TAB_WIDTH);
			break;
			
			default: width++;
		}
	}
	
	return width;
}

// given a X position within the string, returns the character position
// of the coordinate, expanding tabs.
int clLine::ScreenCoordToCharCoord(int x)
{
int index, width;
int length;

	if (x == 0) return 0;
	index = width = 0;
	
	length = GetLength();
	
	x++;
	while(index < length)
	{
		switch(GetCharAtIndex(index))
		{
			case TAB:
				width += TAB_WIDTH;
				width -= (width % TAB_WIDTH);
			break;
			
			default: width++;
		}
		
		if (width >= x) return index;
		index++;
	}
	
	return length;
}

// returns the number of leading tabs in "line".
int clLine::GetIndentationLevel()
{
int i;
int indent_level = 0;
char ch;

	for(i=0;;i++)
	{
		ch = GetCharAtIndex(i);
		
		if (ch==TAB)
			indent_level++;
		else
			return indent_level;
	}
}

// retrieves the extents of the word which occupies position "x".
// seeks both backwards and forwards to find the start and end of the word.
void clLine::GetWordExtent(int x, int *x1_out, int *x2_out)
{
BString *str;
char *line;
int linelength;
int x1, x2;
char invert, ch;

	str = this->GetLineAsString();
	line = (char *)str->String();
	linelength = str->Length();
	
	x1 = x2 = x;
	
	// if clicked in a space, select the whole space
	ch = line[x1];
	invert = (ch==TAB || ch==' ');
	
	// extend out to the left
	if (x1 >= 0)
	{
		for(;;)
		{
			if (IsWordSeperator(line[x1]) ^ invert) { x1++; break; }
			if (--x1 < 0) { x1 = 0; break; }
		}
	}
	
	// extend out to right
	while(x2 < linelength)
	{
		if (IsWordSeperator(line[x2]) ^ invert) { x2--; break; }
		x2++;
	}
	
	*x1_out = x1;
	*x2_out = x2;
	delete str;
}

static char IsWordSeperator(int ch)
{
	if (ch >= 'a' && ch <= 'z') return 0;
	if (ch >= 'A' && ch <= 'Z') return 0;
	if (ch == '_') return 0;
	if (ch == '%') return 0;	// for "%d" etc in strings
	if (ch >= '0' && ch <= '9') return 0;
	
	return 1;
}

