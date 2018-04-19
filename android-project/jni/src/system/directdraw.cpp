/**** SYSTEM INCLUDE ****/
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
//#include<direct.h>
//#include<errno.h>

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/map.h"
#include "../systeminc/anim_tbl.h"
#include "SDL.h"
#include "../newproto/autil.h"
#include <SDL_ttf.h>
#include <string>

#define PAL_CHANGE_INTERVAL_WIN		120		// ????��??????????????
#define PAL_CHANGE_INTERVAL_FULL	60		// ????��????????????????

extern int blitFlag;
//---------------------------------------------------------------------------//
// ?????��?�e?                                                        //
//---------------------------------------------------------------------------//
DIRECT_DRAW	*lpDraw;		// DirectDraw??????????
bool DDinitFlag = false;	// DirectDraw??????

int hFont;				// ????????
extern SDL_Renderer* mainRenderer;

int displayBpp = 32;			// ?����?

int rBitLShift;				// ?????????�k
int gBitLShift;				// ?????????�k
int bBitLShift;				// ?????????�k

int rBitRShift;				// ?????????�k
int gBitRShift;				// ?????????�k
int bBitRShift;				// ?????????�k

PALETTEENTRY	Palette[256];	// ?????�B�l
SDL_Color palColors[256];
SDL_Palette* mainPalette;
PALETTE_STATE 	PalState;		// ????�֢t?�B�l
bool 			PalChangeFlag;	// ???????????
#ifdef _HI_COLOR_32
SDL_Color highColor32Palette[256];
#endif
unsigned short highColorPalette[256];

extern SDL_Texture* screenTexture;
struct RGBQUAD;
#ifdef _READ16BITBMP
extern RGBQUAD g_rgbPal[256];
bool g_bUseAlpha = false;
#endif

#ifdef _READ16BITBMPVARIABLES	//�� _READ16BITBMP ����Ҫ�Ĳ���
bool g_bUseAlpha = false;
#endif

#ifdef _TALK_WINDOW
bool g_bTalkWindow = false;
#endif

#ifdef _DEBUG
extern int g_iMallocCount;
#endif
void SetAnimTbl();

// ???????????
char *palFileName[] = {	
	#include "../systeminc/palName.h"
};

const int MAX_PAL = sizeof( palFileName )/sizeof( palFileName[0] );
// ??�k????????????��??
int getBitCount( int bit )
{
	int i, j, k;
	j = 1;
	k = 0;
	for( i = 0; i < sizeof( int )*8; i++ ){
		if( (bit & j) )
			k++;
		j <<= 1;
	}
	return k;
}

#ifdef SWITCH_MODE
DEVMODE g_OriginalMode;
#endif

extern SDL_Surface* screenSurface;
extern SDL_Surface* CreateSurface( short, short, int, int );

bool InitDirectDraw( void )
{
//	if( ( lpBattleSurface = CreateSurface( DEF_APPSIZEX, DEF_APPSIZEY, DEF_COLORKEY, 1 )) == NULL ){
//#ifdef _DEBUG
//		//MessageBox( hWnd ,"����VideoRam BattleSurfaceʧ�ܣ�" ,"ȷ��",MB_OK | MB_ICONSTOP );
//#endif
//		if( ( lpBattleSurface = CreateSurface( DEF_APPSIZEX, DEF_APPSIZEY, DEF_COLORKEY, 1 )) == NULL ){
//			//MessageBox( hWnd ,"����BattleSurfaceʧ�ܣ�" ,"ȷ��",MB_OK | MB_ICONSTOP );
//			return false;
//		}
//	}
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha){
//		if((lpBattleSurfaceSys = CreateSurface(DEF_APPSIZEX,DEF_APPSIZEY,DEF_COLORKEY,1)) == NULL){
//			//MessageBox(hWnd,"����BattleSurface(sys)ʧ�ܣ�","ȷ��",MB_OK | MB_ICONSTOP);
//			return false;
//		}
//	}
//#endif
//#ifdef __SKYISLAND
//	ClearSurface( lpBattleSurface);
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha) ClearSurface(lpBattleSurfaceSys);
//#endif
//	extern void SkyIslandLoadBmp();
//	SkyIslandLoadBmp();
//#endif
//	DDinitFlag = true;	
	SetAnimTbl();
	return true;
}


void PaletteForColors(PALETTEENTRY* pal)
{
    extern SDL_Surface* screenSurface;
    for(int i = 0 ; i < 256; i++)
    {
        palColors[i].r = pal[i].peRed;
        palColors[i].g = pal[i].peGreen;
        palColors[i].b = pal[i].peBlue;
    }
}

bool InitPalette( void )
{
	int i;
	FILE *fp;
	
	PALETTEENTRY pal[ 32 ]={
		// ?????��
		{0x00 ,0x00, 0x00, 0 | 2}, // 0:?
		{0x80 ,0x00, 0x00, 0 | 2}, // 1:??��
		{0x00 ,0x80, 0x00, 0 | 2}, // 2:???
		{0x80 ,0x80, 0x00, 0 | 2}, // 3:???
		{0x00 ,0x00, 0x80, 0 | 2}, // 4:??��
		{0x80 ,0x00, 0x80, 0 | 2}, // 5:???
		{0x00 ,0x80, 0x80, 0 | 2}, // 6:???��
		{0xc0 ,0xc0, 0xc0, 0 | 2}, // 7:???
		{0xc0 ,0xdc, 0xc0, 0 | 2}, // 8:?
		{0xa6 ,0xca, 0xf0, 0 | 2}, // 9:?
		
		//��ϵͳɫ��// ��????????
		{ 0xde, 0x00, 0x00, 0 | 2 },
		{ 0xff, 0x5f, 0x00, 0 | 2 },
		{ 0xff, 0xff, 0xa0, 0 | 2 },
		{ 0x00, 0x5f, 0xd2, 0 | 2 },
		{ 0x50, 0xd2, 0xff, 0 | 2 },
		{ 0x28, 0xe1, 0x28, 0 | 2 },
		
		//��ϵͳɫ��// ��????????
		{ 0xf5, 0xc3, 0x96, 0 | 2 },
		{ 0xe1, 0xa0, 0x5f, 0 | 2 },
		{ 0xc3, 0x7d, 0x46, 0 | 2 },
		{ 0x9b, 0x55, 0x1e, 0 | 2 },
		{ 0x46, 0x41, 0x37, 0 | 2 },
		{ 0x28, 0x23, 0x1e, 0 | 2 },
		
		// ?????��
		{0xff ,0xfb, 0xf0, 0 | 2}, // 246:?
		{0xa0 ,0xa0, 0xa4, 0 | 2}, // 247:?
		{0x80 ,0x80, 0x80, 0 | 2}, // 248:?��
		{0xff ,0x00, 0x00, 0 | 2}, // 249:��
		{0x00 ,0xff, 0x00, 0 | 2}, // 250:?
		{0xff ,0xff, 0x00, 0 | 2}, // 251:?
		{0x00 ,0x00, 0xff, 0 | 2}, // 252:��
		{0xff ,0x00, 0xff, 0 | 2}, // 253:?
		{0x00 ,0xff, 0xff, 0 | 2}, // 254:?��
		{0xff ,0xff, 0xff, 0 | 2}  // 255:?
	};


	// ?????????�@�e
	for( i = 0; i < 10; i++ ){
		Palette[i].peBlue  	= pal[i].peBlue;
		Palette[i].peGreen 	= pal[i].peGreen;
		Palette[i].peRed 	= pal[i].peRed;
		Palette[i].peFlags 	= 3;
		
		Palette[i+246].peBlue  	= pal[i+22].peBlue;
		Palette[i+246].peGreen 	= pal[i+22].peGreen;
		Palette[i+246].peRed 	= pal[i+22].peRed;
		Palette[i+246].peFlags 	= 3;
	}
	
	// ��?????????�@�e
	for( i = 0; i < 6; i++ ){
		Palette[i+10].peBlue  	= pal[i+10].peBlue;
		Palette[i+10].peGreen 	= pal[i+10].peGreen;
		Palette[i+10].peRed 	= pal[i+10].peRed;
		Palette[i+10].peFlags 	= 0 | 2;
		
		Palette[i+240].peBlue  	= pal[i+16].peBlue;
		Palette[i+240].peGreen 	= pal[i+16].peGreen;
		Palette[i+240].peRed 	= pal[i+16].peRed;
		Palette[i+240].peFlags 	= 0 | 2;
	}
	
	//ֻ�е�һ�β���(û�г�ʼ��ʱ)// ???????????�k?????
	if( PalState.flag == false ){
		// ????????????
#ifdef PROFILE_TEST
		fp = fopen("/sdcard/jerrysa/data/pal/Palet_1.sap", "rb" );
#else
		fp = fopen( palFileName[ 0 ], "rb" );
#endif
		if(fp == NULL){
			//MessageBox(hWnd,"ɫ�̵���ȡʧ��","Error",MB_OK| MB_ICONSTOP);
			return false;
		}
		else{
			//������ʹ�õĵ�ɫ���趨// ��??��???????�@�e
			for( i = 16; i < 240; i++ ){
				//��������// ?????��???
				Palette[i].peBlue  	= fgetc( fp );
				Palette[i].peGreen 	= fgetc( fp );
				Palette[i].peRed 	= fgetc( fp );
				Palette[i].peFlags = 0 | 2;
			}
			fclose( fp );
		}
	}
	if( !transmigrationEffectFlag )
	{
		Palette[168].peBlue  	= 0;
		Palette[168].peGreen 	= 0;
		Palette[168].peRed 		= 0;
		//Palette[168].peFlags 	= 3;
	}

#if 0
	else{
		// ��??��???????�@�e
		for( i = 16; i < 240; i++ ){
			// ??????????
			if( WindowMode ){
				Palette[i].peFlags = 0 | 2;
			}else{
				Palette[i].peFlags = 3;
			}
		}
	}
#endif

    PaletteForColors(Palette);
    //SDL_FreePalette(mainPalette);
    //mainPalette = SDL_AllocPalette(256);
    //int res = SDL_SetPaletteColors(mainPalette, palColors, 0, 256);
    //res = SDL_SetSurfacePalette(screenSurface, mainPalette);
    screenTexture = SDL_CreateTextureFromSurface(mainRenderer, screenSurface);
	//lpDraw->lpDD2->CreatePalette( DDPCAPS_8BIT, Palette, &lpDraw->lpPALETTE, NULL );
	/*if( lpDraw->lpPALETTE == NULL ){
		//MessageBox(hWnd, "��ɫ�̴���ʧ��", "Error", MB_OK| MB_ICONSTOP );
		return false;
	}*/
// WON REM 
	// Robin 05/02
#ifdef SWITCH_MODE
	if( lpDraw->lpFRONTBUFFER->SetPalette( lpDraw->lpPALETTE ) != DD_OK ){
		if( //MessageBox(hWnd, "��ʹ�ã�����ɫ����ʾģʾ", "ȷ��", MB_RETRYCANCEL | MB_ICONEXCLAMATION ) == IDCANCEL)
			return false;
		return false;
	}
#else
	/*while( lpDraw->lpFRONTBUFFER->SetPalette( lpDraw->lpPALETTE ) != DD_OK ){
		if( //MessageBox(hWnd, "��ʹ�ã�����ɫ����ʾģʾ", "ȷ��", MB_RETRYCANCEL | MB_ICONEXCLAMATION ) == IDCANCEL)
			return false;
	}*/
#endif


		// ????????��??????��
        highColor32Palette[0].r = 0;
        highColor32Palette[0].g = 0;
        highColor32Palette[0].b = 0;
        highColor32Palette[0].a = 0;

		for( i = 1; i < 256; i++ ){
            highColor32Palette[i].r = Palette[i].peRed;
            highColor32Palette[i].g = Palette[i].peGreen;
            highColor32Palette[i].b = Palette[i].peBlue;
            highColor32Palette[i].a = 1;
        }
	PalState.flag = true;
	return true;
}

// ???????? ***********************************************************/
void PaletteChange( int palNo, int time )
{
	// ????????
	if( palNo >= MAX_PAL )
		return;
	// ????�k?
	PalState.palNo = palNo;
	// ????��???
	PalState.time = time;
	// ????????
	if( PalState.time <= 0 )
		PalState.time = 1;
}

// ɫ�̴��� ***************************************************************/
void PaletteProc( void )
{
	FILE *fp; // ????????
	static PALETTEENTRY	pal[256];	// ?????�B�l
	static float	dRed[256];		// ?????��
	static float	dGreen[256];	// ?????��
	static float	dBlue[256];		// ?????��
	static float	dRedBak[256];	// ��?��????
	static float	dGreenBak[256];	// ��?��????
	static float	dBlueBak[256];	// ��?��????
	static int 	timeCnt;			// ????????
	static int 	changeCnt;			// ?????????
	static int 	palNoBak = 0;		// ?????
	static int 	openFlag = false;	// ???????????
	int i;
	// ?????��???????
	if( palNoBak == PalState.palNo && openFlag == false )
		return;
	// ???????????
	if( palNoBak != PalState.palNo ){
		// ????????????
		fp = fopen( palFileName[ PalState.palNo ], "rb" );
		// ��??��???????�@�e
		for( i = 16; i < 240; i++ ){
			pal[i].peBlue  	= fgetc( fp );
			pal[i].peGreen 	= fgetc( fp );
			pal[i].peRed 	= fgetc( fp );
			// 168?��?( 0, 0, 0 )?��?
			if( i == 168 ){
				pal[168].peBlue  	= 0;
				pal[168].peGreen 	= 0;
				pal[168].peRed 		= 0;
			}
			// ��?????
			dBlueBak[ i ] 	= 	Palette[i].peBlue;
			dGreenBak[ i ] 	= 	Palette[i].peGreen;
			dRedBak[ i ] 	= 	Palette[i].peRed;
			// ?��?��
			dBlue[ i ] 	= 	( float )( pal[ i ].peBlue - Palette[i].peBlue ) 	/ ( float )PalState.time;
			dGreen[ i ] = 	( float )( pal[ i ].peGreen - Palette[i].peGreen )	/ ( float )PalState.time;
			dRed[ i ] 	= 	( float )( pal[ i ].peRed - Palette[i].peRed ) 		/ ( float )PalState.time;
#if 0
			// ??????????
			if( WindowMode ){
				Palette[i].peFlags = 0 | 2;
			}else{
				Palette[i].peFlags = 3;
			}
#endif
		}
		fclose( fp );				// ????��??
		timeCnt = 0;				// ��?��???????????????????
		changeCnt = 0;				// ��????????????????????;
		palNoBak = PalState.palNo;	// ??????
		openFlag = true;			// ?????
	}
	// ?????
	timeCnt++;
	// ��????
	if( timeCnt <= PalState.time ){	
		// ��??��???????�@�e
		for( i = 16; i < 240; i++ ){
				// ��?��????
			dBlueBak[ i ] 	+= 	dBlue[ i ];
			dGreenBak[ i ]	+=	dGreen[ i ];
			dRedBak[ i ]	+=	dRed[ i ];
			// ��?????
			Palette[i].peBlue  	= ( unsigned char )dBlueBak[ i ];
			Palette[i].peGreen 	= ( unsigned char )dGreenBak[ i ];
			Palette[i].peRed 	= ( unsigned char )dRedBak[ i ];
		}
	}else{ 
		// ??????????�u??????�k????
		// ��??��???????�@�e
		for( i = 16; i < 240; i++ ){
			Palette[i].peBlue  	= pal[ i ].peBlue;
			Palette[i].peGreen 	= pal[ i ].peGreen;
			Palette[i].peRed 	= pal[ i ].peRed;
		}
		openFlag = false;	// ??????
		transEffectPaletteStatus = 2;
	}
	// ???????????����?��??????
	if( changeCnt == 0 || openFlag == false ){
		// ?????????????
		PalChangeFlag = true;
		// ??????��?�@�e
		//lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette );
	}
	// ????????
	changeCnt++;
	// ??????????
	if( transmigrationEffectFlag )
	{
		// ����??????
		if( changeCnt >= 10 ) changeCnt = 0;
	}
	if( WindowMode ){
		if( changeCnt >= PAL_CHANGE_INTERVAL_WIN )
			changeCnt = 0;
	}else{
		if( changeCnt >= PAL_CHANGE_INTERVAL_FULL )
			changeCnt = 0;
	}
}	

//---------------------------------------------------------------------------//
// ?? ��?���i?�_?????????????????????????       //
// ?? ��??                         										 //
// ?�k ��??                                                               //
//---------------------------------------------------------------------------//

//extern	RECT	g_clientRect;
//extern	POINT	g_clientPoint;
//extern	RECT	g_moveRect;

void Flip( void )
{
	return;
}

//---------------------------------------------------------------------------//
// ?? ��?????????????                                         //
// ?? ��DIRECT_DRAW *lpDraw : DirectDraw???�B�l                         //
// ?�k ��??                                                               //
//---------------------------------------------------------------------------//
void ClearBackSurface( void )
{
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0,0,0));
}

