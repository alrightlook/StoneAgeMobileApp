#include "../systeminc/version.h"
#include"../systeminc/system.h"
#include"../systeminc/action.h"
#include"../systeminc/pc.h"
#include"../systeminc/character.h"
#include"../systeminc/map.h"
#include"../systeminc/tool.h"
#include"../systeminc/anim_tbl.h"
#include"../systeminc/loadsprbin.h"
#include"../systeminc/netmain.h"
#include"../systeminc/lssproto_cli.h"
#include"../systeminc/savedata.h"
#include"../systeminc/menu.h"
#include"../systeminc/field.h"
#include"../systeminc/login.h"
#include"../systeminc/netproc.h"

PC pc;
short maxPcNo;
short selectPcNo;
short prSendMode = 0;
short prSendFlag = 0;

short jbSendFlag;
short duelSendFlag;

int loginDp;

short helpFlag = 0;

short tradeFlag = 0;
short tradeStatus = 0;

PET pet[MAX_PET];

#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
PROFESSION_SKILL profession_skill[MAX_PROFESSION_SKILL];
#endif

MAGIC magic[MAX_MAGIC];


PARTY party[MAX_PARTY];
short partyModeFlag = 0;


CHARLISTTABLE chartable[MAXCHARACTER];

ADDRESS_BOOK addressBook[MAX_ADR_BOOK];

BATTLE_RESULT_MSG battleResultMsg;

PET_SKILL petSkill[MAX_PET][MAX_SKILL];


char newCharacterName[CHAR_NAME_LEN+1];
int newCharacterGraNo;
int newCharacterFaceGraNo;
int newCharacterVit;
int newCharacterStr;
int newCharacterTgh;
int newCharacterDex;
int newCharacterEarth;
int newCharacterWater;
int newCharacterFire;
int newCharacterWind;
int newCharacterHomeTown;

template <typename T>
inline void swap(T &T1, T &T2)
{
	T TTemp;

	TTemp = T1;
	T1 = T2;
	T2 = TTemp;
}

void swap(char *a, char *b)
{
	char tmp[256];

	if(strlen(a) > 255 || strlen(b) > 255)
		return;

	strcpy(tmp, a);
	strcpy(a, b);
	strcpy(b, tmp);
}

//void swapInteger(int *, int *);
//void swapShort(short *, short *);
//void swapString(char *, char *);

void initPcAll(void)
{
	int i;

	memset(&pc, 0, sizeof(pc));
	memset(&pet, 0, sizeof(pet));
	memset(&magic, 0, sizeof(magic));
	memset(&party, 0, sizeof(party));
	memset(&petSkill, 0, sizeof(petSkill));

#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ	
	memset(&profession_skill, 0, sizeof(profession_skill));
#endif

	pc.mailPetNo = -1;

	getUserSetting(selectPcNo);
		
	partyModeFlag = 0;
	sPetStatFlag = 1;

	for(i = 0; i < MAX_PARTY; i++)
	{
		party[i].useFlag = 0;
		party[i].id = 0;
		party[i].ptAct = NULL;
	}


#if 1
	pc.faceGraNo = newCharacterFaceGraNo;
	pc.dp = loginDp;
#endif

	// shan 2002/01/18	
	{
		FILE *fp;
		char filename[255];
		char vTemp[32];		

		sprintf(filename, "/sdcard/jerrysa/map/bgm%d.dat", sCharSide);
		if((fp = fopen(filename, "rb+"))!=NULL)
		{
			fread(&vTemp, sizeof(char), 2, fp);
			fclose (fp);
			fMapBgm = atoi(vTemp);
		}		
	}
#ifdef _STREET_VENDOR
	pc.iOnStreetVendor = 0;
#endif
}


void initPc(void)
{
	int walk = 0, height = 0;
	int i;

#ifndef _DEBUG
	createPc(pc.graNo, nowGx, nowGy, pc.dir);
#else
	if(offlineFlag)
	{
		createPc(SPR_pet004, nowGx, nowGy, pc.dir);
	}
	else
	{
		createPc(pc.graNo, nowGx, nowGy, pc.dir);
	}
#endif

	if((pc.status & CHR_STATUS_W))
	{
		walk = 1;
	}
	if((pc.status & CHR_STATUS_H))
	{
		height = 1;
	}
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
//    #ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
//	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height, pc.profession_class, pc.profession_level, pc.profession_exp, pc.profession_skill_point, pc.ptAct->gm_name);
//	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height, pc.profession_class, pc.profession_level, pc.profession_skill_point, pc.ptAct->gm_name);    
//	#else	
//	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height, pc.profession_class, pc.profession_level, pc.profession_exp, pc.profession_skill_point);
#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height, pc.profession_class, pc.profession_level, pc.profession_skill_point , pc.herofloor);
#else
	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height, pc.profession_class, pc.profession_level, pc.profession_skill_point);
