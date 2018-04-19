#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
#include <SDL_mixer.h>
//#include "../caryIme.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/pc.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/netmain.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/savedata.h"
#include "../systeminc/t_music.h"
#include "../systeminc/menu.h"
#include "../systeminc/tool.h"
#include "../systeminc/map.h"
#include "../systeminc/field.h"
#include "../systeminc/pet_skillinfo.h"
#include "../wgs/descrypt.h"
#include "../systeminc/character.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif

#ifdef _NEWPANEL //Syu ADD 7.0 ????????????

#ifdef _SYUTEST3
#define MENU_STATUS_0   25
#else
#define MENU_STATUS_0   17
#endif
#else
#define MENU_STATUS_0	12
#endif
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
#define MENU_PET_0		28
#else
#define MENU_PET_0		28
#endif
#define MENU_ITEM_0		40
#define MENU_MAIL_0		40
#define MENU_ALBUM_0	20
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
#define MENU_TRADE_0	22
#endif
#define MENU_BANK_0		20
#define MENU_BTL_RESULT_0	1
//Terry add 2003/11/19
extern bool g_bTradesystemOpen;
//end
#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif
// ????
#define MAX_CHAT_REGISTY_STR		8		// ?????????K???
#define MAX_CHAT_REGISTY_STR_LEN	26 		// ?????????K??????


bool mergeItemFlag = false;
int lastChoosedItem = -1;
int lastClickTime = 0;
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
bool DropPetWndflag = false ;			//????????????????????
short DropI = -1;						//???i?
#endif

#ifdef _AniCrossFrame	  // Syu ADD ???????��?????????
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
int UpDownflag = 0 ;
#endif

#define CAHT_REGISTY_STR_FILE_NAME 	"/sdcard/jerrysa/data/chatreg.dat" 	// ??????K??????V??????
static int systemWndFontNo[ MENU_SYSTEM_0 ]; 			// ????R?e?k?

unsigned int  systemWndNo;						// ??????k?
STR_BUFFER chatRegistryStr[ MAX_CHAT_REGISTY_STR ];		// ?????????K???V??????
int MouseCursorFlag = false;							// ???????

#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
char talkmsg[4][256];					//???��????Buffer
int talkwndx = 300 , talkwndy = 350 ;   //???��??
bool talkwndflag = false ;				//????????????
bool tradetalkwndflag = false ;			//??????????????
#endif

#ifdef _MONEYINPUT //Syu ADD ???????????
STR_BUFFER MymoneyBuffer;
bool Moneyflag = false;
#endif
STR_BUFFER TradeBuffer;
bool Tradeflag = false;    //????��??Focus???
bool TradeBtnflag = false; //??????????????
// ???
int mapWndFontNo[ MENU_MAP_0 ]; 	// ????R?e?k?
static unsigned int  mapWndNo;		// ??????k?
int MapWmdFlagBak;					// ?P?f?????h?????????????????

// ?????
static int statusWndFontNo[ MENU_STATUS_0 ]; 	// ????R?e?k?
static int statusWndBtnFlag[ MENU_STATUS_0 ];	// ?????????
static unsigned int statusWndNo;				// ??????k?
STR_BUFFER shougouChange;						// ??????????????
int StatusUpPoint;								// ????????????

#ifdef _TRADESYSTEM2
static int tradeWndFontNo[ MENU_TRADE_0 ]; 	// ????R?e?k?
static int tradeWndBtnFlag[ MENU_TRADE_0 ];	// ?????????
#endif
static unsigned int tradeWndNo;				// ??????k?

int showindex[7] ={0,0,0,0,0,0,0};

static char opp_sockfd[128] = "-1";
static char opp_name[128] = "";
static char trade_command[128];
static char trade_kind[128];
static int opp_showindex;
static char opp_goldmount[1024];
static char opp_itemgraph[1024];
static char opp_itemname[1024];
static char opp_itemeffect[1024];
static char opp_itemindex[1024];
static char opp_itemdamage[1024];

struct showitem {
	char name[128];
	char freename[256];
	char graph[128];
	char effect[1024];
	char color[128];
	char itemindex[128];
	char damage[128];
};
static int tradePetIndex = -1;
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
//??????????????????????????????????????????????
//????????????????????????????????????????????????

static showitem opp_item[15];	//??????????????15??
struct showpet {
	char opp_petname[128];
	char opp_petfreename[128];
	char opp_petgrano[128];
	char opp_petlevel[128];
	char opp_petatk[128];
	char opp_petdef[128];
	char opp_petquick[128];
	char opp_petindex[128];
	char opp_pettrans[128];
	char opp_petshowhp[128];
	char opp_petslot[128];
	char opp_petskill1[128];
	char opp_petskill2[128];
	char opp_petskill3[128];
	char opp_petskill4[128];
	char opp_petskill5[128];
	char opp_petskill6[128];
	char opp_petskill7[128];
#ifdef _SHOW_FUSION
	char opp_fusion[64];
#endif
#ifdef _PET_ITEM
	PetItemInfo oPetItemInfo[MAX_PET_ITEM];			// ????????????
#endif
};											//?????????????????
static showpet opp_pet[5];

int itemflag[15];

static ACTION *SecondActPet;				//??????????Action
bool MainTradeWndflag = true ;				//????????????flag
bool SecondTradeWndflag = false ;			//?????????????flag
ACTION *SecondTradeWnd;						//??????
ACTION *TradeTalkWnd;						//??????
int ShowPetNum = 0 ;						//?????????????????
static int SecondtradeWndFontNo[ 6 ];		//??????????
int mytradelist[21] = { -1 } ;				//??????????????��
int opptradelist[21] = { -1 } ;				//??????????????��
int drag1Y = 67 , drag2Y = 257 ;			//???????��??
bool dragflag1 = false , dragflag2 = false ; //????????????
int locknum = -1 , locknum2 = -1 ;
#endif
static PET tradePet[2];
static char tradepetindexget[128] = "-1";
#ifdef _PET_ITEM
static bool g_bPetItemWndFlag = false;
#endif

static ACTION *pActPet3; //?????????????Pet Action
static ACTION *pActPet4;
static ACTION *pActPet5;

struct tradelist {
	char kind;
	int data;
	char name[256];
	char damage[256];
	char freename[256];
	int level;
	int trns;
#ifdef _ITEM_PILENUMS
	int pilenum;
#endif
#ifdef _SHOW_FUSION
	int fusion;
#endif
};
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
static tradelist tradeList[43];
#endif
static int mine_itemindex[2] = { -1, -1};
static char mine_itemname[2][128];

static int bankGold;
static int bankWndFontNo[ MENU_TRADE_0 ]; 	// ????R?e?k?
static int bankWndBtnFlag[ MENU_TRADE_0 ];	// ?????????
static int cashGold = 0;
static int totalGold = 0;
static int bankGoldInc = 0;
static int bankGoldCnt =0;

#ifdef _TELLCHANNEL				//ROG ADD ???????
bool MultiTells = false;
ACTION *pActMsgWnd;
int CharNum = 0;
char TellInfo[10][128];
char name[10][32];
int menuIndex[10];
char reSendMsg[STR_BUFFER_SIZE];
int  TalkMode = 0;						//0:??? 1:???? 2: ???? 3:???? 4:??
char secretName[32] = {""};				//???????????
#endif

#ifdef _FRIENDCHANNEL				//ROG ADD ???????
typedef struct{
	char	roomNo[4];
	char	chiefName[CHAR_NAME_LEN+1];
	int		chiefIndex;
	int		chiefFlag;
	int		memberNum;
	char    memberName[MAX_ROOM_NUM][CHAR_NAME_LEN+1];
	int     memberIndex[MAX_ROOM_NUM];
	char    roomName[32];
	char    nickName[MAX_ROOM_NUM][CHAR_FREENAME_LEN+1];
}CHATINFO;
CHATINFO chatInfo;

bool secretFlag = false;				//???????
bool BtnType = false;					//????????
bool setRoomFlag = false;
bool assentFlag = false;				//?????????
bool scrollFlag = false;
char roomInfo[MAX_ROOM_NUM][64];		//???????????
char memInfo[64];						//member information
int  scrlBtnIndex = 0;					//scroll��??
int  BtnNo = 0;							//??????
int  firMemNo = 0;						//????????
int  selChar = -1;						//?????????
int  closeBtn ,leaveBtn ,delBtn,chaBtn,outBtn,scrlHBtn,scrlLBtn; //??????
int  roomIndex[MAX_ROOM_NUM];
int roomNum = 0;
int memIndex = 0;						//member index
static int ChatRoomBtn[ 16 ];			//????????
STR_BUFFER chatRoomName;				//?څ????????????
ACTION *pSetRoomWnd;					//?څ????????????
ACTION *pAssentWnd;						//????????
ACTION *pChtChanlWnd;					//???????????
ACTION *pSelChanlWnd;					//??????????????
#endif

#ifdef _TIMEBAR_FUNCTION			//???bar???
ACTION *pTimeBarWnd;
char   timeBarTitle[32];
int    timeBarRange;
int    timeBarCurPos;
bool   timeBarFlag = false;
bool   barHolder[2];                //?????��??????
void DrawTimeBar();
/////////TEST/////////////////
int StartTime = 0;
int timBarIdent = -1;
//////////////////////////////
#endif

// ???
static int petWndFontNo[ MENU_PET_0 ]; 	// ????R?e?k?
static int petWndBtnFlag[ MENU_PET_0 ]; // ?????????
static unsigned int  petWndNo;			// ?????????k?
int  petStatusNo;				// ???????????????????????k?
int  mixPetNo;					// ???????????k?
static ACTION *pActPet;					// ?????????????
bool BattlePetReceiveFlag;				// ??P?????????????????
#ifdef _STANDBYPET
bool StandbyPetSendFlag = false;
#endif
STR_BUFFER petNameChange;				// ???????q????????
int BattlePetReceivePetNo = -1;			// ?????????????k?
int SelectWazaNo;						// ?k??????k?

// ????
static int itemWndFontNo[ MENU_ITEM_0 ]; 	// ????R?e?k?
static int itemWndBtnFlag[ MENU_ITEM_0 ];	// ?????????
static unsigned int  itemWndNo;
static int  jujutuNo = -1;					// ?k???????????k?
static int  itemNo = -1;					// ?k????????????k?
static int  itemWndDropGold;				// ??????
static int  itemWndDropGoldInc;				// ?????????
static int  itemWndDropGoldCnt;				// ??????????????

static int  tradeWndDropGold = 0;				// trade??????
static int  tradeWndDropGoldInc = 0;			// trade?????????
static int  tradeWndDropGoldCnt =0;			// trade??????????????
static int  tradeWndDropGoldSend =0;
static int  tradeWndDropGoldGet =0;
#ifdef _PET_ITEM
static int	nSelectPet;						// ?????????????????????
#endif

static ACTION *pActPet2;					// ????????????????
bool ItemMixRecvFlag;						// ??????????
static int ItemMixPetNo = -1;				// ????????????k?
static unsigned int ItemUseTime = 0;		// ?????????????

// ?????????
ITEM_BUFFER ItemBuffer[ MAX_ITEM ];

#ifdef _PET_ITEM
// ????????????????????????,???????????????????
int nPetItemEquipBmpNumber[PET_EQUIPNUM][2] =
{
	{26463, 26470}, {26460, 26467}, {26458, 26465}, {26461, 26468}, {26459, 26466},
	{26457, 26464}, {26462, 26469}
};
#endif
// ???
#define MAIL_HISTORY_FILE_NAME 	"/sdcard/jerrysa/data/mail.dat" 	// ????V??????
#ifdef _TRANS_6
char *TransmigrationStr[ 7 ] = { "","?","??","??","??","??","?" };
#else
char *TransmigrationStr[ 6 ] = { "","?","??","??","??","??" };	//?????????// ??
#endif
static int mailWndFontNo[ MENU_MAIL_0 ];
static int mailWndBtnFlag[ MENU_MAIL_0 ];			// ????????��
static MAIL_WND_TYPE mailWndNo;						// ?F??��?`????????????
static MAIL_WND_TYPE mailWndNoBak;					// ??��?`????????????
static int mailViewWndPageNo;						// ??????E?????????��?`??????
static int mailSendWndPageNo;						// ???????????????��?`??????
static int mailPetSendWndPageNo;					// ????????????????��?`??????
static int mailItemNo;								// ????????????????��????????
static int mailWndSendFlag[ MAX_ADR_BOOK ]; 		// ?????????????��
int mailHistoryWndPageNo;							// ??s?????????��?`??????
int mailHistoryWndSelectNo;							// ?x?k???????????????
static int mailHistoryWndNowPageNo;					// ?g?H??????????s????
STR_BUFFER MailStr;									// ???????
MAIL_HISTORY MailHistory[ MAX_ADR_BOOK ];			// ??`?????s??????
ACTION *pActLetter[ 4 ];							// ????????`???????????????
ACTION *pActMailItem;								// ?????`????????????????


int petWndCalcGrowUp;

// ????
#define ALBUM_FILE_NAME 	"/sdcard/jerrysa/data/album.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_4 	"/sdcard/jerrysa/data/album_4.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_5 	"/sdcard/jerrysa/data/album_5.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_6 	"/sdcard/jerrysa/data/album_6.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_7   "/sdcard/jerrysa/data/album_7.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_8   "/sdcard/jerrysa/data/album_8.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_9   "/sdcard/jerrysa/data/album_9.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_10  "/sdcard/jerrysa/data/album_10.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_11  "/sdcard/jerrysa/data/album_11.dat" 	// ??????V??????
#define ALBUM_FILE_NAME_12  "/sdcard/jerrysa/data/album_12.dat"
#define ALBUM_FILE_NAME_13	"/sdcard/jerrysa/data/album_13.dat"
#define ALBUM_FILE_NAME_14	"/sdcard/jerrysa/data/album_14.dat"
#define ALBUM_FILE_NAME_15  "/sdcard/jerrysa/data/album_14.dat"
#define ALBUM_FILE_NAME_16  "/sdcard/jerrysa/data/album_15.dat"
#define ALBUM_FILE_NAME_17  "/sdcard/jerrysa/data/album_16.dat"
#define ALBUM_FILE_NAME_18  "/sdcard/jerrysa/data/album_17.dat"
#define ALBUM_FILE_NAME_19  "/sdcard/jerrysa/data/album_18.dat"
#define ALBUM_FILE_NAME_20  "/sdcard/jerrysa/data/album_19.dat"
#define ALBUM_FILE_NAME_21  "/sdcard/jerrysa/data/album_20.dat"
#define ALBUM_FILE_NAME_22  "/sdcard/jerrysa/data/album_21.dat"
#define ALBUM_FILE_NAME_23  "/sdcard/jerrysa/data/album_22.dat"
#define ALBUM_FILE_NAME_24  "/sdcard/jerrysa/data/album_23.dat"
#define ALBUM_FILE_NAME_25  "/sdcard/jerrysa/data/album_24.dat"
#define ALBUM_FILE_NAME_26  "/sdcard/jerrysa/data/album_25.dat"
#define ALBUM_FILE_NAME_27  "/sdcard/jerrysa/data/album_26.dat"
#define ALBUM_FILE_NAME_28  "/sdcard/jerrysa/data/album_27.dat"
#define ALBUM_FILE_NAME_29  "/sdcard/jerrysa/data/album_28.dat"
#define ALBUM_FILE_NAME_30  "/sdcard/jerrysa/data/album_29.dat"
#define ALBUM_FILE_NAME_31  "/sdcard/jerrysa/data/album_30.dat"
#define ALBUM_FILE_NAME_32  "/sdcard/jerrysa/data/album_31.dat"
#define ALBUM_FILE_NAME_33  "/sdcard/jerrysa/data/album_32.dat"
#define ALBUM_FILE_NAME_34  "/sdcard/jerrysa/data/album_33.dat"
#define ALBUM_FILE_NAME_35  "/sdcard/jerrysa/data/album_34.dat"
#define ALBUM_FILE_NAME_36  "/sdcard/jerrysa/data/album_35.dat"
#define ALBUM_FILE_NAME_37  "/sdcard/jerrysa/data/album_36.dat"
#define ALBUM_FILE_NAME_38  "/sdcard/jerrysa/data/album_37.dat"
#define ALBUM_FILE_NAME_39  "/sdcard/jerrysa/data/album_38.dat"
#define ALBUM_FILE_NAME_40  "/sdcard/jerrysa/data/album_39.dat"
#define ALBUM_FILE_NAME_41  "/sdcard/jerrysa/data/album_40.dat"
#define ALBUM_FILE_NAME_42  "/sdcard/jerrysa/data/album_41.dat"
#define ALBUM_FILE_NAME_43  "/sdcard/jerrysa/data/album_42.dat"
#define ALBUM_FILE_NAME_44  "/sdcard/jerrysa/data/album_43.dat"
#define ALBUM_FILE_NAME_45  "/sdcard/jerrysa/data/album_44.dat"
#define ALBUM_FILE_NAME_46  "/sdcard/jerrysa/data/album_45.dat"
#define ALBUM_FILE_NAME_47  "/sdcard/jerrysa/data/album_46.dat"
#define ALBUM_FILE_NAME_48  "/sdcard/jerrysa/data/album_47.dat"

static int albumWndFontNo[ MENU_ALBUM_0 ];
static int albumWndBtnFlag[ MENU_ALBUM_0 ];
static unsigned int albumWndNo;
static int albumWndPageNo;					// ??`??????
static int albumNo;						// ????��???
// ????��???????`????`???????
PET_ALBUM_TBL PetAlbumTbl[] = {
	#include "../systeminc/petName.h"	// ???????????
};
PET_ALBUM PetAlbum[ MAX_PET_KIND ];		// ????��?????
int AlbumIdCnt = 0;					// ???��???��????????i???z???????????
// ?????????`???
int IdEncryptionTbl[ 16 ] = { 48, 158, 98, 23, 134, 29, 92, 67,
								70, 28, 235, 20, 189, 48, 57, 125 };

// ???L?Y??????????
int resultWndFontNo[ MENU_BTL_RESULT_0 ];	// ???????��?????
int ResultWndTimer;							// ????????��r?g??????`

// ?????
#define TASK_BAR	7
#define TASK_BAR_X 320
#define TASK_BAR_Y 468 + DISPLACEMENT_Y
bool TaskBarFlag = false;	// ???????��`??��
// Terry add 2003/12/16 for ????????????,??????????????????
bool bShowItemExplain = false;
// end
static int taskBarFontNo[ TASK_BAR ];
static int taskBarX = TASK_BAR_X, taskBarY = TASK_BAR_Y + 24;

// ??????
unsigned int MenuToggleFlag;
#ifdef _NEWREQUESTPROTOCOL			// (?????) Syu ADD ????Protocol??????
#define CHAR_MAX_DETAIL 8
char CharDetail[CHAR_MAX_DETAIL][16] ;
char DetailDesc[CHAR_MAX_DETAIL][64] = {
	"��ħ������ :" ,
	"ˮħ������ :" ,
	"��ħ������ :" ,
	"��ħ������ :" ,
	"��ħ�������� :" ,
	"ˮħ�������� :" ,
	"��ħ�������� :" ,
	"��ħ�������� :"
};
#endif

#ifdef _ALCHEPLUS
int iCharAlchePlus[25];
char sAlchePlus_list[25][16] =
{
	"ʯ", "ľ",	"��", "��",	"Ƥ",
	"����", "��", "צ",	"��", "Ҷ",
	"��", "ճ��", "��", "��", "ʯ��",
	"����",	"����",	"˯��",	"�ظ�",	"����",
	"ˮ��",	"��", "ˮ",	"��", "��"
};

int iAlchePlusIcon[25] = {  26536, 26529, 26545, 26534, 26535,	/*"?", "?",	"??", "??",	"?"*/
							26540, 26548, 26533, 26541, 26549,	/*"????", "??", "?",	"??", "?"*/
							26551, 26552, 26553, 26542, 26537,	/*"??", "???", "??", "??", "???"*/
							26546, 26544, 26550, 26538, 26547,	/*"????",	"????",	"???",	"???",	"????"*/
							26531, 26539, 26530, 26532, 26543};	/*"???",	"??", "?",	"??", "??"*/
#endif

// ??????????????
ACTION *pActMenuWnd;
ACTION *pActMenuWnd2;
ACTION *pActMenuWnd3;
ACTION *pActMenuWnd4;
ACTION *pActYesNoWnd;
// Bankman
ACTION *pActMenuWnd5;

void checkRidePet( int );
//andy_add 2002/06/24
int RIDEPET_getNOindex( int baseNo);
int RIDEPET_getPETindex( int PetNo, int learnCode);
int RIDEPET_getRIDEno( int index, int ti);


static char *monoStereoStr[] = { 	"       ������       ",
									"       ������       "};

static char *mouseCursor[] = { 	"     ��  ��     ",
								"     ƽ  ��     "};

#define WINDOW_CREATE_FRAME 10	// ??????????????????

// ??????????????????
extern unsigned char crs_change_tbl[];
// ??????????????????
extern unsigned char crs_change_tbl2[];
// ?T??????
extern unsigned char crs_bound_tbl[][32];

#ifdef _STREET_VENDOR
extern short sStreetVendorBuyBtn;
#endif

#ifdef _PET_ITEM
/*	0x01:PET_HEAD	// ?
	0x02:PET_WING	// ??
	0x04:PET_TOOTH	// ??
	0x08:PET_PLATE	// ????
	0x10:PET_BACK	// ??
	0x20:PET_CLAW	// ?
	0x40:PET_FOOT	// ??(??)
*/
// ????????????????????????��
unsigned char byShowPetItemBackground[MAX_PET_SPECIES + 1] =
{
/*	????	????	?????	????	????	????	????	?????	??????	????*/
	0x5d,	0x5d,	0x7d,	0x5d,	0x5d,	0x7d,	0x5f,	0x5d,	0x5f,	0x7f,
/*	?????	???	????	??????	??????	????	????	??????	????	????*/
	0x5d,	0x5d,	0x5d,	0x5d,	0x5d,	0x7d,	0x7d,	0x5d,	0x5d,	0x7d,
/*	???	????	???	???	?????	???	???	?????	?????	?????*/
	0x5d,	0x7d,	0x7d,	0x5d,	0x5d,	0x5f,	0x5d,	0x5d,	0x09,	0x1d,
/*	��??	????	???	????	?��	????	??	????	??????	?????*/
	0x5d,	0x79,	0x1d,	0x5d,	0x79,	0x5d,	0x5d,	0x5d,	0x5d,	0x5d,
/*	?????	?????	????*/
	0x5d,	0x5d,	0x00
};
#endif

int charDetailPage =3;


#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
bool SkillWndflag = false ;
short SkillWnd = 0 ;
ACTION *pActSkillMenuWnd;
void SkillWndfunc2() {
	int x =0, y=0, j=0 ;
	char buf[256];
	char msg[256];
	char msg2[256];
	if( pActSkillMenuWnd == NULL ) {
			pActSkillMenuWnd = MakeWindowDisp( 354 , 0, 300, 456, 0, -1 );
	}else{
		if( pActSkillMenuWnd->hp > 0 ){
			StockDispBuffer( ( ( WINDOW_DISP *)pActSkillMenuWnd->pYobi )->mx - 10, ( ( WINDOW_DISP *)pActSkillMenuWnd->pYobi )->my - 5 , DISP_PRIO_MENU, CG_FIELD_SKILL_PANEL, 1 );
			x = pActSkillMenuWnd->x + 2;
			y = pActSkillMenuWnd->y - 5;
			statusWndFontNo[ 15 ] = StockDispBuffer( x + 201, y + 442, DISP_PRIO_IME3, CG_CLOSE_BTN , 2 )  ;
			StockFontBuffer( x + 58 , y + 40  , FONT_PRIO_FRONT, 1 , pc.profession_class_name , 0 );
			sprintf( msg , "%d" , pc.profession_skill_point ) ;
			StockFontBuffer( x + 228 , y + 40  , FONT_PRIO_FRONT, 1 , msg , 0 );
			memset( msg, -1, sizeof( msg ) );
		}
	}
	for ( int i = 0 ; i < 3 ; i ++ ) {
#ifdef _READ16BITBMP
		if(g_bUseAlpha){
			if( MakeHitBox( x + 254 , y + 15 + i * 78, x + 284  , y + 93 + i * 78 , DISP_PRIO_IME3 ) == true ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					SkillWnd = i ;
					play_se( 217, 320, 240 );
					break;
				}
			}
		}
		else
#endif
		if( MakeHitBox( x + 250 , y + 18 + i * 79, x + 280  , y + 96 + i * 79 , DISP_PRIO_IME3 ) == true ) {
			if( mouse.onceState & MOUSE_LEFT_CRICK ) {
				SkillWnd = i ;
				play_se( 217, 320, 240 );
				break;
			}
		}
	}
	if( mouse.onceState & MOUSE_LEFT_CRICK ) {
		if( HitDispNo == statusWndFontNo[ 15 ] ) {
			SkillWndflag = false ;
			play_se( 217, 320, 240 );
		}
	}
	extern int BattleSkill[20];
	extern int AssitSkill[20];
	extern int AdvanceSkill[20];
	for ( int i = 0 ; i < 4 ; i ++ ) {
		for ( j = 0 ; j < 4 ; j ++ ) {
			StockDispBuffer( x + 143 + j * 60 , y + 210 + i * 57 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 0 );
		}
	}
	sprintf( msg , " ");
	switch ( SkillWnd ) {
	case 0 :
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
		else
#endif
		StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( j = 0 ; j < 4 ; j ++ ) {
				if ( AssitSkill [ j + i * 4 ] == -1 )
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AssitSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;
					}

					sprintf ( msg , "%s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%d%", profession_skill[AssitSkill [ j + i * 4 ]].skill_level ) ;
					StockFontBuffer( 440, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf ( msg , "(�ķ�MP:%d)" , profession_skill[AssitSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%s" , profession_skill[AssitSkill [ j + i * 4 ]].memo ) ;
#ifdef _OUTOFBATTLESKILL			// (?????) Syu ADD ??????????Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag == 0 ) {
						if( pc.mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ) {
							lssproto_BATTLESKILL_send ( sockfd , AssitSkill [ j + i * 4 ] ) ;
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf( msg2 , "%8s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag + 1 , msg2 , 2 );
			}
		}
		break;
	case 1 :
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
		else
#endif
		StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( j = 0 ; j < 4 ; j ++ ) {
				if ( BattleSkill [ j + i * 4 ] == -1 )
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ BattleSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;
					}

					sprintf ( msg , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%d%", profession_skill[BattleSkill [ j + i * 4 ]].skill_level ) ;
					StockFontBuffer( 440, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf ( msg , "(�ķ�MP:%d)" , profession_skill[BattleSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].memo ) ;
#ifdef _OUTOFBATTLESKILL			// (?????) Syu ADD ??????????Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[ BattleSkill [ j + i * 4 ]].useFlag == 0 ) {
						if( pc.mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0 ) {
							lssproto_BATTLESKILL_send ( sockfd , BattleSkill [ j + i * 4 ] ) ;
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf( msg2 , "%8s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[ BattleSkill [ j + i * 4 ]].useFlag + 1 , msg2 , 0 );
			}
		}
		break;
	case 2:
#ifdef _READ16BITBMP
		if(g_bUseAlpha) StockDispBuffer( x + 147, y + 225, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
		else
#endif
		StockDispBuffer( x + 138, y + 228, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
		for ( int i = 0 ; i < 4 ; i ++ ) {
			for ( j = 0 ; j < 4 ; j ++ ) {
				if ( AdvanceSkill [ j + i * 4 ] == -1 )
					break;
				if ( HitDispNo == StockDispBuffer( x + 29 + j * 60 , y + 102 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AdvanceSkill [ j + i * 4 ] ].icon  , 2 ) ) {

					int use_color = 0;
					if( pc.mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 ){
						use_color = FONT_PAL_WHITE;
					}else{
						use_color = FONT_PAL_GRAY;
					}

					sprintf ( msg , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
					StockFontBuffer( 355, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%d%", profession_skill[AdvanceSkill [ j + i * 4 ]].skill_level ) ;
					StockFontBuffer( 440, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf ( msg , "(�ķ�MP:%d)" , profession_skill[AdvanceSkill [ j + i * 4 ]].costmp );
					StockFontBuffer( 500, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

					sprintf( msg , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].memo ) ;
#ifdef _OUTOFBATTLESKILL			// (?????) Syu ADD ??????????Protocol
					if ( mouse.onceState & MOUSE_LEFT_CRICK && profession_skill[AdvanceSkill [ j + i * 4 ]].useFlag == 0 ) {
						if( pc.mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 ) {
							lssproto_BATTLESKILL_send ( sockfd , AdvanceSkill [ j + i * 4 ] ) ;
							play_se( 217, 320, 240 );
						}
					}
#endif
				}
				sprintf( msg2 , "%8s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
				StockFontBuffer(x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, profession_skill[AdvanceSkill [ j + i * 4 ]].useFlag + 1 , msg2 , 0 );
			}
		}
		break;
	}
	char *splitPoint =  msg  ;
	sprintf( buf , "%s" , msg ) ;
	x = 355 ;
	y = 340 ;
	while( 1 ){
		if( strlen( splitPoint ) > 34 ) {
			strncpy( msg, splitPoint, 34 );
			buf[ 34 ] = NULL;
			if( GetStrLastByte( buf ) == 3 ){
				buf[ 33 ] = NULL;
				splitPoint += 33;
			}else{
				buf[ 34 ] = NULL;
				splitPoint += 34;
			}
			StockFontBuffer( x, y , FONT_PRIO_FRONT, 0, buf, 0 ); y += 24;
		}else{
			strcpy( buf, splitPoint );
			StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, buf, 0 );
			break;
		}
	}
}
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
void TradeTalk ( char *msg ) {
	char buf[256];
	if( pActMenuWnd4 != NULL || SecondTradeWnd != NULL ) {
		sprintf ( buf , "%s" , msg ) ;
		//?��????????????????
		if ( strstr ( buf , pc.name ) || strstr( buf , opp_name )) {
			//???????
			while( 1 ){
				if( strlen( msg ) > 44 ) {
					strncpy( buf, msg , 44 );
					buf[ 44 ] = NULL;
					if( GetStrLastByte( buf ) == 3 ){
						buf[ 43 ] = NULL;
						msg += 43;
					}else{
						buf[ 44 ] = NULL;
						msg += 44;
					}
					for ( int i = 0 ; i < 3 ; i ++ )
						strcpy ( talkmsg[i] , talkmsg[i+1] ) ;
					sprintf ( talkmsg[3] , "%s" , buf );
				}else{
					for ( int i = 0 ; i < 3 ; i ++ )
						strcpy ( talkmsg[i] , talkmsg[i+1] ) ;
					strcpy ( buf , msg ) ;
					sprintf( talkmsg[3] , "%s" , buf ) ;
					break;
				}
			}
			tradetalkwndflag = true ;
		}
	}
}
#endif
void LockAndOkfunction ( )
{
	char buffer[1024];
	char myitembuff[1024];
	char mypetbuff[1024];
	char oppitembuff[1024];
	char opppetbuff[1024];
	// ???????????????
	if( pc.trade_confirm == 4) {
		// ???????????
		if( ( MainTradeWndflag == true && HitDispNo == tradeWndFontNo[ 0 ] ) ||
			//andy_reEdit 2003/04/27
			(  SecondTradeWndflag == true && HitDispNo == SecondtradeWndFontNo[ 2 ] ) ){
			// ????????????????buffer
			sprintf( myitembuff, "T|%s|%s|K|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|"
				, opp_sockfd, opp_name
				, tradeList[1].data, tradeList[2].data
				, tradeList[3].data, tradeList[4].data
				, tradeList[5].data, tradeList[6].data
				, tradeList[7].data, tradeList[8].data
				, tradeList[9].data, tradeList[10].data
				, tradeList[11].data, tradeList[12].data
				, tradeList[13].data, tradeList[14].data
				, tradeList[15].data );
			sprintf( mypetbuff , "P|%d|P|%d|P|%d|P|%d|P|%d|G|%d|"
				, tradeList[16].data, tradeList[17].data
				, tradeList[18].data, tradeList[19].data
				, tradeList[20].data, tradeList[21].data );
			sprintf( oppitembuff, "I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|I|%d|"
				, tradeList[22].data, tradeList[23].data
				, tradeList[24].data, tradeList[25].data
				, tradeList[26].data, tradeList[27].data
				, tradeList[28].data, tradeList[29].data
				, tradeList[30].data, tradeList[31].data
				, tradeList[32].data, tradeList[33].data
				, tradeList[34].data, tradeList[35].data
				, tradeList[36].data );
			sprintf( opppetbuff , "P|%d|P|%d|P|%d|P|%d|P|%d|G|%d|"
				, tradeList[37].data, tradeList[38].data
				, tradeList[39].data, tradeList[40].data
				, tradeList[41].data, tradeList[42].data );
			sprintf( buffer , "%s%s%s%s" , myitembuff , mypetbuff , oppitembuff , opppetbuff ) ;
			lssproto_TD_send( sockfd, buffer );
			tradeStatus = 2;
		}
	}
	// ???????Lock??????????Lock??
	if( pc.trade_confirm == 1 || pc.trade_confirm == 3 ) {
		if( ( MainTradeWndflag == true && HitDispNo == tradeWndFontNo[ 0 ] ) ||
			//andy_reEdit 2003/04/27
			(  SecondTradeWndflag == true && HitDispNo == SecondtradeWndFontNo[ 2 ])  ) {
			//?????��Lock?????? 2
			if(pc.trade_confirm==1) pc.trade_confirm = 2;
			//??????Lock?????? 4
			if(pc.trade_confirm==3)	pc.trade_confirm = 4;
			sprintf(buffer, "T|%s|%s|C|confirm", opp_sockfd, opp_name);
			lssproto_TD_send( sockfd, buffer );
		}
	}
// ???? Lock ?? ????? End
}
#endif

// ?????????????? ***************************************************/
void WindowDisp( ACTION *pAct )
{
	// ?????B?l?????
	WINDOW_DISP *pYobi = ( WINDOW_DISP *)pAct->pYobi;
	int i, j;
	int x = pAct->x + 32, y = pAct->y + 24;
	// ??h?k?????
	switch( pAct->actNo ){

	case 0:	// ??????????

		StockBoxDispBuffer( pYobi->mx - pYobi->nowX,
							pYobi->my - pYobi->nowY,
							pYobi->mx + pYobi->nowX,
							pYobi->my + pYobi->nowY,
							pYobi->boxDispPrio, SYSTEM_PAL_BLACK, 0 );
		// ??????
		pYobi->nowX += pAct->dx * 3;
		pYobi->nowY += pAct->dy * 3;
		// ????????
		pYobi->cnt++;
		// ????????
		if( pYobi->cnt >= WINDOW_CREATE_FRAME / 3){
			// ?????
			if( pYobi->wndType == -1 ) pAct->actNo = 1;
			else
			// ??_???????
			if( pYobi->wndType == -2 ) pAct->actNo = 3;
			else pAct->actNo = 2;
		}
		break;
	case 1:	// ?????
		pAct->hp = 1;
		break;
	case 2:	// ????????
		for( j = 0 ; j < pYobi->sizeY ; j++ ){
			// ?????
			if( j == 0 ){
				for( i = 0 ; i < pYobi->sizeX ; i++ ){
					// ??Z
					if( i == 0 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 0, pYobi->hitFlag );
					// ??Z
					else if( i == pYobi->sizeX - 1 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 2, pYobi->hitFlag );
					// ????
#ifdef _READ16BITBMP
					else if(g_bUseAlpha){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0)StockDispBuffer( x, y+1, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
						else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
					}
#endif
					else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 1, pYobi->hitFlag );
					x += 64; // ??????
				}
			}else
			// ?????
			if( j == pYobi->sizeY - 1 ){
				for( i = 0 ; i < pYobi->sizeX ; i++ ){
					// ??Z
					if( i == 0 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 6, pYobi->hitFlag );
					// ??Z
					else if( i == pYobi->sizeX - 1 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 8, pYobi->hitFlag );
					// ????
#ifdef _READ16BITBMP
					else if(g_bUseAlpha){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0) StockDispBuffer( x, y-1, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
						else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
					}
#endif
					else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 7, pYobi->hitFlag );
					x += 64; // ??????
				}
			}else
			// ????????
			for( i = 0 ; i < pYobi->sizeX ; i++ ){
				// ??Z
				if( i == 0 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 3, pYobi->hitFlag );
				// ??Z
#ifdef _READ16BITBMP
				else if(g_bUseAlpha){
					if( i == pYobi->sizeX - 1 ){
						if(pYobi->wndType == CG_WND_G_0) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
						else if(pYobi->wndType == CG_WND2_G_0) StockDispBuffer( x-1, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
					}
					else if( i == pYobi->sizeX - 1 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
					else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 4, pYobi->hitFlag );
				}
#endif
				else if( i == pYobi->sizeX - 1 ) StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 5, pYobi->hitFlag );
				// ????
				else StockDispBuffer( x, y, DISP_PRIO_MENU, pYobi->wndType + 4, pYobi->hitFlag );
				x += 64; // ??????
			}
			x = pAct->x + 32; 	// ??Z???
			y += 48; 		// ??????
		}
		// ???????
		if( pYobi->titleNo != 0 ) StockDispBuffer( pYobi->titleX, pYobi->titleY, DISP_PRIO_IME3, pYobi->titleNo, pYobi->hitFlag );
		pAct->hp = 1;
		break;
	case 3:	// ??_?????
		// ????????
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			// ????
			if( HitDispNo == pYobi->yesDispNo ){
				pYobi->yesNoResult = true;
			}
			// ?????
			if( HitDispNo == pYobi->noDispNo ){
				pYobi->yesNoResult = false;
			}
		}
		// ??_?????????????
		StockDispBuffer( pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_WND, CG_COMMON_WIN_YORO, 1 );
		// ?????
		pYobi->yesDispNo = StockDispBuffer( pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2);
		// ??????
		pYobi->noDispNo = StockDispBuffer( pYobi->mx, pYobi->my, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2 );
		break;
	}
}

// ??????????????? **************************************************************/
//	????	int x,y 			????????????
//			int sizeX,sizeY 	?????????????O?????????????O??
//			int titleNo 		?????????????k???????????????
//			int wndType 		????????????????R?e???
//			 					???????????????R?e???
//								????????????????R?e????
//								???????????????R?e????
//								?????_??????????R?e???
//								-1?????
//****************************************************************************************/
ACTION *MakeWindowDisp( int x, int y, int sizeX, int sizeY, int titleNo, int wndType )
{
	ACTION *pAct;
	WINDOW_DISP *pYobi;
#ifdef _NEW_RESOMODE  //800 600??
	if( x > 320 ){
		x += DISPLACEMENT_X;
	}else if(x > 40)
	{
		x += DISPLACEMENT_X / 2;
	}
	if(y > 40)
		y += DISPLACEMENT_Y / 2;

#endif
	// ???????????x
	pAct = GetAction( PRIO_JIKI, sizeof( WINDOW_DISP ) );
	if( pAct == NULL ) return NULL;

	// ?????
	pAct->func = WindowDisp;
	// ??????I?T
	pAct->dispPrio = DISP_PRIO_MENU;
	// ????R?e??
	pAct->atr |= ACT_ATR_HIT;
	// ???????
	pAct->atr |= ACT_ATR_HIDE;

	// ?????B?l?????
	pYobi = ( WINDOW_DISP *)pAct->pYobi;
	// ??????
	pAct->x = x;
	pAct->y = y;
	pYobi->titleX = x + ( sizeX * 64 ) / 2;
	pYobi->titleY = pAct->y + 27;
	// ??????
	pYobi->sizeX = sizeX;
	pYobi->sizeY = sizeY;
	// ?????k???
	pYobi->titleNo = titleNo;
	pYobi->boxDispPrio = DISP_PRIO_MENU;
	// ??????????
	if( wndType == 0 ) pYobi->wndType = CG_WND_G_0;
	else if( wndType == 1 ) pYobi->wndType = CG_WND_G_0;
	else if( wndType == 2 ) pYobi->wndType = CG_WND_G_0;
	else if( wndType == 3 ) pYobi->wndType = CG_WND_G_0;
	else if( wndType == 4 ){
		pYobi->wndType = -2;
		// ????R?e???
		pYobi->yesDispNo = -2;
		pYobi->noDispNo = -2;
		pYobi->boxDispPrio = DISP_PRIO_YES_NO_WND;
	}
	else pYobi->wndType = -1;

	// YesNo ??????
	pYobi->yesNoResult = -1;

	// ????R?e???
	if( wndType >= 2 ) pYobi->hitFlag = false;
	else pYobi->hitFlag = true;

	// ?????????
	// ??????????_???????
	if( wndType == -1 || wndType == 4 ){
		pYobi->mx = sizeX / 2 + pAct->x;
		pYobi->my = sizeY / 2 + pAct->y;
	}else{	// ?[????????
		pYobi->mx = ( sizeX * 64 ) / 2 + pAct->x;
		pYobi->my = ( sizeY * 48 ) / 2 + pAct->y;
	}
	// ??????
	pAct->dx = ( pYobi->mx - pAct->x ) / WINDOW_CREATE_FRAME;
	pAct->dy = ( pYobi->my - pAct->y ) / WINDOW_CREATE_FRAME;
    pAct->deathFlag = false;
	return pAct;
}

// ???????????????? ***********************************************/
void AnimDisp( ACTION *pAct )
{
	int flag = false;
	int x, y, i;

	// ?????
	switch( pAct->actNo ){

	case ANIM_DISP_PET:	// ???????

		// ????????????
		if (pAct->hitDispNo == HitDispNo && mouse.autoState & MOUSE_LEFT_CRICK)
		{
			// ??????
			pAct->anim_ang++;
			// ????????
			if( pAct->anim_ang >= 8 ) pAct->anim_ang = 0;
			play_se( 217, 320, 240 ); // ?????
		}
		// ???????
		pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );

		break;

	case ANIM_DISP_PET_SEND:	// ??????????????????h

		// ?h??k?
		pAct->anim_no = ANIM_WALK;
		// ?????????( ??? )( ??????? )
		pAct->anim_ang = 2;
		// ???
		pAct->crs = crs_change_tbl2[ pAct->anim_ang ];
		// ?????
		pAct->dx++;
		// ??????????L
		if( pAct->dx % 3 == 0 ){
			// ??????L
			if( pAct->spd < 20 ){
				pAct->spd++;
			}
		}
		// ??????????L
		if( pAct->dx % 5 == 0 ){
			if( pAct->dy >= 2 ) pAct->dy--;
		}
		// ??h??
		gemini( pAct );

		// ???????
		if( pActMailItem != NULL ){
			// ??????????
			if( pAct->x <= pActMailItem->x ){
				pActMailItem->x = pAct->x;
			}
		}
		// ???????????????????
		if( pAct->x <= - 100 ){
			// ????????
			DeathAction( pActMenuWnd );
			pActMenuWnd = NULL;
			// ??????
			DeathAction( pActPet );
			pActPet = NULL;
			// ??????????
			DeathAction( pActMailItem );
			pActMailItem = NULL;
			// ????????
			mailWndNo = MAIL_WND_VIEW;
			// ??????????
			play_se( 203, 320, 240 );
			// ?V????????
			GetKeyInputFocus( &MyChatBuffer );
		}
		// ???????
		pattern( pAct, pAct->dy, ANM_LOOP );

		break;

	case ANIM_DISP_PET_RECIEVE:	// ????????????????????h

		// ?h??k?
		pAct->anim_no = ANIM_WALK;
		// ?????????( ??? )( ??????? )
		pAct->anim_ang = 6;
		// ???
		pAct->crs = crs_change_tbl2[ pAct->anim_ang ];
		// ?????
		pAct->dx++;
		// ??????L
		if( pAct->spd > 0 ){
			// ??????????L
			if( pAct->dx % 3 == 0 ){
				pAct->spd--;
				// ?????????h????
				if( pAct->spd <= 0 ){
					pAct->actNo = ANIM_DISP_PET_MAIN;
					// ???????
					if( pActMailItem != NULL ){
						pActMailItem->dispPrio = DISP_PRIO_BOX3;
					}
				}
			}
		}
		// ??????????L
		if( pAct->dx % 8 == 0 ){
			// ?????????L
			if( pAct->dy < 12 ) pAct->dy++;
		}
		// ??h??
		gemini( pAct );

		// ???????
		if( pActMailItem != NULL ){
			// ??????????
			if( pAct->x >= pActMailItem->x ){
				pActMailItem->x = pAct->x;
			}
		}
		// ???????
		pattern( pAct, pAct->dy, ANM_LOOP );

		break;

	case ANIM_DISP_PET_MAIN:	// ??????h?k????

		// ?????????
		if( Rnd( 0, 4 ) >= 2 ){
			pAct->actNo = ANIM_DISP_PET_WALK;
			// ???????
			pAct->anim_ang = Rnd( 0, 7 );
			// ????@?e
			pAct->crs = crs_change_tbl2[ pAct->anim_ang ];
			// ??????h????
			pAct->spd = Rnd( 1, 12 );
			// ???????????
			pAct->dy = 13 - pAct->spd;
		}
		else pAct->actNo = Rnd( ANIM_DISP_PET_STAND, ANIM_DISP_PET_DEAD );

		// ?q???????????????
		pAct->anim_no_bak = -1;

		break;

	case ANIM_DISP_PET_WALK:	// ???

		// ?h??k?
		pAct->anim_no = ANIM_WALK;

		// ??h??
		gemini( pAct );

		// ???????
		if( pattern( pAct, pAct->dy, ANM_NO_LOOP ) ){
			// ?????????
			if( Rnd( 0, 3 ) == 0 ){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}else{
				// ?q???????????????
				pAct->anim_no_bak = -1;
			}
		}
		if( pActPet == NULL || pActMenuWnd == NULL ){
			return;
		}
		// ????????
		// ??
		if( pActPet->y < pActMenuWnd->y + 64 ){
			pActPet->y = pActMenuWnd->y + 64;
			// ????@?e
			pAct->crs = crs_bound_tbl[ 0 ][ pAct->crs ];
			// ???@?e
			pAct->anim_ang = crs_change_tbl[ pAct->crs ];
		}
		// ?
		if( pActPet->y > pActMenuWnd->y + 424 - 32 ){
			pActPet->y = pActMenuWnd->y + 424 - 32;
			// ????@?e
			pAct->crs = crs_bound_tbl[ 2 ][ pAct->crs ];
			// ???@?e
			pAct->anim_ang = crs_change_tbl[ pAct->crs ];
		}
		// ?
		if( pActPet->x < pActMenuWnd->x + 32 ){
			pActPet->x = pActMenuWnd->x + 32;
			// ????@?e
			pAct->crs = crs_bound_tbl[ 3 ][ pAct->crs ];
			// ???@?e
			pAct->anim_ang = crs_change_tbl[ pAct->crs ];
		}
		// ?
		if( pActPet->x > pActMenuWnd->x + 272 - 32 ){
			pActPet->x = pActMenuWnd->x + 272 - 32;
			// ????@?e
			pAct->crs = crs_bound_tbl[ 1 ][ pAct->crs ];
			// ???@?e
			pAct->anim_ang = crs_change_tbl[ pAct->crs ];
		}
		break;

	case ANIM_DISP_PET_STAND:	// ?r?

		// ?h??k?
		pAct->anim_no = ANIM_STAND;

		// ???????
		if( pattern( pAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) ){
			// ?????????
				pAct->actNo = ANIM_DISP_PET_MAIN;
		}

		break;

	case ANIM_DISP_PET_ATTACK:	// ??

		// ?h??k?
		pAct->anim_no = ANIM_ATTACK;

		// ???????
		if( pattern( pAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) ){
			// ?????????
			if( Rnd( 0, 1 ) == 0 ){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}else{
				// ?q???????????????
				pAct->anim_no_bak = -1;
			}
		}

		break;

	case ANIM_DISP_PET_GUARD:	// ???

		// ?h??k?
		pAct->anim_no = ANIM_GUARD;

		// ???????
		if( pattern( pAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) ){
			// ?????????
			if( Rnd( 0, 1 ) == 0 ){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
		}

		break;

	case ANIM_DISP_PET_DAMAGE:	// ????

		// ?h??k?
		pAct->anim_no = ANIM_DAMAGE;

		// ???????
		if( pattern( pAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) ){
			// ?????????
			if( Rnd( 0, 1 ) == 0 ){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}else{
				// ?q???????????????
				pAct->anim_no_bak = -1;
			}
		}

		break;

	case ANIM_DISP_PET_DEAD:	// ???

		// ?h??k?
		pAct->anim_no = ANIM_DEAD;

		// ???????
		if( pattern( pAct, ANM_NOMAL_SPD, ANM_NO_LOOP ) ){
			// ?????????
			if( Rnd( 0, 2 ) == 0 ){
				pAct->actNo = ANIM_DISP_PET_MAIN;
			}
		}

		break;

	case ANIM_DISP_PET_ITEM:	// ??????????????

		// ???????
		pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );

		break;

	case ANIM_DISP_LETTER_MOVE:	// ?????h

		// ??h
		if( pAct->x < 245 ) pAct->x += 5;
		// ???????????
		else{
			pAct->actNo = ANIM_DISP_LETTER;
			// ???????
			pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );
			pAct->anim_cnt += 8;
		}
		break;

	case ANIM_DISP_LETTER:		// ??????????

		// ???????
		pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );

		break;

	case ANIM_DISP_MIX_PET_INIT:	// ?????????

		// ??????h????
		pAct->spd = 11;//Rnd( 6, 12 );
		// ???????????
		pAct->dy = 14 - pAct->spd;

		// ?h??k?
		pAct->anim_no = ANIM_WALK;

		// ?????????S?
		for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
			if( ItemBuffer[ i ].mixFlag >= 1 ){
				pAct->dx = i;
				break;
			}
		}
		// ??h?
		pAct->actNo = ANIM_DISP_MIX_PET_MOVE;
		break;

	case ANIM_DISP_MIX_PET_MOVE:	// ??????????????h

		// ?????@?e
		x = ItemBuffer[ pAct->dx ].defX;
		y = ItemBuffer[ pAct->dx ].defY;

		// ????
		radar( pAct, &x, &y );

		// ?h??k?
		pAct->anim_no = ANIM_WALK;

		// ?????????
		if( y >= pAct->spd / 2 ){
			// ????@?e
			pAct->crs = x;
			// ???@?e???????????????
			if( pAct->level == false ){
				pAct->anim_ang = crs_change_tbl[ pAct->crs ];
				// ????????????
				pAct->level = true;
			}

			// ??h??
			gemini( pAct );
			// ???????
			pattern( pAct, pAct->dy, ANM_LOOP );
		}
		// ?????
		else{
			// ?????????????
			pAct->level = false;
			// ????????
			play_se( 204, 320, 240 );

			// ??????????????
			if( pAct->delta == 1 ){
				// ???????
				pAct->actNo = ANIM_DISP_MIX_PET_MOJI;
				// ?h??k?
				pAct->anim_no = ANIM_STAND;

				// ??????h????
				pAct->spd = 10;//Rnd( 6, 12 );
				// ???????????
				pAct->dy = 14 - pAct->spd;

				// ????????
				for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
					if( ItemBuffer[ i ].mixFlag >= 1 ){
						ItemBuffer[ i ].mixFlag = 4;
					}
				}

				// ????????????
				pAct->delta = Rnd( 2, 15 );
				pAct->mp = 15;
				break;
			}

			// ??h?????w???
			pAct->spd = ( int )( ( double )pAct->spd * 0.79 );
			// ????????
			if( pAct->spd < 2 ) pAct->spd = 2;
			// ???????????
			pAct->dy = 13 - pAct->spd;

			ItemBuffer[ pAct->dx ].mixFlag = 3;

			// ????????S?
			for( i = pAct->dx + 1 ; i < MAX_ITEM ; i++ ){
				if( ItemBuffer[ i ].mixFlag >= 1 ){
					pAct->dx = i;
					break;
				}
			}

			// ????????
			if( i >= MAX_ITEM ){
				// ??????????????S?
				for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
					// ?????????????
					if( pc.item[ i ].useFlag == false ){
						// ?????e
						pAct->dx = i;
						break;
					}else
					// ??????????????
					if( ItemBuffer[ i ].mixFlag >= 1 ){
						// ?????e
						pAct->dx = i;
						break;
					}
				}
				// ????????????
				pAct->delta = 1;
			}
		}
		break;

	case ANIM_DISP_MIX_PET_MOJI:	// ?????????????????

		// ???????
		pattern( pAct, 1, ANM_LOOP );
		// ??????
		if( pAct->maxHp >= pAct->mp ) pAct->maxHp = 0;
		else{
			pAct->maxHp++;
			break;
		}
		// ??????
		switch( pAct->hp ){

			// ???
			case 0:
			case 3:
				// ???@?e
				pAct->anim_ang++;
				// ????????
				if( pAct->anim_ang >= 8 ) pAct->anim_ang = 0;
				// ?????
				if( pAct->hp == 0 ){
					// ????????
					play_se( 216, 320, 240 );
				}
				// ???????
				if( pAct->hp == 3 ){
					pAct->dir++;
					pAct->hp = 0;
					// ????????????
					pAct->mp--;
					if( pAct->mp < 4 ) pAct->mp = 4;
				}
				else pAct->hp++;
			break;

			// ???
			case 1:
			case 2:
				// ???@?e
				pAct->anim_ang--;
				// ????????
				if( pAct->anim_ang < 0 ) pAct->anim_ang = 7;
				pAct->hp++;
			break;
		}

		// ?????????
		if( pAct->dir >= pAct->delta ){
			// ?????
			if( pAct->dir == pAct->delta ){
				pAct->dir++;
				// ??????
				char moji[ 256 ];
				// ??????
				moji[ 0 ] = NULL;
				// ??????????????
				for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
					// ????????????
					if( ItemBuffer[ i ].mixFlag >= true ){//ttom
						char work[ 256 ];
						// ???????
						sprintf( work,"%d|", i );
						strcat( moji, work );
						// ??????
					}//ttom
				}
				// ???|????????
				moji[ strlen( moji ) - 1 ] = NULL;

				// ????????????????????????
				if( bNewServer)
					lssproto_PS_send( sockfd, mixPetNo, SelectWazaNo, 0, moji );
				else
					old_lssproto_PS_send( sockfd, mixPetNo, SelectWazaNo, 0, moji );
				// ????r??????
				ItemMixRecvFlag = true;
			}

			// ????r??????
			if(	ItemMixRecvFlag == false ){
				// ????????
				play_se( 115, 320, 240 );
				// ??????????
				for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
					ItemBuffer[ i ].mixFlag = false;
				}
				// ??????h????
				pAct->spd = 11;
				// ???????????
				pAct->dy = 14 - pAct->spd;
				// ?????
				pAct->actNo = ANIM_DISP_MIX_PET_END;
			}
		}

		break;

	case ANIM_DISP_MIX_PET_END:	// ???????????

		// ?????@?e
		x = 750;
		y = 240;

		// ????
		radar( pAct, &x, &y );

		// ?h??k?
		pAct->anim_no = ANIM_WALK;

		// ?????????
		if( y >= pAct->spd / 2 ){
			// ????@?e
			pAct->crs = x;

			// ???@?e???????????????
			if( pAct->level == false ){
				pAct->anim_ang = crs_change_tbl[ pAct->crs ];
				// ????????????
				pAct->level = true;
			}

			// ??h??
			gemini( pAct );
			// ???????
			pattern( pAct, pAct->dy, ANM_LOOP );
		}
		// ?????
		else{
			// ????????
			DeathAction( pAct );
			pActPet2 = NULL;
			// ???????k????
			ItemMixPetNo = -1;
		}
		break;
#ifdef _AniCrossFrame	  // Syu ADD ???????��?????????
	case ANIM_DISP_CROSSFRAME:
		{
			extern bool delFlag;
			extern bool flag22;
			pAct->anim_no = ANIM_WALK;
			pAct->anim_ang = 2;
			pAct->crs = crs_change_tbl2[ pAct->anim_ang ];
			//??????
			pAct->spd = 1;
			//?????
			pAct->dispPrio = DISP_PRIO_JIKI;
			if( pAct->dx % 5 == 0 ){
				if( pAct->dy >= 2 ) pAct->dy--;
			}
			gemini( pAct );
			//???????��??
			if ( ((pAct->anim_chr_no > 101511) && (pAct->anim_chr_no < 101516)) ||
				(pAct->anim_chr_no == 101517) || (pAct->anim_chr_no == 101519) )
			{
				pAct->x--;
				if ( pAct->y == 0 )
					UpDownflag = 4;
				if ( UpDownflag == 0 )
					UpDownflag = RAND ( 1 , 5 );
				if ( UpDownflag > 3 )
					pAct->y ++ ;
				else
					pAct->y = pAct->y ;
			}
			else if ( (pAct->anim_chr_no == 101516) || (pAct->anim_chr_no == 101518) )
			{
				pAct->x++;
				if ( pAct->y == 0 )
					UpDownflag = 4;
				if ( UpDownflag == 0 )
					UpDownflag = RAND ( 1 , 5 );
				if ( UpDownflag > 3 )
					pAct->y ++ ;
				else
					pAct->y = pAct->y ;
			}
			else if ( (pAct->anim_chr_no == 101520) || (pAct->anim_chr_no == 101522) ||
				( pAct->anim_chr_no <= 101593 && pAct->anim_chr_no >= 101588 )  ) //????
			{
				pAct->x++;
				pAct->y -- ;
			}
			else if ( (pAct->anim_chr_no == 101521) || (pAct->anim_chr_no == 101523) ||
				( pAct->anim_chr_no <= 101587 && pAct->anim_chr_no >= 101582 )  ) //????
			{
				pAct->x--;
				pAct->y-- ;
			}
			else
			{
				pAct->x++;
				pAct->y++;
			}
			//????????��????
			if( (pAct->x <= - 100 || pAct->y <= -100 || pAct->x >= 740 || pAct->y >= 580) && delFlag == true && flag22 == true)
			{
				flag22 = false;
				UpDownflag = 0 ;
			}
			pattern( pAct, pAct->dy, ANM_LOOP );
		}
		break;
#endif
	}

	// ??????????h?????
	if( ANIM_DISP_PET_MAIN <= pAct->actNo && pAct->actNo <= ANIM_DISP_PET_DEAD ){
		// ????????????
		if( pAct->hitDispNo == HitDispNo &&
			mouse.autoState & MOUSE_LEFT_CRICK ){
			// ??h???
			pAct->actNo = ANIM_DISP_PET_WALK;
			// ???????
			pAct->anim_ang = Rnd( 0, 7 );
			// ????@?e
			pAct->crs = crs_change_tbl2[ pAct->anim_ang ];
			// ??????h????
			pAct->spd = 15;

			pAct->dy = 2;
			play_se( 217, 320, 240 ); // ?????
		}
	}
}

// ????????????? ***********************************************/
ACTION *MakeAnimDisp( int x, int y, int sprNo, int mode )
{
	ACTION *pAct;

	// ???????????x
	pAct = GetAction( PRIO_JIKI, NULL );
	if( pAct == NULL ) return NULL;

	// ?????
	pAct->func = AnimDisp;
	// ????R?e??
	pAct->atr |= ACT_ATR_HIT;
	pAct->anim_chr_no = sprNo;
	// ?h??k?
	pAct->anim_no = ANIM_STAND;
	// ?????????( ??? )( ??????? )
	pAct->anim_ang = 1;
	/* ??????I?T */
	pAct->dispPrio = DISP_PRIO_BOX3;
	/* ????t */
	pAct->x = x;
	pAct->y = y;
	// ????@?e
	pAct->actNo = mode;
	// ??????????
	if( pAct->actNo == ANIM_DISP_PET ){
		// ???????????
		pAct->dy = 12;
	}else
	if( pAct->actNo == ANIM_DISP_PET_RECIEVE ){
		pAct->dy = 1;
		pAct->spd = 30;
	}
	// ???????
	pattern( pAct, ANM_NOMAL_SPD, ANM_LOOP );

	return pAct;
}

// ?????????V ****************************************************************/
bool SaveMailHistory( int no )
{
	FILE *fp;

	// ????????????
	if( ( fp = fopen( MAIL_HISTORY_FILE_NAME, "r+b" ) ) == NULL ){
		return false;
	}

	// ?????????????????????????????
	fseek( fp, sizeof( MAIL_HISTORY ) * MAX_ADR_BOOK * selectPcNo, SEEK_SET );

	// ??????????k???????????????????
	fseek( fp, sizeof( MAIL_HISTORY ) * no, SEEK_CUR );

	// ???????
	if( fwrite( &MailHistory[ no ], sizeof( MAIL_HISTORY ), 1, fp ) < 1 ){

		fclose( fp );// ????????
		return false;
	}

	fclose( fp );

	return true;
}

// ???????????? ****************************************************************/
bool LoadMailHistory( void )
{
	FILE *fp;
	MAIL_HISTORY work[ MAX_ADR_BOOK * 2 ];	// ??????????B?l

	// ?????????????
	if( ( fp = fopen( MAIL_HISTORY_FILE_NAME, "rb" ) ) == NULL ){
		// ????????????
		if( ( fp = fopen( MAIL_HISTORY_FILE_NAME, "wb" ) ) != NULL ){
			// ???????
			fwrite( MailHistory, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK, fp );
			fwrite( MailHistory, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK, fp );
			fclose( fp );	// ????????
		}

		return false;
	}

	// ????????????
	if( fread( work, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK * 2, fp ) < MAX_ADR_BOOK * 2 ){
		// ???????
		// ????????????
		if( ( fp = fopen( MAIL_HISTORY_FILE_NAME, "wb" ) ) != NULL ){
#ifdef _DEBUG
		//MessageBox( hWnd, "????????mail?????", "???", MB_OK | MB_ICONSTOP );
#endif
			// ???????
			fwrite( MailHistory, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK, fp );
			fwrite( MailHistory, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK, fp );
			fclose( fp );	// ????????
		}

		return false;
	}


	// ????????????????????????
	fseek( fp, sizeof( MAIL_HISTORY ) * MAX_ADR_BOOK * selectPcNo, SEEK_SET );

	// ????????
	if( fread( MailHistory, sizeof( MAIL_HISTORY ), MAX_ADR_BOOK, fp ) < MAX_ADR_BOOK ){

		fclose( fp );	// ????????
		return false;
	}
	// ????????
	fclose( fp );

	return true;
}

// ??????K???????V ****************************************************************/
bool SaveChatRegistyStr( void )
{
	FILE *fp;

	// ????????????
	if( ( fp = fopen( CAHT_REGISTY_STR_FILE_NAME, "r+b" ) ) == NULL ){
#ifdef _DEBUG
		//MessageBox( hWnd, "????????????????????", "???", MB_OK | MB_ICONSTOP );
#endif
		return false;
	}

	// ???????
	if( fwrite( chatRegistryStr, sizeof( STR_BUFFER ), MAX_CHAT_REGISTY_STR, fp ) < MAX_CHAT_REGISTY_STR ){

#ifdef _DEBUG
		//MessageBox( hWnd, "????????????????????", "???", MB_OK | MB_ICONSTOP );
#endif
		fclose( fp );// ????????
		return false;
	}
#ifdef _TALK_WINDOW
	fwrite(&g_bTalkWindow,sizeof(bool),1,fp);
#endif
	// ????????
	fclose( fp );

	return true;
}

// ??????K?????????? ****************************************************************/
bool LoadChatRegistyStr( void )
{
	FILE *fp;
	int i;

	// ?????????????
	if( ( fp = fopen( CAHT_REGISTY_STR_FILE_NAME, "rb" ) ) == NULL ){
		// ????????????
		if( ( fp = fopen( CAHT_REGISTY_STR_FILE_NAME, "wb" ) ) != NULL ){
#ifdef _DEBUG
		//MessageBox( hWnd, "????????????????????", "???", MB_OK );
#endif
			// ?????????K???V??????????
			for( i = 0 ; i < 8 ; i++ ){
				// ??????? ???@?e
				chatRegistryStr[ i ].buffer[ 0 ] = NULL;
				chatRegistryStr[ i ].cnt = 0;
				chatRegistryStr[ i ].cursor=0;
				// ????????
				chatRegistryStr[ i ].len = MAX_CHAT_REGISTY_STR_LEN;
				// ??????
				chatRegistryStr[ i ].lineLen = 0;
				// ?????
				chatRegistryStr[ i ].lineDist = 20;
				// ???????
				chatRegistryStr[ i ].color = 0;
				// ????????
				// ???????????I??
				chatRegistryStr[ i ].fontPrio = FONT_PRIO_FRONT;
			}
			// ???????
			fwrite( chatRegistryStr, sizeof( STR_BUFFER ), MAX_CHAT_REGISTY_STR, fp );
#ifdef _TALK_WINDOW
			fwrite(&g_bTalkWindow,sizeof(bool),1,fp);
#endif
			fclose( fp );	// ????????
		}
		return false;
	}

	// ????????
	if( fread( chatRegistryStr, sizeof( STR_BUFFER ), MAX_CHAT_REGISTY_STR, fp ) < MAX_CHAT_REGISTY_STR ){

#ifdef _DEBUG
		//MessageBox( hWnd, "?????????????????????????", "???", MB_OK | MB_ICONSTOP );
#endif
		fclose( fp );	// ????????
		return false;
	}
#ifdef _TALK_WINDOW
	fread(&g_bTalkWindow,sizeof(bool),1,fp);
	if(g_bTalkWindow) TalkWindow.Create();
#endif
	// ????????
	fclose( fp );

	return true;
}
#ifdef __ALBUM_47
int MAX_PET_TBL;
#endif
// ???????V ****************************************************************/
bool SaveAlbum( int no )
{
	FILE	*fp;

#if defined(__ALBUM_47)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_48, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
        {
        }
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_46)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_47, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_45)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_46, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_44)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_45, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_43)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_44, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_42)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_43, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_41)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_42, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_40)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_41, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_39)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_40, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_38)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_39, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_37)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_38, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_36)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_37, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;

#elif defined(__ALBUM_35)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_36, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_34)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_35, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_33)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_34, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;
#elif defined(__ALBUM_32)
    bool	ret = false;
	if( 0 <= no && no < MAX_PET_TBL ){
		if( fp=fopen( ALBUM_FILE_NAME_33, "r+b")){
			if( 0 != fseek( fp, (16+sizeof(PET_ALBUM)*MAX_PET_KIND)*AlbumIdCnt, SEEK_SET))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( 0 != fseek( fp, 16+sizeof(PET_ALBUM)*no, SEEK_CUR))
				//MessageBox( hWnd, "????????????????", "???", MB_OK );
			if( fwrite( &PetAlbum[no], sizeof(PET_ALBUM), 1, fp) >= 1)
				ret = true;
			fclose( fp );
		}else
			//MessageBox( hWnd, "????????????????", "???", MB_OK );
	}
	return ret;
#else
	// ????????????
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL ){
#ifdef _DEBUG
		//MessageBox( hWnd, "????????????????", "???", MB_OK );
#endif
		return false;
	}
	// ?\?????????????
	for( int i = 0 ; i < AlbumIdCnt ; i++ ){
		// ????????????????
		fseek( fp, 16, SEEK_CUR );
		fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND, SEEK_CUR );
	}
	// ???????????
	fseek( fp, 16, SEEK_CUR );
	// ???????????k???????
	fseek( fp, sizeof( PET_ALBUM ) * no, SEEK_CUR );

	// ???????
	if( fwrite( &PetAlbum[ no ], sizeof( PET_ALBUM ), 1, fp ) < 1 ){

#ifdef _DEBUG
		//MessageBox( hWnd, "????????????????", "???", MB_OK );
#endif
		fclose( fp );// ????????
		return false;
	}
	// ????????
	fclose( fp );

	return true;
#endif
}

#if defined(__ALBUM_4)

bool LoadAlbum_4( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_4, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_4, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			// ??????????????????
			// ????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			//
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbum1_4(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

#elif defined(__ALBUM_47)
bool LoadAlbum_47( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	/*DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");	*/
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_48, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_48, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo47(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				memcpy( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_47(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						memcpy( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_46)
bool LoadAlbum_46( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_47, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_47, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo46(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_46(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_45)
bool LoadAlbum_45( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_46, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_46, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo45(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_45(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_44)
bool LoadAlbum_44( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_45, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_45, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo44(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_44(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_43)
bool LoadAlbum_43( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_44, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_44, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo43(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_43(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_42)
bool LoadAlbum_42( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_43, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_43, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo42(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_42(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_41)
bool LoadAlbum_41( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_42, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_42, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo41(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_41(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_40)
bool LoadAlbum_40( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_41, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_41, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo40(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_40(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_39)
bool LoadAlbum_39( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_40, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_40, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo39(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_39(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_38)
bool LoadAlbum_38( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_39, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_39, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo38(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_38(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_37)
bool LoadAlbum_37( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_38, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_38, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo37(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_37(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_36)
bool LoadAlbum_36( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_37, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_37, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo36(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				if( 440 <= i && i <= 461 ) continue;		//????????Q ????
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_36(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_35)
bool LoadAlbum_35( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_36, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_36, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo35(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_35(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_34)
bool LoadAlbum_34( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_35, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_35, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo34(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_34(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_33)
bool LoadAlbum_33( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_34, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_34, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo33(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_33(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#elif defined(__ALBUM_32)
bool LoadAlbum_32( char *user)
{
	FILE *fp;
	bool ret = false;
	char id[16];
	char id2[ 16 ];
	int i;
	MAX_PET_TBL = sizeof(PetAlbumTbl) / sizeof(PetAlbumTbl[0]);
	AlbumIdCnt = 0;
	memcpy( id, user, 16);
	DeleteFile( "/sdcard/jerrysa/data/album_2.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_3.dat");
	DeleteFile( "/sdcard/jerrysa/data/album_10.dat");
	for( i = 0; i < MAX_PET_KIND; i++){
		for( int j = 0 ; j < PET_NAME_LEN+1 ; j++){
			PetAlbum[i].name[j]		 = Rnd( 1, 255 );
			PetAlbum[i].freeName[j]	 = Rnd( 1, 255 );
		}
		PetAlbum[i].flag		 = 0;
		PetAlbum[i].faceGraNo	 = Rnd( 1, 60000 );
		PetAlbum[i].level		 = Rnd( 1, 30000 );
		PetAlbum[i].faceGraNo	 = Rnd( 1, 30000 );
		PetAlbum[i].maxHp		 = Rnd( 1, 30000 );
		PetAlbum[i].str			 = Rnd( 1, 30000 );
		PetAlbum[i].quick		 = Rnd( 1, 30000 );
		PetAlbum[i].def			 = Rnd( 1, 30000 );
		PetAlbum[i].earth		 = Rnd( 1, 30000 );
		PetAlbum[i].water		 = Rnd( 1, 30000 );
		PetAlbum[i].fire		 = Rnd( 1, 30000 );
		PetAlbum[i].wind		 = Rnd( 1, 30000 );
	}
	if( (fp=fopen( ALBUM_FILE_NAME_33, "r+b")) == NULL){
		if( (fp=fopen( ALBUM_FILE_NAME_33, "wb")) != NULL){
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0; i < 16; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			//?????????????
			id[15] &= 0xf;
			id[id[15]] = Rnd( 1, 100);
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[ i ];
			if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
			}
			fclose( fp);
			return false;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[ i ] ^= IdEncryptionTbl[ i ];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			//???????????
			if( fseek( fp, sizeof(PET_ALBUM)*MAX_PET_KIND, SEEK_CUR) != 0){
				fseek( fp, (sizeof(PET_ALBUM)*MAX_PET_KIND+16)*AlbumIdCnt, SEEK_SET);
				id[15] &= 0xf;
				id[id[15]] = Rnd( 1, 100);
				for( i = 0; i < 16; i++)
					id[ i ] ^= IdEncryptionTbl[ i ];
				if( fwrite( id, sizeof( id ), 1, fp ) >= 1){
					fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				}
				fclose( fp);
				return false;
			}
		}else{
			//???????????
			if( fread( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp) < MAX_PET_KIND){
				fwrite( PetAlbum, sizeof(PET_ALBUM), MAX_PET_KIND, fp);
				fclose( fp);
				return false;
			}
			fclose( fp);
			return true;
		}
		AlbumIdCnt++;
	}
	fclose( fp );
	return ret;
}

bool ConvertAlbumTo32(  char *user, char *oldfile, unsigned long num)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( oldfile, "r+b" ) ) == NULL )
		return false;
	while( 1){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * num, SEEK_CUR ) != 0 ){//?????
				fclose( fp );
				return false;
			}
		}else{
			//???ID???
			if( fread( OldAlbum, sizeof( PET_ALBUM ), num, fp ) < num ){
				fclose( fp );
				return false;
			}
			for( unsigned long i = 0; i < num; i++){
				CopyMemory( &PetAlbum[i], &OldAlbum[i], sizeof(PET_ALBUM));
				SaveAlbum( i);
			}
			break;
		}
	}
	fclose( fp );
	return true;
}

bool ConvertAlbum1_32(  char *user)
{
	PET_ALBUM OldAlbum[ MAX_PET_KIND_1 ];
	char id2[ 16 ];
	int i;
	FILE *fp;
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	while( 1 ){
		//????ID
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			fclose( fp );
			return false;
		}
		//ID????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		id2[ id2[ 15 ] ] = NULL;
		if( strcmp( user, id2 ) != 0 ){
			//ID????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND_1, SEEK_CUR ) != 0 ){//???????
				fclose( fp );
				return false;
			}
		}else{
			//????????
			if( fread( OldAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND_1, fp ) < MAX_PET_KIND_1 ){
				fclose( fp );
				return false;
			}
			for( int i = 0; i < MAX_PET_KIND_1; i++){
				for( int j = 0; j < MAX_PET_TBL; j++){
					if( !OldAlbum[i].flag)
						break;
					if( OldAlbum[i].faceGraNo == PetAlbumTbl[j].faceGraNo){
						CopyMemory( &PetAlbum[PetAlbumTbl[j].albumNo], &OldAlbum[i], sizeof(PET_ALBUM));
						SaveAlbum( PetAlbumTbl[j].albumNo);
						break;
					}
				}
			}
			break;
		}
	}
	fclose( fp );
	return true;
}
#endif


// ?????????? ****************************************************************/
bool LoadAlbum( void )
{
	char id[ 32 ];

	// ???????x
	extern char szUser[];
	memcpy(id,szUser,32);
	ecb_crypt("f;encor1c",id,32,DES_DECRYPT);
	// ??????????x
	id[ 15 ] = strlen( id );

#if defined(__ALBUM_47)
	if( LoadAlbum_47( id) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_47, MAX_PET_KIND_46) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_46, MAX_PET_KIND_45) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo47( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_47( id) )
		return true;
	return false;
#elif defined(__ALBUM_46)
	if( LoadAlbum_46( id) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_46, MAX_PET_KIND_45) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo46( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_46( id) )
		return true;
	return false;
#elif defined(__ALBUM_45)
	if( LoadAlbum_45( id) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_45, MAX_PET_KIND_44) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo45( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_45( id) )
		return true;
	return false;
#elif defined(__ALBUM_44)
	if( LoadAlbum_44( id) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_44, MAX_PET_KIND_43) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo44( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_44( id) )
		return true;
	return false;
#elif defined(__ALBUM_43)
	if( LoadAlbum_43( id) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_43, MAX_PET_KIND_42) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo43( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_43( id) )
		return true;
	return false;
#elif defined(__ALBUM_42)
	if( LoadAlbum_42( id) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_42, MAX_PET_KIND_41) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo42( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_42( id) )
		return true;
	return false;
#elif defined(__ALBUM_41)
	if( LoadAlbum_41( id) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_41, MAX_PET_KIND_40) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo41( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_41( id) )
		return true;
	return false;
#elif defined(__ALBUM_40)
	if( LoadAlbum_40( id) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_40, MAX_PET_KIND_39) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo40( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_40( id) )
		return true;
	return false;
#elif defined(__ALBUM_39)
	if( LoadAlbum_39( id) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_39, MAX_PET_KIND_38) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo39( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_39( id) )
		return true;
	return false;
#elif defined(__ALBUM_38)
	if( LoadAlbum_38( id) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_38, MAX_PET_KIND_37) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo38( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_38( id) )
		return true;
	return false;
#elif defined(__ALBUM_37)
	if( LoadAlbum_37( id) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_37, MAX_PET_KIND_36) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo37( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_37( id) )
		return true;
	return false;
#elif defined(__ALBUM_36)
	if( LoadAlbum_36( id) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_36, MAX_PET_KIND_35) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo36( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_36( id) )
		return true;
	return false;
#elif defined(__ALBUM_35)
	if( LoadAlbum_35( id) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_35, MAX_PET_KIND_34) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo35( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_35( id) )
		return true;
	return false;
#elif defined(__ALBUM_34)
	if( LoadAlbum_34( id) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_34, MAX_PET_KIND_33) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo34( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_34( id) )
		return true;
	return false;
#elif defined(__ALBUM_33)
	if( LoadAlbum_33( id) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_33, MAX_PET_KIND_32) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo33( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_33( id) )
		return true;
	return false;
#elif defined(__ALBUM_32)
	if( LoadAlbum_32( id) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_32, MAX_PET_KIND_31) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_31, MAX_PET_KIND_30) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_30, MAX_PET_KIND_29) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_29, MAX_PET_KIND_28) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_28, MAX_PET_KIND_27) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_27, MAX_PET_KIND_26) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_26, MAX_PET_KIND_25) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_25, MAX_PET_KIND_24) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_24, MAX_PET_KIND_23) )
		return true;
    if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_23, MAX_PET_KIND_22) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_22, MAX_PET_KIND_21) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_21, MAX_PET_KIND_20) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_20, MAX_PET_KIND_19) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_19, MAX_PET_KIND_18) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_18, MAX_PET_KIND_17) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_17, MAX_PET_KIND_16) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_16, MAX_PET_KIND_15) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo32( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_32( id) )
		return true;
	return false;
#elif defined(__ALBUM_15)
	if( LoadAlbum_15( id) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_15, MAX_PET_KIND_14) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_14, MAX_PET_KIND_13) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_13, MAX_PET_KIND_12) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_12, MAX_PET_KIND_11) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_10, MAX_PET_KIND_10) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_9, MAX_PET_KIND_9) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_8, MAX_PET_KIND_8) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_7, MAX_PET_KIND_7) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_6, MAX_PET_KIND_6) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_5, MAX_PET_KIND_5) )
		return true;
	if( ConvertAlbumTo15( id, ALBUM_FILE_NAME_4, MAX_PET_KIND_4) )
		return true;
	if( ConvertAlbum1_15( id) )
		return true;
	return false;
#else
	char id2[ 16 ];
	int i;
	FILE *fp;
	// ?????????
	AlbumIdCnt = 0;
	// ?????????????
	if( ( fp = fopen( ALBUM_FILE_NAME, "r+b" ) ) == NULL ){

		// ????????????
		if( ( fp = fopen( ALBUM_FILE_NAME, "wb" ) ) != NULL ){
			// ????????????
			id[ id[ 15 ] ] = Rnd( 1, 100 );
			// ??????
			for( i = 0 ; i < 16 ; i++ ){
				id[ i ] ^= IdEncryptionTbl[ i ];

			}
			// ?????????
			fwrite( id, sizeof( id ), 1, fp );

			// ??????B?l????????
			for( i = 0 ; i < MAX_PET_KIND ; i++ ){
				int j;
				// ???q?????
				for( j = 0 ; j < PET_NAME_LEN + 1 ; j++ ){
					PetAlbum[ i ].name[ j ] = Rnd( 1, 255 );
					PetAlbum[ i ].freeName[ j ] = Rnd( 1, 255 );
				}

				PetAlbum[ i ].faceGraNo = Rnd( 1, 60000 );
				PetAlbum[ i ].level = Rnd( 1, 30000 );
				PetAlbum[ i ].faceGraNo = Rnd( 1, 30000 );
				PetAlbum[ i ].maxHp = Rnd( 1, 30000 );
				PetAlbum[ i ].str = Rnd( 1, 30000 );
				PetAlbum[ i ].quick = Rnd( 1, 30000 );
				PetAlbum[ i ].def = Rnd( 1, 30000 );

				PetAlbum[ i ].earth = Rnd( 1, 30000 );
				PetAlbum[ i ].water = Rnd( 1, 30000 );
				PetAlbum[ i ].fire = Rnd( 1, 30000 );
				PetAlbum[ i ].wind = Rnd( 1, 30000 );
			}

			// ???????
			fwrite( PetAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND, fp );
			fclose( fp );	// ????????
		}

		return false;
	}
	// ???S??????? ??????????? ?
	while( 1 ){
		// ????????
		if( fread( id2, sizeof( id2 ), 1, fp ) < 1 ){
			// ??????????????????
			// ????????????
			id[ id[ 15 ] ] = Rnd( 1, 100 );
			// ??????
			for( i = 0 ; i < 16 ; i++ ){
				id[ i ] ^= IdEncryptionTbl[ i ];
			}
			// ?????????
			fwrite( id, sizeof( id ), 1, fp );
			// ???????
			fwrite( PetAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND, fp );
			fclose( fp );	// ????????
			return false;
		}
		// ?????????
		for( i = 0 ; i < 16 ; i++ ){
			id2[ i ] ^= IdEncryptionTbl[ i ];
		}
		// ???????????
		id2[ id2[ 15 ] ] = NULL;
		// ??????
		if( strcmp( id, id2 ) != 0 ){
			// ????????????????????????
			if( fseek( fp, sizeof( PET_ALBUM ) * MAX_PET_KIND, SEEK_CUR ) != 0 ){
				fclose( fp );	// ????????
				return false;
			}
		}else{// ????
			// ????????????
			if( fread( PetAlbum, sizeof( PET_ALBUM ), MAX_PET_KIND, fp ) < MAX_PET_KIND ){
				fclose( fp );	// ????????
				return false;
			}
			break;
		}
		// ??????
		AlbumIdCnt++;
	}
	// ????????
	fclose( fp );

	return true;
#endif
}

// ???????????^???M?? **************************************************/
void InitOhtaParam( void )
{
	LoadMailHistory();					// ????????????
	LoadChatHistoryStr();				// ????????????????
	//LoadReadNameShield();
	//LoadReadSayShield();
	mailLamp = CheckMailNoReadFlag();	// ?t??????
	StatusUpPoint = 0;					// ???????????????
	battleResultMsg.useFlag = 0;		// ?P?f????????
	MapWmdFlagBak = 0;
}

// ????t?????? ****************************************************************/
bool CheckMailNoReadFlag( void )
{
	int i,j;

	// ????????
	for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
		// ???????
		for( j = 0 ; j < MAIL_MAX_HISTORY ; j++ ){
			// ?t?????????
			if( MailHistory[ i ].noReadFlag[ j ] >= true ) return true;
		}
	}
	return false;
}

// ?????????? ****************************************************************/
void CheckNewPet( int sprNo )
{
	int tblNo = sprNo - 100250; // ??????k?
	int albumNo;
				// ?????k?
#if defined(__ALBUM_47)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_46)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_45)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_44)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_43)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_42)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_41)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_40)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_39)
	if( tblNo > 1800 ){					//��???
		tblNo -= 1208;
	}else if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_38)

	if( tblNo > 1755 ){					//???????????? ????
		tblNo -= 1201;
	}else if (tblNo > 1739 ){ //1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_37)

	if (tblNo > 1710 ){			//	???????? ????
		tblNo -= 1200;
	}else if (tblNo > 1686 ){			//????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_36)

	if (tblNo > 1686 ){			//??????4 ???2	8.0????????????
		tblNo -= 1175;
	}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_35)

	if (tblNo > 1641 ){			//??????4 ???2 8.0????????????
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_34)

	if (tblNo > 1639 ){			//??????4
		tblNo -= 1167;
	}else if (tblNo > 1635 ){	//????
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2 ????
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_33)

	if (tblNo > 1635 ){
		tblNo -= 1148;
	}else if ( tblNo > 1634 ){	//��?1 ��?2 ????
		tblNo -= 1149;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_32)

	if ( tblNo > 1634 ){	//��?1 ��?2
		tblNo -= 1149;
	}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
		tblNo -= 1147;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_31)

	if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
		tblNo -= 1147;
	}else if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_30)

	if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_29)

	if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_28)

	if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ??????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_27)

	if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ?????????? ???1 ???2
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_26)

	if ( tblNo > 1616 ){//????????  ???1 ???2 ????1 ????2 ??????????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_25)
	if( tblNo > 1616 ){//????????
		tblNo -= 1146;
	}else if ( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_24)
	if( tblNo > 1568 ){//??????
		tblNo -= 1103;
	}else if( tblNo > 1564 ){//???????
		tblNo -= 1101;
	}else if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_23)
	if( tblNo > 1516 ){
		tblNo -= 1055;
	}else if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_22)
	if ( tblNo == 1516 ){//??????????
	    tblNo = 455;
	}else if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_21)
	if ( tblNo > 1509 ){
	    tblNo -= 1054;
	}else if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_20)
	if ( tblNo == 1497 ){
	    tblNo = 395;
	}else if ( tblNo > 1495 ){
        tblNo -= 1044 ;
	}else if ( tblNo > 1490 ){
	    tblNo -= 1043 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_19)
	if ( tblNo > 1490 ){
	    tblNo -= 1044 ;
	}else if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_18)
	if ( tblNo > 1485 ){
		tblNo -= 1040 ;
	}else if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_17)
	if ( tblNo > 1454 ) {
		tblNo -= 1034 ;
	}else if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}

#elif defined(__ALBUM_16)
	if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#elif defined(__ALBUM_15)
	if ( tblNo == 1214 ) {
		tblNo = 331 ;
	}else if ( tblNo == 1217 ) {
		tblNo = 328 ;
	}else if ( tblNo > 1361){
		tblNo -= 952;
	}else if( tblNo > 1356){
		tblNo -= 999;
	}else if( tblNo > 1281){
		tblNo -= 919;
	}else if( tblNo > 1239){
		tblNo -= 889;
	}else if( tblNo > 1186 ){
		tblNo -= 886;
	}else if( tblNo > 1173 ){
		tblNo -= 877;
	}else if( tblNo > 1172){
		tblNo -= 877;
	}else if( tblNo > 1161){
		tblNo -= 868;
	}else if( tblNo > 930){
		tblNo -= 745;
	}else if( tblNo == 927){
		tblNo -= 634;
	}else if( tblNo > 929){
		tblNo -= 644;
	}else if( tblNo > 927){
		tblNo -= 643;
	}else if( tblNo > 900){
		tblNo -= 621;
	}else if( tblNo > 180){
		tblNo -= (333+56);
	}
#endif
	// ????????k?????
	if( 0 <= tblNo && tblNo < MAX_PET_TBL ){
		// ???K?????????k?
		albumNo = PetAlbumTbl[ tblNo ].albumNo;
		// ????????
		if( albumNo != -1 ){
			// ???????????
			if( PetAlbum[ albumNo ].flag == 0 ){
				// ???????
				PetAlbum[ albumNo ].flag = 1;
				// ???q???
				strcpy( PetAlbum[ albumNo ].name, PetAlbumTbl[ tblNo ].name );
				// ???????V
				SaveAlbum( albumNo );
			}
		}
	}
}

// ?P?f???????????? **********************************************************/
void CheckBattleNewPet( void )
{
	int i;

	// ???l?????
	for( i = 0 ; i < 20 ; i++ ){
		// ????????????
		if( p_party[ i ] == NULL ) continue;
		// ??????????
		if( p_party[ i ]->func == NULL ) continue;
		// ???????????
		CheckNewPet( p_party[ i ]->anim_chr_no );
	}
}

// ???????????? *****************************************************/
void CenteringStr( char *inStr, char *outStr, int max )
{
	int len, space, amari;

	// ???????
	len = strlen( inStr );

	// ????????
	if( len >= max ){
		// ???????
		strncpy( outStr, inStr, max );
		outStr[ max ] = NULL;	// ???Z??????
		return;
	}

	// ???????
	amari= ( max - len ) % 2;
	// ???????
	space = ( max - len ) / 2;

	// ?????????
	if( space != 0 ){
		// ???????
		sprintf( outStr, "%*c%s%*c", space, ' ', inStr, space + amari, ' ' );
	}else{
		// ???????
		sprintf( outStr, "%s ", inStr );
	}
}

// ???e????????????????? *************************************/
int CheckPetSkill( int skillId )
{
	int i, j;

	// ??????????
	for( j = 0 ; j < MAX_PET ; j++ ){
		// ???????????
		if( pet[ j ].useFlag == true ){
			// ???????
			for( i = 0 ; i < pet[ j ].maxSkill ; i++ ){
				// ????????
				if( petSkill[ j ][ i ].useFlag == true ){
					// ??????
					if( petSkill[ j ][ i ].skillId == skillId ){
						return true;
					}
				}
			}
		}
	}
	return false;
}

// ?????????? *******************************************************/
void InitItem( int x, int y , bool bPetItemFlag)
{
	int i, j = 0, k = 0;

#ifdef _ITEM_EQUITSPACE
	struct _tagInitXY{
		int x;
		int y;
	}InitXY[CHAR_EQUIPPLACENUM]={
		{x+86,y+34},	{x+86,y+82},	{x+36,y+82},
		{x+136,y+34},	{x+36,y+34},	{x+86,y+130},
						{x+136,y+82},	{x+136,y+130}
#ifdef _EQUIT_NEWGLOVE
		,{x+36,y+130}
#endif
	};
#ifdef _PET_ITEM
	if (bPetItemFlag)		// ?????????????????
	{
		// seting x
		InitXY[0].x = InitXY[5].x = x + 63;
		InitXY[1].x = InitXY[6].x = x + 113;
		InitXY[2].x = x + 36;
		InitXY[3].x = x + 87;
		InitXY[4].x = x + 138;
		// seting y
		InitXY[0].y = InitXY[1].y = y + 34;
		InitXY[2].y = InitXY[3].y = InitXY[4].y = y + 83;
		InitXY[5].y = InitXY[6].y = y + 132;
	}
#endif
	for ( i=0; i<MAX_ITEMSTART; i++)
	{
		ItemBuffer[ i ].defX = InitXY[i].x;
		ItemBuffer[ i ].defY = InitXY[i].y;
		ItemBuffer[ i ].x = ItemBuffer[ 0 ].defX;
		ItemBuffer[ i ].y = ItemBuffer[ 0 ].defY;
		ItemBuffer[ i ].bmpNo = 20000 + 0;
		ItemBuffer[ i ].dispPrio = DISP_PRIO_ITEM;
	}
#else
	ItemBuffer[ 0 ].defX = x + 136;
	ItemBuffer[ 0 ].defY = y + 59;
	ItemBuffer[ 0 ].x = ItemBuffer[ 0 ].defX;
	ItemBuffer[ 0 ].y = ItemBuffer[ 0 ].defY;
	ItemBuffer[ 0 ].bmpNo = 20000 + 0;
	ItemBuffer[ 0 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 1 ].defX = x + 134;
	ItemBuffer[ 1 ].defY = y + 129;
	ItemBuffer[ 1 ].x = ItemBuffer[ 1 ].defX;
	ItemBuffer[ 1 ].y = ItemBuffer[ 1 ].defY;
	ItemBuffer[ 1 ].bmpNo = 20200 + 1;
	ItemBuffer[ 1 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 2 ].defX = x + 61;
	ItemBuffer[ 2 ].defY = y + 129;
	ItemBuffer[ 2 ].x = ItemBuffer[ 2 ].defX;
	ItemBuffer[ 2 ].y = ItemBuffer[ 2 ].defY;
	ItemBuffer[ 2 ].bmpNo = 20400 + 2;
	ItemBuffer[ 2 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 3 ].defX = x + 32;
	ItemBuffer[ 3 ].defY = y + 68;
	ItemBuffer[ 3 ].x = ItemBuffer[ 3 ].defX;
	ItemBuffer[ 3 ].y = ItemBuffer[ 3 ].defY;
	ItemBuffer[ 3 ].bmpNo = 20600 + 3;
	ItemBuffer[ 3 ].dispPrio = DISP_PRIO_ITEM;

	ItemBuffer[ 4 ].defX = x + 83;
	ItemBuffer[ 4 ].defY = y + 68;
	ItemBuffer[ 4 ].x = ItemBuffer[ 4 ].defX;
	ItemBuffer[ 4 ].y = ItemBuffer[ 4 ].defY;
	ItemBuffer[ 4 ].bmpNo = 20800 + 4;
	ItemBuffer[ 4 ].dispPrio = DISP_PRIO_ITEM;
#endif

	for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
		ItemBuffer[ i ].defX = x + 32 + j;
		ItemBuffer[ i ].defY = y + 56 + 48 + 48 + 48 + k;
		ItemBuffer[ i ].x = ItemBuffer[ i ].defX;
		ItemBuffer[ i ].y = ItemBuffer[ i ].defY;
		ItemBuffer[ i ].bmpNo = 20000 + j / 48;
		ItemBuffer[ i ].dispPrio = DISP_PRIO_ITEM;
		ItemBuffer[ i ].mixFlag = false;
		j += 51;
		if( j >= 48 * 5 ){
			j = 0;
			k += 48;
		}
	}
}

// CoolFish: Trade 04/14
void InitItem3( int x, int y )
{
	int i, j = 0, k = 0;
	// ????????
	for( i = MAX_ITEMSTART ; i < MAX_ITEM ; i++ ){
		ItemBuffer[ i ].defX = x + 32 + j;
		ItemBuffer[ i ].defY = y + 56 + 48 + 48 + 48 -160 + k;
		ItemBuffer[ i ].x = ItemBuffer[ i ].defX;
		ItemBuffer[ i ].y = ItemBuffer[ i ].defY;
		ItemBuffer[ i ].bmpNo = 20000 + j / 48;
		ItemBuffer[ i ].dispPrio = DISP_PRIO_ITEM;
		j += 51; // ???????
		// ????????
		if( j >= 48 * 5 ){
			j = 0;
			k += 48; // ???????
		}
	}
}
// CoolFish: End

// ????R?e??????? ******************************************************/
bool MakeHitBox( int x1, int y1, int x2, int y2, int dispPrio )
{
	// ????????R?e
	if( mouse.nowPoint.x <= x2 && x1 <= mouse.nowPoint.x &&
		mouse.nowPoint.y <= y2 && y1 <= mouse.nowPoint.y ){
		// ???????????
		if( dispPrio >= 0 ){
			// ????????????????????
			StockBoxDispBuffer( x1, y1, x2, y2, dispPrio, BoxColor, 0 );
		}
		return true;
	}
	return false;
}

// ????????????? **************************************************************/
void DeathLetterAction( void )
{
	int i;

	for( i = 0 ; i < 4 ; i++ ){
		DeathAction( pActLetter[ i ] );
		pActLetter[ i ] = NULL;
	}
}

// ???????????? ***************************************************************/
void InitMailSendFlag( void )
{
	int i;

	for( i = 0 ; i < MAX_ADR_BOOK ; i++ ) mailWndSendFlag[ i ] = 0;
}

// ???????????????D? *****************************************************/
void DeathMenuAction( void )
{
	// ????????
	DeathAction( pActMenuWnd );
	pActMenuWnd = NULL;


#ifdef _FRIENDCHANNEL			//ROG ADD ???????
	chatRoomBtn = 0;
	assentFlag = false;
	DeathAction( pAssentWnd );
	pAssentWnd = NULL;
	setRoomFlag = false;
	DeathAction( pSetRoomWnd );
	pSetRoomWnd = NULL;
#endif

	// ???????q????????
	DeathAction( pActMenuWnd3 );
	pActMenuWnd3 = NULL;
	// ?V????????
	GetKeyInputFocus( &MyChatBuffer );
	// ??????
	DeathAction( pActPet );
	pActPet = NULL;
	// ??????????
	DeathAction( pActMailItem );
	pActMailItem = NULL;
	// ??_????????
	DeathAction( pActYesNoWnd );
	pActYesNoWnd = NULL;
	// ?????????????
	DeathLetterAction();

	// Robin 04/14 trade
	if( pActMenuWnd4){
		DeathAction( pActMenuWnd4 );
		pActMenuWnd4 = NULL;
	}
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
	//????????Action
	if( SecondTradeWnd){
		DeathAction( SecondTradeWnd );
		SecondTradeWnd = NULL;
	}
	if( SecondActPet){
		DeathAction( SecondActPet );
		SecondActPet = NULL;
	}
	if( TradeTalkWnd){
		DeathAction( TradeTalkWnd );
		TradeTalkWnd = NULL;
	}
#endif
	if( pActPet3){
		DeathAction( pActPet3 );
		pActPet3 = NULL;
	}

	if( pActPet4){
		DeathAction( pActPet4 );
		pActPet4 = NULL;
	}
	if( pActPet5){
		DeathAction( pActPet5 );
		pActPet5 = NULL;
	}
	if( pActMenuWnd5){
		DeathAction( pActMenuWnd5 );
		pActMenuWnd5 = NULL;
	}
}
// ???????????????D? *****************************************************/
void DeathMenuAction2( void )
{
	// ????????
	DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
	GetKeyInputFocus( &MyChatBuffer );
	Moneyflag = false;
#endif
	pActMenuWnd2 = NULL;
	// ?????????
	DeathAction( pActPet2 );
	pActPet2 = NULL;
	// ???????????k????
	ItemMixPetNo = -1;

	// Robin 04/14 trade
	DeathAction( pActMenuWnd4 );
#ifdef _FRIENDCHANNEL			//ROG ADD ???????
	chatRoomBtn = 0;
	assentFlag = false;
	DeathAction( pAssentWnd );
	pAssentWnd = NULL;
	setRoomFlag = false;
	DeathAction( pSetRoomWnd );
	pSetRoomWnd = NULL;
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
	//????????Action
	DeathAction( SecondTradeWnd );
	SecondTradeWnd = NULL ;
	DeathAction( SecondActPet ) ;
	SecondActPet = NULL ;
	DeathAction( TradeTalkWnd ) ;
	TradeTalkWnd = NULL ;
#endif
	//????????????څ
	Tradeflag = false;
	pActMenuWnd4 = NULL;
	DeathAction( pActPet3 );
	pActPet3 = NULL;
	DeathAction( pActPet4 );
	pActPet4 = NULL;
	DeathAction( pActPet5 );
	pActPet5 = NULL;
	DeathAction( pActMenuWnd5 );
	pActMenuWnd5 = NULL;

}

void onSwitchAutoBattleClicked()
{
	if (AI == AI_SELECT)
	{
		AI = AI_NONE;
	}
	else
	{
		AI = AI_SELECT;
	}
	extern bool AI_Save();
	AI_Save();
}

// ?????????? *******************************************************/
void InitMenu( void )
{

	int i;
	mergeItemFlag = false;
	// ????
	for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
	// ???
	for( i = 0 ; i < MENU_MAP_0 ; i++ ) mapWndFontNo[ i ] = -2;
	mapWndNo = 0;
	// ?????
	for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
	for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;

	// Robin 04/14 trade
	for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndFontNo[ i ] = -2;
	for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndBtnFlag[ i ] = 0;

	// ???
	for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
	petWndNo = 0;
	// ????
	for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndFontNo[ i ] = -2;
	for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndBtnFlag[ i ] = 0;
	itemWndNo = 0;
	// ???
	for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
	for( i = 0 ; i < MAX_ADR_BOOK ; i++ ) mailWndSendFlag[ i ] = 0;
	mailWndNo = MAIL_WND_VIEW;
	// ????
	for( i = 0 ; i < MENU_ALBUM_0 ; i++ ) albumWndFontNo[ i ] = -2;
	albumWndNo = 0;
	// ?????
	for( i = 0 ; i < TASK_BAR ; i++ ) taskBarFontNo[ i ] = -2;
	// ?P?f???????
	for( i = 0 ; i < MENU_BTL_RESULT_0 ; i++ ) resultWndFontNo[ i ] = -2;
	// ??????
	MenuToggleFlag = 0;
	#ifdef _TELLCHANNEL				//ROG ADD ???????
	MultiTells = false;
	#endif

	// ???????????????D?
	DeathMenuAction();
	// ???????????????D?
	DeathMenuAction2();
	// ???????????
	mouse.itemNo = -1;
	// ?????k???k????
	itemNo = -1;
	// ?k?????????????
	jujutuNo = -1;
	// ???????k????
	ItemMixPetNo = -1;
#ifdef __AI
			void AI_CloseWnd();
			AI_CloseWnd();
#endif
}

// ??????????? *******************************************************/
void InitMenu2( void )
{
	// ??????
	MenuToggleFlag = 0;
	// ???????????????D?
	DeathMenuAction();
	// ???????????????D?
	DeathMenuAction2();
	// ???????????
	mouse.itemNo = -1;
	// ?????k???k????
	itemNo = -1;
	// ?k?????????????
	jujutuNo = -1;
	// ?P?f???????????
	BattleResultWndFlag = false;

}

// ????????????? *******************************************************/
bool CheckMenuFlag( void )
{
	bool flag = false;

	// ??????
	if( MenuToggleFlag != 0 ) flag = true;
	// ?P?f???????????
	if( BattleResultWndFlag >= 1 ) flag = true;
	// ?????????????????
	if( MapWmdFlagBak >= true ) flag = true;
#ifdef _FRIENDCHANNEL
	if( assentFlag ) flag = assentFlag;
	if( setRoomFlag ) flag = setRoomFlag;
#endif

	return flag;
}

#ifdef __PHONEMESSAGE
void CleanSMS()
{
	extern STR_BUFFER ptext;
	extern HANDLE MHandle;
	ptext.buffer[0]='\0';
	if(MHandle)
	{
		TerminateThread(MHandle,1);
		CloseHandle(MHandle);
	}
}
#endif

// ****************************************************************************/
// ?????? ***************************************************************/
// ****************************************************************************/
#ifdef _AI_OTHER
extern int AI_Other_State;
extern void AI_OtherProc();
#endif
void MenuProc( void )
{
	int x, y, i;
	x = y = i = 0;
	char moji[ 256 ];
#ifdef __AI
	extern int AI_State;
	if( AI_State){
		extern bool AI_SettingProc();
		if( AI_SettingProc()){
			AI_State = 0;
			extern short actBtn;
			actBtn = 0;
		}
	}
#endif
#ifdef _AI_OTHER
	if(AI_Other_State) AI_OtherProc();
#endif

#ifdef _TELLCHANNEL				//ROG ADD ???????
	if(MultiTells == true)
	{
		SelectChar();
		if( MenuToggleFlag & JOY_ESC )
			DeathTellChannel();
	}
#endif



#ifdef _TIMEBAR_FUNCTION
	if(timeBarFlag == true)
	{
		DrawTimeBar();
	}
#endif

	// ??????????????
#ifdef __AI
	if( joy_trg[ 0 ] & JOY_ESC && checkFieldMenuFlag() == false && !AI_State){
#else
	if( joy_trg[ 0 ] & JOY_ESC && checkFieldMenuFlag() == false ){
#endif
		// ?????????????????
		if( /*GetImeString() == NULL*/ 0){
			// ???????
			if( MenuToggleFlag != 0 || BattleResultWndFlag >= 1 ){
				MenuToggleFlag = 0;
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ???????????
				mouse.itemNo = -1;
				// ?????k???k????
				itemNo = -1;
				// ?k?????????????
				jujutuNo = -1;
				// ??????????????
				if( MapWmdFlagBak != true ){
					// ??????????
					play_se( 203, 320, 240 );
				}
			}else{  // ??????????
				if( /*GetImeString() == NULL*/ 1){

					MenuToggleFlag ^= JOY_ESC;	// ?????
					// ????????
					play_se( 202, 320, 240 );
					// ????
					for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
					systemWndNo = 0;		// ??????k????

					// ????????
					if( MenuToggleFlag & JOY_ESC ){
						MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
						MenuToggleFlag |= JOY_ESC;	// ?????
						// ?P?f???????????
						BattleResultWndFlag = false;
						// ????????
						play_se( 202, 320, 240 );
					}else{
						// ??????????
						play_se( 203, 320, 240 );
					}
				}
			}
			// ????????????
			DeathMenuAction();
			DeathMenuAction2();
			// ????????
			saveUserSetting();
		}
	}
	// ??????????????
#ifdef __AI
	if( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 3 ] && !AI_State){
#else
	if( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 3 ] ){
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 )
			play_se( 220, 320, 240 );
		else {
#endif
		MenuToggleFlag ^= JOY_ESC;	// ?????
		// ????
		for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
		systemWndNo = 0;		// ??????k????
		// ????????????
		DeathMenuAction();
		// ????????
		saveUserSetting();

		// ????????
		if( MenuToggleFlag & JOY_ESC ){
			MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
			MenuToggleFlag |= JOY_ESC;	// ?????
			// ?P?f???????????
			BattleResultWndFlag = false;
			// ????????
			play_se( 202, 320, 240 );
		}else{
			// ??????????
			play_se( 203, 320, 240 );
		}
#ifdef _BATTLESKILL
		}
#endif
	}

	// ??????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_S /*&& GetImeString() == NULL*/)
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 1 ] ) ) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_S && GetImeString() == NULL )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 1 ] ) ){
#endif
		// ?P?f???
		if( ProcNo == PROC_BATTLE ){
			// ???
			play_se( 220, 320, 240 );
		}else{

			MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
			// ?????
			for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
			for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
			statusWndNo = 0;
			// ????????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if( MenuToggleFlag & JOY_CTRL_S ){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
				MenuToggleFlag |= JOY_CTRL_S;	// ??????????
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ??????????
				play_se( 203, 320, 240 );
			}
		}
	}

// Nuke 0413: Trade
#ifdef __AI
	if ( !AI_State && joy_trg[ 0 ] & JOY_CTRL_T /*&& GetImeString() == NULL*/) {
#else
	if ( joy_trg[ 0 ] & JOY_CTRL_T && GetImeString() == NULL ) {
#endif
		// ?P?f???
		if( ProcNo == PROC_BATTLE ){
			// ???
			play_se( 220, 320, 240 );
		}else{
			MenuToggleFlag ^= JOY_CTRL_T;	// CTRL + T ??

			for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndFontNo[ i ] = -2;
			for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndBtnFlag[ i ] = 0;

			tradeWndNo = 0;

			tradeInit();

			DeathMenuAction();
			DeathMenuAction2();
			// ????????
			saveUserSetting();

			// ????????
			if( MenuToggleFlag & JOY_CTRL_T ){
				MenuToggleFlag &= 0;
				MenuToggleFlag |= JOY_CTRL_T;	// ??????????
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ??????????
				play_se( 203, 320, 240 );
			}
		}
	}

	// ????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_P /*&& GetImeString() == NULL*/ )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 2 ] ) ) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_P && GetImeString() == NULL )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 2 ] ) ){
#endif
		// ?P?f?????????????????
		if( ProcNo == PROC_BATTLE ){
			// ???
			play_se( 220, 320, 240 );
		}else{
			{
				MenuToggleFlag ^= JOY_CTRL_P;	// CTRL + P ??
				// ???
				for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
				petWndNo = 0;		// ??????k????

				// ????????????
				DeathMenuAction();
				// ????????
				saveUserSetting();

				// ????????
				if( MenuToggleFlag & JOY_CTRL_P ){
					MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
					MenuToggleFlag |= JOY_CTRL_P;
					// ?P?f???????????
					BattleResultWndFlag = false;
					// ????????
					play_se( 202, 320, 240 );
				}else{
					// ??????????
					play_se( 203, 320, 240 );
				}
			}
		}
	}
	// ?????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_I /*&& GetImeString() == NULL*/ )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 4 ] ) ) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_I && GetImeString() == NULL )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 4 ] ) ){
#endif
		// ?P?f???
		if( ProcNo == PROC_BATTLE ){
			// ???
			play_se( 220, 320, 240 );
		}else{
#ifdef _STREET_VENDOR
			if(pc.iOnStreetVendor == 1 || sStreetVendorBuyBtn == 2) StockChatBufferLine("????��?????????",FONT_PAL_RED);
			else
#endif

			{
				MenuToggleFlag ^= JOY_CTRL_I;	// CTRL + I ??
				// ?????????k????
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndBtnFlag[ i ] = 0;
				itemWndNo = 0;
				mouse.itemNo = -1;
				itemWndDropGold = 0;

				// ????????????
				if( BattleResultWndFlag >= 1 ) DeathMenuAction();
				// ????????????
				DeathMenuAction2();
				// ????????
				saveUserSetting();
				// ????????
				if( MenuToggleFlag & JOY_CTRL_I ){
					MenuToggleFlag &= ~JOY_CTRL_M;	// ?????????????
					MenuToggleFlag &= ~JOY_CTRL_T;
					MenuToggleFlag &= ~JOY_B;
					MenuToggleFlag |= JOY_CTRL_I;
					// ?P?f???????????
					BattleResultWndFlag = false;
					// ?????????
					MapWmdFlagBak = false;
					// ????????
					play_se( 202, 320, 240 );

				}else{
					// ??????????
					play_se( 203, 320, 240 );
				}
			}
		}
	}

	// ????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_M /*&& GetImeString() == NULL*/ )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 0 ] )
		|| ( MapWmdFlagBak == true && BattleResultWndFlag == false && EncountFlag == false && ProcNo == PROC_GAME && SubProcNo == 3 )
		) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_M && GetImeString() == NULL )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 0 ] )
		|| ( MapWmdFlagBak == true && BattleResultWndFlag == false && EncountFlag == false && ProcNo == PROC_GAME && SubProcNo == 3 )
		){
#endif
		// ?P?f???
		if( ProcNo == PROC_BATTLE ){
			// ???
			play_se( 220, 320, 240 );
		}else{

			MenuToggleFlag ^= JOY_CTRL_M;	// CTRL + M ??
			// ???
			for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
			mapWndNo = 0;		// ??????k????

			// ????????????
			if( BattleResultWndFlag >= 1 ) DeathMenuAction();
			DeathMenuAction2();
			// ????????
			// ????????
			if( MenuToggleFlag & JOY_CTRL_M ){
				MenuToggleFlag &= ~JOY_CTRL_I;	// ??????????????
				MenuToggleFlag &= ~JOY_CTRL_T;
				MenuToggleFlag &= ~JOY_B;
				MenuToggleFlag |= JOY_CTRL_M;
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ?????????
				MapWmdFlagBak = false;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ??????????
				play_se( 203, 320, 240 );
			}
		}
	}

	// ????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_E /*&& GetImeString() == NULL*/ )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 5 ] ) ) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_E && GetImeString() == NULL )
		|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 5 ] ) ){
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 )
			play_se( 220, 320, 240 );
		else {
#endif
			MenuToggleFlag ^= JOY_CTRL_E;	// CTRL + E ??
			// ???
			for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
			mailWndNo = MAIL_WND_VIEW;	// ??????k????
			mailViewWndPageNo = 0;		// ????????????????k????
			mailItemNo = -1;			// ???????????k????
			InitMailSendFlag();			// ????????????
			// ????????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if( MenuToggleFlag & JOY_CTRL_E ){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
				MenuToggleFlag |= JOY_CTRL_E;
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ??????????
				play_se( 203, 320, 240 );
			}
#ifdef _BATTLESKILL
		}
#endif
		//}
	}

	// ?????????
#ifdef __AI
	if( !AI_State && (( joy_trg[ 0 ] & JOY_CTRL_A /*&& GetImeString() == NULL*/ )
	 	|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 6 ] ) ) ){
#else
	if( ( joy_trg[ 0 ] & JOY_CTRL_A && GetImeString() == NULL )
	 	|| ( TaskBarFlag == true && mouse.onceState & MOUSE_LEFT_CRICK && HitDispNo == taskBarFontNo[ 6 ] ) ){
#endif
#ifdef _BATTLESKILL
		extern int wonflag ;
		if ( wonflag == 1 )
			play_se( 220, 320, 240 );
		else {
#endif
			MenuToggleFlag ^= JOY_CTRL_A;	// CTRL + A ??
			// ???
			for( i = 0 ; i < MENU_ALBUM_0 ; i++ ) albumWndFontNo[ i ] = -2;
			mapWndNo = 0;		// ??????k????

			// ????????????
			DeathMenuAction();
			// ????????
			saveUserSetting();

			// ????????
			if( MenuToggleFlag & JOY_CTRL_A ){
				MenuToggleFlag &= JOY_CTRL_I | JOY_CTRL_M;	// ????????????????????
				MenuToggleFlag |= JOY_CTRL_A;
				// ?P?f???????????
				BattleResultWndFlag = false;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ??????????
				play_se( 203, 320, 240 );
			}
		//}
#ifdef _BATTLESKILL
		}
#endif
	}
	StockDispBuffer( TASK_BAR_X, TASK_BAR_Y, DISP_PRIO_MENU, CG_TASK_BAR_BACK, 1 );
	//?��???? Y????????????
	if( mouse.nowPoint.y >= 456 + DISPLACEMENT_Y ){
		// ??????????
		if( taskBarY > TASK_BAR_Y ) taskBarY--;
		// ??????????
		TaskBarFlag = true;

	}else{
		// ???????????
		if( TaskBarFlag == true ){
			// ??????????
			if( taskBarY < TASK_BAR_Y + 24 ) taskBarY++;
			// ????????R?e???
			if( taskBarY == TASK_BAR_Y + 24 ){
				for( i = 0 ; i < TASK_BAR ; i++ ) albumWndFontNo[ i ] = -2;
				TaskBarFlag = false;
			}
		}
	}

	if( TaskBarFlag == true ){
		x = taskBarX, y = taskBarY;
        char buffer[256];


		taskBarFontNo[ 0 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_MAP_UP 		+ ( ( MenuToggleFlag & JOY_CTRL_M ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 1 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_STATUS_UP 	+ ( ( MenuToggleFlag & JOY_CTRL_S ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 2 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_PET_UP 		+ ( ( MenuToggleFlag & JOY_CTRL_P ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 4 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_ITEM_UP 	+ ( ( MenuToggleFlag & JOY_CTRL_I ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 5 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_MAIL_UP 	+ ( ( MenuToggleFlag & JOY_CTRL_E ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 6 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_ALBUM_UP 	+ ( ( MenuToggleFlag & JOY_CTRL_A ) ? 1 : 0 ), 2 );
		taskBarFontNo[ 3 ] = StockDispBuffer( x, y, DISP_PRIO_IME3, CG_TASK_BAR_SYSTEM_UP 	+ ( ( MenuToggleFlag & JOY_ESC ) ? 1 : 0 ), 2 );
	}

	if( MenuToggleFlag & JOY_ESC ){
		// ??????k?????
		switch( systemWndNo ){

		case 0:	// ???? ??????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 4, 9, CG_WND_TITLE_SYSTEM, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ????????
#ifdef __PHONEMESSAGE
					CleanSMS();
#endif
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 1;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				// ?????@?e??
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 2;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				// ????@?e??
				if( HitFontNo == systemWndFontNo[ 4 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 4;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				// ????@?e??
				if( HitFontNo == systemWndFontNo[ 3 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 3;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				// ?????@?e??
				if( HitFontNo == systemWndFontNo[ 5 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 6;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				//?????
				if( HitFontNo == systemWndFontNo[ 6 ] ){
					// ????????
#ifdef __PHONEMESSAGE
					CleanSMS();
#endif
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 7;
					// ????????
					play_se( 202, 320, 240 );

					break;
				}
				// ?????
				if( HitFontNo == systemWndFontNo[ 2 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					MenuToggleFlag ^= JOY_ESC;
					play_se( 203, 320, 240 );
					break;
				}
#ifdef __AI
				//???????څ
				if( HitFontNo == systemWndFontNo[ 7 ] ){
					// ????????
					extern int AI_State;
					AI_State = 1;
					extern short actBtn;
					actBtn = 1;
					closeEtcSwitch();
					closeJoinChannelWN();
					MenuToggleFlag = 0;
					DeathMenuAction2();
					DeathMenuAction();
					// ????????
					play_se( 202, 320, 240 );
					break;
				}

				if (HitFontNo == systemWndFontNo[8]) {
					closeEtcSwitch();
					closeJoinChannelWN();
					DeathMenuAction2();
					DeathMenuAction();
					MenuToggleFlag = 0;
					onSwitchAutoBattleClicked();
					play_se( 202, 320, 240 );
					break;
				}

#endif

			}
			// ????@?e
			x = pActMenuWnd->x + 28;

			// ???????????????
#ifdef __AI
			y = pActMenuWnd->y + 54;
#ifdef _CHAR_NEWLOGOUT
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    回记录点    ", 2, 0 );	y += 34;//y += 40;
#else
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    ��   ��   ", 2, 0 );	y += 34;//y += 40;
#endif
			systemWndFontNo[ 6 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    原地登出    ", 2, 0 );	y += 34;//y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    聊天设定    ", 2, 0 );	y += 34;//y += 40;
			systemWndFontNo[ 4 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"  背景音乐设定  ", 2 , 0);	y += 34;//y += 40;
			systemWndFontNo[ 3 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    音效设定    ", 2, 0 );	y += 34;//y += 40;
			systemWndFontNo[ 5 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"    滑鼠设定    ", 2, 0 );	y += 34;//y += 52;
			systemWndFontNo[ 7 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"  自动战斗开关  ", 2, 0 );	y += 34;//y += 40;
			systemWndFontNo[ 8 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"  自动战斗设定  ", 2, 0 );	y += 34;//y += 40;

#else
			y = pActMenuWnd->y + 56;
			systemWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    ??????    ", 2 );	y += 36;//y += 40;
			systemWndFontNo[ 6 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    ?????    ", 2 );	y += 36;//y += 40;
			systemWndFontNo[ 1 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    ?????څ    ", 2 );	y += 36;//y += 40;
			systemWndFontNo[ 4 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"  ?????????څ  ", 2 );	y += 36;//y += 40;
			systemWndFontNo[ 3 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    ??��?څ    ", 2 );	y += 36;//y += 40;
			systemWndFontNo[ 5 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"    ?????څ    ", 2 );	y += 40;//y += 52;
#endif
			systemWndFontNo[ 2 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 	"     关       闭      ", 2, 0 );

			break;

		case 1:	// ????? ??????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 3, 3, CG_WND_TITLE_LOGOUT, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){

				// ????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
#ifdef _CHAR_NEWLOGOUT
					systemWndNo = 0;
					lssproto_CharLogout_send( sockfd, 1);
#else
					GameState = GAME_LOGIN;
					ChangeProc2( PROC_CHAR_LOGOUT );
#endif
					play_se( 206, 320, 240 );
					break;
				}
				// ?????
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ????????
					systemWndNo = 0;
					play_se( 203, 320, 240 );
					break;
				}
			}

			// ????@?e
			x = pActMenuWnd->x + 28;
			y = pActMenuWnd->y + 56;

			// ???????????????
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     确   定     ", 2, 0 );	y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     不   要     ", 2, 0 );	y += 40;
			break;

		case 2:	// ??????@?e ??????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ?????????????
#ifndef _TALK_WINDOW
				pActMenuWnd = MakeWindowDisp( 4, 4, 4, 8, CG_WND_TITLE_CHAT, 1 );
#else
				pActMenuWnd = MakeWindowDisp( 4, 4, 4, 9, CG_WND_TITLE_CHAT, 1 );
#endif
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ?????????????
			if( mouse.autoState & MOUSE_LEFT_CRICK ){
				// ???????????  ????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ?????
					NowMaxChatLine++;
					// ????????
					if( NowMaxChatLine > MAX_CHAT_LINE ){
						NowMaxChatLine = MAX_CHAT_LINE;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
				// ???????????  ????
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ?????
					NowMaxChatLine--;
					// ????????
					if( NowMaxChatLine <= -1 ){
						NowMaxChatLine = 0;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
				// ??????????@?e??
				if( HitFontNo == systemWndFontNo[ 2 ] ){
					// ????
					MyChatBuffer.color++;
					// ????????
					if( MyChatBuffer.color >= 10 ) MyChatBuffer.color = 0;
					// ?????
					play_se( 217, 320, 240 );
				}

				// ????K?
				if( HitFontNo == systemWndFontNo[ 4 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					systemWndNo = 5;
					// ????????
					play_se( 202, 320, 240 );
					break;
				}
				// ???????????  ????
				if( HitFontNo == systemWndFontNo[ 5 ] ){
					// ???????
					NowMaxVoice++;
					// ????????
					if( NowMaxVoice > MAX_VOICE ){
						NowMaxVoice = MAX_VOICE;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
				// ???????????  ????
				if( HitFontNo == systemWndFontNo[ 6 ] ){
					// ???????
					NowMaxVoice--;
					// ????????
					if( NowMaxVoice <= 0 ){
						NowMaxVoice = 1;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
#ifdef _TALK_WINDOW
				if(HitFontNo == systemWndFontNo[7]){
					g_bTalkWindow = !g_bTalkWindow;
					if(!WindowMode && g_bTalkWindow){
						StockChatBufferLine("??????????????????",FONT_PAL_RED);
						g_bTalkWindow = false;
					}
					play_se(217,320,240);
					if(g_bTalkWindow) TalkWindow.Create();
					else TalkWindow.Visible(false);
				}
#endif
				// ???
				if( HitFontNo == systemWndFontNo[ 3 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ?????@?e????????
					systemWndNo = 0;
					// ??????????
					play_se( 203, 320, 240 );
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ????@?e
			x = pActMenuWnd->x + 48 - 5;
			y = pActMenuWnd->y + 56;
			sprintf( moji,"�?前现实的行数�?%3d�?",NowMaxChatLine );
			StockFontBufferUtf( x - 16, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0, 0 );				y += 32;
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 					"      增     加      ", 2 , 0);	y += 32;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 					"      减     少      ", 2 , 0);	y += 32;
			systemWndFontNo[ 2 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, MyChatBuffer.color, 	"     改变文字颜色    ", 2 , 0);	y += 32;
			systemWndFontNo[ 4 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0				, 	"      记录文字      ", 2, 0 );	y += 44;

			sprintf( moji,"�?前的音量�?%3d ",NowMaxVoice );
			StockFontBufferUtf( x - 16, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0, 0 );				y += 32;
			systemWndFontNo[ 5 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 					"     增     加     ", 2, 0 );	y += 32;
			systemWndFontNo[ 6 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 					"     减     少     ", 2 , 0);	y += 44;

#ifdef _TALK_WINDOW
			StockFontBuffer(x - 16,y,FONT_PRIO_FRONT,FONT_PAL_YELLOW,"??????????څ??",0);y += 32;
			systemWndFontNo[7] = StockFontBuffer(x + 20,y,FONT_PRIO_FRONT,0,g_bTalkWindow ? "??????????":"?????????",2);y += 32;
#endif

			systemWndFontNo[ 3 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 		"     回上一页     ", 2 ,0);	y += 40;

			break;

		case 3:	// ?????@?e ??????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 4, 6, CG_WND_TITLE_SE, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ?????????????
			if( mouse.autoState & MOUSE_LEFT_CRICK ){
				// ?????????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ?????
					t_music_se_volume++;
					// ????????
					if( t_music_se_volume > 15 ){
						t_music_se_volume = 15;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
				// ?????????
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ?????
					t_music_se_volume--;
					// ????????
					if( t_music_se_volume < 0 ){
						t_music_se_volume = 0;
						// ???
						play_se( 220, 320, 240 );
					}else{
						// ?????
						play_se( 217, 320, 240 );
					}
				}
				// ??????????????v???
				if( HitFontNo == systemWndFontNo[ 2 ] ){
					// ???????????
					stereo_flg = !stereo_flg;
					// ?????
					play_se( 217, 320, 240 );
				}
			}
			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???
				if( HitFontNo == systemWndFontNo[ 3 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ?????@?e????????
					systemWndNo = 0;
					// ??????????
					play_se( 203, 320, 240 );
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ????@?e
			x = pActMenuWnd->x + 48 - 5;
			y = pActMenuWnd->y + 64;
			sprintf( moji,"   目前的音量%3d   ",t_music_se_volume );
			StockFontBufferUtf( x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 , 0);				y += 40;
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     增      加     ", 2, 0 );	y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     减      少     ", 2, 0 );	y += 40;
			systemWndFontNo[ 2 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, monoStereoStr[ stereo_flg ], 2, 0 );	y += 52;
			systemWndFontNo[ 3 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 	"      回上一页      ", 2, 0 );	y += 40;

			break;

		case 4:	// ?????@?e ??????
			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 4, 8, CG_WND_TITLE_BGM, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ?????????????
			if( mouse.autoState & MOUSE_LEFT_CRICK ){
				// ?????????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ?????
					t_music_bgm_volume++;
					
					// ????????
					if( t_music_bgm_volume > 15 ){
						t_music_bgm_volume = 15;
						// ???
						play_se( 220, 320, 240 );
					}else{
						play_se( 217, 320, 240 );	// ?????
						bgm_volume_change();		// ?????
					}

					
				}
				// ?????????
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ?????
					t_music_bgm_volume--;
					// ????????
					if( t_music_bgm_volume < 0 ){
						t_music_bgm_volume = 0;
						// ???
						play_se( 220, 320, 240 );
					}else{
						play_se( 217, 320, 240 );	// ?????
						bgm_volume_change();		// ?????
					}
				}
				// ?????L????
				if( HitFontNo == systemWndFontNo[ 3 ] ){
					// ??????
					t_music_bgm_pitch[t_music_bgm_no]++;
					set_gbm_pitch();
					// ????????
					if( t_music_bgm_pitch[t_music_bgm_no] > 8 ){
						t_music_bgm_pitch[t_music_bgm_no] = 8;
						// ???
						play_se( 220, 320, 240 );
					}else{
						play_se( 217, 320, 240 );	// ?????
						set_gbm_pitch();			// ??????
					}
				}
				// ?????w???
				if( HitFontNo == systemWndFontNo[ 4 ] ){
					// ??????
					t_music_bgm_pitch[t_music_bgm_no]--;
					// ????????
					if( t_music_bgm_pitch[t_music_bgm_no] < -8 ){
						t_music_bgm_pitch[t_music_bgm_no] = -8;
						// ???
						play_se( 220, 320, 240 );
					}else{
						play_se( 217, 320, 240 );	// ?????
						set_gbm_pitch();			// ??????
					}
				}
			}
			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???
				if( HitFontNo == systemWndFontNo[ 2 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ?????@?e????????
					systemWndNo = 0;
					// ??????????
					play_se( 203, 320, 240 );
					// ????????
					saveUserSetting();
					break;
				}
			}
			// ????@?e
			x = pActMenuWnd->x + 48 - 5;
			y = pActMenuWnd->y + 64;
			sprintf( moji,"   目前的音量%3d   ",t_music_bgm_volume );
			StockFontBufferUtf( x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 , 0);				y += 40;
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"      增      加     ", 2, 0 );	y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"	   减      少     ", 2, 0 );	y += 48;
			if( t_music_bgm_pitch[t_music_bgm_no] == 0 ){
				sprintf( moji,"   目前的节奏：      0" );
			}
			else
				sprintf( moji,"   目前的节奏： %+3d   ",t_music_bgm_pitch[t_music_bgm_no] );
			StockFontBufferUtf( x - 8, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 , 0);				y += 40;
			systemWndFontNo[ 3 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"      加      快      ", 2 , 0);	y += 40;
			systemWndFontNo[ 4 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"      减      慢      ", 2 , 0);	y += 52;
			systemWndFontNo[ 2 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 	"      回上一页      ", 2, 0 );	y += 40;

			break;

		case 5:	// ????K???

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 430, CG_WND_TITLE_CHAT, -1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				// ?V?????????
				GetKeyInputFocus( &chatRegistryStr[ 0 ] );

				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ????K??????????
			StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_CHAT_REGISTY_WND, 1 );
			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???
				if( HitDispNo == systemWndFontNo[ 3 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ?V????????
					GetKeyInputFocus( &MyChatBuffer );
					// ?????@?e????????
					systemWndNo = 0;
					// ??????????
					play_se( 203, 320, 240 );
					// ????????
					saveUserSetting();
					// ??????K???????V
					SaveChatRegistyStr();
					break;
				}
			}
			// ????@?e
			x = pActMenuWnd->x + 18;
			y = pActMenuWnd->y + 58;

			// ?K???????
			for( i = 0 ; i < 8 ; i++ ){
				// ????????
				chatRegistryStr[ i ].x = x + 2;
				chatRegistryStr[ i ].y = y;
				StockFontBuffer2( &chatRegistryStr[ i ] ); y += 43;
				// ?????V?????
				if( MakeHitBox( chatRegistryStr[ i ].x - 4, chatRegistryStr[ i ].y - 3,
					chatRegistryStr[ i ].x + 234 + 3, chatRegistryStr[ i ].y + 16 + 3, DISP_PRIO_IME3 ) == true ){
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?V?????????
						GetKeyInputFocus( &chatRegistryStr[ i ] );
						play_se( 217, 320, 240 ); // ?????
					}
				}
			}
			// ?????
			systemWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, 410, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );

			break;

		case 6:	// ????@?e??

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 3, 4, CG_WND_TITLE_MOUSE, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ????????????
			if( HitFontNo == systemWndFontNo[ 0 ] ){
				// ?????
				if( MouseCursorFlag == true ){
					// ??????
					strcpy( OneLineInfoStr, "两色的滑鼠游标，反应较快");

					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????????
#ifdef _TALK_WINDOW
						g_iCursorCount = ShowCursor( false );
#else
						//ShowCursor( false );
#endif
						MouseCursorFlag = false;
						play_se( 217, 320, 240 );	// ?????
					}
				}else{
					// ??????
					strcpy( OneLineInfoStr, "�?通的滑鼠游标");
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????????
#ifdef _TALK_WINDOW
						g_iCursorCount = ShowCursor( true );
#else
						//ShowCursor( true );
#endif
						MouseCursorFlag = true;
						play_se( 217, 320, 240 );	// ?????
					}
				}
			}
			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ????@?e?????????
					systemWndNo = 0;
					// ??????????
					play_se( 203, 320, 240 );
					// ????????
					saveUserSetting();
					break;
				}
			}

			// ????@?e
			x = pActMenuWnd->x + 28;
			y = pActMenuWnd->y + 56;

			// ???????????????
			StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "   游标设定   ", 0 , 0);	y += 40;
			systemWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, mouseCursor[ MouseCursorFlag ], 2);	y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, FONT_PAL_AQUA, 	"    回上一�?    ", 2, 0 );	y += 40;
			break;

		case 7:	// ????? ??????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 3, 3, CG_WND_TITLE_LOGOUT, 1 );
				for( i = 0 ; i < MENU_SYSTEM_0 ; i++ ) systemWndFontNo[ i ] = -2;
				break;
			}else{
				if( pActMenuWnd->hp <= 0 ) break;
			}

			// ????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ????
				if( HitFontNo == systemWndFontNo[ 0 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					GameState = GAME_LOGIN;
					ChangeProc2( PROC_CHAR_LOGOUT );

					play_se( 206, 320, 240 );
					break;
				}
				// ?????
				if( HitFontNo == systemWndFontNo[ 1 ] ){
					// ????????
					DeathAction( pActMenuWnd );
					pActMenuWnd = NULL;
					// ????????
					systemWndNo = 0;
					play_se( 203, 320, 240 );
					break;
				}
			}

			// ????@?e
			x = pActMenuWnd->x + 28;
			y = pActMenuWnd->y + 56;

			// ???????????????
			systemWndFontNo[ 0 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     �?  �?     ", 2, 0 );	y += 40;
			systemWndFontNo[ 1 ] = StockFontBufferUtf( x, y, FONT_PRIO_FRONT, 0, 	"     �?  �?     ", 2, 0 );	y += 40;
			break;

		}
	}

//? ???????????? *****************************************************/
#ifdef _FRIENDCHANNEL				//ROG ADD ???????
	if(setRoomFlag == true)
	{
		setRoomName();
	}

	if(assentFlag == true)
	{
		AssentWnd();
	}
#endif


	if( MenuToggleFlag & JOY_CTRL_S ){

		// ??????k?????
		switch( statusWndNo ){

		case 0:	// ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????

				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 360, 0, -1 );

				// ????R?e???
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
				SkillWndflag = false ;
#endif
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ???????????????????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
					if ( SkillWndflag == true ) {
						SkillWndfunc2();
					}
					else {
						if ( pActSkillMenuWnd != NULL ) {
							DeathAction( pActSkillMenuWnd ) ;
							pActSkillMenuWnd = NULL ;
						}
					}
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx-1, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my+34, DISP_PRIO_MENU, CG_NEW_STATUS_WND, 1 );
#else
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_STATUS_WND, 1 );
#endif
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ????
						if( HitDispNo == statusWndFontNo[ 0 ] ){
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							statusWndNo = 1;
							play_se( 202, 320, 240 );
						}
						// ???
						if( HitDispNo == statusWndFontNo[ 1 ] ){
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							MenuToggleFlag ^= JOY_CTRL_S;
							play_se( 203, 320, 240 );
						}
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						if( HitDispNo == statusWndFontNo[ 12 ] ){
							DeathAction( pActMenuWnd);
							pActMenuWnd = NULL;
							statusWndNo = 3;
							play_se( 202, 320, 240);
#ifdef _NEWREQUESTPROTOCOL			// (?????) Syu ADD ????Protocol??????
							lssproto_RESIST_send ( sockfd , "" ) ;
#endif
#ifdef _ALCHEPLUS
							lssproto_ALCHEPLUS_send ( sockfd , "" ) ;
#endif
						}
						if( HitDispNo == statusWndFontNo[ 14 ] ){
							play_se( 202, 320 , 240 ) ;
							SkillWndflag = true ;
							MenuToggleFlag &= ~JOY_CTRL_I;
							MenuToggleFlag &= ~JOY_CTRL_M;
							if( pActMenuWnd3 != NULL ) {
								DeathAction ( pActMenuWnd3 ) ;
								pActMenuWnd3 = NULL ;
								GetKeyInputFocus( &MyChatBuffer );
							}
						}
#endif

					}
					// ?????
					if( HitDispNo == statusWndFontNo[ 6 ] ){
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
							SkillWndflag = false ;
							MenuToggleFlag &= ~JOY_CTRL_I;
							//MenuToggleFlag &= ~JOY_CTRL_M;
#endif

							if( pActMenuWnd3 == NULL ){
								// ???????????????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
								pActMenuWnd3 = MakeWindowDisp( 304, 16 + 280 + 48, 272, 88, 0, -1 );
#else
								pActMenuWnd3 = MakeWindowDisp( 4, 16 + 280 + 70, 272, 88, 0, -1 );
#endif
								// ????????
								play_se( 202, 320, 240 );
								// ???q????
								shougouChange.buffer[ 0 ] = NULL;
								shougouChange.cnt = 0;
								shougouChange.cursor=0;
								// ????????
								shougouChange.len = 12;
								// ???????
								shougouChange.color = 0;
								// ????????
								shougouChange.x = pActMenuWnd3->x + 38;
								shougouChange.y = pActMenuWnd3->y + 25;
								// ???????????I??
								shougouChange.fontPrio = FONT_PRIO_FRONT;
								// ????
								statusWndBtnFlag[ 6 ] = true;
							}else{
								// ????????
								DeathAction( pActMenuWnd3 );
								pActMenuWnd3 = NULL;
								// ?V????????
								GetKeyInputFocus( &MyChatBuffer );
								// ??????????
								play_se( 203, 320, 240 );
							}
							statusWndBtnFlag[ 6 ] = true;

						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && statusWndBtnFlag[ 6 ] == true ){
							// ????
							statusWndBtnFlag[ 6 ] = true;
						}else{
							// ?????
							statusWndBtnFlag[ 6 ] = false;
						}
					}else{
						// ?????
						statusWndBtnFlag[ 6 ] = false;
					}
					// ??????????
					if( StatusUpPoint != 0 ){
						// ????????
						for( i = 2 ; i < 6 ; i++ ){
							// ???????
							if( HitDispNo == statusWndFontNo[ i ] ){
								// ????
								// ????????
								if( mouse.onceState & MOUSE_LEFT_CRICK ){
									// ?????????
									if( bNewServer)
										lssproto_SKUP_send( sockfd, i - 2 );
									else
										old_lssproto_SKUP_send( sockfd, i - 2 );


#ifndef _CHAR_PROFESSION			// WON ADD ??????
									StatusUpPoint--;
#endif

									play_se( 211, 320, 240 );

									statusWndBtnFlag[ i ] = true;
								}
								// ?????????
								if( mouse.state & MOUSE_LEFT_CRICK && statusWndBtnFlag[ i ] == true ){
									// ????
									statusWndBtnFlag[ i ] = true;
								}else{
									// ?????
									statusWndBtnFlag[ i ] = false;
								}
							}else{
								// ?????
								statusWndBtnFlag[ i ] = false;
							}
						}
					}
					// ????????
					if( pActMenuWnd != NULL ){

					#define PET_WND_ATTR_X 159
					#define PET_WND_ATTR_Y 5

						int x2; // ????????

						// ????@?e

						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 28;
						// ????????????
						CenteringStr( pc.name, moji, CHAR_NAME_LEN );

						StockFontBuffer( x - 6, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji,"%8d", pc.dp );
						StockFontBuffer( x + 170, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						// ????????????
						CenteringStr( pc.freeName, moji, 12 );	// ??e?u?????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						//��????
						StockFontBuffer( x + 77, y + 2 , FONT_PRIO_FRONT, 0, moji, 0 );
#ifdef _ALLDOMAN // (?????) Syu ADD ???��?NPC
						//?????????
						if ( pc.herofloor == 132 )		sprintf( moji , "%s" , "��˹��½Ӣ��" ) ;
						else if ( pc.herofloor >= 130 )	sprintf( moji , "%s" , "��˹��½սʿ" ) ;
						else if ( pc.herofloor >= 125 )	sprintf( moji , "%s" , "��˹��½��ʿ" ) ;
						else if ( pc.herofloor >= 120 )	sprintf( moji , "%s" , "������˹Ӣ��" ) ;
						else if ( pc.herofloor >= 115 )	sprintf( moji , "%s" , "˹�ʹ�սʿ" ) ;
						else if ( pc.herofloor >= 110 )	sprintf( moji , "%s" , "��ķ����սʿ" ) ;
						else if ( pc.herofloor >= 100 )	sprintf( moji , "%s" , "������˿սʿ" ) ;
						else if ( pc.herofloor >=  80 )	sprintf( moji , "%s" , "��̹սʿ" ) ;
						else if ( pc.herofloor >=  60 )	sprintf( moji , "%s" , "���ض�սʿ" ) ;
						else if ( pc.herofloor >=  40 )	sprintf( moji , "%s" , "��ħ��ʿ" ) ;
						else if ( pc.herofloor >=  20 )	sprintf( moji , "%s" , "ʥ����ʿ" ) ;
						else if ( pc.herofloor >=   1 )	sprintf( moji , "%s" , "������ʿ" ) ;
						else							sprintf( moji , " " );
						StockFontBuffer( x + 72, y + 28 , FONT_PRIO_FRONT, 5, moji, 0 );
						sprintf( moji , "%d" , pc.profession_level);
						StockFontBuffer( x + 157, y + 53 , FONT_PRIO_FRONT, 0, moji, 0 );
#endif
#else
						StockFontBuffer( x + 2, y, FONT_PRIO_FRONT, 0, moji, 0 );
#endif
#ifdef _TRANS_6
						if( pc.transmigration >= 1 && pc.transmigration <= 6 ){
#else
						if( pc.transmigration >= 1 && pc.transmigration <= 5 ){
#endif
							sprintf( moji,"???%s", TransmigrationStr[ pc.transmigration ] );
							StockFontBuffer( x + 178 + 12, y + 2, FONT_PRIO_FRONT, FONT_PAL_AQUA, moji, 0 );
						}
						y += 21;
						sprintf( moji,"%3d",pc.level );

#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						y += 35;
						StockFontBuffer( x + 38, y - 2, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%8d", pc.exp );
						StockFontBuffer( x + 48, y + 2 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%8d",pc.maxExp );
						StockFontBuffer( x + 48, y + 4, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%4d", pc.hp );
						StockFontBuffer( x + 48, y + 8, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%4d", pc.maxHp );
						StockFontBuffer( x + 98, y + 8, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.mp );
						StockFontBuffer( x + 50, y + 12 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.atk );
						StockFontBuffer( x + 50, y + 16, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.def );
						StockFontBuffer( x + 50, y + 20, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.quick );
						StockFontBuffer( x + 50, y + 24, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.charm );
						StockFontBuffer( x + 50, y + 28, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						y += 29;
						sprintf( moji, "%3d", pc.vital );
						StockFontBuffer( x + 55, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%3d", pc.str );
						StockFontBuffer( x + 55 + 120, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
						sprintf( moji, "%3d", pc.tgh );
						StockFontBuffer( x + 55, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%3d", pc.dex );
						StockFontBuffer( x + 55 + 120, y + 34 , FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
#else
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%8d", pc.exp );
						StockFontBuffer( x + 48, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%8d",pc.maxExp );
						StockFontBuffer( x + 48, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%4d", pc.hp );
						StockFontBuffer( x + 48, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%4d", pc.maxHp );
						StockFontBuffer( x + 98, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.mp );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.atk );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.def );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.quick );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						sprintf( moji, "%3d", pc.charm );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 21;
						y += 29;
						sprintf( moji, "%3d", pc.vital );
						StockFontBuffer( x + 61, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%3d", pc.str );
						StockFontBuffer( x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
						sprintf( moji, "%3d", pc.tgh );
						StockFontBuffer( x + 61, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%3d", pc.dex );
						StockFontBuffer( x + 61 + 120, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
#endif

#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						y += 50;
#else
						y += 17;
#endif
						// ????
						statusWndFontNo[ 0 ] = StockDispBuffer( x + 59, y, DISP_PRIO_IME3, CG_STATUS_WND_GROUP_BTN, 2 );
						// ????
						statusWndFontNo[ 1 ] = StockDispBuffer( x + 173, y, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
						// ???????????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						statusWndFontNo[ 12 ] = StockDispBuffer( x + 164 , y - 102, DISP_PRIO_IME3, CG_STATUS_DETAIL, 2 );
						if ( pc.profession_class != 0 )
							statusWndFontNo[ 14 ] = StockDispBuffer( x + 213 , y - 318 , DISP_PRIO_IME3, CG_WAR_ICON_BIG + pc.profession_class - 1, 2 );
						//else //????????
						//	statusWndFontNo[ 14 ] = StockDispBuffer( x + 213 , y - 318 , DISP_PRIO_IME3, CG_WAR_ICON_BIG + pc.profession_class - 1, 2 );


						StockDispBuffer( pActMenuWnd->x + 216, pActMenuWnd->y + 168, DISP_PRIO_IME3, pc.faceGraNo, 0 );
#else
						StockDispBuffer( pActMenuWnd->x + 210, pActMenuWnd->y + 119, DISP_PRIO_IME3, pc.faceGraNo, 0 );
#endif
						if( StatusUpPoint != 0 ){
							// ??????????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
							StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 14 , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 67 , DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0 );
							sprintf( moji, "%2d", StatusUpPoint );
							StockFontBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 70, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 156, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 );
							statusWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx  , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 66 , DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 2 ], 2 );
							statusWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 66 , DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 3 ], 2 );
							statusWndFontNo[ 4 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx  , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 86, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 4 ], 2 );
							statusWndFontNo[ 5 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 86, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 5 ], 2 );
#else
							StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_LV_UP_POINT, 0 );
							sprintf( moji, "%2d", StatusUpPoint );
							StockFontBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 50, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 88, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 );

							// ??????
							statusWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 2 ], 2 );
							statusWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 3 ], 2 );
							statusWndFontNo[ 4 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 20, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 4 ], 2 );
							statusWndFontNo[ 5 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 120, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my + 20, DISP_PRIO_IME3, CG_STATUS_WND_UP_BTN_UP + statusWndBtnFlag[ 5 ], 2 );
#endif
						}
						// ??????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						//��???????
						statusWndFontNo[ 6 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx - 124 , ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[ 6 ], 2 );
#else
						statusWndFontNo[ 6 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_STATUS_WND_SHOUGOU_BTN_UP + statusWndBtnFlag[ 6 ], 2 );
#endif

						// ????@?e
						x = pActMenuWnd->x + 19;
						y = pActMenuWnd->y + 174;
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
						x -= 4;
						y += 43;
#endif

						// ??N?????
						if( pc.earth > 0 ){
							x2 = ( int )( pc.earth * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
						}
						y += 20;

						// ??N?????
						if( pc.water > 0 ){
							x2 = ( int )( pc.water * 0.8 );

							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
						}
						y += 20;

						// ??N?????
						if( pc.fire > 0 ){
							x2 = ( int )( pc.fire * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
						}
						y += 20;

						// ??N?????
						if( pc.wind > 0 ){
							x2 = ( int )( pc.wind * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
						}

					}
				}
			}
			// ???????????????
			if( pActMenuWnd3 != NULL ){
				// ????????????????
				if( pActMenuWnd3->hp > 0 ){
					// ???????????????
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1 );
#else
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1 );
#endif
					// ?V????????x
					GetKeyInputFocus( &shougouChange );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ????????????
						if( HitDispNo == statusWndFontNo[ 7 ] ){
							// ????????
							KeyboardReturn();
						}
						// ??????????????
						if( HitDispNo == statusWndFontNo[ 8 ] ){
							// ????????
							DeathAction( pActMenuWnd3 );
							pActMenuWnd3 = NULL;
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ????????????????
					if(	pActMenuWnd3 != NULL ){
						// ?????????
						StockFontBuffer2( &shougouChange );

						// ????@?e
						x = pActMenuWnd3->x + 20;
						y = pActMenuWnd3->y + 60;
						statusWndFontNo[ 7 ] = StockDispBuffer( x + 60, y + 7, DISP_PRIO_IME3, CG_OK_BTN, 2 );
						statusWndFontNo[ 8 ] = StockDispBuffer( x + 170, y + 7, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
					}
				}
			}
			break;

		case 1:	// ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 0, 256, 456, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_STATUS_WND_GROUP_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ???
						if( HitDispNo == statusWndFontNo[ 11 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							statusWndNo = 0;
							// ??????????
							play_se( 203, 320, 240 );
						}
#ifdef _TEAM_KICKPARTY
						for( i=0; i<5; i++){
							if( HitDispNo == statusWndFontNo[ i+12] ){
								lssproto_KTEAM_send( sockfd, i);
								play_se( 203, 320, 240 );
							}
						}
#endif
					}
					// ????????
					if( pActMenuWnd != NULL ){
						int selectFlag;	// ?k????????????????
						// ????@?e
						x = pActMenuWnd->x + 17;
						y = pActMenuWnd->y + 25;

						if( itemNo != -1 || jujutuNo != -1 ) selectFlag = 2;
						else selectFlag = false;

						CenteringStr( pc.name, moji, CHAR_NAME_LEN );
						statusWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji,selectFlag ); y += 19;
						sprintf( moji,"%4d", pc.mp );
						StockFontBuffer( x + 77, y, FONT_PRIO_FRONT, 0, 	moji, 0 );
						sprintf( moji,"%4d   %4d", pc.hp, pc.maxHp );
						StockFontBuffer( x + 142, y, FONT_PRIO_FRONT, 0, 	moji, 0 ); y += 20;
						for( i = 0 ; i < 5 ; i++ ){
							if( pet[ i ].useFlag == true ){
								if( pet[ i ].freeName[ 0 ] != NULL ) CenteringStr( pet[ i ].freeName, moji, PET_NAME_LEN );
								else CenteringStr( pet[ i ].name, moji, PET_NAME_LEN );
								statusWndFontNo[ i + 1 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji,selectFlag ); y += 20;
								sprintf( moji,"%4d   %4d", pet[ i ].hp, pet[ i ].maxHp );
								StockFontBuffer( x + 142, y, FONT_PRIO_FRONT, 0, 	moji, 0 ); y += 20;
							}
						}
						y = 268;
						for( i = 0 ; i < 5 ; i++ ){
							if( pc.id != party[ i ].id && party[ i ].useFlag == true ){
								CenteringStr( party[ i ].name, moji, CHAR_NAME_LEN );
								statusWndFontNo[ i + 6 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji,selectFlag ); y += 20;
#ifdef _TEAM_KICKPARTY
								//andy_add ???????
								statusWndFontNo[ i+12 ] = StockDispBuffer( x + 187, y-10, DISP_PRIO_IME3, CG_MAIL_WND_DELETE_BTN , 2 );
#endif
								sprintf( moji,"%4d", party[ i ].mp );
								StockFontBuffer( x + 77, y, FONT_PRIO_FRONT, 0, 	moji, 0 );
								sprintf( moji,"%4d %4d", party[ i ].hp, party[ i ].maxHp );
								StockFontBuffer( x + 142, y, FONT_PRIO_FRONT, 0, 	moji, 0 ); y += 20;
							}
						}
						statusWndFontNo[ 11 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, 441, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;
#ifdef _NEWPANEL
		case 3:
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 0, 256, 456, 0, -1 );
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
			}else{
				if( pActMenuWnd->hp > 0 ){
					x = pActMenuWnd->x ;
					y = pActMenuWnd->y ;

					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx+8, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my-5, DISP_PRIO_MENU, CG_SKILL_TABLE, 1 );
					statusWndFontNo[ 13 ] = StockDispBuffer( x+139, y+423, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );

					statusWndFontNo[ 13 ] = StockDispBuffer( x+215, y+437, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
#ifdef _SYUTEST3
					for ( i = 14 ; i < 22 ; i ++ )
						statusWndFontNo[ i ] = StockDispBuffer( x+139 , y+ 200 + ( i - 14 ) * 20 , DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
#endif

					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						if( HitDispNo == statusWndFontNo[ 13 ] ){
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							statusWndNo = 0;
							play_se( 202, 320, 240);
						}
#ifdef _SYUTEST3
						if( HitDispNo == statusWndFontNo[ 14 ] )
							lssproto_CHATROOM_send ( sockfd , "C|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 15 ] )
							lssproto_CHATROOM_send ( sockfd , "D|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 16 ] )
							lssproto_CHATROOM_send ( sockfd , "J|0" ) ;
						if( HitDispNo == statusWndFontNo[ 17 ] )
							lssproto_CHATROOM_send ( sockfd , "L|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 18 ] )
							lssproto_CHATROOM_send ( sockfd , "K|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 19 ] )
							lssproto_CHATROOM_send ( sockfd , "M|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 20 ] )
							lssproto_CHATROOM_send ( sockfd , "T|123456798" ) ;
						if( HitDispNo == statusWndFontNo[ 21 ] )
							lssproto_CHATROOM_send ( sockfd , "B|123456798" ) ;
#endif
					}
				}
			}
			break;
#endif
		}

	}

//? ?????????? ********************************************************/

	if( MenuToggleFlag & JOY_CTRL_P ){
#ifdef _STREET_VENDOR
		if(pc.iOnStreetVendor == 1 || sStreetVendorBuyBtn == 2){
			StockChatBufferLine("��̯�в��ø�������״̬",FONT_PAL_RED);
			MenuToggleFlag ^= JOY_CTRL_P;
			// ??????��???????????څ? 3,??????
			petWndNo = 3;
		}
#endif


		// ??????k?????
		switch( petWndNo ){

		case 0:	// ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 320, 0, -1 );

				// ????R?e???
				for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
				// ????k????
				BattlePetReceivePetNo = -1;
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
				DropPetWndflag = false ;
				DropI = -1 ;
#endif
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ??????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_PET_WND_VIEW, 1 );

					// ??????????
					for( i = 0 ; i < 5 ; i++ ){
						// ????????????
						if( pet[ i ].useFlag == false ){
							// ??????
							if( i == pc.battlePetNo )
								pc.battlePetNo = -1;
							// ?????
							if( i == pc.mailPetNo )
								pc.mailPetNo = -1;
							// ??????????
							pc.selectPetNo[ i ] = false;
						}
					}
					// ???????????
					BattlePetStMenCnt = 0;
					// ????????
					for( i = 0 ; i < 5 ; i++ ){
						if( pc.selectPetNo[ i ] == true ) BattlePetStMenCnt++;
					}

					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????????
						for( i = 0 ; i < 5 ; i++ ){
							// ???????????????????????
							if( pet[ i ].useFlag == true && BattlePetReceivePetNo != i ){
								// ????k?????
								if( HitDispNo == petWndFontNo[ i ] ){
									// ride Pet
#if 1
									if( i == pc.mailPetNo ){
										pc.mailPetNo = -1;
										// shan
										if( (bNewServer & 0xf000000) == 0xf000000)
											lssproto_PETST_send( sockfd, i, 0);
										checkRidePet( i );

										play_se( 217, 320, 240 ); // ?????
									}else
										// when ride Pet
										if( i == pc.ridePetNo && pc.graNo != SPR_pet021
											&& pc.graNo != 100362){//???
											char buf[64];
											sprintf( buf, "R|P|-1");
											if( bNewServer)
												lssproto_FM_send( sockfd, buf );
											else
												lssproto_FM_send( sockfd, buf );
											play_se( 217, 320, 240 ); // ?????
											// shan
											if( (bNewServer & 0xf000000) == 0xf000000)
												lssproto_PETST_send( sockfd, i, 0);
										}else
											// ?I?D??
											if( i == pc.battlePetNo && BattlePetReceiveFlag == false ){
												// ?I?D?????
												if( bNewServer)
													lssproto_KS_send( sockfd, -1 );
												else
													old_lssproto_KS_send( sockfd, -1 );
												// ????r??????
												BattlePetReceiveFlag = true;
												// ?????????????k???
												BattlePetReceivePetNo = i;

												// ?????
												pc.selectPetNo[ i ] = false;
												BattlePetStMenCnt--; // ?????????
												// ?????????
												if( pc.mailPetNo == -1 ){
													// ??????
													pc.mailPetNo = i;
													// shan
													if( (bNewServer & 0xf000000) == 0xf000000)
														lssproto_PETST_send( sockfd, i, 4);
												}else							{
													checkRidePet( i );
													// shan
													if( (bNewServer & 0xf000000) == 0xf000000)
														lssproto_PETST_send( sockfd, i, 0);
												}
												play_se( 217, 320, 240 ); // ?????
											}else
												// ??????
												if( pc.selectPetNo[ i ] == true ){
													// ?I?D???????
													if( pc.battlePetNo == -1 && BattlePetReceiveFlag == false ){
														// ???????
														if( pet[ i ].hp > 0 ){
															// ?I?D???
															if( bNewServer)
																lssproto_KS_send( sockfd, i );
															else
																old_lssproto_KS_send( sockfd, i );
															// ????r??????
															BattlePetReceiveFlag = true;
															// ?????????????k???
															BattlePetReceivePetNo = i;
															play_se( 217, 320, 240 ); // ?????
														}else{
															play_se( 220, 320, 240 ); // ???
														}
													}else{
														// ?????????
														if( pc.mailPetNo == -1 ){
															pc.mailPetNo = i;
															// shan
															if( (bNewServer & 0xf000000) == 0xf000000)
																lssproto_PETST_send( sockfd, i, 4);
														}
														// ride Pet
														else
														{
															checkRidePet( i );
															// shan
															if( (bNewServer & 0xf000000) == 0xf000000)
																lssproto_PETST_send( sockfd, i, 0);
														}

														pc.selectPetNo[ i ] = false;
														BattlePetStMenCnt--;
														play_se( 217, 320, 240 );
													}

												}else
													if( pc.selectPetNo[ i ] == false ){
														// ?????????
														if( BattlePetStMenCnt < 3 ){
															// ???????
															pc.selectPetNo[ i ] = true;
															BattlePetStMenCnt++; // ????????
															play_se( 217, 320, 240 ); // ?????
															// shan
															if( (bNewServer & 0xf000000) == 0xf000000)
																lssproto_PETST_send( sockfd, i, 1);
														}else{
															// ?????????
															if( pc.mailPetNo == -1 ){
																pc.mailPetNo = i;
																play_se( 217, 320, 240 ); // ?????
																// shan
																if( (bNewServer & 0xf000000) == 0xf000000)
																	lssproto_PETST_send( sockfd, i, 4);
															}
															// ride Pet
															else
															{
																checkRidePet( i );
															}
														}
													}
#endif

								}
								// ?????????k?????
								if( HitFontNo == petWndFontNo[ i + 5 ] ){
									petStatusNo = i; // ????k???
									petWndNo = 1;
									// ????????
									DeathAction( pActMenuWnd );
									pActMenuWnd = NULL;
									// ????????
									play_se( 202, 320, 240 );
								}
								if (HitFontNo == petWndFontNo[ 23 + i]) {
									play_se (202, 320, 240);
									DropPetWndflag = true;
									DropI = i;
									StockFontBuffer(245, 220, FONT_PRIO_FRONT, 3, "ȷ��Ҫ������ĳ�����??", 0); y += 40;
									StockDispBuffer(320, 240, DISP_PRIO_YES_NO_WND, CG_DROPWND, 0);
									petWndFontNo[21] = StockDispBuffer ( 320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2);
									petWndFontNo[22] = StockDispBuffer ( 320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2);
								}
							}
						}
						// ????
						if( HitDispNo == petWndFontNo[ 10 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ????????????
							MenuToggleFlag ^= JOY_CTRL_P;
							// ??????????
							play_se( 203, 320, 240 );
							// ????????

							saveUserSetting();
						}
						// ??????
						if( HitDispNo == petWndFontNo[ 11 ] ){
							petStatusNo = 0; // ????k???
							// ??????????
							for( i = 0 ; i < 5 ; i++ ){
								// ???????????
								if( pet[ i ].useFlag == true ){
									petStatusNo = i;
									break;
								}
							}
							petWndNo = 1;
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ????????
							play_se( 202, 320, 240 );
						}
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
						if( HitDispNo == petWndFontNo[ 21 ] ) {
							i = DropI ;
							DropI = -1 ;
							lssproto_DP_send( sockfd, nowGx, nowGy, i );
							if( pc.selectPetNo[ i ] == true ) {
								pc.selectPetNo[ i ] = false;
								BattlePetStMenCnt--;
							}
							if( i == pc.battlePetNo )
								lssproto_KS_send( sockfd, -1 );
							if( pc.mailPetNo == i ) {
								pc.mailPetNo = -1;
							}
							play_se( 217, 320, 240 );
							DropPetWndflag = false ;
						}
						if( HitDispNo == petWndFontNo[ 22 ] ) {
							DropPetWndflag = false ;
							DropI = -1 ;
							play_se( 217, 320, 240 );
						}
#endif
					}
					// ????????
					if( mouse.onceState & MOUSE_RIGHT_CRICK ){
						// ??????????
						for( i = 0 ; i < 5 ; i++ ){
							// ???????????
							if( pet[ i ].useFlag == true && eventWarpSendFlag == false ){
								// ????k?????
								if( HitFontNo == petWndFontNo[ i + 5 ] ){
									// ?????????????? or ride Pet
									if( ItemMixPetNo != i
										&& pc.ridePetNo != i )
									{
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
										DropPetWndflag = true ;
										DropI = i ;
#else


										// ?????t????
										if( bNewServer)
											lssproto_DP_send( sockfd, nowGx, nowGy, i );
										else
											old_lssproto_DP_send( sockfd, nowGx, nowGy, i );
										// ??????
										if( pc.selectPetNo[ i ] == true ){
											pc.selectPetNo[ i ] = false; // ????
											BattlePetStMenCnt--; // ?????????
										}
										// ?I?D??
										if( i == pc.battlePetNo ){
											// ?I?D?????
											if( bNewServer)
												lssproto_KS_send( sockfd, -1 );
											else
												old_lssproto_KS_send( sockfd, -1 );
										}
										// ?????
										if( pc.mailPetNo == i ){
											pc.mailPetNo = -1;
										}
										play_se( 217, 320, 240 ); // ?????
#endif
									}
									else
									{
										// ???
										play_se( 220, 320, 240 );
									}
								}
							}
						}
					}
#ifdef _DROPPETWND					// (?????) Syu ADD ???????????
					if ( DropPetWndflag == true ) {
						StockFontBuffer( 245 , 220 , FONT_PRIO_AFRONT, 3, "ȷ��Ҫ������ĳ�����??", 0 ); y += 40;
						StockDispBuffer( 320, 240, DISP_PRIO_YES_NO_WND, CG_DROPWND, 0 );
						petWndFontNo[21] = StockDispBuffer( 320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2 );
						petWndFontNo[22] = StockDispBuffer( 320, 240, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2 );
					}
#endif
					// ????????
					if( pActMenuWnd != NULL ){

						int flag = false;
						int color;
						int btnNo;
						int atrFlag = false;
						int atrGraNo[ 4 ];

						x = pActMenuWnd->x + 16 + 50, y = pActMenuWnd->y + 31;
						// ????????
						for( i = 0 ; i < 5 ; i++ ){
							// ???????????
							if( pet[ i ].useFlag == true ){

								color = FONT_PAL_WHITE;		// ???????
								btnNo = 0;					// ???????
								// ?????V???
								if( pc.selectPetNo[ i ] == true ){
									color = FONT_PAL_AQUA;
									btnNo = 2;
								}
								// ?I?D??
								if( i == pc.battlePetNo ){
									color = FONT_PAL_YELLOW;
									btnNo = 1;
								}
								// ?????
								if( i == pc.mailPetNo ){
									color = FONT_PAL_GREEN;
									btnNo = 3;
								}
								// ride Pet
								if( i == pc.ridePetNo ){
									color = FONT_PAL_YELLOW;
									btnNo = 195;
								}

								// ???q????????
								// ????????
								if( pet[ i ].freeName[ 0 ] != NULL ){
									// ????????????
									CenteringStr( pet[ i ].freeName, moji, PET_NAME_LEN );
								}else{
									// ????????????
									CenteringStr( pet[ i ].name, moji, PET_NAME_LEN );
								}
								petWndFontNo[ i + 5 ] = StockFontBuffer( x + 3, y, FONT_PRIO_FRONT, color, moji, 2 );
								petWndFontNo[ i + 23 ] = StockFontBufferUtf(x - 40, y + 24, FONT_PRIO_FRONT, FONT_PAL_RED,"丢弃", 2, 0);
								sprintf( moji,"%dת", pet[ i ].trn );
								StockFontBuffer( pActMenuWnd->x + 190, y, FONT_PRIO_FRONT, FONT_PAL_GREEN, 	moji, 0 ); y += 24;
								atrFlag = false;
								// ?N?????
								if( pet[ i ].earth > 0 ){	// ??
									// ????
									if( pet[ i ].earth > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_EARTH_BIG;
									// ????
									else atrGraNo[ atrFlag ] = CG_ATR_ICON_EARTH_SML;
									atrFlag++; // ?????
								}
								if( pet[ i ].water > 0 ){	// ?
									// ????
									if( pet[ i ].water > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_WATER_BIG;
									// ????
									else atrGraNo[ atrFlag ] = CG_ATR_ICON_WATER_SML;
									atrFlag++; // ?????
								}
								if( pet[ i ].fire > 0 ){	// ?
									// ????
									if( pet[ i ].fire > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_FIRE_BIG;
									// ????
									else atrGraNo[ atrFlag ] = CG_ATR_ICON_FIRE_SML;
									atrFlag++; // ?????
								}
								if( pet[ i ].wind > 0 ){	// ?
									// ????
									if( pet[ i ].wind > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_WIND_BIG;
									// ????
									else atrGraNo[ atrFlag ] = CG_ATR_ICON_WIND_SML;
									atrFlag++; // ?????
								}
								// ???
								if( atrFlag > 0 ) StockDispBuffer( pActMenuWnd->x + 228, y - 16, DISP_PRIO_IME3, atrGraNo[ 0 ], 0 );
								// ?K??
								if( atrFlag > 1 ) StockDispBuffer( pActMenuWnd->x + 228 + 16 , y - 16, DISP_PRIO_IME3, atrGraNo[ 1 ], 0 );

								petWndFontNo[ i ] = StockDispBuffer( x - 27, y - 14, DISP_PRIO_IME3, CG_PET_WND_REST_BTN + btnNo, 2 );

								sprintf( moji, "%3d     %4d   %4d", pet[ i ].level, pet[ i ].hp, pet[ i ].maxHp );
								StockFontBuffer( x + 26 + 27, y, FONT_PRIO_FRONT, color, 	moji, 0 ); y += 27;
								flag = true;
							}
						}
						// ????????
						if( flag != true ){
							// ????????
							petWndFontNo[ 10 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
							StockFontBuffer( x + 10, y, FONT_PRIO_FRONT, 0, "��û�г���", 0 ); y += 40;
						}else{
							// ????????
							petWndFontNo[ 10 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 56, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
							petWndFontNo[ 11 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx - 56, pActMenuWnd->y + 299, DISP_PRIO_IME3, CG_PET_WND_STATUS_BTN, 2 );
						}
					}
				}
			}
			break;

		case 1: // ????????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????

				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 332, 0, -1 );

				// ????R?e???
				for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;

			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ???????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_PET_WND_DETAIL, 1 );
					// ????????????????
					if( pActPet == NULL )
					{
#ifdef _LIZARDPOSITION			   // (?????) Syu ADD ???????????��?��???
						if((pet[ petStatusNo ].graNo==101493)||(pet[ petStatusNo ].graNo==101494)||
							(pet[ petStatusNo ].graNo==101495)||(pet[ petStatusNo ].graNo==101496))
						{
							pActPet = MakeAnimDisp( pActMenuWnd->x + 220, pActMenuWnd->y + 124, pet[ petStatusNo ].graNo, ANIM_DISP_PET );
						}
						else
							pActPet = MakeAnimDisp( pActMenuWnd->x + 200, pActMenuWnd->y + 144, pet[ petStatusNo ].graNo, ANIM_DISP_PET );
#else
						pActPet = MakeAnimDisp( pActMenuWnd->x + 200, pActMenuWnd->y + 144, pet[ petStatusNo ].graNo, ANIM_DISP_PET );
#endif
					}
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?????????????
						if( HitDispNo == petWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ?????????????
							petWndNo = 0;
							// ???q???????????
							DeathAction( pActMenuWnd3 );
							pActMenuWnd3 = NULL;
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
							// ??????????
							play_se( 203, 320, 240 );
						}
						// ????????
						if( HitDispNo == petWndFontNo[ 4 ] ){
							// ???????k?
							petWndNo = 2;
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ?????????????
							petWndNo = 2;
							// ???q???????????
							DeathAction( pActMenuWnd3 );
							pActMenuWnd3 = NULL;
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
							// ????????
							play_se( 202, 320, 240 );
						}
					}
					// ?q??????????
					if( HitDispNo == petWndFontNo[ 1 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							// ???q????????????
							if( pActMenuWnd3 != NULL ){
								// ????????
								DeathAction( pActMenuWnd3 );
								pActMenuWnd3 = NULL;
								// ?V????????
								GetKeyInputFocus( &MyChatBuffer );
								// ??????????
								play_se( 203, 320, 240 );
							}
							while( 1 ){
								petStatusNo--;
								// ????????
								if( petStatusNo <= -1 ) petStatusNo = 4;
								if( pet[ petStatusNo ].useFlag == true ) break;
							}
							// ????????k????
#ifdef _LIZARDPOSITION			   // (?????) Syu ADD ???????????��?��???
							if((pet[ petStatusNo ].graNo==101493)||(pet[ petStatusNo ].graNo==101494)||
								(pet[ petStatusNo ].graNo==101495)||(pet[ petStatusNo ].graNo==101496))
							{
								pActPet->x = pActMenuWnd->x + 220;
								pActPet->y = pActMenuWnd->y + 124;
							}
							else
							{
								pActPet->x = pActMenuWnd->x + 200;
								pActPet->y = pActMenuWnd->y + 144;
							}
#endif
							pActPet->anim_chr_no = pet[ petStatusNo ].graNo;
							// ?????
							play_se( 217, 320, 240 );
							petWndBtnFlag[ 1 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && petWndBtnFlag[ 1 ] == true ){
							// ????
							petWndBtnFlag[ 1 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 1 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 1 ] = false;
					}
					// ???????????
					if( HitDispNo == petWndFontNo[ 2 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							// ???q????????????
							if( pActMenuWnd3 != NULL ){
								// ????????
								DeathAction( pActMenuWnd3 );
								pActMenuWnd3 = NULL;
								// ?V????????
								GetKeyInputFocus( &MyChatBuffer );
								// ??????????
								play_se( 203, 320, 240 );
							}
							while( 1 ){
								petStatusNo++;
								// ????????
								if( petStatusNo >= 5 ) petStatusNo = 0;
								if( pet[ petStatusNo ].useFlag == true ) break;
							}
							// ????????k???
#ifdef _LIZARDPOSITION			   // (?????) Syu ADD ???????????��?��???
							if((pet[ petStatusNo ].graNo==101493)||(pet[ petStatusNo ].graNo==101494)||
								(pet[ petStatusNo ].graNo==101495)||(pet[ petStatusNo ].graNo==101496))
							{
								pActPet->x = pActMenuWnd->x + 220;
								pActPet->y = pActMenuWnd->y + 124;
							}
							else
							{
								pActPet->x = pActMenuWnd->x + 200;
								pActPet->y = pActMenuWnd->y + 144;
							}
#endif
							pActPet->anim_chr_no = pet[ petStatusNo ].graNo;
							// ?????
							play_se( 217, 320, 240 );
							petWndBtnFlag[ 2 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && petWndBtnFlag[ 2 ] == true ){
							// ????
							petWndBtnFlag[ 2 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 2 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 2 ] = false;
					}
					if (HitFontNo == petWndCalcGrowUp && mouse.onceState & MOUSE_LEFT_CRICK)
					{
						play_se(202, 320, 240);
						lssproto_petGrowUp_send(sockfd, 0, petStatusNo);
					}
					// ???q??????
					if( HitDispNo == petWndFontNo[ 3 ] ){
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ???????????????u?
							if( pet[ petStatusNo ].changeNameFlag == true ){
								if( pActMenuWnd3 == NULL ){
									// ???????????????
									pActMenuWnd3 = MakeWindowDisp( 4, 4 + 280 + 56, 272, 88, 0, -1 );
									// ????????
									play_se( 202, 320, 240 );
									// ???q????
									petNameChange.buffer[ 0 ] = NULL;
									petNameChange.cnt = 0;
									petNameChange.cursor=0;
									// ????????
									petNameChange.len = 16;
									// ???????
									petNameChange.color = 0;
									// ????????
									petNameChange.x = pActMenuWnd3->x + 22;
									petNameChange.y = pActMenuWnd3->y + 25;
									// ???????????I??
									petNameChange.fontPrio = FONT_PRIO_FRONT;
								}else{	// ?????????????
									// ????????
									DeathAction( pActMenuWnd3 );
									pActMenuWnd3 = NULL;
									// ?V????????
									GetKeyInputFocus( &MyChatBuffer );
									// ??????????
									play_se( 203, 320, 240 );
								}
								petWndBtnFlag[ 3 ] = true;

							}else{
								// ???
								play_se( 220, 320, 240 );
							}
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && petWndBtnFlag[ 3 ] == true ){
							// ????
							petWndBtnFlag[ 3 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 3 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 3 ] = false;
					}
					// ???????????
					if( HitDispNo == petWndFontNo[ 7 ] ){
						// ????????
						if( pet[ petStatusNo ].graNo!=100451 && pet[ petStatusNo ].graNo!=100432 &&
							pet[ petStatusNo ].graNo!=101280 && pet[ petStatusNo ].graNo!=101281 &&
							pet[ petStatusNo ].graNo!=100015 && pet[ petStatusNo ].graNo!=100018 &&
							pet[ petStatusNo ].graNo!=101279 && pet[ petStatusNo ].graNo!=100401 &&
							pet[ petStatusNo ].graNo!=101414 && pet[ petStatusNo ].graNo!=101167 &&
							pet[ petStatusNo ].graNo!=101172 && pet[ petStatusNo ].graNo!=102011 &&
							pet[ petStatusNo ].graNo!=102012 )	// fix ??��???????????
							if( mouse.onceState & MOUSE_LEFT_CRICK ){

								int tblNo = pet[ petStatusNo ].graNo - 100250; // ??????k?
#if defined(__ALBUM_47)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_46)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_45)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_44)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_43)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_42)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_41)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_40)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_39)
								if( tblNo > 1800 ){					//��???
									tblNo -= 1208;
								}else if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_38)
								if( tblNo > 1755 ){					//???????????? ????
									tblNo -= 1201 ;
								}else if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_37)
								if (tblNo > 1739 ){			//????
									tblNo -= 1200;
								}else if (tblNo > 1686 ){			//8.0????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_36)
								if (tblNo > 1686 ){			//??????4 ???2	8.0????????????
									tblNo -= 1175;
								}else if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_35)
								if (tblNo > 1641 ){			//??????4 ???2	8.0????????????
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_34)
								if (tblNo > 1639 ){			//??????4
									tblNo -= 1167;
								}else if (tblNo > 1635 ){   //????
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_33)
								if (tblNo > 1635 ){
									tblNo -= 1148;
								}else if ( tblNo > 1634 ){	//��?1 ��?2 ????
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_32)
								if ( tblNo > 1634 ){	//��?1 ��?2
									tblNo -= 1149;
								}else if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_31)
								if ( tblNo > 1628 ){	//????1 ????2 ???1 ???2
									tblNo -= 1147;
								}else if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_30)
								if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ???? ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_29)
								if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ?????? ????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_28)
								if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2 ??????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_27)
								if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ?????????? ???1 ???2
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_26)
								if( tblNo > 1615 ){//???????? ???1 ???2 ????1 ????2 ??????????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_25)
								if( tblNo > 1615 ){//????????
									tblNo -= 1145;
								}else if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_24)
								if( tblNo > 1568 ){//??????
									tblNo -= 1103;
								}else if( tblNo > 1564 ){//???????
									tblNo -= 1101;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_23)
								if( tblNo > 1518 ){
									tblNo -= 1057;
								}else if( tblNo > 1516 ){
									tblNo -= 1055;
								}else if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_22)
								if ( tblNo == 1516 ){//??????????
									tblNo = 455 ;
								}else if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_21)
								if ( tblNo > 1509 ){
									tblNo -= 1054;
								}else if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_20)
								if ( tblNo == 1497 ){
									tblNo = 395;
								}else if ( tblNo > 1495 ){
									tblNo -= 1044 ;
								}else if ( tblNo > 1490 ){
									tblNo -= 1043 ;
								}else if ( tblNo > 1485 ){
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}

#elif defined(__ALBUM_19)
								if ( tblNo > 1485 ) {
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_18)
								if ( tblNo > 1490 ){
									tblNo -= 1044;
								}else if ( tblNo > 1485 ) {
									tblNo -= 1040 ;
								}else if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_17)
								if ( tblNo > 1454 ) {
									tblNo -= 1034 ;
								}else if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_16)
								if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#elif defined(__ALBUM_15)
								if ( tblNo == 1214 ) {
									tblNo = 331 ;
								}else if ( tblNo == 1217 ) {
									tblNo = 328 ;
								}else if ( tblNo > 1361){
									tblNo -= 952;
								}else if( tblNo > 1356){
									tblNo -= 999;
								}else if( tblNo > 1281){
									tblNo -= 919;
								}else if( tblNo > 1239){
									tblNo -= 889;
								}else if( tblNo > 1186 ){
									tblNo -= 886;
								}else if( tblNo > 1173 ){
									tblNo -= 877;
								}else if( tblNo > 1172){
									tblNo -= 877;
								}else if( tblNo > 1161){
									tblNo -= 868;
								}else if( tblNo > 930){
									tblNo -= 745;
								}else if( tblNo == 927){
									tblNo -= 634;
								}else if( tblNo > 929){
									tblNo -= 644;
								}else if( tblNo > 927){
									tblNo -= 643;
								}else if( tblNo > 900){
									tblNo -= 621;
								}else if( tblNo > 180){
									tblNo -= (333+56);
								}
#endif
								int albumNo;
								// ???????
								if( 0 <= tblNo && tblNo < MAX_PET_TBL ){
									// ?????????

									if( tblNo == 442 )
										albumNo = 0;

									albumNo = PetAlbumTbl[ tblNo ].albumNo;
									// ????????
									if( albumNo != -1 ){
										// ?????????????
										PetAlbum[ albumNo ].flag = 2;

										// ????????
										if( pet[ petStatusNo ].freeName[ 0 ] != NULL ){
											strcpy( PetAlbum[ albumNo ].freeName, pet[ petStatusNo ].freeName );
										}else{
											// ?o?????
											strcpy( PetAlbum[ albumNo ].freeName, PetAlbumTbl[ tblNo ].name );
										}
										// ?o?????
										strcpy( PetAlbum[ albumNo ].name, PetAlbumTbl[ tblNo ].name );
										// ????????k?
										PetAlbum[ albumNo ].faceGraNo = PetAlbumTbl[ tblNo ].faceGraNo;
										// ???
										PetAlbum[ albumNo ].level = pet[ petStatusNo ].level;
										// ?????
										PetAlbum[ albumNo ].maxHp = pet[ petStatusNo ].maxHp;
										// ??
										PetAlbum[ albumNo ].str = pet[ petStatusNo ].atk;
										// ????
										PetAlbum[ albumNo ].quick = pet[ petStatusNo ].quick;
										// ?\?
										PetAlbum[ albumNo ].def = pet[ petStatusNo ].def;

										// ??
										PetAlbum[ albumNo ].earth = pet[ petStatusNo ].earth;
										// ?
										PetAlbum[ albumNo ].water = pet[ petStatusNo ].water;
										// ?
										PetAlbum[ albumNo ].fire = pet[ petStatusNo ].fire;
										// ?
										PetAlbum[ albumNo ].wind = pet[ petStatusNo ].wind;

										// ?????????
										play_se( 201, 320, 240 );
										// ???????????
										sprintf( moji,"%s ????????", PetAlbum[ albumNo ].freeName );
										// ????????????????????
										StockChatBufferLine( moji, FONT_PAL_WHITE );

										petWndBtnFlag[ 7 ] = true;
										// ???????V
										SaveAlbum( albumNo );

									}else{
										// ???
										play_se( 220, 320, 240 );
#ifdef _DEBUG
										sprintf( moji,"??��?????? %d",pet[ petStatusNo ].graNo );
										//MessageBox( hWnd, moji, "???", MB_OK | MB_ICONSTOP );
#endif
										//	}
									}
								}else{
									// ???
									play_se( 220, 320, 240 );
#ifdef _DEBUG
									sprintf( moji,"?????table??????? %d",tblNo );
									//MessageBox( hWnd, moji, "???", MB_OK | MB_ICONSTOP );
#endif
								}

						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && petWndBtnFlag[ 7 ] == true ){
							// ????
							petWndBtnFlag[ 7 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 7 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 7 ] = false;
					}
					// ????????
					if( pActMenuWnd != NULL ){

#define PET_WND_ATTR_X 159
#define PET_WND_ATTR_Y 5

						int x2; // ????????
						// ????@?e
						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 35;

						// ????????
						if( pet[ petStatusNo ].freeName[ 0 ] != NULL )
							// ????????????
							CenteringStr( pet[ petStatusNo ].freeName, moji, PET_NAME_LEN );
						else
							// ????????????
							CenteringStr( pet[ petStatusNo ].name, moji, PET_NAME_LEN );
						StockFontBuffer( x - 7, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji,"%dת", pet[ i ].trn );
						StockFontBuffer( x + 150, y, FONT_PRIO_FRONT, FONT_PAL_GREEN, 	moji, 0 );
						sprintf(moji, "计算成长");
						petWndCalcGrowUp = StockFontBufferUtf(x + 175, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 2, 0);
						y += 24;

						// ???q???
						petWndFontNo[ 3 ] = StockDispBuffer( x + 66, y + 7, DISP_PRIO_IME3, CG_NAME_CHANGE_BTN + petWndBtnFlag[ 3 ], 2 );
						y += 23;
						sprintf( moji,"%3d",pet[ petStatusNo ].level );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
						sprintf( moji, "%8d", pet[ petStatusNo ].exp );
						StockFontBuffer( x + 48, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
						sprintf( moji, "%8d", pet[ petStatusNo ].maxExp );
						StockFontBuffer( x + 48, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

						sprintf( moji, "%4d", pet[ petStatusNo ].hp );
						StockFontBuffer( x + 46, y, FONT_PRIO_FRONT, 0, moji, 0 );
						sprintf( moji, "%4d", pet[ petStatusNo ].maxHp );
						StockFontBuffer( x + 48 + 45, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

						// ??N?????
						if( pet[ petStatusNo ].earth > 0 ){
							x2 = ( int )( pet[ petStatusNo ].earth * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
						}
						sprintf( moji, "%3d", pet[ petStatusNo ].atk );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

						// ??N?????
						if( pet[ petStatusNo ].water > 0 ){
							x2 = ( int )( pet[ petStatusNo ].water * 0.8 );

							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
						}
						sprintf( moji, "%3d", pet[ petStatusNo ].def );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

						// ??N?????
						if( pet[ petStatusNo ].fire > 0 ){
							x2 = ( int )( pet[ petStatusNo ].fire * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
						}
						sprintf( moji, "%3d", pet[ petStatusNo ].quick );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

						// ??N?????
						if( pet[ petStatusNo ].wind > 0 ){
							x2 = ( int )( pet[ petStatusNo ].wind * 0.8 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 0, y + PET_WND_ATTR_Y + 0, x + PET_WND_ATTR_X + 0 + x2, y + PET_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 1, y + PET_WND_ATTR_Y + 1, x + PET_WND_ATTR_X + 1 + x2, y + PET_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
							StockBoxDispBuffer( x + PET_WND_ATTR_X + 2, y + PET_WND_ATTR_Y + 2, x + PET_WND_ATTR_X + 2 + x2, y + PET_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
						}
						sprintf( moji, "%3d", pet[ petStatusNo ].ai );
						StockFontBuffer( x + 50, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
						y += 12;
						// ???????????
						petWndFontNo[ 7 ] = StockDispBuffer( x + 52, y, DISP_PRIO_IME3, CG_ALBUM_WND_SNAP_BTN_UP + petWndBtnFlag[ 7 ], 2 );
						petWndFontNo[ 4 ] = StockDispBuffer( x + 178, y, DISP_PRIO_IME3, CG_PET_WND_WAZA_BTN, 2 ); y += 31;
						petWndFontNo[ 0 ] = StockDispBuffer( x + 100 + 70, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
						petWndFontNo[ 1 ] = StockDispBuffer( x + 16 + 16 + 8, y, DISP_PRIO_IME3, CG_PREV_BTN + petWndBtnFlag[ 1 ], 2 );
						petWndFontNo[ 2 ] = StockDispBuffer( x + 50 + 16 + 18, y, DISP_PRIO_IME3, CG_NEXT_BTN + petWndBtnFlag[ 2 ], 2 );
					}
				}
			}

			// ???q????????????
			if( pActMenuWnd3 != NULL ){
				// ????????????????
				if( pActMenuWnd3->hp > 0 ){
					// ???????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd3->pYobi )->my, DISP_PRIO_MENU, CG_NAME_CHANGE_WND, 1 );
					// ?V????????x
					GetKeyInputFocus( &petNameChange );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ???q?????????
						if( HitDispNo == petWndFontNo[ 5 ] ){
							// ????????
							KeyboardReturn();

						}
						// ???q???????????
						if( HitDispNo == petWndFontNo[ 6 ] ){
							// ????????
							DeathAction( pActMenuWnd3 );
							pActMenuWnd3 = NULL;
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ????????????????
					if(	pActMenuWnd3 != NULL ){
						// ?????????
						StockFontBuffer2( &petNameChange );

						// ????@?e
						x = pActMenuWnd3->x + 20;
						y = pActMenuWnd3->y + 60;
						petWndFontNo[ 5 ] = StockDispBuffer( x + 60, y + 7, DISP_PRIO_IME3, CG_OK_BTN, 2 );
						petWndFontNo[ 6 ] = StockDispBuffer( x + 170, y + 7, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
					}
				}
			}
			break;

		case 2: // ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 348, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;

			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ?????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_PET_WAZA_WND, 1 );
					// ???????????
					for( i = 0 ; i < pet[ petStatusNo ].maxSkill ; i++ ){
						// ?????????
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_PET_WAZA_BAR_1 + i, 1 );
					}
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????????????
						if( HitDispNo == petWndFontNo[ 7 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							petWndNo = 1;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ?q??????????
					if( HitDispNo == petWndFontNo[ 8 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							while( 1 ){
								petStatusNo--;
								// ????????
								if( petStatusNo <= -1 ) petStatusNo = 4;
								if( pet[ petStatusNo ].useFlag == true ) break;
							}
							// ????R?e???
							for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
							// ?????
							play_se( 217, 320, 240 );
							petWndBtnFlag[ 8 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && petWndBtnFlag[ 8 ] == true ){
							// ????
							petWndBtnFlag[ 8 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 8 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 8 ] = false;
					}
					// ???????????
					if( HitDispNo == petWndFontNo[ 9 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							while( 1 ){
								petStatusNo++;
								// ????????
								if( petStatusNo >= 5 ) petStatusNo = 0;
								if( pet[ petStatusNo ].useFlag == true ) break;
							}
							// ????R?e???
							for( i = 0 ; i < MENU_PET_0 ; i++ ) petWndFontNo[ i ] = -2;
							// ?????
							play_se( 217, 320, 240 );
							petWndBtnFlag[ 9 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && petWndBtnFlag[ 9 ] == true ){
							// ????
							petWndBtnFlag[ 9 ] = true;
						}else{
							// ?????
							petWndBtnFlag[ 9 ] = false;
						}
					}else{
						// ?????
						petWndBtnFlag[ 9 ] = false;
					}

					// ????????
					if( pActMenuWnd != NULL ){

						// ????@?e
						x = pActMenuWnd->x + 32;
						y = pActMenuWnd->y + 252;

						// ?C???
						for( i = 0 ; i < pet[ petStatusNo ].maxSkill ; i++ ){
							// ????????
							if( petSkill[ petStatusNo ][ i ].useFlag == true ){
								// ???????
								if( HitFontNo == petWndFontNo[ i ] ){
									// ??????
									char *splitPoint = petSkill[ petStatusNo ][ i ].memo;
									// ???????
									while( 1 ){
										// ???????????????
										if( strlen( splitPoint ) > 24 ){
											strncpy( moji, splitPoint, 24 );
											moji[ 24 ] = NULL;	// ???Z??????
											// ?u???S??????
											if( GetStrLastByte( moji ) == 3 ){
												moji[ 23 ] = NULL;
												splitPoint += 23;
											}else{
												moji[ 24 ] = NULL;
												splitPoint += 24;
											}
											StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
										}else{
											strcpy( moji, splitPoint );
											StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
											break;
										}
									}
#ifdef _DEBUG
									// ????????
									if( mouse.onceState & MOUSE_RIGHT_CRICK ){
										// ???????????
										if( petSkill[ petStatusNo ][ i ].field != PETSKILL_FIELD_BATTLE ){
											int j;
											int cnt = 0;
											switch( petSkill[ petStatusNo ][ i ].skillId ){

											case PETSKILL_MERGE: 	// ??G???????
												// ?K???????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEMSTART ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 1 ) cnt++;
												}
												break;

											case PETSKILL_MERGE2: 	// ?????????
												// ?K???????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 2 ) cnt++;
												}
												break;
											}

											// ?K??????????
											if( cnt >= 2 ){
												// ??????
												moji[ 0 ] = NULL;
												// ??????????????
												for( j = MAX_ITEMSTART ; j <  MAX_ITEM ; j++ ){
													// ????????????
													if( ItemBuffer[ j ].mixFlag >= true ){
														char work[ 256 ];
														// ???????
														sprintf( work,"%d|", j );
														strcat( moji, work );
														// ??????
														ItemBuffer[ j ].mixFlag = false;
													}
												}
												// ???|????????
												moji[ strlen( moji ) - 1 ] = NULL;

												// ????????????????????????
												if( bNewServer)
													lssproto_PS_send( sockfd, mixPetNo, i, 0, moji );
												else
													old_lssproto_PS_send( sockfd, mixPetNo, i, 0, moji );

												// ????????
												play_se( 212, 320, 240 );
											}else{
												// ???
												play_se( 220, 320, 240 );
											}
										}else{
											// ???
											play_se( 220, 320, 240 );
										}
									}
#endif
									// ????????
									if( mouse.onceState & MOUSE_LEFT_CRICK ){
										// ????????????????????????????????????????
										if( petSkill[ petStatusNo ][ i ].field != PETSKILL_FIELD_BATTLE && pActPet2 == NULL
											/* && MenuToggleFlag & JOY_CTRL_I*/ ){
											int j;
											int cnt = 0;
											// ???????????
											switch( petSkill[ petStatusNo ][ i ].skillId ){

											case PETSKILL_MERGE: 	// ??G???????
												// ?K???????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 1 ) cnt++;
												}
												break;

											case PETSKILL_MERGE2: 	// ?????????
												// ?K???????????????????
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 2 ) cnt++;
												}
												break;
											case PETSKILL_INSLAY: //andy_add inslay
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 10 ) cnt++;
												}
												break;
											case PETSKILL_FIXITEM:
											case PETSKILL_FIXITEM2:
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 11 ) cnt++;
												}
												break;
#ifdef _ITEM_INTENSIFY
											case PETSKILL_INTENSIFY:
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 12 ) cnt++;
												}
												break;
#endif

#ifdef _ALCHEMIST
											case PETSKILL_ALCHEMIST:
												for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ ){
													if( ItemBuffer[ j ].mixFlag == 9 ) cnt++;
												}
												break;
#endif
											}
											// ?K??????????
											if( cnt >= 2 ){

												// ????????????????
												if( pActPet2 == NULL ){
													// ???????????
													pActPet2 = MakeAnimDisp( 750, 240, pet[ petStatusNo ].graNo, ANIM_DISP_MIX_PET_INIT );
													// ???????k???
													ItemMixPetNo = petStatusNo;
												}
												// ??k???
												SelectWazaNo = i;
												// ???????????k???
												mixPetNo = petStatusNo;

												// ????????
												play_se( 212, 320, 240 );
											}else{
													// ?????????????????
													if( !( MenuToggleFlag & JOY_CTRL_I ) || ( MenuToggleFlag & JOY_CTRL_I && itemWndNo != 0 ) ){
														int j;
														MenuToggleFlag |= JOY_CTRL_I;	// CTRL + I ??
														// ?????????k????
														for( j = 0 ; j < MENU_ITEM_0 ; j++ ) itemWndFontNo[ j ] = -2;
														for( j = 0 ; j < MENU_ITEM_0 ; j++ ) itemWndBtnFlag[ j ] = 0;
														itemWndNo = 0;
														mouse.itemNo = -1;
														itemWndDropGold = 0;

														// ????????????
														if( BattleResultWndFlag >= 1 ) DeathMenuAction();
														// ????????????
														DeathMenuAction2();
														// ????????
														saveUserSetting();

														MenuToggleFlag &= ~JOY_CTRL_M;	// ?????????????
														// ?P?f???????????
														BattleResultWndFlag = false;
														// ?????????
														MapWmdFlagBak = false;
														// ????????
														play_se( 202, 320, 240 );
													}else
														// ???
														play_se( 220, 320, 240 );
											}
										}else{
											// ???
											play_se( 220, 320, 240 );
										}
									}
								}
							}
						}

						x = pActMenuWnd->x + 40;
						y = pActMenuWnd->y + 32;
						if( pet[ petStatusNo ].freeName[ 0 ] != NULL )
							CenteringStr( pet[ petStatusNo ].freeName, moji, PET_NAME_LEN );
						else
							CenteringStr( pet[ petStatusNo ].name, moji, PET_NAME_LEN );
						StockFontBuffer( x - 28, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 26; x += 18;
						for( i = 0 ; i < pet[ petStatusNo ].maxSkill ; i++ ){
							if( petSkill[ petStatusNo ][ i ].useFlag == true ){
								int color = FONT_PAL_GRAY;
								sprintf( moji,"       %-22s",petSkill[ petStatusNo ][ i ].name );
								if(	petSkill[ petStatusNo ][ i ].field != PETSKILL_FIELD_BATTLE ) color = FONT_PAL_WHITE;
								petWndFontNo[ i ] = StockFontBuffer( x - 43, y, FONT_PRIO_FRONT, color, moji, 2 );
							}
							y += 25;
						}

						// ????@?e
						x = pActMenuWnd->x + 20;
						y = pActMenuWnd->y + 330;

						// ???????????????
						petWndFontNo[ 7 ] = StockDispBuffer( x + 100 + 70, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
						petWndFontNo[ 8 ] = StockDispBuffer( x + 16 + 16 + 8, y, DISP_PRIO_IME3, CG_PREV_BTN + petWndBtnFlag[ 8 ], 2 );
						petWndFontNo[ 9 ] = StockDispBuffer( x + 50 + 16 + 18, y, DISP_PRIO_IME3, CG_NEXT_BTN + petWndBtnFlag[ 9 ], 2 );
					}
				}
			}
			break;
#ifdef _STREET_VENDOR
		case 3: petWndNo = 0; break;
#endif
		}
	}

//? ??????????? *******************************************************/
	if( MenuToggleFlag & JOY_CTRL_I ){
		int x1 = 0 , y1 = 0;
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
		SkillWndflag = false ;
/*		if( pActMenuWnd3 != NULL ) {
			DeathAction ( pActMenuWnd3 ) ;
			pActMenuWnd3 = NULL ;
			GetKeyInputFocus( &MyChatBuffer );
		}
*/
#endif

		// ??????k?????
		switch( itemWndNo ){

		case 0:	// ????  ??????

			// ?????????
			if( pActMenuWnd2 == NULL )
			{
				// ???????????????
#ifndef _PET_ITEM
				pActMenuWnd2 = MakeWindowDisp( 365  , 4, 271, 440, 0, -1 );
				// ??????????
				InitItem( pActMenuWnd2->x, pActMenuWnd2->y, 0);
#else
				pActMenuWnd2 = MakeWindowDisp(351 + DISPLACEMENT_X, 4, 271, 440, 0, -1);
				pActMenuWnd2->x += 14;
				((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx = 271 / 2 + pActMenuWnd2->x;
				InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
#endif
				// ?k??????????k????
				itemNo = -1;
				// ???????k????
				ItemMixPetNo = -1;
				// ????????????k?????????
				if( MenuToggleFlag & JOY_CTRL_E && mailWndNo == MAIL_WND_ITEM ){
					// ?????????????
					MenuToggleFlag &= ~JOY_CTRL_E;
					DeathMenuAction();
				}
#ifdef _MONEYINPUT //Syu ADD ???????????
				MymoneyBuffer.buffer[0] = NULL;
				MymoneyBuffer.cnt = 0;
				MymoneyBuffer.cursor = 0;
				MymoneyBuffer.len = 8;
				MymoneyBuffer.color = 0;
				MymoneyBuffer.x = pActMenuWnd2->x +191;
				MymoneyBuffer.y = pActMenuWnd2->y +138;
				MymoneyBuffer.fontPrio = FONT_PRIO_FRONT;
				x1 = pActMenuWnd2->x + 173;
				y1 = pActMenuWnd2->y + 133;
#endif
#ifdef _PET_ITEM
				nSelectPet = -1;

				for (i = 0; i < MAX_PET; i++)
				{
					// ???????
					if (pet[i].useFlag)
					{
						nSelectPet = i;
						break;
					}
				}
#endif
			}
			else
			{
				//andy_log ?????��????
				if( pActMenuWnd2->hp > 0 )
				{
#ifdef _PET_ITEM
					// ??????????????????????
					x1 = pActMenuWnd2->x - 21;
					y1 = pActMenuWnd2->y + 12;
					if (g_bPetItemWndFlag)
					{
						if (MakeHitBox(x1, y1, x1 + 23, y1 + 60, DISP_PRIO_IME3) == true)	// ?????????????
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								g_bPetItemWndFlag = false;
								InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
							}
					}
					else
					{
						if (MakeHitBox(x1, y1 + 78, x1 + 23, y1 + 142, DISP_PRIO_IME3) == true)	// ????????????
							if (mouse.onceState & MOUSE_LEFT_CRICK)
							{
								g_bPetItemWndFlag = true;
								InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
							}
					}
					if (g_bPetItemWndFlag)
						StockDispBuffer(((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx - 14, ((WINDOW_DISP*)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, 26456, 1);
					else
						StockDispBuffer(((WINDOW_DISP*)pActMenuWnd2->pYobi)->mx - 14, ((WINDOW_DISP*)pActMenuWnd2->pYobi)->my, DISP_PRIO_MENU, CG_NEWITEM_WND, 1);
#else
#ifdef _ITEM_EQUITSPACE
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_MENU, CG_NEWITEM_WND, 1 );
#else
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_MENU, CG_ITEM_WND_0, 1 );

					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_MENU, CG_ITEM_WND_1, 1 );
#endif
#endif
#ifdef _MONEYINPUT //Syu ADD ???????????
#ifdef _PET_ITEM
					if (!g_bPetItemWndFlag)	// ???????????????????????
#endif
					{
						x1 = pActMenuWnd2->x + 175;
						y1 = pActMenuWnd2->y + 133;
						if( MakeHitBox( x1 , y1, x1 + 86 , y1 + 25 , DISP_PRIO_IME3 ) == true )
						{
							if(mouse.onceState & MOUSE_LEFT_CRICK)
							{
								strcpy(MymoneyBuffer.buffer,"");
								MymoneyBuffer.buffer[0] = NULL;
								MymoneyBuffer.cursor = 0;
								MymoneyBuffer.cnt = 0;
								Moneyflag = true;
								GetKeyInputFocus( &MymoneyBuffer );
								play_se( 217, 320, 240 );
							}
						}
					}
					if( Moneyflag == true)
					{
						MymoneyBuffer.x = pActMenuWnd2->x +240 - strlen(MymoneyBuffer.buffer) * 7;
						StockFontBuffer2( &MymoneyBuffer );
						itemWndDropGold = atoi(MymoneyBuffer.buffer);
						if( atoi(MymoneyBuffer.buffer) >= pc.gold )
						{
							sprintf(MymoneyBuffer.buffer,"%d",pc.gold);
							itemWndDropGold = pc.gold;
						}
					}
#endif
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK )
					{
						// ???????
						if( HitDispNo == itemWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd2 );

							pActMenuWnd2 = NULL;
							// ??????
							DeathAction( pActPet2 );
							pActPet2 = NULL;
							// ???????k????
							ItemMixPetNo = -1;
							// ?????????????
							MenuToggleFlag ^= JOY_CTRL_I;
							// ????????????????????
							if( MenuToggleFlag & JOY_CTRL_S ){

								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								// ????????????
								DeathMenuAction();
							}
							// ??????????
							play_se( 203, 320, 240 );
							itemNo = -1;
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
						}
						// ????????
						if( HitDispNo == itemWndFontNo[ 1 ] ){
							itemWndNo = 1;
							DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							DeathAction( pActPet2 );
							pActPet2 = NULL;
							ItemMixPetNo = -1;
							play_se( 202, 320, 240 );
							if( !( MenuToggleFlag & JOY_CTRL_S ) ){
								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndFontNo[ i ] = -2;
								for( i = 0 ; i < MENU_STATUS_0 ; i++ ) statusWndBtnFlag[ i ] = 0;
								statusWndNo = 1;
								DeathMenuAction();
								saveUserSetting();
								MenuToggleFlag &= JOY_CTRL_I;
								MenuToggleFlag |= JOY_CTRL_S;
							}else{
								if( statusWndNo != 1 ){
									DeathAction( pActMenuWnd );
									pActMenuWnd = NULL;
									statusWndNo = 1;
								}
							}
						}
						if( itemNo != -1 && MenuToggleFlag & JOY_CTRL_S && statusWndNo == 1 ){
							for( i = 0 ; i < 11 ; i++ ){
								if( HitFontNo == statusWndFontNo[ i ] && eventWarpSendFlag == false ){
									if( bNewServer)
										lssproto_ID_send( sockfd, nowGx, nowGy, itemNo, i );
									else
										old_lssproto_ID_send( sockfd, nowGx, nowGy, itemNo, i );
									play_se( 212, 320, 240 );
									itemNo = -1;

								}
							}
						}
#ifdef _PET_ITEM
						// ???????????????
						if (g_bPetItemWndFlag)
						{
							// ??????????
							if (HitDispNo == itemWndFontNo[5])
							{
								if (nSelectPet != -1)
								{
									do
									{
										nSelectPet--;
										if (nSelectPet == pc.ridePetNo)
											nSelectPet--;
										if (nSelectPet < 0)
											nSelectPet = MAX_PET - 1;
									}
									while (!pet[nSelectPet].useFlag);
								}
							}
							// ??????????
							if (HitDispNo == itemWndFontNo[6])
							{
								if (nSelectPet != -1)
								{
									do
									{
										nSelectPet++;
										if (nSelectPet == pc.ridePetNo)
											nSelectPet++;
										if (nSelectPet >= MAX_PET)
											nSelectPet = 0;
									}
									while (!pet[nSelectPet].useFlag);
								}
							}
						}
#endif
					}

					if( HitDispNo == itemWndFontNo[ 2 ] ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							if( itemWndDropGold > 0 && eventWarpSendFlag == false ){
								itemWndBtnFlag[ 2 ] = true;
								play_se( 217, 320, 240 );
							}else{
								play_se( 220, 320, 240 );
							}
						}
						if( mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[ 2 ] == true ){
							itemWndBtnFlag[ 2 ] = false;
							play_se( 212, 320, 240 );
							if( bNewServer)
								lssproto_DG_send( sockfd, nowGx, nowGy, itemWndDropGold );
							else
								old_lssproto_DG_send( sockfd, nowGx, nowGy, itemWndDropGold );
							itemWndDropGold = 0;
#ifdef _MONEYINPUT //Syu ADD ???????????
							Moneyflag = false;
#endif

						}
					}
					else
						itemWndBtnFlag[ 2 ] = false;

					if( HitDispNo == itemWndFontNo[ 3 ] )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[ 3 ] == true ){

							itemWndBtnFlag[ 3 ] = false;
						}
						// ??????
						if( itemWndBtnFlag[ 3 ] == true ){
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							// ???
							itemWndDropGold += itemWndDropGoldInc;
							// ?????????????????
							itemWndDropGoldCnt++;
							// ???????
							if( itemWndDropGoldCnt >= 30 ){
								// ?????????????????
								itemWndDropGoldCnt = 0;
								// ????????
								if( itemWndDropGoldInc == 0 ) itemWndDropGoldInc = 1;
								else{
									// ???????
									itemWndDropGoldInc *= 5;
									// ????????
									if( itemWndDropGoldInc > 10000 ) itemWndDropGoldInc = 10000;
								}
							}
							// ????????
							if( itemWndDropGold >= pc.gold ){
								itemWndDropGold = pc.gold;
								// ???
								play_se( 220, 320, 240 );
							}
						}
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ???
							itemWndDropGold++;
							// ????????
							if( itemWndDropGold >= pc.gold ){
								itemWndDropGold = pc.gold;
								// ???
								play_se( 220, 320, 240 );
							}else{
								// ?????????
								itemWndDropGoldInc = 0;
								// ?????????????????
								itemWndDropGoldCnt = 0;
								// ???????
								itemWndBtnFlag[ 3 ] = true;
								// ?????
								play_se( 217, 320, 240 );
							}
						}

					}
					else
						// ????????
						itemWndBtnFlag[ 3 ] = false;

					// ??????????
					if( HitDispNo == itemWndFontNo[ 4 ] )
					{
						// ???????????????
						if( mouse.onceState & MOUSE_LEFT_CRICK_UP && itemWndBtnFlag[ 4 ] == true ){
							itemWndBtnFlag[ 4 ] = false;
						}
						// ??????
						if( itemWndBtnFlag[ 4 ] == true ){
							// ????
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							itemWndDropGold -= itemWndDropGoldInc;
							// ?????????????????
							itemWndDropGoldCnt++;
							// ???????
							if( itemWndDropGoldCnt >= 30 ){
								// ?????????????????
								itemWndDropGoldCnt = 0;
								// ????????
								if( itemWndDropGoldInc == 0 ) itemWndDropGoldInc = 1;
								else{
									// ???????
									itemWndDropGoldInc *= 5;
									// ????????
									if( itemWndDropGoldInc > 10000 ) itemWndDropGoldInc = 10000;
								}
							}
							// ????????
							if( itemWndDropGold < 0 ){
								itemWndDropGold = 0;
								// ???
								play_se( 220, 320, 240 );
							}
						}
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ????
							itemWndDropGold--;
							// ????????
							if( itemWndDropGold <= 0 ){
								itemWndDropGold = 0;
								// ???
								play_se( 220, 320, 240 );
							}else{
								// ?????????
								itemWndDropGoldInc = 0;
								// ?????????????????
								itemWndDropGoldCnt = 0;
								// ???????
								itemWndBtnFlag[ 4 ] = true;
								// ?????
								play_se( 217, 320, 240 );

							}
						}
					}
					else
						itemWndBtnFlag[ 4 ] = false;

					if( pActMenuWnd2 != NULL )
					{
						x = pActMenuWnd2->x + 16;
						y = pActMenuWnd2->y + 351 + 7;
						if( itemNo != -1 )
						{
							StockBoxDispBuffer( ItemBuffer[ itemNo ].defX - 24, ItemBuffer[ itemNo ].defY - 24,
								ItemBuffer[ itemNo ].defX + 26, ItemBuffer[ itemNo ].defY + 23,
								DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 0 );
						}
						for(  i = MAX_ITEM -1 ; i >= 0 ; i-- )
						{
#ifdef _PET_ITEM
							// ???????????????,???????????????????,??????????????
							if (g_bPetItemWndFlag && (i >= PET_EQUIPNUM && i < MAX_ITEMSTART))
								continue;
#endif
							if( MakeHitBox( ItemBuffer[ i ].defX - 24, ItemBuffer[ i ].defY - 24,
								ItemBuffer[ i ].defX + 26, ItemBuffer[ i ].defY + 23, DISP_PRIO_IME3 ) == true )
							{
#ifdef _PET_ITEM
								// ??????????????
								if (g_bPetItemWndFlag && (i >= PET_HEAD && i < PET_EQUIPNUM) && nSelectPet > -1)
								{
									if (pet[nSelectPet].item[i].useFlag == true && (ItemBuffer[i].mixFlag <= 2 || ItemBuffer[i].mixFlag == 10))
									{
										char *splitPoint = pet[nSelectPet].item[i].memo;
										char damage_msg[256];
										int color = pet[nSelectPet].item[i].color;

										StockFontBuffer(pActMenuWnd2->x + 16, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, pet[nSelectPet].item[i].name, 0);

										// ??????????
										sprintf(damage_msg, "�;ö�(%s)", pet[nSelectPet].item[i].damage);
										StockFontBuffer(pActMenuWnd2->x + 150, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, damage_msg, 0);

										while (1)
										{
											if (strlen(splitPoint) > 28)
											{
												strncpy(moji, splitPoint, 28);
												moji[28] = NULL;
												if (GetStrLastByte(moji) == 3)
												{
													moji[27] = NULL;
													splitPoint += 27;
												}
												else
												{
													moji[28] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0);
												y += 24;
											}
											else
											{
												strcpy(moji, splitPoint);
												StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0);
												break;
											}
										}
									}
								}
								else
									// ???????
#endif
								{
#ifdef _ALCHEMIST // ???Item??? (????Bug)
									if( pc.item[ i ].useFlag == true ){
#else
									if( pc.item[ i ].useFlag == true && (ItemBuffer[ i ].mixFlag <= 2 || ItemBuffer[ i ].mixFlag == 10)){
#endif
										char *splitPoint = pc.item[ i ].memo;
										int color = pc.item[ i ].color;
										if( pc.transmigration == 0 && pc.level < pc.item[ i ].level ) color = FONT_PAL_RED;
										StockFontBuffer( pActMenuWnd2->x + 16, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, pc.item[ i ].name, 0 );
										{

											// ??????????
											char damage_msg[256];
											sprintf( damage_msg, "�;ö�(%s)", pc.item[ i ].damage );
											StockFontBuffer( pActMenuWnd2->x + 150, pActMenuWnd2->y + 331, FONT_PRIO_FRONT, color, damage_msg, 0 );
										}
										while( 1 ){
											if( strlen( splitPoint ) > 28 ){
												strncpy( moji, splitPoint, 28 );
												moji[ 28 ] = NULL;
												if( GetStrLastByte( moji ) == 3 ){
													moji[ 27 ] = NULL;
													splitPoint += 27;
												}else{
													moji[ 28 ] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
											}else{
												strcpy( moji, splitPoint );
												StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
												break;
											}
										}
									}
								}
								if( pActPet2 == NULL )
								{
									if( mouse.onceState & MOUSE_LEFT_CRICK && mouse.itemNo == -1 )
									{
										ItemBuffer[ i ].mixFlag = false;
#ifdef _PET_ITEM
										if (g_bPetItemWndFlag && (i >= PET_HEAD && i < PET_EQUIPNUM) && nSelectPet > -1)
										{
											if (pet[nSelectPet].item[i].useFlag == true)
											{
												ItemBuffer[i].dragFlag = true;
												mouse.itemNo = i;
												itemNo = -1;
											}
										}
										else
#endif
											if( pc.item[ i ].useFlag == true ){
												ItemBuffer[ i ].dragFlag = true;
												mouse.itemNo = i;
												itemNo = -1;
											}
									}
									if (mouse.onceState & MOUSE_LEFT_CRICK_UP)
									{
										int dt = SDL_GetTicks() - lastClickTime;
										lastClickTime = SDL_GetTicks();
										if (lastChoosedItem == i && dt > 1000) {
											SDL_Log("Jerry MergeFlag true");
											mergeItemFlag = true;
										}
										else {
											SDL_Log("Jerry MergeFlag false");
											mergeItemFlag = false;
										}
										lastChoosedItem = i;
										if (mouse.itemNo != -1)
										{
											if (mouse.itemNo != i && ItemBuffer[i].mixFlag == 0)
											{
#ifdef _PET_ITEM
												if (g_bPetItemWndFlag)
												{
													if (nSelectPet > -1 && !(mouse.itemNo >= CHAR_EQUIPPLACENUM && i >= CHAR_EQUIPPLACENUM))
													{
														if (i < CHAR_EQUIPPLACENUM && nSelectPet == pc.ridePetNo)	// ?????????,???????????
															StockChatBufferLine("��費��װװ����??", FONT_PAL_YELLOW);
														else
															lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, mouse.itemNo, i);
													}
													else
													{
														if (bNewServer)
															lssproto_MI_send(sockfd, mouse.itemNo, i);
														else
															old_lssproto_MI_send(sockfd, mouse.itemNo, i);
													}
												}
												else
#endif
												{
													if (bNewServer)
														lssproto_MI_send(sockfd, mouse.itemNo, i);
													else
														old_lssproto_MI_send(sockfd, mouse.itemNo, i);
												}
												play_se(217, 320, 240);
											}
											ItemBuffer[mouse.itemNo].dragFlag = false;
											mouse.itemNo = -1;
										}
									}
									if( mouse.onceState & MOUSE_LEFT_DBL_CRICK && ItemUseTime < timeGetTime() - 500 )
									{
										ItemUseTime = timeGetTime();
#ifdef _PET_ITEM
										// ????????????????????????????????,????��??????????
										if (!g_bPetItemWndFlag)
										{
											if (pc.item[i].useFlag && pc.item[i].type >= ITEM_PET_HEAD && pc.item[i].type < ITEM_CATEGORYNUM)
											{
												g_bPetItemWndFlag = true;
												InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
												break;
											}
										}
										// ???????????????????????????????,????��??????????
										else
										{
											if (pc.item[i].useFlag && pc.item[i].type < ITEM_PET_HEAD && i >= MAX_ITEMSTART)
											{
												g_bPetItemWndFlag = false;
												InitItem(pActMenuWnd2->x, pActMenuWnd2->y, g_bPetItemWndFlag);
												break;
											}
										}
#endif
										if( pc.item[ i ].useFlag != true || pc.item[ i ].field == ITEM_FIELD_BATTLE || pc.transmigration == 0 && pc.level < pc.item[ i ].level ){
											play_se( 220, 320, 240 );
										}else{ // ?????u??
											mouse.itemNo = -1;
											switch( pc.item[ i ].target ){
											case ITEM_TARGET_MYSELF:	// ??????
												if( eventWarpSendFlag == false ){
													if( bNewServer)
														lssproto_ID_send( sockfd, nowGx, nowGy, i, 0 );
													else
														old_lssproto_ID_send( sockfd, nowGx, nowGy, i, 0 );
													play_se( 212, 320, 240 );
												}
												break;
											case ITEM_TARGET_OTHER:	// ?\??????????)
												itemNo = i;
												play_se( 217, 320, 240 );
												if( !( MenuToggleFlag & JOY_CTRL_S ) ){
													int j;
													MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
													for( j = 0 ; j < MENU_STATUS_0 ; j++ ) statusWndFontNo[ j ] = -2;
													for( j = 0 ; j < MENU_STATUS_0 ; j++ ) statusWndBtnFlag[ j ] = 0;
													statusWndNo = 1;
													DeathMenuAction();
													saveUserSetting();
													play_se( 202, 320, 240 );
													MenuToggleFlag &= JOY_CTRL_I;
													MenuToggleFlag |= JOY_CTRL_S;
												}else{
													if( statusWndNo != 1 ){
														DeathAction( pActMenuWnd );
														pActMenuWnd = NULL;
														statusWndNo = 1;
														play_se( 202, 320, 240 );
													}
												}
												break;
#ifdef _PET_ITEM
											case ITEM_TARGET_PET:
												if (eventWarpSendFlag == false)
												{
													// ??????????????????????????????
													if (i >= PET_HEAD && i < PET_EQUIPNUM)
														break;
													if (pc.ridePetNo != -1 && nSelectPet == pc.ridePetNo)	// ???????????
														StockChatBufferLine("��費��װװ����??", FONT_PAL_YELLOW);
													else
														lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, i, -1);
													play_se(212, 320, 240);
												}
												break;
#endif
											}
										}
									}
									//#ifdef _DEBUG
									// ???????????????????????��???
									if( mergeItemFlag && i == lastChoosedItem && mouse.itemNo == -1 )
									{
										mergeItemFlag = false;
										int j;
										int cnt = 0;
										// ?????????????????????????????????
										if( pc.item[ i ].useFlag == true && i >= MAX_ITEMSTART /*&& pc.item[ i ].sendFlag & ITEM_FLAG_MIX */)
										{
											int j;
											int flag = 0;
											// ?k???????k????
											if( itemNo != -1 ) itemNo = -1;
											// ??????????
											for( j = MAX_ITEMSTART ; j <  MAX_ITEM ; j++ )
											{
												// ????????????
												if( ItemBuffer[ j ].mixFlag >= true ){
													flag = ItemBuffer[ j ].mixFlag;
													break;
												}
											}
											// ???????????
											if( ItemBuffer[ i ].mixFlag == false )
											{
												// ?????k??????????
												if( flag == 0 ){
													// ????
													if( pc.item[ i ].sendFlag & ITEM_FLAG_COOKING_MIX ){
														// ??????????????
														if( CheckPetSkill( PETSKILL_MERGE2 ) == true ){
															ItemBuffer[ i ].mixFlag = 2;
															SDL_Log("Jerry CheckPetSkill is true");
															// ?????
															play_se( 217, 320, 240 );
														}else{
															SDL_Log("Jerry CheckPetSkill is false");
															// ???
															play_se( 220, 320, 240 );
														}
													}
													//andy_add
													else if( pc.item[ i ].sendFlag & ITEM_FLAG_METAL_MIX ||
														pc.item[ i ].sendFlag & ITEM_FLAG_JEWEL_MIX ){
														if( CheckPetSkill( PETSKILL_INSLAY) ){
															ItemBuffer[ i ].mixFlag = 10;
															play_se( 217, 320, 240 );
														}else{
															play_se( 220, 320, 240 );
														}
													}else if( pc.item[ i ].sendFlag & ITEM_FLAG_FIX_MIX ){
														if( CheckPetSkill( PETSKILL_FIXITEM) || CheckPetSkill(PETSKILL_FIXITEM2) ){
															ItemBuffer[ i ].mixFlag = 11;
															play_se( 217, 320, 240 );
														}else{
															play_se( 220, 320, 240 );
														}
													}
#ifdef _ITEM_INTENSIFY
													else if( pc.item[ i ].sendFlag & ITEM_FLAG_INTENSIFY_MIX ){
														if( CheckPetSkill( PETSKILL_INTENSIFY ) ){
															ItemBuffer[ i ].mixFlag = 12;
															play_se( 217, 320, 240 );
														}else{
															play_se( 220, 320, 240 );
														}
													}
#endif

											else{
#ifdef _ALCHEMIST // ????????????
														if( CheckPetSkill( PETSKILL_ALCHEMIST ) == true &&
															pc.item[ i ].sendFlag & ITEM_FLAG_MIX &&
															strcmp( pc.item[i].alch, "��") != NULL ){
															ItemBuffer[ i ].mixFlag = 9;
														}
														else
#endif
															// ??????????????
															if( CheckPetSkill( PETSKILL_MERGE ) == true &&
																pc.item[ i ].sendFlag & ITEM_FLAG_MIX ){
																ItemBuffer[ i ].mixFlag = 1;
																// ?????
																play_se( 217, 320, 240 );
															}
															else{
																// ???
																play_se( 220, 320, 240 );
															}
													}
												}else{

													// ????
													if( pc.item[ i ].sendFlag & ITEM_FLAG_COOKING_MIX ){
														if( flag == 2 ){
															ItemBuffer[ i ].mixFlag = 2;
															// ?????
															play_se( 217, 320, 240 );
														}else{
															// ???
															play_se( 220, 320, 240 );
														}
													}

													else{
														if( flag == 1 ){
															ItemBuffer[ i ].mixFlag = 1;
															// ?????
															play_se( 217, 320, 240 );
														}
#ifdef _ALCHEMIST // ??????????????
														else if( flag == 9 ){
															if( strcmp( pc.item[i].alch, pc.item[j].alch) == false ) {
																ItemBuffer[ i ].mixFlag = 9;
															}
															play_se( 217, 320, 240 );
														}
#endif
														else if( flag == 10 ){	//andy_add inslay
															ItemBuffer[ i ].mixFlag = 10;
															play_se( 217, 320, 240 );
														}
														else if( flag == 11 ){	//andy_add fixitem
															ItemBuffer[ i ].mixFlag = 11;
															play_se( 217, 320, 240 );
														}
#ifdef _ITEM_INTENSIFY
														else if( flag == 12 ){
															ItemBuffer[ i ].mixFlag = 12;
															play_se( 217, 320, 240 );
														}
#endif

														else{
															// ???
															play_se( 220, 320, 240 );
														}

													}
												}
											}
											else
											{
#ifdef _ALCHEMIST // ????????????...
												if( ItemBuffer[ i ].mixFlag == 9 ) {
													int k;
													ItemBuffer[ i ].mixFlag = 1;
													for( k = MAX_ITEMSTART ; k <  MAX_ITEM ; k++ ){
														if( ItemBuffer[k].mixFlag >= true && (k != i) ){
															ItemBuffer[ i ].mixFlag = false;
															break;
														}
													}
												}
												else {
													ItemBuffer[ i ].mixFlag = false;
												}
#else
												ItemBuffer[ i ].mixFlag = false;
#endif
												play_se( 217, 320, 240 );
											}
										}
										else
											play_se( 220, 320, 240 );

										for( j = MAX_ITEMSTART ; j < MAX_ITEM ; j++ )
										{
											if( ItemBuffer[ j ].mixFlag >= 1 ) cnt++;
										}
										if( cnt >= 2 )
										{
											if( !( MenuToggleFlag & JOY_CTRL_P ) )
											{

												MenuToggleFlag ^= JOY_CTRL_P;	// CTRL + S ??
												for( j = 0 ; j < MENU_PET_0 ; j++ ) petWndFontNo[ j ] = -2;
												for( j = 0 ; j < MENU_PET_0 ; j++ ) petWndBtnFlag[ j ] = 0;
												petWndNo = 0;
												DeathMenuAction();
												saveUserSetting();
												play_se( 202, 320, 240 );

												MenuToggleFlag &= JOY_CTRL_I;	// ????????????????
												MenuToggleFlag |= JOY_CTRL_P;	// ??????????
											}
										}
									}
								}
							}
#ifdef _PET_ITEM
							if (g_bPetItemWndFlag && nSelectPet > -1 && (i >= PET_HEAD && i < PET_EQUIPNUM) && pet[nSelectPet].useFlag == true)
								StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, ItemBuffer[i].dispPrio, pet[nSelectPet].item[i].graNo, 0);
							else
#endif
							if( pc.item[ i ].useFlag == true )
							{
#ifdef _PET_ITEM
								if (!(g_bPetItemWndFlag && i < CHAR_EQUIPPLACENUM))
#endif
								{
#if 0 //#ifdef _ALCHEMIST
										if( ItemBuffer[ i ].mixFlag <= 9 )
#else
										if( ItemBuffer[ i ].mixFlag <= 2 )
#endif
										{
											StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
#ifdef _ITEM_PILENUMS
											if( pc.item[ i ].pile > 1 )
											{
												char pile[256];
												sprintf( pile, "%d", pc.item[ i ].pile );
												StockFontBuffer( ItemBuffer[ i ].defX+10, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, FONT_PAL_WHITE, pile, 0 );
											}
#endif
										}
#ifdef _ALCHEMIST
										else if( ItemBuffer[ i ].mixFlag == 9 )
										{
											StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
										}
#endif
										else if( ItemBuffer[ i ].mixFlag == 10 )
										{ //andy_add inslay
											StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
										}
										else if( ItemBuffer[ i ].mixFlag == 11 )
										{ //andy_add fixitem
											StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
										}
#ifdef _ITEM_INTENSIFY
										else if( ItemBuffer[ i ].mixFlag == 12 )
										{ //andy_add fixitem
											StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
										}
#endif

										else if( ItemBuffer[ i ].mixFlag == 3 )
										{	// ??????????
											// ????????
											StockDispBuffer( pActPet2->x, pActPet2->y - 16, DISP_PRIO_DRAG, pc.item[ i ].graNo, 0 );
										}
										else if( ItemBuffer[ i ].mixFlag == 4 )
										{	// ??????????
											// ????????
											StockDispBuffer( pActPet2->x, pActPet2->y, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
										}

								}
							}
							// ??????????
							if( ItemBuffer[ i ].mixFlag >= true )
							{
								// ?????????????
								if( pc.item[ i ].useFlag == true ){
									// ??????????
									if( ItemBuffer[ i ].mixFlag == 1 ){
										StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "�ϳ�", 0 );
									}else
										// ??????????
										if( ItemBuffer[ i ].mixFlag == 2 ){
											StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "����", 0 );
										}
#ifdef _ALCHEMIST
										if( ItemBuffer[ i ].mixFlag == 9 ){
											StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "����", 0 );
										}
#endif
										if( ItemBuffer[ i ].mixFlag == 10 ){// PETSKILL_INSLAY ANDY_ADD
											StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "����", 0 );
										}
										if( ItemBuffer[ i ].mixFlag == 11 ){// PETSKILL_FIXITEM ANDY_ADD
											StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "�޸�", 0 );
										}
#ifdef _ITEM_INTENSIFY
										if( ItemBuffer[ i ].mixFlag == 12){
											StockFontBuffer( ItemBuffer[ i ].defX - 17, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, 0, "ǿ��", 0 );
										}
#endif

								}else{
									// ?????????
									ItemBuffer[ i ].mixFlag = true;
								}
							}
						}
						// ?????????
						if( mouse.onceState & MOUSE_LEFT_CRICK_UP )
						{
							// ???????????
							if( mouse.itemNo != -1 ){
								// ????????
								if( mouse.nowPoint.x <= pActMenuWnd2->x && mouse.itemNo != -1 && eventWarpSendFlag == false ){
#ifdef _PET_ITEM
									if (g_bPetItemWndFlag)
										lssproto_PetItemEquip_send(sockfd, nowGx, nowGy, nSelectPet, mouse.itemNo, -2);
									else
#endif
										// ??????t????
										if( bNewServer) {
											lssproto_DI_send( sockfd,nowGx, nowGy, mouse.itemNo );
										}
										else
											old_lssproto_DI_send( sockfd,nowGx, nowGy, mouse.itemNo );

								}
								// ??????????
								ItemBuffer[ mouse.itemNo ].dragFlag = false;
								// ?????????k????
								mouse.itemNo = -1;
								// ?????
								play_se( 217, 320, 240 );
							}
						}
						// ?????????
						if( mouse.itemNo != -1 )
						{
							// ??????????????
							if( mouse.onceState & MOUSE_RIGHT_CRICK )
							{
								// ????t???
								// ??????????
								ItemBuffer[ mouse.itemNo ].dragFlag = false;
								// ?????????k????
								mouse.itemNo = -1;
								// ?????
								play_se( 217, 320, 240 );
							}
							// ????????
#ifdef _PET_ITEM
							if (g_bPetItemWndFlag && nSelectPet > -1 && (mouse.itemNo >= PET_HEAD && mouse.itemNo < PET_EQUIPNUM) && pet[nSelectPet].useFlag == true)
								StockDispBuffer( mouse.nowPoint.x, mouse.nowPoint.y,DISP_PRIO_DRAG, pet[nSelectPet].item[mouse.itemNo].graNo, 0);
							else
#endif
								StockDispBuffer( mouse.nowPoint.x, mouse.nowPoint.y,DISP_PRIO_DRAG, pc.item[ mouse.itemNo ].graNo, 0 );
						}
						// ????????
						if( pActMenuWnd2 != NULL )
						{
							// ???????????????
							// ???????
							itemWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, pActMenuWnd2->y + 422, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
#ifdef _PET_ITEM
							if (g_bPetItemWndFlag)
							{
								itemWndFontNo[1] = -2;
								// ???????
								itemWndFontNo[5] = StockDispBuffer(pActMenuWnd2->x + 188, pActMenuWnd2->y + 142, DISP_PRIO_IME3, 26047, 2);
								// ???????
								itemWndFontNo[6] = StockDispBuffer(pActMenuWnd2->x + 236, pActMenuWnd2->y + 142, DISP_PRIO_IME3, 26048, 2);

								// ???????????
								if (nSelectPet > -1 && pet[nSelectPet].useFlag)
								{
									char szTemp[16];
									unsigned short wSpecies = HIWORD(pet[nSelectPet].fusion);

									if (wSpecies < 0 || wSpecies >= MAX_PET_SPECIES)
										wSpecies = MAX_PET_SPECIES;
									// ????????????????
									for (i = 0; i < PET_EQUIPNUM; i++)
									{
										if (byShowPetItemBackground[wSpecies] & (1 << i))
											StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[i][0], 0);
										else
											StockDispBuffer(ItemBuffer[i].defX, ItemBuffer[i].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[i][1], 0);
									}

									// ???????????
									if (strlen(pet[nSelectPet].freeName) > 0)
										StockFontBuffer(pActMenuWnd2->x + 148, pActMenuWnd2->y + 17, FONT_PRIO_FRONT, 0, pet[nSelectPet].freeName, 0);
									else
										StockFontBuffer(pActMenuWnd2->x + 148, pActMenuWnd2->y + 17, FONT_PRIO_FRONT, 0, pet[nSelectPet].name, 0);
									// ????????????
//									itoa(pet[nSelectPet].maxHp, szTemp, 10);
                                    sprintf(szTemp, "%d", pet[nSelectPet].maxHp);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
//									itoa(pet[nSelectPet].atk, szTemp, 10);
                                    sprintf(szTemp, "%d", pet[nSelectPet].atk);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 25, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
//									itoa(pet[nSelectPet].def, szTemp, 10);s
                                    sprintf(szTemp, "%d", pet[nSelectPet].def);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 49, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
//									itoa(pet[nSelectPet].quick, szTemp, 10);
                                    sprintf(szTemp, "%d", pet[nSelectPet].quick);
									StockFontBuffer(pActMenuWnd2->x + 222, pActMenuWnd2->y + 40 + 73, FONT_PRIO_FRONT, FONT_PAL_WHITE, szTemp, 0);
								}
								else
								{
									nSelectPet = -1;
									for (i = 0; i < MAX_PET; i++)
									{
										// ???????
										if (pet[i].useFlag)
										{
											nSelectPet = i;
											break;
										}
									}
								}
							}
							else
#endif
							{
								itemWndFontNo[ 1 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_IME3, CG_ITEM_WND_JUJUTU_BTN, 2 );
								// ??????
								sprintf( moji,"%7d", pc.gold );
								StockFontBuffer( pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 87, FONT_PRIO_FRONT, 0, moji, 0 );
								// ?????????
								sprintf( moji,"%7d", itemWndDropGold );
#ifdef _MONEYINPUT //Syu ADD ???????????
								if(Moneyflag == false)
									StockFontBuffer( pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 138, FONT_PRIO_FRONT, 0, moji, 0 );
#else
								StockFontBuffer( pActMenuWnd2->x + 32 + 48 * 3 + 16, pActMenuWnd2->y + 138, FONT_PRIO_FRONT, 0, moji, 0 );
#endif
								// ????????
								itemWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_DROP_BTN_UP + itemWndBtnFlag[ 2 ], 2 );
								// ????????
								itemWndFontNo[ 3 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_INC_BTN_UP + itemWndBtnFlag[ 3 ], 2 );
								// ?????????
								itemWndFontNo[ 4 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_IME3, CG_ITEM_WND_GOLD_DEC_BTN_UP + itemWndBtnFlag[ 4 ], 2 );
							}
						}
					}
				}
			}
			break;

		case 1:	// ????k???????

			// ?????????
			if( pActMenuWnd2 == NULL ){
				pActMenuWnd2 = MakeWindowDisp( 364 , 4, 272, 280, 0, -1 );
				// ????
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndBtnFlag[ i ] = 0;
				jujutuNo = -1;	// ?k?????????????

			}else{
				// ????????????????
				if( pActMenuWnd2->hp > 0 ){
					x = pActMenuWnd2->x + 74;
					y = pActMenuWnd2->y + 208;
					// ??????????????
#ifdef _READ16BITBMP
					if(g_bUseAlpha) StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my + 7, DISP_PRIO_MENU, CG_JUJUTU_WND, 1 );
					else
#endif
						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my + 10, DISP_PRIO_MENU, CG_JUJUTU_WND, 1 );
					// ????k?????
					for( i = 0 ; i < 5 ; i++ ){
						if( HitFontNo == itemWndFontNo[ i ] ){
							// ??????
							char *splitPoint = magic[ i ].memo;
							sprintf( moji,"%2d/%2d", magic[ i ].mp, pc.mp );
							StockFontBuffer( pActMenuWnd2->x + 74 + 81, pActMenuWnd2->y + 183, FONT_PRIO_FRONT, 0, moji, 0 );
							while( 1 ){
								// ???????????????
								if( strlen( splitPoint ) > 22 ){
									strncpy( moji, splitPoint, 22 );
									moji[ 22 ] = NULL;	// ???Z??????
									// ?u???S??????
									if( GetStrLastByte( moji ) == 3 ){
										moji[ 21 ] = NULL;
										splitPoint += 21;
									}else{
										moji[ 22 ] = NULL;
										splitPoint += 22;
									}
									StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
								}else{
									strcpy( moji, splitPoint );
									StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
									break;
								}
							}
#if 0
							// ????????
							if( magic[ i ].memo[ 0 ] != NULL ){
								strncpy( moji, magic[ i ].memo, 22 );
								moji[ 22 ] = NULL;
								StockFontBuffer( pActMenuWnd2->x + 74, pActMenuWnd2->y + 204, FONT_PRIO_FRONT, 0, moji, 0 );

								if( magic[ i ].memo[ 22 ] != NULL ){
									strncpy( moji, magic[ i ].memo + 22, 22 );
									moji[ 22 ] = NULL;
									StockFontBuffer( pActMenuWnd2->x + 74, pActMenuWnd2->y + 224, FONT_PRIO_FRONT, 0, moji, 0 );
								}
							}
#endif
							// ????????
							StockDispBuffer( pActMenuWnd2->x + 37, pActMenuWnd2->y + 220, DISP_PRIO_ITEM, pc.item[ i ].graNo, 0 );

							// ????????
							if( mouse.onceState & MOUSE_LEFT_CRICK ){
								// ????????????K??
								if( magic[ i ].field == MAGIC_FIELD_BATTLE || magic[ i ].mp > pc.mp ){
									// ???
									play_se( 220, 320, 240 );
								}else{ // ?????u??
									// ?????????
									switch( magic[ i ].target ){

									case MAGIC_TARGET_MYSELF:	// ??????

										// ????????????
										if( eventWarpSendFlag == false ){
											// ??????????
											if( bNewServer)
												lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
											else
												old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
											play_se( 100, 320, 240 );
										}
										break;

									case MAGIC_TARGET_OTHER:	// ?\??????????)
										jujutuNo = i;
										// ?????
										play_se( 217, 320, 240 );
										//itemNo = -1;
										break;
#if 0
									case MAGIC_TARGET_ALLMYSIDE:	// ?s???u?l

										// ??????????
										if( bNewServer)
											lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										else
											old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										// ?????
										play_se( 100, 320, 240 );
										break;

									case MAGIC_TARGET_ALLOTHERSIDE:	// ???D?u?l

										// ??????????
										if( bNewServer)
											lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										else
											old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										// ?????
										play_se( 100, 320, 240 );
										break;

									case MAGIC_TARGET_ALL:	// ?u?

										// ??????????
										if( bNewServer)
											lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										else
											old_lssproto_MU_send( sockfd, nowGx, nowGy, i, 0 );
										// ?????
										play_se( 100, 320, 240 );
										break;
#endif
									}
								}
							}
						}
					}
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ???
						if( HitDispNo == itemWndFontNo[ 12 ] ){
							// ????????
							DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							itemWndNo = 0;
							// ??????????
							play_se( 203, 320, 240 );
							// ?k?????????????
							jujutuNo = -1;

							// ????????????????????
							if( MenuToggleFlag & JOY_CTRL_S ){

								MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
								// ????????????
								DeathMenuAction();
							}
						}
						// ????k???????
						if( jujutuNo != -1 && MenuToggleFlag & JOY_CTRL_S && statusWndNo == 1 ){
							// ????????
							for( i = 0 ; i < 11 ; i++ ){
								// ??????????k??
								if( HitFontNo == statusWndFontNo[ i ] && eventWarpSendFlag == false ){
									// ??????????
									if( bNewServer)
										lssproto_MU_send( sockfd, nowGx, nowGy, jujutuNo, i );
									else
										old_lssproto_MU_send( sockfd, nowGx, nowGy, jujutuNo, i );
									// ?????
									play_se( 100, 320, 240 );
									// ????????
									if( magic[ jujutuNo ].mp > pc.mp - magic[ jujutuNo ].mp ) jujutuNo = -1;;	// ????K????

								}
							}
						}

					}
					// ????????
					if( pActMenuWnd2 != NULL ){
						int flag = false;
						int col;
						// ????@?e
						x = pActMenuWnd2->x + 30;
						y = pActMenuWnd2->y + 35;

						// ???????
						for( i = 0 ; i < 5 ; i++ ){
							if( magic[ i ].useFlag == true ){
								col = FONT_PAL_WHITE;	// ?????u??????
								if( jujutuNo == i ) col = FONT_PAL_AQUA;	// ?k????????
								//cary 2001.12.3
								if( pc.familySprite == 0){
									if( (magic[ i ].mp*80/100) > pc.mp ) col = FONT_PAL_RED;	// ????K????
								}else{
									if( magic[ i ].mp > pc.mp ) col = FONT_PAL_RED;	// ????K????
								}
								if( magic[ i ].field == MAGIC_FIELD_BATTLE ) col = FONT_PAL_GRAY;	// ?????????????
								CenteringStr( magic[ i ].name, moji, MAGIC_NAME_LEN );
								itemWndFontNo[ i ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, col, moji, 2 ); y += 28;
								flag = true;
							}
						}
						if( flag == false ) StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, "???????", 0 );
						// ???????????????
						itemWndFontNo[ 12 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, pActMenuWnd2->y + 262 + 10, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;

		case 2:	// ??????k???????

			// ?????????
			if( pActMenuWnd2 == NULL ){
				pActMenuWnd2 = MakeWindowDisp( 368, 4, 192, 304, 0, -1 );
				// ????
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndFontNo[ i ] = -2;
				for( i = 0 ; i < MENU_ITEM_0 ; i++ ) itemWndBtnFlag[ i ] = 0;

			}else{
				// ????????????????
				if( pActMenuWnd2->hp > 0 ){
					// ??????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_MENU, CG_ITEM_WND_SELECT_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????k?????
						for( i = 0 ; i < 11 ; i++ ){
							if( HitFontNo == itemWndFontNo[ i ] && eventWarpSendFlag == false ){
								// ????k????
								if( jujutuNo != -1 ){
									// ??????????
									if( bNewServer)
										lssproto_MU_send( sockfd, nowGx, nowGy, jujutuNo, i );
									else
										old_lssproto_MU_send( sockfd, nowGx, nowGy, jujutuNo, i );
									// ?????
									play_se( 100, 320, 240 );
									// ???????????
									if( magic[ jujutuNo ].mp > pc.mp - magic[ jujutuNo ].mp ){
										// ????????
										DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
										GetKeyInputFocus( &MyChatBuffer );
										Moneyflag = false;
#endif
										pActMenuWnd2 = NULL;
										itemWndNo = 1;
										// ??????????
										play_se( 203, 320, 240 );
									}
								}else{	// ??????
									//??????????
									if( bNewServer)
										lssproto_ID_send( sockfd, nowGx, nowGy, itemNo, i );
									else
										old_lssproto_ID_send( sockfd, nowGx, nowGy, itemNo, i );
									// ????????
									play_se( 212, 320, 240 );
									// ????????
									DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
									GetKeyInputFocus( &MyChatBuffer );
									Moneyflag = false;
#endif
									pActMenuWnd2 = NULL;
									itemWndNo = 0;
									// ????????????????????
									if( MenuToggleFlag & JOY_CTRL_S ){

										MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
										// ????????????
										DeathMenuAction();
									}
									// ??????????
									play_se( 203, 320, 240 );
								}
							}
						}
						// ???
						if( HitDispNo == itemWndFontNo[ 11 ] ){
							// ????????
							DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							// ????k????
							if( jujutuNo != -1 ) itemWndNo = 1;
							else{
								// ????????????
								itemWndNo = 0;
								// ????????????????????
								if( MenuToggleFlag & JOY_CTRL_S ){

									MenuToggleFlag ^= JOY_CTRL_S;	// CTRL + S ??
									// ????????????
									DeathMenuAction();
								}
							}
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ????????
					if( pActMenuWnd2 != NULL ){
						// ????@?e
						x = pActMenuWnd2->x + 25;
						y = pActMenuWnd2->y + 30;

						// ???????????????
						// ????????q
						sprintf( moji,"%-16s", pc.name );
						itemWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 2 ); y += 23;
						// ???
						for( i = 0 ; i < 5 ; i++ ){
							// ???????????
							if( pet[ i ].useFlag == true ){
								// ????????
								if( pet[ i ].freeName[ 0 ] != NULL )
									sprintf( moji,"%-16s", pet[ i ].freeName );
								else
									sprintf( moji,"%-16s", pet[ i ].name );
								itemWndFontNo[ i + 1 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 2 ); y += 23;
							}
						}
						y = 183;
						// ??
						for( i = 0 ; i < 5 ; i++ ){
							// ????????????????????
							if( pc.id != party[ i ].id && party[ i ].useFlag == true ){
								sprintf( moji,"%-16s", party[ i ].name );
								itemWndFontNo[ i + 6 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 2 ); y += 23;
							}
						}
						// ???????????????
						itemWndFontNo[ 11 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, pActMenuWnd2->y + 287, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;
		}
	}

//? ?????????? *******************************************************/

	if( MenuToggleFlag & JOY_CTRL_M ){
#ifdef _NEWPANEL //Syu ADD 7.0 ????????????
		SkillWndflag = false ;
#endif
		// ??????k?????
		switch( mapWndNo ){

		case 0:	// ????????

			// ?????????
			if( pActMenuWnd2 == NULL ){
				pActMenuWnd2 = MakeWindowDisp( 388 + DISPLACEMENT_X , 4, 248, 240, 0, -1 );
				for( i = 0 ; i < MENU_MAP_0 ; i++ ) mapWndFontNo[ i ] = -2;

			}else{
				// ????????????????
				if( pActMenuWnd2->hp > 0 ){
					// ????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->my, DISP_PRIO_MENU, CG_MAP_WND, 0 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?????
						if( HitDispNo == mapWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd2 );
#ifdef _MONEYINPUT //Syu ADD ???????????
							GetKeyInputFocus( &MyChatBuffer );
							Moneyflag = false;
#endif
							pActMenuWnd2 = NULL;
							// ?????????
							MenuToggleFlag ^= JOY_CTRL_M;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}

					// ????????
					if( pActMenuWnd2 != NULL ){

						// ????@?e
						x = pActMenuWnd2->x + 61;
						y = pActMenuWnd2->y + 186;

						// ???????q????
						StockFontBuffer( pActMenuWnd2->x + 22, pActMenuWnd2->y + 31, FONT_PRIO_FRONT, 0, nowFloorName, 0 );

						// ???????????????
						sprintf( moji, "E  %3d", nowGx );
						StockFontBuffer( x, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 );
						sprintf( moji, "S  %3d", nowGy );
						StockFontBuffer( x + 73, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, moji, 0 ); y += 36;


						// ???????
						mapWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd2->pYobi )->mx, y, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}
			break;
		}
	}

//? ?????????? *******************************************************/

	if( MenuToggleFlag & JOY_CTRL_E ){
		// ??????k?????
		switch( mailWndNo ){

		case MAIL_WND_VIEW:	// ???????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 440, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
				// ??????????????k???S?????????????
				for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
					int j;
					// ??????????
					for( j = 0 ; j < MAIL_MAX_HISTORY ; j++ ){
						// ?t??????
						if( MailHistory[ i ].noReadFlag[ j ] >= true ){
							// ????k?????
							mailViewWndPageNo = i / MAX_ADR_BOOK_COUNT;
							i =  MAX_ADR_BOOK;
							break;
						}
					}
				}
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					static int nowDelNo;
					// ???????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_MAIL_WND, 1 );

					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK && pActYesNoWnd == NULL ){

						int nowNo = mailViewWndPageNo * MAX_ADR_BOOK_COUNT;
						// ????????
						for( i = 0 ; i < 12 ; i += 3, nowNo++ ){
							// ???q??????????V???????
#ifdef _EXTEND_AB
							if( addressBook[ nowNo ].useFlag == true
								|| addressBook[ nowNo ].useFlag == 2
								|| addressBook[ nowNo ].useFlag == 3 ){
#else
							if( addressBook[ nowNo ].useFlag == true ){
#endif
								if( HitFontNo == mailWndFontNo[ i ] ){
									mailHistoryWndSelectNo = nowNo;
									// ????????
									DeathAction( pActMenuWnd );
									pActMenuWnd = NULL;
									// ?????????????
									DeathLetterAction();
									// ????????
									mailWndNo = MAIL_WND_HISTORY;
									// ????????
									play_se( 202, 320, 240 );
								}

								// ?????
								if( HitDispNo == mailWndFontNo[ i + 1 ] ){
									// ??_????????
									if( pActYesNoWnd == NULL ){
										// ????????
										play_se( 202, 320, 240 );
										// ??_???????????????
										pActYesNoWnd = MakeWindowDisp( pActMenuWnd->x + 42, i * 32 + 54, 176, 56, 0, 4 );

										// ?????k????
										nowDelNo = nowNo;
									}
								}
								// ??????
								if( HitDispNo == mailWndFontNo[ i + 2 ] ){
									// ????????
									DeathAction( pActMenuWnd );
									pActMenuWnd = NULL;
									// ?????????????
									DeathLetterAction();
									// ????????????
									InitMailSendFlag();
									// ??????????????
									mailWndSendFlag[ nowNo ] = true;
									// ??????k???????
									mailWndNoBak = mailWndNo;
									// ?P?f???
									if( ProcNo == PROC_BATTLE ){
										// ??G???????????
										mailWndNo = MAIL_WND_SEND;
									}else{
										// ????????k????????
										if( pc.mailPetNo != -1 ){
											// ?k????????
											mailWndNo = MAIL_WND_SELECT;
										}else{
											// ??G???????????
											mailWndNo = MAIL_WND_SEND;
										}
									}
									// ????????
									play_se( 202, 320, 240 );
								}
							}
						}
						// ?????
						if( HitDispNo == mailWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							DeathLetterAction();
							MenuToggleFlag ^= JOY_CTRL_E;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}

					// ?q???
					if( ( HitDispNo == mailWndFontNo[ 26 ] || joy_con[ 0 ] & JOY_A ) && pActYesNoWnd == NULL ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							mailViewWndPageNo--; // ?????
							if( mailViewWndPageNo < 0 ) mailViewWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 26 ] = true;
							// ?????????????
							DeathLetterAction();
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && mailWndBtnFlag[ 26 ] == true ){
							// ????
							mailWndBtnFlag[ 26 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 26 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 26 ] = false;
					}
					// ?????
					if( ( HitDispNo == mailWndFontNo[ 27 ] || joy_con[ 0 ] & JOY_B ) && pActYesNoWnd == NULL ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							mailViewWndPageNo++; // ????????
							if( mailViewWndPageNo >= MAX_ADR_BOOK_PAGE ) mailViewWndPageNo = 0;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 27 ] = true;
							// ?????????????
							DeathLetterAction();
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && mailWndBtnFlag[ 27 ] == true ){
							// ????
							mailWndBtnFlag[ 27 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 27 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 27 ] = false;
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int nowNo = mailViewWndPageNo * MAX_ADR_BOOK_COUNT;
						// ????@?e
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;

						// ?????????????????????k?
						int graNo[7] = { 	CG_MAIL_WND_OFF_LINE_BTN,
											CG_MAIL_WND_ON_LINE_SUN_BTN,
											CG_MAIL_WND_ON_LINE_MOON_BTN,
											CG_MAIL_WND_ON_LINE_MERCURY_BTN,
											CG_MAIL_WND_ON_LINE_VINUS_BTN,
											CG_MAIL_WND_ON_LINE_EARTH_BTN,
											CG_MAIL_WND_ON_LINE_MARS_BTN
										};

						for( i = 0 ; i < 12 ; i += 3, nowNo++ ){
							// ???q????
#ifdef _EXTEND_AB
							if( addressBook[ nowNo ].useFlag == true
								|| addressBook[ nowNo ].useFlag == 2
								|| addressBook[ nowNo ].useFlag == 3 ){
#else
							if( addressBook[ nowNo ].useFlag == true ){
#endif
								int j = 0;
								// ??????????????
								for( j = 0 ; j < MAIL_MAX_HISTORY ; j++ ){
									// ????????????
									if( pActLetter[ i / 3 ] == NULL ){
										if( MailHistory[ nowNo ].noReadFlag[ j ] >= true ){
											// ?????????????
											pActLetter[ i / 3 ] = MakeAnimDisp( x - 20, y + 38, SPR_mail, ANIM_DISP_LETTER_MOVE );
										}
									}
								}
								// ?
#ifdef _EXTEND_AB
								if( addressBook[ nowNo ].useFlag == 2)
									StockFontBuffer( x + 15, y + 85, FONT_PRIO_FRONT, 5, "???????", 0 );
								if( addressBook[ nowNo ].useFlag == 3)
									StockFontBuffer( x + 15, y + 85, FONT_PRIO_FRONT, 5, "????????", 0 );
#endif
								StockDispBuffer( x + 44, y + 68, DISP_PRIO_IME3, addressBook[ nowNo ].graNo, 0 );
								CenteringStr( addressBook[ nowNo ].name, moji, CHAR_NAME_LEN );
								mailWndFontNo[ i ] = StockFontBuffer( x + 80, y + 30, FONT_PRIO_FRONT, 0, moji, 2 );
								sprintf( moji,"%3d", addressBook[ nowNo ].level );
								StockFontBuffer( x + 104, y + 60, FONT_PRIO_FRONT, 0, moji, 0 );
#ifdef _MAILSHOWPLANET				// (?????) Syu ADD ??????????
								sprintf( moji,"%8s", addressBook[ nowNo ].planetname );
								StockFontBuffer( x + 198, y + 30, FONT_PRIO_FRONT, 5, moji, 0 );
#endif
								//
#ifdef _TRANS_6
								if( addressBook[ nowNo ].transmigration < 0 || addressBook[ nowNo ].transmigration >= 7 ) addressBook[ nowNo ].transmigration = 0;
#else
								if( addressBook[ nowNo ].transmigration < 0 || addressBook[ nowNo ].transmigration >= 6 ) addressBook[ nowNo ].transmigration = 0;
#endif
								StockFontBuffer( x + 152, y + 59, FONT_PRIO_FRONT, FONT_PAL_AQUA, TransmigrationStr[ addressBook[ nowNo ].transmigration ], 0 );
								sprintf( moji,"%8d", addressBook[ nowNo ].dp );
								StockFontBuffer( x + 103, y + 88, FONT_PRIO_FRONT, 0, moji, 0 );
								StockDispBuffer( x + 225, y + 60, DISP_PRIO_IME3, graNo[ (addressBook[ nowNo ].onlineFlag==0)?0:1 ], 0 );
								mailWndFontNo[ i + 2 ] = StockDispBuffer( x + 225, y + 80, DISP_PRIO_IME3, CG_MAIL_WND_MAIL_BTN, 2 );
								mailWndFontNo[ i + 1 ] = StockDispBuffer( x + 225, y + 100, DISP_PRIO_IME3, CG_MAIL_WND_DELETE_BTN, 2 );

							}
#ifdef _EXTEND_AB
							else{
								if(  nowNo == MAX_ADR_BOOK-1 )
								{
									StockFontBuffer( x + 100, y + 30, FONT_PRIO_FRONT, FONT_PAL_RED , "?????????", 0 );
								}
							}
#endif
							y += 96;
						}
						// ?q???
						mailWndFontNo[ 26 ] = StockDispBuffer( pActMenuWnd->x + 25 + 28 - 8, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[ 26 ], 2 );
						// ????k?????
						sprintf( moji,"%2d", mailViewWndPageNo + 1 );
						StockFontBuffer( pActMenuWnd->x + 46 + 28 - 8, pActMenuWnd->y + 413, FONT_PRIO_FRONT, 0, moji, 0 );
						// ????
						mailWndFontNo[ 27 ] = StockDispBuffer( pActMenuWnd->x + 75 + 28, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[ 27 ], 2 );
						mailWndFontNo[ 19 ] = StockDispBuffer( pActMenuWnd->x + 220 - 28, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}

					// ??_????????
					if( pActYesNoWnd != NULL ){
						// ????
						if( ( ( WINDOW_DISP *)pActYesNoWnd->pYobi )->yesNoResult == true ){
							// ????????
							if( bNewServer)
								lssproto_DAB_send( sockfd, nowDelNo );
							else
								old_lssproto_DAB_send( sockfd, nowDelNo );
							play_se( 217, 320, 240 );
							// ??_????????
							DeathAction( pActYesNoWnd );
							pActYesNoWnd = NULL;
							// ??????????
							play_se( 203, 320, 240 );
						}else
						// ?????
						if( ( ( WINDOW_DISP *)pActYesNoWnd->pYobi )->yesNoResult == false ){
							// ??_????????
							DeathAction( pActYesNoWnd );
							pActYesNoWnd = NULL;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
				}

			}
			break;

		case MAIL_WND_SELECT:	// ?k???????
			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 40, 196, 3, 3, NULL, 0 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
				// ??????????
				for( i = 0 ; i < 5 ; i++ ){
					// ????????????
					if( pet[ i ].useFlag == false ){
						// ?????
						if( i == pc.mailPetNo ){
							pc.mailPetNo = -1;
							// ????????
							saveUserSetting();
						}
					}
				}
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??G?????
						if( HitFontNo == mailWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ??G????????????
							mailWndNo = MAIL_WND_SEND;
							// ????????
							play_se( 202, 320, 240 );
						}
						// ????????
						if( HitFontNo == mailWndFontNo[ 1 ] ){

	#ifdef _STREET_VENDOR
							if(pc.iOnStreetVendor == 1){
								play_se( 220, 320, 240 );
								StockChatBufferLine("�ڵ�̯�в���ʹ�ó����ʼ�",FONT_PAL_RED);
							}
							else
	#endif

								if( pc.mailPetNo != -1 ){
								// ????????
								DeathAction( pActMenuWnd );
								pActMenuWnd = NULL;
								// ???????????????
								mailWndNo = MAIL_WND_PET_SEND;
								// ????????
								play_se( 202, 320, 240 );
							}else{
								// ???
								play_se( 220, 320, 240 );
							}

						}
						// ???
						if( HitDispNo == mailWndFontNo[ 2 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int color = FONT_PAL_WHITE;

						// ???????k?????????
						if( pc.mailPetNo == -1 )
						color = FONT_PAL_RED;

						// ????@?e
						x = pActMenuWnd->x + 28;
						y = pActMenuWnd->y + 32;

						// ???????????????
						mailWndFontNo[ 0 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, 	"  ��  ͨ  ��  ��   ", 2 );	y += 32;
						mailWndFontNo[ 1 ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, color, 	"  ��  ��  ��  ��   ", 2 );	y += 48;
						mailWndFontNo[ 2 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, y, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;

		case MAIL_WND_SEND:	// ??G???????????

			// ?????????
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 304, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
				mailPetSendWndPageNo = mailViewWndPageNo;		// ??G??????????k????

				MailStr.len = 140;
				// ??????
				MailStr.lineLen = 28;
				// ?????
				MailStr.lineDist = 20;
				// ???????
				MailStr.color = 0;
				// ????????
				MailStr.x = pActMenuWnd->x + 12;
				MailStr.y = pActMenuWnd->y + 280 - 136;
				// ???????????I??
				MailStr.fontPrio = FONT_PRIO_FRONT;

			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_MAIL_WND_SEND_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						// ????????
						for( i = 0 ; i < MAX_ADR_BOOK_COUNT ; i++ ){
							// ??????k?????
							if( HitFontNo == mailWndFontNo[ i ] ){
								// ???q??????
#ifdef _EXTEND_AB
								if( addressBook[ nowNo + i ].useFlag == true
									|| addressBook[ nowNo + i ].useFlag == 2
									|| addressBook[ nowNo + i ].useFlag == 3 ){
#else								????V???????
								if( addressBook[ nowNo + i ].useFlag == true ){
#endif
									// ?k???????????
									if( mailWndSendFlag[ nowNo + i ] == false ) mailWndSendFlag[ nowNo + i ] = true;
									else mailWndSendFlag[ nowNo + i ] = false;
									// ?????
									play_se( 217, 320, 240 );
								}
							}

						}
						// ???
						if( HitDispNo == mailWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?q?????????
							mailWndNo = mailWndNoBak;
							// ??????????
							play_se( 203, 320, 240 );
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
						}
					}
					// ??????
					if( HitDispNo == mailWndFontNo[ 18 ] ){
						// ????????????????????????0.5?????
						if( mouse.onceState & MOUSE_LEFT_CRICK && ItemUseTime < timeGetTime() - 5000 ){
							int flag = false;
							// ?????
							ItemUseTime = timeGetTime();
							// ???k??????????
							for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
								if( mailWndSendFlag[ i ] == true ) flag = true;
							}
							// ????????
							if( MailStr.buffer[ 0 ] != NULL && flag == true ){
								// ?????????
								for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
									// ????????
									if( mailWndSendFlag[ i ] == true ){
										char moji2[ 256 ];
										// ????????
										strcpy( moji2, MailStr.buffer );
										// ???????
										makeEscapeString( moji2, moji, sizeof( moji ) );
										// ??????
										if( bNewServer)
											lssproto_MSG_send( sockfd, i, moji, FONT_PAL_WHITE );
										else
											old_lssproto_MSG_send( sockfd, i, moji, FONT_PAL_WHITE );

									}
								}
								play_se( 101, 320, 240 );
								mailWndBtnFlag[ 18 ] = true;
							}else{
								// ???
								play_se( 220, 320, 240 );
							}
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[ 18 ] == true ){
							// ????
							mailWndBtnFlag[ 18 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 18 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 18 ] = false;
					}

					// ?q???
					if( HitDispNo == mailWndFontNo[ 16 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							mailPetSendWndPageNo--; // ?????
							if( mailPetSendWndPageNo < 0 ) mailPetSendWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 16 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK  || joy_con[ 0 ] & JOY_A ) && mailWndBtnFlag[ 16 ] == true ){
							// ????
							mailWndBtnFlag[ 16 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 16 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 16 ] = false;
					}
					// ?????
					if( HitDispNo == mailWndFontNo[ 17 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							mailPetSendWndPageNo++; // ????????
							if( mailPetSendWndPageNo >= MAX_ADR_BOOK_PAGE ) mailPetSendWndPageNo = 0;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 17 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && mailWndBtnFlag[ 17 ] == true ){
							// ????
							mailWndBtnFlag[ 17 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 17 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 17 ] = false;
					}

					// ??????????
					if( HitDispNo == mailWndFontNo[ 14 ] ){
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ??????? ???@?e
							MailStr.buffer[ 0 ] = NULL;
							MailStr.cnt = 0;
							MailStr.cursor=0;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 14 ] = true;
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[ 14 ] == true ){
							// ????
							mailWndBtnFlag[ 14 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 14 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 14 ] = false;
					}
					// ?k????????
					if( HitDispNo == mailWndFontNo[ 15 ] ){
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ????????????
							InitMailSendFlag();
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 15 ] = true;
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[ 15 ] == true ){
							// ????
							mailWndBtnFlag[ 15 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 15 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 15 ] = false;
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						int color;

						// ????@?e
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						// ?V????????x
						GetKeyInputFocus( &MailStr );
						// ?????????
						StockFontBuffer2( &MailStr );

						for( i = 0 ; i < MAX_ADR_BOOK_COUNT ; i++ ){
							// ???q??????????V???????
#ifdef _EXTEND_AB
							if( addressBook[ nowNo + i ].useFlag == true
								|| addressBook[ nowNo + i ].useFlag == 2
								|| addressBook[ nowNo + i ].useFlag == 3 ){
#else
							if( addressBook[ nowNo + i ].useFlag == true ){
#endif
								color = FONT_PAL_GRAY;	// ???????
								// ???????
								if( addressBook[ nowNo + i ].onlineFlag > 0 ){
									// ???
									color = FONT_PAL_WHITE;
								}
								// ?k????????
								if( mailWndSendFlag[ nowNo + i ] == true ){
									// ???
									color = FONT_PAL_AQUA;
								}
								// ???q????
								CenteringStr( addressBook[ nowNo + i ].name, moji, CHAR_NAME_LEN );
								mailWndFontNo[ i ] = StockFontBuffer( x + 111, y + 177 - 136, FONT_PRIO_FRONT, color, moji, 2 );

							}
							y += 23;
						}
						// ?k????????
						mailWndFontNo[ 15 ] = StockDispBuffer( pActMenuWnd->x + 51, pActMenuWnd->y + 211 - 136, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[ 15 ], 2 );
						// ?q???
						mailWndFontNo[ 16 ] = StockDispBuffer( pActMenuWnd->x + 26 - 8 + 4, pActMenuWnd->y + 241 - 132, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[ 16 ], 2 );
						// ????k?????
						sprintf( moji,"%2d", mailPetSendWndPageNo + 1 );
						StockFontBuffer( pActMenuWnd->x + 47 - 8 + 4, pActMenuWnd->y + 233 - 132, FONT_PRIO_FRONT, 0, moji, 0 );
						// ????
						mailWndFontNo[ 17 ] = StockDispBuffer( pActMenuWnd->x + 76 + 4, pActMenuWnd->y + 241 - 132, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[ 17 ], 2 );
						// ??????????
						mailWndFontNo[ 14 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 60, pActMenuWnd->y + 390 - 136, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[ 14 ], 2 );
						// ??????
						mailWndFontNo[ 18 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx - 60, pActMenuWnd->y + 421 - 136, DISP_PRIO_IME3, CG_SEND_BTN + mailWndBtnFlag[ 18 ] * 75, 2 );
						// ??
						mailWndFontNo[ 19 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 60, pActMenuWnd->y + 421 - 136, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;

		case MAIL_WND_PET_SEND:	// ??????????????

			// ?????????
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 440, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
				mailPetSendWndPageNo = mailViewWndPageNo;		// ??G??????????k????
				MailStr.len = MAIL_STR_LEN;
				// ??????
				MailStr.lineLen = 28;
				// ?????
				MailStr.lineDist = 20;
				// ???????
				MailStr.color = 0;
				// ????????
				MailStr.x = pActMenuWnd->x + 12;
				MailStr.y = pActMenuWnd->y + 280;
				// ???????????I??
				MailStr.fontPrio = FONT_PRIO_FRONT;
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????????????
					if( pActPet == NULL ){
						// ???????????
						pActPet = MakeAnimDisp( pActMenuWnd->x + 190, pActMenuWnd->y + 120, pet[ pc.mailPetNo ].graNo, ANIM_DISP_PET );
					}
					// ????????????????????k????????
					if( pActMailItem == NULL && mailItemNo != -1 ){
						// ????????????
						pActMailItem = MakeAnimDisp( pActMenuWnd->x + 56, pActMenuWnd->y + 96, pc.item[ mailItemNo ].graNo, ANIM_DISP_PET_ITEM );
						pActMailItem->atr |= ACT_ATR_INFO;
						pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
						pActMailItem->dispPrio = DISP_PRIO_DRAG;
						// ?????????
						strcpy( pActMailItem->name, pc.item[ mailItemNo ].name );
					}
					// ????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_MAIL_WND_PET_SEND_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						// ????????
						for( i = 0 ; i < MAX_ADR_BOOK_COUNT ; i++ ){
							// ??????k????????????????????????
							if( HitFontNo == mailWndFontNo[ i ] ){
								// ???q????????
#ifdef _EXTEND_AB
								if( addressBook[ nowNo + i ].useFlag == true
									|| addressBook[ nowNo + i ].useFlag == 2
									|| addressBook[ nowNo + i ].useFlag == 3 ){
#else								??V???????
								if( addressBook[ nowNo + i ].useFlag == true ){
#endif
									int j, flag = false;
									// ???k??????????
									for( j = 0 ; j < MAX_ADR_BOOK ; j++ ){
										if( mailWndSendFlag[ j ] == true ) flag = true;
									}

									// ?k?????????
									if( flag == false ){
										mailWndSendFlag[ nowNo + i ] = true;
										// ?????
										play_se( 217, 320, 240 );
									}else{ // ???k????????
										// ?k??????????????????
										if( mailWndSendFlag[ nowNo + i ] == true ){
											mailWndSendFlag[ nowNo + i ] = false;
											// ?????
											play_se( 217, 320, 240 );
										}else{ // ?k???????????????????
											// ???
											play_se( 220, 320, 240 );
										}
									}
								}
							}
						}
						// ???
						if( HitDispNo == mailWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ??????????
							play_se( 203, 320, 240 );
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
						}
						// ???????
						if( HitDispNo == mailWndFontNo[ 15 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ?q?????????
							mailWndNo = MAIL_WND_ITEM;
							// ????????
							play_se( 202, 320, 240 );
							// ?V????????
							GetKeyInputFocus( &MyChatBuffer );
						}
					}
					// ??????
					if( HitDispNo == mailWndFontNo[ 18 ] ){

						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							int flag = false;
							// ???k??????????
							for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
								if( mailWndSendFlag[ i ] == true ) flag = true;
							}
							// ????????
							if( MailStr.buffer[ 0 ] != NULL && flag == true
								&& pActPet != NULL
								&& pActPet->actNo == ANIM_DISP_PET
								&& pc.mailPetNo != ItemMixPetNo ){
								for( i = 0 ; i < MAX_ADR_BOOK ; i++ ){
									// ????????
									if( mailWndSendFlag[ i ] == true){
// Terry add fix can send mail to offline character 2004/2/5
										if(addressBook[i].onlineFlag == 0) StockChatBufferLine("����Ҳ������ϣ�??",FONT_PAL_RED);
										else
// end
										{
											char moji2[ 256 ];
											// ????????
											strcpy( moji2, MailStr.buffer );
											// ???????
											makeEscapeString( moji2, moji, sizeof( moji ) );
											// ?????????
											if( bNewServer)
												lssproto_PMSG_send( sockfd, i, pc.mailPetNo, mailItemNo, moji2, FONT_PAL_WHITE );
											else
												old_lssproto_PMSG_send( sockfd, i, pc.mailPetNo, mailItemNo, moji2, FONT_PAL_WHITE );
											pc.mailPetNo = -1;
											// ????????????
											ItemBuffer[ mailItemNo ].mixFlag = 0;
											// ????????
											saveUserSetting();
											// ????????????????
											if( pActPet != NULL ){
												// ????????
												pActPet->actNo = 1;
											}
										}
									}
								}
								play_se( 101, 320, 240 );
								mailWndBtnFlag[ 18 ] = true;
							}else{
								// ???
								play_se( 220, 320, 240 );
							}
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[ 18 ] == true ){
							// ????
							mailWndBtnFlag[ 18 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 18 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 18 ] = false;
					}

					// ?q???
					if( HitDispNo == mailWndFontNo[ 16 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							mailPetSendWndPageNo--; // ?????
							if( mailPetSendWndPageNo < 0 ) mailPetSendWndPageNo = MAX_ADR_BOOK_PAGE - 1;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 16 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && mailWndBtnFlag[ 16 ] == true ){
							// ????
							mailWndBtnFlag[ 16 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 16 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 16 ] = false;
					}
					// ?????
					if( HitDispNo == mailWndFontNo[ 17 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							mailPetSendWndPageNo++; // ????????
							if( mailPetSendWndPageNo >= MAX_ADR_BOOK_PAGE ) mailPetSendWndPageNo = 0;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 17 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && mailWndBtnFlag[ 17 ] == true ){
							// ????
							mailWndBtnFlag[ 17 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 17 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 17 ] = false;
					}

					// ??????????
					if( HitDispNo == mailWndFontNo[ 14 ] ){
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ??????? ???@?e
							MailStr.buffer[ 0 ] = NULL;
							MailStr.cnt = 0;
							MailStr.cursor=0;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 14 ] = true;
						}
						// ?????????
						if( mouse.state & MOUSE_LEFT_CRICK && mailWndBtnFlag[ 14 ] == true ){
							// ????
							mailWndBtnFlag[ 14 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 14 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 14 ] = false;
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int nowNo = mailPetSendWndPageNo * MAX_ADR_BOOK_COUNT;
						int color;

						// ????@?e
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						// ?V????????x
						GetKeyInputFocus( &MailStr );

						if( pet[ pc.mailPetNo ].freeName[ 0 ] == NULL ){
							CenteringStr( pet[ pc.mailPetNo ].name, moji, PET_NAME_LEN );
						}else{
							CenteringStr( pet[ pc.mailPetNo ].freeName, moji, PET_NAME_LEN );
						}
						StockFontBuffer( x + 120, y + 14, FONT_PRIO_FRONT, 0, moji, 0 );
						StockFontBuffer2( &MailStr );
						for( i = 0 ; i < MAX_ADR_BOOK_COUNT ; i++ ){
							// ???q??????????V???????
#ifdef _EXTEND_AB
							if( addressBook[ nowNo + i ].useFlag == true
								|| addressBook[ nowNo + i ].useFlag == 2
								|| addressBook[ nowNo + i ].useFlag == 3 ){
#else
							if( addressBook[ nowNo + i ].useFlag == true ){
#endif
								color = FONT_PAL_GRAY;	// ???????
								// ???????
								if( addressBook[ nowNo + i ].onlineFlag > 0 ){
									// ???
									color = FONT_PAL_WHITE;
								}
								// ?k????????
								if( mailWndSendFlag[ nowNo + i ] == true ){
									// ???
									color = FONT_PAL_AQUA;
								}
								// ???q????
								CenteringStr( addressBook[ nowNo + i ].name, moji, CHAR_NAME_LEN );
								mailWndFontNo[ i ] = StockFontBuffer( x + 111, y + 177, FONT_PRIO_FRONT, color, moji, 2 );

							}
							y += 23;
						}
						// ???????
						mailWndFontNo[ 15 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_IME3, CG_MAIL_WND_ITEM_BTN, 2 );
						// ?q???
						mailWndFontNo[ 16 ] = StockDispBuffer( pActMenuWnd->x + 25 - 8 + 4, pActMenuWnd->y + 241, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[ 16 ], 2 );
						// ????k?????
						sprintf( moji,"%2d", mailPetSendWndPageNo + 1 );
						StockFontBuffer( pActMenuWnd->x + 46 - 8 + 4, pActMenuWnd->y + 233, FONT_PRIO_FRONT, 0, moji, 0 );
						// ????
						mailWndFontNo[ 17 ] = StockDispBuffer( pActMenuWnd->x + 75 + 4, pActMenuWnd->y + 241, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[ 17 ], 2 );
						// ??????????
						mailWndFontNo[ 14 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 60, pActMenuWnd->y + 390, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + mailWndBtnFlag[ 14 ], 2 );
						// ??????
						mailWndFontNo[ 18 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx - 60, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_SEND_BTN + mailWndBtnFlag[ 18 ] * 75, 2 );
						// ??
						mailWndFontNo[ 19 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx + 60, pActMenuWnd->y + 421, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;

		case MAIL_WND_ITEM:	// ?????k???????
			// ?????????
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 24, 271, 281, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;
				mailSendWndPageNo = 0;		// ??????????k????
				// ??????????
				InitItem( pActMenuWnd->x, pActMenuWnd->y - 1, false);
				// ???????????
				if( MenuToggleFlag & JOY_CTRL_I && itemWndNo == 0 ){
					// ?????????????
					MenuToggleFlag &= ~JOY_CTRL_I;
					DeathMenuAction2();
				}
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ??????????????
#ifdef _READ16BITBMP
					if(g_bUseAlpha) StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_ITEM_WND_1, 1 );
#endif
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my - 80, DISP_PRIO_MENU, CG_ITEM_WND_1, 1 );
					// ?????????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my - 80, DISP_PRIO_MENU, CG_BTL_ITEM_WND_TITLE, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ???
						if( HitDispNo == mailWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ????????
							mailWndNo = MAIL_WND_PET_SEND;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ????????
					if( pActMenuWnd != NULL ){
						// ????@?e
						x = pActMenuWnd->x + 16;
						y = pActMenuWnd->y + 191 + 6;

						// ???????
						for(  i = MAX_ITEM -1 ; i >= MAX_ITEMSTART ; i-- ){
							// ?????V?????
							if( MakeHitBox( ItemBuffer[ i ].defX - 24, ItemBuffer[ i ].defY - 24 - 160,
											ItemBuffer[ i ].defX + 25, ItemBuffer[ i ].defY + 23 - 160, DISP_PRIO_IME3 ) == true ){
								// ?????????????
								if( pc.item[ i ].useFlag == true ){
									// ??????
									char *splitPoint = pc.item[ i ].memo;
									int color = pc.item[ i ].color;

									// ???????????
									if( pc.level < pc.item[ i ].level ) color = FONT_PAL_RED;
									// ????????q????
									StockFontBuffer( pActMenuWnd->x + 16, pActMenuWnd->y + 332 - 160 - 1, FONT_PRIO_FRONT, color, pc.item[ i ].name, 0 );
									// ??????C??????
									// ???????
									while( 1 ){
										// ???????????????
										if( strlen( splitPoint ) > 28 ){
											strncpy( moji, splitPoint, 28 );
											moji[ 28 ] = NULL;	// ???Z??????
											// ?u???S??????
											if( GetStrLastByte( moji ) == 3 ){
												moji[ 27 ] = NULL;
												splitPoint += 27;
											}else{
												moji[ 28 ] = NULL;
												splitPoint += 28;
											}
											StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;
										}else{
											strcpy( moji, splitPoint );
											StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
											break;
										}
									}
								}

								// ????????????????
								if( mouse.onceState & MOUSE_LEFT_DBL_CRICK ){
									if( pc.item[ i ].useFlag == true && pc.item[ i ].sendFlag & ITEM_FLAG_PET_MAIL ){
										mailItemNo = i;	// ?????k???
										// ????????
										DeathAction( pActMenuWnd );
										pActMenuWnd = NULL;
										// ????????
										mailWndNo = MAIL_WND_PET_SEND;
										// ??????????
										play_se( 203, 320, 240 );
										// ?V????????
										GetKeyInputFocus( &MyChatBuffer );
									}else{
										// ???
										play_se( 220, 320, 240 );
									}
								}
							}
							if( pc.item[ i ].useFlag == true ){
								// ????????
								StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY - 160, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
							}
						}
						// ?????????
						if( pActMenuWnd != NULL ){
							// ???????????????
							mailWndFontNo[ 19 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 262, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
						}
					}
				}
			}
			break;

		case MAIL_WND_HISTORY:	// ???????
			// ?????????
			if( pActMenuWnd == NULL ){
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 424, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_MAIL_0 ; i++ ) mailWndFontNo[ i ] = -2;

				// ???????????@?e???k???t???????k?????
				// ???????
				for( mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1 ; mailHistoryWndPageNo > 0 ; mailHistoryWndPageNo-- ){
					// ????????
					if( mailHistoryWndPageNo < 0 ) mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1;
					// ?????????????k?
					mailHistoryWndNowPageNo = ( MailHistory[ mailHistoryWndSelectNo ].newHistoryNo + mailHistoryWndPageNo ) % MAIL_MAX_HISTORY;
					// ?t?????????????I??
					if( MailHistory[ mailHistoryWndSelectNo ].noReadFlag[ mailHistoryWndNowPageNo ] >= true ) break;
				}
#if 0
				// ????????????????
				if( pActPet == NULL ){
					// ???????????
					pActPet = MakeAnimDisp( pActMenuWnd->x - 204, pActMenuWnd->y + 160, pet[ pc.mailPetNo ].graNo, ANIM_DISP_PET_RECIEVE );
					pActPet->atr |= ACT_ATR_INFO;
					pActPet->atr |= ACT_ATR_TYPE_PET;
					pActPet->level = 32;
					strcpy( pActPet->name, "???????" );
				}
				// ???????????????????????????
				if( pActMailItem == NULL && MailHistory[ mailHistoryWndSelectNo ].itemGraNo[ mailHistoryWndNowPageNo ] != -1 ){
					// ????????????
					pActMailItem = MakeAnimDisp( pActMenuWnd->x - 204, pActMenuWnd->y + 128, pc.item[ 5 ].graNo, ANIM_DISP_PET_ITEM );
					pActMailItem->atr |= ACT_ATR_INFO;
					pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
					pActMailItem->dispPrio = DISP_PRIO_DRAG;
					// ?????????
					strcpy( pActMailItem->name, "???????" );
				}
#endif
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					static int nowDelNo;
					// ???????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_MAIL_WND_HISTORY_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ??????
						if( HitDispNo == mailWndFontNo[ 18 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							DeathLetterAction();
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ??????k???????
							mailWndNoBak = mailWndNo;
							// ????????????
							InitMailSendFlag();
							// ??????????????
							mailWndSendFlag[ mailHistoryWndSelectNo ] = true;
							// ?P?f???
							if( ProcNo == PROC_BATTLE ){
								// ??G???????????
								mailWndNo = MAIL_WND_SEND;
							}else{
								// ????????k????????
								if( pc.mailPetNo != -1 ){
									// ?k????????
									mailWndNo = MAIL_WND_SELECT;
								}else{
									// ??G???????????
									mailWndNo = MAIL_WND_SEND;
								}
							}
							// ????????
							play_se( 202, 320, 240 );
						}
						// ???
						if( HitDispNo == mailWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							DeathLetterAction();
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ????????
							mailWndNo = MAIL_WND_VIEW;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}
					// ?q???
					if( HitDispNo == mailWndFontNo[ 16 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							mailHistoryWndPageNo--; // ?????
							if( mailHistoryWndPageNo < 0 ) mailHistoryWndPageNo = MAIL_MAX_HISTORY - 1;
							// ?????????????
							DeathLetterAction();
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 16 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && mailWndBtnFlag[ 16 ] == true ){
							// ????
							mailWndBtnFlag[ 16 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 16 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 16 ] = false;
					}
					// ?????
					if( HitDispNo == mailWndFontNo[ 17 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							mailHistoryWndPageNo++; // ????????
							if( mailHistoryWndPageNo >= MAIL_MAX_HISTORY ) mailHistoryWndPageNo = 0;
							// ?????????????
							DeathLetterAction();
							// ??????
							DeathAction( pActPet );
							pActPet = NULL;
							// ???????
							DeathAction( pActMailItem );
							pActMailItem = NULL;
							// ?????
							play_se( 217, 320, 240 );
							mailWndBtnFlag[ 17 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && mailWndBtnFlag[ 17 ] == true ){
							// ????
							mailWndBtnFlag[ 17 ] = true;
						}else{
							// ?????
							mailWndBtnFlag[ 17 ] = false;
						}
					}else{
						// ?????
						mailWndBtnFlag[ 17 ] = false;
					}

					// ????????
					if( pActMenuWnd != NULL ){
						// ????@?e
						x = pActMenuWnd->x;
						y = pActMenuWnd->y;
						char *splitPoint;
						// ?????????????k?
						mailHistoryWndNowPageNo = ( MailHistory[ mailHistoryWndSelectNo ].newHistoryNo + mailHistoryWndPageNo ) % MAIL_MAX_HISTORY;
						// ?t???????????
						if( MailHistory[ mailHistoryWndSelectNo ].noReadFlag[ mailHistoryWndNowPageNo ] >= true ){
							// ????????
							if( MailHistory[ mailHistoryWndSelectNo ].noReadFlag[ mailHistoryWndNowPageNo ] > true ){
								// ????????????????
								if( pActPet == NULL ){
									// ???????????
									pActPet = MakeAnimDisp( pActMenuWnd->x - 204, pActMenuWnd->y + 160, MailHistory[ mailHistoryWndSelectNo ].noReadFlag[ mailHistoryWndNowPageNo ], ANIM_DISP_PET_RECIEVE );
									pActPet->atr |= ACT_ATR_INFO;
									pActPet->atr |= ACT_ATR_TYPE_PET;
									// ??????
									pActPet->level = MailHistory[ mailHistoryWndSelectNo ].petLevel[ mailHistoryWndNowPageNo ];
									// ???????q
									strcpy( pActPet->name, MailHistory[ mailHistoryWndSelectNo ].petName[ mailHistoryWndNowPageNo ] );
								}
								// ???????????????????????????
								if( pActMailItem == NULL && MailHistory[ mailHistoryWndSelectNo ].itemGraNo[ mailHistoryWndNowPageNo ] != -1 ){
									// ????????????
									pActMailItem = MakeAnimDisp( pActMenuWnd->x - 204, pActMenuWnd->y + 128, MailHistory[ mailHistoryWndSelectNo ].itemGraNo[ mailHistoryWndNowPageNo ], ANIM_DISP_PET_ITEM );
									pActMailItem->atr |= ACT_ATR_INFO;
									pActMailItem->atr |= ACT_ATR_TYPE_ITEM;
									pActMailItem->dispPrio = DISP_PRIO_DRAG;
									// ?????????
									strcpy( pActMailItem->name, "�������??" );
								}
							}
							// ?????????????
							if( pActLetter[ 0 ] == NULL ){
								// ?????????????
								pActLetter[ 0 ] = MakeAnimDisp( x + 28, y + 244, SPR_mail, ANIM_DISP_LETTER );
							}
							// ??????
							MailHistory[ mailHistoryWndSelectNo ].noReadFlag[ mailHistoryWndNowPageNo ] = false;
							// ?????????V
							SaveMailHistory( mailHistoryWndSelectNo );
							// ?t??????
							mailLamp = CheckMailNoReadFlag();

						}
						// ??????
						splitPoint = MailHistory[ mailHistoryWndSelectNo ].str[ mailHistoryWndNowPageNo ];
						// ???q????
						CenteringStr( addressBook[ mailHistoryWndSelectNo ].name, moji, CHAR_NAME_LEN );
						StockFontBuffer( x + 64, y + 34, FONT_PRIO_FRONT, 0, moji, 0 );
						// ?T?????
						StockFontBuffer( x + 148, y + 236, FONT_PRIO_FRONT, 0, MailHistory[ mailHistoryWndSelectNo ].dateStr[ mailHistoryWndNowPageNo ], 0 );
						// ????????
						while( 1 ){
							// ???????????????
							if( strlen( splitPoint ) > 28 ){
								strncpy( moji, splitPoint, 28 );
								moji[ 28 ] = NULL;	// ???Z??????
								// ?u???S??????
								if( GetStrLastByte( moji ) == 3 ){
									moji[ 27 ] = NULL;
									splitPoint += 27;
								}else{
									moji[ 28 ] = NULL;
									splitPoint += 28;
								}
								StockFontBuffer( x + 12, y + 260, FONT_PRIO_FRONT, 0, moji, 0 ); y += 20;
							}else{
								strcpy( moji, splitPoint );
								StockFontBuffer( x + 12, y + 260, FONT_PRIO_FRONT, 0, moji, 0 );
								break;
							}
						}
						// ?q???
						mailWndFontNo[ 16 ] = StockDispBuffer( pActMenuWnd->x + 25 + 16 + 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_PREV_BTN + mailWndBtnFlag[ 16 ], 2 );
						// ????k?????
						sprintf( moji,"%2d", mailHistoryWndPageNo + 1 );
						StockFontBuffer( pActMenuWnd->x + 46 + 16 + 9, pActMenuWnd->y + 397, FONT_PRIO_FRONT, 0, moji, 0 );
						// ????
						mailWndFontNo[ 17 ] = StockDispBuffer( pActMenuWnd->x + 75 + 16 + 9 + 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_NEXT_BTN + mailWndBtnFlag[ 17 ], 2 );
						// ??????
						mailWndFontNo[ 18 ] = StockDispBuffer( pActMenuWnd->x + 196, pActMenuWnd->y + 371, DISP_PRIO_IME3, CG_MAIL_WND_MAIL_BTN, 2 );
						// ???????
						mailWndFontNo[ 19 ] = StockDispBuffer( pActMenuWnd->x + 220 -16 - 8, pActMenuWnd->y + 405, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
					}
				}
			}
			break;


		}
	}

//? ??????????? *******************************************************/

	if( MenuToggleFlag & JOY_CTRL_A ){

		// ??????k?????
		switch( albumWndNo ){

		case 0:	// ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 4, 4, 272, 448, 0, -1 );
				// ????R?e???
				for( i = 0 ; i < MENU_ALBUM_0 ; i++ ) albumWndFontNo[ i ] = -2;
				// ????k????
				albumWndPageNo = 0;
				// ?????k????
				albumNo = -1;
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ?????????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->my, DISP_PRIO_MENU, CG_ALBUM_WND, 1 );
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?????I?d?????k?
						int nowTopNo = albumWndPageNo * 8;
						// ?????????
						for( i = 0 ; i < 8 ; i++ ){
							// ????k?????????????????
							if( HitFontNo == albumWndFontNo[ i ] && PetAlbum[ nowTopNo + i ].flag >= 2 ){
								// ?????k???
								albumNo = nowTopNo + i;
								// ????????
								PetAlbum[ albumNo ].flag = 3;
								// ???????V
								SaveAlbum( albumNo );
								// ?????
								play_se( 217, 320, 240 );
							}
						}
						// ?????
						if( HitDispNo == albumWndFontNo[ 19 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????
							MenuToggleFlag ^= JOY_CTRL_A;
							// ??????????
							play_se( 203, 320, 240 );
						}
					}

					// ?q???
					if( HitDispNo == albumWndFontNo[ 16 ] || joy_con[ 0 ] & JOY_A ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_A ){
							int limitPage = ((MAX_PET_KIND+MAX_PET_PAGE_SIZE-1) / MAX_PET_PAGE_SIZE)-1;
							albumWndPageNo--; // ?????
							// ????????
							if( albumWndPageNo < 0 ) albumWndPageNo = limitPage;
							// ?????
							play_se( 217, 320, 240 );
							albumWndBtnFlag[ 16 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_A ) && albumWndBtnFlag[ 16 ] == true ){
							// ????
							albumWndBtnFlag[ 16 ] = true;
						}else{
							// ?????
							albumWndBtnFlag[ 16 ] = false;
						}
					}else{
						// ?????
						albumWndBtnFlag[ 16 ] = false;
					}
					// ?????
					if( HitDispNo == albumWndFontNo[ 17 ] || joy_con[ 0 ] & JOY_B ){
						// ????????
						if( mouse.autoState & MOUSE_LEFT_CRICK || joy_auto[ 0 ] & JOY_B ){
							int limitPage = ((MAX_PET_KIND+MAX_PET_PAGE_SIZE-1) / MAX_PET_PAGE_SIZE)-1;
							albumWndPageNo++; // ????????
							// ????????
							if( albumWndPageNo > limitPage ) albumWndPageNo = 0;
							// ?????
							play_se( 217, 320, 240 );
							albumWndBtnFlag[ 17 ] = true;
						}
						// ?????????
						if( ( mouse.state & MOUSE_LEFT_CRICK || joy_con[ 0 ] & JOY_B ) && albumWndBtnFlag[ 17 ] == true ){
							// ????
							albumWndBtnFlag[ 17 ] = true;
						}else{
							// ?????
							albumWndBtnFlag[ 17 ] = false;
						}
					}else{
						// ?????
						albumWndBtnFlag[ 17 ] = false;
					}


					// ????????
					if( pActMenuWnd != NULL ){
						#define ALBUM_WND_ATTR_X 34
						#define ALBUM_WND_ATTR_Y 0
						int x2;
						int nowTopNo;	// ?????I?d?????k?
						// ????@?e
						x = pActMenuWnd->x;
						y = pActMenuWnd->y + 28;

						// ?????k????
						if( albumNo != -1 ){
							// ???q????????????
							CenteringStr( PetAlbum[ albumNo ].freeName, moji, PET_NAME_LEN );
							StockFontBuffer( x + 13, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 22;

							sprintf( moji,"%3d",PetAlbum[ albumNo ].level );
							StockFontBuffer( x + 36, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

							sprintf( moji, "%4d", PetAlbum[ albumNo ].maxHp );
							StockFontBuffer( x + 36, y, FONT_PRIO_FRONT, 0, moji, 0 );

							sprintf( moji, "%3d", PetAlbum[ albumNo ].str );
							StockFontBuffer( x + 36 + 72, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 24;

							sprintf( moji, "%4d", PetAlbum[ albumNo ].quick );
							StockFontBuffer( x + 36, y, FONT_PRIO_FRONT, 0, moji, 0 );

							sprintf( moji, "%3d", PetAlbum[ albumNo ].def );
							StockFontBuffer( x + 36 + 72, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 29;

							// ??N?????
							if( PetAlbum[ albumNo ].earth > 0 ){
								x2 = ( int )( PetAlbum[ albumNo ].earth * 0.8 );

								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_GREEN, 1 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_GREEN2, 0 );
							}
							y += 20;

							// ??N?????
							if( PetAlbum[ albumNo ].water > 0 ){
								x2 = ( int )( PetAlbum[ albumNo ].water * 0.8 );

								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_AQUA, 1 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_AQUA2, 0 );
							}
							y += 20;

							// ??N?????
							if( PetAlbum[ albumNo ].fire > 0 ){
								x2 = ( int )( PetAlbum[ albumNo ].fire * 0.8 );

								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_RED, 1 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_RED2, 0 );
							}
							y += 20;

							// ??N?????
							if( PetAlbum[ albumNo ].wind > 0 ){
								x2 = ( int )( PetAlbum[ albumNo ].wind * 0.8 );

								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 0, y + ALBUM_WND_ATTR_Y + 0, x + ALBUM_WND_ATTR_X + 0 + x2, y + ALBUM_WND_ATTR_Y + 8, DISP_PRIO_IME4, SYSTEM_PAL_YELLOW, 1 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 1, y + ALBUM_WND_ATTR_Y + 1, x + ALBUM_WND_ATTR_X + 1 + x2, y + ALBUM_WND_ATTR_Y + 9, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
								StockBoxDispBuffer( x + ALBUM_WND_ATTR_X + 2, y + ALBUM_WND_ATTR_Y + 2, x + ALBUM_WND_ATTR_X + 2 + x2, y + ALBUM_WND_ATTR_Y + 10, DISP_PRIO_IME3, SYSTEM_PAL_YELLOW2, 0 );
							}

							StockDispBuffer( pActMenuWnd->x + 151 + 48, pActMenuWnd->y + 65 + 52, DISP_PRIO_IME3, PetAlbum[ albumNo ].faceGraNo, 0 );
						}

						y = pActMenuWnd->y + 215;

						// ?????I?d?????k?
						nowTopNo = albumWndPageNo * 8;
						// ?????????
						for( i = 0 ; i < 8 ; i++ ){
							// ????????
							if( nowTopNo + i >= MAX_PET_KIND ) break;

							// ????????????
							if( PetAlbum[ nowTopNo + i ].flag == 2 ){
								// ???????
								StockDispBuffer( x + 24, y + 9, DISP_PRIO_IME3, CG_ALBUM_WND_NEW_ICON, 0 );
							}
							// ??????????
							sprintf( moji,"No.%3d",nowTopNo + i + 1 );
							StockFontBuffer( x + 46, y + 1, FONT_PRIO_FRONT, 0, moji, 0 );

							if( PetAlbum[ nowTopNo + i ].flag != 0 ){
								// ????
								if( PetAlbum[ nowTopNo + i ].flag == 1 ){
									CenteringStr( PetAlbum[ nowTopNo + i ].name, moji, PET_NAME_LEN );
									StockFontBuffer( x + 112, y, FONT_PRIO_FRONT, FONT_PAL_GRAY, moji, 0 );
									// ????R?e?k????
									albumWndFontNo[ i ] = -2;
								}else
								// ?????????
								if( PetAlbum[ nowTopNo + i ].flag >= 2 ){
									CenteringStr( PetAlbum[ nowTopNo + i ].name, moji, PET_NAME_LEN );
									albumWndFontNo[ i ] = StockFontBuffer( x + 112, y, FONT_PRIO_FRONT, 0, moji, 2 );
								}
							}else{
								// ????R?e?k????
								albumWndFontNo[ i ] = -2;
							}
							y += 22;
						}

						// ?q???
						albumWndFontNo[ 16 ] = StockDispBuffer( pActMenuWnd->x + 25 + 32 + 10 + 40, pActMenuWnd->y + 400, DISP_PRIO_IME3, CG_PREV_BTN + albumWndBtnFlag[ 16 ], 2 );
						albumWndFontNo[ 17 ] = StockDispBuffer( pActMenuWnd->x + 75 + 32 + 9 + 10 + 40, pActMenuWnd->y + 400, DISP_PRIO_IME3, CG_NEXT_BTN + albumWndBtnFlag[ 17 ], 2 );
						// ???????
						albumWndFontNo[ 19 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 448 - 19, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}
			break;
		}
	}

//? ?P?f??????? ***********************************************************/
	if( BattleResultWndFlag >= 1 && ProcNo == PROC_GAME && SubProcNo == 3 ){
		// ??????
		if( -- ResultWndTimer <= 0 ){
			// ????????
			DeathAction( pActMenuWnd );
			pActMenuWnd = NULL;
			// ?????????????
			BattleResultWndFlag = false;
			// ??????????????
			if( MapWmdFlagBak != true ){
				// ??????????
				play_se( 203, 320, 240 );
			}
		}

		// ?P?f???????????????
		switch( BattleResultWndFlag ){

		case 1:	// ???k???????
			// ?????????
			if( pActMenuWnd == NULL ){
				int flag = 0;
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 320 - 160, 240 - 120, 5, 6, CG_WND_TITLE_RESULT, 2 );
				// ????R?e???
				for( i = 0 ; i < MENU_BTL_RESULT_0 ; i++ ) resultWndFontNo[ i ] = -2;
				// ??????????
				for( i = 0 ; i < 4 ; i++ ){
					// ?????????????
					if( battleResultMsg.resChr[ i ].levelUp == true ) flag = true;
				}
				if( flag == true ){
					// ???????
					play_se( 211, 320, 240 );
				}else{
					// ????????
					play_se( 202, 320, 240 );
				}
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?????
						if( HitDispNo == resultWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							BattleResultWndFlag = false;
							// ??????????????
							if( MapWmdFlagBak != true ){
								// ??????????
								play_se( 203, 320, 240 );
							}
						}
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int color;
						int flag, j;
						// ????@?e
						x = pActMenuWnd->x + 12;
						y = pActMenuWnd->y + 44;

						// ????????q
						CenteringStr( pc.name, moji, CHAR_NAME_LEN );
						StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
						color = FONT_PAL_GRAY;	// ???????
						// ????????
						if( battleResultMsg.resChr[ 0 ].levelUp == true ){
							// ??????????
							StockFontBuffer( x + 152, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "LvUp!", 0 );
						}
						// ???k????
						sprintf( moji,"Exp %+5d", battleResultMsg.resChr[ 0 ].exp );
						StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, moji, 0 );
						y += 20;

						// ????????
						for( i = 0 ; i < 5 ; i++ ){
							// ??????????? ?? ??????
							if( pet[ i ].useFlag == true
									&& (pc.selectPetNo[ i ] == true || pc.ridePetNo == i ) ){
								// ???q????????
								// ????????
								if( pet[ i ].freeName[ 0 ] != NULL )
									CenteringStr( pet[ i ].freeName, moji, PET_NAME_LEN );
								else
									CenteringStr( pet[ i ].name, moji, PET_NAME_LEN );
								StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );
								color = FONT_PAL_GRAY;	// ???????
								flag = false;			// ??????
								// ?P?f????B?l?????S??
								for( j = 1 ; j < RESULT_CHR_EXP ; j++ ){
									// ????
									if( battleResultMsg.resChr[ j ].petNo == i ){
										// ????????
										if( battleResultMsg.resChr[ j ].levelUp == true ){	// ????????
											// ??????????
											StockFontBuffer( x + 152, y, FONT_PRIO_FRONT, FONT_PAL_YELLOW,"LvUp!", 0 );
										}
										// ???k????
										sprintf( moji,"Exp %+5d", battleResultMsg.resChr[ j ].exp );
										StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, moji, 0 );
										flag = true;
									}
								}
								// ??????
								if( flag == false ){
									sprintf( moji,"Exp %+4d", battleResultMsg.resChr[ j ].exp );
									StockFontBuffer( x + 148 + 58, y, FONT_PRIO_FRONT, 0, "Exp    +0", 0 );
								}
								y += 20;
							}
						}
						flag = false;
						y = pActMenuWnd->y + 134;
						// 	????: ????
						StockFontBufferUtf( x, y+30, FONT_PRIO_FRONT, 0, "  道具 �?", 0 , 0);
						// ????????????
						for( i = 0 ; i < 3 ; i++ ){
							// ?????????
							if( battleResultMsg.item[ i ][ 0 ] != NULL ){
								StockFontBuffer( x + 85, y+30, FONT_PRIO_FRONT, 0, battleResultMsg.item[ i ], 0 );
								y += 22;
								flag = true;
							}
						}
						// ????????
						if( flag == false ){
							StockFontBufferUtf( x + 85, y+30, FONT_PRIO_FRONT, 0, "没有得到任何道具�?", 0 , 0);
						}
						// ???????????????
						resultWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 216+30, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}
			break;

		case 2:	// ??????????

			// ?????????
			if( pActMenuWnd == NULL ){
				int flag = 0;
				// ???????????????
				pActMenuWnd = MakeWindowDisp( 320 - 96, 240 - 72, 3, 3, CG_WND_TITLE_RESULT, 2 );
				// ????R?e???
				for( i = 0 ; i < MENU_BTL_RESULT_0 ; i++ ) resultWndFontNo[ i ] = -2;
				// ????????
				play_se( 202, 320, 240 );
			}else{
				// ????????????????
				if( pActMenuWnd->hp > 0 ){
					// ????????
					if( mouse.onceState & MOUSE_LEFT_CRICK ){
						// ?????
						if( HitDispNo == resultWndFontNo[ 0 ] ){
							// ????????
							DeathAction( pActMenuWnd );
							pActMenuWnd = NULL;
							// ?????????????
							BattleResultWndFlag = false;
							// ??????????????
							if( MapWmdFlagBak != true ){
								// ??????????
								play_se( 203, 320, 240 );
							}
						}
					}

					// ????????
					if( pActMenuWnd != NULL ){
						int color = FONT_PAL_WHITE;
						// ????@?e
						x = pActMenuWnd->x + 17;
						y = pActMenuWnd->y + 48;
						// ??????
						sprintf( moji," ??  ??  %+8d", battleResultMsg.resChr[ 0 ].exp );
						StockFontBuffer( x, y, FONT_PRIO_FRONT, color, moji, 0 );
						y += 28;
						// ??????
						sprintf( moji," ??  ??  %8d", battleResultMsg.resChr[ 1 ].exp );
						StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, moji, 0 );

						// ???????????????
						resultWndFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd->pYobi )->mx, pActMenuWnd->y + 120, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}
			break;
		}
	}

//??????????????????
	if( MenuToggleFlag & JOY_CTRL_T )
	{
		char buffer[1024];
#ifdef _TRADESYSTEM2	// Syu ADD ???????
		char buffer2[1024];
		int j ;
#endif
#ifdef _PET_ITEM
		static bool	bViewPetEquip = false;
#endif
		switch( tradeWndNo ){
		case 0:
			//??????????????
			if( pActMenuWnd4 == NULL )
			{
				DeathMenuAction();
				DeathMenuAction2();
				//?????????buff?????
				TradeBuffer.buffer[0] = NULL;
				TradeBuffer.cnt = 0;
				TradeBuffer.cursor = 0;
				TradeBuffer.len = 8;
				TradeBuffer.color = 0;
				TradeBuffer.x = 600;
				TradeBuffer.y = 120;
				TradeBuffer.fontPrio = FONT_PRIO_FRONT;

				pActMenuWnd4 = MakeWindowDisp( 10, 0, 620, 456, 0, -1 );

#ifdef _CHANGETRADERULE		   // (?????) Syu ADD ??????????
				TradeBtnflag = false;
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
				//???????
				locknum = -1 ;
				locknum2 = -1 ;
				MainTradeWndflag = true;
				SecondTradeWndflag = false;

				//andy_add 20030610
				for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndFontNo[ i ] = -2;
				for( i = 0 ; i < 6 ; i++ ) SecondtradeWndFontNo[ i ] = -2;

				for ( i = 0 ; i < 43 ; i ++ )
					tradeList[i].data = -1 ;
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
				//??????????????
				tradetalkwndflag = false ;
				for ( i = 0 ; i < 4 ; i ++ )
					sprintf( talkmsg[i] , "" ) ;
#endif
				drag1Y = 67 ;
				drag2Y = 257 ;
				SecondTradeWnd = MakeWindowDisp( 10, 0, 620, 456, 0, -1 );
				for ( i = 0 ; i <= 42 ; i ++ ) {
					tradeList[i].data = -1 ;
					tradeList[i].kind = 'S';
				}
				for( i=0; i<21; i++){
					mytradelist[i]=-1;
					opptradelist[i]=-1;
				}

				for ( i = 0 ; i < 15; i ++ ) {
#ifdef _ITEM_PILENUMS
					itemflag[i] = pc.item[i+MAX_ITEMSTART].pile;
#else
					itemflag[i] = 0 ;
#endif
					strcpy ( opp_item[i].damage , "" ) ;
					strcpy ( opp_item[i].effect , "" ) ;
					strcpy ( opp_item[i].name , "" ) ;
				}
#endif

				InitItem3( 325, 230 );
				//Terry add 2003/11/19
				g_bTradesystemOpen = true;
				//end

			}
			else
			{

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
				// ?????????
				if ( MainTradeWndflag == true )
				{
					SecondTradeWndflag = false ;
#endif

					if( pActMenuWnd4->hp > 0 )
					{


						//?????????????
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
						//??????????????????PetAction
						if ( SecondActPet != NULL )
						{
							DeathAction ( SecondActPet ) ;
							SecondActPet = NULL ;
						}


						StockDispBuffer( ( ( WINDOW_DISP *)pActMenuWnd4->pYobi )->mx, ( ( WINDOW_DISP *)pActMenuWnd4->pYobi )->my - 20 , DISP_PRIO_MENU, CG_TRADE_WND, 1 );
#endif


						// ??????????????????? Start
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							// ????????? Start
							//???????????
							if( HitDispNo == tradeWndFontNo[ 1 ])
							{
								//?????????
								MenuToggleFlag ^= JOY_CTRL_T;
								//????????��
								play_se( 203, 320, 240 );
								sprintf(buffer, "W|%s|%s", opp_sockfd, opp_name);
								//??????????Server?????
								lssproto_TD_send( sockfd, buffer);
								tradeStatus = 0;
								tradeInit();
								pc.trade_confirm = 1;
							}
							// ????????? End


							// ???? Lock ?? ????? Start
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							//?????????????????Lock?????function
							LockAndOkfunction();
							// ???? Lock ?? ????? End

							//????????��??
							if( HitDispNo == tradeWndFontNo[ 21 ] )
							{
								MainTradeWndflag = false;
								SecondTradeWndflag = true;
								//andy_add 20030610
								for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndFontNo[ i ] = -2;
								for( i = 0 ; i < 6 ; i++ ) SecondtradeWndFontNo[ i ] = -2;

							}
							//????????????
							if( HitDispNo == tradeWndFontNo[ 18 ] )
							{
								if ( drag1Y >= 67 )
									drag1Y -= 4 ;
								if ( drag1Y < 67 )
									drag1Y = 67 ;
							}
							//????????????
							if( HitDispNo == tradeWndFontNo[ 19 ] )
							{
								if ( drag1Y <= 151 )
									drag1Y += 4 ;
								if ( drag1Y > 151 )
									drag1Y = 151 ;
							}
							//????????????
							if( HitDispNo == tradeWndFontNo[ 15 ] )
							{
								if ( drag2Y >= 257 )
									drag2Y -= 4 ;
								if ( drag2Y < 257 )
									drag2Y = 257 ;
							}
							//????????????
							if( HitDispNo == tradeWndFontNo[ 16 ] )
							{
								if ( drag2Y <= 341 )
									drag2Y += 4 ;
								if ( drag2Y > 341 )
									drag2Y = 341 ;
							}
							//???????????
							if( HitDispNo == tradeWndFontNo[ 20 ] )
								dragflag1 = true ;
							//???????????
							if( HitDispNo == tradeWndFontNo[ 17 ] )
								dragflag2 = true ;
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
							//????
							if( HitDispNo == tradeWndFontNo[ 14 ] )
								talkwndflag = true ;
							//????

#endif
#endif

						}
						// ??????????????????? End


#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
						//???????????????
						if ( dragflag1 == true && mouse.nowPoint.y <= 151 &&
							mouse.nowPoint.y >= 67 )
							drag1Y = mouse.nowPoint.y ;
						//???????????????
						if ( dragflag2 == true && mouse.nowPoint.y <= 341 &&
							mouse.nowPoint.y >= 257 )
							drag2Y = mouse.nowPoint.y ;
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
						//??????????��??
						if ( talkwndflag == true )
						{
							talkwndx = mouse.nowPoint.x ;
							talkwndy = mouse.nowPoint.y ;
						}
						if ( mouse.onceState & MOUSE_RIGHT_CRICK )
							tradetalkwndflag = false ;
#endif
#endif

						// ???????????? Start
						// ????????????
						if( HitDispNo == tradeWndFontNo[ 2 ] )
						{
							if( mouse.onceState & MOUSE_LEFT_CRICK )
							{
								tradeWndBtnFlag[ 2 ] = true;
								int i=0;
								while( 1 )
								{
									tradePetIndex++;
									if( tradePetIndex >=5 ) tradePetIndex =0;
									if( pet[tradePetIndex].useFlag != NULL
										&& pc.ridePetNo != tradePetIndex )
										break;
									if( ( i++ ) > 5 )
									{
										tradePetIndex = -1;
										break;
									}
								}
								if( pActPet3 != NULL )
								{
									DeathAction( pActPet3 );
									pActPet3 = NULL;
								}

							}
						}
						// ???????????? End

						// ???????????? Start
						// ????????????
						if( HitDispNo == tradeWndFontNo[ 3 ] )
						{
							if( mouse.onceState & MOUSE_LEFT_CRICK )
							{
								tradeWndBtnFlag[ 3 ] = true;
								int i =0;
								while( 1 )
								{
									tradePetIndex--;
									if( tradePetIndex <0 ) tradePetIndex =4;
									if( pet[tradePetIndex].useFlag != NULL
										&& pc.ridePetNo != tradePetIndex )
										break;
									if( (i++) >5 )
									{
										tradePetIndex = -1;
										break;
									}
								}
								if( pActPet3 != NULL )
								{
									DeathAction( pActPet3 );
									pActPet3 = NULL;
								}

							}
						}

						// ???????????? End

						// ??????????? Start
						if( HitDispNo == tradeWndFontNo[ 4 ] )
						{
							if( mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[ 4 ] == true )
								tradeWndBtnFlag[ 4 ] = false;
							if( tradeWndBtnFlag[ 4 ] == true )
							{
								// ???????????Focus????ChatBuffer
								GetKeyInputFocus( &MyChatBuffer );
								// ????????????څ
								Tradeflag = false;
								tradeWndDropGold += tradeWndDropGoldInc;
								tradeWndDropGoldCnt++;
								if( tradeWndDropGoldCnt >= 30 ){
									tradeWndDropGoldCnt = 0;
									if( tradeWndDropGoldInc == 0 )
										tradeWndDropGoldInc = 1;
									else {
										tradeWndDropGoldInc *= 5;
										if( tradeWndDropGoldInc > 10000 )
											tradeWndDropGoldInc = 10000;
									}
								}
								//??????????
								if( tradeWndDropGold >= pc.gold )
								{
									tradeWndDropGold = pc.gold;
									play_se( 220, 320, 240 );
								}
							}
							if( mouse.onceState & MOUSE_LEFT_CRICK )
							{
								tradeWndDropGold++;
								if( tradeWndDropGold >= pc.gold )
								{
									tradeWndDropGold = pc.gold;
									play_se( 220, 320, 240 );
								}
								else
								{
									tradeWndDropGoldInc = 0;
									tradeWndDropGoldCnt = 0;
									tradeWndBtnFlag[ 4 ] = true;
									play_se( 217, 320, 240 );
								}
							}
						}
						else
							tradeWndBtnFlag[ 4 ] = false;
						// ??????????? End

						// ?????????? Start
						if( HitDispNo == tradeWndFontNo[ 5 ] )
						{
							if( mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[ 5 ] == true )
								tradeWndBtnFlag[ 5 ] = false;
							if( tradeWndBtnFlag[ 5 ] == true )
							{
								// ??????????Focus????ChatBuffer
								GetKeyInputFocus( &MyChatBuffer );
								// ????????????څ
								Tradeflag = false;
								tradeWndDropGold -= tradeWndDropGoldInc;
								tradeWndDropGoldCnt++;
								if( tradeWndDropGoldCnt >= 30 )
								{
									tradeWndDropGoldCnt = 0;
									if( tradeWndDropGoldInc == 0 )
									{
										tradeWndDropGoldInc = 1;
									}
									else
									{
										tradeWndDropGoldInc *= 5;
										if( tradeWndDropGoldInc > 10000 )
											tradeWndDropGoldInc = 10000;
									}
								}
								if( tradeWndDropGold < 0 )
								{
									tradeWndDropGold = 0;
									play_se( 220, 320, 240 );
								}
							}
							if( mouse.onceState & MOUSE_LEFT_CRICK ) {
								tradeWndDropGold--;
								if( tradeWndDropGold <= 0 )
								{
									tradeWndDropGold = 0;
									play_se( 220, 320, 240 );
								}
								else
								{
									tradeWndDropGoldInc = 0;
									tradeWndDropGoldCnt = 0;
									tradeWndBtnFlag[ 5 ] = true;
									play_se( 217, 320, 240 );
								}
							}
						}
						else
							tradeWndBtnFlag[ 5 ] = false;
						// ?????????? End

						// ????????��? Start
						if( HitDispNo == tradeWndFontNo[ 6 ] )
						{
							if( mouse.onceState & MOUSE_LEFT_CRICK )
							{
								// ???��??????Focus????ChatBuffer
								GetKeyInputFocus( &MyChatBuffer );
								// ????????????څ
								Tradeflag = false;
								if( tradeWndDropGold > 0 && eventWarpSendFlag == false )
								{
									tradeWndBtnFlag[ 6 ] = true;
									play_se( 217, 320, 240 );
								}
								else
									play_se( 220, 320, 240 );
							}
							if( mouse.onceState & MOUSE_LEFT_CRICK_UP && tradeWndBtnFlag[ 6 ] == true )
							{
								if( tradeStatus != 1 )	return;
								if( pc.trade_confirm == 2 || pc.trade_confirm == 4 ) return;
								tradeWndBtnFlag[ 6 ] = false;
								int TradeGoldIndex = 0;

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
								if ( ( tradeList[21].data == -1 ) )
								{
									tradeList[21].data = tradeWndDropGold ;
									tradeList[21].kind = 'G' ;
									TradeGoldIndex = 3 ;
								}
#endif

								if( TradeGoldIndex != 0 )
								{
#ifdef _CHANGETRADERULE		   // (?????) Syu ADD ??????????
									TradeBtnflag = true;
#endif
									play_se( 212, 320, 240 );
									tradeWndDropGoldSend = tradeWndDropGold;
									sprintf( buffer, "T|%s|%s|G|%d|%d", opp_sockfd, opp_name, TradeGoldIndex, tradeWndDropGoldSend);
									lssproto_TD_send( sockfd, buffer);
									tradeWndDropGold = 0;
								}
							}
						}
						else
							tradeWndBtnFlag[ 6 ] = false;
						// ????????��? End

						// ?????????��? Start
						if( HitDispNo == tradeWndFontNo[ 7 ] )
						{
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							tradeWndBtnFlag[ 7 ] = true;
#endif
							if( mouse.onceState & MOUSE_LEFT_CRICK_UP )
							{
								if( tradeStatus != 1 )	return;
								if( pc.trade_confirm == 2 || pc.trade_confirm == 4 ) return;

								if( pet[ tradePetIndex ].useFlag
									&&  pc.ridePetNo != tradePetIndex )
								{

									if( pet[ tradePetIndex ].freeName[0] != NULL )
										strcpy(tradePet[0].name ,pet[ tradePetIndex ].freeName);
									else
										strcpy(tradePet[0].name ,pet[ tradePetIndex ].name);
									tradePet[0].level = pet[ tradePetIndex ].level;
									tradePet[0].atk = pet[ tradePetIndex ].atk;
									tradePet[0].def = pet[ tradePetIndex ].def;
									tradePet[0].quick = pet[ tradePetIndex ].quick;
									tradePet[0].graNo = pet[ tradePetIndex ].graNo;
									tradePet[0].trn = pet[tradePetIndex].trn;
									tradePet[0].maxHp = pet[tradePetIndex].maxHp;

									tradePet[0].index = tradePetIndex;
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
									pet[ tradePetIndex ].useFlag = NULL ;
									if( pActPet3 )
									{
										DeathAction( pActPet3 );
										pActPet3 = NULL;
									}
#endif


									DeathAction( pActPet4 );
									pActPet4 = NULL;
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
									tradeList[ tradePetIndex+16 ].data = tradePetIndex ;
									tradeList[ tradePetIndex+16 ].kind = 'P' ;
									strcpy ( tradeList[ tradePetIndex+16 ].name ,  pet[ tradePetIndex].name );
									tradeList[ tradePetIndex+16 ].level = pet[ tradePetIndex ].level;
									tradeList[ tradePetIndex+16 ].trns = pet[ tradePetIndex ].trn;
#ifdef _SHOW_FUSION
									tradeList[ tradePetIndex+16 ].fusion = pet[ tradePetIndex ].fusion;
#endif

									if( pet[ tradePetIndex ].freeName[0] != NULL )
										strcpy ( tradeList[ tradePetIndex+16 ].freename , pet[ tradePetIndex ].freeName ) ;
									else
										strcpy ( tradeList[ tradePetIndex+16 ].freename ,"");// change fix ???��??????????????????????????????bug
#endif

									if( tradeStatus )
									{
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
										//????????????????????????????????
										sprintf( buffer2, "%s|%s|%s|%s|%s|%s|%s|%s|%s" ,
											petSkill[ tradePetIndex ][ 0 ].name ,
											petSkill[ tradePetIndex ][ 1 ].name ,
											petSkill[ tradePetIndex ][ 2 ].name ,
											petSkill[ tradePetIndex ][ 3 ].name ,
											petSkill[ tradePetIndex ][ 4 ].name ,
											petSkill[ tradePetIndex ][ 5 ].name ,
											petSkill[ tradePetIndex ][ 6 ].name ,
											pet[ tradePetIndex].name ,
											pet[ tradePetIndex ].freeName
											) ;
										sprintf( buffer, "T|%s|%s|P|3|%d|%s", opp_sockfd, opp_name, tradePetIndex , buffer2);
#endif
										lssproto_TD_send( sockfd, buffer);
									}
								}
							}
						}
						// ?????????��? End

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
						//???????????????????????
						if( mouse.onceState & MOUSE_LEFT_CRICK_UP )
						{
							dragflag1 = false ;
							dragflag2 = false ;
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
							talkwndflag = false ;
#endif
						}
#endif
						//��??????????????
						for( i = 2 ; i <= 8 ; i++ )
						{
							if( mouse.state & MOUSE_LEFT_CRICK && tradeWndBtnFlag[ i ] == true )
								tradeWndBtnFlag[ i ] = true;
							else
								tradeWndBtnFlag[ i ] = false;
						}
						// ??????????????
						if( pActMenuWnd4 != NULL )
						{
							// ?????????X , Y????
							x = pActMenuWnd4->x ;
							y = pActMenuWnd4->y ;
							// ?????????????

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
							//?????????
							if ( tradetalkwndflag != false )
							{
								for ( i = 3 ; i >= 0 ; i -- )
								{
									sprintf( moji, "%s", talkmsg[i]);
									StockFontBuffer( talkwndx - 154 , talkwndy -26 + i * 20 , FONT_PRIO_AFRONT, FONT_PAL_YELLOW, moji, 0 );
								}
								tradeWndFontNo[ 14 ] = StockDispBuffer( talkwndx , talkwndy, DISP_PRIO_BOX3 , CG_TRADE_TALK_WND, 2 );
							}
#endif
							int ShowPoint ;
							int j = 0 ;
							//????????????��??
							ShowPoint = ( drag2Y - 257 ) / 4;
							//???????List?��
							for ( i = 0 ; i < 21 ; i ++ )
							{
								if ( tradeList[ i + 1 ].data != -1 )
								{
									mytradelist[j] = i + 1 ;
									j ++ ;
								}
							}
							j = 0 ;
							//???????List?��
							for ( i = 0 ; i < 21 ; i ++ )
							{
								if ( tradeList[ i + 22 ].data != -1 )
								{
									opptradelist[j] = i + 22 ;
									j ++ ;
								}
							}
							j = 0 ;
							//????????��
							for ( i = ShowPoint  ; i < ShowPoint + 5  ; i ++ )
							{
								if ( mytradelist[i] == -1 ) break;
								if ( i > 21 ) break;
								//????????HitBox???????
								//								if( MakeHitBox( x + 10 , y + 226 + j * 29, x + 280  , y + 244 + j * 29 , DISP_PRIO_IME3 ) == true ){
								//								}
								sprintf( moji , "%c" , tradeList[mytradelist[i]].kind ) ;
								//???????
								if( strcmp( moji , "I" ) == 0 )
								{
									sprintf( moji , "%s" , tradeList[mytradelist[i]].name ) ;
									StockFontBuffer(x + 12 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);

#ifdef _ITEM_PILENUMS
									sprintf( moji , "x%d" , tradeList[mytradelist[i]].pilenum ) ;
									StockFontBuffer(x + 180 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
#endif
									sprintf( moji , "%s" , tradeList[mytradelist[i]].damage ) ;
									StockFontBuffer(x + 220 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									j ++ ;
								}
								//???????
								else if( strcmp ( moji , "P" ) == 0 )
								{
									int colors = 0;
									int mylist = mytradelist[i];
									sprintf( moji , "%s" , tradeList[mylist].name ) ;
									StockFontBuffer(x + 12 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);

									if( strcmp( tradeList[mylist].freename, tradeList[mylist].name) )
									{
										sprintf( moji , "[%s]" , tradeList[mylist].freename ) ;
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102 , y + 228 + j * 29 , FONT_PRIO_FRONT, colors, moji, 0);
									}
#ifdef _PET_2TRANS
									sprintf( moji , "Lv:%d%s" , tradeList[mylist].level, "");
									if( tradeList[mylist].trns==1 )
										sprintf( moji , "Lv:%d%s" , tradeList[mylist].level, "һת");
									else if( tradeList[mylist].trns==2 )
										sprintf( moji , "Lv:%d%s" , tradeList[mylist].level, "��ת");
#ifdef _SHOW_FUSION
									if (LOWORD(tradeList[mylist].fusion) == 1)
										sprintf( moji , "Lv:%d%s" , tradeList[mylist].level, "�ں�");
#endif
#else
									sprintf( moji , "Lv:%d%s" , tradeList[mylist].level, (tradeList[mylist].trns==0)?"":"?");
#endif
									StockFontBuffer(x + 220 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);

									j ++ ;
								}
								//??????
								else if( strcmp ( moji , "G" ) == 0 )
								{
									StockFontBuffer(x + 12 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, "ʯ��", 0);
									sprintf( moji , "%d" , tradeList[mytradelist[i]].data ) ;
									StockFontBuffer(x + 102 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									sprintf( moji , "%s" , "Gold" ) ;
									StockFontBuffer(x + 220 , y + 228 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									j ++ ;
								}
								if ( j == 5 )
									break;
							}
							j = 0 ;
							//????????????��??
							ShowPoint = ( drag1Y - 67 ) / 4 ;
							//????????��
							for ( i = ShowPoint + 22  ; i < ShowPoint + 27 ; i ++ )
							{
								if ( opptradelist[ i - 22] == -1 ) break;
								if ( i > 42 ) break;
								//????????HitBox???????
								//								if( MakeHitBox( x + 10 , y + 37 + j * 29, x + 280  , y + 55 + j * 29 , DISP_PRIO_IME3 ) == true ){
								//								}
								sprintf( moji , "%c" , tradeList[ opptradelist[i - 22]].kind ) ;
								//???????
								if( strcmp( moji , "I" ) == 0 )
								{
									int colors = 0;
									int opptradlist = opptradelist[i - 22];
// Terry modify for ??????? 2004/6/9
									sprintf( moji , "%s" , tradeList[ opptradlist].name);
//									sprintf( moji , "%s" , tradeList[ opptradlist].freename ) ;
//									if( strcmp( tradeList[opptradlist].name, tradeList[opptradlist].freename ) ){
//										sprintf( moji , "[%s]" , tradeList[ opptradlist].freename ) ;
//										colors = 6;
//									}
									StockFontBuffer(x + 12 , y + 40 + j * 29  , FONT_PRIO_FRONT, colors, moji, 0);
#ifdef _ITEM_PILENUMS
									sprintf( moji , "x%d" , tradeList[opptradelist[i - 22]].pilenum ) ;
									StockFontBuffer(x + 180 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
#endif

									sprintf( moji , "%s" , tradeList[opptradelist[i - 22]].damage ) ;
									StockFontBuffer(x + 220 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									j ++ ;
								}
								else if( strcmp ( moji , "P" ) == 0 )
								{//???????
									int colors = 0; //FONT_PAL_RED
									int opplist = opptradelist[i - 22];
									sprintf( moji , "%s" , tradeList[opplist].name ) ;
									StockFontBuffer(x + 12 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
#ifdef _SHOW_FUSION
									// change fix ??????-----> []
									if( strcmp( tradeList[opplist].freename, tradeList[opplist].name) == 0 )
									{
										sprintf( moji , "[]" ) ;
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102 , y + 40 + j * 29 , FONT_PRIO_FRONT, colors, moji, 0);
									}
#endif
									if( strcmp( tradeList[opplist].freename, tradeList[opplist].name) )
									{
										sprintf( moji , "[%s]" , tradeList[opplist].freename ) ;
										colors = FONT_PAL_RED;
										StockFontBuffer(x + 102 , y + 40 + j * 29 , FONT_PRIO_FRONT, colors, moji, 0);
									}
#ifdef _PET_2TRANS
									sprintf( moji , "Lv:%d%s" , tradeList[opplist].level, (tradeList[opplist].trns==0)?"":"");
									if( tradeList[opplist].trns==1 )
										sprintf( moji , "Lv:%d%s" , tradeList[opplist].level, "һת");
									else if( tradeList[opplist].trns==2 )
										sprintf( moji , "Lv:%d%s" , tradeList[opplist].level, "��ת");
#ifdef _SHOW_FUSION
									if (LOWORD(tradeList[opplist].fusion) == 1)
										sprintf( moji , "Lv:%d%s" , tradeList[opplist].level, "�ں�");
#endif
#else
									sprintf( moji , "Lv:%d%s" , tradeList[opplist].level, (tradeList[opplist].trns==0)?"":"?");
#endif
									StockFontBuffer(x + 220 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									j ++ ;
								}
								//??????
								else if( strcmp ( moji , "G" ) == 0 )
								{
									StockFontBuffer(x + 12 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, "ʯ��", 0);
									sprintf( moji , "%d" , tradeList[opptradelist[i - 22]].data ) ;
									StockFontBuffer(x + 102 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);
									sprintf( moji , "%s" , "Gold" ) ;
									StockFontBuffer(x + 220 , y + 40 + j * 29 , FONT_PRIO_FRONT, 0, moji, 0);

									j ++ ;
								}
								if ( j == 5 )
									break;
							}
#endif
							sprintf(moji, "%7d", pc.gold);
							StockFontBuffer(x + 550, y + 65, FONT_PRIO_FRONT, 0, moji, 0);
							// Focus???????????????????
							if ( Tradeflag == false )
							{
								sprintf( moji,"%7d", tradeWndDropGold );
								StockFontBuffer(x + 550, y + 120, FONT_PRIO_FRONT, 0, moji, 0 );
							}
							if( pet[ tradePetIndex ].useFlag
								&& pc.ridePetNo != tradePetIndex )
							{
								if( pActPet3 == NULL )
								{
#ifdef _LIZARDPOSITION			   // (?????) Syu ADD ???????????��?��???
									if((pet[ tradePetIndex ].graNo == 101493)||(pet[ tradePetIndex ].graNo == 101494)||
										(pet[ tradePetIndex ].graNo == 101495)||(pet[ tradePetIndex ].graNo == 101496))
									{
										pActPet3 = MakeAnimDisp( 400, /*175*/125, pet[ tradePetIndex ].graNo, ANIM_DISP_PET );
									}
									else
										pActPet3 = MakeAnimDisp( 400, /*175*/145, pet[ tradePetIndex ].graNo, ANIM_DISP_PET );
#else
									pActPet3 = MakeAnimDisp( 400, /*175*/145, pet[ tradePetIndex ].graNo, ANIM_DISP_PET );
#endif
								}

								if( pet[ tradePetIndex ].freeName[0] != NULL )
									CenteringStr( pet[ tradePetIndex ].freeName, moji, PET_NAME_LEN );
								else
									CenteringStr( pet[ tradePetIndex ].name, moji, PET_NAME_LEN );

								//???????????????
#ifdef _TRADESYSTEM2			// (?????) Syu ADD ???????
								//????��?????????
								if(pet[tradePetIndex].trn==1)
									StockFontBuffer( x +330, y+ 33, FONT_PRIO_FRONT, 1, moji, 0 );
								else
									StockFontBuffer( x +330, y+ 33, FONT_PRIO_FRONT, 0, moji, 0 );

								sprintf( moji,"%3d",pet[ tradePetIndex ].level );
								StockFontBuffer( x + 485, y +105, FONT_PRIO_FRONT, 0, moji, 0 );
								sprintf( moji, "%3d", pet[ tradePetIndex ].atk );
								StockFontBuffer( x + 485, y +129, FONT_PRIO_FRONT, 0, moji, 0 );
								sprintf( moji, "%3d", pet[ tradePetIndex ].def );
								StockFontBuffer( x + 485, y +153, FONT_PRIO_FRONT, 0, moji, 0 );
								sprintf( moji, "%3d", pet[ tradePetIndex ].quick );
								StockFontBuffer( x + 485, y +177, FONT_PRIO_FRONT, 0, moji, 0 );
								sprintf( moji, "%3d", pet[ tradePetIndex ].maxHp );
								StockFontBuffer( x + 485, y +201, FONT_PRIO_FRONT, 0, moji, 0 );
#endif
							}


#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							// ???????��?????????Button???
							if( pc.trade_confirm == 1 || pc.trade_confirm == 3 )
								tradeWndFontNo[ 0 ] = StockDispBuffer( x+59, y+390, DISP_PRIO_IME3, CG_TRADE_LOCK_BTN, 2 );
							if( pc.trade_confirm == 4 )
								tradeWndFontNo[ 0 ] = StockDispBuffer( x+59, y+390, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2 );
							//???
							tradeWndFontNo[ 1 ] = StockDispBuffer( x+240, y+390, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2 );
#endif

							tradeWndFontNo[ 2 ] = StockDispBuffer( x+452+20, y+63+8, DISP_PRIO_IME3, CG_TRADE_LEFT_BTN_UP + tradeWndBtnFlag[2], 2 );
							tradeWndFontNo[ 3 ] = StockDispBuffer( x+486+20, y+63+8, DISP_PRIO_IME3, CG_TRADE_RIGHT_BTN_UP + tradeWndBtnFlag[3], 2 );
							tradeWndFontNo[ 4 ] = StockDispBuffer( x+554-94, y+93+106, DISP_PRIO_IME3, CG_TRADE_UP_BTN_UP + tradeWndBtnFlag[4], 2 );
							tradeWndFontNo[ 5 ] = StockDispBuffer( x+560-94, y+93+106, DISP_PRIO_IME3, CG_TRADE_DOWN_BTN_UP + tradeWndBtnFlag[5], 2 );
#ifdef _CHANGETRADERULE		   // (?????) Syu ADD ??????????
							if( TradeBtnflag == false )
								tradeWndFontNo[ 6 ] = StockDispBuffer( x+562-62+25, y+148+108+8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[6], 2 );
#else
							tradeWndFontNo[ 6 ] = StockDispBuffer( x+562-62+25, y+148+108+8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[6], 2 );
#endif
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							//???
							tradeWndFontNo[ 7 ] = StockDispBuffer( x+365-62+25, y+190+108+8, DISP_PRIO_IME3, CG_TRADE_PUT_BTN_UP + tradeWndBtnFlag[7], 2 );
#endif
#ifndef _CHANGETRADERULE		   // (?????) Syu ADD ??????????
							tradeWndFontNo[ 8 ] = StockDispBuffer( x+55+25, y+190+18, DISP_PRIO_IME3, CG_MAIL_WND_CLEAR_BTN_UP + tradeWndBtnFlag[8], 2 );
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							//???
							//??????????????
							StockFontBuffer( x + 5, y + 193  , FONT_PRIO_FRONT, FONT_PAL_PURPLE, pc.name , 0 );
							StockFontBuffer( x + 5, y + 6, FONT_PRIO_FRONT, FONT_PAL_PURPLE, opp_name, 0 );
							//????????
							tradeWndFontNo[ 21 ] = StockDispBuffer( x+150, y+390, DISP_PRIO_IME3, CG_TRADE_VIEW_BTN, 2 );
							//???????????
							tradeWndFontNo[ 18 ] = StockDispBuffer( x+302, y+42, DISP_PRIO_IME3, CG_TRADE_SCROLL_UP, 2 );
							//????????????
							tradeWndFontNo[ 19 ] = StockDispBuffer( x+302, y+176, DISP_PRIO_IME3, CG_TRADE_SCROLL_DOWN, 2 );
							//????????????
							tradeWndFontNo[ 20 ] = StockDispBuffer( x+302, y+drag1Y, DISP_PRIO_IME3, CG_TRADE_SCROLL_BTN, 2 );
							//???????????
							tradeWndFontNo[ 15 ] = StockDispBuffer( x+302, y+232, DISP_PRIO_IME3, CG_TRADE_SCROLL_UP, 2 );
							//????????????
							tradeWndFontNo[ 16 ] = StockDispBuffer( x+302, y+366, DISP_PRIO_IME3, CG_TRADE_SCROLL_DOWN, 2 );
							//????????????
							tradeWndFontNo[ 17 ] = StockDispBuffer( x+302, y+drag2Y, DISP_PRIO_IME3, CG_TRADE_SCROLL_BTN, 2 );
#endif

							// ?????��?????��???
							if( MakeHitBox( x + 530 , y + 115 , x + + 530 + 86 , y + 115 + 25 , DISP_PRIO_IME3 ) == true )
							{
								// ??????????focus???????
								if(mouse.onceState & MOUSE_LEFT_CRICK)
								{
									strcpy(TradeBuffer.buffer,"");
									TradeBuffer.buffer[0] = NULL;
									TradeBuffer.cursor = 0;
									TradeBuffer.cnt = 0;
									Tradeflag = true;
									GetKeyInputFocus( &TradeBuffer );
									play_se( 217, 320, 240 );
								}
							}
							// focus?????????????buffer
							if( Tradeflag == true)
							{
								TradeBuffer.x = pActMenuWnd4->x + 600 - strlen(TradeBuffer.buffer) * 7;
								StockFontBuffer2( &TradeBuffer );
								tradeWndDropGold = atoi(TradeBuffer.buffer);
								if ( atoi(TradeBuffer.buffer) >= pc.gold )
								{
									sprintf(TradeBuffer.buffer,"%d",pc.gold);
									tradeWndDropGold = pc.gold;
								}
							}

							if(pc.trade_confirm==2)
							{
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
								//???
								StockFontBuffer( x + 220, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "��������", 0 );
							}
							if(pc.trade_confirm==3)
							{
								StockFontBuffer( x + 210, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "�Է���������", 0 );
							}
							if(pc.trade_confirm==4)
							{
								if( tradeStatus == 2 )
									StockFontBuffer( x + 220, y + 194 , FONT_PRIO_FRONT, FONT_PAL_RED, "����ȷ��", 0 );
								else
									StockFontBuffer( x + 210, y + 194, FONT_PRIO_FRONT, FONT_PAL_RED, "�����ȷ��??", 0 );
							}
#endif
						}

						for(  i = MAX_ITEM -1 ; i >= MAX_ITEMSTART ; i-- )
						{
							// ?????V?????
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
							//???????????????
#ifdef _ITEM_PILENUMS
							if ( itemflag[ i - MAX_ITEMSTART ] <= 0 )
							{
#else
								if ( itemflag[ i - MAX_ITEMSTART ] == 1 )
								{
#endif
									//???????????????????
									//StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, CG_TRADE_SEAL , 0 );
									//????????????????????????????????

									continue ;

								}
#endif

								if( MakeHitBox( ItemBuffer[ i ].defX - 24, ItemBuffer[ i ].defY - 24,
									ItemBuffer[ i ].defX + 26, ItemBuffer[ i ].defY + 23, DISP_PRIO_IME3 ) == true )
								{
#ifdef _TELLCHANNEL
									// Terry fix 2003/12/16 for ????????????,??????????????????
									//TaskBarFlag = true;???��??
									bShowItemExplain = true; // ????????
									// end
#endif
									// ????��?��???????
									if( pc.item[ i ].useFlag == true && ItemBuffer[ i ].mixFlag <= 2 )
									{
										char *splitPoint = pc.item[ i ].memo;
										int color = pc.item[ i ].color;
										// ?????????????Check
										if( pc.transmigration == 0 && pc.level < pc.item[ i ].level )
											color = FONT_PAL_RED;
										// ???????????
										StockFontBuffer( 0, 460, FONT_PRIO_FRONT, color, pc.item[ i ].name, 0 );
										// ???????
										StockFontBuffer( 160, 460, FONT_PRIO_FRONT, 0, splitPoint, 0 );
									}
									// ????????????????
									if( mouse.onceState & MOUSE_LEFT_DBL_CRICK )
									{
										if( tradeStatus != 1 )	return;
										if( pc.trade_confirm == 2 || pc.trade_confirm == 4 ) return;
										// ????????
										ItemBuffer[ i ].mixFlag = false;
										// ????��???��???
										if( pc.item[ i ].useFlag == true ){
											ItemBuffer[ i ].dragFlag = true;
											mouse.itemNo = i;
											// ???????????
											itemNo = -1;
										}
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
										if (mouse.itemNo != -1)
										{

											char buf[1024];
											tradeList[ i - 4 ].kind = 'I';
											tradeList[ i - 4 ].data = i ;
											strcpy ( tradeList[ i - 4 ].name , pc.item[ i ].name ) ;
											strcpy ( tradeList[ i - 4 ].damage , pc.item[ i ].damage ) ;
#ifdef _ITEM_PILENUMS
											itemflag[ i - MAX_ITEMSTART ]--;
#else
											itemflag[ i - MAX_ITEMSTART ] = 1;
#endif
#ifdef _ITEM_PILENUMS
											tradeList[ i - 4 ].pilenum = pc.item[ i ].pile - itemflag[i-MAX_ITEMSTART];
#endif
											sprintf(buf, "T|%s|%s|I|1|%d", opp_sockfd, opp_name, i);
											lssproto_TD_send(sockfd, buf);

										}
#endif
										mouse.itemNo = -1;
									}
								}
								if( pc.item[ i ].useFlag == true )
								{
									// ????????????
									if( ItemBuffer[ i ].mixFlag <= 2 ) {
										char buf[256];
										// ???????
										StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );


#ifdef _ITEM_PILENUMS
										sprintf(buf, "%d", itemflag[ i - MAX_ITEMSTART ]);
										StockFontBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, FONT_PRIO_FRONT, FONT_PAL_RED, buf, 0 );
#endif

									}else if( ItemBuffer[ i ].mixFlag ==  10 )
										StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
									else if( ItemBuffer[ i ].mixFlag ==  11 )
										StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
									else if( ItemBuffer[ i ].mixFlag == 3 )
										StockDispBuffer( pActPet2->x, pActPet2->y - 16, DISP_PRIO_DRAG, pc.item[ i ].graNo, 0 );
									else if( ItemBuffer[ i ].mixFlag == 4 )
										StockDispBuffer( pActPet2->x, pActPet2->y, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );

								}

							}
						}
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
				}
				// ???????????
				else  if ( SecondTradeWndflag == true )
				{
					MainTradeWndflag = false ;
					if ( pActMenuWnd4 != NULL && SecondTradeWnd != NULL )
					{
						x = SecondTradeWnd->x ;
						y = SecondTradeWnd->y ;
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
						//?????????
						if ( tradetalkwndflag != false )
						{
							for ( i = 3 ; i >= 0 ; i -- )
							{
								sprintf( moji, "%s", talkmsg[i]);
								StockFontBuffer( talkwndx - 154 , talkwndy -26 + i * 20 , FONT_PRIO_AFRONT, FONT_PAL_YELLOW, moji, 0 );
							}
							SecondtradeWndFontNo[ 4 ] = StockDispBuffer( talkwndx , talkwndy, DISP_PRIO_BOX3 , CG_TRADE_TALK_WND, 2 );
						}
#endif
						SecondtradeWndFontNo[ 0 ] = StockDispBuffer( x+254, y+380, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2 );
						SecondtradeWndFontNo[ 1 ] = StockDispBuffer( x+159, y+380, DISP_PRIO_IME3, CG_TRADE_BACK_BTN, 2 );
						//?????????
						if( pc.trade_confirm == 1 || pc.trade_confirm == 3 )
							SecondtradeWndFontNo[ 2 ] = StockDispBuffer( x+63, y+380, DISP_PRIO_IME3, CG_TRADE_LOCK_BTN, 2 );
						if( pc.trade_confirm == 4 )
							SecondtradeWndFontNo[ 2 ] = StockDispBuffer( x+63, y+380, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2 );

						sprintf( moji,"%7d", tradeWndDropGoldGet );

#ifdef _PET_ITEM
						// ??????????????
						if (locknum != -1 && tradeList[locknum + 37].data != -1)
						{
							if (bViewPetEquip)
								SecondtradeWndFontNo[5] = StockDispBuffer(x + 259, y + 310, DISP_PRIO_IME3, 26472, 2);
							else
								SecondtradeWndFontNo[5] = StockDispBuffer(x + 259, y + 310, DISP_PRIO_IME3, 26471, 2);
						}
#endif

						//??????
						if( tradeWndDropGoldGet > 0 )
							StockFontBuffer( x + 95 , y + 336 , FONT_PRIO_FRONT, 0, moji, 0 );
						for ( i = 0 ; i < 5 ; i ++ )
						{
							if ( MakeHitBox ( 325 , 12 + i * 42 , 600 , 51 + i * 42 , DISP_PRIO_IME3 ) == true )
							{
#ifdef _TRADELOCKBTN				// (?????) Syu ADD ??????????
								if ( mouse.onceState & MOUSE_LEFT_CRICK )
								{
	#ifdef _PET_ITEM
									bViewPetEquip = false;
									HitDispNo = 0;
	#endif
									locknum = i ;
									locknum2 = -1 ;
								}
								if ( locknum == -1 )
								{
									locknum = i ;
									locknum2 = -1 ;
								}
							}
							if ( locknum != -1 )
							{
								if ( tradeList[ locknum + 37 ].data != -1 )
								{
	#ifdef _PET_ITEM
									if (bViewPetEquip)
									{
										int		iY = 251, iColor;
										char	*splitPoint;

										// ?????????????????????
										for (int iCount = 0; iCount < PET_EQUIPNUM; iCount++)
										{
											StockDispBuffer(ItemBuffer[iCount].defX, ItemBuffer[iCount].defY, DISP_PRIO_IME3, nPetItemEquipBmpNumber[iCount][0], 0);
											if (opp_pet[locknum].oPetItemInfo[iCount].bmpNo > 0)
												StockDispBuffer(ItemBuffer[iCount].defX, ItemBuffer[iCount].defY, DISP_PRIO_ITEM, opp_pet[locknum].oPetItemInfo[iCount].bmpNo, 0);
										}
										for (int iCount = 0; iCount < PET_EQUIPNUM; iCount++)
										{
											// ??????????
											if (MakeHitBox( ItemBuffer[iCount].defX - 26, ItemBuffer[iCount].defY - 26,
												ItemBuffer[iCount].defX + 26, ItemBuffer[iCount].defY + 23, DISP_PRIO_IME3) == true)
											{
												if (opp_pet[locknum].oPetItemInfo[iCount].bmpNo > 0)
												{
													iColor = opp_pet[locknum].oPetItemInfo[iCount].color;
													splitPoint = opp_pet[locknum].oPetItemInfo[iCount].memo;
													// ???????
													StockFontBuffer(x + 25, y + 195, FONT_PRIO_FRONT, iColor, opp_pet[locknum].oPetItemInfo[iCount].name,0);
													// ????
													sprintf(moji, "�;ö�(%s)", opp_pet[locknum].oPetItemInfo[iCount].damage);
													StockFontBuffer(x + 25, y + 215, FONT_PRIO_FRONT, iColor, moji, 0);

													// ???????????
													while (1)
													{
														if (strlen(splitPoint) > 28)
														{
															sprintf(moji, "%s", splitPoint);
															moji[28] = NULL;
															if (GetStrLastByte(moji) == 3)
															{
																moji[27] = NULL;
																splitPoint += 27;
															}
															else
															{
																moji[28] = NULL;
																splitPoint += 28;
															}
															StockFontBuffer(x + 25, iY, FONT_PRIO_FRONT, 0, moji, 0);
															iY += 24;
														}
														else
														{
															strcpy(moji, splitPoint);
															StockFontBuffer(x + 25, iY, FONT_PRIO_FRONT, 0, moji, 0);
															break;
														}
													}
												}
											}
										}
									}
									else
	#endif
									{
										//????????Action
										if ( SecondActPet == NULL ) {
											ShowPetNum = atoi(opp_pet[locknum].opp_petgrano) ;
											SecondActPet = MakeAnimDisp( x+215, y+130, ShowPetNum , ANIM_DISP_PET );
										}
										//?????????????
										else if ( SecondActPet != NULL && ShowPetNum != atoi(opp_pet[locknum].opp_petgrano) ) {
											DeathAction(SecondActPet);
											SecondActPet = NULL ;
											ShowPetNum = atoi(opp_pet[locknum].opp_petgrano) ;
											SecondActPet = MakeAnimDisp( x+215, y+130, atoi(opp_pet[locknum].opp_petgrano), ANIM_DISP_PET );
										}

										//opp_petfreename
										//???????????? ( ?? )
										StockDispBuffer(  95 ,  40 , DISP_PRIO_IME3, CG_TRADE_LINE , 0 );
										sprintf( moji,"%s", opp_pet[locknum].opp_petname );
										StockFontBuffer(  55 , 30 , FONT_PRIO_FRONT, 0, moji, 0 );

										if( strcmp( opp_pet[locknum].opp_petfreename, opp_pet[locknum].opp_petname) ){
											sprintf( moji,"[%s]", opp_pet[locknum].opp_petfreename );
											StockFontBuffer(  155 , 30 , FONT_PRIO_FRONT, 6, moji, 0 );
										}

										StockDispBuffer(  55 ,  70 , DISP_PRIO_IME3, CG_TRADE_LV_LINE , 0 );

										sprintf( moji,"%3d",atoi(opp_pet[locknum].opp_petlevel) );
										StockFontBuffer( 55 ,  62 , FONT_PRIO_FRONT, 0, moji, 0 );

	#ifdef _PET_2TRANS
										sprintf( moji,"%s", "" );
										if(	atoi(opp_pet[locknum].opp_pettrans)==1 )
											sprintf( moji,"%s", "һת");
										else if( atoi(opp_pet[locknum].opp_pettrans)==2 )
											sprintf( moji,"%s", "��ת");
		#ifdef _SHOW_FUSION
										if(	LOWORD(atoi(opp_pet[locknum].opp_fusion)) == 1 )
											sprintf( moji,"%s", "�ں�");
		#endif
	#else
										sprintf( moji,"%s", (atoi(opp_pet[locknum].opp_pettrans)==0)?"":"?");
	#endif
										StockFontBuffer( 90 ,  62 , FONT_PRIO_FRONT, 2, moji, 0 );


										StockDispBuffer(  55 , 100 , DISP_PRIO_IME3, CG_TRADE_HP_LINE , 0 );
										sprintf( moji,"%3d", atoi(opp_pet[locknum].opp_petshowhp) );
										StockFontBuffer( 55 , 92 , FONT_PRIO_FRONT, 0, moji, 0 );

										StockDispBuffer(  55 , 130 , DISP_PRIO_IME3, CG_TRADE_ATK_LINE , 0 );
										sprintf( moji, "%3d", atoi(opp_pet[locknum].opp_petatk) );
										StockFontBuffer( 55 , 124 , FONT_PRIO_FRONT, 0, moji, 0 );

										StockDispBuffer(  55 , 160 , DISP_PRIO_IME3, CG_TRADE_DEF_LINE , 0 );
										sprintf( moji, "%3d", atoi(opp_pet[locknum].opp_petdef) );
										StockFontBuffer( 55 , 154 , FONT_PRIO_FRONT, 0, moji, 0 );

										StockDispBuffer(  55 , 190 , DISP_PRIO_IME3, CG_TRADE_DEX_LINE , 0 );
										sprintf( moji, "%3d", atoi(opp_pet[locknum].opp_petquick) );
										StockFontBuffer( 55 , 184 , FONT_PRIO_FRONT, 0, moji, 0 );
										for ( j = 0 ; j < 4 ; j ++ )
											StockDispBuffer( 100 , 220 + j * 30  , DISP_PRIO_IME3, CG_TRADE_SK1_LINE + j , 0 );
										for ( j = 4 ; j < atoi(opp_pet[locknum].opp_petslot) ; j ++ )
											StockDispBuffer( 230 , 220 + ( j - 4 ) * 30 , DISP_PRIO_IME3, CG_TRADE_SK5_LINE + j - 4 , 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill1 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer(  65 , 212   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill2 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer(  65 , 242   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill3 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer(  65 , 272   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill4 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer(  65 , 302   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill5 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer( 195 , 212   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill6 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer( 195 , 242   , FONT_PRIO_FRONT, 0, moji, 0 );
										sprintf( moji, "%s", opp_pet[locknum].opp_petskill7 );
										if ( strcmp ( moji , "(null)" ) != 0 )
											StockFontBuffer( 195 , 272   , FONT_PRIO_FRONT, 0, moji, 0 );
									}
								}
								else if ( SecondActPet != NULL ) {
									DeathAction(SecondActPet);
									SecondActPet = NULL ;
								}
#endif
							}
							//???????????? ( ??? )
							if ( tradeList[ i + 37 ].data != -1 )
							{
								sprintf( moji,"%s", opp_pet[i].opp_petname );
								StockFontBuffer( x + 322 , y + 13 + i * 42 , FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );

								if( strcmp( opp_pet[i].opp_petname, opp_pet[i].opp_petfreename) )
								{
									sprintf( moji,"[%s]", opp_pet[i].opp_petfreename );
									StockFontBuffer( x + 442 , y + 13 + i * 42 , FONT_PRIO_FRONT, 6, moji, 0 );
								}

#ifdef _PET_2TRANS
								sprintf( moji,"%s", "" );
								if( atoi(opp_pet[i].opp_pettrans)==1 )
									sprintf( moji,"%s", "һת" );
								else if( atoi(opp_pet[i].opp_pettrans)==2 )
									sprintf( moji,"%s", "��ת" );
#ifdef _SHOW_FUSION
								if(	LOWORD(atoi(opp_pet[i].opp_fusion)) == 1 )
									sprintf( moji,"%s", "�ں�");
#endif
#else
								sprintf( moji,"%s",  (atoi(opp_pet[i].opp_pettrans)==0)?"":"?" );
#endif
								StockFontBuffer( x + 590 , y + 13 + i * 42 , FONT_PRIO_FRONT, 2, moji, 0 );

								sprintf( moji,"%4d", atoi(opp_pet[i].opp_petshowhp) );
								StockFontBuffer( x + 504 , y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );
								sprintf( moji,"%3d",atoi(opp_pet[i].opp_petlevel) );
								StockFontBuffer( x + 558 , y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );
								sprintf( moji, "%3d", atoi(opp_pet[i].opp_petatk) );
								StockFontBuffer( x + 345 , y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );
								sprintf( moji, "%3d", atoi(opp_pet[i].opp_petdef) );
								StockFontBuffer( x + 398 , y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );
								sprintf( moji, "%3d", atoi(opp_pet[i].opp_petquick) );
								StockFontBuffer( x + 451 , y + 33 + i * 42, FONT_PRIO_FRONT, atoi(opp_pet[i].opp_pettrans), moji, 0 );
							}
						}
						//???????
						for ( i = 0 ; i < 3  ; i ++ )
						{
							for ( j = 0 ; j < 5 ; j ++ )
							{
								if( MakeHitBox( 368 + 51 * j -25 , 265 + 47 * i - 23,
									368 + 51 * j + 25  , 265 + 47 * i + 23 ,  DISP_PRIO_IME3 ) == true )
								{
#ifdef _TRADELOCKBTN				// (?????) Syu ADD ??????????
									if ( mouse.onceState & MOUSE_LEFT_CRICK ) {
										locknum2 = i * 5 + j ;
										locknum = -1 ;
									}
									if ( locknum2 == -1 ) {
										locknum2 = i * 5 + j ;
										locknum = -1 ;
									}
									if ( SecondActPet != NULL ) {
										DeathAction(SecondActPet);
										SecondActPet = NULL ;
									}
#ifdef _PET_ITEM
									bViewPetEquip = false;
#endif
								}
								if ( locknum2 != -1 )
								{
									x = SecondTradeWnd->x ;
									y = SecondTradeWnd->y ;
									if ( tradeList[ locknum2 + 22 ].data != -1 )
									{
										//???????????
										char *splitPoint = opp_item[  locknum2 ].effect ;
										while( 1 )
										{
											if ( y > 40 )
												break;
											if( strlen( splitPoint ) > 28 )
											{
												strncpy( moji, splitPoint, 28 );
												moji[ 28 ] = NULL;
												if( GetStrLastByte( moji ) == 3 )
												{
													moji[ 27 ] = NULL;
													splitPoint += 27;
												}
												else
												{
													moji[ 28 ] = NULL;
													splitPoint += 28;
												}
												StockFontBuffer( 50, y + 160, FONT_PRIO_FRONT, 0, moji, 0 ); y += 40;
											}
											else
											{
												strcpy( moji, splitPoint );
												StockFontBuffer( 50, y + 160, FONT_PRIO_FRONT, 0, moji, 0 );
												break;
											}
										}
										StockDispBuffer( 100 , 70 , DISP_PRIO_IME3, atoi(opp_item[ locknum2 ].graph), 0 );

										sprintf( moji, "%s ", opp_item[  locknum2 ].name );
										StockFontBuffer( 50 , 120 , FONT_PRIO_FRONT, 0, moji, 0 );
										if( strcmp( opp_item[  locknum2 ].name, opp_item[  locknum2 ].freename) )
										{
											sprintf( moji, "[%s]", opp_item[  locknum2 ].freename);
											StockFontBuffer( 50 , 140 , FONT_PRIO_FRONT, 6, moji, 0 );
										}
#ifdef _ITEM_PILENUMS
										sprintf( moji, "x%d", tradeList[locknum2 + 22].pilenum);
#else
										sprintf( moji, "%s", opp_item[  locknum2 ].name);
#endif
										StockFontBuffer( 140 , 80 , FONT_PRIO_FRONT, 0, moji, 0 );


										if (  strcmp ( opp_item[locknum2 ].damage , "" ) != 0  )
											sprintf( moji, "�;öȣ�%s", opp_item[  locknum2 ].damage);
										StockFontBuffer( 50 , 240 , FONT_PRIO_FRONT, 0, moji, 0 );

									}
#endif
								}
								if ( tradeList[ i * 5 + j + 22 ].data != -1 )
								{
									StockDispBuffer( 368 + 51 * j , 265 + 47 * i , DISP_PRIO_IME3, atoi(opp_item[ i * 5 + j ].graph), 0 );
#ifdef _ITEM_PILENUMS
									if( tradeList[i * 5 + j + 22].pilenum > 1 )
									{
										sprintf( moji , "%d" , tradeList[i * 5 + j + 22].pilenum ) ;
										StockFontBuffer( 370 + 51 * j + 10 , 265 + 47 * i +2 , FONT_PRIO_FRONT, 0, moji, 0);
									}
#endif

								}
							}
						}
					}
					if ( SecondTradeWnd != NULL ) {

						if ( SecondTradeWnd->hp > 0 )
						{
							//?????????????
							if ( pActPet3 != NULL )
							{
								DeathAction(pActPet3);
								pActPet3 = NULL ;
							}
							StockDispBuffer( ( ( WINDOW_DISP *)SecondTradeWnd->pYobi )->mx, ( ( WINDOW_DISP *)SecondTradeWnd->pYobi )->my - 25 , DISP_PRIO_MENU, CG_TRADE_VIEWWND, 1 );

							if( mouse.onceState & MOUSE_LEFT_CRICK )
							{
								if( HitDispNo == SecondtradeWndFontNo[ 0 ] ) {
									//?????????
									MenuToggleFlag ^= JOY_CTRL_T;
									//????????��
									play_se( 203, 320, 240 );
									sprintf(buffer, "W|%s|%s", opp_sockfd, opp_name);
									//??????????Server?????
									lssproto_TD_send( sockfd, buffer);
									tradeStatus = 0;
									tradeInit();
									pc.trade_confirm = 1;
								}
								else if( HitDispNo == SecondtradeWndFontNo[ 1 ] )
								{
									MainTradeWndflag = true ;
									SecondTradeWndflag = false ;
									//andy_add 20030610
									for( i = 0 ; i < MENU_TRADE_0 ; i++ ) tradeWndFontNo[ i ] = -2;
									for( i = 0 ; i < 6 ; i++ ) SecondtradeWndFontNo[ i ] = -2;
#ifdef _PET_ITEM
									InitItem3( 325, 230 );
									bViewPetEquip = false;
#endif
								}
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
								else if( HitDispNo == SecondtradeWndFontNo[ 4 ] )
									talkwndflag = true ;
#endif
#ifdef _PET_ITEM
								else if (HitDispNo == SecondtradeWndFontNo[5])
								{
									bViewPetEquip = !bViewPetEquip;
									InitItem(SecondTradeWnd->x, SecondTradeWnd->y + 20, bViewPetEquip);
								}
#endif
								else
									LockAndOkfunction ();
							}
#ifdef _TRADETALKWND				// (?????) Syu ADD ??????????????
							if ( talkwndflag == true )
							{
								talkwndx = mouse.nowPoint.x ;
								talkwndy = mouse.nowPoint.y ;
							}
							if( mouse.onceState & MOUSE_LEFT_CRICK_UP )
								talkwndflag = false ;
							if ( mouse.onceState & MOUSE_RIGHT_CRICK )
								tradetalkwndflag = false ;
#endif
						}
					}
				}

#endif
			}
			break;
		}
	}
	else if( tradeStatus != 0 )
	{
		char buffer[1024] = "";
		tradeStatus = 0;
		sprintf(buffer, "W|%s|%s", opp_sockfd, opp_name);
		lssproto_TD_send( sockfd, buffer);
		tradeInit();
		pc.trade_confirm = 1;
	}

	// ?????????????????


	// show Bank Window
	if( MenuToggleFlag & JOY_B ){

		char buffer[1024];
		int x, y ,w ,h;

		if( checkPcWalkFlag() == 1 ) closeBankman();


		if( joy_trg[ 0 ] & JOY_ESC
			|| actBtn == 1
			|| menuBtn == 1
			|| disconnectServerFlag == true
			)
		{
			closeBankman();
			return;
		}

		// ?????????
		if( pActMenuWnd5 == NULL ){
			w=270; h=160;
			x = (640-w)/2;
			y = (480-h)/2;

			pActMenuWnd5 = MakeWindowDisp( x, y, w, h, NULL, -1 );
			play_se( 202, 320, 240 );	// ????????

		}else{
			// ????????????????
			if( pActMenuWnd5->hp > 0 ){

				if( mouse.onceState & MOUSE_LEFT_CRICK ){

					// ?????
					if( HitDispNo == bankWndFontNo[ 0 ] ){
						closeBankman();
						return;
					}

					if( HitDispNo == bankWndFontNo[ 1 ] ) {
						sprintf(buffer, "B|G|%d", pc.gold - cashGold);
						if( bNewServer)
							lssproto_FM_send( sockfd, buffer);
						else
							old_lssproto_FM_send( sockfd, buffer);
      						closeBankman();
									return;
					}

				}

				// Add Gold
				if( HitDispNo == bankWndFontNo[ 2 ] ){

					if( mouse.onceState & MOUSE_LEFT_CRICK_UP && bankWndBtnFlag[ 2 ] == true ){

						bankWndBtnFlag[ 2 ] = false;
					}

					if( ( cashGold < CHAR_getMaxHaveGold() )&&( totalGold-cashGold > 0 ) ) {

						if( bankWndBtnFlag[ 2 ] == true ){

							cashGold += bankGoldInc;
							bankGoldCnt++;

							// ???????
							if( bankGoldCnt >= 30 ){
								// ?????????????????
								bankGoldCnt = 0;
								// ????????
								if( bankGoldInc == 0 ) {
									bankGoldInc = 1;
								}else{
									// ???????
									bankGoldInc *= 5;
									// ????????
									if( bankGoldInc > 10000 ) {
										bankGoldInc = 10000;
									}
								}
							}
							// ????????
							if( cashGold >= CHAR_getMaxHaveGold() ) {
								cashGold = CHAR_getMaxHaveGold();
								play_se( 220, 320, 240 );
							}
							if( cashGold >= totalGold ) {
								cashGold = totalGold;
								play_se( 220, 320, 240 );
							}

						}

						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ) {
							// ???
							cashGold++;
							// ????????
							if( cashGold >= CHAR_getMaxHaveGold() ) {
								cashGold = CHAR_getMaxHaveGold();
								// ???
								play_se( 220, 320, 240 );
							} else {
								// ?????????
								bankGoldInc = 0;
								// ?????????????????
								bankGoldCnt = 0;
								// ???????
								bankWndBtnFlag[ 2 ] = true;
								// ?????
								play_se( 217, 320, 240 );
							}

						}
					}
				}else{
					// ????????
					bankWndBtnFlag[ 2 ] = false;
				}
				// ??????????
				if( HitDispNo == bankWndFontNo[ 3 ] ){
					// ???????????????
					if( mouse.onceState & MOUSE_LEFT_CRICK_UP && bankWndBtnFlag[ 3 ] == true ){
						bankWndBtnFlag[ 3 ] = false;
						// ????????
						//play_se( 212, 320, 240 );
					}
					if( ( cashGold > 0 )&&( totalGold-cashGold < MAX_BANKGOLD ) ) {
						// ??????
						if( bankWndBtnFlag[ 3 ] == true ){

							// ????
							cashGold -= bankGoldInc;
							// ?????????????????
							bankGoldCnt++;

							// ???????
							if( bankGoldCnt >= 30 ){
								// ?????????????????
								bankGoldCnt = 0;
								// ????????
								if( bankGoldInc == 0 ) {
									bankGoldInc = 1;
								}else{
									// ???????
									bankGoldInc *= 5;
									// ????????
									if( bankGoldInc > 10000 ) {
										bankGoldInc = 10000;
									}
								}
							}
							// ????????
							if( cashGold <= 0 ){
								cashGold = 0;
								play_se( 220, 320, 240 );
							}
							if( totalGold - cashGold >= MAX_BANKGOLD ){
								cashGold = totalGold - MAX_BANKGOLD;
								play_se( 220, 320, 240 );
							}

						}
						// ????????
						if( mouse.onceState & MOUSE_LEFT_CRICK ) {

							cashGold--;

							if( cashGold <= 0 ){
								cashGold = 0;
								// ???
								play_se( 220, 320, 240 );
							}
							else if( totalGold - cashGold >= MAX_BANKGOLD ) {
								cashGold = totalGold - MAX_BANKGOLD;
								play_se( 220, 320, 240 );
							}else{
								// ?????????
								bankGoldInc = 0;
								// ?????????????????
								bankGoldCnt = 0;
								// ???????
								bankWndBtnFlag[ 3 ] = true;
								// ?????
								play_se( 217, 320, 240 );

							}

						}
					}
					else {
						bankGoldInc = 0;
						bankGoldCnt = 0;
					}

				}else{
					// ????????
					bankWndBtnFlag[ 3 ] = false;
				}
				// ?????????
				for(int i=2;i<=8;i++) {
					if( mouse.state & MOUSE_LEFT_CRICK && tradeWndBtnFlag[ i ] == true ){
						// ????
						tradeWndBtnFlag[ i ] = true;
					}else{
						// ?????
						tradeWndBtnFlag[ i ] = false;
					}
				}
				if( pActMenuWnd5 != NULL ) {
					// ????@?e
					x = pActMenuWnd5->x ;
					y = pActMenuWnd5->y ;

					bankWndFontNo[ 0 ] = StockDispBuffer( x+200, y+133, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2 );
					bankWndFontNo[ 1 ] = StockDispBuffer( x+75, y+133, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2 );
					bankWndFontNo[ 2 ] = StockDispBuffer( x+140, y+70, DISP_PRIO_IME3, CG_UP_BTN + bankWndBtnFlag[ 2 ], 2 );
					bankWndFontNo[ 3 ] = StockDispBuffer( x+200, y+70, DISP_PRIO_IME3, CG_DOWN_BTN + bankWndBtnFlag[ 3 ], 2 );
					sprintf(moji, "%7d", cashGold);
					StockFontBuffer(x+180, y+40, FONT_PRIO_FRONT, 0, moji, 0);
					sprintf( moji,"%7d", totalGold - cashGold);
					StockFontBuffer(x+180, y+86, FONT_PRIO_FRONT, 0, moji, 0 );
					StockDispBuffer( 320, 240, DISP_PRIO_IME3, CG_FAMILY_BANK_WIN, 1 );
				}
			}
		}
	}

#ifdef _STANDBYPET
	if( StandbyPetSendFlag == false ){
		int s_pet = 0;
		int i;

		for(i =0; i <MAX_PET; i++) {
			if( pc.selectPetNo[i] )
				s_pet |= ( 1 << i );
		}
		if( s_pet != pc.standbyPet )
			send_StandBy_Pet();
	}
#endif


}

#ifdef _NEWREQUESTPROTOCOL			// (?????) Syu ADD ????Protocol??????
void lssproto_RESIST_recv( int fd, char *data)
{
	for ( int  i = 0 ; i < CHAR_MAX_DETAIL ; i ++ )
		getStringToken( data, '|', i + 1 , sizeof( CharDetail[i] ) - 1 , CharDetail[i] );
}
#endif

#ifdef _ALCHEPLUS
void lssproto_ALCHEPLUS_recv( int fd, char *data)
{
	char token[64];
	int i;
	for ( i = 0 ; i < 25 ; i ++ ) {
		getStringToken( data, '|', i + 1 , sizeof( token ) - 1 , token );
		iCharAlchePlus[i] = atoi( token);
	}
}
#endif

#ifdef _OUTOFBATTLESKILL			// (?????) Syu ADD ??????????Protocol
void lssproto_BATTLESKILL_recv ( int fd, char *data) {
	setCharMind( pc.ptAct, atoi( data ) );
}
#endif

void lssproto_TD_recv( int fd, char *data)
{
	char Head[2] = "";
	char buf_sockfd[128] ="";
	char buf_name[128] ="";
	char buf[128] = "";
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
	char opp_index[128];
	int  index ;
	char realname[256];
	char freename[256];
#endif
	getStringToken( data, '|', 1, sizeof( char ), Head);
	// ???????????????
	if( strcmp(Head,"C") == 0 ) {
		if( /*(tradeFlag==0) ||*/ (tradeStatus!=0) )
			return;

		strcpy( opp_sockfd,"");
		strcpy( opp_name,"");
		strcpy( trade_command,"");

		getStringToken( data, '|', 2, sizeof( opp_sockfd )-1, opp_sockfd );
		getStringToken( data, '|', 3, sizeof( opp_name )-1, opp_name );
		getStringToken( data, '|', 4, sizeof( trade_command )-1, trade_command );

		if( strcmp( trade_command,"0" ) == 0) {
			return;
		}
		else if( strcmp(trade_command,"1") == 0) {
			tradeStatus = 1;
			MenuToggleFlag = JOY_CTRL_T;
// shan trade(DoubleCheck)
	        pc.trade_confirm = 1;
		}
	}
		//??????????????????
	else if( strcmp(Head,"T") == 0 ) {

		if( tradeStatus == 0 )	return;
		char buf_showindex[128];

		//andy_add mttrade
		getStringToken( data, '|', 4, sizeof( trade_kind)-1, trade_kind);
		if( strcmp(trade_kind,"S") == 0 ) {
			char buf1[256];
			int objno=-1, showno=-1;
			if( pActMenuWnd4 == NULL ){
				DeathMenuAction();
				DeathMenuAction2();
				pActMenuWnd4 = MakeWindowDisp( 10, 0, 620, 456, 0, -1 );
				InitItem3( 325, 230 );
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
				if ( SecondTradeWnd == NULL )
					SecondTradeWnd = MakeWindowDisp( 10, 0, 620, 456, 0, -1 );
#endif
			}
			getStringToken( data, '|', 6, sizeof( buf1)-1, buf1);
			objno = atoi( buf1);
			getStringToken( data, '|', 7, sizeof( buf1)-1, buf1);
			showno = atoi( buf1);
			getStringToken( data, '|', 5, sizeof( buf1)-1, buf1);

			ItemBuffer[ objno ].mixFlag = false;
			if( pc.item[ objno ].useFlag == true ){
				ItemBuffer[ objno ].dragFlag = true;
				mouse.itemNo = objno;
			}

			if( !strcmp( buf1, "I") )	{	//I
			}else {	//P
				tradePetIndex = objno;
				tradePet[0].index = objno;

				if( pet[ objno ].useFlag &&  pc.ridePetNo != objno ){
					if( pet[ objno ].freeName[0] != NULL )
						strcpy(tradePet[0].name ,pet[ objno ].freeName);
					else
						strcpy(tradePet[0].name ,pet[ objno ].name);
					tradePet[0].level = pet[ objno ].level;
					tradePet[0].atk = pet[ objno ].atk;
					tradePet[0].def = pet[ objno ].def;
					tradePet[0].quick = pet[ objno ].quick;
					tradePet[0].graNo = pet[ objno ].graNo;

					showindex[3] = 3;
					DeathAction( pActPet4 );
					pActPet4 = NULL;
				}
			}

			mouse.itemNo = -1;

			return;
		}

		getStringToken( data, '|', 2, sizeof( buf_sockfd )-1, buf_sockfd );
		getStringToken( data, '|', 3, sizeof( buf_name )-1, buf_name );
		getStringToken( data, '|', 4, sizeof( trade_kind )-1, trade_kind );
		getStringToken( data, '|', 5, sizeof( buf_showindex )-1, buf_showindex );
		opp_showindex = atoi( buf_showindex );

		if( (strcmp(buf_sockfd,opp_sockfd)!=0) || (strcmp(buf_name,opp_name)!=0) )
			return;

		if( strcmp(trade_kind,"G") == 0 ) {

			getStringToken( data, '|', 6, sizeof( opp_goldmount )-1, opp_goldmount );
			int mount = atoi(opp_goldmount);
#ifdef _CHANGETRADERULE		   // (?????) Syu ADD ??????????
			if ( tradeWndDropGoldGet != 0 ) {
				MenuToggleFlag ^= JOY_CTRL_T;
				play_se( 203, 320, 240 );
				sprintf(buf, "W|%s|%s", opp_sockfd, opp_name);
				if( bNewServer)
					lssproto_TD_send( sockfd, buf);
				else
					old_lssproto_TD_send( sockfd, buf);
				sprintf(buf, "%s��������ʽ�޸Ľ��׽�Ǯ��ϵͳǿ�ƹرս����Ӵ���" , opp_name ) ;
				StockChatBufferLine(  buf , FONT_PAL_RED);
				return ;
			}
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
			//andy_reEdit
				if( mount != -1 ) {
					tradeList[42].kind = 'G' ;
					tradeList[42].data = mount ;
					tradeWndDropGoldGet = mount;
				}else {
					tradeList[42].data = 0;
					tradeWndDropGoldGet = 0;
				}
			/*
			if( opp_showindex == 3 ) {
				if( mount != -1 ) {
					tradeList[42].kind = 'G' ;
					tradeList[42].data = mount ;
					tradeWndDropGoldGet = mount;
				}else {
					tradeList[42].data = 0;
					tradeWndDropGoldGet = 0;
				}
			}else
			*/

#else
			if( opp_showindex == 1 ) {
				if( mount != -1 ) {
					showindex[4] = 2;
					tradeWndDropGoldGet = mount;
				}else {
					showindex[4] = 0;
					tradeWndDropGoldGet = 0;
				}
			}
			else if( opp_showindex == 2 ) {
				if( mount != -1 ) {
					showindex[5] = 2;
					tradeWndDropGoldGet = mount;
				}else {
					showindex[5] = 0;
					tradeWndDropGoldGet = 0;
				}
			}else return;
#endif

		}

		if( strcmp(trade_kind,"I") == 0 ) {
			char pilenum[256], item_freename[256],itemup[64];

			getStringToken( data, '|', 6, sizeof( opp_itemgraph )-1, opp_itemgraph );

			getStringToken( data, '|', 7, sizeof( opp_itemname )-1, opp_itemname );
			getStringToken( data, '|', 8, sizeof( item_freename )-1, item_freename );

			getStringToken( data, '|', 9, sizeof( opp_itemeffect )-1, opp_itemeffect );
			getStringToken( data, '|',10, sizeof( opp_itemindex )-1, opp_itemindex );
			getStringToken( data, '|',11, sizeof( opp_itemdamage )-1, opp_itemdamage );// ??????????

#ifdef _ITEM_PILENUMS
			getStringToken( data, '|',12, sizeof( pilenum )-1, pilenum );//pilenum
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
			if( strcmp(opp_itemgraph,"-1") ==0 )
				return ;
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].name, makeStringFromEscaped(opp_itemname) );
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].freename, makeStringFromEscaped(item_freename) );
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].graph, opp_itemgraph);
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].effect, makeStringFromEscaped(opp_itemeffect) );
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].itemindex, opp_itemindex);
			strcpy( opp_item[atoi(opp_itemindex)-MAX_ITEMSTART].damage, makeStringFromEscaped(opp_itemdamage) );

			tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].kind = 'I' ;
			tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].data = atoi(opp_itemindex) ;
			strcpy ( tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].name , makeStringFromEscaped(opp_itemname) ) ;

			strcpy ( tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].freename , makeStringFromEscaped(item_freename) ) ;

			strcpy ( tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].damage , makeStringFromEscaped(opp_itemdamage) ) ;
#ifdef _ITEM_PILENUMS
			tradeList[atoi(opp_itemindex) -MAX_ITEMSTART + 22].pilenum = atoi( pilenum);
#endif
#endif  //_TRADESYSTEM2
		}

		if( strcmp(trade_kind,"P") == 0 ) {
#ifdef _PET_ITEM
			int		iItemNo;
			char	szData[256];
#endif

#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
			getStringToken( data, '|', 12, sizeof( opp_index )-1, opp_index );
			index = atoi ( opp_index ) ;
			getStringToken( data, '|', 6, sizeof( opp_pet[index].opp_petgrano )-1, opp_pet[index].opp_petgrano );
			getStringToken( data, '|', 7, sizeof( opp_pet[index].opp_petname )-1, opp_pet[index].opp_petname );
			getStringToken( data, '|', 8, sizeof( opp_pet[index].opp_petlevel )-1, opp_pet[index].opp_petlevel );
			getStringToken( data, '|', 9, sizeof( opp_pet[index].opp_petatk )-1, opp_pet[index].opp_petatk );
			getStringToken( data, '|', 10, sizeof( opp_pet[index].opp_petdef )-1, opp_pet[index].opp_petdef );
			getStringToken( data, '|', 11, sizeof( opp_pet[index].opp_petquick )-1, opp_pet[index].opp_petquick );
			getStringToken( data, '|', 12, sizeof( opp_pet[index].opp_petindex )-1, opp_pet[index].opp_petindex );
			getStringToken( data, '|', 13, sizeof( opp_pet[index].opp_pettrans )-1, opp_pet[index].opp_pettrans );
			getStringToken( data, '|', 14, sizeof( opp_pet[index].opp_petshowhp )-1, opp_pet[index].opp_petshowhp );
			getStringToken( data, '|', 15, sizeof( opp_pet[index].opp_petslot )-1, opp_pet[index].opp_petslot );
			getStringToken( data, '|', 16, sizeof( opp_pet[index].opp_petskill1 )-1, opp_pet[index].opp_petskill1 );
			getStringToken( data, '|', 17, sizeof( opp_pet[index].opp_petskill2 )-1, opp_pet[index].opp_petskill2 );
			getStringToken( data, '|', 18, sizeof( opp_pet[index].opp_petskill3 )-1, opp_pet[index].opp_petskill3 );
			getStringToken( data, '|', 19, sizeof( opp_pet[index].opp_petskill4 )-1, opp_pet[index].opp_petskill4 );
			getStringToken( data, '|', 20, sizeof( opp_pet[index].opp_petskill5 )-1, opp_pet[index].opp_petskill5 );
			getStringToken( data, '|', 21, sizeof( opp_pet[index].opp_petskill6 )-1, opp_pet[index].opp_petskill6 );
			getStringToken( data, '|', 22, sizeof( opp_pet[index].opp_petskill7 )-1, opp_pet[index].opp_petskill7 );
			getStringToken( data, '|', 23, sizeof( realname ), realname );
			getStringToken( data, '|', 24, sizeof( freename ), freename );
#ifdef _SHOW_FUSION
			getStringToken( data, '|', 25, sizeof( opp_pet[index].opp_fusion )-1, opp_pet[index].opp_fusion );
#endif
			strcpy( opp_pet[ index].opp_petname, realname);
			strcpy( opp_pet[ index].opp_petfreename, freename);
#endif

#ifdef _PET_ITEM
			for (int i = 0;; i++){
				if (getStringToken(data, '|', 26 + i * 6, sizeof(szData), szData))
					break;
				iItemNo = atoi(szData);
				getStringToken(data, '|', 27 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].name), opp_pet[index].oPetItemInfo[iItemNo].name);
				getStringToken(data, '|', 28 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].memo), opp_pet[index].oPetItemInfo[iItemNo].memo);
				getStringToken(data, '|', 29 + i * 6, sizeof(opp_pet[index].oPetItemInfo[iItemNo].damage), opp_pet[index].oPetItemInfo[iItemNo].damage);
				getStringToken(data, '|', 30 + i * 6, sizeof(szData), szData);
				opp_pet[index].oPetItemInfo[iItemNo].color = atoi(szData);
				getStringToken(data, '|', 31 + i * 6, sizeof(szData), szData);
				opp_pet[index].oPetItemInfo[iItemNo].bmpNo = atoi(szData);
			}
#endif


#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
			tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].data = atoi(opp_pet[index].opp_petindex) ;
			tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].kind = 'P' ;
			strcpy ( tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].freename , freename) ;
			strcpy ( tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].name , realname ) ;
			tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].level = atoi ( opp_pet[index].opp_petlevel ) ;
			tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].trns = atoi(opp_pet[index].opp_pettrans) ;
#ifdef _SHOW_FUSION
			tradeList[ atoi(opp_pet[index].opp_petindex)+37 ].fusion = atoi(opp_pet[index].opp_fusion) ;
#endif
#endif

			if( opp_showindex == 3 ) {
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
				if( strcmp(opp_pet[atoi(opp_index)].opp_petgrano,"-1") ==0 ) {
#endif
					showindex[6] = 0;
				}else {
					showindex[6] = 3;
				}
				DeathAction( pActPet5 );
				pActPet5 = NULL;
			}
		}

// shan trade(DoubleCheck) begin
		if( strcmp( trade_kind, "C" ) == 0 ) {
			if(pc.trade_confirm==1) pc.trade_confirm = 3;
			if(pc.trade_confirm==2)	pc.trade_confirm = 4;
		}
// end

		if( strcmp( trade_kind, "A" ) == 0 )
			tradeStatus = 2;

	}else if( strcmp(Head,"W") == 0 ) {//???????
		getStringToken( data, '|', 2, sizeof( buf_sockfd )-1, buf_sockfd );
		getStringToken( data, '|', 3, sizeof( buf_name )-1, buf_name );
		if( (strcmp(buf_sockfd,opp_sockfd)==0) && (strcmp(buf_name,opp_name)==0) ) {
			tradeStatus = 0;
			MenuToggleFlag ^= JOY_CTRL_T;
			tradeInit();
// shan trade(DoubleCheck)
	        pc.trade_confirm = 1;
			play_se( 203, 320, 240 );

		}
	}
}

void tradeInit( void )
{
#ifdef _TRADESYSTEM2	// (?????) Syu ADD ???????
	//?????????Action
	DeathAction( SecondTradeWnd );
	SecondTradeWnd = NULL ;
	DeathAction( SecondActPet ) ;
	SecondActPet = NULL ;
	DeathAction ( TradeTalkWnd ) ;
	TradeTalkWnd = NULL ;
//Terry add 2003/11/25
	g_bTradesystemOpen = false;
//end
#endif
	DeathAction( pActMenuWnd4 );
	// ???????????????focus????chat
	GetKeyInputFocus( &MyChatBuffer );
	Tradeflag = false;
	pActMenuWnd4 = NULL;
	DeathAction( pActPet3 );
	pActPet3 = NULL;
	DeathAction( pActPet4 );
	pActPet4 = NULL;
	DeathAction( pActPet5 );
	pActPet5 = NULL;
	for(int i=0;i<=6;i++)
		showindex[i] = 0;
	strcpy( opp_sockfd, "-1");
	strcpy( opp_name, "");
	tradePetIndex = 0;
	strcpy( tradepetindexget, "-1");
	mine_itemindex[0] = -1;
	mine_itemindex[1] = -1;


	tradeWndDropGoldSend = 0;
	tradeWndDropGoldGet = 0;
	tradeWndDropGold = 0;
	strcpy( opp_item[0].itemindex, "-1");
	strcpy( opp_item[1].itemindex, "-1");
}

void BankmanInit( char *data )
{
	char token1[1024];
	getStringToken( data, '|', 3, sizeof( token1 )-1, token1 );

	bankGold = atoi( token1 );
	cashGold = pc.gold;
	totalGold = bankGold + cashGold;

	MenuToggleFlag = JOY_B;
	DeathAction( pActMenuWnd5 );
	pActMenuWnd5 = NULL;
}

void closeBankman( void ) {

	MenuToggleFlag ^= JOY_B;
	play_se( 203, 320, 240 );
	DeathAction( pActMenuWnd5 );
	pActMenuWnd5 = NULL;

}

void checkRidePet(int pindex)
{
	int j;
#ifdef _PET_ITEM
	bool	bHavePetItem = false;
#endif

	if (!bNewServer)
		return;
#ifdef _PET_ITEM
	// ?????????????????
	for (j = 0; j < MAX_PET_ITEM; ++j){
		if (pet[pindex].item[j].useFlag){	// ??????????
			bHavePetItem = true;
			break;
		}
	}
#endif
	if( pc.ridePetNo < 0
		&& pc.learnride >= pet[pindex].level //Change fix ?????????? 20050801??
		&& (pc.level + 5) > pet[pindex].level
		&& pet[pindex].ai >= 100
		&& pc.graNo != SPR_pet021
		&& pc.graNo != 100362 //???
#ifdef _PETSKILL_BECOMEPIG // ???????��?????
		&& pc.graNo != 100250
#endif

#ifdef _PET_2TRANS // ????�M????
		&& pet[pindex].trn <= 1
#endif
		)
	{
		pc.ridePetNo = -1;
		for (j = 0; j < sizeof(ridePetTable) / sizeof(tagRidePetTable); j++){
			int baseimageNo = pc.graNo - (pc.graNo % 5);
			int leaderimageNo = 100700 + ((baseimageNo - 100000) / 20) * 10 + (pc.familySprite) * 5;
			//andy_edit
			if (((ridePetTable[j].charNo == pc.graNo) || (ridePetTable[j].charNo == pc.baseGraNo)) && ridePetTable[j].petNo == pet[pindex].graNo || true){
				char buf[64];
#ifdef _PET_ITEM
				if (bHavePetItem){	// ???????????
					StockChatBufferLine("����������װ��������ˣ�", FONT_PAL_YELLOW);
					pc.selectPetNo[pindex] = 0;
					return;
				}
#endif
				sprintf(buf, "R|P|%d", pindex);
				if (bNewServer)
					lssproto_FM_send(sockfd, buf);
				else
					old_lssproto_FM_send(sockfd, buf);
				pc.ridePetNo = pindex;
				return;
			}
#ifdef _LEADERRIDE
			if (ridePetTable[j].charNo == leaderimageNo && ridePetTable[j].petNo == pet[pindex].graNo &&
				pc.big4fm != 0 && pc.familyleader != FMMEMBER_APPLY && pc.familyleader != FMMEMBER_NONE){
				char buf[64];
#ifdef _PET_ITEM
				if (bHavePetItem){	// ???????????
					StockChatBufferLine("����������װ��������ˣ�??", FONT_PAL_YELLOW);
					pc.selectPetNo[pindex] = 0;
					return;
				}
#endif
				sprintf(buf, "R|P|%d", pindex);
				if (bNewServer)
					lssproto_FM_send(sockfd, buf);
				else
					old_lssproto_FM_send(sockfd, buf);
				pc.ridePetNo = pindex;
				return;
			}
#endif
		}
		{//andy_add ?????
			int ti = -1, index;
			unsigned int LRCode = 1<<30;
			if ((ti = RIDEPET_getPETindex(pet[pindex].graNo, pc.lowsride)) < 0)
			{
			}
			if ((index = RIDEPET_getNOindex(pc.baseGraNo)) >= 0){
				char buf[64];
				if (RIDEPET_getRIDEno(index, ti) >= 0){
#ifdef _PET_ITEM
					if (bHavePetItem){	// ???????????
						StockChatBufferLine("����������װ��������ˣ�??", FONT_PAL_YELLOW);
						pc.selectPetNo[pindex] = 0;
						return;
					}
#endif
					sprintf(buf, "R|P|%d", pindex);
					if (bNewServer)
						lssproto_FM_send(sockfd, buf);
					else
						old_lssproto_FM_send(sockfd, buf);
					pc.ridePetNo = pindex;
					return;
				}
			}
		}
	}
}
//andy_add 2002/06/24
int RIDEPET_getNOindex( int baseNo)
{
	int i;
	for( i=0; i< sizeof(RPlistMode)/sizeof(tagRidePetList); i++ )	{
		if( RPlistMode[i].charNo == baseNo )
			return RPlistMode[i].Noindex;
	}
	return -1;
}
//andy_add 2002/06/24
int RIDEPET_getPETindex( int PetNo, int learnCode)
{
	int i;
	for( i=0; i< sizeof(RideCodeMode)/sizeof(tagRideCodeMode); i++ ){
		if( RideCodeMode[i].petNo == PetNo &&
			(RideCodeMode[i].learnCode & learnCode) )
			return i;
	}
	return -1;
}

//andy_add 2002/06/24
int RIDEPET_getRIDEno( int index, int ti)
{
	if( index < 0 || index >= sizeof( RideNoList)/sizeof( tagRideNoList) )
		return -1;
	if( ti < 0 || ti >= MAXNOINDEX )
		return -1;
	return RideNoList[index].RideNo[ti];
}

#ifdef _TELLCHANNEL				//ROG ADD ???????
void InitSelectChar( char *msg, bool endFlag )
{
	int turn,level;
	char name1[32];
	if(MultiTells == true)
		DeathTellChannel();

	if(!endFlag){
#ifndef _CHANNEL_MODIFY
		index[CharNum] = getIntegerToken( msg, '|', 2 );
		turn = getIntegerToken( msg, '|', 3 );
		level = getIntegerToken( msg, '|', 4 );
		getStringToken( msg, '|', 5 , sizeof(name[CharNum]) -1 ,name[CharNum] );
		getStringToken( msg, '|', 6 , sizeof(name1) -1 ,name1 );
#else
		menuIndex[CharNum] = getIntegerToken( msg, '|', 3 );
		turn = getIntegerToken( msg, '|', 4 );
		level = getIntegerToken( msg, '|', 5 );
		getStringToken( msg, '|', 6 , sizeof(name[CharNum]) -1 ,name[CharNum] );
		getStringToken( msg, '|', 7 , sizeof(name1) -1 ,name1 );
#endif
		sprintf(TellInfo[CharNum]," %15s %15s  %5d  %5d   ",name[CharNum],name1,turn,level);
		CharNum ++;
	}
	else{
#ifndef _CHANNEL_MODIFY
		getStringToken( msg, '|', 2 , sizeof(reSendMsg) -1 ,reSendMsg );
#else
		getStringToken( msg, '|', 3 , sizeof(reSendMsg) -1 ,reSendMsg );
#endif
		MultiTells = true;
	}
//test//////////////
#ifdef _TIMEBAR_FUNCTION
	StartTime = timeGetTime();
	timBarIdent = SetTimeBar("aaaaaa",50);
#endif
//////////////////
}

void SelectChar(void)
{
	char tmpMsg[1024];
	int x,y;
	if(pActMsgWnd == NULL){
			pActMsgWnd = MakeWindowDisp( 80, 200, 6, CharNum / 2 + 1 , 2, 0 );
	}
	else if( pActMsgWnd->hp > 0){
		x = pActMsgWnd->x;
		y = pActMsgWnd->y;
		char title[] = {"           �� ��           �� ��  ת ��  �� �� "};
		StockFontBuffer( x + 10, y + 15, FONT_PRIO_FRONT,4 , title, 0 );
		for(int i = 0 ; i < CharNum ; i++ ){
			StockFontBuffer( x + 10, y + 40 + i * 20, FONT_PRIO_FRONT,5 , TellInfo[i], 0 );
   			if( MakeHitBox(x + 10 , y + 37 + i * 20 , x + 360 , y + 56 + i * 20, DISP_PRIO_BOX2 )){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					sprintf(tmpMsg,"%s %s /T%d  ", name[i], reSendMsg, menuIndex[i]);
					TalkMode = 1;
					chatStrSendForServer(tmpMsg,0);
					DeathTellChannel();
				}
			}
		}
	}

//test//////////////
#ifdef _TIMEBAR_FUNCTION
	int Now = timeGetTime();
	Now -= StartTime;
	int rsut = SetTimeBarPos(timBarIdent,Now / 1000 );
	if( rsut == 1)
		StartTime += 50000;
	else if( rsut == -1){
		DeathAction(pTimeBarWnd);
		pTimeBarWnd = NULL;
	}

#endif
////////////////////
}

void DeathTellChannel(void)
{
		DeathAction(pActMsgWnd);
		pActMsgWnd = NULL;
		MultiTells = false;
		CharNum = 0;
}

#endif

#ifdef _FRIENDCHANNEL				//ROG ADD ???????

void initSetRoomName()
{
	DeathMenuAction();
	DeathMenuAction2();
	DeathAction(pSetRoomWnd);
	pSetRoomWnd = NULL;
	GetKeyInputFocus( &MyChatBuffer );
	setRoomFlag = true;
	SelRoomBtn = 0;
}

void setRoomName(void)
{
	static int setRoomBtn[2];				//?څ????????????

	if(pSetRoomWnd == NULL){
			pSetRoomWnd = MakeWindowDisp( 270, 0, 3, 2, NULL, 0 );
			play_se( 202, 320, 240 );
			chatRoomName.buffer[ 0 ] = NULL;
			chatRoomName.cnt = 0;
			chatRoomName.cursor=0;
			chatRoomName.len = 16;
			chatRoomName.color = 0;
			chatRoomName.x = pSetRoomWnd->x + 22;
			chatRoomName.y = pSetRoomWnd->y + 35;
			chatRoomName.fontPrio = FONT_PRIO_FRONT;
	}
	else if(pSetRoomWnd->hp > 0 ){
		int x = pSetRoomWnd->x;
		int y = pSetRoomWnd->y;

		setRoomBtn[0] = StockDispBuffer( pSetRoomWnd->x + 53, pSetRoomWnd->y + 70, DISP_PRIO_IME3, CG_OK_BTN, 2 );
		setRoomBtn[1] = StockDispBuffer( pSetRoomWnd->x + 140, pSetRoomWnd->y + 70, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );

		char title[] = {"������Ƶ������"};
		StockFontBuffer( x + 22, y + 10, FONT_PRIO_FRONT,4 , title, 0 );
		GetKeyInputFocus( &chatRoomName );
		StockFontBuffer2( &chatRoomName );

		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			if( HitDispNo == setRoomBtn[0] )
				KeyboardReturn();
			else if( HitDispNo == setRoomBtn[1] ){
				GetKeyInputFocus( &MyChatBuffer );
#ifdef _CHATROOMPROTOCOL
				lssproto_CHATROOM_send ( sockfd , "B|" ) ;
#endif
			 	DeathAction(pSetRoomWnd);
				pSetRoomWnd = NULL;
				setRoomFlag = false;
			}
		}
	}
}

void InitSelChatRoom(char *msg)
{
	char tempRoomNum[32],RoomName[32];
	int memberNum = 0;
	char chiefName[128] = {""};
	char temp[16],*temp1;
	int i = 2, k=0;
	roomNum = 0;

	while( getStringToken( msg, '|', i , sizeof(tempRoomNum) -1 ,tempRoomNum )!=1){
		if( tempRoomNum[0] == 'r' ){
			temp1 = tempRoomNum;
			temp1++;
			roomNum = atoi( temp1);

			getStringToken( msg, '|', i+1 , sizeof(RoomName) -1 ,RoomName );
			getStringToken( msg, '|', i+3 , sizeof(chiefName) -1 ,chiefName );
			getStringToken( msg, '|', i+4 , sizeof(temp) -1 ,temp );

			if( (temp1 = strtok(temp,"p"))!= NULL )
				memberNum = atoi(temp1);

			roomIndex[k] = roomNum;
			sprintf(roomInfo[k],"  %16s  %16s   %2d ",RoomName,chiefName,memberNum);
			k++;
			roomNum ++;
			i += 5;
		}
		else
			break;
	}
	SelRoomBtn = 1;
}

#ifdef _CHATROOMPROTOCOL
void SelectChatRoom(void)
{
	char tmpMsg[128] = {""};

	if(pSelChanlWnd == NULL){
			pSelChanlWnd = MakeWindowDisp( 120, 100, 7, roomNum / 2 + 2 , 2, 0 );
	}
	else if(pSelChanlWnd->hp > 0 ){
		int x = pSelChanlWnd->x;
		int y = pSelChanlWnd->y;
		char title[] = {"          Ƶ������          �ӳ�����  ���� "};
		StockFontBuffer( x + 10, y + 15, FONT_PRIO_FRONT,4 , title, 0 );
		int i;
		for(i = 0 ; i < roomNum ; i++ ){
			StockFontBuffer( x + 10, y + 40 + i * 20, FONT_PRIO_FRONT,5 , roomInfo[i], 0 );
   			if( MakeHitBox( x +10, y+ 37 + i * 20 , x + 320 , y + 58 + i * 20, DISP_PRIO_BOX2 )){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					sprintf(tmpMsg,"J|%d",roomIndex[i]);
					lssproto_CHATROOM_send ( sockfd , tmpMsg ) ;
					SelRoomBtn = 0;         //????????????
				}
			}
		}

		StockFontBuffer( x + 30, y + 50 + i * 20, FONT_PRIO_FRONT,5 , "  �����µ�������", 0 );
   		if( MakeHitBox(x + 30 ,y + 47 + i * 20 , x + 340 , y + 68 + i * 20, DISP_PRIO_BOX2 )){
			if( mouse.onceState & MOUSE_LEFT_CRICK )
				initSetRoomName();
		}
	}
}
#endif

void SwapOrder(int pos1,int pos2 )
{
	char    tempName[CHAR_NAME_LEN+1];
	int     tempIndex;
	char    tempNick[CHAR_FREENAME_LEN+1];

	strcpy(tempName,chatInfo.memberName[pos1]);
	strcpy(tempNick,chatInfo.nickName[pos1]);
	tempIndex = chatInfo.memberIndex[pos1];

	strcpy(chatInfo.memberName[pos1],chatInfo.memberName[pos2]);
	strcpy(chatInfo.nickName[pos1],chatInfo.nickName[pos2]);
	chatInfo.memberIndex[pos1] = chatInfo.memberIndex[pos2];

	strcpy(chatInfo.memberName[pos2],tempName);
	strcpy(chatInfo.nickName[pos2],tempNick);
	chatInfo.memberIndex[pos2] = tempIndex;
}

void InitRoomInfo()
{
	for (int i=0; i<50; i++){
		chatInfo.memberIndex[i] = 0;
		strcpy(chatInfo.memberName[i],"");
		strcpy(chatInfo.nickName[i],"");
	}
	chatInfo.chiefFlag = 0;
	chatInfo.chiefIndex = 0;
	chatInfo.memberNum = 0;
	firMemNo = 0;
}

void InitCreateChatRoom(char *msg)		//??????????????
{
	char temp[64],*temp1;
	chatInfo.chiefFlag = 0;
	InitRoomInfo();					    //?????????
	chatRoomBtn = 1;
	getStringToken( msg, '|', 2 , sizeof(temp) -1 ,temp );
	if((temp1 = strtok(temp,"r"))!= NULL ){
		strcpy(chatInfo.roomNo,temp1);
		strcpy(pc.chatRoomNum,chatInfo.roomNo);

		getStringToken( msg, '|', 3 , sizeof(chatInfo.roomName) -1 ,chatInfo.roomName );

		chatInfo.chiefIndex = getIntegerToken( msg, '|', 4 );

		getStringToken( msg, '|', 5 , sizeof(temp) -1 ,temp );
		strcpy(chatInfo.chiefName,temp);

		getStringToken( msg, '|', 6 , sizeof(temp) -1 ,temp );

		if( (temp1 = strtok(temp,"p"))!= NULL )
			chatInfo.memberNum = atoi(temp1);
		int i;
		for(i = 0; i< chatInfo.memberNum; i ++){
			getStringToken( msg, '|', 7+i*3, sizeof(temp) -1 ,temp );
			if ((temp1 = strchr(temp,'I')) == NULL )
				break;
			chatInfo.memberIndex[i] = atoi(temp1+1);
			getStringToken( msg, '|', 8+i*3, sizeof(chatInfo.memberName[i]) -1
							,chatInfo.memberName[i] );
			getStringToken( msg, '|', 9+i*3, sizeof(chatInfo.nickName[i]) -1
							,chatInfo.nickName[i] );
			if(i > 0 && chatInfo.memberIndex[i] == chatInfo.chiefIndex)
				SwapOrder(i, 0);                 //???????
				secretFlag = false;
				selChar = -1;
		}
		chatInfo.chiefFlag = getIntegerToken( msg, '|', 9 + i * 3 - 2 );
	}
#ifdef _CHANNEL_MODIFY
	pc.etcFlag |= PC_ETCFLAG_CHAT_CHAT;
#endif
}

#ifdef _CHATROOMPROTOCOL
void ChatRoomWnd( void )			//?????????
{
	char tmpMsg[STR_BUFFER_SIZE];
	int nameColor;
	int scrlLenth = 183;

	if(pChtChanlWnd == NULL){
		pChtChanlWnd = MakeWindowDisp( 350, 60, 540, 456, 0, -1 );
	}
	else if( pChtChanlWnd->hp > 0 ){
		int	x = pChtChanlWnd->x + 2;
		int y = pChtChanlWnd->y - 5;

		int scrTop = y + 69;           //Scroll???

		StockDispBuffer(  ( ( WINDOW_DISP *)pChtChanlWnd->pYobi )->mx - 120, ( ( WINDOW_DISP *)pChtChanlWnd->pYobi )->my -28, DISP_PRIO_MENU, CG_FIELD_CHATROOM_PANEL, 1 );
		char title[32] = {""};

		sprintf(title,"%s" ,chatInfo.roomName);

		StockFontBuffer( x + 140 - strlen(title) * 4 , y + 50, FONT_PRIO_FRONT,4 , title, 0 );

		if(!BtnType){
			closeBtn = CG_FIELD_CLOSE_BTN_UP;
			leaveBtn = CG_FIELD_LEAVE_BTN_UP;
			scrlHBtn = CG_FIELD_SCROLL_HUP;
			scrlLBtn = CG_FIELD_SCROLL_LUP;

			if(chatInfo.chiefFlag == 1){									//?????��??
				delBtn = CG_FIELD_DELETE_BTN_UP;
				if(secretFlag && chatInfo.chiefIndex != chatInfo.memberIndex[selChar]){
					outBtn = CG_FIELD_OUTMEMBER_BTN_UP;
					chaBtn = CG_FIELD_CHANGECHIEF_BTN_UP;
				}
				else{
					outBtn = CG_FIELD_OUTMEMBER_BTN_DISABLE;
					chaBtn = CG_FIELD_CHANGECHIEF_BTN_DISABLE;
				}
			}else{
				outBtn = CG_FIELD_OUTMEMBER_BTN_DISABLE;
				chaBtn = CG_FIELD_CHANGECHIEF_BTN_DISABLE;
				delBtn = CG_FIELD_DELETE_BTN_DISABLE;
			}
		}
		ChatRoomBtn[0] = StockDispBuffer( x + 70 , y + 335, DISP_PRIO_IME3, closeBtn, 2 );
		ChatRoomBtn[1] = StockDispBuffer( x + 230, y + 335, DISP_PRIO_IME3, leaveBtn, 2 );
		ChatRoomBtn[2] = StockDispBuffer( x + 150, y + 305, DISP_PRIO_IME3, outBtn, 2 );
		ChatRoomBtn[3] = StockDispBuffer( x + 230, y + 305, DISP_PRIO_IME3, chaBtn, 2 );
		ChatRoomBtn[4] = StockDispBuffer( x + 70, y + 305, DISP_PRIO_IME3, delBtn, 2 );
		ChatRoomBtn[5] = StockDispBuffer( x + 267, scrTop + scrlBtnIndex, DISP_PRIO_IME3, CG_FIELD_CHANNEL_SCROLL ,2);
		ChatRoomBtn[6] = StockDispBuffer( x + 267, scrTop -20, DISP_PRIO_IME3, scrlHBtn ,2);
		ChatRoomBtn[7] = StockDispBuffer( x + 267, y + 274, DISP_PRIO_IME3, scrlLBtn ,2);

		for(int i = 0 ; i < 10 ; i++ ){
			if(chatInfo.memberNum - 1 < i + firMemNo )
				break;
				//???????
   			if( MakeHitBox(x + 20 ,y + 77 + i * 20 , x + 250 , y + 96+ i * 20, DISP_PRIO_BOX2 )){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					if(strcmp(chatInfo.memberName[i + firMemNo], pc.name) != 0 || strcmp(chatInfo.nickName[i + firMemNo], pc.freeName) != 0){
						if(secretFlag == true && selChar == i){
							strcpy(secretName,"");
							pNowStrBuffer->buffer[ 0 ] = NULL;
							pNowStrBuffer->cursor=0;
							pNowStrBuffer->cnt = 0;
							StrToNowStrBuffer(secretName);
							secretFlag = false;
							selChar = -1;
							TalkMode = 0;
						}else{
							sprintf(secretName,"%s ",chatInfo.memberName[i+ firMemNo]);
							pNowStrBuffer->buffer[ 0 ] = NULL;
							pNowStrBuffer->cursor=0;
							pNowStrBuffer->cnt = 0;
							StrToNowStrBuffer(secretName);
							secretFlag = true;
							TalkMode = 1;
							selChar = i + firMemNo;
						}
					}
				}
			}

			if(secretFlag && selChar == i + firMemNo)
				nameColor = 4;
			else
				nameColor = 5;

			StockFontBuffer( x + 30, y + 80 + i * 20, FONT_PRIO_FRONT,nameColor , chatInfo.memberName[i + firMemNo], 0 );
			StockFontBuffer( x + 160, y + 80 + i * 20, FONT_PRIO_FRONT,nameColor , chatInfo.nickName[i + firMemNo], 0 );

		}

		for( int i = 0; i < 8; i++){
			if( i == 2 &&  chatInfo.chiefFlag == 0 )		//???????????????????
				i = 5;
			else if( i == 2 && chatInfo.chiefFlag == 1 && selChar == -1)			//????��???
				i = 4;

			if( HitDispNo == ChatRoomBtn[ i ] ) {
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					switch(i){
					case 0:
						closeBtn = CG_FIELD_CLOSE_BTN_DOWN;
						break;
					case 1:
						leaveBtn = CG_FIELD_LEAVE_BTN_DOWN;
						break;
					case 2:
						outBtn = CG_FIELD_OUTMEMBER_BTN_DOWN;
						break;
					case 3:
						chaBtn = CG_FIELD_CHANGECHIEF_BTN_DOWN;
						break;
					case 4:
						delBtn = CG_FIELD_DELETE_BTN_DOWN;
						break;
					case 5:
						scrollFlag = true;
						break;
					case 6:
						if(firMemNo >0)
						{
							firMemNo --;
							scrlBtnIndex = (scrlLenth/(chatInfo.memberNum -10))  * firMemNo;
							scrlHBtn = CG_FIELD_SCROLL_HDOWN;
						}
						break;
					case 7:
						if( chatInfo.memberNum > firMemNo + 10 && chatInfo.memberNum > 10)
						{
							firMemNo ++;
							scrlBtnIndex = (scrlLenth/(chatInfo.memberNum -10))  * firMemNo;
							scrlLBtn = CG_FIELD_SCROLL_LDOWN;
						}
						break;
					}
					BtnNo = i;
					BtnType = true;

				}else if(mouse.onceState & MOUSE_LEFT_CRICK_UP){
					switch(i){
						case 0:						//??????
							DeathMenuAction();
							break;

						case 1:						//?????
							if(chatInfo.chiefFlag == 1)
								StockChatBufferLine(  "�ҳ������뿪������,�����뿪�뻻���˵��ҳ���" , FONT_PAL_RED);
							else{
								lssproto_CHATROOM_send ( sockfd , "L|" ) ;
								strcpy(pc.chatRoomNum,"");
								DeathMenuAction();
								StockChatBufferLine(  "�����뿪������" , FONT_PAL_BLUE);
								TalkMode = 0;
#ifdef _CHANNEL_MODIFY
								pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
							}
							break;

						case 2:						//????
							sprintf(tmpMsg,"K|%d", chatInfo.memberIndex[selChar]);
							lssproto_CHATROOM_send ( sockfd , tmpMsg ) ;
							break;

						case 3:					//?????
							sprintf(tmpMsg,"M|%d", chatInfo.memberIndex[selChar]);
							lssproto_CHATROOM_send ( sockfd , tmpMsg );
							TalkMode = 0;
							break;

						case 4:					//??????
							lssproto_CHATROOM_send ( sockfd ,"D|") ;
							strcpy(pc.chatRoomNum,"");
							TalkMode = 0;
#ifdef _CHANNEL_MODIFY
							pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
							break;
						}
					secretFlag = false;
					selChar = -1;

					}
				}
			}

		if(mouse.onceState & MOUSE_LEFT_CRICK_UP || HitDispNo != ChatRoomBtn[ BtnNo ])
			BtnType = false;

		if(mouse.onceState & MOUSE_LEFT_CRICK_UP || mouse.nowPoint.x < x )
			scrollFlag =false;

		if(scrollFlag){
			if(	mouse.nowPoint.y > scrTop -2 && mouse.nowPoint.y < (scrTop + scrlLenth + 4) ){
				if(chatInfo.memberNum > 10){
					firMemNo = (mouse.nowPoint.y - scrTop) / (scrlLenth/(chatInfo.memberNum -10));
					scrlBtnIndex = mouse.nowPoint.y - scrTop;
				}else
					scrlBtnIndex = 0;
			}
		}
	}
}
#endif

void initAssentWnd(char *data)			//?????????
{
	assentFlag = true;
	char temp[64];
	getStringToken( data, '|', 2 , sizeof(temp) -1 ,temp );
	sprintf(memInfo,"%s ???????",temp);
	memIndex = getIntegerToken( data, '|', 3);
}

#ifdef _CHATROOMPROTOCOL
void AssentWnd(void)                    //?????????
{
	int i = 0;
	char tmpMsg[128] = {""};

	int AssentBtn[2];
	int x,y;
	if(pAssentWnd == NULL)
		pAssentWnd = MakeWindowDisp( 270, 0, 3, 2 , 2, 0 );
	else if( pAssentWnd->hp > 0){
		x = pAssentWnd->x;
		y = pAssentWnd->y;

		StockFontBuffer( x + 20 , y + 30, FONT_PRIO_FRONT,5 , memInfo, 0 );
		AssentBtn[0] = StockDispBuffer( x + 90, y + 60, DISP_PRIO_IME3, CG_FIELD_AGREE_BTN, 2 );
		AssentBtn[1] = StockDispBuffer( x + 100, y + 60, DISP_PRIO_IME3, CG_FIELD_DISAGREE_BTN, 2 );

		for( i = 0; i < 2; i++){
			if( HitDispNo == AssentBtn[ i ] ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					if(i == 0){
						sprintf(tmpMsg,"A|%d|1", memIndex);
						lssproto_CHATROOM_send ( sockfd ,tmpMsg) ;
					}
					else{
						sprintf(tmpMsg,"A|%d|0", memIndex);
						lssproto_CHATROOM_send ( sockfd , tmpMsg ) ;
					}
					DeathAction(pAssentWnd);
					pAssentWnd = NULL;
					assentFlag = false;
				}
			}
		}
	}
}
#endif

#ifdef _CHANNEL_MODIFY
void SaveChatData(char *msg,char KindOfChannel,bool bCloseFile);
#endif

void InitRecvMsg(char *data)
{
	char temp[STR_BUFFER_SIZE];
	char msg[STR_BUFFER_SIZE];
	getStringToken( data, '|', 2 , sizeof(temp) -1 ,temp );
#ifndef _CHANNEL_MODIFY
	sprintf(msg,"[Ƶ��]%s",temp);
#else
	sprintf(msg,"[��]%s",temp);
	TradeTalk(msg);
	SaveChatData(msg,'R',false);
#endif
	StockChatBufferLine( msg, 2);
}

#ifdef _CHATROOMPROTOCOL			// (?????) Syu ADD ?????????
void lssproto_CHATROOM_recv ( int fd, char *data)
{
	char type[3] = {""};
	int i = 0;
	getStringToken( data, '|', 1, sizeof( type ) - 1, type);
	switch(type[0]){
	case 'B':
			InitSelChatRoom(data);
			break;
	case 'D':						//Delete
			chatRoomBtn = 0;
			strcpy(pc.chatRoomNum,"");
			StockChatBufferLine(  "�������ѱ�ɾ����" , FONT_PAL_RED);
#ifdef _CHANNEL_MODIFY
			pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
			break;
	case 'T':
	    	InitRecvMsg(data);    //??????
			break;
	case 'K'://???
			chatRoomBtn = 0;
			strcpy(pc.chatRoomNum,"");
			StockChatBufferLine( "���ѱ��ҳ���������ң�??" , FONT_PAL_RED);
#ifdef _CHANNEL_MODIFY
			pc.etcFlag &= ~PC_ETCFLAG_CHAT_CHAT;
#endif
			break;
	case 'J'://????????
			if(assentFlag == false)
			{
				DeathAction(pAssentWnd);
				pAssentWnd = NULL;
				initAssentWnd(data);
			}
			break;
	case 'R':			//?????????????
			InitCreateChatRoom(data);
			break;
	}
}
#endif

#endif

#ifdef _TIMEBAR_FUNCTION
/***********************************************************
????:
title :  TimeBar ???  ??��32bit
range :  TimeBar ??��
????:	 TimeBar ?????.?????څ??????

barHolder[timBarIdent] : ???????��????????????????????
***********************************************************/
int SetTimeBar(char *title, int range)
{
	int timBarIdent = 0;
	if(strlen(title) > 32)
		return -1;
	if(timeBarFlag = true){
		DeathAction(pTimeBarWnd);
		pTimeBarWnd = NULL;
		if(barHolder[0]){
			barHolder[0] = false;
			barHolder[1] = true;
			timBarIdent = 1;
		}
		else{
			barHolder[1] = false;
			barHolder[0] = true;
			timBarIdent = 0;
		}
	}
	else{
		barHolder[0] = true;
		timBarIdent = 0;
	}
	strcpy(timeBarTitle , title);
	timeBarRange = range;
	timeBarFlag  = true;

	return timBarIdent;
}
/*******************************************
return -1: ???!!????????څ?????��?,???????????????
return  1: ?????? 100 %
*******************************************/
int SetTimeBarPos(int timBarIdent, int CurPos)
{
	if(!barHolder[timBarIdent] )//|| CurPos > timeBarRange)
		return -1;
	timeBarCurPos = 20 * CurPos / timeBarRange;  //20?bar???��????.
 	if(timeBarCurPos > 20){
		timeBarCurPos = 20;
		return 1;				//???????100 %??
	}
	return 0;
}

void DrawTimeBar()
{
	if(pTimeBarWnd == NULL){
		pTimeBarWnd = MakeWindowDisp( 270, 0, 3, 2 , 2, 0 );
	}
	else if( pTimeBarWnd->hp > 0){
		int x = pTimeBarWnd->x;
		int y = pTimeBarWnd->y;
		char percent[4] = {""};
		sprintf(percent,"%d", (timeBarCurPos * 5));
		StockFontBuffer( x + 20 , y + 30, FONT_PRIO_FRONT,5 , timeBarTitle, 0 );
		StockFontBuffer( x + 50 , y + 50, FONT_PRIO_FRONT,5 , percent, 0 );
		for(int i = 0 ; i < timeBarCurPos ; i++)
			StockDispBuffer( x + i * 10, y + 60, DISP_PRIO_IME3, CG_TIMEBAR_UNIT, 2 );
	}
}
#endif

#ifdef _STANDBYPET
void send_StandBy_Pet( void )
{
	int	standbypet =0;
	int i, cnt =0;
	for( i =0; i <MAX_PET; i++){
		if( pc.selectPetNo[ i ] == true ) {
			cnt++;
			if( cnt > 3 )
				break;
			standbypet |= ( 1 << i );
		}
	}
	lssproto_SPET_send( sockfd, standbypet );
	StandbyPetSendFlag = true;
}
#endif
/* =========================================

???!!
??????????????1??5???????????VC????��?????
??????????????????Code???

~Robin~

????????menu2.cpp  by  Change
========================================= */