//---------------------------------------------------------------------------//
// ?? ��??????????                 		                         //
// ?? ��DIRECT_DRAW *lpDraw : DirectDraw???�B�l                         //
// ?�k ��??                                                               //
//---------------------------------------------------------------------------//
void ClearSurface( SDL_Surface* lpSurface )
{
	/*DDBLTFX ddbltfx;

	ZeroMemory( &ddbltfx, sizeof( DDBLTFX ) );
	ddbltfx.dwSize = sizeof( DDBLTFX );
	ddbltfx.dwFillColor = DEF_COLORKEY; // �a����?�@�e
	
	lpSurface->Blt( NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx );*/
    if (lpSurface == 0)
    {
        return;
    }
    SDL_FillRect(lpSurface, 0, SDL_MapRGB(lpSurface->format, 0,0,0));
	return;
}

// ???????????????????
int BmpOffBits;
//---------------------------------------------------------------------------//
// ?? ��???????????��??????????��                     //
// ?? ��char * pFile  : ??????????��                             //
// ??�k��LPBITMAPINFO : NULL .????��Or????����                     //
//                NULL??.LPBITMAPINFO????                              //
//---------------------------------------------------------------------------//
//LPBITMAPINFO LoadDirectDrawBitmap( char *pFile )
//{
//	HFILE hFile;
//	OFSTRUCT ofSt;
//	BITMAPFILEHEADER BmpFileHeader;
//	LPBITMAPINFO lpBmpInfo;
//
//	//????��???
//	if( ( hFile = OpenFile( pFile, &ofSt, OF_READ ) ) == HFILE_ERROR )
//		return (LPBITMAPINFO)NULL; // File Open Error
//
//	//??????????????��???
//	_hread( hFile, &BmpFileHeader, sizeof(BITMAPFILEHEADER) );
//
//	//??????????��
//	if( (lpBmpInfo = (LPBITMAPINFO)HeapAlloc( GetProcessHeap(),HEAP_ZERO_MEMORY, BmpFileHeader.bfSize )) == NULL ){
//		//MessageBox( hWnd, "Heap�����ü�����ʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return (LPBITMAPINFO)NULL; //Memory Error
//	}
//	
//	//??????��?��?
//	_hread( hFile, (void *)lpBmpInfo, BmpFileHeader.bfSize );
//
//	//????????
//	_lclose( hFile );
//	
//	// ???????????????????
//	BmpOffBits = BmpFileHeader.bfOffBits - sizeof( BITMAPFILEHEADER );
//	
//	return lpBmpInfo;
//}

//---------------------------------------------------------------------------//
// ?? ��????????????��                                         //
// ?? ��short bxsize           : ?�θϐD???Ρ�)                       //
//        short bysize           : ?�θϐD?c?(Ρ�)                       //
//        unsigned long ColorKey         : �a����???��k?(0?255)                //
// ??�k��������? ... ?????????? / ���� ... NULL                 //
//---------------------------------------------------------------------------//
SDL_Surface* CreateSurface( short bxsize, short bysize, int ColorKey, int VramOrSysram )
{
	SDL_Surface* lpSurface;

    lpSurface = createSDLSurface(bxsize, bysize, 0);
    return lpSurface;
}


//---------------------------------------------------------------------------//
// ?? ��???????���e??????��?                                 //
// ?? ��SDL_Surface* lpSurface : ��??????                     //
//        short Xpoint        : ��?��??????�t                         //
//        short Ypoint        :	��?��????��?�t                         //
//        LPBITMAPINFO pInfo  : ��??????????�B�l                   //
// ?�k ��??                                                               //
//---------------------------------------------------------------------------//
// ??????????��??? ? StretchDIBits ?��? ?
void DrawBitmapToSurface( SDL_Surface* lpSurface, int offsetX, int offsetY, void* pBmpInfo )
{
	//HDC hDcDest;
	//// ���e?????�k????????????��?
	//lpSurface->GetDC( &hDcDest );
	//StretchDIBits( 	hDcDest, 
	//				0, 0, 
	//				//cary
	//				pBmpInfo->bmiHeader.biWidth, pBmpInfo->bmiHeader.biHeight, 
	//				//SurfaceSizeX, SurfaceSizeY,
	//				offsetX, offsetY, 
	//				pBmpInfo->bmiHeader.biWidth, pBmpInfo->bmiHeader.biHeight,
	//				//SurfaceSizeX, SurfaceSizeY,
	//				//end
	//				(void *)((unsigned char *)pBmpInfo + (sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256)), 
	//				pBmpInfo, 
	//				
	//				//DIB_PAL_COLORS,	
	//				DIB_RGB_COLORS, 
	//				//DIB_PAL_INDICES,
	//				
	//				SRCCOPY );
	//				//NOTSRCCOPY );
	//				//DSTINVERT );
	//				//BLACKNESS );
	//lpSurface->ReleaseDC( hDcDest );

	return;
}

// ??????????��??? ? memcpy ?��? ?****************************/
//Jerry Draw Bitmap to surface?
#ifdef _READ16BITBMP
void DrawBitmapToSurface2(SDL_Surface* lpSurface,SDL_Surface* lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,void* pBmpInfo)
{
//	//DDSURFACEDESC ddsd;	// ??????�B�l
//	char *pDest;			//Ŀ�ĵ�ָ��// ��?�I?????
//	char *pSource; 		//��Դָ��// ��???????
//	short *pDest2;		//Ŀ�ĵ�ָ��(unsigned short type)// ��?�I???????????
//#ifdef _HI_COLOR_32
//	int *pDest3;
//	int *pDestSys3;
//#endif
//	int surfacePitch;	//source face ���// ??????????
//	int bmpWidth;			//bmpͼ�Ŀ��// ????????
//	int i;
//#ifdef _READ16BITBMP
//	//DDSURFACEDESC ddsdsys;
//	short *pDestSys;			//Ŀ�ĵ�ָ�� systemmemory
//	int surfacePitchSys;	//source face
//#endif
//	
//	// ??????????NULL????
//	if( lpSurface == NULL ) return;
//#ifdef _READ16BITBMP
//	if(lpSurfaceSys == NULL ) return;
//#endif
//	// ?�B�l????
//	//ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
//	//ddsd.dwSize = sizeof( DDSURFACEDESC );
//#ifdef _READ16BITBMP
//	//ZeroMemory(&ddsdsys,sizeof(DDSURFACEDESC));
//	//ddsdsys.dwSize = sizeof(DDSURFACEDESC);
//#endif
//	
//	// ?????????????????( �i?? ddsd ?�ѩ�?�V????? )
//	if( lpSurface->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK ){
//		////MessageBox( hWnd, "Surface��lockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	
//#ifdef _READ16BITBMP
//	/*if(lpSurfaceSys->Lock(NULL,&ddsdsys,DDLOCK_WAIT,NULL) != DD_OK){
//		lpSurface->Unlock(NULL);
//		return;
//	}*/
//#endif
//	
//	// ???????��??????
//	pDest = ( char *)( ddsd.lpSurface );
//#ifdef _READ16BITBMP
//	pDestSys = (short*)(ddsdsys.lpSurface);
//#endif
//	
//#if 0	// ?????????��?????
//	
//	// ��???????
//	pSource = ( char *)pBmpInfo + BmpOffBits
//				+ offsetY * pBmpInfo->bmiHeader.biWidth + offsetX;
//	
//#else	// Realbin ??��?????
//	
//	// ��???????
//	pSource = pRealBinBits
//				+ offsetY * RealBinWidth + offsetX;
//	
//#endif
//	
//	// ??????????
//	surfacePitch = ddsd.lPitch;
//#ifdef _READ16BITBMP
//	surfacePitchSys = ddsdsys.lPitch >> 1;
//#endif
//#if 0
//	// ????????
//	bmpWidth = pBmpInfo->bmiHeader.biWidth;
//	
//#else // Realbin ��?????
//	// ????????
//	bmpWidth = RealBinWidth;
//#endif
//	
//	//???????????????
//	if(ResoMode == 1){
//		//��???????????��???
//		pSource -= bmpWidth;
//		//????????
//		sizeY >>= 1;
//	}
//	// ��????��?????
//	for( i = 0 ; i < sizeY ; i++ ){
//		// ?????????????
//		
//		//???????????????
//		if(ResoMode == 1){
//			_asm{
//				mov		edi,[pDest]		//��?�I????
//					mov		esi,[pSource]	//��??????
//					mov		eax,[sizeX]		//????????
//					//			mov		ah,al			//
//					shr		ax,1
//					mov		cx,ax
//					inc		esi				//��??????????????
//					
//loop_100:
//				mov		al,[esi]		//????��???
//					//			cmp		al,240			//????�a��?
//					//			jne		loop_200		//?????????
//					
//					//			xor		al,al			//??�a��?��?
//					
//					//loop_200:
//					mov		[edi],al		//????????
//					inc		esi				//��???????��
//					inc		esi				//��???????��
//					inc		edi				//��?�I?????��
//					//			dec		ah				//��??
//					dec		cx				//��??
//					jne		loop_100		//?????????��?
//			}
//			// ????????��???????? ???��??????? ?
//			pSource -= bmpWidth*2;
//		} else {
//			//?????????????????
//			
//			//HiO ????????
//#ifdef _HI_COLOR_32
//			if( displayBpp == 32 )
//			{
//				int j;
//				int pixel;
//				pDest3 = (int *)pDest;
//				
//				for( j = 0; j < sizeX; j++ )
//				{
//					// �a?��????�V??
//					if( pSource[j] == DEF_COLORKEY )
//					{
//						pDest3[j] = 0;
//#ifdef _READ16BITBMP
//						pDestSys3[j] = 0;
//#endif
//					}
//					else
//					{
//						pixel = highColor32Palette[(unsigned char)pSource[j]];
//						if( pixel == 0 )
//						{
//							pDest3[j] = 1;
//#ifdef _READ16BITBMP
//							pDestSys3[j] = 1;
//#endif
//						}
//						else
//						{
//							pDest3[j] = pixel;
//#ifdef _READ16BITBMP
//							pDestSys3[j] = pixel;
//#endif
//						}
//					}
//				}
//			}else
//#endif
//			if( displayBpp == 16 )
//			{
//				int j;
//				short pixel;
//				
//				pDest2 = (short *)pDest;
//				
//				for( j = 0; j < sizeX; j++ )
//				{
//					// �a?��????�V??
//					if( pSource[j] == DEF_COLORKEY )
//					{
//						pDest2[j] = 0;
//#ifdef _READ16BITBMP
//						pDestSys[j] = 0;
//#endif
//					}
//					else
//					{
//						pixel = highColorPalette[(unsigned char)pSource[j]];
//						if( pixel == 0 )
//						{
//							pDest2[j] = 1;
//#ifdef _READ16BITBMP
//							pDestSys[j] = 1;
//#endif
//						}
//						else
//						{
//							pDest2[j] = pixel;
//#ifdef _READ16BITBMP
//							pDestSys[j] = pixel;
//#endif
//						}
//					}
//				}
//			}else{
//				memcpy( pDest, pSource, sizeX );
//			}
//			//HiO????????
//			
//			// ????????��???????? ???��??????? ?
//			pSource -= bmpWidth;
//		}
//		// ????????��?????��??
//		pDest += surfacePitch;
//#ifdef _READ16BITBMP
//		pDestSys += surfacePitchSys;
//#endif
//	}
//	
//	// ???????????????????
//	if( lpSurface->Unlock( NULL ) != DD_OK ){
//		////MessageBox( hWnd, "Surface��Unlockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	
//#ifdef _READ16BITBMP
//	lpSurfaceSys->Unlock(NULL);
//#endif
//	
	return;
}
#endif

