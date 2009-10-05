
#include <stdio.h>
#include <stdlib.h>
#include <View.h>
#include "testview.h"

testview::testview(BRect frame, uint32 resizingMode)
		: BView(frame, "Test", resizingMode, 
		B_WILL_DRAW|B_FULL_UPDATE_ON_RESIZE)
{	
	r = (rand() & 255) >> 1;
	g = rand();
	b = rand();
	// don't erase before redraw, we'll take care of it
	SetViewColor(B_TRANSPARENT_COLOR);
}

testview::~testview()
{
	/*printf("testview destroyed\n");
	fflush(stdout);*/
}

void testview::Draw(BRect where)
{
	SetHighColor(r, g, b);
	FillRect(Bounds());
	
	BRect rc(Bounds());
	SetHighColor(255, 0, 0);
	StrokeRect(rc);
	/*rc.InsetBy(1, 1);
	StrokeRect(rc);
	rc.InsetBy(1, 1);
	StrokeRect(rc);*/
	
	BPoint s, e;
	
	s.Set(rc.left, rc.top);
	e.Set(rc.right, rc.bottom);
	StrokeLine(s, e);
	s.Set(rc.left, rc.bottom);
	e.Set(rc.right, rc.top);
	StrokeLine(s, e);
}



