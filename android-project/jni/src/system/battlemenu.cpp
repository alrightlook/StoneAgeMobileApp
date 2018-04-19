/************************/
/*	battleMenu.cpp		*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
//#include "../caryIme.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/netmain.h"
#include "../systeminc/pc.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/t_music.h"
#include "../systeminc/menu.h"
#include "../systeminc/map.h"
#include "../systeminc/field.h"
#include "../systeminc/character.h"

// ?????
enum{ 
	BATTLE_ATTACK,
	BATTLE_JUJUTU,
	BATTLE_CAPTURE,
	BATTLE_ITEM,
	BATTLE_WAZA,
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
	BATTLE_PROWAZA,
#endif
};

// ???
#define BATTLE_MENU				( 1 << 0 )
#define BATTLE_MENU_PET			( 1 << 1 )

// ????????
#define BATTLE_MENU_SUU					9
#define BATTLE_MENU_FONTS				9
#define BATTLE_MENU_JUJUTSU_FONTS		6
#define BATTLE_MENU_ITEM_FONTS			6
#define BATTLE_MENU_PET_FONTS			7
#define BATTLE_MENU_WAZA_FONTS			9

// ????????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
#define BATTLE_BUTTON_SUU	10
#else
#define BATTLE_BUTTON_SUU	9
#endif

// ???????????
char BattleCmd[ BATTLE_COMMAND_SIZE ];
// ?????????????
char BattleCmdBak[ BATTLE_BUF_SIZE ][ BATTLE_COMMAND_SIZE ];
// ????????????????
int BattleCmdReadPointer;
int BattleCmdWritePointer;

// ?????t????
char BattleStatus[ BATTLE_COMMAND_SIZE ];
// ?????t??????????
char BattleStatusBak[ BATTLE_BUF_SIZE ][ BATTLE_COMMAND_SIZE ];
// ?????t?????????
int BattleStatusReadPointer;
int BattleStatusWritePointer;
// ?????????k?
int BattleMyNo = 0;
// ?P?f?????????
int BattleMyMp;
// ?P?f?????????
int BattleEscFlag = false;

// ????????????
int BattlePetStMenCnt;

// ?????
int BattleBpFlag = false;
// ?????k?
int BattleCmdNo = -1;
// ????k?
int BattleJujutuNo;
// ??k?
int BattleWazaNo;
// ??P????k???????
int battlePetNoBak = -2;
// ??P????k????????
int battlePetNoBak2 = -2;
// ?????k?
int BattleItemNo;

// ??????`?????
int battlePlayerEscFlag = false;

// ?????V??????
int BattleAnimFlag;
// ??????????
bool BattleTurnReceiveFlag;

// ?????????????k?
int BattleCliTurnNo;
// ???????????k?
int BattleSvTurnNo;


// ?P?f??????????
bool BattleResultWndFlag = false;

// ???????
unsigned long BattleCntDown;
unsigned long BattleIntervalCnt;
int AI;
unsigned long PauseAI;
// ??????????????
bool BattleCntDownFlag = false;

// ???????
static bool battleButtonFlag[ BATTLE_BUTTON_SUU ];
static int battleButtonDispNo[ BATTLE_BUTTON_SUU ];
static int battleMenuJujutsuFontNo[ BATTLE_MENU_JUJUTSU_FONTS ];
static int battleMenuItemFontNo[ BATTLE_MENU_ITEM_FONTS ];
static int battleMenuPetFontNo[ BATTLE_MENU_PET_FONTS ];
static int battleMenuWazaFontNo[ BATTLE_MENU_WAZA_FONTS ];
int battleMenuFlag = false;				// ????????????????????
static int battleMenuReturn = false;			// ??????????????
static int battlePetMenuFlag = false;			// ?????????????
int battlePetButtonFlag = false;			// ??????????????
static int battleTimeUpFlag = false;
static int battleTargetSelectFlag = false;
int battleMenuFlag2 = false;	// ???????????v??
int battleButtonBak = -1;		// ?q???????
static int battleButtonBak2 = -1;		// ?q????????
int battleWazaTargetBak = -1;		// ?q????k?

// ??????????????
static ACTION *pActInfoWnd;
static ACTION *pActWnd = NULL;

// ??????????
static int buttonX, buttonY, buttonA;

#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
int wonflag = 0 ; 
int prouseskill = -1 ; 
int SkillWnd2 = 0 ; 
static int AdvanceSkillBtn[ 26 ] ; 
static int AssitSkillBtn[ 26 ]; 
static int BattleSkillBtn[ 26 ]; 
#endif

#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif
// ?????????? ********************************************************/
void InitBattleMenu( void )
{
	int i;
	// ???
	for( i = 0 ; i < BATTLE_BUTTON_SUU ; i++ ) battleButtonDispNo[ i ] = -2;
	// ????
	for( i = 0 ; i < BATTLE_MENU_JUJUTSU_FONTS ; i++ ) battleMenuJujutsuFontNo[ i ] = -2;
	// ????
	for( i = 0 ; i < BATTLE_MENU_ITEM_FONTS ; i++ ) battleMenuItemFontNo[ i ] = -2;
	// ???
	for( i = 0 ; i < BATTLE_MENU_PET_FONTS ; i++ ) battleMenuPetFontNo[ i ] = -2;
	// ?
	for( i = 0 ; i < BATTLE_MENU_WAZA_FONTS ; i++ ) battleMenuWazaFontNo[ i ] = -2;

	battleMenuFlag = false;				// ??????????
	battleMenuReturn = true;			// ????????????
//	battleButtonBak = -1;				// ?q???????
	battleButtonBak2 = -1;				// ?q????????
	BattleEscFlag = false;				// ?P?f????????????
	// ?P?f??????????
	BattleResultWndFlag = false;
	// ???????u???
	ClearBattleButton();
	// ?????????????
	battleButtonBak = 0;
#ifdef __AI
	extern int AI_First;
	AI_First = 1;
#else
	if( AI != AI_NONE)
		battleButtonBak = -1;
#endif
	pActWnd = NULL;				// ????????????
	pActInfoWnd = NULL;
	// ?????V??v????????
	if( battlePetNoBak != -2 ){
		battlePetButtonFlag = false;			// ????????
	}else{
		battlePetButtonFlag = true;			// ????????
	}

}

// ??????K???????? ********************************************/
int CheckBattle1P2P( void )
{
	int i, cnt = 0;
	// ?????????
	for( i = 0 ; i < 20 ; i++ ){
		// ??????????
		if( p_party[ i ]->func == NULL ) continue;
		// ???????
		if( ( (ATR_EQU *)p_party[ i ]->pYobi )->pet_ok == true ) cnt++;
	}
	if( cnt >= 2 ) return 2;
	else return 1;
}

// ?????V??????????? ********************************************/
void CheckBattleAnimFlag( void )
{
	int i;
	// ???????????????????i????u???????
	if( BattleCliTurnNo != BattleSvTurnNo ){ 
		// ?????????
		for( i = 0 ; i < 20 ; i++ ){
			// ????????R?e
			p_party[ i ]->atr |= ACT_ATR_BTL_CMD_END;
		}
		return;
	}
	// ?????????
	for( i = 0 ; i < 20 ; i++ ){
		// ????????R?e
		if( ( BattleAnimFlag >> i ) & 0x00000001 )
			p_party[ i ]->atr |= ACT_ATR_BTL_CMD_END;
	}
}

// ?????V?????????? ********************************************/
void InitBattleAnimFlag( void )
{
	int i;
	// ?????????
	for( i = 0 ; i < 20 ; i++ ){
		// ????????R?e
		p_party[ i ]->atr &= ~ACT_ATR_BTL_CMD_END;
	}
}

// ??????????? *******************************************************/
int CheckPetSuu( void )
{
	int i,cnt = 0;
	for( i = 0 ; i < 5 ; i++ ){
		if( pet[ i ].useFlag == true ) cnt++;
	}
	return cnt;
}

// ????R?e??????? *****************************************************/
void ClearBoxFlag( void )
{
	int i;
	// ????R?e???????
	for( i = 0 ; i < 20 ; i++ ){
		// ??????????
		if( p_party[ i ]->func == NULL ) continue;
		// ????R?e??????????????

#ifdef __ATTACK_MAGIC
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL1;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL2;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL3;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL4;
#endif
#ifdef _SKILL_ADDBARRIER
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL5;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL6;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL7;
		p_party[i]->atr &= ~ACT_ATR_HIT_BOX_COL8;
#endif


		p_party[ i ]->atr &= ~ACT_ATR_HIT_BOX;
		p_party[ i ]->atr &= ~ACT_ATR_HIT_BOX_ALL1;
		p_party[ i ]->atr &= ~ACT_ATR_HIT_BOX_ALL2;
		p_party[ i ]->atr &= ~ACT_ATR_HIT_BOX_ALL3;
		p_party[ i ]->atr |= ACT_ATR_HIT;
		// ????R?e?k????
		p_party[ i ]->hitDispNo = -2;
	}
}

// ???????????R?e ***************************************************/
int CheckBattelTarget( void )
{
	int i;
	// ?????????????
	for( i = 0 ; i < 20 ; i++ ){
		// ??????????
		if( p_party[ i ]->func == NULL ) continue;
		// ?k??????

#ifdef __ATTACK_MAGIC
#ifdef _SKILL_ADDBARRIER
		if( !( p_party[i]->atr & ACT_ATR_HIT_BOX ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL1 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL2 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL3 ) && 
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL1 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL2 ) && 
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL3 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL4 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL5 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL6 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL7 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL8 )) continue;
#else
		if( !( p_party[i]->atr & ACT_ATR_HIT_BOX ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL1 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL2 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_ALL3 ) && 
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL1 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL2 ) && 
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL3 ) &&
			!( p_party[i]->atr & ACT_ATR_HIT_BOX_COL4 ) ) continue;
#endif
#else
		if( !( p_party[ i ]->atr & ACT_ATR_HIT_BOX ) &&
			!( p_party[ i ]->atr & ACT_ATR_HIT_BOX_ALL1 ) &&
			!( p_party[ i ]->atr & ACT_ATR_HIT_BOX_ALL2 ) &&
			!( p_party[ i ]->atr & ACT_ATR_HIT_BOX_ALL3 ) ) continue;

#endif

#ifdef _CANT_ATK
		if( !((pc.sideNo*10) <= i && ((pc.sideNo+1)*10) > i) )
#endif
		// ???????
		if( p_party[ i ]->hitDispNo == HitDispNo )
			return i;
	}	
	// ????????
	return -1;
}

// ???????u???  *****************************************************/
void ClearBattleButton( void )
{
	// ????
	memset( battleButtonFlag, 0, sizeof( battleButtonFlag ) );
}

// ???????????????? *******************************************/
void BattleButtonOff( void )
{
	play_se( 217, 320, 240 );
	// ?????????????
	DeathAction( pActWnd );
	pActWnd = NULL;				// ????????????
	// ???????????????
	battleTargetSelectFlag = false;
	// ?????k?
	BattleCmdNo = -1;

#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
	wonflag = 0 ; 
#endif

	// ???????u???
	ClearBattleButton();
	// ????R?e???????
	ClearBoxFlag();
}

// ?????????? *******************************************************/
void InitItem2( int x, int y )
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

// ?o?????????? ***********************************************************/
void HpMeterDisp( int no )
{
	int meterX = p_party[ no ]->x;
	int meterY = p_party[ no ]->y - 80;
	int graNo;
	// ?V??????????????
	if( p_party[ no ]->func == NULL || p_party[ no ]->hp <= 0 ) return;
	// ?????
	if( p_party[ no ]->atr & ACT_ATR_TRAVEL ) return;
	// ??????
	if( no == BattleMyNo ){
		// ?????
		StockDispBuffer( meterX, meterY, DISP_PRIO_IME1, CG_BATTLE_BAR_PLAYER_2, 0 );
		// ?o??????
		StockBoxDispBuffer( meterX - 21,
							meterY - 3,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->hp / (double)p_party[ BattleMyNo ]->maxHp ) * 40.0 ),
							meterY - 3,
							DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		StockBoxDispBuffer( meterX - 21,
							meterY - 2,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->hp / (double)p_party[ BattleMyNo ]->maxHp ) * 40.0 ),
							meterY - 2,
							DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		// ??????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		StockBoxDispBuffer( meterX - 21,
							meterY + 1,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->mp / (double)p_party[ BattleMyNo ]->maxMp ) * 40.0 ),
							meterY + 1,
							DISP_PRIO_IME2, SYSTEM_PAL_YELLOW, 2 );
		StockBoxDispBuffer( meterX - 21,
							meterY + 2,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->mp / (double)p_party[ BattleMyNo ]->maxMp ) * 40.0 ),
							meterY + 2,
							DISP_PRIO_IME2, SYSTEM_PAL_YELLOW, 2 );
#else
		StockBoxDispBuffer( meterX - 21,
							meterY + 1,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->mp / 100.0 ) * 40.0 ),
							meterY + 1,
							DISP_PRIO_IME2, SYSTEM_PAL_YELLOW, 2 );
		StockBoxDispBuffer( meterX - 21,
							meterY + 2,
							meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->mp / 100.0 ) * 40.0 ),
							meterY + 2,
							DISP_PRIO_IME2, SYSTEM_PAL_YELLOW, 2 );