SDL_Color getColorFromPalette(unsigned char index)
{
    return highColor32Palette[index];
}

void DrawBitmapToSurface2( SDL_Surface*& lpSurface, int offsetX, int offsetY, int sizeX, int sizeY, void* pBmpInfo )
{
    char *pSource;
    pSource = pRealBinBits + offsetY * RealBinWidth + offsetX;
    

    if (lpSurface == 0) {
        SDL_Color* pixelDataSurface = (SDL_Color*)malloc(sizeX* sizeY* sizeof(SDL_Color));
        int currentPixelPos = 0;
        for(int i = 0; i < sizeY; i++)
        {
            for(int j = 0; j < sizeX ; j++)
            {
                pixelDataSurface[currentPixelPos + j ]  = getColorFromPalette(pSource[j]);
            }
            pSource -= RealBinWidth;
            currentPixelPos += sizeX;
        }
        lpSurface = createSDLSurface(sizeX, sizeY, pixelDataSurface);
    }
    else
    {
        free(lpSurface->pixels);
        lpSurface->pixels = 0;
        SDL_Color* pixelDataSurface = (SDL_Color*)malloc(sizeX* sizeY* sizeof(SDL_Color));
        int currentPixelPos = 0;
        for(int i = 0; i < sizeY; i++)
        {
            for(int j = 0; j < sizeX ; j++)
            {
                pixelDataSurface[currentPixelPos + j ]  = getColorFromPalette(pSource[j]);
            }
            pSource -= RealBinWidth;
            currentPixelPos += sizeX;
        }
        //SDL_FillRect(lpSurface, NULL, SDL_MapRGB(lpSurface->format, 0,0,0));
        lpSurface = createSDLSurface(sizeX, sizeY, pixelDataSurface);
    }
    //free(pixelDataSurface);
    SDL_SetColorKey(lpSurface,SDL_TRUE, SDL_MapRGB(lpSurface->format, 0, 0, 0));

    //SDL_SaveBMP(lpSurface, buff);
//    if (pBmpInfo == 0) {
//        OutputDebugString("DrawBitmapToSurface2 pBmpInfo is NULL");
//    }
//	DDSURFACEDESC ddsd;	// ??????�B�l
//	char *pDest;			//Ŀ�ĵ�ָ��// ��?�I?????
//	char *pSource; 		//��Դָ��// ��???????
//	short *pDest2;		//Ŀ�ĵ�ָ��(unsigned short type)// ��?�I???????????
//	int surfacePitch;	//source face ���// ??????????
//	int bmpWidth;			//bmpͼ�Ŀ��// ????????
//	int i;
//#ifdef _HI_COLOR_32
//	int *pDest3;
//#endif
//	// ??????????NULL????
//	if( lpSurface == NULL ) return;
//	// ?�B�l????
//	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
//	ddsd.dwSize = sizeof( DDSURFACEDESC );
//	
//	// ?????????????????( �i?? ddsd ?�ѩ�?�V????? )
//	if( lpSurface->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK ){
//		////MessageBox( hWnd, "Surface��lockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	
//	
//	// ???????��??????
//	pDest = ( char *)( ddsd.lpSurface );
//	
//#if 0	// ?????????��?????
//	
//	// ��???????
//	pSource = ( char *)pBmpInfo + BmpOffBits
//				+ offsetY * pBmpInfo->bmiHeader.biWidth + offsetX;
//	
//#else	// Realbin ??��?????
//	
//	// ��???????
//	pSource = pRealBinBits
//				+ offsetY * RealBinWidth + offsetX;
//	
//#endif
//	
//	// ??????????
//	surfacePitch = ddsd.lPitch;
//#if 0
//	// ????????
//	bmpWidth = pBmpInfo->bmiHeader.biWidth;
//	
//#else // Realbin ��?????
//	// ????????
//	bmpWidth = RealBinWidth;
//#endif
//	
//	//???????????????
//	if(ResoMode == 1){
//		//��???????????��???
//		pSource -= bmpWidth;
//		//????????
//		sizeY >>= 1;
//	}
//	// ��????��?????
//	for( i = 0 ; i < sizeY ; i++ ){
//		// ?????????????
//		
//		//???????????????
//		if(ResoMode == 1){
//			_asm{
//				mov		edi,[pDest]		//��?�I????
//					mov		esi,[pSource]	//��??????
//					mov		eax,[sizeX]		//????????
//					//			mov		ah,al			//
//					shr		ax,1
//					mov		cx,ax
//					inc		esi				//��??????????????
//					
//loop_100:
//				mov		al,[esi]		//????��???
//					//			cmp		al,240			//????�a��?
//					//			jne		loop_200		//?????????
//					
//					//			xor		al,al			//??�a��?��?
//					
//					//loop_200:
//					mov		[edi],al		//????????
//					inc		esi				//��???????��
//					inc		esi				//��???????��
//					inc		edi				//��?�I?????��
//					//			dec		ah				//��??
//					dec		cx				//��??
//					jne		loop_100		//?????????��?
//			}
//			// ????????��???????? ???��??????? ?
//			pSource -= bmpWidth*2;
//		} else {
//			//?????????????????
//			
//			//HiO ????????
//#ifdef _HI_COLOR_32
//			if( displayBpp == 32 )
//			{
//				int j;
//				int pixel;
//				pDest3 = (int *)pDest;
//				
//				for( j = 0; j < sizeX; j++ )
//				{
//					// �a?��????�V??
//					if( pSource[j] == DEF_COLORKEY )
//					{
//						pDest3[j] = 0;
//					}
//					else
//					{
//						pixel = highColor32Palette[(unsigned char)pSource[j]];
//						if( pixel == 0 )
//						{
//							pDest3[j] = 1;
//						}
//						else
//						{
//							pDest3[j] = pixel;
//						}
//					}
//				}
//			}else
//#endif
//
//			if( displayBpp == 16 )
//			{
//				int j;
//				short pixel;
//				
//				pDest2 = (short *)pDest;
//				
//				for( j = 0; j < sizeX; j++ )
//				{
//					// �a?��????�V??
//					if( pSource[j] == DEF_COLORKEY )
//					{
//						pDest2[j] = 0;
//					}
//					else
//					{
//						pixel = highColorPalette[(unsigned char)pSource[j]];
//						if( pixel == 0 )
//						{
//							pDest2[j] = 1;
//						}
//						else
//						{
//							pDest2[j] = pixel;
//						}
//					}
//				}
//			}else{
//				memcpy( pDest, pSource, sizeX );
//			}
//			//HiO????????
//			
//			// ????????��???????? ???��??????? ?
//			pSource -= bmpWidth;
//		}
//		// ????????��?????��??
//		pDest += surfacePitch;
//	}
//	
//	// ???????????????????
//	if( lpSurface->Unlock( NULL ) != DD_OK ){
//		////MessageBox( hWnd, "Surface��Unlockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	

	return;
}

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    if ( x < 0 || x > surface->w || y < 0 || y > surface->h)
    {
        return;
    }
      Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
      *(Uint32 *)target_pixel = pixel;
}

// ?????��? **************************************************************/
void DrawBox( SDL_Rect *rect, unsigned char color, bool fill )
{
    SDL_Rect destRect =  {rect->x, rect->y, rect->w, rect->h};
    SDL_Color pixel = highColor32Palette[color];
    Uint32 pixelColor = SDL_MapRGB(screenSurface->format, pixel.r, pixel.g, pixel.b);
    SDL_LockSurface(screenSurface);
    int w = destRect.w;
    int h = destRect.h;
    if (!fill)
    {
        for(int i = 0 ;i < w - 2; i++)
        {
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + 1, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + destRect.h - 1, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + destRect.h, pixelColor);
        }

        for(int j = 0; j < h - 2; j++)
        {
            set_pixel(screenSurface, destRect.x, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + 1, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + destRect.w - 1, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + destRect.w, destRect.y + 1 + j, pixelColor);
        }
    }
    else
    {
        for(int i = 0 ;i < w - 2; i++)
        {
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + 1, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + destRect.h - 1, pixelColor);
            set_pixel(screenSurface, destRect.x + 1 + i, destRect.y + destRect.h, pixelColor);
        }

        for(int j = 0; j < h - 2; j++)
        {
            set_pixel(screenSurface, destRect.x, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + 1, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + destRect.w - 1, destRect.y + 1 + j, pixelColor);
            set_pixel(screenSurface, destRect.x + destRect.w, destRect.y + 1 + j, pixelColor);
        }
        destRect.x = rect->x + 1;
        destRect.y = rect->y + 1;
        destRect.w = w - 2;
        destRect.h = h - 2;
        SDL_FillRect(screenSurface, &destRect, pixelColor);
    }
    SDL_UnlockSurface(screenSurface);
    return;
    //SDL_Color col = highColor32Palette[color];
    //SDL_SetRenderDrawColor(mainRenderer, col.r, col.g, col.b, col.a);
    //SDL_RenderDrawRect(mainRenderer, &destRect);
    
	//DDSURFACEDESC ddsd;	// ??????�B�l

}


//---------------------------------------------------------------------------//
// ?????????��?                                                    //
//---------------------------------------------------------------------------//
// ??��	int x, y;						?�Ƥ���?��
//			unsigned char *autoMap;			???����?���ѩ�
//			int w, h;						autoMap ????
void DrawAutoMapping( int x, int y, unsigned char *autoMap, int w, int h )
{
//	DDSURFACEDESC ddsd;	// ??????�B�l
	Uint8 *ptDest;		// ��?�I?????
	Uint8 *tmpPtDest;	// ???
	Uint8 *tmpPtDest2;	// ????
	int surfacePitch;	// ??????????
	int i, j;			// ???????
	int color, pc_color;
	static short pcFlush = 0;
	static unsigned int pcFlushTime = 0;
	int xx, yy;
	int ww, hh;
	int mul;

	xx = 18;
	yy = 118;
	ww = w;
	hh = h;
	mul = 1;

	//???????????????
	if( ResoMode == 1 )
	{
		x  = x/2;
		y  = y/2;
		xx = xx/2+1;
		yy = yy/2-1;
		ww = ww/2;
		hh = hh/2;
		mul = 2;
	}

	if( pcFlushTime+1000 <= TimeGetTime() )
	{
		pcFlushTime = TimeGetTime();
		pcFlush++;
		pcFlush &= 1;
	}
	if( pcFlush ){
		pc_color = 255;
	}else{
		pc_color = 0;
	}
    surfacePitch = screenSurface->pitch;
    ptDest = (Uint8 *)(screenSurface->pixels) + (y+yy) * surfacePitch;
    SDL_LockSurface(screenSurface);

    ptDest += ((x+xx)<<2);
	tmpPtDest  = ptDest;
	tmpPtDest2 = ptDest;
	for( i = 0; i < hh; i++ ){
		ptDest = tmpPtDest;
		for( j = 0; j < ww; j++ ){
            color = SDL_MapRGB(screenSurface->format, highColor32Palette[autoMap[(i*mul)*w+(j*mul)]].r, highColor32Palette[autoMap[(i*mul)*w+(j*mul)]].g, highColor32Palette[autoMap[(i*mul)*w+(j*mul)]].b);
			*(int *)(ptDest-4)				= color;
			*(int *)(ptDest)				= color;
			*(int *)(ptDest+4)				= color;
			*(int *)(ptDest-surfacePitch)	= color;
			ptDest -= (surfacePitch-8);
		}
		tmpPtDest += (surfacePitch+8);
	}
	ptDest = tmpPtDest2 + (surfacePitch+4)*hh - (surfacePitch-4)*ww;
	color = color = SDL_MapRGB(screenSurface->format, highColor32Palette[pc_color].r, highColor32Palette[pc_color].g, highColor32Palette[pc_color].b);
	// ??????����?????????����?
	*(int *)(ptDest-4)				= color;
	*(int *)(ptDest)				= color;
	*(int *)(ptDest+4)				= color;
	*(int *)(ptDest-surfacePitch)	= color;

    SDL_UnlockSurface(screenSurface);
	return;
}

typedef unsigned long   COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))
// ???��??��?????????��???
int getAutoMapColor( unsigned int GraphicNo )
{
	int index = 0;
	static int width, height;
	static unsigned char *graBuf;
	unsigned int red = 0, green = 0, blue = 0;
	unsigned int cnt = 0;
	int color;

	int i, j;
	// real.bin?�G?�k???????????��???
	if(realGetImage( GraphicNo, ( unsigned char **)&graBuf, &width, &height ) == false ) return 0;
	// ??????�Χ�?��???
	for( i = 0; i < height; i++ ){
		for( j = 0; j < width; j++ ){
			index = graBuf[i*width+j];
			if( index != DEF_COLORKEY ){	// �a?��??????
				red   += Palette[index].peRed;
				green += Palette[index].peGreen;
				blue  += Palette[index].peBlue;
				cnt++;
			}
		}
	}

	if(cnt == 0) return 0;
	// ��?��??�k??��????????????�S?
	color = getNearestColorIndex( RGB( red/cnt, green/cnt, blue/cnt ), Palette, 256 );

	return color;
}

//---------------------------------------------------------------------------//
// ��?entry?????palette?????color??�k?????index?��??
//---------------------------------------------------------------------------//
//ref Ѱ��ɫ������ӽ�����ɫ



