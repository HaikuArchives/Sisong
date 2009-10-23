
class OffscreenBuffer : public BView
{
public:
	OffscreenBuffer(int width, int height);
	~OffscreenBuffer();
	void BlitTo(BView *target, int x, int y);
	void BlitTo(BView *target, BRect source, BRect dest);
	void BlitTo(BView *target, int sx, int sy, int w, int h, int dx, int dy);
	void BlitRect(BView *target, int x1, int y1, int x2, int y2);

	BBitmap *bitmap;
	
	virtual void Lock();
	virtual void Unlock();
	
private:
	int lock_count;
};
