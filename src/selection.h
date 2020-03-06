
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
