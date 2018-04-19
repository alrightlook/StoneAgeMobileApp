#ifndef _LOADREALBIN_H_
#define _LOADREALBIN_H_

#include <SDL.h>

void initAutoMapColor( char * );
void makeAutoMapColor( void );
int writeAutoMapColor( char *, char * );
int readAutoMapColor( char *, char * );


#ifdef _HI16_REEDITIMAGE
void initRealbinFPoint();
#endif

bool initRealbinFileOpen(char *realbinfilename, char *addrbinfilename);
#ifdef _READ16BITBMP
int InitRealTruebinFileOpen(char *szRealTrueBinFileName,char *szAdrnTruebinFileName);
#endif
void cleanupRealbin(void);

extern unsigned char autoMapColorTbl[];

#include <stdio.h>
typedef unsigned char MOJI;
typedef unsigned char U1;
typedef          char S1;
typedef unsigned short U2;
typedef          short S2;
typedef unsigned long U4;
typedef          long S4;
typedef float  F4;
typedef double F8;
#ifdef _READ16BITBMP
bool Read16BMP(int BmpNo,unsigned char **BmpData,int *width,int *height,unsigned char **AlphaData,bool *useAlpha);
#endif
bool realGetImage(int graphicNo, unsigned char **bmpdata, int *width, int *height);
bool realGetPos(U4 GraphicNo , S2 *x , S2 *y);
bool realGetWH(U4 GraphicNo , S2 *w , S2 *h);
bool realGetHitPoints(U4 GraphicNo , S2 *HitX , S2 *HitY);
bool realGetHitFlag(U4 GraphicNo , S2 *Hit);
bool realGetPrioType(U4 GraphicNo , S2 *prioType);
bool realGetHeightFlag(U4 GraphicNo , S2 *Height);
bool realGetNo( U4 CharAction , U4 *GraphicNo );
int realGetSoundEffect(U4 GraphicNo);
int realGetWalkSoundEffect(U4 GraphicNo);
bool realGetBitmapNo( int num );
typedef struct {
	unsigned char atari_x,atari_y;	//��??
	unsigned short hit;				// �ɷ����� //��???
	short height;				//?????
	short broken;				//????
	short indamage;				//�VHP????
	short outdamage;			//��????
	short inpoison;				//�V��
	short innumb;				//�V???
	short inquiet;				//�V��?
	short instone;				//�V��?
	short indark;				//�V??
	short inconfuse;			//�V??
	short outpoison;			//�ҥ�
	short outnumb;				//��??
	short outquiet;				//�ң�?
	short outstone;				//����?
	short outdark;				//��??
	short outconfuse;			//��??
	short effect1;				//?????1??��????????��?????
	short effect2;				//?????2?�V???????��?????
	unsigned short damy_a;
	unsigned short damy_b;
	unsigned short damy_c;
	unsigned int bmpnumber;		//??�k?
} MAP_ATTR;

struct ADRNBIN{
	unsigned int	bitmapno;
	unsigned int	adder;
	unsigned int	size;
	int	xoffset;
	int	yoffset;
	unsigned int width;
	unsigned int height;

	MAP_ATTR attr;

};

#ifdef _READ16BITBMP
struct AddressBin_s{
	unsigned int bitmapno;			// ͼ��
	unsigned int adder;					// ������ͼ�ص������ʼλַ
	unsigned int palSize;				// ɫ�����ϴ�С
	unsigned int size;					// һ�����ϵĴ�С
	unsigned int alpha_size;		// alpha_size��Ϊ0��ʾ��ͼ�����Ϻ�������alpha����,alpha_size����alpha���ϵĴ�С
															// ���alpha_sizeΪ0��ʾû�д�alpha����
	int	xoffset;
	int	yoffset;
	unsigned int width;					// ͼ��
	unsigned int height;				// ͼ��
	unsigned int staturated;		// �Ƿ��ñ���Ч��
};
#endif

// ????????
static SDL_Color FontPal[]={
    {255 , 255, 255, 255 }, // 0:?
    {0x00 ,0xff, 0xff, 255 }, // 1:?��
    {0xff ,0x00, 0xff, 255 }, // 2:?
    {0x00 ,0x00, 0xff, 255 }, // 3:��
    {0xff ,0xff, 0x00, 255 }, // 4:?
    {0x00 ,0xff, 0x00, 255 }, // 5:?
    {0xff ,0x00, 0x00, 255 }, // 6:��
    {0xa0 ,0xa0, 0xa4, 255 }, // 7:?��
    {0xa6 ,0xca, 0xf0, 255 }, // 8:�V?��
    {0xc0 ,0xdc, 0xc0, 255 }  // 9:�V??
};

/*
bool InitRealbinFileOpen( char *realbinfilename, char *addrbinfilename);	//��ʼ��realbin�����ϲ�������صļ�����
//void SetCurFrame();									//�ۼ�CurFrame��counter����ÿ��frameִ��һ��
void CacheRLU( int howlong);							//�ͷ�̫��ûʹ�õ�ͼ��������
bool realGetNo( unsigned long CharAction, unsigned long *GraphicNo);	//��char actionת��ͼ�����
unsigned long realGetNo2( unsigned long bnum);							//��bnumת��ͼ�����
bool realIsValid( unsigned long CharAction);					//���ͼ���Ƿ���Ч
unsigned long realGetBitmapNo( unsigned long num);						//��ͼ�����ת��bnum
bool realGetPos( unsigned long GraphicNo, short *x, short *y);	//ȡ��GraphicNo��x��y offset
bool realGetWH( unsigned long GraphicNo, short *w, short *h);	//ȡ��GraphicNo�Ŀ�͸�
bool realGetHitPoints( unsigned long GraphicNo, short *HitX, short *HitY);	//ȡ��GraphicNo��ʵ�ʿ�͸�
bool realGetPrioType( unsigned long GraphicNo , short *prioType);	//ȡ��GraphicNo������Ȩ����
bool realGetHitFlag( unsigned long GraphicNo, short *Hit);			//ȡ��GraphicNo��Hit
bool realGetHeightFlag( unsigned long GraphicNo, short *Height);	//ȡ��GraphicNo�Ƿ��и߶�
int realGetSoundEffect( unsigned long GraphicNo);					//ȡ��GraphicNo�Ļ�����
int realGetWalkSoundEffect( unsigned long GraphicNo);				//ȡ��GraphicNo�ĽŲ���
LPBYTE ReadRealBin( unsigned long GraphicNo);
//����realbin
bool DrawScaleImage( unsigned long GraphicNo, int screenx, int screeny, int screenw, int screenh, LPWORD dest,
					int destw, int desth, int pitch, LPWORD pal);
*/
#endif /*_LOADREALBIN_H_*/
