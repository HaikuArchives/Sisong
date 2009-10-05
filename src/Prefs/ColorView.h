
#include "ColorItem.h"

class PrefsWindow;

class ColorView : public MessageView
{
public:
	ColorView(BRect rc, PrefsWindow *prefsWindow);
	~ColorView();
	virtual void TargetedByScrollView(BScrollView *scrollview);
	virtual void MessageReceived(BMessage *msg);
	void Update();

	bool CanRevertColors;
	void RevertColors();
	void ResetRevertBuffer();
	
	// used by ColorItem
	ColorItem *GetPickedUpColor(bool *is_bg);
	void SetPickedUpColor(ColorItem *who, bool is_bg);
	
private:
	friend status_t StartReporterThread(void *cv);
	void ReporterThread();
	bool quit_reporter_thread;
	int32 report_queue_count;
	thread_id reporter_thread;
	
	PrefsWindow *fPrefsWindow;
	
	float fBottom;
	ColorItem *fItems[100];
	
	ColorItem *fPickedUpColor;
	bool fPickedUpIsBG;
	
	ColorScheme RevertBuffer;
};
