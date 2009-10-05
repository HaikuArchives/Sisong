
#include "editor.h"
#include "messages.h"

#include "cmdseq.fdh"

const struct
{
	const char *str;
	int message;
} cmdlist[] =
{
	"fs", M_FILE_SAVE,
	"fo", M_FILE_OPEN,
	"fa", M_FILE_SAVE_AS,
	"sa", M_FILE_SAVE_ALL,
	"fn", M_FILE_NEW,
	"fx", M_FILE_EXIT,
	"sf", M_SEARCH_FIND,
	"sr", M_SEARCH_REPLACE,
	"sd", M_SEARCH_FIND_FILES,
	"fll", M_FILE_LOAD_LAYOUT1,
	"fls", M_FILE_SAVE_LAYOUT1,
	NULL, 0
};

// checks if "command" is a partial or complete match for any of the
// commands in the list.
//
// if the command could not be any of the commands in the list,
// returns false.
//
// else, returns true, and, if the command is an exact (complete) match,
// sets *message to the associated message of the command, else, message
// is set to 0.
bool IsCommand(char *command, int *message)
{
int i;
bool matched = false;

	for(i=0;cmdlist[i].str;i++)
	{
		if (strbegin(cmdlist[i].str, command))
		{
			matched = true;
			// is partial match, is it an exact match?
			if (!strcmp(cmdlist[i].str, command))
			{
				if (message) *message = cmdlist[i].message;
				return true;
			}
		}
	}
	
	if (message) *message = 0;
	return matched;
}


/*
void c------------------------------() {}
*/

// processes an ALT+... command sequence.
// returns true if the key should be blocked from normal processing.
bool EditView::ProcessCommandSeq(int ch)
{
BStringView *preview = MainWindow->main.editarea->cmd_preview;
int message;
bool command_in_progress;
bool block_key = false;

	if (ch == KEY_MOUSEWHEEL_DOWN || \
		ch == KEY_MOUSEWHEEL_UP) return false;

	if (IsAltDown())
	{
		if (!cmdseq.active)
		{
			cmdseq.nchars = 0;
			cmdseq.active = true;
			strcpy(cmdseq.displaybuffer, "cmd+");
			cmdseq.buffer = cmdseq.displaybuffer + 4;
		}
	}
	
	if (cmdseq.active)
	{
		cmdseq.buffer[cmdseq.nchars++] = ch;
		cmdseq.buffer[cmdseq.nchars] = 0;
		
		command_in_progress = IsCommand(cmdseq.buffer, &message);
		block_key = command_in_progress;
		
		if (message)
		{
			MainWindow->PostMessage(new BMessage(message));
			command_in_progress = false;
		}
		
		if (command_in_progress)
		{
			preview->SetText(cmdseq.displaybuffer);
		}
		else
		{	// command matched some chars, but now no longer matches
			preview->SetText("");
			cmdseq.active = false;
			block_key = true;
		}
	}
	
	return block_key;
}

void EditView::CancelCommandSeq()
{
	if (cmdseq.active)
	{
		cmdseq.active = false;
		MainWindow->main.editarea->cmd_preview->SetText("");
	}
}

bool EditView::IsCommandSeqActive()
{
	return cmdseq.active;
}