#endif
//	#endif
#else
	setPcParam(pc.name, pc.freeName, pc.level, pc.ridePetName, pc.ridePetLevel, pc.nameColor, walk, height);
#endif

#ifdef _FRIENDCHANNEL
//	strcpy(pc.chatRoomNum,"");
#endif

	if(pc.ptAct != NULL)
	{
		pc.ptAct->hp = pc.hp;
		pc.ptAct->maxHp = pc.maxHp;

		// ??�ѩ�????????????????��
		for(i = 0; i < MAX_PARTY; i++)
		{
			if(party[i].useFlag != 0 && party[i].id == pc.id)
			{
				party[i].ptAct = pc.ptAct;
				break;
			}
		}
	}


	prSendMode = 0;
	prSendFlag = 0;
	jbSendFlag = 0;
	duelSendFlag = 0;
	helpFlag = 0;

}


// PC????��
void createPc(int graNo, int gx, int gy, int dir)
{
	pc.graNo = graNo;
	pc.dir = dir;
	if(pc.ptAct == NULL)
	{
		pc.ptAct = createCharAction(graNo, gx, gy, dir);
		if(pc.ptAct != NULL)
		{
			pc.ptAct->atr |= ACT_ATR_TYPE_PC;
		}
	}
	else
	{
		setPcGraNo(graNo, dir);
		setPcWarpPoint(gx, gy);
		setPcPoint();
	}
}


