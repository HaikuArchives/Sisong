
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
static const char *bigsep =
	"\n/*\nvoid c------------------------------() {}\n*/\n";
	//"/*\n\t#pragma mark -\n*/";
static const char *smallsep =
	"// ---------------------------------------\n";
const char *sep = bigsep;

EditView *ev = editor.curev;
clLine *line;
int y;
uint32 buttons;

	y = ((int)where.y / editor.font_height) + ev->scroll.y;
	if (y < 0) y = 0;
	if (y >= ev->nlines) y = ev->nlines - 1;
	
	line = ev->GetLineHandle(y);
	if (!line) return;
	
	LockWindow();
	
	GetMouse(NULL, &buttons);
	if (buttons & B_SECONDARY_MOUSE_BUTTON)
		sep = smallsep;
	
	BeginUndoGroup(ev);
	ev->action_insert_string(0, y, sep, NULL, NULL);
	EndUndoGroup(ev);
	
	ev->RedrawView();
	FunctionList->ScanIfNeeded();
	
	UnlockWindow();
}


