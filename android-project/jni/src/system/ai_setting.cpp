
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/pc.h"
#include "../systeminc/menu.h"
#include "../systeminc/t_music.h"
#include "../systeminc/battlemenu.h"
#include "../systeminc/netmain.h"
#include "../systeminc/lssproto_cli.h"
#include "../NewProto/protocol.h"
#include "../wgs/descrypt.h"
#include"../systeminc/login.h"

#define AI_FILE_NAME "/sdcard/jerrysa/AISetting.dat"
#define TARGET_MYSELF		1
#define TARGET_MYPET		2
#define TARGET_MYSIDE		3
#define TARGET_OTHERONE		4
#define TARGET_OTHERSIDE	5
#define TARGET_ALL			6

#define TARGET_1			7
#define TARGET_2			8
#define TARGET_3			9
#define TARGET_4			10
#define TARGET_5			11

#define TARGET_ONE_ROW	12

#ifndef _ATTACK_AI
int AI_method[4];
int	AI_target[4];
int AI_limit[4];
#else
int AI_method[5];
int	AI_target[5];
int AI_limit[5];
void AI_SetUpAttackMethod(int which);
void AI_SetDownAttackMethod(int which);
#endif

#ifdef _AI_OTHER
ACTION *pAIOtherFunctionWnd = NULL;
int AI_Other_State = 0;
int AI_OtherSetting[1];
#endif
#ifdef _AI_NEW_FIRST
bool bFirstFlag = false;	// true:use skill false:use magic
#endif
int AI_State = 0;
int AI_First = 1;
char *AI_Choosen = NULL;
static int AICnt = 0;
ACTION *pAISettingWnd = NULL;

extern int IdEncryptionTbl[];

#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif

#ifdef _ATTACK_AI
int g_iPSindex = -1;
bool g_bUsePS = false;
#endif

bool AI_Load( char *user)
{
	char id[16];
	char id2[16];
	FILE	*fp;
	bool ret = false;
	int i;
	memcpy( id, user, 16);
	AI = AI_NONE;
	AI_method[0] = MAX_MAGIC;
	AI_target[0] = TARGET_MYSELF;
	AI_limit[0] = 30;
#ifndef _ATTACK_AI
	AI_method[1] = MAX_MAGIC;
#else
	AI_method[1] = -1;
#endif
	AI_target[1] = TARGET_MYSELF;
	AI_limit[1] = 30;
	AI_method[2] = -1;
	AI_method[3] = MAX_MAGIC;
	AI_target[3] = TARGET_MYSELF;
#ifdef _ATTACK_AI
	AI_method[4] = MAX_PROFESSION_SKILL;
	AI_target[4] = TARGET_OTHERONE;
#endif
#ifdef _AI_OTHER
	AI_OtherSetting[0] = 0;
#endif
#ifdef _AI_NEW_FIRST
	bFirstFlag = false;
#endif
	if( (fp=fopen( AI_FILE_NAME, "r+b")) == NULL){
		if( (fp=fopen( AI_FILE_NAME, "wb")) != NULL){
			for( i = 0; i < 16; i++)
				id[i] ^= IdEncryptionTbl[16-i];
			fwrite( id, sizeof(id), 1, fp);
			fwrite( &AI, sizeof(AI), 1, fp);
			fwrite( AI_method, sizeof(AI_method), 1, fp);
			fwrite( AI_target, sizeof(AI_target), 1, fp);
			fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
			fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
			fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
			fclose( fp);
		}
		return ret;
	}
	while( 1 ){
		if( fread( id2, sizeof(id2), 1, fp) < 1){
			for( i = 0 ; i < 16 ; i++)
				id[ i ] ^= IdEncryptionTbl[16-i];
			fwrite( id, sizeof(id), 1, fp);
			fwrite( &AI, sizeof(AI), 1, fp);
			fwrite( AI_method, sizeof(AI_method), 1, fp);
			fwrite( AI_target, sizeof(AI_target), 1, fp);
			fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
			fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
			fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
			break;
		}
		for( i = 0 ; i < 16 ; i++)
			id2[i] ^= IdEncryptionTbl[16-i];
		id2[15] &= 0xf;
		id2[id2[15]] = NULL;
		if( strcmp( id, id2) != 0){
			if( fseek( fp, sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
				+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
				+sizeof(bFirstFlag)
#endif
				,SEEK_CUR) != 0)
			{
				fwrite( &AI, sizeof(AI), 1, fp);
				fwrite( AI_method, sizeof(AI_method), 1, fp);
				fwrite( AI_target, sizeof(AI_target), 1, fp);
				fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
				fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
				fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
				break;
			}
		}else{
			if( fread( &AI, sizeof(AI), 1, fp)<1 ||
#ifndef _ATTACK_AI
				fread( AI_method, sizeof(int), 4, fp)<4 ||
				fread( AI_target, sizeof(int), 4, fp)<4 ||
				fread( AI_limit, sizeof(int), 4, fp)<4
#else
				fread( AI_method, sizeof(int), 5, fp)<5 ||
				fread( AI_target, sizeof(int), 5, fp)<5 ||
				fread( AI_limit, sizeof(int), 5, fp)<5
	#ifdef _AI_OTHER
				|| fread(AI_OtherSetting,sizeof(int),1,fp) < 1
	#endif
	#ifdef _AI_NEW_FIRST
				|| fread(&bFirstFlag,sizeof(bool),1,fp) < 1
	#endif
#endif
			){
				fwrite( &AI, sizeof(AI), 1, fp);
				fwrite( AI_method, sizeof(AI_method), 1, fp);
				fwrite( AI_target, sizeof(AI_target), 1, fp);
				fwrite( AI_limit, sizeof(AI_limit), 1, fp);
#ifdef _AI_OTHER
				fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp);
#endif
#ifdef _AI_NEW_FIRST
				fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp);
#endif
				break;
			}
			ret = true;
			break;
		}
		AICnt++;
	}
	fclose( fp );
#ifdef _AI_NEW_FIRST
	// ?????
	if(pc.profession_class == 0) bFirstFlag = false;
#endif
	return ret;
}

bool AI_ReLoad()
{
	FILE	*fp;
	bool	ret = false;
	if( fp=fopen( AI_FILE_NAME, "rb")){
		if( 0 == fseek( fp, (16+sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
			+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
			+sizeof(bFirstFlag)
#endif
			)*AICnt+16, SEEK_SET))
		{
			if( fread( &AI, sizeof(AI), 1, fp)>=1 &&
				fread( AI_method, sizeof(AI_method), 1, fp)>=1 &&
				fread( AI_target, sizeof(AI_target), 1, fp)>=1 &&
				fread( AI_limit, sizeof(AI_limit), 1, fp)>=1
#ifdef _AI_OTHER
				&& fread(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp) >= 1
#endif
#ifdef _AI_NEW_FIRST
				&& fread(&bFirstFlag,sizeof(bFirstFlag),1,fp) >= 1
#endif
				) ret = true;
		}
		fclose( fp);
	}
	return ret;
}

bool AI_Save()
{
	FILE	*fp;
	bool	ret = false;
	if( fp=fopen( AI_FILE_NAME, "r+b")){
		if( 0 == fseek( fp, (16+sizeof(AI)+sizeof(AI_method)+sizeof(AI_target)+sizeof(AI_limit)
#ifdef _AI_OTHER
			+sizeof(AI_OtherSetting)
#endif
#ifdef _AI_NEW_FIRST
			+sizeof(bFirstFlag)
#endif
			)*AICnt+16, SEEK_SET))
		{
			if( fwrite( &AI, sizeof(AI), 1, fp)>=1 &&
				fwrite( AI_method, sizeof(AI_method), 1, fp)>=1 &&
				fwrite( AI_target, sizeof(AI_target), 1, fp)>=1 &&
				fwrite( AI_limit, sizeof(AI_limit), 1, fp)>=1
#ifdef _AI_OTHER
				&& fwrite(AI_OtherSetting,sizeof(AI_OtherSetting),1,fp) >= 1
#endif
#ifdef _AI_NEW_FIRST
				&& fwrite(&bFirstFlag,sizeof(bFirstFlag),1,fp) >= 1
#endif
				)	ret = true;
			fclose( fp);
		}
	}
	return ret;
}

