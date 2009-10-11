
#include "editor.h"
#include "projects.fdh"


CProjectManager::CProjectManager()
{
	fCurProject[0] = 0;
	fProjectPath[0] = 0;
}

// update the projects menu with the list of active projects
void CProjectManager::UpdateProjectsMenu()
{
BMenu *menu, *submenu;
BList projects;
char configdir[MAXPATHLEN];
int i, count;

	MainWindow->top.menubar->ChangeMenusLock.Lock();
	menu = MainWindow->top.menubar->ProjectMenu;

	// remove the old project entries
	count = menu->CountItems();
	for(i=count-1;i>0;i--)
		delete menu->RemoveItem(i);

	// check the filesystem for a list of projects
	if (!GetConfigDir(configdir)) return;
	if (GetDirectoryContents(configdir, NULL, NULL, &projects)) return;

	if ((count = projects.CountItems()))
	{
		menu->AddSeparatorItem();

		for(i=0;i<count;i++)
		{
			// get text to show and path to project
			char *path = (char *)projects.ItemAt(i);
			char *menuText = path;

			char *ptr = strrchr(menuText, '/');
			if (ptr) menuText = (ptr + 1);

			// create menu items
			if (menuText[0] != '.')
			{
				submenu = new BMenu(menuText);
				submenu->AddItem(new BMenuItem("Open", NewMenuMessage(M_PROJECT_SELECT, path)));
				submenu->AddItem(new BMenuItem("Open Build Script", NewMenuMessage(M_PROJECT_OPEN_SCRIPT, path)));
				submenu->AddSeparatorItem();
				submenu->AddItem(new BMenuItem("Delete...", NewMenuMessage(M_PROJECT_DELETE, path)));
				menu->AddItem(new BMenuItem(submenu, NewMenuMessage(M_PROJECT_SELECT, path)));
			}
		}

		FreeBList(&projects);
	}

	if (fCurProject[0])
	{
		menu->AddSeparatorItem();
		menu->AddItem(new BMenuItem("Exit Project Mode", new BMessage(M_PROJECT_CLOSE)));
	}

	MainWindow->top.menubar->ChangeMenusLock.Unlock();
}

static BMessage *NewMenuMessage(uint what, const char *path)
{
	BMessage *msg = new BMessage(what);
	msg->AddString("path", path);
	return msg;
}

// returns the name and path of a project file such as "silayout" given the
// project base path. the buffer returned is statically allocated and must
// be copied by the caller before the function can be called again.
const char *CProjectManager::GetProjectFile(const char *path, const char *filename)
{
static char pFile[MAXPATHLEN + 32];
int pathLen = strlen(path);
int fileLen = strlen(filename);

	memcpy(pFile, path, pathLen);

	if (pathLen > 0 && path[pathLen - 1] != '/')
		pFile[pathLen++] = '/';

	memcpy(pFile+pathLen, filename, fileLen);

	pFile[pathLen+fileLen] = 0;
	return pFile;
}

/*
void c------------------------------() {}
*/

// create a project by name and return the path to the project in a BString.
BString *CProjectManager::CreateProject(const char *name)
{
char tempbuffer[MAXPATHLEN];

	GetConfigDir(tempbuffer);
	BString *projectPath = new BString(tempbuffer);

	projectPath->Append(name);

	if (mkdir(projectPath->String(), 0xffffffff))
	{
		delete projectPath;
		return NULL;
	}

	UpdateProjectsMenu();
	return projectPath;
}

// open the build script for the given project
void CProjectManager::OpenBuildScript(const char *path)
{
char scriptname[MAXPATHLEN];

	strcpy(scriptname, GetProjectFile(path, P_BUILDSCRIPT));
	DoFileOpen(scriptname);

	if (editor.settings.ShowBuildHelp)
	{
		MainWindow->popup.buildhelp->SetProjectBeingEdited(GetFileSpec(path));
		MainWindow->popup.pane->SetContents("Build Help", MainWindow->popup.buildhelp);

		if (MainWindow->popup.pane->IsOpen())
			MainWindow->popup.buildhelp->PopupOpening();
		else
			MainWindow->popup.pane->Open();
	}
}

/*
void c------------------------------() {}
*/

// switch the editor to a given project by it's path.
// (get the available paths from the "path" field of the menu messages created
//  by UpdateProjectsMenu()).
//
// if allow_none_open is passed and is true, then in case of the layout
// not existing yet, the function will exit with NO files open. this is only
// a good idea during project creation when you KNOW you are about to open more
// files, else the editor could crash.
void CProjectManager::OpenProject(const char *path, bool allow_none_open=false)
{
bool AProjectOpenPreviously = fCurProject[0] ? true : false;

	stat("ActivateProject: %s", path);
	LockWindow();

	// set new project name	and path
	maxcpy(fProjectPath, path, sizeof(fProjectPath) - 1);
	maxcpy(fCurProject, GetFileSpec(path), sizeof(fCurProject) - 1);

	MainWindow->UpdateWindowTitle();

	// try to load last layout
	if (load_layout(GetProjectFile(path, P_LAYOUT)))
	{
		EditView::Close_All();

		if (!allow_none_open)
			TabBar->SetActiveTab(CreateEditView(NULL));
	}

	// set build script name
	const char *buildscript = GetProjectFile(fProjectPath, P_BUILDSCRIPT);
	touch(buildscript);

	MainWindow->popup.compile->SetScriptName(buildscript);

	// add the "Close Project" item if not already there
	if (!AProjectOpenPreviously)
		UpdateProjectsMenu();

	UnlockWindow();
}

// save the currently active project, if any
bool CProjectManager::SaveProject()
{
	if (!fCurProject[0]) return 0;
	stat("saving project '%s'", fCurProject);

	if (save_layout(GetProjectFile(fProjectPath, P_LAYOUT)))
	{
		(new BAlert("", "Failed to save current project state: layout could not be saved.", "OK"))->Go();
		return 1;
	}

	return 0;
}

// close the currently active project, and exit project mode
void CProjectManager::ExitProjectMode()
{
	if (!fCurProject[0]) return;

	SaveProject();

	fCurProject[0] = fProjectPath[0] = 0;
	MainWindow->popup.compile->SetScriptName(NULL);

	MainWindow->UpdateWindowTitle();

	// remove "Close Project" menu item
	UpdateProjectsMenu();
}





