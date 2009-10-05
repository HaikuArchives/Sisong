
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "basics.h"
#include "stat.fdh"


void stat(const char *str, ...)
{
va_list ar;

	va_start(ar, str);
	
	vprintf(str, ar);
	printf("\n");
	fflush(stdout);
	
	va_end(ar);
}

void staterr(const char *str, ...)
{
va_list ar;
char buf[4096];

	va_start(ar, str);
	vsprintf(buf, str, ar);
	va_end(ar);

	fprintf(stderr, "%s\n", buf);
	fflush(stderr);
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
