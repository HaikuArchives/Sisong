#ifndef SELECTION_H
#define SELECTION_H

// defines a selection start or end point
class SelPoint
{
public:
	int y;			// line number point is on
	int x;			// index within line
	clLine *line;	// stLine structure the selpoint is on
	
	void Increment();
	void Decrement();
	void SetToCursor(EditView *ev);
};

struct SelectionData
{
	bool present;
	// the ANCHOR is the point in the document that the selection was created at.
	// the EXTENT is the point that the selection is being dragged to.
	DocPoint anchor;
	DocPoint extent;
};

void selection_create(EditView *ev);
void selection_extend(EditView *ev);
void selection_drop(EditView *ev);
char selection_IsPointWithin(EditView *ev, int lineno, int x);
void GetSelectionExtents(EditView *ev, int *startx, int *starty, int *endx, int *endy);
void SetSelectionExtents(EditView *ev, int x1, int y1, int x2, int y2);
void selection_SelectCurrentLine(EditView *ev);
void selection_SelectCurrentWord(EditView *ev);
void selection_SelectAll(EditView *ev);
void selection_select_range(EditView *ev, DocPoint *start, DocPoint *end);
void selection_SelectFullLines(EditView *ev);

#endif // SELECTION_H
