
#ifndef _VG410_H_
#define _VG410_H_

//#include "windows.h"
#include "../systeminc/action.h"

typedef unsigned char	Uint8 ;		/* ????????? */
typedef signed   char	Sint8 ;		/* ????????? */
typedef unsigned short	Uint16 ;	/* ????????? */
typedef signed   short	Sint16 ;	/* ????????? */
typedef unsigned long	Uint32_ ;	/* ????????? */
typedef signed   long	Sint32_ ;	/* ????????? */
typedef float		Float32 ;	/* ????? */
typedef double		Float64 ;	/* ????? */

/*----------  ????????????  ----------*/
#define	ATR_MAX	256

typedef	void (*FUNC)(Uint16);
typedef	void (*FUNC2)();
typedef struct{
	Uint8	vct_no;
	Uint8	stimer;
	ACTION	*body_work[20];
	Uint8	body_cnt;
	int		damage;

	Uint8	hit_stop;
	Uint8	hit_timing;
	Uint8	group_flg;
	int		attack_kind[20];
	int		attack_pow[20];
	Uint8	combo;
	Uint8	kaishin;
	int	life;
	ACTION	*counter;
	Uint8	counter_flg;
	ACTION	*jujutsu_work;
	ACTION	*magicstatus_work;
	ACTION	*attrib_work;
	Uint8	guard_flg;
	Uint8	damage_ang;
	Uint8	first_flg;
	Uint8	place_no;
	Uint8	pet_ok;
	Uint8	ako_flg;
	Uint8	crs_old;
	Uint8	sync_flg;
	int		int_work0;
	int		int_work1;
	int		int_work2;
	int		int_work3;
	int		long_work[20];
	// Robin 0729
	int		attack_pet_kind[20];
	int		attack_pet_pow[20];
	int		pet_damage;
	int		int_workp;
	int		onRide;
	//andy_mp
	int		mpdamage;
	int		addhp;
	int		mpflg;
	int		addhpflg;
	int		showaddhp;
	int		graNo;
#ifdef __NEW_BATTLE_EFFECT
	int		effect_flg;
	int		effect_anim;
	int		effect_param;
#endif
#ifdef _SKILL_SELFEXPLODE //自爆
	int		petSelfExplode;		//大吼(克年兽)
#endif 
#ifdef _PETSKILL_BATTLE_MODEL
	int	battle_model;
#endif
} ATR_EQU;

#define ATR_NAME(a)			a->func
#define ATR_ATTRIB(a)		a->atr
#define ATR_H_POS(a)		a->x
#define ATR_V_POS(a)		a->y
#define ATR_H_MINI(a)		a->h_mini
#define ATR_V_MINI(a)		a->v_mini
#define	ATR_SPD(a)			a->spd
#define ATR_CRS(a)			a->crs
//#define	ATR_SPD(a)		((ATR_EQU *)a->pReg)->spd
//#define ATR_CRS(a)		((ATR_EQU *)a->pReg)->crs
//#define	ATR_H_MINI(a)		((ATR_EQU *)a->pReg)->h_mini
//#define ATR_V_MINI(a)		((ATR_EQU *)a->pReg)->v_mini
#define ATR_PAT_NO(a)		a->bmpNo
#define ATR_CHR_NO(a)		a->anim_chr_no
#define ATR_CHR_NO_OLD(a)	a->anim_chr_no_bak
#define	ATR_CHR_ACT(a)		a->anim_no
#define	ATR_CHR_ACT_OLD(a)	a->anim_no_bak
#define	ATR_CHR_ANG(a)		a->anim_ang
#define	ATR_CHR_ANG_OLD(a)	a->anim_ang_bak
#define	ATR_CHR_CNT(a)		a->anim_cnt
#define	ATR_CHR_TIM(a)		a->anim_frame_cnt
#define	ATR_CHR_H_POS(a)	a->anim_x
#define	ATR_CHR_V_POS(a)	a->anim_y
#define ATR_TASK_PRIO(a)	a->prio						/* ?????? */
#define ATR_DISP_PRIO(a)	a->dispPrio					/* ???? */
#define ATR_LIFE(a)			a->hp
#ifdef _PET_ITEM
#define ATR_OLD_LIFE(a)		a->iOldHp
#endif
#define ATR_MP(a)			a->mp
#define ATR_MAX_LIFE(a)		a->maxHp
#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
#define ATR_MAX_MANA(a)		a->maxMp
#endif
#define ATR_HANDLE(a)		a->name
#define ATR_TITLE(a)		a->freeName
#define ATR_LEVEL(a)		a->level
#define ATR_HIT(a)			a->anim_hit
#define ATR_STATUS(a)		a->status						/* ????? */
// Robin 0728 ridePet
#define ATR_RIDE(a)			a->onRide
#define ATR_PETNAME(a)		a->petName
#define ATR_PETLEVEL(a)		a->petLevel
#define ATR_PET_LIFE(a)		a->petHp
#define ATR_PET_MAX_LIFE(a)		a->petMaxHp
#define ATR_PETFALL(a)		a->petFall
#ifdef _SKILL_ROAR  
#define ATR_ROAR(a)		    a->petRoar		        //大吼(克年兽)
#endif 
#ifdef _SKILL_SELFEXPLODE //自爆
#define ATR_SELFEXPLODE(a)		    a->petSelfExplode	
#endif 
#ifdef _MAGIC_DEEPPOISION   //剧毒
#define ATR_DEEPPOISION(a)		    a->petDeepPoision	
#endif 
//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
#define ATR_WARRIOR_EFFECT(a) a->warrioreffect
//#endif

