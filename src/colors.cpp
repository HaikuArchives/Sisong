
#include "editor.h"
#include <ctype.h>
#include "../common/basics.h"

#include "colors.fdh"

ColorScheme CurrentColorScheme;
const int kCurrentSchemeVersion = 2;


static const char *color_names[] =
{
	"Text",
	"Identifier",
	"Operator",
	"System Constant",
	"Number",
	"Preprocessor",
	"Line Comment",
	"Block Comment",
	"String (double-quoted)",
	"String (single-quoted)",
	"String (broken)",
	"Brace",
	"Brace (being matched)",
	"Brace (broken)",
	"Line Numbers",
	"Selection",
	"Tab line",
	"Tab line (active)",
	"Cursor"
};

static const rgb_color scheme_Eggplant[] =
{
	{ 0xe8, 0xe8, 0xe8 }, { 0x22, 0x03, 0x40 },		// text
	{ 0xc3, 0xcf, 0x5c }, { 0x22, 0x03, 0x40 },		// id
	{ 0xe8, 0xe8, 0xe8 }, { 0x22, 0x03, 0x40 },		// operator
	{ 0xd6, 0x40, 0x59 }, { 0x22, 0x03, 0x40 },		// system constant
	{ 0xa7, 0xec, 0xbb }, { 0x22, 0x03, 0x40 },		// number
	{ 0xe7, 0x8d, 0x58 }, { 0x22, 0x03, 0x40 },		// pp
	{ 0x1b, 0x8c, 0xa0 }, { 0x22, 0x03, 0x40 },		// line comment
	{ 0x1b, 0x8c, 0xa0 }, { 0x22, 0x03, 0x40 },		// block comment
	{ 0xa0, 0xa0, 0xa0 }, { 0x22, 0x03, 0x40 },		// string
	{ 0x80, 0x80, 0x80 }, { 0x22, 0x03, 0x40 },		// single string
	{ 0xff, 0x80, 0x80 }, { 0x22, 0x03, 0x40 },		// broken string
	{ 0xe8, 0xe8, 0xe8 }, { 0x22, 0x03, 0x40 },		// brace
	{ 0xff, 0xff, 0x80 }, { 0x40, 0x00, 0x80 },		// matched brace
	{ 0xff, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },		// broken brace
	{ 0xff, 0xff, 0x80 }, { 0x1a, 0x40, 0x00 },		// line numbers	
	{ 0x40, 0x00, 0x80 }, { 0xff, 0xff, 0xff }, 	// selection
	{ 0xc0, 0xc0, 0xc0 }, { 0xff, 0xff, 0xff },		// tabline
	{ 0xff, 0xff, 0x80 }, { 0x40, 0x00, 0x80 },		// tabline (active)
	{ 0xff, 0xff, 0x80 }, { 0xee, 0xee, 0xee }		// cursor
};

static const rgb_color scheme_Brown[] =
{
	{ 0xff, 0xbb, 0xbb }, { 0x3d, 0x00, 0x00 },		// text
	{ 0xff, 0xff, 0x80 }, { 0x40, 0x00, 0x80 },		// id
	{ 0xff, 0xbb, 0xbb }, { 0x3d, 0x00, 0x00 },		// operator
	{ 0xd6, 0x40, 0x59 }, { 0x3d, 0x00, 0x00 },		// sys constant
	{ 0xff, 0x80, 0x80 }, { 0x3d, 0x00, 0x00 },		// number
	{ 0xe7, 0x8d, 0x58 }, { 0x3d, 0x00, 0x00 },		// pp
	{ 0x80, 0x80, 0x40 }, { 0x3d, 0x00, 0x00 },		// line comment
	{ 0x80, 0x80, 0x40 }, { 0x3d, 0x00, 0x00 },		// block comment
	{ 0xa0, 0xa0, 0xa0 }, { 0x3d, 0x00, 0x00 },		// string
	{ 0x80, 0x80, 0x80 }, { 0x3d, 0x00, 0x00 },		// single string
	{ 0xff, 0x80, 0x80 }, { 0x3d, 0x00, 0x00 },		// broken string
	{ 0xff, 0xbb, 0xbb }, { 0x3d, 0x00, 0x00 },		// brace
	{ 0xff, 0xff, 0x80 }, { 0x40, 0x00, 0x80 },		// matched brace
	{ 0xff, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },		// broken brace
	{ 0xff, 0xff, 0x80 }, { 0x80, 0x40, 0x00 },		// line numbers	
	{ 0x40, 0x00, 0x80 }, { 0xff, 0xff, 0xff }, 	// selection	
	{ 0xc0, 0xc0, 0xc0 }, { 0xff, 0xff, 0xff },		// tabline
	{ 0xff, 0xff, 0x80 }, { 0x40, 0x00, 0x80 },		// tabline (active)
	{ 0xff, 0xff, 0x80 }, { 0xee, 0xee, 0xee }		// cursor
};

