
#include "editor.h"
#include "bmatch.fdh"


void bmatch_update(EditView *ev)
{
clLine *line;
char bracetype;
int bx;

	if (!editor.settings.DoBraceMatching)
		return;
	
	line = ev->GetLineHandle(ev->cursor.y);
	
	// try to find a brace cursor is hovering over
	bracetype = 0;
	if (ev->cursor.x > 0)
	{
		bx = (ev->cursor.x - 1);
		bracetype = is_brace(line, bx);
	}
	
	if (!bracetype)
	{
		bx = ev->cursor.x;
		bracetype = is_brace(line, bx);
	}
	
	// activate or deactive brace highlighting as necessary
	if (bracetype)
	{
		// when moving cursor directly from one brace set to another,
		// deactivate the old brace set.
		if (ev->bmatch.active)
		{
			if (ev->bmatch.match_x != bx ||
				ev->bmatch.match_y != ev->cursor.y)
			{
				DeactivateBrace(ev);
			}
		}
		// v Do not merge this statement into the above via an "else",
		//	 it IS possible for both cases to run.
		if (!ev->bmatch.active)
		{
			ActivateBrace(ev, line, bx, ev->cursor.y, bracetype);
			
			// light up tab lines for { and } braces
			if ((bracetype == '{' || bracetype == '}') && ev->bmatch.matched)
			{
				ev->bmatch.highlight_tab_level = min(ev->bmatch.x1, ev->bmatch.x2);
				rd_invalidate_range(ev, ev->bmatch.y1, ev->bmatch.y2);
			}
		}
		
		if (ev->bmatch.matched)
		{
			clLine *line1, *line2;
			
			line1 = ev->GetLineHandle(ev->bmatch.y1);
			if (ev->bmatch.y1 == ev->bmatch.y2) { line2 = line1; }
			else { line2 = ev->GetLineHandle(ev->bmatch.y2); }
			
			SetBraceColor(line1, ev->bmatch.x1, COLOR_BRACE_MATCHED);
			SetBraceColor(line2, ev->bmatch.x2, COLOR_BRACE_MATCHED);			
		}
		else
		{
			SetBraceColor(ev->GetLineHandle(ev->bmatch.match_y),
						  ev->bmatch.match_x, COLOR_BRACE_UNMATCHED);
		}
	}
	else
	{
		if (ev->bmatch.active)
			DeactivateBrace(ev);
	}
}


static void ActivateBrace(EditView *ev, clLine *line1, int x1, int y1, char bracetype)
{
int x2, y2;
clLine *line2;

	//staterr("activating brace '%c' at [%d,%d]", bracetype, x1, y1);
	
	// scan for the other brace of the pair
	if (IsOpenBrace(bracetype))
	{	// scan forward for end of pair
		line2 = find_end_of_pair(line1, x1, y1, bracetype, InvertBrace(bracetype), &x2, &y2);
		
		ev->bmatch.x1 = x1;
		ev->bmatch.y1 = y1;
		ev->bmatch.x2 = x2;
		ev->bmatch.y2 = y2;
	}
	else
	{	// scan backwards for start of pair
		line2 = find_start_of_pair(line1, x1, y1, bracetype, InvertBrace(bracetype), &x2, &y2);
		
		ev->bmatch.x1 = x2;
		ev->bmatch.y1 = y2;
		ev->bmatch.x2 = x1;
		ev->bmatch.y2 = y1;
	}
	
	ev->bmatch.active = 1;
	ev->bmatch.match_x = x1;
	ev->bmatch.match_y = y1;
	
	if (line2)
	{
		ev->bmatch.matched = 1;
		
		rd_invalidate_line(ev, y1);
		rd_invalidate_line(ev, y2);		
	}
	else
	{		
		ev->bmatch.matched = 0;
		
		rd_invalidate_line(ev, y1);
	}
}

static void DeactivateBrace(EditView *ev)
{
	if (ev->bmatch.active)
	{
		//staterr("deactivating brace set");
		if (ev->bmatch.highlight_tab_level > 0)
		{
			ev->bmatch.highlight_tab_level = -1;
			rd_invalidate_range(ev, ev->bmatch.y1, ev->bmatch.y2);
		}
		
		if (ev->bmatch.matched)
		{
			SetBraceColor(ev->GetLineHandle(ev->bmatch.y1), ev->bmatch.x1, COLOR_BRACE);
			SetBraceColor(ev->GetLineHandle(ev->bmatch.y2), ev->bmatch.x2, COLOR_BRACE);
			rd_invalidate_line(ev, ev->bmatch.y1);
			rd_invalidate_line(ev, ev->bmatch.y2);
		}
		else
		{
			SetBraceColor(ev->GetLineHandle(ev->bmatch.match_y), ev->bmatch.match_x, COLOR_BRACE);
			rd_invalidate_line(ev, ev->bmatch.match_y);
		}
		
		ev->bmatch.active = 0;
		ev->bmatch.highlight_tab_level = -1;
	}
}