#define ATR_VCT_NO(a)		((ATR_EQU *)a->pYobi)->vct_no
#define ATR_STIMER(a)		((ATR_EQU *)a->pYobi)->stimer
#define ATR_BODY_WORK(b,a)	((ATR_EQU *)a->pYobi)->body_work[b]
#define ATR_BODY_CNT(a)		((ATR_EQU *)a->pYobi)->body_cnt
#define ATR_DAMAGE(a)		((ATR_EQU *)a->pYobi)->damage

//andy_mp
#define ATR_MPDAMAGE(a)		((ATR_EQU *)a->pYobi)->mpdamage
#define ATR_MPDFLG(a)		((ATR_EQU *)a->pYobi)->mpflg
#define ATR_ADDHP(a)		((ATR_EQU *)a->pYobi)->addhp
#define ATR_ADDHPFLG(a)		((ATR_EQU *)a->pYobi)->addhpflg
#define ATR_SHOWADDHP(a)	((ATR_EQU *)a->pYobi)->showaddhp
#define ATR_BATTLEGRANO(a)	((ATR_EQU *)a->pYobi)->graNo

#define ATR_GROUP_FLG(a)	((ATR_EQU *)a->pYobi)->group_flg
#define ATR_ATTACK_KIND(b,a)	((ATR_EQU *)a->pYobi)->attack_kind[b]

#define ATR_ATTACK_POW(b,a)	((ATR_EQU *)a->pYobi)->attack_pow[b]
#define ATR_HIT_STOP(a)		((ATR_EQU *)a->pYobi)->hit_stop
#define ATR_HIT_TIMING(a)	((ATR_EQU *)a->pYobi)->hit_timing
#define ATR_COMBO(a)		((ATR_EQU *)a->pYobi)->combo
#define ATR_KAISHIN(a)		((ATR_EQU *)a->pYobi)->kaishin
#define ATR_COUNTER(a)		((ATR_EQU *)a->pYobi)->counter
#define ATR_COUNTER_FLG(a)	((ATR_EQU *)a->pYobi)->counter_flg
#define ATR_JUJUTSU_WORK(a)	((ATR_EQU *)a->pYobi)->jujutsu_work
#define ATR_MAGICSU_WORK(a)	((ATR_EQU *)a->pYobi)->magicstatus_work
#define ATR_ATTRIB_WORK(a)	((ATR_EQU *)a->pYobi)->attrib_work
#define ATR_GUARD_FLG(a)	((ATR_EQU *)a->pYobi)->guard_flg
#define ATR_DAMAGE_ANG(a)	((ATR_EQU *)a->pYobi)->damage_ang
#define ATR_FIRST_FLG(a)	((ATR_EQU *)a->pYobi)->first_flg
#define ATR_PLACE_NO(a)		((ATR_EQU *)a->pYobi)->place_no
#define ATR_PET_OK(a)		((ATR_EQU *)a->pYobi)->pet_ok
#define ATR_AKO_FLG(a)		((ATR_EQU *)a->pYobi)->ako_flg
#define ATR_CRS_OLD(a)		((ATR_EQU *)a->pYobi)->crs_old
#define ATR_SYNC_FLG(a)		((ATR_EQU *)a->pYobi)->sync_flg
#define ATR_INT_WORK0(a)	((ATR_EQU *)a->pYobi)->int_work0
#define ATR_INT_WORK1(a)	((ATR_EQU *)a->pYobi)->int_work1
#define ATR_INT_WORK2(a)	((ATR_EQU *)a->pYobi)->int_work2
#define ATR_INT_WORK3(a)	((ATR_EQU *)a->pYobi)->int_work3
#define ATR_LONG_WORK(b,a)	((ATR_EQU *)a->pYobi)->long_work[b]
// Robin 0729 ride pet
#define ATR_ATTACK_PET_KIND(b,a)	((ATR_EQU *)a->pYobi)->attack_pet_kind[b]
#define ATR_ATTACK_PET_POW(b,a)	((ATR_EQU *)a->pYobi)->attack_pet_pow[b]
#define ATR_PET_DAMAGE(a)		((ATR_EQU *)a->pYobi)->pet_damage
#define ATR_INT_WORKp(a)	((ATR_EQU *)a->pYobi)->int_workp
#define ATR_RIDE_FLAG(a)	((ATR_EQU *)a->pYobi)->onRide

