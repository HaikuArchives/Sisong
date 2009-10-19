

struct ATNode
{
	// unlike QSNodes, an ATNode is always exactly 4 levels deep.
	// thus there is no 4th level; once at the 3rd level, all the
	// branches point to answers, using the answer[] array.
	union
	{
		ATNode *nodes[256];
		void *answers[256];
	};
};


class ATree
{
public:

	ATree();
	~ATree();
	
	void AddMapping(void *map_from, void *map_to);
	void *Lookup(void *map_from);
	void Delete(void *object_to_delete);

private:
	ATNode *BaseNode;
};
