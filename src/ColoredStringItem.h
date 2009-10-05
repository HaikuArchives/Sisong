
#include <ListItem.h>
#include <GraphicsDefs.h>


class ColoredStringItem : public BListItem
{
public:
	ColoredStringItem(const char *text, rgb_color fg, rgb_color bg, rgb_color bg_selected);
	~ColoredStringItem();

	virtual	void DrawItem(BView *owner, BRect frame, bool complete=false);
	virtual	void SetText(const char* text);
	const char *Text() const;
	void SetColor(rgb_color newColor);
	void SetBackgroundColor(rgb_color newColor);
	void SetSelectionColor(rgb_color newColor);
	
	virtual	void Update(BView* owner, const BFont* font);
	virtual	status_t Perform(perform_code code, void* arg);

private:
	char *fText;
	float fBaselineOffset;
	
	rgb_color fg, bg, bg_selected;
};
