
#include "Preflet.h"
#include "ColorItem.h"
#include "ColorView.h"
#include "../ColorPicker/ColorPicker.h"
#include "ColorView.fdh"

#define COLOR_ITEM_HEIGHT		32

#define M_FG_CHANGED			'FGch'
#define M_BG_CHANGED			'BGch'
#define M_DO_UPDATE				'UPD!'


ColorView::ColorView(BRect frame, PrefsWindow *prefsWindow)
	: MessageView(frame, "colorslist", 0, 0),
	  fPrefsWindow(prefsWindow),
	  CanRevertColors(false),
	  fPickedUpColor(NULL)
{
int i, count;
BRect rc;

	memset(fItems, 0, sizeof(fItems));

	// backup current color scheme to revert buffer
	RevertBuffer = CurrentColorScheme;

	rc = Bounds();
	rc.bottom = COLOR_ITEM_HEIGHT - 1;
	count = CurrentColorScheme.GetNumColors();
	
	for(i=0;i<count;i++)
	{
		const char *name = CurrentColorScheme.GetColorName(i);
		const rgb_color fg = GetEditFGColor(i);
		const rgb_color bg = GetEditBGColor(i);
		const bool boldState = GetColorBoldState(i);
		const bool UsesBothColors = GetColorUsesBothColors(i);
		
		fItems[i] = new ColorItem(rc, name, fg, bg, boldState, UsesBothColors, i, this);
		AddChild(fItems[i]);
		
		fBottom = rc.bottom + 1;
		rc.OffsetBy(0, COLOR_ITEM_HEIGHT);
	}
	
	// start reporter thread (prevents huge message buildup when user slides
	// controls around in Color Picker, by pausing just a moment when the
	// update message comes in to see if another will arrive, before redrawing
	// the document.
	report_queue_count = 0;
	quit_reporter_thread = false;
	
	reporter_thread = spawn_thread(StartReporterThread, "ColorReporter Thread", B_NORMAL_PRIORITY, this);
	resume_thread(reporter_thread);
}

ColorView::~ColorView()
{
	quit_reporter_thread = true;
	wait_for_thread(reporter_thread, NULL);
}


// erase the revert buffer and set it to the current state of the view
void ColorView::ResetRevertBuffer()
{
	RevertBuffer = CurrentColorScheme;
	CanRevertColors = false;
}


void ColorView::Update()
{
	int i, count = CurrentColorScheme.GetNumColors();
	for(i=0;i<count;i++)
	{
		const rgb_color fg = GetEditFGColor(i);
		const rgb_color bg = GetEditBGColor(i);
		const bool boldState = GetColorBoldState(i);
		
		fItems[i]->UpdateData(fg, bg, boldState);
	}
}

void ColorView::RevertColors()
{
	if (CanRevertColors)
	{
		CanRevertColors = false;
		CurrentColorScheme = RevertBuffer;
		Update();
	}
}


void ColorView::TargetedByScrollView(BScrollView *scrollview)
{
BScrollBar *bar = scrollview->ScrollBar(B_VERTICAL);

	if (bar)
	{
		bar->SetRange(0, (int)(fBottom - HEIGHTOF(Bounds())));
		bar->SetSteps(COLOR_ITEM_HEIGHT/2, HEIGHTOF(Bounds()) - (COLOR_ITEM_HEIGHT/2));
	}
}