static const rgb_color scheme_Lightbulb[] =
{
	{ 0x0b, 0x0b, 0x0b }, { 0xff, 0xff, 0xff },		// text
	{ 0x39, 0x74, 0x79 }, { 0xff, 0xff, 0xff },		// id
	{ 0x44, 0x8a, 0x00 }, { 0xff, 0xff, 0xff },		// operator
	{ 0xfb, 0x00, 0x00 }, { 0xff, 0xff, 0xff },		// sys constant
	{ 0x8e, 0x2a, 0x2a }, { 0xff, 0xff, 0xff },		// number
	{ 0x39, 0x74, 0x79 }, { 0xff, 0xff, 0xff },		// pp
	{ 0xa1, 0x64, 0x0e }, { 0xff, 0xff, 0xff },		// line comment
	{ 0xa1, 0x64, 0x0e }, { 0xff, 0xff, 0xff },		// block comment
	{ 0xa0, 0xa0, 0xa0 }, { 0xff, 0xff, 0xff },		// string
	{ 0x80, 0x80, 0x80 }, { 0xff, 0xff, 0xff },		// single string
	{ 0xff, 0x80, 0x80 }, { 0xff, 0xff, 0xff },		// broken string
	{ 0x39, 0x74, 0x79 }, { 0xff, 0xff, 0xff },		// brace
	{ 0x39, 0x74, 0x79 }, { 0xff, 0xec, 0x7c },		// matched brace
	{ 0xff, 0x00, 0x00 }, { 0x00, 0x00, 0x00 },		// broken brace
	{ 0x00, 0x00, 0x00 }, { 0xef, 0xef, 0xef },		// line numbers	
	{ 0xff, 0xec, 0x7c }, { 0xff, 0xff, 0xff }, 	// selection	
	{ 0x40, 0x40, 0x40 }, { 0xff, 0xff, 0xff },		// tabline
	{ 0x00, 0x00, 0xff }, { 0xff, 0xff, 0xff },		// tabline (active)
	{ 0x00, 0x00, 0x00 }, { 0xff, 0xff, 0xff }		// cursor
};

