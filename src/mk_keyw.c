
#include <stdio.h>

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

int main(void)
{
FILE *fpi, *fpo;
char line[80];

	fpi = fopen("keywords_constant", "rb");
	fpo = fopen("keywords_constant.h", "wb");
	
	if (!fpi || !fpo)
	{
		printf("egads!\n");
		return 1;
	}
	
	fprintf(fpo, "static const char *keywords_constant[] = {\n");
	
	for(;;)
	{
		fgetline(fpi, line, sizeof(line));
		if (line[0] == 0) break;
		
		fprintf(fpo, "\t\"%s\",\n", line);
	}
	
	fprintf(fpo, "NULL\n");
	fprintf(fpo, "};\n\n");
	
	fclose(fpi);
	fclose(fpo);
}
