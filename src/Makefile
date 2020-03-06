
all:	sisong

sisong:  MainWindow.o EditArea.o EditPane.o main.o \
	 edit.o edit_files.o edit_keys.o edit_cursor.o UpdateLine.o \
	 selection.o edit_actions.o line.o lexer.o redraw.o \
	 Templates.o InputBox.o NewProject.o projects.o layout.o \
	 PopupPane.o CompilePane.o SearchResults.o BuildHelpPane.o menu.o \
	 draw_cursor.o LNPanel.o spacer.o cmdseq.o FindBox.o \
	 DocPoint.o edit_mouse.o undo.o bmatch.o tabs.o \
	 misc.o clipboard.o colors.o config.o ViewTimer.o \
	 FontDrawer.o SubWindow.o ColoredStringItem.o OffscreenBuffer.o FunctionList.o \
	 AutoSaver.o testview.o CList.o MessageView.o NotifyUpdate.o \
	 stats.o Prefs/Prefs.o Prefs/AboutPreflet.o Prefs/CheckboxPreflet.o Prefs/ColorsPreflet.o \
	 Prefs/ColorItem.o Prefs/ColorView.o Prefs/ShortcutsPreflet.o Prefs/StatsPreflet.o FileView/IconCache.o \
	 FileView/QuickSearch.o FileView/FileView.o FileView/FileItem.o FileView/TitleView.o DirMenu/DirMenu.o \
	 DirMenu/DirMenuItem.o ColorPicker/ColorPicker.o Walter/Spinner.o ../common/stat.o ../common/misc.o
	g++ \
	 MainWindow.o EditArea.o EditPane.o main.o \
	 edit.o edit_files.o edit_keys.o edit_cursor.o UpdateLine.o \
	 selection.o edit_actions.o line.o lexer.o redraw.o \
	 Templates.o InputBox.o NewProject.o projects.o layout.o \
	 PopupPane.o CompilePane.o SearchResults.o BuildHelpPane.o menu.o \
	 draw_cursor.o LNPanel.o spacer.o cmdseq.o FindBox.o \
	 DocPoint.o edit_mouse.o undo.o bmatch.o tabs.o \
	 misc.o clipboard.o colors.o config.o ViewTimer.o \
	 FontDrawer.o SubWindow.o ColoredStringItem.o OffscreenBuffer.o FunctionList.o \
	 AutoSaver.o testview.o CList.o MessageView.o NotifyUpdate.o \
	 stats.o Prefs/Prefs.o Prefs/AboutPreflet.o Prefs/CheckboxPreflet.o Prefs/ColorsPreflet.o \
	 Prefs/ColorItem.o Prefs/ColorView.o Prefs/ShortcutsPreflet.o Prefs/StatsPreflet.o FileView/IconCache.o \
	 FileView/QuickSearch.o FileView/FileView.o FileView/FileItem.o FileView/TitleView.o DirMenu/DirMenu.o \
	 DirMenu/DirMenuItem.o ColorPicker/ColorPicker.o Walter/Spinner.o ../common/stat.o ../common/misc.o \
	 -o sisong -lbe -ltracker

MainWindow.o:	MainWindow.cpp MainWindow.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c MainWindow.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o MainWindow.o

EditArea.o:	EditArea.cpp EditArea.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c EditArea.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o EditArea.o

EditPane.o:	EditPane.cpp EditPane.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c EditPane.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o EditPane.o

main.o:	main.cpp main.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c main.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o main.o

edit.o:	edit.cpp edit.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit.o

edit_files.o:	edit_files.cpp edit_files.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit_files.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit_files.o

edit_keys.o:	edit_keys.cpp edit_keys.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit_keys.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit_keys.o

edit_cursor.o:	edit_cursor.cpp edit_cursor.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit_cursor.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit_cursor.o

UpdateLine.o:	UpdateLine.cpp UpdateLine.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c UpdateLine.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o UpdateLine.o

selection.o:	selection.cpp selection.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c selection.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o selection.o

edit_actions.o:	edit_actions.cpp edit_actions.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit_actions.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit_actions.o

line.o:	line.cpp line.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c line.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o line.o

lexer.o:	lexer.cpp lexer.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		keywords_constant.h
	g++ -g -c lexer.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o lexer.o

redraw.o:	redraw.cpp redraw.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c redraw.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o redraw.o

Templates.o:	Templates.cpp Templates.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c Templates.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Templates.o

InputBox.o:	InputBox.cpp InputBox.fdh ../common/basics.h InputBox.h
	g++ -g -c InputBox.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o InputBox.o

NewProject.o:	NewProject.cpp NewProject.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		NewProject.h FileView/FileView.h FileView/TitleView.h \
		FileView/IconCache.h FileView/QuickSearch.h DirMenu/DirMenu.h
	g++ -g -c NewProject.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o NewProject.o

projects.o:	projects.cpp projects.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c projects.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o projects.o

layout.o:	layout.cpp layout.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c layout.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o layout.o

