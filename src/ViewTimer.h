
class CViewTimer
{
public:
	CViewTimer(BLooper *view, int msg, uint interval);
	~CViewTimer();

	friend int32 ThreadFunc(CViewTimer *data);
	
//private:
public:
	thread_id _thread;
	uint _us_delay;
	BLooper *MessageTarget;
	int message;
};
