/************************/
/*	battleMap.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
//#include "../caryIme.h"
#include "../systeminc/battleMap.h"
#include "math.h"
#include <stdlib.h>

// ??????????
#define BATTLE_MAP_SIZE 400

#ifdef _NEWDRAWBATTLEMAP		   // (���ɿ���) Syu ADD �Զ�����BattleMap
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
int RandBattleBg = 0;
#endif

#ifdef __ATTACK_MAGIC
extern SDL_Surface* screenSurface;
extern SDL_Surface* battleSurface;

// Global vars
extern int			g_iRunEarthQuake;			// �����״��: 0 --> û���� , 1 --> ��ʼ������ , 2 --> ������
extern int			g_iCurRunEarthQuake;		// Ŀǰ�����λ����
extern int			g_iNumRunEarthQuake;		// ������ܴ���

#endif

#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif

// ?????????�k?
int BattleMapNo = 0;

#ifdef _NEWDRAWBATTLEMAP_FIX		   // (���ɿ�)ROG ADD ս����ͼ�޸�
int BattleMapAdvance = 0;			   // ս����ͼЧ�ܸĽ�
#endif
// ?????? ????��
char *BattleMapFile[] = {	
	#include "../systeminc/battleMapName.h"
};
						
// ??????��????????????????�� ***************************/
// ˮ���ر�ս����ͼ���
static int iCrystalBattleMap[4][3] =
{	// ��								// ��								// ��								// ��
	{29060,29061,29062},{29058,29059,29066},{29063,29064,29065},{29057,29067,29068}
};

