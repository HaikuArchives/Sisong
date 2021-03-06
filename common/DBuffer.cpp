
#include <OS.h>
#include <stdlib.h>
#include <netdb.h>
#include "DBuffer.h"

#define INITIAL_SIZE		4096

DBuffer::DBuffer()
{
	fAllocSize = INITIAL_SIZE;
	fLength = 0;

	fData = (uchar *)malloc(fAllocSize);
}

DBuffer::~DBuffer()
{
	free(fData);
	fData = NULL;
}

/*
void c------------------------------() {}
*/

void DBuffer::Append(const uchar *data, int length)
{
	int required_size = fLength + (length - 1);
	if (required_size > fAllocSize)
	{
		fAllocSize = (required_size * 2);
		fData = (uchar *)realloc(fData, fAllocSize);
	}

	memcpy(&fData[fLength], data, length);
	fLength += length;
}

void DBuffer::AppendString(const char *str)
{
	Append((uchar *)str, strlen(str));
}

void DBuffer::AppendPString(const char *str)
{
	ushort len = strlen(str);

	Append16(len);
	Append((uchar *)str, len);
}

void DBuffer::AppendChar(uchar ch)
{
	Append((uchar *)&ch, 1);
}

void DBuffer::Append16(ushort value)
{
	Append((uchar *)&value, 2);
}

void DBuffer::Append32(uint value)
{
	Append((uchar *)&value, 4);
}


void DBuffer::Clear()
{
	if (fAllocSize > INITIAL_SIZE)
	{
		fAllocSize = INITIAL_SIZE;

		free(fData);
		fData = (uchar *)malloc(fAllocSize);
	}

	fLength = 0;
}

/*
void c------------------------------() {}
*/

uchar *DBuffer::Data()
{
	return fData;
}

int DBuffer::Length()
{
	return fLength;
}





