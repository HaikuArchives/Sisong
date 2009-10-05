
#include "editor.h"
#include "edit.fdh"

static int NextDocID = 0x00;


EditView::EditView()
{
	stat("EditView::EditView: should fix the need to memset() myself; is not C++ way");
	memset(this, 0, sizeof(EditView));
	
	DocID = NextDocID++;
	cursor.ev = this;
	CloseAfterSave = false;
}

void EditView::SetDirty()
{
	if (this == editor.curev)
	{
		FunctionList->ResetTimer();
		AutoSaver_ResetTimer();
	}

	if (!IsDirty)
	{
		IsDirty = 1;
		TabBar->SetDirtyState(this, true);
	}	
}

void EditView::ClearDirty()
{
	if (IsDirty)
	{
		IsDirty = 0;
		TabBar->SetDirtyState(this, false);
	}
}


// open a new editor tab.
// filename: specify the name of a file to load. if NULL, a new document is created.
EditView *CreateEditView(const char *filename)
{
EditView *ev;
	
	if (filename)
	{
		if (!(ev = OpenDocument(filename)))
			return NULL;
	}
	else
	{	// create new empty document here
		ev = CreateEmptyDocument();
	}
	
	undo_init(ev);
	
	ev->curline = ev->firstline;
	ev->scroll.topline = ev->firstline;

	ev->cursor.x = ev->cursor.y = 0;
	ev->scroll.y = 0;
	
	ev->ModifiedSinceRedraw = true;
	
	for(int i=0;i<6;i++)
	{
		if (ev->curline->next)
		{
			ev->curline = ev->curline->next;
			ev->cursor.y++;
		}
	}
	
	ev->cursor.xseekmode = CM_FREE;
	UpdateCursorPos(ev);
	
	editor.DocList->AddItem(ev);
	TabBar->AddTab(ev);
	
	return ev;
}

void CloseEditView(EditView *ev)
{
	if (ev == editor.curev)
	{
		// must always be something open
		if (TabBar->GetTabCount() <= 1)
			TabBar->SetActiveTab(CreateEditView(NULL));
	}
	
	edit_CloseDocument(ev);
}

// closes the edit view, prompting the user to save it first
// if it is dirty. returns one of:
//		CEV_CLOSED_SAVED
//		CEV_CLOSED_NOT_SAVED
//		CEV_CLOSE_CANCELED
int ConfirmCloseEditView(EditView *ev, bool OfferCancelButton=true)
{
BAlert *alert;
char alert_str[8000];
bool need_to_save = false;
int result;

	if (ev->IsDirty)
	{
		// important so save works properly and nice for UI since
		// document being saved is visible: switch to document.
		TabBar->SetActiveTab(ev);
		
		sprintf(alert_str, "Unsaved changes to \"%s\"; save them now?", GetFileSpec(ev->filename));
		
		if (OfferCancelButton)
		{
			alert = new BAlert("", alert_str, "Cancel", "Don't Save", "Save Now");
			alert->SetShortcut(0, B_ESCAPE);
			alert->SetShortcut(1, 'n');
			alert->SetShortcut(2, 'y');
			result = alert->Go();
			
			if (result==0) return CEV_CLOSE_CANCELED;
			if (result==2) need_to_save = true;
		}
		else
		{
			alert = new BAlert("", alert_str, "Don't Save", "Save Now");			
			alert->SetShortcut(0, 'n');
			alert->SetShortcut(1, 'y');
			if (alert->Go() == 1) need_to_save = true;
		}
		
		if (need_to_save)
		{
			if (FileSave())
				return CEV_CLOSE_CANCELED;
			
			// temphack to work around the fact that the dialog is asynchronous
			if (editor.curev->IsUntitled)
			{
				editor.curev->CloseAfterSave = true;
				return CEV_CLOSE_CANCELED;
			}
		}
	}
	
	CloseEditView(ev);
	return need_to_save ? CEV_CLOSED_SAVED : CEV_CLOSED_NOT_SAVED;
}

