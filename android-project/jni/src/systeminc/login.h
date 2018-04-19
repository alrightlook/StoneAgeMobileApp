#ifndef _LOGIN_H_
#define _LOGIN_H_
#include "version.h"
#include "action.h"
enum
{
	WINDOW_MESSAGETYPE_MESSAGE,					// ???????
	WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT,		// ?????????
	WINDOW_MESSAGETYPE_SELECT,					// ?????
	WINDOW_MESSAGETYPE_PETSELECT,				// ????????
	WINDOW_MESSAGETYPE_PARTYSELECT,				// ???????
	WINDOW_MESSAGETYPE_PETANDPARTYSELECT,		// ???????????
	WINDOW_MESSAGETYPE_ITEMSHOPMENU,			// ???????????
	WINDOW_MESSAGETYPE_ITEMSHOPMAIN,			// ??????????
	WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN,		// ??????????????????
	WINDOW_MESSAGETYPE_PETSKILLSHOP,			// ?????????????
	WINDOW_MESSAGETYPE_WIDEMESSAGE,				// ???????????
	WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT,	// ?????????????
	WINDOW_MESSAGETYPE_POOLITEMSHOPMENU,		// ?????????????????
	WINDOW_MESSAGETYPE_POOLITEMSHOPMAIN,			// ?????????????????

	WINDOW_MESSAGETYPE_FAMILYADD,
	WINDOW_MESSAGETYPE_FAMILYJOIN,
	WINDOW_MESSAGETYPE_FAMILYLEAVE,
	WINDOW_MESSAGETYPE_FAMILYEND,

	// shan add	
	WINDOW_FMMESSAGETYPE_SELECT,
	WINDOW_FMMESSAGETYPE_DENGON,
  WINDOW_FMMESSAGETYPE_FMSDENGON,	//20
	WINDOW_FMMESSAGETYPE_POINTLIST,
#ifdef _FMVER21
  WINDOW_FMMESSAGETYPE_TOP30DP,
#endif
	WINDOW_FMMESSAGETYPE_DP,

  WINDOW_MESSAGETYPE_BANK,
	
	// Arminius 7.12 scheduleman
	WINDOW_MESSAGETYPE_PKSCHEDULELIST,	//25
	WINDOW_MESSAGETYPE_PKSCHEDULESELECTFAMILY,
	WINDOW_MESSAGETYPE_PKSCHEDULEDETAIL,

	// Robin
	WINDOW_MESSAGETYPE_LOGINMESSAGE,
	WINDOW_MESSAGETYPE_FAMILYTAX,
	WINDOW_MESSAGETYPE_SHOWRIDEPET,	//30
	WINDOW_MESSAGETYPE_FAMILYDETAIL,

