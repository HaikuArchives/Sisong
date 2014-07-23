
#include "editor.h"
#include "edit_keys.h"

#include "misc.h"
#include "misc2.h"
#include "stat.h"

// this key can extend or create a selection if it is pushed with SHIFT down,
// and removes the selection if it is pressed without SHIFT down.
#define KF_AFFECTS_SELECTION		0x0001
// this key deletes the contents of the selection if it is pushed
#define KF_SELDEL					0x0002
// if "SELDEL" is activated by this key, the keys normal function does not run
#define KF_SELDEL_ONLY				0x0004
// disables the default behavior of ensuring the line the cursor is on is visible
#define KF_NO_VIEW_TO_CURSOR		0x0008
// set this on all commands which modify the document
#define KF_UNDOABLE					0x0020
// special case for handling undo on typing, which can merge later letters into
// the same undo action.
#define KF_UNDO_MERGEABLE			0x0040


uint GetKeyAttr(int ch)
{
	switch(ch)
	{
		case B_UP_ARROW:
		case B_DOWN_ARROW:
		case B_LEFT_ARROW:
		case B_RIGHT_ARROW:
		case B_HOME:
		case B_END:
			return KF_AFFECTS_SELECTION;
		
		case B_PAGE_UP:
		case B_PAGE_DOWN:
			return KF_AFFECTS_SELECTION;
		
		case KEY_MOUSEWHEEL_DOWN:
		case KEY_MOUSEWHEEL_UP:
			return KF_NO_VIEW_TO_CURSOR;
		
		case B_DELETE:
		case B_BACKSPACE:
			return KF_SELDEL | KF_SELDEL_ONLY | KF_UNDOABLE | KF_UNDO_MERGEABLE;
		
		case B_ENTER:
			return KF_SELDEL | KF_UNDOABLE;
		
		case B_TAB:
			return KF_UNDOABLE;
		
		default:	// normal char
			return KF_SELDEL | KF_UNDOABLE | KF_UNDO_MERGEABLE;
		
		case B_ESCAPE:
			return 0;
	}
}

// processes a keystroke
void EditView::HandleKey(int ch)
{
	//stat("got ch: %c", ch);
	if (modifiers() & B_CONTROL_KEY)
	{
		// translate e.g., CTRL+C back into just "C".
		// todo: fixup so we don't have to do this.
		ch = TranslateAltKey(ch);
	}
	//stat("translated ch: %c", ch);
	
	// ctrl+tab shortcut
	if (ch==TAB && IsCtrlDown())
	{
		if (IsShiftDown())
			TabBar->SwitchToPrevTab();
		else
			TabBar->SwitchToNextTab();
		
		return;
	}
	
	// catch command-shortcut sequences
	if (ProcessCommandSeq(ch))
		return;
	
	// ctrl+shift+z (Redo) shortcut
	if ((ch=='z' || ch=='Z') && IsCtrlDown() && IsShiftDown())
	{
		MainWindow->ProcessMenuCommand(M_EDIT_REDO);
		return;
	}
	
	// catch some other shortcuts before passing to main key processor
	switch(ch)
	{
		case B_DELETE:
			if (IsShiftDown())	// SHIFT+DEL: shortcut for cut
			{
				MainWindow->ProcessMenuCommand(M_EDIT_CUT);
				break;
			}
			else
			{
				ProcessKey(this, ch);
			}
		break;
		
		case B_INSERT:
			if (IsShiftDown())	// SHIFT+INS: legacy shortcut for paste
			{
				MainWindow->ProcessMenuCommand(M_EDIT_PASTE);
				break;
			}
			else
			{
				editor.InOverwriteMode ^= 1;
				MainView->cursor.SetThick(editor.InOverwriteMode);
			}
		break;
		
		default:
			if (IsCtrlDown() || IsAltDown()) break;
		case B_TAB: case B_HOME: case B_END:
		case KEY_MOUSEWHEEL_UP: case KEY_MOUSEWHEEL_DOWN:
			ProcessKey(this, ch);
		break;
	}
	
	this->RedrawView();
}

