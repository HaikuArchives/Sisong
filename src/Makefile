
all: ../line

../line:  IComm/IShelf.o MainWindow.o EditArea.o EditPane.o \
	 main.o edit.o edit_files.o edit_keys.o edit_cursor.o \
	 UpdateLine.o selection.o edit_actions.o line.o lexer.o \
	 redraw.o Templates.o InputBox.o NewProject.o projects.o \
	 layout.o PopupPane.o CompilePane.o SearchResults.o BuildHelpPane.o \
	 menu.o draw_cursor.o LNPanel.o spacer.o cmdseq.o \
	 FindBox.o DocPoint.o edit_mouse.o undo.o bmatch.o \
	 tabs.o misc.o clipboard.o colors.o config.o \
	 ViewTimer.o FontDrawer.o SubWindow.o ColoredStringItem.o OffscreenBuffer.o \
	 FunctionList.o AutoSaver.o testview.o CList.o MessageView.o \
	 NotifyUpdate.o stats.o Prefs/Prefs.o Prefs/AboutPreflet.o Prefs/CheckboxPreflet.o \
	 Prefs/ColorsPreflet.o Prefs/ColorItem.o Prefs/ColorView.o Prefs/ShortcutsPreflet.o Prefs/StatsPreflet.o \
	 FileView/IconCache.o FileView/QuickSearch.o FileView/FileView.o FileView/FileItem.o FileView/TitleView.o \
	 DirMenu/DirMenu.o DirMenu/DirMenuItem.o ColorPicker/ColorPicker.o Walter/Spinner.o ../common/stat.o \
	 ../common/smal.o
	gcc -o ../line \
	 IComm/IShelf.o MainWindow.o EditArea.o EditPane.o \
	 main.o edit.o edit_files.o edit_keys.o edit_cursor.o \
	 UpdateLine.o selection.o edit_actions.o line.o lexer.o \
	 redraw.o Templates.o InputBox.o NewProject.o projects.o \
	 layout.o PopupPane.o CompilePane.o SearchResults.o BuildHelpPane.o \
	 menu.o draw_cursor.o LNPanel.o spacer.o cmdseq.o \
	 FindBox.o DocPoint.o edit_mouse.o undo.o bmatch.o \
	 tabs.o misc.o clipboard.o colors.o config.o \
	 ViewTimer.o FontDrawer.o SubWindow.o ColoredStringItem.o OffscreenBuffer.o \
	 FunctionList.o AutoSaver.o testview.o CList.o MessageView.o \
	 NotifyUpdate.o stats.o Prefs/Prefs.o Prefs/AboutPreflet.o Prefs/CheckboxPreflet.o \
	 Prefs/ColorsPreflet.o Prefs/ColorItem.o Prefs/ColorView.o Prefs/ShortcutsPreflet.o Prefs/StatsPreflet.o \
	 FileView/IconCache.o FileView/QuickSearch.o FileView/FileView.o FileView/FileItem.o FileView/TitleView.o \
	 DirMenu/DirMenu.o DirMenu/DirMenuItem.o ColorPicker/ColorPicker.o Walter/Spinner.o ../common/stat.o \
	 ../common/smal.o \
	 -lbe -ltracker

IComm/IShelf.o:	IComm/IShelf.cpp IComm/IShelf.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h IComm/IComm.h
	gcc -O2 -c IComm/IShelf.cpp -o IComm/IShelf.o

MainWindow.o:	MainWindow.cpp MainWindow.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c MainWindow.cpp -o MainWindow.o

EditArea.o:	EditArea.cpp EditArea.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c EditArea.cpp -o EditArea.o

EditPane.o:	EditPane.cpp EditPane.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c EditPane.cpp -o EditPane.o

main.o:	main.cpp main.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c main.cpp -o main.o

edit.o:	edit.cpp edit.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit.cpp -o edit.o

edit_files.o:	edit_files.cpp edit_files.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit_files.cpp -o edit_files.o

edit_keys.o:	edit_keys.cpp edit_keys.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit_keys.cpp -o edit_keys.o

edit_cursor.o:	edit_cursor.cpp edit_cursor.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit_cursor.cpp -o edit_cursor.o

UpdateLine.o:	UpdateLine.cpp UpdateLine.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c UpdateLine.cpp -o UpdateLine.o

selection.o:	selection.cpp selection.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c selection.cpp -o selection.o

