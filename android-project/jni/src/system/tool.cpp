#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/* WIN32_LEAN_AND_MEAN?define???WINDOWS.H???
????úé?????§Æ??????????????
????????????????? */
#define WIN32_LEAN_AND_MEAN 
//#include <windows.h>
//#include <Mmsystem.h>

#include "../systeminc/version.h"
#include "../systeminc/tool.h"
#include "../systeminc/system.h"

#ifdef _TIME_GET_TIME
LARGE_INTEGER tickCount;
LARGE_INTEGER CurrentTick;
#endif

unsigned int TimeGetTime(void)
{
#ifdef _TIME_GET_TIME
	static __int64 time;

	QueryPerformanceCounter(&CurrentTick);
	return (unsigned int)(time = CurrentTick.QuadPart/tickCount.QuadPart);
#else
	return timeGetTime();
#endif
}
bool IsDBCSLeadByte(unsigned char str)
{
    if ( 0x81 < str && str < 0xfe)
    {
        return true;
    }
    else {
        return false;
    }
}
static int copyStringUntilDelim( unsigned char *, char delim, int, unsigned char * );


// ??§ó???????£t??????????
int wordchk( char **pp )
{
	char *p = *pp;

	while( *p != 0 )
	{
		switch( *p )
		{
			case '\t':
				p++;
				break;

			case ' ':
				p++;
				break;

			default:
				*pp = p;
				return 1;
		}
	}

	return 0;
}


// ??§ó????????§Æ???????????¥h
int getword( char **pp, char *q )
{
	int i=0;
	char *p = *pp;

	wordchk( &p );

	for( i = 0; *p != ' ' && *p != '\t' && *p != '\0'; p++ , q++ , i++ )
	{
		*q = *p;
	}

	*q = '\0';
	*pp = p;

	return i;
}


#if 0
// ?????????
void freadline( char *ptbuf, FILE *fp )
{
	char buf;
	int i;

	for( i = 0; ; i++ )
	{
		if( feof( fp ) != 0 )
		{
			*ptbuf--;
			*ptbuf = '\0';
			break;
		}

		fread( &buf, 1, sizeof( unsigned char ), fp );
		if( buf == ' ' )
		{
			*ptbuf++ = buf;
		}else if (buf == '\t'){
			*ptbuf++ = buf;
		}else if (buf != 0x0d && buf != 0x0a){
			*ptbuf++ = buf;
		}else{
			*ptbuf++ = '\0';
			break;
		}
	}
	while ( feof(fp ) == 0 ){
		fread( &buf ,1 , sizeof(unsigned char) , fp );
		if (buf != 0x0d && buf != 0x0a ){
			fseek( fp, -1, SEEK_CUR);
			break;
		}
	}
}
#endif


// ?§ó???¥i?????§ó???¨ë????¥h??
int strstr2( char **pp , char *q )
{
	char *p = *pp;
	int i;

	wordchk( &p );

	for( i = 1; *p++ == *q++; i++ )
	{
		if( *q == 0 )
		{
			*pp = p;
			return i;
		}
	}

	return 0;
}


// ??§ó????????§Æ???????£k???¨ë??
int strint2( char **pp )
{
	char moji[1024] = "";
	char *p = *pp;

	getword( &p , moji );
	*pp = p;

	return atoi(moji);
}


// ??§ó????????§Æ??????long??£k???¨ë??
long strlong2( char **pp )
{
	char moji[1024] = "";
	char *p = *pp;

	getword( &p , moji );
	*pp = p;

	return atol(moji);
}

// ?þú?§ó??£S??????????¨ë?
//   ??£k£ºNULL?? ... ?þú?§ó????????
//           NULL     ... §ó???ü¬£Z???
inline unsigned char *searchDelimPoint( unsigned char *src, unsigned char delim )
{
	unsigned char *pt = src;

	while( 1 )
	{
		if( *pt == '\0' )
			return (unsigned char *)0;

		if( *pt < 0x80 )
		{
			// 1bayte§ó????
			if( *pt == delim )
			{
				// ?þú?§ó???????????????¨ë?
				pt++;
				return pt;
			}
			pt++;
		}
		else
		{
			// 2byte§ó????
			pt++;
			if( *pt == '\0' )
				return (unsigned char *)0;
			pt++;
		}
	}
}


