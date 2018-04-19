// Arminius' protocol utilities ver 0.1
//
// Any questions and bugs, mailto: arminius@mail.hwaei.com.tw

// -------------------------------------------------------------------
// The following definitions is to define game-dependent codes.
// Before compiling, remove the "//".
#define __STONEAGE

////#include <windows.h>
#include <stdio.h>

#include "autil.h"

#ifdef __STONEAGE
#include "../systeminc/version.h"
#include "../systeminc/lssproto_util.h"
//#include "../systeminc/common.h"
#endif
#include "SDL.h"

char **MesgSlice;
int SliceCount;

char PersonalKey[32];

#ifdef _DEBUG
extern int g_iMallocCount;
#endif

// -------------------------------------------------------------------
// Initialize utilities
//
void util_Init(void)
{
  int i;
  
  MesgSlice = (char **) malloc(sizeof(char *) * SLICE_MAX);
#ifdef _DEBUG
	g_iMallocCount++;
#endif
  for (i=0; i<SLICE_MAX; i++){
    MesgSlice[i] = (char *) malloc(SLICE_SIZE);
#ifdef _DEBUG
		g_iMallocCount++;
#endif
	}
  SliceCount = 0;
}

// -------------------------------------------------------------------
// Split up a message into slices by spearator.  Store those slices
// into a global buffer "char **MesgSlice"
//
// arg: source=message string;  separator=message separator (1 byte)
// ret: (none)
void util_SplitMessage(char *source, char *separator)
{
  if (source && separator) {	// NULL input is invalid.
    char *ptr;
    char *head = source;
    
    while ((ptr = (char *) strstr(head, separator)) && (SliceCount<=SLICE_MAX)) {
      ptr[0] = '\0';
      if (strlen(head)<SLICE_SIZE) {	// discard slices too large
        strcpy(MesgSlice[SliceCount], head);
        SliceCount++;
      }
      head = ptr+1;
    }
    strcpy(source, head);	// remove splited slices
  }
}

// -------------------------------------------------------------------
// Encode the message
//
// arg: dst=output  src=input
// ret: (none)
void util_EncodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);
  
  int rn = rand()%99;
  int t1, t2;
  char t3[65500], tz[65500];

#ifdef _BACK_VERSION
  util_swapint(&t1, &rn, "3421");	// encode seed
#else
  util_swapint(&t1, &rn, "2413");	// encode seed
#endif
//  t2 = t1 ^ 0x0f0f0f0f;
  t2 = t1 ^ 0xffffffff;
  util_256to64(tz, (char *) &t2, sizeof(int), DEFAULTTABLE);
  
  util_shlstring(t3, src, rn);
//  printf("random number=%d\n", rn);
  strcat(tz, t3);
  util_xorstring(dst, tz);
  
}

// -------------------------------------------------------------------
// Decode the message
//
// arg: dst=output  src=input
// ret: (none)
void util_DecodeMessage(char *dst, char *src)
{
//  strcpy(dst, src);
//  util_xorstring(dst, src);

#define INTCODESIZE	(sizeof(int)*8+5)/6

  int rn;
  int *t1, t2;
  char t3[4096], t4[4096];	// This buffer is enough for an integer.
  char tz[65500];

  if( src[strlen(src)-1] == '\n')
	src[strlen(src)-1] = 0;
  util_xorstring(tz, src);

  rn=INTCODESIZE;
//  printf("INTCODESIZE=%d\n", rn);

  strncpy(t4, tz, INTCODESIZE);
  t4[INTCODESIZE] = '\0';
  util_64to256(t3, t4, DEFAULTTABLE);
  t1 = (int *) t3;
//  t2 = *t1 ^ 0x0f0f0f0f;
  t2 = *t1 ^ 0xffffffff;
#ifdef _BACK_VERSION
  util_swapint(&rn, &t2, "4312");
#else
  util_swapint(&rn, &t2, "3142");
#endif
//  printf("random number=%d\n", rn);
  util_shrstring(dst, tz + INTCODESIZE, rn);
  
}

