
typedef struct CmdSeqData
{
	bool active;	// command sequence in progress
	
	char displaybuffer[80];
	char *buffer;
	int nchars;
};
