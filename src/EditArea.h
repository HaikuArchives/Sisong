
class DocScrollBar;
#include "IComm/IShelf.h"


class CEditArea : public BView
{
public:
	CEditArea(BRect frame, uint32 resizingMode);
	~CEditArea();
	
	void SetFontSize(int newsize);
	
	LNPanel *ln;					// line number display
	CSpacerView *spacer;			// spacer between ln and document
	CEditPane *editpane;			// main edit pane showing document
	DocScrollBar *VScrollbar;		// v-scrollbar for document
	DocScrollBar *HScrollbar;		// h-scrollbar for document
	BView *ScrollbarCorner;			// white square at intersection of h & v scrollbars
	BStringView *cmd_preview;		// command preview pane for ALT+ shortcuts (cmdseq.cpp)
	CFunctionList *functionlist;	// function list
	
	IShelfView *shelf;				// replicant shelf
};


class DocScrollBar : public BScrollBar
{
public:
	// BScrollBar's have no way to set the resizingMode in the constructor,
	// but it turns out that the modes we want are the default, so for now,
	// we accept a resizingMode parameter to be consistent, but just throw it away.
	DocScrollBar(BRect frame, uint32 resizingMode, orientation posture)
		: BScrollBar(frame, "doc_bar", NULL, 0, 100, posture)
	{ }
	
	virtual void ValueChanged(float newValue);
};

