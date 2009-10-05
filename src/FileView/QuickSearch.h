
#ifndef _QUICKSEARCH_H
#define _QUICKSEARCH_H

typedef struct QSNode
{
	struct QSNode *branch[256];
	int answer;
} QSNode;

typedef QSNode QSTree;


QSNode *QSInit(void);
void QSAddString(QSNode *basenode, const char *str, int answer);
int QSLookup(QSNode *basenode, const char *str);
void QSClose(QSNode *node);

#endif
