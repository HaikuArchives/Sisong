
class MainMenuBar : public BView
{
public:
	MainMenuBar(BRect frame, uint32 resizingMode);
	void UpdateColorSchemesMenu();
	void SetMarkedColorScheme(int index);
	
	BMenuBar *bar;
	
	BMenu *ProjectMenu;
	BMenu *SettingsMenu;
	BMenuItem *ShowConsoleItem;
	BLocker ChangeMenusLock;
};
