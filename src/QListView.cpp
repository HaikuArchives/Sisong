


QListView::QListView(BRect frame, uint32 resizingMode)
	: MessageView(frame, "QListView", resizingMode, 0)
{
	// create list
	BRect rc(Bounds());
	rc.InsetBy(2, 2);
	rc.right -= B_V_SCROLL_BAR_WIDTH;

	fListView = new BListView(rc, "list", B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	
	// create scrollview
	fScrollView = new BScrollView("sv", fListView, B_FOLLOW_ALL, 0, false, true);
	AddChild(fScrollView);
}