static EditView *OpenDocument(const char *fname)
{
FILE *fp;
EditView *ev;
char buf[1024];
clLine *line;

	stat("opening document '%s'", fname);
	if (!(fp = fopen(fname, "rb")))
	{
		staterr("failed to open '%s'", fname);
		return NULL;
	}
	
	ev = new EditView;
	
	while(!feof(fp))
	{
		fgetline(fp, buf, sizeof(buf));
		
		line = new clLine(buf);
		line->next = NULL;
		line->prev = ev->lastline;
		
		if (ev->lastline)
			ev->lastline->next = line;			
		else
			ev->firstline = line;
		
		ev->lastline = line;
		lexer_update_line(line);
		
		ev->nlines++;
	}
	
	fclose(fp);
	
	maxcpy(ev->filename, fname, sizeof(ev->filename) - 1);
	maxcpy(editor.last_filepath_reference, fname, sizeof(editor.last_filepath_reference) - 1);
	return ev;
}

static EditView *CreateEmptyDocument()
{
EditView *ev = new EditView;
	
	ev->firstline = ev->lastline = new clLine;
	ev->firstline->prev = NULL;
	ev->lastline->next = NULL;
	ev->nlines = 1;
	
	ev->MakeUntitled();
	return ev;
}

void EditView::MakeUntitled()
{
	sprintf(this->filename, "new %d", ++editor.NextUntitledID);
	this->IsUntitled = true;
}

void edit_CloseDocument(EditView *ev)
{
	TabBar->RemoveTab(ev);
	editor.DocList->RemoveItem(ev);
	
	undo_close(ev);
	
	clLine *line = ev->firstline, *next;
	while(line)
	{
		next = line->next;
		delete line;
		line = next;
	}
	
	delete ev;
}

// saves the document to the given file.
// returns true if an error occurs.
bool EditView::Save(const char *filename)
{
EditView *ev = this;
static const char newline[] = { '\n', 0 };
char *buffer;
int buffer_sz = 1024;
clLine *line;
FILE *fp;

	if (!(fp = fopen(filename, "wb")))
	{
		staterr("edit_save: failed to open file '%s'", filename);
		return true;
	}

	// this buffer should be start out large enough to hold most lines,
	// although it will grow if needed.
	buffer = (char *)smal(buffer_sz);
	
	line = ev->firstline;
	if (line)
	{
		rept
		{
			int linelength = line->GetLength();
			if (linelength >= buffer_sz - 1)
			{
				buffer_sz = (linelength + 80);
				// we don't care about contents of buffer, so i avoid "resmal"
				frees(buffer);
				buffer = (char *)smal(buffer_sz);
			}
			
			line->GetLineToBuffer(buffer);
			fwrite(buffer, linelength, 1, fp);
			
			line = line->next;
			
			if (line)
				fputs(newline, fp);
			else
				break;
		}
	}
	
	frees(buffer);
	fclose(fp);

	return false;
}


// "nice" version of CreateEditView(filename)
// opens the file and switches to it. if the file is already
// open activates the existing copy.
EditView *OpenFile(const char *filename)
{
EditView *ev;

	LockWindow();
	
	if (!(ev = FindEVByFilename(filename)))
		ev = CreateEditView(filename);
	
	if (ev)
		TabBar->SetActiveTab(ev);
	
	UnlockWindow();
	return ev;
}