#ifdef _ATTACK_AI
int AI_CheckSkillSetting(int methodkind,int method,int UpDown)
{
	int ret = 1;
	if(methodkind == 4 && method == MAX_PROFESSION_SKILL){
		AI_target[methodkind] = TARGET_OTHERONE;
		return ret;
	}
	else if(methodkind == 2 && method == MAX_PROFESSION_SKILL){
		AI_method[methodkind] = -1;
		return ret;
	}

	if(profession_skill[method].costmp <= 0 
		&& profession_skill[method].skillId != 11 
		&& profession_skill[method].skillId != 75) return 0;

	if(AI_target[methodkind] == 0) AI_target[methodkind] = TARGET_ONE_ROW;


	switch(profession_skill[method].skillId){
		case 1://????			?е?????????
		case 2://???????		?ж?????????
		case 3://???????  	?ж?????????
		case 4://??????			?ж?????????
		case 5://??????			?е?????????
		case 6://??????			?е?????????
		case 7://??????			?ж?????????
		case 8://??????			?ж?????????
		case 9://??????			?ж?????????
		case 10://?????		?е?????????
		case 12://??????		?е?????????		
		case 13://?????		?ж?????????
		case 14://??????		?е?????????
		case 22://????			?е?????????
		case 23://????????	?е?????????
		case 24://??????	?е?????????
		case 38://???			?е?????????
		case 39://??????	?е?????????
		case 40://????????	?е?????????
		case 41://????????	?ж?????????
		case 42://???????	?ж?????????
		case 46://????????	?е?????????
		case 48://???????	?е?????????
		case 49://??????	?е?????????
		case 50://????????	?е?????????
		case 51://??????	?е?????????
		case 54://???﹥??	?е?????????
		case 72://??????  ???????
			if(AI_target[methodkind] != TARGET_OTHERONE) AI_target[methodkind] = TARGET_OTHERONE;
			break;
		case 15://????		????????
		case 16://?????		????????
		case 17://??????		????????
			// down
			if(UpDown){
				if(AI_target[methodkind] == TARGET_MYPET || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_OTHERONE;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_1;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_MYSELF;
			}
			// up
			else{
				if(AI_target[methodkind] == TARGET_MYPET || AI_target[methodkind] == TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYSELF;
				if(AI_target[methodkind] == TARGET_OTHERSIDE || AI_target[methodkind] == TARGET_ALL) AI_target[methodkind] = TARGET_OTHERONE;
				if(AI_target[methodkind] == TARGET_ONE_ROW) AI_target[methodkind] = TARGET_5;
			}
			break;
		case 21://???λ?λ	???????
		case 35://????????	???????
		case 36://???????	???????
		case 37://?????	???????
		case 47://????			???????
		case 53://??			???????
		case 59://?翹			???????
		case 60://??			???????
		case 61://????			???????
		case 66://???????	???????
			if(AI_target[methodkind] != TARGET_MYSELF) AI_target[methodkind] = TARGET_MYSELF;
			break;
		case 34://???????	????????


		case 68://????		??????
		case 69://????		??????
		case 70://????		??????
		case 71://????		??????
			if(AI_target[methodkind] != TARGET_MYSIDE) AI_target[methodkind] = TARGET_MYSIDE;
			break;
		case 52://??			?е?????????(?????)
		case 56://???????	????????
		case 62://????			?е?????????(?????)
			if(AI_target[methodkind] != TARGET_OTHERONE) AI_target[methodkind] = TARGET_OTHERONE;
			break;
		case 57://???????	????????
			if(AI_target[methodkind] != TARGET_MYPET) AI_target[methodkind] = TARGET_MYPET;
			break;
		case 11://???????	????????
		case 18://????????	??????
		case 19://????????	??????
		case 20://????????	??????
		case 25://???			??????
		case 26://????		??????
		case 27://?????		??????
		case 28://?????		??????
		case 33://?????	??????
		case 43://??????		??????
		case 44://??????	????????
		case 45://??????	????????
		case 55://???			?
		case 58://???????	?
		case 29://?????		??????
		case 30://?????????		??????
		case 31://???????		??????
		case 32://????????		??????
		default:
			ret = 0;
			break;
	}
	return ret;
}
#endif

void AI_CheckSetting()
{
	char *str;
	int method;
	if( AI)
		AI = AI_SELECT;
	int i;
#ifndef _ATTACK_AI
	for( i = 0; i < 4; i++){
#else
	for( i = 0; i < 5; i++){
#endif
		if( AI_limit[i]>100 || AI_limit[i]<0)
			AI_limit[i] = 30;
		else if( AI_limit[i]%5)
			AI_limit[i] = 30;
	}
#ifdef _ATTACK_AI
	// ??????????? ??????? ?????λ??
	i = 0;
	g_iPSindex = -1;
	g_bUsePS = false;
	while(1){
		if(profession_skill[i].useFlag == 1 && profession_skill[i].skillId == 11){
			g_iPSindex = i;
			g_bUsePS = true;
		}
		if(++i >= MAX_PROFESSION_SKILL) break;
	}
#endif
#ifdef _AI_OTHER
	if(AI_OtherSetting[0] < 0) AI_OtherSetting[0] = 0;
#endif
	method = AI_method[0];
	if( method<MAX_MAGIC && method>=0){//??????
		if( magic[method].useFlag && (str=strstr( magic[method].memo, "?????")) && strstr( str, "???")){//???????????????
			switch( magic[method].target){
			case MAGIC_TARGET_MYSELF:
				AI_target[0] = TARGET_MYSELF;			//???
				break;
			case MAGIC_TARGET_ALLMYSIDE:
			case MAGIC_TARGET_WHOLEOTHERSIDE:
				AI_target[0] = TARGET_MYSIDE;			//??????
				break;
			case MAGIC_TARGET_OTHER:
				if( AI_target[0]!=TARGET_MYSELF && AI_target[0]!=TARGET_MYPET &&
					(TARGET_1>AI_target[0] || AI_target[0]>TARGET_5))//????????????
					AI_target[0] = TARGET_MYSELF;	//???
				break;
			default:
//			case MAGIC_TARGET_ALLOTHERSIDE:				//????????????????
//			case MAGIC_TARGET_ALL:
				AI_method[0] = MAX_MAGIC;			//?????????
				AI_target[0] = TARGET_MYSELF;
				break;
			}
		}else{//???????????????
			AI_method[0] = MAX_MAGIC;
			AI_target[0] = TARGET_MYSELF;
		}
	}else{//????????????
		AI_method[0] = MAX_MAGIC;
		if( AI_target[0]!=TARGET_MYSELF && AI_target[0]!=TARGET_MYPET &&
			(TARGET_1>AI_target[0] || AI_target[0]>TARGET_5)){//????????????
			AI_target[0] = TARGET_MYSELF;
		}
	}
#ifndef _ATTACK_AI
	AI_method[1] = MAX_MAGIC;
#else
	if(!g_bUsePS) AI_method[1] = -1;
#endif
	AI_target[1] = TARGET_MYSELF;

	method = AI_method[2];
#ifdef _AI_NEW_FIRST
	// ??ξ???:??????
	if(bFirstFlag == false){
#endif
		if( method<MAX_MAGIC && method>=0 && magic[method].useFlag){//?????ξ???
			switch( magic[method].target){
			case MAGIC_TARGET_MYSELF:
				if( AI_target[2] != TARGET_MYSELF)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALLMYSIDE:
				if( AI_target[2] != TARGET_MYSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_WHOLEOTHERSIDE:
				if( AI_target[2]!=TARGET_MYSIDE && AI_target[2]!=TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_OTHER:
				if( AI_target[2]!=TARGET_MYSELF && AI_target[2]!=TARGET_MYPET && AI_target[2]!=TARGET_OTHERONE &&
					(TARGET_1>AI_target[2] || AI_target[2]>TARGET_5))
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALLOTHERSIDE:
				if( AI_target[2] != TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
			case MAGIC_TARGET_ALL:
				if( AI_target[2] != TARGET_ALL)
					AI_method[2] = -1;
				break;
#ifdef __ATTACK_MAGIC
				// ?о?????
			case MAGIC_TARGET_SINGLE:
				if( AI_target[2] != TARGET_OTHERONE)
					AI_method[2] = -1;
				break;
				// ?о??????
			case MAGIC_TARGET_ONE_ROW:
				if( AI_target[2]!=TARGET_ONE_ROW)
					AI_method[2] = -1;
				break;
				// ?о????
			case MAGIC_TARGET_ALL_ROWS:
				if( AI_target[2] != TARGET_OTHERSIDE)
					AI_method[2] = -1;
				break;
#endif
			default:
				AI_method[2] = -1;
				break;
			}
		}else{//??????????
			AI_method[2] = -1;
		}
#ifdef _AI_NEW_FIRST
	}
	// ??ξ???:??ü???
	else{
		if(pc.profession_class != 0 && // ????
			(method < MAX_PROFESSION_SKILL && method >= 0) &&
			AI_CheckSkillSetting(2,method,0) == 0) AI_method[2] = -1;
	}
#endif
	method = AI_method[3];
	if( method<MAX_MAGIC && method>=0){//??????
		if( !magic[method].useFlag || !(str=strstr( magic[method].memo, "??????")) || !strstr( str, "???")){//????????????????
			AI_method[3] = MAX_MAGIC;
		}
	}else{//??????????????
		AI_method[3] = MAX_MAGIC;
	}
	if( AI_target[3]!=TARGET_MYSIDE && AI_target[3]!=TARGET_MYPET && (TARGET_1>AI_target[3] || AI_target[3]>TARGET_5))
		AI_target[3] = TARGET_MYPET;

#ifdef _ATTACK_AI
	method = AI_method[4];
	if(pc.profession_class != 0 && // ????
		(method < MAX_PROFESSION_SKILL && method >= 0)){// ?????????
		if(AI_CheckSkillSetting(4,method,0) == 0){
			AI_method[4] = MAX_PROFESSION_SKILL;
			AI_target[4] = TARGET_OTHERONE;
		}
	}
	else{// ???????
		AI_method[4] = MAX_PROFESSION_SKILL;
		AI_target[4] = TARGET_OTHERONE;
	}
#endif
	AI_Save();
}

void AI_Init()
{
	char id[32];
	AI = AI_NONE;
	PauseAI = 0;
	AICnt = 0;
	extern char szUser[];
	memcpy( id, szUser, 32);
	ecb_crypt( "f;encor1c", id, 32, DES_DECRYPT);
	id[15] = strlen(id);
	if( AI_Load( id))
		AI_CheckSetting();
}

void AI_SetUpTarget( int which)
{
	int method = AI_method[which];
#ifdef _ATTACK_AI
	if(which == 4
#ifdef _AI_NEW_FIRST
		|| (which == 2 && bFirstFlag == true)
#endif
	){
		AI_target[which]--;
		AI_CheckSkillSetting(which,method,0);
	}else
#endif
	if( method >= MAX_MAGIC){
		if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
			AI_target[which]--;
			if( AI_target[which] < TARGET_1) AI_target[which] = TARGET_MYSELF;
		}
		else if( AI_target[which] == TARGET_MYPET)	AI_target[which] = TARGET_5;
		else AI_target[which] = TARGET_MYPET;
	}else if( method >= 0)
	{
		switch( magic[method].target)
		{
		case MAGIC_TARGET_MYSELF:
			AI_target[which] = TARGET_MYSELF;			//???
			break;
		case MAGIC_TARGET_ALLMYSIDE:
			AI_target[which] = TARGET_MYSIDE;			//??????
			break;
		case MAGIC_TARGET_WHOLEOTHERSIDE:
			if( which !=0)
			{
				if( AI_target[which] == TARGET_MYSIDE)
					AI_target[which] = TARGET_OTHERSIDE;	//?з????
				else
					AI_target[which] = TARGET_MYSIDE;		//??????
			}
			break;
		case MAGIC_TARGET_OTHER:
			if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5)
			{
				AI_target[which]--;
				if( AI_target[which] < TARGET_1)
					AI_target[which] = TARGET_MYSELF;
			}else if( AI_target[which] == TARGET_MYPET)
			{
				if( which != 0)
					AI_target[which] = TARGET_OTHERONE;
				else
					AI_target[which] = TARGET_5;
			}else if( AI_target[which] == TARGET_OTHERONE)
			{
				AI_target[which] = TARGET_5;
			}else
			{
				AI_target[which] = TARGET_MYPET;
			}
/*			if( AI_target[which] == TARGET_MYSELF)
				AI_target[which] = TARGET_MYPET;		//????
			else if( AI_target[which] == TARGET_MYPET){
				if( which == 2)
					AI_target[which] = TARGET_OTHERONE;	//?з?
				else
					AI_target[which] = TARGET_MYSELF;	//???
			}else// if( AI_target[which] == TARGET_OTHERONE)
				AI_target[which] = TARGET_MYSELF;		//???*/
			break;
		case MAGIC_TARGET_ALLOTHERSIDE:
			AI_target[which] = TARGET_OTHERSIDE;		//?з????
			break;
		case MAGIC_TARGET_ALL:
			AI_target[which] = TARGET_ALL;				//???
			break;
#ifdef __ATTACK_MAGIC
		// ?о?????
		case MAGIC_TARGET_SINGLE:
			AI_target[which] = TARGET_OTHERONE;
			break;
		// ?о??????
		case MAGIC_TARGET_ONE_ROW:
			AI_target[which] = TARGET_ONE_ROW;
			break;
		// ?о????
		case MAGIC_TARGET_ALL_ROWS:
			AI_target[which] = TARGET_OTHERSIDE;
			break;
#endif
		}
//		if( magic[method].target == MAGIC_TARGET_OTHER)
//			AI_target[which] ^= 1;
	}
}

void AI_SetDownTarget( int which)
{
	int method = AI_method[which];
#ifdef _ATTACK_AI
	if(which == 4
#ifdef _AI_NEW_FIRST
		|| (which == 2 && bFirstFlag == true)
#endif
	){
		AI_target[which]++;
		AI_CheckSkillSetting(which,method,1);
	}else
#endif
	if( method >= MAX_MAGIC){
		if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
			AI_target[which]++;
			if( AI_target[which] > TARGET_5)
				AI_target[which] = TARGET_MYPET;
		}else if( AI_target[which] == TARGET_MYPET){
			AI_target[which] = TARGET_MYSELF;
		}else{
			AI_target[which] = TARGET_1;
		}
/*		if( AI_target[which] == TARGET_MYSELF){
			if( which == 2)
				AI_target[which] = TARGET_OTHERONE;		//?з?
			else
				AI_target[which] = TARGET_MYPET;		//????
		}else if( AI_target[which] == TARGET_OTHERONE)
			AI_target[which] = TARGET_MYPET;			//????
		else// if( AI_target[which] == TARGET_MYPET)
			AI_target[which] = TARGET_MYSELF;			//???*/
	}else if( method >= 0){
		switch( magic[method].target){
		case MAGIC_TARGET_MYSELF:
			AI_target[which] = TARGET_MYSELF;			//???
			break;
		case MAGIC_TARGET_ALLMYSIDE:
			AI_target[which] = TARGET_MYSIDE;			//??????
			break;
		case MAGIC_TARGET_WHOLEOTHERSIDE:
			if( which != 0){
				if( AI_target[which] == TARGET_MYSIDE)
					AI_target[which] = TARGET_OTHERSIDE;	//?з????
				else
					AI_target[which] = TARGET_MYSIDE;		//??????
			}
			break;
		case MAGIC_TARGET_OTHER:
			if( TARGET_1<=AI_target[which] && AI_target[which]<=TARGET_5){
				AI_target[which]++;
				if( AI_target[which] > TARGET_5){
					if( which != 0)
						AI_target[which] = TARGET_OTHERONE;
					else
						AI_target[which] = TARGET_MYPET;
				}
			}else if( AI_target[which] == TARGET_OTHERONE){
				AI_target[which] = TARGET_MYPET;
			}else if( AI_target[which] == TARGET_MYPET){
				AI_target[which] = TARGET_MYSELF;
			}else{
				AI_target[which] = TARGET_1;
			}
/*			if( AI_target[which] == TARGET_MYSELF){
				if( which == 2)
					AI_target[which] = TARGET_OTHERONE;	//?з?
				else
					AI_target[which] = TARGET_MYPET;	//????
			}else if( AI_target[which] == TARGET_OTHERONE)
				AI_target[which] = TARGET_MYPET;		//????
			else// if( AI_target[which] == TARGET_MYPET)
				AI_target[which] = TARGET_MYSELF;		//???*/
			break;
		case MAGIC_TARGET_ALLOTHERSIDE:
			AI_target[which] = TARGET_OTHERSIDE;		//?з????
			break;
		case MAGIC_TARGET_ALL:
			AI_target[which] = TARGET_ALL;				//???
			break;
#ifdef __ATTACK_MAGIC
		// ?о?????
		case MAGIC_TARGET_SINGLE:
			AI_target[which] = TARGET_OTHERONE;
			break;
		// ?о??????
		case MAGIC_TARGET_ONE_ROW:
			AI_target[which] = TARGET_ONE_ROW;
			break;
		// ?о????
		case MAGIC_TARGET_ALL_ROWS:
			AI_target[which] = TARGET_OTHERSIDE;
			break;
#endif
		}
//		if( magic[method].target == MAGIC_TARGET_OTHER)
//			AI_target[which] ^= 1;
	}
}

void AI_SetMethod( int target, int which)
{
	switch( target){
	case MAGIC_TARGET_MYSELF:					//???
		AI_target[which] = TARGET_MYSELF;
		break;
	case MAGIC_TARGET_ALLMYSIDE:				//??????
		AI_target[which] = TARGET_MYSIDE;
		break;
	case MAGIC_TARGET_WHOLEOTHERSIDE:			//?κ???????
		AI_target[which] = TARGET_MYSIDE;
		break;
	case MAGIC_TARGET_OTHER:					//?κ????
		AI_target[which] = TARGET_MYSELF;
		break;
	case MAGIC_TARGET_ALLOTHERSIDE:				//?з????
		AI_target[which] = TARGET_OTHERSIDE;
		break;
	case MAGIC_TARGET_ALL:						//???
		AI_target[which] = TARGET_ALL;
		break;
#ifdef __ATTACK_MAGIC
	// ?о?????
	case MAGIC_TARGET_SINGLE:
		AI_target[which] = TARGET_OTHERONE;
		break;
	// ?о??????
	case MAGIC_TARGET_ONE_ROW:
		AI_target[which] = TARGET_ONE_ROW;
		break;
	// ?о????
	case MAGIC_TARGET_ALL_ROWS:
		AI_target[which] = TARGET_OTHERSIDE;
		break;
#endif
	}
}

void AI_SetUpCureMethod( int which)
{
	char *str;
	int method = AI_method[which]+1;
	if( method > MAX_MAGIC)
		method = 0;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYSELF;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "耐久力")) && strstr( str, "回复")){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
/*	if( i >= MAX_MAGIC){
		for( i = 0; i < MAX_ITEM; i++){
			if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "?????")) && strstr( str, "???")){
				AI_method[which] = MAX_MAGIC;
				AI_target[which] &= 1;
				break;
			}
		}
	}*/
}

void AI_SetDownCureMethod( int which)
{
	char *str;
	int method = AI_method[which];
	if( method < 0)
		method = MAX_MAGIC;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYSELF;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "耐久力")) && strstr( str, "回复")){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
/*	if( i < 0){
		for( i = 0; i < MAX_ITEM; i++){
			if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "?????")) && strstr( str, "???")){
				AI_method[which] = MAX_MAGIC;
				AI_target[which] &= 1;
				break;
			}
		}
	}*/
}

