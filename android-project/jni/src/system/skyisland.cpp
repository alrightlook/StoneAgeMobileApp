
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/map.h"
#include "../systeminc/pc.h"
#include <stdlib.h>

unsigned long sky_island_no = 0;
unsigned long sky_island_no_bak = 0;

extern SDL_Surface* CreateSurface( short, short, int, int );

SDL_Surface* lpsurLand = NULL;
SDL_Surface* lpsurCloud1 = NULL;
SDL_Surface* lpsurCloud2 = NULL;

#ifdef _MOON_FAIRYLAND				// (不可开) ROG ADD 月之仙境
bool moonFlag  = false;
#endif
short LandWidth, LandHeight;
short CloudWidth1, CloudHeight1;
short CloudWidth2, CloudHeight2;
int fMapWidth, fMapHeight;
int iCloudX1, iCloudY1, iCloudX2, iCloudY2;
int iCloudDX, iCloudDY;
bool bDrawUpCloud;
SDL_Rect g_rc;
extern void SkyIslandDraw();
extern int displayBpp;
extern unsigned short highColorPalette[256];
#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif

void LoadBmpToSurface( int noFrom, SDL_Surface* lpsurTo, int w, int h)
{
	/*DDSURFACEDESC ddsd;
	ZeroMemory( &ddsd, sizeof(DDSURFACEDESC) );
	ddsd.dwSize = sizeof(DDSURFACEDESC);
	if( lpsurTo->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL) == DD_OK ){
		if( realGetImage( 	noFrom, 
							( unsigned char **)&pRealBinBits, 
							&RealBinWidth, 
							&RealBinHeight ) ){
			// realbin档图素要倒着读
			pRealBinBits += (w * (h-1));
			if( displayBpp == 16){
				LPWORD surface;
				surface = (LPWORD)ddsd.lpSurface;
				// 高彩一个点要用2个byte,所以pitch除2方便填颜色值
				ddsd.lPitch >>= 1; // ddsd.lPitch /= 2;
				unsigned short pixel;
				for( short i = 0; i < h; i++){
					for( short j = 0; j < w; j++){
						pixel = highColorPalette[(unsigned char)pRealBinBits[j]];
						// 透明色不处理
						if( pixel) surface[j] = pixel;
					}
					// 换下一行
					surface += ddsd.lPitch;
					pRealBinBits -= w;
				}
			}else{
				LPBYTE surface;
				surface = (LPBYTE)ddsd.lpSurface;
				for( short i = 0; i < h; i++){
					CopyMemory( surface, pRealBinBits, w);
					surface += ddsd.lPitch;
					pRealBinBits -= w;
				}
			}
		}
		lpsurTo->Unlock( NULL);
	}*/
}

void SkyIslandInit()
{
	sky_island_no = 0;
	sky_island_no_bak = 0;
	LandWidth = 0;
	LandHeight = 0;
	CloudWidth1 = CloudHeight1 = 0;
	CloudWidth2 = CloudHeight2 = 0;
	iCloudX1 = iCloudY1 = 0;
	iCloudX2 = iCloudY2 = 0;
	iCloudDX = iCloudDY = 0;
}

void SkyIslandRelease()
{
	LandWidth = 0;
	CloudWidth1 = 0;
	CloudWidth2 = 0;
	if( lpsurLand){
		SDL_FreeSurface(lpsurLand);
		lpsurLand = NULL;
	}
	if( lpsurCloud1){
        SDL_FreeSurface(lpsurCloud1);
		lpsurCloud1 = NULL;
	}
	if( lpsurCloud2){
		SDL_FreeSurface(lpsurCloud2);
		lpsurCloud2 = NULL;
	}
}

