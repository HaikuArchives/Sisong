#ifndef FINDBOX_H
#define FINDBOX_H

#include <View.h>
#include <Rect.h>
#include <CheckBox.h>
#include <TextControl.h>
#include <Button.h>
#include <TabView.h>
#include <Point.h>
#include <Window.h>

#include "SearchData.h"
#include "edit.h"

// values for mode parameter of constructor
#define FINDBOX_FIND		0
#define FINDBOX_REPLACE		1
#define FINDBOX_FIND_FILES	2

// DoFindNext options flags
#define FINDF_WHOLE_WORD		0x01
#define FINDF_CASE_SENSITIVE	0x02
#define FINDF_RECURSIVE			0x04
#define FINDF_FORCE_Y			0x08
#define FINDF_Y_AT_BOTTOM		0x10
#define FINDF_BACKWARDS			0x20	// find previous instead of find next
#define FINDF_IGNORE_DOT		0x40	// ignore "." hidden folders

// return values from DoFindNext
#define FN_HIT_FOUND			0
#define FN_NO_HITS				1
#define FN_NO_ADDITIONAL_HITS	2

// return values from SelectHit
#define SH_OK					0
#define SH_ALREADY_SELECTED		1

// the view that contains the controls, there is one for each tab
class FBView : public BView
{
	FBView(BRect frame, int initialMode);
	~FBView();
	static void InitTabData();

	friend class CFindBox;
	friend class FBTabView;

private:
	BTextControl *txtFindWhat;
	BTextControl *txtReplaceWith;
	BTextControl *txtFolder;
	BTextControl *txtFilter;
	BButton *DefButton;

	BCheckBox *chkWholeWord;
	BCheckBox *chkCaseSensitive;
	BCheckBox *chkBackwards;
	BCheckBox *chkRecursive;
	BCheckBox *chkIgnoreHiddenFolders;

	int fbmode;
};

// simple override for tabview control
class FBTabView : public BTabView
{
public:
	FBTabView(BRect frame, char *name)
		: BTabView(frame, name) { }

	virtual void MouseDown(BPoint where);
};

// the find box window itself
class CFindBox : public BWindow
{
public:
	CFindBox(int mode);
	~CFindBox();
	virtual void MessageReceived(BMessage *msg);
	virtual void DispatchMessage(BMessage *msg, BHandler *target);

	void HandleSearchRequest(BMessage *msg);
	void ProcessBrowseForFolderResults(BMessage *message);

	friend class FBView;
	friend class FBTabView;

private:
	char *GetSearchSettings(int *options);
	FBView *GetCurrentView();

	BTabView *tabview;
	FBView *views[3];
};


CFindBox *GetCurrentFindBox();
int DoInteractiveReplace(EditView *ev, const char *search_string, const char *replace_string, int options, bool *canceled);
static void SetPromptPos(BAlert *prompt, EditView *ev, int y);
int DoReplaceAll(EditView *ev, const char *search_string, const char *replace_string, int options);
int DoReplaceAllInAll(const char *search_string, const char *replace_string, int options);
int DoFindNext(const char *search_string, int options);
int SelectHit(const char *search_string, DocPoint hit_start, int options);
int DoFindAllInAll(const char *search_string, int options);
int DoFindAll(EditView *ev, const char *search_string, int options, bool clear_window_first);
void AddSearchResult(const char *filename, int x, int y, const char *linestr, const char *search_string);
int DoFindInFiles(const char *search_string, const char *folder, const char *filter, int options, bool recursive);
int SearchInFile(const char *filename, const char *search_string, int options);
DocPoint find_next_hit(EditView *ev, DocPoint start, const char *search_string, uint options);
DocPoint find_prev_hit(EditView *ev, DocPoint start, const char *search_string, uint options);
char *(*GetStringScanner(int options))(const char *, const char *, int);
char *normal_strstr(const char *haystack, const char *needle, int needle_length);
char *normal_strcasestr(const char *haystack, const char *needle, int needle_length);
char *wholeword_strstr(const char *haystack, const char *needle, int needle_length);
char *wholeword_strcasestr(const char *haystack, const char *needle, int needle_length);
char *wholeword_common(const char *haystack, const char *needle, int needle_length, char *(*StringScanner)(const char *haystack, const char *needle));


#endif // FINDBOX_H
