
/*
	Simple "Check for updates on startup" feature--
	allows Sisong users to be notified on startup if a new version is available.
	
	I usually don't like this kind of stuff, but I figured this way the people that
	use the program can be quickly notified of updates, without having to spam
	the forums etc for those who aren't interested.
	
	All it does is check if there is a new message available on the server,
	and if so, pops it up in a BAlert.
*/
#include "editor.h"
#include "NotifyUpdate.h"
#include <sys/wait.h>

namespace UpdateCheck {

#define CURRENT_VERSION			"14"


status_t Thread(void *waittime)
{
FILE *fp;
char line[1024];

	//stat("UpdateCheck started.");
	snooze((int)waittime);
	
	//stat("doing UC");
	
	// get temp file name
	char tmpfilename[MAXPATHLEN];
	find_directory(B_SYSTEM_TEMP_DIRECTORY, 0, true, tmpfilename, MAXPATHLEN-32);
	int len = strlen(tmpfilename);
	if (len && tmpfilename[len-1] != '/') strcat(tmpfilename, "/");
	strcat(tmpfilename, "ucheck");
	
	// ensure it gone
	remove(tmpfilename);
	
	// form the query
	BString page;
	page << "http://five75.sourceforge.net/ucheck.php?app=" <<
		APPLICATION_NAME << "&curversion=" << CURRENT_VERSION;
	
	//stat("fetching %s", page.String());
	
	// would have just used system(), but it didn't wait for wget to return
	// like it was supposed to (I think wget forks?)
	int child = vfork();
	if (child == 0)
	{
		setpgid(0, 0);
		execlp("wget", "wget", "--quiet",
				"--output-document", tmpfilename, page.String(), NULL);
		// in case of failure
		exit(1);
	}
	
	int exitstat;
	waitpid(child, &exitstat, 0);
	
	//stat("me back, estat %d", exitstat);
	
	// open the resultant file
	fp = fopen(tmpfilename, "rb");
	if (!fp) return B_ERROR;
	
	// make sure it's really a valid file, and not a 404 error page etc
	fgetline(fp, line, sizeof(line));
	if (!strbegin(line, "MAGIC"))
	{
		//stat("ucheck: bad magic");
		fclose(fp);
		remove(tmpfilename);
		return B_ERROR;
	}
	
	//stat("MAGIC passed");
	// read in the message number, and see if it's greater than the last
	// message acknowledged by user
	fgetline(fp, line, sizeof(line));
	int current_msg_no = atoi(line);
	int last_msg_no = settings->GetInt("LastAckMsg", 0);
	
	stat("msg numbers: %d vs %d", current_msg_no, last_msg_no);
	
	if (current_msg_no > last_msg_no)
	{
		// read in each line of the message and build the dialog box
		BString boxstr;
		
		while(!feof(fp))
		{
			fgetline(fp, line, sizeof(line));
			
			if (boxstr.Length() > 0)
				boxstr.Append("\n");
			
			boxstr.Append(line);
		}
		
		// display the alert
		BAlert *alert = new BAlert("", boxstr.String(), "OK");
		alert->Go();
		
		// don't show this same message again
		settings->SetInt("LastAckMsg", current_msg_no);
	}
	
	fclose(fp);
	remove(tmpfilename);
	return B_OK;
}




void Go(uint delay)
{
	thread_id thread = spawn_thread(UpdateCheck::Thread, "UpdateCheck", B_LOW_PRIORITY, (void *)delay);
	resume_thread(thread);
}


}
