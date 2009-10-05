
#include <stdlib.h>
#include <memory.h>
#include "QuickSearch.h"


QSNode *QSInit(void)
{
QSNode *node;

	node = (QSNode *)malloc(sizeof(QSNode));
	memset(node, 0, sizeof(QSNode));
	
	return node;
}

// add a string to the given tree.
void QSAddString(QSNode *basenode, const char *str, int answer)
{
QSNode *node = basenode;
unsigned char ch;
int i;

	for(i=0;str[i];i++)
	{
		ch = str[i];
		
		if (!node->branch[ch])
			node->branch[ch] = QSInit();
		
		node = node->branch[ch];
	}
	
	node->answer = answer;
}

// looks up the string you give it, and returns the answer, or -1 if it doesn't exist
int QSLookup(QSNode *basenode, const char *str)
{
QSNode *node = basenode;
int i;

	if (!str) return -1;
	
	for(i=0;str[i];i++)
	{
		node = node->branch[(unsigned char)str[i]];
		if (!node) return -1;
	}
	
	return node->answer;
}

// close down an entire tree
void QSClose(QSNode *node)
{
int i;

	if (node)
	{
		for(i=0;i<256;i++)
		{
			if (node->branch[i])
				QSClose(node->branch[i]);
		}
		
		free(node);
	}
}

