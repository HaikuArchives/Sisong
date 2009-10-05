

class CompilePane : public PopupContents
{
public:
	CompilePane();
	~CompilePane();
	void ChangeFontSize(int new_point_size);
	
	void AddLine(const char *text, rgb_color lineColor, bool selectable);
	void RunScript(bool run_result);
	void SetScriptName(const char *fname);
	void Clear();
	void AbortThread();
	virtual void PopupClosing();
	void ItemClicked(int index);
	
	int RunScriptLine(const char *line);
	int Exec(const char *program, char *args[]);

	bool has_errors;	// 1 if compile errors are in pane
	int firstErrorLine;	// line no of first error
	int firstClickableLine;
	int lineCount;

	friend status_t ScriptRunnerThread(void *);
	
protected:
	BListView *ListView;
	BScrollView *ScrollView;
	
	sem_id ListSemaphore;	
	
	int RunScriptInternal(char *fname);
	
	char fScriptName[MAXPATHLEN];
	char fTempScriptFile[MAXPATHLEN];
	bool fRunResult;
	
	struct
	{
		bool please_quit;
		sem_id quit_ack;
	} thread;

	thread_id CompileThread;
};