void AI_SetUpFirstMethod( int which)
{
	int method = AI_method[which]+1;
#ifndef _AI_NEW_FIRST
	if( method > MAX_MAGIC)	method = 0;
	AI_method[which] = -1;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
#else
	// ????????
	if(bFirstFlag == false){
		if(method > MAX_MAGIC){
			if(pc.profession_class != 0){ // ????
				AI_method[which] = -1;
				AI_SetDownAttackMethod(which);
				// ?趨??ü???
				bFirstFlag = true;
				return;
			}
			else method = 0;
		}
	}
	// ????ü???
	else{
		if(method > MAX_PROFESSION_SKILL){
			method = -1;
			// ?趨??????
			bFirstFlag = false;
		}
		AI_SetDownAttackMethod(which);
	}
	if(bFirstFlag == false){
		AI_method[which] = -1;
		for( int i = method; i < MAX_MAGIC; i++){
			if( magic[i].useFlag){
				AI_method[which] = i;
				AI_SetMethod( magic[i].target, which);
				break;
			}
		}
		if(AI_method[which] == -1 && pc.profession_class != 0){
			AI_SetDownAttackMethod(which);
			// ?趨??ü???
			bFirstFlag = true;
		}
	}
	if(AI_method[which] == -1) bFirstFlag = false;
#endif
}

