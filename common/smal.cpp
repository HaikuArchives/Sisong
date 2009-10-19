
/*
	smal is a memory debugging library (a "malloc" and "new" wrapper).
*/

#include <OS.h>
#include <TLS.h>
#include <Locker.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "smal_atree.cpp"
#include "smal.lh"
#include "smal.fdh"

static ATree *smaldata;
static BLocker SmalLocker;

static int32 tls_file = -1;
static int32 tls_line = -1;
static int32 tls_function = -1;

#define NUM_GUARD_BYTES		16		// number of extra prefix and postfix bytes to allocate
static const uint32 kUnusedValue = 0xaa; 	// Fill value for freshly allocated blocks
static const uint32 kReleasedValue = 0xee; 	// Fill value for deallocated blocks

static const char kExpectedPrefix[NUM_GUARD_BYTES] =
	{ 0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xbe, 0xef };

static const char kExpectedSuffix[NUM_GUARD_BYTES] =
	{ 0xfe, 0xed, 0xfa, 0xce, 0xfe, 0xed, 0xfa, 0xce };

//#define WIPE_ON_MALLOC
//#define WIPE_ON_FREE
//#define CRASH_ON_ERRORS

static int num_blocks = 0;
static int num_bytes = 0;
static int peak_blocks = 0;
static int peak_bytes = 0;
bool use_with_cpp = false;
bool smal_shut_down = false;
bool smal_bypass_cleanup = false;

#define SMAL_TRACE	stat

smalInitilizer smalInstance;
smalInitilizer::smalInitilizer()
{
	tls_file = tls_allocate();
	tls_line = tls_allocate();
	tls_function = tls_allocate();
	
	smaldata = SATInit();
	use_with_cpp = true;
}

smalInitilizer::~smalInitilizer()
{
	smal_shut_down = true;
	use_with_cpp = false;
	
	if (smal_bypass_cleanup)
		return;
	
	SmalLocker.Lock();
	//ATreeLocker.Lock();
	
	if (num_blocks != 0)
	{
		stat("\n [] smal: program did not free %d block%s, totaling %d byte%s:", \
				num_blocks, (num_blocks == 1) ? "":"s", \
				num_bytes, (num_bytes == 1) ? "":"s");
		
		search_atree(smaldata, 0);
	}
	else
	{
		char strUsage[60];
		#define KILOBYTE	1024
		#define MEGABYTE	(KILOBYTE * KILOBYTE)
		
		if (peak_bytes >= MEGABYTE)
		{
			sprintf(strUsage, "%.2fMb", ((double)peak_bytes / MEGABYTE));
		}
		else if (peak_bytes >= KILOBYTE)
		{
			sprintf(strUsage, "%.2fkb", ((double)peak_bytes / KILOBYTE));
		}
		else
		{
			sprintf(strUsage, "%d bytes", peak_bytes);
		}
		
		stat(" [] smal: systems check ok, peak usage %s in %d blocks.", strUsage, peak_blocks);
	}
	
	SATClose(smaldata);
	//return wasError;
}

void search_atree(ATree *tree, int level)
{
	if (!tree) return;
	
	int i;
	if (level < 3)
	{
		for(i=0;i<256;i++)
		{
			if (tree->nodes[i])
				search_atree(tree->nodes[i], level+1);
		}
	}
	else
	{
		for(i=0;i<256;i++)
		{
			SmalRecord *sr = (SmalRecord *)tree->answers[i];
			
			if (sr)
			{
				stat("    >> 0x%08x; %d bytes, from %s(%d) < %s >%s", sr->user_ptr, sr->size, \
						sr->file, sr->line, sr->function,
						sr->reallocd ? "  (buffer had been realloc'd)":"");
				
				if (sr->size < 48)
					smal_hexdump(sr->user_ptr, sr->size, 7);
				stat("");
			}
		}
	}
}

/*
void c------------------------------() {}
*/

