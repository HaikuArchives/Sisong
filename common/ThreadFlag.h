

class ThreadFlag
{
public:
	ThreadFlag();
	~ThreadFlag();

	void Raise();

	void Wait();
	bool IsRaised();

private:
	sem_id fSemaphore;
};