/*
void c------------------------------() {}
*/

// searches for the brace which opens a brace set (cursor on ')', search backwards for '(')
clLine *find_start_of_pair(clLine *line, int x, int y, char MoreNestedChar, char LessNestedChar, int *xm, int *ym)
{
	return BraceScan(-1, line, x, y, MoreNestedChar, LessNestedChar, xm, ym);
}

// searches for the brace which closes an open brace (cursor on '(', search for ')')
clLine *find_end_of_pair(clLine *line, int x, int y, char MoreNestedChar, char LessNestedChar, int *xm, int *ym)
{
	return BraceScan(1, line, x, y, MoreNestedChar, LessNestedChar, xm, ym);
}

static clLine *BraceScan(int direction, clLine *line, int x, int y, char MoreNestedChar, char LessNestedChar, int *xm, int *ym)
{
char ch;
int nestlevel = 0;
int linelength;

	//staterr("find_pair: search for '%c' to close '%c' at [%d,%d]", CloseBrace, OpenBrace, x, y);
	
	linelength = line->GetLength();
	rept
	{
		if (direction == -1)
		{
			if (x <= 0)
			{
				if (!y)		// reached beginning of document and no match
					return NULL;
				
				y--;
				line = line->prev;
				linelength = line->GetLength();
				x = (linelength - 1);
				lexer_update_line(line);
			}
			else
			{
				x--;
			}
		}
		else
		{
			x++;
			
			// move to next line when we reach the end
			if (x >= linelength)
			{
				line = line->next;
				if (!line)	// reached end of document and no match
					return NULL;
				
				x = 0; y++;
				linelength = line->GetLength();
				lexer_update_line(line);
			}
		}
		
		// fetch char at check if it's one of the braces we're looking for
		ch = line->GetCharAtIndex(x);
		
		if (ch == MoreNestedChar)
		{
			if (LexerAgreesIsBrace(line, x))
			{
				nestlevel++;
			}
		}
		else if (ch == LessNestedChar)
		{
			if (LexerAgreesIsBrace(line, x))
			{
				if (!nestlevel)
				{
					//staterr("found match at %d,%d", x, y);
					*xm = x;
					*ym = y;
					return line;
				}
				else
				{
					nestlevel--;
				}
			}
		}		
	}
}

/*
void c------------------------------() {}
*/

// returns nonzero if the given char is open of the open brace chars.
static char IsOpenBrace(char ch)
{
	switch(ch)
	{
		case '(':
		case '{':
		case '[': return 1;
	}
	
	return 0;
}

// given an open brace, return the matching closing brace.
// given a closing brace, return the matching open brace.
static char InvertBrace(char ch)
{
	switch(ch)
	{
		case '(': return ')';
		case ')': return '(';
		
		case '{': return '}';
		case '}': return '{';
		
		case '[': return ']';
		case ']': return '[';
	}
	
	return 0;
}

/*
void c------------------------------() {}
*/

static char is_brace(clLine *line, int x)
{
char ch;

	// get which char this is and check if it's a brace char
	switch(ch = line->GetCharAtIndex(x))
	{
		default: return 0;
		
		case '(': case ')':
		case '{': case '}':
		case '[': case ']':
		break;
	}
	
	if (LexerAgreesIsBrace(line, x)) return ch;
	return 0;
}

static char LexerAgreesIsBrace(clLine *line, int x)
{
	// check if the lexer determined that it was a brace
	// (filter out braces inside of quotes, comments, etc)
	LexPoint *lp = FindLexPoint(line, x);
	
	if (!lp) return 0;
	return (lp->type >= COLOR_BRACE && lp->type <= COLOR_BRACE_UNMATCHED);
}

// modify the type of an existing lexeme within a line.
// this is used to change the color of the braces.
static void SetBraceColor(clLine *line, int x, int newcolor)
{
LexPoint *lp;

	lp = FindLexPoint(line, x);
	if (lp) lp->type = newcolor;
}


// finds the lex point whose X is set to a given index within a line, or NULL.
LexPoint *FindLexPoint(clLine *line, int index)
{
LexResult *lr = &line->lexresult;
int npoints = lr->npoints;
	
	for(int i=0;i<npoints;i++)
	{
		if (lr->points[i].index >= index)
		{
			if (lr->points[i].index == index)
				return &lr->points[i];
			
			return NULL;
		}
	}
	
	return NULL;
}
