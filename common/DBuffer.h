

class DBuffer
{
public:
	DBuffer();
	~DBuffer();

	void Append(const uchar *data, int length);

	void AppendString(const char *str);
	void AppendPString(const char *str);

	void AppendChar(uchar ch);
	void Append16(ushort value);
	void Append32(uint value);

	// ---------------------------------------

	void Clear();
	uchar *Data();
	int Length();

private:
	uchar *fData;
	uint fLength;
	uint fAllocSize;
};