// moves to the given line in the given file, opening the file if necessary.
// x_start and x_end specify the characters within the line which are to be selected.
// if x_start is -1, characters are selected from the line's indentation level.
// if x_end is -1, characters are selected from x_start to the end of the line.
EditView *OpenFileAtLine(const char *filename, int lineNo, int x_start, int x_end)
{
EditView *ev;
	
	LockWindow();
	
	ev = FindEVByFilename(filename);
	if (!ev)	// file already open?
	{
		// no, try to open it
		if (file_exists(filename))
		{
			ev = CreateEditView(filename);
		}
		else
		{
			UnlockWindow();
			
			(new BAlert("", "Unable to display the relevant line, because that document could not be opened.", "Blast!"))->Go();
			return NULL;
		}
	}
	
	if (lineNo >= ev->nlines) lineNo = ev->nlines - 1;
	if (lineNo < 0) lineNo = 0;
	
	// try to center the relevant line in the view
	int top_y_line = lineNo;
	top_y_line -= (editor.height / 2);
	if (top_y_line < 0) top_y_line = 0;	
	
	ev->SetVerticalScroll(top_y_line);	

	// select the hit
	DocPoint hit_start, hit_end;
	int line_length;
	clLine *line;
	
	line = ev->GetLineHandle(lineNo);
	line_length = line->GetLength();
	
	if (x_start < 0) x_start = line->GetIndentationLevel();
	if (x_end < 0) x_end = line_length-1;
	
	if (x_start > line_length) x_start = line_length;
	if (x_end > line_length) x_end = line_length;
	if (x_start > x_end) SWAP(x_start, x_end);
	
	hit_start.Set(ev, x_start, lineNo);
	hit_end.Set(ev, x_end, lineNo);
	
	selection_select_range(ev, &hit_start, &hit_end);
	
	ev->cursor.set_mode(CM_FREE);
	if (x_end >= line_length)
	{
		ev->cursor.move(hit_start.x, hit_start.y);
	}
	else
	{
		hit_end.Increment();
		ev->cursor.move(hit_end.x, hit_end.y);
	}
	
	// make document active
	if (ev != editor.curev)
		TabBar->SetActiveTab(ev);
	else
		ev->FullRedrawView();
	
	UnlockWindow();
	return ev;
}

void EditView::ReloadFile()
{
EditView *ev = this;
EditView *newme;

	newme = CreateEditView(ev->filename);
	if (!newme) return;
	
	newme->SetVerticalScroll(ev->scroll.y);
	newme->cursor.move(ev->cursor.x, ev->cursor.y);
	
	if (ev == editor.curev)
	{
		TabBar->SetActiveTab(newme);
	}
	
	CloseEditView(ev);
}

/*
void c------------------------------() {}
*/

void editor_save_all(void)
{
int i;
int count = editor.DocList->CountItems();

	for(i=0;i<count;i++)
	{
		EditView *ev = (EditView *)editor.DocList->ItemAt(i);
		
		if (ev->IsDirty && !ev->IsUntitled)
		{
			ev->Save(ev->filename);
			ev->ClearDirty();
		}
	}
}

// WARNING:
// this creates the illegal condition of NO documents being open.
// bad idea to call it except in certain special circumstances (such as shutdown).
void editor_close_all(void)
{
	stat("editor_close_all()");
	
	// load_layout() uses us to close all docs then opens new ones.
	// make sure current tab is set to null to avoid bugs in the tab bar
	// when adding the new documents back in.
	TabBar->SetActiveTab(NULL);
	editor.curev = NULL;
	
	// close all documents
	while(editor.DocList->CountItems())
	{
		EditView *c = (EditView *)editor.DocList->FirstItem();
		edit_CloseDocument(c);
	}
	
	editor.NextUntitledID = 0;
}

/*
void c------------------------------() {}
*/

// searches for an EditView structure given it's document id
EditView *FindEVByDocID(uint id)
{
int i;
int count = editor.DocList->CountItems();

	for(i=0;i<count;i++)
	{
		EditView *ev = (EditView *)editor.DocList->ItemAt(i);
		if (ev->DocID==id) return ev;
	}
	
	return NULL;
}

EditView *FindEVByFilename(const char *filename)
{
int i;
int count = editor.DocList->CountItems();

	for(i=0;i<count;i++)
	{
		EditView *ev = (EditView *)editor.DocList->ItemAt(i);
		
		if (!ev->IsUntitled)
		{
			if (!strcmp(ev->filename, filename))
				return ev;
		}
	}
	
	return NULL;
}

/*
void c------------------------------() {}
*/

// insert a line into a document.
//
// ev - the document
// initial_string - initial contents of the line
// insertafter - line is inserted immediately after this line.
// to insert at beginning, pass NULL.
// y - y coordinate of "insertafter" line.
void EditView::insert_line(char *initial_string, clLine *insertafter, int y)
{
EditView *ev = this;
clLine *line;

	line = new clLine(initial_string);
	
	if (insertafter)
	{	// insert after
		line->prev = insertafter;
		line->next = insertafter->next;
		
		if (insertafter->next)
			insertafter->next->prev = line;
		else
			ev->lastline = line;
		
		insertafter->next = line;
	}
	else
	{	// insert at beginning
		
		if (ev->firstline) ev->firstline->prev = line;
		else ev->lastline = line;
		
		line->prev = NULL;
		line->next = ev->firstline;
		
		ev->firstline = line;
	}

	ev->nlines++;
	
	/* prevent line pointers from desync'ing
	    0 LINE A
		  <----- insertion here  cursor Y is still 1, but curline points to line 2
	  > 1 LINE B
		2 LINE C
	*/
	
	if (ev->cursor.y > y)
		ev->curline = ev->curline->prev;
	if (ev->scroll.y > y)
		ev->scroll.topline = ev->scroll.topline->prev;
}

