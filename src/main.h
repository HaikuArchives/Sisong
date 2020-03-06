

// the application class
class EApp : public BApplication
{
    public:
		EApp();
		~EApp();
		
		virtual void RefsReceived(BMessage *message);
		virtual void MessageReceived(BMessage *msg);
};


