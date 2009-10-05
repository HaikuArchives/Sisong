
struct FLResult
{
	BStringItem *text;
	int lineNumber;
};

class CFunctionList : public BView
{
public:
	CFunctionList(BRect frame, uint32 resizingMode);
	~CFunctionList();
	void SetHeight(int height);
	
	// scan the entire document for changes
	void ScanAll();
	void ScanIfNeeded();
	
	// scan the text between the given lines for changes
	void Scan(int startline, int endline);
	
	// scan the given line for changes
	void ScanLine(clLine *line, int y);
	
	// updates the selection highlight which shows which function
	// is currently in view.
	void UpdateSelectionHighlight();
	
	// called by main window when it receives an invocation message
	// from the scrollview.
	void JumpToIndex(int index);
	
	// called by the edit_actions anytime the document is about to be modified.
	void ResetTimer();
	// called every 100ms.
	void TimerTick();
	
	
private:
	BListView *list;
	BScrollView *sv;
	
	BList NewResults;
	BList LineNumbers;
	
	int Timer;
	bool UpToDate;
	
	void ApplyNewResults();
};

