
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Locker.h>
#include "basics.h"

#include "smal_atree.c"
ATNode *MemRecords;
static BLocker smallock;

#include "smal.h"
#include "smal_record.h"
#include "smal.fdh"

static stMemRecord *firstsmalrecord = NULL;
static unsigned int mem_usage = 0;
static unsigned int num_blocks = 0;
static unsigned int peak_usage = 0;
static unsigned int peak_blocks = 0;
static unsigned int smal_errors = 0;

static void watch_bp(void *ptr, int size)
{
	staterr(" >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> *** Watch_bp called: %08x size %d\n", ptr, size);
}

void *smal(int size)
{
void *ptr;

	if (size <= 0)
	{
		staterr("smal: Hey! I got size=%d!", size);
		smal_errors++;
		return NULL;
	}

	smallock.Lock();

	ptr = malloc(size);
	if (!ptr)
	{
		staterr("smal: allocation of %d bytes failed", size);
		smallock.Unlock();
		return NULL;
	}

	mem_usage += size;
	if (mem_usage > peak_usage) peak_usage = mem_usage;
	if (++num_blocks > peak_blocks) peak_blocks = num_blocks;

	AddMemRecord(ptr, size);

	smallock.Unlock();
	return ptr;
}

void *smalz(int size)
{
void *ptr = smal(size);
	memset(ptr, 0, size);
	return ptr;
}

void *resmal(void *ptr, int size)
{
stMemRecord *r;

	smallock.Lock();

	if (!(r = FindMemRecord(ptr)))
	{
		staterr(" **** resmal: attempt to reallocate pointer %08x, which is not allocated", ptr);
		smal_errors++;
		smallock.Unlock();
		return NULL;
	}

	ptr = realloc(ptr, size);
	if (!ptr)
	{
		staterr("resmal: allocation of %d bytes failed", size);
		smallock.Unlock();
		return NULL;
	}

	mem_usage += (size - r->size);
	if (mem_usage > peak_usage) peak_usage = mem_usage;
	if (++num_blocks > peak_blocks) peak_blocks = num_blocks;

	SATDelete(MemRecords, r->ptr);
	SATAddMapping(MemRecords, ptr, r);

	r->ptr = ptr;
	r->size = size;

	smallock.Unlock();
	return ptr;
}

void frees(void *ptr)
{
stMemRecord *r;

	if (ptr==NULL)
	{
		staterr(" ***** frees: received a NULL pointer");
		smal_errors++;
		return;
	}

	smallock.Lock();

	if (!(r = FindMemRecord(ptr)))
	{
		staterr(" **** frees: attempt to free pointer %08x, which is not allocated", ptr);
		smal_errors++;
		smallock.Unlock();
		return;
	}

	memset(ptr, 0xee, r->size);

	mem_usage -= r->size;
	num_blocks--;

	free(ptr);
	RemoveMemRecord(r);
	smallock.Unlock();
}

char *smal_strdup(const char *str)
{
int string_size = strlen(str) + 1;
char *buffer = (char *)smal(string_size);

	memcpy(buffer, str, string_size);
	return buffer;
}

char *smal_strdup(const char *str, int extra)
{
int string_size = strlen(str) + 1;
char *buffer = (char *)smal(string_size + extra);

	memcpy(buffer, str, string_size);
	return buffer;
}


void AddMemRecord(void *ptr, unsigned int size)
{
stMemRecord *r;

	if (!(r = (stMemRecord *)malloc(sizeof(stMemRecord))))
	{
		staterr("smal: failed allocation of memory record");
		return;
	}

	r->ptr = ptr;
	r->size = size;

	r->next = firstsmalrecord;
	r->prev = NULL;
	if (firstsmalrecord) firstsmalrecord->prev = r;
	firstsmalrecord = r;

	SATAddMapping(MemRecords, ptr, r);
}

void RemoveMemRecord(stMemRecord *r)
{
	SATDelete(MemRecords, r->ptr);

	if (r->next) r->next->prev = r->prev;
	if (r->prev) r->prev->next = r->next;
	if (r==firstsmalrecord) firstsmalrecord = r->next;
	free(r);
}

stMemRecord *FindMemRecord(void *ptr)
{
	return (stMemRecord *)SATLookup(MemRecords, ptr);
}

void smal_init(void)
{
	MemRecords = SATInit();
}

void smal_hexdump(void *data_void, int len)
{
uchar *data = (uchar *)data_void;
int i;
int off = 0;
uchar *linedata;
char line[800];
char *ptr;

	do
	{
		linedata = &data[off];
		sprintf(line, "  %02X: ", off);

		// print 16 chars of hex data
		for(i=0;i<16;i++)
		{
			if (off+i < len)
				sprintf(line, "%s%02x", line, linedata[i]);
			else
				strcat(line, "  ");

			strcat(line, " ");
		}

		strcat(line, "    ");

		// print the same chars again as ASCII data
		ptr = &line[strlen(line)];
		for(i=0;i<16;i++)
		{
			if (off+i >= len) break;
			*(ptr++) = ((linedata[i] > 30 && linedata[i] < 129) ? linedata[i] : '.');
		}

		//*(ptr++) = '\n';
		*ptr = 0;

		staterr("%s", line);
		off += 0x10;
	}
	while(off < len);
}

int smal_cleanup(void)
{
stMemRecord *r = firstsmalrecord;
stMemRecord *next;

	smallock.Lock();

	SATClose(MemRecords);

	if (mem_usage != 0)
	{
		if (mem_usage > 0)
		{
			staterr(" *** smal: mem_usage > 0 on exit");
		}
		else
		{
			staterr(" *** smal: mem_usage < 0 on exit");
		}
	}

	while(r)
	{
		next = r->next;

		staterr(" *** smal: program did not free %08x; %d bytes", r->ptr, r->size);
		if (r->size < 48)
			smal_hexdump(r->ptr, r->size);

		smal_errors++;

		free(r->ptr);
		free(r);

		r = next;
	}

	if (!smal_errors)
	{
		char usage[600];
		#define KILOBYTE	1024
		#define MEGABYTE	(KILOBYTE * KILOBYTE)

		if (peak_usage >= MEGABYTE)
		{
			sprintf(usage, "%.2fMb", ((double)peak_usage / MEGABYTE));
		}
		else if (peak_usage >= KILOBYTE)
		{
			sprintf(usage, "%.2fkb", ((double)peak_usage / KILOBYTE));
		}
		else
		{
			sprintf(usage, "%d bytes", peak_usage);
		}

		stat("\nsmal: systems check ok; peak %s in %d blocks.", usage, peak_blocks);
	}
	else
	{
		staterr(" *** smal: %d memory allocation glitches occurred!", smal_errors);
	}

	return smal_errors;
}