// delete a line from the document
void EditView::delete_line(clLine *line, int y)
{
EditView *ev = this;

	// remove line from linked list
	if (line->prev)
		line->prev->next = line->next;
	else
		ev->firstline = line->next;
	
	if (line->next)
		line->next->prev = line->prev;
	else
		ev->lastline = line->prev;
	
	ev->nlines--;
	
	// -- fixup line pointers to handle the deletion --
	// cursor
	if (ev->cursor.y >= ev->nlines)
	{
		ev->cursor.y = (ev->nlines - 1);
		ev->curline = ev->lastline;
	}
	else if (ev->cursor.y >= y)
	{
		ev->curline = ev->curline->next;
	}

	// scroll Y
	int max = ev->GetMaxScroll();
	if (ev->scroll.y >= max)
	{
		ev->scroll.y = max;
		ev->scroll.topline = ev->GetLineHandleFromEnd(max);
	}
	else if (ev->scroll.y >= y)
	{
		ev->scroll.topline = ev->scroll.topline->next;
	}
	
	//staterr("SY:%d   PREDICTED: %x  ACTUAL: %x", ev->scroll.y, ev->scroll.topline, ev->GetLineHandleFromStart(ev->scroll.y));
//	staterr("%d:  %x/%x", ev->scroll.y, ev->scroll.topline, ev->GetLineHandleFromEnd(ev->scroll.y));
	delete line;
}


/*
void c------------------------------() {}
*/

// obtains the clLine structure for a given line number.
clLine *EditView::GetLineHandle(int y)
{
EditView *ev = this;

	if (y == ev->cursor.y) return ev->curline;
	
	if (y < 0) y = 0;
	if (y >= ev->nlines) y = ev->nlines-1;
	
	int dist_start = y;
	int dist_end = (ev->nlines - 1) - y;
	int dist_cursor = abs(ev->cursor.y - y);
	
	if (dist_cursor < dist_start)
	{
		if (dist_cursor < dist_end)
			return GetLineHandleFromCursor(y);
	}
	else
	{
		if (dist_start <= dist_end)
			return GetLineHandleFromStart(y);
	}
	
	return GetLineHandleFromEnd(y);
}

clLine *EditView::GetLineHandleFromStart(int y)
{
clLine *curline = this->firstline;

	while(y > 0)
	{
		curline = curline->next;
		y--;
	}
	
	return curline;
}

clLine *EditView::GetLineHandleFromEnd(int y)
{
clLine *curline = this->lastline;

	y = (this->nlines - 1) - y;	
	while(y > 0)
	{
		curline = curline->prev;
		y--;
	}
	
	return curline;
}

clLine *EditView::GetLineHandleFromCursor(int y)
{
EditView *ev = this;
clLine *curline;

	curline = ev->curline;
	
	if (y > ev->cursor.y)
	{
		rept
		{
			curline = curline->next;
			if (--y == ev->cursor.y) return curline;
		}
	}
	else if (y < ev->cursor.y)
	{
		rept
		{
			curline = curline->prev;
			if (++y == ev->cursor.y)
			{
				return curline;
			}
		}
	}
	
	return curline;
}

/*
void c------------------------------() {}
*/

// return a String containing a specified portion of the document.
// [x1, y1] - [x2, y2]: defines the range of text to include in the string
// crlf_seq: the string with which each line is to be terminated.
BString *EditView::RangeToString(int x1, int y1, int x2, int y2, char *crlf_seq)
{
int y;
clLine *line;
BString *totals;

	totals = new BString;
	
	line = GetLineHandle(y1);
	for(y=y1;;y++)
	{
		BString *string = line->GetLineAsString();
		
		if (y==y2)
		{
			DeleteAfter(string, x2);
		}
		
		if (y==y1)
		{
			if (x1) DeleteBefore(string, x1);
		}
		else
		{
			totals->Append(crlf_seq);
		}
		
		totals->Append(*string);
		delete string;
		
		if (y == y2) break;
		line = line->next;
	}
	
	if (x2 >= line->GetLength())
		totals->Append(crlf_seq);
	
	//stat("RangeToString: '%s'", totals->String());
	return totals;
}

