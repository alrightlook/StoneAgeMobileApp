/************************/
/*	chat.h				*/
/************************/
#ifndef _CHAT_H_
#define _CHAT_H_
//cary 256 -> 260
#define STR_BUFFER_SIZE 	260 		// ???????????
#ifdef _NEW_RESOMODE
	#define MAX_CHAT_LINE 		30 			// ������������
#else
	#define MAX_CHAT_LINE 		20 			// ������������
#endif
#define DEF_CHAT_LINE 		20 			// ??�@�e???????
#define MAX_CHAT_HISTORY 	64			// ???????????
#define DEF_VOICE 			3			// ?????��?��??
#define MAX_VOICE 			5			// ?????��??���k

#define NORMAL_TYPE		0
#define BLIND_TYPE		1
#define NUMBER_TYPE		2

#ifdef _SAHOOK //Syu ADD Hook��ʽ
#define HOOK_TYPE		3
#endif
// ��????????�B�l

typedef struct{
	char 	buffer[ STR_BUFFER_SIZE ]; // ??��?��???????
	unsigned char 	len;		// ��??��?
	unsigned char 	lineLen;	// ???��??��?
	unsigned char 	lineDist;	// ?????????????
	unsigned char 	cnt;		// ???��???
	unsigned char 	color;		// ��??��
	unsigned char	cursor;		//�α��λ��
	int	x, y;					// ��????��
	int	imeX, imeY;				// ???��???����?��?��
	int	fontPrio;				// ����??�I�T
	bool filterFlag;			// ��???????????????
	int hitFontNo;				// ����?��??�k?????????�R�e??
	
}STR_BUFFER;

// ��????????�B�l
typedef struct{
	char 	buffer[ STR_BUFFER_SIZE + 1 ]; // ??��?��???????
	unsigned char 	color;	// ��??��
	bool isUTF;
#ifdef _FONT_SIZE
	int fontsize;
#endif
}CHAT_BUFFER;

// ???????????�B�l
typedef struct{
	char str[ MAX_CHAT_HISTORY ][ STR_BUFFER_SIZE + 1 ];	// ??????????��??
	int newNo;												// ?��??????????�k?
	int nowNo;												// ????????????�k?
}CHAT_HISTORY;

// onlinegm���������¼   
typedef struct{
	char str[115][STR_BUFFER_SIZE + 1 ];
	int newNo;	  							
	int nowNo;
	int addNo;  
	int lockNo;
	bool color[115];
}INPUT_HISTORY;

// ???????????????????????
extern STR_BUFFER *pNowStrBuffer;

// ?????????
extern CHAT_BUFFER ChatBuffer[];
// ����?�V????????
extern STR_BUFFER MyChatBuffer;

// ??????????
extern int NowChatLine;

// ??????????
extern int NowMaxChatLine;

// ??��?��??
extern int NowMaxVoice;

/*	����һ��Ԫ������Ŀǰ������String buffer
parameter:	c:	��Ԫ					*/
void StockStrBufferChar(char c);

/*	��˫λԪ��Ԫ������Ŀǰ������String buffer
parameter:	lpc:	˫λԪ����Ԫ			*/
void StockStrBufferDBChar(char *lpc);

/* ?????????????????????? */
void ChatBufferToFontBuffer( void );
void ChatProc( void );
void StockChatBufferLine( char *str, unsigned char color );
#ifdef _FONT_SIZE
void StockChatBufferLineExt( char *str, unsigned char color, int fontsize, bool isutf = false);
#endif
void ClearChatBuffer( void );
int GetStrLastByte( char *str );
#ifdef HITFLAG_FONT
int GetStrWidth( char *str, int iSize = 14 );
#else
int GetStrWidth( char *str );
#endif
void GetKeyInputFocus( STR_BUFFER *pStrBuffer );

/* ????????? **********************************************************/
void InitChat( void );

/* ???????? ************************************************************/
void ChatProc( void );

// ?????????����?? **************************************************/
void FlashKeyboardCursor( void );

void KeyboardLeft();
void KeyboardRight();

/* ???????? ************************************************************/
void KeyboardReturn( void );

// ?????? ***************************************************************/
void KeyboardBackSpace( void );

// ???????��??��V ****************************************************************/
bool SaveChatHistoryStr( int no );
// ???????��??��??? ****************************************************************/
bool LoadChatHistoryStr( void );

bool LoadReadSayShield( void );
bool LoadReadNameShield( void );

bool CheckSay( const char* strSay, const char szReplace = '*' );
bool CheckName( const char* strSay/*, char* word*/ );
// ?????????�V????????? ********************************************/
void GetClipboad( void );
// ??????????????????��????? ***********************************/
void SetClipboad( void );
int GetCharByte( char c );

#ifdef _FRIENDCHANNEL			//ROG ADD ����Ƶ��
void StrToNowStrBuffer( char *str );
#endif




#endif
