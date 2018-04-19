/************************/
/*	pattern.cpp			*/
/************************/

/*	ENT	:   ACTION *a0 , 
		:	???????�L�T???????????�L�T??
		:	????????????????????????????

	RET	:   ?��????????
		:   ?��?????
*/
#include "../systeminc/version.h"
#include 	"../systeminc/system.h"
#include	"../systeminc/loadrealbin.h"
#include	"../systeminc/loadsprbin.h"
#include	"../systeminc/anim_tbl.h"
#include	"../systeminc/t_music.h"



#ifdef _NPC_PICTURE
extern bool g_bUseAlpha;
void picture_play( ACTION *a0 )
{
	//int i,j; 
	int pictable[9]={100388,100841,101178,100854,101570,100353,101759,101424,101489};
	if( a0->picture != 0 ){
		int picture1 = ((((a0->picture)&0xff000000)>>24)&0x000000ff);
		int picture2 = ((((a0->picture)&0x00ff0000)>>16)&0x000000ff);
		int picture3 = ((((a0->picture)&0x0000ff00)>>8 )&0x000000ff);
		int picture4 = (  (a0->picture)&0x000000ff );
		//a0->anim_no = ANIM_WALK;
		if( picture1 == 1 ){//���ϱ���
			if( a0->anim_chr_no  == 101936 ){
				a0->anim_chr_no = 101863;
				return;
			}
			else if( a0->anim_chr_no  == 101863 ){
				a0->anim_chr_no = pictable[a0->picturetemp];
			}
			else
				a0->anim_chr_no = 101863;
			++a0->picturetemp;
			if(a0->picturetemp == 9 )
				a0->picturetemp = 0;
		}
		else if( picture1 == 2 ){//������ĳ��ͼ��
			if( a0->anim_chr_no != pictable[picture2] ){ 
				if( a0->anim_chr_no  == 101863 )
					a0->anim_chr_no = pictable[a0->picturetemp];
				else
					a0->anim_chr_no = 101863;
			}
			else{
				if( a0->anim_no == 0 )//�趨����
					a0->anim_no = 3;
				else
					a0->anim_no = 0;
			}
			++a0->picturetemp;
			if(a0->picturetemp == 9 )
				a0->picturetemp = 0;
		}
		else if( picture1 == 3 ){
			if(g_bUseAlpha) a0->anim_chr_no = 101805; else a0->anim_chr_no = 101858;
			a0->picture = (((1)<<24)&0xff000000)
				|(((picture2)<<16)&0x00ff0000)
				|(((picture3)<< 8)&0x0000ff00)
				|( (picture4)     &0x000000ff);
		}
		
	}
}
#endif