edit_actions.o:	edit_actions.cpp edit_actions.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit_actions.cpp -o edit_actions.o

line.o:	line.cpp line.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c line.cpp -o line.o

lexer.o:	lexer.cpp lexer.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h keywords_constant.h
	gcc -O2 -c lexer.cpp -o lexer.o

redraw.o:	redraw.cpp redraw.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c redraw.cpp -o redraw.o

Templates.o:	Templates.cpp Templates.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c Templates.cpp -o Templates.o

InputBox.o:	InputBox.cpp InputBox.fdh ../common/basics.h InputBox.h
	gcc -O2 -c InputBox.cpp -o InputBox.o

NewProject.o:	NewProject.cpp NewProject.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h NewProject.h FileView/FileView.h \
		FileView/TitleView.h FileView/IconCache.h FileView/QuickSearch.h \
		DirMenu/DirMenu.h
	gcc -O2 -c NewProject.cpp -o NewProject.o

projects.o:	projects.cpp projects.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c projects.cpp -o projects.o

layout.o:	layout.cpp layout.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c layout.cpp -o layout.o

PopupPane.o:	PopupPane.cpp PopupPane.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c PopupPane.cpp -o PopupPane.o

CompilePane.o:	CompilePane.cpp CompilePane.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h CompilePane.lh
	gcc -O2 -c CompilePane.cpp -o CompilePane.o

SearchResults.o:	SearchResults.cpp SearchResults.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c SearchResults.cpp -o SearchResults.o

BuildHelpPane.o:	BuildHelpPane.cpp BuildHelpPane.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c BuildHelpPane.cpp -o BuildHelpPane.o

menu.o:	menu.cpp menu.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h NewProject.h Prefs/Prefs.h
	gcc -O2 -c menu.cpp -o menu.o

draw_cursor.o:	draw_cursor.cpp draw_cursor.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c draw_cursor.cpp -o draw_cursor.o

LNPanel.o:	LNPanel.cpp LNPanel.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c LNPanel.cpp -o LNPanel.o

spacer.o:	spacer.cpp spacer.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c spacer.cpp -o spacer.o

cmdseq.o:	cmdseq.cpp cmdseq.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c cmdseq.cpp -o cmdseq.o

FindBox.o:	FindBox.cpp FindBox.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c FindBox.cpp -o FindBox.o

DocPoint.o:	DocPoint.cpp DocPoint.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c DocPoint.cpp -o DocPoint.o

edit_mouse.o:	edit_mouse.cpp edit_mouse.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c edit_mouse.cpp -o edit_mouse.o

undo.o:	undo.cpp undo.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c undo.cpp -o undo.o

bmatch.o:	bmatch.cpp bmatch.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c bmatch.cpp -o bmatch.o

tabs.o:	tabs.cpp tabs.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c tabs.cpp -o tabs.o

misc.o:	misc.cpp misc.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c misc.cpp -o misc.o

clipboard.o:	clipboard.cpp clipboard.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c clipboard.cpp -o clipboard.o

colors.o:	colors.cpp colors.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c colors.cpp -o colors.o

config.o:	config.cpp config.fdh ../common/basics.h config.h
	gcc -O2 -c config.cpp -o config.o

ViewTimer.o:	ViewTimer.cpp ViewTimer.fdh ViewTimer.h ../common/basics.h
	gcc -O2 -c ViewTimer.cpp -o ViewTimer.o

FontDrawer.o:	FontDrawer.cpp FontDrawer.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c FontDrawer.cpp -o FontDrawer.o

SubWindow.o:	SubWindow.cpp SubWindow.fdh SubWindow.h
	gcc -O2 -c SubWindow.cpp -o SubWindow.o

ColoredStringItem.o:	ColoredStringItem.cpp ColoredStringItem.fdh ColoredStringItem.h colors.h
	gcc -O2 -c ColoredStringItem.cpp -o ColoredStringItem.o

OffscreenBuffer.o:	OffscreenBuffer.cpp OffscreenBuffer.fdh OffscreenBuffer.h
	gcc -O2 -c OffscreenBuffer.cpp -o OffscreenBuffer.o

FunctionList.o:	FunctionList.cpp FunctionList.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c FunctionList.cpp -o FunctionList.o

AutoSaver.o:	AutoSaver.cpp AutoSaver.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c AutoSaver.cpp -o AutoSaver.o

