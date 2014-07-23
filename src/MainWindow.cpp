
#include "editor.h"
#include "MainWindow.h"

namespace UpdateCheck { void Go(uint delay); }

CMainWindow::CMainWindow(BRect frame)
	: BWindow(frame, "Sisong", B_TITLED_WINDOW, 0)
{
BRect bo(Bounds());
BRect rc;

	MainWindow = this;
	fClosing = false;
	fDoingInstantQuit = false;
	fWindowIsForeground = true;
	
	/* create top area (menu bar and tabs) */
	rc.Set(0, 0, bo.right, TOPVIEW_HEIGHT-1);
	top.topview = new BView(rc, "topview", B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT, 0);
	top.topview->SetViewColor(B_TRANSPARENT_COLOR);
	AddChild(top.topview);
	{
		// menu bar
		rc.Set(0, 0, bo.right, MENU_HEIGHT-1);
		top.menubar = new MainMenuBar(rc, B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT);
		top.topview->AddChild(top.menubar);
		
		// tab bar
		rc = top.topview->Bounds();
		rc.top = MENU_HEIGHT;
		rc.left++;
		top.tabbar = new CTabBar(rc, B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT);
		top.topview->AddChild(top.tabbar);
		
		// the 1-pixel gap at left of tabbar
		rc.left = rc.right = 0;
		BView *gapview = new BView(rc, "tbgap", B_FOLLOW_LEFT | B_FOLLOW_TOP, 0);
		gapview->SetViewColor(top.tabbar->fBackgroundColor);
		AddChild(gapview);
	}
	
	// main editing area
	rc = bo;
	rc.top = TOPVIEW_HEIGHT;
	main.editarea = new CEditArea(rc, B_FOLLOW_ALL);
	AddChild(main.editarea);
	
	// popup panes
	popup.pane = new PopupPane();
	popup.searchresults = new SearchResultsPane();
	popup.compile = new CompilePane();
	popup.buildhelp = new BuildHelpPane();
	
	// this is a joke that nobody will get, a reference to an easter egg in
	// the firmware of something called "Mirack".
	static const rgb_color green = { 0, 255, 0 };
	popup.pane->SetContents("Build", popup.compile);
	popup.compile->AddLine("Bunnies, enchiladas, and tin.", green, false);
	popup.compile->AddLine(" ;-)", green, false);
	
	// init color scheme
	CurrentColorScheme.LoadScheme(settings->GetInt("SelectedColorScheme", 1));
	
	// cursor-flashing pulsar thread
	cursor_timer = new CViewTimer(this, M_CURSOR_TIMER, 100);
	
	// spawn the thread that notifies if an update is available
	if (editor.settings.CheckForUpdate)
		UpdateCheck::Go(100 * 1000);
	
	main.editarea->editpane->MakeFocus();
	
	Show();
	//testprefs();
}

CMainWindow::~CMainWindow()
{
	settings->SetInt("SelectedColorScheme", CurrentColorScheme.GetLoadedSchemeIndex());
	
	// this needs to be done here, because once we leave here, our tab bar will
	// be auto-destroyed, and we don't want to try closing documents with an
	// invalid tab bar pointer.
	stat("~CMainWindow: Close_All()");
	EditView::Close_All();
	
	// close popup panes
	popup.pane->Close();
	popup.pane->RemoveContents();
	delete popup.pane;
	delete popup.searchresults;
	
	delete cursor_timer;
	cursor_timer = NULL;
	
	// save window position
	settings->SetInt("window_left", (int)Frame().left);
	settings->SetInt("window_right", (int)Frame().right);
	settings->SetInt("window_top", (int)Frame().top);
	settings->SetInt("window_bottom", (int)Frame().bottom);
}

void CMainWindow::UpdateWindowTitle()
{
	if (editor.curev)
	{
		BString app_title(editor.curev->filename);
		
		const char *prjName = ProjectManager.GetCurrentProjectName();
		if (prjName[0])
		{
			app_title.Append(" - ");
			app_title.Append(prjName);
		}
		
		app_title.Append(" - ");
		app_title.Append(APPLICATION_NAME);
		
		SetTitle(app_title.String());
	}
	else
	{
		SetTitle(APPLICATION_NAME);
	}
}