#ifdef __NEW_BATTLE_EFFECT
#define ATR_EFFECT_FLAG(a)	((ATR_EQU *)a->pYobi)->effect_flg
#define ATR_EFFECT_ANIM(a)	((ATR_EQU *)a->pYobi)->effect_anim
#define ATR_EFFECT_PARAM(a)	((ATR_EQU *)a->pYobi)->effect_param
#endif

#ifdef _PETSKILL_BATTLE_MODEL
#define ATR_BATTLE_MODEL(a)	((ATR_EQU *)a->pYobi)->battle_model
#endif

#ifdef _SKILL_SELFEXPLODE //自爆 
//#define ATR_SELFEXPLODE(a)	((ATR_EQU *)a->pYobi)->petSelfExplode 		//自爆
#endif 
/*----------  ???  ----------*/
#define ATT_HIT				'H'				//???
#define ATT_FIRE			'Z'				//?
#define ATT_BOW				'B'				//?
#define ATT_TAKE			'T'				//?
#define ATT_ESCAPE			'E'				//
#define ATT_SELECT			'S'				//???
#define ATT_MONSTER			'W'				//
#define ATT_JUJUTSU			'J'				//
#ifdef __NEW_BATTLE_EFFECT
#define ATT_EFFECT			'j'				//其它的战斗特效
#endif
//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
#define ATT_PROSKILL		'+'
//#endif
#ifdef _FIREHUNTER_SKILL				// (不可开) ROG ADD 朱雀技能_火线猎杀
#define ATT_FIRESKILL		'f'				
#endif
#ifdef _PROFESSION_ADDSKILL//结界
#define ATT_BOUNDARY		'a'				
#endif
#ifdef _PETSKILL_BATTLE_MODEL
#define ATT_BATTLE_MODEL 'b'	// 宠物技能战斗模组
#endif
#ifdef _PETSKILL_RIDE
#define ATT_RIDE		'c'				
#endif

//andy_mp
#define ATT_MPDAMAGE		'p'				//MP伤害
#define ATT_DAMAGETOHP		'h'
#define ATT_MAGICSTATUS		'm'				//特殊状态
#ifdef _MAGIC_DEEPPOISION
#define	ATT_DEEPPOISION     'd'             //剧毒
#endif
#define ATT_BOOMERANG		'O'				//?????
#define ATT_SYNCHRONOUS		'Y'				//???
#define ATT_MALFUNCTION		'M'				//
#define ATT_DAMAGE			'D'				//????
#define ATT_FADE_OUT		'F'				//
#define ATT_LIFE			'L'				//佋???
#define ATT_IN				'I'				//佋???
#define ATT_VARIABLE		'V'				//??
#define ATT_REVERSE			'R'				//??
#define ATT_QUIT			'Q'				//?????
#define ATT_NIX				'X'				//????
#define ATT_KANJI			'K'				//??
#define ATT_NEXT_EQUIP		'N'				//??
#define ATT_COMPANIONS		'!'				//??
#define ATT_STEAL			'#'				//??
#define ATT_TALK			'%'				//????
#define ATT_TOCALL			'$'				// 召唤

