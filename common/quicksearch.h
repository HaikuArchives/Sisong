
#include <Locker.h>

class QSTree
{
public:
	QSTree();
	~QSTree();

	void AddMapping(const char *str, void *answer);
	void AddMapping(const char *str, int32 answer);

	void *Lookup(const char *str);
	int32 LookupInt(const char *str);

	void Delete(const char *str);

// ---------------------------------------

	void MTAddMapping(BLocker *lock, const char *str, void *answer)
	{
		lock->Lock();
		AddMapping(str, answer);
		lock->Unlock();
	}

	void MTAddMapping(BLocker *lock, const char *str, int32 answer)
	{
		lock->Lock();
		AddMapping(str, answer);
		lock->Unlock();
	}

	void *MTLookup(BLocker *lock, const char *str)
	{
		lock->Lock();
		void *answer = Lookup(str);
		lock->Unlock();
	}

	int32 MTLookupInt(BLocker *lock, const char *str)
	{
		lock->Lock();
		int32 answer = LookupInt(str);
		lock->Unlock();
	}

	void MTDelete(BLocker *lock, const char *str)
	{
		lock->Lock();
		Delete(str);
		lock->Unlock();
	}

private:
	QSTree *branch[256];
	void *answer;
	bool has_answer;
};