/*
   ????§ó???????????????????úù¤e????????§ó???
   ??????????

  char *src : ???§ó??
  char delim : ????
  int count : ????????????????
  int maxlen : out ??????¢†§ó?????
  char *out : üÒ?


  ??£k  0£º?????ü¬???
          1:§ó??ü¬£Z?ü¬???
*/
int getStringToken( char *src, char delim, int count, int maxlen, char *out )
{
	int c = 1;
	int i;
	unsigned char *pt;

	pt = (unsigned char *)src;
	for( i = 0; i < count-1; i++ )
	{
		if( pt == (unsigned char *)0 )
			break;

		pt = searchDelimPoint( pt, delim );
	}

	if( pt == (unsigned char *)0 )
	{
		out[0] = '\0';
		return 1;
	}

	return copyStringUntilDelim( pt, delim, maxlen, (unsigned char *)out );
}


/*
  §ó????§ó????§ó?????????????????????

  char *src : ??§ó??
  char delim : ????
  int maxlen : ?¢†??????
  char *out : üÒ?

  ??£k  0£º?????ü¬???
          1:§ó??ü¬£Z?ü¬???
*/
static int copyStringUntilDelim( unsigned char *src, char delim,
	int maxlen, unsigned char *out )
{
	int i;

	for( i = 0; i < maxlen; i++ )
	{
		if( src[i] < 0x80 )
		{
			// 1byte§ó????

			if( src[i] == delim )
			{
				// ?þú?§ó????ü¬??
				out[i] = '\0';
				return 0;
			}

			// ???§ó?????
			out[i] = src[i];

			// ü¬£Z§ó???ü¬??
			if( out[i] == '\0' )
				return 1;
		}
		else
		{
			// 2byte§ó????

			// ???§ó?????
			out[i] = src[i];

			i++;
			if( i >= maxlen )	// ??????¦Ž???ü¬??
				break;

			// ???§ó?????
			out[i] = src[i];

			// ü¬£Z§ó???ü¬????????????????
			if( out[i] == '\0' )
				return 1;
		}
	}

	out[i] = '\0';

	return 1;
}


/*
  þÐ???????int£k??????

  char *src : ???§ó??
  char delim: ???????§ó??
  int count :????????? ??????????

  return value : £k

*/
int getIntegerToken( char *src, char delim, int count )
{
	char s[128];

	getStringToken( src, delim, count, sizeof( s )-1, s );

	if( s[0] == '\0' )
		return -1;

	return atoi( s );
}


/*
  double???????????
  char *src : ???§ó??
  char delim: ???????§ó??
  int count :????????? ??????????

  return value: £k
*/
double getDoubleToken( char *src, char delim, int count )
{
	char s[128];

	getStringToken( src, delim, count, sizeof( s )-1, s );

	return strtod( s , NULL );
}


/*
  chop??(UNIX?û°?§ó??)

  char *src : ??§ó???  ???????£šþü¨Äüº???????

*/
void chop( char *src )
{
	int i;

	for( i = 0; ; i++ )
	{
		if( src[i] == 0 )
			break;
		if( src[i] == '\n' && src[i+1] == '\0' )
		{
			src[i] = '\0';
			break;
		}
	}
}


/*
int?¦”?????????

  int *a : ¦”??????
  int siz :  ¦”?????
  int count : ???????????
  ¦”????????????
*/
void shiftIntArray( int *a, int siz, int count )
{
	int i;

	for( i = 0; i < siz - count; i++ )
	{
		a[i] = a[i+count];
	}
	for( i = siz - count; i <siz; i++ )
	{
		a[i] = 0;
	}
}


// 62þ­§ó???int?¨Á???
// 0-9,a-z(10-35),A-Z(36-61)
int a62toi( char *a )
{
	int ret = 0;
	int fugo = 1;

	while( *a != NULL )
	{
		ret *= 62;
		if( '0' <= (*a) && (*a) <= '9' )
			ret += (*a)-'0';
		else
		if( 'a' <= (*a) && (*a) <= 'z' )
			ret += (*a)-'a'+10;
		else
		if( 'A' <= (*a) && (*a) <= 'Z' )
			ret += (*a)-'A'+36;
		else
		if( *a == '-' )
			fugo = -1;
		else
			return 0;
		a++;
	}
	return ret*fugo;
}