/*----------  ?????????  ----------*/
// Return 1:�����˴˶���( ���ظ����ŵ�״���� )
// Return 0:���Ŵ˶�����
int pattern( ACTION *a0, int anim_spd, int loop_flg )
{
	ANIMLIST	*ptAnimlist;
	FRAMELIST	*ptFramelist;
	int i;
	short dx,dy;
	int BmpNo;
	U4 chrNo;
	if(ATR_CHR_ACT_OLD(a0) != ATR_CHR_ACT(a0) ||
	ATR_CHR_ANG(a0) != ATR_CHR_ANG_OLD(a0) ||
	ATR_CHR_NO(a0) != ATR_CHR_NO_OLD(a0)){
		ATR_CHR_NO_OLD(a0) = ATR_CHR_NO(a0);		/*��????????????*/
		ATR_CHR_ACT_OLD(a0) = ATR_CHR_ACT(a0);		/*��??�h????*/
		ATR_CHR_ANG_OLD(a0) = ATR_CHR_ANG(a0);		/*��???????*/
		ATR_CHR_TIM(a0) = 0;//frame counter��Ϊһ
		ATR_CHR_CNT(a0) = 0;//��������ĵڼ���ͼ
	}
	if(ATR_CHR_TIM(a0) > 0){
		ATR_CHR_TIM(a0)--;	//frame counter��һ
		return 0;
	}
	if(ATR_CHR_NO(a0) <= CG_INVISIBLE){
		ATR_PAT_NO(a0) = 0;	//ָ��Ϊ��0��ͼ
#ifdef _NPC_PICTURE
		picture_play(a0);
#endif
		return 0;
	}
	if(ATR_CHR_NO(a0) < SPRSTART){	//����sprite
	 	realGetNo( ATR_CHR_NO(a0) , (U4 *)&BmpNo );
		realGetPos( BmpNo  , &dx, &dy);
		ATR_PAT_NO(a0) = BmpNo;		//BMP��ͼ�����
		ATR_CHR_H_POS(a0) = dx;		//X����
		ATR_CHR_V_POS(a0) = dy;		//Y����
		ATR_CHR_TIM(a0)=0x7fffffff;
		//cary 
		if( ATR_CHR_ACT(a0) == ANIM_ATTACK && ATR_CHR_CNT(a0) == 0){
			ATR_CHR_TIM(a0)=0;
			ATR_HIT(a0) = 10000;
			ATR_CHR_CNT(a0) = 1;
#ifdef _NPC_PICTURE
			picture_play(a0);
#endif
			return 0;
		}
		ATR_CHR_CNT(a0) = 0;
		ATR_CHR_ACT(a0) = ANIM_STAND;
		if( loop_flg == ANM_NO_LOOP){
#ifdef _NPC_PICTURE
			picture_play(a0);
#endif
			return 1;
		}
		//cary end
#ifdef _NPC_PICTURE
		picture_play(a0);
#endif
		return 0;
	}
	if(ATR_CHR_NO(a0) > SPRSTART + mxSPRITE){
#ifdef _NPC_PICTURE
		picture_play(a0);
#endif
		return 0;		//����sprite�ķ�Χ
	}
	chrNo = ATR_CHR_NO(a0) - SPRSTART;
	ptAnimlist = SpriteData[chrNo].ptAnimlist;
	if( SpriteData[chrNo].animSize == 0 ){
#ifdef _NPC_PICTURE
		picture_play(a0);
#endif
		return 0;	//�޶���
	}
	//ѡ����Ҫ�ķ���Ͷ���
	for(i=0 ; i < SpriteData[chrNo].animSize; i++){
		if( ATR_CHR_ANG(a0) == ptAnimlist[i].dir && ptAnimlist[i].no == ATR_CHR_ACT(a0) )
			break;
	}
	if( i >= SpriteData[chrNo].animSize ){		
		// shan remark
		// i = 0;		
		for( int sh_i=0 ; sh_i < SpriteData[chrNo].animSize; sh_i++){
			if( ATR_CHR_ANG(a0) == ptAnimlist[sh_i].dir && ptAnimlist[sh_i].no == ANIM_STAND){
				i = sh_i;
				break;
			}else{
				i = 0;
			}
		}
	}
	ptFramelist = ptAnimlist[i].ptFramelist;
	if(anim_spd)	//��ָ�������ٶ�
		ATR_CHR_TIM(a0) = anim_spd;
	else
		ATR_CHR_TIM(a0) = ptAnimlist[i].dtAnim;
#ifdef _NPC_PICTURE	
	if( a0->picture != 0 ){
		int pictable[9]={100388,100841,101178,100854,101570,100353,101759,101424,101489};
		/*if( (((a0->picture&0xff000000)>>24) & 0x000000ff) == 1 
			|| (((a0->picture&0xff000000)>>24) & 0x000000ff) == 3
			|| ( (((a0->picture&&0xff000000)>>24) & 0x000000ff) == 2 
			&& ( a0->anim_chr_no != pictable[(((a0->picture&0x00ff0000)>>16)&0x000000ff)] ) ) )
			ATR_CHR_TIM(a0) = 1;//����npc�ٶȵ����
		*/
		if( a0->anim_chr_no != pictable[(((a0->picture&0x00ff0000)>>16)&0x000000ff)] 
			|| (((a0->picture&0xff000000)>>24) & 0x000000ff) == 1 
			|| (((a0->picture&0xff000000)>>24) & 0x000000ff) == 3 )
			if( a0->anim_chr_no != 101805 && a0->anim_chr_no != 101858 && a0->anim_chr_no != 101936 )
				ATR_CHR_TIM(a0) = 1;//����npc�ٶȵ����
	}
#endif
	if((U4)ATR_CHR_CNT(a0) >= ptAnimlist[i].frameCnt){	//????????????
#ifdef _NPC_PICTURE	
		picture_play(a0);
#endif
		if(loop_flg == ANM_NO_LOOP){		//???��??
			ATR_CHR_TIM(a0) = 255;
			return 1;		//???????��?
		} else
			ATR_CHR_CNT(a0) = 0;		//�I�d???
	}
	//????��??
	if( ptFramelist[ATR_CHR_CNT(a0)].SoundNo != 0 ){
		if(ptFramelist[ATR_CHR_CNT(a0)].SoundNo < 10000)
			play_se( ptFramelist[ATR_CHR_CNT(a0)].SoundNo, ATR_H_POS(a0), ATR_V_POS(a0) );
		else
			ATR_HIT(a0) = ptFramelist[ATR_CHR_CNT(a0)].SoundNo;
	}
#ifdef _PROFESSION_ADDSKILL
	else{
		if(ATR_CHR_NO(a0) >= 101769 && ATR_CHR_NO(a0) <= 101797){
			int soundnum = 0;
			switch(ATR_PAT_NO(a0)){
			case 156+OLD_GRAPHICS_START:
				soundnum = 390;break;//ˮ���390
			case 193+OLD_GRAPHICS_START:
				soundnum = 390;break;//ˮ���390
			case 304+OLD_GRAPHICS_START:
				soundnum = 327;break;//�ؽ��
			case 341+OLD_GRAPHICS_START:
				soundnum = 327;break;//�ؽ��
			case 231+OLD_GRAPHICS_START:
				soundnum = 395;break;//����
			case 268+OLD_GRAPHICS_START:
				soundnum = 268;break;//����
			case 380+OLD_GRAPHICS_START:
				soundnum = 154;break;//����
			case 417+OLD_GRAPHICS_START:
				soundnum = 154;break;//����	
			case 75+OLD_GRAPHICS_START:
				soundnum = 324;break;//�ƽ��
			case 97+OLD_GRAPHICS_START:
				soundnum = 324;break;//�ƽ��
			case 117+OLD_GRAPHICS_START:
				soundnum = 385;break;//������Ȼ
			case 137+OLD_GRAPHICS_START:
				soundnum = 385;break;//������Ȼ	
			case 60+OLD_GRAPHICS_START:
				soundnum = 388;break;//��Ȼ����	
			}
			play_se(soundnum,ATR_H_POS(a0),ATR_V_POS(a0));
		}
	}
#endif
	ATR_PAT_NO(a0) = ptFramelist[ATR_CHR_CNT(a0)].BmpNo;			//???�k????
	realGetPos(ptFramelist[ATR_CHR_CNT(a0)].BmpNo, &dx, &dy);		//??????????����?��?
	ATR_CHR_H_POS(a0) = ptFramelist[ATR_CHR_CNT(a0)].PosX + dx;		//??��????????
	ATR_CHR_V_POS(a0) = ptFramelist[ATR_CHR_CNT(a0)].PosY + dy;		//??��????????
	ATR_CHR_CNT(a0)++;
	ATR_CHR_TIM(a0)--;
	return 0;
}


