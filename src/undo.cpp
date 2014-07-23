
#include "editor.h"
#include "undo.h"

void undo_init(EditView *ev)
{
	InitStack(&ev->undo.ub);
	InitStack(&ev->undo.rb);
	ev->undo.save_to_redo = 0;
}

void undo_close(EditView *ev)
{
	ev->undo.ub.stack->free();
	ev->undo.ub.groups->free();
	
	ev->undo.rb.stack->free();
	ev->undo.rb.groups->free();
	
	delete ev->undo.ub.stack;
	delete ev->undo.ub.groups;
	delete ev->undo.rb.stack;
	delete ev->undo.rb.groups;
	
	memset(&ev->undo, 0, sizeof(ev->undo));
}

static void InitStack(UndoBuffer *s)
{
	s->stack = new CList((void *)FreeUndoRecord);
	s->groups = new CList((void *)FreeUndoGroup);
}

static void FreeUndoRecord(UndoRecord *rec)
{
	if (rec->deldata)
	{
		delete rec->deldata;
		rec->deldata = NULL;
	}
	
	frees(rec);
}

static void FreeUndoGroup(UndoGroupRecord *grec)
{
	frees(grec);
}

/*
void c------------------------------() {}
*/

// marks the beginning of an undo group
void BeginUndoGroup(EditView *ev, int options=0)
{
UndoGroupRecord *grec;

	if (ev->undo.GroupOpen)
	{
		staterr("BeginUndoGroup assert fail: group already open");
		errorblip();
	}
	
	grec = (UndoGroupRecord *)smalz(sizeof(UndoGroupRecord));
	
	grec->BeforeCursorX = ev->cursor.x;
	grec->BeforeCursorY = ev->cursor.y;
	grec->RecordCount = ev->undo.ub.stack->CountItems();
	grec->options = options;
	
	ev->undo.ub.groups->AddItem(grec);
	ev->undo.GroupOpen = true;
	
	// clear redo stack
	ev->undo.rb.stack->free();
	ev->undo.rb.groups->free();
}

// marks the end of an undo group
// returns: a pointer to the group record (used when function is called internally)
UndoGroupRecord *EndUndoGroup(EditView *ev)
{
UndoGroupRecord *grec;

	grec = (UndoGroupRecord *)ev->undo.ub.groups->LastItem();
	
	if (!grec || !ev->undo.GroupOpen)
	{
		staterr("EndUndoGroup assert fail: group not open or record missing");
		errorblip();
		return NULL;
	}
	
	grec->AfterCursorX = ev->cursor.x;
	grec->AfterCursorY = ev->cursor.y;
	
	grec->RecordCount = (ev->undo.ub.stack->CountItems() - grec->RecordCount);
	ev->undo.GroupOpen = false;
	
	//stat("undo group end: [%d,%d] - [%d,%d]", grec->BeforeCursorX, grec->BeforeCursorY, grec->AfterCursorX, grec->AfterCursorY);
	return grec;
}

void EndUndoGroupSetCursor(EditView *ev, int cx, int cy)
{
UndoGroupRecord *grec;

	if ((grec = EndUndoGroup(ev)))
	{
		grec->BeforeCursorX = cx;
		grec->BeforeCursorY = cy;
	}
}

// updates a merged undo group after adding an action to it while MergeToPrior=1
void UpdateMergedUndoGroup(EditView *ev)
{
UndoGroupRecord *grec;

	grec = (UndoGroupRecord *)ev->undo.ub.groups->LastItem();
	
	if (!grec)
	{
		staterr("EndMergedUndoGroup assert fail: record missing");
		errorblip();
		return;
	}
	
	grec->AfterCursorX = ev->cursor.x;
	grec->AfterCursorY = ev->cursor.y;
}

/*
void c------------------------------() {}
*/

// sets the merge mode of the next action added to the buffer.
// after the action is added, the merge mode automatically reverts to MERGE_NONE.
// merge modes are used to merge user-input operations into a single action.
// they are effective for typing, DEL key, and BACKSPACE.
//
// mode: the merge type to set
// MergeToPrior: if set to 1, the next action added is merged to the top entry in
// the stack. if 0, a new action is added, and is set to be mergeable to future
// entries added with the same merge mode. do not pass 1 unless undo_can_merge
// returns true.
void undo_SetMergeMode(EditView *ev, int mode, int MergeToPrior)
{
	ev->undo.MergeMode = mode;
	ev->undo.MergeToPrior = MergeToPrior;
}


