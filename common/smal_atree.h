

typedef struct ATree
{
	// unlike QSNodes, an ATree is always exactly 4 levels deep.
	// thus there is no 4th level; once at the 3rd level, all the
	// branches point to answers, using the answer[] array.
	union
	{
		struct ATree *nodes[256];
		uint answers[256];
	};
} ATree;

ATree *SATInit(void);
void SATClose(ATree *tree);

void SATAddMapping(ATree *tree, void *map_from, void *map_to);
void *SATLookup(ATree *tree, void *map_from);
void SATDelete(ATree *tree, void *map_from);
