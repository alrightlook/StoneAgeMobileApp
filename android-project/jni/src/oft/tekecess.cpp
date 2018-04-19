/************************/
/*	takecess.cpp		*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
//#include "../caryIme.h"
//#include "../systeminc/ime_sa.h"
#include "../systeminc/map.h"
#include "../systeminc/menu.h"
#include "../systeminc/battlemap.h"

#if 0

extern	ACTION* oft_test();
extern void damage_num( ACTION *a0 );
extern int piyo_point;

int flash_pal_cnt;
PALETTEENTRY	Palette2[256];	// ?????¢B¢l

/* ?©û????? *********************************************************/
void die_flash( void )
{
	int d0 = 0;
	int d1,d7;
	switch(flash_vct_no){
	case 0:
		break;

//??????????  ?©û  ??????????
	case 1:
		flash_vct_no++;
		break;

	case 2:
		for( d7 = 0; d7 < 10; d7++ ){
			Palette2[d7].peBlue  	= Palette[d7].peBlue;
			Palette2[d7].peGreen 	= Palette[d7].peGreen;
			Palette2[d7].peRed 		= Palette[d7].peRed;
			Palette2[d7].peFlags 	= Palette[d7].peFlags;

			Palette2[d7+246].peBlue  = Palette[d7+246].peBlue;
			Palette2[d7+246].peGreen = Palette[d7+246].peGreen;
			Palette2[d7+246].peRed 	= Palette[d7+246].peRed;
			Palette2[d7+246].peFlags = Palette[d7].peFlags;
		}
		for( d7 = 10; d7 < 246; d7++ ){		//??????????
			Palette2[d7].peBlue = Palette[d7].peBlue;
			Palette2[d7].peGreen = Palette[d7].peGreen;
			Palette2[d7].peRed = 0xff;
			Palette2[d7].peFlags = Palette[d7].peFlags;
		}
		lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
		flash_pal_cnt = 20;
		flash_vct_no++;
		break;

	case 3:
		if(--flash_pal_cnt){
			break;
		}
		flash_vct_no++;
		break;

	case 4:
		for(d7=10; d7<246; d7++){
			if(Palette2[d7].peRed == Palette[d7].peRed){
				continue;
			}
			d0 = 1;
			d1 = Palette2[d7].peRed - Palette[d7].peRed;
			if(d1 > 2)
				d1 = 2;
			if(d1 < -2)
				d1 = -2;
			Palette2[d7].peRed -= d1;
		}
		if(WindowMode){		//??????????
			flash_pal_cnt++;
			if(flash_pal_cnt == 10){		//¨Á??¦u??
				flash_pal_cnt = 0;
				lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
			} else {
				d0 = 1;		//ü¬????
			}
		} else {
			lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
		}
		if(d0 == 0){		//ü¬???
			flash_vct_no = 0;
		}
		break;

#if 0
//??????????  ?ýõ  ??????????
	case 4:
		for( d7 = 0; d7 < 10; d7++ ){
			Palette2[d7].peBlue  	= Palette[d7].peBlue;
			Palette2[d7].peGreen 	= Palette[d7].peGreen;
			Palette2[d7].peRed 		= Palette[d7].peRed;
			Palette2[d7].peFlags 	= Palette[d7].peFlags;

			Palette2[d7+246].peBlue  = Palette[d7+246].peBlue;
			Palette2[d7+246].peGreen = Palette[d7+246].peGreen;
			Palette2[d7+246].peRed 	= Palette[d7+246].peRed;
			Palette2[d7+246].peFlags = Palette[d7].peFlags;
		}
		for( d7 = 10; d7 < 246; d7++ ){		//??????????
			Palette2[d7].peBlue = 0xf0;
			Palette2[d7].peGreen = 0xf0;
			Palette2[d7].peRed = 0xf0;
			Palette2[d7].peFlags = Palette[d7].peFlags;
		}
		lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
		flash_pal_cnt = 0;
		flash_vct_no = 6;
		break;

//??????????  ?????  ??????????
	case 5:
		for( d7 = 0; d7 < 10; d7++ ){
			Palette2[d7].peBlue  	= Palette[d7].peBlue;
			Palette2[d7].peGreen 	= Palette[d7].peGreen;
			Palette2[d7].peRed 		= Palette[d7].peRed;
			Palette2[d7].peFlags 	= Palette[d7].peFlags;

			Palette2[d7+246].peBlue  = Palette[d7+246].peBlue;
			Palette2[d7+246].peGreen = Palette[d7+246].peGreen;
			Palette2[d7+246].peRed 	= Palette[d7+246].peRed;
			Palette2[d7+246].peFlags = Palette[d7].peFlags;
		}
		for( d7 = 10; d7 < 246; d7++ ){		//??????????
			Palette2[d7].peBlue = 0xff;
			Palette2[d7].peGreen = 0x80;
			Palette2[d7].peRed = 0x80;
			Palette2[d7].peFlags = Palette[d7].peFlags;
		}
		lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
		flash_pal_cnt = 0;
		flash_vct_no = 6;
		break;

//??????????  ?ýõ  ?  ?????  ??????????
	case 6:
		for(d7=10; d7<246; d7++){
			if(Palette2[d7].peRed != Palette[d7].peRed){
				d0 = 1;
				d1 = Palette2[d7].peRed - Palette[d7].peRed;
				if(d1 > 4)
					d1 = 4;
				if(d1 < -4)
					d1 = -4;
				Palette2[d7].peRed -= d1;
			}
			if(Palette2[d7].peGreen != Palette[d7].peGreen){
				d0 = 1;
				d1 = Palette2[d7].peGreen - Palette[d7].peGreen;
				if(d1 > 4)
					d1 = 4;
				if(d1 < -4)
					d1 = -4;
				Palette2[d7].peGreen -= d1;
			}
			if(Palette2[d7].peBlue != Palette[d7].peBlue){
				d0 = 1;
				d1 = Palette2[d7].peBlue - Palette[d7].peBlue;
				if(d1 > 4)
					d1 = 4;
				if(d1 < -4)
					d1 = -4;
				Palette2[d7].peBlue -= d1;
			}
		}
		if(WindowMode){		//??????????
			flash_pal_cnt++;
			if(flash_pal_cnt == 10){		//¨Á??¦u??
				flash_pal_cnt = 0;
				lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
			} else {
				d0 = 1;		//ü¬????
			}
		} else {
			lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette2 );
		}
		if(d0 == 0){		//ü¬???
			flash_vct_no = 0;
		}
		break;
#endif
	}
}
/* ???????£k¤úû¨ *********************************************************/
void damage_dispx( void )
{
	ACTION *pActLoop = pActTop->pNext; 	/* ¡I¥d????????ûè¥x */
	ACTION *pActLoopBak;  /* ?©û???????? */
	int d0;
	char szMoji[ 256 ];

	// ?????	
	while(1){
	
		/* ??§Ž???????? */	
		if( pActLoop == pActBtm ) break;

		/* þØ????? */
		if( pActLoop->deathFlag == false ){
		
			/* Pd( pActLoop->prio ); */
			/* ?????????ûÂ? */
			if( pActLoop->func == damage_num ){
				if(ATR_INT_WORK1(pActLoop) == 0){		//????
					sprintf( szMoji,"Miss" );
				} else if(ATR_INT_WORK1(pActLoop) == 1){		//???????
					sprintf( szMoji,"Counter" );
				} else if(ATR_INT_WORK1(pActLoop) == 3){		//?????
					sprintf( szMoji,"Guard" );
				} else {
					sprintf( szMoji,"%d", ATR_INT_WORK0(pActLoop) );
				}
				d0 = GetStrWidth( szMoji ) >> 1;
				StockFontBuffer( ATR_H_POS(pActLoop) - d0, ATR_V_POS(pActLoop), FONT_PRIO_BACK, ATR_INT_WORK1(pActLoop), szMoji, 0 );
			}
			
			/* ?????????? */
			pActLoop = pActLoop->pNext;
			
		}else{	/* ?????? */
		
			/* ¡q????? */
			pActLoop->pPrev->pNext = pActLoop->pNext;
			pActLoop->pNext->pPrev = pActLoop->pPrev;
			/* ?????? */
			pActLoopBak = pActLoop->pNext;
			/* ??????úÇ */
			ClearAction( pActLoop );
			/* ???????? */
			pActLoop = pActLoopBak;
		}
	}
}

