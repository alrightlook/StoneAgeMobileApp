/************************/
/*	sprdisp.h			*/
/************************/
#ifndef _SPRDISP_H_
#define _SPRDISP_H_

#include "version.h"
#define DISP_BUFFER_SIZE 4096 	// ???????
enum{
	DISP_PRIO_BG 			= 0,	//背景
	DISP_PRIO_TILE 		= 1,	//地表// ????????䴘????????
	DISP_PRIO_CHAR 		= 10,	//人物// ???
	DISP_PRIO_PARTS 	= 10,	//建物// ???????
	DISP_PRIO_RESERVE = 20,	//预留
	DISP_PRIO_JIKI 		= 30,	/* ? 	*/
	DISP_PRIO_GRID 		= 100,	// ????????
	DISP_PRIO_BOX,				/* ???? */
	DISP_PRIO_IME1,				/* ????????????  ?? */
	DISP_PRIO_IME2,				/* ????????????  ? */
								/* ??????? */
	DISP_PRIO_MENU,				//选单/* ???? */
	DISP_PRIO_IME3,				/* ?????????????  ?? */
	DISP_PRIO_IME4,				/* ?????????????  ? */
	DISP_PRIO_BOX2,				/* ????? */
	DISP_PRIO_ITEM,				/* ???? */
								/* ???????? */
	DISP_PRIO_YES_NO_WND,		/* ?????? */
	DISP_PRIO_YES_NO_BTN,		/* ???? */
	DISP_PRIO_BOX3,				/* ????? */
	DISP_PRIO_DRAG,				/* ????? */
	DISP_PRIO_MOUSE,			/* ??????? 	*/
	DISP_PRIO_TOP = 255			/* ?? 	*/
};

typedef struct {
	int x, y;				//秀图时的座标// ?
	int bmpNo;			//图编号	// ????( ???????? )
	ACTION *pAct;		// ?????????
	bool hitFlag;		// ?????????  ?：?????：????：???
									// 十位数为 1:要显示alpha 2:饱和处理 3:石化 4:中毒	
#ifdef _READ16BITBMP
	char DrawEffect;	// 0:无特别处理 1:alpha 2:饱和处理 3:石化 4:中毒
#endif
}DISP_INFO;

typedef struct {
	short no;					//这张图在DISP_INFO的位置// ????????
	unsigned char	dispPrio; 	//显示时的优先顺序// ????
}DISP_SORT;

typedef struct{
	DISP_INFO DispInfo[ DISP_BUFFER_SIZE ];
	DISP_SORT DispSort[ DISP_BUFFER_SIZE ];
	short 		DispCnt;	//目前储存数量// ?????
}DISP_BUFFER;
extern DISP_BUFFER 	DispBuffer;

extern char *pRealBinBits;
extern int RealBinWidth, RealBinHeight;
#ifdef _READ16BITBMP
extern unsigned char *pRealBinAlpha;
#endif


extern int SurfaceBusyFlag;
void SortDispBuffer( void );
void PutBmp( void );
int StockDispBuffer( int x, int y, unsigned char prio, int bmpNo, bool hitFlag );
int StockDispBuffer2( int x, int y, unsigned char prio, int bmpNo, bool hitFlag );
void StockTaskDispBuffer( void );
void StockTaskDispBuffer2( int prio1, int prio2 );
void StockBoxDispBuffer( int x1, int y1, int x2, int y2, unsigned char dispPrio, int color, bool fill );

#endif