// process the key
void ProcessKey(EditView *ev, int key)
{
uint flags = GetKeyAttr(key);
char MergeToPrior;

	// free xseek mode
	if (ev->cursor.xseekmode != CM_FREE)
	{
		switch(key)
		{
			// these need to maintain the state of the CM_WANT_SCREEN_COORD coordinate
			case B_UP_ARROW:
			case B_DOWN_ARROW:
			case B_PAGE_UP:
			case B_PAGE_DOWN: break;
			
			default: ev->cursor.set_mode(CM_FREE);
		}
	}
	
	// commands which delete selection & contents if a selection is present
	if (flags & KF_SELDEL)
	{
		if (ev->selection.present)
		{
			ev->SelDel();
			
			if (flags & KF_SELDEL_ONLY)
				return;
		}
	}
	
	// create new undo group before executing keys which modify the document
	if (flags & KF_UNDOABLE)
	{
		if (flags & KF_UNDO_MERGEABLE)
		{
			MergeToPrior = undo_can_merge(ev, ev->cursor.x, ev->cursor.y, key);
			
			if (!MergeToPrior)
				BeginUndoGroup(ev);
		}
		else
		{
			MergeToPrior = 0;
			BeginUndoGroup(ev);
		}
	}
	
	if (flags & KF_AFFECTS_SELECTION)	// key can create/remove selection
	{
		if (IsShiftDown() && !ev->selection.present)
			selection_create(ev);
	}
	
	switch(key)
	{
		case B_ESCAPE:
			if (editor.settings.esc_quits_immediately)	// a testmode
			{
				MainWindow->fDoingInstantQuit = true;
				be_app->PostMessage(B_QUIT_REQUESTED);
			}
		break;
		
		case B_LEFT_ARROW: ev->cursor.left(); break;
		case B_RIGHT_ARROW: ev->cursor.right(); break;
		case B_UP_ARROW: ev->cursor.up(); break;
		case B_DOWN_ARROW: ev->cursor.down(); break;
		
		case B_PAGE_DOWN: ev->cursor.pgdn(); break;
		case B_PAGE_UP: ev->cursor.pgup(); break;
		
		case B_HOME: DoHome(ev); break;
		case B_END: DoEnd(ev); break;
		
		case KEY_MOUSEWHEEL_DOWN: ev->scroll_down(3); break;
		case KEY_MOUSEWHEEL_UP: ev->scroll_up(3); break;
		
		case B_ENTER:
			DoEnter(ev);
			editor.stats.CRs_typed++;
			editor.stats.keystrokes_typed++;
		break;
		
		case B_TAB:
		{
			if (IsShiftDown())
			{
				DoShiftTab(ev);
				break;
			}
			
			if (DoTabIndent(ev)) break;
			
			ev->SelDel();
			ev->action_insert_char(ev->cursor.x, ev->cursor.y, TAB);
			ev->cursor.x++;
			
			editor.stats.keystrokes_typed++;
		}
		break;
		
		// BKSP is equivalent to left followed by del
		case B_BACKSPACE:
			if (!ev->cursor.y && !ev->cursor.x) break;
			
			ev->cursor.left();
			
			undo_SetMergeMode(ev, MERGE_BKSP, MergeToPrior);
			ev->action_delete_right(ev->cursor.x, ev->cursor.y, 1);
			editor.stats.keystrokes_typed++;
		break;
		
		case B_DELETE:
			undo_SetMergeMode(ev, MERGE_DEL, MergeToPrior);
			ev->action_delete_right(ev->cursor.x, ev->cursor.y, 1);
			editor.stats.keystrokes_typed++;
		break;
		
		// typing
		default:
		{
			// ignore non-printable keystrokes
			if (key > 127 || key < 9)
				break;
			
			if (editor.InOverwriteMode && \
				ev->cursor.x < ev->curline->GetLength())
			{
				// less than ideal: i wasn't planning on Overwrite Mode when writing
				// the undo feature, so it can't merge undo records that contain both a
				// delete and a insertion. OVR mode isn't used much and undo still works,
				// just one char at a time, so I think it's ok for now but eventually should
				// be looked at.
				if (MergeToPrior)
				{
					MergeToPrior = false;
					BeginUndoGroup(ev);
				}
				
				ev->action_delete_right(ev->cursor.x, ev->cursor.y, 1);
			}
			else
			{
				undo_SetMergeMode(ev, MERGE_TYPING, MergeToPrior);
			}
			
			ev->action_insert_char(ev->cursor.x, ev->cursor.y, key);
			ev->cursor.x++;
			editor.stats.keystrokes_typed++;
		}
		break;
	}
	
	// smart indent (for close quotes)
	if (key == '}' && editor.settings.smart_indent_on_close)
		CloseSmartIndent(ev);
	
	if (flags & KF_AFFECTS_SELECTION)
		ev->ExtendOrDropSel(key);
	
	if (flags & KF_UNDOABLE)
	{
		if (MergeToPrior)
			UpdateMergedUndoGroup(ev);
		else
			EndUndoGroup(ev);
	}
	
	if (!(flags & KF_NO_VIEW_TO_CURSOR))
	{
		ev->MakeCursorVisible();
	}
}

