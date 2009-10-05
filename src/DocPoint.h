

// defines a position with a document
class DocPoint
{
public:
	DocPoint();
	DocPoint(EditView *ev, int x, int y);
	DocPoint(EditView *ev, int x, int y, clLine *line);
	DocPoint(DocPoint *d);
	bool IsValid();
	void Print();
	
	int x;			// index within line
	int y;			// line number point is on
	clLine *line;	// clLine structure the selpoint is on

	// ---------------------------------------
	
	void Set(EditView *ev, int x, int y);
	void Set(EditView *ev, int x, int y, clLine *line);
	void SetToCursor(EditView *ev);
	void SetToStart(EditView *ev);
	void SetToEnd(EditView *ev);

	void Increment();
	void Decrement();
	
	void IncrementBy(int count);
	void DecrementBy(int count);
	
	bool operator==(const DocPoint &other) const;
	bool operator>(const DocPoint &other) const;
	bool operator<(const DocPoint &other) const;
	bool operator>=(const DocPoint &other) const;
	bool operator<=(const DocPoint &other) const;	
};
