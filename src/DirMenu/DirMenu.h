
#ifndef _DIRMENU_H
#define _DIRMENU_H

class BPopUpMenu;
class BMenuField;
class DirMenuItem;
class IconCache;

class DirMenu : public BView
{
public:
	DirMenu(BRect frame, BPath *initialPath, uint32 resizingMode);
	~DirMenu();

	void SetPath(const char *path);
	
private:
	BMenuBar *fMenuBar;
	
	void Populate();
	void Clear();
	void AddItem(const char *path, const char *text);
	
	BPopUpMenu *fMenu;
	BMenuField *fMenuField;
	BPath *fCurPath;
	DirMenuItem *fSuperMenu;
};


#endif
