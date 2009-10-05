
// cursor handling
class EditCursor
{
public:
	// reference back to EditView parent
	EditView *ev;

	// 0-based cursor X (column) and Y (line).
	// this is in characters, so TAB counts as one char.
	int x, y;
	
	// the actual character position of the cursor on the screen, after accounting
	// for things like tabs (which are longer than one char).
	int screen_x, screen_y;	

	// x positioning mode
	int xseekmode;
	int xseekcoord;
	
	struct
	{
		int x, y;
		int xseekmode, xseekcoord;
	} last;
	
// -----------------------------------

	void left();
	void right();
	void up();
	void down();
	void pgdn();
	void pgup();
	void to_eol();
	void to_home();
	
	void move(int x, int y);
	void set_mode(int newmode);
	char *DescribeMode();
};
	
