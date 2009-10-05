
#include <stdio.h>

int main(void)
{
FILE *fpi, *fpo;
char lastch=-1;

	fpi = fopen("keywords_constant", "rb");
	fpo = fopen("k", "wb");
	
	while(1)
	{
		char ch = fgetc(fpi);
		if (ch<0) break;
		
		if (ch==' ') ch='\n';
		if (ch=='\r') continue;
		
		if (lastch=='\n' && ch=='\n') {}
		else
		fputc(ch, fpo);
		
		lastch = ch;
	}
	
	fclose(fpi);
	fclose(fpo);
}