/*
void c------------------------------() {}
*/

// Do "home" key.
//
// * go to start of indentation on the line (end of tabs).
// * if we are already there, go to very start of line (ignore tabs and go to column 0).
// * start tracking that we "want" that position.
void DoHome(EditView *ev)
{
int indent_x;

	if (IsCtrlDown())
	{
		ev->cursor.move(0, 0);
	}
	else
	{
		indent_x = ev->curline->GetIndentationLevel();
		
		if (ev->cursor.x > indent_x)
		{
			ev->cursor.x = indent_x;
			ev->cursor.screen_x = ev->curline->CharCoordToScreenCoord(ev->cursor.x);
			ev->cursor.set_mode(CM_WANT_SCREEN_COORD);
		}
		else
		{
			ev->cursor.x = 0;
		}
	}
}

// END moves the cursor to the end of the current line.
// the cursor then will "stick" at the end of all lines it is moved up/down to,
// until it is released by changing the xseek mode.
void DoEnd(EditView *ev)
{
	if (IsCtrlDown())
	{
		ev->cursor.move(ev->lastline->GetLength(), ev->nlines - 1);
	}
	else
	{
		ev->cursor.to_eol();
		ev->cursor.set_mode(CM_WANT_EOL);
	}
}

/*
void c------------------------------() {}
*/

// insert a new line just after the current one and switch to it.
//
// keep it on the same tab level by having it start off with as many
// tabs as the current line has.
//
// if cursor is before the leading tabs when we push ENTER, then the cursor
// goes to X position 0 instead of following the end of the copied tabs.
//
// if we push ENTER in the middle of a line, we split the line and add
// the rest of the line at the old cursor position to the new one.
void DoEnter(EditView *ev)
{
int ntabs;
int y = ev->cursor.y;
bool extra_indent = false;

	// record current indentation level.
	// if cursor is at or after this level, we will auto-indent the resultant line.
	ntabs = ev->curline->GetIndentationLevel();
	if (ev->cursor.x < ntabs) ntabs = 0;
	
	if (editor.settings.smart_indent_on_open)
	{
		// test if current line ends in a "{"
		if (ev->curline->GetCharAtIndex(ev->curline->GetLength() - 1) == '{')
		{
			if (ntabs > 0 || !editor.settings.no_smart_open_at_baselevel)
			{
				ntabs++;
				extra_indent = true;
			}
		}
	}
	
	// language-aware auto-indenting
	if (editor.settings.language_aware_indent && !extra_indent)
	{
		BString *bstr = ev->curline->GetLineAsString();
		const char *str = bstr->String();
		
		if (*str==TAB || *str==' ')
		{
			do { str++; } while(*str == TAB || *str == ' ');
			
			if (strbegin(str, "case ") || strbegin(str, "default:"))
			{
				ntabs++;
				extra_indent = true;
			}
		}
		
		delete bstr;
	}
	
	ev->action_insert_cr(ev->cursor.x, y);
	
	if (ntabs)
	{
		char *tabs = (char *)smal(ntabs + 1);
		memset(tabs, TAB, ntabs);
		tabs[ntabs] = 0;
		
		ev->action_insert_string(0, y + 1, tabs, NULL, NULL);
		frees(tabs);
	}
	
	ev->cursor.move(ntabs, y + 1);
}