void AI_SetDownFirstMethod( int which)
{
	int method = AI_method[which];
#ifndef _AI_NEW_FIRST
	if( method < 0)	method = MAX_MAGIC;
	AI_method[which] = -1;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag){
			AI_method[which] = i;
			AI_SetMethod( magic[i].target, which);
			break;
		}
	}
#else
	// ????????
	if(bFirstFlag == false){
		if(method < 0){
			if(pc.profession_class != 0){ // ????
				AI_method[which] = MAX_PROFESSION_SKILL;
				AI_SetUpAttackMethod(which);
				// ?趨??ü???
				bFirstFlag = true;
				return;
			}
			else method = MAX_MAGIC;
		}
	}
	// ????ü???
	else{
		AI_SetUpAttackMethod(which);
		if(AI_method[which] == -1){
			method = MAX_MAGIC;
			bFirstFlag = false;
		}
	}
	if(bFirstFlag == false){
		AI_method[which] = -1;
		for( int i = (method-1); i >= 0; i--){
			if( magic[i].useFlag){
				AI_method[which] = i;
				AI_SetMethod( magic[i].target, which);
				break;
			}
		}
	}
#endif
}

void AI_SetUpResurrectMethod( int which)
{
	char *str;
	int method = AI_method[which]+1;
	if( method > MAX_MAGIC)
		method = 0;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYPET;
	for( int i = method; i < MAX_MAGIC; i++){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "气绝状态")) && strstr( str, "回复")){
			AI_method[which] = i;
			break;
		}
	}
}

void AI_SetDownResurrectMethod( int which)
{
	char *str;
	int method = AI_method[which];
	if( method < 0)
		method = MAX_MAGIC;
	AI_method[which] = MAX_MAGIC;
	AI_target[which] = TARGET_MYPET;
	for( int i = (method-1); i >= 0; i--){
		if( magic[i].useFlag && (str=strstr( magic[i].memo, "气绝状态")) && strstr( str, "回复")){
			AI_method[which] = i;
			break;
		}
	}
}

void AI_SetUpLimit( int which)
{
	AI_limit[which] += 5;
	if( AI_limit[which] > 100)
		AI_limit[which] = 100;
}

void AI_SetDownLimit( int which)
{
	AI_limit[which] -= 5;
	if( AI_limit[which] < 0)
		AI_limit[which] = 0;
}

#ifdef _ATTACK_AI
void AI_SetUpAttackMethod(int which)
{
	int method = AI_method[which];
	while(1){
		--method;
		if(method < 0) method = MAX_PROFESSION_SKILL;
		if(method == AI_method[which] || method == MAX_PROFESSION_SKILL){
			AI_method[which] = MAX_PROFESSION_SKILL;
			if(AI_CheckSkillSetting(which,method,0) == 0) continue;
			else break;
		}
		if(profession_skill[method].useFlag == 1){
			AI_method[which] = method;
			if(AI_CheckSkillSetting(which,method,0) == 0) continue;
			else break;
		}
	}
}

void AI_SetDownAttackMethod(int which)
{
	int method = AI_method[which];
	while(1){
		++method;
		if(method > MAX_PROFESSION_SKILL) method = 0;
		if(method == AI_method[which] || method == MAX_PROFESSION_SKILL){
			AI_method[which] = MAX_PROFESSION_SKILL;
			if(AI_CheckSkillSetting(which,method,1) == 0) continue;
			else break;
		}
		if(profession_skill[method].useFlag == 1){
			AI_method[which] = method;
			if(AI_CheckSkillSetting(which,method,1) == 0) continue;
			else break;
		}
	}
}
#endif

