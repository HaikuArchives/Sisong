
#include "Preflet.h"
#include "ColorsPreflet.h"

#include <PopUpMenu.h>
#include <MenuItem.h>
#include <MenuField.h>
#include <StringView.h>

#define M_POINTSIZE_CHANGED		'FSzC'
#define M_SCHEMESEL_CHANGED		'SslC'
#define M_SCHEME_NEW			'SCHn'
#define M_SCHEME_DELETE			'SCHd'
#define M_SCHEME_DEFAULTS		'DEFA'

/*
void c------------------------------() {}
*/

ColorsPreflet::ColorsPreflet(PrefsWindow *parent)
	: Preflet(parent)
{
	// clear colors listview
	BRect lvrc(Bounds());
	lvrc.InsetBy(20, 50);
	lvrc.OffsetBy(0, -5);
	lvrc.right--;
	lvrc.top += 12;	// make room for cut/paste instructions
	lvrc.bottom -= 2;	// looks nicer
	lvrc.OffsetBy(0, 1);
	
	// create list
	lvrc.InsetBy(2, 2);
	lvrc.right -= B_V_SCROLL_BAR_WIDTH;

	fColorsList = new ColorView(lvrc, parent);
	
	// create scrollview
	fScrollView = new BScrollView("sv", fColorsList, B_FOLLOW_ALL, 0, false, true);
	AddChild(fScrollView);
	
	lvrc.right += B_V_SCROLL_BAR_WIDTH;
	
	// the real TargetedByScrollView is for some reason called BEFORE the scrollbars
	// are created, so it doesn't work properly, fix up...
	fColorsList->TargetedByScrollView(fScrollView);
	
	// cut/paste instructions
	BStringView *paste;
	BRect rc(lvrc);
	rc.bottom = rc.top - 1;
	rc.top -= 18;
	//rc.bottom = rc.top + 15;
	rc.right = rc.left + (WIDTHOF(rc) / 2);
	AddChild(new BStringView(rc, "", "Right-click: pick up color", 0));
	
	rc.left = rc.right + 1;
	rc.right = lvrc.right;
	paste = new BStringView(rc, "", "Ctrl-click: paste color", 0);
	paste->SetAlignment(B_ALIGN_RIGHT);
	AddChild(paste);
	
	
	// font selector area
	fFontMenu = new BPopUpMenu("fontsel");
	fFontMenu->AddItem(new BMenuItem("System Fixed Font ", NULL));
	
	int x = 10;
	int y = 273;
	rc.Set(x, y, x+20, y+20);
	fFontField = new BMenuField(rc, "fontfld", "", fFontMenu);
	fFontMenu->ItemAt(0)->SetMarked(true);
	AddChild(fFontField);
	
	rc.Set(200, 270, 353, 290);
	fFontSize = new Spinner(rc, "fontsz", "Point size", new BMessage(M_POINTSIZE_CHANGED));
	fFontSize->SetRange(4, 24);
	fFontSize->SetValue(editor.settings.font_size);
	fFontSize->SetTarget(Looper());
	AddChild(fFontSize);
	
	// scheme selector area
	fSchemeMenu = new BPopUpMenu("schemesel");
	UpdateSchemesMenu();

	x = 10;
	y = 10;
	rc.Set(x, y, x+20, y+20);
	fSchemeField = new BMenuField(rc, "schemefld", "", fSchemeMenu);
	AddChild(fSchemeField);
	
	rc.right = lvrc.right;
	rc.left = rc.right - 48;
	rc.OffsetBy(-80, 0);
	BButton *delbtn = new BButton(rc, "", "Del", new BMessage(M_SCHEME_DELETE));
	rc.OffsetBy(-58, 0);
	BButton *newbtn = new BButton(rc, "", "New", new BMessage(M_SCHEME_NEW));
	
	rc.OffsetBy(115, 0);
	rc.right = lvrc.right + 1;
	BButton *defaultsbtn = new BButton(rc, "", "Defaults", new BMessage(M_SCHEME_DEFAULTS));
	
	newbtn->SetTarget(Looper());
	delbtn->SetTarget(Looper());
	defaultsbtn->SetTarget(Looper());
	AddChild(newbtn);
	AddChild(delbtn);
	AddChild(defaultsbtn);
}

void ColorsPreflet::UpdateSchemesMenu()
{
int i, count;

	// delete old
	count = fSchemeMenu->CountItems();
	for(i=0;i<count;i++) delete fSchemeMenu->RemoveItem((int32)0);
	
	// repopulate
	count = CurrentColorScheme.GetNumColorSchemes();
	for(i=0;i<count;i++)
	{
		BMessage *msg = new BMessage(M_SCHEMESEL_CHANGED);
		msg->AddInt32("newindex", i);
		
		BString str(CurrentColorScheme.GetSchemeName(i));
		str.Append(" ");
		
		BMenuItem *item = new BMenuItem(str.String(), msg);
		item->SetTarget(Looper());
		fSchemeMenu->AddItem(item);
	}
	
	fSchemeMenu->ItemAt(CurrentColorScheme.GetLoadedSchemeIndex())->SetMarked(true);
	
	// update schemes menu on Settings menu in main window
	MainWindow->top.menubar->UpdateColorSchemesMenu();
}

