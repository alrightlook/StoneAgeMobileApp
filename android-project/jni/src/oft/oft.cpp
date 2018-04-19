/************************/
/*	oft.c				*/
/************************/

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "vg410.h"
#include "work.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/chat.h"
#include "../systeminc/tool.h"
#include "../systeminc/pc.h"
#include "../systeminc/t_music.h"
#include "../systeminc/battlemap.h"
#include "../systeminc/menu.h"
#include "../systeminc/battleproc.h"
#include <math.h>
#include "../systeminc/version.h"
#include <stdlib.h>

extern bool g_bUseAlpha;

// Robin 0804 ride Pet
extern	bool	bNewServer;

void petfallChangeGraph(ACTION *a0);
void LogToBattleError(char *data, int line );
void katino( ACTION *a0 );
static int command_point;
extern ACTION *MakeAnimDisp( int x, int y, int sprNo, int mode );
#ifdef _SYUTEST
static ACTION *Light1;
#endif
#ifdef _PETSKILL_LER
extern bool g_bUseAlpha;
#endif

#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
//�����⴦��
int ShooterNum = -1 ;
/*
int nuty[20] = { 384 , 408 , 360 , 432 , 336 , 312 , 360 , 264 , 408 , 216 , 150 , 174 , 
				126 , 198 , 102 , 202 , 250 , 154 , 298 , 106 } ;
*/
#endif

#ifdef _MAGIC_NOCAST//��Ĭ
extern bool NoCastFlag;
#endif
//ACTION *set_bg( void );
ACTION *oft_test( void );
//ACTION *set_teki( void );
//void teki( ACTION *pAct );
void monster( ACTION *a0 );
static int get_num(void);

/* ??????? ****************************************************/
#define HIT_STOP_TIM		2
#define VCT_NO_DIE			250
#define VCT_NO_APPEAR		VCT_NO_DIE - 1
#ifdef __NEW_BATTLE_EFFECT
//#define VCT_NO_PAUSE		254
#define VCT_NO_EFFECT		255
#endif
#define SCREEN_OUT			106
#define ATTACK_MAGIC_CASE	200
#define TOCALL_MAGIC_CASE	210
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
#define PROSKILL			230
int AttPreMagicNum = -1 ;
int AttNextMagicNum = -1 ;
short NoMiss = -1 ; //���⴦��˫�ع���
short StarLoop = 0 ; 
//#endif
#ifdef _FIREHUNTER_SKILL				// (���ɿ�) ROG ADD ��ȸ����_������ɱ
#define FIRE_HUNTER_SKILL   240
#endif
#ifdef _WAVETRACK					// (���ɿ�) Syu ADD ����
int BeAttNum = -1 ; //�������ߵ�λ�ñ��
#endif

#ifdef _FIREHUNTER_SKILL				// (���ɿ�) ROG ADD ��ȸ����_������ɱ
bool FireSkillEnd = false;
bool bFireInit = false;
int tempPosH = 0;
int tempPosV = 0;
int  iBeAttNum = -1;                     //�������߱��
int counter;
int tarpos = -1;
int tarMgiDem = 0;
#endif

/* ?????��? ****************************************************/
//????????
char att_select_flg;

/* ??????��? ****************************************************/
static ACTION *p_kanji;
static char kanji_buf[4][128];
static ACTION *p_master;
static ACTION *p_attrib;
static ACTION *p_missile[20+1];
unsigned char crs_change_tbl[32] = {
	4,4,5,5,5,5,6,6,6,6,7,7,7,7,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4};
unsigned char crs_change_tbl2[8] = {
	16,20,24,28,0,4,8,12};
unsigned char crs_bound_tbl[4][32] = {
//	  0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
	{16,15,14,13,12,11,10, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,23,22,21,20,19,18,17},
	{ 0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 6, 5, 4, 3, 2, 1, 0,31,30,29,28,27,26,25, 0, 0, 0, 0, 0, 0, 0, 0},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
};
typedef struct{
	int		v_pos;
	ACTION	*work;
} SORT_CHR_EQU;
static SORT_CHR_EQU sort_chr_buf[40];
static char pet_nix_tbl[] = 
{	 1,0,0,0,0,0,0,0,0,0,0,0,0,
    -1,0,0,0,0,
    -1,0,0,0,0,0,0,0,0,0,0,0,0,
     1,0,0,0,0,
     1,0,0,0,0,0,0,0,0,0,0,0,0,
    -1,0,0,0,0,
    -1,0,0,0,0,0,0,0,0,0,0,0,0,
     1,0,0,0,0,
    -2,
};

static int monster_start_pos[40];
static int monster_place_pos[40];
static int monster_place_no[20] = { 2,1,3,0,4,7,6,8,5,9,12,11,13,10,14,17,16,18,15,19};

#ifdef __ATTACK_MAGIC

// Global vars
int			g_iRunEarthQuake	= 0;		// �����״��: 0 --> û���� , 1 --> ��ʼ������ , 2 --> ������
int			g_iCurRunEarthQuake = 0;		// Ŀǰ�����λ����
int			g_iNumRunEarthQuake;			// ������ܴ���



#define		ATTACK_MAGIC_ID		12345678	// ����ħ����ʶ�����
#define		MAX_NUM_ATTACKED	16			// ��󱻹����ߵ���Ŀ



// λ�ö���
struct POS
{
	unsigned short	x;							// X���λ��
	unsigned short	y;							// Y���λ��
};



// BJ | �����ߵı�� | 12345678 | ��������ʩ���������ʣ��MP | ǰ�ö����Ķ������ | ���������Ķ������ | 
// ���ö����Ķ������ | �����ķ�ʽ | ������ʱ��� | ��ʾ������λ�õķ�ʽ( ���Ի���� ) | 
// λ��( �����з���������ʾλ�÷�ʽ ) | λ��( ǰ�ö��������λ�÷�ʽ ) | λ��( ���ö��������λ�÷�ʽ ) |
// ǰ�ö�����ʾ�������ǰ���� | ����������ʾ�������ǰ���� | ���ö�����ʾ�������ǰ���� | 
// �𶯻��� | �𶯵���ʼʱ�� | �𶯵Ľ���ʱ�� | �����Ķ���X1 | �����Ķ���X2 | ...�����Ķ���Xn|FF
// 12345678:		��ʾΪ�����Ե�ħ���������Ϊ˯������
struct ATTACK_MAGIC
{
	unsigned long	dwCurFrame1;						// Ŀǰ�Ѳ��Ź���Frames�������δ���Ź�ǰ������ǰ
	unsigned long	dwCurFrame2;						// Ŀǰ�Ѳ��Ź���Frames������Ѿ�������ǰ��������
	unsigned long	dwEQuake;							// �Ƿ���𶯻���
	unsigned long	dwEQuakeSTime;						// ����Ŀ�ʼʱ��
	unsigned long	dwEQuakeETime;						// ����Ľ���ʱ��
	unsigned long	dwEQuakeSFrame;						// ���ŵ���Ŀ�ʼFrame��0XFFFFFFFF��ʾû�е������Ч
	unsigned long	dwEQuakeEFrame;						// ���ŵ���Ľ���Frame��0XFFFFFFFF��ʾû�е������Ч
	int		iPreMgcNum;							// ǰ�ö����Ķ������
	int		iCurMgcNum;							// ���������Ķ������
	int		iPostMgcNum;						// ���ö����Ķ������
	unsigned short	wRunPreMgc;							// �Ƿ��Ѿ�ִ����ǰ������
	unsigned short	wAttackType;						// �����ķ�ʽ: ����( �� )������( �������� )������( ͬʱ���� )��ȫ��( �������� )��ȫ��( ͬʱ���� )
	unsigned short	wAttackTimeSlice;					// ������ʱ���Ժ���Ϊ��λ
	unsigned short	wShowType;							// ��ʾ������λ�õķ�ʽ�����Լ���������
	unsigned short	wScreenX;							// ������ʾλ�õ�X�ᣬ�ھ�����ʾ��ʽʱ
	unsigned short	wScreenY;							// ������ʾλ�õ�Y�ᣬ�ھ�����ʾ��ʽʱ
	unsigned short	wPreMgcX;							// ǰ�ö��������λ��
	unsigned short	wPreMgcY;							// ǰ�ö��������λ��
	unsigned short	wPostMgcX;							// ���ö��������λ��
	unsigned short	wPostMgcY;							// ���ö��������λ��
	unsigned short	wPreMgcOnChar;						// ǰ�ö�����ʾ�ڵذ��ϣ���������
	unsigned short	wCurMgcOnChar;						// ����������ʾ�ڵذ��ϣ���������
	unsigned short	wPostMgcOnChar;						// ���ö�����ʾ�ڵذ��ϣ���������
	unsigned short	wMgcFrameCount[MAX_NUM_ATTACKED];	// ���Ź��������ڵ���ʱ��Frame Count
	unsigned short	wAttackedIndex[MAX_NUM_ATTACKED];	// �������ߵ����������У�0 - 19: ����һλ , 20: ǰ�� , 21: ����  , 22: ȫ��з�
	POS		posAttacked[MAX_NUM_ATTACKED];		// �������ߵı�����λ��
	unsigned short	wNumAttacks;						// �������ܴ���
	unsigned short	wNumAttackeds;						// ����������ϵĴ���
	unsigned short	wCurAttackNum;						// Ŀǰ������������
};



static ATTACK_MAGIC	AttMgc;						// ���������ı���
static bool			bRunAttMgc = false;			// �Ƿ��ѳ�ʼ����һ����������
static int			AttackedInfo[4 * 10];		// ʮ���˵ı���������Ѷ��ÿ���˵���λ��Ѷ( ��������������|�����ŵ���������|�˿۵�Ѫ|����۵�Ѫ )
static int			iAttackedNum;				// ������������Ŀ
static int			iCurAttackedFinishNum;		// ����ɵĵ�����



static bool	BuildAttackMagicData( ACTION *pMaster , ACTION *pAttacker );	// ���������������Ͻ���
static bool RunTimeMagic();													// �����������ļ�غ�ʽ

#endif


#ifdef __TOCALL_MAGIC
// kjl 02/06/24
#define TOCALL_MAGIC_ID		5711438			// �ٻ���ʶ�����

struct TOCALL_MAGIC
{
	unsigned long	dwCurFrame1;						// Ŀǰ�Ѳ��Ź���Frames�������δ���Ź�ǰ������ǰ
	unsigned long	dwCurFrame2;						// Ŀǰ�Ѳ��Ź���Frames������Ѿ�������ǰ��������
	unsigned long	dwEQuake;							// �Ƿ���𶯻���
	unsigned long	dwEQuakeSTime;						// ����Ŀ�ʼʱ��
	unsigned long	dwEQuakeETime;						// ����Ľ���ʱ��
	unsigned long	dwEQuakeSFrame;						// ���ŵ���Ŀ�ʼFrame��0XFFFFFFFF��ʾû�е������Ч
	unsigned long	dwEQuakeEFrame;						// ���ŵ���Ľ���Frame��0XFFFFFFFF��ʾû�е������Ч
	int		iPreMgcNum;							// ǰ�ö����Ķ������
	int		iCurMgcNum;							// ���������Ķ������
	int		iPostMgcNum;						// ���ö����Ķ������
	unsigned short	wRunPreMgc;							// �Ƿ��Ѿ�ִ����ǰ������
	unsigned short	wAttackType;						// �����ķ�ʽ: ����( �� )������( �������� )������( ͬʱ���� )��ȫ��( �������� )��ȫ��( ͬʱ���� )
	unsigned short	wAttackTimeSlice;					// ������ʱ���Ժ���Ϊ��λ
	unsigned short	wShowType;							// ��ʾ������λ�õķ�ʽ�����Լ���������
	unsigned short	wScreenX;							// ������ʾλ�õ�X�ᣬ�ھ�����ʾ��ʽʱ
	unsigned short	wScreenY;							// ������ʾλ�õ�Y�ᣬ�ھ�����ʾ��ʽʱ
	unsigned short	wPreMgcX;							// ǰ�ö��������λ��
	unsigned short	wPreMgcY;							// ǰ�ö��������λ��
	unsigned short	wPostMgcX;							// ���ö��������λ��
	unsigned short	wPostMgcY;							// ���ö��������λ��
	unsigned short	wPreMgcOnChar;						// ǰ�ö�����ʾ�ڵذ��ϣ���������
	unsigned short	wCurMgcOnChar;						// ����������ʾ�ڵذ��ϣ���������
	unsigned short	wPostMgcOnChar;						// ���ö�����ʾ�ڵذ��ϣ���������
	unsigned short	wMgcFrameCount[MAX_NUM_ATTACKED];	// ���Ź��������ڵ���ʱ��Frame Count
	unsigned short	wAttackedIndex[MAX_NUM_ATTACKED];	// �������ߵ����������У�0 - 19: ����һλ , 20: ǰ�� , 21: ����  , 22: ȫ��з�
	POS		posAttacked[MAX_NUM_ATTACKED];		// �������ߵı�����λ��
	unsigned short	wNumAttacks;						// �������ܴ���
	unsigned short	wNumAttackeds;						// ����������ϵĴ���
	unsigned short	wCurAttackNum;						// Ŀǰ������������
	unsigned short	wIsPostDisappear;					// �����������Ƿ�������ʧ
	unsigned short	wToCallMagicNo;						// �ٻ������
};

static TOCALL_MAGIC	ToCallMgc;						// ���������ı���
static bool			bRunToCallMgc = false;			// �Ƿ��ѳ�ʼ����һ����������

static bool	BuildToCallMagicData( ACTION *pMaster , ACTION *pAttacker );	// �ٻ����������Ͻ���
//static bool RunTimeMagic();													// �����������ļ�غ�ʽ

#endif

#ifdef _PROFESSION_ADDSKILL
static int bRunBoundaryMgc_l = 0, bRunBoundaryMgc_r = 0;			// ��Ч״̬
ACTION *boundary_2,*boundary_mark[2];
static bool	BuildBoundaryMagicData( int state );	// ��Ч�����Ͻ���
static void RunTimeMagicBoundary(int state);
#endif

//��������˳��
enum {
	T_PRIO_TOP,						/* ??�I 	*/
	T_PRIO_JIKI = 20,				/* ��? 	*/
	T_PRIO_JIKI_MISSILE,			/* ��?�d 	*/
	T_PRIO_MONSTER = 30,			/* �� 		*/
	T_PRIO_UFO = 60,				/* ??? 	*/
	T_PRIO_MONSTER_MISSILE = 70,	/* ���d 	*/
	T_PRIO_MISSILE,					/* ????	*/
	T_PRIO_MAGIC_EFFECT,			/* ?��??	*/
	T_PRIO_HIT_MARK,				/* ??????	*/
	T_PRIO_DAMAGE_NUM,				/* ????�k */
	T_PRIO_MASTER = 100,			/* ???? */
	T_PRIO_BOW,						/* ? 	*/
	T_PRIO_BTM 						/* ??�� 	*/
};

enum {
	D_PRIO_MASTER = 80,					/* ???? */
	D_PRIO_MONSTER_MISSILE,			/* ���d 	*/
	D_PRIO_UFO,						/* ??? 	*/
	D_PRIO_MONSTER,					/* �� 		*/
	D_PRIO_JIKI_MISSILE,			/* ��?�d 	*/
	D_PRIO_JIKI,					/* ��? 	*/
	D_PRIO_MISSILE,					/* ????	*/
	D_PRIO_MAGIC_EFFECT,			/* ?��??	*/
	D_PRIO_HIT_MARK,				/* ??????	*/
	D_PRIO_DAMAGE_NUM,				/* ????�k */
};



/* ???????? *******************************************************************/
void kakushi_command(void)
{
	//??????
	if (LowResoCmdFlag)		// ??�T��????????
		return;
	//�V?��???
	if ((joy_con[0] & JOY_LEFT) && (joy_con[0] & JOY_RIGHT))
	{
		LowResoCmdFlag = 1;		// ??�T��??????????
		//???????
		play_se(211, 320, 240);
	}
}

#define DAMAGE_SPD	24
//������ʱ����ʾ����
void damage_num(ACTION *a0)
{
	int d0,d1,dx,d2=0;
	char szMoji[256];
	char szMojiP[256];
//andy_mp
	char szMojMp[256];
	char szMojHp[256];

	d0 = 0; dx = 0;
	if (slow_flg)		//������slow״̬
	{
		if (s_timer & 3)		//���ƶ�
			d0 = 1;
	}
	if (d0 == 0)		//�ƶ������
	{
		switch (ATR_INT_WORK1(a0))
		{
		case 0:
		case 36:
		case 37:
		case 38:
		case 39:
		case 40:
#ifdef _SKILL_ADDBARRIER
		case 43:
#endif 
#ifdef _PETSKILL_PEEL
		case 44:
#endif
		case 6:
		case 14:
		case 15:
		case 16:
			gemini(a0);
		}
		switch (ATR_VCT_NO(a0))
		{
		case 0:
			ATR_SPD(a0) -= 8;		//��?��??????
			if (ATR_SPD(a0))		//��?��??????
				break;
			ATR_CRS(a0) = 16;
			ATR_VCT_NO(a0) = 1;
			break;

		case 1:
			ATR_SPD(a0) += 8;
			if (ATR_SPD(a0) >= DAMAGE_SPD){		//�K�ˤʤ�
				ATR_STIMER(a0) = 60;
				ATR_VCT_NO(a0) = 2;
			}
			break;

		case 2:
			ATR_SPD(a0) = 0;		//?�h�_��
			ATR_STIMER(a0) -= 6;
			if (ATR_STIMER(a0)){		//�K�ˤʤ�
				break;
			}
			DeathAction(a0);		//��?
			return;
		}
	}

	switch (ATR_INT_WORK1(a0))
	{
	case 0:		//Miss����
		sprintf(szMoji, "Miss");
		d1 = 0;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_MISS;
		break;
	case 1:		//��������
		sprintf(szMoji, "Counter");
		d1 = 1;		//ˮɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_COUNTER;
		break;
	case 3:		//��������
		sprintf(szMoji, "Guard");
		d1 = 3;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD;
		break;
	case 5:		//��׽����
		sprintf(szMoji, "Capture");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CAPTURE;
		break;

	case 6:		//���˴���
		sprintf(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf(szMojiP, "%d", ATR_INT_WORKp(a0));
//andy_mp
		if (ATR_MPDFLG(a0) == 1)
			sprintf(szMojMp, "%d", ATR_MPDAMAGE(a0));
		if (ATR_ADDHPFLG(a0) == 1)
			sprintf(szMojHp, "%d", ATR_ADDHP(a0));
		d1 = 6;		//��ɫSet
		break;
	case 7:		//��׽�ɹ�����
		sprintf(szMoji, "Success");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GET;
		break;
	case 8:		//��׽ʧ�ܴ���
		sprintf(szMoji, "Fail");
		d1 = 6;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_FAIL;
		break;
	case 9:		//���ܴ���
		sprintf(szMoji, "Escape");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_ESCAPE;
		break;
	case 10:		//�ջس��ﴦ��
		sprintf(szMoji, "Come!");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_COME_ON;
		break;
	case 11:		//�ų����ﴦ��
		sprintf(szMoji, "Go!");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GO;
		break;
	case 12:		//�Ƴ���������
		sprintf(szMoji, "Guard break");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD_BREAK;
		break;
	case 13:		//����???
		sprintf(szMoji, "Danger");
		d1 = 6;		//����???
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_DANGER;
		break;
	case 14:		//�ظ�
		sprintf(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 5;		//��ɫSet
		break;
	case 15:		//MP�ظ�
		sprintf(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 4;		//��ɫSet
		break;
	case 16:		//MP�½�
		sprintf(szMoji, "%d", ATR_INT_WORK0(a0));
		sprintf(szMojiP, "%d", ATR_INT_WORKp(a0));
		d1 = 3;		//��ɫSet
		break;
	case 17:		//��������
		sprintf(szMoji, "Leave");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_LEAVE;
		break;
	case 18:		//����NONO
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_NO;
		break;
	case 19:		//CRUSH
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CRUSH;
		break;
	case 20:		//?????????
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_CAPTURE_UP;
		break;

#ifdef _SKILL_ROAR  //�輼:���(������)
	case 22:		
		sprintf(szMoji, "Roar");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_ROAR;
		break;
#endif

#ifdef _SKILL_SELFEXPLODE //�Ա�
	case 23:		
		sprintf(szMoji, "SelfExplode");
		d1 = 5;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_selfexplod; //����
		break;
#endif

#ifdef _ATTDOUBLE_ATTACK	//	andy_add
	case 25:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_land_att;
		break;
	case 26:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_water_att;
		break;
	case 27:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_fire_att;
		break;
	case 28:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_wind_att;
		break;
#endif
	case 29:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_tooth;
		break;
	case 30:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_mic_def;
		break;
	case 31:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ironwall;
		break;

	case 32:	//����ת��
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_earth;
		break;
	case 33:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_water;
		break;
	case 34:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_fire;
		break;
	case 35:
		sprintf(szMoji, "No");
		d1 = 2;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_ch_wind;
		break;
	case 36:	//andy_add �غϲ�Ѫ
		memset(szMoji, 0, sizeof(szMoji));
		memset(szMojiP, 0, sizeof(szMojiP));
		if (ATR_INT_WORK0(a0) != 0)
		{
			sprintf(szMoji, "%4d", ATR_INT_WORK0(a0));
			sprintf(szMojiP, "%4d", ATR_INT_WORKp(a0));
		}
		if (ATR_MPDAMAGE(a0) != 0)
			sprintf(szMojMp, "%4d", ATR_MPDAMAGE(a0));
		d2 = FONT_PAL_YELLOW;
		d1 = FONT_PAL_GREEN;
		break;
	case 37:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf(szMoji, "�ر� %s", (ATR_INT_WORK0(a0) > 0) ? "����":"�½�");
		d1 = FONT_PAL_GREEN;
		break;
	case 38:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf(szMoji, "�� %s%d��", (ATR_INT_WORK0(a0) > 0) ? "����":"�½�", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
	case 39:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf(szMoji, "�� %s%d��", (ATR_INT_WORK0(a0) > 0) ? "����":"�½�", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
	case 40:
		memset(szMoji, 0, sizeof(szMoji));
		sprintf(szMoji, "�� %s%d��", (ATR_INT_WORK0(a0) > 0) ? "����":"�½�", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
#ifdef _SYUTEST
	case 41:
		sprintf(szMoji, "LightTake");
		d1 = 5;		
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = SPR_lightget;//����
		break;
#endif
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
	case 41:
		sprintf(szMoji, "LightTake");
		d1 = 5;		
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = AttNextMagicNum;//����
#ifdef _WAVETRACK					// (���ɿ�) Syu ADD ����
		if (AttNextMagicNum == 101703 && BeAttNum >= 15 && BeAttNum <= 19)
		{
			if (BeAttNum == 15)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 1 ; 
			else if (BeAttNum == 17)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 2  ; 
			else if (BeAttNum == 19)
				ATR_H_POS(a0) = ATR_H_POS(a0) - 3  ; 
			else if (BeAttNum == 18)
				ATR_V_POS(a0) = ATR_V_POS(a0) - 1  ; 
		}
		if (AttNextMagicNum == 101704 && BeAttNum >= 5 && BeAttNum <= 9)
		{
			if (BeAttNum == 5)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 441;
					ATR_V_POS(a0) = 322;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 1;
			}
			else if (BeAttNum == 6)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 377;
					ATR_V_POS(a0) = 370;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 2;
			}
			else if (BeAttNum == 7)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 505;
					ATR_V_POS(a0) = 274;
				}
			}
			else if (BeAttNum == 8)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 313;
					ATR_V_POS(a0) = 418;
				}
				else
					ATR_H_POS(a0) = ATR_H_POS(a0) + 3;
			}
			else if (BeAttNum == 9)
			{
				if (ATR_CHR_CNT(a0) == 0)
				{
					ATR_H_POS(a0) = 569;
					ATR_V_POS(a0) = 226;
				}
				else
					ATR_V_POS(a0) = ATR_V_POS(a0) + 1;
			}
		}
#endif
		break;
//#endif

#ifdef _EQUIT_ARRANGE
	case 42:		//������
		sprintf(szMoji, "Guard");
		d1 = 3;		//��ɫSet
		ATR_ATTRIB(a0) = 0;
		ATR_CHR_NO(a0) = CG_ICON_GUARD;
		break;
#endif

#ifdef _SKILL_ADDBARRIER		// Change �輼:Ϊħ�������쳣���Թ���
	case 43:
		memset(szMoji, 0, sizeof(szMoji));
		if (ATR_INT_WORK0(a0) == 0)
			sprintf(szMoji, "�쳣���Իظ�");
		else
			sprintf(szMoji, "�쳣���� %s%d��", (ATR_INT_WORK0(a0) > 0) ? "����":"�½�", ATR_INT_WORK0(a0));
		d1 = FONT_PAL_GREEN;
		break;
#endif
#ifdef _PETSKILL_PEEL
	case 44:
		{
			char *tempstr[] = { "ͷ��", "����", "����", "����Ʒ", "����Ʒ", "����", "����", "Ь��", "����"};
			memset(szMoji, 0, sizeof(szMoji));
			if (ATR_INT_WORK0(a0) > 8)
				break;
			sprintf(szMoji, "ж�� %s װ��", tempstr[ATR_INT_WORK0(a0)]);
			d1 = FONT_PAL_GREEN;
		}
		break;
#endif
	}

	switch (ATR_INT_WORK1(a0))
	{
	case 36:	//andy_add �غϲ�Ѫ
		if (ATR_RIDE_FLAG(a0) == 1)
			StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0) + 32, FONT_PRIO_BACK, d1, szMojiP, 0);
	case 37:
	case 38:
	case 39:
	case 40:
#ifdef _SKILL_ADDBARRIER
	case 43:
#endif
#ifdef _PETSKILL_PEEL
	case 44:
#endif
		StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0), FONT_PRIO_BACK, d1, szMoji, 0);
		if (ATR_MPDAMAGE(a0) != 0)
			StockFontBuffer(ATR_H_POS(a0) - 20, ATR_V_POS(a0) + 12, FONT_PRIO_BACK, d2, szMojMp, 0);
		break;
	case 6:
	case 14:
	case 15:
	case 16:
		//andy_mp
		if (ATR_ADDHPFLG(a0) == 1)//��Ѫ��
		{
			StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0), FONT_PRIO_BACK, FONT_PAL_GREEN, szMojHp, 0);
			break;
		}
		d0 = GetStrWidth(szMoji) >> 1;
		StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0), FONT_PRIO_BACK, d1, szMoji, 0);

		if (ATR_MPDAMAGE(a0) != 0 || ATR_MPDFLG(a0) == 1)//�˺�
			StockFontBuffer(ATR_H_POS(a0) - d0, ATR_V_POS(a0) + 12, FONT_PRIO_BACK, FONT_PAL_YELLOW, szMojMp, 0);
		if (ATR_RIDE_FLAG(a0) == 1)
		{
			dx = GetStrWidth(szMojiP) >> 1;
			StockFontBuffer(ATR_H_POS(a0) - dx, ATR_V_POS(a0) + 60, FONT_PRIO_BACK, d1, szMojiP, 0);
		}
		break;
	default:
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
	}
}

//�˺�ֵ�趨����
void set_damage_num(ACTION *a0 ,int color, int v_pos)
{
	ACTION *a1;
	a1 = GetAction(T_PRIO_DAMAGE_NUM, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	/* ��??? */
#ifdef _WAVETRACK					// (���ɿ�) Syu ADD ����
	BeAttNum = a0->hitDispNo ; 
#endif
	ATR_NAME(a1) = damage_num;
	//��ʾ���ȶ�
	ATR_DISP_PRIO(a1) = D_PRIO_DAMAGE_NUM;
	/* ???�t */
	ATR_H_POS(a1) = ATR_H_POS(a0);
	ATR_V_POS(a1) = ATR_V_POS(a0) + v_pos;
	ATR_SPD(a1) = DAMAGE_SPD;
	ATR_ATTRIB(a1) = ACT_ATR_HIDE;
	//Syu mark �������и���ɫ����û��ϵ����damage_num�Ĵ��룬damage_num��switch�Ż����ɫ��
	ATR_INT_WORK1(a1) = color;		//������ɫ�趨
	if (ATR_ADDHPFLG(a0) == 1)
	{
		ATR_ADDHP(a1) = ATR_ADDHP(a0);
		ATR_ADDHPFLG(a1)= ATR_ADDHPFLG(a0);
	}
	int dddd = ATR_DAMAGE(a0);
	ATR_INT_WORK0(a1) = ATR_DAMAGE(a0);		//�˺��趨
	ATR_INT_WORKp(a1) = ATR_PET_DAMAGE(a0);		//�˺��趨
	ATR_RIDE_FLAG(a1) = ATR_RIDE(a0);
	//andy_mp
	ATR_MPDAMAGE(a1) = ATR_MPDAMAGE(a0);
	ATR_MPDFLG(a1) = ATR_MPDFLG(a0);

	switch (ATR_INT_WORK1(a1))
	{
	case 0:
	case 36:
	case 37:
	case 38:
	case 40:
#ifdef _SKILL_ADDBARRIER
	case 43:
#endif
#ifdef _PETSKILL_PEEL
	case 44:
#endif
	case 6:
	case 14:
	case 15:
	case 16:
	case 19:
		break;
	default:
#ifdef _SKILL_SELFEXPLODE //�Ա�
		if (ATR_SELFEXPLODE(a0) == 1)
			ATR_V_POS(a1) = ATR_V_POS(a0) - SpriteInfo[a0->bmpNo].height / 2 + 20;
		else
#endif
#ifdef _SYUTEST
		if (color == 41)
			ATR_V_POS(a1) = ATR_V_POS(a0) + v_pos;
		else
			ATR_V_POS(a1) = ATR_V_POS(a0) - 80;
#else
		ATR_V_POS(a1) = ATR_V_POS(a0) - 80;
#endif
	}
#ifdef _PETSKILL_LER
	// �׶�����ʱ�ķ�����
	if (color == 3 && ATR_CHR_NO(a0) == 101815)
	{
		ACTION *a2;
		a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a2 == NULL)
			return;
		ATR_NAME(a2) = katino;
		ATR_BODY_WORK(0, a2) = a0;
		ATR_JUJUTSU_WORK(a0) = a2;
		ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a0) + 1;
		ATR_H_POS(a2) = ATR_H_POS(a0) + 10;
		ATR_V_POS(a2) = ATR_V_POS(a0) + 10;
		if (g_bUseAlpha)
			ATR_CHR_NO(a2) = 101805;
		else
			ATR_CHR_NO(a2) = 101858;
	}
#endif
}

/* ??????��?? *******************************************************************/
#if 0
�����w��
����������	����
����������	צ
����������  ��
����������	����
����������	��
����������	��
����������	��Τ�Ͷ����
����������	���g

�����w�Фʤ�
����������	����
����������	צ
����������  ��
����������	����
����������	��
����������	��
����������	��Τ�Ͷ����
����������	���g
#endif
void play_damage(int no, int x)
{
	//??��???��????
	if (no >= 10100)
		no -= 100;
	//Ч�������
	switch (no)
	{
	case 10000:		//����
		no = 250;
		break;
	case 10001:		//צ
		no = 254;
		break;
	case 10002:		//��
		no = 251;
		break;
	case 10003:		//����
		no = 251;
		break;
	case 10004:		//ǹ
		no = 252;
		break;
	case 10005:		//��
		no = -1;
		break;
	case 10006:		//Ͷ����
		no = -1;
		break;
	case 10007:		//����
		no = 254;
		break;
	default:
		no = 250;
	}
	//???��????
	if (no != -1)
		play_se(no, x, 240);
}

/* ??����?? *******************************************************************/
void disp_kanji(ACTION *a0)
{
	int d1, d7;

	ACTION *a1;
	a1 = ATR_BODY_WORK(0, a0);		//?????
	if (!--ATR_INT_WORK0(a0)){		//��ʾ�r�g�Ф�ʤ�
		DeathAction(a1);		//????��
		DeathAction(a0);		//��?
		p_kanji = NULL;
		ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
	}
	else
	{
		if (ATR_LIFE(a1)){		//��ʾ�r�g�ʤ�
			for (d1 = 4 + 13, d7 = 0; d7 < 4; d1 += 18, d7++){
				if (kanji_buf[d7][0]){		//�������Ф�ʤ�
					StockFontBuffer(640 - 4 + - 64 * 4 + 24, d1, FONT_PRIO_FRONT, FONT_PAL_WHITE, kanji_buf[d7], 0);
				}
			}
		}
	}
}

/* ?��???? *******************************************************************/
void magic_effect(ACTION *a0)
{
	ACTION *a1;

	switch (ATR_VCT_NO(a0)){
	case 0:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP)){	//���˥�K�ˤʤ�
			ATR_VCT_NO(a0)++;
			ATR_CHR_ACT(a0)++;
		}
		break;
	case 1:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP)){	//���˥�K�ˤʤ�
			ATR_VCT_NO(a0)++;
			ATR_CHR_ACT(a0)++;
		}
		break;
	case 2:
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP)){	//���˥�K�ˤʤ�
			DeathAction(a0);		//��?
			return;
		}
		break;
	}
	a1 = ATR_BODY_WORK(0, a0);		//?������
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1);
}

//HIT_MARK���� ( ��ɱ )
void hit_mark_critical(ACTION *a0)
{
	if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) >> 1){		//���˥�ʤ�
		ATR_CHR_NO(a0)--;
	}
	if (--ATR_STIMER(a0)){
		gemini(a0);
		pattern(a0, 0, 0);
	}
	else {
		DeathAction(a0);		//����
		return;
	}
}
//HIT_MARK����
void hit_mark(ACTION *a0)
{
	//��������ATR_KAISHINΪtrue
	if (ATR_KAISHIN(a0)){
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 1 / 3){		//���˥�ʤ�
			ATR_CHR_NO(a0)--;
		}
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 2 / 3){		//���˥�ʤ�
			ATR_CHR_NO(a0)--;
	}
	}
	else {
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 1 / 3){		//���˥�ʤ�
			ATR_CHR_NO(a0)--;
		}
		if (ATR_STIMER(a0) == ATR_FIRST_FLG(a0) * 2 / 3){		//���˥�ʤ�
			ATR_CHR_NO(a0)--;
		}
	}
	if (--ATR_STIMER(a0)){
		pattern(a0, 0, 0);
	}
	else {
		DeathAction(a0);		//����
		return;
	}
}

void set_hit_mark(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0;

	//��ɱʱ����������ǣ������ǲ��Ǳ�ɱ�����������ǲ�break
	for (d0 = 5; d0 > 0; d0--)
	{
		//�趨HIT_MARK
		a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
		if (a1 == NULL)
			return;
		/* ��??? */
		ATR_NAME(a1) = hit_mark;
		//��ʾ���ȶ�
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		//���豻���������������ɫͼ
		if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
			//�趨ͼ��
			ATR_CHR_NO(a1) = CG_HIT_MARK_12;		//��ɫ
		else
			//�趨ͼ��
			ATR_CHR_NO(a1) = CG_HIT_MARK_22;		//��ɫ
		//����λ������
		a2 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a1) = ATR_H_POS(a2);
		ATR_V_POS(a1) = ATR_V_POS(a2) - 32;
		ATR_SPD(a1) = Rnd(4, 7);
		ATR_CRS(a1) = d0 * 6;
		if (ATR_HIT_STOP(a0) == HIT_STOP_TIM){		//�ҥåȥ��ȥåץ��å�
			ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = ATR_HIT_STOP(a0);		//��ʾʱ���趨
		}
		else {
			ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = ATR_HIT_STOP(a0);		//��ʾʱ���趨
			ATR_KAISHIN(a1) = 1;
		}
		//��ɱʱ������һ�����ǣ��Ǳ�ɱ����
		if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
		{
			ATR_NAME(a1) = hit_mark_critical;
			ATR_CHR_NO(a1) = CG_HIT_MARK_01;
			
		}
		else
			break;
	}
}

/* �\?����???����?? *******************************************************************/
void disp_guard_mark(ACTION *a0)
{
	ACTION *a1;
	a1 = ATR_BODY_WORK(0, a0);
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1) - 32;
	if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//???��???
		DeathAction(a0);		//��?
}

void set_guard_mark(ACTION *a0)
{
	ACTION *a1;

	a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	ATR_NAME(a1) = disp_guard_mark;
	ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
	if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
		ATR_CHR_NO(a1) = SPR_mirror;
	else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)
		ATR_CHR_NO(a1) = SPR_barrior;
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
	else if (ATR_ATTACK_KIND(0, a0) & ATT_TRAP)
	{		//����
		if ( a0->hitDispNo >= 10 && a0->hitDispNo <= 19 )
			ATR_CHR_NO(a1) = 101630;
		else if ( a0->hitDispNo >= 0 && a0->hitDispNo <= 9 )
			ATR_CHR_NO(a1) = 101696;
	#ifdef _PETSKILL_BATTLE_MODEL
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			if (ATR_GROUP_FLG(a0) == 0) ATR_CHR_NO(a1) = 101696;
			else ATR_CHR_NO(a1) = 101630;
		}
	#endif
	}
#endif
#ifdef _PETSKILL_ACUPUNCTURE
	else if (ATR_ATTACK_KIND(0, a0) & ATT_ACUPUNCTURE)
        //Ŀǰ����ʾЧ��ͼ
		ATR_DAMAGE(a0) = ATR_ATTACK_POW(0, a0)*2;//�趨�˺�ֵ
#endif
#ifdef _PET_ITEM
	else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
		ATR_DAMAGE(a0) = ATR_ATTACK_POW(1, a0);
#endif
	else
		ATR_CHR_NO(a1) = SPR_kyusyu;

	ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
}

void set_jujutsu_hit_mark( ACTION *a0 )
{
	ACTION *a1;

	//?????????
	a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
	if (a1 == NULL)
		return;
	/* ��??? */
	ATR_NAME(a1) = hit_mark;
	/* ����?�I�T */
	ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
	/* ?????�k? */
	ATR_CHR_NO(a1) = CG_HIT_MARK_32;		//?��
	/* ???�t */
	ATR_H_POS(a1) = ATR_H_POS(a0);
	ATR_V_POS(a1) = ATR_V_POS(a0) - 32;
	ATR_FIRST_FLG(a1) = ATR_STIMER(a1) = HIT_STOP_TIM;		//����?????
}

static int piyo_loop_v_tbl[] = {
	0,1,2,3,4,4,5,6,7,8,8,9,9,9,9,10,9,
	9,9,9,8,8,7,6,5,5,4,3,2,1,0,-1,-2,
	-3,-4,-4,-5,-6,-7,-8,-8,-9,-9,-9,-9,-10,-9,-9,-9,
	-9,-8,-8,-7,-6,-5,-5,-4,-3,-2,-1,
};
static int piyo_loop_h_tbl[] = {
	-24,-23,-23,-22,-21,-20,-19,-17,-16,-14,-11,-9,-7,-4,-2,0,2,
	4,7,9,11,14,16,17,19,20,21,22,23,23,24,23,23,
	22,21,20,19,17,16,14,12,9,7,4,2,0,-2,-4,-7,
	-9,-11,-14,-16,-17,-19,-20,-21,-22,-23,-23,
};

//??????????  ?????��??  ??????????
void piyo_loop(ACTION *a0)
{
	ACTION *a1;
	int d0;

	a1 = ATR_BODY_WORK(0, a0);		//�o�l??????
	if (ATR_NAME(a1) == NULL || ATR_LIFE(a1) != 0){		//�ԥ�ԥ�K�ˤʤ�
		DeathAction(a0);		//��?
		return;
	}
	/* ����?�I�T */
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	d0 = piyo_loop_h_tbl[ATR_STIMER(a0)];
	ATR_H_POS(a0) = ATR_H_POS(a1) + d0 + ATR_INT_WORK0(a0);
	d0 = piyo_loop_v_tbl[ATR_STIMER(a0)];
	ATR_V_POS(a0) = ATR_V_POS(a1) + d0 + ATR_INT_WORK1(a0);
	ATR_STIMER(a0)++;
	if (ATR_STIMER(a0) == 60)
		ATR_STIMER(a0) = 0;
	pattern(a0,0,0);
}

//??????????  ?????????  ??????????
#define PIYOPIYO_CNT	3
void set_piyo_loop(ACTION *a0)
{
	ACTION *a1;
	int d7;

	for (d7 = 0; d7 < PIYOPIYO_CNT; d7++){
		//�ҥåȥީ`�����å�
		//?????????
		a1 = GetAction(T_PRIO_HIT_MARK, sizeof(ATR_EQU));
		if (a1 == NULL)
			return;
		/* ��??? */
		ATR_NAME(a1) = piyo_loop;
		ATR_CHR_NO(a1) = SPR_star;
		/* ???�t */
		ATR_BODY_WORK(0, a1) = a0;
		ATR_STIMER(a1) = d7 * (60 / PIYOPIYO_CNT);
		//��?��??�h
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_x + SpriteInfo[ATR_PAT_NO(a0)].width / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_x + g_lpRealAdrn[ATR_PAT_NO(a0)].width / 2;
#endif
		ATR_INT_WORK1(a1) = a0->anim_y;
	}
}

/* ?��?? *******************************************************************/
void katino(ACTION *a0)
{
	ACTION *a1;

	a1 = ATR_BODY_WORK(0, a0);		//��������??????????
	if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) == VCT_NO_DIE + 2 || ATR_LIFE(a1) == 0)		//��???
	{
		ATR_JUJUTSU_WORK(a1) = NULL;		//��?
		DeathAction(a0);		//��?
		return;
	}
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
#ifdef _FIXSTATUS					// (���ɿ�) Syu ADD ����ս��״̬��ʾ��ʽ
	if (ATR_CHR_NO(a0) == SPR_shock)			//���
		ATR_V_POS(a0) = ATR_V_POS(a1);
	else if (ATR_CHR_NO(a0) == 35120)		//��������
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 101702)	//��Sars����
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 27692)		//��������������
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 35110)		//���޵���
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == 26517)		//����
		ATR_V_POS(a0) = ATR_V_POS(a1) - 34 ;
	else if (ATR_CHR_NO(a0) == CG_HIT_MARK_00)
	{	//��ѣ
		StarLoop++; 
		if (StarLoop <= 20 && StarLoop > 0) 
			ATR_V_POS(a0) = ATR_V_POS(a1) - 46;
		else if (StarLoop <= 40 && StarLoop > 20)
			ATR_V_POS(a0) = ATR_V_POS(a1) - 56;
		else if (StarLoop <= 60 && StarLoop > 40)
		{
			ATR_V_POS(a0) = ATR_V_POS(a1) - 56;
			ATR_H_POS(a0) = ATR_H_POS(a0) + 16;
		}
		else if (StarLoop <= 80 && StarLoop > 60)
		{
			ATR_V_POS(a0) = ATR_V_POS(a1) - 46;
			ATR_H_POS(a0) = ATR_H_POS(a0) + 16;
		}
		if (StarLoop >= 80)
			StarLoop = 0 ; 
	}
#ifdef _PETSKILL_LER
	// �׶�����ʱ��������λ�ò���
	else if (ATR_CHR_NO(a0) == 101810 || ATR_CHR_NO(a0) == 101811 || ATR_CHR_NO(a0) == 101863 || ATR_CHR_NO(a0) == 101864);
	// �׶�����ʱ�ķ����ܶ���λ��Ҫ��
	else if (ATR_CHR_NO(a0) == 101805 || ATR_CHR_NO(a0) == 101858)
	{
		ATR_H_POS(a0) = ATR_H_POS(a1) + 10;
		ATR_V_POS(a0) = ATR_V_POS(a1) + 10;
	}
#endif
	else 
		ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
#else
	if (ATR_CHR_NO(a0) == SPR_shock)		//????
		ATR_V_POS(a0) = ATR_V_POS(a1);
	else
		ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
#endif
#ifdef _PETSKILL_LER
	// ������Ϊ��������ʱ
	if (ATR_CHR_NO(a0) == 101810 || ATR_CHR_NO(a0) == 101811 || ATR_CHR_NO(a0) == 101805 ||
		 ATR_CHR_NO(a0) == 101863 || ATR_CHR_NO(a0) == 101864 || ATR_CHR_NO(a0) == 101858)
	{
		// ������������,�������
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			ATR_JUJUTSU_WORK(a1) = NULL;
			DeathAction(a0);
			if (ATR_CHR_ACT(a1) == ANIM_DEAD && ATR_CHR_NO(a1) == 101813)
				ATR_CHR_NO(a1) = 101814;
			else if (ATR_CHR_ACT(a1) == ANIM_DEAD && ATR_CHR_NO(a1) == 101814)
				ATR_CHR_NO(a1) = 101815;
			ATR_VCT_NO(a1) = 0;
			int i;
			for (i = 0; i < 20; i++)
			{
				// �ҳ��׶�����һ��
				if (ATR_CHR_NO(p_party[i]) == ATR_CHR_NO(a1))
					break;
			}
			a1->dir = ATR_CHR_ANG(a1) = i < 10 ? 3:7;
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		}
	}
	else 
#endif
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);		//???
}

//��������������״̬����
void set_single_jujutsu(int d0, ACTION *a1)
{
	if (ATR_LIFE(a1) == 0){		//����Ǥ�ʤ�
		return;
	}

	ACTION *a2;

	a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
	if (a2 == NULL)
		return;
	/* ��??? */
	ATR_NAME(a2) = katino;
	ATR_BODY_WORK(0,a2) = a1;
	ATR_JUJUTSU_WORK(a1) = a2;
	/* ����?�I�T */
	ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a1) + 1;
	/* ???�t */
	ATR_H_POS(a2) = ATR_H_POS(a1);
	ATR_V_POS(a2) = ATR_V_POS(a1) - 64;
	ATR_STIMER(a2) = d0;
	/* ?????�k? */
	switch (d0)
	{
	//  "��", 
	case 1:
		ATR_CHR_NO(a2) = SPR_poison;
		break;
	//  "??", 
	case 2:
		ATR_CHR_NO(a2) = SPR_shock;
		break;

	//  "��?", 
	case 3:
		ATR_CHR_NO(a2) = SPR_sleep;
		break;
	//  "��?", 
	case 4:
		ATR_CHR_NO(a2) = SPR_stone;
		break;
	//  "?????", 
	case 5:
		ATR_CHR_NO(a2) = SPR_drunk;
		break;
	//  "??" 
	case 6:
		ATR_CHR_NO(a2) = SPR_conf;
		break;
#ifdef _MAGIC_WEAKEN  //����
	case 7:
		ATR_CHR_NO(a2) = SPR_weaken;
		break;
#endif
#ifdef _MAGIC_DEEPPOISION   //�綾
	case 8:
		ATR_CHR_NO(a2) = SPR_deeppoison;  
		break;
#endif
#ifdef _MAGIC_BARRIER
	case 9:
		ATR_CHR_NO(a2) = SPR_barrier; //ħ�� 
		break;
#endif
#ifdef _MAGIC_NOCAST
	case 10:
		ATR_CHR_NO(a2) = SPR_nocast; //��Ĭ 
		break;
#endif

#ifdef _SARS						// WON ADD ��ɷ����
	case 11:
		ATR_CHR_NO(a2) = 101702; //��ɷ����
		break;
#endif

#ifdef _CHAR_PROFESSION						// WON ADD
	case 12:
		ATR_CHR_NO(a2) = CG_HIT_MARK_00; //��ѣ
		break;
	case 13:
		ATR_CHR_NO(a2) = 35120; //��������
		break;
	case 14:
		ATR_CHR_NO(a2) = 35110; //���޵���
		break;
	case 15:
		ATR_CHR_NO(a2) = 27692;	//������
		break;
	case 16: 
		ATR_CHR_NO(a2) = 26517; //����
		break;
	case 17:
		ATR_CHR_NO(a2) = 27692;	//����
		break;	
	case 18:
		ATR_CHR_NO(a2) = 27012; //��Ѫ��
		break;	
	case 19:
		ATR_CHR_NO(a2) = 27012; //һ���Ѫ
		break;
	case 20:
		ATR_CHR_NO(a2) = SPR_conf; //��
		break;
	case 21:
		ATR_CHR_NO(a2) = 0; //����
		break;
	case 22:
		ATR_CHR_NO(a2) = 0; //������
		break;
	case 23:
		ATR_CHR_NO(a2) = 100551;   //�׸���
		break;
#ifdef _PROFESSION_ADDSKILL
	case 32:
		ATR_CHR_NO(a2) = SPR_barrier; // ˮ����
		break;
	case 33:
		ATR_CHR_NO(a2) = SPR_shock; //�־�
    //case 24:
	//	ATR_CHR_NO(a2) = 0; // ����׸���
		break;
#endif
#endif
#ifdef _PETSKILL_LER
	case 34:
		if (ATR_CHR_NO(a1) == 101814)
		{
			// ��һ�α���
			if (g_bUseAlpha)
				ATR_CHR_NO(a2) = 101810; // ����1
			else
				ATR_CHR_NO(a2) = 101863;	// ����1
			ATR_V_POS(a2) = ATR_V_POS(a1);
			if (ATR_CHR_ACT_OLD(a1) != ANIM_DEAD)
			{
				ATR_VCT_NO(a1) = VCT_NO_DIE;
				ATR_CHR_NO(a1) = 101813;
			}
		}
		else if (ATR_CHR_NO(a1) == 101815)
		{
			// �ڶ��α���
			if (g_bUseAlpha)
				ATR_CHR_NO(a2) = 101811;	// ����2
			else 
				ATR_CHR_NO(a2) = 101864; // ����2
			ATR_V_POS(a2) = ATR_V_POS(a1);
			if (ATR_CHR_ACT_OLD(a1) != ANIM_DEAD)
			{
				ATR_VCT_NO(a1) = VCT_NO_DIE;
				ATR_CHR_NO(a1) = 101814;
			}
		}
		break;
#endif
	}
}

/* �N�ͧT������?? *******************************************************************/
void attrib_reverse(ACTION *a0)
{
	ACTION *a1;

	a1 = ATR_BODY_WORK(0, a0);		//���T����??????????
	if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) == VCT_NO_DIE + 2){		//�K�ˤʤ�
		ATR_ATTRIB_WORK(a1) = NULL;		//��?
		DeathAction(a0);		//��?
		return;
	}
	ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
	ATR_H_POS(a0) = ATR_H_POS(a1);
	ATR_V_POS(a0) = ATR_V_POS(a1) - 64;
	ATR_CHR_NO(a0) = SPR_zokusei;
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);		//???
}

/* �N�ͧT��????? *******************************************************************/
void set_attrib_reverse(ACTION *a1)
{
	ACTION *a2;

	a2 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
	if (a2 == NULL)
		return;
	/* ��??? */
	ATR_NAME(a2) = attrib_reverse;
	ATR_BODY_WORK(0,a2) = a1;
	ATR_ATTRIB_WORK(a1) = a2;
	/* ����?�I�T */
	ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a1) + 1;
	/* ?????�k? */
	ATR_CHR_NO(a2) = CG_ATR_ICON_EARTH_BATTLE;
	/* ???�t */
	ATR_H_POS(a2) = ATR_H_POS(a1);
	ATR_V_POS(a2) = ATR_V_POS(a1) - 64;
}

int boomerang_pos_tbl[] = {800, 280+40,  750, 180+40,  370+20,  86+20,         640*4/5+20, 140+30};
int boomerang_pos_tbl2[] = {750, 280,     700, 180,     320+20,  30+20,         640*2/3+20, 70+30};
//?��???????
int boomerang_pos_tbl3a[] = {260, 420,     100, 330,     640-800, 480-(280+40),  640-750,    480-(180+40)};
int boomerang_pos_tbl4a[] = {340, 450,     180, 400,     640-750, 480-280,       640-700,    480-180};
//?��?????�q?
int boomerang_pos_tbl3b[] = {260-64, 420,  100-32, 330+32,     640-800, 480-(280+40),  640-750,    480-(180+40)};
int boomerang_pos_tbl4b[] = {340   , 450,  180, 400+64,  640-750, 480-280,       640-700,    480-180};

/* ??????? *******************************************************************/
void boomerang(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;
	float dx, dy;

	if (ATR_HIT_STOP(a0)){		//�ҥåȥ��ȥå��Фʤ�
		if (--ATR_HIT_STOP(a0)){		//�ҥåȥ��ȥå��Фʤ�
			return;
		}
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:		//??����??�h
		if (ATR_GROUP_FLG(a0) == 0){		//���¥���`�פʤ�
			d0 = boomerang_pos_tbl[ATR_LONG_WORK(0, a0) * 2];
			d1 = boomerang_pos_tbl[ATR_LONG_WORK(0, a0) * 2 + 1];
		} 
		else {
			//���ϥ���`�פ�ǰ�Фʤ�
			//?��?????�q???
			if (ATR_PLACE_NO(a0) >= 15)
			{
				d0 = boomerang_pos_tbl3b[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl3b[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
			else
			{
				d0 = boomerang_pos_tbl3a[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl3a[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
		}

		if (ATR_FIRST_FLG(a0) == 0){		//�����һ�ؤʤ�
			ATR_SPD(a0) = 40;
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;
			ATR_FIRST_FLG(a0) = 1;
		}
		else
			radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));	//????
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));
		if (d0 < 20){		//���Ť����ʤ�
			ATR_VCT_NO(a0) = 1;
		}
		break;
	case 1:		//?�o?�t??�h
		if (ATR_SPD(a0) != 32){		//���ԩ`�ɥ�����
			ATR_SPD(a0)--;
		}
		if (ATR_GROUP_FLG(a0) == 0){		//���¥���`�פʤ�
			d0 = boomerang_pos_tbl2[ATR_LONG_WORK(0, a0) * 2];
			d1 = boomerang_pos_tbl2[ATR_LONG_WORK(0, a0) * 2 + 1];
		}
		else {
			//���ϥ���`�פ�ǰ�Фʤ�
			//?��?????�q???
			if (ATR_PLACE_NO(a0) >= 15)
			{
				d0 = boomerang_pos_tbl4b[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl4b[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
			else
			{
				d0 = boomerang_pos_tbl4a[ATR_LONG_WORK(0, a0) * 2];
				d1 = boomerang_pos_tbl4a[ATR_LONG_WORK(0, a0) * 2 + 1];
			}
		}
		radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));
		if (d0 < 40){		//���Ť����ʤ�
			//�Ѥ���ȡ�����
			d0 = get_num();
			if (d0 == 255){		//�K�ˤʤ�
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				DeathAction(a0);
				p_missile[0] = NULL;
				return;
			}
			else
			{
				ATR_BODY_WORK(0, a0) = p_party[d0];
				ATR_ATTACK_KIND(0, a0) = get_num();
				ATR_ATTACK_POW(0, a0) = get_num();
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();
				ATR_VCT_NO(a0) = 2;
			}
		}
		break;
	case 2:
		if (ATR_SPD(a0) != 40)
			ATR_SPD(a0)++;
		a1 = ATR_BODY_WORK(0, a0);
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar2(a0, d0, d1, ATR_LONG_WORK(1, a0));
		gemini(a0);
		dx = (float)(d0 - ATR_H_POS(a0));
		dy = (float)(d1 - ATR_V_POS(a0));
		d0 = (int)sqrt((double)(dx * dx + dy * dy));
		if (ATR_COUNTER_FLG(a0) == 1)
		{
			if (BattleMapNo >= 148 && BattleMapNo <= 150)
				d1 = 10 + 5;
			else
				d1 = 10;
		}
		else
		{
			if (BattleMapNo >= 148 && BattleMapNo <= 150)
				d1 = 20 + 10;
			else
				d1 = 20;
		}
		if (d0 < 80)
		{
			if (ATR_COUNTER_FLG(a0) == 1 && ATR_LONG_WORK(2, a0) == 0)
			{
				ATR_LONG_WORK(2, a0) = 1;
				a1 = ATR_BODY_WORK(1, a0);
				ATR_VCT_NO(a1) = 72;
				play_se(14, ATR_H_POS(a0), ATR_V_POS(a0));
			}
		}
		if (d0 < d1)
		{
			if (ATR_COUNTER_FLG(a0) == 1)
			{
				DeathAction(a0);
				p_missile[0] = NULL;
				return;
			}
			ATR_LONG_WORK(2, a0) = 0;
			a2 = ATR_BODY_WORK(0, a0);
			if (ATR_LIFE(a2) <= 0){		//����Ǥ�ʤ�
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;
				goto boomerang_200;
			}
#ifdef _PETSKILL_ACUPUNCTURE
////////////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
////////////////////////////////////
#else
////////////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
////////////////////////////////////
#endif
				set_guard_mark(a0);
			//?????????
			ATR_COMBO(a1) = 0;
			//?��??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_VCT_NO(a1) = 16;		//????
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					ATR_CRS(a1) = crs_change_tbl2[3];		//��?????????????
				else
					ATR_CRS(a1) = crs_change_tbl2[7];		//��?????????????
				ATR_DAMAGE_ANG(a1) = crs_change_tbl[ATR_CRS(a1)];		/* ????? */
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				goto boomerang_200;
			}
			//??????��??????
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//��????????
					//??����
					set_damage_num(a2, 0, -64);
					goto boomerang_200;
				}
			}
			//?��????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH){
				set_damage_num(a1, 19, -112);
			}
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1){		//���ˣϣ��ʤ�
				ATR_AKO_FLG(a1) = 1;
			}
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2){		//���ˣϣ��ʤ�
				ATR_AKO_FLG(a1) = 2;
			}
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH){		//�Ȥɤ�ʤ�
				ATR_LIFE(a1) = 0;		//?��????
			}
			if (ATR_GROUP_FLG(a0) == 0){		//���¥���`�פʤ�
				ATR_DAMAGE_ANG(a1) = 1;		// ?????
			}
			else {
				ATR_DAMAGE_ANG(a1) = 5;		// ?????
			}
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//?��?????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????��?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????��?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????��
			//?��?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//?��??????
			else
				ATR_KAISHIN(a1) = 0;		//�G��?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?��????
			{
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//?��??????
			}
			if (ATR_KAISHIN(a1))		//?��?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//???????????��??
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)	//?????����
						set_damage_num(a0, 1, -64 + 16);
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
boomerang_200:
			ATR_HIT_STOP(a0) /= 4;		//???????��???
			//��?����?��?
			d0 = get_num();
			if (d0 == 255)		//��???
			{
				ATR_BODY_WORK(0, a0) = ATR_BODY_WORK(1, a0);		//�D����
				ATR_COUNTER_FLG(a0) = 1;		//��???????
			}
			else
			{
				ATR_BODY_WORK(0, a0) = p_party[d0];		//��?��
				ATR_ATTACK_KIND(0, a0) = get_num();		//?????
				ATR_ATTACK_POW(0, a0) = get_num();		//???
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet???
			}
		}
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

static char stick_ang_tbl[] = {8,10,12,14,0,2,4,6};
/* ?��???? *******************************************************************/
void stick_bow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);		//�o�l??????
	if (ATR_NAME(a1) == NULL){		//����K�ˤʤ�
		DeathAction(a0);		//��?
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???��
		return;
	}

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//��?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//��???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0){		//�ϕN�ʤ�
			if (ATR_SPD(a0) == 0){		//�½��_ʼ�ʤ�
				ATR_CRS(a0) = 16;
			}
			else {
				ATR_SPD(a0) -= 1;
			}
		}
		else {
			ATR_SPD(a0) += 1;
		}
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
	case 2:
	case 3:
		a1 = ATR_BODY_WORK(0, a0);		//��???????
#if 0
		if(ATR_VCT_NO(a0) == 2){		//�����ƄӤʤ�
			if(ATR_VCT_NO(a1) >= VCT_NO_DIE + 1){		//����ʼ�᤿�ʤ�
				DeathAction(a0);		//��?
				return;
			}
		}
		else
		{
			if(ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0){		//���C��������ʤ�
				if(ATR_VCT_NO(a1) == 0){		//���C�ʤ�
					DeathAction(a0);		//��?
					ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???��
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
#else
		if (ATR_VCT_NO(a0) == 2){		//�����ƄӤʤ�
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE + 1){		//����ʼ�᤿�ʤ�
				DeathAction(a0);		//��?
				return;
			}
		}
		else if (ATR_VCT_NO(a0) == 3)
		{
			if (ATR_VCT_NO(ATR_BODY_WORK(1, a0)) == 0){		//���ĽK�ˤʤ�
				DeathAction(a0);		//��?
				return;
			}
		}
		else
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0){		//���C��������ʤ�
				ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???��
				if (ATR_VCT_NO(a1) == 0){		//���C�ʤ�
					//					DeathAction( a0 );		//�K��
					ATR_VCT_NO(a0) = 3;
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
#endif
		ATR_H_POS(a0) = ATR_H_POS(a1) + ATR_INT_WORK0(a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK1(a0);
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a1) + 4) & 7;		//?????
		ATR_CHR_NO(a0) = CG_ARROW_00 + stick_ang_tbl[ATR_CHR_ANG(a0)];		//
		ATR_H_MINI(a0) = 0;		//?������
		ATR_V_MINI(a0) = 0;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		gemini(a0);
		gemini(a0);
		if (ATR_CHR_ANG(a1) == 3 || ATR_CHR_ANG(a1) == 4 || ATR_CHR_ANG(a1) == 5)		//??????
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1);		//����?�I???
		else
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;		//����?�I???
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
void shoot(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);
	if (ATR_NAME(a1) == NULL)
	{
		DeathAction(a0);
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;
		return;
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0){		//�ϕN�ʤ�
			if (ATR_SPD(a0) == 0)
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
	case 2:
	case 3:
		a1 = ATR_BODY_WORK(0, a0);
		if (ATR_VCT_NO(a0) == 2)
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE + 1)
			{
				DeathAction(a0);
				return;
			}
		}
		else if (ATR_VCT_NO(a0) == 3)
		{
			if (ATR_VCT_NO(ATR_BODY_WORK(1, a0)) == 0)
			{
				DeathAction(a0);	
				return;
			}
		}
		else
		{
			if (ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)
			{
				ATR_STIMER(ATR_BODY_WORK(1, a0))++;		
				if (ATR_VCT_NO(a1) == 0)
				{
					DeathAction(a0);
					ATR_VCT_NO(a0) = 3;
					return;
				}
				else
				{
					ATR_VCT_NO(a0) = 2;
					break;
				}
			}
		}
		ATR_H_POS(a0) = ATR_H_POS(a1) + ATR_INT_WORK0(a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK1(a0) + 32 + (a0->anim_cnt * 4);
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a1) + 4) & 7;
		if (a0 ->anim_cnt == 5)
			ATR_CHR_NO(a0) = 0;
		else if (ATR_CHR_NO(a0) != 0)
			ATR_CHR_NO(a0) = 101611;
		ATR_H_MINI(a0) = 0;		
		ATR_V_MINI(a0) = 0;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a1)];
		gemini(a0);
		gemini(a0);
		if (ATR_CHR_ANG(a1) == 3 || ATR_CHR_ANG(a1) == 4 || ATR_CHR_ANG(a1) == 5)
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1);
		else
			ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}
#endif

/* ??�h?? *******************************************************************/
void bow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//��????????
			if (ATR_LIFE(a2) <= 0){		//����Ǥ�ʤ�
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
///////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
///////////////////////////////
#else
///////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
///////////////////////////////
#endif
				set_guard_mark(a0);
			ATR_COMBO(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//��?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}

			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//��????????
					set_damage_num(a2, 0, -64);		//??����
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					break;
				}
			}
			//?��????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				//?????����
				set_damage_num(a1, 19, -112);
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?��????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//?��?????
			{
				if (!ATR_COMBO(a1))			//????????
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????��?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????��
			//?��?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//?��??????
			else
				ATR_KAISHIN(a1) = 0;		//�G��?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?��????
			{
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//?��??????
			}
			if (ATR_KAISHIN(a1))		//?��?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//???????????��??
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);	//?????����
				}
				//???
				play_se(12, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_STIMER(a0) = ATR_HIT_STOP(a0) + 64;		//��??��?????
			ATR_INT_WORK0(a0) = Rnd(0, 8) - 4;		//??��?????
			ATR_INT_WORK1(a0) = Rnd(0, 8) - 4 - 28;		//??��?????!!!
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
			if (ShooterNum == 101578 )
				ATR_NAME(a0) = shoot;		//��?????
			else
				ATR_NAME(a0) = stick_bow;		//��?????
#else
			ATR_NAME(a0) = stick_bow;		//��?????
#endif
			ATR_VCT_NO(a0) = 1;				//
			ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
			ATR_V_POS(a0) -= 28;		//!!!
			ATR_CHR_ACT(a0) = 0;
			p_missile[0] = NULL;		//???????��
			ATR_SPD(a0) = 40;		//?������
			//???
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?�h
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0))++;		//???��
		DeathAction(a0);		//��?
		break;
	}
}

/* ????? *******************************************************************/
void axe(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//��?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//��???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0){		//�ϕN�ʤ�
			if (ATR_SPD(a0) == 0)		//???��??
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
		break;
	case 2:
		ATR_VCT_NO(a0)++;
		break;
	case 3:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//��?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		if (ATR_CRS(a0) == 0)		//����??
		{
			if (ATR_SPD(a0) == 0)		//???��??
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 2;
		}
		else
			ATR_SPD(a0) += 2;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 4:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		ATR_H_POS(a0) = ATR_H_POS(a1);
		ATR_V_POS(a0) = ATR_V_POS(a1);
		ATR_CHR_ACT(a0) = 1;		//��?????????
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

#ifdef _FIREHUNTER_SKILL				// (���ɿ�) ROG ADD ��ȸ����_������ɱ
void fireHunter(ACTION *a0)
{
	ACTION *a1, *a2;
	int i = 0;
	int j = 0;
	int d0, d1;

	a1 = ATR_BODY_WORK(0, a0);		//���˵�λַ

	switch (ATR_VCT_NO(a0))
	{
	case 0:								//�������
		if (bFireInit)
		{
			int EnemyPos[4][2] = {{403 , 422}, {223 , 388},{7  , 198},{19  , 318},};
			int refPos, difPosH, difPosV;
			int interval = 0;
			int difPos_v = 0;   //������ɱλַ����
			int difPos_h = 0;   //������ɱλַ����
			int j = 1;

			ATR_INT_WORK2(a0) = ATR_H_POS(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);
			for (i = 2; i < 8 ; i++)
			{
				p_missile[i] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
				ATR_INT_WORK2(p_missile[i]) = ATR_H_POS(a0);
				ATR_INT_WORK3(p_missile[i]) = ATR_V_POS(a0);
				ATR_CHR_NO(p_missile[i]) = 101734;	 //ͼ��
				ATR_CHR_ANG(p_missile[i]) = ATR_CHR_ANG(a0);
				ATR_CRS(p_missile[i]) = ATR_CRS(a0);
				ATR_SPD(p_missile[i]) = 60;
			}
			if (iBeAttNum < 5)
			{
				refPos = 0;
				difPosH = 32;
				difPosV = 24;
			}
			else if (iBeAttNum < 10)
			{
				refPos = 1;
				difPosH = 64;
				difPosV = 48;
			}
			else if (iBeAttNum < 15)
			{
				refPos = 2;
				difPosH = 32;
				difPosV = 24;
			}
			else if (iBeAttNum < 20)
			{
				refPos = 3;
				difPosH = 64;
				difPosV = 48;
			}
			for (i = 1; i < 9; i++)
			{
				p_missile[i + 7] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
				ATR_H_POS(p_missile[i + 7]) = EnemyPos[refPos][0] + difPosH * i;
				ATR_V_POS(p_missile[i + 7]) = EnemyPos[refPos][1] - difPosV * i;
				ATR_CHR_NO(p_missile[i + 7]) = 101735;	 //ͼ��
			}
			ATR_STIMER(a0) = 80;
			
			if (iBeAttNum == 18 || iBeAttNum == 11 || iBeAttNum == 19)
				interval = ATR_GROUP_FLG(a0) / 7;
			else			
				interval = ATR_GROUP_FLG(a0) / 6;
			for (i = 0; i < ATR_GROUP_FLG(a0); i++)
			{
				d0 = ATR_H_POS(a1);
				d1 = ATR_V_POS(a1);
				radar(a0, &d0, &d1);	//�״�(����·��)
				ATR_CRS(a0) = d0;		//����·��
				gemini(a0);
				if (i == interval * j)
				{
					switch (iBeAttNum)		//������ɱλַ����
					{
					case 10:
					case 12:
						difPos_v += 7;  //����
						difPos_h += 2;	//�Ƕ�
						break;
					case 14:
						difPos_v += 10;
						difPos_h += 2;
						break;
					case 15:
						difPos_v += 4;
						break;
					case 16:
						difPos_v += 5;
						difPos_h -= 5;
						break;
					case 17:
						difPos_v += 3;
						break;
					case 18:
						difPos_v += 5;
						difPos_h -= 5;
						break;
					case 19:
						difPos_v += 17;
						difPos_h -= 2;
						break;
					case 11:
						difPos_v += 5;
						difPos_h -= 5;
						break;
		/*			case 8:
						difPos_v += 7;
						difPos_h -= 5;
						break;
					case 5:
						difPos_v +=7;
						difPos_h -=10;
						break;*/
					}
					if (iBeAttNum < 10)
					{
						//difPos_h -= 10;//�Ƕ�
						difPos_v += 10;  //����
					}
					ATR_V_POS(p_missile[j+1]) = ATR_V_POS(a0) - difPos_v;
					ATR_H_POS(p_missile[j+1]) = ATR_H_POS(a0) + difPos_h;
					if (j < 6)
						j++;
				}
			}
			ATR_V_POS(a0) -= difPos_v;
			ATR_H_POS(a0) += difPos_h;
			bFireInit = false;
		}
	//׼������
		if (!FireSkillEnd)
		{
#ifdef _PETSKILL_ACUPUNCTURE
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
/////////////////////////////
#else
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
/////////////////////////////
#endif
				set_guard_mark(a0);		//������
			ATR_COMBO(a1) = 0;   //���
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)  //����
			{
				for (i = 0; i < ATR_BODY_CNT(a0); i++) //����ȫ������
				{
					if ((i + 2) != tarpos)
					{
						a2 = ATR_BODY_WORK(i + 2, a0);
						ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
						j = ATR_DAMAGE(a2);
						ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
						ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
						ATR_STIMER(a2) = 0;
						ATR_FIRST_FLG(a2) = 0;		
					}
				}
				ATR_HIT_STOP(a0) = 32;				//����ֹͣ
				ATR_VCT_NO(a0) = 1;					//ֹͣ
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//���ܶ���
				ATR_CRS(a1) = ATR_CRS(a0);		//·��
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		
				break;
			}
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					//�˺�
					set_damage_num(a1, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//����ֹͣ
					ATR_VCT_NO(a0) = 1;		//����ֹͣ
					for (i = 0; i < ATR_BODY_CNT(a0); i++) //����ȫ������
					{
						if ((i + 2) != tarpos)
						{
							a2 = ATR_BODY_WORK(i + 2, a0);
							ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
							j = ATR_DAMAGE(a2);
							ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
							ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
							ATR_STIMER(a2) = 0;
							ATR_FIRST_FLG(a2) = 0;		
						}
					}
					break;
				}
			}
			//ѹ��
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				//ѹ��
				set_damage_num(a1, 19, -112);
			//�趨KO
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//KO1
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//KO2
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//����
				ATR_LIFE(a1) = 0;		//�趨����
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//�趨�˺��Ƕ�
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//���������
			{
				if (!ATR_COMBO(a1))		//���
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		
					ATR_ATTACK_PET_POW(0, a0) = 0;		
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)			//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//�ͳ��˺�ѶϢ
/*			//����һ��
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY){
				ATR_KAISHIN(a1) = 1;		//����һ��
			} else {
				ATR_KAISHIN(a1) = 0;		//ͨ������
			}*/
			ATR_KAISHIN(a1) = 0;		//ͨ������
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//����
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//����
			{
				ATR_LONG_WORK(1, a0) = 1;		//�����趨
				//���
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//�趨����һ��
			}
			if (ATR_KAISHIN(a1))		//�趨����һ��
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//�趨����ֹͣʱ��
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//�趨����ֹͣʱ��
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//�Ƿ����
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);	//�˺���ʾ
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//��ɱ����
				set_hit_mark(a0);
			}
			ATR_HIT_STOP(a0) /= 4;		//???????��???
			ATR_VCT_NO(a0) = 3;				//
			for (i = 0; i < ATR_BODY_CNT(a0); i++) //����ȫ������
			{
				if ((i + 2) != tarpos)
				{
					a2 = ATR_BODY_WORK(i + 2, a0);
					ATR_DAMAGE(a2) = ATR_ATTACK_POW(i + 2, a0);
					j = ATR_DAMAGE(a2);
					ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(i + 2, a0);
					ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
					ATR_STIMER(a2) = 0;
					ATR_FIRST_FLG(a2) = 0;
				}
			}
		}
		else
		{
			if (ATR_STIMER(a0))
				ATR_STIMER(a0)--;

			if (p_missile[2] != NULL)
			{
				if (pattern(p_missile[2], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[2]);
					p_missile[2] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 70 && p_missile[3] != NULL)
			{
				if (pattern(p_missile[3], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[3]);
					p_missile[3] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 60 && p_missile[4] != NULL)
			{
				if (pattern(p_missile[4], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[4]);
					p_missile[4] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 50 && p_missile[5] != NULL)
			{
				if (pattern(p_missile[5], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[5]);
					p_missile[5] = NULL;
				}
			}
			if (ATR_STIMER(a0) < 40 && p_missile[6] != NULL)
			{
				if (pattern(p_missile[6], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[6]);
					p_missile[6] = NULL;
				}
			}
			if (iBeAttNum > 9)
			{
				if (ATR_STIMER(a0) < 30 && p_missile[7] != NULL)
				{
					if (pattern(p_missile[7], ANM_NOMAL_SPD, ANM_NO_LOOP))
					{
						DeathAction(p_missile[7]);
						p_missile[7] = NULL;
					}
				}
			}
			if (ATR_STIMER(a0) < 20)
			{
				if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP) && p_missile[6] == NULL)
				{
					counter = 0;
					ATR_VCT_NO(a0) = 8;
				}
			}
		}
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//����ֹͣ��
			break;
		ATR_CHR_ACT(a0) = 0;		//�ƶ�
		//����
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		if (tarpos != -1)
		{
			a2 = ATR_BODY_WORK(tarpos, a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(tarpos, a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(tarpos, a0);
			ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;
			tarpos = -1;
		}
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		p_missile[1] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//����
		p_missile[0] = NULL;
		DeathAction(a0);		//��?
		break;
	case 3:
		if (--ATR_HIT_STOP(a0))		//����ֹͣ��
			break;
	/*	if ( tarMgiDem )
		{
			a2 = ATR_BODY_WORK(2 ,a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(2 ,a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(2 ,a0);
			ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;		
			int k = ATR_DAMAGE(a2);
			set_damage_num(a2, 0, -64);
			tarMgiDem = 0;
		}*/
		if (tarpos != -1)
		{
			a2 = ATR_BODY_WORK(tarpos, a0);
			ATR_DAMAGE(a2) = ATR_ATTACK_POW(tarpos, a0);
			ATR_PET_DAMAGE(a2) = ATR_ATTACK_PET_POW(tarpos, a0);
			ATR_VCT_NO(a2) = 10;		//�ͳ��˺�ѶϢ
			ATR_STIMER(a2) = 0;
			ATR_FIRST_FLG(a2) = 0;
			tarpos = -1;
		}
		ATR_GROUP_FLG(a0) = 35;
		ATR_SPD(a0) = 16;
		ATR_VCT_NO(a0)++;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?�h
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//��???
		{
			if (ATR_LONG_WORK(1, a0))		//��???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			ATR_VCT_NO(a0)++;
		}
		break;
	case 5:
		play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) = 30;
		break;
	case 6:
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//�r???????
		{
			if (!ATR_LONG_WORK(1, a0))		//��?��?????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			DeathAction(a0);		//��?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
		}
	break;
	case 7:
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) =60;
		break;
	case 8:
		for (i = 8; i < 13; i++)
		{
			if (p_missile[i] != NULL)
			{
				if (pattern(p_missile[i], ANM_NOMAL_SPD, ANM_NO_LOOP))
				{
					DeathAction(p_missile[i]);
					p_missile[i] = NULL;
					counter++;
				}
			}
		}
		if (counter == 5)
		{
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
			FireSkillEnd = false;
			ATR_VCT_NO(a0) = 0;		
		}
		break;
	}
}
#endif

/* ?????? *******************************************************************/
void axe_shadow(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//��????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
/////////////////////////////
#else
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
/////////////////////////////
#endif
				set_guard_mark(a0);		//�\?���Ӥ���?
			//?????????
			ATR_COMBO(a1) = 0;
			//?��??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				ATR_CHR_ACT(a0) = 1;		//��?????????
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//��?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}
			//??????��??????
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//��????????
					//??����
					set_damage_num(a2, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_ACT(a0) = 1;		//��?????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
					break;
				}
			}
			//?��????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				set_damage_num(a1, 19, -112);	//?????����
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?��????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//?��?????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????��?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????��?
				}
			} 
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????��
			//?��?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//?��??????
			else
				ATR_KAISHIN(a1) = 0;		//�G��?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?��????
			{
				ATR_LONG_WORK(1, a0) = 1;		//?��??????
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//?��??????
			}
			if (ATR_KAISHIN(a1))		//?��?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//???????????��??
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);//?????����
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_HIT_STOP(a0) /= 4;		//???????��???
			ATR_VCT_NO(a0) = 3;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//???????
			ATR_VCT_NO(a1) = 1;				//????????
			ATR_CHR_ACT(a1) = 1;		//��?????????
		}
		else 
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?�h
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_CHR_ACT(a0) = 0;		//?�h???????
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
		DeathAction(a0);		//��?
		break;
	case 3:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_CHR_ACT(a1) = 0;		//?�h???????
		//?????????�h??
		ATR_GROUP_FLG(a0) = 35;
		ATR_SPD(a0) = 16;
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_VCT_NO(a1) = 2;				//?�h??
		ATR_SPD(a1) = 32;
		ATR_CRS(a1) = 0;
		ATR_VCT_NO(a0)++;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?�h
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//��???
		{
			if (ATR_LONG_WORK(1, a0))		//��???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			ATR_VCT_NO(a0)++;
		}
		break;
	case 5:
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_VCT_NO(a1) = 4;
		//???
		play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
		ATR_VCT_NO(a0)++;
		ATR_STIMER(a0) = 30;
		break;
	case 6:
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
		a1 = ATR_BODY_WORK(2, a0);		//???????
		ATR_ATTRIB(a1) = ACT_ATR_HIDE;		//����??
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//�r???????
		{
			if (!ATR_LONG_WORK(1, a0))		//��?��?????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			DeathAction(a0);		//��?
			p_missile[0] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//???????
			DeathAction(a1);		//��?
		}
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

/* ��???? *******************************************************************/
void stone(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//��?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		a2 = ATR_BODY_WORK(0, a0);		//��???????
		d0 = ATR_H_POS(a2);
		d1 = ATR_V_POS(a2);
		radar(a1, &d0, &d1);	//????
		ATR_STIMER(a0)++;
		if (ATR_CRS(a0) == 0)		//����??
		{
			if (ATR_SPD(a0) == 0)		//???��??
				ATR_CRS(a0) = 16;
			else
				ATR_SPD(a0) -= 1;
		}
		else
			ATR_SPD(a0) += 1;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 1:
		break;
	case 2:
		ATR_VCT_NO(a0)++;
		break;
	case 3:
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		if (ATR_VCT_NO(a1) == 1)
		{
			DeathAction(a0);		//��?
			return;
		}
		ATR_DISP_PRIO(a0) = ATR_DISP_PRIO(a1) + 1;
		ATR_LONG_WORK(0, a0)++;
		if (ATR_LONG_WORK(0, a0) & 1)
			ATR_SPD(a0) += 2;
		gemini(a0);
		ATR_H_POS(a0) += ATR_INT_WORK2(a1);
		ATR_V_POS(a0) += ATR_INT_WORK3(a1);
		break;
	case 4:
		if (ATR_LONG_WORK(1, a0))
		{
			//????
			if (!--ATR_LONG_WORK(1, a0))
				ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
		}
		a1 = ATR_BODY_WORK(1, a0);		//?�s???????
		ATR_H_POS(a0) = ATR_H_POS(a1);
		ATR_V_POS(a0) = ATR_V_POS(a1);
		break;
	}
	pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
}

/* ��????? *******************************************************************/
void stone_shadow(ACTION *a0)
{
	ACTION *a1, *a2, *a3;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (!--ATR_GROUP_FLG(a0))
		{
			a2 = ATR_BODY_WORK(0, a0);		//��????????
			if (ATR_LIFE(a2) <= 0){		//����Ǥ�ʤ�
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				break;
			}
#ifdef _PETSKILL_ACUPUNCTURE
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
/////////////////////////////
#else
/////////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
/////////////////////////////
#endif
				set_guard_mark(a0);		//�\?���Ӥ���?
			//?????????
			ATR_COMBO(a1) = 0;
			//?��??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				ATR_HIT_STOP(a0) = 32;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				gemini(a0);
				ATR_VCT_NO(a1) = 16;		//????
				ATR_CRS(a1) = ATR_CRS(a0);		//��?????????????
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
				ATR_STIMER(a1) = 0;
				ATR_FIRST_FLG(a1) = 0;		//??????????
				break;
			}
			//??????��??????
			if (!((ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) | (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)))
			{
				if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))		//????
				{
					a2 = ATR_BODY_WORK(0, a0);		//��????????
					//??����
					set_damage_num(a2, 0, -64);
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					ATR_CHR_NO(a0) = ATR_LONG_WORK(0, a0);		//?????��?
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					break;
				}
			}
			//?��????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
				set_damage_num(a1, 19, -112);//?????����
			//?????????
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			if (ATR_ATTACK_KIND(0, a0) & ATT_DEATH)		//?????
				ATR_LIFE(a1) = 0;		//?��????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//?��?????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????��?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????��?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????��
			//?��?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//?��??????
			else
				ATR_KAISHIN(a1) = 0;		//�G��?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?��????
			{
				ATR_LONG_WORK(1, a0) = 1;		//?��??????
				//????????
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;		//?��??????
			}
			if (ATR_KAISHIN(a1))		//?��?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			//???????????��??
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA) || (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						set_damage_num(a0, 1, -64 + 16);		//?????����
				}
				//???
				play_se(252, ATR_H_POS(a0), ATR_V_POS(a0));
				//?????????
				set_hit_mark(a0);
			}
			ATR_VCT_NO(a0) = 3;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//��??????
			ATR_VCT_NO(a1) = 1;				//????????
		}
		else
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?�h
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		//???????
		if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			ATR_STIMER(a0) = 32;
		else
			ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[0] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
		DeathAction(a0);		//��?
		break;
	case 3:
		a1 = ATR_BODY_WORK(2, a0);		//��??????
		ATR_CHR_NO(a1) = SPR_isiware;		//��?????
		if (pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP))	//???��???
			goto stone_shadow_100;
		if (!pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP))	//???��??????
			break;
stone_shadow_100:
		//?????????�h??
		ATR_GROUP_FLG(a0) = 10;
		ATR_SPD(a0) = 6;
		ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		d0 = ATR_CRS(a0);
		if (d0 >= 4 && d0 < 20)
			ATR_CHR_NO(a1) = 25784;		//�𦄨�??��?
		else
			ATR_CHR_NO(a1) = 25783;		//�𦄨�??��?
		ATR_CRS(a0) -= 8;
		ATR_CRS(a0) &= 31;
		a1 = ATR_BODY_WORK(2, a0);		//��??????
		ATR_VCT_NO(a1) = 2;				//?�h??
		ATR_SPD(a1) = 0;
		ATR_CRS(a1) = 16;
		ATR_LONG_WORK(1,a1) = 30;
		ATR_VCT_NO(a0)++;
 		//��?????
		a3 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
 		//?????
		a2 = GetAction(T_PRIO_BOW - 1, sizeof(ATR_EQU));
		ATR_NAME(a2) = ATR_NAME(a0);
		ATR_VCT_NO(a2) = ATR_VCT_NO(a0);
		/* ����?�I�T */
		ATR_DISP_PRIO(a2) = ATR_DISP_PRIO(a0);
		/* ?????�k? */
		ATR_CHR_NO(a2) = ATR_CHR_NO(a0);
		/* ???�t */
		ATR_CHR_ANG(a2) = ATR_CHR_ANG(a0);
		ATR_BODY_WORK(0, a2) = ATR_BODY_WORK(0, a0);		//��????
		ATR_BODY_WORK(1, a2) = ATR_BODY_WORK(1, a0);		//�D��?????
		ATR_BODY_WORK(2, a2) = a3;		//��??????
		ATR_ATTACK_POW(0, a2) = ATR_ATTACK_POW(0, a0);		//???
		ATR_ATTACK_PET_POW(0, a2) = ATR_ATTACK_PET_POW(0, a0);		//???
		ATR_ATTACK_KIND(0, a2) = ATR_ATTACK_KIND(0, a0);		//?????
		ATR_H_POS(a2) = ATR_H_POS(a0);
		ATR_V_POS(a2) = ATR_V_POS(a0);
		ATR_CRS(a2) = (ATR_CRS(a0) + 15) & 31;
		ATR_SPD(a2) = ATR_SPD(a0);
		ATR_BODY_CNT(a2) = ATR_BODY_CNT(a0);
		ATR_GROUP_FLG(a2) = ATR_GROUP_FLG(a0);
		pattern(a2, ANM_NOMAL_SPD, ANM_LOOP);
		p_missile[1] = a1;
		p_missile[2] = NULL;
		/* ?????�k? */
		if (d0 >= 4 && d0 < 20)
			ATR_CHR_NO(a3) = 25783;		//�𦄨�??��?
		else
			ATR_CHR_NO(a3) = 25784;		//�𦄨�??��?
		ATR_NAME(a3) = ATR_NAME(a1);
		ATR_VCT_NO(a3) = ATR_VCT_NO(a1);
		ATR_BODY_WORK(1, a3) = a2;		//?�s???????
		ATR_VCT_NO(a3) = ATR_VCT_NO(a1);
		ATR_DISP_PRIO(a3) = ATR_DISP_PRIO(a1);
		ATR_SPD(a3) = ATR_SPD(a1);
		ATR_CRS(a3) = ATR_CRS(a1);
		ATR_H_POS(a3) = ATR_H_POS(a1);
		ATR_V_POS(a3) = ATR_V_POS(a1);
		ATR_LONG_WORK(1, a3) = 30;
		break;
	case 4:
		ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
		ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
		gemini(a0);		//?�h
		ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
		ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
		if (!--ATR_GROUP_FLG(a0))		//��???
		{
			//????
			ATR_LONG_WORK(2, a0) = 60;
			ATR_LONG_WORK(3, a0) = 1;
			ATR_VCT_NO(a0) = 7;
		}
		break;
	case 5:
		ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
		a1 = ATR_BODY_WORK(2, a0);		//��??????
		ATR_VCT_NO(a1) = 4;
		//???
		ATR_VCT_NO(a0)++;
		break;
	case 6:
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)		//�r???????
		{
			if (!ATR_LONG_WORK(1, a0))		//��?��?????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			DeathAction(a0);		//��?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//��??????
			DeathAction(a1);		//��?
		}
	case 7:
		if (ATR_LONG_WORK(3, a0))
		{
			ATR_LONG_WORK(3, a0) = 0;
			//???
			play_se(251, ATR_H_POS(a0), ATR_V_POS(a0));
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
			a1 = ATR_BODY_WORK(2, a0);		//��??????
			ATR_VCT_NO(a1) = 4;
		}
		if (!--ATR_LONG_WORK(2, a0))		//��???
		{
			if (ATR_LONG_WORK(1, a0))		//��???????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			ATR_VCT_NO(a0) = 5;
		}
	}
}

//Terry add 2001/12/31
/* ����Ӱ�Ӵ��� *******************************************************************/
#ifdef _ITEM_FIRECREAKER
void firecracker_shadow(ACTION *a0)
{
	ACTION *a1;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//���˵�λַ
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//�״�(����·��)
		ATR_CRS(a0) = d0;		//����·��
		//׼������
		if (!--ATR_GROUP_FLG(a0))
		{
			ATR_VCT_NO(a0) = 1;				//????????
			a1 = ATR_BODY_WORK(2, a0);		//��??????
			ATR_VCT_NO(a1) = 1;				//????????
			ATR_STIMER(ATR_BODY_WORK(1, a0)) = 0;		//???��
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;	//Ӱ�ӽ���
			ATR_CHR_NO(a1) = 101121;		//����ը��
			ATR_H_POS(a1) += 50;
		}
		else 
		{
			ATR_INT_WORK2(a0) = ATR_H_POS(a0);		//?�h??��
			ATR_INT_WORK3(a0) = ATR_V_POS(a0);		//
			gemini(a0);		//?�h
			ATR_INT_WORK2(a0) = ATR_H_POS(a0) - ATR_INT_WORK2(a0);
			ATR_INT_WORK3(a0) = ATR_V_POS(a0) - ATR_INT_WORK3(a0);
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
		break;
	case 1:
		a1 = ATR_BODY_WORK(2, a0);		//����λַ
		if (!pattern(a1, 36, ANM_NO_LOOP))	//����δ����
			break;
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_NAME(a1) == NULL || ATR_VCT_NO(a1) >= VCT_NO_DIE || ATR_VCT_NO(a1) == 0)
		{		//�r???????
			if (!ATR_LONG_WORK(1, a0))			//��?��?????
				ATR_STIMER(ATR_BODY_WORK(1, a0)) = 1;		//???��
			DeathAction(a0);		//��?
			p_missile[0] = NULL;	//
			p_missile[1] = NULL;	//
			a1 = ATR_BODY_WORK(2, a0);		//��??????
			DeathAction(a1);		//��?
		}
		break;
	}
}
#endif
//Terry end

/* ?????? *******************************************************************/
void missile(ACTION *a0)
{
	ACTION *a1, *a2;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//???????
		if (d1 <= 32)
		{
			a2 = ATR_BODY_WORK(0, a0);		//��????????
			if (ATR_LIFE(a2) <= 0)		//??????
			{
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				break;
			}
			//????
			if ((ATR_ATTACK_POW(0, a0) == 0) && (ATR_ATTACK_PET_POW(0, a0) == 0))
			{
				a2 = ATR_BODY_WORK(0, a0);		//��????????
				//??����
				set_damage_num(a2, 0, -64);
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_VCT_NO(a0) = 1;		//????????
				break;
			}
			ATR_CRS(a1) = d0;		//��?????????????
			ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)		//?��?????
			{
				if (!ATR_COMBO(a1))		//????????
				{
					//??�k����
					ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
					set_damage_num(a1, 14, -64);
					ATR_ATTACK_POW(0, a0) = 0;		//????��?
					ATR_ATTACK_PET_POW(0, a0) = 0;		//pet????��?
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
				ATR_VCT_NO(a1) = 10;		//?????��
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);		//
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);		//
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0)		//?��????
			{
				ATR_KAISHIN(a1) = 1;		//?��??????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			}
			else
			{
				ATR_KAISHIN(a1) = 0;		//�G��?????
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			}
			ATR_VCT_NO(a0) = 1;		//????????
			//???
			play_se(103, 320, 240);
			//?��?????
			a1 = GetAction(T_PRIO_MAGIC_EFFECT, sizeof(ATR_EQU));
			if (a1 == NULL)
				return;
			/* ��??? */
			ATR_NAME(a1) = magic_effect;
			/* ����?�I�T */
			ATR_DISP_PRIO(a1) = D_PRIO_MAGIC_EFFECT;
			ATR_CHR_NO(a1) = 36009;
			/* ???�t */
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
		}
		else
			gemini(a0);		//?�h
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[ATR_BODY_CNT(a0)] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_DAMAGE(p_master)++;		//???��
		DeathAction(a0);		//��?
		break;
	}
}

/* ???? *******************************************************************/
void amelioration(ACTION *a0)
{
	ACTION *a1;
	int d0, d1;

	switch (ATR_VCT_NO(a0))
	{
	case 0:
		if (ATR_INT_WORK0(a0) == 0)		//��?��?�q??
		{
			d0 = 310;
			d1 = 222;
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			gemini(a0);		//?�h
			if (d1 <= (ATR_SPD(a0) >> 2))		//????
				ATR_INT_WORK0(a0) = 1;		//��?��????
		}
		else 
		{
			a1 = ATR_BODY_WORK(0, a0);		//��????????
			d0 = ATR_H_POS(a1);
			d1 = ATR_V_POS(a1);
			radar2(a0, d0, d1, 1);	//????
			if (ATR_INT_WORK1(a0) == 0)		//??��??????
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			d0 = ATR_H_POS(a1);
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			if (d0 == ATR_CRS(a0))		//??????
				ATR_INT_WORK1(a0) = 1;		//?????��???
			//???????
			if (d1 <= 32)
			{
				a1 = ATR_BODY_WORK(0, a0);		//��????????
				if (ATR_LIFE(a1) <= 0)		//??????
				{
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
					ATR_VCT_NO(a0) = 1;		//????????
					break;
				}
				ATR_COUNTER(a1) = NULL;		//????????
				ATR_VCT_NO(a1) = 15;		//???��
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
				ATR_KAISHIN(a1) = 0;		//?��??????
				ATR_VCT_NO(a0) = 1;		//????????
				//???
				play_se(100, 320, 240);
				//?��?????
				a1 = GetAction(T_PRIO_MAGIC_EFFECT, sizeof(ATR_EQU));
				if (a1 == NULL)
					return;
				/* ��??? */
				ATR_NAME(a1) = magic_effect;
				/* ����?�I�T */
				ATR_DISP_PRIO(a1) = D_PRIO_MAGIC_EFFECT;
				/* ?????�k? */
				ATR_CHR_NO(a1) = 36009;
				/* ???�t */
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
			}
			else
				gemini(a0);		//?�h
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 1:
		if (--ATR_HIT_STOP(a0))		//??????????
			break;
		ATR_STIMER(a0) = 64;
		ATR_VCT_NO(a0) = 2;
		a0->atr = ACT_ATR_HIDE;
		p_missile[ATR_BODY_CNT(a0)] = NULL;
		break;
	case 2:
		if (--ATR_STIMER(a0))
			break;
		ATR_DAMAGE(p_master)++;		//???��
		DeathAction(a0);		//��?
		break;
	}
}

// ???�{??? //////////////////////////////////////////////////////////////
typedef int CMPFUNC(const void*, const void*);
int sort_chr(SORT_CHR_EQU *a0, SORT_CHR_EQU *a1)
{
	if (a0->v_pos > a1->v_pos)
		return 1;
	else
		return -1;
}

extern int piyo_tbl[];
extern int piyo_point;
//static int command_point;

/*-------------------------------------------
	??????????????
--------------------------------------------*/
void set_raster_pos(ACTION *a1)
{
	int d0;
	
#ifdef __ATTACK_MAGIC
	int value = 0;
	
	// ������ڵ����л�����ĵ�ͼ���ʱ
	if ((g_iRunEarthQuake < 2) && (BattleMapNo < 148 || BattleMapNo > 150))
		return ;
	if (2 == g_iRunEarthQuake)
		value = g_iCurRunEarthQuake;
	else if (3 == g_iRunEarthQuake)
	{
		ATR_INT_WORK3(a1) = 0;
		return;
	}
	d0 = ATR_V_POS(a1) >> 3;
	d0 += piyo_point + value;
	d0 &= 63;
	ATR_V_POS(a1)		-= piyo_tbl[d0];
	ATR_INT_WORK1(a1)	-= piyo_tbl[d0];
	ATR_INT_WORK3(a1)	 = piyo_tbl[d0];
#else
	//??????????????
	if (BattleMapNo < 148 || BattleMapNo > 150)
		return;
	d0 = ATR_V_POS(a1) >> 3;
	d0 += piyo_point;
	d0 &= 63;
	ATR_V_POS(a1) -= piyo_tbl[d0];
	ATR_INT_WORK1(a1) -= piyo_tbl[d0];
	ATR_INT_WORK3(a1) = piyo_tbl[d0];
#endif
}

/*-------------------------------------------
	??��?�k��?????
--------------------------------------------*/
static int get_num(void)
{
	int d0 = 0;
	int d1 = 0;

	while (1)
	{
		d0 = BattleCmd[command_point++];
		if (d0 == NULL)
		{
			SDL_Log("Jerry command_point is %d", command_point);
			return -1;
		}
			
		if (d0 >= '0' && d0<= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0<= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';
	while (1)
	{
		d0 = BattleCmd[command_point++];		//?��????��?��?
		if (d0 == NULL)		//��???
		{
			command_point--;
			break;
		}
		if (d0 < '0' || d0 > '9')		//??????
		{
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//��?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}
//andy_add
static int get_mpnum(char code)
{
	int d0, d1, num = 0;

	while (1)
	{
		d0 = BattleCmd[num++];
		if (d0 == NULL)
			return -1;
		if (d0 == (int)code)
			break;
	}

	while (1)
	{
		d0 = BattleCmd[num++];		//?��?��?��?
		if (d0 == NULL)		//��???
			return -1;
		if (d0 >= '0' && d0<= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0<= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';

	while (1)
	{
		d0 = BattleCmd[num++];		//?��????��?��?
		if (d0 == NULL)		//��???
		{
			command_point--;
			break;
		}
		if (d0 < '0' || d0 > '9')		//??????
		{
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//��?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}
/*-------------------------------------------
	????��?????
--------------------------------------------*/
static char get_command(void)
{
	int now_point=0;
	char d0;

	while (1)
	{
		now_point = command_point;
		d0 = BattleCmd[command_point++];
		if (d0 == NULL)
			return -1;
		if (d0 == 'B')
		{
			//andy_reEdit 2003/07/18 21:54
			if (BattleCmd[now_point + 2] == '|' && (BattleCmd[now_point - 1] == '|' || BattleCmd[now_point - 1] == NULL))
				break;
		}
	}
	d0 = BattleCmd[command_point++];
	return d0;
}

static char get_char()
{
	char d0;

	d0 = BattleCmd[command_point++];		//?��?��?
	if (d0 == NULL)		//???��???
		return -1;		//��?
	return d0;
}

/* ���q��??? *******************************************************************/
void get_name(ACTION *a1)
{
	char d0;
	char *a2;

	a2 = ATR_HANDLE(a1);
	while (1)
	{
		d0 = BattleCmd[command_point++];		//?��?��?��?
		if (d0 == NULL)		//��???
			break;
		//?��???
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleCmd[command_point++];
		}
		else
			*a2++ = d0;
	}
	//��????
	*a2 = 0;
	makeStringFromEscaped(ATR_HANDLE(a1));
}

/*-------------------------------------------
	?????��?????
--------------------------------------------*/
static int get_next_flg(void)
{
	int d0, d1;

	d1 = command_point;		//?????????�~��
	d0 = get_num();		//����?��?
	if (d0 == -1)		//��???
		goto get_next_flg_500;
	if (d0 == 255)		//��???
		goto get_next_flg_500;
	d0 = get_num();		//???��?��?
	goto get_next_flg_700;
get_next_flg_500:
	d0 = 0;
get_next_flg_700:
	command_point = d1;		//???????????

	//SDL_Log("Jerry we are in get_next flg %d, %d, %d", d0, d1, command_point);
	return d0;
}

/*-------------------------------------------
	??��?��?��???
--------------------------------------------*/
static int get_body_cnt(void)
{
	int d0, d1, d2 = 0;

	d1 = command_point;		//?????????�~��
	while (1)
	{
		d0 = get_num();		//����?��?
		if (d0 == -1)		//��???
			break;
		if (d0 == 255)		//��???
			break;
		get_num();		//???????
		get_num();		//????????
		if (BattleCmd[command_point] == 'p')
			get_num();		//pet????????
		d2++;		//��??��
	}
	command_point = d1;		//???????????

	return d2;
}

/*-------------------------------------------
	???����?��???
--------------------------------------------*/
static ACTION *get_body_target(void)
{
	int d0, d1;

	d1 = command_point;		//?????????�~��
	d0 = get_num();		//����?��?
	command_point = d1;		//???????????
	if (d0 == -1)
		return 0;
	else
		return p_party[d0];
}

//ȫ��ȷ�ϴ���
int check_all_dead(void)
{
	int d0, d7;

	d0 = 10;		//��������趨
	for (d7 = 0; d7 < 5; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)		//????????????
		{
			if (ATR_PET_OK(p_party[d7]))
				d0 = 5;		//��������趨
			break;
		}
	}
	for (d7 = 0; d7 < d0; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)
		{
			if (ATR_LIFE(p_party[d7]) > 0)		//������һ�����ǻ��ŵ�
				break;
#ifdef _PETSKILL_LER
			// �׶���һ�μ��ڶ�������ʱ������ȫ��,��Ϊ�����
			else if (ATR_CHR_NO(p_party[d7]) == 101813 || ATR_CHR_NO(p_party[d7]) == 101814)
				break;
#endif
#ifdef __NEW_BATTLE_EFFECT
			if (1 == ATR_EFFECT_FLAG(p_party[d7]))
				break;
#endif
		}
	}
	if (d7 == d0)		//ȫ��
		return 1;
	d0 = 20;		//��������趨

	for (d7 = 10; d7 < 15; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)		//????????????
		{
			if (ATR_PET_OK(p_party[d7]))
				d0 = 15;		//��������趨
			break;
		}
	}
	for (d7 = 10; d7 < d0 ; d7++)
	{
		if (ATR_NAME(p_party[d7]) != NULL)
		{
			if (ATR_LIFE(p_party[d7]) > 0)		//������һ�����ǻ��ŵ�
				break;
#ifdef _PETSKILL_LER
			// �׶���һ�μ��ڶ�������ʱ������ȫ��,��Ϊ�����
			else if (ATR_CHR_NO(p_party[d7]) == 101813 || ATR_CHR_NO(p_party[d7]) == 101814)
				break;
#endif
#ifdef __NEW_BATTLE_EFFECT
			if (1 == ATR_EFFECT_FLAG( p_party[d7]))
				break;
#endif
		}
	}
	if (d7 == d0)		//ȫ�� return 1
		return 1;
	return 0;		//�����˻��� return 0 
}

//���Ա�ʾ����
void disp_attrib(ACTION *a0)
{
	ACTION *a1;

	a1 = p_attrib;
	switch (ATR_ATTRIB(a0))
	{
	//������
	case 0:
		ATR_ATTRIB(a1) = ACT_ATR_HIDE;		//����ʾ
		break;
	//������
	case 1:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_EARTH_BATTLE;
		break;
	//ˮ����
	case 2:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_WATER_BATTLE;
		break;
	//������
	case 3:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_FIRE_BATTLE;
		break;
	//������
	case 4:
		ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;
		ATR_CHR_NO(a1) = CG_ATR_ICON_WIND_BATTLE;
		break;
	}
	pattern(a1, ANM_NOMAL_SPD, ANM_NO_LOOP);
}

/* ????��?��??? *******************************************************************/
void get_command_asc(void)
{
	char d0, d1 = 0;
	char *a2;

	kanji_buf[0][0] = kanji_buf[1][0] = kanji_buf[2][0] = kanji_buf[3][0] = 0;
	a2 = kanji_buf[d1];
	while (1)
	{
		d0 = BattleCmd[command_point++];		//?��?��?��?
		if (d0 == NULL)		//��???
		{
			command_point--;
			break;
		}
		//????
		if (d0 == '\t')
		{
			//��????
			*a2 = 0;
			d1++;
			if (d1 == 4)		//?????
				break;
			a2 = kanji_buf[d1];
			continue;
		}
		//?��???
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleCmd[command_point++];
		}
		else
			*a2++ = d0;
	}
	//��????
	*a2 = 0;
}

#ifdef __TOCALL_MAGIC
// �ٻ����������Ͻ���
bool BuildToCallMagicData(ACTION *pMaster, ACTION *pAttacker)
{
	int	i, idx = 0, midx, midy, count = 0;
	int	charidx[MAX_NUM_ATTACKED];

	memset(&ToCallMgc, 0, sizeof(ToCallMgc));
	memset(charidx, 0, sizeof(charidx));

	iAttackedNum			= 0;
	iCurAttackedFinishNum	= 0;

	// B$|Attacker's No( 0 - 19 )|5711438|Attacker's MP|

	ATR_VCT_NO(pAttacker)		= TOCALL_MAGIC_CASE;
	ATR_MP(pAttacker)			= get_num();
//	ToCallMgc.iPreMgcNum			= get_num();
//	ToCallMgc.iCurMgcNum			= get_num();
	ToCallMgc.iPreMgcNum		= get_num();
#ifdef _PETSKILL_LER			// �׶�����
	if (ToCallMgc.iPreMgcNum == 101808 || ToCallMgc.iPreMgcNum == 101809)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iPreMgcNum += 53;
	}
#endif
	ToCallMgc.iCurMgcNum			= get_num();
#ifdef _PROFESSION_ADDSKILL
	if (ToCallMgc.iCurMgcNum >= 101770 && ToCallMgc.iCurMgcNum <= 101797)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum += 55;
	}
#endif
#ifdef _PETSKILL_LER			// �׶�����
	if (ToCallMgc.iCurMgcNum == 101798)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum = 101853;
	}
	if (ToCallMgc.iCurMgcNum == 101800)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum = 101854;
	}
	if (ToCallMgc.iCurMgcNum == 101806 || ToCallMgc.iCurMgcNum == 101807)
	{
		if (!g_bUseAlpha)
			ToCallMgc.iCurMgcNum += 53;
	}
#endif
	ToCallMgc.iPostMgcNum			= get_num();
	ToCallMgc.wAttackType			= ( unsigned short )get_num();
	ToCallMgc.wAttackTimeSlice		= ( unsigned short )get_num();
	ToCallMgc.wShowType				= ( unsigned short )get_num();
//	ToCallMgc.wShowType =0;
	ToCallMgc.wScreenX				= ( unsigned short )get_num();
//	ToCallMgc.wScreenX				= 0;//-150;
	ToCallMgc.wScreenY				= ( unsigned short )get_num();
//	ToCallMgc.wScreenY				= 0;//-20;
	ToCallMgc.wPreMgcX				= ( unsigned short )get_num();
	ToCallMgc.wPreMgcY				= ( unsigned short )get_num();
	ToCallMgc.wPostMgcX				= ( unsigned short )get_num();
	ToCallMgc.wPostMgcY				= ( unsigned short )get_num();
	ToCallMgc.wPreMgcOnChar			= ( unsigned short )get_num();
	ToCallMgc.wCurMgcOnChar			= ( unsigned short )get_num();
	ToCallMgc.wPostMgcOnChar		= ( unsigned short )get_num();
	ToCallMgc.dwEQuake				= ( unsigned long )get_num();
//	ToCallMgc.dwEQuake = 1;
	ToCallMgc.dwEQuakeSTime			= ( unsigned long )get_num();
//	ToCallMgc.dwEQuakeSTime			= 1000;
	ToCallMgc.dwEQuakeETime			= ( unsigned long )get_num();
//	ToCallMgc.dwEQuakeETime			= 4000;
	ToCallMgc.wIsPostDisappear		= ( unsigned short )get_num();
	ToCallMgc.wToCallMagicNo		= ( unsigned short )get_num();

	do
	{
		charidx[idx] = get_num();
	}while (0XFF != charidx[idx] && ++idx);

	if (0 == idx)
		return false;

	while (0X5711438 != ( AttackedInfo[iAttackedNum * 4] = get_num()))
	{
	 	AttackedInfo[iAttackedNum * 4 + 1]  = get_num();
		AttackedInfo[iAttackedNum * 4 + 2]	= get_num();
		AttackedInfo[iAttackedNum * 4 + 3]	= get_num();
		iAttackedNum++;
	}
	ToCallMgc.dwCurFrame1		= 0;
	ToCallMgc.dwCurFrame2		= 0;
	ToCallMgc.dwEQuakeSFrame	= ToCallMgc.dwEQuakeSTime >> 4;
	ToCallMgc.dwEQuakeEFrame	= ToCallMgc.dwEQuakeETime >> 4;

	// �Ƿ���ǰ������
	(0XFFFFFFFF == ToCallMgc.iPreMgcNum) ? ToCallMgc.wRunPreMgc = true : ToCallMgc.wRunPreMgc = false;

	// ����λ����ʾ��
	if (1 == ToCallMgc.wShowType)
	{
		ToCallMgc.wNumAttacks		= 1;
		ToCallMgc.wCurAttackNum		= 0;
		ToCallMgc.posAttacked[0].x	= ToCallMgc.wScreenX;
		ToCallMgc.posAttacked[0].y	= ToCallMgc.wScreenY;
		ToCallMgc.wAttackedIndex[0]	= 20;		// ȫ����Ա��ͬʱ������״̬
		ToCallMgc.wMgcFrameCount[0]	= 0;

		// �������ĵ���������
		for (i = 0; i < idx; i++)
			ToCallMgc.wAttackedIndex[i + 1] = charidx[i];

		ToCallMgc.wAttackedIndex[i + 1] = 0XFF;
	}
	// ����λ����ʾ��
	else
	{
		// �����ķ�ʽΪȫ��ͬʱ����
		if (2 == ToCallMgc.wAttackType || 4 == ToCallMgc.wAttackType)
		{
			midx = 0;
			midy = 0;

			for (i = 0; i < idx; i++)
			{
				midx += p_party[charidx[i]]->x;
				midy += p_party[charidx[i]]->y;
			}
			midx = int(midx / idx);
			midy = int(midy / idx);
			ToCallMgc.wNumAttacks		= 1;
			ToCallMgc.wCurAttackNum		= 0;
			ToCallMgc.posAttacked[0].x	= midx + ToCallMgc.wScreenX;
			ToCallMgc.posAttacked[0].y	= midy + ToCallMgc.wScreenY;
			ToCallMgc.wAttackedIndex[0]	= 20;		// ȫ����Ա��ͬʱ������״̬
			ToCallMgc.wMgcFrameCount[0]	= 0;

			// �������ĵ���������
			for (i = 0; i < idx; i++)
				ToCallMgc.wAttackedIndex[i + 1] = charidx[i];
			ToCallMgc.wAttackedIndex[i + 1] = 0XFF;
		}
		// ��������
		else
		{
			// �������ĵ���������
			for (i = 0; i < idx; i++)
			{
				ToCallMgc.wNumAttacks++;
				ToCallMgc.posAttacked[i].x		= ToCallMgc.wScreenX;
				ToCallMgc.posAttacked[i].y		= ToCallMgc.wScreenY;
				ToCallMgc.wAttackedIndex[i]	= charidx[i];
				ToCallMgc.wMgcFrameCount[i]	= count;
				count += ToCallMgc.wAttackTimeSlice >> 4;
			}
		}
	}
	ATR_VCT_NO(pAttacker)	= TOCALL_MAGIC_CASE;
	ATR_BODY_CNT(pMaster)	= 1;
	bRunToCallMgc			= true;
	
	return true;
}


// ��̬�����µ�ħ��
bool RunTimeMagicToCall()
{
	// ��ʼ����
	if (ToCallMgc.dwEQuakeSFrame == ToCallMgc.dwCurFrame1)
	{
		g_iRunEarthQuake	= 1;
		g_iNumRunEarthQuake = ToCallMgc.dwEQuakeEFrame - ToCallMgc.dwEQuakeSFrame;
	}

	// ��������
	if (ToCallMgc.dwEQuakeEFrame == ToCallMgc.dwCurFrame1)
		g_iRunEarthQuake	= 3;

	ToCallMgc.dwCurFrame1++;
	// ǰ��ħ���Ƿ��Ѿ�������
	if (false == ToCallMgc.wRunPreMgc)
		return true;

	if (ToCallMgc.wToCallMagicNo != 2)
	{
		// �Ƿ�������δ���ŵ�����
		if (ToCallMgc.wCurAttackNum < ToCallMgc.wNumAttacks)
		{
			while (1)
			{
				if (ToCallMgc.dwCurFrame2 == ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum])
				{
					ACTION	*a0;

					a0						= GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					ATR_NAME(a0)			= monster;
					ATR_VCT_NO(a0)			= TOCALL_MAGIC_CASE + 2;		// ������ִ��
					ATR_DISP_PRIO(a0)		= (1 == ToCallMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a0)			= ToCallMgc.iCurMgcNum;
					ATR_LONG_WORK(0 , a0)	= ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum];
					ATR_H_POS(a0)			= (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x : ATR_H_POS( p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]] ) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x;
					ATR_V_POS(a0)			= (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y : ATR_V_POS( p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]] ) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y;

					if (ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum] < 10)
						ATR_CHR_ANG(a0) = (7);
					else
						ATR_CHR_ANG(a0) = (3);
					ToCallMgc.wCurAttackNum++;
				}
				if (ToCallMgc.wCurAttackNum >= ToCallMgc.wNumAttacks || ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum] > ToCallMgc.dwCurFrame2)
					break;
			}
		}
	}
	else
	{
		// �Ƿ�������δ���ŵ�����
		if (ToCallMgc.wCurAttackNum < ToCallMgc.wNumAttacks)
		{
			while (1)
			{
				if (ToCallMgc.dwCurFrame2 == ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum])
				{
					ACTION	*a0;

					a0						= GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					ATR_NAME(a0)			= monster;
					ATR_VCT_NO(a0)		= TOCALL_MAGIC_CASE + 2;		// ������ִ��
					ATR_DISP_PRIO(a0)		= (1 == ToCallMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a0)		= ToCallMgc.iCurMgcNum;
#ifdef _PROFESSION_ADDSKILL
					if (g_bUseAlpha)
					{
						if (ToCallMgc.iCurMgcNum == 101770 || ToCallMgc.iCurMgcNum == 101771)//Ϊ�����Ƴ����ͼ����ʾ���������֮��
							ATR_DISP_PRIO(a0)	= DISP_PRIO_TILE + ToCallMgc.wCurMgcOnChar;
					}
					else
					{
						if (ToCallMgc.iCurMgcNum == 101825 || ToCallMgc.iCurMgcNum == 101826)//Ϊ�����Ƴ����ͼ����ʾ���������֮��
							ATR_DISP_PRIO(a0)	= DISP_PRIO_TILE + ToCallMgc.wCurMgcOnChar;
					}
#endif
					ATR_LONG_WORK(0, a0)	= ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum];
					ATR_H_POS(a0)			= (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x : ATR_H_POS( p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].x;
					ATR_V_POS(a0)			= (20 == ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]) ? ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y : ATR_V_POS( p_party[ToCallMgc.wAttackedIndex[ToCallMgc.wCurAttackNum]]) + ToCallMgc.posAttacked[ToCallMgc.wCurAttackNum].y;
					ToCallMgc.wCurAttackNum++;
				}
				if (ToCallMgc.wCurAttackNum >= ToCallMgc.wNumAttacks || ToCallMgc.wMgcFrameCount[ToCallMgc.wCurAttackNum] > ToCallMgc.dwCurFrame2)
					break;
			}
		}
	}
	ToCallMgc.dwCurFrame2++;

	return true;
}
#endif

#ifdef _PROFESSION_ADDSKILL
bool BuildBoundaryMagicData(int state)
{
	int stateno[] = {101789,101777,101783,101795,101786,101774,101780,101792};
	int mark=-1;

	if (!g_bUseAlpha)
	{
		if (state > 1)
			state += 55;
		stateno[0] = 101844, stateno[1] = 101832, stateno[2] = 101838, stateno[3] = 101850,
		stateno[4] = 101841, stateno[5] = 101829, stateno[6] = 101835, stateno[7] = 101847;
	}
	if (state == 0)//�����
	{
        if (boundary_mark[0])
		{
            DeathAction(boundary_mark[0]);
		    boundary_mark[0] = NULL;
		}
		bRunBoundaryMgc_l = 0;    
	}
	if (state == 1)//�����
	{
        if (boundary_mark[1])
		{
            DeathAction(boundary_mark[1]);
		    boundary_mark[1] = NULL;
		}
		bRunBoundaryMgc_r = 0;
	}
	if (state == stateno[0] //���
		|| state == stateno[1]
		|| state == stateno[2]
		|| state == stateno[3])
	{
        if (bRunBoundaryMgc_l != state)
		{
			if (boundary_mark[0])
			{
                DeathAction(boundary_mark[0]);
		        boundary_mark[0] = NULL;
			}
            bRunBoundaryMgc_l = state;
			if (boundary_mark[0] == NULL)
			{
		    	boundary_mark[0] = MakeAnimDisp(320, 240, state + 2, 0);
		        boundary_mark[0]->actNo = 0;
                boundary_mark[0]->anim_ang = 3;
                boundary_mark[0]->dispPrio = DISP_PRIO_TILE + 1;//ͼ��
			}
		}
	}

	if (state == stateno[4] //�ҵ�
		|| state == stateno[5]
		|| state == stateno[6]
		|| state == stateno[7])
	{
        if (bRunBoundaryMgc_l != state)
		{
			if (boundary_mark[1])
			{
                DeathAction(boundary_mark[1]);
		        boundary_mark[1] = NULL;
			}
            bRunBoundaryMgc_r = state;
			if (boundary_mark[1] == NULL)
			{
		    	boundary_mark[1] = MakeAnimDisp(320, 240, state + 2, 0);
		        boundary_mark[1]->actNo = 0;
                boundary_mark[1]->anim_ang = 3;
                boundary_mark[1]->dispPrio = DISP_PRIO_TILE + 1;//ͼ��
			}
		}
	}

	return true;
}

void RunTimeMagicBoundary(int state)
{
	int stateno[] = { 101774, 101797};

	if (!g_bUseAlpha)
	{
		stateno[0] = 101829;
		stateno[1] = 101852;
	}
	if (boundary_2 == NULL && state >= stateno[0] && state <= stateno[1])
	{
		int mark = state + 1;

	    boundary_2 = MakeAnimDisp(320, 240, mark, 0);
		boundary_2->actNo = 0;
        boundary_2->anim_ang = 3;
        boundary_2->dispPrio = DISP_PRIO_TILE + 1;//ͼ��
	}
	
	/*if ( boundary_r->anim_cnt == 10 ){//���ŵ����һ��
            DeathAction(boundary_r);
			boundary_r = NULL;
			bRunBoundaryMgc = false;
		}*/
}
#endif

#ifdef __ATTACK_MAGIC
// ������ʾ��ǰ��λ��
static int SortIdx(const void *pElement1, const void *pElement2)
{
	int	nth1 = *((int*)pElement1);
	int	nth2 = *((int*)pElement2);

	if (p_party[nth1]->y < p_party[nth2]->y)
		return -1;
	else if (p_party[nth1]->y > p_party[nth2]->y)
		return 1;

	return 0;
}

// ���������������Ͻ���
bool BuildAttackMagicData(ACTION *pMaster , ACTION *pAttacker)
{
	int	i, idx = 0, midx, midy, count = 0;
	int	charidx[MAX_NUM_ATTACKED];

	memset(&AttMgc, 0, sizeof(AttMgc));
	memset(charidx, 0, sizeof(charidx));

	iAttackedNum				= 0;
	iCurAttackedFinishNum		= 0;

	// BJ|Attacker's No( 0 - 19 )|12345678|Attacker's MP|

	ATR_VCT_NO(pAttacker)		= ATTACK_MAGIC_CASE;
	ATR_MP(pAttacker)			= get_num();
	AttMgc.iPreMgcNum			= get_num();
	AttMgc.iCurMgcNum			= get_num();
	AttMgc.iPostMgcNum			= get_num();
	AttMgc.wAttackType			= (unsigned short)get_num();
	AttMgc.wAttackTimeSlice		= (unsigned short)get_num();
	AttMgc.wShowType			= (unsigned short)get_num();
	AttMgc.wScreenX				= (unsigned short)get_num();
	AttMgc.wScreenY				= (unsigned short)get_num();
	AttMgc.wPreMgcX				= (unsigned short)get_num();
	AttMgc.wPreMgcY				= (unsigned short)get_num();
	AttMgc.wPostMgcX			= (unsigned short)get_num();
	AttMgc.wPostMgcY			= (unsigned short)get_num();
	AttMgc.wPreMgcOnChar		= (unsigned short)get_num();
	AttMgc.wCurMgcOnChar		= (unsigned short)get_num();
	AttMgc.wPostMgcOnChar		= (unsigned short)get_num();
	AttMgc.dwEQuake				= (unsigned long)get_num();
	AttMgc.dwEQuakeSTime		= (unsigned long)get_num();
	AttMgc.dwEQuakeETime		= (unsigned long)get_num();

	do
	{
		charidx[idx] = get_num();
	}
	while (0XFF != charidx[idx] && ++idx);

	if (0 == idx)
		return false;

	while (0X12345678 != (AttackedInfo[iAttackedNum * 4] = get_num()))
	{
	 	AttackedInfo[iAttackedNum * 4 + 1]  = get_num();
		AttackedInfo[iAttackedNum * 4 + 2]	= get_num();
		AttackedInfo[iAttackedNum * 4 + 3]	= get_num();
		iAttackedNum++;
	}

	AttMgc.dwCurFrame1		= 0;
	AttMgc.dwCurFrame2		= 0;
	AttMgc.dwEQuakeSFrame	= AttMgc.dwEQuakeSTime >> 4;
	AttMgc.dwEQuakeEFrame	= AttMgc.dwEQuakeETime >> 4;

	// �Ƿ���ǰ������
	(0XFFFFFFFF == AttMgc.iPreMgcNum) ? AttMgc.wRunPreMgc = true : AttMgc.wRunPreMgc = false;

	// ����λ����ʾ��
	if (1 == AttMgc.wShowType)
	{
		AttMgc.wNumAttacks			= 1;
		AttMgc.wCurAttackNum		= 0;
		AttMgc.posAttacked[0].x		= AttMgc.wScreenX;
		AttMgc.posAttacked[0].y		= AttMgc.wScreenY;
		AttMgc.wAttackedIndex[0]	= 20;		// ȫ����Ա��ͬʱ������״̬
		AttMgc.wMgcFrameCount[0]	= 0;

		// �������ĵ���������
		for (i = 0; i < idx; i++)
			AttMgc.wAttackedIndex[i + 1] = charidx[i];

		AttMgc.wAttackedIndex[i + 1] = 0XFF;
	}
	// ����λ����ʾ��
	else
	{
		// �����ķ�ʽΪȫ��ͬʱ����
		if (2 == AttMgc.wAttackType || 4 == AttMgc.wAttackType)
		{
			midx = 0;
			midy = 0;
			for (i = 0; i < idx; i++)
			{
				midx += p_party[charidx[i]]->x;
				midy += p_party[charidx[i]]->y;
			}
			midx = int(midx / idx);
			midy = int(midy / idx);
			AttMgc.wNumAttacks			= 1;
			AttMgc.wCurAttackNum		= 0;
			AttMgc.posAttacked[0].x		= midx + AttMgc.wScreenX;
			AttMgc.posAttacked[0].y		= midy + AttMgc.wScreenY;
			AttMgc.wAttackedIndex[0]	= 20;		// ȫ����Ա��ͬʱ������״̬
			AttMgc.wMgcFrameCount[0]	= 0;
			// �������ĵ���������
			for (i = 0; i < idx; i++)
				AttMgc.wAttackedIndex[i + 1] = charidx[i];

			AttMgc.wAttackedIndex[i + 1] = 0XFF;
		}
		// ��������
		else
		{
			// �������ĵ���������
			for (i = 0; i < idx; i++)
			{
				AttMgc.wNumAttacks++;
				AttMgc.posAttacked[i].x		= AttMgc.wScreenX;
				AttMgc.posAttacked[i].y		= AttMgc.wScreenY;
				AttMgc.wAttackedIndex[i]	= charidx[i];
				AttMgc.wMgcFrameCount[i]	= count;
				count += AttMgc.wAttackTimeSlice >> 4;
			}
		}
	}
	
	ATR_VCT_NO(pAttacker)	= ATTACK_MAGIC_CASE;
	ATR_BODY_CNT(pMaster)	= 1;
	bRunAttMgc				= true;

	return true;
}

// ��̬�����µ�ħ��
bool RunTimeMagic()
{
	// ��ʼ����
	if (AttMgc.dwEQuakeSFrame == AttMgc.dwCurFrame1)
	{
		g_iRunEarthQuake	= 1;
		g_iNumRunEarthQuake = AttMgc.dwEQuakeEFrame - AttMgc.dwEQuakeSFrame;
	}
	// ��������
	if (AttMgc.dwEQuakeEFrame == AttMgc.dwCurFrame1)
		g_iRunEarthQuake	= 3;
	AttMgc.dwCurFrame1++;
	// ǰ��ħ���Ƿ��Ѿ�������
	if (false == AttMgc.wRunPreMgc)
		return true;
	// �Ƿ�������δ���ŵ�����
	if (AttMgc.wCurAttackNum < AttMgc.wNumAttacks)
	{
		while (1)
		{
			if (AttMgc.dwCurFrame2 == AttMgc.wMgcFrameCount[AttMgc.wCurAttackNum])
			{
				ACTION	*a0;

				a0						= GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				ATR_NAME(a0)			= monster;
				ATR_VCT_NO(a0)			= ATTACK_MAGIC_CASE + 2;		// ������ִ��
				ATR_DISP_PRIO(a0)		= (1 == AttMgc.wCurMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
				ATR_CHR_NO(a0)			= AttMgc.iCurMgcNum;
				ATR_LONG_WORK(0, a0)	= AttMgc.wAttackedIndex[AttMgc.wCurAttackNum];
				ATR_H_POS(a0)			= (20 == AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]) ? AttMgc.posAttacked[AttMgc.wCurAttackNum].x : ATR_H_POS(p_party[AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]]) + AttMgc.posAttacked[AttMgc.wCurAttackNum].x;
				ATR_V_POS(a0)			= (20 == AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]) ? AttMgc.posAttacked[AttMgc.wCurAttackNum].y : ATR_V_POS(p_party[AttMgc.wAttackedIndex[AttMgc.wCurAttackNum]]) + AttMgc.posAttacked[AttMgc.wCurAttackNum].y;
				AttMgc.wCurAttackNum++;
			}
			if (AttMgc.wCurAttackNum >= AttMgc.wNumAttacks || AttMgc.wMgcFrameCount[AttMgc.wCurAttackNum] > AttMgc.dwCurFrame2)
				break;
		}
	}
	AttMgc.dwCurFrame2++;

	return true;
}
#endif

//�����ж�����
void master(ACTION *a0)
{
	ACTION *a1, *a2, *a3;
	int d0, d1, d2, d3, d6, d7;
	int z = 0;
	int jjj = 0;
	int command_no;
	int sav_command_point, castflg=0;
	int petfall_flg = 0;
	
	//���Ա�ʾ
	disp_attrib(a0);
	if (ATR_VCT_NO(a0) == 0)	// ����ȷ��
	{
		if (BattleCmd[0] == NULL)		//ս������ȷ��
		{
			goto master_500;
		}
			
	}
	switch (ATR_VCT_NO(a0))
	{
	case 0:		//????�D?
	 	sav_command_point = command_point;		//?????��?????�~��
		command_no = get_command();		//????��?��?
		if (command_no == 255 || command_no == -1)		//��???
		{
			ATR_VCT_NO(a0) = 4;		//???��?�r??
			command_point = 0;		//?????��????????
			break;
		}
		if (command_no == ATT_KANJI)		//��??����??
		{
			if (p_kanji != NULL)		//����???
			{
				a1 = ATR_BODY_WORK(0, p_kanji);		//??????��
				DeathAction(a1);					//
			}
			else 
			{
				p_kanji = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (p_kanji == NULL)
					return;
			}
			//����BOX����
			a1 = MakeWindowDisp(640 - 4 - 64 * 4, 4, 4, 2, 0, 2);
			//�����岻�����ʧ��
			if (a1 == NULL)
				return;
			ATR_BODY_WORK(0, p_kanji) = a1;		//?????��V
			ATR_NAME(p_kanji) = disp_kanji;		//??????
			ATR_ATTRIB(p_kanji) = ACT_ATR_HIDE;		//����??
			// ????????
			play_se(202, 320, 240);
			command_point++;		//??????????
			//????��?��????
			get_command_asc();
			d0 = 60;		//����???��
			for (d7 = 0; d7 < 4; d7++)
			{
				if (kanji_buf[d7][0])		//��??????
					d0 += 60;		//����???��
			}
			ATR_INT_WORK0(p_kanji) = d0;
			ATR_VCT_NO(a0) = 1;		//????�r?
			ATR_BODY_CNT(a0) = 1;
			ATR_DAMAGE(a0) = 0;
			ATR_PET_DAMAGE(a0) = 0;
			break;
		}
		if (command_no == ATT_EFFECT)
		{
#ifdef __NEW_BATTLE_EFFECT
			// Bj | target(who) | effect(effect no) | effect_anim | param | .... | FF
			ACTION *target = NULL;
			int who;
			int effect;
			int effect_anim;
			int param;

			ATR_VCT_NO(a0) = 1;
			while (1)
			{
				who = get_num();
				if (who == 255)
					break;
				effect = get_num();			//��Ч�ı��
				effect_anim = get_num();	//��Ч�Ķ���
				param = get_num();			//����
				target = NULL;
				if (who < 20)
					target = p_party[who];
				if (target)
				{
					ATR_EFFECT_FLAG(target) = effect;
					ATR_EFFECT_ANIM(target) = effect_anim;
					ATR_EFFECT_PARAM(target) = param;
					ATR_BODY_CNT(a0)++;
				}
			}
#endif
			break;
		}
#ifdef _PETSKILL_BATTLE_MODEL
		if (command_no == ATT_BATTLE_MODEL)			// ���＼��ս��ģ��
		{
			ATR_VCT_NO(a0) = 1;
			ATR_PET_DAMAGE(a0) = 0;
			for (d7 = 0; d7 < 20; d7++)
			{
				ATR_SYNC_FLG(p_party[d7]) = 0;
				ATR_ATTACK_KIND(0, p_party[d7]) = 0;
				ATR_ATTACK_KIND(1, p_party[d7]) = 0;
				ATR_ATTACK_POW(0, p_party[d7]) = 0;
				ATR_ATTACK_PET_POW(0, p_party[d7]) = 0;
				ATR_MPDAMAGE(p_party[d7]) = 0;
				ATR_MPDFLG(p_party[d7]) = 0;
				ATR_ADDHP(p_party[d7]) = 0;
				ATR_ADDHPFLG(p_party[d7])=0;
				ATR_SHOWADDHP(p_party[d7])=0;
				ATR_LONG_WORK(0, p_party[d7]) = 0;
				ATR_BATTLE_MODEL(p_party[d7]) = 0;
				memset(&ATR_BODY_WORK(0, p_party[d7]), 0, sizeof(ACTION*) * 20);
				memset(p_missile, 0, sizeof(p_missile));
			}
			d7 = 0;
			// ��ȡ�������ĳ�����
			d0 = get_num();
			a3 = p_party[d0];
			// �жϹ����������ϻ�������������ʼλ��
			if (ATR_GROUP_FLG(a3) == 0)
			{
				d2 = 3;	// ����
				d0 = 0;
			}
			else
			{
				d2 = 7; // ����
				d0 = 10;
			}
			// ȡ��Ŀ�깥��������
			a1 = NULL;
			while ((d6 = get_num()) != 255)
			{
				a1 = p_party[d6];
				if (ATR_VCT_NO(a1) != 55)
					ATR_FIRST_FLG(a1) = 0;
				ATR_ATTACK_KIND(2, a1) = 0;
				d3 = get_num();			// ȡ������������
				// �ж� p_missile[d3] ��û����ʹ��
				// ��ʹ��,��ʾ����������Ҫ��������Ŀ��
				if (p_missile[d3] != NULL)
				{
					a2 = p_missile[d3];
					ATR_BODY_WORK(ATR_INT_WORK1(a2), a2) = a1;				// ������
					ATR_BATTLE_MODEL(a1) = ATT_BATTLE_MODEL;				// ��Ŀ���Ǳ� ATT_BATTLE_MODEL ��̬����
					ATR_ATTACK_KIND(ATR_INT_WORK1(a2), a2) = get_num();		// ��������
					ATR_ATTACK_POW(ATR_INT_WORK1(a2), a2) = get_num();		// ������
					ATR_ATTACK_PET_POW(ATR_INT_WORK1(a2), a2) = get_num();	// pet������
					ATR_CHR_NO(a2) = get_num();																	// �������ͼ��
					// �������ȮЧ��,��¼ʹ����Ȯ��Ŀ��
					if (BattleCmd[command_point] == 'g')
						ATR_LONG_WORK(ATR_INT_WORK1(a2), a2) = get_num();
					ATR_INT_WORK1(a2)++;
					// ��¼Ŀ���б��˹����������
					if (ATR_LONG_WORK(0, a1) == 0)
					{
						ATR_LONG_WORK(0, a1) = 1;		// ATR_LONG_WORK ��λ : λַ 0 : ���趨Ŀ���Ƿ񱻹�����,���Զ����趨Ϊ1
						ATR_LONG_WORK(d3, a1) = d7 + 1; //						λַ d3: d3 �ǹ�������ı��,�� d3 ��λַ������
														//								 ��¼��������ڵ� d7 + 1 �ĻغϹ���Ŀ��
					}
					else
						ATR_LONG_WORK(d3, a1) = d7 + 1;	// �� ATR_LONG_WORK ��λ 0 ��Ϊ 0 ʱ,��ʾ���Ŀ���ѱ���Ĺ����������,
														// ����ֱ���� d3 ��λַ��¼Ҫ�� d7 + 1 �ĻغϹ���Ŀ��
				}
				// ûʹ�ù�
				else
				{
					p_missile[d3] = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
					if (p_missile[d3] == NULL)
						break;	// ��̫����
					a2 = p_missile[d3];
					// ��ʼ��
					ATR_NAME(a2) = monster;
					ATR_CHR_ANG(a2) = d2;
					ATR_H_POS(a2) = monster_start_pos[monster_place_no[d3 + d0] * 2];
					ATR_V_POS(a2) = monster_start_pos[monster_place_no[d3 + d0] * 2 + 1];

					ATR_CHR_ACT(a2) = ANIM_STAND;
					ATR_GROUP_FLG(a2) = ATR_GROUP_FLG(a3);
					a2->hitDispNo = a3->hitDispNo;
					ATR_VCT_NO(a2) = 1;							// ǰ��
					ATR_BODY_WORK(0, a2) = a1;					// ������Ŀ��
					ATR_BATTLE_MODEL(a1) = ATT_BATTLE_MODEL;	// ��Ŀ���Ǳ� ATT_BATTLE_MODEL ��̬����
					// ��¼Ŀ���б��˹����������
					if (ATR_LONG_WORK(0, a1) == 0)
					{
						ATR_LONG_WORK(0, a1) = 1;			// ATR_LONG_WORK ��λ : λַ 0 : ���趨Ŀ���Ƿ񱻹�����,���Զ����趨Ϊ1
						ATR_LONG_WORK(d3, a1) = d7 + 1;		//						λַ d3: d3 �ǹ�������ı��,�� d3 ��λַ������
															//								 ��¼��������ڵ� d7 + 1 �ĻغϹ���Ŀ��
					}
					else
						ATR_LONG_WORK(d3, a1) = d7 + 1;		// �� ATR_LONG_WORK ��λ 0 ��Ϊ 0 ʱ,��ʾ���Ŀ���ѱ���Ĺ����������,
															// ����ֱ���� d3 ��λַ��¼Ҫ�� d7 + 1 �ĻغϹ���Ŀ��
					ATR_ATTACK_KIND(0, a2) = get_num();		// ��������
					ATR_ATTACK_POW(0, a2) = get_num();		// ������
					ATR_ATTACK_PET_POW(0, a2) = get_num();	// pet������
					ATR_CHR_NO(a2) = get_num();				// �������ͼ��
					ATR_PLACE_NO(a2) = d3;					// ��¼����������
					ATR_INT_WORK0(a2) = ATT_BATTLE_MODEL;	// �趨�� action �� ATT_BATTLE_MODEL ��ʹ��,�Է���֮����ı�ʶ,ʹ�� work0 ��¼
					ATR_INT_WORK1(a2) = 1;					// �趨�˹������Ŀǰ����Ŀ������,ʹ�� work1��¼
					ATR_INT_WORK2(a2) = 0;					// ��Ϊ�Ƿ��һ��ִ���뿪�������
					ATR_INT_WORK3(a2) = 0;					// ��Ϊ������Ȯ��������ټ����Ȯ�����
					// �������ȮЧ��,��¼ʹ����Ȯ��Ŀ��
					if (BattleCmd[command_point] == 'g')
						ATR_LONG_WORK(0, a2) = get_num();
					ATR_SPD(a2) = 32;						// �ƶ��ٶ�
				}
				d7++;
			}
			ATR_COUNTER(a0) = a1;
			ATR_DAMAGE(a0) = -d7;	// ��Ϊ����,������ʱ��� ATR_DAMAGE(p_master) ������� ATR_BODY_CNT(p_master)
			ATR_INT_WORK1(a0) = 0;	// ȷ�� p_missile �Ƿ񶼱����
			ATR_INT_WORK2(a0) = 1;	// �����������ʱ��˳��,�ȴ� ATR_LONG_WORK Ϊ 1 ����ִ��
			ATR_BODY_CNT(a0) = d7;	// �����ܴ���,ÿ���һ�ζ��� ATR_DAMAGE(p_master) �������,ֱ�� ATR_DAMAGE(p_master) ==  ATR_BODY_CNT(p_master)
									// p_master �Ż�ȥ����������һ�� BattleCmd ������� (����� a0 ���� p_master)
			break;
		}
#endif
		d0 = get_num();		//?????�k?
		if (d0 >= 20)		//��????�k???
		{
			if (command_no == ATT_VARIABLE)		//���t�N�ͨ�???
			{
				ATR_ATTRIB(p_master) = get_num();		//�N����?��?
				break;
			}
			else 
			{
#ifdef _DEBUG
				//MessageBox(hWnd, "?????????????????????", "Error", MB_OK);
#endif
				break;
			}
		}
		a1 = p_party[d0];		//???????
		if (ATR_NAME(a1) == NULL)		//???????????????
		{
#ifdef _DEBUG
			//MessageBox(hWnd, "???????????????????????????", "Error", MB_OK);
#endif
			command_no = get_command();		//????��?��?
			if (command_no == -1)		//��???
			{
				ATR_VCT_NO(a0) = 4;		//???��?�r??
				command_point = 0;		//?????��????????
				break;
			}
			command_point -= 2;		//?????????��?
			break;
		}
		if (command_no != ATT_MALFUNCTION)		//??????
		{
			if (ATR_LIFE(a1) <= 0)		//??????
				ATR_VCT_NO(a1) = 0;		//�`??
		}
		if (command_no != ATT_MALFUNCTION)		//??????
		{
			if (ATR_VCT_NO(a1) != 0)		//�r?�֢t?			command_point = sav_command_point;		//????�r?
				break;
		}
		ATR_VCT_NO(a0) = 1;		//�s��?�~�r??
		ATR_DAMAGE(a0) = 0;
		ATR_PET_DAMAGE(a0) = 0;
		ATR_COUNTER(a0) = a1;		//?????��V
		//?????????????
		if (ATR_VCT_NO(a1) != 55)
			ATR_FIRST_FLG(a1) = 0;		//????????
		for (d7 = 0; d7 < 20; d7++)		//??????
		{
			ATR_SYNC_FLG(p_party[d7]) = 0;
			ATR_ATTACK_KIND(0, p_party[d7]) = 0;
			ATR_ATTACK_KIND(1, p_party[d7]) = 0;
			ATR_ATTACK_POW(0, p_party[d7]) = 0;
			ATR_ATTACK_PET_POW(0, p_party[d7]) = 0;
			//andy_mp
			ATR_MPDAMAGE(p_party[d7]) = 0;
			ATR_MPDFLG(p_party[d7]) = 0;
			ATR_ADDHP(p_party[d7]) = 0;
			ATR_ADDHPFLG(p_party[d7])=0;
			ATR_SHOWADDHP(p_party[d7])=0;
#ifdef _PETSKILL_BATTLE_MODEL
			ATR_BATTLE_MODEL(p_party[d7]) = 0;
			ATR_BODY_WORK(0, p_party[d7]) = NULL;
#endif
		}
		//??????
		ATR_ATTACK_KIND(2, a1) = 0;
		switch (command_no)
		{
// BI jibun_5 teki_F flg_2 damage_1 teki_9 flg_2 damage_1 FF
		case ATT_IN:		//??????????  ��??����?  ??????????
			if (ATR_GROUP_FLG(a1) == 0)		//????????
			{
				ATR_V_POS(a1) = 8;
				ATR_H_POS(a1) = -SCREEN_OUT;
			}
			else
			{
				ATR_V_POS(a1) = 480 - 8;
				ATR_H_POS(a1) = 640 + SCREEN_OUT;
			}
		case ATT_HIT:		//====================ֱ�ӹ���===============
			ATR_VCT_NO(a1) = 1;		//ǰ��
			ATR_BODY_WORK(0, a1) = p_party[get_num()];	//������
			ATR_ATTACK_KIND(0, a1) = get_num();		//��������
			ATR_ATTACK_POW(0, a1) = get_num();		//������
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet������
#ifdef _DEBUG
			else
				LogToBattleError( BattleCmd, __LINE__ );
#endif
			ATR_BODY_CNT(a0) = 1;
#ifdef _ATTDOUBLE_ATTACK
			//andy_add
			if (ATR_ATTACK_KIND(0, a1) & ATT_ATTDOUBLE || ATR_ATTACK_KIND(0, a1) & BCF_MODIFY)
				ATR_BATTLEGRANO(a1) = get_num();
#endif
			break;
#ifdef _MAGIC_DEEPPOISION
		case ATT_DEEPPOISION:	//�綾
			ATR_VCT_NO(a1) = 1;	//��������
			ATR_BODY_WORK(0, a1) = p_party[get_num()];//�������		//��?��???
			ATR_ATTACK_KIND(0, a1) = get_num();		//�ҷ���������??��????
			ATR_ATTACK_POW(0, a1) = get_num();		//������??????
			ATR_ATTACK_PET_POW(0, a1) = get_num();//�������
			ATR_BODY_CNT(a0) = 1;//��������
			ATR_DEEPPOISION(a1) = 1;
			break;
#endif
//andy_mp
		case ATT_MPDAMAGE://MP�˺�
			ATR_VCT_NO(a1) = 1;
			ATR_BODY_WORK(0, a1) = p_party[get_num()];
			ATR_ATTACK_KIND(0, a1) = get_num();
			ATR_ATTACK_POW(0, a1) = get_num();
			ATR_ATTACK_PET_POW(0, a1) = get_num();
			ATR_MPDAMAGE(a1) = get_num();
			ATR_BODY_CNT(a0) = 1;
			ATR_MPDFLG(a1)=1;
			break;	//set_damage_num( a1, 16, -64 )

		case ATT_DAMAGETOHP:
			ATR_VCT_NO(a1) = 1;
			ATR_BODY_WORK(0, a1) = p_party[get_num()];
			ATR_ATTACK_KIND(0, a1) = get_num();
			ATR_ATTACK_POW(0, a1) = get_num();
			ATR_ATTACK_PET_POW(0, a1) = get_num();
			ATR_ADDHP(a1) = get_num();
			ATR_BODY_CNT(a0) = 1;
			ATR_SHOWADDHP(a1) = 1;
			ATR_ADDHPFLG(a1)=1;
			break;
//BF jibun_5
		case ATT_FADE_OUT:		//??????????  ��??��?��  ??????????
			ATR_VCT_NO(a1) = 80;		//?�~?
			//??????
			ATR_ATTACK_KIND(2, a1) = 1;
			ATR_BODY_CNT(a0) = 1;
			break;
//BN|��???���k?|?????�k?|
		case ATT_NEXT_EQUIP:		//??????????  ?����?  ??????????
			ATR_VCT_NO(a1) = 82;		//?����?
			ATR_BODY_CNT(a0) = 1;
			break;
//BJ|Attacker's No( 0 - 19 )|Attacker's MP|Attacker's animation index|
//                                                Opposite's animation index|Opposite's index ...
//BJ|???���k?|??|???��?????�k?|?????��?????�k?|?????��|?????��|FF
		case ATT_JUJUTSU:		//??????????  ����  ??????????
			ATR_VCT_NO(a1) = 75;		//?��?���ӡu�a??
			ATR_MP(a1) = get_num();		//?????
			ATR_BODY_CNT(a0) = 1;
#ifdef __ATTACK_MAGIC
			if (ATTACK_MAGIC_ID == ATR_MP(a1))
				BuildAttackMagicData(a0, a1);
#endif
			break;
		case ATT_TOCALL:
			ATR_VCT_NO(a1) = 2;		//?��?���ӡu�a??
			ATR_MP(a1) = get_num();		//?????
			ATR_BODY_CNT(a0) = 1;
#ifdef __TOCALL_MAGIC
			if (TOCALL_MAGIC_ID == ATR_MP(a1))
				BuildToCallMagicData(a0, a1);
#endif
			break;
		case ATT_MALFUNCTION:		//??????????  �O�l����  ??????????
			d0 = get_num();		//���ӧk?��?��?
			ATR_STATUS(a1) = d0;		//????????
			switch (d0)
			{
			//��?��?
			case 0:
				ATR_VCT_NO(a0) = 0;		//????
				a2 = ATR_JUJUTSU_WORK(a1);
				ATR_JUJUTSU_WORK(a1) = NULL;
				DeathAction(a2);		//��?
				break;
			//??
			case 2:
				//��?????
				if (ATR_LIFE(a1) > 0)
				{
					ATR_BODY_CNT(a0) = 1;
					ATR_VCT_NO(a1) = 105;		//??�r??
					ATR_STIMER(a1) = 60;
					set_single_jujutsu(d0, a1);		//�O�l����???
				}
				else
					ATR_VCT_NO(a0) = 0;		//????
				break;
			//?��
			default:
				ATR_VCT_NO(a0) = 0;		//????
				set_single_jujutsu(d0, a1);		//�O�l����???
				break;
			}
			break;
		//ATR_ATTACK_KIND(a1);
/*
		case ATT_MAGICSTATUS:	//andy_add magicstatus
			d0 = get_num();
			ATR_STATUS(a1) = d0;
			switch (d0){
			case 0:
				ATR_VCT_NO(a0) = 0;
				a2 = ATR_MAGICSU_WORK(a1);
				ATR_MAGICSU_WORK(a1) = NULL;
				DeathAction( a2 );
				break;

			default:
				ATR_VCT_NO(a0) = 0;
				set_magic_status(d0, a1);
				break;
			}
			break;
*/

// ???????��????????????�k
		case ATT_DAMAGE:
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 79;
			ATR_LONG_WORK(0, a1) = 0;
			d2 = d0 = get_num();
			d3 = d1 = get_num();
att_damage_loop:
			ATR_DAMAGE(a1) = get_num();
			if (BattleCmd[command_point] == 'p')
				ATR_PET_DAMAGE(a1) = get_num();		//pet damage
			switch (d0)
			{
			case 0:		//??
				switch (d1)
				{
				case 0:		//?
					set_damage_num(a1, 6, -64);
					ATR_LIFE(a1) -= ATR_DAMAGE(a1);
					set_jujutsu_hit_mark(a1);		//����??????����
					if (ATR_LIFE(a1) <= 0)		//??????
					{
						ATR_LIFE(a1) = 0;
						slow_flg++;		//????��
					}
					ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
					set_jujutsu_hit_mark(a1);		//
					//andy_log
					if (ATR_PETFALL(a0) == 1)
					{
						if (ATR_LIFE(a0) > 0)
							petfallChangeGraph(a0);
					}
					if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))		//??????
					{
						ATR_PET_LIFE(a1) = 0;
						//slow_flg++;		//????��
						if (ATR_LIFE(a1) > 0)
							petfallChangeGraph(a1);
					}
					ATR_CHR_ACT(a1) = ANIM_DAMAGE;		//???????????
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;		//???????
					break;
				case 1:		//?
					//??�k����
					set_damage_num(a1, 14, -64);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);
					play_se(102, ATR_H_POS(a1), 240);
					break;
				//andy_add	�غϲ�Ѫ
				case 2:
					if (BattleCmd[command_point] == 'm')
						ATR_MPDAMAGE(a1) = get_num();
					set_damage_num(a1, 36, -64);
					ATR_LIFE(a1) += ATR_DAMAGE(a1);
					ATR_MP(a1) += ATR_MPDAMAGE(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);
					ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
					if (ATR_PET_LIFE(a1) >= ATR_PET_MAX_LIFE(a1))
					{
						int k;

						k = ATR_PET_MAX_LIFE(a1);
						k = ATR_PET_LIFE(a1);
						ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);
						k = ATR_PET_LIFE(a1);
					}
					//ATR_CHR_ACT(a1) = ANIM_DAMAGE;
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 3:
					set_damage_num(a1, 37, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 4://��
					set_damage_num(a1, 38, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 5://��
					set_damage_num(a1, 39, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
				case 6://��
					set_damage_num(a1, 40, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#ifdef _SKILL_ADDBARRIER
				case 7://����
					set_damage_num(a1, 43, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif 
#ifdef _PETSKILL_PEEL
				case 8://��ж��װ��
					set_damage_num(a1, 44, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;
#endif
#ifdef _PETSKILL_ADDATTCRAZED
			/*	case 10:
					set_damage_num(a1, 46, -64);
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a1) = 78;
					break;*/
#endif
				}
				break;

			case 1:		//??
				switch (d1)
				{
				case 0:		//?
					set_damage_num(a1, 16, -64);
					ATR_MP(a1) -= ATR_DAMAGE(a1);		//??????
					if (ATR_MP(a1) < 0)		//?�t???
						ATR_MP(a1) = 0;		//????
					break;
				case 1:		//?
					set_damage_num(a1, 15, -64);
					ATR_MP(a1) += ATR_DAMAGE(a1);		//?????

#ifdef _FIXSHOWMPERR //Syu ADD ��������ʱ������ʾ����
					if (ATR_MP(a1) > pc.maxMp)
						ATR_MP(a1) = pc.maxMp;
#else
					if (ATR_MP(a1) > 100)		//????��??
						ATR_MP(a1) = 100;		//??????
#endif
					//???
					play_se(102, ATR_H_POS(a1), 240);
					break;
				}
				break;
#ifdef _PETSKILL_LER
			case 3:	// ��������
				set_damage_num(a1, 0, -64);
				break;
#endif
			}
			d6 = command_point;		//????????�~��
			d0 = get_command();		//??????��?��?
			if (d0 == ATT_DAMAGE)		//??????
			{
				a1 = p_party[d0 = get_num()];		//???????
				d0 = get_num();		//?????��?��?
				if (d2 == d0)		//????�i???
				{
					d1 = get_num();		//?????��?��?
					if (d3 == d1)		//????�i???
						goto att_damage_loop;
				}
			}
			command_point = d6;		//??????????
			break;
//BV|???���k??????|�N�ͧk?
		case ATT_VARIABLE:		//??????????  ?????�N�ͨ�?  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 85;		//?????�N�ͨ�??
			break;
//BR|????��|????????
		case ATT_REVERSE:		//??????????  �N�ͧT��  ??????????
			ATR_VCT_NO(a0) = 0;		//????
			d0 = get_num();		//?????????��?��?
			switch (d0)
			{
			//???
			case 0:
				a2 = ATR_ATTRIB_WORK(a1);
				ATR_ATTRIB_WORK(a1) = NULL;
				DeathAction(a2);		//��?
				break;
			//??
			default:
				set_attrib_reverse(a1);		//�N�ͧT��???
				break;
			}
			break;
//BL|????��|??
		case ATT_LIFE:		//??????????  ��?��?  ??????????
			ATR_VCT_NO(a0) = 0;		//????
			ATR_DAMAGE(a1) = get_num();		//??��?��?
			//??�k����
			ATR_LIFE(a1) = ATR_DAMAGE(a1);
			if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
				ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
			ATR_VCT_NO(a1) = 0;		//�_��?
			ATR_CHR_ACT(a1) = ANIM_STAND;		//�_��???????
			pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
			break;
//BQ|????��
		case ATT_QUIT:		//??????????  ???�`??  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 87;		//???�`??�q?
			break;
//BX|????��
		case ATT_NIX:		//??????????  ????�r  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 90;		//????�r
			break;
//B!|??????????????��????
		case ATT_COMPANIONS:	//??????????  �s????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 95;		//�s?????��?
			break;
//B#|??��?????��?��????
		case ATT_STEAL:	//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 100;		//??????
			break;
//B%|????��????�k?
		case ATT_TALK:	//??????????  ????  ??????????
			ATR_BODY_CNT(a0) = 1;
			ATR_VCT_NO(a1) = 110;		//?????
			ATR_STIMER(a1) = 60;
			d0 = get_num();		//???�k?��?��?
			//?????????����
			set_damage_num(a1, 20, -64);
			break;
#ifdef _FIREHUNTER_SKILL				// (���ɿ�) ROG ADD ��ȸ����_������ɱ
		case ATT_FIRESKILL:
			ATR_VCT_NO(a1) = FIRE_HUNTER_SKILL;		//������ɱ
			ATR_FIRST_FLG(a1) = 0;
			iBeAttNum = get_num();
			ATR_BODY_WORK(0, a1) = p_party[iBeAttNum];		//����λַ
			ATR_ATTACK_KIND(0, a1) = get_num();				//��������
			ATR_ATTACK_POW(0, a1) = get_num();				//������
//			if ( BattleCmd[command_point] == 'p' )	{
			ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet������??
//			}
			d0 = get_num();							//����
			ATR_BODY_CNT(a1) = d0;					//���˹���
			for (d7 = 2; d7 < d0 + 2; d7++)
			{
				int x = get_num();

				if (x == iBeAttNum)               //������Ŀ���ħ���˺�
				{
					tarMgiDem = 1;
					tarpos = d7;//ATR_BODY_CNT(a1)--;
				}
				ATR_BODY_WORK(d7 ,a1) = p_party[x];		//����λַ
				ATR_ATTACK_KIND(d7 ,a1) = get_num();		//��������
				ATR_ATTACK_POW(d7 ,a1) = get_num();				//������
				ATR_ATTACK_PET_POW(d7 ,a1) = get_num();		//pet������
				//}
			}
			break;
#endif
#ifdef _PROFESSION_ADDSKILL
		case ATT_BOUNDARY:
			ATR_VCT_NO(a0) = 0;
			ATR_CHR_ACT(a0) = ANIM_STAND;
			ATR_FIRST_FLG(a0) = 0;	
			ATR_VCT_NO(a1) = 0;		//�_��?
			ATR_CHR_ACT(a1) = ANIM_STAND;		//�_��???????
			ATR_FIRST_FLG(a1) = 0;
			d0 = get_num();		//?????????��?��?
			BuildBoundaryMagicData( d0 );
			d0 = get_num();
			break;
#endif
		case ATT_BOW:		//??????????  ???  ??????????
			ATR_VCT_NO(a1) = 30;		//????
			ATR_LONG_WORK(0, a1) = get_num();		//�ħk????
			if (ATR_LONG_WORK(0, a1) == 0)		//???
			{
				ATR_VCT_NO(a1) = 32;		//????
				ATR_STIMER(a1) = 0;		//?��???????
				ATR_DAMAGE(a1) = 1;		//
				ATR_PET_DAMAGE(a1) = 1;
			}
			ATR_FIRST_FLG(a1) = 0;		//
			ATR_BODY_WORK(0, a1) = p_party[get_num()];		//��?��???
			ATR_ATTACK_KIND(0, a1) = get_num();		//??��????
			ATR_ATTACK_POW(0, a1) = get_num();		//??????
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
			ATR_BODY_CNT(a0) = 1;
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
			(ATR_CHR_NO(a1) == 101578) ? ShooterNum = 101578 : ShooterNum = -1;
#endif
			break;
		case ATT_BOOMERANG:		//??????????  ???????  ??????????
			ATR_VCT_NO(a1) = 70;		//????????
			ATR_FIRST_FLG(a1) = 0;		//
			ATR_BODY_CNT(a0) = 1;
			break;
		case ATT_FIRE:		//??????????  ?��???  ??????????
			d0 = get_body_cnt();		//??��?��?��?
			if (d0 != 1)		//�]�D????
			{
				ATR_VCT_NO(a1) = 20;		//???��?
				ATR_BODY_CNT(a0) = d0;		//??�D??
				ATR_BODY_CNT(a1) = d0;		//
				if (ATR_GROUP_FLG(a1) == 0)		//????????
					ATR_CHR_ANG(a1) = 3;		//��?��?
				else
					ATR_CHR_ANG(a1) = 7;		//
			}
			else
			{	//?�D????
				ATR_VCT_NO(a1) = 20;		//???��?
				ATR_BODY_CNT(a0) = 1;		//?�D??
				ATR_BODY_CNT(a1) = 1;		//
				a2 = ATR_BODY_WORK(0, a1) = p_party[get_num()];		//��?��
			}
			break;
		case ATT_SYNCHRONOUS:		//??????????  �i???  ??????????
			a3 = a2 = a1;		//��?���~��
			d1 = 0;		//????????
			d2 = 0;
			d7 = 0;		//??��????
			while (1)
			{
				d0 = get_num();		//???????
				if (d0 == 255)		//��???
					break;
				a1 = p_party[d0];		//???????
				if (ATR_NAME(a1) == NULL)		//????????
				{
#ifdef _DEBUG
					//MessageBox(hWnd, "û�к��幥�������ֽ������ձ���", "Error", MB_OK);
#endif
					get_num();		//??��????
					get_num();		//??????
					get_num();		//????��
					if (BattleCmd[command_point] == 'p')
						get_num();		//pet????��
					continue;
				}
				if (d7 == 0)		//?��????��??
					ATR_COUNTER(a0) = a1;		//?????��?
				ATR_VCT_NO(a1) = 1;		//�q��?
				ATR_BODY_WORK(0, a1) = a3;		//��?��
				ATR_BODY_WORK(1, a2) = a1;		//�q????��?????��???
				ATR_ATTACK_KIND(0, a1) = get_num();
#ifdef _PETSKILL_ACUPUNCTURE
///////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_TRAP+ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				+ ATT_ATTACKBACK
		#endif
				)
	#else
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
					+ ATT_ATTACKBACK
		#endif
				)
	#endif
///////////////////////
#else
///////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA+ATT_TRAP)
	#else
				if (ATR_ATTACK_KIND(0, a1) & ATT_REFLEX+ATT_ABSORPTION+ATT_BALLIA)
	#endif
///////////////////////
#endif
				{
					ATR_ATTACK_POW(0, a1) = get_num();		//??????
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
				}
				else
				{
					d1 += ATR_ATTACK_POW(0, a1) = get_num();		//????��
					if (BattleCmd[command_point] == 'p')
						d2 += ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet????��
					else
						LogToBattleError( BattleCmd, __LINE__ );
				}
				ATR_FIRST_FLG(a1) = 0;		//??????
				ATR_SYNC_FLG(a1) = 1;		//
				a2 = a1;		//�q????��
				d7++;		//??��??��
			}
			ATR_SYNC_FLG(a1) = 2;		//?????
			if (d1)		//???????
				ATR_ATTACK_POW(0, a1) = d1;		//????????
			if (d2)		//???????
				ATR_ATTACK_PET_POW(0, a1) = d2;		//????????
			ATR_LONG_WORK(0, a3) = d7;		//??????
			ATR_BODY_CNT(a0) = d7;
			break;
		case 2:		//??????????  ???��?  ??????????
			d0 = get_num();		//??��?��?��?
			if (d0 != 1)		//??�D????
			{
				ATR_VCT_NO(a1) = 25;		//???��?
				ATR_BODY_CNT(a0) = d0;		//??�D??
				ATR_BODY_CNT(a1) = d0;		//
				for (d7 = 0; d7 < d0; d7++)
				{
					ATR_BODY_WORK(d7, a1) = p_party[get_num()];		//��?��
					ATR_ATTACK_KIND(d7, a1) = get_num();		//??��????
				}
				d0 = 320;
				d1 = 240;
				radar(a1, &d0, &d1);	//????
				ATR_CRS(a1) = d0;		/* ?????? */
				ATR_CHR_ANG(a1) = crs_change_tbl[d0];		/* ????? */
			}
			else
			{
				ATR_VCT_NO(a1) = 25;		//???��?
				ATR_BODY_CNT(a0) = 1;		//?�D??
				ATR_BODY_CNT(a1) = 1;		//
				ATR_BODY_WORK(0, a1) = p_party[get_num()];		//��?��
				ATR_ATTACK_KIND(0, a1) = get_num();		//??��????
				d0 = 320;
				d1 = 240;
				radar(a1, &d0, &d1);	//????
				ATR_CRS(a1) = d0;		/* ?????? */
				ATR_CHR_ANG(a1) = crs_change_tbl[d0];		/* ????? */
			}
			break;
		case 3:		//??????????  ????  ??????????
			d0 = get_num();		//??��?��?��?
			ATR_VCT_NO(a1) = 30;		//???��?
			ATR_BODY_CNT(a0) = d0;		//?�D??
			ATR_BODY_CNT(a1) = d0;		//
			for (d7 = 0; d7 < d0; d7++)
			{
				ATR_BODY_WORK(d7, a1) = p_party[get_num()];		//��?��
				ATR_ATTACK_KIND(d7, a1) = get_num();		//??��????
			}
			break;
		case ATT_TAKE:		//??????????  ��?  ??????????
			ATR_VCT_NO(a1) = 35;		//��??��
			ATR_BODY_WORK(0, a1) = p_party[get_num()];		//��?��???
			ATR_ATTACK_KIND(0, a1) = get_num();		//��?����???
			ATR_BODY_CNT(a0) = 1;
			break;
		case ATT_ESCAPE:		//??????????  �P�f�D��  ??????????
			ATR_BODY_CNT(a0) = 1;		//�D������????
			d1 = ATR_ATTACK_KIND(0, a1) = get_num();		//��?����???
			ATR_VCT_NO(a1) = 52;		//�D��?
			if (BattleMyNo == ATR_PLACE_NO(a1))		//����??
				d0 = 1;
			else
				d0 = 0;
			//ACTION_INF�@�e��??????
			ATR_GUARD_FLG(a1) = 0;
			//???��?�V?�ϣ�??
			if (ATR_PET_OK(a1))
			{
				a2 = p_party[ATR_PLACE_NO(a1) + 5];		//???��???
				//??????????????????
				if (ATR_NAME(a2) != NULL && ATR_AKO_FLG(a2) == 0)
				{
					//?????
					if (ATR_ATTACK_KIND(2, a2) == 1)
					{
						ATR_ATTRIB(a2) |= ACT_ATR_HIDE;		//����??
						//????��
						ATR_NAME(a2) = NULL;
						ATR_VCT_NO(a2) = VCT_NO_APPEAR;
					}
					else
					{
						//ACTION_INF�@�e��??????
						ATR_GUARD_FLG(a2) = 0;
						ATR_BODY_CNT(a0)++;		//�D������??��
						if (d0 == 1)		//??��?�D��??
							d0 = 2;
					}
				}
			}
			//ACTION_INF�@�e�ԡ@�e
			switch (d0)
			{
			case 1:
				ATR_GUARD_FLG(a1) = 1;
				break;
			case 2:
				ATR_GUARD_FLG(a2) = 1;
				ATR_GUARD_FLG(a1) = 1;
				break;
			}
			break;
		case ATT_SELECT:		//??????????  ???�k��  ??????????
			ATR_BODY_CNT(a0) = 1;
			d0=ATR_PLACE_NO(a1) + 5;		//?????????
			a2 = p_party[ATR_PLACE_NO(a1) + 5];		//?????????
			ATR_BODY_WORK(0, a2) = a1;		//??��??????
			ATR_FIRST_FLG(a2) = 0;		//??????
			if (get_num() == 0)		//????
				ATR_VCT_NO(a2) = 60;		//?????
			else
			{	//��???
				ATR_NAME(a2) = monster;		//???��?
				ATR_VCT_NO(a2) = 62;		//
				if (BattleMyNo == ATR_PLACE_NO(a1))		//����??
					att_select_flg = true;
			}
			break;
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		case ATT_PROSKILL:
			ATR_VCT_NO(a1) = PROSKILL;					//��ʿְҵ����
			ATR_BODY_WORK(0, a1) = p_party[get_num()];	//ȡ������������No
			ATR_ATTACK_KIND(0, a1) = get_num();			//��������
			ATR_ATTACK_POW(0, a1) = get_num();			//������
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
			else
				LogToBattleError(BattleCmd, __LINE__);
			ATR_WARRIOR_EFFECT(a1) = get_num();
			AttPreMagicNum = get_num();
			AttNextMagicNum = get_num();
			ATR_BODY_CNT(a0) = 1;
#ifdef _ATTDOUBLE_ATTACK
			//andy_add
			if (ATR_ATTACK_KIND(0, a1) & ATT_ATTDOUBLE || ATR_ATTACK_KIND(0, a1) & BCF_MODIFY)
				ATR_BATTLEGRANO(a1) = get_num();
#endif
			break;
//#endif
		default:	//???????
			action_inf = -1;
					//?????��????????
#ifdef _DEBUG
			{
				char errbuf[256];

				sprintf(errbuf, "errbuf:%c", command_no);
				//MessageBox(hWnd, errbuf, "Error", MB_OK);
			}
#endif
			command_point = 0;
			break;
		}
#ifdef _PET_ITEM
		if (ATR_ATTACK_KIND(0, a1) & ATT_ATTACKBACK)
		{
			ATR_ATTACK_POW(1, a1) = ATR_ATTACK_POW(0, a1) >> 16;
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a1) & 0x0000ffff;
		}
#endif
	case 1:		//�s��?�~�r?
#ifdef __ATTACK_MAGIC
		if (true == bRunAttMgc)
			RunTimeMagic();
#endif
#ifdef __TOCALL_MAGIC
		if (true == bRunToCallMgc)
			RunTimeMagicToCall();
#endif
#ifdef _PROFESSION_ADDSKILL
//		RunTimeMagicBoundary();
#endif
		if (ATR_DAMAGE(a0) == ATR_BODY_CNT(a0))
		{	//?�~��???
			for (d7 = 0; d7 < 20; d7++)
			{
				if (ATR_NAME(p_party[d7]) != NULL)
				{
					//?��??�r???????
					if (ATR_LIFE(p_party[d7]) > 0 && ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) < VCT_NO_DIE)
						break;
				}
			}
			if (d7 == 20)		//��???
				ATR_VCT_NO(a0) = 0;		//�s��?�h?
		}
		break;
	case 2:		//�u��
		for (d7 = 0; d7 < 20; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				//��??????
				if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
					break;
			}
		}
		if (d7 == 20)
		{	//��???
			action_inf = 2;
			command_point = 0;		//?????��????????
		}
		break;
	case 3:		//�K�͢r?
		for (d7 = 0; d7 < 20; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)		//?��??�r???????
					break;
			}
		}
		if (d7 == 20)
		{
			ATR_VCT_NO(a0) = 0;
			action_inf = 3;		//�K��?����?
			if (BattleBpFlag & BATTLE_BP_JOIN)// �ӡP??
			{
				//�u��??
				if (check_all_dead())
					action_inf = 2;
			}
		}
		break;
	case 4:		//???��?�r?
		for (d7 = 0; d7 < 20; d7++)
		{
			if (ATR_NAME(p_party[d7]) != NULL)
			{
				//�u��??
				if (check_all_dead())
				{
					//?��??�r???????
					if (ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
						break;
				}
				else
				{
					//����?????????????
					if (BattleMyNo == d7)
					{
						if (ATR_VCT_NO(p_party[d7]) == 55)
							break;
					}
					//?��??�r???????
					if (ATR_LIFE(p_party[d7]) > 0 && ATR_VCT_NO(p_party[d7]) != 0 && ATR_VCT_NO(p_party[d7]) != VCT_NO_DIE + 2)
						break;
				}
			}
		}
		if (d7 == 20)
		{
			ATR_VCT_NO(a0) = 0;
			//�t�@�e??
			if (!action_inf)
				action_inf = 1;		//???��?
			//�u��??
			if (check_all_dead())
				action_inf = 2;
		}
		break;
	}
master_500:
	//???????????
#ifdef __ATTACK_MAGIC
		if ((BattleMapNo >= 148 && BattleMapNo <= 150 ) || g_iRunEarthQuake >= 2)
#else
		if (BattleMapNo >= 148 && BattleMapNo <= 150)		//???????????
#endif
		{
			//????
			for (d7 = 0; d7 < 20; d7++)
			{
				a1 = p_party[d7];		//???????
				if (ATR_NAME(a1) == NULL)		//???
					continue;
				ATR_V_POS(a1) += ATR_INT_WORK3(a1);
				ATR_INT_WORK1(a1) += ATR_INT_WORK3(a1);
				//????????????
				set_raster_pos(a1);
			}
			if (g_iRunEarthQuake == 3)
				g_iRunEarthQuake = 0;
		}
	//??????����???
	d6 = 0;
	for (d7 = 0; d7 < 20; d7++)
	{
		a1 = p_party[d7];		//???????		
		if (ATR_NAME(a1) == NULL)		//???
			continue;
		sort_chr_buf[d6].work = a1;		//???????
		sort_chr_buf[d6++].v_pos = ATR_V_POS(a1);		//??��???
	}
	for (d7 = 0; d7 < 20; d7++)
	{
		if ((a1 = p_missile[d7]) == NULL)		//��???
			continue;
		sort_chr_buf[d6].work = a1;		//???????
		sort_chr_buf[d6++].v_pos = ATR_V_POS(a1);		//??��???
	}
	//???????
	qsort(	sort_chr_buf,			// ????
			d6,						// �{?????
			sizeof(SORT_CHR_EQU),	// ???
			(CMPFUNC*)sort_chr		// �{?????????
	);
	d0 = 30;		//����?�I???
	for (d7 = 0; d7 < d6; d7++)
		ATR_DISP_PRIO(sort_chr_buf[d7].work) = d0++;
}

//�����ж�����
void monster(ACTION *a0)
{
	int d0, d1, d6, d7, sav_command_point, idx;
	ACTION *a1, *a2, *a3;
	static ACTION *a0tmp[10];
	static int a0mark[10];				// 0:�ѽ�action release 1:δ
	static int a0tmpcount;

	switch (ATR_VCT_NO(a0))
	{
	case 0:		//����
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		if (ATR_VCT_NO(p_master) == 1)
		{
			a1 = ATR_COUNTER(p_master);		//??��???????
			if (a1 != NULL && ATR_GROUP_FLG(a0) != ATR_GROUP_FLG(a1) && ATR_NAME(a1) != NULL)
			{
				//?����??
				if (ATR_H_POS(a1) < 640 && ATR_H_POS(a1) > 64 && ATR_V_POS(a1) < 480 && ATR_V_POS(a1) > 0-64)
				{
					if (ATR_STATUS(a1) <= JUJUTSU_1)		//??????����?��??
					{
						a1 = ATR_COUNTER(p_master);		//��????????
						d0 = ATR_H_POS(a1);				//��?��??
						d1 = ATR_V_POS(a1);
						radar2(a0, d0, d1, 2);	//????
						//??��??????
						d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
						if (d0 < 0)
							d0 = 0 - d0;
						if (d0 >= 2)
						{
							ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
							ATR_CRS_OLD(a0) = ATR_CRS(a0);
						}
					}
				}
			}
		}
		//��???��???????
		d0 = ATR_STATUS(a0);
		if (ATR_STATUS(a0) != 3 && ATR_STATUS(a0) != 4)
		{
			//?�P??????�e????
			if (BattleMyNo >= 20 || (ATR_ATTRIB(a0) & ACT_ATR_BTL_CMD_END)
#ifdef _PETSKILL_BATTLE_MODEL
				|| ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL
#endif
			){
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			}
			//���ｫͼƬ���¼��غ���Ⱦ wxy
			//pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		}
#ifdef _PETSKILL_BATTLE_MODEL
		// ��黻�ҹ�����û
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			a1 = ATR_BODY_WORK(0, a0);	// ȡ��������Ŀ��
			// ���Ⱥ�˳��ִ��
			if (ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) == ATR_INT_WORK2(p_master))
			{
				// ��Ŀ��ص�����ʱ�ٹ���
				if (ATR_VCT_NO(a1) == 0)
					ATR_VCT_NO(a0) = 2; // ��ʼ����
				// ��Ŀ���뿪ս��������ʱ��������
				if (ATR_VCT_NO(a1) == VCT_NO_APPEAR || ATR_LIFE(a1) <= 0)
				{
					ATR_VCT_NO(a0) = 3; // ��������
					ATR_BODY_WORK(0, a0) = NULL;	// ���
				}
			}
		}
#endif
		break;
	case 1:		//ǰ��
		ATR_SPD(a0) = 63;		//�ƶ��ٶ�
		a1 = ATR_BODY_WORK(0, a0);		//�з���λ��
		d0 =  ATR_H_POS(a1);
		d1 =  ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		//??��??????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
		}
		if (d1 <= 32 * 2)
		{	//����
#ifdef _PIRATE_ANM			
			if (ATR_CHR_NO(a0) == 101491)
				ATR_CHR_NO(a0) = 101490;				
#endif
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_HIT_STOP(a0) = 0;		//??????????
#ifdef _PETSKILL_BATTLE_MODEL
			if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
				ATR_VCT_NO(a0) = 0; // ����֮���ȴ���
			else
#endif
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)
				{	//����??????
					ATR_VCT_NO(a0) = 65;		//����??????�r?
					ATR_BODY_WORK(1, a0) = ATR_BODY_WORK(0, a0);		//��????�~��
					a1 = ATR_BODY_WORK(0, a0);	//??��???��???
					ATR_BODY_WORK(1, a1) = a0;	//
					a1 = p_party[get_num()];	//����??????????
					ATR_VCT_NO(a1) = 69;		//??????����?
					ATR_FIRST_FLG(a1) = 0;		//
					ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//?�h�I??????
				}
				else if (ATR_SYNC_FLG(a0))		//�i?????
				{
					ATR_VCT_NO(a0) = 19;		//�i???�r??
					a1 = ATR_BODY_WORK(0, a0);		//??����?��?
					ATR_LONG_WORK(0, a1)--;		//???��
				}
				else	//andy_bug
					ATR_VCT_NO(a0) = 2;		//???
			}
		}
		else
		{
#ifndef _PIRATE_ANM			
			gemini(a0);		//?�h
#else
			if (ATR_CHR_NO(a0) != 101490)
				gemini(a0);		//?�h
#endif
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
#ifndef _PIRATE_ANM
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
#else		
		if (ATR_CHR_NO(a0) == 101490)
		{			
			pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
			if (a0->bmpNo == 301137)
				ATR_CHR_NO(a0) = 101491;
		}
		else
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
		}
#endif
		break;
	case 2:		//����
		if (ATR_HIT_STOP(a0))			//??????????
		{
			if (--ATR_HIT_STOP(a0))		//??????????
				break;
		}
		a1 = ATR_BODY_WORK(0, a0);		//��????????//�з�ָ��
#ifdef _PETSKILL_BATTLE_MODEL
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL && ATR_FIRST_FLG(a0) == 0 && ATR_INT_WORK3(a0) == 0)
		{
			if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)			// ��Ȯ
			{
				ATR_VCT_NO(a0) = 65;							// ��������Ƚ��еȴ�
				ATR_BODY_WORK(1, a1) = a0;						// �ѹ��������¼����������,֮����õ�
				a1 = p_party[ATR_LONG_WORK(0, a0)];				// ȡ������� action
				ATR_VCT_NO(a1) = 69;							// ��ʾ damage ����
				ATR_FIRST_FLG(a1) = 0;
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);	// �����ǰ��Ŀ��������
				ATR_INT_WORK3(a0) = 1;
				break;
			}
			ATR_SYNC_FLG(a1) = 1;
		}
#endif
		if (ATR_FIRST_FLG(a0) == 0)			//???????
		{
			ATR_HIT(a0) = 0;				//?????????
			ATR_HIT_TIMING(a0) = 0;			//???��???????
			d0 = ATR_H_POS(a1);				//��?��??
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//???��V
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			//??????????
			if (ATR_ATTACK_KIND(0, a0) & ATT_G_CRASH)
				//????????����
				set_damage_num(a0, 12, -64 + 16);
			else if (ATR_ATTACK_KIND(0, a0) & ATT_TOOEH)
				set_damage_num(a0, 29, -64 + 16);
#ifdef _ATTDOUBLE_ATTACK
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTDOUBLE)
			{
				int skill = ATR_BATTLEGRANO(a0);
				switch (skill)
				{
				case 70:
					set_damage_num(a0, 25, -64 + 16);
					break;
				case 71:
					set_damage_num(a0, 26, -64 + 16);
					break;
				case 72:
					set_damage_num(a0, 27, -64 + 16);
					break;
				case 73:
					set_damage_num(a0, 28, -64 + 16);
					break;
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & BCF_MODIFY)
			{
				int skill = ATR_BATTLEGRANO(a0);

				switch (skill)
				{
				case 74://74
					set_damage_num(a0, 32, -64 + 16);
					break;
				case 75://75
					set_damage_num(a0, 33, -64 + 16);
					break;
				case 76://76
					set_damage_num(a0, 34, -64 + 16);
					break;
				case 77://77
					set_damage_num(a0, 35, -64 + 16);
					break;
				}
			}
#endif
#ifdef _SKILL_ROAR  //�輼:���(������)
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTROAR)
			{
				set_damage_num(a0, 22, -64 + 16);
				ATR_ROAR(a1) = 2;
			}
#endif
#ifdef _SKILL_SELFEXPLODE //�Ա�
			else if (ATR_ATTACK_KIND(0, a0) & ATT_ATTSELFEXPLODE)
			{
				ATR_SELFEXPLODE(a0) = 1;
				set_damage_num(a0, 23, -64 + 16);
			}
#endif
#ifdef _PETSKILL_EXPLODE //����
			else if (ATR_ATTACK_KIND(0, a0) & ATT_EXPLODE)
			{
				//ATR_SELFEXPLODE(a0) = 1;
				ATR_LIFE(a0) = 1;
				set_damage_num(a0, 23, -64 + 16);
			}
#endif
			else
			{
				//andy_fall
				if (ATR_ATTACK_KIND(0, a0) & ATT_FALL)
				{
					if (ATR_RIDE(a1) == 1)
					{
						//set_damage_num(a0, 12, -64+16); //�������ݲ���ͼ
						ATR_PETFALL(a1) = 1;
					}
				}
			}
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTPREPROSKILL)
					set_damage_num(a1, 41, -64 + 25);
//#endif
			//�i????����????
			if (ATR_SYNC_FLG(a0))
				d0 = 0;
			else
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					d0 = 0;
				else
#endif
					d0 = get_next_flg();		//?????��?????
			}
			if (d0 & ATT_COUNTER)			//??????????
			{
				ATR_COUNTER_FLG(a0) = 1;
				ATR_COUNTER_FLG(a1) = 1;
				ATR_STIMER(a0) = 255;
			}
			else
			{
				ATR_COUNTER_FLG(a0) = 0;
				ATR_COUNTER_FLG(a1) = 0;
				ATR_STIMER(a0) = 20;
			}
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
//ATT_AKO1				
#ifdef _PETSKILL_ANTINTER
		if (ATR_VCT_NO(a1) >= VCT_NO_DIE && (ATR_ATTACK_KIND(0, a1) & (1 << 28)))		//��?��??
#else
		if (ATR_VCT_NO(a1) >= VCT_NO_DIE)		//��?��??
#endif
		{
			if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER && ATR_COUNTER(p_master) != a0)		//???????
				ATR_VCT_NO(a0) = 0;		//�r??
			else
			{
				ATR_VCT_NO(a0) = 3;
				ATR_STIMER(a0) = 20;
#ifdef _PETSKILL_BATTLE_MODEL
				// �������
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) = 0;
#endif
			}
			break;
		}
#ifdef _SKILL_SELFEXPLODE //�Ա�
	    if (ATR_SELFEXPLODE(a0) == 1)
		{
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_CHR_ACT(a0) = ANIM_ATTACK;
		}
		else
#endif
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER && ATR_COUNTER(p_master) != a0)		//???????
				ATR_VCT_NO(a0) = 0;		//�r??
			else
			{
				ATR_VCT_NO(a0) = 3;
#ifdef _PETSKILL_BATTLE_MODEL
				// �������
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
					ATR_LONG_WORK(ATR_PLACE_NO(a0), a1) = 0;
#endif
			}
			break;
		}
		//???????
		if (ATR_HIT(a0))
		{
			//andy_mp
			if (ATR_SHOWADDHP(a0) == 1)
			{
				set_damage_num(a0, 6, -64);
				ATR_SHOWADDHP(a0) = 0;
				ATR_LIFE(a0) += ATR_ADDHP(a0);
			}
			ATR_ATTACK_KIND(1, a1) = ATR_ATTACK_KIND(0, a0) & ATT_DEATH;
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
				ATR_ATTACK_KIND(1, a1) = 0;
#endif
#ifdef _PETSKILL_ACUPUNCTURE
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA  | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
//////////////////////
#else
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA | ATT_TRAP))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ABSORPTION | ATT_BALLIA))
	#endif
//////////////////////
#endif
			{
				set_guard_mark(a0);
#ifdef _PETSKILL_ACUPUNCTURE
				if (ATR_ATTACK_KIND(0, a0) & (ATT_ACUPUNCTURE
	#ifdef _PET_ITEM
					| ATT_ATTACKBACK
	#endif
				))
				{
	#ifdef _PETSKILL_BATTLE_MODEL
					if (ATR_INT_WORK0(a0) != ATT_BATTLE_MODEL)
	#endif
					{
						ATR_VCT_NO(a1) = 10; //�趨�Լ�����
						ATR_DAMAGE(a1) = ATR_DAMAGE(a0);//ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0)*2;
						//ATR_ATTACK_KIND(0, a1) &= ATT_AKO2;
						if ((float)ATR_DAMAGE(a1) >= (float)(ATR_MAX_LIFE(a1) * 1.2 + 20.0))
						{
							ATR_LIFE(a1) = 0;
							ATR_AKO_FLG(a1) = 2; //���
						}
						a1 = ATR_BODY_WORK(0, a0) = a0;
					}
				}
#endif
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)
				{
					a1 = ATR_BODY_WORK(0, a0);
					ATR_VCT_NO(a1) = 6;
					ATR_STIMER(a1) = 60;
				}
			}
#ifdef _PETSKILL_ACUPUNCTURE
///////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
	#endif
///////////////////
#else
///////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
	#else
			if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
	#endif
///////////////////
#endif
			{
#ifdef _PETSKILL_BATTLE_MODEL
				// ����ǲ��ǹ������
				if (ATR_INT_WORK0(a0) != ATT_BATTLE_MODEL)
#endif
				a1 = ATR_BODY_WORK(0, a0) = a0;
			}
			else
				a1 = ATR_BODY_WORK(0, a0);		//��????????xxx
			//?????????
			ATR_COMBO(a1) = 0;
			//?��??????
			if (ATR_ATTACK_KIND(0, a0) & ATT_DODGE)
			{
				//???????????
				if (ATR_HIT_TIMING(a0) == 0)
				{
					ATR_HIT_TIMING(a0) = 1;		//???��???????
					ATR_VCT_NO(a1) = 16;		//????
					ATR_CRS(a1) = ATR_CRS(a0);		//��?????????????
					ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
					ATR_STIMER(a1) = 0;
					ATR_FIRST_FLG(a1) = 1;		//?????????
					ATR_BODY_WORK(0, a1) = a0;	//
					if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
						//?????����
						set_damage_num(a0, 1, -64 + 16);
				}
				ATR_HIT(a0) = 0;
				play_se(8, ATR_H_POS(a0), ATR_V_POS(a0));
				break;
			}

			//????????
			if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
			{
				//?��????????
				if (ATR_ATTACK_KIND(0, a0) & ATT_CRUSH)
					//?????����
					set_damage_num(a1, 19, -112);
			}
			//?????????//knock out
			ATR_AKO_FLG(a1) = 0;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO1)		//??????
				ATR_AKO_FLG(a1) = 1;
			if (ATR_ATTACK_KIND(0, a0) & ATT_AKO2)		//??????
				ATR_AKO_FLG(a1) = 2;
			//���� ?????
			if (ATR_HIT(a0) >= 10100)
			{
#ifdef _PETSKILL_ACUPUNCTURE
///////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				))
	#else
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				))
	#endif
///////////////////////
#else
///////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
	#else

				if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
	#endif
///////////////////////
#endif
				{
					ATR_HIT(a0) -= 100;	
					if (ATR_SYNC_FLG(a0) == 1)
					{
						a2 = ATR_BODY_WORK(1, a0);
						ATR_VCT_NO(a2) = 2;	
						ATR_COUNTER(p_master) = a2;	
					}
				}
				else
					//?????????
					ATR_COMBO(a1) = 1;
			}
			else if (ATR_SYNC_FLG(a0) == 1)
			{
#ifdef _PETSKILL_ACUPUNCTURE
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				)))
	#else
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				)))
	#endif
/////////////////////
#else
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (!(ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) ))
	#else
				if (!(ATR_ATTACK_KIND(0, a0) & ATT_REFLEX))
	#endif
/////////////////////
#endif
					ATR_COMBO(a1) = 1;
				a2 = ATR_BODY_WORK(1, a0);		//????����?��?
				ATR_VCT_NO(a2) = 2;		//???
				ATR_COUNTER(p_master) = a2;		//?????��?
			}
#ifdef _PETSKILL_ACUPUNCTURE
/////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				))
	#else
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
				))
	#endif
////////////////////
#else
/////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) )
	#else
			if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
	#endif
////////////////////
#endif
			{
#ifdef _PETSKILL_BATTLE_MODEL
				// ����ǲ��ǹ������
				if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
				{
					ATR_ATTACK_POW(0, a0) = 0;		// ���˺�
					ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);
					ATR_ATTACK_KIND(0, a0) |= ATT_NOMISS;
				}
				else
#endif
					ATR_DAMAGE_ANG(a1) = (ATR_CHR_ANG(a0) + 4) & 7;
			}
			else
				ATR_DAMAGE_ANG(a1) = ATR_CHR_ANG(a0);		//
			if (ATR_ATTACK_KIND(0, a0) & BCF_DEFMAGICATT)
				set_damage_num(a1, 30, -64);
			if (ATR_ATTACK_KIND(0, a0) & BCF_SUPERWALL)
				set_damage_num(a1, 31, -64);
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTNEXTPROSKILL)
				set_damage_num(a1, 41, -64 + 25);
//#endif
			if (ATR_ATTACK_KIND(0, a0) & ATT_ABSORPTION)	//?��?????
			{
				if (ATR_SYNC_FLG(a0) == 0)		//�i????????
				{
					if (!ATR_COMBO(a1))			//????????
					{
						//??�k����
#ifdef _SYUTEST
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						if (Light1 == NULL)
							Light1 = MakeAnimDisp(ATR_H_POS(a0), ATR_V_POS(a0), 101581, 0);
						set_damage_num(a1, 41, -64 + 25);
			// �ɹ���Ԥ��
#else
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
						ATR_LIFE(a1) += ATR_DAMAGE(a1);
						if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
							ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
#endif
						ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
						ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
						if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
							ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
						set_damage_num(a1, 14, -64);
						ATR_ATTACK_POW(0, a0) = 0;		//????��?
						ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
					}
				}
				else
				{
					//????????
					if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
					{
						//??�k����
						ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
#ifdef _SYUTEST
			// �ɹ���Ԥ��
#endif
						ATR_LIFE(a1) += ATR_DAMAGE(a1);
						if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
							ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
						ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
						ATR_PET_LIFE(a1) += ATR_PET_DAMAGE(a1);
						if (ATR_PET_LIFE(a1) > ATR_PET_MAX_LIFE(a1))		//?����???
							ATR_PET_LIFE(a1) = ATR_PET_MAX_LIFE(a1);		//?��???
						set_damage_num(a1, 14, -64);
						ATR_ATTACK_POW(0, a0) = 0;		//????��?
						ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
					}
				}
			}
			else if (ATR_ATTACK_KIND(0, a0) & ATT_BALLIA)		//????????
			{
				ATR_ATTACK_POW(0, a0) = 0;		//????��?
				ATR_ATTACK_PET_POW(0, a0) = 0;		//????��?
			}
			else
			{
				ATR_VCT_NO(a1) = 10;		//?????��
                
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
				if (ATR_ATTACK_KIND(0, a0) & ATT_NOMISS)
					NoMiss = 1 ; 
				else
					NoMiss = -1 ;
#endif
			}
			//?��?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_SATISFACTORY)
				ATR_KAISHIN(a1) = 1;		//?��??????
			else
				ATR_KAISHIN(a1) = 0;		//�G��?????
			ATR_DAMAGE(a1) = ATR_ATTACK_POW(0, a0);
			ATR_PET_DAMAGE(a1) = ATR_ATTACK_PET_POW(0, a0);
			//andy_mp
			ATR_MPDAMAGE(a1) = ATR_MPDAMAGE(a0);
			ATR_MPDFLG(a1) =  ATR_MPDFLG(a0);
			//?????
			if (ATR_ATTACK_KIND(0, a0) & ATT_GUARD)
				ATR_GUARD_FLG(a1) = 1;
			else
				ATR_GUARD_FLG(a1) = 0;
#ifdef _EQUIT_ARRANGE
			if (ATR_ATTACK_KIND(0, a0) & ATT_ARRANGE)
			{
				set_damage_num(a1, 42, -64);
				ATR_CHR_ACT(a1) = ANIM_GUARD;
			}
#endif
			if (ATR_LIFE(a1) - ATR_DAMAGE(a1) <= 0 || (ATR_ATTACK_KIND(0, a0) & ATT_DEATH))
			{
				if (ATR_COMBO(a1) == 0)
					ATR_KAISHIN(a1) = 1;
			}
			if (ATR_KAISHIN(a1))
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;
			else
				ATR_HIT_STOP(a0) = HIT_STOP_TIM;
			if (ATR_DAMAGE(a1) || (ATR_ATTACK_KIND(0, a0) & (ATT_BALLIA | ATT_ABSORPTION)))
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_COUNTER)		//???????
				{
					//????????
					if (ATR_COMBO(a1) == 0)
						//?????����
						set_damage_num(a0, 1, -64 + 16);
				}
				//����Ч����
				play_damage(ATR_HIT(a0), ATR_H_POS(a0));
				//��ɱ����
				set_hit_mark(a0);
				if (ATR_COMBO(a1))		//С��
					ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//����ֹͣ
			}
			ATR_HIT(a0) = 0;				//����������
			if (!(ATR_ATTACK_POW(0, a0) || ATR_ATTACK_PET_POW(0, a0)))		//????
			{
				ATR_HIT_STOP(a0) = 0;		//����ֹͣ
				play_se(8, ATR_H_POS(a0), ATR_V_POS(a0));
			}
		}
		break;

	case 3:		//�����ȴ�
#ifdef _PETSKILL_BATTLE_MODEL
		// ����ǲ��ǹ������
		if (ATR_INT_WORK0(a0) == ATT_BATTLE_MODEL)
		{
			// ���˹����������û��Ŀ��
			if (ATR_INT_WORK1(a0) > 1)
			{
				// �ҳ���һ��Ŀ��
				for (d0 = 1; d0 < ATR_INT_WORK1(a0); d0++)
				{
					if (ATR_BODY_WORK(d0, a0) != NULL)	// �ҵ�Ŀ��
					{
						a1 = ATR_BODY_WORK(d0, a0);		// ȡ��Ŀ��
						ATR_BODY_WORK(d0, a0) = NULL;	// ���
						if (ATR_LIFE(a1) <= 0)			// ���Ŀ����������
							continue;
						if (ATR_VCT_NO(a1) == VCT_NO_APPEAR)	// Ŀ���뿪ս��
							continue;
						else
						{
							ATR_INT_WORK3(a0) = 0;
							ATR_BODY_WORK(0, a0) = a1;								// ������
							ATR_ATTACK_KIND(0, a0) = ATR_ATTACK_KIND(d0, a0);		// ��������
							ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(d0, a0);			// ������
							ATR_ATTACK_PET_POW(0, a0) = ATR_ATTACK_PET_POW(d0, a0);	// pet������
							ATR_LONG_WORK(0, a0) = ATR_LONG_WORK(d0, a0);			// �趨ʹ����Ȯ��Ŀ��
							ATR_VCT_NO(a0) = 1;										// ǰ����һ��Ŀ��
							ATR_INT_WORK2(p_master)++;								// ����һ�������������
							ATR_INT_WORK1(a0)--;
							break;
						}
					}
				}
				if (d0 == ATR_INT_WORK1(a0))
					ATR_INT_WORK1(a0) = 0;
			}
			// ûĿ����,�뿪
			else
			{
				// �趨�뿪
				if (ATR_INT_WORK2(a0) == 0)	// ��һ��ִ��
				{
					ATR_SPD(a0) = 16;
					// �������뿪
					if (ATR_GROUP_FLG(a0) == 0)
						ATR_CHR_ANG(a0) = 3;
					// �������뿪
					else
						ATR_CHR_ANG(a0) = 7;
					ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
					ATR_INT_WORK2(a0) = 1;
					ATR_INT_WORK2(p_master)++;	// ����һ�������������
				}
				gemini(a0);		// �ƶ�
				ATR_CHR_ACT(a0) = ANIM_WALK;
				pattern(a0, 1, ANM_LOOP);
				// �ƶ�����ӫĻ
				if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
				{
					DeathAction(a0);
					p_missile[ATR_PLACE_NO(a0)] = NULL;
					// ����Ƿ����е� p_missile �������
					for (d7 = 0; d7 < 20; d7++)
					{
						if (p_missile[d7] != NULL)
							break;
					}
					if (d7 >= 20)
						ATR_INT_WORK1(p_master) = 1;	// ���еĹ���������ж�����
				}
				// ���еĹ���������ж�����
				if (ATR_INT_WORK1(p_master))
				{
					ATR_DAMAGE(p_master) = 0;
					ATR_BODY_CNT(p_master) = 1;
					if (ATR_BODY_WORK(0, a0) == NULL)	// �������Ŀ�겻�ڳ��ϻ�������
						ATR_DAMAGE(p_master)++;
					else // ������һ������������Ŀ���״̬�Ǵ���������
					if (ATR_VCT_NO(ATR_BODY_WORK(0, a0)) == 0 || ATR_LIFE(ATR_BODY_WORK(0, a0)) <= 0)
						ATR_DAMAGE(p_master)++;
				}
			}
			break;
		}
#endif
		if (--ATR_STIMER(a0))		//??�r???
			break;
		if (ATR_SYNC_FLG(a0))
		{	//�i?????
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_VCT_NO(a0) = 4;		//?�~?
			ATR_SPD(a0) = 32;
			break;
		}
		d0 = get_num();		//???�h��?��?
		if (d0 == 255)
		{	//��???
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
#ifdef _SKILL_SELFEXPLODE //�Ա�
			if (ATR_SELFEXPLODE(a0) == 1)
   				ATR_VCT_NO(a0) = 114;
			else
#endif
			    ATR_VCT_NO(a0) = 4;		//?�~?
			ATR_SPD(a0) = 32;
			break;
		}
		if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)		//����?????????
			a2 = ATR_BODY_WORK(1, a0);
		else
			a2 = ATR_BODY_WORK(0, a0);		//??����V
		a1 = ATR_BODY_WORK(0, a0) = p_party[d0];		//��?��???
		
		ATR_ATTACK_KIND(0, a0) = get_num();		//??��????
		ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
		if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
		{
			ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
			ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
		}
#endif
		if (BattleCmd[command_point] == 'p')
			ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
		else
			LogToBattleError(BattleCmd, __LINE__);
		if (a1 == a2)		//??�i?��??
			ATR_VCT_NO(a0) = 5;		//�e?�t�r??
		else
			ATR_VCT_NO(a0) = 1;		//?�h?
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//??????
		ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
		break;
	case 5:		//�e?�t�r??
		a1 = ATR_BODY_WORK(0, a0);
		if (ATR_VCT_NO(a1) == 0)		//��?�e?�t??????
			ATR_VCT_NO(a0) = 1;		//?�h?
		break;
	case 4:		//����
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = d0;		//??????
#ifndef _PIRATE_ANM			
		gemini(a0);		//?�h
#else
		if (ATR_CHR_NO(a0) != 101490)
			gemini(a0);		//?�h
#endif			
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			if (ATR_DAMAGE(p_master) != ATR_BODY_CNT(p_master))		//???��???
				ATR_DAMAGE(p_master)++;		//???��???
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��?��?
			else
				ATR_CHR_ANG(a0) = 7;		//
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
#ifdef _PIRATE_ANM			
			if (ATR_CHR_NO(a0) == 101492)
				ATR_CHR_NO(a0) = 101490;				
#endif
			ATR_VCT_NO(a0) = 0;		//�r??
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
#ifndef _PIRATE_ANM
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
#else		
		if (ATR_CHR_NO(a0) == 101490)
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
			if (a0->bmpNo == 301026 || a0->bmpNo == 301138)
				ATR_CHR_NO(a0) = 101492;
		}
		else
		{
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	
		}
#endif	
		break;
	case 6:		//����?�\?����
		if (--ATR_STIMER(a0))
			break;
		ATR_SPD(a0) = 10;
		ATR_VCT_NO(a0) = 14;		//???????�~?
		ATR_FIRST_FLG(a0) = 0;		//?�~?
		break;
	case 10:		//������
		if (!ATR_DAMAGE(a0) && ATR_LIFE(a0) > 0 && !ATR_PET_DAMAGE(a0))	//???��?????
		{
			//????????
			if (!ATR_COMBO(a0))
			{
				//??����
#ifdef _SKILL_ROAR  //�輼:���(������)//tuen "miss"  off
				if (ATR_ROAR(a0) == 2){
					//	set_damage_num(a0, 18, -64);
				}
				else
				{
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
					if (NoMiss == 1);//set_damage_num(a0, 1, -64);
					else if (NoMiss == -1)
						set_damage_num(a0, 0, -64);
#else
						set_damage_num(a0, 0, -64);
#endif
				}
#else
				set_damage_num(a0, 0, -64);
#endif
				d0 = get_next_flg();		//?????��?????
				if (d0 & ATT_COUNTER)		//???????
					ATR_VCT_NO(a0) = 11;		//?????
				else
				{
					pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_VCT_NO(a0) = 0;		//�r??
				}
			}
			else
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				ATR_VCT_NO(a0) = 0;		//�r??
			}
			break;
		}
		if (ATR_GUARD_FLG(a0))
		{	//?????
			//????????
			if (!ATR_COMBO(a0))
				//???����
				set_damage_num(a0, 3, -64 + 16);
			ATR_CHR_ACT(a0) = ANIM_GUARD;		//??????????//��
		}
		else
			ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//???????????//����
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		//??????????????????
		if (!ATR_COMBO(a0) || ATR_CHR_ACT_OLD(a0) != ANIM_DAMAGE)
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_DAMAGE_ANG(a0)];
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		ATR_VCT_NO(a0) = 11;
		//?????
		if (ATR_COMBO(a0))
			ATR_STIMER(a0) = 255;
		else
		{
			if (ATR_KAISHIN(a0))		//?��?????
				ATR_STIMER(a0) = HIT_STOP_TIM;
			else
				ATR_STIMER(a0) = HIT_STOP_TIM;
		}
#ifdef _PETSKILL_ACUPUNCTURE
/////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
			| ATT_ATTACKBACK
		#endif
			) && ATR_SYNC_FLG(a0) == 0)
	#else		
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
			| ATT_ATTACKBACK
		#endif
			) && ATR_SYNC_FLG(a0) == 0)
	#endif
/////////////////////////
#else
/////////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) && ATR_SYNC_FLG(a0) == 0)
	#else		
		if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX && ATR_SYNC_FLG(a0) == 0)
	#endif
/////////////////////////
#endif
			ATR_STIMER(a0) = HIT_STOP_TIM;//817333333
		ATR_SPD(a0) = 16;
#ifdef _MAGIC_DEEPPOISION   //�綾
        if ((ATR_DEEPPOISION(a0) == 1))
			ATR_STIMER(a0) = 50;
#endif
		//????????
		if (!ATR_COMBO(a0))
		{
			//????�k����
			set_damage_num(a0, 6, -64);
			ATR_LIFE(a0) -= ATR_DAMAGE(a0);		//?????��
			//andy_mp
			ATR_MP(a0) -= ATR_MPDAMAGE(a0);
			ATR_PET_LIFE(a0) -= ATR_PET_DAMAGE(a0);
			//andy_fall
			if (ATR_PETFALL(a0) == 1)
			{
				if (ATR_LIFE(a0) > 0)
					petfallChangeGraph(a0);
			}
			if (ATR_PET_LIFE(a0) <= 0 && ATR_RIDE(a0))
			{	//?��??????
				ATR_PET_LIFE(a0) = 0;
				if (ATR_LIFE(a0) > 0)
					petfallChangeGraph(a0);
			}
			if (ATR_LIFE(a0) <= 0 || (ATR_ATTACK_KIND(1, a0) & ATT_DEATH))
			{	//?��??????
				ATR_LIFE(a0) = 0;
				slow_flg++;		//????��
				flash_vct_no = 1;		//?????????
			}
		}
		break;
	case 11:		//????�r?
		if (ATR_DAMAGE(a0))
		{	//???????
			gemini(a0);		//??????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
			if (--ATR_STIMER(a0))		//??�r???
				break;
		}
#ifdef _PETSKILL_ACUPUNCTURE
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
			| ATT_ATTACKBACK
		#endif
			) && ATR_SYNC_FLG(a0) == 0 )
	#else		
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
			| ATT_ATTACKBACK
		#endif
			) && ATR_SYNC_FLG(a0) == 0)
	#endif
//////////////////////
#else
//////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP) && ATR_SYNC_FLG(a0) == 0)
	#else		
		if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX && ATR_SYNC_FLG(a0) == 0)
	#endif
//////////////////////
#endif
		{
			sav_command_point = command_point;		//?????��?????�~��
			d0 = get_num();
			command_point = sav_command_point;		//?????��???????
			if (d0 != 255)
			{	//??����??
				a1 = ATR_BODY_WORK(0, a0) = p_party[get_num()];		//��?��???
				ATR_ATTACK_KIND(0, a0) = get_num();		//??��????
				ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
				if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
				{
					ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
					ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
				}
#endif
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
				else
					LogToBattleError(BattleCmd, __LINE__);
				ATR_HIT_STOP(a0) = 0;		//??????????
				ATR_VCT_NO(a0) = 2;		//???
				ATR_FIRST_FLG(a0) = 0;		//????????
				ATR_CHR_ACT_OLD(a0) = -1;
				if (ATR_ATTACK_KIND(0, a0) & ATT_VICARIOUS)		//����??????
					ATR_VCT_NO(a0) = 1;		//�q��?
				break;
			}
		}
		if (ATR_COUNTER_FLG(a0))
		{	//???????//counter
			a1 = ATR_BODY_WORK(0, a0) = p_party[get_num()];		//��?��???
			ATR_ATTACK_KIND(0, a0) = get_num();		//??��????
			ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
			if (BattleCmd[command_point] == 'p')
				ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
			else
				LogToBattleError(BattleCmd, __LINE__);
			ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			ATR_BODY_CNT(a0) = 1;			//
			ATR_BODY_WORK(0, a0) = a1;		//��?��
			ATR_HIT_STOP(a0) = HIT_STOP_TIM;		//??????????
			ATR_VCT_NO(a0) = 2;		//???
			ATR_FIRST_FLG(a0) = 0;		//????????
			//?????����
			ATR_CHR_ACT_OLD(a0) = -1;
			break;
		}
		if (ATR_LIFE(a0) <= 0)		//?��??
			slow_flg--;		//?????
		//��� ?????
		if (ATR_AKO_FLG(a0))
		{
			//????
			ATR_FIRST_FLG(a0) = 0;
			ATR_VCT_NO(a0) = 55;
			//???��?�V?�ϣ�??
			if (ATR_PET_OK(a0))
			{
				a1 = p_party[ATR_PLACE_NO(a0) + 5];		//???��?????
				if (ATR_NAME(a1) != NULL)
				{
					ATR_BODY_WORK(0, a0) = a1;		//?????????
					if (ATR_LIFE(a1) > 0)		//???��?????
					{
						ATR_VCT_NO(a1) = 56;		//????????
						ATR_BODY_WORK(0, a1) = a0;		//??��??????
					}
				}
				else
					ATR_BODY_WORK(0, a0) = NULL;		//pet work clear?????????
			}
			else
				ATR_BODY_WORK(0, a0) = NULL;		//?????????
			//����??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ??????��??????
				if (att_select_flg == true)
				{
					//????
					pc.selectPetNo[ pc.battlePetNo ] = false;
					pc.battlePetNo = -1;
				}
				else
				{
					// �i?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[ pc.battlePetNo ] = false;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[ battlePetNoBak2 ] = false;
				}
			}
			break;
		}
		if (ATR_KAISHIN(a0))		//?��?????
			ATR_SPD(a0) = 35;//���˾���
		else
			ATR_SPD(a0) = 28;
#ifdef _MAGIC_DEEPPOISION   //�綾
        if (ATR_DEEPPOISION(a0) == 1)
		{
            ATR_SPD(a0) = 0;
            ATR_LIFE(a0) = 0; 
		}
#endif
		ATR_VCT_NO(a0) = 12;		//???????
		break;
	case 12:		//??????
		//??��??
		if ((ATR_CHR_NO(a0) >= SPR_mwood1 && ATR_CHR_NO(a0) <= SPR_mstone3) ||
			(ATR_CHR_NO(a0) == 15323) || (ATR_CHR_NO(a0) == 10812) || (ATR_CHR_NO(a0) == 15527) ||
			(ATR_CHR_NO(a0) == 15543) || (ATR_CHR_NO(a0) == 15575) || (ATR_CHR_NO(a0) == 15607)
		)
			ATR_SPD(a0) -= 8;
		else
		{
			//���˶���
			if (ATR_CHR_ACT(a0) == ANIM_DAMAGE && ATR_CHR_NO(a0) >= SPR_pet061 && ATR_CHR_NO(a0) <= SPR_pet064 || ATR_CHR_NO(a0) == SPR_pet065)
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
				gemini(a0);
				ATR_SPD(a0) -= 8;
			}
			else 
			{
				pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
				gemini(a0);		//?�h
				ATR_SPD(a0) -= 8;
			}
		}
		if (ATR_SPD(a0) < 0)		//??????��???
		{
			ATR_SPD(a0) = 10;
			ATR_VCT_NO(a0) = 13;		//??????�r?
			ATR_STIMER(a0) = 16;
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
			a1 = ATR_COUNTER(p_master);
			if (a1->anim_chr_no_bak == 101578)
				ATR_STIMER(a0) = 26;
#endif
		}
		break;
	case 13:		//??????�r?
		if (--ATR_STIMER(a0))		//??�r???
			break;
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
		a1 = ATR_COUNTER(p_master);
		if (a1->anim_chr_no_bak == 101578)
		{
			if ((abs(ATR_H_POS(a0) - ATR_INT_WORK0(a0)) + abs(ATR_V_POS(a0) - ATR_INT_WORK1(a0)) > 80))
			{
				ATR_H_POS(a0) = ATR_INT_WORK0(a0);
				ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			}
		}
#endif
		if (ATR_LIFE(a0) > 0)			//��?????
		{
			if (ATR_COUNTER(p_master) == a0 || ATR_SYNC_FLG(a0) == 1)		//����????�i??????
			{
				d0 = ATR_INT_WORK0(a0);		//��???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
				ATR_SPD(a0) = 32;
				ATR_VCT_NO(a0) = 4;		//?�~?
			}
			else
			{
				ATR_VCT_NO(a0) = 14;		//???
				ATR_FIRST_FLG(a0) = 0;		//??????
			}
		}
		else
		{
#ifdef _PETSKILL_ACUPUNCTURE
////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
					| ATT_ATTACKBACK
		#endif
				))
	#else
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
				| ATT_ATTACKBACK
		#endif
			))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_ACUPUNCTURE
		#ifdef _PET_ITEM
					| ATT_ATTACKBACK
		#endif
				))
	#endif
////////////////////
#else
////////////////////
	#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP))
			{
				if (ATR_ATTACK_KIND(0, a0) & (ATT_REFLEX | ATT_TRAP))
	#else
			if (ATR_COUNTER(p_master) == a0 || ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
			{
				if (ATR_ATTACK_KIND(0, a0) & ATT_REFLEX)
	#endif
////////////////////
#endif
					ATR_DAMAGE(p_master)++;		//??��?
				else
				{
#ifdef _PETSKILL_BATTLE_MODEL
					if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)	//���ǲ��Ǳ������������
#endif
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
				}
				d0 = ATR_INT_WORK0(a0);		//��???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			}
#ifdef _MAGIC_DEEPPOISION   //�綾
            if (ATR_DEEPPOISION(a0) == 1)
                ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
            else
#endif
				ATR_VCT_NO(a0) = VCT_NO_DIE;		//?��?
		}
		break;
	case 14:		//????????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		if (ATR_FIRST_FLG(a0) == 0)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;
		}
		ATR_CRS(a0) = d0;		//??????
		//??��??
		if ((ATR_CHR_NO(a0) >= SPR_mwood1 && ATR_CHR_NO(a0) <= SPR_mstone3) ||
			(ATR_CHR_NO(a0) == 15323) || (ATR_CHR_NO(a0) == 10812) || (ATR_CHR_NO(a0) == 15527) ||
			(ATR_CHR_NO(a0) == 15543) || (ATR_CHR_NO(a0) == 15575) || (ATR_CHR_NO(a0) == 15607)
		)
			d1 = ATR_SPD(a0) = 0;		//????
		else
			gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//�r??
			ATR_SPD(a0) = 64;
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 15:		//??
		ATR_VCT_NO(a0) = 0;		//�r??
		//??�k����
		ATR_DAMAGE(a0) = Rnd(1, 500);
		ATR_PET_DAMAGE(a0) = Rnd(1, 500);
		set_damage_num(a0, 5, -64);
		ATR_LIFE(a0) += ATR_DAMAGE(a0);		//???��
		ATR_PET_LIFE(a0) += ATR_PET_DAMAGE(a0);		//???��
		break;
	case 16:		//????
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		ATR_SPD(a0) = 16;
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;		//��?��?
			ATR_VCT_NO(a0)++;		//?????
			ATR_STIMER(a0) = 0;
		}
		break;
	case 17:		//????
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		d0 = 0;
		if (ATR_FIRST_FLG(a0) == 0)
		{	//????????
			if (ATR_STIMER(a0) == 10)		//????
				d0 = 1;
		}
		else
		{
			a1 = ATR_BODY_WORK(0, a0);		//��?????
			if (ATR_VCT_NO(a1) != 2)		//???????��???
				d0 = 1;
		}
		if (d0)		//???��???
		{
			if (ATR_COUNTER(p_master) == a0)
			{		//����?????
				sav_command_point = command_point;		//?????��?????�~��
				d0 = get_num();
				command_point = sav_command_point;		//?????��???????
				if (d0 == 255)		//��???
				{
					ATR_STIMER(a0) = 1;		//?�L
					ATR_VCT_NO(a0) = 3;		//??�r??
					break;
				}
			}
			ATR_VCT_NO(a0)++;		//?????
			ATR_STIMER(a0) = 0;
		}
		break;
	case 18:		//????
		gemini(a0);
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
#ifdef _PETSKILL_BATTLE_MODEL
			a1 = ATR_BODY_WORK(0, a0);
			if (a1 && ATR_INT_WORK0(a1) == ATT_BATTLE_MODEL)
				ATR_VCT_NO(a0) = 4;
			else
#endif
			if (ATR_COUNTER(p_master) == a0)
			{	//����?????
				ATR_STIMER(a0) = 1;		//?�L
				ATR_VCT_NO(a0) = 3;		//??�r??
			}
			else
			{
				if (ATR_COUNTER_FLG(a0))
				{	//???????
					ATR_VCT_NO(a0) = 2;		//???
					ATR_BODY_WORK(0, a0) = p_party[get_num()];		//��?��???
					ATR_ATTACK_KIND(0, a0) = get_num();		//??��????
					ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
					if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
					{
						ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
						ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
					}
#endif
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
					else
						LogToBattleError(BattleCmd, __LINE__);
					ATR_FIRST_FLG(a0) = 0;		//????????
				}
				else
					ATR_VCT_NO(a0) = 0;		//�r??
			}
		}
		break;
	case 19:		//�i???�r?
		a1 = ATR_BODY_WORK(0, a0);		//??����?��?
		if (!ATR_LONG_WORK(0, a1))		//???��??
		{
			if (ATR_BODY_WORK(1, a1) == a0)		//����????�k??
			{
				ATR_VCT_NO(a0) = 2;		//???
				break;
			}
		}
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 20:		//??????
		if (ATR_BODY_CNT(a0) == 1 && ATR_FIRST_FLG(a0) == 0)		//�O�D????????
		{
			a2 = ATR_BODY_WORK(0, a0);		//��?��??
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 0;		//�r??
			break;
		}
		//????�D��????
		if (ATR_CHR_CNT(a0) == ATR_HIT_TIMING(a0) && ATR_CHR_TIM(a0) == 0)
		{
			if (ATR_BODY_CNT(a0) == 1)		//�O�D??
			{
				//???????
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = 1;		//???��
					ATR_VCT_NO(a0) = 0;		//�r??
					return;
				}
				/* ��??? */
				ATR_NAME(a1) = missile;
				/* ����?�I�T */
				ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
				/* ?????�k? */
				if (ATR_CHR_NO(a0) >= 31027)		//??????
					ATR_CHR_NO(a1) = 32053;
				else
					ATR_CHR_NO(a1) = 32055;
				ATR_ATTACK_KIND(0, a1) = get_num();		//??��????
				ATR_ATTACK_POW(0, a1) = get_num();		//??????
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
				else
					LogToBattleError(BattleCmd, __LINE__);
				/* ???�t */
				ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
				ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);
				ATR_H_POS(a1) = ATR_H_POS(a0);
				ATR_V_POS(a1) = ATR_V_POS(a0);
				ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_SPD(a1) = 32;
				gemini(a1);
				gemini(a1);
				gemini(a1);
				ATR_BODY_CNT(a1) = 0;
				p_missile[0] = a1;
				p_missile[1] = NULL;
			}
			else
			{
				for (d7 = 0; d7 < ATR_BODY_CNT(a0); d7++)
				{
					//???????
					a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
					if (a1 == NULL)
					{
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(a0);		//???��
						ATR_VCT_NO(a0) = 0;		//�r??
						return;
					}
					/* ��??? */
					ATR_NAME(a1) = missile;
					/* ����?�I�T */
					ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
					/* ?????�k? */
					if (ATR_CHR_NO(a0) >= 31027)		//??????
						ATR_CHR_NO(a1) = 32053;
					else
						ATR_CHR_NO(a1) = 32055;
					/* ???�t */
					a2 = ATR_BODY_WORK(0, a1) = p_party[get_num()];		//��?��
					ATR_ATTACK_KIND(0, a1) = get_num();		//??��????
					ATR_ATTACK_POW(0, a1) = get_num();		//??????
					if (BattleCmd[command_point] == 'p')
						ATR_ATTACK_PET_POW(0, a1) = get_num();		//pet??????
					else
						LogToBattleError(BattleCmd, __LINE__);
					ATR_H_POS(a1) = ATR_H_POS(a0);
					ATR_V_POS(a1) = ATR_V_POS(a0);
					d0 = ATR_H_POS(a2);
					d1 = ATR_V_POS(a2);
					radar(a1, &d0, &d1);
					ATR_CHR_ANG(a1) = crs_change_tbl[ d0 ];		/* ????? */
					ATR_SPD(a1) = 32;		//�D��?�t???
					ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
					gemini(a1);
					gemini(a1);
					gemini(a1);
					ATR_CRS(a1) = d0;
					ATR_BODY_CNT(a1) = d7;
					p_missile[d7] = a1;
				}
				p_missile[d7] = NULL;
			}
		}
		break;
	case 25:		//??�d
		if (ATR_GROUP_FLG(a0) == 0)		//????????
			ATR_CHR_ANG(a0) = 3;		//��?��?
		else
			ATR_CHR_ANG(a0) = 7;		//
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 0;		//�r??
			break;
		}
		//???????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//???��?�Ҥ���
			a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			/* ��??? */
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = 26;
			/* ����?�I�T */
			ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
			/* ?????�k? */
			ATR_CHR_NO(a1) = SPR_effect01;
			/* ???�t */
			LoadBmp( ATR_PAT_NO(a0) );
#ifndef __CARYTEST
			ATR_V_POS(a1) = ATR_V_POS(a0) + a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
			ATR_V_POS(a1) = ATR_V_POS(a0) + a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
			ATR_H_POS(a1) = ATR_H_POS(a0);
		}
		break;
	case 26:		//???��?�Ҥ���
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//???��???
			//??????����
			while (1)
			{
				int dx;

				d0 = get_num();		//??��??��?��?
				if (d0 == 255)		//��???
					break;
				a2 = p_party[d0];		//??��??????????
				d0 = get_num();		//??????��?��?
				if (BattleCmd[command_point] == 'p')
					dx = get_num();		//pet??????��?��?
				else
					LogToBattleError(BattleCmd, __LINE__);
				//???????�Ҥ���
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
					break;
				}
				/* ��??? */
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = 27;
				ATR_BODY_WORK(0, a1) = a2;
				ATR_INT_WORK0(a1) = d0;		//?????
				if (BattleCmd[command_point] == 'p')
					ATR_INT_WORKp(a1) = dx;		//pet?????
				else
					LogToBattleError(BattleCmd, __LINE__);
				/* ����?�I�T */
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				/* ?????�k? */
				d0 = get_num();
				d0 = 0;
				ATR_CHR_NO(a1) = SPR_heal + d0;
				/* ???�t */
				LoadBmp( ATR_PAT_NO(a2) );
				ATR_H_POS(a1) = ATR_H_POS(a2);
#ifndef __CARYTEST
				ATR_V_POS(a1) = ATR_V_POS(a2) + a2->anim_y + SpriteInfo[ATR_PAT_NO(a2)].height / 2;
#else
				ATR_V_POS(a1) = ATR_V_POS(a2) + a2->anim_y + g_lpRealAdrn[ATR_PAT_NO(a2)].height / 2;
#endif
			}
			DeathAction(a0);		//��?
			return;
		}
		break;
	case 27:		//??????����
		if (ATR_HIT_STOP(a0))
		{
			ATR_HIT_STOP(a0)--;
			break;
		}
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//???��???
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			DeathAction(a0);		//��?
			return;
		}
		if (ATR_CHR_CNT(a0) == 9 && ATR_CHR_TIM(a0) == 0)
		{
			a2 = ATR_BODY_WORK(0, a0);
			ATR_DAMAGE(a2) = ATR_INT_WORK0(a0);		//?????
			ATR_LIFE(a2) += ATR_DAMAGE(a2);		//
			ATR_HIT_STOP(a0) = 40;
			//??�k����
			if (ATR_LIFE(a2) > ATR_MAX_LIFE(a2))		//?����???
				ATR_LIFE(a2) = ATR_MAX_LIFE(a2);		//?��???
			ATR_PET_DAMAGE(a2) = ATR_INT_WORKp(a0);		//?????
			ATR_PET_LIFE(a2) += ATR_PET_DAMAGE(a2);		//
			//??�k����
			if (ATR_PET_LIFE(a2) > ATR_PET_MAX_LIFE(a2))		//?����???
				ATR_PET_LIFE(a2) = ATR_PET_MAX_LIFE(a2);		//?��???
			set_damage_num(a2, 14, -64);
		}
		break;
#ifdef _FIREHUNTER_SKILL				// (���ɿ�) ROG ADD ��ȸ����_������ɱ
	case FIRE_HUNTER_SKILL:
		if (ATR_FIRST_FLG(a0) == 0)
		{	//�����һ�غ�
			a2 = ATR_BODY_WORK(0, a0);		//����λַ
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//�״�
			ATR_CRS(a0) = d0;		//·�ߴ���
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		//����
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//��������
		}
		ATR_HIT(a0) = 30;		//�������
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//������������
			ATR_CHR_ACT_OLD(a0) = -1;
			//Change fix ԭ��Ϊ 31 �ĳ� FIRE_HUNTER_SKILL+1
			ATR_VCT_NO(a0) = FIRE_HUNTER_SKILL+1;	//�������˴���
			ATR_FIRST_FLG(a0) = 0;		//�趨Ϊ���һ�غ�
			break;
		}
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));		//����
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//�����ѶϢ
				ATR_VCT_NO(a0) = 0;		//����
				return;
			}
			ATR_STIMER(a0) = 0;		//?��???????
			ATR_NAME(a1) = fireHunter;   //�����function
			ATR_CHR_NO(a1) = 101734;	 //ͼ��
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;  //���ȶ�
			/*��ʼλ��*/
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);						//����
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);				//����λַ
			ATR_BODY_WORK(1, a1) = a0;								//�������ĳ�
			for (int i = 2; i < ATR_BODY_CNT(a0) + 2; i++)
			{
				ATR_BODY_WORK(i, a1) = ATR_BODY_WORK(i, a0);			//����λַ
				ATR_ATTACK_POW(i, a1) = ATR_ATTACK_POW(i, a0);			//������
				ATR_ATTACK_PET_POW(i, a1) = ATR_ATTACK_PET_POW(i, a0);	//���﹥����
			}
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);			//������
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);	//���﹥����
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);			//��������
			ATR_SPD(a1) = 60;										//�ٶ�	
			ATR_H_POS(a1) = ATR_H_POS(a0) ;								
			ATR_V_POS(a1) = ATR_V_POS(a0) - 50;
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];			//·��
			ATR_COUNTER_FLG(a1) = ATR_CRS(a1);						//���˹���·���趨
			ATR_BODY_CNT(a1) = ATR_BODY_CNT(a0);									//�������
			bFireInit = true;
			FireSkillEnd = true;
			p_missile[0] = a1;
			p_missile[1] = NULL;
			d6 = 0;		//���㵽��ʱ��
			while (1)
			{
				d0 = ATR_H_POS(ATR_BODY_WORK(0, a1));
				d1 = ATR_V_POS(ATR_BODY_WORK(0, a1));
				radar(a1, &d0, &d1);	//�״�
				//����
				if (d1 <= 10)
					break;
				ATR_CRS(a1) = d0;
				gemini(a1);
				d6++;
			}
			if (!(ATR_GROUP_FLG(a1) = d6))		//����ʱ�䴢��
				ATR_GROUP_FLG(a1) = 1;
			ATR_H_POS(a1) = ATR_H_POS(a0) ;								
			ATR_V_POS(a1) = ATR_V_POS(a0) - 30;
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);						//����
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];			//·��
			ATR_COUNTER_FLG(a1) = ATR_CRS(a1);						//���˹���·���趨
		}
		break;
		//Change fix �з�˯�ߺ��ٻ���,����ȥ 31 (ATR_VCT_NO(a0) = 31;),����������ȥ,��Ϊ ATR_VCT_NO(a0) = FIRE_HUNTER_SKILL+1; �ĳ���һ��
	case FIRE_HUNTER_SKILL + 1:
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (!ATR_STIMER(a0))		//?��??????
			break;
		ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		ATR_VCT_NO(a0) = 0;		//�r??
		break;
#endif
	case 30:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a2 = ATR_BODY_WORK(0, a0);		//��?��??
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???��V
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			if (ATR_LONG_WORK(0, a0) == 0)		//?????
				ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			else
				ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 31;		//???��?�r??
			ATR_FIRST_FLG(a0) = 0;		//????????
			break;
		}
		//???????
		//?�D��????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//??????
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			ATR_STIMER(a0) = 0;		//?��???????
			/* ��??? */
			switch (ATR_LONG_WORK(0, a0))
			{
			case 0:		//?
				ATR_NAME(a1) = bow;
				ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
				break;
			case 1:		//???
				ATR_NAME(a1) = axe_shadow;
				ATR_CHR_NO(a1) = SPR_onokage;
				break;
			case 2:		//��??
				ATR_NAME(a1) = stone_shadow;
				ATR_CHR_NO(a1) = 25786;
				break;
//Terry add 2001/12/28
#ifdef _ITEM_FIRECREAKER
			case 3:		//����
				ATR_NAME(a1) = firecracker_shadow;
				ATR_CHR_NO(a1) = 25786;
				break;
#endif
//Terry end
			default:
				ATR_NAME(a1) = bow;
			}
			/* ����?�I�T */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ???�t */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//��?????
			ATR_BODY_WORK(1, a1) = a0;		//�D��?????
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);		//???
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);		//???
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);		//?????
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			ATR_BODY_CNT(a1) = 0;
			ATR_COUNTER_FLG(a1) = ATR_CRS(a0);		//��?????????????
			p_missile[0] = a1;
			p_missile[1] = NULL;
			//????
			a2 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a2 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			/* ��??? */
			switch (ATR_LONG_WORK(0, a0))
			{
			case 0:		//?
				ATR_NAME(a2) = stick_bow;
				ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
				break;
			case 1:		//???
				ATR_NAME(a2) = axe;
				ATR_CHR_NO(a2) = SPR_ono;
				break;
			case 2:		//��??
				ATR_NAME(a2) = stone;
				ATR_CHR_NO(a2) = 25785;
				break;
//Terry add 2001/12/28
#ifdef _ITEM_FIRECREAKER
			case 3:		//����
				ATR_NAME(a2) = stone;
				ATR_CHR_NO(a2) = 24350;
				break;
#endif
//Terry end
			default:
				ATR_NAME(a2) = stick_bow;
			}
			/* ����?�I�T */
			ATR_DISP_PRIO(a2) = D_PRIO_MISSILE;
			//?????????
			ATR_LONG_WORK(0, a1) = ATR_CHR_NO(a2);
			ATR_BODY_WORK(2, a1) = a2;
			/* ???�t */
			ATR_CHR_ANG(a2) = ATR_CHR_ANG(a1);
			a3 = ATR_BODY_WORK(0, a2) = ATR_BODY_WORK(0, a1);		//��?????
			ATR_BODY_WORK(1, a2) = a1;		//?�s?????
			ATR_H_POS(a2) = ATR_H_POS(a1);
			ATR_V_POS(a2) = ATR_V_POS(a1);
			//ATR_INT_WORK0(a2) = d1;		//��?????��V
			d6 = 0;		//��????��
			ATR_SPD(a2) = 32;
			while (1){
				d0 = ATR_H_POS(a3);
				d1 = ATR_V_POS(a3);
				radar(a2, &d0, &d1);	//????
				//???????
				if (d1 <= 20){
					break;
				}
				ATR_CRS(a2) = d0;
				gemini(a2);
				d6++;
			}
			ATR_INT_WORK0(a2) = d1;
			if (!(ATR_GROUP_FLG(a1) = d6)){		//???????��V
				ATR_GROUP_FLG(a1) = 1;
			}
			d6 = d6 >> 1;
			ATR_SPD(a2) = d6;		//
			ATR_CRS(a2) = 0;		//
			ATR_H_MINI(a2) = 0;		//
			ATR_V_MINI(a2) = 0;		//
			ATR_H_POS(a2) = ATR_H_POS(a1);		//?������
			ATR_V_POS(a2) = ATR_V_POS(a1) - 28;		//
		}
		break;

	case 31:		//?????��?�r?
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (!ATR_STIMER(a0)){		//?��??????
			break;
		}
		d0 = get_num();		//???�h��?��?
		if (d0 == 255){		//��???
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//�r??
		} else {
			ATR_VCT_NO(a0) = 30;		//???�D
			ATR_FIRST_FLG(a0) = 0;		//
			ATR_BODY_WORK(0, a0) = p_party[d0];		//��?��???
			ATR_ATTACK_KIND(0, a0) = get_num();		//??��????
			ATR_ATTACK_POW(0, a0) = get_num();		//??????
#ifdef _PET_ITEM
			if (ATR_ATTACK_KIND(0, a0) & ATT_ATTACKBACK)
			{
				ATR_ATTACK_POW(1, a0) = ATR_ATTACK_POW(0, a0) >> 16;
				ATR_ATTACK_POW(0, a0) = ATR_ATTACK_POW(0, a0) & 0x0000ffff;
			}
#endif
			if ( BattleCmd[command_point] == 'p' )
				ATR_ATTACK_PET_POW(0, a0) = get_num();		//pet??????
			else
				LogToBattleError( BattleCmd, __LINE__ );
		}
		break;

	case 32:		//������
		// a2Ϊ����a1ΪӰ
		if (ATR_FIRST_FLG(a0) == 0){		//???????
			a2 = ATR_BODY_WORK(0, a0);		//��?��??
			d0 = ATR_H_POS(a2);
			d1 = ATR_V_POS(a2);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???��V
			ATR_CHR_ANG(a0) = crs_change_tbl[ d0 ];		/* ????? */
			if (ATR_LONG_WORK(0, a0) == 0){		//?????
				ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			} else {
				ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
			}
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}

#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
		else if ( ShooterNum == 101578 ) {
			a2 = ATR_BODY_WORK(0, a0);		//��?��??
			//������	
			//d0 = /*ATR_H_POS(a2) =*/ nutx[ a2->hitDispNo ] ;
			//d1 = /*ATR_V_POS(a2) =*/ nuty[ a2->hitDispNo ] ;
			//����
			d0 = ATR_H_POS(a2) = ATR_INT_WORK0(a2);
			d1 = ATR_V_POS(a2) = ATR_INT_WORK1(a2);

			radar(a0, &d0, &d1);	//????
//			ATR_CRS(a0) = d0;		//???��V
//			ATR_CHR_ANG(a0) = crs_change_tbl[ d0 ];		/* ????? */
		}
#endif


		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP)){	//?????��???
			ATR_VCT_NO(a0) = 33;		//???��?�r??
			break;
		}
		//???????
		//?�D��????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100){
			//??????
			a1 = GetAction( T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL){
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			/* ��??? */
			switch (ATR_LONG_WORK(0, a0)){
			case 0:		//?
				ATR_NAME(a1) = bow;
				break;
			case 1:		//???
				ATR_NAME(a1) = axe_shadow;
				break;
			case 2:		//��??
				ATR_NAME(a1) = stone_shadow;
				break;
			default:
				ATR_NAME(a1) = bow;
			}
			/* ����?�I�T */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ?????�k? */
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
			if ( ShooterNum == 101578 ) 
				ATR_CHR_NO(a1) = 27001;
			else
				ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
#else
			ATR_CHR_NO(a1) = CG_ARROW_00 + ATR_CRS(a0) / 2 + 20;
#endif
			/* ???�t */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(0, a1) = ATR_BODY_WORK(0, a0);		//��?????
			ATR_BODY_WORK(1,a1) = a0;		//�D��?????
			ATR_ATTACK_POW(0, a1) = ATR_ATTACK_POW(0, a0);		//???
			ATR_ATTACK_PET_POW(0, a1) = ATR_ATTACK_PET_POW(0, a0);		//???
			ATR_ATTACK_KIND(0, a1) = ATR_ATTACK_KIND(0, a0);		//?????
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			ATR_BODY_CNT(a1) = 0;
			ATR_COUNTER_FLG(a1) = ATR_CRS(a0);		//��?????????????
			p_missile[0] = a1;
			p_missile[1] = NULL;

			//????
			a2 = GetAction( T_PRIO_BOW, sizeof(ATR_EQU));
			if ( a2 == NULL ){
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			/* ��??? */
			switch (ATR_LONG_WORK(0, a0)){
			case 0:		//?
				ATR_NAME(a2) = stick_bow;
				break;
			case 1:		//???
				ATR_NAME(a2) = axe;
				break;
			case 2:		//��??
				ATR_NAME(a2) = stone;
				break;
			default:
				ATR_NAME(a2) = stick_bow;
			}
			/* ����?�I�T */
			ATR_DISP_PRIO(a2) = D_PRIO_MISSILE;
			/* ?????�k? */
#ifdef _SHOOTCHESTNUT	// Syu ADD �輼��������
			//���Ĳ���
			if ( ShooterNum == 101578 ) 
				ATR_CHR_NO(a2) = 26995 ; 
			else
				ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
#else
			ATR_CHR_NO(a2) = CG_ARROW_00 + ATR_CRS(a0) / 2;
#endif
			//?????????
			ATR_LONG_WORK(0, a1) = ATR_CHR_NO(a2);
			ATR_BODY_WORK(2, a1) = a2;
			/* ???�t */
			ATR_CHR_ANG(a2) = ATR_CHR_ANG(a1);
			a3 = ATR_BODY_WORK(0,a2) = ATR_BODY_WORK(0, a1);		//��?????
			ATR_BODY_WORK(1,a2) = a1;		//?�s?????
			ATR_H_POS(a2) = ATR_H_POS(a1);
			ATR_V_POS(a2) = ATR_V_POS(a1);
			ATR_INT_WORK0(a2) = d1;		//��?????��V

			d6 = 0;		//��????��
			ATR_SPD(a2) = 32;
			while (1)
			{
				d0 = ATR_H_POS(a3);
				d1 = ATR_V_POS(a3);
				radar(a2, &d0, &d1);	//????
				//???????
				if (d1 <= 20)
					break;
				ATR_CRS(a2) = d0;
				gemini(a2);
				d6++;
			}
			if (!(ATR_GROUP_FLG(a1) = d6))		//???????��V
				ATR_GROUP_FLG(a1) = 1;
			d6 = d6 >> 1;
			ATR_SPD(a2) = d6;		//
			ATR_CRS(a2) = 0;		//
			ATR_H_MINI(a2) = 0;		//
			ATR_V_MINI(a2) = 0;		//
			ATR_H_POS(a2) = ATR_H_POS(a1);		//?������
			ATR_V_POS(a2) = ATR_V_POS(a1) - 28;		//
			d0 = get_num();		//???�h��?��?
			if (d0 != 255)
			{	//��??????
				if (ATR_CHR_CNT(a0) >= 3)		//???�D
					ATR_CHR_CNT(a0) -= 3;
				else
					ATR_CHR_ACT_OLD(a0) = -1;		//???�D
				ATR_DAMAGE(a0)++;		
				ATR_FIRST_FLG(a0) = 0;		
				ATR_BODY_WORK(0, a0) = p_party[d0];		//���������趨
				ATR_ATTACK_KIND(0, a0) = get_num();		//���������趨
				ATR_ATTACK_POW(0, a0) = get_num();		//�������趨
				if (BattleCmd[command_point] == 'p')
					ATR_ATTACK_PET_POW(0, a0) = get_num();		//���﹥�����趨
				else
					LogToBattleError(BattleCmd, __LINE__);
			}
		}
		break;
	case 33:		//�����������ȴ�
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_STIMER(a0) == ATR_DAMAGE(a0))
		{	//?��????
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//����
		}
		break;
	case 35:		//�����ƶ�
		ATR_SPD(a0) = 32;		//�ٶ�
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 = ATR_H_POS(a1);
		d1 = ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0)++;		//?��?��?
			ATR_FIRST_FLG(a0) = 0;		//????????
		}
		else
			gemini(a0);		//?�h
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 36:		//��??��
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_HIT(a0) = 0;				//?????????
			d0 = ATR_H_POS(a1);				//��?��??
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???��V
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];		/* ????? */
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			ATR_VCT_NO(a0)++;		//????�r??
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_STIMER(a0) = 60;
			break;
		}
		//???????
		if (ATR_HIT(a0) != 0 && ATR_HIT(a0) < 10100)
		{
			//?????
			play_damage(ATR_HIT(a0), ATR_H_POS(a0));
			//?�����
			set_damage_num(a0, 5, -64);
			//??????��?
			ATR_VCT_NO(a1) = 45;
			//??��???????
			ATR_BODY_WORK(0, a1) = a0;
			//?�h�L�T
			ATR_SPD(a1) = ATR_SPD(a0) = 8;
			//?????????
			ATR_HIT(a0) = 0;
		}
		break;
	case 37:		//????�r?
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		ATR_VCT_NO(a0)++;		//?????
		break;
	case 38:		//????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��?��?
			else
				ATR_CHR_ANG(a0) = 7;
			ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			a1 = ATR_BODY_WORK(0, a0);		//��????????
			if (ATR_ATTACK_KIND(0, a0) == 0)
			{	//����??
				if (ATR_VCT_NO(a1) == 45)
				{	//?????�D��??
					ATR_VCT_NO(a0) = 40;		//����????????
					ATR_DAMAGE(a0) = 0;		//?????
				}
				else
					ATR_VCT_NO(a0) = 42;		//����?
			}
			else
			{
				ATR_VCT_NO(a0) = 44;		//��??
				//��?����
				set_damage_num(a0, 7, -64);
				ATR_STIMER(a0) = 255;
				ATR_DAMAGE(a0) = t_music_bgm_no;		//???�k?�~��
				play_bgm(0);		//???????????
			}
		}
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		if (ATR_ATTACK_KIND(0, a0) == 0 && ATR_VCT_NO(a1) == 45)
		{	//����??
			if (!Rnd(0, 100))
			{	//�`??????
				ATR_VCT_NO(a0) = 40;		//����????????
				ATR_DAMAGE(a0) = 1;		//?????
			}
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_SPD(a0) == 32)		//??????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 40:		//����???????
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		ATR_VCT_NO(a1)++;		//?????�D��
		ATR_FIRST_FLG(a1) = 0;		//????????
		//��������
		set_damage_num(a0, 8, -64);
		ATR_STIMER(a0) = 100;		//????????
		if (ATR_DAMAGE(a0) == 1)		//????
			ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 4) & 7;		//��?��?
		ATR_VCT_NO(a0)++;		//����?????
		break;
	case 41:		//����????
		if (!--ATR_STIMER(a0))
		{
			if (ATR_DAMAGE(a0) == 1)
			{		//????
				ATR_VCT_NO(a0) = 38;		//????
				ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 4) & 7;		//��?��?
				ATR_SPD(a0) = 32;		//?????
			}
			else
				ATR_VCT_NO(a0)++;		//����?
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		}
		else
			ATR_CHR_ACT(a0) = ANIM_ANGRY;		//�????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 42:		//ʧ��
		ATR_CHR_ACT(a0) = ANIM_STAND;		//ֹͣ����
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		a1 = ATR_BODY_WORK(0, a0);		//ȡ���з�ָ��
		if (ATR_VCT_NO(a1) == 0)
		{	//?????�`?��????
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//�r??
		}
		break;
	case 44:		//�ɹ�
		if (!--ATR_STIMER(a0))
		{	//ʱ�䵽
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//����
			play_bgm(ATR_DAMAGE(a0));		//?????
		}
		ATR_CHR_ACT(a0) = ANIM_HAPPY;		//???????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 45:		//??????��
		a1 = ATR_BODY_WORK(0, a0);		//??��????????
		if (ATR_VCT_NO(a1) == 38)
		{	//??��?????
			d0 = ATR_H_POS(a1);				//��?��??
			d1 = ATR_V_POS(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//???��V
			gemini(a0);
		}
		if (ATR_VCT_NO(a1) == 0)
		{	//??��?????
			ATR_ATTRIB(a0) = ACT_ATR_HIDE;		//����??
			ATR_NAME(a0) = NULL;
			return;
		}
		ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//???????????
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + 1) & 7;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 46:		//??????????
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		ATR_SPD(a0) = 16;
		gemini(a0);		//?�h
		if (ATR_FIRST_FLG(a0) == 0)
		{		//???????
			ATR_CHR_ANG(a0) = crs_change_tbl[d0];
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			a1 = ATR_BODY_WORK(0, a0);		//??��????????
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��?��?
			else
				ATR_CHR_ANG(a0) = 7;		//
			ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//�r??
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 50:		//�P�f�D����?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 16;
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					//��????
					ATR_CRS(a0) = crs_change_tbl2[7];
				else
					ATR_CRS(a0) = crs_change_tbl2[3];
			}
			else 
			{
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					ATR_CHR_ANG(a0) = 7;		//��????
				else
					ATR_CHR_ANG(a0) = 3;		//
				ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			}
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		gemini(a0);		//?�h
		if (ATR_LIFE(a0) > 0)
		{		//��?????
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			pattern(a0, 1, ANM_LOOP);
		}
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//�D��?��
			if (ATR_DAMAGE(p_master) == ATR_BODY_CNT(p_master) && ATR_GUARD_FLG(a0) == 1)
			{	//ACTION_INF�@�e??
				action_inf = 2;
				command_point = 0;		//?????��????????
			}
		}
		break;
	case 52:		//ս������ʧ��(��������)
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				//???��?�V?�ϣ�??
				if (ATR_PET_OK(a0))
				{
					a2 = p_party[ATR_PLACE_NO(a0) + 5];		//???��?????
					if (ATR_NAME(a2) != NULL)
					{
						ATR_VCT_NO(a2) = 50;		//????�`??
						ATR_FIRST_FLG(a2) = 0;		//????????
					}
				}
				ATR_VCT_NO(a0) = 50;		//���ܳɹ�
				break;
			}
			//��ʾ����
			set_damage_num(a0, 9, -64);
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 7;		//��????
			else
				ATR_CHR_ANG(a0) = 3;		//
			ATR_STIMER(a0) = 12;		//???????
			ATR_COUNTER_FLG(a0) = 0;		//
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		pattern(a0, ATR_STIMER(a0), ANM_LOOP);
		//???????�L�T��???
		ATR_COUNTER_FLG(a0) = (ATR_COUNTER_FLG(a0) + 1) & 7;
		if (!ATR_COUNTER_FLG(a0))
		{
			ATR_STIMER(a0)--;
			if (ATR_STIMER(a0) == 1)
			{	//????�L??
				if (ATR_ATTACK_KIND(0, a0) == 0)		//�D������??
					ATR_VCT_NO(a0)++;		//�D������?
				else
				{
					ATR_VCT_NO(a0) = 50;		//�D����??
					ATR_FIRST_FLG(a0) = 0;		//????????
					//???��?�V?�ϣ�??
					if (ATR_PET_OK(a0))
					{
						a2 = p_party[ATR_PLACE_NO(a0) + 5];		//???��?????
						if (ATR_NAME(a2) != NULL)
						{
							ATR_VCT_NO(a2) = 50;		//????�`??
							ATR_FIRST_FLG(a2) = 0;		//????????
						}
					}
				}
			}
		}
		break;
	case 53:		//ս������ʧ��(��������)
		pattern(a0, ATR_STIMER(a0), ANM_LOOP);
		//???????�L�T��???
		ATR_COUNTER_FLG(a0) = (ATR_COUNTER_FLG(a0) + 1) & 7;
		if (!ATR_COUNTER_FLG(a0))
		{
			ATR_STIMER(a0)++;
			if (ATR_STIMER(a0) == 12)
			{	//????????
				ATR_STIMER(a0) = 60;
				ATR_VCT_NO(a0)++;
				//��ʾʧ��
				set_damage_num(a0, 8, -64);
			}
		}
		break;
	case 54:		//�P�f�D������??????
		ATR_CHR_ACT(a0) = ANIM_SAD;		//???????????
		pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (!--ATR_STIMER(a0))
		{	//???��???
			ATR_DAMAGE(p_master)++;		//�D������?��
			//???��?�V?�ϣ�??
			if (ATR_PET_OK(a0))
			{
				a2 = p_party[ATR_PLACE_NO(a0) + 5];		//???��?????
				if (ATR_NAME(a2) != NULL)		//
					ATR_DAMAGE(p_master)++;		//???�D������?��
			}
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��????
			else
				ATR_CHR_ANG(a0) = 7;		//
			ATR_VCT_NO(a0) = 0;		//�r??
		}
		break;
	case 55:		//���
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 63;		//?�h�L�T???
			if (ATR_AKO_FLG(a0) == 2)		//??????
				ATR_LONG_WORK(0, a0) = 0;		//��?��??????
			else
			{
				ATR_LONG_WORK(0, a0) = 3;		//��?��??????
				//��ɵ�����
				play_se(11, ATR_H_POS(a0), ATR_V_POS(a0));
			}
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		gemini(a0);
		if (ATR_LONG_WORK(0, a0) < 3)
		{	//��?��???
			if (ATR_CRS(a0) < 8 || ATR_CRS(a0) > 24)
			{
				if (ATR_V_POS(a0) <= 0)
				{		//�ƣZ?
					d0 = 0;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) < 16 && ATR_CRS(a0) > 0)
			{
				if (ATR_H_POS(a0) >= 640)
				{		//?�Z?
					d0 = 1;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) > 8 && ATR_CRS(a0) < 24)
			{
				if (ATR_V_POS(a0) >= 480)
				{		//?�Z?
					d0 = 2;
					goto monster_case55_100;		//??????
				}
			}
			if (ATR_CRS(a0) > 16)
			{
				if (ATR_H_POS(a0) <= 0)
				{		//?�Z?
					d0 = 3;
					goto monster_case55_100;		//??????
				}
			}
			goto monster_case55_200;		//??????

monster_case55_100:
			d1 = ATR_CRS(a0);
			d1 = ATR_CRS(a0) = crs_bound_tbl[d0][ATR_CRS(a0)];		//��?��?
			ATR_LONG_WORK(0, a0)++;
			//��?��??
			play_se(66, ATR_H_POS(a0), ATR_V_POS(a0));

monster_case55_200:
			;
		}
		else 
		{
			if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT || ATR_V_POS(a0) > 480 + SCREEN_OUT || ATR_V_POS(a0) < 0 - SCREEN_OUT)
			{		//?��???
				ATR_NAME(a0) = NULL;
				ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
				ATR_VCT_NO(a0) = VCT_NO_APPEAR;
				if (BattleMyNo == ATR_PLACE_NO(a0))
				{	//����??
					action_inf = 2;		////ACTION_INF�@�e
					command_point = 0;		//?????��????????
					break;
				}
				//???��?????
				a1 = ATR_BODY_WORK(0, a0);
				if (a1 != NULL)
				{
					//?????
					if (ATR_ATTACK_KIND(2, a1) == 1)
					{
						//????��
						ATR_ATTRIB(a1) |= ACT_ATR_HIDE;		//����??
						ATR_NAME(a1) = NULL;
						ATR_VCT_NO(a1) = VCT_NO_APPEAR;
						if (ATR_COUNTER(p_master) == a0)		//����?????
#ifdef _PETSKILL_BATTLE_MODEL
							if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
								ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
					}
					else
					{
						ATR_VCT_NO(a1) = 57;		//???�`??
						ATR_FIRST_FLG(a1) = 0;
					}
				}
				else
				{
					if (ATR_COUNTER(p_master) == a0)		//����?????
					{
#ifdef _PETSKILL_BATTLE_MODEL
						if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
							ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
					}
				}
				break;
			}
		}
		if (ATR_AKO_FLG(a0) == 2)
		{	//??????
			if (ATR_LONG_WORK(0, a0) & 1)
				ATR_CHR_ANG(a0)--;		//??��
			else
				ATR_CHR_ANG(a0)++;		//??��
			ATR_CHR_ANG(a0) &= 7;		//?��
			pattern(a0, 1, ANM_NO_LOOP);
		}
		break;
	case 56:		//?????????
		a1 = ATR_BODY_WORK(0, a0);		//??��??????
		//?����??
		if (ATR_H_POS(a1) < 640 && ATR_H_POS(a1) > 64 && ATR_V_POS(a1) < 480 && ATR_V_POS(a1) > 0-64)
		{
			d0 = ATR_H_POS(a1);				//??��?��??
			d1 = ATR_V_POS(a1);
			radar2(a0, d0, d1, 2);	//????
			//??��??????
			d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
			if (d0 < 0)
				d0 = 0 - d0;
			if (d0 >= 2)
			{
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
				ATR_CRS_OLD(a0) = ATR_CRS(a0);
			}
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 57:		//?��??????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			ATR_SPD(a0) = 16;
			if (ATR_LIFE(a0) <= 0)
			{		//??????
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					//��????
					ATR_CRS(a0) = crs_change_tbl2[7];
				else
					ATR_CRS(a0) = crs_change_tbl2[3];
			}
			else 
			{
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					ATR_CHR_ANG(a0) = 7;		//��????
				else
					ATR_CHR_ANG(a0) = 3;		//
				ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			}
			//????????????�V??
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		gemini(a0);		//?�h
		//????????????�V??
		if (ATR_LIFE(a0) > 0)		//��?????
		{
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			pattern(a0, 1, ANM_LOOP);
		}
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)		//?��???
		{
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
				//??????????????????????????????????????????????
			if (check_all_dead())
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
			if (ATR_COUNTER(p_master) == ATR_BODY_WORK(0, a0))		//����???��?????
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
			}
			if (ATR_COUNTER(p_master) == a0)		//����?????
			{
#ifdef _PETSKILL_BATTLE_MODEL
				if (ATR_BATTLE_MODEL(a0) != ATT_BATTLE_MODEL)
#endif
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
			}
		}
		break;
	case 60:		//?????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a1 = ATR_BODY_WORK(0, a0);		//??��??????
			set_damage_num(a1, 10, -64);		//??����
			ATR_STIMER(a0) = 40;
			play_se(216, ATR_H_POS(a0), 240);		//??????
			if (ATR_GROUP_FLG(a1) == 0)		//????????
				ATR_CHR_ANG(a1) = 3;		//��?��?
			else
				ATR_CHR_ANG(a1) = 7;		//
			ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_FIRST_FLG(a0) = 0;		//??????
			ATR_VCT_NO(a0)++;		//??
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;

	case 61:		//?????
		a1 = ATR_BODY_WORK(0, a0);		//??��??????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_SPD(a0) = 12;
			if (ATR_LIFE(a0) > 0)
			{	//��?????
				d0 = ATR_INT_WORK0(a1);
				d1 = ATR_INT_WORK1(a1);
				radar(a0, &d0, &d1);	//????
				ATR_CRS(a0) = d0;		//??????
				ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
				ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			}
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		d0 = ATR_INT_WORK0(a1);
		d1 = ATR_INT_WORK1(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			//???��?
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_NAME(a0) = NULL;
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��?
			break;
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 62:		//???��?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			a1 = ATR_BODY_WORK(0, a0);		//??��??????
			set_damage_num(a1, 11, -64);		//??����
			if (ATR_GROUP_FLG(a1) == 0)		//????????
				ATR_CHR_ANG(a1) = 3;		//��?��?
			else
				ATR_CHR_ANG(a1) = 7;		//
			ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_STIMER(a0) = 40;
			//???�k?��???
			ATR_CHR_NO(a0) = get_num();
			//???��???
			ATR_LEVEL(a0) = get_num();
			//??��???
			ATR_LIFE(a0) = get_num();
			//���q��???
			get_name(a0);
			//?��??��???
			ATR_MAX_LIFE(a0) = get_num();
			//??��??????
			a1 = ATR_BODY_WORK(0, a0);
			//?��???
			ATR_H_POS(a0) = ATR_H_POS(a1);
			ATR_V_POS(a0) = ATR_V_POS(a1) - 1;
			//����??
			ATR_ATTRIB(a0) &= ~ACT_ATR_HIDE;
			ATR_SPD(a0) = 12;
			d0 = ATR_INT_WORK0(a0);
			d1 = ATR_INT_WORK1(a0);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_AKO_FLG(a0) = 0;		//?????????
			ATR_ATTACK_KIND(2, a0) = 0;
			ATR_STATUS(a0) = 0;		//??????��???
			play_se(216, ATR_H_POS(a0), 240);		//??????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_FIRST_FLG(a0) = 0;		//??????
			ATR_VCT_NO(a0)++;		//??
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 63:		//???��?
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 2))
		{		//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//�r??
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//??��????
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 64:		//����????��?�r?
		a1 = ATR_BODY_WORK(1, a0);		//??��???
		//??��???��?????
		if (ATR_VCT_NO(a1) != 2)
		{
			ATR_VCT_NO(a0) = 13;		//??????�r?
			ATR_STIMER(a0) = 30;
		}
		break;
	case 65:		//����????
		break;
	case 66:		//����???�h
		a1 = ATR_BODY_WORK(0, a0);		//??��??????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			ATR_SPD(a0) = 28;
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		d0 = ATR_INT_WORK0(a1);
		d1 = ATR_INT_WORK1(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 1))		//????
		{
			//??�祆?��??
			ATR_VCT_NO(a1) = 67;
			ATR_CRS(a1) = ATR_CRS(a0);		//??��?��????
			ATR_SPD(a1) = 40;
			ATR_STIMER(a1) = 8;
			//??�Ԩ�?
#ifdef _PETSKILL_BATTLE_MODEL
			// ���Ǳ������������
			if (ATR_BATTLE_MODEL(a1) == ATT_BATTLE_MODEL)
				a1 = ATR_BODY_WORK(1, a1);	// �� ATR_BODY_WORK(1,a1) ȡ����������� action
			else 
#endif
				a1 = ATR_COUNTER(p_master);
			ATR_BODY_WORK(0, a1) = a0;		//��????��?
			//???????�_��
			ATR_VCT_NO(a0) = 65;
			//????
			play_se(5, ATR_H_POS(a0), 240);
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 67:		//��?��?????
		gemini(a0);		//??????
		ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
		if (--ATR_STIMER(a0))		//??�r???
			break;
		ATR_VCT_NO(a0) = 68;
		//????
#ifdef _PETSKILL_BATTLE_MODEL
		// ���Ǳ������������
		if (ATR_BATTLE_MODEL(a0) == ATT_BATTLE_MODEL)
			a1 = ATR_BODY_WORK(1, a0);	// �� ATR_BODY_WORK(1, a0) ȡ����������� action
		else 
#endif
			a1 = ATR_COUNTER(p_master);		//??��???????
		ATR_VCT_NO(a1) = 2;
		break;
	case 68:		//��?��?????
		gemini(a0);		//?�h
		ATR_SPD(a0) -= 2;
		if (ATR_SPD(a0) < 0)		//??????��???
		{
			ATR_SPD(a0) = 10;
			ATR_VCT_NO(a0) = 64;		//����????��?�r??
		}
		break;
	case 69:		//??????����
		if (ATR_FIRST_FLG(a0) == 0)
		{		//???????
			a1 = ATR_BODY_WORK(0, a0);		//??��??????
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
			//??����
			set_damage_num(a0, 13, -64);
			ATR_STIMER(a0) = 35;
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (--ATR_STIMER(a0))		//??�r???
			break;
		ATR_VCT_NO(a0) = 66;		//����??�h?
		ATR_FIRST_FLG(a0) = 0;		//??????
		break;
	case 70:		//???????
		if (ATR_FIRST_FLG(a0) == 0)		//???????
		{
			int d2;

			d1 = command_point;		//?????????�~��
			d0 = get_num();		//����?��?
			command_point = d1;		//???????????
			d0 /= 5;
			if (ATR_GROUP_FLG(a0) == 0)		//????????
			{
				d1 = boomerang_pos_tbl[d0 * 2];
				d2 = boomerang_pos_tbl[d0 * 2 + 1];
			}
			else
			{
				//?��?????�q???
				if (ATR_PLACE_NO(a0) >= 15)
				{
					d1 = boomerang_pos_tbl3b[d0 * 2];
					d2 = boomerang_pos_tbl3b[d0 * 2 + 1];
				}
				else
				{
					d1 = boomerang_pos_tbl3a[d0 * 2];
					d2 = boomerang_pos_tbl3a[d0 * 2 + 1];
				}
			}
			radar(a0, &d1, &d2);	//????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = d1;
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_THROW;		//?????????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		ATR_HIT(a0) = 0;		//?????????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))	//?????��???
		{
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 71;		//?????????
			break;
		}
		//???????
		//?????�D��????
		if (ATR_HIT(a0) >= 10000 && ATR_HIT(a0) < 10100)
		{
			//????????
			a1 = GetAction(T_PRIO_BOW, sizeof(ATR_EQU));
			if (a1 == NULL)
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
				ATR_VCT_NO(a0) = 0;		//�r??
				return;
			}
			//??????��???
			ATR_BODY_WORK(0, a0) = a1;		//?????????
			/* ��??? */
			ATR_NAME(a1) = boomerang;
			/* ����?�I�T */
			ATR_DISP_PRIO(a1) = D_PRIO_MISSILE;
			/* ?????�k? */
			ATR_CHR_NO(a1) = SPR_boomerang;//SPR_ono;//
			/* ???�t */
			ATR_CHR_ANG(a1) = ATR_CHR_ANG(a0);
			ATR_BODY_WORK(1, a1) = a0;		//�D��?????
			d1 = command_point;		//?????????�~��
			d0 = get_num();		//����?��?
			command_point = d1;		//???????????
			//?�k????
			ATR_LONG_WORK(0, a1) = d0 / 5;
			//?��?�T???
			if (ATR_GROUP_FLG(a0) == 0)
			{
				if (ATR_LONG_WORK(0, a1) <= 1)
					ATR_LONG_WORK(1, a1) = 1;
				else
					ATR_LONG_WORK(1, a1) = 2;
			}
			else
			{
				if (ATR_LONG_WORK(0, a1) <= 1)
					ATR_LONG_WORK(1, a1) = 2;
				else
					ATR_LONG_WORK(1, a1) = 1;
			}
			ATR_GROUP_FLG(a1) = ATR_GROUP_FLG(a0);
			ATR_PLACE_NO(a1) = ATR_PLACE_NO(a0);
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
			ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
			ATR_SPD(a1) = 32;
			gemini(a1);
			gemini(a1);
			gemini(a1);
			p_missile[0] = a1;
			p_missile[1] = NULL;
		}
		break;
	case 71:		//????????
		ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
		a1 = ATR_BODY_WORK(0, a0);		//????????????
		//?����??
		d0 = ATR_H_POS(a1);				//��?��??
		d1 = ATR_V_POS(a1);
		radar2(a0, d0, d1, 2);	//????
		//??��??????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 72:		//??????��?��?
		ATR_CHR_ACT(a0) = ANIM_HAND;		//��?��????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_VCT_NO(a0) = 73;		//??????��?��???
		ATR_STIMER(a0) = 16;
		break;
	case 73:		//??????��?��??
		if (!(--ATR_STIMER(a0)))		//��???
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//�_��?
		}
		break;
	case 75:		//?������
		if (ATR_GROUP_FLG(a0) == 0)		//????????
			ATR_CHR_ANG(a0) = 3;		//��?��?
		else
			ATR_CHR_ANG(a0) = 7;		//
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		//����?????�Ҥ���
		a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a1 == NULL)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//�r??
			return;
		}
		/* ��??? */
		ATR_NAME(a1) = monster;
		ATR_VCT_NO(a1) = 76;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		/* ?????�k? */
		ATR_CHR_NO(a1) = get_num();		//?????????
#ifdef _VARY_WOLF
		if (ATR_CHR_NO(a1)==101120)
			ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0)-1;
#endif
#ifdef _PROFESSION_ADDSKILL
		if ( !g_bUseAlpha ){
			if ( ATR_CHR_NO(a1) == 101769 || ATR_CHR_NO(a1) == 101772 || ATR_CHR_NO(a1) == 101773 )
				ATR_CHR_NO(a1) += 55;
			if ( ATR_CHR_NO(a1) == 101802 || ATR_CHR_NO(a1) == 101803 || ATR_CHR_NO(a1) == 101804 )
				ATR_CHR_NO(a1) += 53;
		}
#endif
		/* ???�t */
		LoadBmp( ATR_PAT_NO(a0) );
		//??��?????
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
		//???????????
		ATR_BODY_WORK(0, a1) = a0;
		ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
		ATR_H_POS(a1) = ATR_H_POS(a0);
		//�r??
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 76:		//?��?�Ҥ���
		//����?�t
		a1 = ATR_BODY_WORK(0, a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
		//?��??��??????
		if (ATR_CHR_NO(a0) == SPR_effect01)
			d0 = pattern(a0, 5, ANM_NO_LOOP);
		else
			d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (d0)
		{	//???��???
			DeathAction(a0);		//��?
			d0 = get_num();		//?????????
			d6 = 0;		//��?????????
#ifdef _VARY_WOLF
			if (d0 >= 101429 && d0 <= 101436)
				a1->anim_chr_no = 0;				
#endif
#ifdef _PROFESSION_ADDSKILL
			if (!g_bUseAlpha)
			{
				if (d0 == 101769 || d0 == 101772 || d0 == 101773)
					d0 += 55;
				if (d0 == 101802 || d0 == 101803 || d0 == 101804)
					d0 += 53;
			}
#endif
			while (1)
			{
				d1 = get_num();		//�������ԧk?��?��?
				if (d1 == 255)
				{	//��???
					if (d6 == 0)		//?��??
						ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
					break;
				}
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
					return;
				}
				//��??����?????�Ҥ���
				a2 = p_party[d1];		//��������??????????
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = 77;
				ATR_CHR_NO(a1) = d0;		//??????
				/* ???�t */
				LoadBmp(ATR_PAT_NO(a2));
				//??��?????
#ifndef __CARYTEST
				ATR_INT_WORK0(a1) = a2->anim_y + SpriteInfo[ATR_PAT_NO(a2)].height / 2;
#else
				ATR_INT_WORK0(a1) = a2->anim_y + g_lpRealAdrn[ATR_PAT_NO(a2)].height / 2;
#endif
				//???????????
				ATR_BODY_WORK(0, a1) = a2;
				ATR_V_POS(a1) = ATR_V_POS(a2) + ATR_INT_WORK0(a1);
				ATR_H_POS(a1) = ATR_H_POS(a2);
				d6++;		//��???????��
			}
			return;
		}
		break;
	case 77:		//?��?�Ҥ���
		//����?�t
		a1 = ATR_BODY_WORK(0, a0);
		ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//???��???
			DeathAction(a0);		//��?
#ifdef _VARY_WOLF
			if (a0->anim_chr_no_bak>=101429 && a0->anim_chr_no_bak<=101436)
				a1->anim_chr_no = 101428;
#endif
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			return;
		}
		break;
	case 78:		//???????
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60)
		{
			ATR_FIRST_FLG(a0) = 0;
			if (ATR_LIFE(a0) <= 0)
			{	//??????
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;		//?��?
				slow_flg--;		//?????
			}
			else
				ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		}
		break;
	case 79:		//?���r?
		ATR_LONG_WORK(0, a0)++;
		if (ATR_LONG_WORK(0, a0) == 60)
		{
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 80:		//��??��
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 7;		//��????
			else
				ATR_CHR_ANG(a0) = 3;		//
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_SPD(a0) = 32;
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		gemini(a0);		//?�h
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_VCT_NO(a0) = 0;		//�_��?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 82:		//??��?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 7;		//��????
			else
				ATR_CHR_ANG(a0) = 3;		//
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_CHR_ACT(a0) = ANIM_SIT;		//?????????
			ATR_CHR_ACT_OLD(a0) = -1;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
			ATR_STIMER(a0) = 60;
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (!--ATR_STIMER(a0))
		{	//��???
			ATR_CHR_NO(a0) = get_num();		//?????�k?��?��?
			ATR_CHR_ACT_OLD(a0) = -1;
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��????
			else
				ATR_CHR_ANG(a0) = 7;		//
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
			ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
			ATR_CHR_ACT_OLD(a0) = -1;
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
			ATR_VCT_NO(a0) = 0;		//�_��?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		}
		break;
	case 85:		//?????�N�ͨ�?
		if (ATR_GROUP_FLG(a0) == 0)		//????????
			ATR_CHR_ANG(a0) = 3;		//��?��?
		else
			ATR_CHR_ANG(a0) = 7;		//
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		//����?????�Ҥ���
		a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
		if (a1 == NULL)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
			ATR_VCT_NO(a0) = 0;		//�r??
			return;
		}
		/* ��??? */
		ATR_NAME(a1) = monster;
		ATR_VCT_NO(a1) = 86;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
		/* ?????�k? */
		ATR_CHR_NO(a1) = SPR_effect01;
		/* ???�t */
		LoadBmp(ATR_PAT_NO(a0));
		//??��?????
#ifndef __CARYTEST
		ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
		ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
		//???????????
		ATR_BODY_WORK(0, a1) = a0;
		ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
		ATR_H_POS(a1) = ATR_H_POS(a0);
		//�r??
		ATR_VCT_NO(a0) = 0;
		ATR_CHR_ACT_OLD(a0) = -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);	//???
		break;
	case 86:		//?????�N�ͨ�??�Ҥ���
		//?��??��??????
		if (ATR_CHR_NO(a0) == SPR_effect01)
			d0 = pattern(a0, 5, ANM_NO_LOOP);
		else
			d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP);
		if (d0)
		{	//???��???
			//�N�ͨ�?
			ATR_ATTRIB(p_master) = get_num();		//�N����?��?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��
			DeathAction(a0);		//��?
			return;
		}
		break;
	case 87:		//???�`??�q
		ATR_SPD(a0) = 32;		//?�h�L�Tspeed
		a1 = p_party[ATR_PLACE_NO(a0) - 5];		//��?��???//�з�
		d0 = ATR_H_POS(a1);//y
		d1 = ATR_V_POS(a1);//x
		radar(a0, &d0, &d1);	//????//d0:course1  d1:distance
		ATR_CRS(a0) = d0;		//??????
		//??��??????
		d0 = ATR_CRS(a0) - ATR_CRS_OLD(a0);
		if (d0 < 0)
			d0 = 0 - d0;
		if (d0 >= 2)
		{
#ifdef _SKILL_ROAR  //�輼:���(������)//�������ܷ���
#else
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CRS_OLD(a0) = ATR_CRS(a0);
#endif
		}
		if (d1 <= 32 * 2)
		{	//????
			ATR_FIRST_FLG(a0) = 0;		//????????
			ATR_VCT_NO(a0) = 88;		//???�`???
		}
		else
			gemini(a0);		//?�h
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 88:		//???�`??
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_STIMER(a0) = 40;
#ifdef _SKILL_ROAR  //�輼:���(������)//�������ܷ���
			//?�h�L�T???
			ATR_SPD(a0) = 60;
			ATR_CHR_ACT(a0) = ANIM_DAMAGE;		//�_��???????
#else
			//?�h�L�T???
			ATR_SPD(a0) = 40;
			ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
#endif
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		//?�h?��??
		if (!ATR_STIMER(a0)--)
		{
			ATR_VCT_NO(a0) = 89;		//???�`????
			ATR_STIMER(a0) = 40;
			//???�`?����
			set_damage_num(a0, 17, -64);
#ifdef _SKILL_ROAR  //�輼:���(������)�������ܷ���
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 7;		//��????
			else
				ATR_CHR_ANG(a0) = 3;		//
#else
			if (ATR_GROUP_FLG(a0) == 0)		//????????
				ATR_CHR_ANG(a0) = 3;		//��????
			else
				ATR_CHR_ANG(a0) = 7;		//
#endif
			//��????
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (crs_change_tbl2[ATR_CHR_ANG(a0)]) & 31;
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 89:		//???�`???
		if (ATR_STIMER(a0))
		{
			ATR_STIMER(a0)--;
			break;
		}
		gemini(a0);
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//�D��?��
			//����??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ??????��??????
				if (att_select_flg == true)
				{
					//????
					pc.selectPetNo[ pc.battlePetNo ] = false;
					pc.battlePetNo = -1;
				}
				else
				{
					// �i?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[ pc.battlePetNo ] = false;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[ battlePetNoBak2 ] = false;
				}
			}
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, 1, ANM_LOOP);
		break;
	case 90:		//????�r
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			//????�r����
			set_damage_num(a0, 18, -64);
			ATR_STIMER(a0) = 0;		//????????
			ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		//��?��?��?
		d0 = pet_nix_tbl[ATR_STIMER(a0)++];
		if (d0 == -2)
		{	//��???
			ATR_VCT_NO(a0) = 0;		//�_��?
			ATR_DAMAGE(p_master)++;		//�D��?��
			break;
		}
		ATR_CHR_ANG(a0) = (ATR_CHR_ANG(a0) + d0) & 7;		//??��?
		pattern(a0, 1, ANM_LOOP);
		break;
	case 95:		//�s?????��
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_LONG_WORK(0, a0) = get_num();		//��?��???
			ATR_LONG_WORK(1, a0) = get_num();		//��???????
			a1 = ATR_BODY_WORK(0, a0) = p_party[ATR_LONG_WORK(0, a0)];		//��?��???
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			ATR_SPD(a0) = 32;		//?�h�L�T
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 =  ATR_H_POS(a1);
		d1 =  ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0) = 96;		//�s????��??
			if (ATR_LONG_WORK(1, a0))
			{	//��???
				a1 = ATR_BODY_WORK(0, a0);		//��????????�_��
				ATR_VCT_NO(a1) = 98;			//
				ATR_SPD(a1) = ATR_SPD(a0);
				ATR_CRS(a1) = ATR_CRS(a0);
			}
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 96:		//�s????��?
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT		//?��???
		|| ATR_V_POS(a0) > 480 + SCREEN_OUT || ATR_V_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��
			//����??????
			if (BattleMyNo + 5 == ATR_PLACE_NO(a0))
			{
				// ??????��??????
				if (att_select_flg == true)
				{
					//????
					pc.selectPetNo[pc.battlePetNo] = false;
					pc.battlePetNo = -1;
				}
				else
				{
					// �i?????
					if (battlePetNoBak2 == pc.battlePetNo)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = false;
						pc.battlePetNo = -1;
					}
					else
						//????
						pc.selectPetNo[battlePetNoBak2] = false;
				}
			}
			if (ATR_LONG_WORK(1, a0))
			{	//��???
				a1 = ATR_BODY_WORK(0, a0);		//��?????
				ATR_NAME(a1) = NULL;
				ATR_ATTRIB(a1) |= ACT_ATR_HIDE;		//����??
				ATR_VCT_NO(a1) = VCT_NO_APPEAR;
				//����??????
				if (BattleMyNo + 5 == ATR_LONG_WORK(0, a0))
				{
					// ??????��??????
					if (att_select_flg == true)
					{
						//????
						pc.selectPetNo[pc.battlePetNo] = false;
						pc.battlePetNo = -1;
					}
					else
					{
						// �i?????
						if (battlePetNoBak2 == pc.battlePetNo)
						{
							//????
							pc.selectPetNo[pc.battlePetNo] = false;
							pc.battlePetNo = -1;
						}
						else
							//????
							pc.selectPetNo[battlePetNoBak2] = false;
					}
				}
			}
		}
		break;
	case 98:		//�s????��????????�_��
		gemini(a0);
		break;
	case 100:		//?????��
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_LONG_WORK(0, a0) = get_num();		//��?��???
			ATR_LONG_WORK(1, a0) = get_num();		//��???????
			a1 = ATR_BODY_WORK(0, a0) = p_party[ATR_LONG_WORK(0, a0)];		//��?��???
			d0 = ATR_INT_WORK0(a1);
			d1 = ATR_INT_WORK1(a1);
			radar(a0, &d0, &d1);	//????
			ATR_CRS(a0) = d0;		//??????
			ATR_CHR_ANG(a0) = crs_change_tbl[ATR_CRS(a0)];		/* ????? */
			ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
			ATR_SPD(a0) = 32;		//?�h�L�T
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		a1 = ATR_BODY_WORK(0, a0);		//��????????
		d0 =  ATR_H_POS(a1);
		d1 =  ATR_V_POS(a1);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);
		if (d1 <= 32 * 2)
		{	//????
			ATR_VCT_NO(a0) = 101;		//?????
			ATR_FIRST_FLG(a0) = 0;		//???????
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 101:		//????
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_CHR_ACT(a0) = ANIM_ATTACK;		//?????????
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//?????��???
			ATR_CHR_ACT_OLD(a0) = -1;
			ATR_VCT_NO(a0) = 102;		//????�r??
			ATR_FIRST_FLG(a0) = 0;		//???????
		}
		break;
	case 102:		//????�r?
		if (ATR_FIRST_FLG(a0) == 0)
		{	//???????
			ATR_STIMER(a0) = 30;		//��?????
			if (ATR_LONG_WORK(1, a0))		//��???
				//��?����
				set_damage_num(a0, 7, -64);
			else
				//��������
				set_damage_num(a0, 8, -64);
			ATR_FIRST_FLG(a0) = 1;		//?????��?
		}
		ATR_STIMER(a0)--;
		if (ATR_STIMER(a0) == 0)
		{	//��???
			if (ATR_LONG_WORK(1, a0))
			{	//��???
				ATR_VCT_NO(a0) = 103;		//�`??��??
				if (ATR_GROUP_FLG(a0) == 0)		//????????
					ATR_CHR_ANG(a0) = 7;		//��????
				else
					ATR_CHR_ANG(a0) = 3;		//
				//��????
				ATR_CRS_OLD(a0) = ATR_CRS(a0) = (crs_change_tbl2[ATR_CHR_ANG(a0)]) & 31;
				a1 = ATR_BODY_WORK(0, a0);		//?????????
				ATR_VCT_NO(a1) = 104;
			}
			else
			{
				d0 = ATR_INT_WORK0(a0);		//��???
				d1 = ATR_INT_WORK1(a0);
				radar(a0, &d0, &d1);	//????
				ATR_CHR_ANG(a0) = crs_change_tbl[d0];
				ATR_SPD(a0) = 32;
				ATR_VCT_NO(a0) = 4;		//?�~?
			}
		}
		break;
	case 103:		//�`??��?
		gemini(a0);
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_NAME(a0) = NULL;
			ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
			ATR_VCT_NO(a0) = VCT_NO_APPEAR;
			ATR_DAMAGE(p_master)++;		//�D��?��
			a1 = ATR_BODY_WORK(0, a0);		//?????�_��?
			ATR_VCT_NO(a1) = 0;
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, 1, ANM_LOOP);
		break;
	case 104:		//�`?????
		ATR_CHR_ACT(a0) = ANIM_ANGRY;		//�????????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case 105:		//??�r?
		if (!--ATR_STIMER(a0))
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��
			ATR_VCT_NO(a0) = 0;		//�_��?
		}
		break;
	case 110:		//????
		if (!--ATR_STIMER(a0))
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��
			ATR_VCT_NO(a0) = 0;		//�_��?
		}
		break;
#ifdef _SKILL_SELFEXPLODE //�Ա�
	case 114:		//�Ա�
		ATR_SPD(a0) = 63;
		ATR_ATTRIB(a0) |= ACT_ATR_HIDE;		//����??
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		gemini(a0);		//?�h
		if (ATR_H_POS(a0) > 640 + SCREEN_OUT || ATR_H_POS(a0) < 0 - SCREEN_OUT)
		{	//?��???
			ATR_VCT_NO(a0) = 0;		//�_��?
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);		//???��???
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
#endif
//249:
	case VCT_NO_APPEAR:		//�K��
		if (ATR_GROUP_FLG(a0) == 0)		//????????
			ATR_CHR_ANG(a0) = 3;		//��????
		else
			ATR_CHR_ANG(a0) = 7;		//
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		ATR_SPD(a0) = 63;
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 2) || ATR_CHR_NO(a0) == 101490)
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = 0;		//�r??
			ATR_CHR_ACT(a0) = ANIM_STAND;		//�_��???????
			ATR_SPD(a0) = 32;
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		}
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case VCT_NO_DIE:		//?��
		ATR_SPD(a0) = 4;
		d0 = ATR_INT_WORK0(a0);
		d1 = ATR_INT_WORK1(a0);
		radar(a0, &d0, &d1);	//????
		ATR_CRS(a0) = d0;		//??????
		gemini(a0);		//?�h
		if (d1 <= (ATR_SPD(a0) >> 2))
		{	//????
			ATR_H_POS(a0) = ATR_INT_WORK0(a0);		//?������
			ATR_V_POS(a0) = ATR_INT_WORK1(a0);
			ATR_VCT_NO(a0) = VCT_NO_DIE + 1;		//?��????
		}
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		if (s_timer & 1)
			pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case VCT_NO_DIE + 1:		//?��
		ATR_CHR_ACT(a0) = ANIM_DEAD;		//?��???????
		if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{	//???��???
#ifdef __NEW_BATTLE_EFFECT
			int effect = ATR_EFFECT_FLAG(a0);
			int effect_anim = ATR_EFFECT_ANIM(a0);

			switch (effect)
			{
			case 1:	//��������
				a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));
				if (a1 == NULL)
				{
					ATR_EFFECT_FLAG(a0) = 0;
					ATR_EFFECT_ANIM(a0) = 0;
					ATR_EFFECT_PARAM(a0) = 0;
					return;
				}
				//��??����?????�Ҥ���
				ATR_DISP_PRIO(a1) = D_PRIO_HIT_MARK;
				ATR_NAME(a1) = monster;
				ATR_VCT_NO(a1) = VCT_NO_EFFECT;
				ATR_CHR_NO(a1) = effect_anim;		//??????
				/* ???�t */
				LoadBmp(ATR_PAT_NO(a0));
				//??��?????
#ifndef __CARYTEST
				ATR_INT_WORK0(a1) = a0->anim_y + SpriteInfo[ATR_PAT_NO(a0)].height / 2;
#else
				ATR_INT_WORK0(a1) = a0->anim_y + g_lpRealAdrn[ATR_PAT_NO(a0)].height / 2;
#endif
				//???????????
				ATR_BODY_WORK(0, a1) = a0;
				ATR_V_POS(a1) = ATR_V_POS(a0) + ATR_INT_WORK0(a1);
				ATR_H_POS(a1) = ATR_H_POS(a0);
				break;
			}
#endif
			ATR_VCT_NO(a0) = VCT_NO_DIE + 2;		//?��?
			//????����
			set_piyo_loop(a0);
			//?????
			play_se(6, ATR_H_POS(a0), 240);
		}
		break;
	case VCT_NO_DIE + 2:		//?��
		break;
#ifdef __NEW_BATTLE_EFFECT
//	case VCT_NO_PAUSE:
//		break;
	case VCT_NO_EFFECT:
		{
			a1 = ATR_BODY_WORK(0, a0);
			int effect = ATR_EFFECT_FLAG(a1);

			switch (effect)
			{
			case 1:			//��������
				ATR_V_POS(a0) = ATR_V_POS(a1) + ATR_INT_WORK0(a0);
				if (pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
				{	//???��???
					//??�k����
					ATR_LIFE(a1) = ATR_EFFECT_PARAM(a1);
					if (ATR_LIFE(a1) > ATR_MAX_LIFE(a1))		//?����???
						ATR_LIFE(a1) = ATR_MAX_LIFE(a1);		//?��???
					ATR_VCT_NO(a1) = 0;		//�_��?
					ATR_CHR_ACT(a1) = ANIM_STAND;		//�_��???????
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					ATR_EFFECT_FLAG(a1) = 0;
					ATR_EFFECT_ANIM(a1) = 0;
					ATR_EFFECT_PARAM(a1) = 0;
					DeathAction(a0);		//��?
					ATR_DAMAGE(p_master)++;
				}
				return;
			default:
				ATR_EFFECT_FLAG(a1) = 0;
				ATR_EFFECT_ANIM(a1) = 0;
				ATR_EFFECT_PARAM(a1) = 0;
				DeathAction(a0);		//��?
				ATR_DAMAGE(p_master)++;
				break;
			}
		}
		break;
#endif
#ifdef __ATTACK_MAGIC
	// ����ǰ�ö���
	case ATTACK_MAGIC_CASE:
		// �Ƿ������·�
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;
		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];
		if (0XFFFFFFFF != AttMgc.iPreMgcNum )
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master)	= ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0)			= 0;
				return;
			}
			ATR_NAME(a1)			= monster;
			ATR_VCT_NO(a1)			= ATTACK_MAGIC_CASE + 1;		// ǰ�ö���
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (AttMgc.iPreMgcNum == 101120)
				ATR_DISP_PRIO(a1)	= (1 == AttMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
			else 
				ATR_DISP_PRIO(a1)	= ATR_DISP_PRIO(a0) + 1;
#else
			ATR_DISP_PRIO(a1)		= (1 == AttMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
#endif
			ATR_CHR_NO(a1)			= AttMgc.iPreMgcNum;
			ATR_BODY_WORK(0, a1)	= a0;
			ATR_H_POS(a1)			= ATR_H_POS(a0) + AttMgc.wPreMgcX;
			ATR_V_POS(a1)			= ATR_V_POS(a0) + AttMgc.wPreMgcY;
		}
		ATR_VCT_NO(a0)		= 0;
		ATR_CHR_ACT_OLD(a0)	= -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	// ִ��ǰ�ö���
	case ATTACK_MAGIC_CASE + 1:
		a1				= ATR_BODY_WORK(0, a0);
		ATR_H_POS(a0)	= ATR_H_POS(a1) + AttMgc.wPreMgcX;
		ATR_V_POS(a0)	= ATR_V_POS(a1) + AttMgc.wPreMgcY;
		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			AttMgc.wRunPreMgc = true;
			//ToCallMgc.wRunPreMgc = true;
		}
		break;
	// ִ��Ŀǰ������
	case ATTACK_MAGIC_CASE + 2:
	{
		idx = ATR_LONG_WORK(0, a0);

		// ���ŵ�������������λ��
		if (20 != idx)
		{
			a1				= p_party[idx];
			ATR_H_POS(a0)	= ATR_H_POS(a1) + AttMgc.posAttacked[0].x;
			ATR_V_POS(a0)	= ATR_V_POS(a1) + AttMgc.posAttacked[0].y;
		}
		// ���ž������������
		else
		{
			int value = 0;

			// Ŀǰ���ڵ�����
			if (2 == g_iRunEarthQuake)
				value = piyo_tbl[(piyo_point + g_iCurRunEarthQuake ) & 63] + 12;

			ATR_V_POS(a0) = AttMgc.posAttacked[0].y + value;
		}

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			// ���û�к��ö���
			if (0XFFFFFFFF == AttMgc.iPostMgcNum)
			{
				AttMgc.wNumAttackeds++;
				if (20 == idx)
				{
					// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
					// wAttackedIndex[n] ����0XFF -- �������
					int i = 0, j, charahurt, pethurt;

					while (++i && 0XFF != AttMgc.wAttackedIndex[i])
					{
						for (j = 0; j < iAttackedNum; j++)
						{
							if (AttMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
							{
								a1			= p_party[AttackedInfo[j * 4 + 1]];
								charahurt	= AttackedInfo[j * 4 + 2];
								pethurt		= AttackedInfo[j * 4 + 3];
								ATR_LONG_WORK(0, a1)	= 0;
								ATR_DAMAGE(a1)			= charahurt;
								ATR_PET_DAMAGE(a1)		= pethurt;
								if (charahurt == 0)
								{
									set_damage_num(a1, 0, -64);
									ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 5;
								}
								else
								{
									set_damage_num(a1, 6, -64);
									ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
								}
								ATR_LIFE(a1) -= ATR_DAMAGE(a1);
								set_jujutsu_hit_mark(a1);
								if (ATR_LIFE(a1) <= 0)
								{
									ATR_LIFE(a1) = 0;
									slow_flg++;
								}
								ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
								//andy_fall
								if (ATR_PETFALL(a1) == 1)
								{
									if (ATR_LIFE(a1) > 0)
										petfallChangeGraph(a1);
								}
								if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
								{
									ATR_PET_LIFE(a1) = 0;
									if (ATR_LIFE(a1) > 0)
										petfallChangeGraph(a1);
								}
								if (charahurt == 0)
									ATR_CHR_ACT(a1) = ANIM_STAND;
								else
									ATR_CHR_ACT(a1)	= ANIM_DAMAGE;
								ATR_CHR_ACT_OLD(a1)	= -1;
								pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
							}
						}
					}
				}
				else
				{
					int j, charahurt, pethurt;

					for (j = 0; j < iAttackedNum; j++)
					{
						if (idx == AttackedInfo[j * 4])
						{
							a1			= p_party[AttackedInfo[j * 4 + 1]];
							charahurt	= AttackedInfo[j * 4 + 2];
							pethurt		= AttackedInfo[j * 4 + 3];
							ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1) = charahurt;
							ATR_PET_DAMAGE(a1)	= pethurt;
							if (charahurt == 0) 
							{
								set_damage_num(a1, 0, -64);
								ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 5;
							}
							else							 
							{
								set_damage_num(a1, 6, -64);
								ATR_VCT_NO(a1) = ATTACK_MAGIC_CASE + 4;
							}
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if (ATR_LIFE(a1) <= 0)
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}
							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if (ATR_PETFALL(a1) == 1)
							{
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
							{
								ATR_PET_LIFE(a1) = 0;
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (charahurt == 0)
								ATR_CHR_ACT(a1) = ANIM_STAND;
							else
								ATR_CHR_ACT(a1) = ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1)	= -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			// ӵ�к��ö���
			else
			{
				// ������еĵ���ʩ���ö���
				if (20 == idx) // idx = wAttackedIndex[0]
				{
					// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
					// wAttackedIndex[n] ����0XFF -- �������
					int i = 0;

					while (++i && 0XFF != AttMgc.wAttackedIndex[i])
					{
						a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

						ATR_NAME(a1)		= monster;
						ATR_VCT_NO(a1)		= ATTACK_MAGIC_CASE + 3;		// ���ö���
						ATR_DISP_PRIO(a1)	= (1 == AttMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
						ATR_CHR_NO(a1)		= AttMgc.iPostMgcNum;
						ATR_LONG_WORK(0, a1) = AttMgc.wAttackedIndex[i];
						ATR_H_POS(a1)		= ATR_H_POS( p_party[AttMgc.wAttackedIndex[i]] ) + AttMgc.wPostMgcX;
						ATR_V_POS(a1)		= ATR_V_POS( p_party[AttMgc.wAttackedIndex[i]] ) + AttMgc.wPostMgcY;
					}
				}
				// ���ĳһ������ʩ���ö���
        else
				{
					a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU));

					ATR_NAME(a1)		= monster;
					ATR_VCT_NO(a1)		= ATTACK_MAGIC_CASE + 3;		// ���ö���
					ATR_DISP_PRIO(a1)	= (1 == AttMgc.wPostMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a1)		= AttMgc.iPostMgcNum;
					ATR_LONG_WORK(0, a1) = idx;
					ATR_H_POS(a1)		= ATR_H_POS(p_party[idx]) + AttMgc.wPostMgcX;
					ATR_V_POS(a1)		= ATR_V_POS(p_party[idx]) + AttMgc.wPostMgcY;
				}
			}
		}

		// �����������еĶ�����
		if (AttMgc.wNumAttackeds == AttMgc.wNumAttacks && 0 == iAttackedNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc			= false;
			g_iRunEarthQuake	= 0;
		}
		break;
	}
	// ִ�к��ö���
	case ATTACK_MAGIC_CASE + 3:
		idx				= ATR_LONG_WORK(0, a0);
		a1				= p_party[idx];
		ATR_H_POS(a0)	= ATR_H_POS(a1) + AttMgc.wPostMgcX;
		ATR_V_POS(a0)	= ATR_V_POS(a1) + AttMgc.wPostMgcY;

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			AttMgc.wNumAttackeds++;
			if (20 == idx)
			{
				// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
				// wAttackedIndex[n] ����0XFF -- �������
				int i = 0, j, charahurt, pethurt;

				while (++i && 0XFF != AttMgc.wAttackedIndex[i])
				{
					for (j = 0; j < iAttackedNum; j++)
					{
						if (AttMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
						{
							a1			= p_party[AttackedInfo[j * 4 + 1]];
							charahurt	= AttackedInfo[j * 4 + 2];
							pethurt		= AttackedInfo[j * 4 + 3];

							ATR_VCT_NO(a1)		= ATTACK_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1)		= charahurt;
							ATR_PET_DAMAGE(a1)	= pethurt;
							set_damage_num(a1, 6, -64);
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if (ATR_LIFE(a1) <= 0)
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}

							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if (ATR_PETFALL(a1) == 1)
							{
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
							{
								ATR_PET_LIFE(a1) = 0;

								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							ATR_CHR_ACT(a1)		= ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1)	= -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			else
			{
				int j, charahurt, pethurt;

				for (j = 0; j < iAttackedNum; j++)
				{
					if (idx == AttackedInfo[j * 4])
					{
						a1			= p_party[AttackedInfo[j * 4 + 1]];
						charahurt	= AttackedInfo[j * 4 + 2];
						pethurt		= AttackedInfo[j * 4 + 3];

						ATR_VCT_NO(a1)		= ATTACK_MAGIC_CASE + 4;
						ATR_LONG_WORK(0, a1) = 0;
						ATR_DAMAGE(a1)		= charahurt;

						ATR_PET_DAMAGE(a1)	= pethurt;
						set_damage_num(a1, 6, -64);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						set_jujutsu_hit_mark(a1);
						if (ATR_LIFE(a1) <= 0)
						{
							ATR_LIFE(a1) = 0;
							slow_flg++;
						}
						ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
						//andy_fall
						if (ATR_PETFALL(a1) == 1)
						{
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
						{
							ATR_PET_LIFE(a1) = 0;
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						ATR_CHR_ACT(a1)		= ANIM_DAMAGE;
						ATR_CHR_ACT_OLD(a1)	= -1;
						pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					}
				}
			}
		}
		// �����������еĶ�����
		if (AttMgc.wNumAttackeds == AttMgc.wNumAttacks && 0 == iAttackedNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc			= false;
			g_iRunEarthQuake	= 0;
		}
		break;
	// ���������˵�ͼ
	case ATTACK_MAGIC_CASE + 4:
		if (ATR_FIRST_FLG(a0) > 40)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
		gemini(a0);
		if (40 == ATR_FIRST_FLG(a0))
		{
			if (ATR_LIFE(a0) <= 0)
			{
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
				slow_flg--;
			}
			else
				ATR_VCT_NO(a0) = 0;

			if (ATR_GROUP_FLG(a0) == 1)
				ATR_CRS(a0) = crs_change_tbl2[7];
			else 
				ATR_CRS(a0) = crs_change_tbl2[3];
			gemini(a0);
			++iCurAttackedFinishNum;
		}
		// �����������еĶ�����
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc			= false;
			g_iRunEarthQuake	= 0;
		}
		break;
	case ATTACK_MAGIC_CASE + 5:
		if (ATR_FIRST_FLG(a0) > 60)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60) 
		{
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunAttMgc			= false;
			g_iRunEarthQuake	= 0;
		}
		break;
#endif
#ifdef __TOCALL_MAGIC
	// ����ǰ�ö���
	case TOCALL_MAGIC_CASE:
		// �Ƿ������·�
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;

		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];

		if (0XFFFFFFFF != ToCallMgc.iPreMgcNum)
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0) = 0;
				return;
			}
			ATR_NAME(a1)		= monster;
			ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 1;		// ǰ�ö���
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
			if (ToCallMgc.iPreMgcNum == 101120)
				ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
			else 
				ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0) + 1;
#else
			ATR_DISP_PRIO(a1) = (1 == ToCallMgc.wPreMgcOnChar) ? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
#endif
			ATR_CHR_NO(a1) = ToCallMgc.iPreMgcNum;
			ATR_BODY_WORK(0, a1) = a0;
			ATR_H_POS(a1) = ATR_H_POS(a0) + ToCallMgc.wPreMgcX;
			ATR_V_POS(a1) = ATR_V_POS(a0) + ToCallMgc.wPreMgcY;
		}
		ATR_VCT_NO(a0)		= 0;
		ATR_CHR_ACT_OLD(a0)	= -1;
#ifdef _PETSKILL_LER			// �׶�����
		if (ATR_CHR_NO(a0) == 101815)
			ATR_CHR_NO(a0) = ATR_CHR_NO(a0) * -1;
#endif
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	// ִ��ǰ�ö���
	case TOCALL_MAGIC_CASE + 1:

		a1				= ATR_BODY_WORK( 0 , a0 );
		ATR_H_POS(a0)	= ATR_H_POS(a1) + ToCallMgc.wPreMgcX;
		ATR_V_POS(a0)	= ATR_V_POS(a1) + ToCallMgc.wPreMgcY;

		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			ToCallMgc.wRunPreMgc = true;
#ifdef _PETSKILL_LER			// �׶�����
			if (ATR_CHR_NO_OLD(a1) < 0)
			{
				int side;

				ATR_CHR_NO(a1) = ATR_CHR_NO(a1) * -1;
				if (ToCallMgc.iCurMgcNum == 101798 || ToCallMgc.iCurMgcNum == 101800 || ToCallMgc.iCurMgcNum == 101853 || ToCallMgc.iCurMgcNum == 101854)
				{
					int i;
					for (i = 0; i < 20; i++)
					{
						// ���ҵ��׶�λ��
						if (ATR_CHR_NO(p_party[i]) == ATR_CHR_NO(a1))
							break;
					}
					side = i = (i < 10 ? 0:10);
					// �׶������ȫ����ʧ
					for (; i < side + 10; i++)
						ATR_H_POS(p_party[i]) += -640;
				}
			}
#endif
		}
		break;
	// ִ��Ŀǰ������
	case TOCALL_MAGIC_CASE + 2:
	{
		idx = ATR_LONG_WORK(0, a0);
#ifdef _PROFESSION_ADDSKILL
		RunTimeMagicBoundary(ATR_CHR_NO(a0));
#endif
		// ���ŵ�������������λ��
		if (20 != idx)
		{
			a1 = p_party[idx];
			if (idx < 10)
			{
				ATR_H_POS(a0)	= ATR_H_POS(a1) - ToCallMgc.posAttacked[0].x;
				ATR_V_POS(a0)	= ATR_V_POS(a1) - ToCallMgc.posAttacked[0].y;
			}
			else
			{
				ATR_H_POS(a0)	= ATR_H_POS(a1) + ToCallMgc.posAttacked[0].x;
				ATR_V_POS(a0)	= ATR_V_POS(a1) + ToCallMgc.posAttacked[0].y;
			}
		}
		// ���ž������������
		else
		{
			int value = 0;

			// Ŀǰ���ڵ�����
			if (2 == g_iRunEarthQuake)
				value = piyo_tbl[(piyo_point + g_iCurRunEarthQuake) & 63] + 12;
			ATR_V_POS(a0) = ToCallMgc.posAttacked[0].y + value;
		}
		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
#ifdef _PETSKILL_LER			// �׶�����
			int side;

			if (ATR_CHR_NO(a0) == 101798 || ATR_CHR_NO(a0) == 101800 || ATR_CHR_NO(a0) == 101853 || ATR_CHR_NO(a0) == 101854)
			{
				int i;
				for (i = 0; i < 20; i++)
				{
					// ���ҵ��׶�λ��
					if (ATR_CHR_NO(p_party[i]) == 101815)
						break;
				}
				side = i = (i < 10 ? 0:10);
				// �׶�����˳���
				for (; i < side + 10; i++)
					ATR_H_POS(p_party[i]) += 640;
			}
#endif
// Terry add fix �������׶���
			if (ToCallMgc.wCurAttackNum == 101628)
				DeathAction(a0);
			else
// end
			if (ToCallMgc.wIsPostDisappear)
			{
				a0tmp[a0tmpcount] = a0;
				a0mark[a0tmpcount] = 1;
				a0tmpcount++;
			}
			else
			{
				DeathAction(a0);
#ifdef _PROFESSION_ADDSKILL
				if (boundary_2)
				{
			        DeathAction(boundary_2);
				    boundary_2 = NULL;
				}
#endif
			}
			// ���û�к��ö���
			if (0XFFFFFFFF == ToCallMgc.iPostMgcNum)
			{
				ToCallMgc.wNumAttackeds++;
				if (20 == idx)
				{
					// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
					// wAttackedIndex[n] ����0XFF -- �������
					int i = 0, j, charahurt, pethurt;

					while (++i && 0XFF != ToCallMgc.wAttackedIndex[i])
					{
						for (j = 0; j < iAttackedNum; j++)
						{
							if (ToCallMgc.wAttackedIndex[i] == AttackedInfo[j * 4])
							{
								a1			= p_party[AttackedInfo[j * 4 + 1]];
								charahurt	= AttackedInfo[j * 4 + 2];
								pethurt		= AttackedInfo[j * 4 + 3];
								ATR_LONG_WORK(0, a1) = 0;
								ATR_DAMAGE(a1)		= charahurt;
								ATR_PET_DAMAGE(a1)	= pethurt;
								if (charahurt == 0)//�˺�ֵΪ0 
								{
#ifdef _BATTLESKILL				// (�ɿ�) Syu ADD ս�����ܽ���
									if (a0->anim_chr_no == 101651 || a0->anim_chr_no == 101650) //��������һ��Miss����
										ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
									else
									{
										set_damage_num(a1, 0, -64);
										ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
										if ( ToCallMgc.iCurMgcNum == 101676 //�ᴩ�������˺���Ϊ0,�������Ҫ�����ܵĶ���
											 || ToCallMgc.iCurMgcNum == 101675
											 || ToCallMgc.iCurMgcNum == 101674
											 || ToCallMgc.iCurMgcNum == 101673
											 || ToCallMgc.iCurMgcNum == 101672
											 || ToCallMgc.iCurMgcNum == 101665
											 || ToCallMgc.iCurMgcNum == 101664
											 || ToCallMgc.iCurMgcNum == 101663
											 || ToCallMgc.iCurMgcNum == 101662
											 || ToCallMgc.iCurMgcNum == 101661
											 || ToCallMgc.iCurMgcNum == 101656)
										{	//��������
											ATR_STIMER(a1) = 0;
											ATR_FIRST_FLG(a1) = 0;		//?????????
											ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 6;
										}
									}
#else
									set_damage_num(a1, 0, -64);
									ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
#endif
								}
								else
								{
									set_damage_num(a1, 6, -64);
									ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
								}
								ATR_LIFE(a1) -= ATR_DAMAGE(a1);
								set_jujutsu_hit_mark(a1);
								if (ATR_LIFE(a1) <= 0)
								{
									ATR_LIFE(a1) = 0;
									slow_flg++;
								}
								ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
								//andy_fall
								if (ATR_PETFALL(a1) == 1)
								{
									if (ATR_LIFE(a1) > 0)
										petfallChangeGraph(a1);
								}
								if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
								{
									ATR_PET_LIFE(a1) = 0;
									if (ATR_LIFE(a1) > 0)
										petfallChangeGraph(a1);
								}
								if (charahurt == 0)
									ATR_CHR_ACT(a1)	= ANIM_STAND;
								else
									ATR_CHR_ACT(a1)	= ANIM_DAMAGE;
								ATR_CHR_ACT_OLD(a1)	= -1;
								pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
							}
						}
					}
				}
				else
				{
					int j, charahurt, pethurt;

					for (j = 0; j < iAttackedNum; j++)
					{
						if (idx == AttackedInfo[j * 4])
						{
							a1			= p_party[AttackedInfo[j * 4 + 1]];
							charahurt	= AttackedInfo[j * 4 + 2];
							pethurt		= AttackedInfo[j * 4 + 3];
							ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1)		= charahurt;
							ATR_PET_DAMAGE(a1)	= pethurt;
							if (charahurt == 0)
							{
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
//								if ( a0->anim_chr_no == 101651 || a0->anim_chr_no == 101650 ) //��������һ��Miss����
//									ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 5;
//								else {
//									set_damage_num( a1 , 0 , -64 );
//									ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 5;
//								}
//#else
								set_damage_num(a1, 0, -64);
								ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 5;
//#endif
							}
							else				 
							{
								set_damage_num(a1, 6, -64);
								ATR_VCT_NO(a1) = TOCALL_MAGIC_CASE + 4;
							}
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if (ATR_LIFE(a1) <= 0)
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}
							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if (ATR_PETFALL(a1) == 1)
							{
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
							{
								ATR_PET_LIFE(a1) = 0;
								if (ATR_LIFE(a1) > 0)
									petfallChangeGraph(a1);
							}
							if (charahurt == 0)
								ATR_CHR_ACT(a1) = ANIM_STAND;
							else
								ATR_CHR_ACT(a1)	= ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1)	= -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			// ӵ�к��ö���
			else
			{
				// ������еĵ���ʩ���ö���
				if ( 20 == idx ) // idx = wAttackedIndex[0]
				{
					// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
					// wAttackedIndex[n] ����0XFF -- �������
					int i = 0;

					while ( ++i && 0XFF != ToCallMgc.wAttackedIndex[i] )
					{
						a1 = GetAction( T_PRIO_MISSILE , sizeof(ATR_EQU));

						ATR_NAME(a1)			= monster;
						ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 3;		// ���ö���
						ATR_DISP_PRIO(a1)		= ( 1 == ToCallMgc.wPostMgcOnChar )? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
						ATR_CHR_NO(a1)		= ToCallMgc.iPostMgcNum;
						ATR_LONG_WORK(0, a1) = ToCallMgc.wAttackedIndex[i];
						ATR_H_POS(a1)			= ATR_H_POS( p_party[ToCallMgc.wAttackedIndex[i]] ) + ToCallMgc.wPostMgcX;
						ATR_V_POS(a1)			= ATR_V_POS( p_party[ToCallMgc.wAttackedIndex[i]] ) + ToCallMgc.wPostMgcY;
					}
				}
				// ���ĳһ������ʩ���ö���
                else
				{
					a1 = GetAction( T_PRIO_MISSILE , sizeof(ATR_EQU));

					ATR_NAME(a1)			= monster;
					ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 3;		// ���ö���
					ATR_DISP_PRIO(a1)		= ( 1 == ToCallMgc.wPostMgcOnChar )? D_PRIO_HIT_MARK : DISP_PRIO_TILE + 1;
					ATR_CHR_NO(a1)		= ToCallMgc.iPostMgcNum;
					ATR_LONG_WORK(0, a1) = idx;
					ATR_H_POS(a1)			= ATR_H_POS( p_party[idx] ) + ToCallMgc.wPostMgcX;
					ATR_V_POS(a1)			= ATR_V_POS( p_party[idx] ) + ToCallMgc.wPostMgcY;
				}
			}
		}

		// �����������еĶ�����
		if ( ToCallMgc.wNumAttackeds == ToCallMgc.wNumAttacks && 0 == iAttackedNum )
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc			= false;
			g_iRunEarthQuake	= 0;
		}

		break;
	}
	// ִ�к��ö���
	case TOCALL_MAGIC_CASE + 3:

		idx				= ATR_LONG_WORK(0, a0);
		a1				= p_party[idx];
		ATR_H_POS(a0)	= ATR_H_POS(a1) + ToCallMgc.wPostMgcX;
		ATR_V_POS(a0)	= ATR_V_POS(a1) + ToCallMgc.wPostMgcY;

		if ( d0 = pattern( a0 , ANM_NOMAL_SPD , ANM_NO_LOOP ) )
		{
			DeathAction(a0);
			ToCallMgc.wNumAttackeds++;

			if ( 20 == idx )
			{
				// wAttackedIndex[0] ����  20 -- ����ȫ�幥��
				// wAttackedIndex[n] ����0XFF -- �������
				int i = 0 , j , charahurt , pethurt;

				while ( ++i && 0XFF != ToCallMgc.wAttackedIndex[i] )
				{
					for ( j = 0 ; j < iAttackedNum ; j++ )
					{
						if ( ToCallMgc.wAttackedIndex[i] == AttackedInfo[j * 4] )
						{
							a1			= p_party[AttackedInfo[j * 4 + 1]];
							charahurt	= AttackedInfo[j * 4 + 2];
							pethurt		= AttackedInfo[j * 4 + 3];

							ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 4;
							ATR_LONG_WORK(0, a1) = 0;
							ATR_DAMAGE(a1)		= charahurt;


							ATR_PET_DAMAGE(a1)	= pethurt;

							set_damage_num( a1 , 6 , -64 );
							ATR_LIFE(a1) -= ATR_DAMAGE(a1);
							set_jujutsu_hit_mark(a1);
							if ( ATR_LIFE(a1) <= 0 )
							{
								ATR_LIFE(a1) = 0;
								slow_flg++;
							}


							ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
							//andy_fall
							if ( ATR_PETFALL(a1) == 1 )	{
								if ( ATR_LIFE(a1) > 0 )	{
									petfallChangeGraph(a1);
								}
							}
							if ( ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1) ){
								ATR_PET_LIFE(a1) = 0;

								if ( ATR_LIFE(a1) > 0 )
									petfallChangeGraph(a1);
							}
							
							ATR_CHR_ACT(a1)		= ANIM_DAMAGE;
							ATR_CHR_ACT_OLD(a1)	= -1;
							pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
						}
					}
				}
			}
			else
			{
				int j, charahurt, pethurt;

				for (j = 0; j < iAttackedNum; j++)
				{
					if (idx == AttackedInfo[j * 4])
					{
						a1			= p_party[AttackedInfo[j * 4 + 1]];
						charahurt	= AttackedInfo[j * 4 + 2];
						pethurt		= AttackedInfo[j * 4 + 3];
						ATR_VCT_NO(a1)		= TOCALL_MAGIC_CASE + 4;
						ATR_LONG_WORK(0, a1) = 0;
						ATR_DAMAGE(a1)		= charahurt;
						ATR_PET_DAMAGE(a1)	= pethurt;
						set_damage_num(a1, 6, -64);
						ATR_LIFE(a1) -= ATR_DAMAGE(a1);
						set_jujutsu_hit_mark(a1);
						if (ATR_LIFE(a1) <= 0)
						{
							ATR_LIFE(a1) = 0;
							slow_flg++;
						}
						ATR_PET_LIFE(a1) -= ATR_PET_DAMAGE(a1);
						//andy_fall
						if (ATR_PETFALL(a1) == 1)
						{
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						if (ATR_PET_LIFE(a1) <= 0 && ATR_RIDE(a1))
						{
							ATR_PET_LIFE(a1) = 0;
							if (ATR_LIFE(a1) > 0)
								petfallChangeGraph(a1);
						}
						ATR_CHR_ACT(a1)		= ANIM_DAMAGE;
						ATR_CHR_ACT_OLD(a1)	= -1;
						pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
					}
				}
			}

		}

		// �����������еĶ�����
		if (ToCallMgc.wNumAttackeds == ToCallMgc.wNumAttacks && 0 == iAttackedNum)
		{
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = false;
			g_iRunEarthQuake = 0;
		}
		break;
	// ���������˵�ͼ
	case TOCALL_MAGIC_CASE + 4:
		if (ATR_FIRST_FLG(a0) > 40)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_LIFE(a0) <= 0)
		{
			ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;
			gemini(a0);
		}
		if (40 == ATR_FIRST_FLG(a0))		// �������40��
		{
			if (ATR_LIFE(a0) <= 0)
			{
				ATR_VCT_NO(a0) = VCT_NO_DIE + 1;
				slow_flg--;
			}
			else
				ATR_VCT_NO(a0) = 0;

			if (ATR_GROUP_FLG(a0) == 1)
				ATR_CRS(a0) = crs_change_tbl2[7];
			else
				ATR_CRS(a0) = crs_change_tbl2[3];
			gemini(a0);
			++iCurAttackedFinishNum;
		}
		// �����������еĶ�����
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			for (int i = 0; i < 10; i++)
			{
				if (a0mark[i] != 0)
				{
					DeathAction(a0tmp[i]);
					a0mark[i] = 0;
				}
			}
			a0tmpcount = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = false;
			g_iRunEarthQuake = 0;
		}
		break;
	case TOCALL_MAGIC_CASE + 5:
		if (ATR_FIRST_FLG(a0) > 60)
			ATR_FIRST_FLG(a0) = 0;
		ATR_FIRST_FLG(a0)++;
		if (ATR_FIRST_FLG(a0) == 60) 
		{
			if (ToCallMgc.wIsPostDisappear)
			{
				for (int i = 0; i < 10; i++)
				{
					if (a0mark[i] != 0)
					{
						DeathAction(a0tmp[i]);
						a0mark[i] = 0;
					}
				}
			}
			a0tmpcount = 0;
			ATR_VCT_NO(a0) = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = false;
			g_iRunEarthQuake = 0;			
		}
		break;
		//��������
	case TOCALL_MAGIC_CASE + 6:
		ATR_CHR_ACT(a0) = ANIM_WALK;		//?�h???????
		ATR_SPD(a0) = 16;
		ATR_CHR_ANG(a0) = (ATR_DAMAGE_ANG(a0) + 4) & 7;
		if (ATR_GROUP_FLG(a0) == 1)
			ATR_CRS(a0) = crs_change_tbl2[3];
		else
			ATR_CRS(a0) = crs_change_tbl2[7];
		gemini(a0);
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		ATR_STIMER(a0)++;
		if (ATR_STIMER(a0) == 20)
		{
			ATR_CRS_OLD(a0) = ATR_CRS(a0) = (ATR_CRS(a0) + 16) & 31;//��?��?
			ATR_VCT_NO(a0)=17;		//?????
			ATR_STIMER(a0) = 0;
			++iCurAttackedFinishNum;
		}
		// �����������еĶ�����
		if (iAttackedNum == iCurAttackedFinishNum)
		{
			for (int i = 0; i < 10; i++)
			{
				if (a0mark[i] != 0)
				{
					DeathAction(a0tmp[i]);
					a0mark[i] = 0;
				}
			}
			a0tmpcount = 0;
			ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
			bRunToCallMgc = false;
			g_iRunEarthQuake = 0;
		}
		break;
#endif
//#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
	case PROSKILL:
		// �Ƿ������·�
		(0 == ATR_GROUP_FLG(a0)) ? ATR_CHR_ANG(a0) = 3 : ATR_CHR_ANG(a0) = 7;

		ATR_CRS_OLD(a0) = ATR_CRS(a0) = crs_change_tbl2[ATR_CHR_ANG(a0)];

		if (0XFFFFFFFF != AttMgc.iPreMgcNum)
		{
			if (NULL == (a1 = GetAction(T_PRIO_MISSILE, sizeof(ATR_EQU))))
			{
				ATR_DAMAGE(p_master) = ATR_BODY_CNT(p_master);
				ATR_VCT_NO(a0) = 0;
				return;
			}
			if (AttPreMagicNum == 0)
			{
				ATR_VCT_NO(a0) = 1;
				return;
			}
			ATR_NAME(a1) = monster;
			ATR_VCT_NO(a1) = PROSKILL + 1;		// ǰ�ö���
			ATR_CHR_NO(a1) = AttPreMagicNum;
			ATR_DISP_PRIO(a1) = ATR_DISP_PRIO(a0) + 1;
			ATR_BODY_WORK(0, a1) = a0;
			ATR_H_POS(a1) = ATR_H_POS(a0);
			ATR_V_POS(a1) = ATR_V_POS(a0);
		}
		if (ATR_WARRIOR_EFFECT(a0) == 1)
			ATR_VCT_NO(a0) = 0;
		else if (ATR_WARRIOR_EFFECT(a0) == 2)
			ATR_VCT_NO(a0) = 1;
		ATR_CHR_ACT_OLD(a0)	= -1;
		pattern(a0, ANM_NOMAL_SPD, ANM_LOOP);
		break;
	case PROSKILL + 1:
		a1 = ATR_BODY_WORK(0, a0);
		ATR_H_POS(a0) = ATR_H_POS(a1) ;
		ATR_V_POS(a0) = ATR_V_POS(a1) - 35;
		if (ATR_CHR_NO(a0) == 101631 || ATR_CHR_NO(a0) == 101632)
			ATR_V_POS(a0) = ATR_V_POS(a1) - 65;
		if (d0 = pattern(a0, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			DeathAction(a0);
			if (ATR_WARRIOR_EFFECT(a1) == 1)
				ATR_VCT_NO(a1) = 1;
		}
		break;
//#endif
	}
#ifdef _SYUTEST
	if (Light1 != NULL)
	{
		if (Light1->anim_cnt == 5)
		{	//���ŵ����һ��
			DeathAction(Light1);
			Light1 = NULL;
		}
	}
#endif
}

ACTION *oft_test(void)
{
	int d0, d1, d2, d7;
	int id_no = 0;
	ACTION *a1;

	p_kanji = NULL;
	for (d7 = 0; d7 < 21; d7++)		//���
		p_missile[d7] = NULL;
	for (d7 = 0; d7 < 20; d7++)		//���
		p_party[d7] = NULL;

	// ���� master action list /* ?????????????�K? */
	a1 = GetAction(T_PRIO_MASTER, sizeof(ATR_EQU));
	if (a1 == NULL)
		return NULL;
	//Ҫִ�е�Func
	ATR_NAME(a1) = master;
	//�����ȸ���ȴ�
	ATR_VCT_NO(a1) = 3;
	//��ʾ����˳��
	ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
	// sprite ���/* ?????�k? */
	ATR_CHR_NO(a1) = 31027;
	ATR_CHR_ANG(a1)=3;		//����
	//����λ��
	ATR_H_POS(a1) = 320;
	ATR_V_POS(a1) = 240;
	// ��������
	ATR_CHR_ACT(a1) = ANIM_ATTACK;		// �ƶ�����
	// ��¼ a1 �� p_master/* ???????��V */
	p_master = a1;
	// ���� field �����µ� action list/* ?????�N��???????????�K? */
	a1 = GetAction(T_PRIO_MASTER, sizeof(ATR_EQU));
	if (a1 == NULL)
		return NULL;
	ATR_ATTRIB(a1) = ACT_ATR_HIDE;	// ����ʾ	//����??
	//��ʾ����˳��
	ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
	//����λ��
	ATR_H_POS(a1) = 640-32;
	ATR_V_POS(a1) = 480-64;
	// ��¼ a1 �� p_attrib/* ???????��V */
	p_attrib = a1;
	//X����
	d0 = 416 + 32 + 5;
	//Y����
	d1 = 408 + 24;
	// Ԥ��Ϊͼ��Ϊ SPR_021em
	d2 = SPR_021em;		/*?????*/
	for (d7 = 0; d7 < 5; d7++)
	{	//�ҷ�����
		// �����ҷ��� monster action list /* ??????????????�K? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		//Ҫִ�е�Func /* ��??? */
		ATR_NAME(a1) = NULL;
		// ����Ϊ�ǳ�
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ��ʾ�����ѶϢ // ??????����????
		a1->atr |= ACT_ATR_INFO;
		// ���Ա���ѡ // ???�R�e??
		a1->atr |= ACT_ATR_HIT;
		// sprite ��� /* ?????�k? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 3;		//����
		ATR_HIT_TIMING(a1) = 5;
		// ����λ�� /* ???�t */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0 + DISPLACEMENT_X;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1 + DISPLACEMENT_Y;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) += 300;
		// һ��ʼ�ڻ������λ��
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???��? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//?�h???????
		ATR_SPD(a1) = 32;		//�ƶ��ٶ�
		ATR_GROUP_FLG(a1) = 0;		// ���� group set //?????????
		ATR_PLACE_NO(a1) = d7;		// ��¼��ʼλ�� //��?�k?��V
		// ��¼ a1 �� �����б���/* ???????��V */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?��?�� */
		d0 += 32;
		d1 -= 24;
	}
	d0 = 320 - 32 + 5;		//??��
	d1 = 432 - 24;		//??��
	d2 = SPR_pet001;		/*???*/
	for (d7 = 5; d7 < 10; d7++)
	{	//�ҷ�����
		/* ??????????????�K? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????����????
		a1->atr |= ACT_ATR_INFO;
		// ???�R�e??
		a1->atr |= ACT_ATR_HIT;
		/* ?????�k? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 3;		//?��
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ???�t */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0 + DISPLACEMENT_X;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1 + DISPLACEMENT_Y;
		ATR_H_POS(a1) += 300;
		ATR_V_POS(a1) += 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???��? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//?�h???????
		ATR_SPD(a1) = 32;		//?�h�L�T
		ATR_GROUP_FLG(a1) = 0;		//?????????
		ATR_PLACE_NO(a1) = d7;		//��?�k?��V
		/* ???????��V */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?��?�� */
		d0 += 32 * 2;
		d1 -= 24 * 2;
	}
	/* ��???? */
	p_party[d7] = NULL;
	d0 = 64 - 32 + 5;		//??��
	d1 = 192 - 24 + 30;		//??��
	d2 = SPR_021em;		/*?????*/
	for (d7 = 10; d7 < 15; d7++)
	{	//�з�����
		/* ??????????????�K? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????����????
		a1->atr |= ACT_ATR_INFO;
		// ???�R�e??
		a1->atr |= ACT_ATR_HIT;
		/* ?????�k? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 7;		//??
		ATR_HIT_TIMING(a1) = 5;
		/* ???�t */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???��? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//?�h???????
		ATR_SPD(a1) = 32;		//?�h�L�T
		ATR_GROUP_FLG(a1) = 1;		//?��???????
		ATR_PLACE_NO(a1) = d7;		//��?�k?��V
		/* ???????��V */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?��?�� */
		d0 += 32;
		d1 -= 24;
	}
	d0 = 32 + 32 + 5;		//??��
	d1 = 264 + 24 + 10;	//??��
	d2 = SPR_pet001;		/*???*/
	for (d7 = 15; d7 < 20; d7++)
	{	//�з�����
		/* ??????????????�K? */
		a1 = GetAction(T_PRIO_JIKI, sizeof(ATR_EQU));
		if (a1 == NULL)
			return NULL;
		ATR_NAME(a1) = NULL;
		ATR_VCT_NO(a1) = VCT_NO_APPEAR;
		/* ����?�I�T */
		ATR_DISP_PRIO(a1) = D_PRIO_MASTER;
		// ??????����????
		a1->atr |= ACT_ATR_INFO;
		// ???�R�e??
		a1->atr |= ACT_ATR_HIT;
		/* ?????�k? */
		ATR_CHR_NO(a1) = d2;
		ATR_CHR_ANG(a1) = 7;		//??
		ATR_HIT_TIMING(a1) = 3 + 2;
		/* ???�t */
		monster_place_pos[id_no] = ATR_H_POS(a1) = ATR_INT_WORK0(a1) = d0;
		monster_place_pos[id_no + 1] = ATR_V_POS(a1) = ATR_INT_WORK1(a1) = d1;
		ATR_H_POS(a1) -= 300;
		ATR_V_POS(a1) -= 300;
		monster_start_pos[id_no++] = ATR_H_POS(a1);
		monster_start_pos[id_no++] = ATR_V_POS(a1);
		/* ???��? */
		ATR_CHR_ACT(a1) = ANIM_STAND;		//?�h???????
		ATR_SPD(a1) = 32;		//?�h�L�T
		ATR_GROUP_FLG(a1) = 1;		//?��???????
		ATR_PLACE_NO(a1) = d7;		//��?�k?��V
		/* ???????��V */
		p_party[d7] = a1;
		pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
		/* ?��?�� */
		d0 += 32 * 2;
		d1 -= 24 * 2;
	}
	// ����λ�������� //???�t?���t
	for (d7 = 0; d7 < 20; d7++)
	{
		a1 = p_party[d7];
		ATR_H_POS(a1) = monster_start_pos[monster_place_no[d7] * 2];
		ATR_V_POS(a1) = monster_start_pos[monster_place_no[d7] * 2 + 1];
		ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d7] * 2];
		ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d7] * 2 + 1];
	}
	command_point = 0;		// clear command point //?????��????????
	return a1;
}

static int bc_pointer;
/* ????��??? *******************************************************************/
int get_bc_num(void)
{
	int d0, d1;

	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//?��?��?��?
		if (d0 == NULL)
		{	//��???
			bc_pointer--;
			return -1;
		}
		if (d0 >= '0' && d0<= '9')		//???
			break;						//??????
		if (d0 >= 'A' && d0<= 'F')		//?????
			break;						//??????
	}
	if (d0 >= 'A')		//?????
		d1 = d0 - 'A' + 10;
	else
		d1 = d0 - '0';

	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//?��????��?��?
		if (d0 == NULL)
		{	//��???
			bc_pointer--;
			break;
		}
		if (d0 < '0' || d0 > '9')
		{	//??????
			if (d0 < 'A' || d0 > 'F')		//????????
				break;						//��?
		}
		d1 = d1 << 4;		//???????
		if (d0 >= 'A')		//?????
			d1 += d0 - 'A' + 10;
		else
			d1 += d0 - '0';
	}
	return d1;
}

/* ??��?��??? *******************************************************************/
void get_bc_asc(ACTION *a1, int flg)
{
	char d0;
	char *a2;

	if (flg == 0)
		a2 = ATR_HANDLE(a1);
	else
		a2 = ATR_TITLE(a1);
	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//?��?��?��?
		if (d0 == NULL)
		{	//��???
			bc_pointer--;
			break;
		}
		//?��???
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleStatus[bc_pointer++];
		}
		else
			*a2++ = d0;
	}
	//��????
	*a2 = 0;
	if (flg == 0)
		makeStringFromEscaped(ATR_HANDLE(a1));
	else
		makeStringFromEscaped(ATR_TITLE(a1));
}

// Robin 0728
/* ??��?��??? for Ride Pet*******************************************************************/
void get_bc_asc_ridepet(ACTION *a1)
{
	char d0;
	char *a2;

	a2 = ATR_PETNAME(a1);
	while (1)
	{
		d0 = BattleStatus[bc_pointer++];		//?��?��?��?
		if (d0 == NULL)
		{	//��???
			bc_pointer--;
			break;
		}
		//?��???
		if (d0 == '|')
			break;
		if (IsDBCSLeadByte(d0))
		{
			*a2++ = d0;
			*a2++ = BattleStatus[bc_pointer++];
		}
		else
			*a2++ = d0;
	}
	//��????
	*a2 = 0;
	makeStringFromEscaped(ATR_PETNAME(a1));
}

//�����Ƿ���״̬������ȷ��
//����p_party�����е�ֵ�����еĳ�ʼ��
void set_bc(void)
{
	ACTION *a1;
	int d0, d2, d3, d1 = 0;

	//????????
	bc_pointer = 3;
	//?????�N��
	ATR_ATTRIB(p_master) = get_bc_num();
	//�P�f????????
	while (BattleStatus[bc_pointer] != NULL)
	{
		//??�k?��???
		d2 = get_bc_num();
#ifdef _DEBUG
		if (d2 >= 20)
        {
        }
			//MessageBox(hWnd, "?????????????????????", "Error", MB_OK);
#endif
		//?????????????
		a1 = p_party[d2];
		/* ��??? */
		ATR_NAME(a1) = monster;
		//���q��???
		get_bc_asc(a1, 0);
		//��?��???
		get_bc_asc(a1, 1);
		//???�k?��???
		ATR_CHR_NO(a1) = get_bc_num();
		//???��???
		ATR_LEVEL(a1) = get_bc_num();
		//??��???
		ATR_LIFE(a1) = get_bc_num();
		//?��??��???
		ATR_MAX_LIFE(a1) = get_bc_num();
#ifdef _BATTLESKILL				// (���ɿ�) Syu ADD ս�����ܽ���
		ATR_MAX_MANA(a1) = pc.maxMp ;
#endif
		//???��???
		d0 = get_bc_num();
		// Robin 0728 ridePet
		if (bNewServer)
		{
			ATR_RIDE(a1) = get_bc_num();
			get_bc_asc_ridepet(a1);
			ATR_PETLEVEL(a1) = get_bc_num();
			ATR_PET_LIFE(a1) = get_bc_num();
			int k = ATR_PET_LIFE(a1);
			ATR_PET_MAX_LIFE(a1) = get_bc_num();
			int o = ATR_PET_MAX_LIFE(a1);
		}
		else
		{
			ATR_RIDE(a1) = 0;
			ATR_PETNAME(a1)[0] = NULL;
			ATR_PETLEVEL(a1) = 0;
			ATR_PET_LIFE(a1) = 0;
			ATR_PET_MAX_LIFE(a1) = 0;
		}
		//��???
		if (d0 & BC_FRESH)
		{
			//??��??
			if ((ATR_CHR_NO(a1) >= SPR_mwood1 && ATR_CHR_NO(a1) <= SPR_mstone3) ||
				 (ATR_CHR_NO(a1) == 15323) || (ATR_CHR_NO(a1) == 10812) || (ATR_CHR_NO(a1) == 15527) ||
				 (ATR_CHR_NO(a1) == 15543) || (ATR_CHR_NO(a1) == 15575) || (ATR_CHR_NO(a1) == 15607))
			{
				ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;		//����??
				//�r??
				ATR_VCT_NO(a1) = 0;
				ATR_CHR_ACT(a1) = ANIM_STAND;
				ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
				//???����
				if (ATR_GROUP_FLG(a1) == 0)		//????????
					ATR_CHR_ANG(a1) = 3;		//��????
				else
					ATR_CHR_ANG(a1) = 7;		//
				ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_CHR_ACT(a1) = ANIM_WALK;
				ATR_CHR_ACT_OLD(a1) = -1;
				pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
			}
			else
			{
				//�K��?
				ATR_VCT_NO(a1) = VCT_NO_APPEAR;
				ATR_ATTRIB(a1) &= ~ACT_ATR_HIDE;		//����??
				//�K�͢r?
				ATR_VCT_NO(p_master) = 3;
				/* ?��??? */
				ATR_H_POS(a1) = monster_start_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = monster_start_pos[monster_place_no[d2] * 2 + 1];
				ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
			}
			//????????????
			set_raster_pos(a1);
//148 149 150
		}
		else 
		{
			if (ATR_LIFE(a1) > 0)
			{	//��?????
				if (d0 & BC_FADE_OUT)
				{	// ??s???
					ATR_ATTRIB(a1) |= ACT_ATR_TRAVEL;
					//??????
					ATR_ATTACK_KIND(2, a1) = 1;
					if (ATR_GROUP_FLG(a1) == 0)
					{	//????????
						ATR_V_POS(a1) = 480 + 16;
						ATR_H_POS(a1) = 640 + SCREEN_OUT;
					}
					else
					{
						ATR_V_POS(a1) = -18;
						ATR_H_POS(a1) = -SCREEN_OUT;
					}
				}
				else
				{
					ATR_ATTRIB(a1) &= ~ACT_ATR_TRAVEL;
					ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
					ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
					//????????????
					set_raster_pos(a1);
				}
				//�r??
				ATR_VCT_NO(a1) = 0;
				ATR_CHR_ACT(a1) = ANIM_STAND;
				if (BattleBpFlag & BATTLE_BP_JOIN)
				{	// �ӡP??
					//???����
					if (ATR_GROUP_FLG(a1) == 0)		//????????
						ATR_CHR_ANG(a1) = 3;		//��????
					else
						ATR_CHR_ANG(a1) = 7;		//
					ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
					ATR_CHR_ACT(a1) = ANIM_WALK;
					ATR_CHR_ACT_OLD(a1) = -1;
					pattern(a1, ANM_NOMAL_SPD, ANM_LOOP);
				}
			}
		}
		//?��??
		if (d0 & BC_DEATH)
		{
			if (ATR_VCT_NO(a1) == VCT_NO_APPEAR)
			{	//?��??????
				//?��???
				ATR_VCT_NO(a1) = VCT_NO_DIE + 2;
				ATR_H_POS(a1) = ATR_INT_WORK0(a1) = monster_place_pos[monster_place_no[d2] * 2];
				ATR_V_POS(a1) = ATR_INT_WORK1(a1) = monster_place_pos[monster_place_no[d2] * 2 + 1];
				//????????????
				set_raster_pos(a1);
				//????
				ATR_LIFE(a1) = 0;
				//?��???????
				if (ATR_GROUP_FLG(a1) == 0)		//????????
					ATR_CHR_ANG(a1) = 3;		//��????
				else
					ATR_CHR_ANG(a1) = 7;		//
				ATR_CRS_OLD(a1) = ATR_CRS(a1) = crs_change_tbl2[ATR_CHR_ANG(a1)];
				ATR_CHR_ACT(a1) = ANIM_DEAD;		//?��???????
				ATR_CHR_ACT_OLD(a1) = -1;
				while (1)
				{
					if (pattern(a1, 1, ANM_NO_LOOP))	//???��???
						break;
				}
				//????����
				LoadBmp(ATR_PAT_NO(a1));
#ifndef _PETSKILL_ANTINTER
				set_piyo_loop(a1);
#endif			
				//?���r?`
				ATR_VCT_NO(p_master) = 3;
			}
		}
		//???��?�V?�ϣ�??
		if (d0 & BC_PET_OK)
			ATR_PET_OK(a1) = 1;
		//??????��????????
		if (ATR_JUJUTSU_WORK(a1) == NULL)
		{
			d3 = 0;
			if (d0 & BC_BIT3)		//  "��"??
				d3 = 1;
			else if (d0 & BC_BIT4)		//  "??"??
				d3 = 2;
			else if (d0 & BC_BIT5)		//  "��?"??
				d3 = 3;
			else if (d0 & BC_BIT6)		//  "��?"??
				d3 = 4;
			else if (d0 & BC_BIT7)		//  "??��?"??
				d3 = 5;
			else if (d0 & BC_BIT8)		//  "??"??
				d3 = 6;
#ifdef _MAGIC_WEAKEN
			if (d0 & BC_WEAKEN)    //����
				d3 = 7;
#endif
#ifdef _MAGIC_DEEPPOISION
			if (d0 & BC_DEEPPOISON)  //�綾
				d3 = 8;
#endif
#ifdef _MAGIC_BARRIER
			if (d0 & BC_BARRIER)  //ħ��
				d3 = 9;
#endif
#ifdef _MAGIC_NOCAST
			if (d0 & BC_NOCAST)   //��Ĭ
				d3 = 10;
#endif
#ifdef _SARS						// WON ADD ��ɷ����
			if (d0 & BC_SARS)		// ��ɷ����
				d3 = 11;
#endif
#ifdef _CHAR_PROFESSION						// WON ADD	��ѣ
			if (d0 & BC_DIZZY)		// ��ѣ
				d3 = 12;
			if (d0 & BC_ENTWINE)	// ��������
				d3 = 13;
			if (d0 & BC_DRAGNET)	// ���޵���
				d3 = 14;
			if (d0 & BC_ICECRACK)	// ������
				d3 = 15;
			if (d0 & BC_OBLIVION)	// ����
				d3 = 16;
			if (d0 & BC_ICEARROW)	// ����
				d3 = 17;
			if (d0 & BC_BLOODWORMS)// ��Ѫ��
				d3 = 18;
			if (d0 & BC_SIGN)		// һ���Ѫ
				d3 = 19;
			if (d0 & BC_CRAZY)		// ��
				d3 = 20;
			if (d0 & BC_F_ENCLOSE)		// ����
				d3 = 21;
			if (d0 & BC_I_ENCLOSE)		// ������
				d3 = 22;
			if (d0 & BC_T_ENCLOSE)		// �׸���
				d3 = 23;
#ifdef _PROFESSION_ADDSKILL
			if (d0 & BC_WATER)
				d3 = 32;		// ˮ����
			if (d0 & BC_WATER)
				d3 = 33;		// �־�
			//if (d0 & BC_F_I_T_ENCLOSE)  // ����׸���
			//    d3 = 24;
#endif
#ifdef _PETSKILL_LER
			if (d0 & BC_CHANGE)
				d3 = 34; // �׶�����
#endif

#endif
			if (d3)
			{	//?��?????
				set_single_jujutsu(d3, a1);		//�O�l����???
				ATR_STATUS(a1) = d3;		//????????
			}
			else
				ATR_STATUS(a1) = 0;		//?????����
		}
		//�N�ͧT��????????
		if (ATR_ATTRIB_WORK(a1) == NULL)
		{
			if (d0 & BC_REVERSE)		//�T��??
				set_attrib_reverse(a1);		//�N�ͧT��???
		}
	}
	//�K�͢r??????
	if (ATR_VCT_NO(p_master) != 3)
		action_inf = 3;		//�K��?����?
}

// ??????��?? ????? ?************************************/

void petfallChangeGraph(ACTION *a0)
{
	//andy_fall
	if (ATR_PETFALL(a0) != 1)
		return;
	if (ATR_RIDE(a0) != 1)
		return;


	for (int j = 0; j < sizeof(ridePetTable) / sizeof(tagRidePetTable); j++)
	{
		if (ridePetTable[j].rideNo == ATR_CHR_NO(a0))
		{
			ATR_CHR_NO(a0) = ridePetTable[j].charNo;
			break;
		}
	}
	ATR_PETFALL(a0) = 2;
	//andy_fall
	ATR_RIDE(a0) = 0;
}

void LogToBattleError(char *data, int line)
{
#ifdef _DEBUG
	FILE *rfp;
	rfp = fopen("battleerror.txt", "a+");
	if (rfp)
	{
		fprintf(rfp, "%d: %s\n", line, data);
		fclose(rfp);
	}
#endif
}
