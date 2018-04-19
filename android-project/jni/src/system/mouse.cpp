/************************/
/*	mouse.c				*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/pc.h"
#include "../systeminc/menu.h"
#include "../systeminc/battleMenu.h"
//#include "../systeminc/ime_sa.h"
#include "../systeminc/map.h"
#include "../systeminc/login.h"

// ???????�R�e???
#define MOUSE_HIT_SIZE_X 48
#define MOUSE_HIT_SIZE_Y 48

// ??????????????
#define MOUSE_AUTO_REPEATE_TIME 100

// Robin

// ????�B�l
MOUSE mouse;
// ????????????????�k????????
int	HitFontNo;
bool isMouseHoverItem = false;
// ????????????�����k???????
int HitDispNo;
// ??????��??
char OneLineInfoStr[ 256 ];

int oneLineInfoFlag = 1;
// ����?????��
int BoxColor;
extern int transmigrationEffectFlag;

// ???�ѩ�???? ////////////////////////////////////////////////////////
void MouseInit( void )
{
	memset( &mouse, 0, sizeof( MOUSE ) );
	mouse.itemNo = -1; // ????�k????
}

// ????????�t????? //////////////////////////////////////////////
void MouseNowPoint( int x, int y )
{
	// ?��????
	//???????????????
	if(ResoMode == 1){
		mouse.nowPoint.x = x * 2;
		mouse.nowPoint.y = y * 2;
	} else {
		mouse.nowPoint.x = x;
		mouse.nowPoint.y = y;
	}
}

// ????????????????????�t????? //////////////////////
void MouseCrickLeftDownPoint( int x, int y )
{
	
	// ��?t????
	mouse.state |= MOUSE_LEFT_CRICK;
	mouse.onceState |= MOUSE_LEFT_CRICK;
	mouse.autoState |= MOUSE_LEFT_CRICK;
	// ?��????
	mouse.crickLeftDownPoint.x = x;
	mouse.crickLeftDownPoint.y = y;
	// ????????????????�@�e??�q?��
	mouse.beforeLeftPushTime = TimeGetTime();
	mouse.leftPushTime = 0;
}

// ???????????????????�t????? ////////////////////////
void MouseCrickLeftUpPoint( int x, int y )
{
	// ��?t????
	DISP_INFO 	*pDispInfo;
	mouse.state &= (~MOUSE_LEFT_CRICK);
	mouse.onceState |= MOUSE_LEFT_CRICK_UP;
	mouse.autoState &= (~MOUSE_LEFT_CRICK);
	// ?��????
	mouse.crickLeftUpPoint.x = x;
	mouse.crickLeftUpPoint.y = y;
	// ????????????????
	mouse.beforeLeftPushTime = 0;
	mouse.leftPushTime = 0;
}

// ??????????????????�t????? //////////////////////////
void MouseDblCrickLeftUpPoint( int x, int y )
{
	// ��?t????
	//mouse.onceState |= ( MOUSE_LEFT_DBL_CRICK | MOUSE_LEFT_CRICK );
	mouse.state |= MOUSE_LEFT_CRICK;
	mouse.onceState |= MOUSE_LEFT_DBL_CRICK;
	mouse.onceState |= MOUSE_LEFT_CRICK;
	mouse.autoState |= MOUSE_LEFT_CRICK;
	// ?��????
	mouse.crickLeftDownPoint.x = x;
	mouse.crickLeftDownPoint.y = y;
	// ????????????????�@�e??�q?��
	mouse.beforeLeftPushTime = TimeGetTime();
	mouse.leftPushTime = 0;
}

// ???????????????????�t????? ////////////////////////
void MouseCrickRightDownPoint( int x, int y )
{
	// ��?t????
	mouse.state |= MOUSE_RIGHT_CRICK;
	mouse.onceState |= MOUSE_RIGHT_CRICK;
	mouse.autoState |= MOUSE_RIGHT_CRICK;
	// ?��????
	mouse.crickRightDownPoint.x = x;
	mouse.crickRightDownPoint.y = y;
	// ????????????????�@�e??�q?��
	mouse.beforeRightPushTime = TimeGetTime();
	mouse.rightPushTime = 0;
}

// ???????????????????�t????? ////////////////////////
void MouseCrickRightUpPoint( int x, int y )
{
	// ��?t????
	mouse.state &= (~MOUSE_RIGHT_CRICK);
	mouse.onceState |= MOUSE_RIGHT_CRICK_UP;
	mouse.autoState &= (~MOUSE_RIGHT_CRICK);
	// ?��????
	mouse.crickRightUpPoint.x = x;
	mouse.crickRightUpPoint.y = y;
	// ????????????????
	mouse.beforeRightPushTime = 0;
	mouse.rightPushTime = 0;
}

// ??????????????????�t????? //////////////////////////
void MouseDblCrickRightUpPoint( int x, int y )
{
	// ��?t????
	mouse.state |= MOUSE_RIGHT_CRICK;
	mouse.onceState |= MOUSE_RIGHT_DBL_CRICK;
	mouse.onceState |= MOUSE_RIGHT_CRICK;
	mouse.autoState |= MOUSE_RIGHT_CRICK;
	// ?��????
	mouse.crickRightDownPoint.x = x;
	mouse.crickRightDownPoint.y = y;
	// ????????????????�@�e??�q?��
	mouse.beforeRightPushTime = TimeGetTime();
	mouse.rightPushTime = 0;
}

// ?????  ///////////////////////////////////////////////////////////////
void MouseProc( void )
{
	static unsigned int leftPushTimeBak;
	static unsigned int rightPushTimeBak;
	
	// ��?t????
	mouse.onceState = MOUSE_NO_CRICK;
	// ???????????????????
	if( mouse.beforeLeftPushTime > 0 )
	{
		mouse.leftPushTime = TimeGetTime() - mouse.beforeLeftPushTime;
	}
	// ???????????????????
	if( mouse.beforeRightPushTime > 0 )
	{
		mouse.rightPushTime = TimeGetTime() - mouse.beforeRightPushTime;
	}
	// ?????????
	// ????????????
	// ???��??????��
	if( mouse.leftPushTime > 500 ){
		// ???????
		if( leftPushTimeBak == 0 ){
			mouse.autoState |= MOUSE_LEFT_CRICK; // ?????
			leftPushTimeBak = mouse.leftPushTime;	// ?????
		}else
		// ????????��?
		if( mouse.leftPushTime - leftPushTimeBak >= MOUSE_AUTO_REPEATE_TIME ){
			mouse.autoState |= MOUSE_LEFT_CRICK; // ?????
			leftPushTimeBak = mouse.leftPushTime;	// ?????
		}else{	// ?????????
			mouse.autoState &= (~MOUSE_LEFT_CRICK);	// ??????
		}
	}else{ 
		mouse.autoState &= (~MOUSE_LEFT_CRICK);	// ??????
		leftPushTimeBak = 0;
	}
	
	// ????????????
	// ???��??????��
	if( mouse.rightPushTime > 500 ){
		// ???????
		if( rightPushTimeBak == 0 ){
			mouse.autoState |= MOUSE_RIGHT_CRICK; // ?????
			rightPushTimeBak = mouse.rightPushTime;	// ?????
		}else
		// ????????��?
		if( mouse.rightPushTime - rightPushTimeBak >= MOUSE_AUTO_REPEATE_TIME ){
			mouse.autoState |= MOUSE_RIGHT_CRICK; // ?????
			rightPushTimeBak = mouse.rightPushTime;	// ?????
		}else{	// ?????????
			mouse.autoState &= (~MOUSE_RIGHT_CRICK);	// ??????
		}
	}else{ 
		mouse.autoState &= (~MOUSE_RIGHT_CRICK);	// ??????
		rightPushTimeBak = 0;
	}
	
	
	
	// ????????????
	//if( mouse.rightPushTime > 0 ){
	//}else mouse.state &= (~MOUSE_RIGHT_AUTO_CRICK);
	// ????????????
	//if( !WindowMode ){
	//if( 0 <= mouse.nowPoint.x && mouse.nowPoint.x < 640 &&
	//	0 <= mouse.nowPoint.y && mouse.nowPoint.y < 480 ){
	
	// ?��???????
	if( MouseCursorFlag == false ){
		if( mouse.flag == false ){ 
			// ????????����???�I����?
			StockDispBuffer( mouse.nowPoint.x + 16, mouse.nowPoint.y + 16, DISP_PRIO_MOUSE, CG_MOUSE_CURSOR, 0 );
		}
	}
		
}
// ????�k��???? *********************************************************/
void CheckGroupSelect( int no )
{
	int i;
	DISP_INFO 	*pDispInfo;
	DISP_SORT 	*pDispSort;
	/* ???����?????????����??�I?????��??? */
	for( i = 0 ; i < DispBuffer.DispCnt ; i++ ){
	
		pDispInfo = DispBuffer.DispInfo + i;
		pDispSort = DispBuffer.DispSort + i;
		
		// ??????????
		if( pDispInfo->hitFlag == no ){
			// ??????��????
			if( pDispSort->dispPrio >= DISP_PRIO_MENU ){
				// ????����????????????
#ifndef __CARYTEST
				StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
									pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width + 2, 
									pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height + 2, 
									DISP_PRIO_BOX2, BoxColor, 0 );
#else
				StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
									pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width + 2, 
									pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height + 2, 
									DISP_PRIO_BOX2, BoxColor, 0 );
#endif
			}else{
				// ????����????????????
#ifndef __CARYTEST
				StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
									pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width + 2, 
									pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height + 2, 
									DISP_PRIO_BOX, BoxColor, 0 );
									//pDispSort->dispPrio, 250, 0 );
#else
				StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
									pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width + 2, 
									pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height + 2, 
									DISP_PRIO_BOX, BoxColor, 0 );
