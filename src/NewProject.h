
class DirMenu;
class FileView;

class NewProjectWindow : public SubWindow
{
public:
	NewProjectWindow();
	~NewProjectWindow();
	
	virtual void MessageReceived(BMessage *msg);
	
private:
	void HandleOKButton();
	
	BTextControl *txtProjectName;
	DirMenu *dirmenu;
	FileView *fileview;
	
	BList files_to_open;
};
