
typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned short	ushort;

#define SWAP(a, b)		{ a ^= b; b ^= a; a ^= b; }

#define WIDTHOF(R)		((R.right - R.left) + 1)
#define HEIGHTOF(R)		((R.bottom - R.top) + 1)

#ifndef bp
	#define bp	_asm { int 3 }
#endif

#define rept			for(;;)
#define stop			{ /*smal_bypass_cleanup = 1;*/ staterr("programmer stop"); exit(1); }
#define here			{ staterr("---- Here ----"); }

void stat(const char *fmt, ...);
void staterr(const char *fmt, ...);

#ifndef DEBUG
	#define ASSERT(x)
#else
	#define ASSERT(x) \
	{ \
		if (!(x)) \
		{	\
			printf("assertion failed: \"%s\" at %s(%d)\n", #x, __FILE__, __LINE__);	\
			fflush(stdout); \
		}	\
	}
#endif

//#include "smal.h"
#define smal		malloc
#define resmal		realloc
#define frees		free
#define smal_strdup	strdup