void ColorsPreflet::ReloadSettings()
{
	fColorsList->Update();
	
	// revert font size if needed
	if (editor.settings.font_size != fFontSize->Value())
	{
		fFontSize->SetValue(editor.settings.font_size);
		Looper()->PostMessage(M_POINTSIZE_CHANGED);
	}
}

/*
void c------------------------------() {}
*/

// handle reverting of colors
bool ColorsPreflet::HaveSpecialRevert()
{
	return fColorsList->CanRevertColors;
}

void ColorsPreflet::DoSpecialRevert()
{
	fColorsList->RevertColors();
}

// handle saving of colors
void ColorsPreflet::PrefletClosing()
{
	CurrentColorScheme.SaveScheme();
}

/*
void c------------------------------() {}
*/

void ColorsPreflet::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_POINTSIZE_CHANGED:
		{
			int newsize = fFontSize->Value();
			MainWindow->main.editarea->SetFontSize(newsize);
			
			fParent->SettingsChanged();
		}
		break;
		
		case M_SCHEMESEL_CHANGED:
		{			
			int32 newindex;
			msg->FindInt32("newindex", &newindex);
			
			if (CurrentColorScheme.GetLoadedSchemeIndex() == newindex)
				break;
			
			BMenuItem *item = fSchemeMenu->ItemAt(newindex);
			if (item) item->SetMarked(true);
			
			CurrentColorScheme.SaveScheme();
			CurrentColorScheme.LoadScheme(newindex);
			
			editor.curev->FullRedrawView();
			
			fColorsList->ResetRevertBuffer();
			ReloadSettings();
		}
		break;
		
		case M_SCHEME_DELETE:
		{
			int numschemes = ColorScheme::GetNumColorSchemes();
			
			if (numschemes <= 1)
			{
				(new BAlert("", "Cannot delete that color scheme. There must always least one scheme available.", "OK", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
				break;
			}
			
			BString prompt;
			prompt << "Are you sure you want to delete the scheme \"" <<
				CurrentColorScheme.GetSchemeName() << "\"?";
			
			BAlert *alert = new BAlert("", prompt.String(), "No", "Yes");
			if (alert->Go())
			{
				int newindex;
				int curscheme = CurrentColorScheme.GetLoadedSchemeIndex();
				
				// make some other scheme active
				if (curscheme == numschemes - 1)
					newindex = curscheme - 1;
				else
					newindex = curscheme + 1;
				
				BMessage msg(M_SCHEMESEL_CHANGED);
				msg.AddInt32("newindex", newindex);
				MessageReceived(&msg);
				
				// now, delete the scheme
				ColorScheme::DeleteScheme(curscheme);
				
				// fixup loaded index for the deletion
				if (newindex > curscheme)
					CurrentColorScheme.LoadScheme(curscheme);
				
				UpdateSchemesMenu();
				
				fColorsList->ResetRevertBuffer();
				ReloadSettings();
			}
		}
		break;
		
		case M_SCHEME_NEW:
		{
			BString *schemeName;
			BString defaultName(CurrentColorScheme.GetSchemeName());
			defaultName.Append(" Copy");
			
			schemeName = InputBox::Go(MainWindow, "New Scheme", "Name of new scheme:", defaultName.String());
			if (!schemeName) break;
			if (!schemeName->String()[0]) break;
			
			stat("creating new scheme '%s'", schemeName->String());
			
			// duplicate current scheme into the next available index
			CurrentColorScheme.SaveScheme();
			ColorScheme tempscheme = CurrentColorScheme;
			
			int count = ColorScheme::GetNumColorSchemes();
			tempscheme.SetSchemeName(schemeName->String());
			tempscheme.SaveScheme(count);
			
			UpdateSchemesMenu();
			
			// set the new scheme to be active
			CurrentColorScheme.LoadScheme(count);
			BMenuItem *item = fSchemeMenu->ItemAt(count);
			if (item) item->SetMarked(true);
			
			// clean up and we're done
			fColorsList->ResetRevertBuffer();
			delete schemeName;
		}
		break;
		
		case M_SCHEME_DEFAULTS:
		{
			BAlert *alert = new BAlert("", "This will erase all user color-schemes and reset"
										   " colors to the built-in defaults. Is that want you"
										   " want to do?", "Cancel", "Set Defaults", NULL,
										   B_WIDTH_AS_USUAL, B_STOP_ALERT);
			
			if (alert->Go())
			{
				ColorScheme::ResetToDefaults();
				CurrentColorScheme.LoadScheme(1);
				UpdateSchemesMenu();
				
				editor.curev->FullRedrawView();
				
				ReloadSettings();
				
				fColorsList->ResetRevertBuffer();
				fParent->SettingsChanged();
			}
		}
		break;
		
		default:
			MessageView::MessageReceived(msg);
		break;
	}	
}