#define AIBtnNum 24
//???????趨
bool AI_SettingProc()
{
	static char Notice[][64] = { "储存设定", "取消设定",
		"选择回复精灵", "选择回复精灵", "选择优先顺序", "选择优先顺序", "增加耐久力忍受度", "减少耐久力忍受度",
		"建议设定高于使用精灵所需MP", "建议设定高于使用精灵所需MP",
		"选择首次精灵", "选择首次精灵", "选择使用目标", "选择使用目标",
		"选择气绝回复方式", "选择气绝回复方式", "选择使用目标", "选择使用目标"
#ifdef _ATTACK_AI
		,"选择攻击方式","选择攻击方式","选择使用目标","选择使用目标","选择回复方式","选择回复方式"
#endif
	};
	static int x = 0, y = 0;
	static unsigned long dwPressTime;
	static int btnId[AIBtnNum], btnState[AIBtnNum];
	static int u_offx[] = { 209-93, 296-93, 379-93};
	static int d_offx[] = { 209-112, 296-112, 379-112};
	static char MethodName[] = "不能使用";
	char *str;
	//AI_menu_state
	if( pAISettingWnd == NULL){
#ifndef _ATTACK_AI
		pAISettingWnd = MakeWindowDisp( x, y, 413, 302, 26261, -1);
#else
	#ifdef _READ16BITBMP
		if(g_bUseAlpha) pAISettingWnd = MakeWindowDisp( x, y, 413, 302, OLD_GRAPHICS_START + 57, -1);
		else
	#endif
		pAISettingWnd = MakeWindowDisp( x, y, 413, 302, 35225, -1);
#endif
		dwPressTime = 0;
		AI_ReLoad();
		AI_CheckSetting();
		for( int i = 0 ; i < AIBtnNum ; i++){
			btnId[i] = -2;
			btnState[i] = 0;
		}
	}else{
		if( pAISettingWnd->hp >= 1){
			if( joy_trg[ 0 ] & JOY_ESC){
				DeathAction( pAISettingWnd );
				pAISettingWnd = NULL;
#ifdef _AI_OTHER
				if(pAIOtherFunctionWnd){
					DeathAction(pAIOtherFunctionWnd);
					pAIOtherFunctionWnd = NULL;
					AI_Other_State = 0;
				}
#endif
				return true;
			}
			if( dwPressTime){
				if( TimeGetTime()>(dwPressTime+100)){
					dwPressTime = 0;
					if( btnState[0] == 1){//????"???"
						btnState[0] = 0;
						DeathAction( pAISettingWnd );
						pAISettingWnd = NULL;
#ifdef _AI_OTHER
						if(pAIOtherFunctionWnd){
							DeathAction(pAIOtherFunctionWnd);
							pAIOtherFunctionWnd = NULL;
							AI_Other_State = 0;
						}
#endif
						play_se( 217, 320, 240 );
						AI_Save();
						return true;
					}else if( btnState[1] == 1){//????"???"
						btnState[1] = 0;
						DeathAction( pAISettingWnd );
						pAISettingWnd = NULL;
#ifdef _AI_OTHER
						if(pAIOtherFunctionWnd){
							DeathAction(pAIOtherFunctionWnd);
							pAIOtherFunctionWnd = NULL;
							AI_Other_State = 0;
						}
#endif
						play_se( 203, 320, 240 );
						return true;
					}else if( btnState[2] == 1){//????????????????
						btnState[2] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpCureMethod( 0);
					}else  if( btnState[3] == 1){//????????????????
						btnState[3] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownCureMethod( 0);
					}else if( btnState[4] == 1){//????????????????
						btnState[4] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget( 0);
					}else if( btnState[5] == 1){//????????????????
						btnState[5] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget( 0);
					}else if( btnState[6] == 1){//????????????????
						btnState[6] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpLimit( 0);
					}else  if( btnState[7] == 1){//????????????????
						btnState[7] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownLimit( 0);
					}else if( btnState[8] == 1){//?????????????????
						btnState[8] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpLimit( 1);
					}else  if( btnState[9] == 1){//?????????????????
						btnState[9] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownLimit( 1);
					}else if( btnState[10] == 1){//??????ε?????????
						btnState[10] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpFirstMethod( 2);
					}else  if( btnState[11] == 1){//??????ε?????????
						btnState[11] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownFirstMethod( 2);
					}else if( btnState[12] == 1){//??????ε?????????
						btnState[12] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget( 2);
					}else  if( btnState[13] == 1){//??????ε?????????
						btnState[13] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget( 2);
					}else if( btnState[14] == 1){//????????????????
						btnState[14] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpResurrectMethod( 3);
					}else  if( btnState[15] == 1){//????????????????
						btnState[15] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownResurrectMethod( 3);
					}else if( btnState[16] == 1){//????????????????
						btnState[16] = 0;
						play_se( 217, 320, 240 );
						if( TARGET_1<=AI_target[3] && AI_target[3]<=TARGET_5){
							AI_target[3]--;
							if( AI_target[3] < TARGET_1)
								AI_target[3] = TARGET_MYSIDE;
						}else if( AI_target[3] == TARGET_MYPET){
							AI_target[3] = TARGET_5;
						}else{
							AI_target[3] = TARGET_MYPET;
						}
					}else  if( btnState[17] == 1){//????????????????
						btnState[17] = 0;
						play_se( 217, 320, 240 );
						if( TARGET_1<=AI_target[3] && AI_target[3]<=TARGET_5){
							AI_target[3]++;
							if( AI_target[3] > TARGET_5)
								AI_target[3] = TARGET_MYPET;
						}else if( AI_target[3] == TARGET_MYPET){
							AI_target[3] = TARGET_MYSIDE;
						}else{
							AI_target[3] = TARGET_1;
						}
					}
#ifdef _ATTACK_AI
					else if(btnState[18] == 1){// ????????????е?????????????
						btnState[18] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpAttackMethod(4);
					}
					else if(btnState[19] == 1){// ????????????е?????????????
						btnState[19] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownAttackMethod(4);
					}
					else if(btnState[20] == 1){// ????????????е?????????????
						btnState[20] = 0;
						play_se( 217, 320, 240 );
						AI_SetUpTarget(4);
					}else if(btnState[21] == 1){// ????????????е?????????????
						btnState[21] = 0;
						play_se( 217, 320, 240 );
						AI_SetDownTarget(4);
					}else if( btnState[22] == 1){//?????????????????
						btnState[22] = 0;
						play_se( 217, 320, 240 );
						// ???в???????
						if(g_bUsePS){
							if(AI_method[1] == -1) AI_method[1] = g_iPSindex;
							else AI_method[1] = -1;
						}
					}else if( btnState[23] == 1){//?????????????????
						btnState[23] = 0;
						play_se( 217, 320, 240 );
						// ???в???????
						if(g_bUsePS){
							if(AI_method[1] == -1) AI_method[1] = g_iPSindex;
							else AI_method[1] = -1;
						}
					}
#endif
				}
			}else{
				//????°??
				for( int i = 0; i < AIBtnNum; i++){
					if( HitDispNo == btnId[i]){
						if( mouse.onceState & MOUSE_LEFT_CRICK){
							dwPressTime = TimeGetTime();
							btnState[i] = 1;
							break;
						}
					}
				}
#ifdef _AI_OTHER
				//????????
				if(MakeHitBox(x+317,y+168,x+386,y+193,DISP_PRIO_BOX2))
					if(mouse.onceState & MOUSE_LEFT_CRICK) AI_Other_State = 1;
#endif
			}
			//???????
			for( int i = 0; i < AIBtnNum; i++){
				if( HitDispNo == btnId[i]){
					strcpy( OneLineInfoStr, Notice[i]);
					break;
				}
			}
#ifndef _ATTACK_AI
			StockDispBuffer( ((WINDOW_DISP *)pAISettingWnd->pYobi)->mx, ((WINDOW_DISP *)pAISettingWnd->pYobi)->my, DISP_PRIO_MENU, 26261, 1);
#else
	#ifdef _READ16BITBMP
			if(g_bUseAlpha) StockDispBuffer( ((WINDOW_DISP *)pAISettingWnd->pYobi)->mx, ((WINDOW_DISP *)pAISettingWnd->pYobi)->my, DISP_PRIO_MENU, OLD_GRAPHICS_START + 57, 1);
			else
	#endif
			StockDispBuffer( ((WINDOW_DISP *)pAISettingWnd->pYobi)->mx, ((WINDOW_DISP *)pAISettingWnd->pYobi)->my, DISP_PRIO_MENU, 35225, 1);
#endif
			btnId[0] = StockDispBuffer( x+207, y+152, DISP_PRIO_IME3, 26262+btnState[0], 2);
			btnId[1] = StockDispBuffer( x+207, y+152, DISP_PRIO_IME3, 26264+btnState[1], 2);
			for( int i = 0; i < 3; i++){
				btnId[2+i*2] = StockDispBuffer( x+u_offx[i], y+92+105, DISP_PRIO_IME3, 26064+btnState[2+i*2], 2);
				btnId[3+i*2] = StockDispBuffer( x+d_offx[i], y+107+106, DISP_PRIO_IME3, 26066+btnState[3+i*2], 2);
			}
			btnId[8] = StockDispBuffer( x+u_offx[2], y+128+105, DISP_PRIO_IME3, 26064+btnState[8], 2);
			btnId[9] = StockDispBuffer( x+d_offx[2], y+143+106, DISP_PRIO_IME3, 26066+btnState[9], 2);
			for( int i = 0; i < 2; i++){
				btnId[10+i*2] = StockDispBuffer( x+u_offx[i], y+164+105, DISP_PRIO_IME3, 26064+btnState[10+i*2], 2);
				btnId[11+i*2] = StockDispBuffer( x+d_offx[i], y+179+106, DISP_PRIO_IME3, 26066+btnState[11+i*2], 2);
			}
			for( int i = 0; i < 2; i++){
				btnId[14+i*2] = StockDispBuffer( x+u_offx[i], y+200+105, DISP_PRIO_IME3, 26064+btnState[14+i*2], 2);
				btnId[15+i*2] = StockDispBuffer( x+d_offx[i], y+215+106, DISP_PRIO_IME3, 26066+btnState[15+i*2], 2);
			}
#ifdef _ATTACK_AI
			for( int i = 0; i < 2; i++){
				btnId[18+i*2] = StockDispBuffer( x+u_offx[i], y+236+105, DISP_PRIO_IME3, 26064+btnState[18+i*2], 2);
				btnId[19+i*2] = StockDispBuffer( x+d_offx[i], y+251+106, DISP_PRIO_IME3, 26066+btnState[19+i*2], 2);
			}
			btnId[22] = StockDispBuffer( x+u_offx[0], y+128+105, DISP_PRIO_IME3, 26064+btnState[22], 2);
			btnId[23] = StockDispBuffer( x+d_offx[0], y+143+106, DISP_PRIO_IME3, 26066+btnState[23], 2);
#endif
#ifdef _AI_OTHER
			StockFontBuffer(x + 322,y + 173,FONT_PRIO_FRONT,FONT_PAL_WHITE,"其他功能",0);
#endif
			char limit[8];
			int dy = 102;
#ifndef _ATTACK_AI
			for( int i = 0; i < 4; i++, dy+=36){
#else
			for( int i = 0; i < 5; i++, dy+=36){
#endif
				str = MethodName;
#ifdef _ATTACK_AI
				if(i == 4){
					if(MAX_PROFESSION_SKILL > AI_method[i] && AI_method[i] >= 0) str = profession_skill[AI_method[i]].name;
					else if(AI_method[i] == MAX_PROFESSION_SKILL)
						str = "普通攻击";
				}
				else if(i == 1){
					if(g_bUsePS && AI_method[1] != -1) str = profession_skill[g_iPSindex].name;
					else str = "使用道具";
				}else
#endif
#ifdef _AI_NEW_FIRST
				if(i == 2){
					if(bFirstFlag){
						if(MAX_PROFESSION_SKILL > AI_method[i] && AI_method[i] >= 0) str = profession_skill[AI_method[i]].name;
					}
					else if(MAX_MAGIC > AI_method[i] && AI_method[i] >= 0) str = magic[AI_method[i]].name;
				}else
#endif
				{
					if( MAX_MAGIC>AI_method[i] && AI_method[i]>=0)
						str = magic[AI_method[i]].name;
					else if( AI_method[i] == MAX_MAGIC)
						str = "使用道具";
				}
				StockFontBuffer( x+85, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, str, 0 );
				limit[0] = 0;
				if( str != MethodName){
					switch( AI_target[i]){
					case TARGET_MYSELF:
						str = "自己";
						break;
					case TARGET_MYPET:
						str = "宠物";
						break;
					case TARGET_MYSIDE:
						str = "我方全体";
						break;
					case TARGET_OTHERONE:
						str = "敌人";
						break;
					case TARGET_OTHERSIDE:
						str = "敌人全体";
						break;
					case TARGET_ALL:
						str = "全体";
						break;
					case TARGET_1:
						str = "玩家一";
						break;
					case TARGET_2:
						str = "玩家二";
						break;
					case TARGET_3:
						str = "玩家三";
						break;
					case TARGET_4:
						str = "玩家四";
						break;
					case TARGET_5:
						str = "玩家五";
						break;
#ifdef __ATTACK_MAGIC
					case TARGET_ONE_ROW:
						str = "敌一排";
						break;
#endif
					default:
						str = "错误请重设";
						break;
					}
//					itoa( AI_limit[i], limit, 10);
					
                    sprintf(limit, "%d", AI_limit[i]);
					strcat( limit, "％");
					
				}
				StockFontBuffer( x+236, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, str, 0 );
				if( i < 2)
					StockFontBuffer( x+336, y+dy, FONT_PRIO_FRONT, FONT_PAL_YELLOW, limit, 0 );
			}
		}
	}
	return false;
}

static int Ordinal[] = { 4, 2, 0, 1, 3 };
int GetResurrectTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func && p_party[start]->hp<=0){
				target = start;
				break;
			}
		}
	}
	return target;
}

int GetFirstTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func){
				target = start;
				break;
			}
		}
	}
	return target;
}

int GetCureTarget( int target_flag)
{
	int target = -1, start;
	if( TARGET_1<=target_flag && target_flag<=TARGET_5){
		start = Ordinal[target_flag-TARGET_1];
		for( int i = 0; i < 2; i++, start+=5){
			if( p_party[start]->func && p_party[start]->hp>0 && p_party[start]->maxHp*AI_limit[0]/100 > p_party[start]->hp){
				target = start;
				break;
			}
		}
	}
	return target;
}

extern bool bNewServer;
extern int GetBattelTarget();
extern int battlePlayerEscFlag;

#ifdef _ATTACK_AI
static int Ordinal2[] = {
	19,17,15,16,18,
	14,12,10,11,13,
	 9, 7, 5, 6, 8,
	 4, 2, 0, 1, 3
};

int GetDeadTarget()
{
	int i, index, end;
	i = 10;
	end = 20;
	if(BattleMyNo < 10){
		i = 0 ;
		end = 10;
	}
	for(;i<end;i++){
		index = Ordinal2[i];
		if(p_party[index]->func == NULL ) continue;
		if(p_party[index]->hp > 0) continue;
		return index;
	}
	return -1;
}

int AttackAIProc(int which,char *moji)
{
	int targetNo;
	int method = AI_method[which];
	int no;

	if(method != MAX_PROFESSION_SKILL){
		if(profession_skill[method].costmp <= p_party[BattleMyNo]->mp){
			targetNo = AI_target[which];
			switch(profession_skill[method].target){
			case PETSKILL_TARGET_NONE:
			case PETSKILL_TARGET_MYSELF:
				sprintf( moji,"P|%X|%X", method,BattleMyNo);
				break;
			case PETSKILL_TARGER_DEATH:
				targetNo = GetDeadTarget();
				if(targetNo != -1) sprintf(moji,"P|%X|%X",method,targetNo);
				else{
					targetNo = GetBattelTarget();
					sprintf(moji,"H|%X",targetNo);
				}
				break;
			case PETSKILL_TARGET_OTHER:
			case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
			case PETSKILL_TARGET_WITHOUTMYSELFANDPET:
			case PETSKILL_TARGET_ONE_LINE:
				if(targetNo == TARGET_MYSELF)	targetNo = BattleMyNo;
				else if(targetNo == TARGET_MYPET) targetNo = BattleMyNo + 5;
				else if(targetNo < TARGET_OTHERONE || targetNo > TARGET_ALL || 
					targetNo == TARGET_ONE_ROW) targetNo = GetFirstTarget(targetNo);
				else targetNo = GetBattelTarget();
				sprintf( moji,"P|%X|%X", method,targetNo);
				break;
			case PETSKILL_TARGET_ALLMYSIDE:
				if( BattleMyNo < 10 ) no = 20;
				else no = 21;
				sprintf( moji,"P|%X|%X", method, no );
				break;
			case PETSKILL_TARGET_ALLOTHERSIDE:
				if( BattleMyNo < 10 ) no = 21;
				else no = 20;
				sprintf( moji,"P|%X|%X", method, no );
				break;
			case PETSKILL_TARGET_ALL:
				sprintf( moji,"P|%X|%X", method, 22 );
				break;
			case PETSKILL_TARGET_ONE_ROW:
#ifdef _SKILL_ADDBARRIER
			case PETSKILL_TARGET_ONE_ROW_ALL:
#endif
				targetNo = GetBattelTarget();
				if ( targetNo >= 0 && targetNo < 5 ) no = 26 ; 
				if ( targetNo >= 5 && targetNo < 10 ) no = 25 ; 
				if ( targetNo >= 10 && targetNo < 15 ) no = 23 ; 
				if ( targetNo >= 15 && targetNo < 20 ) no = 24 ; 
				sprintf( moji,"P|%X|%X", method, no );
				break;
			}
			AI_Choosen = profession_skill[method].name;
			return 1;
		}
		return 0;
	}
	return -1;
}
#endif

