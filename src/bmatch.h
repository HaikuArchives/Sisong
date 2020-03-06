

/*
	lexer split out to before drawing is done
	
	lexer identifies '(' parens outside of strings etc and marks them with COLOR_BRACE.
	
	bmatcher checks cursor to see if it is near a ( paren marked with COLOR_BRACE.
	if so, it finds the matching brace, and patches the color in the lexer point table
	to either COLOR_BRACE_MATCHED or COLOR_BRACE_UNMATCHED.
	
	could also have a flag set which marks whether a line contains any braces or not.
*/

struct BMatchData
{
	// 1 if a brace match is currently active (the cursor is currently over a brace)
	char active;
	// 1 if a match for the brace was found (this means x2,y2 is valid).
	char matched;
	
	// coordinates of the brace that was matched
	int match_x, match_y;
	
	// coordinates of the braces
	int x1, y1, x2, y2;
	
	// when a { or } brace is lighted, this is set to the indentation level of the brace,
	// which causes all tabs between y1 and y2 on that same level to be highlighted.
	// if no brace is lighted, this is set to -1.
	int highlight_tab_level;
};
