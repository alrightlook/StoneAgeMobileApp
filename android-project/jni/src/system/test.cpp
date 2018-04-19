#include "../systeminc/version.h"
#include <stdio.h>
#include "test.h"

void test(char *data)
{
	FILE *logfile;
	logfile = fopen("test.txt","a+");
	if(logfile)
	{
		fprintf(logfile,"%s\n",data);
		fclose(logfile);
	}
}