void SkyIslandLoadBmp()
{
	if( sky_island_no){
		short w, h;
		unsigned long no;
		realGetNo( sky_island_no & 0xffff, &no );
		realGetWH( no, &w, &h);
		if( w != LandWidth || h != LandHeight){
			fMapWidth = ((nowFloorGxSize + nowFloorGySize) * SURFACE_WIDTH) >> 1; // fMapWidth = (nowFloorGxSize+nowFloorGySize)*SURFACE_WIDTH/2;
			fMapHeight = ((nowFloorGxSize + nowFloorGySize) * SURFACE_HEIGHT) >> 1; //fMapWidth = (nowFloorGxSize+nowFloorGySize)*SURFACE_WIDTH/2;
			LandWidth = w;
			LandHeight = h;
			if( lpsurLand){
				SDL_FreeSurface(lpsurLand);
				lpsurLand = NULL;
			}
			lpsurLand = CreateSurface( w, h, 0, 0);
		}
		if( lpsurLand){
			LoadBmpToSurface( no, lpsurLand, w, h);
			if( !lpsurCloud1){
					realGetNo( 40500+(rand()&0x3), &no);
				realGetWH( no, &w, &h);
				CloudWidth1 = w;
				CloudHeight1 = h;
				iCloudX1 = rand() % w;
				iCloudY1 = rand() % h;

				if( lpsurCloud1 = CreateSurface( 640, 480, 0, 0) ){
					LoadBmpToSurface( no, lpsurCloud1, w, h);
				}
			}
			if( !lpsurCloud2){
				short w, h;
				unsigned long no;
					realGetNo( 40500+(rand()&0x3), &no);
				realGetWH( no, &w, &h);
				CloudWidth2 = w;
				CloudHeight2 = h;
				iCloudX2 = rand() % w;
				iCloudY2 = rand() % h;
				if( lpsurCloud2 = CreateSurface( 640, 480, 0, 0) ){
					LoadBmpToSurface( no, lpsurCloud2, w, h);
				}
			}			
		}

	}
}

void SkyIslandSetNo( int fl)
{
	sky_island_no_bak = sky_island_no;
	sky_island_no = 0;

#ifdef _MOON_FAIRYLAND				// (不可开) ROG ADD 月之仙境
	moonFlag =	false;
#endif
	switch( fl){
//避免重复的loadmap动作
	case 30691:
		sky_island_no = (1<<16) | 40510;
		break;
	case 30692:
		sky_island_no = (2<<16) | 40510;
		break;
	case 30693:
		sky_island_no = (3<<16) | 40510;
		break;
	case 30694:
		sky_island_no = (4<<16) | 40510;
		break;
	case 30695:
		sky_island_no = (5<<16) | 40510;
		break;
	case 30689:
		sky_island_no = (6<<16) | 40510;
		break;
	case 5581://金飞航空
		sky_island_no = (1<<16) | 40511;
		break;
#ifdef _MOON_FAIRYLAND				// (不可开) ROG ADD 月之仙境
	case 8255:
		sky_island_no = (1<<16) | 40512;
		moonFlag =	true;
		break;
	case 8256:
		sky_island_no = (2<<16) | 40512;
		moonFlag =	true;
		break;
	case 8257:
		sky_island_no = (3<<16) | 40512;
		moonFlag =	true;
		break;
#endif

#ifdef _LOST_FOREST_FOG
	case 7450:
	case 7451:
		sky_island_no = (1<<16);
		break;
#endif
	// 神台
	case 104:
		sky_island_no = (2<<16) | 40511;
		break;
	}
	if( sky_island_no_bak != sky_island_no){
		SkyIslandLoadBmp();
		int x,y;
		
		x = ((nowGx + nowGy) * (SURFACE_WIDTH >> 1));
		y = ((nowFloorGySize + nowGy - nowGx) * (SURFACE_HEIGHT >> 1));
		g_rc.x = ((LandWidth-DEF_APPSIZEX)*x/fMapWidth);
		g_rc.y = ((LandHeight-DEF_APPSIZEY)*y/fMapHeight);
		g_rc.w = DEF_APPSIZEX;
		g_rc.y =  DEF_APPSIZEY;
	}
}

