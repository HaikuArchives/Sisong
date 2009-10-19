
//void smal_init();
//bool smal_cleanup();
#define smal_init()		;
#define smal_cleanup()	0

void *internal_smal(size_t size, const char *file, const char *function, int line);
void *internal_smalz(size_t size, const char *file, const char *function, int line);
void frees(void *block);//, const char *file, const char *function, int line);

void *resmal(void *ptr, size_t newsize);

char *smal_strdup(const char *str);
char *smal_strdup(const char *str, int extra);

extern bool smal_bypass_cleanup;


void sm_remember(const char *file, int line, const char *function);

#define SMAL	\
	void *operator new(size_t size) { return smal(size); }	\
	void operator delete(void *ptr) { frees(ptr); }	\


#define smal(X)		internal_smal(X, __FILE__, __PRETTY_FUNCTION__, __LINE__)
#define smalz(X)	internal_smalz(X, __FILE__, __PRETTY_FUNCTION__, __LINE__)

//#define frees(X)	internal_frees(X, __FILE__, __PRETTY_FUNCTION__, __LINE__)

//#define	new    (sm_remember(__FILE__, __LINE__, __PRETTY_FUNCTION__), false) ? NULL : new
//#define delete (sm_remember(__FILE__, __LINE__, __PRETTY_FUNCTION__), false) ? sm_remember("",0,"") : delete
