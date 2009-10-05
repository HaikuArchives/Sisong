
class CFlashingCursor
{
public:
	CFlashingCursor(BView *parentView);
	
	void tick();
	void draw();
	void erase();
	void erase(int cx, int cy);
	void move(int x, int y);
	void bump();	
	void EnableFlashing(bool newActive);
	void SetThick(bool enabled);
	
	char flashstate;
	char visible;
	int timer;

	inline int get_x() { return _cx; }
	inline int get_y() { return _cy; }
	
private:
	BView *view;		// associated editview
	int _cx, _cy;
	bool fActive;
	bool fThick;
	int fFlashRate;
};