int getNearestColorIndex( unsigned long color, PALETTEENTRY *palette, int entry )
{
	double distance, mindist;
	int min_index;
	int i;
	mindist = (palette[0].peRed - GetRValue(color))*(palette[0].peRed - GetRValue(color))
		      + (palette[0].peGreen - GetGValue(color))*(palette[0].peGreen - GetGValue(color))
			  +(palette[0].peBlue - GetBValue(color))*(palette[0].peBlue - GetBValue(color));
	min_index = 0;
	for( i = 16; i < entry - 16; i++ ){
		distance = (palette[i].peRed - GetRValue(color))*(palette[i].peRed - GetRValue(color))
			+ (palette[i].peGreen - GetGValue(color))*(palette[i].peGreen - GetGValue(color))
			+(palette[i].peBlue - GetBValue(color))*(palette[i].peBlue - GetBValue(color));
		if( distance < mindist ){
			min_index = i;
			mindist = distance;
		}
	}
	return min_index;
}



//---------------------------------------------------------------------------//
// ?????????��?                                                    //
//---------------------------------------------------------------------------//
//ref ������ͼ����Ч 
void DrawMapEffect( void )
{
    return;
//	DDSURFACEDESC ddsd;
//	char *ptDest;
//	short *ptDest2;
//#ifdef _HI_COLOR_32
//	int *ptDest3;
//#endif
//	int surfacePitch;
//	int ww, hh;
//	MAP_EFFECT *buf;
//	char color;
//	int i, j;
//
//	ww = DEF_APPSIZEX;
//	hh = DEF_APPSIZEY;
//	//???????????????
//	if( ResoMode == 1 ){
//		ww >>= 1;
//		hh >>= 1;
//	}
//	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
//	ddsd.dwSize = sizeof( DDSURFACEDESC );
//	if( lpDraw->lpBACKBUFFER->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK )
//		return; 
//#ifdef _HI_COLOR_32
//	if( displayBpp == 32 )
//		surfacePitch = ddsd.lPitch >> 2;
//#endif
//	if( displayBpp == 16 )
//		surfacePitch = ddsd.lPitch >> 1;
//	else
//		surfacePitch = ddsd.lPitch;
//	buf = useBufMapEffect;
//	while( buf != (MAP_EFFECT *)NULL ){
//		if( 0 < buf->x && buf->x < ww-4 && 0 < buf->y && buf->y < hh-12 ){
//#ifdef _HI_COLOR_32
//			if( displayBpp == 32 ){
//				ptDest3 = (int *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
//			}else
//#endif
//			if( displayBpp == 16 )
//				ptDest2 = (short *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
//			else
//				ptDest = (char *)(ddsd.lpSurface) + buf->y * surfacePitch + buf->x;
//			if( buf->type == MAP_EFFECT_TYPE_RAIN ){
//#ifdef _HI_COLOR_32
//				if( displayBpp == 32 ){
//					*(ptDest3) = highColor32Palette[143];
//					*(ptDest3+surfacePitch) = highColor32Palette[143];
//					*(ptDest3+surfacePitch*2+1) = highColor32Palette[143];
//					*(ptDest3+surfacePitch*3+1) = highColor32Palette[143];
//				}else
//#endif
//				if( displayBpp == 16 ){
//					*(ptDest2) = highColorPalette[143];
//					*(ptDest2+surfacePitch) = highColorPalette[143];
//					*(ptDest2+surfacePitch*2+1) = highColorPalette[143];
//					*(ptDest2+surfacePitch*3+1) = highColorPalette[143];
//				}else{
//					*(ptDest) = (char)143;
//					*(ptDest+surfacePitch) = (char)143;
//					*(ptDest+surfacePitch*2+1) = (char)143;
//					*(ptDest+surfacePitch*3+1) = (char)143;
//				}
//			}else if( buf->type == MAP_EFFECT_TYPE_SNOW ){
//				if( buf->mode == 0 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3)   = highColor32Palette[161];
//						*(ptDest3+1) = highColor32Palette[159];
//						*(ptDest3+2) = highColor32Palette[161];
//						*(ptDest3+surfacePitch)   = highColor32Palette[159];
//						*(ptDest3+surfacePitch+1) = highColor32Palette[159];
//						*(ptDest3+surfacePitch+2) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2)   = highColor32Palette[161];
//						*(ptDest3+surfacePitch*2+1) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2+2) = highColor32Palette[161];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2)   = highColorPalette[161];
//						*(ptDest2+1) = highColorPalette[159];
//						*(ptDest2+2) = highColorPalette[161];
//						*(ptDest2+surfacePitch)   = highColorPalette[159];
//						*(ptDest2+surfacePitch+1) = highColorPalette[159];
//						*(ptDest2+surfacePitch+2) = highColorPalette[159];
//						*(ptDest2+surfacePitch*2)   = highColorPalette[161];
//						*(ptDest2+surfacePitch*2+1) = highColorPalette[159];
//						*(ptDest2+surfacePitch*2+2) = highColorPalette[161];
//					}else{
//						*(ptDest)   = (char)161;
//						*(ptDest+1) = (char)159;
//						*(ptDest+2) = (char)161;
//						*(ptDest+surfacePitch)   = (char)159;
//						*(ptDest+surfacePitch+1) = (char)159;
//						*(ptDest+surfacePitch+2) = (char)159;
//						*(ptDest+surfacePitch*2)   = (char)161;
//						*(ptDest+surfacePitch*2+1) = (char)159;
//						*(ptDest+surfacePitch*2+2) = (char)161;
//					}
//				}else if( buf->mode == 1 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3)   = highColor32Palette[162];
//						*(ptDest3+1) = highColor32Palette[159];
//						*(ptDest3+2) = highColor32Palette[159];
//						*(ptDest3+3) = highColor32Palette[162];
//						*(ptDest3+surfacePitch)   = highColor32Palette[159];
//						*(ptDest3+surfacePitch+1) = highColor32Palette[159];
//						*(ptDest3+surfacePitch+2) = highColor32Palette[159];
//						*(ptDest3+surfacePitch+3) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2)   = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2+1) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2+2) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*2+3) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*3)   = highColor32Palette[162];
//						*(ptDest3+surfacePitch*3+1) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*3+2) = highColor32Palette[159];
//						*(ptDest3+surfacePitch*3+3) = highColor32Palette[162];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2)   = highColorPalette[162];
//						*(ptDest2+1) = highColorPalette[159];
//						*(ptDest2+2) = highColorPalette[159];
//						*(ptDest2+3) = highColorPalette[162];
//						*(ptDest2+surfacePitch)   = highColorPalette[159];
//						*(ptDest2+surfacePitch+1) = highColorPalette[159];
//						*(ptDest2+surfacePitch+2) = highColorPalette[159];
//						*(ptDest2+surfacePitch+3) = highColorPalette[159];
//						*(ptDest2+surfacePitch*2)   = highColorPalette[159];
//						*(ptDest2+surfacePitch*2+1) = highColorPalette[159];
//						*(ptDest2+surfacePitch*2+2) = highColorPalette[159];
//						*(ptDest2+surfacePitch*2+3) = highColorPalette[159];
//						*(ptDest2+surfacePitch*3)   = highColorPalette[162];
//						*(ptDest2+surfacePitch*3+1) = highColorPalette[159];
//						*(ptDest2+surfacePitch*3+2) = highColorPalette[159];
//						*(ptDest2+surfacePitch*3+3) = highColorPalette[162];
//					}else{
//						*(ptDest)   = (char)162;
//						*(ptDest+1) = (char)159;
//						*(ptDest+2) = (char)159;
//						*(ptDest+3) = (char)162;
//						*(ptDest+surfacePitch)   = (char)159;
//						*(ptDest+surfacePitch+1) = (char)159;
//						*(ptDest+surfacePitch+2) = (char)159;
//						*(ptDest+surfacePitch+3) = (char)159;
//						*(ptDest+surfacePitch*2)   = (char)159;
//						*(ptDest+surfacePitch*2+1) = (char)159;
//						*(ptDest+surfacePitch*2+2) = (char)159;
//						*(ptDest+surfacePitch*2+3) = (char)159;
//						*(ptDest+surfacePitch*3)   = (char)162;
//						*(ptDest+surfacePitch*3+1) = (char)159;
//						*(ptDest+surfacePitch*3+2) = (char)159;
//						*(ptDest+surfacePitch*3+3) = (char)162;
//					}
//				}
//			}else if( buf->type == MAP_EFFECT_TYPE_STAR ){
//				if( buf->mode == 0 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3-surfacePitch-1) = highColor32Palette[8];
//						*(ptDest3-surfacePitch) = highColor32Palette[255];
//						*(ptDest2-surfacePitch+1) = highColor32Palette[8];
//						*(ptDest3-1) = highColor32Palette[255];
//						*(ptDest3) = highColor32Palette[255];
//						*(ptDest3+1) = highColorPalette[255];
//						*(ptDest3+surfacePitch-2) = highColor32Palette[8];
//						*(ptDest3+surfacePitch) = highColor32Palette[255];
//						*(ptDest3+surfacePitch+2) = highColor32Palette[8];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						// ��?????
//						*(ptDest2-surfacePitch-1) = highColorPalette[8];
//						*(ptDest2-surfacePitch) = highColorPalette[255];
//						*(ptDest2-surfacePitch+1) = highColorPalette[8];
//						*(ptDest2-1) = highColorPalette[255];
//						*(ptDest2) = highColorPalette[255];
//						*(ptDest2+1) = highColorPalette[255];
//						*(ptDest2+surfacePitch-1) = highColorPalette[8];
//						*(ptDest2+surfacePitch) = highColorPalette[255];
//						*(ptDest2+surfacePitch+1) = highColorPalette[8];
//					}else{
//						// ��?????
//						*(ptDest-surfacePitch-1) = (char)8;
//						*(ptDest-surfacePitch) = (char)255;
//						*(ptDest-surfacePitch+1) = (char)8;
//						*(ptDest-1) = (char)255;
//						*(ptDest) = (char)255;
//						*(ptDest+1) = (char)255;
//						*(ptDest+surfacePitch-1) = (char)8;
//						*(ptDest+surfacePitch) = (char)255;
//						*(ptDest+surfacePitch+1) = (char)8;
//					}
//				}else if( buf->mode == 1 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3-surfacePitch) = highColor32Palette[251];
//						*(ptDest3-1) = highColor32Palette[251];
//						*(ptDest3) = highColor32Palette[251];
//						*(ptDest3+1) = highColor32Palette[251];
//						*(ptDest3+surfacePitch) = highColor32Palette[251];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						// ��??��?��??
//						*(ptDest2-surfacePitch) = highColorPalette[251];
//						*(ptDest2-1) = highColorPalette[251];
//						*(ptDest2) = highColorPalette[251];
//						*(ptDest2+1) = highColorPalette[251];
//						*(ptDest2+surfacePitch) = highColorPalette[251];
//					}else{
//						// ��??��?��??
//						*(ptDest-surfacePitch) = (char)251;
//						*(ptDest-1) = (char)251;
//						*(ptDest) = (char)251;
//						*(ptDest+1) = (char)251;
//						*(ptDest+surfacePitch) = (char)251;
//					}
//				}else if( buf->mode == 2 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[251];
//						*(ptDest3+1) = highColor32Palette[255];
//						*(ptDest3+surfacePitch) = highColor32Palette[255];
//						*(ptDest3+surfacePitch+1) = highColor32Palette[251];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						// ��??��????��?
//						*(ptDest2) = highColorPalette[251];
//						*(ptDest2+1) = highColorPalette[255];
//						*(ptDest2+surfacePitch) = highColorPalette[255];
//						*(ptDest2+surfacePitch+1) = highColorPalette[251];
//					}else{
//						// ��??��????��?
//						*(ptDest) = (char)251;
//						*(ptDest+1) = (char)255;
//						*(ptDest+surfacePitch) = (char)255;
//						*(ptDest+surfacePitch+1) = (char)251;
//					}
//				}else if( buf->mode == 3 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[255];
//						*(ptDest3+1) = highColor32Palette[251];
//						*(ptDest3+surfacePitch) = highColor32Palette[251];
//						*(ptDest3+surfacePitch+1) = highColor32Palette[255];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						// ��????��??��?
//						*(ptDest2) = highColorPalette[255];
//						*(ptDest2+1) = highColorPalette[251];
//						*(ptDest2+surfacePitch) = highColorPalette[251];
//						*(ptDest2+surfacePitch+1) = highColorPalette[255];
//					}else{
//						// ��????��??��?
//						*(ptDest) = (char)255;
//						*(ptDest+1) = (char)251;
//						*(ptDest+surfacePitch) = (char)251;
//						*(ptDest+surfacePitch+1) = (char)255;
//					}
//				}else if( buf->mode == 4 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[251];
//						*(ptDest3+1) = highColor32Palette[193];
//						*(ptDest3+surfacePitch) = highColor32Palette[193];
//						*(ptDest3+surfacePitch+1) = highColor32Palette[193];
//					}else
//#endif	
//					if( displayBpp == 16 ){
//						// ��??��?��???��?
//						*(ptDest2) = highColorPalette[251];
//						*(ptDest2+1) = highColorPalette[193];
//						*(ptDest2+surfacePitch) = highColorPalette[193];
//						*(ptDest2+surfacePitch+1) = highColorPalette[193];
//					}else{
//						// ��??��?��???��?
//						*(ptDest) = (char)251;
//						*(ptDest+1) = (char)193;
//						*(ptDest+surfacePitch) = (char)193;
//						*(ptDest+surfacePitch+1) = (char)193;
//					}
//				}else if( buf->mode == 5 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[198];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2) = highColorPalette[198];
//					}else{
//						*(ptDest) = (char)198;
//					}
//				}else if( buf->mode == 6 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32){
//						*(ptDest3) = highColor32Palette[193];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2) = highColorPalette[193];
//					}else{
//						*(ptDest) = (char)193;
//					}
//				}else if( buf->mode == 7 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[208];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2) = highColorPalette[208];
//					}else{
//						// ��???��??
//						*(ptDest) = (char)208;
//					}
//				}else if( buf->mode == 8 ){
//#ifdef _HI_COLOR_32
//					if( displayBpp == 32 ){
//						*(ptDest3) = highColor32Palette[251];
//					}else
//#endif
//					if( displayBpp == 16 ){
//						*(ptDest2) = highColorPalette[251];
//					}else{
//						// ��??��?
//						*(ptDest) = (char)251;
//					}
//				}
//			}else if( buf->type == MAP_EFFECT_TYPE_KAMIFUBUKI ){
//				// ????
//				if( buf->mode == 0 ){
//					if( buf->type2 == 0 )
//						color = (char)223;
//					else if( buf->type2 == 1 )
//						color = (char)217;
//					else
//						color = (char)159;
//				}else if( buf->mode == 1 ){
//					if( buf->type2 == 0 )
//						color = (char)212;
//					else if( buf->type2 == 1 )
//						color = (char)208;
//					else
//						color = (char)159;
//				}else if( buf->mode == 2 ){
//					if( buf->type2 == 0 )
//						color = (char)130;
//					else if( buf->type2 == 1 )
//						color = (char)147;
//					else
//						color = (char)159;
//				}else{
//					if( buf->type2 == 0 )
//						color = (char)116;
//					else if( buf->type2 == 1 )
//						color = (char)99;
//					else
//						color = (char)159;
//				}
//				for( i = 0; i < buf->h; i++ ){
//					for( j = 0; j < buf->w; j++ ){
//						if( 0 <= (buf->ex+j) && (buf->ex+j) < ww && 0 <= (buf->ey+i) && (buf->ey+i) < hh ){
//#ifdef _HI_COLOR_32
//							if( displayBpp == 32 ){
//								*(ptDest3+surfacePitch*(i+buf->ey)+j+buf->ex) = highColor32Palette[(unsigned char)color];
//							}else
//#endif
//							if( displayBpp == 16 )
//								*(ptDest2+surfacePitch*(i+buf->ey)+j+buf->ex) = highColorPalette[(unsigned char)color];
//							else
//								*(ptDest+surfacePitch*(i+buf->ey)+j+buf->ex) = color;
//						}
//					}
//				}
//			}else if( buf->type == MAP_EFFECT_TYPE_HOTARU ){
//			}
//		}
//		buf = buf->next;
//	}
//	if( lpDraw->lpBACKBUFFER->Unlock( NULL ) != DD_OK )
//		return; 
	return;
}