void AI_ChooseAction()
{
	char *str;
	bool done = 0;
	int targetNo = -1;
	char moji[64];
	battlePlayerEscFlag = false;
	//?????
	if( BattleMyNo >= 20 ){
		if( bNewServer)
			lssproto_B_send( sockfd, "N" );
		else
			old_lssproto_B_send( sockfd, "N" );
		extern void InitBattleAnimFlag();
		InitBattleAnimFlag();
		SubProcNo++;
		return;
	}
	if( p_party[ BattleMyNo ]->hp <= 0 ||
		BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){
		if( BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){
			if( bNewServer)
				lssproto_B_send( sockfd, "N" );
			else
				old_lssproto_B_send( sockfd, "N" );
		}
	}else{
		if( AI_First){//?????
			AI_First = 0;
			int method = AI_method[2];
#ifdef _AI_NEW_FIRST
			if(bFirstFlag == false){
#endif
				if( method>-1 && method<MAX_MAGIC && magic[method].useFlag){
					bool mp_enough = true;
					if( pc.familySprite == 0){
						if( (magic[method].mp*80/100) > p_party[BattleMyNo]->mp )
							mp_enough = false;
					}else{
						if( magic[method].mp > p_party[BattleMyNo]->mp )
							mp_enough = false;
					}
					if( mp_enough){
						switch( AI_target[2]){
						case TARGET_MYSELF:
							targetNo = BattleMyNo;
							break;
						case TARGET_MYPET:
							if( p_party[BattleMyNo+5]->func)
								targetNo = BattleMyNo + 5;
							break;
						case TARGET_MYSIDE:
							switch( magic[method].target){
							case MAGIC_TARGET_WHOLEOTHERSIDE:
								if( BattleMyNo < 10 ) targetNo = 20;
								else targetNo = 21;
								break;
							case MAGIC_TARGET_ALLMYSIDE:
								if( BattleMyNo < 10 ) targetNo = 20;
								else targetNo = 21;
								break;
							}
							break;
							case TARGET_OTHERONE:
								targetNo = GetBattelTarget();
								break;
							case TARGET_OTHERSIDE:
								switch( magic[method].target){
#ifdef __ATTACK_MAGIC
								case MAGIC_TARGET_ALL_ROWS:
#endif
								case MAGIC_TARGET_WHOLEOTHERSIDE:
								case MAGIC_TARGET_ALLOTHERSIDE:
									if( BattleMyNo < 10 ) targetNo = 21;
									else targetNo = 20;
									break;
								}
								break;
								case TARGET_ALL:
									targetNo = 22;
									break;
								case TARGET_1:
								case TARGET_2:
								case TARGET_3:
								case TARGET_4:
								case TARGET_5:
									targetNo = GetFirstTarget( AI_target[2]);
									break;
#ifdef __ATTACK_MAGIC
									// 23: ???????? , 24: ???????? , 25: ???????? , 26: ????????
								case TARGET_ONE_ROW:
									targetNo = GetBattelTarget();
									if( targetNo < 5)
										targetNo = 25;
									else if( targetNo < 10)
										targetNo = 26;
									else if( targetNo < 15)
										targetNo = 23;
									else
										targetNo = 24;
									break;
#endif
								default:
									targetNo = -1;
									break;
						}
						if( targetNo > -1){
							AI_Choosen = "使用首次精灵";
							sprintf( moji, "J|%X|%X", method, targetNo);
							done = 1;
						}
					}else
						StockChatBufferLine( "MP不足无法使用首次的精灵", FONT_PAL_WHITE);
				}
				//	else
				//		StockChatBufferLine( "????趨??ε????", FONT_PAL_WHITE);
#ifdef _AI_NEW_FIRST
			}
			else{
				int ret;
				ret = AttackAIProc(2,moji);
				if(ret) done = 1;
				else if(ret == 0) StockChatBufferLine("MP不足无法使用首次的技能", FONT_PAL_WHITE);
				else if(ret == -1) StockChatBufferLine("技能代号错误", FONT_PAL_WHITE);
			}
#endif
		}
		if( 0 == done){
			int magic_id = -1, item_id = -1, target = -1, meat = -1;
			//???????????????????????
			for( int i = 0; i < MAX_ITEM; i++){
				if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "耐久力")) && strstr( str, "回复")){
					item_id = i;
					if( strstr( pc.item[i].name, "肉")){
						meat = i;
						break;
					}
				}
			}
			//????????????????????????mp?????
			int method = AI_method[0];
			if( (method>-1 && method<MAX_MAGIC) && magic[method].useFlag && (str=strstr( magic[method].memo, "耐久力")) && strstr( str, "回复")){
				if( pc.familySprite == 0){
					if( (magic[method].mp*80/100) <= p_party[BattleMyNo]->mp )
						magic_id = method;
				}else{
					if( magic[method].mp <= p_party[BattleMyNo]->mp )
						magic_id = method;
				}
			}
			//?????????????
			if( TARGET_1<=AI_target[0] && AI_target[0]<=TARGET_5){
				target = GetCureTarget( AI_target[0]);
			}
			if( target == -1){
				if( AI_target[0] == TARGET_MYSELF){//???????
					if( p_party[BattleMyNo]->maxHp*AI_limit[0]/100 > p_party[BattleMyNo]->hp){
						target = BattleMyNo;
					}else if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100>pet[pc.battlePetNo].hp){
						target = BattleMyNo + 5;
					}
				}else{// if( AI_target[0] == TARGET_MYPET){//????????
					if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100 > pet[pc.battlePetNo].hp){
						target = BattleMyNo + 5;
					}else if( p_party[BattleMyNo]->maxHp*AI_limit[0]/100>p_party[BattleMyNo]->hp){
						target = BattleMyNo;
					}
				}
			}
			if( target > -1){
				if( item_id>-1 || magic_id>-1){
					if( meat > -1){//??????????
						done = 1;
						sprintf( moji,"I|%X|%X", meat, target);
						AI_Choosen = "使用肉回复耐久力";
					}else{
						if( MAX_MAGIC>AI_method[0] && AI_method[0]>=0){//?????鲹?????
							if( magic[AI_method[0]].target != MAGIC_TARGET_MYSELF || target==BattleMyNo){
								if( magic_id > -1){
									int method = AI_method[0];
									if( MAGIC_TARGET_WHOLEOTHERSIDE == magic[method].target){
										if( BattleMyNo < 10 ) target = 20;
										else target = 21;
									}
									done = 1;
									AI_Choosen = "使用耐久力回复精灵";
									sprintf( moji,"J|%X|%X", magic_id, target);
								}else{
									StockChatBufferLine( "无法使用耐久力回复的精灵", FONT_PAL_WHITE);
								}
							}
						}
						if( !done){//????????????
							if( item_id > -1){
								done = 1;
								sprintf( moji,"I|%X|%X", item_id, target);
								AI_Choosen = "使用耐久力回复道具";
							}else{
								StockChatBufferLine( "没有耐久力回复的道具", FONT_PAL_WHITE);
							}
						}
					}
				}else
					StockChatBufferLine( "无法回复耐久力了", FONT_PAL_WHITE);
			}
		}
		if( 0==done){
			int start = 0, end = 10, target = -1;
			switch( AI_target[3]){
			case TARGET_MYSIDE:
				if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp<=0){
					target = BattleMyNo+5;
				}else{
					if( BattleMyNo >= 10){
						start = 10;
						end = 20;
					}
					for( ; start < end; start++){
						if( BattleMyNo==start || (BattleMyNo+5)==start)
							continue;
						if( p_party[start]->func == NULL)
							continue;
						if( p_party[start]->hp>0)
							continue;
						target = start;
						break;
					}
				}
				break;
			case TARGET_MYPET:
				if( p_party[BattleMyNo+5]->func && pet[pc.battlePetNo].hp<=0)
					target = BattleMyNo+5;
				break;
			case TARGET_1:
			case TARGET_2:
			case TARGET_3:
			case TARGET_4:
			case TARGET_5:
				target = GetResurrectTarget( AI_target[3]);
				break;
			}
			if( target > -1){//????????
				if( MAX_MAGIC == AI_method[3]){
					int i;
					for( i = 0; i < MAX_ITEM; i++){
						if( pc.item[i].useFlag && strstr( pc.item[i].memo, "气绝回复")){
							AI_Choosen = "使用气绝回复道具";
							done = 1;
							sprintf( moji,"I|%X|%X", i, target);
							break;
						}
					}
					if( i >= MAX_ITEM)
						StockChatBufferLine( "没有气绝回复的道具", FONT_PAL_WHITE);
				}else{
					int method = AI_method[3];
					if( method>-1 && magic[method].useFlag && (str=strstr( magic[method].memo, "气绝状态")) && strstr( str, "回复")){
						if( pc.familySprite == 0){
							if( (magic[method].mp*80/100) <= p_party[BattleMyNo]->mp )
								done = 1;
						}else{
							if( magic[method].mp <= p_party[BattleMyNo]->mp )
								done = 1;
						}
						if( done == 1){
							AI_Choosen = "使用气绝回复精灵";
							sprintf( moji,"J|%X|%X", method, target);
						}
						else
							StockChatBufferLine( "MP不足无法使用气绝回复的精灵", FONT_PAL_WHITE);
					}
					else
						StockChatBufferLine( "没有设定气绝回复的精灵", FONT_PAL_WHITE);
				}
			}
		}
