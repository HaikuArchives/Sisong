

typedef struct ATNode
{
	// unlike QSNodes, an ATNode is always exactly 4 levels deep.
	// thus there is no 4th level; once at the 3rd level, all the
	// branches point to answers, using the answer[] array.
	union
	{
		struct ATNode *nodes[256];
		uint answers[256];
	};
} ATNode;

uint ATLookup(ATNode *basenode, uint map_from);

typedef ATNode ATree;