PopupPane.o:	PopupPane.cpp PopupPane.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c PopupPane.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o PopupPane.o

CompilePane.o:	CompilePane.cpp CompilePane.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		CompilePane.lh
	g++ -g -c CompilePane.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o CompilePane.o

SearchResults.o:	SearchResults.cpp SearchResults.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c SearchResults.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o SearchResults.o

BuildHelpPane.o:	BuildHelpPane.cpp BuildHelpPane.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c BuildHelpPane.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o BuildHelpPane.o

menu.o:	menu.cpp menu.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		NewProject.h Prefs/Prefs.h
	g++ -g -c menu.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o menu.o

draw_cursor.o:	draw_cursor.cpp draw_cursor.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c draw_cursor.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o draw_cursor.o

LNPanel.o:	LNPanel.cpp LNPanel.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c LNPanel.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o LNPanel.o

spacer.o:	spacer.cpp spacer.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c spacer.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o spacer.o

cmdseq.o:	cmdseq.cpp cmdseq.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c cmdseq.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o cmdseq.o

FindBox.o:	FindBox.cpp FindBox.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c FindBox.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FindBox.o

DocPoint.o:	DocPoint.cpp DocPoint.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c DocPoint.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o DocPoint.o

edit_mouse.o:	edit_mouse.cpp edit_mouse.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c edit_mouse.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o edit_mouse.o

undo.o:	undo.cpp undo.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c undo.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o undo.o

bmatch.o:	bmatch.cpp bmatch.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c bmatch.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o bmatch.o

tabs.o:	tabs.cpp tabs.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c tabs.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o tabs.o

misc.o:	misc.cpp misc.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c misc.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o misc.o

clipboard.o:	clipboard.cpp clipboard.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c clipboard.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o clipboard.o

colors.o:	colors.cpp colors.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c colors.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o colors.o

config.o:	config.cpp config.fdh ../common/basics.h config.h
	g++ -g -c config.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o config.o

ViewTimer.o:	ViewTimer.cpp ViewTimer.fdh ViewTimer.h ../common/basics.h
	g++ -g -c ViewTimer.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o ViewTimer.o

FontDrawer.o:	FontDrawer.cpp FontDrawer.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c FontDrawer.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FontDrawer.o

SubWindow.o:	SubWindow.cpp SubWindow.fdh SubWindow.h
	g++ -g -c SubWindow.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o SubWindow.o

ColoredStringItem.o:	ColoredStringItem.cpp ColoredStringItem.fdh ColoredStringItem.h colors.h
	g++ -g -c ColoredStringItem.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o ColoredStringItem.o

OffscreenBuffer.o:	OffscreenBuffer.cpp OffscreenBuffer.fdh OffscreenBuffer.h
	g++ -g -c OffscreenBuffer.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o OffscreenBuffer.o

FunctionList.o:	FunctionList.cpp FunctionList.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c FunctionList.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FunctionList.o

AutoSaver.o:	AutoSaver.cpp AutoSaver.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c AutoSaver.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o AutoSaver.o

testview.o:	testview.cpp testview.fdh testview.h
	g++ -g -c testview.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o testview.o

CList.o:	CList.cpp CList.fdh CList.h
	g++ -g -c CList.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o CList.o

MessageView.o:	MessageView.cpp MessageView.fdh MessageView.h
	g++ -g -c MessageView.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o MessageView.o

NotifyUpdate.o:	NotifyUpdate.cpp NotifyUpdate.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c NotifyUpdate.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o NotifyUpdate.o

stats.o:	stats.cpp stats.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		IComm/IShelf.h MainWindow.h ColoredStringItem.h
	g++ -g -c stats.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o stats.o

Prefs/Prefs.o:	Prefs/Prefs.cpp Prefs/Prefs.fdh editor.h ../common/basics.h \
		SubWindow.h QListView.h MessageView.h \
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
		Prefs/Prefs.h Prefs/Preflet.h Prefs/AboutPreflet.h \
		Prefs/CheckboxPreflet.h Prefs/ColorsPreflet.h Prefs/ColorView.h \
		Prefs/ColorItem.h Walter/Spinner.h Prefs/ShortcutsPreflet.h \
		Prefs/StatsPreflet.h
	g++ -g -c Prefs/Prefs.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/Prefs.o

Prefs/AboutPreflet.o:	Prefs/AboutPreflet.cpp Prefs/AboutPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/AboutPreflet.h
	g++ -g -c Prefs/AboutPreflet.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/AboutPreflet.o

Prefs/CheckboxPreflet.o:	Prefs/CheckboxPreflet.cpp Prefs/CheckboxPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/CheckboxPreflet.h
	g++ -g -c Prefs/CheckboxPreflet.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/CheckboxPreflet.o

Prefs/ColorsPreflet.o:	Prefs/ColorsPreflet.cpp Prefs/ColorsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/ColorsPreflet.h \
		Prefs/ColorView.h Prefs/ColorItem.h Walter/Spinner.h
	g++ -g -c Prefs/ColorsPreflet.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/ColorsPreflet.o

