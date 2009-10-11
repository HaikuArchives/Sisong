
#include "editor.h"
#include "layout.fdh"

#define CURRENTVERSION		0


// save the current state of the editor (open documents etc)
// to the given file.
bool save_layout(const char *filename)
{
FILE *fp;
EditView *ev;
int i, docCount;
int x1, y1, x2, y2;

	fp = fopen(filename, "wb");
	if (!fp)
	{
		stat("save_layout: failed to open file '%s'", filename);
		return 1;
	}

	fprintf(fp, "%d : VERSION\n", CURRENTVERSION);

	// write # of open documents
	docCount = editor.DocList->CountItems();
	fprintf(fp, "%d : RESERVED\n", docCount);

	// save info on each doc
	for(i=0;i<docCount;i++)
	{
		ev = (EditView *)editor.DocList->ItemAt(i);
		if (ev->IsUntitled) continue;

		fprintf(fp, "%s\n", ev->filename);
		fprintf(fp, "%d : active\n", ev == editor.curev);
		fprintf(fp, "%d : nlines\n", ev->nlines);
		fprintf(fp, "%d : scroll_y\n", ev->scroll.y);
		fprintf(fp, "%d : cursor_x\n", ev->cursor.x);
		fprintf(fp, "%d : cursor_y\n", ev->cursor.y);
		fprintf(fp, "%d : cursor_mode\n", ev->cursor.xseekmode);

		fprintf(fp, "%d : selection_present\n", ev->selection.present);

		if (ev->selection.present)
		{
			GetSelectionExtents(ev, &x1, &y1, &x2, &y2);
			fprintf(fp, "%d : selection_x1\n", x1);
			fprintf(fp, "%d : selection_y1\n", y1);
			fprintf(fp, "%d : selection_x2\n", x2);
			fprintf(fp, "%d : selection_y2\n", y2);
		}

		fprintf(fp, "-\n");
	}
	fprintf(fp, "*END*\n");

	stat("wrote layout file %s", filename);
	fclose(fp);
	return 0;
}


bool load_layout(const char *filename)
{
FILE *fp;
int i;
char docfname[MAXPATHLEN];
BStopWatch *w = new BStopWatch("load_layout");

	fp = fopen(filename, "rb");
	if (!fp) return 1;

	int version = readnum(fp);
	if (version != CURRENTVERSION)
	{
		BString str("Unable to load layout file '");
		str.Append(filename);
		str.Append("', because it is the wrong version.");
		(new BAlert("", str.String(), "OK"))->Go();

		fclose(fp);
		return 1;
	}

	// we no longer pay attention to the "document count" value,
	// because it can be wrong if untitled documents were open.
	// leaving the line for now though as a reserved value,
	// for backwards compatibility.
	int docCount = readnum(fp);
	if (docCount <= 0)
	{
		BString message;
		message << "Warning: (docCount <= 0) in layout file.\n\n"
			<< "(Your data is fine, just your saved layout might be messed up).\n\n"
			<< "Please notify the author of this error.\n\n"
			<< filename;
		(new BAlert("", message.String(), "OK"))->Go();
	}

	EditView::Close_All();

	EditView *ev = NULL, *lastvalidev = NULL;
	for(i=0;;i++)
	{
		fgetline(fp, docfname, sizeof(docfname) - 1);
		//stat("'%s'", docfname);
		if (!strcmp(docfname, "*END*") || !docfname[0] || feof(fp))
			break;

		int active = readnum(fp);
		int nlines = readnum(fp);
		int scroll_y = readnum(fp);
		int cursor_x = readnum(fp);
		int cursor_y = readnum(fp);
		int cursor_mode = readnum(fp);
		int selection_present = readnum(fp);
		int x1, y1, x2, y2;
		if (selection_present)
		{
			x1 = readnum(fp);
			y1 = readnum(fp);
			x2 = readnum(fp);
			y2 = readnum(fp);
		}
		readnum(fp);	// the "-" seperator between documents

		ev = CreateEditView(docfname);
		if (!ev) continue;

		if (ev->nlines == nlines)	// ensure doc hasn't changed since last load
		{
			ev->SetVerticalScroll(scroll_y);
			ev->cursor.move(cursor_x, cursor_y);
			ev->cursor.set_mode(cursor_mode);
			ev->XScrollToCursor();

			if (selection_present)
			{
				DocPoint start(ev, x1, y1);
				DocPoint end(ev, x2, y2);

				selection_select_range(ev, &start, &end);
			}
		}

		if (active)
			TabBar->SetActiveTab(ev);

		lastvalidev = ev;
	}

	if (!lastvalidev)	// failsafe: all docs in file are since deleted
		TabBar->SetActiveTab(CreateEditView(NULL));
	else if (!editor.curev)	// failsafe: active document marked in file is since deleted
		TabBar->SetActiveTab(lastvalidev);

	delete w;
	stat("loaded layout %s", filename);
	fclose(fp);

	return 0;
}


int readnum(FILE *fp)
{
char line[500];

	fgetline(fp, line, sizeof(line)-1);
	return atoi(line);
}

