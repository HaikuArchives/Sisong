
#include "editor.h"
#include "spacer.fdh"

/***********************************************
*  THE											*
*   SPACER										*
*    VIEW										*
************************************************/

CSpacerView::CSpacerView(BRect frame, uint32 resizingMode)
            : BView(frame, "SpacerView", resizingMode, B_WILL_DRAW)
{
}

void CSpacerView::Draw(BRect updateRect)
{
static const pattern grey50 = { 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55 };
BRect r(Bounds());

	// 1-pixel border between spacer and text
	r.left = r.right;
	SetHighColor(GetEditBGColor(COLOR_TEXT));
	FillRect(r);

	// 50% grey pattern
	r.left = 0;
	r.right--;

	SetHighColor(0xff, 0xff, 0xff);
	SetLowColor(0x00, 0x00, 0x00);
	FillRect(r, grey50);
}

// insert separator feature
void CSpacerView::MouseDown(BPoint where)
{
static const char *bigsep   = "\n/*\nvoid c------------------------------() {}\n*/\n";
static const char *smallsep = "// ---------------------------------------\n";

EditView *ev = editor.curev;
uint32 buttons;

	if (!ev) return;
	LockWindow();

	int y = ev->scroll.y + ((int)where.y / editor.font_height);

	if (y < 0) y = 0;
	if (y >= ev->nlines) y = (ev->nlines - 1);

	clLine *line = ev->GetLineHandle(y);
	if (!line)
	{
		UnlockWindow();
		return;
	}

	GetMouse(&where, &buttons);
	const char *sep = (buttons & B_SECONDARY_MOUSE_BUTTON) ? smallsep : bigsep;

	BeginUndoGroup(ev);
	ev->action_insert_string(0, y, sep, NULL, NULL);
	EndUndoGroup(ev);

	ev->RedrawView();

	if (sep == bigsep)
		FunctionList->ScanIfNeeded();

	UnlockWindow();
}


