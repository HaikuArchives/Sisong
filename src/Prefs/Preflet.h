
/*
	simple superclass for preflets in prefs window
*/

#include <Application.h>
#include <Window.h>
#include <View.h>
#include <ListView.h>
#include <ScrollView.h>
#include <StringView.h>
#include <TextView.h>
#include <CheckBox.h>

#include "Prefs.h"
#include "../MessageView.h"
#include "../editor.h"

#define M_CHECKBOX_CLICKED		'CBck'


class Preflet : public MessageView
{
public:
	Preflet(PrefsWindow *parent)
		: MessageView(parent->GetContainerBounds(), "preflet", B_FOLLOW_ALL, 0)
	{
		fParent = parent;
		SetViewColor(fParent->bgview->ViewColor());
	}
	
	virtual void ReloadSettings() { }
	virtual void PrefletOpening() { }
	virtual void PrefletClosing() { }
	
	virtual bool HaveSpecialRevert() { return false; }
	virtual void DoSpecialRevert() { }
	
protected:
	PrefsWindow *fParent;
};

