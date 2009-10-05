
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include "../../../common/basics.h"

#include <Application.h>
#include <OS.h>
#include <Window.h>
#include <DirectWindow.h>
#include <View.h>
#include <Deskbar.h>
#include <Dragger.h>

#include "../../IComm/ICommClient.h"


#define APP_SIGNATURE			"application/SS-IComponent-CursorPos"

class CMainWindow : public BWindow
{
public:
	CMainWindow(BRect frame);
	~CMainWindow();
	
	virtual void MessageReceived(BMessage *msg);
	virtual bool QuitRequested();


private:
	BView *BackView;
};

class ComponentView : public BView
{
public:
	ComponentView(BRect frame, uint32 resizingMode);
	ComponentView(BMessage *archive);
	~ComponentView();
	
	status_t Archive(BMessage *archive, bool deep) const;
	static BArchivable *Instantiate(BMessage *archive);

	void Draw(BRect updateRect);
	void MouseDown(BPoint where);
	void MessageReceived(BMessage *msg);
	
	
private:
	void _Init();
	ICommClient *ICC;
	
	int fCursorX, fCursorY;
	BFont fSmallFont;
	BFont fBigFont;
};


// the application class
class MyApp : public BApplication
{
public:
	MyApp();
	CMainWindow *MainWindow;
};

