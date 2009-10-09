
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
#define stop			{ staterr("programmer stop"); exit(1); }
#define here			{ staterr("---- Here ----"); }

void stat(const char *fmt, ...);
void staterr(const char *fmt, ...);

#include "smal.h"
