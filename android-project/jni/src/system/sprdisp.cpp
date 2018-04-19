/************************/
/*	sprdisp.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/map.h"
#include "../systeminc/battleMap.h"
#include <stdlib.h>

#define STOCK_DISP_BUFFER_NO_BMP	( 1 << 31 ) 	// ???????
#define STOCK_DISP_BUFFER_LINE		( 1 << 30 ) 	// ???
#define STOCK_DISP_BUFFER_BOX		( 1 << 29 ) 	// ????
#define STOCK_DISP_BUFFER_BOX_FILL	( 1 << 28 )		// ????�D????
#define STOCK_DISP_BUFFER_CIRCLE	( 1 << 27 ) 	// ?

extern SDL_Surface* screenSurface;
extern SDL_Surface* battleSurface;
// ����?????�B�l
DISP_BUFFER DispBuffer;

#ifndef __CARYTEST
// Realbin ��???????????????
char *pRealBinBits;
// ?? Realbin ??��??????????
int RealBinWidth, RealBinHeight;
#endif
#ifdef _READ16BITBMP
unsigned char *pRealBinAlpha;
#endif
// ????????????
int SurfaceBusyFlag = 0;

// ?????????
extern ACTION* pActMenuWnd2;
// ??????
extern unsigned int MenuToggleFlag;

// Robin 04/14 for Trade
extern ACTION* pActMenuWnd4;

// Bankman
extern ACTION* pActMenuWnd5;

extern unsigned short highColorPalette[256];
#ifdef _READ16BITBMP
extern AddressBin_s adrntruebuff[MAX_GRAPHICS_24];
extern bool g_bUseAlpha;
#endif

// ???�{???
int SortComp( DISP_SORT *pDisp1, DISP_SORT *pDisp2 );
typedef int CMPFUNC( const void *, const void * );
// ����???????????????��?��?
inline void GetBoxDispBuffer( DISP_INFO *pDispInfo, int bmpNo );
#ifdef _SURFACE_ANIM
void DrawAni(void);
int iProcessAniNum;
#endif
#ifdef _READ16BITBMP
void DrawAlpha(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,int Alpha,bool bLastOne);
extern int displayBpp;
void DrawStaturated(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne);
void DrawGray(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType);
void DrawAlphaChannel(SURFACE_INFO *surface_info,unsigned char *AlphaData,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne);
#endif

// ����??????? ///////////////////////////////////////////////////////////
void SortDispBuffer( void )
{	
	//???????
	qsort( 	DispBuffer.DispSort,	// ?�B�l?????
			DispBuffer.DispCnt,		// �{?????
			sizeof( DISP_SORT ), 	// ?�B�l????
			( CMPFUNC * )SortComp 	// �{?????????
		);				
}

#ifdef __SKYISLAND
DISP_SORT *pSortTileTail;
// ????��? /////////////////////////////////////////////////////////////////
bool PutTileBmp( void )
{
	DISP_INFO 	*pDispInfo;
	int i;
	int bmpNo;
	SDL_Rect src;
	int sx, sy;
	bool retainbackbuffer = false;
	pSortTileTail = DispBuffer.DispSort;
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif
	if( (ProcNo==PROC_GAME/* && SubProcNo==3*/) || ProcNo==PROC_BATTLE){
#ifdef _SURFACE_ANIM
		iProcessAniNum = 0;
#endif
		if( fastDrawTile && (fastDrawTileFlag==0 || amountXFastDraw || amountYFastDraw) ){
			retainbackbuffer = true;
			// ?????????????????????????????
			if( ProcNo != PROC_BATTLE){
				src.y = 0;
				src.x = 0;
				src.w = DEF_APPSIZEX;
				src.h = DEF_APPSIZEY;
				sx = 0;
				sy = 0;
				//???????????????
				if( ResoMode == 1 ){
					//src.right /= 2;
					//src.bottom /= 2;
					src.w >>= 1;
					src.h >>= 1;
				}
				if( amountXFastDraw > 0 ){
					src.w -= amountXFastDraw;
					sx += amountXFastDraw;
				}else if( amountXFastDraw < 0 ){
					src.x -= amountXFastDraw;
				}
				if( amountYFastDraw > 0 ){
					src.h -= amountYFastDraw;
					sy += amountYFastDraw;
				}else if( amountYFastDraw < 0 ){
					src.y -= amountYFastDraw;
				}
                SDL_Rect srcRect = {src.x, src.y, src.w, src.h};
                SDL_Rect destRect = {sx, sy, src.w, src.h};
                SDL_BlitSurface(battleSurface, &srcRect, screenSurface, &destRect);
				//if( ////lpDraw->lpBACKBUFFER->BltFast( sx, sy,  , &src, DDBLTFAST_WAIT ) == 0 )       //Jerry ���Ƶذ��Blit
					//SurfaceBusyFlag = true;
#ifdef _READ16BITBMP
				/*if(g_bUseAlpha){
					if(lpDraw->lpBACKBUFFERSYS->BltFast(sx,sy,lpBattleSurfaceSys,&src,DDBLTFAST_WAIT) == 0)
						SurfaceBusyFlag = true;
				}*/
#endif
			}
			// ???????????
			for( i = 0; i<DispBuffer.DispCnt ; i++, pSortTileTail++ ){
#ifdef _SURFACE_ANIM
				if(pSortTileTail->dispPrio == 0) continue;
#endif
				// ��Ϊ�������,�������Ŀǰ����ʾ˳��ȵر��Ļ�,��ʾ�ر��Ѿ���������
				if( pSortTileTail->dispPrio > DISP_PRIO_TILE){
					DispBuffer.DispCnt -= i;
#ifdef _SURFACE_ANIM
					iProcessAniNum = i;
#endif
					break;
				}
				// ����????�V????????????
				// ȡ�õ�һ��Ҫ�����ͼ�� DispInfo ����
				pDispInfo = DispBuffer.DispInfo + pSortTileTail->no;
				bmpNo = pDispInfo->bmpNo; // ???�k?
				
				// ?????
				// ???�R�e?????������?
				if( pDispInfo->pAct != NULL ){
					if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
				}
				// ?????????????????????????
				// ?????????
				if( LoadBmp( bmpNo ) == false ) continue;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo,lpSurfaceInfoSys = SpriteInfo[bmpNo].lpSurfaceInfoSys
					;lpSurfaceInfo != NULL,lpSurfaceInfoSys != NULL
					;lpSurfaceInfo = lpSurfaceInfo->pNext,lpSurfaceInfoSys = lpSurfaceInfoSys->pNext){
						if(ResoMode == 1){
							if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
								(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
								lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0) SurfaceBusyFlag = true;
						}
						else{
							if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
								pDispInfo->y + lpSurfaceInfo->offsetY,
								lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0) SurfaceBusyFlag = true;
						}
						lpSurfaceInfo->date = SurfaceDate;
					}
				}
				else
#endif
				{
					for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext ){
						if(ResoMode == 1){
							if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
							(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface ) == 0) SurfaceBusyFlag = true;
						}
						else{
							if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
							pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface ) == 0) SurfaceBusyFlag = true;
						}
						lpSurfaceInfo->date = SurfaceDate;
					}
				}
			}
		}
	}
	if(retainbackbuffer){
        SDL_BlitSurface(screenSurface, 0, battleSurface, 0);
		//if( lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT ) == 0 )	SurfaceBusyFlag = true;
#ifdef _READ16BITBMP
		//if(g_bUseAlpha) if(lpBattleSurfaceSys->BltFast(0,0,lpDraw->lpBACKBUFFERSYS,NULL,DDBLTFAST_WAIT) == 0)	SurfaceBusyFlag = true;
#endif
	}
	if( ProcNo != PROC_BATTLE){
#ifdef _LOST_FOREST_FOG		
		if(nowFloor!=7450 && nowFloor!=7451
		){
#endif
			extern unsigned long sky_island_no;
			if( sky_island_no){
				extern void SkyIslandDraw();
		 		SkyIslandDraw();
				return false;
			}
#ifdef _LOST_FOREST_FOG
		}
#endif
	}
	return retainbackbuffer;
}
#endif

