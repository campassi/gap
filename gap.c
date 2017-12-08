// gcc -std=c11 -lcrypto -Wextra -Wall -pedantic gap.c -o gap
// remove all characters that create newline gaps in file
// limit removal to gaps between quotes (check if quote is escaped)
//// scratch that, just remove all newlines unless after a }
// remove anything not <0x20 

#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

#define INPUT "summary.json"
#define TEMP "temp.json"
#define OUTPUT "output.json"

int input()
{
	// need to read in large blocks for speed enhancement

	// input/output file pointer
	FILE *ifp, *tfp, *ofp;
	// read in binary mode
	if(!(ifp = fopen (INPUT, "rb")))
	{
		printf("INPUT does not exist\n");
		fclose(ifp);
		return 1;
	}
	else
	{
		printf("INPUT successfully opened\n");
	}

	// write in binary mode
	if(!(tfp = fopen (TEMP, "wb")))
	{
		printf("cannot create TEMP\n");
		fclose(tfp);
		return 2;
	}
	else
	{
		printf("TEMP successfully opened\n");
	}
	
	// read in one character at a time from input.json
	int c, prev=0;
	printf("begin reading INPUT, searching <0x20\n");
	while((c = fgetc(ifp)) != EOF)
	{
		// to remove chinese & funky characters change this to
		// if(c < 0x20 || c > 0x7e)
		// this line is the pre-filter
		// 20171201 also add remove backslashes 0x5c
		if(c < 0x20 || c > 0x7e)
		{
			if(c == 0xa /*\n*/)
			{
				fputc(c, tfp);
				continue;
			}
		}
                else if(c == 0x5c /*\*/)
                {
			//I don't know why 0x5c72 "\r" is gumming up the works but this will fix it
                        int next = fgetc(ifp);
                        if(next == 0x72 /*r*/)
                        {
                                //found a \r so don't write any of this to tmp file
                                continue;
                        }
                        else
                        {
				// only a single \ was found so write both to tmp file
                                fputc(c, tfp);
                                fputc(next, tfp);
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
		printf("INPUT successfully closed\n");
	}
	else
	{
		printf("INPUT closure failure\n");
	}
	if(!fclose(tfp))
	{
		printf("TEMP successfully closed\n");
	}
	else
	{
		printf("TEMP closure failure\n");
	}
	if(!(tfp = fopen (TEMP, "rb")))
	{
		printf("TEMP does not exist\n");
		fclose(tfp);
		return 1;
	}
	else
	{
		printf("TEMP successfully reopened, file pointer reset\n");
	}
	if(!(ofp = fopen (OUTPUT, "wb")))
	{
		printf("cannot create OUTPUT\n");
		fclose(ofp);
		return 2;
	}
	else
	{
		printf("OUTPUT successfully opened\n");
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
                printf("TEMP successfully closed\n");
        }
        else
        {
                printf("TEMP closure failure\n");
        }
        if(!fclose(ofp))
        {
                printf("OUTPUT successfully closed\n");
        }
        else
        {
                printf("OUTPUT closure failure\n");
        }


	return 0;
}

int main()
{
	input();
	return 0;
}
