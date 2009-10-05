

class BuildHelpPane : public CompilePane
{
public:
	virtual void SetProjectBeingEdited(const char *name);
	virtual void PopupOpening();
	virtual void PopupClosing();

private:
	char fProjectName[1024];
};