// ????��? /////////////////////////////////////////////////////////////////
void PutBmp( void )
{
#ifdef __SKYISLAND
	// ????????��?����?��?
	switch( BackBufferDrawType ){
		case DRAW_BACK_NORMAL:	// ???????????????
			ClearBackSurface();	
			/*void SkyIslandProc();
			SkyIslandProc();*/
			if( !PutTileBmp() ){
                SDL_BlitSurface(battleSurface, 0, screenSurface, 0);
				/*if( fastDrawTile && ////lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == 0 ){
					SurfaceBusyFlag = true;
					return;
				}*/
			}
#ifdef _SURFACE_ANIM
			//DrawAni();
#endif
			break;
		case DRAW_BACK_BATTLE:	// ??????��?
			amountXFastDraw = amountYFastDraw = 0;
			PutTileBmp();
			DrawBattleMap();
			break;
		default:
			if( !PutTileBmp() ){
				/*if( fastDrawTile && ////lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == 0 ){
					SurfaceBusyFlag = true;
					return;
				}*/
			}
			break;
	}
	DISP_SORT 	*pDispSort = pSortTileTail;
#else
	DISP_SORT 	*pDispSort = DispBuffer.DispSort;
#endif
	DISP_INFO 	*pDispInfo;
	int i;
	int bmpNo;
	unsigned char putTextFlag = 0; // ?????��???????
	unsigned char drawMapEffectFlag = 0;
	unsigned char drawFastTileFlag = 0;
	unsigned char drawFastTileFlag2 = 0;
#ifndef __CARYTEST
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif
#else
	DDSURFACEDESC ddsd;	// ??????�B�l
	LPWORD surface;
	int pitch;
	bool locked = false;
	int x, y;
	short width, height;
	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
	ddsd.dwSize = sizeof( DDSURFACEDESC );
	if( lpDraw->lpBACKBUFFER->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) == DD_OK ){
		surface = (LPWORD)ddsd.lpSurface;
		pitch = ddsd.lPitch;
		locked = true;
	}
#endif
	// ???????????
	for( i = 0; i < DispBuffer.DispCnt ; i++, pDispSort++ )
    {
		// ����????�V????????????
		pDispInfo = DispBuffer.DispInfo + pDispSort->no;
		bmpNo = pDispInfo->bmpNo; // ???�k?
		
		// ?????����( BACK )
		if( putTextFlag == 0 ){
			// ?????�q????
			if( pDispSort->dispPrio >= DISP_PRIO_MENU ){
				// ?????????????????
				PutText( FONT_PRIO_BACK );
#ifdef _CHANNEL_MODIFY
				PutText(FONT_PRIO_CHATBUFFER);
#endif
				// ?????
				putTextFlag = 1;	
			}
		}
		// ?????����( FRONT )
		if( putTextFlag == 1 ){
			// ????????????�q????
			if( pDispSort->dispPrio >= DISP_PRIO_YES_NO_WND ){
				// ?????????????????
				PutText( FONT_PRIO_FRONT );	
				// ?????
				putTextFlag = 2;	
			}
		}
#ifdef _TRADETALKWND				// Syu ADD ���������Ի����
		//����һ���µ�������ʾ˳��
		if( putTextFlag == 2 ){
			if( pDispSort->dispPrio >= DISP_PRIO_BOX3 ){
				// ?????????????????
				PutText( FONT_PRIO_AFRONT );	
				// ?????
				putTextFlag = 2;	
			}
		}
#endif
		// ??????��????
		if( pDispSort->dispPrio > DISP_PRIO_MENU ){
			// ???????????
			if( MenuToggleFlag & JOY_CTRL_M && pActMenuWnd2 != NULL ){
				// ?????��????????
				if( pActMenuWnd2->hp > 0 ){
					// ??????��?
#ifdef _2005_ValentineDay
					// ������ҿ���С��ͼ
					if (nowFloor != 17006)
#endif
					drawAutoMap( pActMenuWnd2->x, pActMenuWnd2->y );
				}
			}
		}
		// ?????????					
		if( pDispSort->dispPrio >= DISP_PRIO_RESERVE ){
#ifdef __SKYISLAND
			extern void SkyIslandDraw2();
			SkyIslandDraw2();
#endif
			drawMapEffect();
			drawMapEffectFlag = 1;
		}
#ifndef __SKYISLAND
		// ?????�L��?
		if( fastDrawTile ){
			RECT src;
			int sx, sy;
			if( pDispSort->dispPrio >= DISP_PRIO_TILE && drawFastTileFlag2 == 0 ){
				if( ProcNo == PROC_GAME && SubProcNo == 3 && fastDrawTileFlag ){
					// ?????????????????????????????
					src.top = 0;
					src.left = 0;
					src.right = DEF_APPSIZEX;
					src.bottom = DEF_APPSIZEY;
					sx = 0;
					sy = 0;
					//???????????????
					if( ResoMode == 1 ){
						src.right >>= 1;
						src.bottom >>= 1;
					}
					if( amountXFastDraw > 0 ){
						src.right -= amountXFastDraw;
						sx += amountXFastDraw;
					}
					else if( amountXFastDraw < 0 ) src.left -= amountXFastDraw;

					if( amountYFastDraw > 0 ){
						src.bottom -= amountYFastDraw;
						sy += amountYFastDraw;
					}
					else if( amountYFastDraw < 0 ) src.top -= amountYFastDraw;

					if( ////lpDraw->lpBACKBUFFER->BltFast( sx, sy, lpBattleSurface, &src, DDBLTFAST_WAIT ) == 0 )
						SurfaceBusyFlag = true;
					drawFastTileFlag2 = 1;
				}
			}
			if( pDispSort->dispPrio > DISP_PRIO_TILE && drawFastTileFlag == 0 ){
				if( ProcNo == PROC_GAME && SubProcNo == 3 && fastDrawTileFlag ){
					// ????????????????????????????
					if( lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT ) == 0 )
						SurfaceBusyFlag = true;
					drawFastTileFlag = 1;
				}
			}
		}
#endif
		// ???????
		if( bmpNo & STOCK_DISP_BUFFER_NO_BMP ){
			// ?????????????
			if( bmpNo & ( STOCK_DISP_BUFFER_BOX | STOCK_DISP_BUFFER_BOX_FILL | STOCK_DISP_BUFFER_LINE ) ){
				// ����???????????????��?��?
				GetBoxDispBuffer( pDispInfo, bmpNo );
			}
			continue;
		}
		
		// ?????
		// ???�R�e?????������?
		if( pDispInfo->pAct != NULL ){
			if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
		}
		// ?????????????????????????
		// ?????????
		if( LoadBmp( bmpNo ) == false ) continue;
		// ???????????????

#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo,lpSurfaceInfoSys = SpriteInfo[bmpNo].lpSurfaceInfoSys
			;lpSurfaceInfo != NULL,lpSurfaceInfoSys != NULL
			;lpSurfaceInfo = lpSurfaceInfo->pNext,lpSurfaceInfoSys = lpSurfaceInfoSys->pNext
			){
				if(ResoMode == 1){
					if( DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX, 
						(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY, 
						lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0 ) SurfaceBusyFlag = true;
				}
				else{
					if(displayBpp == 16){
						int ret;
						
						if(lpSurfaceInfo->useAlpha == true)
							DrawAlphaChannel(lpSurfaceInfoSys,lpSurfaceInfo->lpAlphaData,
							pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
							SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false));
						else if(pDispInfo->DrawEffect == 1)
							DrawAlpha(lpSurfaceInfoSys->lpSurface,
							pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
							SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,16,(lpSurfaceInfo->pNext == NULL ? true:false));
						else if(pDispInfo->DrawEffect == 2)
							DrawStaturated(lpSurfaceInfoSys->lpSurface,
							pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
							SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false));
						else if(pDispInfo->DrawEffect >= 3)
							DrawGray(lpSurfaceInfoSys->lpSurface,
							pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->offsetX,lpSurfaceInfo->offsetY,
							SpriteInfo[bmpNo].width,SpriteInfo[bmpNo].height,(lpSurfaceInfo->pNext == NULL ? true:false),pDispInfo->DrawEffect);
						else if((ret = DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX, 
							pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface)) == 0 ) SurfaceBusyFlag = true;
					}
					else{
						if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,pDispInfo->y + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0 ) SurfaceBusyFlag = true;
					}
				}
				lpSurfaceInfo->date = SurfaceDate;
			}
		}
		else
