
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <Locker.h>

#include "basics.h"
#include "stat.fdh"

static BLocker statlock;
static char log_file_name[MAXPATHLEN] = { 0 };

void SetLogFilename(const char *nn)
{
	strcpy(log_file_name, nn);
	remove(log_file_name);
}


void stat(const char *str, ...)
{
va_list ar;
char buf[40000];

	statlock.Lock();

	va_start(ar, str);
	vsprintf(buf, str, ar);
	va_end(ar);

	fprintf(stdout, "%s\n", buf);
	fflush(stdout);

	if (log_file_name[0])
	{
		lstat("%s", buf);
	}

	statlock.Unlock();
}

void staterr(const char *str, ...)
{
va_list ar;
char buf[40000];

	statlock.Lock();

	va_start(ar, str);
	vsprintf(buf, str, ar);
	va_end(ar);

	fprintf(stderr, "%s\n", buf);
	fflush(stderr);

	if (log_file_name[0])
	{
		lstat("<< error: '%s' >>", buf);
	}

	statlock.Unlock();
}


void lstat(const char *str, ...)
{
va_list ar;
char buf[4096];

	if (!log_file_name[0])
		return;

	statlock.Lock();

	FILE *fp = fopen(log_file_name, "a+");
	if (fp)
	{
		va_start(ar, str);
		vsprintf(buf, str, ar);
		va_end(ar);

		fprintf(fp, "%s\n", buf);
		fclose(fp);
	}

	statlock.Unlock();
}



/*
char *stprintf(const char *fmt, ...)
{
va_list ar;
char *str = GetStaticStr();

	va_start(ar, fmt);
	vsprintf(str, fmt, ar);
	va_end(ar);

	return str;
}
*/
