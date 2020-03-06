
#include "editor.h"
#include "misc.fdh"
#include <Beep.h>

// centers window "child" within window "parent"
void CenterWindow(BWindow *parent, BWindow *child, bool at_bottom)
{
	BRect mrect(parent->Frame());

	int x = (int)(mrect.left + (WIDTHOF(mrect) / 2));
	int y = (int)(mrect.top + (HEIGHTOF(mrect) / 2));

	if (at_bottom)
		y = (int)(mrect.bottom - HEIGHTOF(child->Frame()) - 16);
	else
		y -= (int)HEIGHTOF(child->Frame())/2;

	x -= (int)WIDTHOF(child->Frame())/2;
	child->MoveTo(x, y);
}

/*
void c------------------------------() {}
*/

// simple function to copy small files.
// returns nonzero on error.
bool CopyFile(const char *src, const char *dst)
{
FILE *fpi, *fpo;
int file_size;
char *buffer;

	fpi = fopen(src, "rb");
	if (!fpi) return 1;
	fpo = fopen(dst, "wb");
	if (!fpo) { fclose(fpi); return 1; }

	fseek(fpi, 0, SEEK_END);
	file_size = ftell(fpi);
	fseek(fpi, 0, SEEK_SET);

	buffer = (char *)malloc(file_size);

	fread(buffer, file_size, 1, fpi);
	fwrite(buffer, file_size, 1, fpo);

	free(buffer);

	fclose(fpi);
	fclose(fpo);
	return 0;
}


uint timer()
{
bigtime_t us = system_time();

	return (us / 1000);
}

