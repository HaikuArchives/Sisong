
#include "editor.h"
#include "draw.h"

CFontDrawer::CFontDrawer(int font_size)
{
	font = new BFont(be_fixed_font);
	_face = B_REGULAR_FACE;
	
	SetSize(font_size);
}

CFontDrawer::~CFontDrawer()
{
	delete font;
}

/*
void c------------------------------() {}
*/

// sets the size of the font (but you must reapply it to the view before it will change)
void CFontDrawer::SetSize(int newsize)
{
font_height fh;

	font->SetSize(newsize);
	font->GetHeight(&fh);

	_ascent = (int)(fh.ascent + 0.5f);
	_descent = (int)(fh.descent + 0.5f);
	
	fontheight = (int)(((fh.ascent + fh.descent) + fh.leading) + 0.5);
	fontwidth = (int)(font->StringWidth("M") + 0.5);
}


void CFontDrawer::SetColors(BView *view, rgb_color fg, rgb_color bg)
{
	view->SetHighColor(fg);
	view->SetLowColor(bg);
}


int CFontDrawer::GetStringWidth(char *string)
{
	return (fontwidth * strlen(string));
}


void CFontDrawer::SetFace(BView *view, int newface)
{
	if (_face != newface || view != _lastview)
	{
		font->SetFace(newface);
		view->SetFont(font);
		
		_face = newface;
		_lastview = view;
	}
}


void CFontDrawer::DrawString(BView *view, char *string, int x, int y)
{
	view->MovePenTo(x, y + _ascent);
	view->DrawString(string);
}


void CFontDrawer::DrawString(BView *view, char *string, int x, int y, int len)
{
	view->MovePenTo(x, y + _ascent);
	view->DrawString(string, len);
}



