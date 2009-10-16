
#include "Preflet.h"
#include "AboutPreflet.h"

static const char *info_text[] =
{
	B_UTF8_COPYRIGHT"2009 Caitlin Shaw",
	"Released under GNU/GPL version 3",
	"",
	"Release Candidate 2",
	"Revision 14",
	"",
	NULL
};

static const char *disclaimer[] =
{
	"This program is distributed in the hope that it will be useful,",
	"but WITHOUT ANY WARRANTY; without even the implied warranty of",
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the",
	"GNU General Public License for more details.",
	NULL
};

#define M_UPDATE		'UPD1'

static const char *name = "Sisong";
static const rgb_color colors[] =
{
	{ 8, 171, 239 },
	{ 227, 97, 11 },
	{ 33, 242, 23 },
	{ 65, 47, 173 },
	{ 218, 188, 46 },
	{ 245, 85, 110 }
};

#define LOGO_SIZE			22
#define LOGO_SHEAR			92

AboutPreflet::AboutPreflet(PrefsWindow *parent)
	: Preflet(parent),
	  fTimer(NULL)
{
BRect rc;

	int i;

	#define XTRA_SPACING		1
	#define TINT_AMT			1.1f

	BFont font(be_bold_font);
	font.SetSize(LOGO_SIZE);
	font.SetFace(B_BOLD_FACE);
	font.SetShear(LOGO_SHEAR);

	font_height fh;
	font.GetHeight(&fh);
	int ht = (int)(fh.ascent + fh.descent);

	const int y = 16;
	int str_width = (int)font.StringWidth(name) + (strlen(name)*XTRA_SPACING);
	int x = (int)((WIDTHOF(Bounds()) / 2) - (str_width / 2)) + 1;
	char str[2]; str[1] = 0;

	for(i=0;name[i];i++)
	{
		str[0] = name[i];
		int wd = (int)font.StringWidth(str);

		rc.Set(x, y, x+(wd - 1), y+(ht-1));
		LogoChars[i] = new BStringView(rc, "", str, B_FOLLOW_LEFT_RIGHT);

		LogoChars[i]->SetFont(&font);
		LogoChars[i]->SetHighColor(tint_color(colors[i], TINT_AMT));

		AddChild(LogoChars[i]);

		x += wd + XTRA_SPACING;
	}

	MakeStrings(info_text, Bounds().top+51, 30, NULL);

	font = be_bold_font;
	font.SetSize(8);
	font.SetFace(B_BOLD_FACE | B_ITALIC_FACE);
	MakeStrings(disclaimer, 233, 16, &font);
}

void AboutPreflet::Animate()
{
const int nFrames = strlen(name);

	if (++rotate_frame >= nFrames)
		rotate_frame = 0;

	int ri = -rotate_frame;
	if (ri) ri += nFrames;
	for(int i=0;i<nFrames;i++)
	{
		if (LogoChars[i])
		{
			LogoChars[i]->SetHighColor(tint_color(colors[ri], TINT_AMT));
			LogoChars[i]->Invalidate();
		}

		if (++ri >= nFrames) ri = 0;
	}
}

/*
void c------------------------------() {}
*/

void AboutPreflet::ReloadSettings()
{
	if (!fTimer)
		fTimer = new CViewTimer(Looper(), M_UPDATE, 125);
}

void AboutPreflet::PrefletClosing()
{
	if (fTimer)
	{
		delete fTimer;
		fTimer = NULL;
	}
}

AboutPreflet::~AboutPreflet()
{
	PrefletClosing();
}


void AboutPreflet::MessageReceived(BMessage *msg)
{
	if (msg->what == M_UPDATE)
		Animate();

	Preflet::MessageReceived(msg);
}


/*
void c------------------------------() {}
*/

void AboutPreflet::MakeStrings(const char *list[], int y_start, int spacing, BFont *font)
{
BRect rc;
BStringView *sv;

	rc = Bounds();
	rc.top = y_start;
	for(int i=0;list[i];i++)
	{
		//if (font && i == 2)
		//	font->SetFace(B_BOLD_FACE | B_ITALIC_FACE);

		rc.bottom = rc.top + spacing;

		if (list[i][0])
		{
			sv = new BStringView(rc, "info", list[i], B_FOLLOW_LEFT_RIGHT);
			sv->SetAlignment(B_ALIGN_CENTER);
			if (font) sv->SetFont(font);

			AddChild(sv);
		}

		rc.top = rc.bottom+1;
	}
}



