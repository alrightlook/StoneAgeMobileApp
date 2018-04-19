#ifndef _TOOL_H_
#define _TOOL_H_

#include<stdio.h>
bool IsDBCSLeadByte(unsigned char str);
#if 0
//??????????希?��???
void freadline( char *ptbuf , FILE *ptFile );
#endif
unsigned int timeGetTime();
//??更???????ㄅ??????????
int wordchk( char **pp );
//?更???可?????更???刺????去??
int strstr2( char **pp , char *q );
//??更????????希???????????去
int getword(char **pp , char *q);
//??更????????希???π???刺??
int strint2( char **pp );
//??更????????希??????long??π???刺??
long strlong2( char **pp );

int getStringToken( char *src , char delim , int count , int maxlen , char *out );
int getIntegerToken( char *src , char delim , int count );
double getDoubleToken( char *src , char delim , int count );
void chop( char *src );
void shiftIntArray( int *a , int siz , int count );

int a62toi( char * );
int getInteger62Token( char *, char, int );


void jEncode( char *, int, int, char *, int *, int );
void jDecode( char *, int, int, char *, int * );

int strncmpi( char *s1 , char *s2 , int len );
void insertString( char *buffer, char *string, int whereToInsert );
void insertChar( char *buffer, char character, int whereToInsert );
int isOnlySpaceChars( char *data );

char makeCharFromEscaped( char );

char*   makeStringFromEscaped( char* src );
char*   makeEscapeString( char* src , char* dest, int sizeofdest);


#endif /*_TOOL_H_*/