SDL_Surface* createSDLSurface(int w, int h, SDL_Color* pixels = 0)
{
    Uint32 rmask, gmask, bmask, amask;
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0;

    int depth, pitch;
    depth = 32;
    pitch = w * 4;

    //SDL_Color* pixelData = (SDL_Color*)malloc(w * h* sizeof(SDL_Color));
    SDL_Color* pixelData = pixels;
    if (pixels == 0)
    {
        pixelData = (SDL_Color*)malloc(w*h*sizeof(SDL_Color));
        memset(pixelData, 0, w * h * sizeof(SDL_Color));
    }
    
    SDL_Surface* surface;
    surface = SDL_CreateRGBSurfaceFrom((void*)pixelData, w, h, depth, pitch,
                                             rmask, gmask, bmask, 0);
    char error[355];
    sprintf(error, "%s", SDL_GetError());
    //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0));
    return surface;
}

// -------------------------------------------------------------------
// Get a function information from MesgSlice.  A function is a complete
// and identifiable message received, beginned at DEFAULTFUNCBEGIN and
// ended at DEFAULTFUNCEND.  This routine will return the function ID
// (Action ID) and how many fields this function have.
//
// arg: func=return function ID    fieldcount=return fields of the function
// ret: 1=success  0=failed (function not complete)
int util_GetFunctionFromSlice(int *func, int *fieldcount)
{
  char t1[16384];
  int i;

//  if (strcmp(MesgSlice[0], DEFAULTFUNCBEGIN)!=0) util_DiscardMessage();
  
  strcpy(t1, MesgSlice[1]);
  // Robin adjust
  //*func=atoi(t1);
  *func=atoi(t1)-23;
  for (i=0; i<SLICE_MAX; i++)
    if (strcmp(MesgSlice[i], DEFAULTFUNCEND)==0) {
      *fieldcount=i-2;	// - "&" - "#" - "func" 3 fields
      return 1;
    }

  return 0;	// failed: message not complete
}

// -------------------------------------------------------------------
// Discard a message from MesgSlice.
//
void util_DiscardMessage(void)
{

  SliceCount=0;
/*
  int i,j;
  void *ptr;
  
  i=0;
  while ((i<SliceCount)&&(strcmp(MesgSlice[i], DEFAULTFUNCBEGIN)!=0)) i++;

  if (i>=SliceCount) {
    // discard all message
    for (j=0; j<SliceCount; j++) strcpy(MesgSlice[j],"");
  } else {
    for (j=0; j<SliceCount-i; j++) {
      ptr=MesgSlice[j];
      MesgSlice[j]=MesgSlice[j+i];
      MesgSlice[j+i]=ptr;
    }
  }
*/
}

// -------------------------------------------------------------------
// Send a message
//
// arg: fd=socket fd   func=function ID   buffer=data to send
void util_SendMesg(int fd, int func, char *buffer)
{
  char t1[16384], t2[16384];

  // Robin adjust
  //sprintf(t1, "&;%d%s;#;", func, buffer);
  sprintf(t1, "&;%d%s;#;", func+13, buffer);
#if 0
	if( lssproto_writelogfilename[0] != '\0' ){
		FILE *wfp = fopen( lssproto_writelogfilename , "a+" );
		if(wfp){
			fprintf( wfp , "�ͳ���\t%s\n", t1 );
			fclose(wfp);
		}
	}
#endif
  util_EncodeMessage(t2, t1);
  // send t2
  /**/
#ifdef __STONEAGE
//  print("Send=%s\n", t2);
  lssproto_Send(fd, t2);
#endif
}

