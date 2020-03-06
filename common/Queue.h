

typedef void (*queue_freer_func)(void *item);


class Queue
{
public:
	Queue(queue_freer_func func);
	~Queue();

	void EnqueueItem(void *item);
	void *GetNextItem();

// ---------------------------------------

	BList fQueue;
	BLocker fLock;

	queue_freer_func fFreeItemFunc;

};