testview.o:	testview.cpp testview.fdh testview.h
	gcc -O2 -c testview.cpp -o testview.o

CList.o:	CList.cpp CList.fdh CList.h
	gcc -O2 -c CList.cpp -o CList.o

MessageView.o:	MessageView.cpp MessageView.fdh MessageView.h
	gcc -O2 -c MessageView.cpp -o MessageView.o

NotifyUpdate.o:	NotifyUpdate.cpp NotifyUpdate.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c NotifyUpdate.cpp -o NotifyUpdate.o

stats.o:	stats.cpp stats.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h
	gcc -O2 -c stats.cpp -o stats.o

Prefs/Prefs.o:	Prefs/Prefs.cpp Prefs/Prefs.fdh editor.h ../common/basics.h \
		../common/smal.h SubWindow.h QListView.h \
		MessageView.h testview.h CList.h \
		config.h colors.h messages.h \
		lexer.h line.h DocPoint.h \
		FindBox.h selection.h undo.h \
		bmatch.h clipboard.h cmdseq.h \
		edit_cursor.h edit_mouse.h edit.h \
		tabs.h draw.h FontDrawer.h \
		draw_cursor.h LNPanel.h spacer.h \
		ViewTimer.h EditPane.h OffscreenBuffer.h \
		menu.h main.h projects.h \
		InputBox.h PopupPane.h SearchResults.h \
		CompilePane.h BuildHelpPane.h FunctionList.h \
		EditArea.h IComm/IShelf.h MainWindow.h \
		ColoredStringItem.h Prefs/Prefs.h Prefs/Preflet.h \
		Prefs/AboutPreflet.h Prefs/CheckboxPreflet.h Prefs/ColorsPreflet.h \
		Prefs/ColorView.h Prefs/ColorItem.h Walter/Spinner.h \
		Prefs/ShortcutsPreflet.h Prefs/StatsPreflet.h
	gcc -O2 -c Prefs/Prefs.cpp -o Prefs/Prefs.o

Prefs/AboutPreflet.o:	Prefs/AboutPreflet.cpp Prefs/AboutPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/AboutPreflet.h
	gcc -O2 -c Prefs/AboutPreflet.cpp -o Prefs/AboutPreflet.o

Prefs/CheckboxPreflet.o:	Prefs/CheckboxPreflet.cpp Prefs/CheckboxPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/CheckboxPreflet.h
	gcc -O2 -c Prefs/CheckboxPreflet.cpp -o Prefs/CheckboxPreflet.o

Prefs/ColorsPreflet.o:	Prefs/ColorsPreflet.cpp Prefs/ColorsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/ColorsPreflet.h Prefs/ColorView.h Prefs/ColorItem.h \
		Walter/Spinner.h
	gcc -O2 -c Prefs/ColorsPreflet.cpp -o Prefs/ColorsPreflet.o

Prefs/ColorItem.o:	Prefs/ColorItem.cpp Prefs/ColorItem.fdh ../common/basics.h MessageView.h \
		colors.h Prefs/ColorItem.h Prefs/ColorView.h
	gcc -O2 -c Prefs/ColorItem.cpp -o Prefs/ColorItem.o

Prefs/ColorView.o:	Prefs/ColorView.cpp Prefs/ColorView.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/ColorItem.h Prefs/ColorView.h ColorPicker/ColorPicker.h
	gcc -O2 -c Prefs/ColorView.cpp -o Prefs/ColorView.o

Prefs/ShortcutsPreflet.o:	Prefs/ShortcutsPreflet.cpp Prefs/ShortcutsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/ShortcutsPreflet.h
	gcc -O2 -c Prefs/ShortcutsPreflet.cpp -o Prefs/ShortcutsPreflet.o

Prefs/StatsPreflet.o:	Prefs/StatsPreflet.cpp Prefs/StatsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h ../common/smal.h QListView.h \
		testview.h CList.h config.h \
		colors.h messages.h lexer.h \
		line.h DocPoint.h FindBox.h \
		selection.h undo.h bmatch.h \
		clipboard.h cmdseq.h edit_cursor.h \
		edit_mouse.h edit.h tabs.h \
		draw.h FontDrawer.h draw_cursor.h \
		LNPanel.h spacer.h ViewTimer.h \
		EditPane.h OffscreenBuffer.h menu.h \
		main.h projects.h InputBox.h \
		PopupPane.h SearchResults.h CompilePane.h \
		BuildHelpPane.h FunctionList.h EditArea.h \
		IComm/IShelf.h MainWindow.h ColoredStringItem.h \
		Prefs/StatsPreflet.h
	gcc -O2 -c Prefs/StatsPreflet.cpp -o Prefs/StatsPreflet.o

