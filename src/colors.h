
#define MAX_COLORS		24
enum EditorColor
{
	COLOR_TEXT,
	COLOR_IDENTIFIER,
	COLOR_OPERATOR,
	COLOR_SYSTEM_CONSTANT,
	COLOR_NUMBER,
	COLOR_PP,
	COLOR_CMT_LINE,
	COLOR_CMT_BLOCK,
	COLOR_STRING,
	COLOR_SINGLE_STRING,
	COLOR_BROKEN_STRING,
	COLOR_BRACE,
	COLOR_BRACE_MATCHED,
	COLOR_BRACE_UNMATCHED,
	COLOR_LINENUM,
	COLOR_SELECTION,
	COLOR_TABLINE,
	COLOR_TABLINE_ACTIVE,
	COLOR_CURSOR,
	
	NUM_COLORS,
	COLOR_NORMAL = 0
};

struct ColorDef
{
	rgb_color fg, bg;
	bool isBold;
};

class ColorScheme
{
public:
	ColorScheme();
	ColorScheme(int schemeNo);
	
	static void ResetToDefaults();
	
	void LoadScheme(int schemeNo);
	void SaveScheme();
	void SaveScheme(int schemeNo);
	
	int GetLoadedSchemeIndex();

	static bool SchemeExists(int index);
	static int GetNumColorSchemes();
	const char *GetSchemeName();
	static const char *GetSchemeName(int schemeNo);
	void SetSchemeName(const char *newName);
	
	static const char *GetColorName(int index);
	static int GetNumColors();
	
	static void DeleteScheme(int schemeNo);
	
	friend rgb_color GetEditColor(int colorNum);
	friend rgb_color GetEditFGColor(int colorNum);
	friend rgb_color GetEditBGColor(int colorNum);
	friend bool GetColorBoldState(int colorNum);
	friend uchar GetFGBGApplicable(int colorNum);
	
	void SetFGColor(int colorNum, rgb_color newColor);
	void SetBGColor(int colorNum, rgb_color newColor);
	void SetBoldState(int colorNum, bool newState);

	bool operator==(const ColorScheme &other) const;
	bool operator!=(const ColorScheme &other) const;

private:
	void _FirstTimeInit();
	void _CreateDefaultScheme(const char *name, const rgb_color initdata[], int index);

	char fSchemeName[1024];
	ColorDef _colors[MAX_COLORS];
	int fCurrentSchemeIndex;
};

extern ColorScheme CurrentColorScheme;