// PC???????
void resetPc(void)
{
	int i;

	if(pc.ptAct != NULL)
	{
#ifdef _ITEM_FIREWORK
		if (((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[0])
		{
			DeathAction(((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[0]);
			((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[0] = NULL;
		}
		if (((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[1])
		{
			DeathAction(((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[1]);
			((CHAREXTRA*)pc.ptAct->pYobi)->pActFirework[1] = NULL;
		}
#endif

		DeathAction(pc.ptAct);
		pc.ptAct = NULL;
	}

	// ????????��????
	delPcLeader();
}


// PC?????????�k?�@�e
void setPcGraNo(int graNo, int dir)
{
	pc.graNo = graNo;
	pc.dir = dir;

	if(pc.ptAct == NULL)
		return;

	pc.ptAct->anim_chr_no = graNo;
	pc.ptAct->anim_ang = dir;
}


// PC???ID?�@�e
void setPcId(int id)
{
	pc.id = id;
}


// PC??????�I�@�e
void setPcWarpPoint(int gx, int gy)
{
//	if(pc.ptAct == NULL)
//		return;

	setWarpMap(gx, gy);
}


// PC???????�t?�i???
void setPcPoint(void)
{
	if(pc.ptAct == NULL)
		return;

	pc.ptAct->mx = nowX;
	pc.ptAct->my = nowY;
	pc.ptAct->gx = nowGx;
	pc.ptAct->gy = nowGy;
	pc.ptAct->vx = nowVx;
	pc.ptAct->vy = nowVy;
	pc.ptAct->nextGx = nextGx;
	pc.ptAct->nextGy = nextGy;
}


// PC???????�@�e
void setPcDir(int dir)
{
	pc.dir = dir;

	if(pc.ptAct == NULL)
		return;

	pc.ptAct->anim_ang = dir;
}


// ???????�@�e
void setPcAction(int act)
{
	if(pc.ptAct == NULL)
		return;

	pc.ptAct->anim_no = act;
	if(pc.ptAct->anim_no != ANIM_WALK)
	{
		pc.ptAct->anim_no_bak = -1;
	}
}

#ifdef __EMOTION
void setPcEmotion(int emotion)
{
	CHAREXTRA *ext;
	if(pc.ptAct == NULL)
		return;
	ext = (CHAREXTRA *)pc.ptAct->pYobi;
	if(ext->ptActEmotion){
		DeathAction(ext->ptActEmotion);
		ext->ptActEmotion = NULL;
	}
	ext->ptActEmotion = 
		createCommmonEffectAction(emotion, pc.ptAct->gx, pc.ptAct->gy, 0, 0, DISP_PRIO_CHAR);
}
#endif

// ????????�֢t��x
int getPcAction(void)
{
	if(pc.ptAct == NULL)
		return -1;

	return pc.ptAct->anim_no;
}

#ifdef _CHANNEL_MODIFY
extern int TalkMode;
#endif

#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
//    #ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
//  void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height, int profession_class, int profession_level, int profession_exp, int profession_skill_point , char *gm_name)
//    void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height, int profession_class, int profession_level, int profession_skill_point , char *gm_name)    
//	#else
//	void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height, int profession_class, int profession_level, int profession_exp, int profession_skill_point)
#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
	void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height, int profession_class, int profession_level, int profession_skill_point , int herofloor)
#else
	void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height, int profession_class, int profession_level, int profession_skill_point)
#endif
// 	#endif
#else
    void setPcParam(char *name, char *freeName, int level, char *petname, int petlevel, int nameColor, int walk, int height)
#endif
{
	int nameLen;
	int freeNameLen;
	int petnameLen;
#ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
	int gmnameLen;
#endif
	nameLen = strlen(name);
	if(nameLen <= CHAR_NAME_LEN)
	{
		strcpy(pc.name, name);
	}

	freeNameLen = strlen(freeName);
	if(freeNameLen <= CHAR_FREENAME_LEN)
	{
		strcpy(pc.freeName, freeName);
	}

	pc.level = level;

	petnameLen = strlen(petname);
	if(petnameLen <= CHAR_FREENAME_LEN)
	{
		strcpy(pc.ridePetName, petname);
	}

	pc.ridePetLevel = petlevel;

	pc.nameColor = nameColor;
	if(walk != 0)		// ??��?��??
	{
		pc.status |= CHR_STATUS_W;
	}
	if(height != 0)	// ?????
	{
		pc.status |= CHR_STATUS_H;
	}

	if(pc.ptAct == NULL)
		return;

	if(nameLen <= CHAR_NAME_LEN)
	{
		strcpy(pc.ptAct->name, name);
	}
	if(freeNameLen <= CHAR_FREENAME_LEN)
	{
		strcpy(pc.ptAct->freeName, freeName);
	}
	pc.ptAct->level = level;
	
	if(petnameLen <= CHAR_FREENAME_LEN)
	{
		strcpy(pc.ptAct->petName, petname);
	}
	pc.ptAct->petLevel = petlevel;

	pc.ptAct->itemNameColor = nameColor;

#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
	pc.profession_class	= profession_class;
	pc.ptAct->profession_class = profession_class;
	pc.profession_level = profession_level;
//	pc.profession_exp = profession_exp;
	pc.profession_skill_point = profession_skill_point;
#endif
#ifdef _ALLDOMAN // (���ɿ�) Syu ADD ���а�NPC
	pc.herofloor = herofloor;
#endif
#ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
	gmnameLen = strlen(gm_name);
	if(gmnameLen <= 33){
		strcpy(pc.ptAct->gm_name, gm_name);
	}
#endif

#ifdef _CHANNEL_MODIFY
	if(pc.profession_class == 0){
		pc.etcFlag &= ~PC_ETCFLAG_CHAT_OCC;
		//TalkMode = 0;
	}
#endif
}

extern int nServerGroup;
// PC????�ѩ�???????�@�e????
void updataPcAct(void)
{
	if(pc.ptAct == NULL)
		return;

	if(strlen(pc.name) <= CHAR_NAME_LEN)
	{
		strcpy(pc.ptAct->name, pc.name);
	}
	if(strlen(pc.freeName) <= CHAR_FREENAME_LEN)
	{
		strcpy(pc.ptAct->freeName, pc.freeName);
	}
	pc.ptAct->level = pc.level;
	pc.ptAct->hp    = pc.hp;
	pc.ptAct->maxHp = pc.maxHp;


}


// PC???????????
void setPcLeader(void)
{
	pc.status |= CHR_STATUS_LEADER;
}


// PC????????????????
void delPcLeader(void)
{
	pc.status &= (~CHR_STATUS_LEADER);
}


// PC???????????????
void setPcParty(void)
{
	pc.status |= CHR_STATUS_PARTY;
}


// PC?????????????????
void delPcParty(void)
{
	pc.status &= (~CHR_STATUS_PARTY);
}


// PC?????�P??????
void setPcWatch(void)
{
	pc.status |= CHR_STATUS_WATCH;
}


// PC?????�P??????????
void delPcWatch(void)
{
	pc.status &= (~CHR_STATUS_WATCH);
}


// ??��??????????
//?��?????????????��?�v???����?
void setPcWalkFlag(void)
{
	if(pc.ptAct == NULL)
		return;

	pc.ptAct->walkFlag = 1;
}

// ??????��??????????
//?��?????????????��?�v???����?
void delPcWalkFlag(void)
{
	if(pc.ptAct == NULL)
		return;

	pc.ptAct->walkFlag = 0;
}


// ��?????����
int checkPcWalkFlag(void)
{
	if(pc.ptAct == NULL)
		return 0;

	return (int)pc.ptAct->walkFlag;
}


// ������????????
void setPcUseMagic(void)
{
	pc.status |= CHR_STATUS_USE_MAGIC;
}


// ������????????
void delPcUseMagic(void)
{
	pc.status &= (~CHR_STATUS_USE_MAGIC);
}


// PC???��??��?
void setPcFukidashi(unsigned int offTime)
{
	CHAREXTRA *ext;

	if(pc.ptAct == NULL)
		return;

	pc.status |= CHR_STATUS_FUKIDASHI;
	ext = (CHAREXTRA *)pc.ptAct->pYobi;
	ext->drawFukidashiTime = offTime + TimeGetTime();
}


// ???????????���q?��?�@�e
void setPcNameColor(int color)
{
	pc.pcNameColor = color;

	if(pc.ptAct == NULL)
		return;

	pc.ptAct->charNameColor = color;
}

#ifdef _ANGEL_SUMMON
void setPcAngel(void)
{
	int find = false;
	for(int i =0; i <CHAR_EQUIPPLACENUM; i++) {
		if(pc.item[i].graNo == 35268)
			find = true;
	}
	if(find)
		pc.status |= CHR_STATUS_ANGEL;
}

void delPcAngel(void)
{
	pc.status &= (~CHR_STATUS_ANGEL);
}
#endif


// PC???�h?��? /////////////////////////////////////////////////////////
void changePcAct(int x, int y, int dir, int action,
					int effectno, int effectparam1, int effectparam2)
{
	switch(action)
	{
		// ??�֢t
		case 0:
#if 0
#if 1
			setPcWarpPoint(x, y);
			setPcDir(dir);
#else
			// ??????????????????
			if(partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0)
			{
				setPcWarpPoint(x, y);
				setPcDir(dir);
			}
#endif
#endif
			setPcAction(ANIM_STAND);
			break;

		// ???
		case 31:
#if 1
			setPcWarpPoint(x, y);
			setPcDir(dir);
#else
			// ??????????????????
			if(partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0)
			{
				setPcWarpPoint(x, y);
				setPcDir(dir);
			}
#endif
			setPcAction(ANIM_STAND);
			break;

		// ��?��?
		case 30:
			setPcDir(dir);
			break;

		// ��?
		case 1:
			setPcAction(ANIM_WALK);
			break;

		// ??
		case 2:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_ATTACK);
			break;

		// ???
		case 3:
			setPcDir(dir);
			setPcAction(ANIM_THROW);
			break;

		// ????
		case 4:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_DAMAGE);
			break;

		// ?��
		case 5:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_DEAD);
			break;

		// ?����?
		case 6:
			break;

		// ????��?
		case 7:
			break;

		// ?????
		case 8:		/* UsedMagic */
			break;

		// �X??
		case 10:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_DEAD);
			break;

		// ??
		case 11:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_SIT);
			break;

		// ��?��?
		case 12:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_HAND);
			break;

		// ??
		case 13:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_HAPPY);
			break;

		// �W?
		case 14:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_ANGRY);
			break;

		// �v??
		case 15:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_SAD);
			break;

		// ???
		case 16:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_GUARD);
			break;

		// ��??�h????
		case 17:
			setPcDir(dir);
			setPcAction(ANIM_WALK);
			break;

		// ????
		case 18:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_NOD);
			break;

		// ??�֢t???????
		case 19:
			//setPcWarpPoint(x, y);
			setPcDir(dir);
			setPcAction(ANIM_STAND);
			break;

		// �P�f�ѩ�
		case 20:
			setPcWarpPoint(x, y);
			setPcDir(dir);
			break;

		// ????�ѩ�
		case 21:
			setPcWarpPoint(x, y);
			setPcDir(dir);
			if(effectno == 1)
			{
				// ???????����
				setPcLeader();
#if 0
				if(pc.ptAct != NULL)
				{
					pc.ptAct->partyGx = -1;
					pc.ptAct->partyGy = -1;
				}
#endif
			}
			else
			{
				// ???????��?
				delPcLeader();
			}
			break;

		// ?�P�ѩ�
		case 22:
			setPcWarpPoint(x, y);
			setPcDir(dir);
			if(effectno == 1)
			{
				setPcWatch();
			}
			else
			{
				delPcWatch();
			}
			break;

		// ???????????���q?��
		case 23:
			setPcNameColor(effectno);
			break;