void SkyIslandDraw()
{
	static unsigned long dwSlow1;
	static int compx = (int)pc.ptAct->mx,compy = (int)pc.ptAct->my;
	int x,y;
	unsigned long now;
	SDL_Rect rc;

	if( sky_island_no && lpsurCloud1 && lpsurLand && pc.ptAct){
		bDrawUpCloud = true;
		if(compx != (int)pc.ptAct->mx || compy != (int)pc.ptAct->my){
			switch(pc.dir){
			// 下
			case 0:	g_rc.y++; break;
			// 左下
			case 1:
				g_rc.x--;
				g_rc.y++;
				break;
			// 左
			case 2:	g_rc.x--; break;
			// 左上
			case 3:
				g_rc.x--;
				g_rc.y--;
				break;
			// 上
			case 4:	g_rc.y--; break;
			// 右上
			case 5:
				g_rc.x++;
				g_rc.y--;
				break;
			// 右
			case 6:	g_rc.x++; break;
			// 右下
			case 7:
				g_rc.x++;
				g_rc.y++;
				break;
			}
			compx = (int)pc.ptAct->mx;
			compy = (int)pc.ptAct->my;
		}
		g_rc.w =  DEF_APPSIZEX;
		g_rc.h =  DEF_APPSIZEY;
		//SetRect(&rc,g_rc.left,g_rc.top,g_rc.right,g_rc.bottom);
		if(g_rc.x < 0){
			rc.x = 0;
			rc.w = DEF_APPSIZEX;
		}
		if(g_rc.x < 0){
			rc.x = 0;
			rc.h =  DEF_APPSIZEY;
		}
		if(g_rc.w > LandWidth){
			rc.w = LandWidth;
			rc.x = LandWidth - DEF_APPSIZEX;
		}
		if(g_rc.h > LandHeight){
			rc.h = LandHeight;
			rc.y = LandHeight - DEF_APPSIZEY;
		}
		////lpDraw->lpBACKBUFFER->BltFast( 0, 0, lpsurLand, &rc, DDBLTFAST_WAIT);	
#ifdef _READ16BITBMP
		//if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast( 0, 0, lpsurLand, &rc, DDBLTFAST_WAIT);	
#endif
#ifdef _MOON_FAIRYLAND				// (不可开) ROG ADD 月之仙境
		if(moonFlag == true) return;
#endif

		if( (now = TimeGetTime()) > dwSlow1){
			dwSlow1 = now + 177;
			iCloudX1 += iCloudDX;
			iCloudY1 += iCloudDY;
			if(((iCloudX1>=DEF_APPSIZEX) || (iCloudY1>=DEF_APPSIZEY)) ){
				//产生一朵新的云
#ifdef __CARYTEST
				iCloudNo1 = 40500+(rand()&0x3);
				unsigned long no;
				short w, h;
				realGetNo( iCloudNo1, &no);
				realGetWH( no, &w, &h);
				CloudWidth1 = w;
				CloudHeight1 = h;
#else
				unsigned long no;
				short w, h;
					realGetNo( 40500+(rand()&0x3), &no);
				realGetWH( no, &w, &h);
				if( w!=CloudWidth1 || h!=CloudHeight1){
					CloudWidth1 = w;
					CloudHeight1 = h;
					LoadBmpToSurface( no, lpsurCloud1, w, h);
				}
#endif
				int dx = 0, dy = 0;
				switch( rand()%3){
				case 0:
					dx = rand() & 0x1f;
					break;
				case 1:
					dy = rand() & 0x1f;
					break;
				default:
					break;
				}
				iCloudX1 = -CloudWidth1 + dx + 40;
				iCloudY1 = -CloudHeight1 + dy + 30;
			}
		}
	//		int x, y;
			x = iCloudX1;
			y = iCloudY1;
			//SetRect( &rc, 0, 0, CloudWidth1, CloudHeight1);
			if( x < 0){
				rc.x = -x;
				x = 0;
			}
			if( (x+CloudWidth1) >= DEF_APPSIZEX){
				rc.w = DEF_APPSIZEX - x;
			}
			if( y < 0){
				rc.y = -y;
				y = 0;
			}
			if( (y+CloudHeight1) >= DEF_APPSIZEY){
				rc.h = DEF_APPSIZEY -y;
			}
			if( rc.h >0 && rc.w>0){
				////lpDraw->lpBACKBUFFER->BltFast( x, y, lpsurCloud1, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
#ifdef _READ16BITBMP
				//if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast( x, y, lpsurCloud1, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
#endif

		}
	}
}

