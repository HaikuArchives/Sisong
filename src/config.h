

class Config : public BMessage
{
public:
	static Config *load();
	static void save(Config *settings);
	
	void SetString(const char *name, const char *value);
	const char *GetString(const char *name, const char *Default);
	
	void SetInt(const char *name, const int value);
	int GetInt(const char *name, const int Default);
};

#define M_SETTINGS		'seti'
extern Config *settings;
