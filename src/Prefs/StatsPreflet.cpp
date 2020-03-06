
#include "Preflet.h"
#include "StatsPreflet.h"
#include "StatsPreflet.fdh"

#define M_UPDATE	'UPD1'


StatsPreflet::StatsPreflet(PrefsWindow *parent)
	: Preflet(parent),
	  prefsWin(parent),
	  fTimer(NULL)
{
//BStringView *sv;
BRect leftrc, rightrc;

	leftrc.Set(16, 16, 200, 38);
	rightrc.Set(leftrc.right+1, leftrc.top, Bounds().right-16, leftrc.bottom);

	AddChild(new BStringView(leftrc, "", "Keystrokes Typed:", 0));
	fKeystrokesTyped = new BStringView(rightrc, "", "-", 0);
	AddChild(fKeystrokesTyped);

	leftrc.OffsetBy(0, 28);
	rightrc.OffsetBy(0, 28);

	AddChild(new BStringView(leftrc, "", "Newlines Typed:", 0));
	fCRsTyped = new BStringView(rightrc, "", "-", 0);
	AddChild(fCRsTyped);

	leftrc.OffsetBy(0, 28);
	rightrc.OffsetBy(0, 28);

	AddChild(new BStringView(leftrc, "", "Total Mouse Clicks:", 0));
	fMouseClicks = new BStringView(rightrc, "", "-", 0);
	AddChild(fMouseClicks);

	leftrc.OffsetBy(0, 40);

	AddChild(new BStringView(leftrc, "", "Time spent in foreground:"));

	leftrc.OffsetBy(0, 24);
	leftrc.bottom += 100;
	leftrc.right = rightrc.right;

	BRect textrc(leftrc);
	textrc.OffsetTo(0, 0);
	textrc.InsetBy(8, 8);

	fTimeUsed = new BTextView(leftrc, "", textrc, 0, B_WILL_DRAW | B_PULSE_NEEDED);
	fTimeUsed->MakeSelectable(false);
	fTimeUsed->MakeEditable(false);

	// the scroll view adds a nice border
	AddChild(new BScrollView("", fTimeUsed));
}

StatsPreflet::~StatsPreflet()
{
	PrefletClosing();	// make extra sure timer is dead
}

void StatsPreflet::ReloadSettings()
{
BString num;

	// this is called when our panel is first opened;
	// so take the chance to start the timer.
	if (!fTimer)
		fTimer = new CViewTimer(Looper(), M_UPDATE, 100);

	// these three are a piece of cake
	NiceFmtNumber(editor.stats.keystrokes_typed, &num);
	fKeystrokesTyped->SetText(num.String());

	NiceFmtNumber(editor.stats.CRs_typed, &num);
	fCRsTyped->SetText(num.String());

	NiceFmtNumber(editor.stats.mouse_clicks, &num);
	fMouseClicks->SetText(num.String());

	// refresh the uptime display
	BString uptime;

	AddUptime(&uptime, editor.stats.days_used, "day", "days");
	AddUptime(&uptime, editor.stats.hours_used, "hour", "hours");
	AddUptime(&uptime, editor.stats.minutes_used, "minute", "minutes");

	if (editor.stats.seconds_used)
		AddUptime(&uptime, editor.stats.seconds_used, "second", "seconds");

	uptime.Append(".");

	fTimeUsed->SetText(uptime.String());
}

/*
void c------------------------------() {}
*/

void AddUptime(BString *str, unsigned int value, const char *singular, const char *plural)
{
	if (str->Length() > 0)
		str->Append(", ");

	BString num;
	char text[100];
	NiceFmtNumber(value, &num);
	sprintf(text, "%s %s", num.String(), (value == 1) ? singular : plural);

	str->Append(text);
}

// formats a number with "," separators
void NiceFmtNumber(unsigned int number, BString *out)
{
char sn[80], *ptr;
int counter = 0;

	sprintf(sn, "%d", number);

	out->SetTo("");
	ptr = strchr(sn, '\0');
	while(--ptr >= sn)
	{
		if (counter >= 3)
		{
			out->Prepend(",");
			counter = 0;
		}

		out->Prepend(ptr, 1);
		counter++;
	}

	//if (out->ByteAt(0) == ',')
	//	out->Remove(0, 1);
}



/*
void c------------------------------() {}
*/


void StatsPreflet::PrefletClosing()
{
	if (fTimer)
	{
		delete fTimer;
		fTimer = NULL;
	}
}


/*
void c------------------------------() {}
*/

void StatsPreflet::MessageReceived(BMessage *msg)
{
	if (msg->what == M_UPDATE)
		ReloadSettings();

	Preflet::MessageReceived(msg);
}