// handles the "smart indent" for "}"
void CloseSmartIndent(EditView *ev)
{
int indent_level;
int pair_x, pair_y;
clLine *pair_line;

	// check that cursor is at end of the line
	if (ev->cursor.x != ev->curline->GetLength())
		return;
	
	// check that this is the first non-blank char on the line
	indent_level = ev->curline->GetIndentationLevel();
	if (ev->cursor.x != indent_level+1)
		return;
	
	// get the indent level of the opening '{'
	pair_line = find_start_of_pair(ev->curline, ev->cursor.x, ev->cursor.y, \
									'}', '{', &pair_x, &pair_y);
	
	if (!pair_line)
		return;
	
	// in code-style where { is on same line, the { may actually be
	// to the right of the just-entered }. fix this.
	if (pair_x >= ev->cursor.x)
	{
		pair_x = pair_line->GetIndentationLevel();
	}
	
	// if they have already closed the brace, do not mess things up
	// by trying to close it for them. this is only accurate for certain
	// coding styles (such as the one this function is written in) which
	// is why it is a seperate option.
	if (ev->cursor.x-1 <= pair_x)
	{
		return;
	}
	
	// remove indentation to automatically close the brace set
	int rem_amt = (ev->cursor.x - pair_x) - 1;
	
	if (indent_level)
	{
		ev->action_delete_right(0, ev->cursor.y, rem_amt);
		ev->cursor.x -= rem_amt;
	}
}

/*
void c------------------------------() {}
*/

// the following is the behavior of TAB:
//
// * if there is a selection and the selection encompasses more than one line or
//	 the entire line is selected:
//		- the selection is expanded to encompass whole lines.
//		- the selected lines are then indented using (1) additional tab.
// * else,
//		- a TAB character is inserted normally.
// returns 1 if the special behavior was activated.
char DoTabIndent(EditView *ev)
{
int y, y1, y2;

	if (!IsWholeLineSelected(ev)) return 0;
	selection_SelectFullLines(ev);
	
	GetSelectionExtents(ev, NULL, &y1, NULL, &y2);
	
	for(y=y1;y<=y2;y++)
	{
		ev->action_insert_char(0, y, TAB);
	}
	
	TIndent_SetCursorPos(ev, y1, y2);
	return 1;
}

// the following is the behavior of SHIFT+TAB:
//
// * if there is a selection and the selection encompasses more than one line or
//	 the entire line is selected:
//		- the selection is expanded to encompass whole lines.
//		- (1) indentation level is removed from the start of each line.
// * else,
//		- any existing selection is dropped.
//		- if the cursor is currently at or before the indentation level:
//			- the current line has (1) level removed from it's indentation.
//		- else,
//			- the cursor is moved back to the previous tab mark.
void DoShiftTab(EditView *ev)
{
	if (IsWholeLineSelected(ev))
	{
		// the more common "shift+tab" to move a whole block in 1 indent level
		int y, y1, y2;
		clLine *line;
		
		selection_SelectFullLines(ev);
		GetSelectionExtents(ev, NULL, &y1, NULL, &y2);
		
		line = ev->GetLineHandle(y1);
		for(y=y1;y<=y2;y++)
		{
			DecreaseIndentation(ev, line, y);
			line = line->next;
		}
		
		TIndent_SetCursorPos(ev, y1, y2);
	}
	else
	{
		// handle shift+tab on a single line or with less than a full line of
		// text selected.
		selection_drop(ev);
		
		int current_x = ev->cursor.x;
		int ident_x = ev->curline->GetIndentationLevel();
		
		// if cursor @ or before indentation level, dec indent level by 1
		if (current_x <= ident_x)
		{
			ev->cursor.x -= DecreaseIndentation(ev, ev->curline, ev->cursor.y);
			if (ev->cursor.x < 0) ev->cursor.x = 0;
		}
		else
		{	// move cursor back to previous tab position
			int x = ev->cursor.x;
			int mod;
			
			x = ev->curline->CharCoordToScreenCoord(x);
			mod = (x % TAB_WIDTH);
			if (mod) x -= mod; else x -= TAB_WIDTH;
			x = ev->curline->ScreenCoordToCharCoord(x);
			
			ev->cursor.move(x, ev->cursor.y);
		}
	}
	
	if (ev->cursor.x >= ev->curline->GetLength())
		ev->cursor.x = ev->curline->GetLength();
}


// decreases the indentation of the specified line by one level.
// used by SHIFT+TAB command. returns the number of chars which were deleted.
static int DecreaseIndentation(EditView *ev, clLine *line, int y)
{
char ch = line->GetCharAtIndex(0);

	if (ch == TAB)
	{
		// delete a single tab
		ev->action_delete_right(0, y, 1);
		return 1;
	}
	else if (ch == ' ')
	{
		int i;
		
		// or up to a full tab's width of spaces
		for(i=1;i<TAB_WIDTH;i++)
			if (line->GetCharAtIndex(i) != ' ') break;
		
		ev->action_delete_right(0, y, i);
		return i;
	}
	
	return 0;
}

