
#include "editor.h"
#include "stats.fdh"


// updates the "Hours/Minutes/Seconds" used tracker
void Stats_Tick(bool in_foreground)
{
bigtime_t curtime = system_time();
bigtime_t elapsed;

	// get how much time has elapsed since we were called last
	elapsed = (curtime - editor.stats.LastUsageUpdateTime);
	editor.stats.LastUsageUpdateTime = curtime;
	
	if (!in_foreground)
		return;
	
	// cascade on down the counters
	editor.stats.us_used += elapsed;
	
	while(editor.stats.us_used >= 1000 * 1000)
	{
		editor.stats.us_used -= 1000 * 1000;		
		editor.stats.seconds_used++;
		
		while(editor.stats.seconds_used >= 60)
		{
			editor.stats.seconds_used -= 60;
			editor.stats.minutes_used++;
			
			while(editor.stats.minutes_used >= 60)
			{
				editor.stats.minutes_used -= 60;
				editor.stats.hours_used++;
				
				while(editor.stats.hours_used >= 24)
				{
					editor.stats.hours_used -= 24;
					editor.stats.days_used++;
				}
			}
		}
	}
}
















