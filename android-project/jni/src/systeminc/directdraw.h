#ifndef _DIRECT_DRAW_H_
#define _DIRECT_DRAW_H_

#include "sprmgr.h"
#include <SDL.h>

extern const int MAX_PAL;
#define DEF_PAL	0	//?????
#define PAL_CHANGE_TIME			3600
#define SYSTEM_PAL_WHITE 		255
#define SYSTEM_PAL_AQUA 		254
#define SYSTEM_PAL_PURPLE 		253
#define SYSTEM_PAL_BLUE 		252
#define SYSTEM_PAL_YELLOW 		251
#define SYSTEM_PAL_GREEN 		250
#define SYSTEM_PAL_RED 			249
#define SYSTEM_PAL_GRAY 		248
#define SYSTEM_PAL_BLUE3 		247
#define SYSTEM_PAL_GREEN3 		246

#define SYSTEM_PAL_WHITE2 		7
#define SYSTEM_PAL_AQUA2 		6
#define SYSTEM_PAL_PURPLE2 		5
#define SYSTEM_PAL_BLUE2 		4
#define SYSTEM_PAL_YELLOW2 		3
#define SYSTEM_PAL_GREEN2 		2
#define SYSTEM_PAL_RED2 		1
#define SYSTEM_PAL_BLACK 		0
#define SYSTEM_PAL_BLUE4 		8
#define SYSTEM_PAL_GREEN4 		9
#define FONT_PAL_WHITE 		0
#define FONT_PAL_AQUA 		1
#define FONT_PAL_PURPLE 	2
#define FONT_PAL_BLUE 		3
#define FONT_PAL_YELLOW 	4
#define FONT_PAL_GREEN 		5
#define FONT_PAL_RED 		6
#define FONT_PAL_GRAY 		7
#define FONT_PAL_BLUE2 		8
#define FONT_PAL_GREEN2 	9
enum{
	DRAW_BACK_NORMAL,	// �G��
	DRAW_BACK_NON,		// ��????
	DRAW_BACK_PRODUCE,	// ?��
	DRAW_BACK_BATTLE,	// �P�f
};
typedef struct
{
	int			lpDD;				// DirectDraw??????
	int			lpDD2;				// DirectDraw2??????
	SDL_Surface*		lpFRONTBUFFER;		// ��????
	SDL_Surface*		lpBACKBUFFER;		// ?????
#ifdef _READ16BITBMP
	SDL_Surface*		lpBACKBUFFERSYS;
#endif
	int		lpCLIPPER;			// ?????
	int			ddsd;				// ??????????????�B�l�e?
	int		lpPALETTE;			// ????
	int						xSize, ySize;		// ?????????
} DIRECT_DRAW;

typedef struct tagPALETTEENTRY {
    unsigned char        peRed;
    unsigned char        peGreen;
    unsigned char        peBlue;
    unsigned char        peFlags;
} PALETTEENTRY;

extern PALETTEENTRY			Palette[256];
typedef struct{
	int palNo;		// ????�k?
	int time;		// ?????
	int flag;		// ??????
}PALETTE_STATE;
extern DIRECT_DRAW	*lpDraw;
extern bool DDinitFlag;	
//extern HFONT hFont;
extern PALETTE_STATE PalState;	
extern bool	PalChangeFlag;	
extern int BackBufferDrawType;
bool InitDirectDraw( void );
SDL_Color getColorFromPalette(unsigned char index);
bool InitPalette( void );
void ClearBackSurface( void );
void ClearSurface( SDL_Surface* lpSurface );
//LPBITMAPINFO LoadDirectDrawBitmap( char *pFile );
SDL_Surface* CreateSurface( short sizeX, short sizeY, unsigned long ColorKey, int VramOrSysram );
#ifdef _READ16BITBMP
int DrawSurfaceFast( short bx, short by, SDL_Surface* lpSurface,SDL_Surface* lpSurfaceSys);
void DrawBitmapToSurface2(SDL_Surface* lpSurface,SDL_Surface* lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,void* pBmpInfo);
void Draw16BitmapToSurface2(SURFACE_INFO *surface_info,SDL_Surface* lpSurfaceSys,int offsetX,int offsetY,int sizeX,int sizeY,void* pBmpInfo);
#endif
int DrawSurfaceFast( short bx, short by, SDL_Surface* lpSurface );
void DrawBitmapToSurface2( SDL_Surface*& lpSurface, int offsetX, int offsetY, int sizeX, int sizeY, void* pBmpInfo );
int DrawSurfaceFast2( short bx, short by, SDL_Rect *rect, SDL_Surface* lpSurface );
void DrawBitmapToSurface( SDL_Surface* lpSurface, int offsetX, int offsetY, void* pInfo );
void Flip( void );
bool CheckSurfaceLost( void );
void ReleaseDirectDraw( void );
void DrawDebugLine( unsigned char color );
void DrawBox( SDL_Rect *rect, unsigned char color, bool fill );
void InitFont( int fontNo );
void PutText( char fontPrio ); 
void PaletteProc( void );
void PaletteChange( int palNo, int time );
void DrawAutoMapping( int x, int y, unsigned char *autoMap, int w, int h );
int getAutoMapColor( unsigned int GraphicNo );
int getNearestColorIndex( unsigned long color, PALETTEENTRY *palette, int entry );
void snapShot( void );
//bool saveBmpFile( const char *, unsigned char *,	int, int, int, int, int, RGBQUAD *, int );
//bool saveBmpFile16( const char *, unsigned char *,	int, int);
void DrawMapEffect( void );
#endif