// ??????????��?? ????? ?************************************/
void DrawDebugLine( unsigned char color )
{
//	DDSURFACEDESC ddsd;	// ??????�B�l
//    char *pDest;		// ��?�I?????
//    short *pDest2;		// ��?�I???????????
//	int surfacePitch;	// ??????????
//	int i, j, k, l, m;
//	short pixel;
//#ifdef _HI_COLOR_32
//	int *pDest3;
//#endif
//	// ????????????
//	if( WindowMode ) return;
//	
//	// ?�B�l????
//	ZeroMemory( &ddsd, sizeof( DDSURFACEDESC ) );
//	ddsd.dwSize = sizeof( DDSURFACEDESC );
//	
//	// ?????????????????( �i?? ddsd ?�ѩ�?�V????? )
//	if( lpDraw->lpFRONTBUFFER->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DD_OK ){
//		////MessageBox( hWnd, "Surface��lockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	
//	//??��
//	j = 630;
//	k = 480;
//	l = 8;
//	//???????????????
//	if(ResoMode == 1){
//		j >>= 1;
//		k >>= 1;
//		l >>= 1;
//	}
//#ifdef _HI_COLOR_32
//	if( displayBpp == 32 )
//	{
//		int pixel;
//		pixel = highColor32Palette[color];
//		// �D?��????????????
//		pDest3 = (int *)ddsd.lpSurface + j;
//		// ??????????
//		surfacePitch = ddsd.lPitch >> 1;
//		// ��???
//		for( i = 0 ; i < k ; i++ )
//		{
//			// ?????��?
//			for( m = 0; m < l; m++ )
//			{
//				*(pDest3+m) = pixel;
//			}
//			// ????????��?????��??
//			pDest3 += surfacePitch;
//    	}
//	}
//	else
//#endif
//	if( displayBpp == 16 )
//	{
//		pixel = highColorPalette[color];
//		// �D?��????????????
//		pDest2 = (short *)ddsd.lpSurface + j;
//		// ??????????
//		surfacePitch = ddsd.lPitch >> 1;
//		// ��???
//		for( i = 0 ; i < k ; i++ )
//		{
//			// ?????��?
//			for( m = 0; m < l; m++ )
//			{
//				*(pDest2+m) = pixel;
//			}
//			// ????????��?????��??
//			pDest2 += surfacePitch;
//    	}
//	}
//	else
//	{
//		// �D?��????????????
//		pDest = ( char *)( ddsd.lpSurface ) + j;
//
//		// ??????????
//		surfacePitch = ddsd.lPitch;
//
//		// ��???
//		for( i = 0 ; i < k ; i++ )
//		{
//			// ?????��?
//			memset( pDest, color, l );
//			// ????????��?????��??
//			pDest += surfacePitch;
//    	}
//    }
//
//	// ???????????????????
//	if( lpDraw->lpFRONTBUFFER->Unlock( NULL ) != DD_OK ){
//		//MessageBox( hWnd, "Surface��Unlockʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//		return; 
//	}	
//	
	return;
}
//---------------------------------------------------------------------------//
// ?? ��RECT?��?��?���q��ֱ??��վ�ϐD????]?                    //
// ?? ��DIRECT_DRAW *lpDraw : DirectDraw???�B�l                         //
//        short  bx           : ��?�I??�t                                 //
//        short  by           : ��?�I��?�t                                 //
//        SDL_Surface* lpSurface : ��???????                   //
// ?�k ��DD_OK:������?                                                     //
//---------------------------------------------------------------------------//
#ifdef _READ16BITBMP
int DrawSurfaceFast( short bx, short by, SDL_Surface* lpSurface,SDL_Surface* lpSurfaceSys)
{
	short x0, y0;
	long w, h;
	
	SDL_Rect rect = { 0, 0, SurfaceSizeX, SurfaceSizeY };
	x0 = bx;
	y0 = by;
	w = rect.w;
	h = rect.h;

	/*if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
		return DD_OK;
	}
	
	// ?�Z?????
	if( bx < 0 ){
		rect.left -= bx;
		x0 = 0;
	}
	// ?�Z?????
	if( bx + w > lpDraw->xSize ){
		rect.right -= bx + w - lpDraw->xSize;
	}
	// �ƣZ?????
	if( by < 0 ){
		rect.top -= by;
		y0 = 0;
	}
	// ?�Z?????
	if( by + h > lpDraw->ySize ){
		rect.bottom -= by + h - lpDraw->ySize;
	}
	
#ifdef _DEBUG		
	// ??����???????????????
	SurfaceDispCnt++;
#endif
	RECT rectD;

	rectD.left = x0;
	rectD.top = y0;
	rectD.right = x0 + (rect.right - rect.left);
	rectD.bottom = y0 + (rect.bottom - rect.top);
	
	lpDraw->lpBACKBUFFERSYS->Blt(&rectD,lpSurfaceSys,&rect,DDBLT_KEYSRC | DDBLT_WAIT,NULL);
	return lpDraw->lpBACKBUFFER->Blt(&rectD,lpSurface,&rect,DDBLT_KEYSRC | DDBLT_WAIT,NULL);*/
    return 0;
}
#endif


extern int blitFlag;
int DrawSurfaceFast( short bx, short by, SDL_Surface* lpSurface)
{
	short x0, y0;
	long w, h;
	
	// ��???????
    SDL_Rect rect = { bx, by, lpSurface->w, lpSurface->h }; // ?????��?
	
	x0 = bx;
	y0 = by;
	w = rect.w;
	h = rect.h;

	// ????????
	//   ????? RECT ????��?????����?????

	// �u?����???��?��?????
	//if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
	//	return 0;
	//}
	
	// ?�Z?????
	//if( bx < 0 ){
	//	rect.x -= bx;
	//	x0 = 0;
	//}
	//// ?�Z?????
	//if( bx + w > lpDraw->xSize ){
	//	rect.w -= bx + w - lpDraw->xSize;
	//}
	//// �ƣZ?????
	//if( by < 0 ){
	//	rect.top -= by;
	//	y0 = 0;
	//}
	//// ?�Z?????
	//if( by + h > lpDraw->ySize ){
	//	rect.bottom -= by + h - lpDraw->ySize;
	//}
	
#ifdef _DEBUG		
	// ??����???????????????
	SurfaceDispCnt++;
#endif
	//RECT rectD;

	//rectD.left = x0;
	//rectD.top = y0;
	//rectD.right = x0 + (rect.right - rect.left);
	//rectD.bottom = y0 + (rect.bottom - rect.top);
	
	// ??????????�L��?
	//return //lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpSurface, &rect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
	//return lpDraw->lpBACKBUFFER->Blt(&rectD,lpSurface,&rect,DDBLT_KEYSRC | DDBLT_WAIT,NULL);*/
	//SDL_Log("Jerry BlitFlag is %d", blitFlag);
	SDL_BlitSurface(lpSurface, NULL, screenSurface, &rect);
    //SDL_UpdateTexture(screenTexture, NULL, screenSurface->pixels, screenSurface->pitch);


    return 0;
}

//---------------------------------------------------------------------------//
// ?? ��RECT?��?��?���q��ֱ??��վ�ϐD????]?                    //
// ?? ��DIRECT_DRAW *lpDraw : DirectDraw???�B�l                         //
//        short  bx           : ��?�I??�t                                 //
//        short  by           : ��?�I��?�t                                 //
//        SDL_Rect * rect         : ��?????�B�l                             //
//        SDL_Surface* lpSurface : ��???????                   //
// ?�k ��DD_OK:������?                                                     //
//---------------------------------------------------------------------------//
int DrawSurfaceFast2( short bx, short by, SDL_Rect *rect, SDL_Surface* lpSurface )
{
	short x0, y0;
	long w, h;
	
	x0 = bx;
	y0 = by;
	w = rect->w;
	h = rect->h;

	// ????????
	//   ????? RECT ????��?????����?????

	// �u?����???��?��?????
	//if( bx >= lpDraw->xSize || bx + w <= 0 || by >= lpDraw->ySize || by + h <= 0 ){
	//	return 0;
	//}
	//
	//// ?�Z?????
	//if( bx < 0 ){
	//	rect->x -= bx;
	//	x0 = 0;
	//}
	//// ?�Z?????
	//if( bx + w > lpDraw->xSize ){
	//	rect->w -= w - lpDraw->xSize;
	//}
	//// �ƣZ?????
	//if( by < 0 ){
	//	rect->h -= by;
	//	y0 = 0;
	//}
	//// ?�Z?????
	//if( by + h > lpDraw->ySize ){
	//	rect->h -= h - lpDraw->ySize;
	//}
	
#ifdef _DEBUG		
	// ??����???????????????
	SurfaceDispCnt++;
#endif
	
	// ??????????�L��?
	//return //lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpSurface, rect, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT );
    SDL_Rect destRect = {x0, y0, 0, 0};
	SDL_BlitSurface(lpSurface, rect, screenSurface, &destRect);
    
	return 0; //lpDraw->lpBACKBUFFER->BltFast( x0, y0, lpSurface, rect, DDBLTFAST_WAIT );
}


/* ????????��?? ****************************************************/
void ReleaseDirectDraw( void )
{
//	int i;
//	// �u????????????????��
//	for( i = 0 ; i < SurfaceCnt ; i++ ){
//		// ??????????
//		if( SurfaceInfo[ i ].lpSurface != NULL ){
//			// ????
//            SDL_FreeSurface(SurfaceInfo[ i ].lpSurface);
//			SurfaceInfo[ i ].lpSurface = NULL;
//#ifdef _READ16BITBMP
//			if(g_bUseAlpha){
//				SDL_FreeSurface(SurfaceInfo[ i ].lpSurface);
//				SurfaceInfoSys[i].lpSurface = NULL;
//			}
//#endif
//		}
//#ifdef _READ16BITBMP
//		if(g_bUseAlpha){
//			if(SurfaceInfo[i].lpAlphaData != NULL){
//				FREE(SurfaceInfo[i].lpAlphaData);
//#ifdef _DEBUG
//				g_iMallocCount--;
//#endif
//				SurfaceInfo[i].lpAlphaData = NULL;
//			}
//		}
//#endif
//	}
//#ifdef __SKYISLAND
//	extern void SkyIslandRelease();
//	SkyIslandRelease();
//#endif
//	// ??????????��
//	if( lpBattleSurface != NULL ){
//		// ????
//		lpBattleSurface->Release();
//		lpBattleSurface = NULL;
//	}
//	if( lpDraw){
//		// ?????��
//		if( lpDraw->lpPALETTE != NULL ){
//			lpDraw->lpPALETTE->Release();
//			lpDraw->lpPALETTE = NULL;
//		}
//		// ??????��
//		if( lpDraw->lpCLIPPER != NULL ){
//			lpDraw->lpCLIPPER->Release();
//			lpDraw->lpCLIPPER = NULL;
//		}
//		// ?????????��
//		if( lpDraw->lpBACKBUFFER != NULL ){
//			lpDraw->lpBACKBUFFER->Release();
//			lpDraw->lpBACKBUFFER = NULL;
//		}
//#ifdef _READ16BITBMP
//		if(g_bUseAlpha){
//			if(lpDraw->lpBACKBUFFERSYS != NULL){
//				lpDraw->lpBACKBUFFERSYS->Release();
//				lpDraw->lpBACKBUFFERSYS = NULL;
//			}
//			if(lpBattleSurfaceSys != NULL){
//				lpBattleSurfaceSys->Release();
//				lpBattleSurfaceSys = NULL;
//			}
//		}
//#endif
//		// ???????????��
//		if( lpDraw->lpFRONTBUFFER != NULL ){
//			lpDraw->lpFRONTBUFFER->Release();
//			lpDraw->lpFRONTBUFFER = NULL;
//		}
//		// DirectDraw??��
//		if( lpDraw->lpDD2 != NULL){ 
//			lpDraw->lpDD2->Release();
//			lpDraw->lpDD2 = NULL;
//		}
//		//if( lpDraw->lpDD != NULL){ 
//		//	lpDraw->lpDD->Release();
//		//	lpDraw->lpDD = NULL;
//		//}
//		// DIRECT_DRAW ?�B�l?��
//		HeapFree( GetProcessHeap(), NULL, lpDraw);
//		lpDraw = NULL;
//	}	
//	// DirectDraw ??????? false ???
//	DDinitFlag = false;	
}

