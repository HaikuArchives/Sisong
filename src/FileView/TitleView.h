
#ifndef _TITLEVIEW_H
#define _TITLEVIEW_H

class TitleView : public BView
{
public:
	TitleView(BRect frame, uint32 resizingMode);	
	virtual void Draw(BRect updateRect);
	
private:
};

#endif
