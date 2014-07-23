#ifndef MAIN_H
#define MAIN_H

// the application class
class EApp : public BApplication
{
public:
	EApp();
	~EApp();
	
	void RefsReceived(BMessage *message);
};

void LoadEditorSettings();
void SaveEditorSettings();
void LockWindow();
void UnlockWindow();

#endif // MAIN_H