// adds an undo record to the top of the buffer.
// ev: pointer to the EditView this is relevant for.
// x: X position of the edit.
// y: line number of the edit.
// length: numbers of chars which were inserted/deleted, counting newlines as 1 char.
// deldata: if edit is a deletion, a BString containing the deleted data,
//			with LF's terminating each line, else NULL, and an insertion is recorded.
void undo_add(EditView *ev, int x, int y, int length, BString *deldata)
{
UndoRecord *rec;
BList *stack;

	// select whether this action goes to Undo or Redo stack
	if (!ev->undo.save_to_redo)
	{
		stack = ev->undo.ub.stack;
		
		if (!ev->undo.GroupOpen && !ev->undo.MergeToPrior)
			staterr("undo_add: egads, no undo group open! Use BeginUndoGroup()/EndUndoGroup()");
	}
	else
	{
		stack = ev->undo.rb.stack;
	}
	
	if (ev->undo.MergeToPrior)
	{
		rec = (UndoRecord *)stack->LastItem();
		
		switch(ev->undo.MergeMode)
		{
			case MERGE_TYPING:
				rec->length += length;
			break;
			
			case MERGE_DEL:
				rec->length += length;
				
				rec->deldata->Append(*deldata);
				delete deldata;
			break;
			
			case MERGE_BKSP:
				rec->x = x;
				rec->y = y;
				rec->length += length;
				
				deldata->Append(*rec->deldata);
				delete rec->deldata;
				rec->deldata = deldata;
			break;
		}
	}
	else
	{
		rec = (UndoRecord *)smalz(sizeof(*rec));
		
		rec->x = x;
		rec->y = y;
		rec->length = length;
		rec->deldata = deldata;
		rec->mergetype = ev->undo.MergeMode;
		
		stack->AddItem(rec);
	}
	
	ev->undo.MergeMode = MERGE_NONE;
	ev->undo.MergeToPrior = 0;
	
	//stat("added %sdo record at [%d,%d]; %s", ev->undo.save_to_redo ? "re":"un", x, y, deldata ? "deletion" : "insertion");
	//if (deldata) stat("deleted text: '%s'", deldata->String());
}


// for mergeable insertions.
// returns true if the top entry is mergeable.
//
// typing: each character is inserted at a successive position from the last,
//		   the LENGTH and AfterCursor fields are updated with each keystroke.
//
// DEL: each character is deleted from the exact same position.
//		the LENGTH field is updated.
//
// BKSP: each character is inserted at the previous position.
//		 the record X,Y, length, and AfterCursor fields are updated.
char undo_can_merge(EditView *ev, int x, int y, int key)
{
UndoRecord *lastrec;

	lastrec = (UndoRecord *)ev->undo.ub.stack->LastItem();
	if (!lastrec) return 0;				// no undo records present
	
	switch(key)
	{
		default:		// typing insertions
			if (lastrec->mergetype != MERGE_TYPING) return 0;
			
			if (lastrec->y != y) return 0;		// not on same line
			if (lastrec->x != (x - lastrec->length)) return 0;	// not just prior to the new insertion
			
			return 1;							// yes, is ok
		break;
		
		case B_DELETE:	// each char is inserted at same position
			if (lastrec->mergetype != MERGE_DEL &&
				lastrec->mergetype != MERGE_BKSP) return 0;
			
			if (lastrec->y != y) return 0;
			if (lastrec->x != x) return 0;
			
			return 1;
		break;
		
		case B_BACKSPACE:	// each char is inserted from the previous position
			if (lastrec->mergetype != MERGE_BKSP &&
				lastrec->mergetype != MERGE_DEL) return 0;
			
			// for BKSP, the X position is one less each time. however, because the
			// cursor-left is done after this function is called, they will appear
			// to be the same coordinate.
			if (lastrec->y != y) return 0;
			if (lastrec->x != x) return 0;
			
			return 1;
		break;
	}
}

/*
void c------------------------------() {}
*/

void undo_undo(EditView *ev)
{
	ev->undo.save_to_redo = true;
	RevertAction(ev, &ev->undo.ub, &ev->undo.rb);
	ev->undo.save_to_redo = false;
	
	// if they undid all the way to the beginning, clear the dirty bit
	if (ev->IsUntitled && ev->undo.ub.groups->CountItems() == 0)
		ev->ClearDirty();
}

