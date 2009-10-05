
#include "editor.h"
#include "NewProject.h"
#include <FindDirectory.h>

#include "FileView/FileView.h"
#include "DirMenu/DirMenu.h"

#include "NewProject.fdh"

#define M_NEW_FOLDER				'mNFO'
#define M_NEW_FILE					'mNFL'
#define M_FILE_FROM_TEMPLATE		'mFFT'
#define M_FILE_FROM_TEMPLATE_RESULT	'mFF2'
#define M_OK						'OK!!'
#define M_CANCEL					'CNCL'

/*void NPTest()
{
	stat("cheese!");
	new NewProjectWindow;
}*/


NewProjectWindow::NewProjectWindow()
	: SubWindow(BRect(0, 0, 370, 415), "New Project")
{
	SetFeel(B_MODAL_APP_WINDOW_FEEL);
	
	// get initial dir of file pane from config file
	char desktop_path[MAXPATHLEN];
	find_directory(B_DESKTOP_DIRECTORY, 0, true, desktop_path, sizeof(desktop_path));
	BPath path(settings->GetString("NPProjectDir", desktop_path));
	
	// create file view
	#define FILEVIEW_HEIGHT		235
	#define FILEVIEW_LEFT		16
	#define FILEVIEW_RIGHT		(Bounds().right-FILEVIEW_LEFT)
	fileview = new FileView(BRect(FILEVIEW_LEFT, 38, FILEVIEW_RIGHT, FILEVIEW_HEIGHT), path.Path(), B_FOLLOW_LEFT_RIGHT);	
	bgview->AddChild(fileview);
	
	// create dirmenu
	BRect rc;
	rc.top = 8;
	rc.left = 15;
	rc.bottom = rc.top+22;
	rc.right = rc.left+300;
	
	dirmenu = new DirMenu(rc, &path, B_FOLLOW_LEFT | B_FOLLOW_TOP);
	bgview->AddChild(dirmenu);
	
	// create buttons beneath dirmenu
	#define BUTTON_HEIGHT		27
	const int button_y = (FILEVIEW_HEIGHT + 8);
	int button_width = 96;
	int x = FILEVIEW_LEFT;
	bgview->AddChild(new BButton(BRect(x, button_y, x+button_width, button_y+BUTTON_HEIGHT), "", \
								"New Folder", new BMessage(M_NEW_FOLDER)));
	x += (button_width + 10);
	button_width = 80;
	bgview->AddChild(new BButton(BRect(x, button_y, x+button_width, button_y+BUTTON_HEIGHT), "", \
								"New File", new BMessage(M_NEW_FILE)));
	x += (button_width + 10);
	bgview->AddChild(new BButton(BRect(x, button_y, FILEVIEW_RIGHT, button_y+BUTTON_HEIGHT), "", \
								"File from Template", new BMessage(M_FILE_FROM_TEMPLATE)));
	
	// project name
	#define TEXT_Y		(button_y+BUTTON_HEIGHT+18)
	txtProjectName = new BTextControl(BRect(FILEVIEW_LEFT, TEXT_Y, FILEVIEW_RIGHT, TEXT_Y+20),
							"", "Project Name:", "", NULL);
	
	txtProjectName->SetDivider(106);
	bgview->AddChild(txtProjectName);
	
	rc.Set(FILEVIEW_LEFT, TEXT_Y+40, FILEVIEW_RIGHT, TEXT_Y+40+15);
	bgview->AddChild(new BStringView(rc, "",
								"Create any new folders or source files above, then"));
	rc.OffsetBy(0, 16);
	bgview->AddChild(new BStringView(rc, "",
								"enter the name you want to appear in Projects menu."));
	
	
	// OK and Cancel
	rc = Bounds();
	rc.InsetBy(10, 10);
	rc.left = rc.right - 96;
	rc.top = rc.bottom - BUTTON_HEIGHT;
	
	BButton *OKButton = new BButton(rc, "", "OK", new BMessage(M_OK));
	OKButton->MakeDefault(true);
	bgview->AddChild(OKButton);
	
	rc.OffsetBy(-(rc.Width() + 12), 0);
	bgview->AddChild(new BButton(rc, "", "Cancel", new BMessage(M_CANCEL)));
	
	txtProjectName->MakeFocus();
	Show();
}

NewProjectWindow::~NewProjectWindow()
{
	FreeBList(&files_to_open);
}

