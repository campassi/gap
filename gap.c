// gcc -std=c11 -lcrypto -Wextra -Wall -pedantic gap.c -o gap
// remove all characters that create newline gaps in file
// limit removal to gaps between quotes (check if quote is escaped)
//// scratch that, just remove all newlines unless after a }
// remove anything not <0x20 

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

int input()
{
	// need to read in large blocks for speed enhancement

	// input/output file pointer
	FILE *ifp, *tfp, *ofp;
	// read in binary mode
	if(!(ifp = fopen ("input.json", "rb")))
	{
		printf("input.json does not exist\n");
		fclose(ifp);
		return 1;
	}
	else
	{
		printf("input.json successfully opened\n");
	}

	// write in binary mode
	if(!(tfp = fopen ("temp.json", "wb")))
	{
		printf("cannot create temp.json\n");
		fclose(tfp);
		return 2;
	}
	else
	{
		printf("temp.json successfully opened\n");
	}
	
	// read in one character at a time from input.json
	int c, prev=0;
	printf("being reading input file, searching <0x20\n");
	while((c = fgetc(ifp)) != EOF)
	{
		// to remove chinese & funky characters change this to
		// if(c < 0x20 || c > 0x7e)
		if(c < 0x20)
		{
			if(c == 0xa /*\n*/)
			{
				fputc(c, tfp);
				continue;
			}
		}
		else
		{
			fputc(c, tfp);
		}			
	}
	printf("characters <0x20 removed\n");
	if(!fclose(ifp))
	{
		printf("input file successfully closed\n");
	}
	else
	{
		printf("input file closure failure\n");
	}
	if(!fclose(tfp))
	{
		printf("temp file successfully closed\n");
	}
	else
	{
		printf("temp file closure failure\n");
	}
	if(!(tfp = fopen ("temp.json", "rb")))
	{
		printf("temp.json does not exist\n");
		fclose(tfp);
		return 1;
	}
	else
	{
		printf("temp file successfully reopened, file pointer reset\n");
	}
	if(!(ofp = fopen ("output.json", "wb")))
	{
		printf("cannot create output.json\n");
		fclose(ofp);
		return 2;
	}
	else
	{
		printf("output.json successfully opened\n");
	}
	prev=0;
	printf("remove newlines unless after }\n");
	while((c = fgetc(tfp)) != EOF)
	{
		// to remove chinese & funky characters change this to
		// if(c < 0x20 || c > 0x7e)
		if(c == 0xa /*\n*/)
		{
			if(prev == 0x7d /*}*/)
			{
				fputc(c, ofp);
				prev=c;				
				continue;
			}
		}
		else
		{
			fputc(c, ofp);
			prev=c;
		}			
	}
	printf("newlines removed\n");
        if(!fclose(tfp))
        {
                printf("temp file successfully closed\n");
        }
        else
        {
                printf("temp file closure failure\n");
        }
        if(!fclose(ofp))
        {
                printf("output file successfully closed\n");
        }
        else
        {
                printf("output file closure failure\n");
        }


	return 0;
}

int main()
{
	input();
	return 0;
}
