
#include <OS.h>
#include <View.h>
#include <Bitmap.h>
#include "OffscreenBuffer.h"
#include "OffscreenBuffer.fdh"

OffscreenBuffer::OffscreenBuffer(int width, int height)
	: BView(BRect(0, 0, width-1, height-1), "Offscreen Buffer",
		B_FOLLOW_ALL_SIDES, B_WILL_DRAW)
{
	lock_count = 0;
	bitmap = new BBitmap(Bounds(), B_RGB32, true);
	
	Lock();
	bitmap->AddChild(this);
	Unlock();
}

OffscreenBuffer::~OffscreenBuffer()
{
	Lock();
	bitmap->RemoveChild(this);
	Unlock();
	
	delete bitmap;
}

void OffscreenBuffer::Lock()
{
	if (lock_count == 0)
		bitmap->Lock();
	
	lock_count++;
}

void OffscreenBuffer::Unlock()
{
	if (lock_count > 0)
	{
		lock_count--;
		
		if (lock_count == 0)
			bitmap->Unlock();
	}
}

void OffscreenBuffer::BlitTo(BView *target, int x, int y)
{
	Sync();
	target->DrawBitmap(bitmap, BPoint(x, y));
}

void OffscreenBuffer::BlitTo(BView *target, BRect source, BRect dest)
{
	Sync();
	target->DrawBitmap(bitmap, source, dest);
}

void OffscreenBuffer::BlitTo(BView *target, int sx, int sy, int w, int h, int dx, int dy)
{
BRect source, dest;

	source.Set(sx, sy, (sx + (w - 1)), (sy + (h - 1)));
	dest.Set(dx, dy, (dx + (w - 1)), (dy + (h - 1)));
	
	BlitTo(target, source, dest);
}

void OffscreenBuffer::BlitRect(BView *target, int x1, int y1, int x2, int y2)
{
BRect r;

	r.Set(x1, y1, x2, y2);
	BlitTo(target, r, r);
}