	WINDOW_MESSAGETYPE_LEADERSELECT,
	WINDOW_MESSAGETYPE_LEADERSELECTQ,
	WINDOW_MESSAGETYPE_LEADERSELECTA,
//Terry add 2002/01/03
#ifdef __EDEN_AUCTION
	WINDOW_MESSAGETYPE_AUCTIONNEW,	//35
	WINDOW_MESSAGETYPE_AUCTIONLIST_BUY,
	WINDOW_MESSAGETYPE_AUCTIONSURVEY,
	WINDOW_MESSAGETYPE_AUCTIONMODIFY,
	WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY,
#endif
//Terry end
#ifdef _BLACK_MARKET
    WINDOW_MESSAGETYPE_BLACKMARKET,
#endif
#ifdef _ADD_FAMILY_TAX			   // WON ADD 增加庄园税收	
	WINDOWS_MESSAGETYPE_FAMILY_TAX,
#endif
#ifdef _NPC_FUSION
	WINDOWS_MESSAGETYPE_PETFUSION,
#endif
#ifdef _PETSKILL_CANNEDFOOD
	WINDOWS_MESSAGETYPE_PETSKILLSHOW,
#endif

#ifdef _NPC_SELLSTH
	WINDOWS_MESSAGETYPE_SELLSTHMENU,
	WINDOWS_MESSAGETYPE_SELLSTHVIEW,
	WINDOWS_MESSAGETYPE_SELLSTHSELL,
#endif

#ifdef _CHAR_PROFESSION			// WON ADD 人物职业技能
	WINDOW_MESSAGETYPE_PROFESSIONSHOP,
#endif
#ifdef _NPC_WELFARE_2				// WON ADD 职业NPC-2
	WINDOW_MESSAGETYPE_PROFESSIONSHOP2,
#endif
#ifdef _NEW_MANOR_LAW
	WINDOW_FMMESSAGETYPE_10_MEMONTUM,	// 十大气势家族
	WINDOW_FMMESSAGETYPE_FM_MEMONTUM,	// 自己家族气势排名
	WINDOW_FMMESSAGETYPE_MANOR_SCHEDULE,	// 挑战庄园排行
#endif

#ifdef _ANGEL_SUMMON
	WINDOW_MESSAGETYPE_ANGELMESSAGE = 101,
	WINDOW_MESSAGETYPE_ANGELASK,
#endif

#ifdef _MOUSE_DBL_CLICK
	WINDOW_MESSAGETYPE_MOUSEGETNAME,
#endif

#ifdef _CONTRACT
	WINDOW_MESSAGETYPE_CONTRACT,
#endif

#ifdef _RACEMAN
	WINDOW_MESSAGETYPE_RACEMAN_RANK,
#endif

#ifdef _DRAGON_FUSION
	WINDOWS_MESSAGETYPE_DRAGONFUSION,
#endif

#ifdef _NPC_DAYACTION
	WINDOW_MESSAGETYPE_DAYACTION_FMRANK,
#endif

#ifdef PK_SERVICE_MAN
	WINDOW_MESSAGETYPE_DAYACTION_PKLIST,
	WINDOW_MESSAGETYPE_DAYACTION_PKHISTORYLIST,
#endif
};

#define	WINDOW_BUTTONTYPE_NONE		(0)
#define	WINDOW_BUTTONTYPE_OK		(1 << 0)
#define	WINDOW_BUTTONTYPE_CANCEL	(1 << 1)
#define	WINDOW_BUTTONTYPE_YES		(1 << 2)
#define	WINDOW_BUTTONTYPE_NO		(1 << 3)
#define	WINDOW_BUTTONTYPE_PRE		(1 << 4)
#define	WINDOW_BUTTONTYPE_NEXT		(1 << 5)

#define MAX_GMGROUP		19
#define MAX_GMSV		150

extern bool logOutFlag;
extern unsigned int MsgCooltime;

extern short createCharFlag;

extern int windowTypeWN;
extern short wnCloseFlag;

void idPasswordProc( void );
void titleProc( void );
void selectCharacterProc( void );
void characterLoginProc( void );

void makeCharacterProc( void );

void characterLogoutProc( void );
#ifdef _80_LOGIN_PLAY
void _80LoginProc(void);
#endif

void initOpenServerWindow( void );
void openServerWindow( int, int, int, int, char * );
void openServerWindowProc( void );



int selGraId( int *, int );
int selRepGraId( int *, int );
int pushGraId( int *, int );
int selFontId( int *, int );
int focusGraId( int *, int );
int focusFontId( int *, int );
#ifdef _BLACK_MARKET
void closeBlackMarketWN( void );
#endif


#ifdef _PET_TRACE_MOUSE
class SCPlayPet{
private:
	ACTION *m_pPet;
	int m_iDestX,m_iDestY,m_iX,m_iY,m_iCount,m_iDirx,m_iDiry,b,m_iActionStep;
	bool m_bMove,m_bChangeDir,m_bAIWalk;
	unsigned int m_iTimeToChangeAction,m_iTimeToChangeTime,m_iKindOfChangeTime;

	void CnangePetAction(bool bUserMove);
public:
	SCPlayPet();
	~SCPlayPet();

	void Proc(void);
	void SetActionStep(int iActionStep) { m_iActionStep = iActionStep;};
};
#endif

#ifdef _PK2007
void selectpkProc( void );
#endif

#endif
