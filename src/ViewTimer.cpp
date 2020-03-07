
#include <OS.h>
#include <Window.h>
#include <scheduler.h>
#include "ViewTimer.h"
#include "../common/basics.h"

status_t ThreadFunc(void *vtimer)
{
CViewTimer *timer = (CViewTimer *)vtimer;
	rept
	{
		usleep(timer->_us_delay);
		timer->MessageTarget->PostMessage(timer->message);
	}
}

// post message "msg" to "target' every "interval" milliseconds
CViewTimer::CViewTimer(BLooper *target, int msg, uint interval)
{
	MessageTarget = target;
	message = msg;
	_us_delay = (interval * 1000);
	
	_thread = spawn_thread(ThreadFunc, "ViewTimer thread", suggest_thread_priority(), this);
	resume_thread(_thread);
}

CViewTimer::~CViewTimer()
{
	kill_thread(_thread);
}