#define ATT_DEATH			(1 << 0)		//???
#define ATT_NORMAL			(1 << 1)		//??
#define ATT_SATISFACTORY	(1 << 2)		//????
#define ATT_GUARD			(1 << 3)		//㘎?
#define ATT_COUNTER			(1 << 4)		//???????
#define ATT_DODGE			(1 << 5)		//???
#define ATT_AKO1			(1 << 6)		//????????????
#define ATT_AKO2			(1 << 7)		//????????????
#define ATT_G_CRASH			(1 << 8)		//????????
#define ATT_VICARIOUS		(1 << 9)		//??
#define ATT_REFLEX			(1 << 10)		//
#define ATT_ABSORPTION		(1 << 11)		//?
#define ATT_BALLIA			(1 << 12)		//???
#define ATT_CRUSH			(1 << 13)		//?????
#define ATT_FALL			(1 << 14)		//落马
#define ATT_TOOEH			(1 << 15)		//啮齿
#define ATT_ATTDOUBLE		(1 << 16)		//属性强化攻击
#ifdef _SKILL_ROAR  
#define ATT_ATTROAR		    (1 << 17)		//大吼(克年兽)
#endif 
#ifdef _SKILL_SELFEXPLODE //自爆
#define ATT_ATTSELFEXPLODE	(1 << 18)		//自爆
#endif
#ifdef _PETSKILL_EXPLODE
#define ATT_EXPLODE	(1 << 29)		//爆弹(跟自爆一样,只是角色不会消失)
#endif 

#define BCF_DEFMAGICATT		(1 << 19)
#define BCF_SUPERWALL		(1 << 20)
#define BCF_MODIFY			(1 << 21)		//属性转换


//#ifdef _BATTLESKILL				// (不可开) Syu ADD 战斗技能介面
#define ATT_ATTPREPROSKILL		(1 << 22)		//职业技能
#define ATT_ATTNEXTPROSKILL		(1 << 23)		//职业技能
#define ATT_TRAP				(1 << 25)		//陷阱
#define ATT_NOMISS				(1 << 26)		//暴击第一下击空
//#endif

#ifdef _EQUIT_ARRANGE
#define ATT_ARRANGE				(1 << 24)	//格挡秀图#endif
#endif

#ifdef _PETSKILL_ACUPUNCTURE
#define ATT_ACUPUNCTURE			(1 << 27)
#endif

#ifdef _PETSKILL_ANTINTER
#define ATT_ANTINTER            (1 << 28)
#endif

#ifdef _PET_ITEM
#define ATT_ATTACKBACK			(1 << 30)
#endif

#define JUJUTSU_1			(1)				//  "", 
#define JUJUTSU_2			(2)				//  "??", 
#define JUJUTSU_3			(3)				//  "䞍?", 
#define JUJUTSU_4			(4)				//  "?", 
#define JUJUTSU_5			(5)				//  "?????", 
#define JUJUTSU_6			(6)				//  "??" 

#define BC_FRESH			(1 << 0)		//??
#define BC_DEATH			(1 << 1)		//?
#define BC_PET_OK			(1 << 2)		//?????ＯＫ
#define BC_BIT3				(1 << 3)		//  "", 
#define BC_BIT4				(1 << 4)		//  "??", 
#define BC_BIT5				(1 << 5)		//  "䞍?", 
#define BC_BIT6				(1 << 6)		//  "?", 
#define BC_BIT7				(1 << 7)		//  "?????", 
#define BC_BIT8				(1 << 8)		//  "??" 
#define BC_FADE_OUT			(1 << 9)		//??? 
#define BC_REVERSE			(1 << 10)		//
#ifdef _MAGIC_WEAKEN
#define BC_WEAKEN           (1 << 11)      //虚弱
#endif
#ifdef _MAGIC_DEEPPOISION
#define BC_DEEPPOISON       (1 << 12)      //剧毒   
#endif
#ifdef _MAGIC_BARRIER
#define BC_BARRIER          (1 << 13)      //魔障   
#endif
#ifdef _MAGIC_NOCAST
#define BC_NOCAST           (1 << 14)      //沉默   
#endif

