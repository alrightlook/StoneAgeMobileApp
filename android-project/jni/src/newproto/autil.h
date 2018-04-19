// Arminius' protocol utilities ver 0.1
//
// Any questions and bugs, mailto: arminius@mail.hwaei.com.tw

#ifndef __UTIL_H_
#define __UTIL_H_

#define SLICE_MAX	20
#define SLICE_SIZE	65500
extern char **MesgSlice;	// store message slices
extern int SliceCount;		// count slices in MesgSlice
#include "SDL.h"
extern char PersonalKey[];

#define DEFAULTTABLE	\
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}"
#define DEFAULTFUNCBEGIN	"&"
#define DEFAULTFUNCEND		"#"

void util_Init(void);
void util_Release(void);
void util_SplitMessage(char *source, char *separator);
void util_EncodeMessage(char *dst, char *src);
void util_DecodeMessage(char *dst, char *src);
int util_GetFunctionFromSlice(int *func, int *fieldcount);
void util_DiscardMessage(void);
void util_SendMesg(int fd, int func, char *buffer);
SDL_Surface* createSDLSurface(int w, int h,  SDL_Color* pixels);

// -------------------------------------------------------------------
// Encoding function units.  Use in Encrypting functions.
int util_256to64(char *dst, char *src, int len, char *table);
int util_64to256(char *dst, char *src, char *table);
int util_256to64_shr(char *dst, char *src, int len, char *table, char *key);
int util_shl_64to256(char *dst, char *src, char *table, char *key);
int util_256to64_shl(char *dst, char *src, int len, char *table, char *key);
int util_shr_64to256(char *dst, char *src, char *table, char *key);

void util_swapint(int *dst, int *src, char *rule);
void util_xorstring(char *dst, char *src);
void util_shrstring(char *dst, char *src, int offs);
void util_shlstring(char *dst, char *src, int offs);
// -------------------------------------------------------------------
// Encrypting functions
int util_deint(int sliceno, int *value);
int util_mkint(char *buffer, int value);
int util_destring(int sliceno, char *value);
int util_mkstring(char *buffer, char *value);

#endif
