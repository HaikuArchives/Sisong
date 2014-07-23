#ifndef UNDO_H
#define UNDO_H

#define MERGE_NONE		0
#define MERGE_TYPING	1
#define MERGE_DEL		2
#define MERGE_BKSP		3

// undo group options to BeginUndoGroup
#define UG_NO_AFFECT_CURSOR		0x01
void BeginUndoGroup(EditView *ev, int options=0);

struct UndoRecord
{
	int y, x;				// position that the edit occurred
	int length;				// length of the edit
	
	// a String containing the data that was deleted.
	// if NULL, this is an insertion.
	BString *deldata;
	
	// set to true for typing records where later letters
	// could be merged into this same record.
	char mergetype;
};

struct UndoGroupRecord
{
	int BeforeCursorX, BeforeCursorY;		// cursor position before the edit
	int AfterCursorX, AfterCursorY;			// cursor position after the edit
	int RecordCount;						// number of undo records in this group
	int options;
};


struct UndoBuffer
{		
	CList *stack;		// a list object of UndoRecord holding the records
	CList *groups;		// a list organizing the stack into undo groups
	
	// stuff saved for creating next group
	int BeforeCursorX, BeforeCursorY;
	int GroupRecordCount;
};


struct UndoData
{
	// Undo and Redo buffers
	UndoBuffer ub, rb;
	
	// set to 1 while an undo is in progress
	bool save_to_redo;
	// set to 1 while an undo is being recorded
	bool GroupOpen;
	
	int MergeMode;
	int MergeToPrior;
};

void undo_init(EditView *ev);
void undo_close(EditView *ev);
static void InitStack(UndoBuffer *s);
static void FreeUndoRecord(UndoRecord *rec);
static void FreeUndoGroup(UndoGroupRecord *grec);
UndoGroupRecord *EndUndoGroup(EditView *ev);
void EndUndoGroupSetCursor(EditView *ev, int cx, int cy);
void UpdateMergedUndoGroup(EditView *ev);
void undo_SetMergeMode(EditView *ev, int mode, int MergeToPrior);
void undo_add(EditView *ev, int x, int y, int length, BString *deldata);
char undo_can_merge(EditView *ev, int x, int y, int key);
void undo_undo(EditView *ev);
void undo_redo(EditView *ev);
static void RevertAction(EditView *ev, UndoBuffer *ub, UndoBuffer *rb);

#endif // UNDO_H
