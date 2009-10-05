
class testview : public BView
{
	public:
		testview(BRect frame, uint32 resizingMode);
		~testview();
		virtual void Draw(BRect updateRect);

	private:
		unsigned char r, g, b;
};
