
/*
	EditView functions having to do with file handling.
*/

#include "editor.h"
#include "edit_files.fdh"


// open a new editor tab.
// filename: specify the name of a file to load. if NULL, a new document is created.
// you should generally always call this function instead of directly
// using e.g. CreateEVFromFile.
EditView *CreateEditView(const char *filename)
{
EditView *ev;
	
	if (filename)
	{
		if (!(ev = InitEVFromFile(filename)))
			return NULL;
	}
	else
	{	// create new empty document here
		ev = InitAsNewDocument();
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

// creates an editview and loads lines into it from a file.
// don't use directly, use CreateEditView instead.
static EditView *InitEVFromFile(const char *fname)
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

// creates an empty editview with a single blank line in it.
// don't use directly, use CreateEditView instead.
static EditView *InitAsNewDocument()
{
EditView *ev = new EditView;
	
	// if there are no other untitled documents open,
	// we can safely reset the "new 1...new 2..." counter
	int i, count = editor.DocList->CountItems();
	bool haveUntitled = false;
	EditView *cv;
	for(i=0;i<count;i++)
	{
		cv = (EditView *)editor.DocList->ItemAt(i);
		if (cv->IsUntitled)
		{
			haveUntitled = true;
			break;
		}
	}
	if (!haveUntitled)
		editor.NextUntitledID = 0;
	
	// initilize it
	ev->firstline = ev->lastline = new clLine;
	ev->firstline->prev = NULL;
	ev->lastline->next = NULL;
	ev->nlines = 1;
	
	ev->MakeUntitled();
	return ev;
}

/*
void c------------------------------() {}
*/

// closes the given document.
// if DoSanityCheck is true, then a new empty document will be created,
// if this document was the last one left.
void EditView::Close(bool DoSanityCheck)
{
EditView *ev = this;

	TabBar->RemoveTab(ev);
	editor.DocList->RemoveItem(ev);

	// editor will normally crash if all tabs are allowed to be closed;
	// do sanity check to prevent this if requested.
	if (DoSanityCheck && editor.DocList->CountItems()==0)
	{
		TabBar->SetActiveTab(CreateEditView(NULL));
	}
	
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

// closes the edit view, prompting the user to save it first if it is dirty.
// returns one of:
//		CEV_CLOSED_SAVED
//		CEV_CLOSED_NOT_SAVED
//		CEV_CLOSE_CANCELED
uint8 EditView::ConfirmClose(bool OfferCancelButton)
{
EditView *ev = this;
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
	
	// user confirmed it or it wasn't dirty
	ev->Close();
	return need_to_save ? CEV_CLOSED_SAVED : CEV_CLOSED_NOT_SAVED;
}


// WARNING:
// this creates an illegal condition of NO documents being open.
// it should only be called during certain special circumstances (such as shutdown).
void EditView::Close_All(void)
{	
	// load_layout() uses us to close all docs, then it opens new ones.
	// make sure current tab is set to null to avoid bugs in the tab bar
	// when adding the new documents back in.
	TabBar->SetActiveTab(NULL);
	editor.curev = NULL;
	
	// close all documents
	while(editor.DocList->CountItems())
	{
		EditView *cev = (EditView *)editor.DocList->FirstItem();
		cev->Close(false);
	}
	
	editor.NextUntitledID = 0;
}

/*
void c------------------------------() {}
*/

// saves the document to the given file.
// returns nonzer if an error occurs.
bool EditView::Save(const char *filename)
{
EditView *ev = this;
FILE *fp;

	if (!(fp = fopen(filename, "wb")))
	{
		staterr("EditView::Save(): failed to open file '%s'", filename);
		return 1;
	}
	
	clLine *line = ev->firstline;
	if (line)
	{
		// the starting size should be large enough to all lines in most documents,
		// but it will grow if needed.
		int buffer_sz = 1024;
		char *buffer = (char *)smal(buffer_sz);
		
		rept
		{
			// check line length. if we need to grow the buffer, do so now.
			int linelength = line->GetLength();
			if (linelength >= buffer_sz - 1)
			{
				buffer_sz = (linelength + 80);
				// we don't care about prior contents of the buffer, so i avoid "resmal"
				frees(buffer);
				buffer = (char *)smal(buffer_sz);
			}
			
			// copy line to buffer, and optionally remove unnecessary whitespace
			line->GetLineToBuffer(buffer);
			
			if (editor.settings.TrimTrailingOnSave)
				linelength = RTrimWhitespace(buffer, linelength);
			
			// write the line
			fwrite(buffer, linelength, 1, fp);
			
			// advance to next line
			line = line->next;
			
			if (!line) break;
			fputc('\n', fp);
		}
		
		frees(buffer);
	}
	
	fclose(fp);
	return 0;
}


void EditView::Save_All(void)
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

/*
void c------------------------------() {}
*/

// "nice" version of CreateEditView(filename)
// opens the file and switches to it. if the file is already
// open activates the existing copy.
EditView *DoFileOpen(const char *filename)
{
EditView *ev;

	LockWindow();
	
	// if document is already open just switch to it
	ev = FindEVByFilename(filename);
	if (!ev)
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
EditView *DoFileOpenAtLine(const char *filename, int lineNo, int x_start, int x_end)
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


// do the "reload file from disk" menu command
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
	
	ev->Close();
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

// mark document dirty
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

// mark document as non-dirty
void EditView::ClearDirty()
{
	if (IsDirty)
	{
		IsDirty = 0;
		TabBar->SetDirtyState(this, false);
	}
}

// make the document an "untitled" document
void EditView::MakeUntitled()
{
	sprintf(this->filename, "new %d", ++editor.NextUntitledID);
	this->IsUntitled = true;
}