#ifdef _STREET_VENDOR
		case 41:
			{
				CHAREXTRA *ext;
				if(pc.ptAct == NULL) break;
				ext = (CHAREXTRA *)pc.ptAct->pYobi;
				if(ext->ptStreetVendor){
					DeathAction(ext->ptStreetVendor);
					ext->ptStreetVendor = NULL;
				}
				ext->ptStreetVendor = GetAction(PRIO_CHR,0);
			}
			break;
		case 42:
			{
				CHAREXTRA *ext;
				if(pc.ptAct == NULL) break;
				ext = (CHAREXTRA *)pc.ptAct->pYobi;
				if(ext->ptStreetVendor){
					DeathAction(ext->ptStreetVendor);
					ext->ptStreetVendor = NULL;
				}
			}
			break;
#endif

#ifdef _ANGEL_SUMMON
		case 34:
			setPcWarpPoint(x, y);
			setPcDir(dir);
			if(effectno == 1)
			{
				setPcAngel();
			}
			else
			{
				delPcAngel();
			}
			break;
#endif
#ifdef _ITEM_FIREWORK
		case 51:
			{
				CHAREXTRA *ext;

				if (pc.ptAct == NULL)
					break;
				ext = (CHAREXTRA *)pc.ptAct->pYobi;
				if (ext->pActFirework[0])
				{
					DeathAction(ext->pActFirework[0]);
					ext->pActFirework[0] = NULL;
				}
				if (ext->pActFirework[1])
				{
					DeathAction(ext->pActFirework[1]);
					ext->pActFirework[1] = NULL;
				}
				ext->pActFirework[0] = GetAction(PRIO_CHR, 0);
				ext->pActFirework[0]->damage		= effectno;	// ���� damage ������ type (effectno = type)
				ext->pActFirework[0]->anim_chr_no	= effectparam1;
				ext->pActFirework[0]->anim_no		= ANIM_STAND;
				ext->pActFirework[0]->anim_ang		= 1;
				ext->pActFirework[0]->dispPrio		= DISP_PRIO_RESERVE;
				if (effectno == 3)
				{
					ext->pActFirework[0]->gx		= pc.ptAct->gx + 3;
					ext->pActFirework[0]->gy		= pc.ptAct->gy - 3;
					ext->pActFirework[0]->bufGx[0]	= pc.ptAct->gx;
					ext->pActFirework[0]->bufGy[0]	= pc.ptAct->gy;
				}
				else
				{
					ext->pActFirework[0]->gx		= pc.ptAct->gx;
					ext->pActFirework[0]->gy		= pc.ptAct->gy;
				}
				ext->pActFirework[0]->actNo			= 0;
			}
			break;
