#ifndef CMDSEQ_H
#define CMDSEQ_H

typedef struct CmdSeqData
{
	bool active;	// command sequence in progress
	
	char displaybuffer[80];
	char *buffer;
	int nchars;
};

bool IsCommand(char *command, int *message);

#endif