#endif
		//cary ????????????? 
		if( p_party[ BattleMyNo ]->onRide > 0 ){
			meterY += 25;
			meterX += 20;
			// ?????
			StockDispBuffer( meterX , meterY, DISP_PRIO_IME1, CG_BATTLE_BAR_PET_2, 0 );
			// ?o?????????????
			StockBoxDispBuffer( meterX - 21,
								meterY - 1,
								meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->petHp / (double)p_party[ BattleMyNo ]->petMaxHp ) * 40.0 ),
								meterY - 1,
								DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
			// ?o?????????????
			StockBoxDispBuffer( meterX - 21,
								meterY + 0,
								meterX - 21 + (int)( ( (double)p_party[ BattleMyNo ]->petHp / (double)p_party[ BattleMyNo ]->petMaxHp ) * 40.0 ),
								meterY + 0,
								DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		}
	}else{	// ????????
		if( no == BattleMyNo + 5 ) graNo = CG_BATTLE_BAR_PET_2;
		else graNo = CG_BATTLE_BAR_PET;
		// ?????
		StockDispBuffer( meterX , meterY, DISP_PRIO_IME1, graNo, 0 );
		// ?o?????????????
		StockBoxDispBuffer( meterX - 21,
							meterY - 1,
							meterX - 21 + (int)( ( (double)p_party[ no ]->hp / (double)p_party[ no ]->maxHp ) * 40.0 ),
							meterY - 1,
							DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		// ?o?????????????
		StockBoxDispBuffer( meterX - 21,
							meterY + 0,
							meterX - 21 + (int)( ( (double)p_party[ no ]->hp / (double)p_party[ no ]->maxHp ) * 40.0 ),
							meterY + 0,
							DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		//cary ????????????? 
		if( p_party[ no ]->onRide > 0 ){
			meterY += 25;
			meterX += 20;
			// ?????
			StockDispBuffer( meterX , meterY, DISP_PRIO_IME1, CG_BATTLE_BAR_PET, 0 );
			// ?o?????????????
			StockBoxDispBuffer( meterX - 21,
								meterY - 1,
								meterX - 21 + (int)( ( (double)p_party[ no ]->petHp / (double)p_party[ no ]->petMaxHp ) * 40.0 ),
								meterY - 1,
								DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
			// ?o?????????????
			StockBoxDispBuffer( meterX - 21,
								meterY + 0,
								meterX - 21 + (int)( ( (double)p_party[ no ]->petHp / (double)p_party[ no ]->petMaxHp ) * 40.0 ),
								meterY + 0,
								DISP_PRIO_IME2, SYSTEM_PAL_GREEN, 2 );
		}
	}
}

// ???q????? ************************************************************************/
void BattleNameDisp( void )
{
	int i;
	int color = FONT_PAL_WHITE;
	// ???q????????????
	if( nameOverTheHeadFlag != true ) return;
	// ?????????????
	for( i = 0 ; i < 20 ; i++ ){
		// ??????????
		if( p_party[ i ]->func == NULL ) continue;
		// ??????
		if( i == BattleMyNo || i == ( BattleMyNo + 5 ) )
			color = FONT_PAL_YELLOW;
		else
			color = FONT_PAL_WHITE;
		// ???q?????
		StockFontBuffer( p_party[ i ]->x - GetStrWidth( p_party[ i ]->name )/2, p_party[ i ]->y - 10, FONT_PRIO_BACK, color, p_party[ i ]->name, 0 );
	}	
}


void BattleButtonAttack( void )
{
	int i, j, k, bak, hitBoxAll;
	if( HitDispNo == battleButtonDispNo[ 0 ] || battleButtonBak2 == 0 ){

		if( mouse.onceState & MOUSE_LEFT_CRICK || battleButtonBak2 == 0 ){
			bak = battleButtonFlag[ 0 ];
			BattleButtonOff();

			if( bak == false ){

				battleTargetSelectFlag = true;

				BattleCmdNo = BATTLE_ATTACK;

				battleButtonFlag[ 0 ] = true;

				battleButtonBak = 0;

				battleButtonBak2 = -1;

				if( BattleBpFlag & BATTLE_BP_BOOMERANG ){

					hitBoxAll = ACT_ATR_HIT_BOX_ALL1;
					j = 0;

					for( i = 0 ; i < 4 ; i++ ){

						if( !( j <= BattleMyNo && BattleMyNo < j + 5 ) ){

							for( k = j ; k < j + 5 ; k++ ){

								if( p_party[ k ]->func == NULL ) continue;

								if( p_party[ k ]->hp <= 0 ) continue;

								if( p_party[ k ]->atr & ACT_ATR_TRAVEL ) continue;

								p_party[ k ]->atr |= hitBoxAll;
							}

							hitBoxAll <<= 1;
						}
						j += 5;
					}
				}else{

					for( i = 0 ; i < 20 ; i++ ){

						if( i == BattleMyNo ) continue;

						if( p_party[ i ]->func == NULL ) continue;

						if( p_party[ i ]->hp <= 0 ) continue;

						p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
					}
				}
			}
		}
		strcpy( OneLineInfoStr,"攻击");
	}
}


void BattleButtonJujutsu( void )
{
	int i, x, y, bak;
	char moji[ 256 ];
	if( HitDispNo == battleButtonDispNo[ 1 ] || battleButtonBak2 == 1 ){
#ifdef _MAGIC_NOCAST
      if( NoCastFlag == false )
	  {
#endif

		if( mouse.onceState & MOUSE_LEFT_CRICK || battleButtonBak2 == 1 ){
			bak = battleButtonFlag[ 1 ];
			BattleButtonOff();

			if( bak == false ){

				battleButtonFlag[ 1 ] = true;

				BattleCmdNo = BATTLE_JUJUTU;

				for( i = 0 ; i < BATTLE_MENU_JUJUTSU_FONTS ; i++ ) battleMenuJujutsuFontNo[ i ] = -2;

				pActWnd = MakeWindowDisp( 361, 144, 271, 281, 0, -1 );

				battleButtonBak = 1;

				battleButtonBak2 = -1;
			}
		}

		strcpy( OneLineInfoStr, "吟唱咒术。");
#ifdef _MAGIC_NOCAST
	  }
	  else
	  {
        strcpy( OneLineInfoStr, "沉默中无法吟唱咒术。");
	  }
#endif
	}
#ifdef _MAGIC_NOCAST
					if(!( NoCastFlag == false )){
			            battleButtonFlag[ 1 ] = false;
					}
#endif

	if( battleButtonFlag[ 1 ] == true ){

		if( pActWnd != NULL ){

			if( pActWnd->hp > 0 ){
				x = pActWnd->x + 74;
				y = pActWnd->y + 208;

#ifdef _READ16BITBMP
				if(g_bUseAlpha) StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my + 7, DISP_PRIO_MENU, CG_JUJUTU_WND, 1 );
				else
#endif
				StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my + 10, DISP_PRIO_MENU, CG_JUJUTU_WND, 1 );

				for( i = 0 ; i < 5 ; i++ ){

					if( HitFontNo == battleMenuJujutsuFontNo[ i ] && magic[ i ].useFlag == true ){

						char *splitPoint = magic[ i ].memo;

						sprintf( moji,"%2d/%2d", magic[ i ].mp, p_party[ BattleMyNo ]->mp );
						StockFontBuffer( pActWnd->x + 74 + 81, pActWnd->y + 183, FONT_PRIO_FRONT, 0, moji, 0 );

						while( 1 ){

							if( strlen( splitPoint ) > 22 ){
								strncpy( moji, splitPoint, 22 );
								moji[ 22 ] = NULL;

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

						StockDispBuffer( pActWnd->x + 37, pActWnd->y + 220, DISP_PRIO_ITEM, pc.item[ i ].graNo, 0 );

						if( mouse.onceState & MOUSE_LEFT_CRICK ){

							if( magic[ i ].field == MAGIC_FIELD_MAP || magic[ i ].mp > p_party[ BattleMyNo ]->mp ){
								play_se( 220, 320, 240 );
							}else{ 
								BattleJujutuNo = i;

								battleTargetSelectFlag = true;

								switch( magic[ i ].target ){
								case MAGIC_TARGET_MYSELF:

									p_party[ BattleMyNo ]->atr |= ACT_ATR_HIT_BOX;

									battleTargetSelectFlag = true;
									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;
									
								case MAGIC_TARGET_OTHER:

									for( i = 0 ; i < 20 ; i++ ){

										if( p_party[ i ]->func == NULL ) continue;

										if( magic[ BattleJujutuNo ].deadTargetFlag == false )
											if( p_party[ i ]->hp <= 0 ) continue;

										p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
									}

									battleTargetSelectFlag = true;

									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;

								#ifdef  __ATTACK_MAGIC

								case MAGIC_TARGET_SINGLE:

									if( BattleMyNo < 10 )
									{
										for( i = 10 ; i < 20 ; i++ )
										{
											if( p_party[i]->func == NULL )
												continue;

											if( false == magic[ BattleJujutuNo ].deadTargetFlag )
												if( p_party[ i ]->hp <= 0 )
													continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
										}
									}
									else
									{
										for( i = 0 ; i < 10 ; i++ )
										{
											if( p_party[i]->func == NULL )
												continue;

											if( false == magic[ BattleJujutuNo ].deadTargetFlag )
												if( p_party[ i ]->hp <= 0 )
													continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
										}
									}

									battleTargetSelectFlag = true;
									DeathAction( pActWnd );
									pActWnd	= NULL;
									ClearBattleButton();
									BattleCmdNo = BATTLE_JUJUTU;
									play_se( 217 , 320 , 240 );

									break;

								case MAGIC_TARGET_ONE_ROW:

									for( i = 0 ; i < 5 ; i++ )
									{
										if( p_party[i]->func == NULL )
											continue;

										if( false == magic[BattleJujutuNo].deadTargetFlag )
											if( p_party[i]->hp <= 0 )
												continue;

										if( p_party[i]->atr & ACT_ATR_TRAVEL )
											continue;

										// ????????
										p_party[i]->atr |= ACT_ATR_HIT_BOX_COL4;
									}

									for( i = 5 ; i < 10 ; i++ )
									{
										if( p_party[i]->func == NULL )
											continue;

										if( false == magic[BattleJujutuNo].deadTargetFlag )
											if( p_party[i]->hp <= 0 )
												continue;

										if( p_party[i]->atr & ACT_ATR_TRAVEL )
											continue;

										// ????????
										p_party[i]->atr |= ACT_ATR_HIT_BOX_COL3;
									}

									for( i = 10 ; i < 15 ; i++ )
									{
										if( p_party[i]->func == NULL )
											continue;

										if( false == magic[BattleJujutuNo].deadTargetFlag )
											if( p_party[i]->hp <= 0 )
												continue;

										if( p_party[i]->atr & ACT_ATR_TRAVEL )
											continue;

										// ????????
										p_party[i]->atr |= ACT_ATR_HIT_BOX_COL1;
									}

									for( i = 15 ; i < 20 ; i++ )
									{
										if( p_party[i]->func == NULL )
											continue;

										if( false == magic[BattleJujutuNo].deadTargetFlag )
											if( p_party[i]->hp <= 0 )
												continue;

										if( p_party[i]->atr & ACT_ATR_TRAVEL )
											continue;

										// ????????
										p_party[i]->atr |= ACT_ATR_HIT_BOX_COL2;
									}

									battleTargetSelectFlag = true;
									DeathAction( pActWnd );
									pActWnd = NULL;
									ClearBattleButton();
									BattleCmdNo = BATTLE_JUJUTU;
									play_se( 217 , 320 , 240 );

									break;
								
								case MAGIC_TARGET_ALL_ROWS:

									if( BattleMyNo >= 10 )
									{
										for( i = 0 ; i < 10 ; i++ )
										{
											if( p_party[ i ]->func == NULL )
												continue;

											if( false == magic[ BattleJujutuNo ].deadTargetFlag )
												if( p_party[ i ]->hp <= 0 )
													continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL )
												continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
										}
									}
									else
									{
										for( i = 10 ; i < 20 ; i++ )
										{
											if( p_party[ i ]->func == NULL )
												continue;

											if( false == magic[ BattleJujutuNo ].deadTargetFlag )
												if( p_party[ i ]->hp <= 0 )
													continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL )
												continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
										}
									}

									battleTargetSelectFlag = true;
									DeathAction( pActWnd );
									pActWnd = NULL;
									ClearBattleButton();
									BattleCmdNo = BATTLE_JUJUTU;
									play_se( 217 , 320 , 240 );

									break;

								#endif

								case MAGIC_TARGET_ALLMYSIDE:

									if( BattleMyNo < 10 ){

										for( i = 0 ; i < 10 ; i++ ){

											if( p_party[ i ]->func == NULL ) continue;

											if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
										}
									}else{

										for( i = 10 ; i < 20 ; i++ ){

											if( p_party[ i ]->func == NULL ) continue;

											if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
										}
									}

									battleTargetSelectFlag = true;

									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;
									
								case MAGIC_TARGET_ALLOTHERSIDE:

									if( BattleMyNo >= 10 ){

										for( i = 0 ; i < 10 ; i++ ){

											if( p_party[ i ]->func == NULL ) continue;

											if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
										}
									}else{

										for( i = 10 ; i < 20 ; i++ ){

											if( p_party[ i ]->func == NULL ) continue;

											if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

											if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

											p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
										}
									}

									battleTargetSelectFlag = true;

									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;
									
								case MAGIC_TARGET_ALL:

									for( i = 0 ; i < 20 ; i++ ){

										if( p_party[ i ]->func == NULL ) continue;

										if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

										if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

										p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
									}

									battleTargetSelectFlag = true;

									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;
									
								case MAGIC_TARGET_WHOLEOTHERSIDE:

									for( i = 0 ; i < 10 ; i++ ){

										if( p_party[ i ]->func == NULL ) continue;

										if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

										if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

										p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
									}

									for( i = 10 ; i < 20 ; i++ ){

										if( p_party[ i ]->func == NULL ) continue;

										if( magic[ BattleJujutuNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

										if( p_party[ i ]->atr & ACT_ATR_TRAVEL ) continue;

										p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
									}

									battleTargetSelectFlag = true;

									DeathAction( pActWnd );
									pActWnd = NULL;

									ClearBattleButton();

									BattleCmdNo = BATTLE_JUJUTU;

									play_se( 217, 320, 240 );
									break;
								}
							}
						}
					}
				}

				if( mouse.onceState & MOUSE_LEFT_CRICK ){

					if( HitDispNo == battleMenuJujutsuFontNo[ 5 ] ){
						BattleButtonOff();				
					}
				}

				if( pActWnd != NULL ){
					int flag = false;
					int col;

					x = pActWnd->x + 30;
					y = pActWnd->y + 35;

					for( i = 0 ; i < 5 ; i++ ){

						if( magic[ i ].useFlag == true ){
							col = FONT_PAL_WHITE;
							if( magic[ i ].mp > p_party[ BattleMyNo ]->mp ) col = FONT_PAL_RED;	// ????K????
							if( magic[ i ].field == MAGIC_FIELD_MAP ) col = FONT_PAL_GRAY;	// ?????????????

							CenteringStr( magic[ i ].name, moji, MAGIC_NAME_LEN );
							battleMenuJujutsuFontNo[ i ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, col, moji, 2 ); y += 28;
							flag = true;
						}
					}
					if( flag == false ) StockFontBuffer( x, y, FONT_PRIO_FRONT, 0, "???????", 0 );

					battleMenuJujutsuFontNo[ 5 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, pActWnd->y + 262 + 10, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
				}
			}
		}
	}
}

// ??????? *****************************************************************/
void BattleButtonCapture( void )//???????
{
	int i, bak;
	if( HitDispNo == battleButtonDispNo[ 2 ] || battleButtonBak2 == 2 ){
		// ???????????
		if( CheckPetSuu() >= 5 ){
			// ??????
			strcpy( OneLineInfoStr, "不能再捕获更多的宠物了。");
			// ?????????
			if( mouse.onceState & MOUSE_LEFT_CRICK ){
				// ???
				play_se( 220, 320, 240 );
			}
			// ?q????????
			battleButtonBak2 = -1;
		}else{
			// ??????
			strcpy( OneLineInfoStr, "捕捉。");
			// ?????????
			if( mouse.onceState & MOUSE_LEFT_CRICK || battleButtonBak2 == 2 ){
				bak = battleButtonFlag[ 2 ];	// ??????
				BattleButtonOff();				// ??????u???????
				// ?????????
				if( bak == false ){
					// ??????????????
					battleTargetSelectFlag = true;
					// ?????k?
					BattleCmdNo = BATTLE_CAPTURE;
					// ?????????
					battleButtonFlag[ 2 ] = true;
					// ????????
					battleButtonBak = 2;
					// ?q????????
					battleButtonBak2 = -1;
					// ????R?e????????
					for( i = 0 ; i < 20 ; i++ ){
						// ??????????
						if( p_party[ i ]->func == NULL ) continue;
						// ?s??????
						if( ( (ATR_EQU *)p_party[ i ]->pYobi )->group_flg == ( (ATR_EQU *)p_party[ BattleMyNo ]->pYobi )->group_flg ) continue;
						// ????????
						if( i == BattleMyNo ) continue;
						// ??????
						if( p_party[ i ]->hp <= 0 ) continue;
						// ????R?e?????????????
						p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
					}
				}
			}
		}
	}
}


void BattleButtonHelp( void )
{
	if( HitDispNo == battleButtonDispNo[ 3 ] && battleButtonFlag[ 3 ] == false ){

		strcpy( OneLineInfoStr, "求救。");
		if( mouse.onceState & MOUSE_LEFT_CRICK ){

			if( NoHelpFlag == true ){

				play_se( 220, 320, 240 );
			}else{

				if( helpFlag == true ){
					helpFlag = false;
				}else{
					helpFlag = true;
				}

				if( bNewServer)
					lssproto_HL_send( sockfd, helpFlag );
				else
					old_lssproto_HL_send( sockfd, helpFlag );

				play_se( 217, 320, 240 );
			}
		}
	}
}


void BattleButtonGuard( void )
{
	if( HitDispNo == battleButtonDispNo[ 4 ] && battleButtonFlag[ 4 ] == false ){

		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			BattleButtonOff();

			battleButtonFlag[ 4 ] = true;

			if( bNewServer)
				lssproto_B_send( sockfd, "G" );
			else
				old_lssproto_B_send( sockfd, "G" );

			battleMenuReturn = true;

			battleButtonBak = -1;

			battleButtonBak2 = -1;

			play_se( 203, 320, 240 );
		}

		strcpy( OneLineInfoStr, "防御。");
	}
}


void BattleButtonItem( void )
{
	int i, bak;
	char moji[ 256 ];
	int x, y;
	if( HitDispNo == battleButtonDispNo[ 5 ] || battleButtonBak2 == 5 ){

		if( mouse.onceState & MOUSE_LEFT_CRICK || battleButtonBak2 == 5 ){
			bak = battleButtonFlag[ 5 ];
			BattleButtonOff();

			if( bak == false ){

				battleButtonFlag[ 5 ] = true;

				BattleCmdNo = BATTLE_ITEM;

				for( i = 0 ; i < BATTLE_MENU_ITEM_FONTS ; i++ ) battleMenuItemFontNo[ i ] = -2;

				pActWnd = MakeWindowDisp( 361, 160, 271, 281, 0, -1 );

				InitItem2( pActWnd->x, pActWnd->y );

				battleButtonBak = 5;

				battleButtonBak2 = -1;
			}
		}

		strcpy( OneLineInfoStr, "使用道具。");
	}

	if( battleButtonFlag[ 5 ] == true ){

		if( pActWnd != NULL ){

			if( pActWnd->hp > 0 ){
#ifdef _READ16BITBMP
				if(g_bUseAlpha) StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my, DISP_PRIO_MENU, CG_ITEM_WND_1, 1 );
				else
#endif
				StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my - 80, DISP_PRIO_MENU, CG_ITEM_WND_1, 1 );

				StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my - 80, DISP_PRIO_MENU, CG_BTL_ITEM_WND_TITLE, 1 );

				if( mouse.onceState & MOUSE_LEFT_CRICK ){

					if( HitDispNo == battleMenuItemFontNo[ 0 ] ){
						BattleButtonOff();				
					}
				}

				if( pActWnd != NULL ){

					x = pActWnd->x + 16;
					y = pActWnd->y + 191 + 7;

					for(  i = MAX_ITEM -1 ; i >= MAX_ITEMSTART ; i-- ){

						if( MakeHitBox( ItemBuffer[ i ].defX - 24, ItemBuffer[ i ].defY - 24, 
										ItemBuffer[ i ].defX + 25, ItemBuffer[ i ].defY + 23, DISP_PRIO_IME3 ) == true ){

							if( pc.item[ i ].useFlag == true ){ 
								int color = FONT_PAL_WHITE;

								char *splitPoint = pc.item[ i ].memo;

								if( pc.transmigration == 0 && pc.level < pc.item[ i ].level ) color = FONT_PAL_RED;

								StockFontBuffer( pActWnd->x + 16, pActWnd->y + 332 - 160, FONT_PRIO_FRONT, color, pc.item[ i ].name, 0 );	

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

							if( mouse.onceState & MOUSE_LEFT_DBL_CRICK ){

								if( pc.item[ i ].useFlag == true ){

									if( pc.item[ i ].useFlag != true || pc.item[ i ].field == ITEM_FIELD_MAP ||  pc.transmigration == 0 && pc.level < pc.item[ i ].level ){

										play_se( 220, 320, 240 );
									}else{
										BattleItemNo = i;

										switch( pc.item[ i ].target ){
										case ITEM_TARGET_MYSELF:

											p_party[ BattleMyNo ]->atr |= ACT_ATR_HIT_BOX;

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
											
										case ITEM_TARGET_OTHER:

											for( i = 0 ; i < 20 ; i++ ){

												if( p_party[ i ]->func == NULL ) continue;

												if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

												p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
											
										case ITEM_TARGET_ALLMYSIDE:

											if( BattleMyNo < 10 ){

												for( i = 0 ; i < 10 ; i++ ){

													if( p_party[ i ]->func == NULL ) continue;

													if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

													p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
												}
											}

											else{

												for( i = 10 ; i < 20 ; i++ ){

													if( p_party[ i ]->func == NULL ) continue;

													if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

													p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
												}
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
											
										case ITEM_TARGET_ALLOTHERSIDE:

											if( BattleMyNo >= 10 ){

												for( i = 0 ; i < 10 ; i++ ){

													if( p_party[ i ]->func == NULL ) continue;

													if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

													p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
												}
											}else{

												for( i = 10 ; i < 20 ; i++ ){

													if( p_party[ i ]->func == NULL ) continue;

													if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

													p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
												}
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
										
										case ITEM_TARGET_ALL:

											for( i = 0 ; i < 20 ; i++ ){

												if( p_party[ i ]->func == NULL ) continue;

												if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

												p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
											
										case ITEM_TARGET_NONE:

											BattleButtonOff();
											battleMenuReturn = true;

											if( bNewServer)
												lssproto_ID_send( sockfd, nowGx, nowGy, i, 0 );
											else
												old_lssproto_ID_send( sockfd, nowGx, nowGy, i, 0 );

											play_se( 203, 320, 240 );

											battleTargetSelectFlag = false;
											break;
											
										case ITEM_TARGET_OTHERWITHOUTMYSELF:

											for( i = 0 ; i < 20 ; i++ ){

												if( p_party[ i ]->func == NULL ) continue;

												if( i == BattleMyNo ) continue;

												if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

												p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;
	
											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
											
										case ITEM_TARGET_WITHOUTMYSELFANDPET:

											for( i = 0 ; i < 20 ; i++ ){

												if( p_party[ i ]->func == NULL ) continue;

												if( i == BattleMyNo + 5 ) continue;

												if( i == BattleMyNo ) continue;

												if( pc.item[ BattleItemNo ].deadTargetFlag == false ) if( p_party[ i ]->hp <= 0 ) continue;

												p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
											}

											battleTargetSelectFlag = true;

											DeathAction( pActWnd );
											pActWnd = NULL;

											ClearBattleButton();

											BattleCmdNo = BATTLE_ITEM;

											play_se( 217, 320, 240 );
											break;
#ifdef _PET_ITEM
										case ITEM_TARGET_PET:	// ????в?????????????
											StockChatBufferLine("战斗中不可更换宠物装备",FONT_PAL_RED);
											break;
#endif
										}
										break;
									}
								}
							}
						}
						if( pc.item[ i ].useFlag == true ){
							StockDispBuffer( ItemBuffer[ i ].defX, ItemBuffer[ i ].defY, ItemBuffer[ i ].dispPrio, pc.item[ i ].graNo, 0 );
						}
					}

					if( pActWnd != NULL ){
						battleMenuItemFontNo[ 0 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, pActWnd->y + 262, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}  
		} 
	} 
}


void BattleButtonPet( void )
{
	int i, bak;
	char moji[ 256 ];
	int x, y;
	int flag = false;
	int atrFlag = false;
	int atrGraNo[ 4 ];
	int color;
	if( HitDispNo == battleButtonDispNo[ 6 ] || battleButtonBak2 == 6 ){

		if( mouse.onceState & MOUSE_LEFT_CRICK || battleButtonBak2 == 6 ){
			bak = battleButtonFlag[ 6 ];
			BattleButtonOff();

			if( bak == false ){

				battleButtonFlag[ 6 ] = true;

				for( i = 0 ; i < BATTLE_MENU_PET_FONTS ; i++ ) battleMenuPetFontNo[ i ] = -2;

				pActWnd = MakeWindowDisp( 376, 132, 256, 320, 0, -1 );

				battleButtonBak = 6;

				battleButtonBak2 = -1;
			}	
		}

		strcpy( OneLineInfoStr, "更换宠物。");
	}

	if( battleButtonFlag[ 6 ] == true ){
		if( pActWnd != NULL ){
			if( pActWnd->hp > 0 ){

				StockDispBuffer(((WINDOW_DISP*)pActWnd->pYobi)->mx,((WINDOW_DISP*)pActWnd->pYobi)->my,DISP_PRIO_MENU,CG_BTL_PET_CHANGE_WND,1);

				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					for( i = 0 ; i < 5 ; i++ ){
						if( pet[ i ].useFlag == true /*&& pc.selectPetNo[ i ] == true*/ ){
							if( HitFontNo == battleMenuPetFontNo[ i ] ){
								if( pet[ i ].hp > 0 && i != pc.battlePetNo ){
									sprintf( moji, "S|%d", i );

									if( bNewServer)
										lssproto_B_send( sockfd, moji );
									else
										old_lssproto_B_send( sockfd, moji );

									play_se( 203, 320, 240 );

									DeathAction( pActWnd );

									pActWnd = NULL;

									battleMenuReturn = true;

									battleButtonBak = -1;

									battleButtonBak2 = -1;
								}else{
									play_se( 220, 320, 240 );
								}
							}
						}
					}

					if( HitDispNo == battleMenuPetFontNo[ 5 ] && pc.battlePetNo != -1 ){

						if( bNewServer)
							lssproto_B_send( sockfd, "S|-1" );
						else
							old_lssproto_B_send( sockfd, "S|-1" );

						play_se( 203, 320, 240 );

						DeathAction( pActWnd );

						pActWnd = NULL;

						battleMenuReturn = true;

						battleButtonBak = -1;

						battleButtonBak2 = -1;
					}

					if( HitDispNo == battleMenuPetFontNo[ 6 ] ){
						BattleButtonOff();				
					}
				}

				if( pActWnd != NULL ){
					x = pActWnd->x + 17, y = pActWnd->y + 33;

					for( i = 0 ; i < 5 ; i++ ){

						if( pet[ i ].useFlag == true /*&& pc.selectPetNo[ i ] == true */ && pc.ridePetNo != i  && pc.battlePetNo != i ){
							atrFlag = false;
							color = FONT_PAL_AQUA;
							if( pet[ i ].hp <= 0 ) color = FONT_PAL_RED;
							if( i == pc.battlePetNo ) color = FONT_PAL_YELLOW;

							if( pet[ i ].freeName[ 0 ] != NULL )
								CenteringStr( pet[ i ].freeName, moji, PET_NAME_LEN );
							else
								CenteringStr( pet[ i ].name, moji, PET_NAME_LEN );
							battleMenuPetFontNo[ i ] = StockFontBuffer( x, y, FONT_PRIO_FRONT, color, moji, 2 );

							if( pet[ i ].earth > 0 ){
								if( pet[ i ].earth > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_EARTH_BIG;
								else atrGraNo[ atrFlag ] = CG_ATR_ICON_EARTH_SML;
								atrFlag++; 
							}
							if( pet[ i ].water > 0 ){
								if( pet[ i ].water > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_WATER_BIG;
								else atrGraNo[ atrFlag ] = CG_ATR_ICON_WATER_SML;
								atrFlag++;
							}
							if( pet[ i ].fire > 0 ){
								if( pet[ i ].fire > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_FIRE_BIG;
								else atrGraNo[ atrFlag ] = CG_ATR_ICON_FIRE_SML;
								atrFlag++;
							}
							if( pet[ i ].wind > 0 ){
								if( pet[ i ].wind > 50 ) atrGraNo[ atrFlag ] = CG_ATR_ICON_WIND_BIG;
								else atrGraNo[ atrFlag ] = CG_ATR_ICON_WIND_SML;
								atrFlag++; 
							}

							if( atrFlag > 0 ) StockDispBuffer( pActWnd->x + 214, y + 8, DISP_PRIO_IME3, atrGraNo[ 0 ], 0 );

							if( atrFlag > 1 ) StockDispBuffer( pActWnd->x + 214 + 16 , y + 8, DISP_PRIO_IME3, atrGraNo[ 1 ], 0 );
							 y += 26;

							sprintf( moji, "%3d          %4d   %4d", pet[ i ].level, pet[ i ].hp, pet[ i ].maxHp );
				//cary
							StockFontBuffer( x + 27, y, FONT_PRIO_FRONT, color, 	moji, 0 ); y += 25;
				//end

							sprintf( moji, "%3d", pet[ i ].ai );
							StockFontBuffer( x + 60, y, FONT_PRIO_FRONT, color, 	moji, 0 ); y += 36;
							flag = true;
						}
					}

					if( flag == true ){ 

						if( pc.battlePetNo != -1 ){ 
							battleMenuPetFontNo[ 5 ] = StockDispBuffer( pActWnd->x + 70, pActWnd->y + 299, DISP_PRIO_IME3, CG_BTL_PET_RETURN_BTN, 2 );
							battleMenuPetFontNo[ 6 ] = StockDispBuffer( pActWnd->x + 187, pActWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
						}else{
							battleMenuPetFontNo[ 6 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, pActWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
						}
					}else{
						StockFontBuffer( x + 11, y, FONT_PRIO_FRONT, 0, "没有参战的宠物", 0 );
						battleMenuPetFontNo[ 6 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, pActWnd->y + 299, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}			
			}
		}
	}
}
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
void BattleButtonPPLSKILL( void )
{
	if ( MenuToggleFlag & JOY_ESC || MenuToggleFlag & JOY_CTRL_E || MenuToggleFlag & JOY_CTRL_A) {
		//play_se( 220, 320, 240 );
		return; 
	}
	char msg[256] = "";
	char buf[256];
	char msg2[256];
	if( HitDispNo == battleButtonDispNo[ 9 ] ){
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			play_se( 217, 320, 240 );
			DeathAction( pActWnd );
			pActWnd = NULL;
			battleTargetSelectFlag = false;
			BattleCmdNo = -1;
			ClearBattleButton();
			ClearBoxFlag();
			//MenuToggleFlag &= ~JOY_CTRL_E;
			//MenuToggleFlag &= ~JOY_CTRL_A;
			( wonflag == 0 ) ? wonflag = 1 : wonflag = 0  ; 
		}
		strcpy( OneLineInfoStr, "职业技能");
	}
	if ( wonflag == 1 ) {
		for ( int i = 0 ; i < 3 ; i ++ ) {
#ifdef _READ16BITBMP
			if(g_bUseAlpha){
				if( MakeHitBox(  278 , 11 + i	 * 78, 308  , 89 + i * 78 , DISP_PRIO_IME3 ) == true ) {
					if( mouse.onceState & MOUSE_LEFT_CRICK ) {
						SkillWnd2 = i ; 
						play_se( 217, 320, 240 );
						break;
					}
				}
			}
			else
#endif
			if( MakeHitBox(  270 , 14 + i * 79, 300  , 92 + i * 79 , DISP_PRIO_IME3 ) == true ) {
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					SkillWnd2 = i ; 
					play_se( 217, 320, 240 );
					break;
				}
			}
		}
		StockDispBuffer( 159, 225 , DISP_PRIO_MENU, CG_BATTTLE_SKILLCHOICE, 1 );
		extern int BattleSkill[20];
		extern int AssitSkill[20];
		extern int AdvanceSkill[20];
		int x = 26 , y = 5 ; 
			for ( int i = 0 ; i < 4 ; i ++ ) {
				for ( int j = 0 ; j < 4 ; j ++ ) {
//					StockDispBuffer( x + 138 + j * 60 , y + 228 + i * 72 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 2 );
				}
			}

		switch ( SkillWnd2 ) {
		case 0 :
#ifdef _READ16BITBMP
			if(g_bUseAlpha) StockDispBuffer( 168, 223, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
			else
#endif
			StockDispBuffer( 159, 226, DISP_PRIO_IME3, CG_FIELD_SKILL_ASSIT , 0 );
			for ( int i = 0 ; i < 4 ; i ++ ) {
				for ( int j = 0 ; j < 4 ; j ++ ) {
					if ( AssitSkill [ j + i * 4 ] == -1 ) 
						break;
					if ( profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag == 0 )
						StockDispBuffer ( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , CG_BTL_BUTTON_CROSS , 0 ) ;
					//StockDispBuffer( x + 138 + j * 48 , y + 228 + i * 52 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 0 );
					AssitSkillBtn [ j + i * 4 ] = StockDispBuffer( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AssitSkill [ j + i * 4 ] ].icon  , 2 );
					sprintf ( msg2 , "%8s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
					StockFontBuffer( x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, FONT_PAL_WHITE, msg2, 0 );
					if ( HitDispNo == AssitSkillBtn [ j + i * 4 ] ){
						int use_color=0;
						if( p_party[ BattleMyNo ]->mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ){
							use_color = FONT_PAL_WHITE;
						}else{
							use_color = FONT_PAL_GRAY;									
						}
						if ( profession_skill[ AssitSkill [ j + i * 4 ] ].skillId == 11	)
							use_color = FONT_PAL_WHITE; //??????????mp????
						sprintf ( msg2 , "%s" , profession_skill[AssitSkill [ j + i * 4 ]].name );
						StockFontBuffer( 20, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );

						sprintf( msg , "  %d％", profession_skill[AssitSkill [ j + i * 4 ]].skill_level ) ; 
						StockFontBuffer( 85, 310, FONT_PRIO_FRONT, use_color, msg, 0 );
				
						sprintf ( msg2 , "(耗费MP:%d)" , profession_skill[AssitSkill [ j + i * 4 ]].costmp );
						StockFontBuffer( 170, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );				

						sprintf( msg , "%s" ,	 profession_skill[AssitSkill [ j + i * 4 ]].memo ) ; 
					}
					if( mouse.onceState & MOUSE_LEFT_CRICK_UP ) {
						if ( HitDispNo == AssitSkillBtn [ j + i * 4 ]  && profession_skill[ AssitSkill [ j + i * 4 ] ].useFlag != 0) {
							if( p_party[ BattleMyNo ]->mp >= profession_skill[AssitSkill [ j + i * 4 ]].costmp && profession_skill[AssitSkill [ j + i * 4 ]].costmp != 0 ) {
								wonflag = 0 ; 
								prouseskill = AssitSkill [ j + i * 4 ] ; 
								BattleSetWazaHitBox( profession_skill[AssitSkill [ j + i * 4 ] ].target , 1) ; 
							}
							else if ( profession_skill[ AssitSkill [ j + i * 4 ] ].skillId == 11 && profession_skill[AssitSkill [ j + i * 4 ]].costmp == 0 ) { //??????????mp????
								wonflag = 0 ; 
								prouseskill = AssitSkill [ j + i * 4 ] ; 
								BattleSetWazaHitBox( profession_skill[AssitSkill [ j + i * 4 ] ].target , 1) ; 
							}
						}
					}
				}
			}
			break;
		case 1 :
#ifdef _READ16BITBMP
			if(g_bUseAlpha) StockDispBuffer( 168, 223, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
			else
#endif
			StockDispBuffer( 159, 226, DISP_PRIO_IME3, CG_FIELD_SKILL_BATTLE , 0 );
			for ( int i = 0 ; i < 4 ; i ++ ) {
				for ( int j = 0 ; j < 4 ; j ++ ) {
					if ( BattleSkill [ j + i * 4 ] == -1 ) 
						break;
					if ( profession_skill[ BattleSkill [ j + i * 4 ] ].useFlag == 0 )
						StockDispBuffer ( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , CG_BTL_BUTTON_CROSS , 0 ) ;
					if ( profession_skill[ BattleSkill [ j + i * 4 ] ].skillId == 40 && p_party[ BattleMyNo ]->hp <= 10 )
						StockDispBuffer ( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , CG_BTL_BUTTON_CROSS , 0 ) ;
					//StockDispBuffer( x + 138 + j * 48 , y + 228 + i * 52 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 0 );
					BattleSkillBtn [ j + i * 4 ] = StockDispBuffer( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ BattleSkill [ j + i * 4 ] ].icon  , 2 );
					sprintf ( msg2 , "%8s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
					StockFontBuffer( x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, FONT_PAL_WHITE, msg2, 0 );
					if ( HitDispNo == BattleSkillBtn [ j + i * 4 ] ){ 

						int use_color=0;
						if( p_party[ BattleMyNo ]->mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0 ) {
							use_color = FONT_PAL_WHITE;
						}else{
							use_color = FONT_PAL_GRAY;									
						}

						sprintf ( msg2 , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].name );
						StockFontBuffer( 20, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );
						
						sprintf( msg , "  %d％", profession_skill[BattleSkill [ j + i * 4 ]].skill_level ) ; 
						StockFontBuffer( 85, 310, FONT_PRIO_FRONT, use_color, msg, 0 );

						sprintf ( msg2 , "(耗费MP:%d)" , profession_skill[BattleSkill [ j + i * 4 ]].costmp );
						StockFontBuffer( 170, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );

						sprintf( msg , "%s" , profession_skill[BattleSkill [ j + i * 4 ]].memo ) ; 
					}
					if( mouse.onceState & MOUSE_LEFT_CRICK_UP ) {
						if ( HitDispNo == BattleSkillBtn [ j + i * 4 ] && profession_skill[ BattleSkill [ j + i * 4 ] ].useFlag != 0) {
							if( p_party[ BattleMyNo ]->mp >= profession_skill[BattleSkill [ j + i * 4 ]].costmp && profession_skill[BattleSkill [ j + i * 4 ]].costmp != 0  ) {
								if ( !(profession_skill[ BattleSkill [ j + i * 4 ] ].skillId == 40 && p_party[ BattleMyNo ]->hp <= 10) ) { //????????????
									wonflag = 0 ; 
									prouseskill = BattleSkill [ j + i * 4 ] ; 
									BattleSetWazaHitBox( profession_skill[BattleSkill [ j + i * 4 ] ].target , 1) ; 
								}
							}
						}
					}
				}
			}
			break;
		case 2:
#ifdef _READ16BITBMP
			if(g_bUseAlpha) StockDispBuffer( 168, 223, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
			else
#endif
			StockDispBuffer( 159, 226, DISP_PRIO_IME3, CG_FIELD_SKILL_ADVSK , 0 );
			for ( int i = 0 ; i < 4 ; i ++ ) {
				for ( int j = 0 ; j < 4 ; j ++ ) {
					if ( AdvanceSkill [ j + i * 4 ] == -1 ) 
						break;
					if ( profession_skill[ AdvanceSkill [ j + i * 4 ] ].useFlag == 0 )
						StockDispBuffer ( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , CG_BTL_BUTTON_CROSS , 0 ) ;
					//StockDispBuffer( x + 138 + j * 48 , y + 228 + i * 52 , DISP_PRIO_IME3, CG_FIELD_SKILL_TILE , 0 );
					AdvanceSkillBtn [ j + i * 4 ] = StockDispBuffer( x + 24 + j * 60 , y + 98 + i * 57 , DISP_PRIO_IME3 + 1 , profession_skill[ AdvanceSkill [ j + i * 4 ] ].icon  , 2 );
					sprintf ( msg2 , "%8s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
					StockFontBuffer( x + j * 60 , y + 110 + i * 57 , FONT_PRIO_FRONT, FONT_PAL_WHITE, msg2, 0 );
					if ( HitDispNo == AdvanceSkillBtn [ j + i * 4 ] ){ 
						
						int use_color = 0;
						if( p_party[ BattleMyNo ]->mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 
							){
							use_color = FONT_PAL_WHITE;
						}else{
							use_color = FONT_PAL_GRAY;									
						}
#ifdef _PROFESSION_ADDSKILL
					//	if ( profession_skill[ AdvanceSkill [ j + i * 4 ] ].skillId == 2 )
					//		use_color = FONT_PAL_WHITE; //??????????mp????
#endif			
						sprintf ( msg2 , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].name );
						StockFontBuffer( 20, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );

						sprintf( msg , "  %d％", profession_skill[AdvanceSkill [ j + i * 4 ]].skill_level ) ; 
						StockFontBuffer( 85, 310, FONT_PRIO_FRONT, use_color, msg, 0 );
						
						sprintf ( msg2 , "(耗费MP:%d)" , profession_skill[AdvanceSkill [ j + i * 4 ]].costmp );
						StockFontBuffer( 170, 312, FONT_PRIO_FRONT, use_color, msg2, 0 );

						sprintf( msg , "%s" , profession_skill[AdvanceSkill [ j + i * 4 ]].memo ) ; 
					}
					if( mouse.onceState & MOUSE_LEFT_CRICK_UP ) {
						if ( HitDispNo == AdvanceSkillBtn [ j + i * 4 ] && profession_skill[ AdvanceSkill [ j + i * 4 ] ].useFlag != 0 ) {
							if( p_party[ BattleMyNo ]->mp >= profession_skill[AdvanceSkill [ j + i * 4 ]].costmp && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp != 0 
								){																					
								wonflag = 0 ; 
								prouseskill = AdvanceSkill [ j + i * 4  ]; 
								BattleSetWazaHitBox( profession_skill[AdvanceSkill [ j + i * 4 ] ].target , 1) ; 
							}
#ifdef _PROFESSION_ADDSKILL
					//		else if ( profession_skill[ AdvanceSkill [ j + i * 4 ] ].skillId == 2 && profession_skill[AdvanceSkill [ j + i * 4 ]].costmp == 0 ) { //??????????mp????
					//			wonflag = 0 ; 
					//			BattleSetWazaHitBox( profession_skill[AdvanceSkill [ j + i * 4 ] ].target , 1) ; 
					//			prouseskill = AdvanceSkill [ j + i * 4  ];  
					//		}
#endif
						}
					}
				}
			}
			break;
		default:
			break;
		}
		char *splitPoint =  msg  ; 
		sprintf( buf , "%s" , msg ) ; 
		x = 18 ; 
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
//	StockFontBuffer ( 300 , 400 , FONT_PRIO_FRONT , FONT_PAL_WHITE , msg , 0 ) ; 
	}
}
#endif


void BattleButtonEscape( void )
{
	if( HitDispNo == battleButtonDispNo[ 7 ] && battleButtonFlag[ 7 ] == false ){

		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			BattleButtonOff();
			battleButtonFlag[ 7 ] = true;
			// ?`????
			if( bNewServer)
				lssproto_B_send( sockfd, "E" );
			else
				old_lssproto_B_send( sockfd, "E" );
			// ??????`?????
			battlePlayerEscFlag = true;
			// ??????
			battleMenuReturn = true;
			// ????????
			battleButtonBak = -1;
			// ?q????????
			battleButtonBak2 = -1;
			// ??????????
			play_se( 203, 320, 240 );
		}
		// ??????
		strcpy( OneLineInfoStr, "逃脱。");
	}
}

// ?????????????? **************************************************/
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
void BattleSetWazaHitBox( int no , int typeflag )
#else
void BattleSetWazaHitBox( int no )
#endif
{
	char moji[ 256 ];
	int i;

#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
	int switchnum = 0 ;

	if ( typeflag == 0 ){
		BattleWazaNo = no;
		switchnum = petSkill[ battlePetNoBak ][ no ].target;
	}else{
		switchnum = no ; 
	}
	battleTargetSelectFlag = true;
	switch( switchnum ) {
#else
	BattleWazaNo = no;
	battleTargetSelectFlag = true;
	switch( petSkill[ battlePetNoBak ][ no ].target ) {
#endif

	case PETSKILL_TARGET_MYSELF:	// ?????
		// ????R?e?????????????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 ) 
			p_party[ BattleMyNo ]->atr |= ACT_ATR_HIT_BOX;
		else
			p_party[ BattleMyNo + 5 ]->atr |= ACT_ATR_HIT_BOX;
#else
		p_party[ BattleMyNo + 5 ]->atr |= ACT_ATR_HIT_BOX;
#endif
//		if( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif
		BattleCmdNo = BATTLE_WAZA;
		// ?????
		play_se( 217, 320, 240 );
		// ??k???????
		battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_OTHER:	// ?\??????????)
		for( i = 0 ; i < 20 ; i++ ){
			if( p_party[ i ]->func == NULL ) continue;
			if( p_party[ i ]->hp <= 0 ) continue;
			p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
		}
///		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif
		BattleCmdNo = BATTLE_WAZA;
		play_se( 217, 320, 240 );
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag != 1 )
#endif
			battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_ALLMYSIDE:	// ?s???u?l
		// ????????
		if( BattleMyNo < 10 ){
			// ????R?e????????
			for( i = 0 ; i < 10 ; i++ ){
				// ??????????
				if( p_party[ i ]->func == NULL ) continue;
				// ??????
				if( p_party[ i ]->hp <= 0 ) continue;
				// ?????????
				p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
			}
		}
		// ????????
		else{
			// ????R?e????????
			for( i = 10 ; i < 20 ; i++ ){
				// ??????????
				if( p_party[ i ]->func == NULL ) continue;
				// ??????
				if( p_party[ i ]->hp <= 0 ) continue;
				// ?????????
				p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
			}
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif
		BattleCmdNo = BATTLE_WAZA;
		play_se( 217, 320, 240 );
		battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_ALLOTHERSIDE:	// ???D?u?l
		// ????????
		if( BattleMyNo >= 10 ){
			// ????R?e????????
			for( i = 0 ; i < 10 ; i++ ){
				// ??????????
				if( p_party[ i ]->func == NULL ) continue;
				// ??????
				if( p_party[ i ]->hp <= 0 ) continue;
				// ?????????
				p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
			}
		}
		// ????????
		else{
			// ????R?e????????
			for( i = 10 ; i < 20 ; i++ ){
				// ??????????
				if( p_party[ i ]->func == NULL ) continue;
				// ??????
				if( p_party[ i ]->hp <= 0 ) continue;
				// ?????????
				p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL2;
			}
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		// ?????????????
		DeathAction( pActWnd );
		pActWnd = NULL;
		// ???????u???
		ClearBattleButton();
		// ?????k?
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif

		BattleCmdNo = BATTLE_WAZA;
		// ?????
		play_se( 217, 320, 240 );
		// ??k???????
		battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_ALL:	// ?u?
		// ????R?e????????
		for( i = 0 ; i < 20 ; i++ ){
			// ??????????
			if( p_party[ i ]->func == NULL ) continue;
			// ??????
			if( p_party[ i ]->hp <= 0 ) continue;
			// ?????????
			p_party[ i ]->atr |= ACT_ATR_HIT_BOX_ALL1;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		// ???????u???
		ClearBattleButton();
		// ?????k?
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif

		BattleCmdNo = BATTLE_WAZA;
		// ?????
		play_se( 217, 320, 240 );
		// ??k???????
		battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_NONE:	// ?M??k?????????\????
		BattleButtonOff(); // ??????u???????
		battleMenuReturn = true;
		// ???????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 ) 
			sprintf( moji,"P|%X|%X", prouseskill, BattleMyNo );
		else
#endif
		sprintf( moji,"W|%X|%X", BattleWazaNo, BattleMyNo + 5 );
		// ????
		lssproto_B_send( sockfd, moji );

		// ??????????
		play_se( 203, 320, 240 );
		//andy_add
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 0 ){
			battleWazaTargetBak = -1;
			battleTargetSelectFlag = false;
		}
#endif
		break;
		
	case PETSKILL_TARGET_OTHERWITHOUTMYSELF:	// ?\????????????) 
		// ????R?e????????
		for( i = 0 ; i < 20 ; i++ ){
			// ??????????
			if( p_party[ i ]->func == NULL ) continue;
			// ????????????
			if( i == BattleMyNo + 5 ) continue;
			// ??????
			if( p_party[ i ]->hp <= 0 ) continue;
			// ?????????
			p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif

		BattleCmdNo = BATTLE_WAZA;
		// ?????
		play_se( 217, 320, 240 );
		// ??k???????
		battleWazaTargetBak = no;
		break;
		
	case PETSKILL_TARGET_WITHOUTMYSELFANDPET:	// ?????????? 
		// ????R?e????????
		for( i = 0 ; i < 20 ; i++ ){
			// ??????????
			if( p_party[ i ]->func == NULL ) continue;
			// ????????????
			if( i == BattleMyNo + 5 ) continue;
			// ????????
			if( i == BattleMyNo ) continue;
			// ??????
			if( p_party[ i ]->hp <= 0 ) continue;
			// ?????????
			p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		// ?????????????
		DeathAction( pActWnd );
		pActWnd = NULL;
		// ???????u???
		ClearBattleButton();
		// ?????k?
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
#endif
		BattleCmdNo = BATTLE_WAZA;
		// ?????
		play_se( 217, 320, 240 );
		// ??k???????
		battleWazaTargetBak = no;
		break;
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
	case PETSKILL_TARGET_ONE_ROW:
		for( i = 0 ; i < 5 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL4;
		}
		for( i = 5 ; i < 10 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL3;
		}
		for( i = 10 ; i < 15 ; i++ ) {
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL ) continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL1;
		}
		for( i = 15 ; i < 20 ; i++ ) {
			if( p_party[i]->func == NULL ) continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL2;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;

		DeathAction( pActWnd );
		pActWnd = NULL;

		ClearBattleButton();

		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
			BattleCmdNo = BATTLE_WAZA;
		
		play_se( 217, 320, 240 );

		if ( typeflag != 1 )
			battleWazaTargetBak = no;

		break;
#ifdef _SKILL_ADDBARRIER
	case PETSKILL_TARGET_ONE_ROW_ALL:
		for( i = 0 ; i < 5 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL8;
		}
		for( i = 5 ; i < 10 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL7;
		}
		for( i = 10 ; i < 15 ; i++ ) {
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL ) continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL5;
		}
		for( i = 15 ; i < 20 ; i++ ) {
			if( p_party[i]->func == NULL ) continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX_COL6;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
			BattleCmdNo = BATTLE_WAZA;
		play_se( 217, 320, 240 );
		if ( typeflag != 1 )
			battleWazaTargetBak = no;
		break;
#endif
	case PETSKILL_TARGET_ONE_LINE:
		for( i = 0 ; i < 5 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX;
			p_party[i+5]->atr |= ACT_ATR_HIT_BOX;
		}
		for( i = 5 ; i < 10 ; i++ )	{
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX;
			p_party[i-5]->atr |= ACT_ATR_HIT_BOX;

		}
		for( i = 10 ; i < 15 ; i++ ) {
			if( p_party[i]->func == NULL )	continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL ) continue;
			// ????????
			p_party[i]->atr |= ACT_ATR_HIT_BOX;
			p_party[i+5]->atr |= ACT_ATR_HIT_BOX;

		}
		for( i = 15 ; i < 20 ; i++ ) {
			if( p_party[i]->func == NULL ) continue;
			//if( false == magic[BattleJujutuNo].deadTargetFlag )
			if( p_party[i]->hp <= 0 ) continue;
			if( p_party[i]->atr & ACT_ATR_TRAVEL )	continue;
			// ????????
			p_party[i]->atr = ACT_ATR_HIT_BOX;
			p_party[i-5]->atr = ACT_ATR_HIT_BOX;

		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;
		DeathAction( pActWnd );
		pActWnd = NULL;
		ClearBattleButton();
		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
			BattleCmdNo = BATTLE_WAZA;
		play_se( 217, 320, 240 );
		battleWazaTargetBak = no;
		break;
	case PETSKILL_TARGER_DEATH:
		for( i = 0 ; i < 20 ; i++ ){
			if( p_party[ i ]->func == NULL ) continue;
			if( p_party[ i ]->hp > 0 ) continue;
			p_party[ i ]->atr |= ACT_ATR_HIT_BOX;
		}
//		if ( typeflag == 0 )
			battleTargetSelectFlag = true;

		DeathAction( pActWnd );
		pActWnd = NULL;

		ClearBattleButton();

		if ( typeflag == 1 )
			BattleCmdNo = BATTLE_PROWAZA;
		else
			BattleCmdNo = BATTLE_WAZA;

		play_se( 217, 320, 240 );

		if ( typeflag != 1 )
			battleWazaTargetBak = no;

		break;
#endif
	}
}

// ????? *****************************************************************/
void BattleButtonWaza( void )
{
	int i, bak;
	int x, y;
	char moji[ 256 ];
	if( HitDispNo == battleButtonDispNo[ 8 ] || battlePetButtonFlag == true ){
		// ?????????
		if( mouse.onceState & MOUSE_LEFT_CRICK || battlePetButtonFlag == true ){
			bak = battleButtonFlag[ 8 ];	// ??????
			BattleButtonOff();				// ??????u???????
			// ?????????
			if( bak == false ){
				// ?????????
				battleButtonFlag[ 8 ] = true; //?????????
				// ?????????
				battleWazaTargetBak = -1;
				// ?????k?
				//BattleCmdNo = BATTLE_WAZA;
				// ????????R?e???
				for( i = 0 ; i < BATTLE_MENU_WAZA_FONTS ; i++ ) battleMenuWazaFontNo[ i ] = -2;
				// ???????????????
				pActWnd = MakeWindowDisp( 364, 96, 272, 348, 0, -1 );
			}
		}
		// ?????????
		battlePetButtonFlag = false;
		// ??????				 
		strcpy( OneLineInfoStr, "使用技能。");
	}
	// ????????
	if( battleButtonFlag[ 8 ] == true ){
		// ?????????
		if( pActWnd != NULL ){
			// ????????????????
			if( pActWnd->hp > 0 ){
				// ?????????
				StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my, DISP_PRIO_MENU, CG_PET_WAZA_WND, 1 );
				// ???????????
				for( i = 0 ; i < pet[ battlePetNoBak ].maxSkill ; i++ ){
					// ?????????
					StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, ( ( WINDOW_DISP *)pActWnd->pYobi )->my, DISP_PRIO_IME3, CG_PET_WAZA_BAR_1 + i, 1 );
				}
				// ????????
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					// ??????????
					if( HitDispNo == battleMenuWazaFontNo[ 7 ] ){
						// ??????u???????
						BattleButtonOff();				
					}
				}
				// ????????
				if( pActWnd != NULL ){
					// ????@?e
					x = pActWnd->x + 32;
					y = pActWnd->y + 252;
					// ?C???
					for( i = 0 ; i < pet[ battlePetNoBak ].maxSkill ; i++ ){
						// ????????
						if( petSkill[ battlePetNoBak ][ i ].useFlag == true ){
							if( HitFontNo == battleMenuWazaFontNo[ i ] ){
								// ??????
								char *splitPoint = petSkill[ battlePetNoBak ][ i ].memo;
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
								// ????????
								if( mouse.onceState & MOUSE_LEFT_CRICK ){
									// ?P?f??????
									if(	petSkill[ battlePetNoBak ][ i ].field != PETSKILL_FIELD_MAP ){
#ifdef _VARY_WOLF
										if(!((pet[battlePetNoBak].graNo==101428)&&(petSkill[battlePetNoBak][i].skillId==600))
											)
#endif
											// ??????????????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
											BattleSetWazaHitBox( i , 0 ) ; 
#else
										BattleSetWazaHitBox( i );
#endif
									}else{
										int flag = 0;
										int j;
										// ?P?f???????????????????
										for( j = 0 ; j < pet[ battlePetNoBak ].maxSkill ; j++ ){
											// ????????
											if( petSkill[ battlePetNoBak ][ j ].useFlag == true ){
												// ?P?f??????
												if(	petSkill[ battlePetNoBak ][ j ].field != PETSKILL_FIELD_MAP ) flag++;
											}
										}
										// ?P?f????????????
										if( flag > 0 ){
											// ???
											play_se( 220, 320, 240 );
										}
										// ??????
										else{
											BattleButtonOff(); // ??????u???????
											//battleMenuReturn = true;
											ClearBoxFlag();
											// ?????????????
											if( bNewServer)
												lssproto_B_send( sockfd, "W|FF|FF" );
											else
												old_lssproto_B_send( sockfd, "W|FF|FF" );
											// ??????????
											play_se( 203, 320, 240 );
											// ??k???????
											battleWazaTargetBak = -1;
											// ??????????????
											battleTargetSelectFlag = false;
										}
									}
								}
							}
						}
					}
/*
#ifndef __AI
					//cary ???????????
					if( !PauseAI && AI!=AI_NONE && false==battleTargetSelectFlag){
						if( (BattleIntervalCnt==0 && (BattleCntDown-TimeGetTime())<=29000) || BattleIntervalCnt<TimeGetTime()){
							BattleIntervalCnt = TimeGetTime()+500;
							int first = -1;
							int firstattack = -1;
							unsigned long ai = AI;
							if( ai == AI_SELECT){
								if( pet[ battlePetNoBak ].maxHp*3/10 > pet[ battlePetNoBak ].hp)
									ai = AI_GUARD;
								else
									ai = AI_ATTACK;
							}
							for( i = 0 ; i < pet[ battlePetNoBak ].maxSkill ; i++ ){
								if( petSkill[ battlePetNoBak ][ i ].useFlag == true && 
									petSkill[ battlePetNoBak ][ i ].field != PETSKILL_FIELD_MAP ){
									if( first == -1)
										first = i;
									if( ai == AI_ATTACK){
										if( petSkill[ battlePetNoBak ][ i ].skillId!=2 && firstattack==-1)
											firstattack = i;
										if( petSkill[ battlePetNoBak ][ i ].skillId == 1){
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
											BattleSetWazaHitBox( i , 0 );

#else
											BattleSetWazaHitBox( i );
#endif
											break;
										}
									}else if( ai == AI_GUARD){
										if( petSkill[ battlePetNoBak ][ i ].skillId == 2){
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
											BattleSetWazaHitBox( i , 0 );
#else
											BattleSetWazaHitBox( i );
#endif
											break;
										}
									}
								}
							}
							if( i==pet[ battlePetNoBak ].maxSkill){
								if( ai==AI_ATTACK && firstattack!=-1)
									first = firstattack;
								if( first == -1){
									// ??????????????
									battleTargetSelectFlag = true;
									// ?????????????
									DeathAction( pActWnd );
									pActWnd = NULL;
									// ???????u???
									ClearBattleButton();
									// ?????
									//play_se( 217, 320, 240 );
								}else
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
									BattleSetWazaHitBox( first , 0 );
#else
									BattleSetWazaHitBox( first );
#endif
							}
						}
					}
#endif
*/
					// ?????????
					if( pActWnd != NULL ){
						// ????@?e
						x = pActWnd->x + 40;
						y = pActWnd->y + 32;
						// ????????
						if( pet[ battlePetNoBak ].freeName[ 0 ] != NULL )
							CenteringStr( pet[ battlePetNoBak ].freeName, moji, PET_NAME_LEN );
						else
							CenteringStr( pet[ battlePetNoBak ].name, moji, PET_NAME_LEN );
							StockFontBuffer( x - 28, y, FONT_PRIO_FRONT, 0, moji, 0 ); y += 26; x += 18;
						// ?????
						for( i = 0 ; i < pet[ battlePetNoBak ].maxSkill ; i++ ){
							// ????????
							if( petSkill[ battlePetNoBak ][ i ].useFlag == true ){
								int color = FONT_PAL_GRAY;
								//cary
								sprintf( moji,"       %-22s",petSkill[ battlePetNoBak ][ i ].name );
								//end
								// ?P?f??????
								if(	petSkill[ battlePetNoBak ][ i ].field != PETSKILL_FIELD_MAP ){
#ifdef _VARY_WOLF
									if(!((pet[battlePetNoBak].graNo==101428)&&(petSkill[battlePetNoBak][i].skillId==600))
										)
#endif
										color = FONT_PAL_WHITE;
								}
								battleMenuWazaFontNo[ i ] = StockFontBuffer( x - 43, y, FONT_PRIO_FRONT, color, moji, 2 );
							}
							y += 25;
						}
						// ????@?e
						y = pActWnd->y + 330;
						// ???????????????
						battleMenuWazaFontNo[ 7 ] = StockDispBuffer( ( ( WINDOW_DISP *)pActWnd->pYobi )->mx, y, DISP_PRIO_IME3, CG_CLOSE_BTN, 2 );
					}
				}
			}
		}
	}
}

void BattleTargetSelect( void )
{
	char moji[ 256 ];
	int targetNo;
	int no;

	if( !( MenuToggleFlag & JOY_ESC ) && !( MenuToggleFlag & JOY_CTRL_E ) && !( MenuToggleFlag & JOY_CTRL_A ) && !( menuBtn == 1 ) ){

		if( pActInfoWnd == NULL ){

			if( mouse.nowPoint.y < 228 ){ 
				pActInfoWnd = MakeWindowDisp( 210, 356, 3, 2, 0, 1 );
			}else{ 
				pActInfoWnd = MakeWindowDisp( 210, 4, 3, 2, 0, 1 );
			}
		}else{	

			if( pActInfoWnd->hp > 0 ){
				StockFontBuffer( pActInfoWnd->x + 38, pActInfoWnd->y + 28, FONT_PRIO_FRONT, FONT_PAL_YELLOW, 	"请选择", 0 );
				StockFontBuffer( pActInfoWnd->x + 38, pActInfoWnd->y + 52, FONT_PRIO_FRONT, FONT_PAL_YELLOW, 	"您的目标", 0 );
			}

			if( mouse.nowPoint.y > 300 ) pActInfoWnd->y = 4;
			if( mouse.nowPoint.y < 156 ) pActInfoWnd->y = 356;
		}
	}else{
		DeathAction( pActInfoWnd );
		pActInfoWnd = NULL;
	}

	//cary ???????
	if( mouse.onceState & MOUSE_LEFT_CRICK ){

		if( ( targetNo = CheckBattelTarget() ) != -1 ){

			switch( BattleCmdNo ){
			case BATTLE_ATTACK:
				sprintf( moji, "H|%X", targetNo );
				if( bNewServer)
					lssproto_B_send( sockfd, moji );
				else
					old_lssproto_B_send( sockfd, moji );
				break;
			
			case BATTLE_CAPTURE:
				sprintf( moji, "T|%X", targetNo );
				if( bNewServer)
					lssproto_B_send( sockfd, moji );
				else
					old_lssproto_B_send( sockfd, moji );
				break;
				
			case BATTLE_JUJUTU:
				//cary*
				switch( magic[ BattleJujutuNo ].target ){

				#ifdef __ATTACK_MAGIC

				// ?о?????
				case MAGIC_TARGET_SINGLE:

					sprintf( moji , "J|%X|%X" , BattleJujutuNo , targetNo );
					if( bNewServer)
						lssproto_B_send( sockfd , moji );
					else
						old_lssproto_B_send( sockfd , moji );
					play_se( 217 , 320 , 240 );

					break;

				// ?о??????
				case MAGIC_TARGET_ONE_ROW:

					// 23: ???????? , 24: ???????? , 25: ???????? , 26: ????????
					if( targetNo >= 0 && targetNo < 5 )
						no = 26;
					else if( targetNo >= 5 && targetNo < 10 )
						no = 25;
					else if( targetNo >= 10 && targetNo < 15 )
						no = 23;
					else if( targetNo >= 15 && targetNo < 20 )
						no = 24;

					sprintf( moji , "J|%X|%X" , BattleJujutuNo , no );
					if( bNewServer)
						lssproto_B_send( sockfd , moji );
					else
						old_lssproto_B_send( sockfd , moji );
					play_se( 217 , 320 , 240 );

					break;

				// ?о????
				case MAGIC_TARGET_ALL_ROWS:

					( BattleMyNo < 10 )? no = 21 : no = 20;
					sprintf( moji , "J|%X|%X" , BattleJujutuNo , no );
					if( bNewServer)
						lssproto_B_send( sockfd , moji );
					else
						old_lssproto_B_send( sockfd , moji );
					play_se( 217 , 320 , 240 );

					break;

				#endif

				case MAGIC_TARGET_MYSELF:
				case MAGIC_TARGET_OTHER:
					sprintf( moji,"J|%X|%X", BattleJujutuNo, targetNo );
					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
					
				case MAGIC_TARGET_ALLMYSIDE:
					if( BattleMyNo < 10 ) no = 20;
					else no = 21;
					sprintf( moji,"J|%X|%X", BattleJujutuNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case MAGIC_TARGET_ALLOTHERSIDE:
					if( BattleMyNo < 10 ) no = 21;
					else no = 20;

					sprintf( moji,"J|%X|%X", BattleJujutuNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case MAGIC_TARGET_ALL:

					sprintf( moji,"J|%X|%X", BattleJujutuNo, 22 );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case MAGIC_TARGET_WHOLEOTHERSIDE:

					if( targetNo < 10 ) no = 20;
					else no = 21;

					sprintf( moji,"J|%X|%X", BattleJujutuNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
				}
				break;
				
				//Change note ?????????????????
			case BATTLE_ITEM:
				//cary*
				switch( pc.item[ BattleItemNo ].target ){
				case ITEM_TARGET_MYSELF:
				case ITEM_TARGET_OTHER:
				case ITEM_TARGET_OTHERWITHOUTMYSELF:
				case ITEM_TARGET_WITHOUTMYSELFANDPET:
					sprintf( moji,"I|%X|%X", BattleItemNo, targetNo );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case ITEM_TARGET_ALLMYSIDE:

					if( BattleMyNo < 10 ) no = 20;
					else no = 21;

					sprintf( moji,"I|%X|%X", BattleItemNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case ITEM_TARGET_ALLOTHERSIDE:

					if( BattleMyNo < 10 ) no = 21;
					else no = 20;

					sprintf( moji,"I|%X|%X", BattleItemNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case ITEM_TARGET_ALL:

					sprintf( moji,"I|%X|%X", BattleItemNo, 22 );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
				}
				break;

				//Change note ?輼?????????????
			case BATTLE_WAZA:
				switch( petSkill[ battlePetNoBak ][ BattleWazaNo ].target ){
				case PETSKILL_TARGET_MYSELF:
				case PETSKILL_TARGET_OTHER:
				case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
				case PETSKILL_TARGET_WITHOUTMYSELFANDPET:
#ifdef _BATTLESKILL
				case PETSKILL_TARGER_DEATH:
#endif
					sprintf( moji,"W|%X|%X", BattleWazaNo, targetNo );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALLMYSIDE:

					if( BattleMyNo < 10 ) no = 20;
					else no = 21;

					sprintf( moji,"W|%X|%X", BattleWazaNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALLOTHERSIDE:

					if( BattleMyNo < 10 ) no = 21;
					else no = 20;

					sprintf( moji,"W|%X|%X", BattleWazaNo, no );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALL:

					sprintf( moji,"W|%X|%X", BattleWazaNo, 22 );

					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );

					play_se( 217, 320, 240 );
					break;
#ifdef _BATTLESKILL
				case PETSKILL_TARGET_ONE_ROW:
#endif
#ifdef _SKILL_ADDBARRIER
				case PETSKILL_TARGET_ONE_ROW_ALL:
#endif
					if ( targetNo >= 0 && targetNo < 5 ) no = 26 ; 
					if ( targetNo >= 5 && targetNo < 10 ) no = 25 ; 
					if ( targetNo >= 10 && targetNo < 15 ) no = 23 ; 
					if ( targetNo >= 15 && targetNo < 20 ) no = 24 ; 
					sprintf( moji,"W|%X|%X", BattleWazaNo, no );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				}
				break;
				
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
				//Change note ???????????????????
			case BATTLE_PROWAZA:
				switch( profession_skill[prouseskill].target ){
				case PETSKILL_TARGET_MYSELF:
				case PETSKILL_TARGET_OTHER:
				case PETSKILL_TARGET_OTHERWITHOUTMYSELF:
				case PETSKILL_TARGET_WITHOUTMYSELFANDPET:
				case PETSKILL_TARGET_ONE_LINE:
#ifdef _BATTLESKILL
				case PETSKILL_TARGER_DEATH:
#endif
					sprintf( moji,"P|%X|%X", prouseskill, targetNo );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				case PETSKILL_TARGET_ALLMYSIDE:
					if( BattleMyNo < 10 ) no = 20;
					else no = 21;
					sprintf( moji,"P|%X|%X", prouseskill, no );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				case PETSKILL_TARGET_ALLOTHERSIDE:
					if( BattleMyNo < 10 ) no = 21;
					else no = 20;
					sprintf( moji,"P|%X|%X", prouseskill, no );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				case PETSKILL_TARGET_ALL:
					sprintf( moji,"P|%X|%X", prouseskill, 22 );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				case PETSKILL_TARGET_ONE_ROW:
#ifdef _SKILL_ADDBARRIER
				case PETSKILL_TARGET_ONE_ROW_ALL:
#endif
					if ( targetNo >= 0 && targetNo < 5 ) no = 26 ; 
					if ( targetNo >= 5 && targetNo < 10 ) no = 25 ; 
					if ( targetNo >= 10 && targetNo < 15 ) no = 23 ; 
					if ( targetNo >= 15 && targetNo < 20 ) no = 24 ; 
					sprintf( moji,"P|%X|%X", prouseskill, no );
					lssproto_B_send( sockfd, moji );
					play_se( 217, 320, 240 );
					break;
				}
				break;
#endif
			}

			play_se( 203, 320, 240 );

			battleTargetSelectFlag = false;

			battleMenuReturn = true;

			DeathAction( pActInfoWnd );
			pActInfoWnd = NULL;
		}
	}
}

static int Ordinal[] = { 4+15, 2+15, 0+15, 1+15, 3+15,
						 4+10, 2+10, 0+10, 1+10, 3+10,
						 4+5, 2+5, 0+5, 1+5, 3+5,
						 4, 2, 0, 1, 3 };
// ???????????R?e ***************************************************/
int GetBattelTarget()
{
	int i, index, end;
	i = 10;
	end = 20;
	if( BattleMyNo < 10){
		i = 0 ;
		end = 10;
	}
#ifdef __AI
	for( ; i < end ; i++ ){
		index = Ordinal[i];
		if( p_party[ index ]->func == NULL ) continue;
		if( p_party[index]->hp<=0 || index==BattleMyNo || index==5+BattleMyNo)
			continue;
		return index;
	}
#else
	for( ; i < end ; i++ ){
		index = Ordinal[i];
		if( p_party[ index ]->func == NULL ) continue;
		if( index==BattleMyNo || index==5+BattleMyNo)
			continue;

#ifdef __ATTACK_MAGIC
#ifdef _SKILL_ADDBARRIER
		if( ( p_party[ index ]->atr & ACT_ATR_HIT_BOX ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL1 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL2 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL3 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL4 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL1 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL2 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL3 )


			 || ( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL5 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL6 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL7 ) || 
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL8 ) 
			)
			return index;
#else
		if( ( p_party[ index ]->atr & ACT_ATR_HIT_BOX ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL1 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL2 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL3 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_COL4 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL1 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL2 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL3 ) )
			return index;
#endif
#else
		if( ( p_party[ index ]->atr & ACT_ATR_HIT_BOX ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL1 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL2 ) ||
			( p_party[ index ]->atr & ACT_ATR_HIT_BOX_ALL3 ) )
			return index;
#endif

	}
#endif
	return -1;
}

// ????????? **********************************************************/
void BattleCntDownDisp( void )
{
	char moji[ 16 ];
	char *work = moji;
	int x = 320 - 16 + DISPLACEMENT_X / 2;
	int y = 240 + DISPLACEMENT_Y / 2;
	int i;
	// ?????

	int BattleCntDownRest = BattleCntDown - TimeGetTime();
	// ???????
	if( BattleCntDownFlag == false ) return;

	// ?????????
/*//????ai
#ifndef __AI
	//cary ??????
	if( AI!=AI_NONE && !PauseAI){
		if( battleMenuFlag&BATTLE_MENU && battleTargetSelectFlag){
			if( (BattleIntervalCnt==0 && BattleCntDownRest<=28000) || (BattleIntervalCnt!=0 && BattleIntervalCnt<TimeGetTime())){
				BattleIntervalCnt = TimeGetTime()+500;
				// ??????
				int targetNo = GetBattelTarget();
				char moji[256];
				sprintf( moji, "H|%X", targetNo );
				// ?G????
				if( bNewServer)
					lssproto_B_send( sockfd, moji );
				else
					old_lssproto_B_send( sockfd, moji );
				battleTargetSelectFlag = false;
				battleMenuReturn = true;
				// ??????????
				play_se( 203, 320, 240 );
				// ????????
				battleButtonBak = battleButtonBak2;
				// ?????????????????
				if( pActInfoWnd){
					DeathAction( pActInfoWnd );
					pActInfoWnd = NULL;
				}
			}
		}else if( battleMenuFlag&BATTLE_MENU_PET && battleTargetSelectFlag){
			if( (BattleIntervalCnt==0 && BattleCntDownRest<=28000) || BattleIntervalCnt<TimeGetTime()){
				int no;
				int targetNo = GetBattelTarget();
				char moji[256];
				switch( petSkill[ battlePetNoBak ][ BattleWazaNo ].target ){
				case PETSKILL_TARGET_MYSELF:				// ??????
				case PETSKILL_TARGET_OTHER:					// ?\??????????)
				case PETSKILL_TARGET_OTHERWITHOUTMYSELF:	// ?\????????????) 
				case PETSKILL_TARGET_WITHOUTMYSELFANDPET:	// ??????????
					// ???????
					sprintf( moji,"W|%X|%X", BattleWazaNo, targetNo );
					// ????
					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );
					// ?????
					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALLMYSIDE:	// ?s???u?l
					// ????????
					if( BattleMyNo < 10 ) no = 20;
					else no = 21;
					// ???????
					sprintf( moji,"W|%X|%X", BattleWazaNo, no );
					// ????
					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );
					// ?????
					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALLOTHERSIDE:	// ???D?u?l
					// ????????
					if( BattleMyNo < 10 ) no = 21;
					else no = 20;
					// ???????
					sprintf( moji,"W|%X|%X", BattleWazaNo, no );
					// ????
					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );
					// ?????
					play_se( 217, 320, 240 );
					break;
					
				case PETSKILL_TARGET_ALL:	// ?u?
					// ???????
					sprintf( moji,"W|%X|%X", BattleWazaNo, 22 );
					// ????
					if( bNewServer)
						lssproto_B_send( sockfd, moji );
					else
						old_lssproto_B_send( sockfd, moji );
					// ?????
					play_se( 217, 320, 240 );
					break;
				}
				// ??????
				battleTargetSelectFlag = false;
				battleMenuReturn = true;
				// ??????????
				play_se( 203, 320, 240 );
				// ????????
				battleButtonBak = battleButtonBak2;
				// ?????????????????
				if( pActInfoWnd){
					DeathAction( pActInfoWnd );
					pActInfoWnd = NULL;
				}
			}
		}
	}
#endif
*/

	if( BattleCntDownRest <= 0 ){
		// ??????
		BattleCntDownFlag = false;
		BattleCntDownRest = 0;
		// ????R?e???????
		ClearBoxFlag();
		// ?????????????
		DeathAction( pActWnd );
		// ????????????
		pActWnd = NULL;				
		// ???????u???
		ClearBattleButton();
		// ????????h?k????????????????

		if( battleMenuFlag & BATTLE_MENU && battleMenuReturn == false ){
			// ?????????????
			if( bNewServer)
				lssproto_B_send( sockfd, "N" );
			else
				old_lssproto_B_send( sockfd, "N" );
			// ??P?????????????????
			if( battlePetNoBak == -1 || 
				p_party[ BattleMyNo + 5 ]->hp <= 0 ){
				// ??????
			}else{
				// ????????????
				if( bNewServer)
					lssproto_B_send( sockfd, "W|FF|FF" );
				else
					old_lssproto_B_send( sockfd, "W|FF|FF" );
			}
		}else{
			// ??????h?k?????
			if( ( battleMenuFlag & BATTLE_MENU && battleMenuReturn == true ) ||
				( battleMenuFlag & BATTLE_MENU_PET && battleMenuReturn == false ) ){ 
				// ??P?????????????????
				if( battlePetNoBak == -1 || 
					p_party[ BattleMyNo + 5 ]->hp <= 0 ){
					// ??????
				}else{
					// ????????????
					if( bNewServer)
						lssproto_B_send( sockfd, "W|FF|FF" );
					else
						old_lssproto_B_send( sockfd, "W|FF|FF" );
				}
			}
		}
		// ??????
		battleTargetSelectFlag = false;
		battleMenuReturn = true;
		// ??????????
		play_se( 203, 320, 240 );
		// ????????
		battleButtonBak = battleButtonBak2;
		// ?????????????????
		DeathAction( pActInfoWnd );
		pActInfoWnd = NULL;
	}

	sprintf( moji, "%2d", BattleCntDownRest / 1000 );

	for( i = 0 ; i < 2 ; i++ ){
		if( *work != 0x20 ){
			StockDispBuffer( x, y, DISP_PRIO_IME1, *work - '0' + CG_CNT_DOWN_0, 0 );
		}

		x += 32;
		work++;
	}

}


void BattleMenuProc( void )
{
	char moji[ 256 ];
	int i;

	if( BattleMyNo >= 20 ){

		if( bNewServer)
			lssproto_B_send( sockfd, "N" );
		else
			old_lssproto_B_send( sockfd, "N" );

		InitBattleAnimFlag();

		SubProcNo++;
		return;
	}

	if( BattleMyNo < 10 ){
		for( i  = 0 ; i < 10 ; i++ ) HpMeterDisp( i ); // ?o??????????
	}else 

	if( BattleMyNo >= 10 ){ 
		for( i = 10 ; i < 20 ; i++ ) HpMeterDisp( i ); // ?o??????????
	}

	if( p_party[ BattleMyNo ]->hp > 0 && 
		!( BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON && BattleBpFlag & BATTLE_BP_PET_MENU_NON ) ){

		if( battleTargetSelectFlag ){ 
			BattleTargetSelect();
		}else{

			DeathAction( pActInfoWnd );
			pActInfoWnd = NULL;
		}
	}

	if( battleMenuFlag2 == true ){
		buttonX = 815 + DISPLACEMENT_X;
		buttonY = 65 ;
		buttonA = 5;

		InitBattleAnimFlag();

		battlePlayerEscFlag = false;

		BattleCntDownFlag = true;

		BattleCntDown = TimeGetTime() + 99999;

		BattleIntervalCnt = 0;

		if( pc.battlePetNo == battlePetNoBak ){

			if( battleWazaTargetBak != -1 ){
				battlePetButtonFlag = false;
			}else{
				battlePetButtonFlag = true;
			}
		}else{
			battlePetButtonFlag = true;
			battleWazaTargetBak = -1;
		}

		if( p_party[ BattleMyNo ]->hp <= 0 ||

			BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){ 

			if( BattleBpFlag & BATTLE_BP_PLAYER_MENU_NON ){ 
				if( bNewServer)
					lssproto_B_send( sockfd, "N" );
				else
					old_lssproto_B_send( sockfd, "N" );
			}

			p_party[ BattleMyNo ]->atr |= ACT_ATR_BTL_CMD_END;

			battlePetMenuFlag = true;
		}else{

			battleMenuFlag |= BATTLE_MENU; 

			battleMenuReturn = false; 

			battleTimeUpFlag = false;

			play_se( 202, 320, 240 );

			for( i = 0 ; i < BATTLE_BUTTON_SUU ; i++ ) battleButtonDispNo[ i ] = -2;

			ClearBoxFlag();

			battleButtonBak2 = battleButtonBak;
		}

		battlePetNoBak = pc.battlePetNo;
		battlePetNoBak2 = pc.battlePetNo;

		battleMenuFlag2 = false;

		BattleBpFlag &= ~BATTLE_BP_PLAYER_MENU_NON;
	}

	if( battleMenuFlag & BATTLE_MENU ){ 

		if( battleMenuReturn == false ){
			if( buttonA > 0 ){
				buttonA--;
				buttonX -= 60;
			}
/*
#ifndef __AI
			if( !PauseAI && AI!=AI_NONE && battleButtonBak==-1 && (BattleCntDown-TimeGetTime())<=29500){
				unsigned long ai = AI;
				if( ai == AI_SELECT){
					if( p_party[ BattleMyNo ]->maxHp*3/10 < p_party[ BattleMyNo ]->hp)
						ai = AI_ATTACK;
					else if( p_party[ BattleMyNo ]->maxHp*15/100 < p_party[ BattleMyNo ]->hp)
						ai = AI_GUARD;
					else
						ai = AI_ESCAPE;
				}
				BattleIntervalCnt = TimeGetTime() + 500;
				if( ai==AI_ATTACK && battleButtonDispNo[ 0 ]!=-2){
					mouse.onceState = MOUSE_LEFT_CRICK;
					HitDispNo = battleButtonDispNo[ 0 ];
				}else if( ai==AI_GUARD && battleButtonDispNo[ 4 ]!=-2){
					mouse.onceState = MOUSE_LEFT_CRICK;
					HitDispNo = battleButtonDispNo[ 4 ];
				}else if( ai==AI_ESCAPE && battleButtonDispNo[ 7 ]!=-2){
					mouse.onceState = MOUSE_LEFT_CRICK;
					HitDispNo = battleButtonDispNo[ 7 ];
				}
			}
#endif
*/

			BattleButtonAttack();
			BattleButtonJujutsu();
			BattleButtonCapture();
			BattleButtonHelp();
			BattleButtonGuard();
			BattleButtonItem();
			BattleButtonPet();
			BattleButtonEscape();
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
			BattleButtonPPLSKILL();
#endif

			if( mouse.onceState & MOUSE_RIGHT_CRICK ) BattleButtonOff();
		}else{
			if( buttonA <= 5 ){
				buttonA++;
				buttonX += 60;
			}else{

				ClearBoxFlag();

				battleTargetSelectFlag = false;
				battleMenuFlag = false;

				if( BattleCntDownFlag == true ){
					battlePetMenuFlag = true;
				}else{
					SubProcNo++;
				}

				BattleCmdNo = -1;

				ClearBattleButton();
			}
		}
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
		battleButtonDispNo[ 0 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_ATT_UP 		- battleButtonFlag[ 0 ], 2 );
		battleButtonDispNo[ 1 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_JUJUTSU_UP 	- battleButtonFlag[ 1 ], 2 );
		battleButtonDispNo[ 2 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_CAPTURE_UP 	- battleButtonFlag[ 2 ], 2 );
		battleButtonDispNo[ 3 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_HELP_UP 		- helpFlag, 2 );
		battleButtonDispNo[ 4 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_DEF_UP 		- battleButtonFlag[ 4 ], 2 );
		battleButtonDispNo[ 5 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_ITEM_UP 		- battleButtonFlag[ 5 ], 2 );
		battleButtonDispNo[ 6 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_PET_UP 		- battleButtonFlag[ 6 ], 2 );
		battleButtonDispNo[ 7 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_ESCAPE_UP 	- battleButtonFlag[ 7 ], 2 );
		battleButtonDispNo[ 9 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BATTLE_PROSKL_UP	- wonflag , 2 );
		StockDispBuffer( buttonX, buttonY, DISP_PRIO_MENU, CG_BATTLE_PANEL, 0 );
#else
		battleButtonDispNo[ 0 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_ATTACK_UP 	+ battleButtonFlag[ 0 ], 2 );
		battleButtonDispNo[ 1 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_JUJUTU_UP 	+ battleButtonFlag[ 1 ], 2 );
		battleButtonDispNo[ 2 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_CAPTURE_UP 	+ battleButtonFlag[ 2 ], 2 );
		battleButtonDispNo[ 3 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_HELP_UP 		+ helpFlag, 2 );
		battleButtonDispNo[ 4 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_GUARD_UP 	+ battleButtonFlag[ 4 ], 2 );
		battleButtonDispNo[ 5 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_ITEM_UP 		+ battleButtonFlag[ 5 ], 2 );
		battleButtonDispNo[ 6 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_PET_UP 		+ battleButtonFlag[ 6 ], 2 );
		battleButtonDispNo[ 7 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_BTL_BUTTON_ESCAPE_UP 	+ battleButtonFlag[ 7 ], 2 );
		StockDispBuffer( buttonX, buttonY, DISP_PRIO_MENU, CG_BTL_BUTTON_BASE, 0 );
#endif

		if( CheckPetSuu() >= 5 ){
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
			StockDispBuffer( buttonX + 44, buttonY - 14, DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#else
			StockDispBuffer( buttonX + 28, buttonY - 18, DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#endif
		}
#ifdef _LOCKHELP_OK				// (?????) Syu ADD ??????????????
		extern int nowFloor;
		if((nowFloor <= 8213 && nowFloor >= 8200) ||
			 (nowFloor >= 30017 && nowFloor <= 30021)
		)	NoHelpFlag = true ; 
#endif
		if( NoHelpFlag == true ){
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
			StockDispBuffer( buttonX + 28 + 54 + 4 , buttonY - 18 + 4 , DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#else
			StockDispBuffer( buttonX + 28 + 54, buttonY - 18, DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#endif
		}
#ifdef _MAGIC_NOCAST
		if( NoCastFlag == true ){
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
			StockDispBuffer( buttonX + 28 -54 - 14, buttonY - 18 + 4, DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#else
			StockDispBuffer( buttonX + 28 -54, buttonY - 18, DISP_PRIO_IME4, CG_BTL_BUTTON_CROSS, 0 );
#endif
		}
#endif
	}

	if( battlePetMenuFlag == true ){
		buttonX = 815 + DISPLACEMENT_X;
		buttonY = 48;
		buttonA = 5;

		if( battlePetNoBak <= -1 || p_party[ BattleMyNo + 5 ]->hp <= 0 ){
			BattleCntDownFlag = false;
			SubProcNo++;
		}else if( BattleBpFlag & BATTLE_BP_PET_MENU_NON || p_party[ BattleMyNo ]->hp <= 0 || battlePlayerEscFlag == true ){
			if( bNewServer)
				lssproto_B_send( sockfd, "W|FF|FF" );
			else
				old_lssproto_B_send( sockfd, "W|FF|FF" );

			BattleCntDownFlag = false;
			SubProcNo++;
		}else{
			battleMenuFlag |= BATTLE_MENU_PET;

			battleMenuReturn = false; 

			play_se( 202, 320, 240 );

			ClearBoxFlag();

#ifdef _PETSKILL_DAMAGETOHP	  // ?輼:??????(??????????) 
			if( battleWazaTargetBak != -1 ){//????????????
				//????????????????????,????????????,???????????????????
				if( petSkill[battlePetNoBak][battleWazaTargetBak].field == 2 //???????????
					|| petSkill[battlePetNoBak][battleWazaTargetBak].skillId == 0 //Change fix ???????????????????,???????????????,??????????????????????,????????????????
					|| petSkill[battlePetNoBak][battleWazaTargetBak].skillId == 2 ){ 
				//if( petSkill[battlePetNoBak][battleWazaTargetBak].skillId == 623 ){ //623:??petskill2.txt????????
				    battleWazaTargetBak = -1;
				    battlePetButtonFlag = true;
				}
				else{
					
#ifdef _BATTLESKILL              // (?????) Syu ADD ??????????
//					BattleWazaNo = battlePetNoBak;
					BattleSetWazaHitBox( BattleWazaNo , 0 );
#else
					BattleSetWazaHitBox( BattleWazaNo );
#endif
				}
			}
#else
			if( battleWazaTargetBak != -1  ){ //????????????
#ifdef _BATTLESKILL				// (?????) Syu ADD ??????????
//				BattleWazaNo = battlePetNoBak;
				BattleSetWazaHitBox( BattleWazaNo , 0 );
#else
				BattleSetWazaHitBox( BattleWazaNo );
#endif
			}
#endif

		}
		BattleBpFlag &= ~BATTLE_BP_PET_MENU_NON;
		battlePetMenuFlag = false;
	}
	
	if( battleMenuFlag & BATTLE_MENU_PET ){ 
		
		if( battleMenuReturn == false ){
			
			if( buttonA > 0 ){
				buttonA--;
				buttonX -= 60;
			}
			
			BattleButtonWaza();
			
			if( mouse.onceState & MOUSE_RIGHT_CRICK ){ 
				battlePetButtonFlag = true;
				battleWazaTargetBak = -1;
			}
		}else{
			if( buttonA <= 5 ){
				buttonA++;
				buttonX += 60;
			}else{
				ClearBoxFlag();
				battleTargetSelectFlag = false;
				battleMenuFlag = false;
				p_party[ BattleMyNo + 5 ]->atr |= ACT_ATR_BTL_CMD_END;
				SubProcNo++;
			}
		}
		
		if( battleWazaTargetBak != -1 ){
			sprintf( moji,"%-24s",petSkill[ battlePetNoBak ][ BattleWazaNo ].name );
			StockFontBuffer( buttonX - 100, buttonY - 7, FONT_PRIO_FRONT, 0, moji, 0 );
			battleButtonDispNo[ 8 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_PET_BTL_BUTTON_CANCEL_UP, 2 );

		}else{
			battleButtonDispNo[ 8 ] = StockDispBuffer( buttonX, buttonY, DISP_PRIO_IME3, CG_PET_BTL_BUTTON_WAZA_UP + battleButtonFlag[ 8 ], 2 );
		}
		StockDispBuffer( buttonX, buttonY, DISP_PRIO_MENU, CG_PET_BTL_BUTTON_BASE, 0 );
	}

	BattleCntDownDisp();
}

void CloseInfoWnd()
{
	if( pActInfoWnd){
		DeathAction( pActInfoWnd);
		pActInfoWnd = NULL;
	}
}