#ifndef _ATTACK_AI
		if( 0==done && (/*p_party[BattleMyNo]->maxMp*/100*AI_limit[1]/100 > p_party[BattleMyNo]->mp) ){//????
#else
		if(0 == done && ((p_party[BattleMyNo]->maxMp * AI_limit[1] / 100) > p_party[BattleMyNo]->mp) ){//????
			if(g_bUsePS && AI_method[1] != -1){
				sprintf( moji,"P|%X|%X",g_iPSindex,BattleMyNo);
				done = 1;
				AI_Choosen = profession_skill[g_iPSindex].name;
			}
			else
#endif
			{
				int i;
				for( i = 0; i < MAX_ITEM; i++){
					if( pc.item[i].useFlag && (str=strstr( pc.item[i].memo, "气")) && strstr( str, "回复")){
						AI_Choosen = "使用恢复气力道具";
						done = 1;
						sprintf( moji,"I|%X|%X", i, BattleMyNo);
						break;
					}
				}
				if( i >= MAX_ITEM){
#ifdef _DEBUG
					extern STR_BUFFER MyChatBuffer;
					sprintf(MyChatBuffer.buffer,"[additem 6 2400]");
					MyChatBuffer.cnt = strlen(MyChatBuffer.buffer);
					KeyboardReturn();
#endif
					StockChatBufferLine( "没有回复气力的道具", FONT_PAL_WHITE);
				}
			}
		}
		if( 0 == done){
//			if( BattleBpFlag & BATTLE_BP_BOOMERANG ){
//			}
			//?????????????? ?? ????? ??????
			if( ( (p_party[BattleMyNo]->maxHp*AI_limit[0]/100 > p_party[BattleMyNo]->hp) ||
				(p_party[BattleMyNo+5]->func &&		 
				( (pet[pc.battlePetNo].hp>0 && pet[pc.battlePetNo].maxHp*AI_limit[0]/100>pet[pc.battlePetNo].hp) ||
				pet[pc.battlePetNo].hp<=0) ) ) ){
				int mp_enough = false;
				if( 0<=AI_method[0] && AI_method[0]<MAX_MAGIC){//???趨??ò??????
					mp_enough = true;//???MP?????
					if( pc.familySprite == 0){
						if( (magic[AI_method[0]].mp*80/100) > p_party[BattleMyNo]->mp )
							mp_enough = false;
					}else{
						if( magic[AI_method[0]].mp > p_party[BattleMyNo]->mp )
							mp_enough = false;
					}
				}
				if( mp_enough == false){//????趨??ò?????? ?? MP????
					battlePlayerEscFlag = true;
					strcpy( moji, "E");
					done = 1;
				}
			}
			if( !done){
				int targetNo;
#ifdef _ATTACK_AI
				if(AttackAIProc(4,moji) <= 0)
#endif
				{
					targetNo = GetBattelTarget();
					sprintf( moji, "H|%X", targetNo );
					AI_Choosen = "自动攻击";
				}
			}
		}
		if( bNewServer)
			lssproto_B_send( sockfd, moji );
		else
			old_lssproto_B_send( sockfd, moji );
	}
	done = 0;
	strcpy( moji, "W|FF|FF");
	if( pc.battlePetNo>-1 && p_party[BattleMyNo+5]->hp>0 ){
		if( BattleBpFlag & BATTLE_BP_PET_MENU_NON || p_party[ BattleMyNo ]->hp <= 0 || battlePlayerEscFlag == true){
		}else{
			int skill_id = -1;
			for( int i = 0; i < pet[pc.battlePetNo].maxSkill; i++){
				if( petSkill[pc.battlePetNo][i].useFlag == true && 
					petSkill[pc.battlePetNo][i].field != PETSKILL_FIELD_MAP){
					if( skill_id == -1)
						skill_id = i;
					if( petSkill[pc.battlePetNo][i].skillId!=2 && petSkill[pc.battlePetNo][i].skillId!=0)
						skill_id = i;
					if( petSkill[pc.battlePetNo][i].skillId == 1){
						skill_id = i;
						break;
					}
				}
			}
			if( skill_id > -1){
				switch( petSkill[pc.battlePetNo][skill_id].target){
				case PETSKILL_TARGET_MYSELF:
					targetNo = BattleMyNo + 5;
					break;
				case PETSKILL_TARGET_OTHER:
				case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
					targetNo = GetBattelTarget();
					break;
				case PETSKILL_TARGET_ALLMYSIDE:
					if( BattleMyNo < 10 ) targetNo = 20;
					else targetNo = 21;
					break;
				case PETSKILL_TARGET_ALLOTHERSIDE:
					if( BattleMyNo < 10 ) targetNo = 21;
					else targetNo = 20;
					break;
				case PETSKILL_TARGET_ALL:
					targetNo = 22;
					break;
				}
				sprintf( moji, "W|%X|%X", skill_id, targetNo);
			}
		}
	}
	if( bNewServer)
		lssproto_B_send( sockfd, moji);
	else
		old_lssproto_B_send( sockfd, moji);
	for( int i = 0; i < 20; i++){
		if( 0 == (p_party[i]->atr & p_party[i]->atr) )
			p_party[i]->atr = ACT_ATR_INFO | ACT_ATR_HIT;
	}
	p_party[ BattleMyNo ]->atr |= ACT_ATR_BTL_CMD_END;
	p_party[ BattleMyNo + 5 ]->atr |= ACT_ATR_BTL_CMD_END;
	SubProcNo++;
}

void AI_CloseWnd()
{
	AI_State = 0;
	if( pAISettingWnd){
		DeathAction( pAISettingWnd );
		pAISettingWnd = NULL;
	}
#ifdef _AI_OTHER
	if(pAIOtherFunctionWnd){
		DeathAction(pAIOtherFunctionWnd);
		pAIOtherFunctionWnd = NULL;
		AI_Other_State = 0;
	}
#endif
}

#ifdef _AI_OTHER
#define AI_OTHER_BTN 2
void AI_OtherProc(){
	static int btnId[AI_OTHER_BTN];
	int x,y,i;
	static char szFunctionName[AI_OTHER_BTN][24] = {"","自动丢肉"};
	static char szFunctionSwitch[2][24] = {" N  O "," YES "};

	if(pAIOtherFunctionWnd == NULL){
		// ?????????
		if(ProcNo == PROC_BATTLE) pAIOtherFunctionWnd = MakeWindowDisp(413,150,3,2,NULL,1);
		else pAIOtherFunctionWnd = MakeWindowDisp(413,0,3,2,NULL,1);
	}
	else{
		if(pAIOtherFunctionWnd->hp >= 1 ){
			if(joy_trg[0] & JOY_ESC){
				DeathAction(pAIOtherFunctionWnd);
				pAIOtherFunctionWnd = NULL;
				AI_Other_State = 0;
				return;
			}
			x = pAIOtherFunctionWnd->x;
			y = pAIOtherFunctionWnd->y;
			for(i=1;i<AI_OTHER_BTN;i++){
				StockFontBuffer(x+25,y+i*20,FONT_PRIO_FRONT,FONT_PAL_WHITE,szFunctionName[i],0);
				btnId[i] = StockFontBuffer(x+100,y+i*20,FONT_PRIO_FRONT,FONT_PAL_WHITE,
																	 AI_OtherSetting[i-1]?szFunctionSwitch[1]:szFunctionSwitch[0],2);
			}
			btnId[0] = StockDispBuffer(x+100,y+(i+1)*20,DISP_PRIO_IME3,26042,2);

			if(mouse.onceState & MOUSE_LEFT_CRICK){
				int id;
				id = selFontId(btnId,sizeof(btnId)/sizeof(int));
				switch(id){
				// ???????
				case 1:
					AI_OtherSetting[0] = !AI_OtherSetting[0];
					break;
				}
				// ???
				if(HitDispNo == btnId[0]){
					DeathAction(pAIOtherFunctionWnd);
					pAIOtherFunctionWnd = NULL;
					AI_Other_State = 0;
					AI_Save();
				}
			}
		}
	}
}
#endif