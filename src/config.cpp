
#include <stdio.h>
#include <stdlib.h>

#include <OS.h>
#include <Message.h>
#include <String.h>
#include <FindDirectory.h>
#include <File.h>

#include "../common/basics.h"
#include "config.h"
#include "config.fdh"

Config *settings = NULL;


Config *Config::load()
{
FILE *fpi;
char *filename;
char line[1024];
char *equ, *ptr;

	settings = (Config *)(new BMessage(M_SETTINGS));
	
	filename = GetConfigFileName();
	if (!filename) return settings;
	
	fpi = fopen(filename, "rb");
	if (!fpi) return settings;
	
	while(!feof(fpi))
	{
		fgetline(fpi, line, sizeof(line) - 1);
		
		equ = strchr(line, '=');
		if (!equ) continue;
		
		*equ = 0;
		ptr = equ-1;
		while(ptr > line && (*ptr == ' ')) { *ptr = 0; ptr--; }
		equ++;
		while(*equ == ' ') equ++;
		
		switch(line[0])
		{
			case '%':
				settings->AddInt32(line+1, atoi(equ));
			break;
			
			case '$':
				settings->AddString(line+1, equ);
			break;
		}
	}


	fclose(fpi);
	return settings;
}

void Config::save(Config *settings)
{
FILE *fpo;
char *name;
uint32 type;
int32 i, count;
char *filename;
BString line("", 128);

	filename = GetConfigFileName();
	if (!filename) return;
	
	fpo = fopen(filename, "wb");
	if (!fpo) return;
	
	for(i=0; settings->GetInfo(B_ANY_TYPE, i, &name, &type, &count) == B_OK; i++)
	{
		switch(type)
		{
			case B_STRING_TYPE:
			{
				const char *strData = "";
				settings->FindString(name, &strData);
				
				line = "$";
				line.Append(name);
				line.Append(" = ");
				line.Append(strData);
			}
			break;
			
			case B_INT32_TYPE:
			{
				int32 num = 0;
				char strData[64];
				
				settings->FindInt32(name, &num);
				sprintf(strData, "%d", num);
				
				line = "%";
				line.Append(name);
				line.Append(" = ");
				line.Append(strData);
			}
			break;
			
			default: continue;
		}
		
		fprintf(fpo, "%s\n", line.String());
	}
	
	fclose(fpo);
}

/*
void c------------------------------() {}
*/

void Config::SetString(const char *name, const char *value)
{
	if (value == NULL) value = "";
	
	if (HasString(name))
		ReplaceString(name, value);
	else
		AddString(name, value);
}

const char *Config::GetString(const char *name, const char *Default)
{
const char *str;

	if (FindString(name, &str) == B_OK)
		return str;
	else
		return Default;
}


void Config::SetInt(const char *name, int value)
{
	if (HasInt32(name))
		ReplaceInt32(name, value);
	else
		AddInt32(name, value);
}

int Config::GetInt(const char *name, const int Default)
{
int32 value;

	if (FindInt32(name, &value) == B_OK)
		return value;
	else
		return Default;
}

/*
void c-----------------------------() {}
*/

static char *GetConfigFileName()
{
static char cfg_path[MAXPATHLEN];
char *dir;

	dir = GetConfigDir(cfg_path);
	if (dir)
	{
		strcat(dir, "settings");
		return dir;
	}
	else
	{
		return NULL;
	}
}

// copies the path to the settings directory into "buffer", which must be
// of at least size MAXPATHLEN.
// returns buffer on success, NULL on failure.
char *GetConfigDir(char *buffer)
{
	// -32: reserve rooms for strcat()'s. meow!
	if (find_directory(B_USER_SETTINGS_DIRECTORY, 0, true, buffer, MAXPATHLEN-32)
		 == B_OK)
	{
		int len = strlen(buffer);
		if (len && buffer[len-1] != '/') strcat(buffer, "/");
		
		strcat(buffer, "Sisong/");
		
		mkdir(buffer, S_IRUSR | S_IWUSR);
		return buffer;
	}
	
	// failure
	buffer[0] = 0;
	return NULL;
}