// ??????????????? ////////////////////////////////////////////
bool CheckSurfaceLost( void )
{
//	bool SurfaceLostFlag = false;
//	int i;
//	// �u?????????????????
//	for( i = 0 ; i < SurfaceCnt ; i++ ){
//		// ??????????
//		if( SurfaceInfo[ i ].lpSurface != NULL ){
//			// ????????????????
//#ifdef _READ16BITBMP
//			//if(g_bUseAlpha) 
//				//if(SurfaceInfoSys[i].lpSurface->IsLost()) SurfaceLostFlag = true;
//#endif
//			//if( SurfaceInfo[ i ].lpSurface->IsLost() ) SurfaceLostFlag = true;
//		}
//	}
//	// ???????????
//	if( lpBattleSurface != NULL ){
//		// ????
//		if( lpBattleSurface->IsLost() ){
//			SurfaceLostFlag = true;
//		}
//	}
//#ifdef __SKYISLAND
//#ifndef __CARYTEST
//	extern bool SkyIslandCheckSurface();
//	if( SkyIslandCheckSurface())
//		SurfaceLostFlag = true;
//#endif
//#endif
//	// ??????????
//	if( lpDraw->lpBACKBUFFER != NULL ){
//		// ????????????????
//		if( lpDraw->lpBACKBUFFER->IsLost() ){
//			SurfaceLostFlag = true;
//		}
//	}
//	// ????????????
//	if( lpDraw->lpFRONTBUFFER != NULL ){
//		// ????????????????
//		if( lpDraw->lpFRONTBUFFER->IsLost() ){
//			SurfaceLostFlag = true;
//		}
//	}
//	
	return false;
}	

// ????????????�� *************************************************/
void InitFont( int fontNo )
{
    TTF_Init();
	// ?????���q
	//char *fontName[] = { "����","����_GB2312" };
	//
	//// ?????????
	//if( hFont != NULL ){
	//	// ????????????��
	//	DeleteObject( hFont );
	//}

	////???????????????
	//if(ResoMode == 1){
	//	// MS??????���e????????????????��
	//	hFont = CreateFont( 
	//						FONT_SIZE/2, 	/* ????????	*/ 
	//						0, 		/* ��????��??	*/ 
	//						0, 		/* ???????�T	*/ 
	//						0,		/* ??? ????x��??�T	*/ 
	//						/* ?????�^?	*/ 
	//						FW_NORMAL,		// 0 
	//						/* ?????�l�N��????	*/ 
	//						false, 					
	//						/* ?�_��?�N��????	*/ 
	//						false,					
	//						/* �e?��?�_��?�N��????	*/ 
	//						false,					
	//						/* ��????������	*/ 
	//						GB2312_CHARSET/*CHINESEBIG5_CHARSET*/, 	// ?�G?��?
	//						/* ��?�ڥT	*/ 
	//						OUT_DEFAULT_PRECIS, 
	//						/* ??????�ڥT	*/ 
	//						CLIP_DEFAULT_PRECIS,
	//						/* ��?����	*/ 
	//						DEFAULT_QUALITY,	// ????????��????????
	//						/* ??? */ 
	//						FIXED_PITCH |
	//						/* ????	*/ 
	//						FF_ROMAN,			// ??��?????????????��????????MS(R) Serif????????
	//						fontName[ fontNo ] ); 
	//} else {
	//	// MS??????���e????????????????��
	//	hFont = CreateFont( 
	//						FONT_SIZE, 	/* ????????	*/ 
	//						0, 		/* ��????��??	*/ 
	//						0, 		/* ???????�T	*/ 
	//						0,		/* ??? ????x��??�T	*/ 

	//						/* ?????�^?	*/ 
	//						//FW_DONTCARE,		// 0 
	//						//FW_THIN,			// 100 
	//						//FW_EXTRALIGHT,	// 200 
	//						//FW_LIGHT,			// 300 
	//						FW_NORMAL,			// 400 
	//						//FW_MEDIUM,		// 500 
	//						//FW_SEMIBOLD,		// 600 
	//						//FW_BOLD,			// 700 
	//						//FW_EXTRABOLD,		// 800 
	//						//FW_HEAVY,			// 900
	//					
	//						/* ?????�l�N��????	*/ 
	//						false, 					
	//						//true,
	//						
	//						/* ?�_��?�N��????	*/ 
	//						false,					
	//						//true,
	//						
	//						/* �e?��?�_��?�N��????	*/ 
	//						false,					
	//						//true,
	//					
	//						/* ��????������	*/ 
	//						//ANSI_CHARSET,		// ��?��?
	//						//UNICODE_CHARSET,	//�e???????
	//						//SYMBOL_CHARSET,	// ��?��?
	//						//SHIFTJIS_CHARSET,	// ?�G?��?
	//						//HANGEUL_CHARSET,	// ?�G?��?
	//						GB2312_CHARSET/*CHINESEBIG5_CHARSET*/,// ?�G?��?
	//						//OEM_CHARSET,		// ��?��?
	//						//DEFAULT_CHARSET, 	// ?�G?��?
	//						
	//						/* ��?�ڥT	*/ 
	//						OUT_DEFAULT_PRECIS, 
	//						//OUT_STRING_PRECIS,
	//						//OUT_CHARACTER_PRECIS, 
	//						//OUT_STROKE_PRECIS,
	//						//OUT_TT_PRECIS,
	//						//OUT_DEVICE_PRECIS,
	//						//OUT_RASTER_PRECIS,
	//						//OUT_TT_ONLY_PRECIS,
	//						//OUT_OUTLINE_PRECIS,
	//
	//						/* ??????�ڥT	*/ 
	//						CLIP_DEFAULT_PRECIS,
	//						//CLIP_CHARACTER_PRECIS,
	//						//CLIP_STROKE_PRECIS,
	//						//CLIP_MASK,
	//						//CLIP_LH_ANGLES,
	//						//CLIP_TT_ALWAYS,
	//						//CLIP_EMBEDDED,
	//
	//						/* ��?����	*/ 
	//						DEFAULT_QUALITY,	// ????????��????????
	//						//DRAFT_QUALITY,		// ????????��???????
	//						//PROOF_QUALITY,		// ??????????��???????
	//						
	//						
	//						/* ??? */ 
	//						FIXED_PITCH |
	//						
	//						//DEFAULT_PITCH, 
	//						//VARIABLE_PITCH, 
	//						
	//						//| FF_ROMAN
	//						
	//						/* ????	*/ 
	//						//FF_DECORATIVE,	// ??�ߦ�????????Old English????????
	//						//FF_DONTCARE,		// ??????�{?���e????????????????����????????
	//						//FF_MODERN,		// ??�e?????????????��?????????????????Pica?Elite?Courier New(R) ????????
	//						FF_ROMAN,			// ??��?????????????��????????MS(R) Serif????????
	//						//FF_SCRIPT,		// ?��????����??????????Script?Cursive????????
	//						//FF_SWISS,			// ??��???????????????
	//										
	//						/* ???????����???????	*/ 				
	//						//"MSGOTHIC");			
	//						//"Times New Roman"); 				
	//						//"MS Scan Serif");
	//						//"Small Fonts"); 				
	//						//"HG��??�l-PRO"); 				
	//						//"?? ����"); 		//?		
	//				//"?? ????"); 	//?			
	//						//"MS UI Gothic"); 				
	//				//"??POP�l"); 		//?		
	//						//"HG?��Ħ��-PRO"); 	//?				
	//						//"??��Ħ��-PRO"); 	//?			
	//						//"??�{�^????�l"); 	//?			
	//						fontName[ fontNo ] ); 
	//}
}

#ifdef _CHANNEL_MODIFY
char g_szChannelTitle[6][13] = {"","[��]","[��]","[��]","[ְ]","[��]"};
extern int TalkMode;
#endif
// ????????????????? ////////////////////////////////////////
void PutText( char fontPrio )
{
    static TTF_Font* mFont = TTF_OpenFont("/sdcard/jerrysa/simsun.ttf", FONT_SIZE);
    int i = 0;
    if (FontCnt == 0) return;
    for(int i = 0; i < FontCnt; i++)
    {
        if( FontBuffer[ i ].fontPrio == fontPrio ){
            if(ResoMode == 1)
            {
                /*SDL_Color white = {255, 255, 255, 255};
                SDL_Surface* surfaceMessage = TTF_RenderUNICODE_Solid(mFont, &FontBuffer[ i ].wstr, white);
                int length = FontBuffer[ i ].size;
                SDL_Rect destRect = {FontBuffer[ i ].x/2 + 1, FontBuffer[ i ].y/2 + 1, FONT_SIZE * length, FONT_SIZE};
                SDL_BlitSurface(surfaceMessage, 0, screenSurface, &destRect);
                SDL_FreeSurface(surfaceMessage);*/
            }
            else
            {
                //char text[] = "������";
                //char utf8text[256];
                //gb2312ToUtf8(utf8text, 256, text, strlen(text));
                SDL_Color shadeColor = {0,0,0,255};
                SDL_Surface* surfaceMessage = TTF_RenderUTF8_Solid(mFont,  FontBuffer[ i ].wstr, FontPal[ FontBuffer[ i ].color ]);
                SDL_Surface* outLine = TTF_RenderUTF8_Solid(mFont,  FontBuffer[ i ].wstr, shadeColor);
                int length = strlen( FontBuffer[ i ].wstr);
                SDL_Rect destRect = {FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FONT_SIZE * length, FONT_SIZE * 2};
                SDL_Rect outRect = {FontBuffer[ i ].x + 2, FontBuffer[ i ].y + 2, FONT_SIZE * length, FONT_SIZE * 2};
                SDL_BlitSurface(outLine, 0, screenSurface, &outRect);
                SDL_BlitSurface(surfaceMessage, 0, screenSurface, &destRect);
                SDL_FreeSurface(surfaceMessage);
                SDL_FreeSurface(outLine);
            }
        }
    }
//	HDC  hDc;
//#ifdef _READ16BITBMP
//	HDC  hDcSys;
//#endif
//	int i;
//	//int j;
//	bool colorFlag = false;
//	int	color;
//		
//	// ����??��???????
//	if( FontCnt == 0 ) return;
//	
//	// ????????????????????��x
//	lpDraw->lpBACKBUFFER->GetDC( &hDc );
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha) lpDraw->lpBACKBUFFERSYS->GetDC(&hDcSys);
//#endif
//
//	// ��?����????�@�e ( ��?��?��??? )
//	SetBkMode( hDc, TRANSPARENT );
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha) SetBkMode(hDcSys,TRANSPARENT);
//#endif
//	
//	HFONT hOldFont = ( HFONT )SelectObject( hDc, hFont ); // ?�q??????���V
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha) SelectObject(hDcSys,hFont);
//#endif
//	
//	// ?��????
//	SetTextColor( hDc, 0 );
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha) SetTextColor(hDcSys,0);
//#endif
//	// ??????��???????????��?
//	for( i = 0 ; i < FontCnt ; i++ ){
//		// �q?����????����?
//		if( FontBuffer[ i ].fontPrio == fontPrio ){
//			//???????????????
//			if(ResoMode == 1){
//				// ???��???��?
//				TextOut( hDc, FontBuffer[ i ].x/2 + 1, FontBuffer[ i ].y/2 + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//			} else {
//				// ???��???��?
//#ifdef _FONT_SIZE
//				static HFONT newFont = NULL;
//				static HFONT oldFont = NULL;
//				
//				if( newFont != NULL ) {
//					DeleteObject( newFont );
//					//SelectObject( hDc, oldFont );
//				}
//				if( FontBuffer[ i ].size <= 0 )
//					SelectObject( hDc, hFont );
//				else {
//					newFont = CreateNewFont( FontBuffer[ i ].size);
//					if( newFont != NULL )
//						oldFont = ( HFONT )SelectObject( hDc, newFont );
//				}
//#endif
//
//#ifndef _CHANNEL_MODIFY
//				TextOut( hDc, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//	#ifdef _READ16BITBMP
//				if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//	#endif
//#else
//				if(FontBuffer[i].fontPrio != FONT_PRIO_CHATBUFFER){
//					TextOut( hDc, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#ifdef _READ16BITBMP
//					if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#endif
//				}
//				else{
//#ifdef _TALK_WINDOW
//					if(g_bTalkWindow) {
//						break;
//					}
//#endif
//					if(TalkMode == 0){
//						TextOut( hDc, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#ifdef _READ16BITBMP
//						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#endif
//					}
//					else{
//						TextOut( hDc, FontBuffer[ i ].x + 1 + 22, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#ifdef _READ16BITBMP
//						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 1 + 22, FontBuffer[ i ].y + 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#endif
//					}
//					TextOut(hDc,2 + 1,432 + DISPLACEMENT_Y + 1,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
//#ifdef _READ16BITBMP
//					if(g_bUseAlpha) TextOut(hDcSys,2 + 1,432 + DISPLACEMENT_Y + 1,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
//#endif
//				}
//#endif
//#ifdef _FONT_SIZE
//				if( newFont != NULL ) {
//					DeleteObject( newFont );
//					SelectObject( hDc, oldFont );
//				}
//#endif
//			}
//		}
//	}
//	// �訞????? SetTextColor?????????????�L? ?
//	for( color = 0 ; color < 10 ; color++ ){
//		// ?????��???
//		for( i = 0 ; i < FontCnt ; i++ ){
//			// �q?����????����?
//			if( FontBuffer[ i ].fontPrio == fontPrio ){
//				// color ?��?��??????
//				if( FontBuffer[ i ].color == color ){
//					// ????��?????????????
//					if( colorFlag == false ){
//						SetTextColor( hDc, FontPal[ color ] );
//#ifdef _READ16BITBMP
//						if(g_bUseAlpha) SetTextColor( hDcSys, FontPal[ color ] );
//#endif
//						colorFlag = true;
//					}
//					//???????????????
//					if(ResoMode == 1){
//						// ??��??���e???��?��?
//						TextOut( hDc, FontBuffer[ i ].x >> 1, FontBuffer[ i ].y >> 1, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//					} else {
//						// ??��??���e???��?��?
//#ifdef _FONT_SIZE
//						static HFONT newFont = NULL;
//						static HFONT oldFont = NULL;
//						
//						if( newFont != NULL ) {
//							DeleteObject( newFont );
//							//SelectObject( hDc, oldFont );
//						}
//						if( FontBuffer[ i ].size <= 0 )
//							SelectObject( hDc, hFont );
//						else {
//							newFont = CreateNewFont( FontBuffer[ i ].size);
//							if( newFont != NULL )
//								oldFont = ( HFONT )SelectObject( hDc, newFont );
//						}
//#endif
//
//#ifndef _CHANNEL_MODIFY
//						TextOut( hDc, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//	#ifdef _READ16BITBMP
//						if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//	#endif
//#else
//						if(FontBuffer[i].fontPrio != FONT_PRIO_CHATBUFFER){
//							TextOut( hDc, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) );
//#ifdef _READ16BITBMP
//							if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) );
//#endif
//						}
//						else{
//#ifdef _TALK_WINDOW
//							if(g_bTalkWindow) break;
//#endif
//							if(TalkMode == 0){
//								TextOut( hDc, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#ifdef _READ16BITBMP
//								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#endif
//							}
//							else{
//								TextOut( hDc, FontBuffer[ i ].x + 22, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#ifdef _READ16BITBMP
//								if(g_bUseAlpha) TextOut( hDcSys, FontBuffer[ i ].x + 22, FontBuffer[ i ].y, FontBuffer[ i ].str, ( int )strlen( FontBuffer[ i ].str ) ); 
//#endif
//							}
//							TextOut(hDc,2,432 + DISPLACEMENT_Y,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
//#ifdef _READ16BITBMP
//							if(g_bUseAlpha) TextOut(hDcSys,2,432 + DISPLACEMENT_Y,g_szChannelTitle[TalkMode],(int)strlen(g_szChannelTitle[TalkMode])); 
//#endif
//						}
//#endif
//#ifdef _FONT_SIZE
//						if( newFont != NULL ) {
//							DeleteObject( newFont );
//							SelectObject( hDc, oldFont );
//						}
//#endif
//					}
//				}
//			}
//		}
//		colorFlag = false;
//	}
//	
//	SelectObject( hDc ,hOldFont );
//	lpDraw->lpBACKBUFFER->ReleaseDC( hDc );
//#ifdef _READ16BITBMP
//	if(g_bUseAlpha){
//		SelectObject(hDcSys,hOldFont);
//		lpDraw->lpBACKBUFFERSYS->ReleaseDC(hDcSys);
//	}
//#endif
}

