

// spacer between line numbers and editor pane
class CSpacerView : public BView
{
	public:
		CSpacerView(BRect frame, uint32 resizingMode);
		virtual void Draw(BRect updateRect);
		virtual void MouseDown(BPoint where);
};