#endif
		{
			for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext){
				if(ResoMode == 1){
					if( DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX, 
						(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY, 
						lpSurfaceInfo->lpSurface ) == 0 ) SurfaceBusyFlag = true;
				}
				else{
					if( DrawSurfaceFast( 	pDispInfo->x + lpSurfaceInfo->offsetX, 
									pDispInfo->y + lpSurfaceInfo->offsetY, 
									lpSurfaceInfo->lpSurface ) == 0 ) SurfaceBusyFlag = true;
				}
				lpSurfaceInfo->date = SurfaceDate;
			}
		}
	}
#ifdef _LOST_FOREST_FOG
	if(nowFloor==7450||nowFloor==7451
		){
		extern unsigned long sky_island_no;
		if( sky_island_no){
			extern void SkyIslandDraw();
	 		SkyIslandDraw();
		}
	}
#endif	
#ifdef __SKYISLAND		
	extern void SkyIslandDraw2();
	SkyIslandDraw2();	
#endif
	// ????����??�\��
	// ?????����( BACK )
	if( putTextFlag == 0 ){
		// ?????????????????
		PutText( FONT_PRIO_BACK );	
		// ?????
#ifdef _CHANNEL_MODIFY
		PutText(FONT_PRIO_CHATBUFFER);
#endif
		putTextFlag = 1;	
	}
	// ?????����( FRONT )
	if( putTextFlag == 1 ){
		// ?????????????????
		PutText( FONT_PRIO_FRONT );	
	}
	// ?????????
	if( drawMapEffectFlag == 0 ) drawMapEffect();
#ifdef _READ16BITBMP
	// memset(&DispBuffer,0,sizeof(DispBuffer));
#endif
}

//**************************************************************************/
// 	??��	??����????????????
// 	??��	unsigned char dispPrio������????????
//		  	int x, int y������?��
//			int bmpNo��???�k?
//			int chr_no��??????�k?
//			int pat_no��????�k?
//**************************************************************************/
// ��������Ҫ���ŵ�Image
int StockDispBuffer( int x, int y, unsigned char dispPrio, int bmpNo, bool hitFlag )
{	
	short dx,dy;
	int BmpNo;
	// ??��V??��????????��???
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	// ??????????
	if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) return -2;
	// ??????????????
	if( -1 <= bmpNo && bmpNo <= CG_INVISIBLE ) return -2;
	// �G�Ф���???
	if( bmpNo > CG_INVISIBLE ){
		realGetNo( bmpNo , (U4 *)&BmpNo );
		realGetPos( BmpNo  , &dx, &dy);
	}else{// ????����????��????
		dx = 0;
		dy = 0;
		BmpNo = bmpNo;
	}
	// ����?�I?????�B�l
	pDispSort->dispPrio = dispPrio;
	pDispSort->no = DispBuffer.DispCnt;
	pDispInfo->hitFlag = hitFlag;
#ifdef _READ16BITBMP
	if(hitFlag >= 10 && hitFlag < 20){
		pDispInfo->DrawEffect = 1;
		pDispInfo->hitFlag = hitFlag - 10;
	}
	else if(hitFlag >= 20 && hitFlag < 30){
		pDispInfo->DrawEffect = 2;
		pDispInfo->hitFlag = hitFlag - 20;
	}
	else if(hitFlag >= 30 && hitFlag < 40){
		pDispInfo->DrawEffect = 3;
		pDispInfo->hitFlag = hitFlag - 30;
	}
	else if(hitFlag >= 40 && hitFlag < 50){
		pDispInfo->DrawEffect = 4;
		pDispInfo->hitFlag = hitFlag - 40;
	}
	else pDispInfo->DrawEffect = 0;
#endif
	// �����ѩ�?�B�l? ??????��? ?
	pDispInfo->x = x + dx;
	pDispInfo->y = y + dy;
	pDispInfo->bmpNo = BmpNo;
	
	pDispInfo->pAct = NULL;
	// ����????????
	return DispBuffer.DispCnt++;
}

//**************************************************************************/
// 	??��	??����????????????
// 	??��	unsigned char dispPrio������????????
//		  	int x, int y������?��
//			int bmpNo��???�k?(realGetNo()????)
//			int chr_no��??????�k?
//			int pat_no��????�k?
//**************************************************************************/
int StockDispBuffer2( int x, int y, unsigned char dispPrio, int bmpNo, bool hitFlag )
{	
	short dx,dy;
	int BmpNo;
	// ??��V??��????????��???
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	// ??????????
	if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) return -2;
	// ??????????????
	if( -1 <= bmpNo && bmpNo <= CG_INVISIBLE ) return -2;
	// �G�Ф���???
	if( bmpNo > CG_INVISIBLE ){
		BmpNo = bmpNo;
		realGetPos( BmpNo  , &dx, &dy);
	}else{// ????����????��????
		dx = 0;
		dy = 0;
		BmpNo = bmpNo;
	}
	// ����?�I?????�B�l
	pDispSort->dispPrio = dispPrio;
	pDispSort->no = DispBuffer.DispCnt;
	// �����ѩ�?�B�l? ??????��? ?
	pDispInfo->x = x + dx;
	pDispInfo->y = y + dy;
	pDispInfo->bmpNo = BmpNo;
	pDispInfo->hitFlag = hitFlag;
	pDispInfo->pAct = NULL;
#ifdef _READ16BITBMP
	if(pDispInfo->bmpNo >= OLD_GRAPHICS_START && pDispInfo->bmpNo < MAX_GRAPHICS){
		if(adrntruebuff[pDispInfo->bmpNo - OLD_GRAPHICS_START].staturated == 1) pDispInfo->DrawEffect = 2;
	}
#endif
	// ����????????
	return DispBuffer.DispCnt++;
}

// ???����???????????? ///////////////////////////////////////////////////
// ��������Ҫ���ŵ�Image����Act��״�����趨
void StockTaskDispBuffer( void )
{
	ACTION *pActLoop; 	/* �I�d????????��x */
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	
	/* ??��???????? */	
	//while( pActLoop != pActBtm ){
	for(pActLoop = pActTop->pNext ; 
		pActLoop != pActBtm ;
		/* ?????????? */
		pActLoop = pActLoop->pNext ){
		
		// ??????????
		if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) break;
		
		// ??????????????
		if( -1 <= pActLoop ->bmpNo && pActLoop ->bmpNo <= CG_INVISIBLE ) continue;
		
		// ?��???????????
		if( pActLoop ->deathFlag == true ) continue;
		
		/* ������??�I?? */
		if( pActLoop->atr & ACT_ATR_HIDE ) continue;

		// ����?�I?????�B�l
		pDispSort->dispPrio = pActLoop->dispPrio;
		pDispSort->no = DispBuffer.DispCnt;
		// �����ѩ�?�B�l? ??????��? ?
		pDispInfo->x = pActLoop->x + pActLoop->anim_x;
		pDispInfo->y = pActLoop->y + pActLoop->anim_y;
		pDispInfo->bmpNo = pActLoop->bmpNo;
		pDispInfo->pAct = pActLoop;
		
		// ???�R�e????
		// ��?�k��??
#ifdef __ATTACK_MAGIC
#ifdef _SKILL_ADDBARRIER
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL5 ) pDispInfo->hitFlag = 10;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL6 ) pDispInfo->hitFlag = 11;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL7 ) pDispInfo->hitFlag = 12;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL8 ) pDispInfo->hitFlag = 13;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL1 ) pDispInfo->hitFlag = 6;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL2 ) pDispInfo->hitFlag = 7;
		else
	    if( pActLoop->atr & ACT_ATR_HIT_BOX_COL3 ) pDispInfo->hitFlag = 8;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL4 ) pDispInfo->hitFlag = 9;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL3 ) pDispInfo->hitFlag = 5;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL2 ) pDispInfo->hitFlag = 4;
		else 
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL1 ) pDispInfo->hitFlag = 3;
		else // ????����
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else // �R�e??
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = false;
#else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL1 ) pDispInfo->hitFlag = 6;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL2 ) pDispInfo->hitFlag = 7;
		else
	    if( pActLoop->atr & ACT_ATR_HIT_BOX_COL3 ) pDispInfo->hitFlag = 8;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_COL4 ) pDispInfo->hitFlag = 9;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL3 ) pDispInfo->hitFlag = 5;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL2 ) pDispInfo->hitFlag = 4;
		else 
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL1 ) pDispInfo->hitFlag = 3;
		else // ????����
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else // �R�e??
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = false;
#endif
#else

		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL3 ) pDispInfo->hitFlag = 5;
		else
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL2 ) pDispInfo->hitFlag = 4;
		else 
		if( pActLoop->atr & ACT_ATR_HIT_BOX_ALL1 ) pDispInfo->hitFlag = 3;
		else // ????����
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else // �R�e??
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = false;
#endif

