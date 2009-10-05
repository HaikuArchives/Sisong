
#include "editor.h"
#include "BuildHelpPane.fdh"
/*
	When you select Execute Build Script from the Run menu, press COMMAND+R,
	or use the function key shortcuts F5 or F7, each line in this file
	will be executed sequentially.
	
	You can use commands such as "cd", "cp", "make", "jam", etc.	

	The build will abort with a failure condition if any executed program
	returns with an exit code other than 0.
	
	To bypass this behavior and ignore the exit code of a program, prefix
	the name of the program with "!".
	
	Lines are executed through bash, so you can include 1-liner bash commands.
	If you need to execute a shell script that cannot fit into 1-line, have
	your script call a real shell script.
	
	It is intended that the last line of the script should usually execute
	your compiled program, but this is not required.
	
	You can specify which line(s) are the lines which execute your program
	by prefixing them with "%". These lines will be skipped if you choose
	"Build but Don't Run".
	
	The compile pane automatically closes when the end of the script is reached
	without an error condition or when the command "hide" is executed. To
	bypass this behavior, keep your script from reaching the end by including
	a line "stop".
*/
static const char *help[] =
{
"Build scripts are Sisong's version of \"Project files\". They are a simple system and",
"prevent your project from being tied to any one IDE. They also have the advantage",
"that you can use them to develop HTML, Perl, etc., and they will work with \"jam\".",
"",
"Each line in the file is passed to \"bash\" in turn, and the compile is aborted if a",
"nonzero value is returned. Most scripts would look something like this:",
"",
"cd /path_to_my_program/",
"make",
"%./my_program",
"",
"The \"%\" tells the editor which line it is that starts your program, so that the",
"\"Build but Don't Run\" menu item will work properly.",
"",
"You can also prefix a line with \"!\". This tells Sisong to always consider the command",
"successful, even if it returns an exit code other than zero.",
"",
"By default, the compile pane is hidden again once the last command is finished. To",
"change this behavior, you can add the command \"show\" at the end of the build script.",
"Or, you can check \"Show Console\" from Run menu to keep the build console up until you",
"explicitly dismiss it.",
"",
"This pane can be opened and closed with the hot-key \"ESC\". The pane remembers it's",
"previous contents and is shared between Compile, Search Results, and help.",
"",
"You can always quick-open a project's build script from the Projects menu.",
NULL
};

void BuildHelpPane::SetProjectBeingEdited(const char *name)
{
	maxcpy(fProjectName, name, sizeof(fProjectName)-1);
}

void BuildHelpPane::PopupOpening()
{
static const rgb_color grey = { 192, 192, 192 };
static const rgb_color yellow = { 250, 250, 0 };
	
	Clear();

	BString str;
	str << "You are now editing the build script for the project \""
		<< fProjectName << "\".";
	
	AddLine(str.String(), yellow, false);
	AddLine("", grey, false);	
	
	for(int i=0;help[i];i++)
	{
		AddLine(help[i], grey, false);
	}
	
	// ensure prevent auto-scrolling
	ScrollView->ScrollBar(B_VERTICAL)->SetValue(0);
}

void BuildHelpPane::PopupClosing()
{
	
}









