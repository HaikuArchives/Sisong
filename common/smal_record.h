
typedef struct stMemRecord
{
	void *ptr;
	unsigned int size;
	
	struct stMemRecord *prev, *next;
} stMemRecord;