static const rgb_color scheme_Console[] =
{
	{ 192, 192, 192 },    { 0, 0, 0 },		// text
	{ 128, 192, 128 }, { 0, 0, 0 },		// id
	{ 192, 192, 192 },    { 0, 0, 0 },		// operator
	{ 0xff, 0xec, 0x7c }, { 0, 0, 0 },		// sys constant
	{ 255, 255, 255 }, { 0, 0, 0 },		// number
	{ 0xff, 0xec, 0x7c }, { 0, 0, 0 },		// pp
	{ 0xc1, 0x74, 0x0e }, { 0, 0, 0 },		// line comment
	{ 0xc1, 0x74, 0x0e }, { 0, 0, 0 },		// block comment
	{ 0xa0, 0xa0, 0xa0 }, { 0, 0, 0 },		// string
	{ 0x80, 0x80, 0x80 }, { 0, 0, 0 },		// single string
	{ 0xff, 0x80, 0x80 }, { 0, 0, 0 },		// broken string
	{ 255, 255, 255 }, { 0, 0, 0 },		// brace
	{ 0xff, 0xec, 0x7c }, { 40, 40, 100  },		// matched brace
	{ 0, 0, 0 }, { 255, 0, 0 },		// broken brace
	{ 0xff, 0xff, 0xff }, { 0, 0, 0 },		// line numbers	
	{ 0x7d, 0x00, 0x00 }, { 0xff, 0xff, 0xff }, 	// selection	
	{ 0xe0, 0xe0, 0xe0 }, { 0, 0, 0 },	// tabline
	{ 0xff, 0xec, 0x7c }, { 40, 40, 100  },	// tabline (active)
	{ 255, 255, 255 }, { 0, 0, 0 },		// cursor
};

static const rgb_color scheme_Hallow[] =
{
	{ 0xe4, 0x62, 0x31 }, { 0x11, 0x00, 0x00 },		// text
	{ 0x3f, 0xff, 0x80 }, { 0x11, 0x00, 0x00 },		// id
	{ 0xdf, 0xdf, 0xdf }, { 0x11, 0x00, 0x00 },		// operator
	{ 0xd6, 0xff, 0x59 }, { 0x11, 0x00, 0x00 },		// sys constant
	{ 0xc2, 0xc2, 0xc2 }, { 0x11, 0x00, 0x00 },		// number
	{ 0xe7, 0x8d, 0x58 }, { 0x11, 0x00, 0x00 },		// pp
	{ 0xff, 0xc3, 0x2f }, { 0x11, 0x00, 0x00 },		// line comment
	{ 0xff, 0xc3, 0x2f }, { 0x39, 0x14, 0x00 },		// block comment
	{ 0xa0, 0xa0, 0xa0 }, { 0x11, 0x00, 0x00 },		// string
	{ 0x80, 0x80, 0x80 }, { 0x11, 0x00, 0x00 },		// single string
	{ 0xff, 0x80, 0x80 }, { 0x11, 0x00, 0x00 },		// broken string
	{ 0xdf, 0xdf, 0xdf }, { 0x11, 0x00, 0x00 },		// brace
	{ 0xff, 0xff, 0xff }, { 0x11, 0x6a, 0x37 },		// matched brace
	{ 0xdf, 0xdf, 0xdf }, { 0x92, 0x00, 0x00 },		// broken brace
	{ 0xe3, 0x66, 0x27 }, { 0x00, 0x05, 0x00 },		// line numbers	
	{ 0x27, 0x27, 0x27 }, { 0xff, 0xff, 0xff }, 	// selection	
	{ 0xc0, 0xc0, 0xc0 }, { 0x11, 0x00, 0x00 },		// tabline
	{ 0x5f, 0xff, 0x60 }, { 0x11, 0x00, 0x00 },		// tabline (active)
	{ 0xdf, 0xdf, 0xdf }, { 0xee, 0xee, 0xee }		// cursor
};
/*
void c------------------------------() {}
*/

// construct and initilize all entries to a failsafe monochrome scheme
ColorScheme::ColorScheme()
{
	for(int i=0;i<MAX_COLORS;i++)
	{
		_colors[i].fg.red = _colors[i].fg.green = _colors[i].fg.blue = 255;
		_colors[i].bg.red = _colors[i].bg.green = _colors[i].bg.blue = 0;
		_colors[i].isBold = false;
	}
	
	fSchemeName[0] = 0;
	fCurrentSchemeIndex = -1;
}

// construct and initilize entries from a colorset
ColorScheme::ColorScheme(int schemeNo)
{
	LoadScheme(schemeNo);
}

bool ColorScheme::operator==(const ColorScheme &other) const
{
int i;

	for(i=0;i<NUM_COLORS;i++)
	{
		if (_colors[i].fg != other._colors[i].fg) return false;
		if (_colors[i].bg != other._colors[i].bg) return false;
		if (_colors[i].isBold != other._colors[i].isBold) return false;
	}
	
	return true;
}

