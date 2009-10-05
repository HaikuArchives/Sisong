
#include "editor.h"
#include <Path.h>
#include <Entry.h>
#include "AutoSaver.fdh"

static int LastAutoSave = 0;
static int Timer = 0;
static bool UpToDate = true;

/*
	Debugging feature to help protect against possible editor crashes
	while working on editor using the editor.
	
	The scheme goes like this:
	
	If the document has been changed since the last autosave,
	and we have not autosaved for at least 30 seconds,
	and user stops typing for at least 1 second,
	
	come up with a filename made up of the current filename's filespec
	plus a number which makes it a unique filename.
	
	then save the document to /tmp/ under that file name, but don't change
	the name of the current document.
*/

void AutoSaver_ResetTimer()
{
	Timer = 0;
	UpToDate = false;
}

void AutoSaver_Tick()
{
	// if document modified since last autosave...
	if (UpToDate) return;
	
	++Timer;
	
	// haven't auto-saved for at least 60 seconds...
	if (LastAutoSave < 600)
	{
		LastAutoSave++;
	}
	else
	{
		// user has stopped typing for at least 1 second...
		if (Timer >= 10)
		{
			// autosave!
			AutoSaver_Fire();
		}
	}
	
	//stat("%d : %d", LastAutoSave, Timer);
}

void AutoSaver_Fire()
{
char *filespec, *ext;
const char *autosv_filename;
char str_num[50];

	if (!editor.curev) return;	// just in case
	
	// get name of current document, minus the path
	filespec = smal_strdup(GetFileSpec(editor.curev->filename));
	
	// seperate extension from filename
	ext = strrchr(filespec, '.');
	if (ext)
	{
		*ext = 0;
		ext++;
	}
	
	// get path where autosaved files are stored
	char dir[MAXPATHLEN];
	find_directory(B_COMMON_TEMP_DIRECTORY, 0, true, dir, sizeof(dir) - 20);
	AddSuffixIfMissing(dir, '/');
	strcat(dir, "sisong/");
	
	BString basepath(dir);
	BString path;
	mkdir(basepath.String(), S_IRUSR | S_IWUSR);

	// find an used filename by adding numbers to the end of filename
	for(int number=0;;number++)
	{
		sprintf(str_num, "%d", number);
		
		path = basepath;
		path.Append(filespec);
		path.Append("_");
		path.Append(str_num);
		if (ext)
		{
			path.Append(".");
			path.Append(ext);
		}
		
		if (!file_exists(path))
		{
			autosv_filename = path.String();
			break;
		}
	}
	
	stat("autosave: %s", autosv_filename);
	editor.curev->Save(autosv_filename);
	
	frees(filespec);

	LastAutoSave = 0;
	Timer = 0;
	UpToDate = true;
}


