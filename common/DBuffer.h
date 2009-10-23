

class DBuffer
{
public:
	DBuffer();
	~DBuffer();
	
	void SetTo(const uchar *data, int length);
	void AppendData(const uchar *data, int length);
	
	void AppendString(const char *str);
	void AppendPString(const char *str);
	
	void AppendBool(bool value);
	void AppendChar(uchar ch);
	void Append16(ushort value);
	void Append32(uint value);
	
	bool DBuffer::ReadTo(DBuffer *line, uchar ch, bool add_null=true);
	
	// ---------------------------------------
	
	void Clear();
	uchar *Data();
	int Length();

private:
	uchar *fData;
	int fLength;
	int fAllocSize;
};
