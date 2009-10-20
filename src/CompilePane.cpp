
#include "editor.h"
#include <unistd.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <signal.h>
#include "CompilePane.lh"
#include "CompilePane.fdh"

// priority of 8 is higher than B_LOW_PRIORITY (5), but won't compete with
// apps running at B_NORMAL_PRIORITY (10).
#define COMPILE_THREAD_PRIORITY		8

static rgb_color color_bg = { 0, 0, 0 };
static rgb_color color_text = { 192, 192, 192 };
static rgb_color color_error = { 225, 60, 60 };
static rgb_color color_warning = { 255, 200, 90 };
static rgb_color color_exec = { 190, 255, 190 };
static rgb_color color_scriptname = { 100, 255, 100 };
static rgb_color color_selection = { 30, 30, 30 };
static rgb_color color_header = { 200, 30, 30 };


CompilePane::CompilePane()
{
	// create list
	BRect lvrect(Bounds());
	lvrect.InsetBy(2, 2);
	lvrect.right -= B_V_SCROLL_BAR_WIDTH;
	
	ListView = new BListView(lvrect, "compilelist", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	ListView->SetViewColor(color_bg);
	
	BFont font(be_fixed_font);
	font.SetSize(editor.settings.font_size);
	ListView->SetFont(&font);
	
	ListView->SetSelectionMessage(new BMessage(M_COMPILEPANE_INVOKE));
	
	// create scrollview
	ScrollView = new BScrollView("ScrollView", ListView, \
							B_FOLLOW_ALL, 0, false, true);
	AddChild(ScrollView);
	
	ListSemaphore = create_sem(1, "list_sem");
	CompileThread = -1;
	
	fScriptName[0] = 0;
	
	// get temp file used when running scripts (see RunScriptLine)
	find_directory(B_COMMON_TEMP_DIRECTORY, 0, true, fTempScriptFile, sizeof(fTempScriptFile) - 32);
	AddSuffixIfMissing(fTempScriptFile, '/');
	strcat(fTempScriptFile, "sicompile");
}

CompilePane::~CompilePane()
{
	Clear();
}

/*
void c------------------------------() {}
*/

void CompilePane::PopupClosing()
{
	// ensure compile thread isn't running
	AbortThread();
}

void CompilePane::ChangeFontSize(int new_point_size)
{
	BFont font(be_fixed_font);
	font.SetSize(new_point_size);
	ListView->SetFont(&font);
	ListView->Invalidate();
}

/*
void c------------------------------() {}
*/

void CompilePane::AddLine(const char *text, rgb_color lineColor, bool selectable)
{
BScrollBar *sbar;
float smax, oldsmax, oldval;

	if (acquire_sem(ListSemaphore) != B_NO_ERROR)
		return;
	
	bool locked = LockLooper();
	
	// get scrollbar range and value before the add; this prevents jerking
	// scroll back down if the user is trying to look at the history.
	sbar = ScrollView->ScrollBar(B_VERTICAL);
	sbar->GetRange(NULL, &oldsmax);
	oldval = sbar->Value();
	
	// if there is a spacer item already, replace it with the new line,
	// else add this line as the first one.
	ColoredStringItem *item = (ColoredStringItem *)ListView->LastItem();
	if (item)
	{
		item->SetText(text);
		item->SetColor(lineColor);
		if (selectable) item->SetSelectionColor(color_selection);
		ListView->InvalidateItem(ListView->CountItems() - 1);
	}
	else
	{
		ListView->AddItem(new ColoredStringItem(text, lineColor, color_bg, color_selection));
	}
	
	// add a spacer item so that it looks nicer when scrolling (leave a blank line)
	// would prefer to use AddList to add both items at once, but the implementation
	// in BListView is lazy and causes flicker
	ListView->AddItem(new ColoredStringItem("", color_text, color_bg, color_bg));
	
	// move the scroll bar so that the latest line is always visible
	if (oldval == oldsmax)
	{
		sbar->GetRange(NULL, &smax);
		if (smax) sbar->SetValue(smax);
	}
	
	if (locked)
	{
		Sync();
		UnlockLooper();
	}
	
	fLineCount++;
	release_sem_etc(ListSemaphore, 1, B_DO_NOT_RESCHEDULE);
}

void CompilePane::Clear()
{
ColoredStringItem *item;

	int i = 0;
	while((item = (ColoredStringItem *)ListView->ItemAt(i++)))
		delete item;
	
	ListView->MakeEmpty();
	
	fAutoScrollLine = -1;
	fAutoScrollLineType = -1;
	fAutoJumpLine = -1;
	fAutoJumpLineType = -1;
	
	fHasErrors = false;
	fLineCount = 0;
}

/*
void c------------------------------() {}
*/

void CompilePane::SetScriptName(const char *fname)
{
	if (fname)
		maxcpy(fScriptName, fname, sizeof(fScriptName));
	else
		fScriptName[0] = 0;
}

// starts the compile thread running the given script.
// if run_result is false, lines beginning with '%' are skipped.
void CompilePane::RunScript(bool runWhenDone)
{
	if (CompileThread != -1)
		AbortThread();
	
	Clear();
	MainWindow->popup.pane->Open(runWhenDone ? "Build" : "Build (no run)", this);
	fRunResult = runWhenDone;
	
	if (!fScriptName[0])
	{
		AddLine("unable to build:", color_warning, false);
		AddLine("  No build script is set...please open a project first", color_error, false);
		return;
	}
	
	thread.quit_ack = create_sem(0, "quit_ack");
	thread.please_quit = false;
	
	CompileThread = spawn_thread(ScriptRunnerThread, "Compile Thread", COMPILE_THREAD_PRIORITY, this);
	resume_thread(CompileThread);
}

// if the compile thread is currently running, sees to it that it is stopped
void CompilePane::AbortThread()
{
status_t ok;
int lockcount = 0;

	if (CompileThread == -1)
		return;
	
	// if we are holding a lock on the looper, temporarily release it, so
	// the thread can use AddLine to report the termination.
	if (Window()->LockingThread() == find_thread(NULL))
	{
		// this is safe because we just proved it's us that holds the lock
		lockcount = Window()->CountLocks();
		for(int i=0;i<lockcount;i++)
			UnlockLooper();
	}
	
	// try to exit gracefully
	stat("aborting compile thread");
	thread.please_quit = true;
	ok = acquire_sem_etc(thread.quit_ack, 1, B_RELATIVE_TIMEOUT, 500 * 1000);
	
	// lock looper back the way we found it
	for(int i=0;i<lockcount;i++)
		LockLooper();
	
	if (ok != B_NO_ERROR)
	{	// time to get the mallet
		stat("compile thread still hasn't quit, getting the mallet");
		kill_thread(CompileThread);
	}
	
	delete_sem(thread.quit_ack);
	CompileThread = -1;
	thread.quit_ack = -1;
}

status_t ScriptRunnerThread(void *data)
{
CompilePane *pane = (CompilePane *)data;
char fname[MAXPATHLEN];

	maxcpy(fname, pane->fScriptName, sizeof(fname) - 1);
	pane->RunScriptInternal(fname);
	return B_OK;
}

/*
void c------------------------------() {}
*/

// this is called from the compile thread, don't call it directly
int CompilePane::RunScriptInternal(char *scriptname)
{
FILE *fp;
char line[MAXPATHLEN + 10];
char olddir[MAXPATHLEN];
int exitcode = 0;
bool scriptEmpty = true;

	BString fn(scriptname);
	fn.Prepend("-> ");
	AddLine(fn.String(), color_scriptname, false);
	
	fp = fopen(scriptname, "rb");
	if (!fp)
	{
		BString str = "unable to open script ";
		str.Append(scriptname);
		
		AddLine(str.String(), color_error, false);
		
		delete_sem(thread.quit_ack);
		return -1;
	}
	
	// save current working directory in case script changes it
	getcwd(olddir, sizeof(olddir));
	
	while(!feof(fp))
	{
		fgetline(fp, line, sizeof(line) - 1);
		
		if (!*line) continue;
		if (line[0] == '#') continue;
		if (line[0] == '/' && line[1] == '/') continue;
		
		scriptEmpty = false;
		
		if (line[0] == '!')	// "ignore exit value"
		{
			RunScriptLine(&line[1]);
		}
		else if (line[0] == '%')	// "skip line in 'Build but Don't Run' mode
		{
			if (fRunResult)
			{
				exitcode = RunScriptLine(&line[1]);
				if (exitcode != 0) break;
			}
		}
		else
		{
			exitcode = RunScriptLine(line);
			if (exitcode != 0) break;
		}
	}
	
	chdir(olddir);
	fclose(fp);
	
	if (scriptEmpty)
	{
		AddLine("  build script is empty!", color_warning, false);
		AddLine("  you can configure it from the Projects menu.", color_warning, false);
	}
	
	//stat("C thread going bye bye, please_quit=%d", thread.please_quit);
	if (thread.please_quit)
	{
		release_sem(thread.quit_ack);
	}
	else
	{
		delete_sem(thread.quit_ack);
		
		CompileThread = -1;
		thread.quit_ack = -1;
		
		if (fHasErrors)
		{
			if (editor.settings.build.JumpToErrors && \
				MainWindow->popup.pane->IsOpen())
			{
				// scroll pane up so first error is visible
				int y = (fAutoScrollLine - 1);
				if (y < 0) y = 0;
				
				if (LockLooper())
				{
					BListItem *item = ListView->ItemAt(0);
					y *= (int)item->Height();
					
					ScrollView->ScrollBar(B_VERTICAL)->SetValue(y);
					UnlockLooper();
				}
				
				// now simulate a click on the first clickable line--take them right to it
				if (fAutoJumpLine != -1)
					ListView->Select(fAutoJumpLine);
			}
		}
		else if (exitcode == 0 && !scriptEmpty)
		{
			if (!MainWindow->top.menubar->ShowConsoleItem->IsMarked())
				MainWindow->PostMessage(M_POPUPPANE_CLOSE);
		}
	}
	
	return exitcode;
}

// interpret and execute a single line from a script, and return the exit/status code.
// nonzero return status causes the script to abort.
int CompilePane::RunScriptLine(const char *line)
{
	AddLine(line, color_exec, false);
	
	if (strbegin(line, "cd "))
	{
		if (chdir(line + 3))
		{
			BString error = "cd failed to directory ";
			error.Append(line + 3);
			AddLine(error.String(), color_error, false);
			return 1;
		}
		
		return 0;
	}
	else if (!strcmp(line, "hide"))
	{
		if (!fHasErrors)
			if (!MainWindow->top.menubar->ShowConsoleItem->IsMarked())
				MainWindow->PostMessage(new BMessage(M_POPUPPANE_CLOSE));
		
		return 1;
	}
	else if (!strcmp(line, "show"))
	{
		return 1;
	}
	
	// else, no commands known, try to exec
	
	// to support things such as "rm *.o", we can create a 1-liner script,
	// then execute it with "bash".
	//
	// this is real messy
	FILE *fp = fopen(fTempScriptFile, "wt");
	fprintf(fp, "%s\n", line);
	fprintf(fp, "exit $?\n");
	fclose(fp);
	
	char *array[5];
	char *program;
	
	array[0] = "bash";
	array[1] = fTempScriptFile;
	array[2] = NULL;
	
	char *ptr = strchr(line, ' ');
	if (!ptr)
	{
		program = smal_strdup(line);
	}
	else
	{
		int len = (ptr - line);
		program = (char *)smal(len + 1);
		memcpy(program, line, len);
		program[len] = 0;
	}
	
	int exitcode = this->Exec(program, array);
	
	frees(program);
	remove(fTempScriptFile);
	
	return exitcode;
}

/*
void c------------------------------() {}
*/


// executes a program and pipes it's output to the compile pane.
// args is an array of command-line arguments and should be null terminated.
int CompilePane::Exec(const char *program, char *args[])
{
rfContext rStdout;
rfContext rStderr;
int pfd[2];
int epfd[2];
int status;
pid_t child;
int exitstat;

#define EXEC_FAILED_CODE	179	// just a magick

	// create pipes for stdout and stderr
	pipe(pfd);
	pipe(epfd);
	
	// increase priority for a sec--we don't want child to inherit our low priority
	set_thread_priority(find_thread(0), B_NORMAL_PRIORITY);
	
	child = vfork();
	if (child == 0)
	{	// this is the child
		// redirect stdout/stderr
		close(STDOUT_FILENO);
		dup(pfd[1]);
		close(pfd[0]);
		
		close(STDERR_FILENO);
		dup(epfd[1]);
		close(epfd[0]);
		
		// this makes spawned child a session group leader so if we change
		// our mind, we can kill it and all of it's children at once.
		setpgid(0, 0);
		
		// execute the program
		execv("/bin/bash", args);
		
		// guess exec must have failed, cause we're still here
		if (!file_exists("/bin/bash"))
			fprintf(stderr, "** cannot find /bin/bash");
		
		// this message is received by the code below, as if we were the child app.
		fprintf(stderr, "** failed exec of '%s'\n", program);
		exit(EXEC_FAILED_CODE);
	}
	
	set_thread_priority(find_thread(0), COMPILE_THREAD_PRIORITY);
	close(pfd[1]);
	close(epfd[1]);
	
	InitRFContext(&rStdout, pfd[0]);
	InitRFContext(&rStderr, epfd[0]);
	
	int counter = 0;
	while(!thread.please_quit)
	{
		status = RunRFContext(this, &rStdout, false);
		status += RunRFContext(this, &rStderr, true);
		if (status == 2) break;	// both pipes are empty & closed
		
		if (++counter >= 1500)
		{
			snooze(10 * 1000);
			counter = 0;
		}
	}
	
	CloseRFContext(&rStdout);
	CloseRFContext(&rStderr);
	
	//stat("outta here, thread.please_quit = %d", thread.please_quit);
	
	if (thread.please_quit)
	{	// compile thread is aborting, slaughter our child process
		char str[80];
		sprintf(str, "compile thread aborting: killing child PID %d", (int)child);
		AddLine(str, color_warning, false);
		
		kill(-child, SIGKILL);
		exitstat = -1;
	}
	else
	{
		// obtain exit code
		waitpid(child, &exitstat, 0);
		exitstat = WEXITSTATUS(exitstat);
		
		if (exitstat != EXEC_FAILED_CODE)
		{
			char str[MAXPATHLEN + 80];
			sprintf(str, "<< %s: exit code %d", program, exitstat);
			AddLine(str, color_exec, false);
		}
	}
	
	return exitstat;
}


static void InitRFContext(rfContext *rf, int fd)
{
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
	rf->fd = fd;
	
	rf->max_line_len = CP_MAX_LINE_LEN;
	rf->line = (char *)smal(rf->max_line_len + 1);
	rf->line_len = 0;
	rf->line[0] = 0;
}

static void CloseRFContext(rfContext *rf)
{
	close(rf->fd);
	frees(rf->line);
}

static int RunRFContext(CompilePane *pane, rfContext *rf, bool from_stderr)
{
char ch;

	int status = read(rf->fd, &ch, 1);
	
	if (status < 0) return 0;	// no data avail
	if (status == 0) return 1;	// EOF
	
	if (ch == '\n')
	{
		rf->line[rf->line_len] = 0;
		rf->line_len = 0;
		
		_cp_gotline(rf->line, from_stderr, pane);
	}
	else if (ch == TAB)
	{
		for(int i=0;i<4;i++)
		{
			if (rf->line_len >= rf->max_line_len)
				break;
			
			rf->line[rf->line_len++] = ' ';
		}
	}
	else if (ch != '\r')
	{
		if (rf->line_len < rf->max_line_len)
			rf->line[rf->line_len++] = ch;
	}
	
	return 0;
}

/*
void c------------------------------() {}
*/


void _cp_gotline(const char *line, bool from_stderr, void *userparam)
{
CompilePane *pane = (CompilePane *)userparam;

	if (from_stderr)
	{
		stLineInfo info;
		ParseLineInfo(line, &info);
		
		// handle auto-jumping in case of errors
		if (info.errorType != ET_NONE)
		{
			pane->fHasErrors = true;
			
			bool error_precedence = (editor.settings.build.NoJumpToWarning && \
									 pane->fAutoScrollLineType != ET_ERROR && \
									 info.errorType == ET_ERROR);
			
			// will scroll the pane up to the first line which is not "normal".
			// errors take precedence over warnings in NoJumpToWarning mode.
			if (pane->fAutoScrollLine == -1 || error_precedence)
			{
				pane->fAutoScrollLine = pane->fLineCount;
				pane->fAutoScrollLineType = info.errorType;
			}
			
			// will jump to the first non-normal line for which a line number is available.
			// again, errors take precedence over warnings in NoJumpToWarning mode.
			if (info.lineNo != -1)
			{
				bool error_precedence = (editor.settings.build.NoJumpToWarning && \
										 pane->fAutoJumpLineType != ET_ERROR && \
										 info.errorType == ET_ERROR);
				
				if (pane->fAutoJumpLine == -1 || error_precedence)
					pane->fAutoJumpLine = pane->fLineCount;
				
				if (pane->fAutoJumpLine == pane->fLineCount - 1)
				{
					if (strstr(line, "at this point in file") || \
						strstr(line, "within this context"))
					{
						pane->fAutoJumpLine = pane->fLineCount;
						pane->fAutoJumpLineType = info.errorType;
					}
				}
			}
		}
		
		switch(info.errorType)
		{
			case ET_INFO:
			break;
			
			case ET_ERROR:
				pane->AddLine(line, color_error, (info.lineNo != -1));
			break;
			
			case ET_WARNING:
				pane->AddLine(line, color_warning, (info.lineNo != -1));
			break;
			
			case ET_HEADER:
				pane->AddLine(line, color_header, false);
			break;
			
			default:
				pane->AddLine(line, color_error, false);
			break;
		}
	}
	else
	{
		pane->AddLine(line, color_text, false);
	}
}


// parses a line, and attempts to detect if it is a compiler error message,
// and if so, extracts information on the error into "info".
static void ParseLineInfo(const char *line, stLineInfo *info)
{
char *ptr;
int length;

	//stat("-- line: '%s'", line);
	
	memset(info, 0, sizeof(stLineInfo));
	info->errorType = ET_NONE;
	info->lineNo = -1;
	
	// check if it is a compiler error message
	if (line[0] != '/') return;
	ptr = strchr(line, ':');
	if (!ptr) return;
	
	// get filename
	length = (ptr - line);
	if (length >= CP_MAX_LINE_LEN-1) length = CP_MAX_LINE_LEN-2;
	memcpy(info->filename, line, length);
	info->filename[length] = 0;
	
	// try to get line no
	ptr++;
	if (*ptr >= '1' && *ptr <= '9')
	{
		info->lineNo = atoi(ptr) - 1;
		info->errorType = ET_ERROR;
	}
	else
	{
		info->errorType = ET_HEADER;
	}
	
	// check for special stuff
	if (strstr(ptr, "warning: "))
		info->errorType = ET_WARNING;
	else
	{
		if (strstr(ptr, "(Each undeclared identifier is reported only once") || \
			strstr(ptr, "for each function it appears in.)"))
		{
			info->errorType = ET_INFO;
		}
	}
}

void CompilePane::ItemClicked(int index)
{
ColoredStringItem *item = (ColoredStringItem *)ListView->ItemAt(index);
if (!item) return;
const char *line = item->Text();
if (!line) return;

	stLineInfo info;
	ParseLineInfo(line, &info);
	
	if (info.lineNo != -1 && file_exists(info.filename))
	{
		DoFileOpenAtLine(info.filename, info.lineNo, -1, -1);
	}
	else
	{
		ListView->DeselectAll();
	}
}


