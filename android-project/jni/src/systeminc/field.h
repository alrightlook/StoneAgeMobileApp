#ifndef _FIELD_H_ 
#define _FIELD_H_

#include<time.h>

// ??????????
#define FIELD_BTN_PUSH_WAIT			500		// 0.5

// ???????䴓??
#define FIELD_MAIL_LAMP_FLASH_TIME	1000	// 1??

enum
{
	FIELD_FUNC_MENU,
	FIELD_FUNC_CARD,
	FIELD_FUNC_PARTY,
	FIELD_FUNC_JOIN_BATTLE,
	FIELD_FUNC_DUEL,
	FIELD_FUNC_ACT,
	FIELD_FUNC_TRADE,	// Robin 04/12 trade
	FIELD_FUNC_CHANNEL,
#ifdef __HELP
	FIELD_FUNC_HELP,	//CARY
#endif
#ifdef __PHONEMESSAGE
	FIELD_FUNC_MSG,     //LeiBoy
#endif
#ifdef _STREET_VENDOR
	FIELD_FUNC_STREET_VENDOR,
#else
	#ifdef _TABLEOFSKILL
	FIELD_FUNC_SKILL,
	#endif
#endif

#ifdef _FRIENDCHANNEL			//ROG ADD 好友频道
	FIELD_FUNC_CHATROOM,
#endif

#ifdef _ANGEL_SUMMON
	FIELD_FUNC_ANGEL,
#endif

#ifdef _TEACHER_SYSTEM
	FIELD_FUNC_TEACHER,
#endif

	FIELD_FUNC_END
};

#ifdef _PET_ITEM
typedef struct SPetItemInfo
{
	int bmpNo;										// 图号
	int color;										// 文字颜色
	char memo[ITEM_MEMO_LEN+1];						// 说明
	char name[ITEM_NAME_LEN+1];						// 名字
	char damage[16];								// 耐久度
}PetItemInfo;
#endif

#ifdef _STREET_VENDOR
#define MAX_SELL_ITEM 20							// 道具加宠物共可卖二十个
typedef struct _Show_Sell_Item{
	int index;										// 索引
	int pile;										// 数量
	int price;										// 价格
	bool needGetPrice;								// 是否要得到价格
	bool usage;										// 是否使用中
	char name[ITEM_NAME_LEN+1];						// 名字
	char freeName[ITEM_NAME_LEN+1];					// 名字
	char kind;										// 道具或是宠物 0:道具 1:宠物
}Show_Sell_Item;

typedef struct _Show_Vendor_Item{
	int bmpNo;										// 图号
	int pile;										// 数量
	int price;										// 价格
	int color;										// 文字颜色
	int level;										// 等级
	int maxhp,attack,defence,dex;					// 四围
	int earth,water,fire,wind,fidelity;				// 四属性及忠诚度
	int maxSkill;									// 宠技数量
	int index;										// 在server的储存位置
	bool usage;										// 是否使用中
	bool bBuy;										// 是否要买
	bool bGetData;									// 是否已接收到详细资料
	char name[ITEM_NAME_LEN+1];						// 名字
	char freeName[ITEM_NAME_LEN+1];					// 名字
	char memo[ITEM_MEMO_LEN+1];						// 说明
	char damage[16];								// 耐久度
	char skillname[MAX_SKILL][SKILL_NAME_LEN+1];	// 宠技名称
	char kind;										// 道具或是宠物 0:道具 1:宠物
	char trans;										// 转生数
#ifdef _PET_ITEM
	PetItemInfo oPetItemInfo[MAX_PET_ITEM];			// 宠物身上的道具
#endif
}Show_Vendor_Item;

#endif

#ifdef _JOBDAILY

#define MAXMISSION	300 
typedef struct _JOBDAILY{
	int JobId;								// 任务编号
	char explain[200];						// 任务说明
	char state[10];							// 状态
}JOBDAILY;

#endif


#ifdef _FRIENDCHANNEL			//ROG ADD 好友频道
	extern short chatRoomBtn;
	extern short SelRoomBtn;
#endif


#ifdef _ANGEL_SUMMON
extern int angelFlag;
extern int angelRecvType;
extern int angelRecvButton;
extern char angelMsg[];
#endif

extern short drawTimeAnimeFlag;

extern short etcSendFlag;

extern short actBtn;
extern short menuBtn;
extern short mailLamp;

extern unsigned int fieldInfoTime;

extern struct tm *serverAliveTime;
extern time_t serverAliveLongTime;


void initFieldProc( void );
void resetFieldProc( void );
void fieldProc( void );
void fieldProc2( void );
void drawField( void );

bool checkFieldMenuFlag( void );

void closeCharActionAnimeChange( void );
void closeEtcSwitch( void );
void closeJoinChannelWN( void );

int disconnectServer( void );

void drawFieldInfoWin( void );

void actionShortCutKeyProc( void );

#ifdef _SPECIALSPACEANIM	// Syu ADD 特殊场景动画配置
	#ifdef _SURFACE_ANIM        //动态地上物显示
//#define TOTAL_ANIM 5 //阵列总量
		#define MAX_ANIM   256 //同一个floor限定最大量动画数
	#else
		#ifdef _MOON_FAIRYLAND       // ROG ADD 月之仙境
			#define TOTAL_ANIM 5 //阵列总量
			#define MAX_ANIM   4 //同一个floor内现存最大量Anim
		#else
			#define TOTAL_ANIM 1 //阵列总量
			#define MAX_ANIM  1  //同一个floor内现存最大量Anim
	#endif
#endif

void SpecAnim ( int ) ; 
void ReleaseSpecAnim( void );

#endif
#endif
