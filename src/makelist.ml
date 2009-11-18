<DEFAULT
COMPILE=gcc -O2 -c %SRCFILE% -o %MODULE%.%OBJ_EXT%
LPREFIX=gcc -o %EXEFILE%
LSUFFIX=-lbe -ltracker

OUTPUT=../line

>>
MainWindow.cpp
EditArea.cpp
EditPane.cpp
main.cpp
edit.cpp
edit_files.cpp
edit_keys.cpp
edit_cursor.cpp
UpdateLine.cpp
selection.cpp
edit_actions.cpp
line.cpp
lexer.cpp
redraw.cpp
Templates.cpp
InputBox.cpp
NewProject.cpp
projects.cpp
layout.cpp
PopupPane.cpp
CompilePane.cpp
SearchResults.cpp
BuildHelpPane.cpp
menu.cpp
draw_cursor.cpp
LNPanel.cpp
spacer.cpp
cmdseq.cpp
FindBox.cpp
DocPoint.cpp
edit_mouse.cpp
undo.cpp
bmatch.cpp
tabs.cpp
misc.cpp
clipboard.cpp
colors.cpp
config.cpp
ViewTimer.cpp
FontDrawer.cpp
SubWindow.cpp
ColoredStringItem.cpp
FunctionList.cpp
AutoSaver.cpp
testview.cpp
CList.cpp
MessageView.cpp
NotifyUpdate.cpp
stats.cpp
Prefs/Prefs.cpp
Prefs/AboutPreflet.cpp
Prefs/CheckboxPreflet.cpp
Prefs/ColorsPreflet.cpp
Prefs/ColorItem.cpp
Prefs/ColorView.cpp
Prefs/ShortcutsPreflet.cpp
Prefs/StatsPreflet.cpp
FileView/IconCache.cpp
FileView/QuickSearch.cpp
FileView/FileView.cpp
FileView/FileItem.cpp
FileView/TitleView.cpp
DirMenu/DirMenu.cpp
DirMenu/DirMenuItem.cpp
ColorPicker/ColorPicker.cpp
Walter/Spinner.cpp
../common/stat.cpp
../common/smal.cpp
../common/misc.cpp
../common/OffscreenBuffer.cpp
<<

IComm/IShelf.cpp

XTRAFLAGS=-g
COMPILE=gcc $XTRAFLAGS -c $SRCFILE -o $MODULE.$OBJ_EXT
PPONLY=gcc $XTRAFLAGS -E -c $SRCFILE -o $MODULE.$OBJ_EXT

LPREFIX=gcc -o %EXEFILE%
LSUFFIX=-lbe -ltracker

OUTPUT=../line