bool ColorScheme::operator!=(const ColorScheme &other) const
{
int i;

	for(i=0;i<NUM_COLORS;i++)
	{
		if (_colors[i].fg != other._colors[i].fg) return true;
		if (_colors[i].bg != other._colors[i].bg) return true;
		if (_colors[i].isBold != other._colors[i].isBold) return true;
	}
	
	return false;
}

/*
void c------------------------------() {}
*/

// check if this is the first time the editor is started, and if so,
// create the default color schemes from the built in settings.
void ColorScheme::_FirstTimeInit()
{
	if (!SchemeExists(0) || \
		settings->GetInt("ColorSchemeVersion", 0) != kCurrentSchemeVersion)
	{
		stat("! ColorScheme::_FirstTimeInit: setting up default colorschemes");
		settings->SetInt("ColorSchemeVersion", kCurrentSchemeVersion);
		ResetToDefaults();
	}
}

void ColorScheme::ResetToDefaults()
{
	while(GetNumColorSchemes() > 4)
		DeleteScheme(0);
	
	ColorScheme temp;
	temp._CreateDefaultScheme("Eggplant Sea", scheme_Eggplant, 0);
	temp._CreateDefaultScheme("Maroon Mountain", scheme_Brown, 1);
	temp._CreateDefaultScheme("Lightbulb City", scheme_Lightbulb, 2);
	temp._CreateDefaultScheme("Console Caves", scheme_Console, 3);
	temp._CreateDefaultScheme("Hallows Eve", scheme_Hallow, 4);
	
	if (MainWindow)
		MainWindow->top.menubar->UpdateColorSchemesMenu();
}

// creates a scheme named "name" from "initdata[]" array, then writes it to scheme index "index".
void ColorScheme::_CreateDefaultScheme(const char *name, const rgb_color initdata[], int index)
{
int i, j;

	// load ourselves up with the built in defaults
	for(i=j=0;i<NUM_COLORS;i++)
	{
		_colors[i].fg = initdata[j];
		_colors[i].bg = initdata[j+1];
		_colors[i].isBold = false;
		j += 2;
	}
	
	_colors[COLOR_BRACE_MATCHED].isBold = true;
	_colors[COLOR_BRACE_UNMATCHED].isBold = true;
	
	strcpy(fSchemeName, name);
	
	// now write ourselves to the config file
	SaveScheme(index);
}

/*
void c------------------------------() {}
*/

// load a scheme by it's index.
void ColorScheme::LoadScheme(int schemeNo)
{
char cfg_key[80];
int i;

	_FirstTimeInit();

	sprintf(cfg_key, "scheme%d_name", schemeNo);
	const char *name = settings->GetString(cfg_key, fSchemeName);
	maxcpy(fSchemeName, name, sizeof(fSchemeName) - 1);

	stat("Loading scheme %d: '%s'", schemeNo, fSchemeName);
	
	for(i=0;i<NUM_COLORS;i++)
	{
		GetColorConfigKey(schemeNo, i, cfg_key, "fg");
		_colors[i].fg = ColorFromHex(settings->GetString(cfg_key, ""));
		
		GetColorConfigKey(schemeNo, i, cfg_key, "bg");
		_colors[i].bg = ColorFromHex(settings->GetString(cfg_key, ""));
		
		GetColorConfigKey(schemeNo, i, cfg_key, "bold");
		_colors[i].isBold = settings->GetInt(cfg_key, 0);
	}

	fCurrentSchemeIndex = schemeNo;
	
	// update menu checkmark when changing active color scheme
	if (this == &CurrentColorScheme)
	{
		MainWindow->top.menubar->SetMarkedColorScheme(schemeNo);
	}
}


