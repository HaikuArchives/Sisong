#ifndef INPUTBOX_H
#define INPUTBOX_H

void DispatchMessage(BMessage *msg, BHandler *target);

class InputBox : public BWindow
{
public:
	InputBox(BWindow *parent, const char *title, const char *prompt, const char *initialValue);	
	virtual void MessageReceived(BMessage *msg);
	virtual bool QuitRequested();
	
	static BString *Go(BWindow *parent, const char *title, const char *prompt, const char *initialValue);
	virtual void DispatchMessage(BMessage *message, BHandler *handler);
	
private:
	BView *backview;
	BTextControl *txtPrompt;
	bool fCanceled;
	bool fBoxDone;
};

#endif // INPUTBOX_H