/*
void c------------------------------() {}
*/

void CMainWindow::DispatchMessage(BMessage *message, BHandler *handler)
{
	switch(message->what)
	{
		// have to hook keyboard input here so we can catch modifier keys
		// that would otherwise be gobbled up by the menu manager
		case B_KEY_DOWN:
		{
			const char *bytes;
			int32 ch;
			if (message->FindString("bytes", &bytes) == B_OK)
			{
				if (!MainView->IsFocus())
					MainView->MakeFocus();
				
				ch = *bytes;
				if (ch == B_FUNCTION_KEY)
				{
					int32 fkey;
					message->FindInt32("key", &fkey);
					fkey -= B_F1_KEY;
					
					if (fkey >= 0 && fkey < NUM_F_KEYS)
					{
						int what = editor.settings.fkey_mapping[fkey];
						if (what) ProcessMenuCommand(what);
					}
				}
				else if (ch == B_ESCAPE && !editor.settings.esc_quits_immediately)
				{
					if (editor.curev->IsCommandSeqActive())
					{
						editor.curev->CancelCommandSeq();
					}
					else
					{
						!popup.pane->IsOpen() ?
								popup.pane->Open() :
								popup.pane->Close();
					}
				}
				else
				{
					editor.curev->HandleKey(ch);
				}
			}
			
			// eat tabs, we don't want focus changing on us.
			switch(*bytes)
			{
				case '\t':
					if (!IsAltDown() && !IsCtrlDown() && !IsShiftDown())
					{
						break;
					}
					// fall thru
				default:
					BWindow::DispatchMessage(message, handler);
			}
		}
		break;
		
		case B_MOUSE_WHEEL_CHANGED:
		{
			float fDelta;
			int delta_y;
			
			if (MainView->IsFocus())
			{
				message->FindFloat("be:wheel_delta_y", &fDelta);
				delta_y = (int)fDelta;
				
				int key = (delta_y > 0) ? KEY_MOUSEWHEEL_DOWN : KEY_MOUSEWHEEL_UP;
				editor.curev->HandleKey(key);
			}
			else
			{
				BWindow::DispatchMessage(message, handler);
			}
		}
		break;
		
		default:
		{
			BWindow::DispatchMessage(message, handler);
		}
		break;
	}
}

void CMainWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
		case M_CURSOR_TIMER:
		{
			if (MainView) MainView->cursor.tick();
			if (FunctionList) FunctionList->TimerTick();
			AutoSaver_Tick();
			
			Stats_Tick(fWindowIsForeground || IsPrefsWindowOpenAndActive());
		}
		break;
		
		case M_TAB_CHANGED:
		{
			EditView *newev;
			
			if (message->FindPointer("ev", (void **)&newev) == B_OK)
			{
				if (editor.curev != newev)
				{
					editor.curev = newev;
					editor.curev->FullRedrawView();
				}
			}
		}
		break;
		
		// a dumb hack; see comments in FindBox for why I do this...
		case M_FINDBOX_PINGPONG:
		{
			int32 old_what;
			
			if (message->FindInt32("old_what", &old_what) == B_OK)
			{
				message->what = old_what;
				CFindBox *FindBox = GetCurrentFindBox();
				
				if (FindBox)
				{
					FindBox->LockLooper();
					FindBox->HandleSearchRequest(message);
					FindBox->UnlockLooper();
				}
			}
		}
		break;
		
		case M_FUNCTIONLIST_INVOKE:
		{
			int32 index;
			
			if (message->FindInt32("index", &index) == B_OK)
			{
				FunctionList->JumpToIndex(index);
			}
		}
		break;
		
		case M_SEARCHRESULTS_INVOKE:
		{
			int32 index;
			
			if (message->FindInt32("index", &index) == B_OK)
			{
				if (popup.searchresults)
					popup.searchresults->ItemClicked(index);
			}
		}
		break;
		
		case M_COMPILEPANE_INVOKE:
		{
			int32 index;
			
			if (message->FindInt32("index", &index) == B_OK)
			{
				if (popup.compile)
					popup.compile->ItemClicked(index);
			}
		}
		break;
		
		case M_POPUPPANE_CLOSE:
		{
			if (MainWindow->popup.pane)
				MainWindow->popup.pane->Close();
		}
		break;
		
		case B_SAVE_REQUESTED:
			FinishFileSaveAs(message);
		break;
		
		case B_REFS_RECEIVED:
		case 'DATA':	// drag n' drop
			ProcessRefs(message);
		break;
		
		case B_CANCEL:
			DismissFilePanel();
		break;
		
		default:
			if (IsMenuCommand(message->what))
			{
				ProcessMenuCommand(message->what, message);
			}
			else
			{
				BWindow::MessageReceived(message);
			}
		break;
	}
}

