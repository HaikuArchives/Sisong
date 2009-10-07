
#include "editor.h"
#include "UpdateLine.fdh"


/***********************************************
*  DRAWING										*
*   TO THE										*
*     EDIT PANE									*
************************************************/

#define CX_TO_X(cx)		((cx * editor.font_width) - ev->xscroll)
#define RESET_BUFFER	{ buffer_length = 0; buffer_xstart = CX_TO_X(cx); }

#define DUMP_BUFFER	\
{		\
	view->FillRect(BRect(buffer_xstart, y,		\
					(buffer_xstart + ((buffer_length * editor.font_width) - 1)), \
					line_bottom), B_SOLID_LOW);			\
	\
	FontDrawer->DrawString(view, buffer, buffer_xstart, y, buffer_length);	\
}
//	\
//	BString cheese(buffer, buffer_length); \
//	stat(" ['%s']", cheese.String()); \

// print a given string at the start of the given line.
// the unused portion of the line (at the right) is cleared.
//
// view: the view to drawn into
// ev: the document that the line is from
// line: a handle to the line to update with
// y: the row (Y char-coordinate) in the view at which to draw the line
// line_number: the line number of the line within the document.
//
// returns: the length of the line in pixels.
int UpdateLine(BView *view, EditView *ev, clLine *line, int y, int line_number)
{
char buffer[4096];
int buffer_xstart, buffer_length;
int cx, index, i;
char ch;

int tablines[4096], tablevel[4096];
int ntablines = 0;
int have_non_tab = 0;

rgb_color fg, bg, bg_under_sel;
int CurLexerPoint;
int NextLexerPos;
char lastch;

char *text;
char *gap_start;
char *text_end;
char in_selection = 0;
char last_in_selection = 0;
bool selection_present = ev->selection.present;
int line_bottom = y + (editor.font_height-1);

	// init buffer
	cx = buffer_length = 0;
	buffer_xstart = -editor.curev->xscroll;
	lastch = -1;

	// init text pointers
	text = line->text;
	gap_start = (text + line->gap_start);
	text_end = (text + line->length);
	index = 0;

	// init font/colors
	fg = GetEditFGColor(COLOR_TEXT);
	bg = GetEditBGColor(COLOR_TEXT);
	bg_under_sel = bg;

	view->SetLowColor(bg);
	view->SetHighColor(fg);

	FontDrawer->SetFace(view, GetColorBoldState(COLOR_TEXT) ? B_BOLD_FACE : B_REGULAR_FACE);

	// get first color-change point
	if (line->lexresult.npoints > 0 && !editor.settings.DisableLexer)
	{
		NextLexerPos = line->lexresult.points[0].index;
		CurLexerPoint = 0;
	}
	else
	{
		NextLexerPos = 0x7fffffff;
	}

	rept
	{
		// change background color when entering/exiting a selected area
		if (selection_present)
		{
			in_selection = selection_IsPointWithin(ev, line_number, index);

			// selection
			if (in_selection != last_in_selection)
			{
				DUMP_BUFFER; RESET_BUFFER;
				last_in_selection = in_selection;

				if (in_selection)
					bg = GetEditColor(COLOR_SELECTION);
				else
					bg = bg_under_sel;

				view->SetLowColor(bg);
			}
		}

		// syntax coloring
		if (index == NextLexerPos)
		{
			// get color we're changing to
			int new_color_code = line->lexresult.points[CurLexerPoint].type;

			// dump anything left over from the old colorset
			DUMP_BUFFER; RESET_BUFFER;

			// change colors
			fg = GetEditFGColor(new_color_code);
			bg_under_sel = GetEditBGColor(new_color_code);
			if (!in_selection) bg = bg_under_sel;

			view->SetLowColor(bg);
			view->SetHighColor(fg);

			// change font face if necessary
			FontDrawer->SetFace(view, \
				GetColorBoldState(new_color_code) ? B_BOLD_FACE : B_REGULAR_FACE);

			// move to next color-change point
			if (++CurLexerPoint >= line->lexresult.npoints)
				NextLexerPos = 0x7fffffff;
			else
				NextLexerPos = line->lexresult.points[CurLexerPoint].index;
		}

		// skip over gap
		if (text == gap_start)
			text = (line->text + line->gap_end + 1);
		// stop at EOL
		if (text >= text_end)
			break;

		ch = *(text++);
		index++;

		if (ch == TAB)
		{
			// record the positions where tablines need to be drawn.
			// * don't draw tabs in column 0
			// * only draw indentation tabs; don't draw tabs once any text is seen.
			if (!have_non_tab && lastch == TAB)
			{
				if (cx > 0)
				{
					tablines[ntablines] = (buffer_xstart + 2);
					tablevel[ntablines] = (index - 1);
					ntablines++;
				}
			}

			// dump anything queued in the buffer so far
			if (buffer_length)
			{
				DUMP_BUFFER;
				RESET_BUFFER;
			}

			// save current pos so we know where to clear from
			int clear_start = buffer_xstart;

			// advance to next tab mark
			cx += TAB_WIDTH;
			cx -= (cx % TAB_WIDTH);

			// recalc buffer X start, since we skipped some space for the tab
			RESET_BUFFER;

			// clear the space occupied by the tab
			view->FillRect(BRect(clear_start, y, buffer_xstart-1, line_bottom), B_SOLID_LOW);
		}
		else
		{
			buffer[buffer_length++] = ch;
			have_non_tab = 1;
			cx++;
		}

		lastch = ch;
	}

	// dump anything still in the buffer
	if (buffer_length)
		DUMP_BUFFER;

	// draw the tab lines
	if (editor.settings.DrawTabLines)
	{
		if (ev->bmatch.highlight_tab_level > 0 && \
			line_number >= ev->bmatch.y1 && \
			line_number <= ev->bmatch.y2)
		{	// if here, some tab lines are lit up due to brace matching
			for(i=0;i<ntablines;i++)
			{
				if (tablevel[i] == ev->bmatch.highlight_tab_level)
				{
					DrawTabLine(view, tablines[i], y, COLOR_TABLINE_ACTIVE);
				}
				else
				{
					DrawTabLine(view, tablines[i], y, COLOR_TABLINE);
				}
			}
		}
		else
		{
			for(i=0;i<ntablines;i++)
				DrawTabLine(view, tablines[i], y, COLOR_TABLINE);
		}
	}

	// if we left the line while still in selection,
	// draw 1 extra char worth of selection
	if (in_selection)
	{
		int x = CX_TO_X(cx);
		cx++;

		view->SetLowColor(GetEditColor(COLOR_SELECTION));
		view->FillRect(BRect(x, y, x+(editor.font_width-1), line_bottom), B_SOLID_LOW);
	}

	// clear the unused rest of the line
	int line_width = CX_TO_X(cx);

	view->SetLowColor(GetEditBGColor(COLOR_TEXT));
	view->FillRect(BRect(line_width, y, editor.pxwidth-1, line_bottom), B_SOLID_LOW);

	return (line_width + editor.curev->xscroll);
}