Prefs/ColorItem.o:	Prefs/ColorItem.cpp Prefs/ColorItem.fdh ../common/basics.h MessageView.h \
		colors.h Prefs/ColorItem.h Prefs/ColorView.h
	g++ -g -c Prefs/ColorItem.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/ColorItem.o

Prefs/ColorView.o:	Prefs/ColorView.cpp Prefs/ColorView.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/ColorItem.h \
		Prefs/ColorView.h ColorPicker/ColorPicker.h
	g++ -g -c Prefs/ColorView.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/ColorView.o

Prefs/ShortcutsPreflet.o:	Prefs/ShortcutsPreflet.cpp Prefs/ShortcutsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/ShortcutsPreflet.h
	g++ -g -c Prefs/ShortcutsPreflet.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/ShortcutsPreflet.o

Prefs/StatsPreflet.o:	Prefs/StatsPreflet.cpp Prefs/StatsPreflet.fdh Prefs/Preflet.h Prefs/Prefs.h \
		SubWindow.h MessageView.h editor.h \
		../common/basics.h QListView.h testview.h \
		CList.h config.h colors.h \
		messages.h lexer.h line.h \
		DocPoint.h FindBox.h selection.h \
		undo.h bmatch.h clipboard.h \
		cmdseq.h edit_cursor.h edit_mouse.h \
		edit.h tabs.h draw.h \
		FontDrawer.h draw_cursor.h LNPanel.h \
		spacer.h ViewTimer.h EditPane.h \
		OffscreenBuffer.h menu.h main.h \
		projects.h InputBox.h PopupPane.h \
		SearchResults.h CompilePane.h BuildHelpPane.h \
		FunctionList.h EditArea.h IComm/IShelf.h \
		MainWindow.h ColoredStringItem.h Prefs/StatsPreflet.h
	g++ -g -c Prefs/StatsPreflet.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Prefs/StatsPreflet.o

FileView/IconCache.o:	FileView/IconCache.cpp FileView/IconCache.fdh FileView/IconCache.h FileView/QuickSearch.h
	g++ -g -c FileView/IconCache.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FileView/IconCache.o

FileView/QuickSearch.o:	FileView/QuickSearch.cpp FileView/QuickSearch.fdh FileView/QuickSearch.h
	g++ -g -c FileView/QuickSearch.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FileView/QuickSearch.o

FileView/FileView.o:	FileView/FileView.cpp FileView/FileView.fdh ../common/basics.h FileView/FileItem.h \
		FileView/IconCache.h FileView/QuickSearch.h FileView/FileView.h \
		FileView/TitleView.h
	g++ -g -c FileView/FileView.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FileView/FileView.o

FileView/FileItem.o:	FileView/FileItem.cpp FileView/FileItem.fdh FileView/FileItem.h FileView/IconCache.h \
		FileView/QuickSearch.h
	g++ -g -c FileView/FileItem.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FileView/FileItem.o

FileView/TitleView.o:	FileView/TitleView.cpp FileView/TitleView.fdh FileView/TitleView.h FileView/FileItem.h \
		FileView/IconCache.h FileView/QuickSearch.h
	g++ -g -c FileView/TitleView.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o FileView/TitleView.o

DirMenu/DirMenu.o:	DirMenu/DirMenu.cpp DirMenu/DirMenu.fdh ../common/basics.h DirMenu/DirMenuItem.h \
		DirMenu/DirMenu.h FileView/IconCache.h FileView/QuickSearch.h \
		FileView/FileView.h FileView/TitleView.h
	g++ -g -c DirMenu/DirMenu.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o DirMenu/DirMenu.o

DirMenu/DirMenuItem.o:	DirMenu/DirMenuItem.cpp DirMenu/DirMenuItem.fdh ../common/basics.h DirMenu/DirMenuItem.h
	g++ -g -c DirMenu/DirMenuItem.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o DirMenu/DirMenuItem.o

ColorPicker/ColorPicker.o:	ColorPicker/ColorPicker.cpp ColorPicker/ColorPicker.fdh ColorPicker/ColorPicker.h SubWindow.h
	g++ -g -c ColorPicker/ColorPicker.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o ColorPicker/ColorPicker.o

Walter/Spinner.o:	Walter/Spinner.cpp Walter/Spinner.fdh Walter/Spinner.h
	g++ -g -c Walter/Spinner.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o Walter/Spinner.o

../common/stat.o:	../common/stat.cpp ../common/stat.fdh ../common/basics.h
	g++ -g -c ../common/stat.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o ../common/stat.o

../common/misc.o:	../common/misc.cpp ../common/misc.fdh ../common/basics.h
	g++ -g -c ../common/misc.cpp -Wno-narrowing -Wno-write-strings -Wno-multichar -Wno-unused-result -Wunused-variable -o ../common/misc.o

.FORCE:

clean:
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
	rm -f ../common/misc.o
	rm -f sisong

cleanfdh:
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
	rm -f ../common/misc.fdh

cleanall: clean cleanfdh