FileView/IconCache.o:	FileView/IconCache.cpp FileView/IconCache.fdh FileView/IconCache.h FileView/QuickSearch.h
	gcc -O2 -c FileView/IconCache.cpp -o FileView/IconCache.o

FileView/QuickSearch.o:	FileView/QuickSearch.cpp FileView/QuickSearch.fdh FileView/QuickSearch.h
	gcc -O2 -c FileView/QuickSearch.cpp -o FileView/QuickSearch.o

FileView/FileView.o:	FileView/FileView.cpp FileView/FileView.fdh ../common/basics.h FileView/FileItem.h \
		FileView/IconCache.h FileView/QuickSearch.h FileView/FileView.h \
		FileView/TitleView.h
	gcc -O2 -c FileView/FileView.cpp -o FileView/FileView.o

FileView/FileItem.o:	FileView/FileItem.cpp FileView/FileItem.fdh FileView/FileItem.h FileView/IconCache.h \
		FileView/QuickSearch.h
	gcc -O2 -c FileView/FileItem.cpp -o FileView/FileItem.o

FileView/TitleView.o:	FileView/TitleView.cpp FileView/TitleView.fdh FileView/TitleView.h FileView/FileItem.h \
		FileView/IconCache.h FileView/QuickSearch.h
	gcc -O2 -c FileView/TitleView.cpp -o FileView/TitleView.o

DirMenu/DirMenu.o:	DirMenu/DirMenu.cpp DirMenu/DirMenu.fdh ../common/basics.h DirMenu/DirMenuItem.h \
		DirMenu/DirMenu.h FileView/IconCache.h FileView/QuickSearch.h \
		FileView/FileView.h FileView/TitleView.h
	gcc -O2 -c DirMenu/DirMenu.cpp -o DirMenu/DirMenu.o

DirMenu/DirMenuItem.o:	DirMenu/DirMenuItem.cpp DirMenu/DirMenuItem.fdh ../common/basics.h DirMenu/DirMenuItem.h
	gcc -O2 -c DirMenu/DirMenuItem.cpp -o DirMenu/DirMenuItem.o

ColorPicker/ColorPicker.o:	ColorPicker/ColorPicker.cpp ColorPicker/ColorPicker.fdh ColorPicker/ColorPicker.h SubWindow.h
	gcc -O2 -c ColorPicker/ColorPicker.cpp -o ColorPicker/ColorPicker.o

Walter/Spinner.o:	Walter/Spinner.cpp Walter/Spinner.fdh Walter/Spinner.h
	gcc -O2 -c Walter/Spinner.cpp -o Walter/Spinner.o

../common/stat.o:	../common/stat.cpp ../common/stat.fdh ../common/basics.h
	gcc -O2 -c ../common/stat.cpp -o ../common/stat.o

../common/smal.o:	../common/smal.cpp ../common/smal.fdh ../common/basics.h ../common/smal_atree.c \
		../common/atree.h ../common/smal.h ../common/smal_record.h
	gcc -O2 -c ../common/smal.cpp -o ../common/smal.o