void ColorView::MessageReceived(BMessage *msg)
{
	switch(msg->what)
	{
		case M_CI_FG_CLICKED:
		case M_CI_BG_CLICKED:
		{
			bool is_bg = (msg->what==M_CI_BG_CLICKED);
			
			int32 index;
			msg->FindInt32("index", &index);
			rgb_color initial_color;
			initial_color = is_bg ? GetEditBGColor(index) : GetEditFGColor(index);
			
			BMessage *report = new BMessage(*msg);	// copy "index" field over
			report->what = is_bg ? M_BG_CHANGED : M_FG_CHANGED;
			
			new ColorPickerWindow(report, Looper(), initial_color);
		}
		break;
		
		case M_FG_CHANGED:
		case M_BG_CHANGED:
		{
			rgb_color newColor;
			int32 colorindex;
			
			// we are only casting from uint8 to int8 so this is safe
			msg->FindInt8("red", (int8 *)&newColor.red);
			msg->FindInt8("green", (int8 *)&newColor.green);
			msg->FindInt8("blue", (int8 *)&newColor.blue);
			msg->FindInt32("index", &colorindex);
			
			if (msg->what == M_FG_CHANGED)
				CurrentColorScheme.SetFGColor(colorindex, newColor);
			else
				CurrentColorScheme.SetBGColor(colorindex, newColor);
			
			atomic_add(&report_queue_count, 1);			
		}
		break;
		
		case M_CI_CONTROL_CLICK_FG:
		case M_CI_CONTROL_CLICK_BG:
		{
			// get pointer to currently "picked up" color
			bool is_bg;
			ColorItem *pickupptr = GetPickedUpColor(&is_bg);
			
			// nothing picked up? reprocess as normal click
			if (!pickupptr)
			{
				msg->what = (msg->what == M_CI_CONTROL_CLICK_FG) ? \
								M_CI_FG_CLICKED : M_CI_BG_CLICKED;
				MessageReceived(msg);
				return;
			}
			
			// convert the picked-up color to a color index
			int pickupindex;
			for(pickupindex=0;pickupindex<50;pickupindex++)
			{
				if (fItems[pickupindex] == pickupptr)
					break;
			}
			
			// get the current value of the picked-up color
			rgb_color pickupcolor;
			pickupcolor = is_bg ? GetEditBGColor(pickupindex) : \
								GetEditFGColor(pickupindex);
			
			// set the clicked-on color to match
			int32 index;
			msg->FindInt32("index", &index);
			
			if (msg->what == M_CI_CONTROL_CLICK_FG)
				CurrentColorScheme.SetFGColor(index, pickupcolor);
			else
				CurrentColorScheme.SetBGColor(index, pickupcolor);
			
			// update immediately
			msg->what = M_DO_UPDATE;
			MessageReceived(msg);
		}
		break;
		
		case M_CI_BOLD_CLICKED:
		{
			int32 index;
			bool value;
			
			msg->FindInt32("index", &index);
			value = fItems[index]->chkBold->Value();
			
			CurrentColorScheme.SetBoldState(index, value);
			
			CanRevertColors = (CurrentColorScheme != RevertBuffer);
			fPrefsWindow->SettingsChanged();
		}
		break;
		
		// from reporter thread below
		case M_DO_UPDATE:
		{
			CanRevertColors = (CurrentColorScheme != RevertBuffer);
			fPrefsWindow->SettingsChanged();
			Update();
		}
		break;
		
		default:
			BView::MessageReceived(msg);
		break;
	}
}

/*
void c------------------------------() {}
*/

// used by ColorItem
ColorItem *ColorView::GetPickedUpColor(bool *is_bg)
{
	if (is_bg) *is_bg = fPickedUpIsBG;
	return fPickedUpColor;
}

void ColorView::SetPickedUpColor(ColorItem *who, bool is_bg)
{
	ColorItem *OldPickedUpColor = fPickedUpColor;
	
	// deselect if click twice on same item
	if (who == fPickedUpColor && \
		is_bg == fPickedUpIsBG)
	{
		who = NULL;
	}
	
	fPickedUpColor = who;
	fPickedUpIsBG = is_bg;	
	
	if (OldPickedUpColor)
		OldPickedUpColor->Invalidate();

	if (fPickedUpColor)
		fPickedUpColor->Invalidate();
}

/*
void c------------------------------() {}
*/

status_t StartReporterThread(void *cv)
{
	((ColorView *)cv)->ReporterThread();
	return B_OK;
}

/* limits possible frequency of doc redraws to prevent messages building up */
void ColorView::ReporterThread()
{
int old_value = report_queue_count;
int ticker = 0;

	while(!quit_reporter_thread)
	{
		snooze(10 * 1000);
		
		if (!ticker)
		{
			if (report_queue_count != old_value)
			{
				ticker = 5;
			}
		}
		else if (!--ticker)
		{
			Looper()->PostMessage(M_DO_UPDATE);
			old_value = report_queue_count;
		}
	}
}







