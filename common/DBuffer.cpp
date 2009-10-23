
#include <OS.h>
#include <stdlib.h>
#include <string.h>
#include "DBuffer.h"
#include "DBuffer.fdh"

#define INITIAL_SIZE		2048

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

void DBuffer::AppendData(const uchar *data, int length)
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
	AppendData((uchar *)str, strlen(str));
}

void DBuffer::AppendPString(const char *str)
{
	ushort len = strlen(str);
	
	Append16(len);
	AppendData((uchar *)str, len);
}

void DBuffer::AppendBool(bool value)
{
uchar ch = (uchar)value;
	AppendData((uchar *)&ch, 1);
}

void DBuffer::AppendChar(uchar ch)
{
	AppendData((uchar *)&ch, 1);
}

void DBuffer::Append16(ushort value)
{
	AppendData((uchar *)&value, 2);
}

void DBuffer::Append32(uint value)
{
	AppendData((uchar *)&value, 4);
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

void DBuffer::SetTo(const uchar *data, int length)
{
	Clear();
	AppendData(data, length);
}

/*
void c------------------------------() {}
*/

// copies data from the start of the buffer up to the first character matching "ch"
// into "line". the buffer is then advanced to contain only the portion immediately
// after the occurance of "ch". the "ch" character is lost.
// 
// if there is no "ch" in the buffer, "line" is set to a copy of the data in the buffer
// and the function returns nonzero.
//
// this function is useful for reading lines or comma-seperated-values.
bool DBuffer::ReadTo(DBuffer *line, uchar ch, bool add_null=true)
{
	DBuffer dat;
	dat.SetTo(fData, fLength);
	dat.AppendChar(0);
	uchar *ptr = (uchar *)strchr((char *)dat.Data(), ch);
	
	if (ptr)
	{
		ptr++;
		
		int index = (ptr - dat.Data());
		line->SetTo(fData, index - 1);
		this->SetTo(ptr, fLength - index);
	}
	else
	{
		line->SetTo(fData, fLength);
		this->Clear();
	}
	
	if (add_null)
		line->AppendChar(0);
	
	return (ptr != NULL);
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