// performs identically to UpdateLine, except that it draws the line via the
// "curline_bb" backbuffer and updates "editor.bbed_line".
// just like UpdateLine, it's return value is the width of the line in pixels
int UpdateLineBB(BView *view, EditView *ev, clLine *line, int y, int line_number)
{
int line_width;

	editor.curline_bb->Lock();

	line_width = UpdateLine(editor.curline_bb, ev, line, 0, line_number);

	// blit the portion of the line that contains actual text
	BRect source(0, 0, line_width-1, editor.font_height-1);
	BRect dest(0, y, line_width-1, y+(editor.font_height-1));

	editor.curline_bb->BlitTo(view, source, dest);

	// fill in the unused area to the right of the line
	view->SetLowColor(GetEditBGColor(COLOR_TEXT));
	view->FillRect(BRect(line_width, y, editor.pxwidth-1, y+(editor.font_height-1)), B_SOLID_LOW);

	editor.curline_bb->Unlock();
	editor.bbed_line = line_number;

	return line_width;
}

/*
void c------------------------------() {}
*/

// draws the "tab lines" showing indentation
void DrawTabLine(BView *view, int x, int y, int UsingColorIndex)
{
static const pattern pAlternating = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };

	view->SetHighColor(GetEditColor(UsingColorIndex));
	view->SetLowColor(GetEditBGColor(COLOR_TEXT));

	view->FillRect(BRect(x, y, x, y + (editor.font_height - 1)), pAlternating);
}

// clears all lines below line Y
void CEditPane::ClearBelow(int lineNo)
{
	int y = lineNo * editor.font_height;

	SetHighColor(GetEditBGColor(COLOR_TEXT));
	FillRect(BRect(0, y, editor.pxwidth-1, editor.pxheight-1));
}




