#ifndef _UNPACK_H_
#define _UNPACK_H_

typedef struct
{
	unsigned char id[2];
	unsigned char compressFlag;
	unsigned int width;
	unsigned int height;
	unsigned int size;
} RD_HEADER;

unsigned char *encoder( unsigned char *, unsigned char **,
	unsigned int, unsigned int, unsigned int *, int );
unsigned char *decoder( unsigned char *, unsigned char **,
	unsigned int *, unsigned int *, unsigned int * );
#endif