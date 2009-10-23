
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <List.h>

#include "basics.h"
#include "smal.h"
#include "misc.fdh"

void stat(const char *fmt, ...);

unsigned short fgeti(FILE *fp)
{
unsigned short value;
	fread(&value, 2, 1, fp);
	return value;
}

unsigned int fgetl(FILE *fp)
{
unsigned int value;
	fread(&value, 4, 1, fp);
	return value;
}

void fputi(unsigned short word, FILE *fp)
{
	fwrite(&word, 2, 1, fp);
}

void fputl(unsigned int word, FILE *fp)
{
	fwrite(&word, 4, 1, fp);
}



double fgetfloat(FILE *fp)
{
char buf[16];
double *float_ptr;
int i;

	for(i=0;i<4;i++) fgetc(fp);
	for(i=0;i<8;i++) buf[i] = fgetc(fp);
	
	float_ptr = (double *)&buf[0];
	return *float_ptr;
}

void fputfloat(double q, FILE *fp)
{
char *float_ptr;
int i;

	float_ptr = (char *)&q;
	
	for(i=0;i<4;i++) fputc(0, fp);
	for(i=0;i<8;i++) fputc(float_ptr[i], fp);
	
	return;
}


// read a string from a file until a null is encountered
void freadstring(FILE *fp, char *buf, int max)
{
int i;

	--max;
	for(i=0;i<max;i++)
	{
		buf[i] = fgetc(fp);
		if (!buf[i])
		{
			return;
		}
	}
	
	buf[i] = 0;
}

// write a string to a file and null-terminate it
void fputstring(char *buf, FILE *fp)
{
	if (buf[0]) fprintf(fp, "%s", buf);
	fputc(0, fp);
}

// write a string to a file-- does NOT null-terminate it
void fputstringnonull(char *buf, FILE *fp)
{
	if (buf[0]) fprintf(fp, "%s", buf);
}


// read data from a file until CR
void fgetline(FILE *fp, char *str, int maxlen)
{
int k;
	str[0] = 0;
	fgets(str, maxlen - 1, fp);
	
	// trim the CRLF that fgets appends
	for(k=strlen(str)-1;k>=0;k--)
	{
		if (str[k] != 13 && str[k] != 10) break;
		str[k] = 0;
	}
}

// read data from a file until ',' or CR
void fgetcsv(FILE *fp, char *str, int maxlen)
{
int i, j;
char ch;

	maxlen--;
	for(i=j=0;i<maxlen;i++)
	{
		ch = fgetc(fp);
		
		if (ch==13 || ch==',' || ch=='}' || ch==-1)
		{
			break;
		}
		
		if (ch != 10)
		{
			str[j++] = ch;
		}
	}
	
	str[j] = 0;
}

// read a number from a CSV'd list in a file
int fgeticsv(FILE *fp)
{
char buffer[80];
	fgetcsv(fp, buffer, sizeof(buffer));
	return atoi(buffer);
}

double fgetfcsv(FILE *fp)
{
char buffer[80];
	fgetcsv(fp, buffer, sizeof(buffer));
	return atof(buffer);
}

// returns the size of an open file.
int getfilesize(FILE *fp)
{
int cp, sz;

	cp = ftell(fp);
	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);
	fseek(fp, cp, SEEK_SET);
	
	return sz;
}

// reads an entire file into memory.
// returns an allocated buffer to it's contents, and optionally the length of the file.
// the buffer will be null-terminated.
char *read_file(const char *fname, int *size_out)
{
FILE *fp;
char *data;
int file_len;

	fp = fopen(fname, "rb");
	if (!fp)
		return NULL;
	
	file_len = getfilesize(fp);
	
	if (file_len)
	{
		data = (char *)smal(file_len + 1);
		fread(data, file_len, 1, fp);
		data[file_len] = 0;
	}
	else
	{
		data = (char *)smal(1);
	}
	
	if (size_out)
		*size_out = file_len;
	
	fclose(fp);
	return data;
}

// return a random number between min and max inclusive
int randrange(int min, int max)
{
int range, val;

	range = (max - min);
	
	if (range < 0)
	{
		//error("random(): warning: max < min [%d, %d]\n", min, max);
		min ^= max;
		max ^= min;
		min ^= max;
		range = (max - min);
	}
	
	if (range >= RAND_MAX)
	{
		//error("random(): range > RAND_MAX\n", min, max);
		return 0;
	}
	
	val = rand() % (range + 1);
	return val + min;
}


void strtoupper(char *str)
{
int i;
	for(i=strlen(str)-1;i>=0;i--) str[i] = toupper(str[i]);
}

void strtolower(char *str)
{
int i;
	for(i=strlen(str)-1;i>=0;i--) str[i] = tolower(str[i]);
}

char *stristr(const char *String, const char *Pattern)
{
char *pptr, *sptr, *start;

	for (start = (char *)String; *start; start++)
	{
		/* find start of pattern in string */
		for ( ; (*start && (toupper(*start) != toupper(*Pattern))); start++)
			  ;
		if (!*start)
			  return 0;
		
		pptr = (char *)Pattern;
		sptr = (char *)start;
		
		while(tolower(*sptr) == tolower(*pptr))
		{
			sptr++;
			pptr++;
			
			/* if end of pattern then pattern was found */
			if (!*pptr)
				return (start);
		}
	}
	return 0;
}