void undo_redo(EditView *ev)
{
	RevertAction(ev, &ev->undo.rb, &ev->undo.ub);
}

// undoes the last action on the undo stack
static void RevertAction(EditView *ev, UndoBuffer *ub, UndoBuffer *rb)
{
UndoRecord *rec;
UndoGroupRecord *grec;
int RecordCount;

	grec = (UndoGroupRecord *)ub->groups->pop();
	if (!grec)
	{
		staterr("nothing to undo!");
		return;
	}
	
	selection_drop(ev);
	
	// undo all actions in the group
	RecordCount = grec->RecordCount;
	ev->undo.GroupOpen = true;	// so the assert in undo_add won't complain
	
	while(RecordCount > 0)
	{
		rec = (UndoRecord *)ub->stack->pop();
		if (!rec)
		{
			BAlert *error = new BAlert("", "SERIOUS ERROR: Undo Group Record Count larger than actual number of records!\r\nSave your work immediately and restart!", "Oh No!");
			error->Go();
			
			return;
		}
		
		if (rec->deldata)
		{
			ev->action_insert_string(rec->x, rec->y, (char *)rec->deldata->String(), NULL, NULL);
		}
		else
		{
			ev->action_delete_right(rec->x, rec->y, rec->length);
		}
		
		FreeUndoRecord(rec);
		RecordCount--;
	}
	
	// jump cursor to the "Before" position
	if (!(grec->options & UG_NO_AFFECT_CURSOR))
		ev->cursor.move(grec->BeforeCursorX, grec->BeforeCursorY);
	
	// switch the before and after positions and push the group record back onto
	// the opposing stack.
	SWAP(grec->BeforeCursorX, grec->AfterCursorX);
	SWAP(grec->BeforeCursorY, grec->AfterCursorY);
	rb->groups->AddItem(grec);
	ev->undo.GroupOpen = false;
}

/*
void c------------------------------() {}
*/

/*
void DumpUndoBuffer(UndoBuffer *ub, int maxrecords)
{
int count = 0;
int gcount;
UndoRecord *rec;
UndoGroupRecord *grec;
int grpcount = 0;

	stat(""); stat(""); stat(""); stat("");
	stat(" { %d records; %d groups }\n", ub->stack->CountItems(), ub->groups->CountItems());
	
	int stack_index = ub->stack->CountItems() - 1;
	int group_index = ub->groups->CountItems() - 1;
	
	gcount = 0;
	while(rec = (UndoRecord *)ub->stack->ItemAt(stack_index--))
	{
		while(!gcount)
		{
			grec = (UndoGroupRecord *)ub->groups->ItemAt(group_index--);
			grpcount++;
			if (!grec) break;
			
			DumpUndoGroupRecord(grec);
			gcount = grec->RecordCount;
			if (++count >= maxrecords) return;
		}
		gcount--;
		
		stat("%s", DescribeUndoRecord(rec));
		
		if (++count >= maxrecords) break;
	}
}

void DumpUndoGroupRecord(UndoGroupRecord *grec)
{
	stat(" >> count:%d  before:[%d,%d]  after:[%d,%d] <<", \
		grec->RecordCount, \
		grec->BeforeCursorX, grec->BeforeCursorY, \
		grec->AfterCursorX, grec->AfterCursorY);
}

char *DescribeUndoRecord(UndoRecord *rec)
{
char data[20000];

	if (rec->deldata)
	{
		strcpy(data, rec->deldata->String());
		if (data[0]=='\n' && data[1]==0)
		{
			strcpy(data, "<CR>");
		}
		else
		{
			char *ptr = strchr(data, '\n');
			if (ptr) strcpy(ptr, "...");
			ptr = strchr(data, '\r');
			if (ptr) strcpy(ptr, "[CR] ...");
		}
	}
	else
	{
		data[0] = 0;
	}
	
	char *mt = "";
	if (rec->mergetype == MERGE_TYPING) mt = ", MERGE_TYPING";
	if (rec->mergetype == MERGE_DEL) mt = ", MERGE_DEL";
	if (rec->mergetype == MERGE_BKSP) mt = ", MERGE_BKSP";
	
	static char retval[10000];
	sprintf(retval, " [%d,%d]; length %d; %s%s; [%s]", \
					rec->x, rec->y, \
					rec->length, \
					rec->deldata ? "deletion" : "insertion", \
					mt, data);
	
	return retval;
}

*/
