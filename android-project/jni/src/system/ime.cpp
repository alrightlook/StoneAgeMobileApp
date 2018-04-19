/************************/
/*	ime.cpp				*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
//#include "winnls32.h"
#include "../systeminc/font.h"
//#include "../caryIme.h"
#include "../systeminc/menu.h"
#include "../systeminc/battleMenu.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif

unsigned long dwInfo;
//void //ShowBottomLineString(int iColor,char* lpstr)
//{
	/*LPSTR lpstr1=GetImeString();
	dwInfo = 0;
	if(!lpstr1 && lpstr[0]!=0){
		dwInfo = 1;
		StockFontBuffer( 8, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, iColor, lpstr, 0 );
	}*/
//}

// Terry add 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
extern bool bShowItemExplain;
// end
//	输入法的处理
void ImeProc()
{
	// 显示输入法的名称
// Terry fix 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
	//if( TaskBarFlag == false ){ 这行改成下一行
//	if(TaskBarFlag == false && bShowItemExplain == false){
//// end
//		LPSTR lpstr=GetImeString();
//		char* lpstr1;
//		if(lpstr){
//#ifdef _TELLCHANNEL
//			StockFontBuffer(8,420 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,FONT_PAL_WHITE,lpstr,0);
//#else
//			StockFontBuffer(8,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,FONT_PAL_WHITE,lpstr,0);
//#endif
//#ifdef _TALK_WINDOW
//		if(g_bTalkWindow) TalkWindow.Update();
//#endif
//		}
//		lpstr1=GetImeDescString();
//		if( 1!=dwInfo){
//			if( lpstr1)
//				StockFontBuffer(530-strlen(lpstr1)*(FONT_SIZE>>1) + DISPLACEMENT_X,460 + DISPLACEMENT_Y ,FONT_PRIO_FRONT,0,lpstr1,0);
////			StockFontBuffer(590-strlen(lpstr)*(FONT_SIZE>>1),460,FONT_PRIO_FRONT,0,lpstr,0);
//			//cary 秀出ping的时间
//			char tmp[64];
//			extern unsigned long dwPingTime, dwPingState;
//			if( dwPingState & 0x80000000){
//				sprintf( tmp, "*%d", dwPingTime);
//				dwPingState++;
//				if( (dwPingState&0xff) > 40)
//					dwPingState = 0;
//			}else
//				sprintf( tmp, " %d", dwPingTime);
//			StockFontBuffer( 605 + DISPLACEMENT_X, 460 + DISPLACEMENT_Y , FONT_PRIO_FRONT, FONT_PAL_WHITE, tmp, 0 );
//#ifndef _REMAKE_20
//			lpstr = "玩家模式";
//			switch( AI){
//			case AI_ATTACK:
//				lpstr = "强制攻击";
//				break;
//			case AI_GUARD:
//				lpstr = "强制防御";
//				break;
//			case AI_SELECT:
//				lpstr = "ＡＩ模式";
//				break;
//			}
//			StockFontBuffer( 540 + DISPLACEMENT_X, 460 + DISPLACEMENT_Y , FONT_PRIO_FRONT, FONT_PAL_WHITE, lpstr, 0 );
//#endif
//#ifdef _CHANNEL_MODIFY
//			int FontColor = 0 ;
//			switch(TalkMode){
//			case 0:
//				lpstr1 = "一般频道";
//				break;
//			case 1:
//				FontColor = FONT_PAL_GREEN;
//				lpstr1 = "密语频道";
//				break;
//			case 2:
//				FontColor = FONT_PAL_AQUA;
//				lpstr1 = "队伍频道";
//				break;
//			case 3:
//				FontColor = FONT_PAL_PURPLE;
//				lpstr1 = "家族频道";
//				break;
//			case 4:
//				FontColor = FONT_PAL_BLUE2;
//				lpstr1 = "职业频道";
//				break;
//			case 5:
//				FontColor = FONT_PAL_PURPLE;
//				lpstr1 = "聊天室频道";
//				break;
//			}
//			StockFontBuffer( 10, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, FontColor, lpstr1, 0 );
//#else
//	#ifdef _TELLCHANNEL 
//			int FontColor = 0 ;
//			switch(TalkMode){
//			case 0:
//				lpstr1 = "一般模式";
//				break;
//			case 1:
//				FontColor = FONT_PAL_GREEN;
//				lpstr1 = "密语模式";
//				break;
//	#ifdef _FRIENDCHANNEL
//			case 2:
//				FontColor = FONT_PAL_PURPLE;
//				lpstr1 = "频道模式";
//				break;
//	#endif
//			}
//			StockFontBuffer( 10, 460 + DISPLACEMENT_Y, FONT_PRIO_FRONT, FontColor, lpstr1, 0 );
//	#endif
//#endif
//		}
//	}
//// Terry add 2003/12/16 for 交易视窗开启时,显示物品说明不显示输入法
//	bShowItemExplain = false;
// end
}