#ifdef _READ16BITBMP
		// 4 ʯ��
		if(pActLoop->status == 4)	pDispInfo->DrawEffect = 3;		
		// 1 ��
		else if(pActLoop->status == 1) pDispInfo->DrawEffect = 4;
		else pDispInfo->DrawEffect = 0;
		if(pDispInfo->bmpNo >= OLD_GRAPHICS_START && pDispInfo->bmpNo < MAX_GRAPHICS){
			if(adrntruebuff[pDispInfo->bmpNo - OLD_GRAPHICS_START].staturated == 1) pDispInfo->DrawEffect = 2;
		}
#endif
		// ??????????�R�e�k???
		pActLoop->hitDispNo = DispBuffer.DispCnt;
		
		// ����????????
		DispBuffer.DispCnt++;
		// ???????
		pDispSort++;
		pDispInfo++;
	}
	
}

// ???����???????????? ///////////////////////////////////////////////////
// ???prio1??prio2???????????
void StockTaskDispBuffer2( int prio1, int prio2 )
{
	ACTION *pActLoop; 	/* �I�d????????��x */
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt;
	DISP_INFO 	*pDispInfo = DispBuffer.DispInfo + DispBuffer.DispCnt;
	int tmp;
	
	if( prio1 > prio2 )
	{
		tmp = prio1;
		prio1 = prio2;
		prio2 = tmp;
	}
	
	/* ??��???????? */	
	//while( pActLoop != pActBtm ){
	for(pActLoop = pActTop->pNext ; 
		pActLoop != pActBtm ;
		/* ?????????? */
		pActLoop = pActLoop->pNext ){
		
		// ??????????
		if( DispBuffer.DispCnt >= DISP_BUFFER_SIZE ) break;
		
		// prio1??prio2?���e�d???????
		if( prio1 <= pActLoop->dispPrio && pActLoop->dispPrio <= prio2 )
			continue;
		
		// ??????????????
		if( -1 <= pActLoop ->bmpNo && pActLoop ->bmpNo <= CG_INVISIBLE ) continue;
		
		// ?��???????????
		if( pActLoop ->deathFlag == true ) continue;
		
		/* ������??�I?? */
		if( pActLoop->atr & ACT_ATR_HIDE ) continue;

		// ����?�I?????�B�l
		pDispSort->dispPrio = pActLoop->dispPrio;
		pDispSort->no = DispBuffer.DispCnt;
		// �����ѩ�?�B�l? ??????��? ?
		pDispInfo->x = pActLoop->x + pActLoop->anim_x;
		pDispInfo->y = pActLoop->y + pActLoop->anim_y;
		pDispInfo->bmpNo = pActLoop->bmpNo;
		pDispInfo->pAct = pActLoop;
		
		// ???�R�e????
		if( pActLoop->atr & ACT_ATR_HIT_BOX ) pDispInfo->hitFlag = 2;
		else 
		if( pActLoop->atr & ACT_ATR_HIT ) pDispInfo->hitFlag = 1;
		else pDispInfo->hitFlag = false;
		// ??????????�R�e�k???
		pActLoop->hitDispNo = DispBuffer.DispCnt;
		
		// ����????????
		DispBuffer.DispCnt++;
		// ???????
		pDispSort++;
		pDispInfo++;
	}
	
}

// ????����???????????? ***************************************/
void StockBoxDispBuffer( int x1, int y1, int x2, int y2, unsigned char dispPrio, int color, bool fill )
{
	int col;	// ��?���e
	// int ? short ??????
	int x = ( x1 << 16 ) | x2; 	// ��????????????????
	int y = ( y1 << 16 ) | y2;	// ��????????????????
	
	// ???????
	if( fill == 0 ) col = color | STOCK_DISP_BUFFER_BOX | STOCK_DISP_BUFFER_NO_BMP ;
	// �D???????
	else if( fill == 1 ) col = color | STOCK_DISP_BUFFER_BOX_FILL | STOCK_DISP_BUFFER_NO_BMP ;
	// ?????
	else if( fill == 2 ) col = color | STOCK_DISP_BUFFER_LINE | STOCK_DISP_BUFFER_NO_BMP ;
	
	// ?????????????
	StockDispBuffer( x, y, dispPrio, col, 0 );
}

// ����???????????????��?��? *********************************/
inline void GetBoxDispBuffer( DISP_INFO *pDispInfo, int bmpNo )
{
	// ??????
	SDL_Rect rect;
	int color; // ��
	bool fill = false; // �D???????
	int DispInfoX = pDispInfo->x;
	int DispInfoY = pDispInfo->y;
	
	// ?��?��?��?

    rect.x = (unsigned int) (DispInfoX >> 16);
    rect.y = (unsigned int) (DispInfoY >> 16);
	// ??????????
	//_asm{
	//	// rect.left ?��?��?
	//	mov		cl,16				// ??????????
	//	mov		eax, [ DispInfoX ]
	//	sar		eax, cl				// ??��????
	//	mov		[ rect.x ], eax
	//	// rect.top ?��?��?
	//	mov		eax, [ DispInfoY ]
	//	sar		eax, cl
	//	mov		[ rect.y ], eax
	//}

    rect.w  =	(unsigned long )( pDispInfo->x & 0xffff ) - rect.x;
	rect.h =	(unsigned long )( pDispInfo->y & 0xffff ) - rect.y;

	
	// ???????
	if( bmpNo & STOCK_DISP_BUFFER_BOX ){ 
		color = bmpNo & 0xff; 			// ��????��?��?
		DrawBox( &rect, color, 0 );	// ????��?
		
	// �D???????
	}else if( bmpNo & STOCK_DISP_BUFFER_BOX_FILL ){ 
		color = color = bmpNo & 0xff;	// �����e
		DrawBox( &rect, color, 1 );	// ????��?
		
	}else if( bmpNo & STOCK_DISP_BUFFER_LINE ){ 
		color = color = bmpNo & 0xff;	// �����e
		DrawBox( &rect, color, 2 );	// ???��?
	}
}			

// ???�{??? //////////////////////////////////////////////////////////////
int SortComp( DISP_SORT *pDisp1, DISP_SORT *pDisp2 )
{
	// pDisp1 ?����?�I�T?��?��????�V?�v?
	if( pDisp1->dispPrio > pDisp2->dispPrio ){
		return 1;
	}
	
	// pDisp2 ?����?�I�T?��?��????????
	if( pDisp1->dispPrio < pDisp2->dispPrio ){
		return -1;
	}
	// ?????�K????
	// pDisp1 ?��?????????????????
	if( pDisp1->no > pDisp2->no ){
		return -1;
	}
	// ????????�V?�v???????
	return 1;
}

