// gcc -std=c11 -lcrypto -Wextra -Wall -pedantic gap.c -o gap
// remove all characters that create newline gaps in file
// limit removal to gaps between quotes (check if quote is escaped)
// 0x0a lf; 0x0b vt; 0x0c ff; 0x0d cr; 0x08 bs; 0x09 tab; 
// or remove anything not >0x1f && <0x7f

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
	if(!(ifp = fopen ("input.txt", "rb")))
	{
		printf("input.txt does not exist\n");
		fclose(ifp);
		return 1;
	}
	else
	{
		printf("input.txt successfully opened\n");
	}

	// write in binary mode
	if(!(tfp = fopen ("temp.txt", "wb")))
	{
		printf("cannot create temp.txt\n");
		fclose(tfp);
		return 2;
	}
	else
	{
		printf("temp.txt successfully opened\n");
	}
	
	// read in one character at a time from input.txt
/*	int c, f=0, g=0;
	while((c = fgetc(ifp)) != EOF)
	{
		// if c == "
		if(c == 0x22)
		{
			fputc(c, ofp);
			while((c = fgetc(ifp)) != EOF )
			{
				switch(c)
				{
					case 0x0a: break; //lf
					case 0x0b: break; //vt
					case 0x0c: break; //ff
					case 0x0d: break; //cr
					case 0x08: break; //bs
					case 0x09: break; //tab
					case 0x5c: //backslash
						f=1; break;
					case 0x22: // quote
						if(f) // if backslash flag previous char
						{
							fputc(c, ofp);
						}
						else
						{ //end of quote stream
							fputc(0x3f, ofp);g=1;//flag to break out of switch
						}	
					default:
					f = g = 0;
					fputc(c, ofp);
				}
				if(g) break;
			}
		}
		else
		{
			fputc(c, ofp);
		}
	}
*/
	int c, f=0, g=0, prev=0;
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
	fclose(ifp);
	fclose(tfp);
	if(!(tfp = fopen ("temp.txt", "rb")))
	{
		printf("temp.txt does not exist\n");
		fclose(tfp);
		return 1;
	}
	if(!(ofp = fopen ("output.txt", "wb")))
	{
		printf("cannot create output.txt\n");
		fclose(ofp);
		return 2;
	}
	else
	{
		printf("output.txt successfully opened\n");
	}
	prev=0;
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
	fclose(tfp);
	fclose(ofp);

	return 0;
}

int input2()
{
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen ( "input.txt" , "rb" );
  if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (char*) malloc (sizeof(char)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile);
  free (buffer);
}

int main()
{
	input();
	//input2();
} 