static void error_occurred(void)
{
	#ifdef CRASH_ON_ERRORS
	{
		char *ptr = (char *)0;
		*ptr = 42;
	}
	#endif
}

static void used_after_shutdown(const char *funcname)
{
	stat(" [] smal: attempt to use %s() after shutdown", funcname);
	error_occurred();
}

/*
void c------------------------------() {}
*/

// inserted just before every "new" via a preprocessor trick
/*void sm_remember(const char *file, int line, const char *function)
{
	//stat(" ----- %s(%d): %s: sm_remember -----", file, line, function);
	
	tls_set(tls_file, (void *)file);
	tls_set(tls_function, (void *)function);
	tls_set(tls_line, (void *)line);
}

void *operator new(size_t size)
{
	if (use_with_cpp)
	{
		char *file = NULL, *function = NULL;
		int line = 0;
		
		// retrieve stashed info from sm_remember
		if (tls_file != -1)
		{
			file = (char *)tls_get(tls_file);
			function = (char *)tls_get(tls_function);
			line = (int)tls_get(tls_line);
			
			tls_set(tls_file, NULL);
			tls_set(tls_function, NULL);
			tls_set(tls_line, 0);
		}
		
		char *user_ptr = (char *)internal_smal(size, file, function, line);
		//SMAL_TRACE(" :: operator new [ %s(%d): %s ] = size %d, ptr %08x", file, line, function, size, user_ptr);
		return user_ptr;
	}
	else
	{
		char *user_ptr = (char *)malloc(size);
		SMAL_TRACE(":: operator new(size = %d) = %x", size, user_ptr);
		return user_ptr;
	}
}

void operator delete(void *ptr)
{
	if (ptr == NULL)
		return;
	
	//SMAL_TRACE(" :: operator delete [ 0x%08x ]", ptr);
	
	if (use_with_cpp)
	{
		char *file = NULL, *function = NULL;
		int line = 0;
		
		// retrieve stashed info from sm_remember
		if (tls_file != -1)
		{
			file = (char *)tls_get(tls_file);
			function = (char *)tls_get(tls_function);
			line = (int)tls_get(tls_line);
			
			tls_set(tls_file, NULL);
			tls_set(tls_function, NULL);
			tls_set(tls_line, 0);
		}
		
		internal_frees(ptr, file, function, line);
	}
	else
	{
		free(ptr);
	}
}*/

/*
void c------------------------------() {}
*/

void *internal_smal(size_t size, const char *file, const char *function, int line)
{
char *ptr, *user_ptr;

	//SMAL_TRACE("smal: size=%d, file=%s, function=%s, line=%d", size, file, function, line);
	
	if (smal_shut_down)
	{
		used_after_shutdown("smal");
		return NULL;
	}
	
	if (!(ptr = (char *)malloc(NUM_GUARD_BYTES + size + NUM_GUARD_BYTES)))
	{
		stat("smal: out of memory condition!!");
		return NULL;
	}
	
	SmalLocker.Lock();
	
	user_ptr = (ptr + NUM_GUARD_BYTES);
	
	// wipe with initial values
	memcpy(ptr, kExpectedPrefix, NUM_GUARD_BYTES);
	memcpy(user_ptr+size, kExpectedSuffix, NUM_GUARD_BYTES);
	#ifdef WIPE_ON_MALLOC
		memset(user_ptr, kUnusedValue, size);
	#endif
	
	//SMAL_TRACE("smal: size=%d, %d bytes, user_ptr=%x, real_ptr=%x.", size, NUM_GUARD_BYTES+size+NUM_GUARD_BYTES, user_ptr, ptr);
	//smal_hexdump(ptr, NUM_GUARD_BYTES+size+NUM_GUARD_BYTES);
	
	// create the record
	SmalRecord *sr = (SmalRecord *)malloc(sizeof(SmalRecord));
	
	sr->user_ptr = user_ptr;
	sr->size = size;
	sr->file = file ? strdup(file) : NULL;
	sr->function = function ? strdup(function) : NULL;
	sr->line = line;
	sr->reallocd = false;
	
	SATAddMapping(smaldata, user_ptr, sr);
	
	// update stats
	num_blocks++;
	num_bytes += size;
	
	if (num_blocks > peak_blocks) peak_blocks = num_blocks;
	if (num_bytes > peak_bytes) peak_bytes = num_bytes;
	
	SmalLocker.Unlock();
	
	// return pointer to user portion of memory block
	return user_ptr;
}