// -------------------------------------------------------------------
// Convert 8-bit strings into 6-bit strings, buffers that store these strings
// must have enough space.
//
// arg: dst=8-bit string;  src=6-bit string;  len=src strlen;
//      table=mapping table
// ret: 0=failed  >0=bytes converted
int util_256to64(char *dst, char *src, int len, char *table)
{
  unsigned int dw,dwcounter;
  int i;

  if (!dst || !src || !table) return 0;
  dw=0;
  dwcounter=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)<<1) ) | dw;
    dst[ dwcounter++ ] = table[ dw & 0x3f ];
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ dw & 0x3f ];
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ dw ];
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Convert 6-bit strings into 8-bit strings, buffers that store these strings
// must have enough space.
//
// arg: dst=6-bit string;  src=8-bit string;  table=mapping table
// ret: 0=failed  >0=bytes converted
int util_64to256(char *dst, char *src, char *table)
{
  unsigned int dw,dwcounter;
  unsigned int i, j;
  char *ptr = NULL;

  dw=0;
  dwcounter=0;
  if (!dst || !src || !table) return 0;
  char c;
  for (i=0; i<strlen(src); i++) {
	  c = src[i];
	  for( j = 0; j < strlen(table); j++){
		  if( table[j] == c){
			  ptr = table+j;
			  break;
		  }
	  }
    if (!ptr) return 0;
    if (i%4) {
      dw = ((unsigned int)(ptr-table) & 0x3f) << ((4-(i%4))<<1) | dw;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      dw = (unsigned int)(ptr-table) & 0x3f;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shr(char *dst, char *src, int len, char *table, char *key)
{
  unsigned int dw,dwcounter,j;
  int i;

  if (!dst || !src || !table || !key) return 0;
  if (strlen(key)<1) return 0;	// key can't be empty.
  dw=0;
  dwcounter=0;
  j=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)<<1) ) | dw;
    dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];	// check!
    j++;  if (!key[j]) j=0;
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ ((dw & 0x3f) + key[j]) % 64 ];// check!
      j++;  if (!key[j]) j=0;
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ (dw + key[j]) % 64 ];	// check!
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shr.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shl_64to256(char *dst, char *src, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j,k;
  char *ptr = NULL;

  if (!key || (strlen(key)<1)) return 0;	// must have key

  dw=0;
  dwcounter=0;
  j=0;
  if (!dst || !src || !table) return 0;
  char c;
  for (i=0; i<strlen(src); i++) {
	  c = src[i];
	  for( k = 0; k < strlen(table); k++){
		  if( table[k] == c){
			  ptr = table+k;
			  break;
		  }
	  }
    if (!ptr) return 0;
    if (i%4) {
      // check!
      dw = ((((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64)
           << ((4-(i%4))<<1) | dw;
      j++;  if (!key[j]) j=0;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      // check!
      dw = (((unsigned int)(ptr-table) & 0x3f) + 64 - key[j]) % 64;
      j++;  if (!key[j]) j=0;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// This basically is a 256to64 encoder.  But it shifts the result by key.
//
// arg: dst=6-bit string;  src=8-bit string;  len=src strlen;
//      table=mapping table;  key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_256to64_shl(char *dst, char *src, int len, char *table, char *key)
{
  unsigned int dw,dwcounter;
  int i, j;

  if (!dst || !src || !table || !key) return 0;
  if (strlen(key)<1) return 0;	// key can't be empty.
  dw=0;
  dwcounter=0;
  j=0;
  for (i=0; i<len; i++) {
    dw = ( ((unsigned int)src[i] & 0xff) << ((i%3)<<1) ) | dw;
    dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
    j++;  if (!key[j]) j=0;
    dw = ( dw >> 6 );
    if (i%3==2) {
      dst[ dwcounter++ ] = table[ ((dw & 0x3f) + 64 - key[j]) % 64 ];	// check!
      j++;  if (!key[j]) j=0;
      dw = 0;
    }
  }
  if (dw) dst[ dwcounter++ ] = table[ (dw + 64 - key[j]) % 64 ];	// check!
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Decoding function of util_256to64_shl.
//
// arg: dst=8-bit string;  src=6-bit string;  table=mapping table;
//      key=rotate key;
// ret: 0=failed  >0=bytes converted
int util_shr_64to256(char *dst, char *src, char *table, char *key)
{
  unsigned int dw,dwcounter,i,j,k;
  char *ptr = NULL;

  if (!key || (strlen(key)<1)) return 0;	// must have key

  dw=0;
  dwcounter=0;
  j=0;
  if (!dst || !src || !table) return 0;
  char c;
  for (i=0; i<strlen(src); i++) {
	  c = src[i];
	  for( k = 0; k < strlen(table); k++){
		  if( table[k] == c){
			  ptr = table+k;
			  break;
		  }
	  }
    if (!ptr) return 0;
    if (i%4) {
      // check!
      dw = ((((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64)
           << ((4-(i%4))<<1) | dw;
      j++;  if (!key[j]) j=0;
      dst[ dwcounter++ ] = dw & 0xff;
      dw = dw >> 8;
    } else {
      // check!
      dw = (((unsigned int)(ptr-table) & 0x3f) + key[j]) % 64;
      j++;  if (!key[j]) j=0;
    }
  }
  if (dw) dst[ dwcounter++ ] = dw & 0xff;
  dst[ dwcounter ] = '\0';
  return dwcounter;
}

// -------------------------------------------------------------------
// Swap a integer (4 byte).
// The value "rule" indicates the swaping rule.  It's a 4 byte string
// such as "1324" or "2431".
//
void util_swapint(int *dst, int *src, char *rule)
{
  char *ptr, *qtr;
  int i;

  ptr = (char *) src;
  qtr = (char *) dst;
  for (i=0; i<4; i++) qtr[rule[i]-'1']=ptr[i];
}

// -------------------------------------------------------------------
// Xor a string.  Be careful that your string contains '0xff'.  Your
// data may lose.
//
void util_xorstring(char *dst, char *src)
{
  unsigned int i;
  
  for (i=0; i<strlen(src); i++) dst[i]=src[i]^255;
  dst[i]='\0';
}

// -------------------------------------------------------------------
// Shift the string right.
//
void util_shrstring(char *dst, char *src, int offs)
{
  char *ptr;
  int len = strlen(src);
  if (!dst || !src || (strlen(src)<1)) return;
  
  offs = strlen(src) - (offs % strlen(src));
  ptr = src+offs;
  strcpy(dst, ptr);
  strncat(dst, src, offs);
  dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Shift the string left.
//
void util_shlstring(char *dst, char *src, int offs)
{
  char *ptr;
  if (!dst || !src || (strlen(src)<1)) return;
  
  offs = offs % strlen(src);
  ptr = src+offs;
  strcpy(dst, ptr);
  strncat(dst, src, offs);
  dst[strlen(src)]='\0';
}

// -------------------------------------------------------------------
// Convert a message slice into integer.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkint
int util_deint(int sliceno, int *value)
{
  int *t1, t2;
  char t3[4096];	// This buffer is enough for an integer.

  util_shl_64to256(t3, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
  t1 = (int *) t3;
  t2 = *t1 ^ 0xffffffff;
#ifdef _BACK_VERSION
  util_swapint(value, &t2, "3421");
#else
  util_swapint(value, &t2, "2413");
#endif
  return *value;
}

// -------------------------------------------------------------------
// Pack a integer into buffer (a string).  Return a checksum.
//
// arg: buffer=output   value=data to pack
// ret: checksum, this value must match the one generated by util_deint
int util_mkint(char *buffer, int value)
{
  int t1, t2;
  char t3[4096];	// This buffer is enough for an integer.

#ifdef _BACK_VERSION
  util_swapint(&t1, &value, "4312");
#else
  util_swapint(&t1, &value, "3142");
#endif
  t2 = t1 ^ 0xffffffff;
  util_256to64_shr(t3, (char *) &t2, sizeof(int), DEFAULTTABLE, PersonalKey);
  strcat(buffer, ";");	// It's important to append a SEPARATOR between fields
  strcat(buffer, t3);

  return value;
}

// -------------------------------------------------------------------
// Convert a message slice into string.  Return a checksum.
//
// arg: sliceno=slice index in MesgSlice    value=result
// ret: checksum, this value must match the one generated by util_mkstring
int util_destring(int sliceno, char *value)
{
  util_shr_64to256(value, MesgSlice[sliceno], DEFAULTTABLE, PersonalKey);
  
  return strlen(value);
}

// -------------------------------------------------------------------
// Convert a string into buffer (a string).  Return a checksum.
//
// arg: buffer=output   value=data to pack
// ret: checksum, this value must match the one generated by util_destring
int util_mkstring(char *buffer, char *value)
{
  char t1[SLICE_SIZE];

  util_256to64_shl(t1, value, strlen(value), DEFAULTTABLE, PersonalKey);
  strcat(buffer, ";");	// It's important to append a SEPARATOR between fields
  strcat(buffer, t1);

  return strlen(value);
}

void util_Release(void)
{
  int i;
  
	if(MesgSlice){
		for(i=0;i<SLICE_MAX;i++){
			if(MesgSlice[i]){
				FREE(MesgSlice[i]);
#ifdef _DEBUG
				g_iMallocCount--;
#endif
			}
		}
		FREE(MesgSlice);
#ifdef _DEBUG
		g_iMallocCount--;
#endif
	}
}