static void GetColorConfigKey(int schemeNo, int colorIndex, char *buffer, const char *suffix)
{
int i, j;
const char *name;
char *ptr;

	sprintf(buffer, "scheme%d_", schemeNo);
	
	name = ColorScheme::GetColorName(colorIndex);
	ptr = strchr(buffer, '\0');
	
	for(i=j=0;name[i];i++)
	{
		if (name[i] == '(' || name[i] == ')') continue;
		
		char ch = name[i];
		if (ch == ' ') ch = '_';
		else ch = tolower(ch);
		
		*(ptr++) = ch;
	}
	
	*(ptr++) = '_';
	strcpy(ptr, suffix);
}

// converts a hex string in format "#000042" into an rgb_color.
static rgb_color ColorFromHex(const char *hcol)
{
rgb_color result;

	if (*hcol=='#') hcol++;
	
	if (strlen(hcol) < 6)
	{
		result.red = result.blue = 255;
		result.green = 0;
		return result;
	}
	
	result.red = ReadHexByte(hcol);
	result.green = ReadHexByte(hcol + 2);
	result.blue = ReadHexByte(hcol + 4);
	
	return result;
}

// converts a hex byte in form "A7" into a char
uchar ReadHexByte(const char *hbyte)
{
uchar hi, lo;

	hi = ReadHexDigit(*hbyte);
	lo = ReadHexDigit(*(hbyte + 1));
	
	return (hi << 4) | lo;
}

// reads a single hex digit and returns a value from 0-15
uchar ReadHexDigit(const char digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return (digit - '0');
	}
	else if (digit >= 'A' && digit <= 'F')
	{
		return (digit - 'A') + 10;
	}
	else if (digit >= 'a' && digit <= 'f')
	{
		return (digit - 'a') + 10;
	}
	else
	{
		return 0;
	}
}

/*
void c------------------------------() {}
*/

// save the colors in the scheme, updating the color scheme they were loaded from.
void ColorScheme::SaveScheme()
{
	if (fCurrentSchemeIndex != -1)
		SaveScheme(fCurrentSchemeIndex);
}

// save the colors in the scheme into the config file as color scheme #"schemeNo".
// the current color scheme number is not changed.
void ColorScheme::SaveScheme(int schemeNo)
{
char cfg_key[80];
char hcol[16];
int i;

	stat("saving scheme %d: '%s'", schemeNo, fSchemeName);
	
	sprintf(cfg_key, "scheme%d_name", schemeNo);
	settings->SetString(cfg_key, fSchemeName);
	
	for(i=0;i<NUM_COLORS;i++)
	{
		GetColorConfigKey(schemeNo, i, cfg_key, "fg");
		ColorToHex(_colors[i].fg, hcol);
		settings->SetString(cfg_key, hcol);
		
		GetColorConfigKey(schemeNo, i, cfg_key, "bg");
		ColorToHex(_colors[i].bg, hcol);
		settings->SetString(cfg_key, hcol);
	}
	
	for(i=0;i<NUM_COLORS;i++)
	{
		GetColorConfigKey(schemeNo, i, cfg_key, "bold");
		settings->SetInt(cfg_key, _colors[i].isBold);
	}	
}

static void ColorToHex(rgb_color color, char *buffer)
{
	sprintf(buffer, "#%02x%02x%02x", color.red, color.green, color.blue);
}

/*
void c------------------------------() {}
*/

// returns whether or not the given colorscheme index is present
bool ColorScheme::SchemeExists(int index)
{
char cfg_key[80];
const char *str;

	GetColorConfigKey(index, 0, cfg_key, "fg");
	str = settings->GetString(cfg_key, NULL);
	
	if (str)
		return true;
	else
		return false;
}


// counts the current number of color schemes currently available
int ColorScheme::GetNumColorSchemes()
{
int i;

	for(i=0;;i++)
	{
		if (!SchemeExists(i))
			return i;
	}
}


// returns the number of colors in each scheme
int ColorScheme::GetNumColors()
{
	return NUM_COLORS;
}