/*
  þÐ???????int£k??????

  char *src : ???§ó??
  char delim: ???????§ó??
  int count :????????? ??????????

  return value : £k

*/
int getInteger62Token( char *src, char delim, int count )
{
	char  s[128];

	getStringToken( src, delim, count, sizeof( s )-1, s );
	if( s[0] == '\0' )
		return -1;

    return a62toi( s );
}


/*
  üí§ó??¢†§ó??????§ó???§{????

  char *s1 : §ó???
  char *s2 : §ó???
  int len : ?¢†§{?§ó???


*/
int strncmpi( char *s1, char *s2, int len )
{
	int i;
	int c1, c2;

	for( i = 0; i < len; i++ )
	{
		if( s1[i] == '\0' || s2[i] == '\0' )
			return 0;

		c1 = tolower( s1[i] );
		c2 = tolower( s2[i] );

		if( c1 != c2 )
			return 1;
	}

	return 0;
}



/*****************************************************************
  ???????§ó??????1?¢\?§ó????????
  0?¨ë??SJIS??
******************************************************************/
int isOnlySpaceChars( char *data )
{
	int i = 0;
	int returnflag = 0;

	while( data[i] != '\0' )
	{
		returnflag = 0;
		if( (unsigned char)data[i] == ' ' )
			returnflag = 1;
		if( IsDBCSLeadByte( data[i] ) )
		{
			if( (unsigned char)data[i] == 0x81
			 && (unsigned char)data[i+1] == 0x40 )
			{
				returnflag = 1;
			}
			i++;
		}
		if( returnflag == 0 )
			return 0;
		i++;
	}

	if( i == 0 )
		return 0;

	return returnflag;
}


/****************************
  §ó????????¡ ¦V???
  buffer: ????
  string: ¡ ¦V??§ó??
  whereToinsert: ???¡ ¦V???
****************************/
void insertString( char *buffer, char *string, int whereToInsert )
{
	int stringLength, bufferLength, i;

	stringLength = strlen( string );
	bufferLength = strlen( buffer );

	for( i = 0; i <= bufferLength - whereToInsert; i++ )
	{
		buffer[bufferLength+stringLength-i] = buffer[bufferLength-i];
	}
	for( i = 0; i < stringLength; i++ )
	{
		buffer[whereToInsert+i] = string[i];
	}
}


/****************************
  §ó???????¡ ¦V???
  buffer: ????
  character: ¡ ¦V??§ó??
  whereToinsert: ???¡ ¦V???
****************************/
void insertChar( char *buffer, char character, int whereToInsert )
{
	int bufferLength, i;

	bufferLength = strlen( buffer );

	for( i = 0; i <= bufferLength - whereToInsert; i++ )
	{
		buffer[bufferLength+1-i] = buffer[bufferLength-i];
	}
	buffer[whereToInsert] = character;
}


/*************************************************************
??????????????????
*************************************************************/
typedef struct tagEscapeChar
{
	char escapechar;
    char escapedchar;
} EscapeChar;

static EscapeChar escapeChar[] =
{
	{ '\n',   'n' },
	{ ',',    'c' },
	{ '|',    'z' },
	{ '\\',   'y' },
};


// ?????§ó??¤GýÐ§ó????
//   ??£k£º¤GýÐ§ó?(?????§ó?????????§ó??¨ë?)
char makeCharFromEscaped( char c )
{
	int i;

	for( i = 0; i < sizeof( escapeChar )/sizeof( escapeChar[0] ); i++ )
	{
		if( escapeChar[i].escapedchar == c )
		{
			c = escapeChar[i].escapechar;
			break;
		}
	}

	return c;
}



/*----------------------------------------
 * makeEscapeString??þÎ???§ó???????
 * ??
 *  src             char*       ????§ó????????¢v??
 * ¨ë?£k
 *  src    ?¨ë??(???¦V??????????)
 ----------------------------------------*/

unsigned int timeGetTime()
{
    return SDL_GetTicks();
}

char *makeStringFromEscaped(char *src)
{
	int		srclen=strlen(src);
	int		searchindex=0;
	for(int i=0;i<srclen;i++){
		if(IsDBCSLeadByte(src[i])){
			src[searchindex++]=src[i++];
			src[searchindex++]=src[i];
		}else{
			if(src[i]=='\\'){
				int j;
				i++;
				for(j=0;j<sizeof(escapeChar)/sizeof(escapeChar[0]);j++)
					if(escapeChar[j].escapedchar==src[i]){
						src[searchindex++]=escapeChar[j].escapechar;
						goto NEXT;
					}
				src[searchindex++]=src[i];
			}else
				src[searchindex++]=src[i];
		}
NEXT:
		;
	}
	src[searchindex]='\0';
	return src;
}