clean:
	rm -f IComm/IShelf.o
	rm -f MainWindow.o
	rm -f EditArea.o
	rm -f EditPane.o
	rm -f main.o
	rm -f edit.o
	rm -f edit_files.o
	rm -f edit_keys.o
	rm -f edit_cursor.o
	rm -f UpdateLine.o
	rm -f selection.o
	rm -f edit_actions.o
	rm -f line.o
	rm -f lexer.o
	rm -f redraw.o
	rm -f Templates.o
	rm -f InputBox.o
	rm -f NewProject.o
	rm -f projects.o
	rm -f layout.o
	rm -f PopupPane.o
	rm -f CompilePane.o
	rm -f SearchResults.o
	rm -f BuildHelpPane.o
	rm -f menu.o
	rm -f draw_cursor.o
	rm -f LNPanel.o
	rm -f spacer.o
	rm -f cmdseq.o
	rm -f FindBox.o
	rm -f DocPoint.o
	rm -f edit_mouse.o
	rm -f undo.o
	rm -f bmatch.o
	rm -f tabs.o
	rm -f misc.o
	rm -f clipboard.o
	rm -f colors.o
	rm -f config.o
	rm -f ViewTimer.o
	rm -f FontDrawer.o
	rm -f SubWindow.o
	rm -f ColoredStringItem.o
	rm -f OffscreenBuffer.o
	rm -f FunctionList.o
	rm -f AutoSaver.o
	rm -f testview.o
	rm -f CList.o
	rm -f MessageView.o
	rm -f NotifyUpdate.o
	rm -f stats.o
	rm -f Prefs/Prefs.o
	rm -f Prefs/AboutPreflet.o
	rm -f Prefs/CheckboxPreflet.o
	rm -f Prefs/ColorsPreflet.o
	rm -f Prefs/ColorItem.o
	rm -f Prefs/ColorView.o
	rm -f Prefs/ShortcutsPreflet.o
	rm -f Prefs/StatsPreflet.o
	rm -f FileView/IconCache.o
	rm -f FileView/QuickSearch.o
	rm -f FileView/FileView.o
	rm -f FileView/FileItem.o
	rm -f FileView/TitleView.o
	rm -f DirMenu/DirMenu.o
	rm -f DirMenu/DirMenuItem.o
	rm -f ColorPicker/ColorPicker.o
	rm -f Walter/Spinner.o
	rm -f ../common/stat.o
	rm -f ../common/smal.o

cleanfdh:
	rm -f IComm/IShelf.fdh
	rm -f MainWindow.fdh
	rm -f EditArea.fdh
	rm -f EditPane.fdh
	rm -f main.fdh
	rm -f edit.fdh
	rm -f edit_files.fdh
	rm -f edit_keys.fdh
	rm -f edit_cursor.fdh
	rm -f UpdateLine.fdh
	rm -f selection.fdh
	rm -f edit_actions.fdh
	rm -f line.fdh
	rm -f lexer.fdh
	rm -f redraw.fdh
	rm -f Templates.fdh
	rm -f InputBox.fdh
	rm -f NewProject.fdh
	rm -f projects.fdh
	rm -f layout.fdh
	rm -f PopupPane.fdh
	rm -f CompilePane.fdh
	rm -f SearchResults.fdh
	rm -f BuildHelpPane.fdh
	rm -f menu.fdh
	rm -f draw_cursor.fdh
	rm -f LNPanel.fdh
	rm -f spacer.fdh
	rm -f cmdseq.fdh
	rm -f FindBox.fdh
	rm -f DocPoint.fdh
	rm -f edit_mouse.fdh
	rm -f undo.fdh
	rm -f bmatch.fdh
	rm -f tabs.fdh
	rm -f misc.fdh
	rm -f clipboard.fdh
	rm -f colors.fdh
	rm -f config.fdh
	rm -f ViewTimer.fdh
	rm -f FontDrawer.fdh
	rm -f SubWindow.fdh
	rm -f ColoredStringItem.fdh
	rm -f OffscreenBuffer.fdh
	rm -f FunctionList.fdh
	rm -f AutoSaver.fdh
	rm -f testview.fdh
	rm -f CList.fdh
	rm -f MessageView.fdh
	rm -f NotifyUpdate.fdh
	rm -f stats.fdh
	rm -f Prefs/Prefs.fdh
	rm -f Prefs/AboutPreflet.fdh
	rm -f Prefs/CheckboxPreflet.fdh
	rm -f Prefs/ColorsPreflet.fdh
	rm -f Prefs/ColorItem.fdh
	rm -f Prefs/ColorView.fdh
	rm -f Prefs/ShortcutsPreflet.fdh
	rm -f Prefs/StatsPreflet.fdh
	rm -f FileView/IconCache.fdh
	rm -f FileView/QuickSearch.fdh
	rm -f FileView/FileView.fdh
	rm -f FileView/FileItem.fdh
	rm -f FileView/TitleView.fdh
	rm -f DirMenu/DirMenu.fdh
	rm -f DirMenu/DirMenuItem.fdh
	rm -f ColorPicker/ColorPicker.fdh
	rm -f Walter/Spinner.fdh
	rm -f ../common/stat.fdh
	rm -f ../common/smal.fdh

cleanall: clean cleanfdh