int min(int a, int b)
{
	return (a < b) ? a : b;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

void DeleteBefore(BString *string, int x)
{
	if (x > 0) string->Remove(0, x);
}

void DeleteAfter(BString *string, int x)
{
	x++;
	int len = string->Length();
	if (x < len)
	{
		string->Remove(x, len-x);
	}
}

// if the last character of string "str" is not "ch", appends "ch" to the end of "str".
// used primarily for path preparation with ch='/'
void AddSuffixIfMissing(char *str, char ch)
{
	int len = strlen(str);

	if (len)
	{
		if (str[len - 1] != ch)
		{
			char append[2];
			append[0] = ch;
			append[1] = 0;

			strcat(str, append);
		}
	}
}

// trims trailing spaces and tabs from the end of a string
int RTrimWhitespace(char *string, int linelength)
{
	char *ptr = (string + (linelength - 1));

	while(ptr >= string)
	{
		if (*ptr == ' ' || *ptr == TAB)
		{
			*ptr = 0;
			ptr--;
		}
		else
		{
			break;
		}
	}

	return (ptr - string) + 1;
}


/*
void c------------------------------() {}
*/

/*
	Here's the deal with all the swapping; it's a little confusing.

	Haiku Keymap can swap CTRL and ALT to emulate Windows/Linux.
	I have mine set to do this, so my idea of an IsCtrlDown() is actually
	to ask the OS whether ALT is down, and vice versa.

	I would like to add a feature to locally swap them as well so
	people can use CTRL+HOME shortcuts as CTRL+HOME if they want,
	instead of ALT+HOME. But this isn't working well yet do the
	TranslateAltKey() mess.
*/

bool IsShiftDown()
{
	return (modifiers() & B_SHIFT_KEY) ? true:false;
}

bool IsCtrlDown()
{
	//int key = (editor.settings.swap_ctrl_and_alt) ? B_CONTROL_KEY : B_COMMAND_KEY;
	return (modifiers() & B_COMMAND_KEY) ? true:false;
}

bool IsAltDown()
{
	//int key = (editor.settings.swap_ctrl_and_alt) ? B_COMMAND_KEY : B_CONTROL_KEY;
	return (modifiers() & B_CONTROL_KEY) ? true:false;
}

// the first key in a cmd-seq is garbled, because Haiku sees it as
// a special escape sequence (such as CTRL+C). this translates all
// the sequences back into just the letter.
//
// This isn't perfect because some of the sequences overlap with
// real keys such as ENTER and Home, so TODO: key off the scancode or
// something instead, so we don't need this anymore.
int TranslateAltKey(int ch)
{
	switch(ch)
	{
		case 17: return 'q';
		case 23: return 'w';
		case 5:  return 'e';
		case 18: return 'r';
		case 20: return 't';
		case 25: return 'y';
		case 21: return 'u';
		case 9:  return 'i';
		case 15: return 'o';
		case 16: return 'p';

		case 1:  return 'a';
		case 19: return 's';
		case 4: return 'd';
		case 6: return 'f';
		case 7:	return 'g';
		case 8: return 'h';
		case 10: return 'j';
		case 11: return 'k';
		case 12: return 'l';

		case 26: return 'z';
		case 24: return 'x';
		case 3:  return 'c';
		case 22: return 'v';
		case 2:  return 'b';
		case 14: return 'n';
		case 13: return 'm';
	}

	return ch;
}

/*
void c------------------------------() {}
*/

// fills "dirs" and "files" with char * strings containing the names of all directories
// and all files matching a given filter within the given folder.
// returns nonzero if an error occurs.
bool GetDirectoryContents(const char *folder, const char *filter, BList *files, BList *dirs)
{
	if (files) files->MakeEmpty();
	if (dirs) dirs->MakeEmpty();

	BEntry entry(folder);
	if (!entry.Exists()) return true;
	BDirectory dir(&entry);
	BDirectory dirchecker;

	dir.Rewind();
	while(dir.GetNextEntry(&entry, false) != B_ENTRY_NOT_FOUND)
	{
		BPath path;
		entry.GetPath(&path);

		// check if this is a file or a directory. to do that, i just pretend it's
		// a directory and see if that assumption causes an error.
		dirchecker.SetTo(&entry);
		if (dirchecker.Rewind() != B_OK)
		{	// is file
			if (files && filter)
			{
				const char *filespec = path.Path();
				filespec = strrchr(filespec, '/');
				if (filespec)
				{
					filespec++;
					if (does_filter_match(filespec, filter))
					{
						files->AddItem((void *)smal_strdup(path.Path()));
					}
				}
			}
		}
		else
		{	// is directory
			if (dirs)
				dirs->AddItem((void *)smal_strdup(path.Path()));
		}
	}

	return false;
}

// checks if "filespec" (a file name) matches "filter" (in form of "*.*" etc)
bool does_filter_match(const char *filespec, const char *filter)
{
char filter_char;
char *match_what;
char *ptr;

	//stat("does_filter_match: '%s', '%s'", filespec, filter);

	rept
	{
		filter_char = *(filter++);

		switch(filter_char)
		{
			case 0:
				return true;

			case '?':	// next char is wildcard, and is for free
				filespec++;
			break;

			case '*':	// skip chars in filespec
			{
				// seek past '*'
				do
				{ filter_char = *(filter++); }
				while(filter_char == '*' || filter_char == '?');

				// the '*' is last char in filter (asdf.*)
				if (!filter_char) return true;

				// find the pattern which must match
				match_what = smal_strdup(--filter);
				ptr = strchr(match_what, '*'); if (ptr) *ptr = 0;
				ptr = strchr(match_what, '?'); if (ptr) *ptr = 0;

				filespec = strstr(filespec, match_what);
				if (!filespec) { frees(match_what); return false; }

				int len = strlen(match_what);
				filespec += len;
				filter += len;
				frees(match_what);
			}
			break;

			default:	// next char has to match our char
			{
				if (*filespec == filter_char)
				{
					filespec++;
				}
				else
				{
					return false;
				}
			}
			break;
		}
	}
}

/*
void c------------------------------() {}
*/

void OpenFolderInTracker(const char *path)
{
	entry_ref ref;
	BEntry(path).GetRef(&ref);

	BMessage openmsg(B_REFS_RECEIVED);
	openmsg.AddRef("refs", &ref);

	BMessenger msgr("application/x-vnd.Be-TRAK");
	msgr.SendMessage(&openmsg);
}

// moves a file or directory into the Tracker's trash
bool MoveToTrash(const char *orgpath)
{
char TrashDirectory[MAXPATHLEN];
BString DestName;

	// get destination filename for the move
	if (find_directory(B_TRASH_DIRECTORY, 0, true, TrashDirectory, sizeof(TrashDirectory)) != B_OK)
		return 1;
	DestName = TrashDirectory;

	int len = strlen(DestName);
	if (len && DestName[len-1] != '/')
		DestName.Append("/");

	DestName.Append(GetFileSpec(orgpath));

	// deal with case where file already exists in Trash
	while(file_exists(DestName.String()))
		DestName.Append(" copy");

	// do the move
	if (rename(orgpath, DestName.String()))
	{
		staterr("could not move to trash: 'rename' failed");
		return 1;
	}

	// save original path for "restore" operation
	BNode node(DestName.String());
	if (node.InitCheck() == B_OK)
	{
		BString OrgPathString(orgpath);
		node.WriteAttrString("_trk/original_path", &OrgPathString);
	}

	return 0;
}

/*
void c------------------------------() {}
*/

void errorblip()
{
	staterr("errorblip!");
	beep();
}

void Unimplemented()
{
	BAlert *alert = new BAlert("", "     The message is heard\n"
								   " but the features are missing\n"
								   "      wait for next version", "Oh, ok", NULL, NULL,
									B_WIDTH_AS_USUAL, B_WARNING_ALERT);

	alert->SetShortcut(0, B_ESCAPE);
	BFont fi;
	alert->TextView()->GetFontAndColor(0, &fi);
	fi.SetFace(B_ITALIC_FACE);
	alert->TextView()->SetFontAndColor(0, 0, &fi);
	alert->Go();
}


// return a bmessage "what" value as a string
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


bool TrySetFontFamily(BFont *font, const char *desired_face, \
								   const char *desired_style)
{
int32 numFamilies = count_font_families();
font_family ff;
font_style fs;
uint32 flags;
int i, j;

	for(i=0;i<numFamilies;i++)
	{
		if (get_font_family(i, &ff, &flags) == B_OK)
		{
			if (!strcasecmp(ff, desired_face))
			{
				int numStyles = count_font_styles(ff);

				for(j=0;j<numStyles;j++)
				{
					if (get_font_style(ff, j, &fs, &flags) == B_OK)
					{
						stat("%s:%s", ff, fs);

						if (!strcasecmp(fs, desired_style))
						{
							font->SetFamilyAndStyle(ff, fs);
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}