#ifdef _SARS						// WON ADD 毒煞蔓延
#define BC_SARS				(1 << 15)	   // 毒煞蔓延
#endif

#ifdef _CHAR_PROFESSION					   // WON ADD
#define	BC_DIZZY			(1 << 16)	   // 晕眩 
#define BC_ENTWINE			(1 << 17)	   // 树根缠绕
#define BC_DRAGNET			(1 << 18)	   // 天罗地网
#define BC_ICECRACK			(1 << 19)	   // 冰爆术	
#define BC_OBLIVION 		(1 << 20)	   // 遗忘
#define BC_ICEARROW         (1 << 21)	   // 冰箭
#define BC_BLOODWORMS		(1 << 22)	   // 嗜血蛊
#define BC_SIGN				(1 << 23)	   // 一针见血
#define BC_CRAZY			(1 << 24)      // 挑拨
#define BC_F_ENCLOSE		(1 << 25)	   // 火附体
#define BC_I_ENCLOSE		(1 << 26)      // 冰附体
#define BC_T_ENCLOSE		(1 << 27)      // 雷附体

#ifdef _PROFESSION_ADDSKILL
#define BC_WATER            (1 << 28)      // 水附体
#define BC_FEAR				(1 << 29)	   // 恐惧
//#define BC_F_I_T_ENCLOSE	(1 << 28)      // 火冰雷附体
#endif
#ifdef _PETSKILL_LER
#define BC_CHANGE			(1<<30) // 雷尔变身
#endif
#ifdef _PRO_KILLME
#define BC_ANGER			(1<<31) // 怒
#endif

#endif


/*----------  ????????  ----------*/
// ??
#define	JOY_RIGHT	(1 << 15)	/* Right Key				*/
#define	JOY_LEFT	(1 << 14)	/*  Left Key				*/
#define	JOY_DOWN	(1 << 13)	/*  Down Key				*/
#define	JOY_UP		(1 << 12)	/*    Up Key				*/
#define	JOY_START	(1 << 11)	/* Start					*/
#define	JOY_A		(1 << 10)	/* A Trigger				*/
#define	JOY_C		(1 <<  9)	/* C Trigger				*/
#define	JOY_B		(1 <<  8)	/* B Trigger				*/
#define	JOY_R		(1 <<  7)	/* R Trigger				*/
#define	JOY_X		(1 <<  6)	/* X Trigger				*/
#define	JOY_DEL		(1 <<  5)	/* DELETE					*/
#define	JOY_INS		(1 <<  4)	/* INSERT					*/
#define	JOY_END		(1 <<  3)	/* END						*/
#define	JOY_HOME	(1 <<  2)	/* HOME						*/
#define	JOY_P_DOWN	(1 <<  1)	/* PAGE_UP					*/
#define	JOY_P_UP	(1 <<  0)	/* PAGE_DOWN				*/

#define	JOY_ESC		(1 << 31)	/* ESC Key					*/
#define	JOY_CTRL_M	(1 << 30)	/* Ctrl + M					*/
#define	JOY_CTRL_S	(1 << 29)	/* Ctrl + S					*/
#define	JOY_CTRL_P	(1 << 28)	/* Ctrl + P					*/
#define	JOY_CTRL_I	(1 << 27)	/* Ctrl + I					*/
#define	JOY_CTRL_E	(1 << 26)	/* Ctrl + E					*/
#define	JOY_CTRL_A	(1 << 25)	/* Ctrl + A					*/

#define	JOY_CTRL_C	(1 << 24)	/* Ctrl + C					*/
#define	JOY_CTRL_V	(1 << 23)	/* Ctrl + V					*/
#define	JOY_CTRL_T	(1 << 22)	/* Ctrl + T					*/

#if 0
#define	JOY_X		(1 << 22)	/* X Trigger				*/
#define	JOY_Y		(1 << 21)	/* Y Trigger				*/
#define	JOY_Z		(1 << 20)	/* Z Trigger				*/
#define	JOY_L		(1 << 19)	/* L Trigger				*/
#define	JOY_G		(1 << 18)	/* L Trigger				*/
#define	JOY_H		(1 << 17)	/* L Trigger				*/
#define	JOY_H		(1 << 16)	/* L Trigger				*/
#endif