// given a position [x,y], returns the position "count" chars forward of that position.
void EditView::AddToDocPoint(int x, int y, clLine *line, int count, int *x_out, int *y_out)
{
	rept
	{
		int line_remain = (line->GetLength() - x) + 1;
		
		if (count <= line_remain)
		{
			*x_out = (x + count);
			break;
		}
		else
		{
			count -= line_remain;
			x = 0; y++;
			line = line->next;
		}
	}
	
	*y_out = y;
}


/*
void c------------------------------() {}
*/

void EditView::scroll_up(int nlines)
{
	if (scroll.y < nlines)
		nlines = scroll.y;

	scroll.y -= nlines;
	
	while(nlines--)
		scroll.topline = scroll.topline->prev;
}

void EditView::scroll_down(int nlines)
{
int max;

	max = this->GetMaxScroll();
	
	if ((scroll.y + nlines) > max)
		nlines = (max - scroll.y);
	
	scroll.y += nlines;
	
	while(nlines > 0)
	{
		scroll.topline = scroll.topline->next;
		nlines--;
	}
}

// returns the lowest possible scroll position of the document
int EditView::GetMaxScroll()
{
int max;

	max = (this->nlines - editor.height);
	if (max < 0) max = 0;
	return max;
}

// sets the vertical scroll position (top visible line) to a particular value
void EditView::SetVerticalScroll(int y)
{
int diff;
int maxScroll = GetMaxScroll();

	if (y < 0) y = 0;
	if (y > maxScroll) y = maxScroll;
	
	diff = (y - this->scroll.y);
	
	if (diff > 0)
	{
		scroll_down(diff);
	}
	else if (diff < 0)
	{
		scroll_up(-diff);
	}
}


// bring a given line into view by scrolling up or down as necessary.
//
// attempts to bring the given line to the same Y coordinate as that of the cursor.
void EditView::BringLineIntoView(int y, int vismode, int target_screen_y)
{
EditView *ev = this;
int line_screen_y;
int window_height;

	//staterr("BringLineIntoView: requested to visibilize line %d, cursor @ %d", y, ev->cursor.y);
	
	// sanity checking of input, if they asked for a line outside the document
	ENSURE_LINE_IN_RANGE(y);
	
	// get current screen position of the requested line
	line_screen_y = (y - ev->scroll.y);
	
	window_height = editor.height;
	if (editor.PartialLineAtBottom) window_height--;
	if (editor.HozBarVisible) window_height--;
	
	// if the line is already visible, we're done
	if (line_screen_y >= 0 && line_screen_y < window_height)
	{
		// FORCE_SPECIFIC_Y always tries to move the document if possible,
		// used for Find command.
		if (vismode != BV_FORCE_SPECIFIC_Y)
			return;
	}
	
	//stat(" ** Visibilizing line: y=%d, vismode=%d, target_screen_y=%d", y, vismode, target_screen_y);
	
	switch(vismode)
	{
		// bring line to top or bottom of display, as needed
		case BV_SIMPLE:
			if (line_screen_y < 0)
				target_screen_y = 0;
			else
				target_screen_y = (window_height - 1);
		break;
		
		// try to bring the line to a specified Y coordinate
		case BV_SPECIFIC_Y:
		case BV_FORCE_SPECIFIC_Y:
			// if the target Y is valid, go ahead and use it,
			// else try for center of screen instead.
			if (target_screen_y >= 0 && target_screen_y < window_height)
			{
				break;
			}
			// fall-thru
		// center line on display
		case BV_CENTERED:
			target_screen_y = (window_height / 2);
		break;		
	}
	
	// how many lines are between the two?
	int dist = (line_screen_y - target_screen_y);
	if (dist != 0)
	{
		ev->scroll.y += dist;
		
		// set new scroll position
		ENSURE_LINE_IN_RANGE(ev->scroll.y);
		ev->scroll.topline = ev->GetLineHandle(ev->scroll.y);
	}
}

