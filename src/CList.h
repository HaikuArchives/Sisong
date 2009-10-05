
class CList : public BList
{
public:
	CList();
	CList(void *FreeVia);
	
	// return the last item from the list,
	// and remove it from the list, but do not
	// free it.
	void *pop();
	// remove every item from the list, passing
	// each in turn to "FreeFunc" if present.
	void free();
	
	void (*FreeFunc)(void *object);
};
