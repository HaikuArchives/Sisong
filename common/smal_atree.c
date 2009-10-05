
// the atree is a quicksearch-like algorithm optimized for
// mapping one uint to another.
#include "atree.h"

#define SPLIT_NUMBER(number, n1, n2, n3, n4)	\
{	\
uint temp = number;	\
	n1 = (uchar)(temp);	\
	n2 = (uchar)(temp >> 8); \
	n3 = (uchar)(temp >> 16); \
	n4 = (uchar)(temp >> 24); \
}

ATNode *SATInit(void)
{
ATNode *a;

	a = (ATNode *)malloc(sizeof(ATNode));
	memset(a, 0, sizeof(ATNode));
	return a;
}

static ATNode *sfollow_tree(ATNode **node)
{
	if (!*node)
	{
		ATNode *newnode = SATInit();
		*node = newnode;
	}
	
	return *node;
}

void SATAddMapping(ATNode *tree, void *map_from, void *map_to)
{
uchar n1, n2, n3, n4;

	SPLIT_NUMBER((uint)map_from, n1, n2, n3, n4);
	
	tree = sfollow_tree(&tree->nodes[n1]);
	tree = sfollow_tree(&tree->nodes[n2]);
	tree = sfollow_tree(&tree->nodes[n3]);
	
	tree->answers[n4] = (uint)map_to;
}

void *SATLookup(ATNode *tree, void *map_from)
{
uchar n1, n2, n3, n4;

	SPLIT_NUMBER((uint)map_from, n1, n2, n3, n4);

	tree = tree->nodes[n1]; if (!tree) return 0;
	tree = tree->nodes[n2]; if (!tree) return 0;
	tree = tree->nodes[n3]; if (!tree) return 0;
	return (void *)tree->answers[n4];
}

void SATDelete(ATNode *tree, void *object_to_delete)
{
	SATAddMapping(tree, object_to_delete, NULL);
}


static void SATCloseInternal(ATNode *tree, uchar rlevel)
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

void SATClose(ATNode *tree)
{
	SATCloseInternal(tree, 0);
}