bool file_exists(const char *filename)
{
FILE *fp;

	fp = fopen(filename, "rb");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	
	return false;
}

// given a full path to a file, return onto the name of the file.
// the pointer returned is within the original string.
const char *GetFileSpec(const char *file_and_path)
{
	char *ptr = strrchr(file_and_path, '/');
	if (ptr)
		return ptr+1;
	else
		return file_and_path;
}

// given a full path to a file, return the path part without the filename.
// the pointer returned is an allocated area of memory which you need to frees().
char *RemoveFileSpec(const char *input_file)
{
	char *buffer = smal_strdup(input_file);
	char *ptr = strrchr(buffer, '/');
	if (ptr) *(ptr + 1) = 0;
	return buffer;
}

// if the file does not exist, creates it
void touch(const char *filename)
{
FILE *fp;

	if (!file_exists(filename))
	{
		if ((fp = fopen(filename, "wb")))
			fclose(fp);
	}
}


// works like QBasic's string$() command.
// returns a string consisting of ch, repeated num times.
char *fillstr(char *buffer, int num, char ch)
{
	memset(buffer, ch, num);
	buffer[num] = 0;
	
	return buffer;
}

// iterates over every entry in a BList, and passes each to frees().
// then clears the list.
void FreeBList(BList *list)
{
int i, count = list->CountItems();

	for(i=0;i<count;i++)
		frees(list->ItemAt(i));
	
	list->MakeEmpty();
}

/*
char *GetStaticStr(void)
{
static int counter = 0;
static struct
{
	char str[1024];
} bufs[24];

	if (++counter >= 24) counter = 0;
	return bufs[counter].str;
}*/

// a strncpy that works as you might expect
void maxcpy(char *dst, const char *src, int maxlen)
{
int len = strlen(src);

	if (len >= maxlen)
	{
		if (maxlen >= 2) memcpy(dst, src, maxlen - 2);
		if (maxlen >= 1) dst[maxlen - 1] = 0;
	}
	else
	{
		memcpy(dst, src, len + 1);
	}
}


/*
// strips leading tabs and whitespace from "string"
void LTrim(char *string)
{
	_asm
	{
		// check if string starts with whitespace
		mov		esi, [string]
		mov		edi, esi
		lodsb
		cmp		al, 9
		je		ContainsWhitespace
		cmp		al, ' '
		jne		done
ContainsWhitespace:

		// find end of whitespace
searchloop:
		lodsb
		cmp		al, 9
		je		searchloop
		cmp		al, ' '
		je		searchloop

// move string back:
// store first char
		stosb
		test	al, al
		jz		done

moveloop:
		lodsb					// load a new byte of string
		stosb					// store to start of string
		test	al, al			// reached end of string?
		jnz		moveloop

done:
	}
}

// strips trailing tabs and whitespace from "string"
void RTrim(char *string)
{
char *endofstring;

	endofstring = (strchr(string, 0) - 1);
	
	_asm
	{
		mov		edx, [string]
		mov		eax, [endofstring]

striploop:
		cmp		eax, edx			// have we reached the beginning of the string?
		je		done				// if so we're done
		
		// if this char isn't whitespace, exit the loop
		cmp		byte ptr [eax], 9
		je		is_whitespace
		cmp		byte ptr [eax], ' '
		jnz		done

is_whitespace:
		dec		eax
		jmp		striploop

done:
		// null-terminate the string at the new position
		inc		eax
		mov		byte ptr [eax], 0
	}
}


// returns nonzero if string haystack begins with string needle
char strbegin(char *haystack, char *needle)
{
char result;

	_asm
	{
		mov		esi, [haystack]
		mov		edi, [needle]
cmploop:
		lodsb
		mov		bl, [edi]
		inc		edi
		
		test	al, al
		jz		stopit
		test	bl, bl
		jz		stopit
		cmp		al, bl
		je		cmploop
stopit:

		dec		edi
		mov		al, byte ptr [edi]
		test	al, al
		setz	al
		mov		[result], al
	}
	
	return result;
}
*/


bool strbegin(const char *bigstr, const char *smallstr)
{
int i;
	for(i=0;smallstr[i];i++)
		if (bigstr[i] != smallstr[i]) return false;
	return true;
}

// pads str with ch so that it is at least len characters long
void strpad(char *str, uchar ch, int len)
{
int curlen, addamt;

	curlen = strlen(str);
	addamt = (len - curlen);
	if (addamt > 0)
	{
		str += curlen;
		memset(str, ch, addamt);
		str[addamt] = 0;
	}
}

// strips enclosing quotes from str and returns a pointer to the result.
// the original string is modified.
char *stripquotes(char *str)
{
char *end;

	if (str[0]=='\"') str++;
	end = strchr(str, 0) - 1;
	if (*end=='\"') *end = 0;
	
	return str;
}

