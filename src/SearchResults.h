

typedef struct stSearchResult
{
	int lineNumber;			// line number hit was found on
	char *filename;			// file name of document hit was found in
	char *lineString;		// contents of line containing hit
	int x;					// exact position within line of start of hit
};


class SearchResultsPane : public PopupContents
{
public:
	SearchResultsPane();
	~SearchResultsPane();

	void AddResult(stSearchResult *result, bool include_filename=true);
	void Clear();
	void ItemClicked(int index);
	
	void SetSearchTerm(const char *newTerm);
	void GetCaptionForTitlebar(BString *title);
	void ChangeFontSize(int new_point_size);
	
	virtual void PopupOpening();

private:
	BView *topview;
	BListView *OnscreenList;		// list of BStringItems shown in listview
	CList ResultsList;				// list of stSearchResults
	BScrollView *ScrollView;
	
	char *search_term;
};