#endif
			}
		}
	}
}

#if 1
// ����????????????
unsigned char BoxColorTbl[] = { 	
						//255, 255, 255, 255, 255,
						//8,8,8,8,8,
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						250,250,250,250,250, 
						15,15,15,15,15,
						2,2,2,2,2, 
						15,15,15,15,15, 
						//250,250,250,250,250, 
						//8,8,8,8,8,
					};
#else
unsigned char BoxColorTbl[] = { 	
						255,255,255,255,255,255,255,255,255,255,
						255,255,255,255,255,255,255,255,255,255,
						7,7,7,7,7,7,7,7,7,7,
						248,248,248,248,248,248,248,248,248,248,
						0,0,0,0,0,0,0,0,0,0,
						248,248,248,248,248,248,248,248,248,248,
						7,7,7,7,7,7,7,7,7,7,
					};
#endif

/* ???????????�R�e **************************************************/
void HitMouseCursor( void )
{	
	int i;					// ???????
	int strWidth;			// ��???????
	int hitFlag = false;	// ???�R�e???
	static int cnt = 0;		// ?????
	int itemNameColor = FONT_PAL_WHITE;	// ?????��
	isMouseHoverItem = false;
	DISP_SORT 	*pDispSort = DispBuffer.DispSort + DispBuffer.DispCnt - 1;
	DISP_INFO 	*pDispInfo;
	
	// ����?????���??
	if( !transmigrationEffectFlag )
		oneLineInfoFlag = 1;
	else
		oneLineInfoFlag = 0;

	if( cnt >= sizeof( BoxColorTbl ) - 1 ) cnt = 0;
	else cnt++;
	
	// ����?????��@�e
	BoxColor = BoxColorTbl[ cnt ];
#ifdef _ITEM_PATH
	ITEMPATHFLAG = false;
#endif
	/* ????����?????????????????????????�i??*/
	for( i = 0 ; i < FontCnt ; i++ ){
	
		// ???�R�e??????
		if( FontBuffer[ i ].hitFlag == 0 ) continue;
		
		strWidth = GetStrWidth( FontBuffer[ i ].str );

		if( mouse.nowPoint.x <= FontBuffer[ i ].x + strWidth + 2 &&
			FontBuffer[ i ].x - 2 <= mouse.nowPoint.x &&

			mouse.nowPoint.y <= FontBuffer[ i ].y + FONT_SIZE  + 2 &&

			FontBuffer[ i ].y - 2 <= mouse.nowPoint.y ){
			
			// ????��??�k?????
			HitFontNo = i;
			
			// ?????����???
			if( FontBuffer[ i ].hitFlag == 2 ){
				// ????����????????????
				StockBoxDispBuffer( FontBuffer[ i ].x - 3, FontBuffer[ i ].y - 4, 
									FontBuffer[ i ].x + strWidth + 2, 
#ifdef HITFLAG_FONT
									FontBuffer[ i ].y + l_iHight + 4, 
#else
									FontBuffer[ i ].y + FONT_SIZE + 4, 
#endif
									//DISP_PRIO_BOX2, 250, 0 );
									DISP_PRIO_BOX2, BoxColor, 0 );

				SortDispBuffer(); 	// ����???????
			}					
			// ????��??�k???????
			HitDispNo = -1;
			// ??????
			mouse.level = DISP_PRIO_MENU;
			// ?????????
			if( TaskBarFlag == false && oneLineInfoFlag ){
				//ShowBottomLineString(itemNameColor, OneLineInfoStr);
				//the third StockFontBuffer( 8, 460, FONT_PRIO_FRONT, itemNameColor, OneLineInfoStr, 0 );
			}
			// ��?????
			OneLineInfoStr[ 0 ] = NULL;
			return;		// ??????????????�R�e????
		}
	}
	// ????��??�k???????
	HitFontNo = -1;
	
	/* ???����?????????����??�I?????��??? */
	for( i = DispBuffer.DispCnt - 1; i >= 0 ; i--, pDispSort-- ){
		// ����????�V????????????
		pDispInfo = DispBuffer.DispInfo + pDispSort->no;
		
		// ???�R�e??????
		if( pDispInfo->hitFlag == 0 ) continue;
		
		// ?��???????????�Q
		//if( SpriteInfo[ pDispInfo->bmpNo ].lpSurfaceInfo == NULL ) continue;
		// ?????????????????????????
		// ?????????
		if( LoadBmp( pDispInfo->bmpNo ) == false ) continue;
		
		// ?????????
		if( pDispInfo->pAct == NULL ){
			// ��?????�R�e
#ifndef __CARYTEST
			if( mouse.nowPoint.x <= pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width &&
				pDispInfo->x <= mouse.nowPoint.x &&
				mouse.nowPoint.y <= pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height &&
				pDispInfo->y <= mouse.nowPoint.y )
#else
			if( mouse.nowPoint.x <= pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width &&
				pDispInfo->x <= mouse.nowPoint.x &&
				mouse.nowPoint.y <= pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height &&
				pDispInfo->y <= mouse.nowPoint.y )
#endif				
				hitFlag = true;
		}else{
			// ��?????�R�e????????
#ifndef __CARYTEST
			if( mouse.nowPoint.x <= pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width * 0.5 + MOUSE_HIT_SIZE_X * 0.5 &&
				pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width * 0.5 - MOUSE_HIT_SIZE_X * 0.5  <= mouse.nowPoint.x &&
				mouse.nowPoint.y <= pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height &&
				pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height - MOUSE_HIT_SIZE_Y <= mouse.nowPoint.y )
#else
			if( mouse.nowPoint.x <= pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width * 0.5 + MOUSE_HIT_SIZE_X * 0.5 &&
				pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width * 0.5 - MOUSE_HIT_SIZE_X * 0.5  <= mouse.nowPoint.x &&
				mouse.nowPoint.y <= pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height &&
				pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height - MOUSE_HIT_SIZE_Y <= mouse.nowPoint.y )
#endif				
				hitFlag = true;
		}
				
		// ???????
		if( hitFlag == true ){

		#ifdef __ATTACK_MAGIC

			int cnt;

			for( cnt = 0 ; cnt < 20 ; cnt++ )
			{
				if( p_party[cnt] == pDispInfo->pAct )
					break;
			}

			// ���ϵ����±���?6 , 7 , 8 , 9
			// ������ѡ������Ŀǰ��ѡ������ҷ���λ��?
			if( BattleMyNo >= 10 && ( 6 == pDispInfo->hitFlag || 7 == pDispInfo->hitFlag ) )
			{
				hitFlag = false;
				continue;
			}
			// ������ѡ������Ŀǰ��ѡ������ҷ���λ��?
			if( BattleMyNo < 10 && ( 8 == pDispInfo->hitFlag || 9 == pDispInfo->hitFlag ) )
			{
				hitFlag = false;
				continue;
			}
#ifdef _SKILL_ADDBARRIER
			// ֻ��ѡ�ҷ�
			if( BattleMyNo < 10 && ( 10 == pDispInfo->hitFlag || 11 == pDispInfo->hitFlag ) )
			{
				hitFlag = false;
				continue;
			}
			if( BattleMyNo >= 10 && ( 12 == pDispInfo->hitFlag || 13 == pDispInfo->hitFlag ) )
			{
				hitFlag = false;
				continue;
			}
#endif
		#endif

			// ��?����????????
//			StockFontBuffer( pDispInfo->x, pDispInfo->y, FONT_PRIO_FRONT, 0, "Hit", 1 );
			
			// ????��??�k????
			HitDispNo = pDispSort->no;
			
			// ?????����???
			if( pDispInfo->hitFlag >= 2 ){
				// ??????��????
				if( pDispSort->dispPrio >= DISP_PRIO_YES_NO_WND ){
					// ????����????????????
#ifndef __CARYTEST
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height + 2, 
										//DISP_PRIO_BOX2, 250, 0 );
										DISP_PRIO_BOX3, BoxColor, 0 );
#else
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height + 2, 
										DISP_PRIO_BOX3, BoxColor, 0 );
#endif
				}else
				// ??????��????
				if( pDispSort->dispPrio >= DISP_PRIO_MENU ){
					// ????����????????????
#ifndef __CARYTEST
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height + 2, 
										//DISP_PRIO_BOX2, 250, 0 );
										DISP_PRIO_BOX2, BoxColor, 0 );
#else
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height + 2, 
										DISP_PRIO_BOX2, BoxColor, 0 );
#endif
				}else{
					// ????����????????????
#ifndef __CARYTEST
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + SpriteInfo[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + SpriteInfo[ pDispInfo->bmpNo ].height + 2, 
										//DISP_PRIO_BOX, 250, 0 );
										DISP_PRIO_BOX, BoxColor, 0 );
										//pDispSort->dispPrio, 250, 0 );
#else
					StockBoxDispBuffer( pDispInfo->x - 2, pDispInfo->y - 2, 
										pDispInfo->x + g_lpRealAdrn[ pDispInfo->bmpNo ].width + 2, 
										pDispInfo->y + g_lpRealAdrn[ pDispInfo->bmpNo ].height + 2, 
										DISP_PRIO_BOX, BoxColor, 0 );
#endif
				}
				
				// ????�k��?????��?�k��? 
				if( pDispInfo->hitFlag >= 3 ) CheckGroupSelect( pDispInfo->hitFlag );
				SortDispBuffer(); 	// ����???????
			}
			
			// ?????????
			if( TaskBarFlag == false ){
				// ?????????????
				if( pDispInfo->pAct != NULL ){
					// ??????����????
					if( pDispInfo->pAct->atr & ACT_ATR_INFO ){
						// ?????��??
						if( ProcNo == PROC_GAME ){
							// ��???��
							//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%4d/%4d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
							// ??????����????
							// ???????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_PC ){
								// ���q?��@�e
								itemNameColor = pDispInfo->pAct->itemNameColor;
								// ��????
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									// shan add 
									//sprintf( OneLineInfoStr,"%s [%s] Lv��%d  �ͣ�%d��%d  ����%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
									if( pDispInfo->pAct->petName[ 0 ] != NULL )
										if( pc.familyName[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s [%s] Lv:%d  HP:%d/%d  MP:%d   %s  Lv:%d",
												pc.familyName, pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
										else
											sprintf( OneLineInfoStr,"%s  [%s]  Lv:%d  HP:%d/%d  MP:%d   %s  Lv:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
									else
										if( pc.familyName[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s [%s] Lv:%d  HP:%d/%d  MP:%d",
												pc.familyName, pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
										else
											sprintf( OneLineInfoStr,"%s  [%s]  Lv:%d  HP:%d/%d  MP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
								}else{
									// shan add
									//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d  ����%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
									if( pDispInfo->pAct->petName[ 0 ] != NULL )
										if( pc.familyName[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s Lv:%d  HP:%d/%d  MP:%d   %s  Lv:%d",
												pc.familyName, pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
										else
											sprintf( OneLineInfoStr,"%s  Lv:%d HP:%d/%d  MP:%d   %s  Lv:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
									else
										if( pc.familyName[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s Lv:%d  HP:%d/%d  MP:%d",
												pc.familyName, pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
										else
										{
											sprintf( OneLineInfoStr,"%s  Lv:%d HP:%d/%d  MP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pc.mp );
										}
								}
							}else
							// �\????????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_OTHER_PC ){
#ifdef _MOUSE_DBL_CLICK
								if( mouseDblRightOn ) {
									openServerWindow( WINDOW_MESSAGETYPE_MOUSEGETNAME, 0, 0, 0, pDispInfo->pAct->name);
									return;
								}
#endif
								// ���q?��@�e
								itemNameColor = pDispInfo->pAct->itemNameColor;
								// ��????
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									// shan add
									//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
									if( pDispInfo->pAct->petName[ 0 ] != NULL )
										if( pDispInfo->pAct->fmname[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s [%s]  Lv:%d   %s  Lv:%d", pDispInfo->pAct->fmname, pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
										else
											sprintf( OneLineInfoStr,"%s  [%s]  Lv:%d   %s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
									else
										if( pDispInfo->pAct->fmname[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s [%s]  Lv:%d", pDispInfo->pAct->fmname, pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
										else
											sprintf( OneLineInfoStr,"%s  [%s]  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
								}else{
									// shan add
									//sprintf( OneLineInfoStr,"%s  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
									if( pDispInfo->pAct->petName[ 0 ] != NULL )
										if( pDispInfo->pAct->fmname[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s Lv:%d  %s Lv:%d", pDispInfo->pAct->fmname, pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
										else
											sprintf( OneLineInfoStr,"%s  Lv:%d   %s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
									else
										if( pDispInfo->pAct->fmname[0] != NULL )
											sprintf( OneLineInfoStr,"{%s} %s  Lv:%d", pDispInfo->pAct->fmname, pDispInfo->pAct->name, pDispInfo->pAct->level );
										else
											sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
									StockFontBuffer( pDispInfo->x, pDispInfo->y, FONT_PRIO_FRONT, 0, pDispInfo->pAct->name, 1 );
								}
							}else
							// ?????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_PET ){
								// ��????
								isMouseHoverItem = true;
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->freeName, pDispInfo->pAct->level );
								}else{
									sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
								}
							}else
							// ??????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_ITEM ){
								// ���q?��@�e
								isMouseHoverItem = true;
								itemNameColor = pDispInfo->pAct->itemNameColor;
								sprintf( OneLineInfoStr,"%s", pDispInfo->pAct->name );
#ifdef _ITEM_PATH
								ITEMPATHFLAG = true;
#endif
							}else
							// ????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_GOLD ){
								isMouseHoverItem = true;
								sprintf( OneLineInfoStr,"%s", pDispInfo->pAct->name );
							}else
							// ??�\??????��?��??�Y?
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_OTHER ){
								sprintf( OneLineInfoStr,"%s", pDispInfo->pAct->name );
								char a[128];
								sprintf(a,"%s-%d",pDispInfo->pAct->name,hitFlag);
								//if(mouse.onceState==4)
									StockFontBuffer( pDispInfo->x, pDispInfo->y, FONT_PRIO_FRONT, 0, a, 1 );
							}
						}else
						// �P�f??
						if( ProcNo == PROC_BATTLE ){
							// ?????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_PET ){
								// ��????
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->freeName, pDispInfo->pAct->level );
								}else{
									sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
								}
							}else
							// ??????
							if( pDispInfo->pAct->atr & ACT_ATR_TYPE_ITEM ){
								// ���q?��@�e
								//itemNameColor = pDispInfo->pAct->itemNameColor;
								sprintf( OneLineInfoStr,"%s", pDispInfo->pAct->name );
							}else
							// ?�P?????????
							if( BattleMyNo < 20 ){
								// ����???????����
								if( ( (ATR_EQU *)pDispInfo->pAct->pYobi )->place_no == BattleMyNo ){
									// ��????
									if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
										// Robin 0728 ride Pet
										if( pDispInfo->pAct->onRide == 1 )
											sprintf( OneLineInfoStr,"%s [%s] Lv:%d  HP:%d/%d MP:%d  %s Lv:%d HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pDispInfo->pAct->mp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp, pDispInfo->pAct->petMaxHp );
										else
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d HP:%d/%d  MP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pDispInfo->pAct->mp );
									}else{
										// shan add
										//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d  ����%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, BattleMyMp );
										//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d  ����%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pDispInfo->pAct->mp );
										// Robin 0728
										if( pDispInfo->pAct->onRide == 1 )
											sprintf( OneLineInfoStr,"%s  Lv:%d  HP:%d/%d  MP:%d   %s  Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pDispInfo->pAct->mp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp, pDispInfo->pAct->petMaxHp );
										else
											sprintf( OneLineInfoStr,"%s  Lv:%d HP:%d/%d  MP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp, pDispInfo->pAct->mp );
									}
								}
								else
								// �s��????����
								if( ( (ATR_EQU *)p_party[ BattleMyNo ]->pYobi )->group_flg == ( (ATR_EQU *)pDispInfo->pAct->pYobi )->group_flg ){
									// ��???��
									//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%4d/%4d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									// ��????
									if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
										// shan add
										//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
										// Robin 0728
										if( pDispInfo->pAct->onRide == 1 )
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d  HP:%d/%d   %s Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp, pDispInfo->pAct->petMaxHp);
										else
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									}else{
										// shan add
										//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
										// Robin 0728
										if( pDispInfo->pAct->onRide == 1 )
											sprintf( OneLineInfoStr,"%s  Lv:%d  HP:%d/%d   %s Lv:%d HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp, pDispInfo->pAct->petMaxHp );
										else
											SDL_Log("Jerry the level is %d", pDispInfo->pAct->level);
											sprintf( OneLineInfoStr,"%s  Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									}
								}else{ // ��???
									// ��????
									if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
										// shan add
										//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
										// Robin
										if( pDispInfo->pAct->onRide == 1 )
#ifdef _DEBUG
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d  HP:%d   %s  Lv:%d HP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp );
#else
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d   %s  Lv:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
#endif
										else
#ifdef _DEBUG
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
#else
											SDL_Log("Jerry the level is %d", pDispInfo->pAct->level);
											sprintf( OneLineInfoStr,"%s [%s]  Lv:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
#endif
									}else{
										// shan add
										//sprintf( OneLineInfoStr,"%s  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
										// Robin
										if( pDispInfo->pAct->onRide == 1 )
#ifdef _DEBUG
											sprintf( OneLineInfoStr,"%s  Lv:%d HP:%d  %s  Lv:%d HP:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel, pDispInfo->pAct->petHp );
#else
											sprintf( OneLineInfoStr,"%s  Lv:%d   %s  Lv:%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level,
												pDispInfo->pAct->petName, pDispInfo->pAct->petLevel );
#endif
										else
#ifdef _DEBUG
											sprintf( OneLineInfoStr,"%s  Lv:%d  HP:%d/%d",
												pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
#else
											sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
#endif
									}
								}
							}else{	// ?�P?????
#ifdef _DEBUG
								// ��????
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									// shan add
									//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									sprintf( OneLineInfoStr,"%s [%s]  Lv:%d  HP:%d/%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
								}else{
									// shan add
									//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									sprintf( OneLineInfoStr,"%s  Lv:%d  HP:%d/%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									//sprintf( OneLineInfoStr,"%s  Lv��%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
								}
#else
								// ��????
								if( pDispInfo->pAct->freeName[ 0 ] != NULL ){
									//sprintf( OneLineInfoStr,"%s [%s]  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									// shan add
									sprintf( OneLineInfoStr,"%s [%s]  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->freeName, pDispInfo->pAct->level );
								}else{
									//sprintf( OneLineInfoStr,"%s  Lv��%d  �ͣ�%d��%d", pDispInfo->pAct->name, pDispInfo->pAct->level, pDispInfo->pAct->hp, pDispInfo->pAct->maxHp );
									// shan add
									sprintf( OneLineInfoStr,"%s  Lv:%d", pDispInfo->pAct->name, pDispInfo->pAct->level );
								}
#endif
							}
						}
					}
				}
                if( oneLineInfoFlag) {
					//SDL_Log("Jerry OnLineInfoStr is %s", OneLineInfoStr);
					
					ShowBottomLineString(itemNameColor, OneLineInfoStr);
				}
					//
			}
			// ��?????
			OneLineInfoStr[ 0 ] = NULL;
			
			// ??????
			mouse.level = pDispSort->dispPrio;
			return;
		}
	}
	// ??????
	mouse.level = DISP_PRIO_TILE;
	// ????��??�k???????
	HitDispNo = -1;
	
	// ?????????
	if( TaskBarFlag == false ){
		//ShowBottomLineString(itemNameColor, OneLineInfoStr);
	}
	// ��?????
	OneLineInfoStr[ 0 ] = NULL;
	
}

void ShowBottomLineString(int iColor, char* lpstr)
{
	StockFontBuffer(8, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, iColor, lpstr, 0);
}

