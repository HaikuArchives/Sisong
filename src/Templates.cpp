
#include "editor.h"
#include "Templates.fdh"

#define M_TEMPLATE_SAVE		'Tsav'
#define M_TEMPLATE_LOAD		'Tlod'
#define M_TEMPLATE_SELECT	'Tsel'

BFilePanel *TemplatePanel;


char *GetTemplateDirectory()
{
char *path = (char *)smal(MAXPATHLEN);

	GetConfigDir(path);
	strcat(path, ".templates");
	mkdir(path, 0xffffffff);
	
	return path;
}

void GetTemplateDirectoryRef(entry_ref *ref_out)
{
	char *tdir = GetTemplateDirectory();
	BEntry entry(tdir);
	entry.GetRef(ref_out);
	frees(tdir);
}



class TemplateLooper : public BLooper
{
public:
	TemplateLooper() : BLooper() { }
	
	void DispatchMessage(BMessage *msg, BHandler *target)
	{
		switch(msg->what)
		{
			case M_TEMPLATE_SAVE:
			{
				EditView *ev;
				entry_ref dir; 
				const char *filespec;
				int32 DocID;
				
				if ((msg->FindInt32("DocID", &DocID) == B_OK) &&
				   (msg->FindRef("directory", &dir) == B_OK) &&
				   (msg->FindString("name", &filespec) == B_OK) &&
				   (ev = FindEVByDocID(DocID)))
				{
					BEntry entry(&dir, true);
					BPath path;
					
					entry.GetPath(&path);
					path.Append(filespec);
					const char *filename = path.Path();
					
					if (ev->Save(filename))
					{
						(new BAlert("", "The save operation failed: the file could not be written to the destination.", "Damned!"))->Go();
					}
				}
				else
				{
					(new BAlert("", "The save operation failed.", "Damned!"))->Go();
				}
				
				if (TemplatePanel)
				{
					delete TemplatePanel;
					TemplatePanel = NULL;
				}
				
				Quit();
			}
			break;
			
			case M_TEMPLATE_LOAD:
			{
				int i;
				uint32 type;
				int32 count;
				entry_ref ref;
				EditView *ev = NULL, *firstev = NULL;
				
				LockWindow();
				
				msg->GetInfo("refs", &type, &count);
				for(i=0;i<count;i++)
				{
					if (msg->FindRef("refs", i, &ref) == B_OK)
					{
						BEntry entry(&ref, true);
						BPath path;
						entry.GetPath(&path);
						
						// open the template
						if (ev = CreateEditView((char *)path.Path()))
						{
							// take away the template filename: make the file untitled
							ev->MakeUntitled();
							
							if (!firstev)
								firstev = ev;
						}
					}
				}
				
				// switch to the first template opened
				if (firstev)
					TabBar->SetActiveTab(firstev);
				TabBar->redraw();
				
				UnlockWindow();
				
				if (TemplatePanel)
				{
					delete TemplatePanel;
					TemplatePanel = NULL;
				}
				
				Quit();
			}
			break;
			
			case M_TEMPLATE_SELECT:
			{
				entry_ref ref;
				
				if (msg->FindRef("refs", &ref) == B_OK)
				{
					BEntry entry(&ref, true);
					BPath path;
					entry.GetPath(&path);
					
					BMessenger target;
					BMessage msg_to_deliver;
					
					msg->FindMessage("message", &msg_to_deliver);
					msg->FindMessenger("target", &target);
					
					msg_to_deliver.AddString("template_name", path.Path());
					
					stat("delivering message");
					target.SendMessage(&msg_to_deliver);
				}
			}
			break;
			
			default:
				BLooper::DispatchMessage(msg, target);
			break;
		}
	}
};


// pop up the Save box for a template, and have it save a copy of the current document.
void run_template_saver()
{
TemplateLooper *looper;
BMessage *message;
BMessenger *messenger;
BFilePanel *panel;

	// create message to deliver when done
	message = new BMessage(M_TEMPLATE_SAVE);
	message->AddInt32("DocID", editor.curev->DocID);

	// create looper to deliver it to
	looper = new TemplateLooper();
	looper->Run();
	messenger = new BMessenger(NULL, looper);

	// set starting dir to that of template directory
	entry_ref start_dir;
	GetTemplateDirectoryRef(&start_dir);

	// create panel
	panel = new BFilePanel(B_SAVE_PANEL, messenger, &start_dir, 0, false, message, NULL, true);
	
	// set window title
	BString title;
	title << "Save as Template: " << GetFileSpec(editor.curev->filename);	
	panel->Window()->SetTitle(title.String());
	
	// and away we go
	if (TemplatePanel) delete TemplatePanel;
	TemplatePanel = panel;
	
	panel->Show();
}


void run_template_loader()
{
TemplateLooper *looper;
BMessenger *messenger;
BFilePanel *panel;

	// create looper to deliver it to
	looper = new TemplateLooper();
	looper->Run();
	messenger = new BMessenger(NULL, looper);

	// set starting dir to that of template directory
	entry_ref start_dir;
	GetTemplateDirectoryRef(&start_dir);

	// create panel
	panel = new BFilePanel(B_OPEN_PANEL, messenger, &start_dir, 0, true, new BMessage(M_TEMPLATE_LOAD), NULL, true);
	panel->Window()->SetTitle("New from Template");
	
	// and away we go
	if (TemplatePanel) delete TemplatePanel;
	TemplatePanel = panel;
	
	panel->Show();
}


char *run_template_selector(BMessage msg_to_deliver, BMessenger target)
{
TemplateLooper *looper;
BMessenger *messenger;
BFilePanel *panel;
BMessage *msg;

	// create looper to deliver it to
	looper = new TemplateLooper();
	looper->Run();
	messenger = new BMessenger(NULL, looper);

	msg = new BMessage(M_TEMPLATE_SELECT);
	msg->AddMessage("message", &msg_to_deliver);
	msg->AddMessenger("target", target);
	
	// set starting dir to that of template directory
	entry_ref start_dir;
	GetTemplateDirectoryRef(&start_dir);

	// create panel
	panel = new BFilePanel(B_OPEN_PANEL, messenger, &start_dir, 0, false, msg, NULL, true);
	panel->Window()->SetTitle("Select Template");
	
	// and away we go
	if (TemplatePanel) delete TemplatePanel;
	TemplatePanel = panel;
	
	panel->Show();
}