/* ?????? ********************************************************************/
void TakeTestProc( void )
{
	s_timer++;
	switch( SubProcNo ){
		case 0:
			slow_flg = 0;
			oft_test();
			SubProcNo++;
			InitChat();
			GetKeyInputFocus( &MyChatBuffer );
			ReadBattleMap( 0 );
			break;
		
		case 1:
			if(joy_trg[0]&JOY_A){
				DeathAllAction();
				SubProcNo = 0;
				break;
			}
			FlashKeyboardCursor();
			if(slow_flg){
				if(!(s_timer&7)){
					piyo_point++;
					piyo_point &= 63;
				}
				if(s_timer&3){
					damage_dispx();		//???????£k¤úû¨
					goto TakeTestProc01_100;
				}
			} else {
				if(!(s_timer&1)){
					piyo_point++;
					piyo_point &= 63;
				}
			}
			/* ????????? */
			RunAction();

TakeTestProc01_100:
			if(flash_vct_no){		//?©û???????
				die_flash();		//?©û?????
			}

			// ???¤úû¨????????????
			StockTaskDispBuffer();
			
			/* ?????? */
			ChatProc();
			// ??????????????????????
			ChatBufferToFontBuffer(); 
			// ??????
			MenuProc();
			// ???????
			//ImeProc();		
			
			break;
	}
}
#endif

/* ?????? ********************************************************************/
void TakeTestProc( void ){}