//---------------------------------------------------------------------------//
// ?????????��V
//---------------------------------------------------------------------------//
void snapShot( void )
{
//	DDSURFACEDESC ddsdDesc;
//	RGBQUAD rgbpal[256];
//	char fileName[256];
//	int i;
//	struct tm *nowTime;
//	time_t longTime;
//	FILE *fp;
//	int w, h;
//	POINT 	clientPoint; // ??????????��???????��?�t??�t
//
//	// ?????�B�l????
//	clientPoint.x = 0;
//	clientPoint.y = 0;
//	// ??????????��???????��?��x
//	ClientToScreen( hWnd, &clientPoint ); 
//
//	// ��V??????��
//	if( _mkdir( "screenshot" ) != 0 )
//	{
//		if( errno != EEXIST )
//			return;
//	}
//
//	// ��V????��??��
//	time( &longTime );					// ???�T??��x
//	nowTime = localtime( &longTime );
//
//	for( i =  0; i < 1000; i ++ )
//	{
//		sprintf( fileName, "screenshot\\sa%02d%02d%02d_%03d.bmp",
//			(nowTime->tm_year % 100), nowTime->tm_mon+1, nowTime->tm_mday, i );
// 
//		if( (fp = fopen( fileName, "r" )) != NULL )
//		{
//			// ????????????????��??
//			fclose( fp );
//			continue;
//		}
//		else
//		{
//			break;
//		}
//    }
//
//	if(displayBpp != 16){
//		// ????��x
//		for( i = 0; i < 256; i++ ){
//			rgbpal[i].rgbRed   = Palette[i].peRed;
//			rgbpal[i].rgbGreen = Palette[i].peGreen;
//			rgbpal[i].rgbBlue  = Palette[i].peBlue;
//			rgbpal[i].rgbReserved = 0;
//		}
//	}
//	memset( &ddsdDesc, 0, sizeof( DDSURFACEDESC ) );
//	ddsdDesc.dwSize = sizeof( DDSURFACEDESC );
//	if( lpDraw->lpFRONTBUFFER->Lock( NULL, &ddsdDesc, 0, NULL ) != DD_OK ){
//#ifdef _DEBUG
//		//MessageBox(hWnd,"ǰ������������ʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//#endif
//		return;
//	}
//
//	if( ResoMode == 1 )
//	{
//		w = 320;
//		h = 240;
//	}
//	else
//	{
//		w = 640;
//		h = 480;
//	}
//	if(displayBpp == 16){
//		unsigned char *mem = new unsigned char[w * h * 3],*pmem,pR,pG,pB;
//		if(mem == NULL){
//#ifdef _DEBUG
//			//MessageBox(hWnd,"����������ʧ��ʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
//#endif
//			return;
//		}
//		unsigned short *work = (unsigned short*)ddsdDesc.lpSurface;
//		pmem = mem;
//		pmem += w * h * 3;
//
//		// source face һ���ƶ�����byte
//		ddsdDesc.lPitch >>= 1;
//		// work ������ӫĻ��λ��,Ҫ��ƫ��
//		work += ddsdDesc.lPitch * g_clientPoint.y + g_clientPoint.x;
//		for(int y=0;y<h;y++){
//			pmem -= w * 3;
//			for(int x=0;x<w;x++){
//				// 565 ��ʾģʽ
//				if(gBitRShift == 2){
//					pR = (unsigned char)((((work[x] & 0xf800)) >> 11) << 3);
//					pG = (unsigned char)((((work[x] & 0x07e0)) >> 5) << 2);
//					pB = (unsigned char)((work[x] & 0x001f) << 3);
//				}
//				// 555 ��ʾģʽ
//				else{
//					pR = (unsigned char)((work[x] >> 10) << 3);
//					pG = (unsigned char)(((work[x] & 0x03e0) >> 5) << 3);
//					pB = (unsigned char)((work[x] & 0x001f) << 3);
//				}
//				*pmem++ = pB;
//				*pmem++ = pG;
//				*pmem++ = pR;
//			}
//			// ����
//			work += ddsdDesc.lPitch;
//			pmem -= w * 3;
//		}
//		
//		saveBmpFile16(fileName,mem,w,h);
//		delete[] mem;
//	}
//	else
//		saveBmpFile( fileName, (unsigned char*)ddsdDesc.lpSurface,
//			clientPoint.x, clientPoint.y, w, h, ddsdDesc.lPitch, rgbpal, 256 );
//
//	if( lpDraw->lpFRONTBUFFER->Unlock( NULL ) != DD_OK )
//		return;
}


// ???????????��V
//bool saveBmpFile( const char *filename, unsigned char *buf,
//	int x, int y, int width, int height, int srcpitch,
//	RGBQUAD *rgbpal, int colorCnt )
//{
//	/*HANDLE fh;
//	BITMAPFILEHEADER fileheader;
//	BITMAPINFOHEADER infoheader;
//
//	int linesize = (width+3)/4;
//	unsigned long writtensize;
//	unsigned char zero = 0;
//	int i;
//
//	fh = CreateFile( filename, GENERIC_WRITE, (unsigned long)NULL,
//			(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL );
//
//	if( fh == INVALID_HANDLE_VALUE )
//	{
//		return false;
//	}
//
//
//	fileheader.bfType = 0x4D42;
//	fileheader.bfSize = sizeof( BITMAPFILEHEADER )
//		+sizeof( BITMAPINFOHEADER )+sizeof(RGBQUAD)*(colorCnt)+linesize*height;
//	fileheader.bfReserved1 = 0;
//	fileheader.bfReserved2 = 0;
//	fileheader.bfOffBits = sizeof( BITMAPFILEHEADER )
//		+sizeof( BITMAPINFOHEADER )+sizeof(RGBQUAD)*(colorCnt);
//
//	WriteFile( fh, (void*)(&fileheader), sizeof( fileheader ), &writtensize, NULL );
//
//
//	infoheader.biSize = sizeof( BITMAPINFOHEADER );
//	infoheader.biWidth = width;
//	infoheader.biHeight = height;
//	infoheader.biPlanes = 1;
//	infoheader.biBitCount = 8;
//	infoheader.biCompression = NULL;
//	infoheader.biSizeImage = sizeof( BITMAPINFOHEADER )
//		+sizeof(RGBQUAD)*(colorCnt)+linesize*height;
//	infoheader.biXPelsPerMeter = 0;
//	infoheader.biYPelsPerMeter = 0;
//	infoheader.biClrUsed = colorCnt;
//	infoheader.biClrImportant = 0;
//
//	WriteFile( fh, (void*)(&infoheader), sizeof(BITMAPINFOHEADER), &writtensize, NULL );
//	WriteFile( fh, (void*)rgbpal, sizeof(RGBQUAD)*(colorCnt), &writtensize, NULL );
//
//	for( i = 0; i < height; i++ )
//	{
//		WriteFile( fh, (void*)(buf+srcpitch*(height+y-1-i)+x), width, &writtensize, NULL );
//		WriteFile( fh, (void*)(&zero), linesize-width, &writtensize, NULL );
//	}
//
//	CloseHandle( fh );
//*/
//	return true;
//}

//bool saveBmpFile16( const char *filename, unsigned char *buf, int width, int height)
//{
//	/*HANDLE fh;
//	BITMAPFILEHEADER fileheader;
//	BITMAPINFOHEADER infoheader;
//
//	unsigned long writtensize;
//	unsigned char zero = 0;
//
//	fh = CreateFile( filename, GENERIC_WRITE, (unsigned long)NULL,
//			(LPSECURITY_ATTRIBUTES)NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL );
//
//	if( fh == INVALID_HANDLE_VALUE ) return false;
//
//	fileheader.bfType = 0x4D42;
//	fileheader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 3;
//	fileheader.bfReserved1 = 0;
//	fileheader.bfReserved2 = 0;
//	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//
//	infoheader.biSize = sizeof(BITMAPINFOHEADER);
//	infoheader.biWidth = width;
//	infoheader.biHeight = height;
//	infoheader.biPlanes = 1;
//	infoheader.biBitCount = 24;
//	infoheader.biCompression = 0;
//	infoheader.biSizeImage = width * height * 3;
//	infoheader.biXPelsPerMeter = 0;
//	infoheader.biYPelsPerMeter = 0;
//	infoheader.biClrUsed = 0;
//	infoheader.biClrImportant = 0;
//
//	WriteFile(fh,(void*)(&fileheader),sizeof(fileheader),&writtensize,NULL);
//	WriteFile(fh,(void*)(&infoheader),sizeof(BITMAPINFOHEADER),&writtensize,NULL);
//	WriteFile(fh,(void*)buf,width * height * 3,&writtensize,NULL);
//
//	CloseHandle( fh );*/
//
//	return true;
//}

#ifdef _READ16BITBMP
void Draw16BitmapToSurface2(SURFACE_INFO *surface_info,SDL_Surface* lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,void* pBmpInfo)
{
//	DDSURFACEDESC ddsd;
//	unsigned short *pDest;
//	unsigned char R,G,B;
//	char *pSource;
//	int surfacePitch,i,j;
//	DDSURFACEDESC ddsdsys;
//	unsigned short *pDestSys;
//	int surfacePitchSys;
//	
//	if(surface_info->lpSurface == NULL) return;
//	if(g_bUseAlpha && lpSurfaceSys == NULL) return;
//	
//	ZeroMemory(&ddsd,sizeof(DDSURFACEDESC));
//	ddsd.dwSize = sizeof(DDSURFACEDESC);
//	ZeroMemory(&ddsdsys,sizeof(DDSURFACEDESC));
//	ddsdsys.dwSize = sizeof(DDSURFACEDESC);
//	
//	/*if(FAILED(surface_info->lpSurface->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL))) return; 
//	if(g_bUseAlpha){
//		if(FAILED(lpSurfaceSys->Lock(NULL,&ddsdsys,DDLOCK_WAIT,NULL))){
//			surface_info->lpSurface->Unlock(NULL);
//			return; 
//		}
//	}*/
//
//	pDest = (unsigned short*)(ddsd.lpSurface);
//	if(g_bUseAlpha) pDestSys = (unsigned short*)(ddsdsys.lpSurface);
//	pSource = pRealBinBits + offsetY * RealBinWidth + offsetX;
//	surfacePitch = ddsd.lPitch >> 1;
//	if(g_bUseAlpha) surfacePitchSys = ddsdsys.lPitch >> 1;
//	
//	for(i=0;i<sizeY;i++){
//		for(j=0;j<sizeX;j++){
//			R = g_rgbPal[(unsigned char)*pSource].rgbRed >> 3;
//			G = g_rgbPal[(unsigned char)*pSource].rgbGreen >> 2;
//			B = g_rgbPal[(unsigned char)*pSource].rgbBlue >> 3;
//			*pDest = (unsigned short)((R << 11) | (G << 5) | B);
//			pDest++;
//			pSource++;
//			if(g_bUseAlpha){
//				*pDestSys = (unsigned short)((R << 11) | (G << 5) | B);
//				pDestSys++;
//			}
//		}
//		// ����һ��
//		pDest += surfacePitch - sizeX;
//		pSource += RealBinWidth - sizeX;
//		if(g_bUseAlpha) pDestSys += surfacePitchSys - sizeX;
//	}
////	surface_info->lpSurface->Unlock(NULL);
//	if(g_bUseAlpha) lpSurfaceSys->Unlock(NULL);
//	// �д�alpha channel
//	if(g_bUseAlpha){
//		if(surface_info->useAlpha == true){
//			unsigned char *pAlphaSource = pRealBinAlpha + offsetY * RealBinWidth + offsetX; 
//			unsigned char *pAlphaDest = surface_info->lpAlphaData;
//			
//			memset(pAlphaDest,0,SurfaceSizeX*SurfaceSizeY);
//			for(i=0;i<sizeY;i++){
//				for(j=0;j<sizeX;j++){
//					*pAlphaDest = (*pAlphaSource) >> 3;
//					pAlphaDest++;
//					pAlphaSource++;
//				}
//				// ����һ��
//				pAlphaDest += SurfaceSizeX - sizeX;
//				pAlphaSource += RealBinWidth - sizeX;
//			}
//		}
//	}
//	
	return;
}
#endif