/*
void smemset(void *dst, ushort pattern, int numwords)
{
	_asm	// memset attributes with a repeating short
	{
		mov		ax, [pattern]
		mov		edi, [dst]
		shl		eax, 16
		
		mov		ecx, [numwords]
		mov		ax, [pattern]
		sar		ecx, 1
		
		rep		stosd
		
		jnc		notodd
		mov		[edi], ax
notodd:
	}
}

void dmemset(void *dst, uint pattern, int numdwords)
{
	_asm	// memset attributes with a repeating int
	{
		mov		edi, [dst]
		mov		ecx, [numdwords]
		mov		eax, [pattern]
		rep		stosd
	}
}
*/

/*
// attempts to convert the number "str" into an integer.
// supports both decimal numbers and hexadecimal numbers (denoted by '0x' prefix).
// if the input string is not a number, returns nonzero.
char SmartAtoi(char *str, int *out)
{
int i;
int len = strlen(str);
int ch;
char sign;

	if (str[0]=='-') { sign = -1; str++; len--; } else sign = 1;
	
	if (str[0]=='0' && str[1]=='x')
	{		// hex number
		int place = 1;
		int value = 0;
		
		for(i=2;i<len;i++)
		{
			if (str[i] < '0' || str[i] > '9')
			{
				ch = toupper(str[i]);
				if (ch < 'A' || ch > 'F') return 1;
			}
		}
		
		for(i=len-1;i>=2;i--)
		{
			ch = toupper(str[i]);
			
			if (ch >= '0' && ch <= '9') ch -= '0'; else ch -= ('A' - 10);
			value += (ch * place);
			place *= 16;
		}
		
		*out = value * sign;
		return 0;
	}
	else
	{		// decimal number
		for(i=0;i<len;i++)
			if (str[i] < '0' || str[i] > '9') return 1;
		
		*out = atoi(str) * sign;
		return 0;
	}
}
*/


// reads strlen(str) bytes from file fp, and returns true if they match "str"
char fverifystring(FILE *fp, const char *str)
{
int i;
char result = 1;
int stringlength = strlen(str);

	for(i=0;i<stringlength;i++)
	{
		if (fgetc(fp) != str[i]) result = 0;
	}
	
	return result;
}


// returns a pointer to the character within fname which is just after the last slash.
// ie, it gives you a string to the filename without the path part.
// if there are no slashes in filename, returns filename.
char *GetFileComponent(char *filename)
{
char *strippedfilename;

	strippedfilename = strrchr(filename, '\\');
	if (!strippedfilename) strippedfilename = strrchr(filename, '/');
	
	if (strippedfilename) strippedfilename++;
	else strippedfilename = filename;
	
	return strippedfilename;
}

// inserts a null in "filename" at the last "\", leaving you with only the path component
void StripFileComponent(char *filename)
{
	*GetFileComponent(filename) = 0;
}

char IsAbsolutePath(char *path)
{
	if (path[1]==':' && toupper(path[0]) >= 'A' && toupper(path[0]) <= 'Z') return 1;
	if (path[0] == '/' || path[0] == '\\') return 1;
	
	return 0;
}



void hexdump(uchar *data, int len)
{
	hexdump_fp(data, len, stat);
}


void hexdump_fp(uchar *data, int len, void (*printfunc)(char *fmt, ...))
{
int i;
int off = 0;
uchar *linedata;
char line[800];
char *ptr;

	do
	{
		linedata = &data[off];
		sprintf(line, "  %02X: ", off);
		
		// print 16 chars of hex data
		for(i=0;i<16;i++)
		{
			if (off+i < len)
				sprintf(line, "%s%02x", line, linedata[i]);
			else
				strcat(line, "  ");
			
			if (i & 1) strcat(line, " ");
		}
		
		strcat(line, "    ");
		
		// print the same chars again, as ASCII data
		ptr = &line[strlen(line)];
		for(i=0;i<16;i++)
		{
			if (off+i >= len) break;
			*(ptr++) = ((linedata[i] > 30 && linedata[i] < 129) ? linedata[i] : '.');
		}
		
		//*(ptr++) = '\n';
		*ptr = 0;
		
		printfunc("%s", line);
		off += 0x10;
	}
	while(off < len);
}



int boolbyte, boolmask_r, boolmask_w;

// prepare for a boolean read operation
void fresetboolean(void)
{
	boolmask_r = 256;
	boolmask_w = 1;
	boolbyte = 0;
}

// read a boolean value (a single bit) from a file
char fbooleanread(FILE *fp)
{
char value;

	if (boolmask_r==256)
	{
		boolbyte = fgetc(fp);
		boolmask_r = 1;
	}
	
	value = (boolbyte & boolmask_r) ? 1:0;
	boolmask_r <<= 1;
	return value;
}

void fbooleanwrite(char bit, FILE *fp)
{
	if (boolmask_w==256)
	{
		fputc(boolbyte, fp);
		boolmask_w = 1;
		boolbyte = 0;
	}
	
	if (bit)
	{
		boolbyte |= boolmask_w;
	}
	
	boolmask_w <<= 1;
}

void fbooleanflush(FILE *fp)
{
	fputc(boolbyte, fp);
	boolmask_w = 1;
}



