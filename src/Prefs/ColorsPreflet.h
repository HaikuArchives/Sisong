
#include "ColorView.h"
#include "ColorItem.h"
#include "../Walter/Spinner.h"

class ColorsPreflet : public Preflet
{
public:
	ColorsPreflet(PrefsWindow *parent);
	virtual void MessageReceived(BMessage *msg);
	virtual void ReloadSettings();
	
	virtual bool HaveSpecialRevert();
	virtual void DoSpecialRevert();

	virtual void PrefletClosing();
	
	void UpdateSchemesMenu();
	
private:
	ColorView *fColorsList;
	BScrollView *fScrollView;
	
	BPopUpMenu *fFontMenu;
	BMenuField *fFontField;
	Spinner *fFontSize;
	
	BPopUpMenu *fSchemeMenu;
	BMenuField *fSchemeField;
};