#ifdef _SURFACE_ANIM
// ����̬�ر�(����ȨΪ0�Ķ�̬�ر�)
void DrawAni()
{
	DISP_INFO 	*pDispInfo;
	int bmpNo;
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif

	if(ProcNo != PROC_BATTLE){
		pSortTileTail = DispBuffer.DispSort;
		DispBuffer.DispCnt += iProcessAniNum;
		for(int i = 0; i<DispBuffer.DispCnt; i++, pSortTileTail++ ){
			if( pSortTileTail->dispPrio == DISP_PRIO_TILE) continue;
			// ��Ϊ�������,�������Ŀǰ����ʾ˳��ȵر��Ļ�,��ʾ�ر��Ѿ���������
			if( pSortTileTail->dispPrio > DISP_PRIO_TILE){
				DispBuffer.DispCnt -= i;
				break;
			}
			// ȡ�õ�һ��Ҫ�����ͼ�� DispInfo ����
			pDispInfo = DispBuffer.DispInfo + pSortTileTail->no;
			bmpNo = pDispInfo->bmpNo; // ???�k?
			
			if( pDispInfo->pAct != NULL ){
				if( pDispInfo->pAct->atr & ACT_ATR_HIDE2 ) continue;
			}
			if( LoadBmp( bmpNo ) == false ) continue;
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo,lpSurfaceInfoSys = SpriteInfo[ bmpNo ].lpSurfaceInfoSys
				;lpSurfaceInfo != NULL,lpSurfaceInfoSys != NULL
				;lpSurfaceInfo = lpSurfaceInfo->pNext,lpSurfaceInfoSys = lpSurfaceInfoSys->pNext){
					if(ResoMode == 1){
						if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
							lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0) SurfaceBusyFlag = true;
					}
					else{
						if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
						pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface,lpSurfaceInfoSys->lpSurface) == 0) SurfaceBusyFlag = true;
					}
					lpSurfaceInfo->date = SurfaceDate;
				}
			}
			else
#endif
			{
				for(lpSurfaceInfo = SpriteInfo[ bmpNo ].lpSurfaceInfo;lpSurfaceInfo != NULL;lpSurfaceInfo = lpSurfaceInfo->pNext ){
					if(ResoMode == 1){
						if(DrawSurfaceFast((pDispInfo->x >> 1) + lpSurfaceInfo->offsetX,
						(pDispInfo->y >> 1) + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface ) == 0) SurfaceBusyFlag = true;
					}
					else{
						if(DrawSurfaceFast(pDispInfo->x + lpSurfaceInfo->offsetX,
						pDispInfo->y + lpSurfaceInfo->offsetY,
						lpSurfaceInfo->lpSurface ) == 0) SurfaceBusyFlag = true;
					}
					lpSurfaceInfo->date = SurfaceDate;
				}
			}
		}
		//if(fastDrawTile && ////lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpBattleSurface, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == 0){
		//	SurfaceBusyFlag = true;
		//	return;
		//}

#ifdef _READ16BITBMP
		/*if(g_bUseAlpha){
			if(fastDrawTile && lpDraw->lpBACKBUFFERSYS->BltFast( 0, 0, lpBattleSurfaceSys, NULL, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY ) == 0){
				SurfaceBusyFlag = true;
				return;
			}
		}*/
#endif
	}
}
#endif

#ifdef _READ16BITBMP
extern int gBitRShift;
extern bool g_bMMXMode;

static long MaskColorKey = 0x0000000000000000;
static long RMask = 0xf800f800f800f800;
static long GMask;
static long BMask = 0x001f001f001f001f;
static long AlphaMask = 0x00ff00ff00ff00ff;
static long GShift;

void DrawAlpha(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,int Alpha,bool bLastOne){
	/*static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	unsigned long Is4Multiple;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		if((w % 2) == 1) odd = true;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
		if((w % 2) == 1) odd = true;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		if((w % 2) == 1) odd = true;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;
	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}
	
	void *ptSourceDest,*ptOverLayerDest;
	surfacePitch1 = ddsdSource.lPitch >> 1;
	surfacePitch2 = ddsdOverlayer.lPitch >> 1;
	ptSourceDest = (unsigned short*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
	ptOverLayerDest = (unsigned short*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;

	if(g_bMMXMode){
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			GMask = 0x07e007e007e007e0;
			GShift = 5;
		}
		else{
			GMask = 0x03e003e003e003e0;
			GShift = 6;
		}
		surfacePitch1 = ddsdSource.lPitch;
		surfacePitch2 = ddsdOverlayer.lPitch;
		Alpha |= Alpha << 8;
		Alpha |= Alpha << 16;
		__asm{
			push eax
			push ebx
			push ecx
			push edx
			push edi
			push esi

			mov	edi,dword ptr ptSourceDest			// Ŀ��ָ��
			mov esi,dword ptr ptOverLayerDest		// ��Դָ��
			movd mm6,Alpha											// �趨alpha ֵ
			punpckldq mm6,mm6
			pand mm6,AlphaMask
			push h															// ��¼ͼ��
Start:
			pop ecx															// ȡ��ͼ��
			cmp ecx,0
			je	End
			dec ecx
			push ecx
			mov	ecx,dword ptr Is4Multiple				// ��¼ͼ���Ƿ�Ϊ4�ı���
Loop1_0:
			cmp	ecx,0
			// ����4�ı���,����alpha�������
			je	Start_4_Multiple
			// ������4�����ĵ�
			dec	ecx
			// һ�δ���һ����
			xor ebx,ebx
			mov bx,word ptr [esi]								// ������Դ��
			cmp ebx,0x0000											// �ǲ��� colorkey
			// ����colorkey,������һ��
			je	Loop1_1
//-------------------------------------------------------
			movd mm0,ebx
			// ȡ�� r
			pand mm0,RMask
			psrlw	mm0,11
			// ȡ�� g
			movd mm1,ebx
			pand mm1,GMask
			psrlw mm1,GShift
			// ȡ�� b
			movd mm2,ebx
			pand mm2,BMask
//-------------------------------------------------------
			mov	bx,word ptr [edi]								// ����Ŀ���
			movd mm3,ebx
			// ȡ�� r
			pand mm3,RMask
			psrlw	mm3,11
			// ȡ�� g
			movd mm4,ebx
			pand mm4,GMask
			psrlw mm4,GShift
			// ȡ�� b
			movd mm5,ebx
			pand mm5,BMask
//-------------------------------------------------------
			// ����alpha
			psubusw	mm3,mm0
			pmullw	mm3,mm6
			psrlw	mm3,5
			paddsw	mm3,mm0
			movd	ebx,mm3
			shl	ebx,11
			psubusw	mm4,mm1
			pmullw	mm4,mm6
			psrlw	mm4,5
			paddsw	mm4,mm1
			movd	edx,mm4
			shl	edx,5
			or	ebx,edx
			psubusw	mm5,mm2
			pmullw	mm5,mm6
			psrlw	mm5,5
			paddsw	mm5,mm2
			movd	edx,mm5
			or	ebx,edx
//-------------------------------------------------------
			//���Ŀ���
			mov	[edi],bx
Loop1_1:
			add edi,2
			add esi,2
			jmp Loop1_0

Start_4_Multiple:
			mov	ecx,dword ptr w									// ��¼ͼ��
			shr ecx,2
Loop2:
			cmp ecx,0
			je	ChangeLine
			dec	ecx
//-------------------------------------------------------
			movq mm0,[edi]											// ����Ŀ���
			movq mm1,[esi]											// ������Դ��
//-------------------------------------------------------
			movq mm7,mm1
			pcmpeqw	mm7,MaskColorKey						// ����color key,���colorkey��ͬ���word�ᱻ�趨Ϊ1,����Ϊ0
			psubusw mm1,mm7
			pand mm7,mm0
			por	mm1,mm7
			
			// ȡ�� g
			movq mm4,mm0
			pand mm4,GMask
			psrlw mm4,GShift
			movq mm5,mm1
			pand mm5,GMask
			psrlw mm5,GShift
			// ȡ�� r
			movq mm2,mm0
			pand mm2,RMask
			psrlw mm2,11
			movq mm3,mm1
			pand mm3,RMask
			psrlw mm3,11
			// ȡ�� b
			pand mm0,BMask
			pand mm1,BMask
//-------------------------------------------------------
			// ����alpha
			// b
			psubusw mm0,mm1
			pmullw mm0,mm6
			psrlw mm0,5
			paddsw mm0,mm1
			// g
			psubusw mm4,mm5
			pmullw mm4,mm6
			psrlw mm4,5
			paddsw mm4,mm5
			psllw mm4,5
			// r
			psubusw mm2,mm3
			pmullw mm2,mm6
			psrlw mm2,5
			paddsw mm2,mm3
			psllw mm2,11

			por	mm0,mm4
			por	mm0,mm2
//-------------------------------------------------------
			//���Ŀ���
			movq [edi],mm0

			add edi,8
			add esi,8

			jmp Loop2
//-------------------------------------------------------
			// ����
ChangeLine:
			mov ebx,w
			shl ebx,1												// һ�������byte,���Կ۵�wʱҪ��2
			add edi,surfacePitch1
			sub edi,ebx
			add esi,surfacePitch2
			sub esi,ebx
			jmp Start
End:
			pop esi
			pop edi
			pop edx
			pop ecx
			pop ebx
			pop eax
			emms
		}
	}
	else{
		int i,j,nColorSource,nColorOverlayer;
		unsigned long SR,SG,SB,OR,OG,OB;

		w >>= 1;
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(odd){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xf800) >> 8; // ԭ��Ҫ����11����3,�򻯳�����8
						SG = (nColorSource & 0x07e0) >> 3; // ԭ��Ҫ����5����2,�򻯳�����3
						SB = (nColorSource & 0x001f) << 3;
						OR = (nColorOverlayer & 0xf800) >> 8;
						OG = (nColorOverlayer & 0x07e0) >> 3;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
				}
				for(i=0;i<w;i++){
					nColorSource = *(unsigned long*)ptSourceDest;
					nColorOverlayer = *(unsigned long*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xf800f800) >> 8;
						SG = (nColorSource & 0x07e007e0) >> 3;
						SB = (nColorSource & 0x001f001f) << 3;
						OR = (nColorOverlayer & 0xf800f800) >> 8;
						OG = (nColorOverlayer & 0x07e007e0) >> 3;
						OB = (nColorOverlayer & 0x001f001f) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800f800;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e007e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f001f;
						*(unsigned long*)ptSourceDest = SB | SG | SR;
					}
					ptSourceDest = (unsigned long*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned long*)ptOverLayerDest + 1;
				}
				ptSourceDest = (unsigned long*)ptSourceDest + (surfacePitch1 >> 1) - w;
				ptOverLayerDest = (unsigned long*)ptOverLayerDest + (surfacePitch2 >> 1) - w;
				if(odd){
					ptSourceDest = (unsigned short*)ptSourceDest - 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest - 1;
				}
			}
		}
		// 555 ��ʾģʽ
		else{
			for(j=0;j<h;j++){
				if(odd){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = nColorSource >> 7;							// ԭ��Ҫ����10����3,�򻯳�����7
						SG = (nColorSource & 0x03e0) >> 2;	// ԭ��Ҫ����5����3,�򻯳�����2
						SB = (nColorSource & 0x001f) << 3;
						OR = nColorOverlayer >> 7;
						OG = (nColorOverlayer & 0x03e0) >> 2;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = ((((OR - SR) * Alpha) >> 5) + SR) << 7;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
				}
				for(i=0;i<w;i++){
					nColorSource = *(unsigned long*)ptSourceDest;
					nColorOverlayer = *(unsigned long*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0x7c007c00) >> 7;
						SG = (nColorSource & 0x03e003e0) >> 2;
						SB = (nColorSource & 0x001f001f) << 3;
						OR = (nColorOverlayer & 0x7c007c00) >> 7;
						OG = (nColorOverlayer & 0x03e003e0) >> 2;
						OB = (nColorOverlayer & 0x001f001f) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 7) & 0x7c007c00;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e003e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f001f;
						*(unsigned long*)ptSourceDest = SB | SG | SR;
						ptSourceDest = (unsigned long*)ptSourceDest + 1;
						ptOverLayerDest = (unsigned long*)ptOverLayerDest + 1;
					}
					ptSourceDest = (unsigned long*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned long*)ptOverLayerDest + 1;
				}
				ptSourceDest = (unsigned long*)ptSourceDest + (surfacePitch1 >> 1) - w;
				ptOverLayerDest = (unsigned long*)ptOverLayerDest + (surfacePitch2 >> 1) - w;
				if(odd){
					ptSourceDest = (unsigned short*)ptSourceDest - 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest - 1;
				}
			}
		}
	}
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	lpSurface->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	////lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);

	return;*/
}