void SkyIslandDraw2()
{
#ifdef _MOON_FAIRYLAND				// (不可开) ROG ADD 月之仙境
	if(moonFlag == true)
	return;
#endif

	static unsigned long dwSlow2;
	unsigned long now;
	SDL_Rect rc;
	if( bDrawUpCloud){
		bDrawUpCloud = false;
		if( sky_island_no && lpsurCloud2 
			){
			if( (now = TimeGetTime()) > dwSlow2){
				dwSlow2 = now + 73;
				iCloudX2 += iCloudDX;
				iCloudY2 += iCloudDY;
				if( (iCloudX2>=DEF_APPSIZEX) || //((iCloudX2+CloudWidth2)<0) ||
					(iCloudY2>=DEF_APPSIZEY) /*|| ((iCloudY2+CloudHeight2)<0)*/){
					//产生一朵新的云
					unsigned long no;
						realGetNo( 40500+(rand()&0x3), &no);
					short w, h;
					realGetWH( no, &w, &h);
					if( w!=CloudWidth2 || h!=CloudHeight2){
						CloudWidth2 = w;
						CloudHeight2 = h;
						LoadBmpToSurface( no, lpsurCloud2, w, h);
					}
					int dx = 0, dy = 0;
					switch( rand()%3){
					case 0:
						dx = rand() & 0x1f;
						break;
					case 1:
						dy = rand() & 0x1f;
						break;
					default:
						break;
					}
					iCloudX2 = -CloudWidth2 + dx + 40;
					iCloudY2 = -CloudHeight2 + dy + 30;
				}
			}
			int x, y;
			x = iCloudX2;
			y = iCloudY2;
			//SetRect( &rc, 0, 0, CloudWidth2, CloudHeight2);
			if( x < 0){
				rc.x = -x;
				x = 0;
			}
			if( (x+CloudWidth2) >= DEF_APPSIZEX){
				rc.w = DEF_APPSIZEX - x;
			}
			if( y < 0){
				rc.y = -y;
				y = 0;
			}
			if( (y+CloudHeight2) >= DEF_APPSIZEY){
				rc.h = DEF_APPSIZEY -y;
			}
			/*if( (rc.bottom-rc.top)>0 && (rc.right-rc.left)>0){
				//lpDraw->lpBACKBUFFER->BltFast( x, y, lpsurCloud2, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
#ifdef _READ16BITBMP
				if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast( x, y, lpsurCloud2, &rc, DDBLTFAST_WAIT|DDBLTFAST_SRCCOLORKEY);
#endif
			}*/
		}
	}
}

bool SkyIslandCheckSurface()
{
	if( lpsurLand)
		//if( lpsurLand->IsLost() )
			return true;
	if( lpsurCloud1)
		//if( lpsurCloud1->IsLost() )
			return true;
	return false;
}

void SkyIslandProc()
{
	static unsigned long speed = 0;
	if( sky_island_no){
		unsigned long now = TimeGetTime();
		if( now > speed){
			speed = now + 1000;
			int dx = 0, dy = 0;
			switch( rand() & 0x3){
			case 0:
				dx = rand() & 0x2;
				break;
			case 1:
				break;
			default:
				dy = rand() & 0x2;
				break;
			}
			iCloudDX = 4 - dx;
			iCloudDY = 2 + dy;
		}
	}
}
