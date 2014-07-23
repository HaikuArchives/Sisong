
#include "editor.h"
#include "edit_actions.h"

#include "misc.h"
#include "misc2.h"
#include "stat.h"
#include "redraw.h"

// inserts a character at the given position.
void EditView::action_insert_char(int x, int y, char ch)
{
EditView *ev = this;
clLine *line;

	ev->SetDirty();
	undo_add(ev, x, y, 1, NULL);
	
	line = ev->GetLineHandle(y);
	
	line->set_insertion_point(x);
	line->insert_char(ch);
	
	rd_invalidate_line(ev, y);
	ModifiedSinceRedraw = true;
}

// inserts multiple chars at the given position.
// the string can contain CR's.
// optionally returns the coordinates of the end of the inserted string
// in final_x/final_y.
void EditView::action_insert_string(int x, int y, const char *str, int *final_x, int *final_y)
{
EditView *ev = this;
clLine *line, *startline;
char hit_cr = 0;
int orgy = y;

	int string_length = strlen(str);
	if (string_length == 0)
		return;
	
	ev->SetDirty();
	undo_add(ev, x, y, string_length, NULL);
	
	line = startline = ev->GetLineHandle(y);
	if (!line)
	{
		stat("action_insert_string: couldn't get handle for line %d", y);
		return;
	}
	
	line->set_insertion_point(x);
	
	while(*str)
	{
		if (*str == '\n')
		{
			InternalInsertCR(ev, x, y);		// calls invalidate_below and updates undo
			hit_cr = 1;
			
			y++;
			x = 0;
			
			line = line->next;
			line->set_insertion_point(0);
		}
		else
		{
			line->insert_char(*str);
			x++;
		}
		
		str++;
	}
	
	// do initial lex of all added lines, so block comments etc look correct
	// in case of large pastes that are taller than the window.
	if (line != startline)
	{
		clLine *lexme = startline;
		do
		{
			lexer_update_line(lexme);
			lexme = lexme->next;
		}
		while(lexme != line && lexme);
	}
	
	if (!hit_cr)
		rd_invalidate_line(ev, orgy);
	else
		rd_invalidate_below(ev, orgy);
	
	if (final_x) *final_x = x;
	if (final_y) *final_y = y;
	ModifiedSinceRedraw = true;
}


// inserts a newline at the given position, splitting lines, etc.
// it does NOT auto-indent.
void EditView::action_insert_cr(int x, int y)
{
EditView *ev = this;

	ev->SetDirty();
	undo_add(ev, x, y, 1, NULL);
	rd_invalidate_below(ev, y);
	
	InternalInsertCR(ev, x, y);
	ModifiedSinceRedraw = true;
}


static void InternalInsertCR(EditView *ev, int x, int y)
{
clLine *line;

	line = ev->GetLineHandle(y);
	
	// if inserting a CR into the middle of a line, split the line
	if (x < line->GetLength())
	{
		BString *initial_string = line->GetLineAsString();
		
		DeleteBefore(initial_string, x);
		line->DeleteAfterIndex(x);
		
		ev->insert_line((char *)initial_string->String(), line, y);
		delete initial_string;
	}
	else
	{
		ev->insert_line("", line, y);
	}
}


/*
void c------------------------------() {}
*/

// deletes "count" chars to the right of the insertion point.
// if the end of the line is encountered, erases the CR by concatenating the next line.
void EditView::action_delete_right(int x, int y, int count)
{
EditView *ev = this;
clLine *line;
int x2, y2;
BString *deldata;
char hit_cr = 0;

	if (count == 0)
		return;

	lstat2("action_delete_right: [%d,%d] %d", x, y, count);
	ev->SetDirty();
	
	line = ev->GetLineHandle(y);
	line->set_insertion_point(x);
	
	// get the range of chars which will be deleted.
	// this is (count-1) because, say we're deleting 1 char:
	// the range is only [x,y]-[x,y].
	ev->AddToDocPoint(x, y, line, count-1, &x2, &y2);
	deldata = ev->RangeToString(x, y, x2, y2, "\n");
	
	// save the to-be-deleted data to the undo buffer
	undo_add(ev, x, y, count, deldata);
	
	rept
	{
		if (x >= line->GetLength())
		{
			// can't delete past end-of-file
			if (!line->next) break;
			
			// concatenate the following line onto the end of the current one,
			// then delete the following line.
			BString *string = line->next->GetLineAsString();
			line->insert_string((char *)string->String());
			delete string;
			
			ev->delete_line(line->next, y+1);
			line->set_insertion_point(x);
			
			hit_cr = 1;
		}
		else
		{
			line->delete_right();
		}
		
		if (count <= 1) break;
		count--;
	}
	
	if (hit_cr)
		rd_invalidate_below(ev, y);
	else
		rd_invalidate_line(ev, y);
	
	ModifiedSinceRedraw = true;
}


void EditView::action_delete_range(int x1, int y1, int x2, int y2)
{
EditView *ev = this;
clLine *line, *line1, *line2;
int y;
char not_wrapped = 1;

	lstat2("action_delete_range: [%d,%d] - [%d,%d]", x1, y1, x2, y2);
	ev->SetDirty();
	
	// save the to-be-deleted data to the undo buffer
	BString *deldata = ev->RangeToString(x1, y1, x2, y2, "\n");
	undo_add(ev, x1, y1, deldata->Length(), deldata);
	
	// case where last line is selected at x=0
	line = ev->GetLineHandle(y2);
	if (x2 >= line->GetLength())
	{
		if (line->next)
		{
			y2++;
			x2 = 0;
			not_wrapped = 0;
		}
		else
		{
			x2 = line->GetLength() - 1;
		}
	}
	
	// if y1 == y2, simply delete text between x1 and x2.
	if (y1 == y2)
	{
		line->delete_range(x1, x2);
		rd_invalidate_line(ev, y1);
		return;
	}
	
	rd_invalidate_below(ev, y1);
	
	// delete everything on line y1 after x1.
	line1 = ev->GetLineHandle(y1);
	line1->DeleteAfterIndex(x1);
	
	// obtain contents of line y2, and get portion of the text after x2.
	line2 = ev->GetLineHandle(y2);
	BString *line2str = line2->GetLineAsString();
	char *y2str = (char *)line2str->String();
	y2str += (x2 + not_wrapped);
	
	// delete line y2.
	ev->delete_line(line2, y2);
	
	// concatenate string obtained previously onto end of line y1.
	line1->set_insertion_point(line1->GetLength());
	line1->insert_string(y2str);
	
	delete line2str;
	
	// now delete all full lines which lie between y1 and y2.
	y = (y1 + 1);
	if (y < y2)
	{
		line = ev->GetLineHandle(y);
		
		for(;y<y2;y++)
		{
			clLine *next = line->next;
			ev->delete_line(line, y);
			line = next;
		}
	}
	
	ModifiedSinceRedraw = true;
}