void frees(void *block)
{
	internal_frees(block, NULL, NULL, 0);
}
void internal_frees(void *block, const char *file, const char *function, int line)
{
char *user_ptr = (char *)block;

	//SMAL_TRACE("frees(%x)", user_ptr);
	
	if (smal_shut_down)
	{
		used_after_shutdown("frees");
		return;
	}
	
	SmalLocker.Lock();
	
	// verify that the memory block actually is allocated
	SmalRecord *sr = (SmalRecord *)SATLookup(smaldata, user_ptr);
	
	if (!sr)
	{
		if (user_ptr == NULL)
		{
			stat(" [] smal: attempt to free a NULL pointer. From %x [ %s(%d) < %s > ]", __builtin_return_address(0), file, line, function);
		}
		else
		{
			stat(" [] smal: attempt to free %08x, which is not allocated. From %x [ %s(%d) < %s > ]", user_ptr, __builtin_return_address(0), file, line, function);
		}
		
		error_occurred();
		SmalLocker.Unlock();
		return;
	}
	
	// check for overrun's / underrun's
	check_block_integrity(sr);
	
	// update stats
	num_blocks--;
	num_bytes -= sr->size;
	
	// free the record
	if (sr->file) free(sr->file);
	if (sr->function) free(sr->function);
	free(sr);
	
	// remove record from table
	SATDelete(smaldata, user_ptr);
	
	// clear out the block
	#ifdef WIPE_ON_FREE
		memset(user_ptr-NUM_GUARD_BYTES, kReleasedValue, \
								NUM_GUARD_BYTES + sr->size + NUM_GUARD_BYTES);
	#endif
	
	// free the block
	free(user_ptr - NUM_GUARD_BYTES);
	SmalLocker.Unlock();
}

// checks if the block was mistreated (overran) while it was owned
// by the application.
static void check_block_integrity(SmalRecord *sr)
{
	if (memcmp(sr->user_ptr+sr->size, kExpectedSuffix, NUM_GUARD_BYTES) != 0)
	{
		block_failed("overrun", "suf", sr, sr->size, kExpectedSuffix);
	}
	
	if (memcmp(sr->user_ptr-NUM_GUARD_BYTES, kExpectedPrefix, NUM_GUARD_BYTES) != 0)
	{
		block_failed("underrun", "pre", sr, -NUM_GUARD_BYTES, kExpectedPrefix);
	}
}

static void block_failed(char *sp, char *sp2, SmalRecord *sr, int offset, const char *expected_bytes)
{
	stat("\n [] smal: block 0x%08x was %s, allocated from [ %s(%d) < %s > ]", sr->user_ptr, sp,\
														 sr->file, sr->line, sr->function);
	
	printf("    >> %sfix is:", sp2); smal_hexdump(sr->user_ptr + offset, NUM_GUARD_BYTES, 2);
	printf("    >> expected:"); smal_hexdump(expected_bytes, NUM_GUARD_BYTES, 3);
	
	error_occurred();
}

/*
void c------------------------------() {}
*/