void CMainWindow::ProcessRefs(BMessage *message)
{
int i;
uint32 type;
int32 count;
entry_ref ref;
EditView *ev;

	message->GetInfo("refs", &type, &count);
	if (type != B_REF_TYPE) return;
	
	LockLooper();
	
	for(i=0;i<count;i++)
	{
		if (message->FindRef("refs", i, &ref) == B_OK)
		{
			BEntry entry(&ref, true);
			BPath path;
			
			entry.GetPath(&path);
			const char *filename = path.Path();
			
			// check that it is a file and not a directory
			BDirectory dircheck(filename);
			if (dircheck.InitCheck() == B_OK)
				continue;
			
			// if file is already open don't open another copy
			ev = FindEVByFilename(filename);
			if (!ev)
			{
				ev = CreateEditView((char *)filename);
			}
		}
	}
	
	top.tabbar->SetActiveTab(ev);
	DismissFilePanel();	// harmless if file panel isn't open
	UnlockLooper();
}

/*
void c------------------------------() {}
*/

void CMainWindow::WindowActivated(bool active)
{
	if (main.editarea && main.editarea->editpane)
		main.editarea->editpane->cursor.EnableFlashing(active);
	
	if (active)
		ProjectManager.UpdateProjectsMenu();
	
	fWindowIsForeground = active;
}

bool CMainWindow::QuitRequested()
{
	// if we've already been here, just return true. this prevents a bug
	// where the layout file was corrupted if SS was quit with the compile
	// pane running, because in that case we will be called twice for some reason.
	if (fClosing) return true;
	
	if (fDoingInstantQuit || ConfirmCloseSaveFiles())
	{
		fClosing = true;
		ProjectManager.SaveProject();
		be_app->PostMessage(B_QUIT_REQUESTED);
		return true;
	}
	
	return false;
}

// triggered from QuitRequested, when the user clicks Close on window, selects Exit
// from menu, or uses ESC quick-quit shortcut.
//
// loops through each unsaved document and asks if the user wants to save it.
//
// if the shutdown is aborted, returns false.
bool ConfirmCloseSaveFiles()
{
EditView *ev;
int i;

	for(i=0;
		ev = (EditView *)editor.DocList->ItemAt(i);
		i++)
	{
		if (!ev->IsDirty) continue;
		
		TabBar->SetActiveTab(ev);
		ev->FullRedrawView();
		
		BString prompt;
		prompt << "Save \"" << GetFileSpec(ev->filename) << "\" before quitting?";
		
		BAlert *alert = new BAlert("", prompt.String(), \
							"Cancel", "Don't Save", "Save It",
							B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		
		switch(alert->Go())
		{
			case 0: return false;		// Cancel
			case 1:	// Don't Save
			break;
			
			case 2:	// Save
			{
				if (!ev->IsUntitled)
				{
					// document already has a name, so this will save immediately,
					// and return to us.
					if (FileSave())
					{
						// Save Failed
						return false;
					}
				}
				else
				{
					// document is Untitled, so popup Save As box. Save As is NOT modal,
					// so we abort the shutdown for now, but ask Save As box to continue
					// it after user makes the selection
					FileSaveAs(false, true);
					return false;
				}
			}
			break;
		}
	}
	
	return true;
}

