#ifndef LEXRESULT_H
#define LEXRESULT_H

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

#endif // LEXRESULT_H