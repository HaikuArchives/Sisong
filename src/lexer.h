
/*

	each lexer structure consists of:

	- an entry state
		the entry state is passed to the lexer when the line is processed.
		the value passed should be equal to the exit state of the previous line.
		this tells information such as if we are within a quote or a block comment.

	* an exit state
		the exit state holds information about any operations which were not
		completed before the end of the line, such as a block comment which was
		started on the line but not closed.
	
	* symbol point list
		this holds a list of interesting points in the document. each point in the list
		denotes a color change from one type of syntax element to another.
*/

/*
	anytime we change a line, we re-lex it. if the exit state of the line changes, then
	we also have to re-lex/re-draw the following line, and so on. but we can stop drawing
	if we go off the screen (can stop lexing too? depends).
*/

/*
	ok so the lexer works something like quicksearch.
	
	we check the next char to see if it is a number. if so, then we read the number,
	properly processing 0x etc. otherwise, we enter identifier search mode:
	
	we read chars and follow the tree until we encounter:
		- the end of the tree. in this case it is not a lexeme.
			- if the char we hit is '/', we check for // and / *.
			- if the char we hit is '"' or '\'', we enter quote-scan mode.
			- if the char is '#' and there are no prior chars on the line other than
			  space or tab, we add the rest of the line up until a '//' or '/ *' as a
			  preprocessor token.
		- a space or EOL. in this case we check if the current node has an "answer".
		  if so, we have found an identifier.
*/

struct LexPoint
{
	int index;			// index within line where lexeme begins
	int type;			// type of lexeme detected
};

struct LexResult
{
	LexPoint *points;
	int npoints, alloc_size;
	
	int exitstate;
};

// for detecting identifiers
struct LTNode
{
	LTNode *branches[256];
	char terminator_type;
	uchar minbranch, maxbranch;
};

#define LTree	LTNode

struct LexInstance
{
	char *line, *line_end;
	char *curchar;
	char *gap_start, *gap_end;
	LexResult *output;
};

