
#include <Path.h>
#include <Roster.h>
#include "editor.h"

#include "messages.h"
#include "NewProject.h"
#include "Prefs/Prefs.h"

#include "menu.fdh"
extern BFilePanel *FilePanel;


MainMenuBar::MainMenuBar(BRect frame, uint32 resizingMode)
	: BView(frame, "mainmenubarview", resizingMode, B_WILL_DRAW)
{
BMenu *menu;

	BRect r(Bounds());
	r.bottom = MENU_HEIGHT-1;
	bar = new BMenuBar(r, "mainmenubar");
	AddChild(bar);

	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("New", new BMessage(M_FILE_NEW), 'N', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("New from Template...", new BMessage(M_FILE_LOAD_TEMPLATE), 'T', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Open...", new BMessage(M_FILE_OPEN), 'O', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Reload from disk", new BMessage(M_FILE_RELOAD), 0, 0));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Save", new BMessage(M_FILE_SAVE), 'S', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Save As...", new BMessage(M_FILE_SAVE_AS), 'S', B_COMMAND_KEY | B_SHIFT_KEY));
	menu->AddItem(new BMenuItem("Save All", new BMessage(M_FILE_SAVE_ALL), 'S', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddItem(new BMenuItem("Save File as Template...", new BMessage(M_FILE_SAVE_TEMPLATE), 'T', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddItem(new BMenuItem("Save As Copy...", new BMessage(M_FILE_SAVE_COPY), 'C', B_COMMAND_KEY | B_SHIFT_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Copy File Location", new BMessage(M_FILE_COPY_LOCATION), 0, 0));
	menu->AddItem(new BMenuItem("Open Containing Folder", new BMessage(M_FILE_OPEN_CONTAINING_FOLDER), 0, 0));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Close", new BMessage(M_FILE_CLOSE), 'W', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Close All", new BMessage(M_FILE_CLOSE_ALL), 'W', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddItem(new BMenuItem("Close All but Active Document", new BMessage(M_FILE_CLOSE_OTHERS), 'W', B_COMMAND_KEY | B_SHIFT_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem(CreateLayoutPositionSubmenu("Load Layout", M_FILE_LOAD_LAYOUT1, false)));
	menu->AddItem(new BMenuItem(CreateLayoutPositionSubmenu("Save Layout", M_FILE_SAVE_LAYOUT1, true)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Exit", new BMessage(M_FILE_EXIT), 'Q', B_COMMAND_KEY));
	bar->AddItem(menu);

	menu = new BMenu("Edit");
	menu->AddItem(new BMenuItem("Undo", new BMessage(M_EDIT_UNDO), 'Z', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Redo", new BMessage(M_EDIT_REDO), 'Y', B_COMMAND_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Cut", new BMessage(M_EDIT_CUT), 'X', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Copy", new BMessage(M_EDIT_COPY), 'C', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Paste", new BMessage(M_EDIT_PASTE), 'V', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Select All", new BMessage(M_EDIT_SELECT_ALL), 'A', B_COMMAND_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Duplicate current line", new BMessage(M_EDIT_DUPLICATE), 'D', B_COMMAND_KEY));
	bar->AddItem(menu);

	menu = new BMenu("Search");
	menu->AddItem(new BMenuItem("Find...", new BMessage(M_SEARCH_FIND), 'F', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Replace...", new BMessage(M_SEARCH_REPLACE), 'F', B_COMMAND_KEY | B_SHIFT_KEY));
	menu->AddItem(new BMenuItem("Find in files...", new BMessage(M_SEARCH_FIND_FILES), 'F', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Find Next", new BMessage(M_SEARCH_FIND_NEXT), 'G', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Find Previous", new BMessage(M_SEARCH_FIND_PREV), 'G', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Show Search Results", new BMessage(M_SEARCH_SHOW_RESULTS), 0, 0));
	bar->AddItem(menu);

	/*menu = new BMenu("View");
	menu->AddItem(new BMenuItem("Next Tab", new BMessage(M_VIEW_NEXT_TAB), 0, 0));
	menu->AddItem(new BMenuItem("Previous Tab", new BMessage(M_VIEW_PREV_TAB), 0, 0));
	bar->AddItem(menu);*/

	SettingsMenu = new BMenu("Settings");
	SettingsMenu->AddItem(new BMenuItem("Preferences...", new BMessage(M_SETTINGS_PREFS)));
	SettingsMenu->AddItem(new BMenuItem("Styler Configurator...", new BMessage(M_SETTINGS_STYLES)));
	SettingsMenu->AddSeparatorItem();
	// add in the color schemes
	UpdateColorSchemesMenu();
	bar->AddItem(SettingsMenu);

	ProjectMenu = new BMenu("Projects");
	ProjectMenu->AddItem(new BMenuItem("New...", new BMessage(M_PROJECT_NEW), 'P', B_COMMAND_KEY));
	bar->AddItem(ProjectMenu);

	menu = new BMenu("Run");
	menu->AddItem(new BMenuItem("Execute Build Script", new BMessage(M_RUN_RUN), 'R', B_COMMAND_KEY));
	menu->AddItem(new BMenuItem("Build but Don't Run", new BMessage(M_RUN_BUILD_NO_RUN), 'R', B_COMMAND_KEY | B_CONTROL_KEY));
	menu->AddSeparatorItem();
	ShowConsoleItem = new BMenuItem("Always Show Console", new BMessage(M_RUN_SHOW_CONSOLE));
	menu->AddItem(ShowConsoleItem);
	menu->AddItem(new BMenuItem("Abort Compile", new BMessage(M_RUN_ABORT), 'A', B_COMMAND_KEY | B_CONTROL_KEY));
	bar->AddItem(menu);

	/*
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About", new BMessage(M_HELP_1), 0, 0));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Why did the chicken cross the road?", new BMessage(M_HELP_2), 0, 0));
	menu->AddItem(new BMenuItem("What is the meaning of life?", new BMessage(M_HELP_3), 0, 0));
	menu->AddItem(new BMenuItem("Quack like Duck", new BMessage(M_HELP_4), 0, 0));
	bar->AddItem(menu);	*/
}

BMenu *CreateLayoutPositionSubmenu(char *menuText, int msg_base, bool isSave)
{
BMenu *menu = new BMenu(menuText);
char text[80];
int i;

	const char *prepend = isSave ? "Save " : "";
	int shortcuts = !isSave ? B_COMMAND_KEY : B_COMMAND_KEY|B_CONTROL_KEY;

	for(i=0;i<3;i++)
	{
		sprintf(text, "%sPosition %d", prepend, i+1);
		menu->AddItem(new BMenuItem(text, new BMessage(msg_base+i), '1'+i, shortcuts));
	}

	return menu;
}

// returns the path to the "quicksave" layout snapshots from the File menu
char *GetQuickLayoutFilename(int index)
{
char *path;
char number[50];

	sprintf(number, "%d", index+1);

	path = (char *)smal(MAXPATHLEN);
	GetConfigDir(path);
	strcat(path, "silayout-");
	strcat(path, number);

	return path;
}

/*
void c------------------------------() {}
*/

// update the list of color schemes under the Settings menu
void MainMenuBar::UpdateColorSchemesMenu()
{
int i, count;

	// remove old entries
	count = SettingsMenu->CountItems();
	for(i=count-1;i>2;i--)
		delete SettingsMenu->RemoveItem(i);

	// repopulate
	count = ColorScheme::GetNumColorSchemes();
	for(i=0;i<count;i++)
	{
		BMessage *msg = new BMessage(M_SETTINGS_LOAD_COLORSET);
		msg->AddInt32("schemeNo", i);

		const char *name = ColorScheme::GetSchemeName(i);
		SettingsMenu->AddItem(new BMenuItem(name, msg, 0, 0));
	}

	// put back the mark on active scheme
	SetMarkedColorScheme(CurrentColorScheme.GetLoadedSchemeIndex());
}

void MainMenuBar::SetMarkedColorScheme(int index)
{
int i, count;

	index += 3;	// color scheme index -> menu item index
	count = SettingsMenu->CountItems();

	for(i=3;i<count;i++)
	{
		BMenuItem *item = SettingsMenu->ItemAt(i);
		item->SetMarked(index == i);
	}
}


/*
void c------------------------------() {}
*/

bool CMainWindow::IsMenuCommand(unsigned int code)
{
	return (code >= M_MENUCMD_FIRST && code <= M_MENUCMD_LAST);
}

void CMainWindow::ProcessMenuCommand(unsigned int code, BMessage *msg=NULL)
{
	LockWindow();
	editor.curev->cursor.set_mode(CM_FREE);

	bool ok = true;

	if (!FileMenu(code, msg))
	if (!EditMenu(code, msg))
	if (!SearchMenu(code, msg))
	if (!SettingsMenu(code, msg))
	if (!ProjectsMenu(code, msg))
	if (!RunMenu(code, msg))
	if (!ExtraMenu(code, msg))	// only accessible via F-key shortcuts
		ok = false;

	if (!ok)
	{
		Unimplemented();
	}

	editor.curev->RedrawView();
	UnlockWindow();
}


static bool FileMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_FILE_EXIT:
			be_app->PostMessage(B_QUIT_REQUESTED);
		break;

		case M_FILE_NEW:
			TabBar->SetActiveTab(CreateEditView(NULL));
		break;

		case M_FILE_OPEN: FileOpen(); break;
		case M_FILE_SAVE: FileSave(); break;
		case M_FILE_SAVE_AS: FileSaveAs(false, false); break;
		case M_FILE_SAVE_COPY: FileSaveAs(true, false); break;
		case M_FILE_SAVE_ALL: EditView::Save_All(); break;

		case M_FILE_CLOSE:
			editor.curev->ConfirmClose(false);
		break;

		case M_FILE_CLOSE_ALL:
		case M_FILE_CLOSE_OTHERS:
		{
			CList list = *editor.DocList;
			int count = list.CountItems();

			EditView *exception;
			char *message;

			if (code == M_FILE_CLOSE_OTHERS)
			{
				if (count <= 1) break;

				exception = editor.curev;
				message = "Close all tabs but current one?";
			}
			else
			{
				exception = NULL;
				message = "Close all open documents?";
			}
			BAlert *alert = new BAlert("", message, "No", "Yes", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
			alert->SetShortcut(0, B_ESCAPE);
			if (!alert->Go()) break;

			for(int i = 0;i<count;i++)
			{
				EditView *ev = (EditView *)list.ItemAt(i);

				if (ev != exception)
				{
					if (ev->ConfirmClose(true) == CEV_CLOSE_CANCELED)
						break;
				}
			}
		}
		break;

		case M_FILE_RELOAD:
		{
			if (editor.curev->IsUntitled)
			{
				(new BAlert("", "No saved copy is available-- file is untitled.", "OK"))->Go();
				return true;
			}

			BString prompt;
			prompt << "Please confirm: reload file \"" << GetFileSpec(editor.curev->filename)
				<< "\"? All unsaved changes will be lost!";

			BAlert *alert = new BAlert("", prompt.String(), "Cancel", "Do it");
			if (!alert->Go())
				break;

			editor.curev->ReloadFile();
		}
		break;

		case M_FILE_SAVE_TEMPLATE:
			run_template_saver();
		break;

		case M_FILE_LOAD_TEMPLATE:
			run_template_loader();
		break;

		case M_FILE_SAVE_LAYOUT1:
		case M_FILE_SAVE_LAYOUT2:
		case M_FILE_SAVE_LAYOUT3:
		{
			char *fname = GetQuickLayoutFilename(code - M_FILE_SAVE_LAYOUT1);

			if (save_layout(fname))
			{
				BString uhoh;
				uhoh << "Failed to save the layout '" << fname << "'.";

				(new BAlert("", uhoh, "OK"))->Go();
			}

			frees(fname);
		}
		break;

		case M_FILE_LOAD_LAYOUT1:
		case M_FILE_LOAD_LAYOUT2:
		case M_FILE_LOAD_LAYOUT3:
		{
			char *fname = GetQuickLayoutFilename(code - M_FILE_LOAD_LAYOUT1);

			stat("'%s'", fname);
			if (load_layout(fname))
			{	// layout probably doesn't exist yet, simulate it being empty
				MainWindow->ProcessMenuCommand(M_FILE_CLOSE_ALL);
			}

			frees(fname);
		}
		break;

		case M_FILE_COPY_LOCATION:
			SetClipboardText(editor.curev->filename);
		break;

		case M_FILE_OPEN_CONTAINING_FOLDER:
		{
			if (editor.curev->IsUntitled)
			{
				(new BAlert("", "Can't open containing folder because this file has not been saved yet.", "OK"))->Go();
				return true;
			}

			char *path = smal_strdup(editor.curev->filename);
			char *ptr = strrchr(path, '/');

			if (!ptr)
			{
				(new BAlert("", "Can't open containing folder because there was an error parsing the path.", "OK"))->Go();
				frees(path);
			}

			*(ptr + 1) = 0;
			OpenFolderInTracker(path);

			frees(path);
		}
		break;

		default: return false;
	}

	return true;
}

static bool EditMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_EDIT_UNDO:
			undo_undo(editor.curev);
			editor.curev->MakeCursorVisible();
		break;
		case M_EDIT_REDO:
			undo_redo(editor.curev);
			editor.curev->MakeCursorVisible();
		break;
		case M_EDIT_SELECT_ALL:
			selection_SelectAll(editor.curev);
			editor.curev->MakeCursorVisible();
		break;
		case M_EDIT_DUPLICATE:
			EditDuplicate(editor.curev);
			editor.curev->MakeCursorVisible();
		break;

		case M_EDIT_COPY:
		{
			if (editor.curev->selection.present)
				editor.curev->CopySelection();
		}
		break;

		case M_EDIT_CUT:
		{
			if (editor.curev->selection.present)
			{
				editor.curev->CopySelection();
				editor.curev->SelDel();
				editor.curev->MakeCursorVisible();
			}
		}
		break;

		case M_EDIT_PASTE:
		{
			editor.curev->SelDel();
			editor.curev->PasteFromClipboard();
			editor.curev->MakeCursorVisible();
		}
		break;

		default: return false;
	}

	return true;
}

static bool SearchMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_SEARCH_FIND: new CFindBox(FINDBOX_FIND); break;
		case M_SEARCH_REPLACE: new CFindBox(FINDBOX_REPLACE); break;
		case M_SEARCH_FIND_FILES: new CFindBox(FINDBOX_FIND_FILES); break;

		case M_SEARCH_FIND_NEXT:
		case M_SEARCH_FIND_PREV:
		{
			if (editor.curev->search.lastSearch)
			{
				if (code == M_SEARCH_FIND_PREV)
					editor.curev->search.lastOptions |= FINDF_BACKWARDS;
				else
					editor.curev->search.lastOptions &= ~FINDF_BACKWARDS;

				DoFindNext(editor.curev->search.lastSearch,
							editor.curev->search.lastOptions);
			}
			else
			{
				new CFindBox(FINDBOX_FIND);
			}
		}
		break;

		case M_SEARCH_SHOW_RESULTS:
		{
			BString title;

			MainWindow->popup.searchresults->GetCaptionForTitlebar(&title);

			MainWindow->popup.pane->SetContents(title.String(), \
												MainWindow->popup.searchresults);

			MainWindow->popup.pane->Open();
		}
		break;

		default: return false;
	}

	return true;
}

static bool SettingsMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_SETTINGS_PREFS:
		{
			new PrefsWindow();
		}
		break;

		case M_SETTINGS_LOAD_COLORSET:
		{
			int32 schemeNo;
			if (msg && msg->FindInt32("schemeNo", &schemeNo) == B_OK)
			{
				CurrentColorScheme.LoadScheme(schemeNo);

				MainWindow->main.editarea->Invalidate();
				rd_invalidate_all(editor.curev);
			}
		}
		break;

		default: return false;
	}

	return true;
}

static bool ProjectsMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_PROJECT_NEW:
			new NewProjectWindow();
		break;

		case M_PROJECT_SELECT:
		{
			const char *projectPath;

			if (msg && msg->FindString("path", &projectPath) == B_OK)
			{
				// save current project if any...
				if (!ProjectManager.SaveProject())
				{
					// switch to new one
					ProjectManager.OpenProject(projectPath);
				}
			}
		}
		break;

		case M_PROJECT_OPEN_SCRIPT:
		{
			const char *projectPath;

			if (msg && msg->FindString("path", &projectPath) == B_OK)
			{
				ProjectManager.OpenBuildScript(projectPath);
			}
		}
		break;

		case M_PROJECT_CLOSE:
			ProjectManager.ExitProjectMode();
		break;

		case M_PROJECT_DELETE:
		{
			const char *projectPath;
			if (!msg || msg->FindString("path", &projectPath) != B_OK) break;

			BString str;
			str << "Are you sure you want to delete the project \"" << GetFileSpec(projectPath);
			str << "\"?\n\n(This will remove the project from the menu and delete " <<
				"the build script, but will not delete any actual source files).\n";

			BAlert *alert = new BAlert("", str, "Cancel", "Delete", NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
			if (!alert->Go()) break;

			if (MoveToTrash(projectPath))
				(new BAlert("", "Operation failed.", "OK"))->Go();

			ProjectManager.UpdateProjectsMenu();
		}
		break;

		default: return false;
	}

	return true;
}


static bool RunMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_RUN_RUN:
		case M_RUN_BUILD_NO_RUN:
		{
			EditView::Save_All();
			MainWindow->popup.compile->RunScript((code == M_RUN_RUN));
		}
		break;

		case M_RUN_ABORT:
			MainWindow->popup.compile->AbortThread();
		break;

		case M_RUN_SHOW_CONSOLE:
		{
			BMenuItem *ShowConsoleItem = MainWindow->top.menubar->ShowConsoleItem;

			if (ShowConsoleItem->IsMarked())
			{
				ShowConsoleItem->SetMarked(false);
				MainWindow->popup.pane->Close();
			}
			else
			{
				ShowConsoleItem->SetMarked(true);

				MainWindow->popup.pane->SetContents("Build", \
													MainWindow->popup.compile);

				MainWindow->popup.pane->Open();
			}
		}
		break;

		default: return false;
	}

	return true;
}

static bool ExtraMenu(unsigned int code, BMessage *msg)
{
	switch(code)
	{
		case M_EXTRA_FULLREDRAW:
			if (editor.curev)
				editor.curev->FullRedrawView();
		break;

		default: return false;
	}

	return true;
}


/*
void c------------------------------() {}
*/

void DismissFilePanel()
{
	if (FilePanel)
	{
		delete FilePanel;
		FilePanel = NULL;
		//stat("FP dismissed");
	}
}

void FileOpen()
{
	DismissFilePanel();

	FilePanel = new BFilePanel(B_OPEN_PANEL);
	FilePanel->SetTarget(MainWindow);

	char *dir = NULL;
	if (editor.curev->IsUntitled)
	{
		if (editor.last_filepath_reference[0])
		{
			dir = RemoveFileSpec(editor.last_filepath_reference);
		}
	}
	else
	{
		dir = RemoveFileSpec(editor.curev->filename);
	}

	if (dir)
	{
		FilePanel->SetPanelDirectory(dir);
		frees(dir);
	}

	FilePanel->Show();
}

char FileSave()
{
	editor.curev->CloseAfterSave = false;

	if (editor.curev->IsUntitled)
	{
		FileSaveAs(false, false);
		return 0;
	}

	if (!editor.curev->IsDirty)
	{
		stat("save: '%s' not dirty; save command ignored", editor.curev->filename);
		return 0;
	}

	if (editor.curev->Save(editor.curev->filename))
	{
		(new BAlert("", "Sorry, but I could not save the file. An OS error occurred!", "Damned!"))->Go();
		return 1;
	}
	else
	{
		stat("save: '%s' written ok", editor.curev->filename);
		editor.curev->ClearDirty();
		return 0;
	}
}

void FileSaveAs(bool as_copy, bool shutdown_afterwards)
{
	BMessage msg(B_SAVE_REQUESTED);
	msg.AddInt32("DocID", editor.curev->DocID);
	msg.AddBool("as_copy", as_copy);
	msg.AddBool("shutdown_afterwards", shutdown_afterwards);

	DismissFilePanel();
	FilePanel = new BFilePanel(B_SAVE_PANEL, NULL, NULL, 0, false, &msg);
	FilePanel->SetTarget(MainWindow);

	BString title;
	title << "Save " << GetFileSpec(editor.curev->filename);
	if (as_copy)
	{
		title << " as Copy";
	}
	FilePanel->Window()->SetTitle(title.String());

	if (!editor.curev->IsUntitled)
	{
		char *dir = RemoveFileSpec(editor.curev->filename);
		FilePanel->SetPanelDirectory(dir);
		frees(dir);

		FilePanel->SetSaveText(GetFileSpec(editor.curev->filename));
	}
	else
	{
		// we're saving an untitled file, so we don't know which directory
		// it should go in. make an educated guess if we can.
		if (editor.last_filepath_reference[0])
		{
			char *dir = RemoveFileSpec(editor.last_filepath_reference);
			FilePanel->SetPanelDirectory(dir);
			frees(dir);
		}
	}

	FilePanel->Show();
}

void FinishFileSaveAs(BMessage *msg)
{
EditView *ev;
entry_ref dir;
const char *filespec;
char *filename;
int32 DocID;
bool as_copy;
bool shutdown_afterwards = false;

	if (msg->what != B_SAVE_REQUESTED) goto failure;
	if (msg->FindInt32("DocID", &DocID) != B_OK) goto failure;
	if (msg->FindRef("directory", &dir) != B_OK) goto failure;
	if (msg->FindString("name", &filespec) != B_OK) goto failure;
	if (msg->FindBool("as_copy", &as_copy) != B_OK) goto failure;
	msg->FindBool("shutdown_afterwards", &shutdown_afterwards);

	if (0)
	{
failure: ;
		BAlert *a = new BAlert("", "The file save operation failed because of an internal error: The BMessage is malformed!", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		a->Go();
		return;
	}

	ev = FindEVByDocID(DocID);
	if (ev == NULL)
	{
		BAlert *alert = new BAlert("", "Unable to save the file, because the document is no longer open (it's tab has been closed since the Save panel was opened).", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		alert->Go();
		return;
	}

	BEntry entry(&dir, true);
	BPath path;

	entry.GetPath(&path);
	path.Append(filespec);

	filename = (char *)path.Path();

	// change stored file name to the new one
	if (!as_copy)
	{
		maxcpy(ev->filename, filename, sizeof(ev->filename) - 1);
		ev->IsUntitled = false;
	}

	maxcpy(editor.last_filepath_reference, filename, sizeof(editor.last_filepath_reference) - 1);

	if (ev->Save(filename))
	{
		BAlert *a = new BAlert("", "Sorry, but the Save As operation failed!", "OK");
		a->Go();
	}
	else
	{
		stat("'%s' written ok", editor.curev->filename);
		ev->ClearDirty();

		if (ev->CloseAfterSave)
		{
			ev->Close();
		}

		if (shutdown_afterwards)
		{
			MainWindow->PostMessage(B_QUIT_REQUESTED);
		}
	}

	// update displayed filenames
	MainWindow->UpdateWindowTitle();
	TabBar->redraw();

	// must not dismiss file panel till we're done with "msg"--
	// "msg" is owned by the FilePanel.
	DismissFilePanel();
	return;
}


void EditDuplicate(EditView *ev)
{
clLine *line;
int final_y;

	// create a string containing <CR> followed by contents of current line
	line = ev->GetLineHandle(ev->cursor.y);
	if (!line) { errorblip(); return; }	// just in case

	BString *LineString = line->GetLineAsString();
	LineString->Prepend("\n");

	// insert <CR> plus <duplicate line> at end of current line
	BeginUndoGroup(ev);
	ev->action_insert_string(line->GetLength(), ev->cursor.y,
						(char *)LineString->String(), NULL, &final_y);

	ev->cursor.move(0, final_y);
	delete LineString;
	EndUndoGroup(ev);
}

