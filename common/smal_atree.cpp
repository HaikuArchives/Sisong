
// the atree is a quicksearch-like algorithm optimized for
// mapping one void * to another.
#include "smal_atree.h"

//static BLocker ATreeLocker;


ATree *SATInit(void)
{
ATree *a;

	a = (ATree *)malloc(sizeof(ATree));
	memset(a, 0, sizeof(ATree));
	return a;
}

/*
void c------------------------------() {}
*/

#define SPLIT_ADDRESS(addr, n1, n2, n3, n4)	\
{	\
	n1 = (uchar)(addr >> 24); \
	n2 = (uchar)(addr >> 16); \
	n3 = (uchar)(addr >> 8); \
	n4 = (uchar)(addr);	\
}

static ATree *follow_tree(ATree **node)
{
	if (!*node)
	{
		ATree *newnode = SATInit();
		*node = newnode;
	}

	return *node;
}

void SATAddMapping(ATree *tree, void *map_from, void *map_to)
{
uchar n1, n2, n3, n4;

	//ATreeLocker.Lock();

	SPLIT_ADDRESS((uint)map_from, n1, n2, n3, n4);

	tree = follow_tree(&tree->nodes[n1]);
	tree = follow_tree(&tree->nodes[n2]);
	tree = follow_tree(&tree->nodes[n3]);

	tree->answers[n4] = (uint)map_to;
	//ATreeLocker.Unlock();
}

/*
void c------------------------------() {}
*/

void *SATLookup(ATree *tree, void *map_from)
{
uchar n1, n2, n3, n4;
void *answer;

	//ATreeLocker.Lock();
	SPLIT_ADDRESS((uint)map_from, n1, n2, n3, n4);

	tree = tree->nodes[n1]; if (!tree) return 0;
	tree = tree->nodes[n2]; if (!tree) return 0;
	tree = tree->nodes[n3]; if (!tree) return 0;
	answer = (void *)tree->answers[n4];
	//ATreeLocker.Unlock();

	return answer;
}

/*
void c------------------------------() {}
*/

void SATDelete(ATree *tree, void *object_to_delete)
{
	SATAddMapping(tree, object_to_delete, NULL);
}


/*
void c------------------------------() {}
*/

static void SATCloseInternal(ATree *tree, uchar rlevel)
{
int i;

	if (rlevel < 3 && tree)
	{
		for(i=0;i<256;i++)
		{
			if (tree->nodes[i])
			{
				SATCloseInternal(tree->nodes[i], rlevel+1);
			}
		}
	}

	free(tree);
}


void SATClose(ATree *tree)
{
	//ATreeLocker.Lock();
	SATCloseInternal(tree, 0);
}


