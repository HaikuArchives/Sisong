

class DBuffer
{
public:
	DBuffer();
	~DBuffer();
	
	void AppendData(const uchar *data, int length);
	
	void AppendString(const char *str);
	void AppendPString(const char *str);
	
	void AppendBool(bool value);
	void AppendChar(uchar ch);
	void Append16(ushort value);
	void Append32(uint value);
	
	// ---------------------------------------
	
	void Clear();
	uchar *Data();
	int Length();

private:
	uchar *fData;
	int fLength;
	int fAllocSize;
};