int CG_PKSERVER_PANEL;
int CG_BATTTLE_SKILLCHOICE;
int CG_FIELD_SKILL_PANEL;
int CG_FIELD_CHATROOM_PANEL;
int CG_FIELD_SV_SELL_PANEL;
int CG_FIELD_SV_SELL_PRICE_PANEL;
int CG_NEW_STATUS_WND;
int CG_NEWITEM_WND;
int CG_TRADE_WND;
int CG_TRADE_VIEWWND;
// �[??????
int CG_WND_G_0;
int CG_WND_G_1;
int CG_WND_G_2;
int CG_WND_G_3;
int CG_WND_G_4;
int CG_WND_G_5;
int CG_WND_G_6;
int CG_WND_G_7;
int CG_WND_G_8;
// �[???????
int CG_WND2_G_0;
int CG_WND2_G_1;
int CG_WND2_G_2;
int CG_WND2_G_3;
int CG_WND2_G_4;
int CG_WND2_G_5;
int CG_WND2_G_6;
int CG_WND2_G_7;
int CG_WND2_G_8;
// �[???????
int CG_WND3_G_7;
int CG_WND3_G_8;
int CG_WND3_G_9;
// �o��?????
int CG_BTL_PET_CHANGE_WND;
// ????????
int CG_PET_WND_VIEW;
int CG_PET_WND_DETAIL;
int CG_NAME_CHANGE_WND;
// ?????????
int CG_ITEM_WND_1;
int CG_JUJUTU_WND;
int CG_ITEM_WND_SELECT_WND;
int CG_STATUS_WND_GROUP_WND;
// ????????
int CG_MAP_WND;
// ??????????
int CG_STATUS_WND;
// ????????
int CG_MAIL_WND;
int CG_MAIL_WND_SEND_WND;
int CG_MAIL_WND_PET_SEND_WND;
int CG_MAIL_WND_HISTORY_WND;
// ?????????
int CG_ALBUM_WND;
// ?????��?�K??????
int CG_CHAT_REGISTY_WND;
// ��?�V?�[??????
int CG_COMMON_WIN_YORO;
int CG_FIELD_HELP_WND;
// ??????????
int CG_MSG_WND;
int CG_PET_WAZA_WND;
// ?????????????
int CG_ITEMSHOP_WIN;
// ??????????
int CG_SKILLSHOP_WIN;
// ??????????
int CG_ITEMSHOP_KOSU_WIN;
int CG_FAMILY_DETAIL_WIN;
int CG_FAMILY_BANK_WIN;
int CG_BM_WND;

void SetAnimTbl()
{
	if(g_bUseAlpha){
		CG_PKSERVER_PANEL = OLD_GRAPHICS_START+37;
		CG_BATTTLE_SKILLCHOICE = OLD_GRAPHICS_START+53;
		CG_FIELD_SKILL_PANEL = OLD_GRAPHICS_START+50;
		CG_FIELD_CHATROOM_PANEL = OLD_GRAPHICS_START+54;
		CG_FIELD_SV_SELL_PANEL = OLD_GRAPHICS_START+55;
		CG_FIELD_SV_SELL_PRICE_PANEL = OLD_GRAPHICS_START+56;
		CG_NEW_STATUS_WND = OLD_GRAPHICS_START+51;
		CG_NEWITEM_WND = OLD_GRAPHICS_START+52;
		CG_TRADE_WND = OLD_GRAPHICS_START+48;
		CG_TRADE_VIEWWND = OLD_GRAPHICS_START+49;
		CG_WND_G_0 = OLD_GRAPHICS_START;
		CG_WND_G_1 = OLD_GRAPHICS_START+1;
		CG_WND_G_2 = OLD_GRAPHICS_START+2;
		CG_WND_G_3 = OLD_GRAPHICS_START+3;
		CG_WND_G_4 = OLD_GRAPHICS_START+4;
		CG_WND_G_5 = OLD_GRAPHICS_START+5;
		CG_WND_G_6 = OLD_GRAPHICS_START+6;
		CG_WND_G_7 = OLD_GRAPHICS_START+7;
		CG_WND_G_8 = OLD_GRAPHICS_START+8;
		CG_WND2_G_0 = OLD_GRAPHICS_START+9;
		CG_WND2_G_1 = OLD_GRAPHICS_START+10;
		CG_WND2_G_2 = OLD_GRAPHICS_START+11;
		CG_WND2_G_3 = OLD_GRAPHICS_START+12;
		CG_WND2_G_4 = OLD_GRAPHICS_START+13;
		CG_WND2_G_5 = OLD_GRAPHICS_START+14;
		CG_WND2_G_6 = OLD_GRAPHICS_START+15;
		CG_WND2_G_7 = OLD_GRAPHICS_START+16;
		CG_WND2_G_8 = OLD_GRAPHICS_START+17;
		CG_WND3_G_7 = OLD_GRAPHICS_START+18;
		CG_WND3_G_8 = OLD_GRAPHICS_START+19;
		CG_WND3_G_9 = OLD_GRAPHICS_START+20;
		CG_BTL_PET_CHANGE_WND = OLD_GRAPHICS_START+21;
		CG_PET_WND_VIEW = OLD_GRAPHICS_START+22;
		CG_PET_WND_DETAIL	= OLD_GRAPHICS_START+23;
		CG_NAME_CHANGE_WND = OLD_GRAPHICS_START+24;
		CG_ITEM_WND_1 = OLD_GRAPHICS_START+25;
		CG_JUJUTU_WND = OLD_GRAPHICS_START+26;
		CG_ITEM_WND_SELECT_WND = OLD_GRAPHICS_START+27;
		CG_STATUS_WND_GROUP_WND = OLD_GRAPHICS_START+28;
		CG_MAP_WND = OLD_GRAPHICS_START+29;
		CG_STATUS_WND = OLD_GRAPHICS_START+452;
		CG_MAIL_WND = OLD_GRAPHICS_START+30;
		CG_MAIL_WND_SEND_WND = OLD_GRAPHICS_START+38;
		CG_MAIL_WND_PET_SEND_WND = OLD_GRAPHICS_START+39;
		CG_MAIL_WND_HISTORY_WND = OLD_GRAPHICS_START+40;
		CG_ALBUM_WND = OLD_GRAPHICS_START+41;
		CG_CHAT_REGISTY_WND = OLD_GRAPHICS_START+42;
		CG_COMMON_WIN_YORO = OLD_GRAPHICS_START+31;
		CG_FIELD_HELP_WND = OLD_GRAPHICS_START+45;
		CG_MSG_WND = OLD_GRAPHICS_START+47;
		CG_PET_WAZA_WND = OLD_GRAPHICS_START+32;
		CG_ITEMSHOP_WIN = OLD_GRAPHICS_START+33;
		CG_SKILLSHOP_WIN = OLD_GRAPHICS_START+34;
		CG_ITEMSHOP_KOSU_WIN = OLD_GRAPHICS_START+35;
		CG_FAMILY_DETAIL_WIN = OLD_GRAPHICS_START+43;
		CG_FAMILY_BANK_WIN = OLD_GRAPHICS_START+44;
		CG_BM_WND = OLD_GRAPHICS_START+36;
	}
	else{
		CG_PKSERVER_PANEL = 26192;							// ѡȡ��ϵ�����
		CG_BATTTLE_SKILLCHOICE = 26389;					// ս����ѡ����
		CG_FIELD_SKILL_PANEL = 26352;						// ְҵ���ܽ���
		CG_FIELD_CHATROOM_PANEL = 26427;				// �����ҽ���
		CG_FIELD_SV_SELL_PANEL = 35221;					// ��̯����(��)
		CG_FIELD_SV_SELL_PRICE_PANEL = 35223;		// �����ۼ��Ӵ�
		CG_NEW_STATUS_WND = 26386;
#ifdef _PET_ITEM
		CG_NEWITEM_WND = 26455;									// ����װ����λ�Ӵ�(�б�ǩ)
#else
		CG_NEWITEM_WND = 26388;									// ����װ����λ�Ӵ�(���֡��š�����)
#endif

		CG_TRADE_WND = 26328;										// �������Ӵ�

		CG_TRADE_VIEWWND = 26329;								// ���׼����Ӵ�
		CG_WND_G_0 = 26001;
		CG_WND_G_1 = 26002;
		CG_WND_G_2 = 26003;
		CG_WND_G_3 = 26004;
		CG_WND_G_4 = 26005;
		CG_WND_G_5 = 26006;
		CG_WND_G_6 = 26007;
		CG_WND_G_7 = 26008;
		CG_WND_G_8 = 26009;
		CG_WND2_G_0 = 26021;
		CG_WND2_G_1 = 26022;
		CG_WND2_G_2 = 26023;
		CG_WND2_G_3 = 26024;
		CG_WND2_G_4 = 26025;
		CG_WND2_G_5 = 26026;
		CG_WND2_G_6 = 26027;
		CG_WND2_G_7 = 26028;
		CG_WND2_G_8 = 26029;
		CG_WND3_G_7 = 26037;
		CG_WND3_G_8 = 26038;
		CG_WND3_G_9 = 26039;
		CG_BTL_PET_CHANGE_WND = 26040;					// �P�f?????�V?�v??????
		CG_PET_WND_VIEW = 26044;								// ???????????????
		CG_PET_WND_DETAIL = 26045;							// ????��??????
		CG_NAME_CHANGE_WND = 26049;							// ���q��??????
		CG_ITEM_WND_1 = 26061;									// ??????????
		CG_JUJUTU_WND = 26068;									// ����?????
		CG_ITEM_WND_SELECT_WND = 26070;					// �k��?????
		CG_STATUS_WND_GROUP_WND = 26071;				// ????????????
		CG_MAP_WND = 26081;											// ????????
		CG_STATUS_WND = 26073;									// ???????????????
		CG_MAIL_WND = 26082;										// ????????
		CG_MAIL_WND_SEND_WND = 26200;						// ????��?????
		CG_MAIL_WND_PET_SEND_WND = 26201;				// ????????��?????
		CG_MAIL_WND_HISTORY_WND = 26203;				// ??????????
		CG_ALBUM_WND = 26230;										// ?????????
		CG_CHAT_REGISTY_WND = 26232;
		CG_COMMON_WIN_YORO = 26090;							// "???????"??????????
		CG_FIELD_HELP_WND = 26258;
		CG_MSG_WND = 26296;											// Message's Main Window ,added by LeiBoy
		CG_PET_WAZA_WND = 26130;
		CG_ITEMSHOP_WIN = 26138;
		CG_SKILLSHOP_WIN = 26139;
		CG_ITEMSHOP_KOSU_WIN = 26140;
		CG_FAMILY_DETAIL_WIN = 26239;
		CG_FAMILY_BANK_WIN = 26240;
		CG_BM_WND = 26141;
	}
}

#ifdef _FONT_SIZE
//HFONT CreateNewFont( int size) {
//
//	//return CreateFont( 
//	//	size, 	/* ????????	*/ 
//	//	0, 		/* ��????��??	*/ 
//	//	0, 		/* ???????�T	*/ 
//	//	0,		/* ??? ????x��??�T	*/ 
//	//	
//	//	/* ?????�^?	*/ 
//	//	FW_NORMAL,			// 400 
//	//	
//	//	/* ?????�l�N��????	*/ 
//	//	false, 					
//	//	
//	//	/* ?�_��?�N��????	*/ 
//	//	false,					
//	//	
//	//	/* �e?��?�_��?�N��????	*/ 
//	//	false,					
//	//	
//	//	/* ��????������	*/ 
//	//	GB2312_CHARSET/*CHINESEBIG5_CHARSET*/,// ?�G?��?
//	//	
//	//	/* ��?�ڥT	*/ 
//	//	OUT_DEFAULT_PRECIS, 
//	//	
//	//	/* ??????�ڥT	*/ 
//	//	CLIP_DEFAULT_PRECIS,
//	//	
//	//	/* ��?����	*/ 
//	//	DEFAULT_QUALITY,	// ????????��????????
//	//	
//	//	/* ??? */ 
//	//	FIXED_PITCH |
//	//	
//	//	/* ????	*/ 
//	//	FF_ROMAN,			// ??��?????????????��????????MS(R) Serif????????
//	//	
//	//	/* ???????����???????	*/ 				
//	//	"����" /*"����_GB2312"*/ ); 
//    return 0;
//
//}
#endif