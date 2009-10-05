
#ifndef _PREFS_H
#define _PREFS_H

#include "../SubWindow.h"

class PrefsWindow : public SubWindow
{
public:
	PrefsWindow();
	~PrefsWindow();
	
	virtual void MessageReceived(BMessage *msg);
	
	friend class Preflet;
	void SettingsChanged();
	
	BRect GetContainerBounds()
		{ return fContainer->Bounds(); }
	
private:
	void AddPreflet(const char *name, BView *preflet);

	BButton *cmdRevert;
	
	BListView *fListView;
	BScrollView *fScrollView;

	// container view that preflets are placed in
	BView *fContainer;
	Preflet *fContainerContents;
	
	// list that maps the textual items in the fListView
	// to the BView's they correspond to
	BList fViewList;
	
	// a copy of "editor.settings"
	char *fRevertBuffer;
};


#endif