void DrawStaturated(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne){
	/*static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	unsigned long Is4Multiple;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}
	void *ptSourceDest,*ptOverLayerDest;
	surfacePitch1 = ddsdSource.lPitch >> 1;
	surfacePitch2 = ddsdOverlayer.lPitch >> 1;
	ptSourceDest = (unsigned short*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
	ptOverLayerDest = (unsigned short*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;

	if(g_bMMXMode){
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			GMask = 0x07e007e007e007e0;
			GShift = 5;
		}
		else{
			GMask = 0x03e003e003e003e0;
			GShift = 6;
		}
		surfacePitch1 = ddsdSource.lPitch;
		surfacePitch2 = ddsdOverlayer.lPitch;

		__asm{
			push eax
			push ebx
			push ecx
			push edx
			push edi
			push esi
				
			mov	edi,dword ptr ptSourceDest			// Ŀ��ָ��
			mov esi,dword ptr ptOverLayerDest		// ��Դָ��
			push h															// ��¼ͼ��
Start:
			pop ecx															// ȡ��ͼ��
			cmp ecx,0
			je	End
			dec ecx
			push ecx
			mov	ecx,dword ptr Is4Multiple				// ��¼ͼ���Ƿ�Ϊ4�ı���
Loop1_0:
			cmp	ecx,0
			// ����4�ı���,���������������
			je	Start_4_Multiple
			// ������4�����ĵ�
			dec	ecx
			// һ�δ���һ����
			xor ebx,ebx
			mov bx,word ptr [esi]								// ������Դ��
			cmp ebx,0x0000											// �ǲ��� colorkey
			// ����colorkey,������һ��
			je	Loop1_1
			//-------------------------------------------------------
			movd mm0,ebx
			// ȡ�� r
			pand mm0,RMask
			psrlw	mm0,11
			// ȡ�� g
			movd mm1,ebx
			pand mm1,GMask
			psrlw mm1,GShift
			// ȡ�� b
			movd mm2,ebx
			pand mm2,BMask
			//-------------------------------------------------------
			mov	bx,word ptr [edi]								// ����Ŀ���
			movd mm3,ebx
			// ȡ�� r
			pand mm3,RMask
			psrlw	mm3,11
			// ȡ�� g
			movd mm4,ebx
			pand mm4,GMask
			psrlw mm4,GShift
			// ȡ�� b
			movd mm5,ebx
			pand mm5,BMask
			//-------------------------------------------------------
			// �������
			// ��Դ��Ŀ��
			psubusw mm0,mm3
			psubusw mm1,mm4
			psubusw mm2,mm5
			// �����Ŀ��
			paddusw mm0,mm3
			paddusw mm1,mm4
			paddusw mm2,mm5

			psllw mm0,11
			psllw mm1,5
			por	mm2,mm0
			por mm2,mm1
			//-------------------------------------------------------
			//���Ŀ���
			movd ebx,mm2
			mov	[edi],bx
Loop1_1:
			add edi,2
			add esi,2
			jmp Loop1_0
					
Start_4_Multiple:
			mov	ecx,dword ptr w									// ��¼ͼ��
			shr ecx,2
Loop2:
			cmp ecx,0
			je	ChangeLine
			dec	ecx
			//-------------------------------------------------------
			movq mm0,[edi]											// ����Ŀ���
			movq mm1,[esi]											// ������Դ��
			//-------------------------------------------------------
			// ȡ�� g
			movq mm4,mm0
			pand mm4,GMask
			psrlw mm4,GShift
			movq mm5,mm1
			pand mm5,GMask
			psrlw mm5,GShift
			// ȡ�� r
			movq mm2,mm0
			pand mm2,RMask
			psrlw mm2,11
			movq mm3,mm1
			pand mm3,RMask
			psrlw mm3,11
			// ȡ�� b
			pand mm0,BMask
			pand mm1,BMask
			//-------------------------------------------------------
			// �������
			// ��Դ��Ŀ��
			psubusw mm3,mm2
			psubusw mm5,mm4
			psubusw mm1,mm0
			// �����Ŀ��
			paddusw mm3,mm2
			paddusw mm5,mm4
			paddusw mm1,mm0

			psllw mm3,11
			psllw mm5,5
			por	mm1,mm3
			por mm1,mm5
			//-------------------------------------------------------
			//���Ŀ���
			movq [edi],mm1
			
			add edi,8
			add esi,8
			
			jmp Loop2
			//-------------------------------------------------------
			// ����
ChangeLine:
			mov ebx,w
			shl ebx,1												// һ�������byte,���Կ۵�wʱҪ��2
			add edi,surfacePitch1
			sub edi,ebx
			add esi,surfacePitch2
			sub esi,ebx
			jmp Start
End:
			pop esi
			pop edi
			pop edx
			pop ecx
			pop ebx
			pop eax
			emms
		}
	}
	else{
		int i,j,nColorSource,nColorOverlayer;
		unsigned long SR,SG,SB,OR,OG,OB;

		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xf800) >> 8; // ԭ��Ҫ����11����3,�򻯳�����8
						SG = (nColorSource & 0x07e0) >> 3; // ԭ��Ҫ����5����2,�򻯳�����3
						SB = (nColorSource & 0x001f) << 3;
						OR = (nColorOverlayer & 0xf800) >> 8;
						OG = (nColorOverlayer & 0x07e0) >> 3;
						OB = (nColorOverlayer & 0x001f) << 3;
						//								SR = (((SR + OR) > 0x00ff ? 0x00ff:SR+OR) << 8) & 0xf800;
						//								SG = (((SG + OG) > 0x00ff ? 0x00ff:SG+OG) << 3) & 0x07e0;
						//								SB = (((SB + OB) > 0x00ff ? 0x00ff:SB+OB) >> 3) & 0x001f;
						SR = ((OR > SR ? OR:SR) << 8) & 0xf800;
						SG = ((OG > SG ? OG:SG) << 3) & 0x07e0;
						SB = ((OB > SB ? OB:SB) >> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
				}
				ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2 - w;
			}
		}
		// 555 ��ʾģʽ
		else{
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					if(nColorOverlayer != DEF_COLORKEY){
						SR = nColorSource >> 7;							// ԭ��Ҫ����10����3,�򻯳�����7
						SG = (nColorSource & 0x03e0) >> 2;	// ԭ��Ҫ����5����3,�򻯳�����2
						SB = (nColorSource & 0x001f) << 3;
						OR = nColorOverlayer >> 7;
						OG = (nColorOverlayer & 0x03e0) >> 2;
						OB = (nColorOverlayer & 0x001f) << 3;
						//								SR = ((SR + OR) & 0x00ff)<< 7;
						//								SG = (((SG + OG) & 0x00ff)<< 2) & 0x03e0;
						//								SB = (((SB + OB) & 0x00ff)>> 3) & 0x001f;
						SR = ((OR > SR ? OR:SR) & 0x00ff)<< 7;
						SG = (((OG > SG ? OG:SG) & 0x00ff)<< 2) & 0x03e0;
						SB = (((OB > SB ? OB:SB) & 0x00ff)>> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
				}
				ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2 - w;
			}
		}
	}
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	lpSurface->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	////lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);

	return;*/
}

