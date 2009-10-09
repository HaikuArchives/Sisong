
class QSTree
{
public:
	QSTree();
	~QSTree();

	void AddMapping(const char *str, void *answer);
	void AddMapping(const char *str, int32 answer);

	void *Lookup(const char *str);
	int32 LookupInt(const char *str);

	void Delete(const char *str);

private:
	QSTree *branch[256];
	void *answer;
	bool has_answer;
};