void *resmal(void *block, size_t newsize)
{
char *user_ptr = (char *)block;
char *ptr;

	//SMAL_TRACE("resmal(%x, %d)", user_ptr, newsize);
	
	if (smal_shut_down)
	{
		used_after_shutdown("resmal");
		return NULL;
	}
	
	SmalLocker.Lock();
	
	// verify that the memory block actually is allocated
	SmalRecord *sr = (SmalRecord *)SATLookup(smaldata, user_ptr);
	
	if (!sr)
	{
		if (user_ptr == NULL)
		{
			stat(" [] smal: attempt to realloc a NULL pointer, from %08x", __builtin_return_address(0));
		}
		else
		{
			stat(" [] smal: attempt to realloc %08x, which is not allocated (from %08x)", user_ptr, __builtin_return_address(0));
		}
		
		error_occurred();
		SmalLocker.Unlock();
		return NULL;
	}
	
	// check for overrun's / underrun's
	check_block_integrity(sr);
	
	//stat("before:");
	//smal_hexdump(user_ptr-NUM_GUARD_BYTES, NUM_GUARD_BYTES+sr->size+NUM_GUARD_BYTES);
	//stat("resmal %08x from %d -> %d", user_ptr, sr->size, newsize);
	
	// realloc the block
	ptr = (char *)realloc((user_ptr - NUM_GUARD_BYTES), \
						NUM_GUARD_BYTES + newsize + NUM_GUARD_BYTES);
	
	user_ptr = (ptr + NUM_GUARD_BYTES);
	
	// wipe the new areas and move the guard bytes
	#ifdef WIPE_ON_MALLOC
		int size_increase = (newsize - sr->size);
		if (size_increase > 0)
			memset(user_ptr + sr->size, kUnusedValue, size_increase);
	#endif
	// move the guard bytes
	memcpy(user_ptr + newsize, kExpectedSuffix, NUM_GUARD_BYTES);
	
	//stat("after:");
	//smal_hexdump(user_ptr-NUM_GUARD_BYTES, NUM_GUARD_BYTES+newsize+NUM_GUARD_BYTES);
	
	// update stats
	num_bytes += (newsize - sr->size);
	
	// update the record
	if (sr->user_ptr != user_ptr)
	{
		SATDelete(smaldata, sr->user_ptr);
		SATAddMapping(smaldata, user_ptr, sr);
	}
	
	sr->user_ptr = user_ptr;
	sr->size = newsize;
	sr->reallocd = true;
	
	SmalLocker.Unlock();
	return user_ptr;
}


/*
void c------------------------------() {}
*/


char *smal_strdup(const char *str)
{
int string_size = strlen(str) + 1;
char *buffer = (char *)internal_smal(string_size, __FILE__, __FUNCTION__, __LINE__);

	memcpy(buffer, str, string_size);
	return buffer;
}


char *smal_strdup(const char *str, int extra)
{
int string_size = strlen(str) + 1;
char *buffer = (char *)internal_smal(string_size + extra, __FILE__, __FUNCTION__, __LINE__);

	memcpy(buffer, str, string_size);
	return buffer;
}

void *internal_smalz(size_t size, const char *file, const char *function, int line)
{
char *user_ptr;

	if ((user_ptr = (char *)internal_smal(size, file, function, line)))
	{
		memset(user_ptr, 0, size);
	}
	
	return user_ptr;
}

/*
void c------------------------------() {}
*/


void smal_hexdump(const char *data, int len, int indent_amt)
{
int i;
int off = 0;
uchar *linedata;
char line[800];
char *ptr;

	do
	{
		memset(line, ' ', indent_amt);
		line[indent_amt] = 0;
		
		linedata = (uchar *)&data[off];
		sprintf(line, "%s%02X: ", line, off);
		
		// print 16 chars of hex data
		for(i=0;i<16;i++)
		{
			if (off+i < len)
				sprintf(line, "%s%02x", line, linedata[i]);
			else
				strcat(line, "  ");
			
			if (i & 1) strcat(line, " ");
		}
		
		strcat(line, "    ");
		
		// print the same chars again, as ASCII data
		ptr = &line[strlen(line)];
		for(i=0;i<16;i++)
		{
			if (off+i >= len) break;
			*(ptr++) = ((linedata[i] > 30 && linedata[i] < 129) ? linedata[i] : '.');
		}
		
		*ptr = 0;
		
		puts(line);
		fflush(stdout);
		off += 0x10;
	}
	while(off < len);
}