void DrawGray(SDL_Surface* lpSurface,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne,int iGrayType){
	/*static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2,nColorOverlayer;
	int i,j,w = SURFACE_WIDTH,h = SURFACE_HEIGHT,subx,dx = 0,dy = 0;
	int R,G,B,Gray,g;
	bool odd = false;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	subx = ox;
	
	if(iGrayType == 3) g = 6;
	else if(iGrayType == 4)	g = 5;

	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
		return;
	}
	if(displayBpp == 16){
		void *ptSourceDest,*ptOverLayerDest;
		surfacePitch1 = ddsdSource.lPitch >> 1;
		surfacePitch2 = ddsdOverlayer.lPitch >> 1;
		ptSourceDest = (unsigned short*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
		ptOverLayerDest = (unsigned short*)(ddsdOverlayer.lpSurface) +dy * surfacePitch2 + dx;
		
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(unsigned short*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = (nColorOverlayer & 0xf800) >> 11;
								G = (nColorOverlayer & 0x07e0) >> 6;
								B = nColorOverlayer & 0x001f;
								Gray = (R*3+G*6+B)/10;
								*(unsigned short*)ptSourceDest = (unsigned short)(Gray << 11 | Gray << g | Gray);
							}
						}
						else subx++;
						ptSourceDest = (unsigned short*)ptSourceDest + 1;
						ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
					}
					ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
		// 555 ��ʾģʽ
		else{
			for(j=0;j<h;j++){
				if(oy >= 0){
					for(i=0;i<w;i++){
						if(subx >= 0){
							nColorOverlayer = *(unsigned short*)ptOverLayerDest;
							if(nColorOverlayer != DEF_COLORKEY){
								R = nColorOverlayer >> 10;
								G = (nColorOverlayer & 0x03e0) >> 5;
								B = nColorOverlayer & 0x001f;
								Gray = (R*3+G*6+B)/10;
								*(unsigned short*)ptSourceDest = (unsigned short)(Gray << 10 | Gray << g | Gray);
							}
						}
						else subx++;
						ptSourceDest = (unsigned short*)ptSourceDest + 1;
						ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
					}
					ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1 - w;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2 - w;
					subx = ox;
				}
				else{
					ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2;
					oy++;
				}
			}
		}
	}
	lpSurface->Unlock(NULL);
	lpDraw->lpBACKBUFFERSYS->Unlock(NULL);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	////lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);
	*/
	return;
}

