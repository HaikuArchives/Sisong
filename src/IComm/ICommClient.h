
class ICommClient
{
public:
	ICommClient(BMessenger messenger);
	~ICommClient();
	
	int GetCurrentCol();
	int GetCurrentLine();
	
	void SendMessageCommand(uint command, BMessage *reply);
	int SendMessageGetInt(uint command);

private:
	BMessenger fServer;		// that is, the editor
};

#define MESSAGE_TIMEOUT		(1000 * 1000)
