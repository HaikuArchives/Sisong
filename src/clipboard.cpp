
#include "editor.h"
#include <Clipboard.h>
#include "clipboard.fdh"

// copy the selected text to the clipboard.
// returns the number of lines copied.
void EditView::CopySelection()
{
int x1, y1, x2, y2;
BString *contents;

	if (!this->selection.present) return;
	GetSelectionExtents(this, &x1, &y1, &x2, &y2);

	contents = this->RangeToString(x1, y1, x2, y2, "\n");
	SetClipboardText(contents->String(), contents->Length());
	delete contents;

	//return (y2 - y1) + 1;
}

// paste the contents of the clipboard into the document
// at the insertion point.
void EditView::PasteFromClipboard()
{
char *contents;

	// get clipboard text
	contents = GetClipboardText();
	if (!contents || contents[0]=='\0')
		return;	// nothing on clipboard

	// remove any CR's from clipboard text, leave only LF's
	RemoveCharFromString(contents, '\r');

	BeginUndoGroup(this);

	int x, y;
	this->action_insert_string(this->cursor.x, this->cursor.y, contents, &x, &y);
	this->cursor.move(x, y);

	EndUndoGroup(this);
	frees(contents);
}

static void RemoveCharFromString(char *str, char ch)
{
char *pin = str;
char *pout = str;

	while(*pin != '\0')
	{
		if (*pin != ch)
			*(pout++) = *pin;

		pin++;
	}

	*pout = 0;
}

void SetClipboardText(const char *text)
{
	SetClipboardText(text, strlen(text));
}

void SetClipboardText(const char *text, int32 textLength)
{
BMessage *clip;

	if (be_clipboard->Lock())
	{
		be_clipboard->Clear();

		if ((clip = be_clipboard->Data()))
		{
			clip->AddData("text/plain", B_MIME_TYPE, text, textLength);
			be_clipboard->Commit();
		}

		be_clipboard->Unlock();
	}
}

static char *GetClipboardText()
{
BMessage *clip;
const char *text = NULL;
char *buffer = NULL;
ssize_t textlength = 0;

	if (be_clipboard->Lock())
	{
		if ((clip = be_clipboard->Data()))
		{
			if (clip->FindData("text/plain", B_MIME_TYPE,
				(const void **)&text, &textlength) == B_OK)
			{
				if (text && textlength >= 0)
				{
					buffer = (char *)smal(textlength + 1);

					if (textlength > 0)
						memcpy(buffer, text, textlength);

					buffer[textlength] = 0;
				}
			}
		}

		be_clipboard->Unlock();
	}

	return buffer;
}