/*
void c------------------------------() {}
*/

// returns the extents of the word the cursor is currently within
void EditView::GetCurrentWordExtent(int *x1_out, int *x2_out)
{
	curline->GetWordExtent(cursor.x, x1_out, x2_out);
}

/*
void c------------------------------() {}
*/

void EditView::SetXScroll(int newvalue)
{
	if (this->xscroll != newvalue)
	{
		this->xscroll = newvalue;
		
		scrHorizontal->SetValue(newvalue);
		rd_invalidate_all(this);
	}
}

// checks if the cursor is currently within the horizontal scroll pane,
// and if not, adjusts the horizontal scroll position so that it is.
void EditView::XScrollToCursor()
{
EditView *ev = this;
int hmin, hmax, cursor_x, cursor_px;
int xscroll;
#define XS_JUMP		60

	// get leftmost and rightmost visible column
	xscroll = ev->xscroll;

	hmin = (ev->xscroll / editor.font_width);
	hmax = (hmin + editor.width);
	
	UpdateCursorPos(ev);
	cursor_x = ev->cursor.screen_x;
	
	//staterr("cursor_x=%d,  hmin/max=%d-%d", cursor_x,hmin,hmax);
	if (cursor_x >= hmax)
	{
		cursor_px = (cursor_x * editor.font_width);
		xscroll = (cursor_px - editor.pxwidth) + XS_JUMP;
		if (xscroll < 0) xscroll = 0;
	}
	else if (cursor_x <= hmin && ev->xscroll > 0)
	{
		cursor_px = (cursor_x * editor.font_width);
		xscroll = (cursor_px - XS_JUMP);
		if (xscroll < 0) xscroll = 0;
	}
	
	if (xscroll != ev->xscroll)
	{
		SetXScroll(xscroll);
	}
}

// handles some stuff having to do with redraw during vertical scrolling operations.
// given oldY and newY, checks if the old and new visible portions of the document
// overlap. if so, invalidates only the newly visible portions, then uses CopyBits
// to transfer the unchanged area without redrawing.
void InvalidateForYScroll(EditView *ev, int oldY1, int newY1)
{
int oldY2, newY2;
int common_start, common_btm;
int delta_px;

	if (oldY1 == newY1)
		return;
	
	// if data was inserted/removed, don't risk it
	if (ev->nlines != ev->lastnlines)
	{
		rd_invalidate_all(ev);
		return;
	}
	
	// get the visible areas before and after
	oldY2 = oldY1 + (editor.height - 1);
	newY2 = newY1 + (editor.height - 1);
	
	//stat("doc scroll: old[%d-%d]  new[%d-%d]", oldY1,oldY2,newY1,newY2);
	
	if (newY1 > oldY1)
	{
		// if the regions don't overlap, can't CopyBits, so just invalidate everything
		if (newY1 > oldY2)
		{
			rd_invalidate_all(ev);
			return;
		}		
		
		if (MainView->cursor.visible)
			MainView->cursor.erase();
		
		common_start = (newY1 - oldY1) * editor.font_height;
		int h = editor.pxheight - 1;
		
		BRect source(0, common_start, editor.pxwidth - 1, h);
		BRect dest(0, 0, editor.pxwidth - 1, h - common_start);
		
		MainView->CopyBits(source, dest);
		rd_invalidate_range(ev, (oldY2 + !editor.PartialLineAtBottom), newY2);
	}
	else
	{
		// if the regions don't overlap, can't CopyBits, so just invalidate everything
		if (newY2 < oldY1)
		{
			rd_invalidate_all(ev);
			return;
		}
		
		if (MainView->cursor.visible)
			MainView->cursor.erase();
		
		delta_px = (oldY1 - newY1) * editor.font_height;
		
		common_btm = (editor.pxheight - 1);
		common_btm -= delta_px;
		
		BRect source(0, 0, editor.pxwidth - 1, common_btm);
		BRect dest(0, delta_px, editor.pxwidth - 1, common_btm + delta_px);
		
		MainView->CopyBits(source, dest);
		rd_invalidate_range(ev, newY1, oldY1 - !editor.PartialLineAtBottom);
	}
}

