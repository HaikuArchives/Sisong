#ifndef EDIT_KEYS_H
#define EDIT_KEYS_H

uint GetKeyAttr(int ch);
void ProcessKey(EditView *ev, int key);
void DoHome(EditView *ev);
void DoEnd(EditView *ev);
void DoEnter(EditView *ev);
void CloseSmartIndent(EditView *ev);
char DoTabIndent(EditView *ev);
void DoShiftTab(EditView *ev);
static int DecreaseIndentation(EditView *ev, clLine *line, int y);
static char IsWholeLineSelected(EditView *ev);
void TIndent_SetCursorPos(EditView *ev, int y1, int y2);

#endif // EDIT_KEYS_H