#endif

	}
}


// ??�ѩ�???? /////////////////////////////////////////////////////////
void clearPartyParam(void)
{
	int i;

	for(i = 0; i < MAX_PARTY; i++)
	{
		// ???????????????????????
		if(party[i].useFlag != 0)
		{
			if( party[i].id == pc.id)
			{
				// ????�h??????????
				if(party[i].ptAct != NULL)
				{
					party[i].ptAct->bufCount = 0;
				}
				delPcParty();
			}
			else
			{
				// ??????????�\?????�h????????
				if((pc.status & CHR_STATUS_LEADER) != 0
				 && party[i].ptAct != NULL)
				{
					party[i].ptAct->bufCount = 0;
				}
				delCharParty(party[i].ptAct);
			}
		}
		party[i].useFlag = 0;
		party[i].id = 0;
		party[i].ptAct = NULL;
	}

	delPcLeader();
}


// ??�ѩ�?ptAct?NULL?�V????
void clearPtActPartyParam(void)
{
	int i;

	for(i = 0; i < MAX_PARTY; i++)
	{
		party[i].ptAct = NULL;
	}
}





// ???????????�@�e???????? /////////////////////////////
int existCharacterListEntry(int index)
{
	if(index < 0 || index >= MAXCHARACTER)
		return -1;

	if(chartable[index].name[0] != '\0')
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


// ???????�i?��????????
int cmpNameCharacterList(char *name)
{
	int i;

	for(i = 0; i < MAXCHARACTER; i++)
	{
		if(strcmp(name, chartable[i].name) == 0)
			return 1;
	}

	return 0;
}


// ??????????�@�e???????????�t?????????????
int setCharacterList(char *name, char *opt)
{
	int index;

	makeStringFromEscaped(name);
	makeStringFromEscaped(opt);

	index = getIntegerToken(opt, '|', 1);

	if(index < 0 || index >= MAXCHARACTER)
		return -1;

	memset(&chartable[index], 0, sizeof(CHARLISTTABLE));

	if(strlen(name) <= CHAR_NAME_LEN)
	{
		strcpy(chartable[index].name, name);
	}
	else
	{
		strcpy(chartable[index].name, "???");
	}
	chartable[index].faceGraNo	= getIntegerToken(opt, '|',  2);
	chartable[index].level		= getIntegerToken(opt, '|',  3);
	chartable[index].hp			= getIntegerToken(opt, '|',  4);
	chartable[index].str		= getIntegerToken(opt, '|',  5);
	chartable[index].def		= getIntegerToken(opt, '|',  6);
	chartable[index].agi		= getIntegerToken(opt, '|',  7);
	chartable[index].app		= getIntegerToken(opt, '|',  8);
	chartable[index].dp			= getIntegerToken(opt, '|',  9);
	chartable[index].attr[0]	= getIntegerToken(opt, '|', 10)/10;
	chartable[index].attr[1]	= getIntegerToken(opt, '|', 11)/10;
	chartable[index].attr[2]	= getIntegerToken(opt, '|', 12)/10;
	chartable[index].attr[3]	= getIntegerToken(opt, '|', 13)/10;
	chartable[index].login		= getIntegerToken(opt, '|', 14);

	return 0;
}


// ???????��?????
//   int index : ???????0  ~ 7
int resetCharacterList(int index)
{
	if(index < 0 || index >= MAXCHARACTER)
		return -1;

	memset(&chartable[index], 0, sizeof(CHARLISTTABLE));

	return 0;
}


// ?????��? /////////////////////////////////////////////////////////
void getItem(void)
{
	float tmpDir;
	float tmpX, tmpY;
	int dir;
	static unsigned int piSendTime = 0;

	// ?��???????????????��????????
	if(windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMENU
	 || windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMAIN
	 || windowTypeWN == WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN)
		return;

	// �k���I?�K????��??????????????
	if(ABS(nowGx - mouseMapGx) > 1
	 || ABS(nowGy - mouseMapGy) > 1)
		return;

	// �k���I??????��???????????
	if(!checkCharObjPoint(mouseMapGx, mouseMapGy,
		CHAROBJ_TYPE_NPC|CHAROBJ_TYPE_ITEM|CHAROBJ_TYPE_MONEY))
//		CHAROBJ_TYPE_ITEM|CHAROBJ_TYPE_MONEY))
		return;

	// ???????��??��??
	tmpX = (float)(mouseMapGx - nowGx);
	tmpY = (float)(mouseMapGy - nowGy);
	tmpDir = Atan(tmpX, tmpY) + 22.5F - 45.0F*3;
	AdjustDir(&tmpDir);
	dir = (int)(tmpDir/45);

	// ?��?��
	if(piSendTime+FIELD_BTN_PUSH_WAIT < TimeGetTime())
	{
		// ??????
		if(bNewServer) {
			lssproto_PI_send(sockfd, nowGx, nowGy, dir);
		}
		else
			old_lssproto_PI_send(sockfd, nowGx, nowGy, dir);
		piSendTime = TimeGetTime();
	}
}

#ifdef __TALK_TO_NPC

bool TalkToNPC(void)
{
	float tmpDir;
	float tmpX, tmpY;
	int dir;
	static unsigned int talkSendTime = 0;
	if(windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMENU
	 || windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMAIN
	 || windowTypeWN == WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN)
		return false;

	if(nowVx != 0 || nowVy != 0)
		return false;
	if(ABS(nowGx - mouseMapGx) > 2
	 || ABS(nowGy - mouseMapGy) > 2)
		return false;
	if(!checkCharObjPoint(mouseMapGx, mouseMapGy, CHAROBJ_TYPE_NPC))
		return false;
	tmpX = (float)(mouseMapGx - nowGx);
	tmpY = (float)(mouseMapGy - nowGy);
	tmpDir = Atan(tmpX, tmpY) + 22.5F;
	AdjustDir(&tmpDir);
	dir = (int)(tmpDir/45);
	if(talkSendTime+FIELD_BTN_PUSH_WAIT < TimeGetTime()){
		if(pc.ptAct->anim_ang != dir){
			char dir2[2];
			setPcDir(dir);

			dir2[0] = cnvServDir(dir, 1);
			dir2[1] = '\0';

			walkSendForServer(nowGx, nowGy, dir2);
		}
		//lookAtAround();
		
		char dest[1024], m[1024];
		extern STR_BUFFER chatRegistryStr[];

		if(chatRegistryStr[7].cnt > 0)
			makeEscapeString(chatRegistryStr[7].buffer, dest, sizeof(dest));
		else
			makeEscapeString("hi", dest, sizeof(dest));
		sprintf(m, "P|%s", dest);
		if(bNewServer)
			lssproto_TK_send(sockfd, nowGx, nowGy, m, MyChatBuffer.color, NowMaxVoice);
		else
			old_lssproto_TK_send(sockfd, nowGx, nowGy, m, MyChatBuffer.color, NowMaxVoice);
		
		talkSendTime = TimeGetTime();
	}
	return true;
}

#endif

// ?????�V?�v? /////////////////////////////////////////////////////
void swapItem(int from, int to)
{
	if(from < 0 || to < 0)
		return;
//	swapShort(&pc.item[from].useFlag, &pc.item[to].useFlag);
	swap(pc.item[from].useFlag, pc.item[to].useFlag);
//	swapString(pc.item[from].name, pc.item[to].name);
	swap(pc.item[from].name, pc.item[to].name);
//	swapString(pc.item[from].name2, pc.item[to].name2);
	swap(pc.item[from].name2, pc.item[to].name2);
//	swapString(pc.item[from].memo, pc.item[to].memo);
	swap(pc.item[from].memo, pc.item[to].memo);
//	swapInteger(&pc.item[from].color, &pc.item[to].color);
	swap(pc.item[from].color, pc.item[to].color);
//	swapInteger(&pc.item[from].graNo, &pc.item[to].graNo);
	swap(pc.item[from].graNo, pc.item[to].graNo);
//	swapInteger(&pc.item[from].level, &pc.item[to].level);
	swap(pc.item[from].level, pc.item[to].level);
//	swapShort(&pc.item[from].field, &pc.item[to].field);
	swap(pc.item[from].field, pc.item[to].field);
//	swapShort(&pc.item[from].target, &pc.item[to].target);
	swap(pc.item[from].target, pc.item[to].target);
//	swapShort(&pc.item[from].deadTargetFlag, &pc.item[to].deadTargetFlag);
	swap(pc.item[from].deadTargetFlag, pc.item[to].deadTargetFlag);
//	swapShort(&pc.item[from].sendFlag, &pc.item[to].sendFlag);
	swap(pc.item[from].sendFlag, pc.item[to].sendFlag);
//	swapString(pc.item[from].damage, pc.item[to].damage);
	swap(pc.item[from].damage, pc.item[to].damage);
#ifdef _ITEM_PILENUMS
//	swapInteger(&pc.item[from].pile, &pc.item[to].pile);
	swap(pc.item[from].pile, pc.item[to].pile);
#endif
#ifdef _PET_ITEM
	swap(pc.item[from].type, pc.item[to].type);
#endif

}

// ��?�k?�V??
/*void swapInteger(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}*/


/*void swapShort(short *a, short *b)
{
	short tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}*/


// ��???�V?�v??????255��????��???
/*void swapString(char *a, char *b)
{
	char tmp[256];

	if(strlen(a) > 255 || strlen(b) > 255)
		return;

	strcpy(tmp, a);
	strcpy(a, b);
	strcpy(b, tmp);
}
*/

// ?��????????? /////////////////////////////////////////////////
bool lookAtAround(void)
{
	float tmpDir;
	float tmpX, tmpY;
	int dir;
	static unsigned int lSendTime = 0;

	// ?��???????????????????????
	if(windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMENU
	 || windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMAIN
	 || windowTypeWN == WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN)
		return false;

	// �k���I?�K????��??????????????
	if(ABS(nowGx - mouseMapGx) > 2
	 || ABS(nowGy - mouseMapGy) > 2)
		return false;

	// �k���I??????��???????????
	if(!checkCharObjPoint(mouseMapGx, mouseMapGy, CHAROBJ_TYPE_LOOKAT))
		return false;

	// ???????��??��??
	tmpX = (float)(mouseMapGx - nowGx);
	tmpY = (float)(mouseMapGy - nowGy);
	if(tmpX == 0 && tmpY == 0)	// ?���Y?��?����?????????��??
		return false;

	tmpDir = Atan(tmpX, tmpY) + 22.5F - 45.0F*3;
	AdjustDir(&tmpDir);
	dir = (int)(tmpDir/45);

	// ?��?��
	if(lSendTime+FIELD_BTN_PUSH_WAIT < TimeGetTime())
	{
		// ??????
		if(bNewServer)
			lssproto_L_send(sockfd, dir);
		else
			old_lssproto_L_send(sockfd, dir);
		lSendTime = TimeGetTime();
	}

	return true;
}





// ????????? /////////////////////////////////////////////////////
bool addressBookFlag = false;
ACTION *ptActAddressBookWin = NULL;
int addressBookX = 16;
int addressBookY = 16;
int addressBookPage = 0;
ACTION *ptActAddressBookChar[MAX_ADR_BOOK_COUNT];

// �k��???��?????
int addressBookSelectStrItem[1+3*MAX_ADR_BOOK_COUNT];
// 0 ... ��??
// n*3+1 ... ?��
// n*3+2 ... ����
// n*3+3 ... ??


bool addressBookSendMsgFlag = false;
ACTION *ptActAddressBookSendMsgWin = NULL;


void addressBookWindow1(void);


// ???????????
void initAddressBook(void)
{
	int i;

	addressBookFlag = false;
	ptActAddressBookWin = NULL;
	memset(&ptActAddressBookChar, 0, sizeof(ptActAddressBookChar));
	addressBookPage = 0;
	for(i = 0; i < sizeof(addressBookSelectStrItem)/sizeof(int); i++)
	{
		addressBookSelectStrItem[i] = -2;
	}

	addressBookSendMsgFlag = false;
	ptActAddressBookSendMsgWin = NULL;
}


// ?????????
void addressBookProc(void)
{
	int i;
	int no;

	// ??????????
	if(addressBookFlag
	 && ptActAddressBookWin == NULL)
	{
		// ??????????????
		ptActAddressBookWin = MakeWindowDisp(addressBookX, addressBookY, 4, 8, NULL, 1);
		// ?��?????????
		for(i = 0; i < MAX_ADR_BOOK_COUNT; i++)
		{
			no = addressBookPage*MAX_ADR_BOOK_COUNT+i;

			ptActAddressBookChar[i] = GetAction(PRIO_CHR, 0);
			if(ptActAddressBookChar[i] != NULL)
			{
				ptActAddressBookChar[i]->anim_chr_no = addressBook[no].graNo;
				ptActAddressBookChar[i]->atr = ACT_ATR_HIDE;
				ptActAddressBookChar[i]->anim_no = ANIM_STAND;
				ptActAddressBookChar[i]->dispPrio = DISP_PRIO_ITEM;
				ptActAddressBookChar[i]->x = addressBookX + 40;
				ptActAddressBookChar[i]->y = addressBookY + i * 110 + 80;
				pattern(ptActAddressBookChar[i], ANM_NOMAL_SPD, ANM_LOOP);
			}
		}
	}
	// ????????��??
	else
	if(!addressBookFlag
	 && ptActAddressBookWin != NULL)
	{
		DeathAction(ptActAddressBookWin);
		ptActAddressBookWin = NULL;
		for(i = 0; i < MAX_ADR_BOOK_COUNT; i++)
		{
			if(ptActAddressBookChar[i] != NULL)
			{
				DeathAction(ptActAddressBookChar[i]);
				ptActAddressBookChar[i] = NULL;
			}
		}
	}
	// ???????����
	else
	if(addressBookFlag
	 && ptActAddressBookWin != NULL)
	{
		addressBookWindow1();
	}
}


// ???????����
void addressBookWindow1(void)
{
	int mode = 0;
	int i;
	int no;
	char msg[256];

	// ��???�k��??????????
	// ???????????????????
	if(mouse.onceState & MOUSE_LEFT_CRICK)
	{
		for(i = 0; i < sizeof(addressBookSelectStrItem)/sizeof(int); i++)
		{
			if(addressBookSelectStrItem[i] == HitFontNo)
			{
				mode = i + 1;
				break;
			}
		}
	}

	// ��????????????��??
	if(mode == 1)
	{
		addressBookFlag = false;
		return;
	}
	// ?????????????????????
	else
	if((mode-2) < MAX_ADR_BOOK_COUNT*3
	 && ((mode-2) % 3) == 2)
	{
		if(bNewServer)
			lssproto_DAB_send(sockfd, addressBookPage*MAX_ADR_BOOK_COUNT+((mode-2)/3));
		else
			old_lssproto_DAB_send(sockfd, addressBookPage*MAX_ADR_BOOK_COUNT+((mode-2)/3));
	}

	// ???????????��?????��?����
	if(ptActAddressBookWin->hp >= 1)
	{
		for(i = 0; i < MAX_ADR_BOOK_COUNT; i++)
		{
			no = addressBookPage*MAX_ADR_BOOK_COUNT+i;

			if(ptActAddressBookChar[i] != NULL)
			{
				// ���q?????????�V???????
				if(strlen(addressBook[no].name) > 0)
				{
					// ??????�k?????�ѩ�����
					// ???����
					ptActAddressBookChar[i]->atr &= (~ACT_ATR_HIDE);
					ptActAddressBookChar[i]->anim_chr_no = addressBook[no].graNo;
					// ���q����
					sprintf(msg, "NAME. %s", addressBook[no].name);
					StockFontBuffer(addressBookX + 68, addressBookY + i * 110 + 28,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0);
				
					// ???����
					sprintf(msg, "LV. %d", addressBook[no].level);
					StockFontBuffer(addressBookX + 68, addressBookY + i * 110 + 46,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0);
					// ?????����
					if(addressBook[i].onlineFlag != 0)
					{
						StockFontBuffer(addressBookX + 130, addressBookY + i * 110 + 46,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, "����", 0);
					}
					else
					{
						StockFontBuffer(addressBookX + 130, addressBookY + i * 110 + 46,
							FONT_PRIO_FRONT, FONT_PAL_GRAY, "����", 0);
					}
					// ?��???����
					addressBookSelectStrItem[i*3+1] =
						StockFontBuffer(addressBookX + 68, addressBookY + i * 110 + 64,
							FONT_PRIO_FRONT, FONT_PAL_YELLOW, "����", 2);
					// ����???����
					addressBookSelectStrItem[i*3+2] =
						StockFontBuffer(addressBookX + 112, addressBookY + i * 110 + 64,
							FONT_PRIO_FRONT, FONT_PAL_YELLOW, "����", 2);
					// ?????����
					addressBookSelectStrItem[i*3+3] =
						StockFontBuffer(addressBookX + 156, addressBookY + i * 110 + 64,
							FONT_PRIO_FRONT, FONT_PAL_YELLOW, "ɾ��", 2);
				}
				else
				{
					ptActAddressBookChar[i]->atr |= ACT_ATR_HIDE;
	
					ptActAddressBookChar[i]->anim_chr_no = 0;
				}
			}
		}

		addressBookSelectStrItem[0] =
			StockFontBuffer(addressBookX + 56, addressBookY + 352,
				FONT_PRIO_FRONT, FONT_PAL_YELLOW, "�ر�", 2);
	}
}

int CHAR_getMaxHaveGold()
{
	int MaxGold;
#ifdef _FIX_MAXGOLD				// WON ADD ���������Ǯ����
	int trans = pc.transmigration;
	MaxGold = 1000000 + trans*(1800000);
#else
	MaxGold = MAX_GOLD;
#endif
	return MaxGold;
}