/*
void c------------------------------() {}
*/

void EditView::FullRedrawView()
{	
	// this *evil hack* avoids a bug where switching from one document
	// to another could cause a false scroll in the destination
	// document.
	ignore_scrollbars++;
	
	rd_invalidate_all(this);
	RedrawView();
	//edit_UpdateHozScrollBar(this);
	
	ignore_scrollbars--;
}

void EditView::RedrawView()
{
EditView *ev = this;
int i, y;
char is_fullredraw = rd_fullredraw();
char redraw_line_numbers = is_fullredraw;

	//BStopWatch *w = new BStopWatch("Redraw");
	//stat("redraw");
	
	if (!app_running) return;
	LockWindow();

	/*
		SCROLLING
	*/
	
	// sanitize scroll position (just in case)
	if (ev->scroll.y > ev->GetMaxScroll())
	{
		staterr("had to sanitize scroll position");
		ev->scroll.y = ev->GetMaxScroll();
		ev->scroll.topline = ev->GetLineHandleFromEnd(ev->scroll.y);
	}

	// double-check to ensure cursor screen position is up-to-date
	UpdateCursorPos(ev);
	
	// if number of lines has changed, update the "max" value of the
	// vertical scrollbar.
	if (ev->nlines != ev->lastnlines || is_fullredraw)
	{
		UpdateVertScrollBarRange(false);
	}

	// if any vertical scrolling has occurred then we must redraw the whole screen
	if (ev->scroll.y != ev->scroll.lasty)
	{
		scrVertical->SetValue(ev->scroll.y);
		
		InvalidateForYScroll(ev, ev->scroll.lasty, ev->scroll.y);
		redraw_line_numbers = true;
		
		FunctionList->UpdateSelectionHighlight();
	}

	/*
		CURSOR
	*/
	
	// get current screen position of cursor
	bool cursor_need_erased = false;
	bool cursor_need_drawn = false;
	bool cursor_moved;
	
	// has the document cursor moved? if so, move the onscreen cursor to match
	int cursor_old_screen_x = MainView->cursor.get_x();
	int cursor_old_screen_y = MainView->cursor.get_y();
	
	if (ev->cursor.screen_x != cursor_old_screen_x || \
		ev->cursor.screen_y != cursor_old_screen_y)
	{
		MainView->cursor.move(ev->cursor.screen_x, ev->cursor.screen_y);
		
		// if cursor is currently visible when we move it, then after we're
		// done drawing we need to ensure it's erased from it's current position.
		if (MainView->cursor.visible)
			cursor_need_erased = true;		
		
		cursor_moved = true;
		cursor_need_drawn = true;
	}
	else
	{
		cursor_moved = false;
		
		if (!MainView->cursor.visible)
		{	// definitely needs drawn, because we are about to "bump" it
			cursor_need_drawn = true;
		}
	}

	// "bump" cursor (force to visible for next full cycle)
	MainView->cursor.bump();
	
	/*
		LEX, BRACE MATCHING, AND DRAW
	*/

	// re-lex dirty lines
	int NumVisibleLines = min(editor.height, ev->nlines);
	int CurrentLineY = (ev->cursor.y - ev->scroll.y);
	clLine *line = ev->scroll.topline;
	
	for(i=0;i<NumVisibleLines;i++)
	{
		if (rd_is_line_dirty(i))
		{
			int OldExitState = line->lexresult.exitstate;
			
			lexer_update_line(line);
			
			// if exit state is different perpetuate invalidity down
			// to next line, for example when a new block comment is opened.
			if (line->lexresult.exitstate != OldExitState &&
				line->next && i+1 < NumVisibleLines)
			{
				rd_invalidate_line(ev, ev->scroll.y+i+1);
			}
		}
		
		line = line->next;
	}
	
	// brace matching
	if (cursor_moved)
	{
		bmatch_update(ev);
	}
	else if (CurrentLineY >= 0 && CurrentLineY < NumVisibleLines)
	{
		if (rd_is_line_dirty(CurrentLineY))
			bmatch_update(ev);
	}
	
	// redraw all dirty lines
	int line_number = ev->scroll.y;
	
	line = ev->scroll.topline;
	for(i=y=0;i<NumVisibleLines;i++)
	{
		if (rd_is_line_dirty(i))
		{			
			if (line != curline)
			{
				line->pxwidth = UpdateLine(MainView, ev, line, y, line_number);
			}
			else
			{
				line->pxwidth = UpdateLineBB(MainView, ev, line, y, line_number);
				
				// if we drew over the line the cursor is on now,
				// we definitely need to draw it.
				cursor_need_drawn = true;
			}
			
			// if we drew over the line the cursor used to be on,
			// we don't have to erase it, if we were planning to.
			if (i == cursor_old_screen_y)
				cursor_need_erased = false;
		}
		
		line = line->next;
		y += editor.font_height;
		line_number++;
	}

	// if document does not fill up entire window,
	// we must clear the unused space anytime a line is deleted.
	if (NumVisibleLines < editor.height)
	{
		if (ev->nlines < ev->lastnlines || is_fullredraw)
		{
			MainView->ClearBelow(NumVisibleLines);
		}
	}
	
	// when adding lines to a document which is less than 1 page high
	// we must always add in the new line numbers.
	if (ev->nlines <= editor.height && ev->nlines > ev->lastnlines)
		redraw_line_numbers = true;

	// update line numbers if necessary
	if (redraw_line_numbers)
	{
		line_number = ev->scroll.y;
		
		for(int i=0;i<NumVisibleLines;i++)
			ln_panel->SetLineNumber(i, ++line_number);
	}
	
	ln_panel->SetNumVisibleLines(NumVisibleLines);
	ln_panel->RedrawIfNeeded();
	
	// update range of H scroll bar
	if (ev->ModifiedSinceRedraw || \
		ev->scroll.y != ev->scroll.lasty)
	{
		ev->UpdateHozScrollBarRange(is_fullredraw);
	}
	
	// ensure erasure of old copy of cursor
	if (cursor_need_erased)
	{
		MainView->cursor.erase(cursor_old_screen_x, \
							   cursor_old_screen_y);
	}
	
	if (cursor_need_drawn)
		MainView->cursor.draw();
	
	// finish up
	ev->lastnlines = ev->nlines;
	ev->scroll.lasty = ev->scroll.y;
	ev->ModifiedSinceRedraw = false;
	rd_clear_dirty_bits(ev);
	
	UnlockWindow();
	
	//delete w;
	//fflush(stdout);
}

