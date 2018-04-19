/************************/
/*	sprmgr.h			*/
/************************/
#ifndef _SPRMGR_H_
#define _SPRMGR_H_
#include <SDL.h>

#ifndef _READ16BITBMP
	//�غ��� _READ16BITBMPVARIABLES �Ĳ���������
	//#define MAX_GRAPHICS	 	350000	// ���ͼ��// ????��?
#else
	#define MAX_GRAPHICS_24		50000		// �߲�ͼ���ͼ��
	#define OLD_GRAPHICS_START	1000000		// ԭ��realbinͼ�������
	#define MAX_GRAPHICS	 	OLD_GRAPHICS_START + MAX_GRAPHICS_24  // ���ͼ��// ????��?
	#define MAX_GRAPHICS_ALPHA	50000		// �߲�ͼalphaͨ�����ͼ��
#endif

#ifdef _READ16BITBMPVARIABLES					//�� _READ16BITBMP ����Ҫ�Ĳ���
	#define MAX_GRAPHICS_24		50000		// �߲�ͼ���ͼ��
	#define OLD_GRAPHICS_START	500000		// ԭ��realbinͼ�������
	#define MAX_GRAPHICS	 	OLD_GRAPHICS_START + MAX_GRAPHICS_24  // ���ͼ��// ??
#endif

#define	DEF_COLORKEY		0				//Ԥ��͸��ɫ// �a����???��k?

#define SURFACE_WIDTH   64 			//��ͼ�õ�source face��// ��?????????
#define SURFACE_HEIGHT  48			//��ͼ�õ�source face��// ��??????????

extern SDL_Surface* lpBattleSurface;
#ifdef _READ16BITBMP
extern SDL_Surface* lpBattleSurfaceSys;
#endif
extern int SurfaceDispCnt;
extern int SurfaceSizeX;
extern int SurfaceSizeY;
extern unsigned int SurfaceDate;
#include <SDL.h>
struct surfaceInfo{

	SDL_Surface* lpSurface; 	// ??????????
#ifdef _READ16BITBMP
	unsigned char *lpAlphaData;	// ��alpha����
	bool useAlpha;			// �Ƿ�ʹ��alpha
#endif

	int	bmpNo;						//ͼ�ı��,-1 ����source faceΪ�յ�// ??????�k?
									// ??�k? -1 ???????????????
	unsigned int date;						//��¼���source faceʹ�õ�����		// ????????��???�T��???
	
	short offsetX;					// ����?�t??????????��
	short offsetY;					// ����?�t??????????��

	surfaceInfo	*pNext;				// ????????????�B�l??????
	
};

typedef struct surfaceInfo SURFACE_INFO;
typedef struct{
	SURFACE_INFO *lpSurfaceInfo;
#ifdef _READ16BITBMP
	SURFACE_INFO *lpSurfaceInfoSys;
#endif
	short width,height;
}SPRITE_INFO;

extern SPRITE_INFO SpriteInfo[];
extern SURFACE_INFO SurfaceInfo[];
#ifdef _READ16BITBMP
extern SURFACE_INFO SurfaceInfoSys[];
#endif
extern int SurfaceCnt;
extern int VramSurfaceCnt;
extern int SysramSurfaceCnt;
extern int SurfaceSearchPoint;
#ifdef _READ16BITBMP
	#ifdef _DEBUG
	extern unsigned int iTotalUseTime;
	extern unsigned int iTotalProcTime;
	extern unsigned int iTotalRunCount;
	extern int tf;
	#endif
#endif

#ifdef _DEBUG		
extern int SurfaceUseCnt;
#endif

bool InitOffScreenSurface( void );
void InitSurfaceInfo( void );
void InitSpriteInfo( void );
bool LoadBmp( int bmpNo );


#endif
