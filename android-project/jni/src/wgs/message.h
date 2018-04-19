/*-----------------------------------------------
   Message.h -- Message format definition
               (c) 许百胜Cary Hsu, 1999.8.18
	+----+------------+------------------+
	|name|Data len'\0'|       Data       |
	|1B  |  5 Byte    |       0~4090B    |
	+----+------------+------------------+
-----------------------------------------------*/
#ifndef __MESSAGE
#define __MESSAGE

//#include <windows.h>
// Message name
#define M_NULL '@'
#define M_ECHO 'A'
#define M_HELO 'H'	// H ID CodingData
#define M_HELO1 'I'	// H ID CodingData
#define M_HELO2 'J'	// H ID CodingData
#define M_REDI 'R'	// R 202.52.121.208:10000 pid
#define M_MESG 'M'
#define M_ERRO 'E'	// (E Error) or E ID
#define M_SKEY 'S'	// S 1234567
#define M_LOAD 'L'	// L 人数
#define M_ACKN 'K'	// K ID CodingData
#define M_DATA 'D'	// D ID SaveData
#define M_GOTO 'G'	// G 202.52.121.208
#define M_LINE 'N'  // N SubMessage
#define M_GAME '('	// M GameMessage
#define M_PKEY 'T'
#define M_CONT 'U'

// SubMessage
#define A_SEND		'T'	// A_SEND string \0
#define A_WHISPER	'W'	// A_WHISPER player \n string \0
#define A_QUERY		'P'	// A_QUERY PlayerName \0
#define A_NEW		'N'	// A_NEW description \0
#define A_QUIT		'Q'	// A_QUIT \0
#define A_KICK		'K'	// A_KICK \0
#define A_START		'S'	// A_START \0
#define A_GAMEOVER	'G'	// A_GAMEOVER PlayerName \n score \0
#define A_JOIN		'J'	// A_JOIN PlayerName \0
#define A_REFRESH	'R'	// A_REFRESH \0
//#define A_CLIENT	'C'	// A_CLIENT \0
#define A_ENTER		'E' // A_ENTER RoomName \0
#define A_EXIT		'X' // A_EXIT \0
#define A_ERROR		'Z'
// 
#define B_PLAYER	'y'	// B_PLAYER numPlayer \n state \n PlayerName \n ..... \0
#define B_DESK		'd'	// B_DESK numDesk \n state \n DealerName \n description \n ...... \0
#define B_ROOM		'r' // B_ROOM numRoom \n numUser \n Name \n ......\0
#define B_SEND		't'	// B_SEND PlayerName:string \0
#define B_WHISPER	'w'	// B_WHISPER PlayerName:string \0
#define B_QUERY		'p'	// B_QUERY string \0
#define B_NEW		'n'	// B_NEW Y \0  or B_NEW N cause \0
#define	B_QUIT		'q'	// B_QUIT \0
#define B_SOMEONE	'o'	// B_SOMEONE PlayerName \n IP \0
#define B_KICK		'k'	// B_KICK \0
#define B_START		's'	// B_START \0
#define B_GAMEOVER	'g'	// B_GAMEOVER numChart \n PlayerName \n score \n ...... \0
#define B_JOIN		'j'	// B_JOIN Y IP \0 or B_JOIN N cause \0
//#define B_CLIENT	'c'	// B_CLIENT \0
#define B_ENTER		'e' // B_ENTER Y \0 or B_ENTER N cause \0
#define B_EXIT		'x' // B_EXIT Y \0 or B_EXIT N cause \0
#define B_ERROR		'z'
// Game Message
#define S_NEWGAME	'N' // S_NEW parameter
#define S_GAMEOVER	'O'	// S_GAMEOVER
#define S_BORNDICE	'B'
#define S_WAVEFLAG	'W'	
#define S_RAISECOUNT 'R' //for RaiseCount
#define S_JiaYiu	'J' //forJiaYiuCount
#define S_CLIENTKEY	'C' //for ParamSav
#define S_SERVERKEY	'S'
#define S_WANTOVER	'A'
#define S_ECHO		'E'
#define S_TOKEN		'T'
// 
#define C_NEW		'n' // C_NEW
#define C_ECHO		'e'
#define C_CANSEND	'x'


//#ifdef _DEBUG
#define NO_WGS		//不使用WGS认证
//#endif

struct Parameter
{
	char		*lpstrParam;
	int			iLen;
	Parameter	*next;
	Parameter(){
		lpstrParam=NULL;
		next=NULL;}
};

struct MessNode
{
	int			nParam;
	Parameter	*param;
	MessNode	*next;
	MessNode(){
		param=NULL;
		next=NULL;}
};

enum
{
	WGS=1,
	CS,
	GS
};

//state of encrypt or decrypt
enum
{
	E_INIT,
	E_ECB,
	E_NO
};

#define FORMAT_APPEND		0x00
#define FORMAT_START		0x01
#define FORMAT_FINISH		0x02
#define FORMAT_ONLYONE		0x03

#ifdef _BACK_VERSION ////
#define SZ_ECB	"9304001" //产品序号
#else
#ifdef _PK_SERVER
#define SZ_ECB	"9306001"
#else
#define SZ_ECB	"8904001"
#endif
#endif

extern const unsigned long RET_OK;
extern const unsigned long RET_MEMORY;
extern const unsigned long RET_PARAM;

extern MessNode	*messNew;
extern MessNode	*messHead;
extern MessNode	*messTail;

extern unsigned long	dwServer;
extern int		iWGS;
extern char	szWGS[];
extern unsigned short		wWGS;
extern char	szCSIP[];
extern unsigned short		wCS;
extern char	szError[1024];

void AddParamForFormat(char* lpstr,int iLen,unsigned long flag);
void AddParamForFormat(char ch,unsigned long flag);
char* GetString(int &iLen,char* lpstrEncrypt);
unsigned long AnalyzeMessage(char* lpstr,int iStrLen);
Parameter *ReleaseMessHeadParam();
void RemoveMessHead();
void ReleaseMessList();
bool	GetIP_PORT(Parameter *param);
void	GetEncrypt(Parameter *param);
void	PKey_Init();
void	ReleaseSendData();
void	ipAddressAnalyze(  );

bool ReadWGSMessage(char* lpstr,int &iStrLen);
void DoHellow();
void DoSKey();
#ifdef _OMIT_WGS
void testtest();
LPTSTR O_WGS_Format();
#endif
unsigned long GetCafeNumber();

#endif