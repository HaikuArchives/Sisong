
class clLine;
#include "line.fdh"

// holds a single line of text
class clLine
{
public:
	clLine(const char *initial_string="");
	~clLine();
	
	char *text;				// pointer to buffer
	int length;				// length of buffer, including gap
	
	int gap_start;			// first index which is in the gap
	int gap_end;			// last index which is in the gap
	
	int pxwidth;			// width of the line in pixels: only update when line is drawn
	
	LexResult lexresult;	// syntax-coloring points from lexer
	
	clLine *next, *prev;
	
	int GetLength();
	void SetContents(const char *newContents);
// -------------------------------

	void insert_char(char ch);
	void insert_string(char *str);
	void delete_left();
	void delete_right();
	void delete_range(int x1, int x2);
	void DeleteAfterIndex(int index);
	char GetCharAtIndex(int index);
	void GetLineToBuffer(char *buffer);
	BString *GetLineAsString();
	BString *GetPartialLine(int start, int end);
	
	void set_insertion_point(int x);
	int get_insertion_point();
	
	int GetIndentationLevel();
	int ScreenCoordToCharCoord(int x);
	int CharCoordToScreenCoord(int x);
	
	void GetWordExtent(int x, int *x1_out, int *x2_out);

private:
	void MoveGapRight(int offset);
	void MoveGapLeft(int offset);
};
