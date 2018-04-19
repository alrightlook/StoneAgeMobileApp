#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/unicodeToGB2312.h"
#include "../systeminc/char_conversion.h"

#define ASCII(a) a-'A'+10
#define ASCII_DEC(a) a-'0'+ 35

#define FONT_BUFFER_SIZE 1024 	// ???????????

//extern void GB2312ToUnicode_Str(u16 * const pOutUnicodeStr,u8 * const pGB2312Str);

// ?????????
FONT_BUFFER FontBuffer[ FONT_BUFFER_SIZE ];

// ?????????
int FontCnt = 0;
// �u?��??????????
int FontZenkauWidth;
// �S?��??????????
int	FontHankakuWidth;
/* ????�ѩ�????????? *********************************************/
#ifdef _FONT_SIZE

int StockFontBufferUtf(int x, int y, char fontPrio, int color, char *str, int hitFlag, int size)
{
	if( FontCnt >= FONT_BUFFER_SIZE ) return -2;
	
	// ??????????�ѩ�????
	FontBuffer[ FontCnt ].x = x;
	FontBuffer[ FontCnt ].y = y;
	FontBuffer[ FontCnt ].fontPrio = fontPrio;
	FontBuffer[ FontCnt ].color = color;
	FontBuffer[ FontCnt ].hitFlag = hitFlag;
	// ��?????
	strcpy( FontBuffer[ FontCnt ].str, str );
	strcpy( FontBuffer[FontCnt].wstr, str);
    //FontBuffer[FontCnt].wstr = GB2312ToUnicode((u16)  FontBuffer[FontCnt].str);
    //memset(FontBuffer[FontCnt].wstr, 0, 256);
    //gb2312ToUtf8(FontBuffer[FontCnt].wstr, 256, FontBuffer[FontCnt].str, strlen(str));

	//if( size < 0 || size > 3)	size = 0;
	FontBuffer[ FontCnt ].size = size;
	
	// ????????????
	//FontCnt++;
	return FontCnt++;
}
int StockFontBufferExt( int x, int y, char fontPrio, int color, char *str, int hitFlag, int size )
{
	// ??????????
	if( FontCnt >= FONT_BUFFER_SIZE ) return -2;
	
	// ??????????�ѩ�????
	FontBuffer[ FontCnt ].x = x;
	FontBuffer[ FontCnt ].y = y;
	FontBuffer[ FontCnt ].fontPrio = fontPrio;
	FontBuffer[ FontCnt ].color = color;
	FontBuffer[ FontCnt ].hitFlag = hitFlag;
	// ��?????
	strcpy( FontBuffer[ FontCnt ].str, str );
    //FontBuffer[FontCnt].wstr = GB2312ToUnicode((u16)  FontBuffer[FontCnt].str);
    memset(FontBuffer[FontCnt].wstr, 0, 256);
    gb2312ToUtf8(FontBuffer[FontCnt].wstr, 256, FontBuffer[FontCnt].str, strlen(str));

	//if( size < 0 || size > 3)	size = 0;
	FontBuffer[ FontCnt ].size = size;
	
	// ????????????
	//FontCnt++;
	return FontCnt++;
	
}
int StockFontBuffer( int x, int y, char fontPrio, int color, char *str, int hitFlag )
{
	return StockFontBufferExt( x, y, fontPrio, color, str, hitFlag, 0 );
}
#else
int StockFontBuffer( int x, int y, char fontPrio, int color, char *str, bool hitFlag )
{
	// ??????????
	if( FontCnt >= FONT_BUFFER_SIZE ) return -2;
	
	// ??????????�ѩ�????
	FontBuffer[ FontCnt ].x = x;
	FontBuffer[ FontCnt ].y = y;
	FontBuffer[ FontCnt ].fontPrio = fontPrio;
	FontBuffer[ FontCnt ].color = color;
	FontBuffer[ FontCnt ].hitFlag = hitFlag;
	// ��?????
	strcpy( FontBuffer[ FontCnt ].str, str );
	
	// ????????????
	//FontCnt++;
	return FontCnt++;
	
}
#endif
/* ????�ѩ�???????????�B�l�J??********************************/
void StockFontBuffer2( STR_BUFFER *strBuffer )
{
	int lineDist = 0; //�о�
	int splitPoint = 0;
	int bakSplitPoint,cursor;
	bool SetCursor=false;
	char splitStr[ 256 ];
	if( FontCnt >= FONT_BUFFER_SIZE ){ 
		strBuffer->hitFontNo = -2;
		return;
	}
	//��Ҫ����ʱ
	if( strBuffer->lineLen != 0 ){
		cursor=strBuffer->cursor;
		while( strlen( strBuffer->buffer + splitPoint ) >= strBuffer->lineLen ){
			bakSplitPoint=splitPoint;
			//copyһ�е��ִ�
			strncpy( splitStr, strBuffer->buffer + splitPoint, strBuffer->lineLen );
			*( splitStr + strBuffer->lineLen ) = NULL;
			if( GetStrLastByte( splitStr ) == 3 ){
				//�ָ��DBCSʱ���˻�һ��byte
				splitPoint = strBuffer->lineLen - 1 + splitPoint;
				*( splitStr + strBuffer->lineLen - 1 ) = NULL; 
			}else
				splitPoint = strBuffer->lineLen + splitPoint;
			StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, splitStr, 0 );
			if(cursor>=bakSplitPoint && cursor<splitPoint){
				strBuffer->imeX = strBuffer->x + (cursor-bakSplitPoint)*(FONT_SIZE>>1);
				strBuffer->imeY = strBuffer->y + lineDist;
				SetCursor=true;
			}
			lineDist += strBuffer->lineDist;
		}
		if(!SetCursor){
			strBuffer->imeX = strBuffer->x + (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
			strBuffer->imeY = strBuffer->y + lineDist;
		}
		StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0, strBuffer->buffer + splitPoint, 0 );
	}else{
		FontBuffer[ FontCnt ].x = strBuffer->x;
		FontBuffer[ FontCnt ].y = strBuffer->y;
		FontBuffer[ FontCnt ].fontPrio = strBuffer->fontPrio;
		FontBuffer[ FontCnt ].color = strBuffer->color;
		FontBuffer[ FontCnt ].hitFlag = 0;
#ifdef _FONT_SIZE
		FontBuffer[ FontCnt ].size = 0;
#endif
//����Hook_type���������ǺŲ���
#ifdef _SAHOOK //Syu ADD Hook��ʽ
		if( strBuffer->filterFlag == HOOK_TYPE){
			extern int HOOK_PASSWD_NUM;
			for(int i = 0 ; i < HOOK_PASSWD_NUM ; i++)
				FontBuffer[ FontCnt ].str[ i ] = '*';
			FontBuffer[ FontCnt].str[i]=NULL;
			strBuffer->cursor = HOOK_PASSWD_NUM;
		}
		else if( strBuffer->filterFlag == BLIND_TYPE ){
#else
		if( strBuffer->filterFlag == BLIND_TYPE ){
#endif
			int i;
			memset(FontBuffer[FontCnt].wstr, 0, 256);
			for(i = 0 ; i < strBuffer->cnt ; i++ )
            {
				FontBuffer[ FontCnt ].str[ i ] = '*';
                FontBuffer[ FontCnt ].wstr[ i ] = '*';
            }
			FontBuffer[ FontCnt ].str[ i ] = NULL;
		}else 
        {
            strcpy( FontBuffer[ FontCnt ].str, strBuffer->buffer );
            strcpy( FontBuffer[ FontCnt ].wstr, strBuffer->buffer );
            //memset(FontBuffer[FontCnt].wstr, 0, 260);
            //gb2312ToUtf8(FontBuffer[FontCnt].wstr, 260, FontBuffer[FontCnt].str, strlen(strBuffer->buffer));
        }
		strBuffer->imeX = strBuffer->x + strBuffer->cursor*(FONT_SIZE>>1);
		strBuffer->imeY = strBuffer->y;
		strBuffer->hitFontNo = FontCnt++;
	}
}
//ONLINEGM USE
int StockFontBuffer3( STR_BUFFER *strBuffer )
{
	int lineDist = 0; //�о�
	int splitPoint = 0;
	int bakSplitPoint,cursor;
	bool SetCursor=false;
	char splitStr[ 256 ];
	
	cursor=strBuffer->cursor;
	//��Ҫ����ʱ
	while( strlen( strBuffer->buffer + splitPoint ) >= (unsigned)strBuffer->lineLen-1 ){
		bakSplitPoint=splitPoint;
		strcpy( splitStr , strBuffer->buffer + splitPoint );  //һ����
		*( splitStr + strBuffer->lineLen ) = NULL;  //ָ�����
		splitPoint = strBuffer->lineLen + splitPoint;
		StockFontBuffer( strBuffer->x, strBuffer->y , strBuffer->fontPrio, 0 , splitStr, 0 );

		if(cursor>=bakSplitPoint && cursor<splitPoint){
			strBuffer->imeX = strBuffer->x + (cursor-bakSplitPoint)*(FONT_SIZE>>1);
			strBuffer->imeY = strBuffer->y + lineDist;
			SetCursor=true;
		}
		lineDist += strBuffer->lineDist;	
		return 1;
	}
	if(!SetCursor){  // �α겻�軻��
		strBuffer->imeX = strBuffer->x + (strBuffer->cursor-splitPoint)*(FONT_SIZE>>1);
		strBuffer->imeY = strBuffer->y + lineDist;
	}
	StockFontBuffer( strBuffer->x, strBuffer->y + lineDist, strBuffer->fontPrio, 0 , strBuffer->buffer + splitPoint, 0 );
	return 0;
}


