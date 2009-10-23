
#include <stdlib.h>
#include <memory.h>

void *smal(int size)					{ return malloc(size); }
void *resmal(void *ptr, int size)		{ return realloc(ptr, size); }
void frees(void *ptr)					{ free(ptr); }
char *smal_strdup(const char *str)		{ return strdup(str); }
