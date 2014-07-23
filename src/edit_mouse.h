
#ifndef EDIT_MOUSE_H
#define EDIT_MOUSE_H

struct MouseData
{
	int lx, ly;
	uint lastclicktime;
	int clickcount;
};

char PixelToCharCoords(EditView *ev, int *x_inout, int *y_inout);

#endif