// calculates the range of the horizontal scrollbar, and sets it.
// if force_set is true, it always updates the horizontal bar with the results,
// otherwise, it only updates if the values have changed.
void EditView::UpdateHozScrollBarRange(bool force_set)
{
EditView *ev = this;
int NumVisibleLines;
int longest = 0;
clLine *line;

	NumVisibleLines = min(editor.height, ev->nlines);
	line = ev->scroll.topline;
	
	// i wanted to come up with some genius algorithm for tracking this,
	// but the obvious method used here takes all of 2 microseconds.
	// you know what they say about premature optimization...
	for(int i=0;i<NumVisibleLines;i++)
	{
		if (line->pxwidth > longest)
			longest = line->pxwidth;
		
		line = line->next;
	}
	
	int newmax = (longest + 8) - editor.pxwidth;
	if (newmax < 0) newmax = 0;
	
	if (newmax != ev->scroll.last_hbar_max || force_set)
	{
		if (newmax > 0)
		{
			scrHorizontal->SetRange(0, newmax);
			scrHorizontal->SetValue(ev->xscroll);
			
			float p = (float)editor.pxwidth / (float)longest;
			scrHorizontal->SetProportion(p);
		}
		else
		{
			scrHorizontal->SetRange(0, 0);
			scrHorizontal->SetValue(0);
			scrHorizontal->SetProportion(1.0);
		}
		
		ev->scroll.last_hbar_max = newmax;
	}
}

void EditView::UpdateVertScrollBarRange(bool force_set)
{
EditView *ev = this;

	scrVertical->SetRange(0, ev->GetMaxScroll());
	scrVertical->SetValue(ev->scroll.y);	
	
	if (ev->nlines >= editor.height)
	{
		float p = (float)editor.height / (float)ev->nlines;
		scrVertical->SetProportion(p);
	}
	else
	{
		scrVertical->SetProportion(1.0);
	}
}