bool ReadBattleMap( int no )
{
#ifdef _NEWDRAWBATTLEMAP_FIX		   // (���ɿ�)ROG ADD ս����ͼ�޸�
	extern void ddrawBattleMap();
	ddrawBattleMap();
#else
	char msg[ 256 ];
	FILE *fp;
	char header[ 5 ];
	unsigned short tile[ BATTLE_MAP_SIZE ];
	int cnt = 0;
	int i, j, x = 0, y = 0, posX, posY;
	unsigned short c1, c2;
	// ????????
	if( no >= BATTLE_MAP_FILES )
		no = 0;
	// ??????�k???
	BattleMapNo = no;
	// ??????????????
	if( ( fp = fopen( BattleMapFile[ no ], "rb" ) ) == NULL ){
		sprintf( msg, "%s��ȡʧ�ܡ�", BattleMapFile[ no ] );
		////MessageBox( hWnd, msg, "ս����ͼ����", MB_OK | MB_ICONSTOP );
		return false;
	}
	// ?????��???
	fread( header, sizeof( char ), 4, fp );
	header[ 4 ] = '\0';
	// ????????
	if( !( strstr( header, "SAB" )) )
    {
        //Jerry Error not sab;
    }
		////MessageBox( hWnd, "SAB ����������", "ս����ͼ����", MB_OK | MB_ICONSTOP );
	// ��??????
	for( i = 0 ; i < BATTLE_MAP_SIZE ; i++ ){
		// ??????��???? ????�B????? ?
		c1 = fgetc( fp );
		c2 = fgetc( fp );
		// ???????
		tile[ i ] = ( c1 << 8 ) | c2;
	}
	// ????��??
	fclose( fp );
	//ROG FIX ս����ͼ��ͼλַ����
	posX = 32 * ( -9 ) + (DISPLACEMENT_X >> 1);
	posY = 24 * 10 + (DISPLACEMENT_Y >> 1);
#ifdef _NEWDRAWBATTLEMAP		   // Syu ADD �Զ�����BattleMap
	extern int nowFloor ; 

	if(((nowFloor == 817) || (nowFloor == 8007) || (nowFloor == 8101) || (nowFloor == 8100) ||
		  (nowFloor == 8027) || (nowFloor == 8028) || (nowFloor == 8029) || (nowFloor == 8015) ||
			(nowFloor == 151) || (nowFloor == 160) || (nowFloor == 161) || (nowFloor >= 30022 && nowFloor <= 30025) || nowFloor == 8113 || nowFloor == 8114
#ifdef _8BATTLEMAP
		|| (nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 || (nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600
#endif
			)
		&& ( RandBattleBg == 0 )  )
	{
		RandBattleBg = RAND ( 0 , 4 );
		if ( RandBattleBg == 0 ) RandBattleBg = 29001 ;
		else if ( RandBattleBg == 1 ) RandBattleBg = 29020 ;
		else if ( RandBattleBg == 2 ) RandBattleBg = 29019 ;
		else if ( RandBattleBg == 3 ) RandBattleBg = 29024 ;
		else if ( RandBattleBg == 4 ) RandBattleBg = 29025 ;
		
		RandBattleBg = RAND ( 29047 , 29049 );
		// ˮ���ر�ս����ͼ
		if(nowFloor == 30022) RandBattleBg = iCrystalBattleMap[0][RAND(0,2)];
		else if(nowFloor == 30023) RandBattleBg = iCrystalBattleMap[1][RAND(0,2)];
		else if(nowFloor == 30024) RandBattleBg = iCrystalBattleMap[2][RAND(0,2)];
		else if(nowFloor == 30025) RandBattleBg = iCrystalBattleMap[3][RAND(0,2)];
#ifdef _8BATTLEMAP
		else if((nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 ) RandBattleBg = 29074+RAND(0,2);
		else if((nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600 ) RandBattleBg = 29070+RAND(0,3);
#endif
		StockDispBuffer ( 320 , 240 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}
	else if ( ((nowFloor == 817) || (nowFloor == 8007) || (nowFloor == 8101) || (nowFloor == 8100)
		|| (nowFloor == 8027) || (nowFloor == 8028) || (nowFloor == 8029) || (nowFloor == 8015) || ( nowFloor == 151 )
		|| (nowFloor == 160) || (nowFloor == 161)	|| (nowFloor >= 30022 && nowFloor <= 30025) || nowFloor == 8113 || nowFloor == 8114
#ifdef _8BATTLEMAP
		|| (nowFloor >= 78 && nowFloor <= 99) || nowFloor == 8107 || (nowFloor >= 8108 && nowFloor <= 8112) || (nowFloor >= 8116 && nowFloor <= 8120) || (nowFloor >= 8124 && nowFloor <= 8126) || nowFloor == 7500 || nowFloor == 7530 || nowFloor == 7600
#endif
		)
		&& (RandBattleBg != 0) )
		StockDispBuffer ( 320 , 240 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
#ifdef _SYUTEST2
	else { 
		RandBattleBg = 29050;
		StockDispBuffer ( 320 , 240 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}
#else
	else if(nowFloor == 8519){
		RandBattleBg = 29069;
		StockDispBuffer ( 320 , 240 , DISP_PRIO_TILE , RandBattleBg , 0 ) ;
	}
	else
	{
		for( i = 0 ; i < 20 ; i++ )
		{
			x = 0;
			y = 0;
			for( j = 0 ; j < 20 ; j++ )
			{
				StockDispBuffer( posX + x, posY + y, DISP_PRIO_TILE, tile[ cnt++ ], 0 );
				x += 32;
				y -= 24;
			}
		posX += 32;
		posY += 24;
		}
	}
#endif

//	extern void ddrawBattleMap();
//	ddrawBattleMap();

#else
	// ???��???

	for( i = 0 ; i < 20 ; i++ ){
		x = 0;
		y = 0;
		for( j = 0 ; j < 20 ; j++ ){
			// ???����????????
			StockDispBuffer( posX + x, posY + y, DISP_PRIO_TILE, tile[ cnt++ ], 0 );
			x += 32;
			y -= 24;
		}
		posX += 32;
		posY += 24;
	}
#endif
#endif
	return true;
}	

// ??????��? ***********************************************************/
#ifdef _NEWDRAWBATTLEMAP_FIX		   // (���ɿ�)ROG ADD ս����ͼ�޸�
#define	RASTER_CLEARANCE	24		   //����Ч��
#else
#define	RASTER_CLEARANCE	8
#endif
int piyo_tbl[]={
	0,1,2,3,4,5,6,7,8,9,10,10,11,11,11,12,
	12,12,11,11,11,10,9,9,8,7,6,5,4,3,2,1,
	0,-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,-10,-11,-11,-11,-12,
	-12,-12,-11,-11,-11,-10,-9,-9,-8,-7,-6,-5,-4,-3,-2,-1,
};
int piyo_point = 0;

void DrawBattleMap( void )
{
	SDL_Rect rect = { 0, 0, DEF_APPSIZEX, DEF_APPSIZEY };
	int d0,d1,d6,d7,value;
	short bx , by;
	


	#ifdef __ATTACK_MAGIC

	// �����ͼ��Ż������

#ifdef _NEWDRAWBATTLEMAP		   // Syu ADD �Զ�����BattleMap
	extern int nowFloor;
	if( ( BattleMapNo >= 148 && BattleMapNo <= 150 ) || 2 == g_iRunEarthQuake || 
		nowFloor == 817 || nowFloor == 8007 || nowFloor == 8101 || nowFloor == 8100 || 
		nowFloor == 8027 || nowFloor == 8028 || nowFloor == 8029 || nowFloor == 8015 || nowFloor == 8113 || nowFloor == 8114 ){
#else
	if( ( BattleMapNo >= 148 && BattleMapNo <= 150 ) || 2 == g_iRunEarthQuake ){
#endif
	#else

	if(BattleMapNo >= 148 && BattleMapNo <= 150){		//???????????

	#endif

		d1 = 1;
	} else {
		d1 = 0;
	}

	switch( d1 ){
	case 0:
        {
		bx = 0;
		by = 0;
		short x0, y0;
		long w, h;
		// ��???????
		x0 = bx;
		y0 = by;
		w = rect.w;
		h = rect.h;
		// ????????
		//   ????? RECT ????��?????����?????

		// �u?����???��?��?????
		//if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
		//	return;
		//}
		//// ?�Z?????
		//if( bx < 0 ){
		//	rect.x -= bx;
		//	x0 = 0;
		//}
		//// ?�Z?????
		//if( bx + w > lpDraw->xSize ){
		//	rect.y -= w - lpDraw->xSize;
		//}
		//// �ƣZ?????
		//if( by < 0 ){
		//	rect.y -= by;
		//	y0 = 0;
		//}
		//// ?�Z?????
		//if( by + h > lpDraw->ySize ){
		//	rect.h -= h - lpDraw->ySize;
		//}
#ifdef _DEBUG		
		// ??����???????????????
		SurfaceDispCnt++;
#endif
		// ??????????�L��?
        SDL_Rect destRect = {x0, y0, 0, 0};
        SDL_BlitSurface(battleSurface, &rect, screenSurface, &destRect);
		////lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#ifdef _READ16BITBMP
		//if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(x0,y0,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
        }
		break;
	case 1:
        {
		value		 = piyo_point;
		d6			 = 0;

		#ifdef __ATTACK_MAGIC

		value		+= g_iCurRunEarthQuake;

		#endif

		rect.x = 0;		//?�Z
		rect.w = 640 + DISPLACEMENT_X;		//?�Z
	/*	if(BattleMapAdvance < 10){
			BattleMapAdvance ++;
			return;
		}
		else
			BattleMapAdvance =0;*/

		for(d7=0; d7 < 480 + DISPLACEMENT_Y; d7+=RASTER_CLEARANCE){
			rect.y = d7 + piyo_tbl[(d6+value) & 63] + 12;		//�ƣZ
			rect.h = RASTER_CLEARANCE;		//?�Z
			SDL_Rect destRect = {0, d7, 0, 0};
        	SDL_BlitSurface(battleSurface, &rect, screenSurface, &destRect);
			// ??????????�L��?
			////lpDraw->lpBACKBUFFER->BltFast( 0, d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#ifdef _READ16BITBMP
			//if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(0,d7,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
			d6++;
		}
        }
		break;
	case 2:
		d6 = 0;
		for(d7=0; d7<480 + DISPLACEMENT_Y; d7+=RASTER_CLEARANCE/2){

			rect.x = d7;		//�ƣZ
			rect.h =   RASTER_CLEARANCE;		//?�Z

			d0 = piyo_tbl[(d6+piyo_point) & 63] / 2;		//?�Z
			if(d0 < 0){		//??????
				rect.x = 0 - d0;
				rect.w = 640 + DISPLACEMENT_X+ d0 - rect.x;		//?�Z
				d0 = 0;
			} else {
				rect.x = 0;
				rect.w = 640 + DISPLACEMENT_X- d0;		//?�Z
			}
			SDL_Rect destRect = {d0, d7, 0, 0};
        	SDL_BlitSurface(battleSurface, &rect, screenSurface, &destRect);
			// ??????????�L��?
			////lpDraw->lpBACKBUFFER->BltFast( d0, d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
#ifdef _READ16BITBMP
			//if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->BltFast(d0,d7,lpBattleSurfaceSys,&rect,DDBLTFAST_WAIT);
#endif
			d6++;
		}
		break;
	}
}
