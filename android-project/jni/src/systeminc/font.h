/************************/
/*	font.h				*/
/************************/
#ifndef _FONT_H_
#define _FONT_H_

// ????????
#define FONT_SIZE 14
#define FONT_ZENKAU_WIDTH FONT_SIZE + 1
// �S?��??????????
#define	FONT_HANKAKU_WIDTH	FONT_SIZE / 2 + 1

// �u?��??????????
extern int FontZenkauWidth;
// �S?��??????????
extern int FontHankakuWidth;

/* ????����?�I??  fontPrio ?�k ****************************************/
enum{
	FONT_PRIO_BACK, 		/* ???���� 	*/
	FONT_PRIO_FRONT			/* �q?���� 	*/

#ifdef _TRADETALKWND				// (���ɿ�) Syu ADD ���������Ի����
	, FONT_PRIO_AFRONT
#endif
#ifdef _CHANNEL_MODIFY
	,FONT_PRIO_CHATBUFFER
#endif
};

// ??????????�B�l
typedef struct{
	short x, y;			// ����?��
	char color;			// ������
	char str[ 256 ]; 	// ��??
    char wstr[256];
	char	fontPrio; 	// ����??�I??
	int hitFlag;		// ???�R�e??????  ??��?  ???��? //Jerry �˴������bool�ᵼ���޷���ʾ�߿�
#ifdef _FONT_SIZE
	int size;
#endif
}FONT_BUFFER;

// ?????????
extern FONT_BUFFER FontBuffer[];

// ?????????
extern int FontCnt;
void FontPrint( char *moji, int x, int y, int dispPrio );

// ??????����?? ///////////////////////////////////////////////////////
void FontPrintDec( char *moji, int x, int y, int dispPrio );

/* ????�ѩ�????????? *********************************************/
int StockFontBuffer( int x, int y, char fontPrio, int color, char *str, int hitFlag );
#ifdef _FONT_SIZE
int StockFontBufferUtf(int x, int y, char fontPrio, int color, char *str, int hitFlag, int size);
int StockFontBufferExt( int x, int y, char fontPrio, int color, char *str, int hitFlag, int size );
//HFONT CreateNewFont( int size);
#endif
/* ????�ѩ�???????????�B�l�J??********************************/
void StockFontBuffer2( STR_BUFFER *strBuffer );

/* ONLINEGM ADD */
int StockFontBuffer3( STR_BUFFER *strBuffer );

#endif