/*----------------------------------------
 * ????????
 * ??
 *  src             char*       ????§ó??
 *  dest            char*       ????????§ó??
 *  sizeofdest      int         dest ? ???
 * ¨ë?£k
 *  dest    ?¨ë??(???¦V??????????)
 ----------------------------------------*/
char *makeEscapeString( char *src, char *dest, int sizeofdest )
{
	int		srclen=strlen(src);
	int		destindex=0;
	for(int i=0;i<srclen;i++){
		if(destindex+1 >= sizeofdest)
			break;
		if(IsDBCSLeadByte(src[i])){
			if(destindex+2 < sizeofdest){
				dest[destindex++]=src[i++];
				dest[destindex++]=src[i];
			}else
				break;
		}else{
			bool dirty=false;
			char escapechar='\0';
			for(int j=0;j<sizeof(escapeChar)/sizeof(escapeChar[0]);j++)
				if(src[i]==escapeChar[j].escapechar){
					dirty=true;
					escapechar=escapeChar[j].escapedchar;
					break;
				}
			if(dirty==true){
				if( destindex + 2 < sizeofdest ){
					dest[destindex++]='\\';
					dest[destindex++]=escapechar;
				}else
					break;
			}else
				dest[destindex++]=src[i];
		}
    }
    dest[destindex] = '\0';
    return dest;
}


/***************************************************************
	????????? by Jun
***************************************************************/
/*
char *src	þØ????adress
int srclen	þØ????src???£•?
int keystring	????????????
char *encoded	????????§ó????¦t??????????
int *encodedlen	????????§ó???£•????????? 
int maxencodedlen ?¢†£•?????????
*/
void jEncode( char *src, int srclen, int key,
	char *encoded, int *encodedlen, int maxencodedlen )
{
	char sum = 0;
	int i;

	if( srclen+1 > maxencodedlen )
	{
		// ???????£•?? 1 ????
		// ????????????????£O????????
		*encodedlen = maxencodedlen;
		for( i = 0; i < (*encodedlen); i++ )
		{
			encoded[i] = src[i];
		}
	}

	if( srclen+1 <= maxencodedlen )
	{
		// ??????????
		*encodedlen = srclen + 1;
		for( i = 0; i < srclen; i++ )
		{
			sum = sum + src[i];
			// ¡I¥d??7?¦•????2?¦•???????????§T¤š
			if( ((key%7) == (i%5)) || ((key%2) == (i%2)) )
			{
				src[i] = ~src[i];
			}
		}
		for( i = 0; i < (*encodedlen); i++ )
		{
			if( abs( (key%srclen) ) > i )
			{
				encoded[i] = src[i] + sum*((i*i)%3);	// ?????????
			}
			else
			// key%srclen??£t?¡u??????????£t?
			if( abs( (key%srclen) ) == i )
			{
				encoded[i] = sum;
			}
			else
			if( abs( (key%srclen) ) < i )
			{
				encoded[i] = src[i-1] + sum*((i*i)%7);	// ?????????
			}
		}
	}
}


/*
char *src	?????????
int srclen	??????????????£•?
int key		????????¥i???
char *decoded	???????§ó???üÒ?
int *decodedlen	???????§ó???£•?
*/
void jDecode( char *src, int srclen, int key, char *decoded, int *decodedlen )
{
	char sum;
	int i;

	*decodedlen = srclen - 1;
	sum = src[abs( key%(*decodedlen) )];

	for( i=0; i < srclen; i++ )
	{
		if( abs( (key%(*decodedlen)) ) > i )
		{
			decoded[i] = src[i] - sum*((i*i)%3);
		}

		if( abs( (key%(*decodedlen)) ) < i )
		{
			decoded[i-1] = src[i] - sum*((i*i)%7);
		}
	}
	for( i = 0; i < (*decodedlen); i++ )
	{
		if( ((key%7) == (i%5)) || ((key%2) == (i%2)) )
		{
			decoded[i] = ~decoded[i];
		}
	}
}