void DrawAlphaChannel(SURFACE_INFO *surface_info,unsigned char *AlphaData,int ox,int oy,int offsetx,int offsety,int ow,int oh,bool bLastOne){
	/*static DDSURFACEDESC ddsdSource,ddsdOverlayer;
	int surfacePitch1,surfacePitch2;
	int w = SURFACE_WIDTH,h = SURFACE_HEIGHT,dx = 0,dy = 0;
	unsigned long Is4Multiple;
	RECT rect;

	if(ox >= lpDraw->xSize || ox + w <= 0 || oy >= lpDraw->ySize || oy + h <= 0 ) return;
	if(bLastOne){
		w = ow - offsetx;
		h = oh - offsety;
	}
	if(ox + w >= lpDraw->xSize){
		w = lpDraw->xSize - ox;
	}
	if(oy + h >= lpDraw->ySize) h = lpDraw->ySize - oy;
	if(ox < 0){
		w = w + ox;
		if(w < 0) return;
		dx = ox*-1;
		ox = 0;
	}
	if(oy < 0){
		h = h + oy;
		if(h < 0) return;
		dy = oy*-1;
		oy = 0;
	}
	Is4Multiple = w % 4;
	
	ZeroMemory(&ddsdSource,sizeof(DDSURFACEDESC));
	ddsdSource.dwSize = sizeof(DDSURFACEDESC);
	ZeroMemory(&ddsdOverlayer,sizeof(DDSURFACEDESC));
	ddsdOverlayer.dwSize = sizeof(DDSURFACEDESC);
	if(lpDraw->lpBACKBUFFERSYS->Lock(NULL,&ddsdSource,DDLOCK_WAIT,NULL) != DD_OK) return;
	if(surface_info->lpSurface->Lock(NULL,&ddsdOverlayer,DDLOCK_WAIT | DDLOCK_READONLY ,NULL) != DD_OK){
		lpDraw->lpBACKBUFFERSYS->Unlock(&ddsdSource);
		return;
	}
	
	void *ptSourceDest,*ptOverLayerDest;
	surfacePitch1 = ddsdSource.lPitch >> 1;
	surfacePitch2 = ddsdOverlayer.lPitch >> 1;
	ptSourceDest = (unsigned short*)(ddsdSource.lpSurface) + oy * surfacePitch1 + ox;
	ptOverLayerDest = (unsigned short*)(ddsdOverlayer.lpSurface) + dy * surfacePitch2 + dx;

	if(g_bMMXMode){
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			GMask = 0x07e007e007e007e0;
			GShift = 5;
		}
		else{
			GMask = 0x03e003e003e003e0;
			GShift = 6;
		}
		surfacePitch1 = ddsdSource.lPitch;
		surfacePitch2 = ddsdOverlayer.lPitch;
		__asm{
			push eax
			push ebx
			push ecx
			push edx
			push edi
			push esi

			mov	edi,dword ptr ptSourceDest			// Ŀ��ָ��
			mov esi,dword ptr ptOverLayerDest		// ��Դָ��
			mov eax,dword ptr AlphaData					// alpha ָ��
			push h															// ��¼ͼ��
Start:
			pop ecx															// ȡ��ͼ��
			cmp ecx,0
			je	End
			dec ecx
			push ecx
			mov	ecx,dword ptr Is4Multiple				// ��¼ͼ���Ƿ�Ϊ4�ı���
Loop1_0:
			cmp	ecx,0
			// ����4�ı���,����alpha�������
			je	Start_4_Multiple
			// ������4�����ĵ�
			dec	ecx
			// һ�δ���һ����
			xor ebx,ebx
			mov bx,word ptr [edi]								// ������Դ��
			cmp ebx,0x0000											// �ǲ��� colorkey
			// ����colorkey,������һ��
			je	Loop1_1
//-------------------------------------------------------
			// ȡ�� r,g,b
			movd mm0,ebx
			movd mm1,ebx
			movd mm2,ebx

			pand mm0,RMask
			pand mm1,GMask
			pand mm2,BMask

			psrlw	mm0,11
			psrlw mm1,GShift
//-------------------------------------------------------
			mov	bx,word ptr [esi]								// ����Ŀ���

			// ȡ��r,g,b
			movd mm3,ebx
			movd mm4,ebx
			movd mm5,ebx

			pand mm3,RMask
			pand mm4,GMask
			pand mm5,BMask

			psrlw	mm3,11
			psrlw mm4,GShift
//-------------------------------------------------------
			xor ebx,ebx
			mov bl,byte ptr [eax]								// ����alpha ֵ
			movd mm6,ebx
//-------------------------------------------------------
			// ����alpha
			psubw	mm3,mm0
			psubw	mm4,mm1
			psubw	mm5,mm2

			pmullw	mm3,mm6
			pmullw	mm4,mm6
			pmullw	mm5,mm6

			psrlw	mm3,5
			psrlw	mm4,5
			psrlw	mm5,5

			paddsw	mm3,mm0
			movd	ebx,mm3
			shl	ebx,11
			
			paddsw	mm4,mm1
			movd	edx,mm4
			shl	edx,5
			or	ebx,edx
			
			paddsw	mm5,mm2
			movd	edx,mm5
			or	ebx,edx
//-------------------------------------------------------
			//���Ŀ���
			mov	[edi],bx
Loop1_1:
			add edi,2
			add esi,2
			add eax,1
			jmp Loop1_0

Start_4_Multiple:
			mov	ecx,dword ptr w									// ��¼ͼ��
			shr ecx,2
Loop2:
			cmp ecx,0
			je	ChangeLine
			dec	ecx
//-------------------------------------------------------
			movq mm1,[edi]											// ����Ŀ���
			movq mm0,[esi]											// ������Դ��
			mov ebx,dword ptr [eax]							// ����alpha ֵ
			movd mm6,ebx
			punpcklbw mm6,mm6
			pand mm6,AlphaMask
//-------------------------------------------------------
			//movq mm7,mm1
			//pcmpeqw	mm7,MaskColorKey						// ����color key,���colorkey��ͬ���word�ᱻ�趨Ϊ1,����Ϊ0
			//psubusw mm1,mm7
			//pand mm7,mm0
			//por	mm1,mm7
			
			// ȡ�� g,b,r
			movq mm2,mm0
			movq mm3,mm1
			movq mm4,mm0
			movq mm5,mm1

			pand mm2,RMask
			pand mm3,RMask
			pand mm4,GMask
			pand mm5,GMask

			psrlw mm2,11
			psrlw mm3,11
			psrlw mm4,GShift
			psrlw mm5,GShift
			
			pand mm0,BMask
			pand mm1,BMask
//-------------------------------------------------------
			// ����alpha
			psubw mm0,mm1
			psubw mm4,mm5
			psubw mm2,mm3

			pmullw mm0,mm6
			pmullw mm4,mm6
			pmullw mm2,mm6

			psrlw mm0,5
			psrlw mm4,5
			psrlw mm2,5

			paddsw mm0,mm1
			paddsw mm4,mm5
			paddsw mm2,mm3

			psllw mm4,5
			psllw mm2,11

			por	mm0,mm4
			por	mm0,mm2
//-------------------------------------------------------
			//���Ŀ���
			movq [edi],mm0

			add edi,8
			add esi,8
			add eax,4

			jmp Loop2
//-------------------------------------------------------
			// ����
ChangeLine:
			mov ebx,w
			shl ebx,1												// һ�������byte,���Կ۵�wʱҪ��2
			add edi,surfacePitch1
			sub edi,ebx
			add esi,surfacePitch2
			sub esi,ebx
			add eax,SURFACE_WIDTH
			sub eax,w
			jmp Start
End:
			pop esi
			pop edi
			pop edx
			pop ecx
			pop ebx
			pop eax
			emms
		}
	}
	else{
		int i,j,nColorSource,nColorOverlayer;
		unsigned long SR,SG,SB,OR,OG,OB,Alpha;
		// 565 ��ʾģʽ
		if(gBitRShift == 2){
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					Alpha = (unsigned short)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = (nColorSource & 0xf800) >> 8; // ԭ��Ҫ����11����3,�򻯳�����8
						SG = (nColorSource & 0x07e0) >> 3; // ԭ��Ҫ����5����2,�򻯳�����3
						SB = (nColorSource & 0x001f) << 3;
						OR = (nColorOverlayer & 0xf800) >> 8;
						OG = (nColorOverlayer & 0x07e0) >> 3;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = (((((OR - SR) * Alpha) >> 5) + SR) << 8) & 0xf800;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 3) & 0x07e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
					AlphaData++;
				}
				ptSourceDest = (unsigned short*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (unsigned short*)ptOverLayerDest + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
		}
		// 555 ��ʾģʽ
		else{
			for(j=0;j<h;j++){
				for(i=0;i<w;i++){
					nColorSource = *(unsigned short*)ptSourceDest;
					nColorOverlayer = *(unsigned short*)ptOverLayerDest;
					Alpha = (unsigned short)(*AlphaData);
					if(nColorOverlayer != DEF_COLORKEY){
						SR = nColorSource >> 7;							// ԭ��Ҫ����10����3,�򻯳�����7
						SG = (nColorSource & 0x03e0) >> 2;	// ԭ��Ҫ����5����3,�򻯳�����2
						SB = (nColorSource & 0x001f) << 3;
						OR = nColorOverlayer >> 7;
						OG = (nColorOverlayer & 0x03e0) >> 2;
						OB = (nColorOverlayer & 0x001f) << 3;
						SR = ((((OR - SR) * Alpha) >> 5) + SR) << 7;
						SG = (((((OG - SG) * Alpha) >> 5) + SG) << 2) & 0x03e0;
						SB = (((((OB - SB) * Alpha) >> 5) + SB) >> 3) & 0x001f;
						*(unsigned short*)ptSourceDest = (unsigned short)(SB | SG | SR);
					}
					ptSourceDest = (unsigned short*)ptSourceDest + 1;
					ptOverLayerDest = (unsigned short*)ptOverLayerDest + 1;
					AlphaData++;
				}
				ptSourceDest = (unsigned long*)ptSourceDest + surfacePitch1 - w;
				ptOverLayerDest = (unsigned long*)ptOverLayerDest + surfacePitch2 - w;
				AlphaData += SURFACE_WIDTH - w;
			}
		}
	}
	lpDraw->lpBACKBUFFERSYS->Unlock(&ddsdSource);
	surface_info->lpSurface->Unlock(&ddsdOverlayer);
	rect.left = ox;
	rect.top = oy;
	rect.right = ox+w;
	rect.bottom = oy+h;
	////lpDraw->lpBACKBUFFER->BltFast(ox,oy,lpDraw->lpBACKBUFFERSYS,&rect,DDBLTFAST_WAIT);
	return;*/
}
#endif
