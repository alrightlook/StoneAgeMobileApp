/************************/
/*	system.h			*/
/************************/
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/**** SYSTEM INCLUDE ****/
////#include <windows.h>
//#include <windowsx.h>
#include <string.h>
#include <stdio.h>

/**** DirectDraw Include ****/
//#include <ddraw.h>

/**** USER INCLUDE ****/
#include "directDraw.h"
#include "main.h"
#include "gamemain.h"
#include "sprmgr.h"
#include "init.h"
#include "process.h"
#include "action.h"
#include "sprdisp.h"
#include "math2.h"
#include "../oft/work.h"
#include "chat.h"
#include "font.h"
#include "mouse.h"
#include "radar.h"
#include "gemini.h"
#include "pattern.h"
#include "SDL.h"


#ifdef _DEBUG
extern unsigned int iTotalUseTime;
extern unsigned int iTotalProcTime;
extern unsigned int iTotalRunCount;
extern int tf;
#endif

#define LOWORD(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define HIWORD(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#define HIBYTE(w)           ((unsigned char)((((unsigned long)(w)) >> 8) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))



#endif