/*
void c------------------------------() {}
*/

void NewProjectWindow::HandleOKButton()
{
const char *projectName;
BString *projectPath;

	txtProjectName->MakeFocus();

	projectName = txtProjectName->Text();
	if (!projectName || !projectName[0])
	{
		(new BAlert("", "Please specify a name to appear in the Projects menu.", "OK"))->Go();
		return;
	}
	
	if (strlen(projectName) >= 32 || \
		strchr(projectName, '/') || \
		strchr(projectName, '|'))
	{
		(new BAlert("", "The project name is too long or contains some invalid characters.\n\n"
						"Projects are stored as folders so must be valid folder names.\n",
						"OK", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
		return;
	}

	// save current project if any
	if (ProjectManager.SaveProject()) return;
	
	// create the project settings directory
	projectPath = ProjectManager.CreateProject(projectName);
	if (!projectPath)
	{
		(new BAlert("", "The project could not be created. Most likely the selected name is already"
						" in use.\n", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
		return;
	}
	
	// open the project. (this also creates an empty build script).
	ProjectManager.OpenProject(projectPath->String(), true);
	editor_close_all();
	
	// open all files that were created from New Project window, as we assume
	// they're part of the project.
	int i, count=files_to_open.CountItems();
	for(i=0;i<count;i++)
	{
		CreateEditView((char *)files_to_open.ItemAt(i));
	}
	
	// lastly, open build script
	ProjectManager.OpenBuildScript(projectPath->String());
	
	
	Quit();
}

/*
void c------------------------------() {}
*/

void NewProjectWindow::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{		
		case M_PATH_CHANGED:
		{
			const char *path;
			if (msg->FindString("path", &path) == B_OK)
			{
				fileview->SetPath(path);
				dirmenu->SetPath(path);
			}
		}
		break;
		
		case M_NEW_FOLDER:
		{
			BString *folderName;
			
			if (folderName = InputBox::Go(MainWindow, "New Folder", "Name of new folder:", ""))
			{
				BPath path;
				
				fileview->GetPath(&path);
				path.Append(folderName->String());
				
				mkdir(path.Path(), 0xffffffff);
				fileview->Update();
				
				fileview->SetPath(path.Path());
				dirmenu->SetPath(path.Path());
				
				delete folderName;
			}
		}
		break;
		
		case M_NEW_FILE:
		{
			BString *fileName;
			
			if (fileName = InputBox::Go(MainWindow, "New File", "Name of new file:", ""))
			{
				BPath bpath;				
				char fname[MAXPATHLEN];
				
				fileview->GetPath(&bpath);
				strcpy(fname, bpath.Path());
				
				int len = strlen(fname);
				if (len && fname[len-1] != '/')
					strcat(fname, "/");
				
				strcat(fname, fileName->String());
				
				touch(fname);
				fileview->Update();
				fileview->SelectItem(fname);
				
				files_to_open.AddItem(smal_strdup(fname));
				
				delete fileName;
			}
		}
		break;
		
		case M_FILE_FROM_TEMPLATE:
			run_template_selector(BMessage(M_FILE_FROM_TEMPLATE_RESULT), \
								BMessenger(NULL, this));
		break;
		
		case M_FILE_FROM_TEMPLATE_RESULT:
		{
			const char *template_name;
			if (msg->FindString("template_name", &template_name) == B_OK)
			{
				BString *fname;
				fname = InputBox::Go(MainWindow, "New from Template", \
									"Name of new file:", GetFileSpec(template_name));
				
				if (fname)
				{
					BPath bpath;
					char dest_name[MAXPATHLEN];
					
					fileview->GetPath(&bpath);
					strcpy(dest_name, bpath.Path());
					
					int len = strlen(dest_name);
					if (len && dest_name[len-1] != '/')
						strcat(dest_name, "/");
					
					strcat(dest_name, fname->String());
					
					stat("%s -> %s", template_name, dest_name);
					CopyFile(template_name, dest_name);
					fileview->Update();
					fileview->SelectItem(dest_name);
					
					files_to_open.AddItem(smal_strdup(dest_name));
					
					delete fname;
				}
			}
		}
		break;
		
		case M_CANCEL:
		{
			Quit();
		}
		break;
		
		case M_OK:
		{
			HandleOKButton();
		}
		break;
		
		default:
			BWindow::MessageReceived(msg);
		break;
	}	
}



