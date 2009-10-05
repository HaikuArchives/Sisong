
#include "CursorPos.h"
#include "../../IComm/IComm.h"
#include "CursorPos.fdh"

int main(void)
{
	MyApp app;
	app.Run();

	return 0;
}

MyApp::MyApp()
	: BApplication(APP_SIGNATURE)
{
	int x = 223;
	int y = 58;
	int w = 120;
	int h = 70;
	BRect rc(x, y, x+(w-1), y+(h-1));
	MainWindow = new CMainWindow(rc);
}

CMainWindow::CMainWindow(BRect frame)
	: BWindow(frame, "CursorPos", B_TITLED_WINDOW,
	B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_ASYNCHRONOUS_CONTROLS)
{
	CenterOnScreen();
	
	BackView = new BView(Bounds(), "background_view", B_FOLLOW_ALL, 0);
	BackView->SetViewColor(0,0,41);
	AddChild(BackView);
	
	BRect rc(Bounds());
	
	ComponentView *replicant = new ComponentView(rc, B_FOLLOW_ALL);
	BackView->AddChild(replicant);
	
	Show();
}

CMainWindow::~CMainWindow()
{
}

/*
void c------------------------------() {}
*/

ComponentView::ComponentView(BRect frame, uint32 resizingMode)
	: BView(frame, "CursorPos", resizingMode, B_WILL_DRAW | B_FULL_UPDATE_ON_RESIZE)
{	
	BRect rc(Bounds());
	rc.left = rc.right - 8;
	rc.top = rc.bottom - 8;
	AddChild(new BDragger(rc, this, B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM));

	_Init();
}

ComponentView::ComponentView(BMessage *archive)
	: BView(archive)
{
	//fMouseInside = false;
	//archive->FindBool("fMouseInside", &fMouseInside);
	
	_Init();
}


void ComponentView::_Init()
{
	fBigFont = be_fixed_font;
	fBigFont.SetSize(14);

	fSmallFont = be_fixed_font;
	fSmallFont.SetSize(10);
	
	// create link to server (editor)
	ICC = NULL;
	
	// garbage values for testing
	fCursorX = 99;
	fCursorY = 999;
}


ComponentView::~ComponentView()
{
	if (ICC)
		delete ICC;
}

/*
void c------------------------------() {}
*/

status_t ComponentView::Archive(BMessage *archive, bool deep) const
{
	BView::Archive(archive, deep);

	//archive->AddBool("fMouseInside", fMouseInside);

	archive->AddString("add_on", APP_SIGNATURE);
	archive->AddString("class", "SpiderView");
	
	return B_OK;
}

BArchivable *ComponentView::Instantiate(BMessage *archive)
{
	if (!validate_instantiation(archive, "SpiderView"))
		return NULL;
	
	return new ComponentView(archive);
}

void ComponentView::Draw(BRect updateRect)
{
	SetLowColor(0, 40, 0);
	SetHighColor(0, 255, 0);
	
	FillRect(updateRect, B_SOLID_LOW);
	
	float middle = WIDTHOF(Bounds()) / 2;
	
	// draw "infotext"
	static const char *infotext = "Cursor Position";
	
	SetFont(&fSmallFont);
	MovePenTo((middle - (fSmallFont.StringWidth(infotext) / 2)), 14);
	DrawString(infotext);
	
	// draw cursor pos
	char cursortext[80];
	sprintf(cursortext, "%d : %d", fCursorX, fCursorY);

	SetFont(&fBigFont);
	MovePenTo((middle - (fBigFont.StringWidth(cursortext) / 2)), 44);
	DrawString(cursortext);	
}

void ComponentView::MouseDown(BPoint where)
{
	printf("Replicant: you clicked me!\n");
	fflush(stdout);
	
	if (ICC)
	{
		int y = ICC->GetCurrentLine();
		stat("got y=%d", y);
	}
}

void ComponentView::MessageReceived(BMessage *msg)
{
	printf("Replicant: Got message %s\n", strwhat(msg->what));
	fflush(stdout);
	
	switch(msg->what)
	{
		case MIC_HELLO:
		{
			BMessenger editor;
			if (msg->FindMessenger("ServerLink", &editor) == B_OK)
			{
				stat("ServerLink is B_OK, instantiating connection");
				ICC = new ICommClient(editor);
			}
			
			stat("Hello to you too, Mr. Editor.");
		}
		break;
	}
	
	
	BView::MessageReceived(msg);
}


char *strwhat(uint in)
{
static char abcd[5];

	abcd[3] = in;
	abcd[2] = (in >> 8);
	abcd[1] = (in >> 16);
	abcd[0] = (in >> 24);
	abcd[4] = 0;
	return abcd;
}

/*
void c------------------------------() {}
*/

bool CMainWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

void CMainWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case B_KEY_DOWN:
			be_app->PostMessage(B_QUIT_REQUESTED);
		break;
		
		default:
			BWindow::MessageReceived(msg);
		break;
	}	
}





