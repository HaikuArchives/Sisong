

class ATree
{
public:
	ATree();
	~ATree();

	void AddMapping(void *map_from, void *map_to);
	void *Lookup(ATNode *basenode, void *map_from);
	void DeleteMapping(void *map_from);

private:

	// unlike QSNodes, an ATNode is always exactly 4 levels deep.
	// thus there is no 4th level; once at the 3rd level, all the
	// branches point to answers, using the answer[] array.
	union
	{
		ATree *nodes[256];
		void *answers[256];
	};

};
