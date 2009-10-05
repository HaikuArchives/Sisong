
#define TAB_WIDTH		4

struct ScrollData
{
	clLine *topline;	// pointer to line at top of visible window
	int y;				// line number of line at top of visible window		
	int lasty;			// last y coordinates scroll was at when screen was redrawn
	
	// longest range of scrollbar UpdateHozScrollBarRange was called
	int last_hbar_max;
};

// holds a document as a double-linked list of lines.
class EditView
{
public:
	EditView();
	
	// first and last lines in document
	clLine *firstline, *lastline;
	// pointer to line cursor is currently on
	clLine *curline;
	
	uint DocID;				// a unique ID number for this session
	int nlines;				// # of lines in document
	int lastnlines;			// for redraw

	EditCursor cursor;
	UndoData undo;
	ScrollData scroll;
	MouseData mouse;
	SelectionData selection;
	BMatchData bmatch;
	CmdSeqData cmdseq;
	SearchData search;
	
	int xscroll;				// amount editor pane is horizontally scrolled over

	char filename[MAXPATHLEN];
	char IsUntitled;			// 1 if filename is an auto-generated "untitled" filename
	bool IsDirty;				// 1 if changes have not been committed to disk
	bool ModifiedSinceRedraw;	// 1 if document has been modified since last redraw
	bool CloseAfterSave;

// -------------------------------------

	// files
	bool Save(const char *filename);
	static void Save_All();
	
	void SetDirty();
	void ClearDirty();
	void MakeUntitled();
	void ReloadFile();

	void Close(bool DoSanityCheck=true);
	uint8 ConfirmClose(bool OfferCancelButton=true);
	static void Close_All();
	
	// key handling
	void HandleKey(int ch);
	bool ProcessCommandSeq(int ch);
	void CancelCommandSeq(void);
	bool IsCommandSeqActive();
	void MakeCursorVisible();
	void SelDel();
	
	// mouse handling
	void MouseDown(int x, int y);
	void MouseDrag(int x, int y);
	void MouseUp();
	
	// edit_keys selection handling
	void ExtendOrDropSel(char key);
	void ExtendSel();
	
	// actions
	void action_insert_char(int x, int y, char ch);
	void action_insert_string(int x, int y, const char *str, int *final_x, int *final_y);
	void action_insert_cr(int x, int y);
	void action_delete_right(int x, int y, int count);
	void action_delete_range(int x1, int y1, int x2, int y2);
	
	void insert_line(char *initial_string, clLine *insertafter, int y);
	void delete_line(clLine *line, int y);

	// stuff
	clLine *GetLineHandle(int y);
	clLine *GetLineHandleFromStart(int y);
	clLine *GetLineHandleFromEnd(int y);
	clLine *GetLineHandleFromCursor(int y);
	
	void AddToDocPoint(int x, int y, clLine *line, int count, int *x_out, int *y_out);
	BString *RangeToString(int x1, int y1, int x2, int y2, char *crlf_seq);

	// misc
	void GetCurrentWordExtent(int *x1_out, int *x2_out);
	void CopySelection(void);
	void PasteFromClipboard(void);
	
	// scrolling
	void scroll_up(int nlines);
	void scroll_down(int nlines);
	void SetVerticalScroll(int y);
	void BringLineIntoView(int y, int vismode, int target_screen_y);
	int GetMaxScroll();
	
	void SetXScroll(int newvalue);
	void XScrollToCursor();
	
	// rendering
	void FullRedrawView();
	void RedrawView();
	void UpdateHozScrollBarRange(bool force_set);
	void UpdateVertScrollBarRange(bool force_set);
};

// if y is not a valid line number in the document, fixes it
#define ENSURE_LINE_IN_RANGE(y)		\
{	\
	if (y >= ev->nlines) {	\
		y = (ev->nlines - 1);	\
	}	\
	else if (y < 0) {	\
		y = 0;	\
	}	\
}

// special cursor modes for determining X coordinate of cursor

// the user has typed a char or bksp, etc.
// in this mode the X position does not change when you go up/down.
#define CM_FREE				0

// this mode is used during an arrow UP/DOWN or PGUP/PGDN seek.
// the cursor tries to move to the same X screen coordinate as it was at
// when the seek began, but is stopped if the line it has moved onto
// is too short.
#define CM_WANT_SCREEN_COORD		1

// this mode is entered after the user pushes the END key.
// in this mode the cursor always seeks to the end of the line.
#define CM_WANT_EOL			2


// for BringLineIntoView
#define BV_CENTERED			0
#define BV_SIMPLE			1
#define BV_SPECIFIC_Y		2
#define BV_FORCE_SPECIFIC_Y	3

#define CEV_CLOSED_SAVED		0
#define CEV_CLOSED_NOT_SAVED	1
#define CEV_CLOSE_CANCELED		2