// returns true if at least one whole line is selected.
static char IsWholeLineSelected(EditView *ev)
{
int x1, y1, x2, y2;

	if (!ev->selection.present) return 0;
	GetSelectionExtents(ev, &x1, &y1, &x2, &y2);
	
	if (y1 == y2)
	{
		if (x1 > 0 || x2 < ev->curline->GetLength())
			return 0;
	}
	
	return 1;
}

// cursor to x=0.
// if cursor is at bottom line of affected text, move to line
// just below affected text, else, leave it on same line.
void TIndent_SetCursorPos(EditView *ev, int y1, int y2)
{
	if (ev->cursor.y == y2)
	{
		ev->cursor.move(0, y2+1);
	}
	else
	{
		ev->cursor.move(0, ev->cursor.y);
	}
}

/*
void c------------------------------() {}
*/

void EditView::MakeCursorVisible()
{
	BringLineIntoView(this->cursor.y, BV_SIMPLE, 0);
	XScrollToCursor();
}

// deletes a selection and it's contents
void EditView::SelDel()
{
EditView *ev = this;
int old_cy;

	if (ev->selection.present)
	{
		int x1, y1, x2, y2;
		
		BeginUndoGroup(ev);
		GetSelectionExtents(ev, &x1, &y1, &x2, &y2);
		
		lstat2(" ** SelDel from [%d,%d] - [%d,%d]", x1, y1, x2, y2);
		
		// remember current screen position of cursor
		old_cy = ev->cursor.screen_y;
		
		// when undoing a seldel always move cursor to the end of the deleted text
		DocPoint endpt(ev, x2, y2);
		endpt.Increment();
		
		// delete selection and it's contents
		selection_drop(ev);
		ev->action_delete_range(x1, y1, x2, y2);
		
		// move to start of selection
		ev->cursor.move(x1, y1);
		
		// ensure cursor is still visible, and try to keep it at the
		// same screen Y position it was at before (when deleting a large portion of text)
		ev->BringLineIntoView(ev->cursor.y, BV_SPECIFIC_Y, old_cy);
		XScrollToCursor();
		
		EndUndoGroupSetCursor(ev, endpt.x, endpt.y);
	}
}

// if SHIFT is down, extend the selection to the cursor,
// creating a selection if none is currently present.
//
// if SHIFT is NOT currently down, remove any selection if present,
// and if "key" is LEFT or RIGHT, jump to beginning or end of the selection.
void EditView::ExtendOrDropSel(char key)
{
EditView *ev = this;

	if (IsShiftDown())
	{
		int oldy1, oldy2;
		GetSelectionExtents(ev, NULL, &oldy1, NULL, &oldy2);
		
		ev->ExtendSel();
		
		// the CopyBits() scroll optimization may not work if we are
		// scrolling down while de-extending the selection.
		if (ev->selection.anchor.y >= ev->cursor.y)
		{
			int y1, y2;
			GetSelectionExtents(ev, NULL, &y1, NULL, &y2);
			if ((y2 - y1) < (oldy2 - oldy1))
			{
				ev->CannotUseCopybits = true;
			}
		}
	}
	else if (ev->selection.present)
	{	// drop selection (it was canceled due to cursor movement or text editing)
		// LEFT/RIGHT jumps cursor to beginning/end of selection, respectively
		if (key==B_LEFT_ARROW || key==B_RIGHT_ARROW)
		{
			int x1, y1, x2, y2;
			GetSelectionExtents(ev, &x1, &y1, &x2, &y2);
			selection_drop(ev);
			
			if (key == B_LEFT_ARROW)
			{
				ev->cursor.move(x1, y1);
			}
			else
			{
				ev->cursor.move(x2, y2);
				ev->cursor.right();
			}
		}
		else
		{
			selection_drop(ev);
		}
	}
}

void EditView::ExtendSel()
{
	// required to ensure x is at correct spot if the key was the type
	// that sets CM_WANT_SCREEN_COORD.
	UpdateCursorPos(this);
	
	// extend selection
	selection_extend(this);
}
