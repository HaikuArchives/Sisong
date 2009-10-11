
#include <OS.h>
#include "ThreadFlag.h"

ThreadFlag::ThreadFlag()
{
	fSemaphore = create_sem(0, "TF");
}

ThreadFlag::~ThreadFlag()
{
	if (fSemaphore != -1)
		delete_sem(fSemaphore);
}

/*
void c------------------------------() {}
*/

// "signal" the threadflag.
void ThreadFlag::Raise()
{
	// deleting the sem unblocks all who are waiting on it
	delete_sem(fSemaphore);
	fSemaphore = -1;
}

/*
void c------------------------------() {}
*/

// block calling thread until the flag becomes signaled
void ThreadFlag::Wait()
{
	if (fSemaphore != -1)
		acquire_sem_etc(fSemaphore, 1, B_DO_NOT_RESCHEDULE, 0);
}

// returns whether or not the flag has been signaled
bool ThreadFlag::IsRaised()
{
	return (fSemaphore == -1);
}