// ??
//#define	JOY_RIGHT	(1 << 15)	/* Right Key				*/
//#define	JOY_LEFT	(1 << 14)	/*  Left Key				*/
//#define	JOY_DOWN	(1 << 13)	/*  Down Key				*/
//#define	JOY_UP		(1 << 12)	/*    Up Key				*/
// ?????????
#define	JOY_F1		(1 <<  0)	/* F1				*/
#define	JOY_F2		(1 <<  1)	/* F2				*/
#define	JOY_F3		(1 <<  2)	/* F3				*/
#define	JOY_F4		(1 <<  3)	/* F4				*/
#define	JOY_F5		(1 <<  4)	/* F5				*/
#define	JOY_F6		(1 <<  5)	/* F6				*/
#define	JOY_F7		(1 <<  6)	/* F7				*/
#define	JOY_F8		(1 <<  7)	/* F8				*/
#define	JOY_F9		(1 <<  8)	/* F9				*/
#define	JOY_F10		(1 <<  9)	/* F10				*/
#define	JOY_F11		(1 << 10)	/* F11				*/
#define	JOY_F12		(1 << 11)	/* F12				*/
// ?????????????
#define	JOY_CTRL_1	(1 << 12)	/* Ctrl + 1			*/
#define	JOY_CTRL_2	(1 << 13)	/* Ctrl + 2			*/
#define	JOY_CTRL_3	(1 << 14)	/* Ctrl + 3			*/
#define	JOY_CTRL_4	(1 << 15)	/* Ctrl + 4			*/
#define	JOY_CTRL_5	(1 << 16)	/* Ctrl + 5			*/
#define	JOY_CTRL_6  (1 << 17)	/* Ctrl + 6			*/
#define	JOY_CTRL_7	(1 << 18)	/* Ctrl + 7			*/
#define	JOY_CTRL_8	(1 << 19)	/* Ctrl + 8			*/
#define	JOY_CTRL_9	(1 << 20)	/* Ctrl + 9			*/
#define	JOY_CTRL_0	(1 << 21)	/* Ctrl + 0			*/
#define	JOY_CTRL_MINUS				(1 << 22)	/* Ctrl + MINUS			*/
#define	JOY_CTRL_CIRCUMFLEX		(1 << 23)	/* Ctrl + DIK_CIRCUMFLEX			*/
#define	JOY_CTRL_YEN				(1 << 28)	/* Ctrl + YEN			*/
// ????
#define	JOY_TAB				(1 << 25)	/* Ctrl + TAB			*/
// ?????
#define	JOY_RSHIFT			(1 << 26)	/* Ctrl + RSHIFT			*/
#define	JOY_LSHIFT			(1 << 27)	/* Ctrl + LSHIFT			*/
// ?????
#define	JOY_CTRL_J			(1 << 24)	/* Ctrl + J	*/
// ??????
#define	JOY_CTRL_Q			(1 << 29)	/* Ctrl + Q	*/
// ??????
#define	JOY_CTRL_W			(1 << 30)	/* Ctrl + W	*/
// ?????????
#define	JOY_CTRL_G			(1 << 31)	/* Ctrl + G	*/

#ifdef _TELLCHANNEL				//ROG ADD 密语频道
#define JOY_CTRL_R  (1 << 31)
#endif

/*---------  ????? ------------------*/
#define	VK_A			0x41
#define	VK_B			0x42
#define	VK_C			0x43
#define	VK_D			0x44
#define	VK_E			0x45
#define	VK_F			0x46
#define	VK_G			0x47
#define	VK_H			0x48
#define	VK_I			0x49
#define	VK_J			0x4A
#define	VK_K			0x4B
#define	VK_L			0x4C
#define	VK_M			0x4D
#define	VK_N			0x4E
#define	VK_O			0x4F
#define	VK_P			0x50
#define	VK_Q			0x51
#define	VK_R			0x52
#define	VK_S			0x53
#define	VK_T			0x54
#define	VK_U			0x55
#define	VK_V			0x56
#define	VK_W			0x57
#define	VK_X			0x58
#define	VK_Y			0x59
#define	VK_Z			0x5A

#endif