// returns the name of the currently loaded color scheme.
const char *ColorScheme::GetSchemeName()
{
	return fSchemeName;
}


// returns the name of the color scheme at the given index, without loading it.
const char *ColorScheme::GetSchemeName(int schemeNo)
{
	if (SchemeExists(schemeNo))
	{
		char cfg_key[80];
		sprintf(cfg_key, "scheme%d_name", schemeNo);
		
		return settings->GetString(cfg_key, "untitled colorscheme");
	}
	else
	{
		return "invalid colorscheme";
	}
}

/*
void c------------------------------() {}
*/

// change the name of the currently-loaded scheme
void ColorScheme::SetSchemeName(const char *newName)
{
	maxcpy(fSchemeName, newName, sizeof(fSchemeName) - 1);
}


/*
void c------------------------------() {}
*/

// returns the index of the scheme currently loaded into the class,
// or -1 if no scheme has ever been loaded.
int ColorScheme::GetLoadedSchemeIndex()
{
	return fCurrentSchemeIndex;
}


// returns the name of a color given it's index.
const char *ColorScheme::GetColorName(int index)
{
	if (index >= 0 && index < NUM_COLORS)
		return color_names[index];
	else
		return "invalid color index";
}

/*
void c------------------------------() {}
*/

// delete a color scheme from the settings file
void ColorScheme::DeleteScheme(int schemeNo)
{
ColorScheme tempscheme;
int i, lastScheme, numSchemes;

	numSchemes = GetNumColorSchemes();
	
	if (schemeNo >= numSchemes || !SchemeExists(schemeNo))
		return;
	
	// delete the scheme by loading each scheme after it and moving them down a slot
	for(i=schemeNo;i<numSchemes-1;i++)
	{
		tempscheme.LoadScheme(i+1);
		tempscheme.SaveScheme(i);
	}
	
	// now the last scheme is a duplicate, get rid of it
	lastScheme = numSchemes - 1;

	for(i=0;i<NUM_COLORS;i++)
	{
		char cfg_key[80];
		
		GetColorConfigKey(lastScheme, i, cfg_key, "fg");
		settings->RemoveName(cfg_key);
		
		GetColorConfigKey(lastScheme, i, cfg_key, "bg");
		settings->RemoveName(cfg_key);
		
		GetColorConfigKey(lastScheme, i, cfg_key, "bold");
		settings->RemoveName(cfg_key);
	}
}

/*
void c------------------------------() {}
*/

rgb_color GetEditFGColor(int colorNum)	{ return CurrentColorScheme._colors[colorNum].fg; }
rgb_color GetEditBGColor(int colorNum)	{ return CurrentColorScheme._colors[colorNum].bg; }
bool GetColorBoldState(int colorNum)	{ return CurrentColorScheme._colors[colorNum].isBold; }

// identical to GetEditFGColor:
//	this is a clearer name for those colors such as selection that do not
//  have both an fg and bg and only have one color.
rgb_color GetEditColor(int colorNum)	{ return CurrentColorScheme._colors[colorNum].fg; }

// returns true on colors that use both fg and bg,
// and false on colors that use only the fg.
bool GetColorUsesBothColors(int colorNum)
{
	switch(colorNum)
	{
		case COLOR_SELECTION:
		case COLOR_TABLINE:
		case COLOR_TABLINE_ACTIVE:
		case COLOR_CURSOR:
			return false;
		
		default:
			return true;
	}
}

/*
void c------------------------------() {}
*/

void ColorScheme::SetFGColor(int index, rgb_color new_color)
{
	if (index >= 0 && index < MAX_COLORS)
		_colors[index].fg = new_color;
}

void ColorScheme::SetBGColor(int index, rgb_color new_color)
{
	if (index >= 0 && index < MAX_COLORS)
		_colors[index].bg = new_color;
}

void ColorScheme::SetBoldState(int index, bool boldState)
{
	if (index >= 0 && index < MAX_COLORS)
		_colors[index].isBold = boldState;
}






