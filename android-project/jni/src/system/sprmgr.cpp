/************************/
/*	sprmgr.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"

extern SDL_Surface* CreateSurface( short, short, int, int );


SDL_Surface* lpBattleSurface;
#ifdef _READ16BITBMP
SDL_Surface* lpBattleSurfaceSys;
#endif

#ifdef _DEBUG
int SurfaceDispCnt;
#endif

int SurfaceSizeX;
int SurfaceSizeY;
unsigned int SurfaceDate = 3;

#define SURACE_INFO_MAX 3072 //1024
#define SURACE_BMP_DEATH_DATE 2
SPRITE_INFO SpriteInfo[ MAX_GRAPHICS ];
SURFACE_INFO SurfaceInfo[ SURACE_INFO_MAX ];
#ifdef _READ16BITBMP
SURFACE_INFO SurfaceInfoSys[SURACE_INFO_MAX];
extern bool g_bUseAlpha;
#endif
int SurfaceSearchPoint;

#ifdef _DEBUG		
int SurfaceUseCnt;
extern int g_iMallocCount;
#endif

int SurfaceCnt;
int VramSurfaceCnt;
int SysramSurfaceCnt;
#ifdef _READ16BITBMP
void AllocateBmpToSurface( int bmpNo,bool useAlpha )
#else
void AllocateBmpToSurface( int bmpNo)
#endif
{
	int bmpWidth  = RealBinWidth;	// ????????
	int bmpHeight = RealBinHeight; 	// ???????
	
	int offsetX, offsetY; 			// ???????????????
	int sizeX, sizeY;				// ??????
	int surfaceCntX;				// ???????????
	int surfaceCntY;				// ???????????
	int totalSurface;				// ??????????
	int totalSurfaceCnt = 0;		// ??????????????? ??????? ?
	int	SurfaceSearchPointBak = SurfaceSearchPoint; // ??????????????????
	int amariSizeX = false;		// ???????????
	int amariSizeY = false;		// ??????????
	bool vramFullFlag = false; 		// VRAM?????????
	SURFACE_INFO *prevSurfaceInfo; 	// ????????????????
	offsetX = 0; 
#ifdef _READ16BITBMP
	SURFACE_INFO *prevSurfaceInfoSys;
	SDL_Rect rect = {0,0,SurfaceSizeX,SurfaceSizeY};
	if(bmpNo >= OLD_GRAPHICS_START) offsetY = 0;
	else
#endif
	offsetY = bmpHeight;
	surfaceCntX = bmpWidth / SURFACE_WIDTH;
	if( ( amariSizeX = bmpWidth % SURFACE_WIDTH ) ){ 
		surfaceCntX++;
	}
	
	surfaceCntY = bmpHeight / SURFACE_HEIGHT;
	if( ( amariSizeY = bmpHeight % SURFACE_HEIGHT ) ){ 
		surfaceCntY++;		// ??????
	}
	totalSurface  = surfaceCntX * surfaceCntY;
#ifdef _READ16BITBMP
	if(bmpNo < OLD_GRAPHICS_START)
#endif
	{
		if((RealBinWidth & 3)) RealBinWidth += 4 - RealBinWidth & 3;
	}
	while( 1 ){
		if( SurfaceInfo[ SurfaceSearchPoint ].date < SurfaceDate - SURACE_BMP_DEATH_DATE ){
#ifdef _DEBUG		
			SurfaceUseCnt++;
#endif
			// 已在使用中
			if( SurfaceInfo[ SurfaceSearchPoint ].bmpNo != -1 ){
				SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
				SURFACE_INFO *lpSurfaceInfoSys;
#endif
				lpSurfaceInfo = SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfo;
				SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfo = NULL;
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					lpSurfaceInfoSys = SpriteInfo[ SurfaceInfo[ SurfaceSearchPoint ].bmpNo ].lpSurfaceInfoSys;
					SpriteInfo[SurfaceInfo[SurfaceSearchPoint].bmpNo].lpSurfaceInfoSys = NULL;
				}
#endif
				for( ; lpSurfaceInfo != NULL;
					lpSurfaceInfo = lpSurfaceInfo->pNext ){
					lpSurfaceInfo->bmpNo = -1;
#ifdef _READ16BITBMP
					if(g_bUseAlpha)	lpSurfaceInfoSys->bmpNo = -1;
#endif
#ifdef _DEBUG		
					SurfaceUseCnt--;
#endif
				}
			}
			if( SpriteInfo[ bmpNo ].lpSurfaceInfo == NULL ){
				// 第一张图的surfaceinfo
				SpriteInfo[ bmpNo ].lpSurfaceInfo = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	SpriteInfo[bmpNo].lpSurfaceInfoSys = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
			}else{
				// 指向下一张surfaceinfo
				prevSurfaceInfo->pNext = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	prevSurfaceInfoSys->pNext = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
			}
			SurfaceInfo[ SurfaceSearchPoint ].bmpNo = bmpNo;
#ifdef _READ16BITBMP
			if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].bmpNo = bmpNo;
#endif
			if(ResoMode == 1){
				SurfaceInfo[ SurfaceSearchPoint ].offsetX = offsetX / 2;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetX = offsetX / 2;
				if(bmpNo >= OLD_GRAPHICS_START){
					SurfaceInfo[SurfaceSearchPoint].offsetY = offsetY / 2;
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = offsetY / 2;
				}
				else
#endif
				{
					SurfaceInfo[ SurfaceSearchPoint ].offsetY = ( bmpHeight - offsetY ) / 2;
#ifdef _READ16BITBMP
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = ( bmpHeight - offsetY ) / 2;
#endif
				}
			}
			else{
				SurfaceInfo[ SurfaceSearchPoint ].offsetX = offsetX;
#ifdef _READ16BITBMP
				if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetX = offsetX;
				if(bmpNo >= OLD_GRAPHICS_START){
					SurfaceInfo[SurfaceSearchPoint].offsetY = offsetY;
					if(g_bUseAlpha) SurfaceInfoSys[SurfaceSearchPoint].offsetY = offsetY;
				}
				else
#endif
				{
					SurfaceInfo[ SurfaceSearchPoint ].offsetY = bmpHeight - offsetY;
#ifdef _READ16BITBMP
					if(g_bUseAlpha)	SurfaceInfoSys[SurfaceSearchPoint].offsetY = bmpHeight - offsetY;
#endif
				}
			}

			if( offsetX >= bmpWidth - SURFACE_WIDTH && amariSizeX ){
				ClearSurface( SurfaceInfo[ SurfaceSearchPoint ].lpSurface );
#ifdef _READ16BITBMP
				if(g_bUseAlpha){
					ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
					if(SurfaceInfo[SurfaceSearchPoint].lpAlphaData)
						memset(SurfaceInfo[SurfaceSearchPoint].lpAlphaData,0,SurfaceSizeX*SurfaceSizeY);
				}
#endif
				sizeX = amariSizeX;				
			}
			else sizeX = SURFACE_WIDTH;
#ifdef _READ16BITBMP
			if(bmpNo >= OLD_GRAPHICS_START){
				if(offsetY >= bmpHeight - SURFACE_HEIGHT && amariSizeY){
					ClearSurface(SurfaceInfo[SurfaceSearchPoint].lpSurface);
					if(g_bUseAlpha){
						ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
						if(SurfaceInfo[SurfaceSearchPoint].lpAlphaData)
							memset(SurfaceInfo[SurfaceSearchPoint].lpAlphaData,0,SurfaceSizeX*SurfaceSizeY);
					}
					sizeY = amariSizeY;
				}else sizeY = SURFACE_HEIGHT;
			}
			else
#endif
			{
				if( offsetY - SURFACE_HEIGHT <= 0 && amariSizeY ){
					if( sizeX != amariSizeX ){
						ClearSurface( SurfaceInfo[ SurfaceSearchPoint ].lpSurface );
#ifdef _READ16BITBMP
						if(g_bUseAlpha) ClearSurface(SurfaceInfoSys[SurfaceSearchPoint].lpSurface);
#endif
					}
					sizeY = amariSizeY;
				}else sizeY = SURFACE_HEIGHT;
			}

#ifdef _READ16BITBMP
			SurfaceInfo[SurfaceSearchPoint].useAlpha = useAlpha;
			SurfaceInfoSys[SurfaceSearchPoint].useAlpha = useAlpha;
			if(bmpNo >= OLD_GRAPHICS_START){
				Draw16BitmapToSurface2(&SurfaceInfo[SurfaceSearchPoint],
									SurfaceInfoSys[SurfaceSearchPoint].lpSurface,
									offsetX, 
									offsetY,
									sizeX,
									sizeY,
									NULL);
			}
			else{
				if(g_bUseAlpha){
					DrawBitmapToSurface2(SurfaceInfo[ SurfaceSearchPoint ].lpSurface,
									SurfaceInfoSys[SurfaceSearchPoint].lpSurface,
									offsetX, 
									offsetY - 1, 
									sizeX,
									sizeY,
									NULL );
				}
				else{
					DrawBitmapToSurface2( 	SurfaceInfo[ SurfaceSearchPoint ].lpSurface, 
									offsetX, 
									offsetY - 1, 
									sizeX,
									sizeY,
									NULL );
				}
			}
#else
			DrawBitmapToSurface2( 	SurfaceInfo[ SurfaceSearchPoint ].lpSurface, 
									offsetX, 
									offsetY - 1, 
									sizeX,
									sizeY,
									NULL );
#endif
			totalSurfaceCnt++;
			// 所有的图都已存入offscreen
			if( totalSurfaceCnt >= totalSurface ){
				SurfaceInfo[ SurfaceSearchPoint ].pNext = NULL;
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	SurfaceInfoSys[SurfaceSearchPoint].pNext = NULL;
#endif
				SurfaceSearchPoint++;

				if( SurfaceSearchPoint >= SurfaceCnt ) 
                {
                        SurfaceSearchPoint = 0;
                }
				break;
			}else{
				// 作连结, SurfaceInfo[ SurfaceSearchPoint ] 就是 SpriteInfo[ bmpNo ].lpSurfaceInfo
				// prevSurfaceInfo 是为了要作 link
				prevSurfaceInfo = &SurfaceInfo[ SurfaceSearchPoint ];
#ifdef _READ16BITBMP
				if(g_bUseAlpha)	prevSurfaceInfoSys = &SurfaceInfoSys[SurfaceSearchPoint];
#endif
				if( offsetX >= bmpWidth - SURFACE_WIDTH ){ 
					offsetX = 0;
#ifdef _READ16BITBMP
					if(bmpNo >= OLD_GRAPHICS_START) offsetY += SURFACE_HEIGHT;
					else
#endif
					offsetY -= SURFACE_HEIGHT;
				}else{ 
					offsetX += SURFACE_WIDTH;
				}
			}
		}
		SurfaceSearchPoint++;
		// 不能大于 surfaceinfo 的最大值(正常为2048)
		if( SurfaceSearchPoint >= SurfaceCnt ) SurfaceSearchPoint = 0;
		// 若相等则表示找了一轮都没有可用的surfaceinfo
		if( SurfaceSearchPoint == SurfaceSearchPointBak ) break;
	}
}

bool InitOffScreenSurface( void )
{
    SurfaceCnt = SURACE_INFO_MAX;
    return true;
	int i;
	bool vramFullFlag = false;
	SurfaceCnt = 0;
	VramSurfaceCnt = 0;
	SysramSurfaceCnt = 0;
	for( i = 0 ; i < SURACE_INFO_MAX ; i++ ){
		if( vramFullFlag == false ){
			if( ( SurfaceInfo[ i ].lpSurface = CreateSurface( SurfaceSizeX, SurfaceSizeY, DEF_COLORKEY, 1 )) == NULL ){
#ifdef _DEBUG
				//MessageBox( hWnd ,"SurfaceInfo:建立VideoRAM Surface失败！" ,"确定",MB_OK | MB_ICONSTOP );
#endif
				vramFullFlag = true;
			}else{
				VramSurfaceCnt++;
			}
		}
		if( vramFullFlag == true ){
			if( ( SurfaceInfo[ i ].lpSurface = 
                CreateSurface( SurfaceSizeX, SurfaceSizeY, DEF_COLORKEY, 1 )) == NULL ){
#ifdef _DEBUG
				//MessageBox( hWnd ,"建立SysRAM Surface失败！" ,"确定",MB_OK | MB_ICONSTOP );
#endif
				return false;
			}else SysramSurfaceCnt++;
		}
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if((SurfaceInfo[i].lpAlphaData = (unsigned char*)malloc(SurfaceSizeX*SurfaceSizeY)) == NULL){
	#ifdef _DEBUG
				//MessageBox( hWnd ,"alpha记忆体配置失败！","确定",MB_OK | MB_ICONSTOP);
	#endif
				return false;
			}
			else{
#ifdef _DEBUG
				g_iMallocCount++;
#endif
				SysramSurfaceCnt++;
			}
		}
#endif
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if((SurfaceInfoSys[i].lpSurface = CreateSurface(SurfaceSizeX,SurfaceSizeY,DEF_COLORKEY,0 )) == NULL){
	#ifdef _DEBUG
				//MessageBox(hWnd,"建立SysRAM Surface(2)失败！","确定",MB_OK | MB_ICONSTOP);
	#endif
				return false;
			}
			else SysramSurfaceCnt++;
		}
#endif
	}
	SurfaceCnt = i;
	InitSurfaceInfo();
	InitSpriteInfo();
	return true;
}

void InitSurfaceInfo( void )
{
	int i;
#ifdef _DEBUG		
	SurfaceUseCnt = 0;
#endif
	SurfaceSearchPoint = 0;
	for( i = 0 ; i < SurfaceCnt ; i++ ){
		SurfaceInfo[ i ].bmpNo = -1;
		SurfaceInfo[ i ].date = 0;
		SurfaceInfo[ i ].pNext = NULL;
#ifdef _READ16BITBMP
		SurfaceInfo[i].useAlpha = false;
		SurfaceInfoSys[i].bmpNo = -1;
		SurfaceInfoSys[i].date = 0;
		SurfaceInfoSys[i].pNext = NULL;
		SurfaceInfoSys[i].useAlpha = false;
#endif
	}
}	

void InitSpriteInfo( void )
{
	int i;
	for( i = 0 ; i < MAX_GRAPHICS ; i++ ){
		SpriteInfo[ i ].lpSurfaceInfo = NULL;
#ifdef _READ16BITBMP
		SpriteInfo[i].lpSurfaceInfoSys = NULL;
#endif
	}
}	

bool LoadBmp( int bmpNo )
{
#ifndef _READ16BITBMP
	if((unsigned)bmpNo > MAX_GRAPHICS) return false;
	if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){
		if( realGetImage(bmpNo,(unsigned char **)&pRealBinBits,
										 &RealBinWidth,&RealBinHeight ) == false ){
			return false;
		}
		SpriteInfo[ bmpNo ].width = RealBinWidth;
		SpriteInfo[ bmpNo ].height = RealBinHeight;
		AllocateBmpToSurface( bmpNo );
	}
	return true;
#else
	bool useAlpha;

	if((unsigned)bmpNo >= OLD_GRAPHICS_START){
		if((unsigned)bmpNo >= MAX_GRAPHICS) return false;
		if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){
			pRealBinAlpha = NULL;
			if(Read16BMP(bmpNo - OLD_GRAPHICS_START,(unsigned char **)&pRealBinBits,
									 &RealBinWidth,&RealBinHeight,(unsigned char **)&pRealBinAlpha,&useAlpha) == false) return false;
			SpriteInfo[bmpNo].width = RealBinWidth;
			SpriteInfo[bmpNo].height = RealBinHeight;
			AllocateBmpToSurface(bmpNo,useAlpha);
		}
		return true;
	}
	else{
		if(SpriteInfo[bmpNo].lpSurfaceInfo == NULL){            //Jerry Get BMP
			if(realGetImage(bmpNo,(unsigned char **)&pRealBinBits,
											 &RealBinWidth,&RealBinHeight) == false) return false;
			SpriteInfo[bmpNo].width = RealBinWidth;
			SpriteInfo[bmpNo].height = RealBinHeight;
			AllocateBmpToSurface(bmpNo,false);
		}
		return true;
	}
#endif
}
