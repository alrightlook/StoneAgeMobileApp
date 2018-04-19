/*-----------------------------------------------------------------
	§[???????üº
-------------------------------------------------------------------*/
//#include <windows.h>

#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "vg410.h"
#include "work.h"
//#include "../caryIme.h"
#include "../systeminc/anim_tbl.h"  
#include "../systeminc/pc.h"        
#include"../systeminc/netmain.h"
#include"../systeminc/lssproto_cli.h"
#include "../systeminc/character.h"
#include"../systeminc/map.h"
//#include <dinput.h>
#define RELEASE(x) 	if(x){x->Release();x=NULL;}

/*-------------------------------------------
	??¨Á?
--------------------------------------------*/

extern void* hInst;
#ifndef DI_2
//LPDIRECTINPUT8 pDInput;
//LPDIRECTINPUT8 pDInput2;
#else
//LPDIRECTINPUT pDInput;
//LPDIRECTINPUT pDInput2;
#endif

#ifdef _MIND_ICON
bool m_bt = false;
#endif

/*??????????  ????????????  ??????????*/
#define		REPEAT_TIME	30
#define		REPEAT_SPEED	7
Uint32_ joy_auto_repeat(Uint8 d1)
{
	Uint32_ d0=0;
	if( (joy_con[d1] & JOY_UP+JOY_DOWN+JOY_RIGHT+JOY_LEFT+JOY_A+JOY_B+JOY_F5+JOY_F6+JOY_F7+JOY_F8)==0  ){	/*???????*/
		joy_timer[d1]=0;		/*?????????????*/
		return 0;
	}
	if(joy_trg[d1] & JOY_UP){				/*ýÆ?*/
		d0|=JOY_UP;					/*????ýÆ???*/
	} else {
		if(joy_con[d1] & JOY_UP){			/*ýÆ?*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_UP;
			}
		}
	}
	if(joy_trg[d1] & JOY_DOWN){				/*??*/
		d0|=JOY_DOWN;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_DOWN){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_DOWN;
			}
		}
	}
	if(joy_trg[d1] & JOY_RIGHT){				/*??*/
		d0|=JOY_RIGHT;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_RIGHT){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_RIGHT;
			}
		}
	}
	if(joy_trg[d1] & JOY_LEFT){				/*??*/
		d0|=JOY_LEFT;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_LEFT){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_LEFT;
			}
		}
	}
	
#if 1
	if(joy_trg[d1] & JOY_A){				/*A?*/
		d0|=JOY_A;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_A){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_A;
			}
		}
	}
	
	if(joy_trg[d1] & JOY_B){				/*B?*/
		d0|=JOY_B;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_B){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_B;
			}
		}
	}
#endif	
	
	if(joy_trg[d1] & JOY_F5){				/*F5?*/
		d0|=JOY_F5;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_F5){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_F5;
			}
		}
	}
	if(joy_trg[d1] & JOY_F6){				/*F6?*/
		d0|=JOY_F6;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_F6){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_F6;
			}
		}
	}
	if(joy_trg[d1] & JOY_F7){				/*F7?*/
		d0|=JOY_F7;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_F7){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_F7;
			}
		}
	}
	if(joy_trg[d1] & JOY_F8){				/*F8?*/
		d0|=JOY_F8;					/*????????*/
	} else {
		if(joy_con[d1] & JOY_F8){			/*??*/
			joy_timer[d1]++;	/*??*/
			if(joy_timer[d1]==REPEAT_TIME){
				joy_timer[d1]-=REPEAT_SPEED;
				d0|=JOY_F8;
			}
		}
	}
	return d0;
}

#if 0
/*??????????  ??????¥‚????????§\?  ??????????*/
void joy_read(void)
{
	Uint16	d0,d1;
	Uint16 a,c;
	d0=0;						//??¢Dûè???
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		d0|=JOY_LEFT;
	if(GetAsyncKeyState(VK_UP) & 0x8000)
		d0|=JOY_UP;
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		d0|=JOY_RIGHT;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		d0|=JOY_DOWN;
	if(GetAsyncKeyState(VK_Z) & 0x8000)
		d0|=JOY_A;
	if(GetAsyncKeyState(VK_X) & 0x8000)
		d0|=JOY_B;
	if(GetAsyncKeyState(VK_G) & 0x8000)
		d0|=JOY_A;
	if(GetAsyncKeyState(VK_H) & 0x8000)
		d0|=JOY_B;
	joy_con[0] = d0;

	d0=0;						//??¢Dûè???
	if(GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
		d0|=JOY_LEFT;
	if(GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
		d0|=JOY_UP;
	if(GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
		d0|=JOY_RIGHT;
	if(GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
		d0|=JOY_DOWN;
	if(GetAsyncKeyState(VK_A) & 0x8000)
		d0|=JOY_A;
	if(GetAsyncKeyState(VK_S) & 0x8000)
		d0|=JOY_B;
	if(GetAsyncKeyState(VK_T) & 0x8000)
		d0|=JOY_A;
	if(GetAsyncKeyState(VK_Y) & 0x8000)
		d0|=JOY_B;
	joy_con[1] = d0;

	d1 = ~(joy_buf[0]);				//??¢D???
	joy_trg[0] = d1 & joy_con[0];

	d1 = ~(joy_buf[1]);				//??¢D???
	joy_trg[1] = d1 & joy_con[1];

	c = ~(joy_con[0]);				//??¢D¢¬úÇ?
	a = ~(joy_buf[0]);
	a ^= c;
	a &= c;
	joy_pul[0] = a;

	c = ~(joy_con[1]);				//??¢D¢¬úÇ?
	a = ~(joy_buf[1]);
	a ^= c;
	a &= c;
	joy_pul[1] = a;

	joy_buf[0] = joy_con[0];
	joy_buf[1] = joy_con[1];

	joy_auto[0] = joy_auto_repeat(0);
	joy_auto[1] = joy_auto_repeat(1);
}
#else
/*-------------------------------------------
	?????????ûè¥x
---------------------------------------------*/
//bool CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,LPVOID pvRef)
//{
//	int ret;
//
//#ifndef DI_2
//	//??????????????????????þÎ
//	ret = pDInput->CreateDevice(lpddi->guidInstance,&pDInputDevice2,NULL);
//	if(ret != DI_OK){
//		return DIENUM_CONTINUE;
//	}
//#else
//	LPDIRECTINPUTDEVICE pDev;
//	//??????????????????????þÎ
//	ret = pDInput->CreateDevice(lpddi->guidInstance,&pDev,NULL);
//	if(ret != DI_OK){
//		return DIENUM_CONTINUE;
//	}
//	pDev->QueryInterface(IID_IDirectInputDevice2,(LPVOID *)&pDInputDevice2);
//#endif
//	return DIENUM_STOP;
//}

/*??????????  ??????¥‚????????????????§\?  ??????????*/
void joy_read(void)
{
//	Uint32_	d0,d1;
//	Uint32_ a,c;
//	int ret = DI_OK;
//	DIJOYSTATE dijs;
//
//	if(keyboad_flg == false){		//?????????
//		return;
//	}
//
//	d0=0;						
//	
//	//??¢Dûè???
//	
//	if( DInputActiveFlag == true ){
//		if(joy_flg == true){		//????????£Ï£Ë??
//			pDInputDevice2->Poll();		//????????¥‚???
//			ret = pDInputDevice2->GetDeviceState(sizeof(DIJOYSTATE),&dijs);
//			if(ret == DI_OK){
//				if(dijs.lX > 500)
//					d0|=JOY_RIGHT;
//				if(dijs.lX < -500)
//					d0|=JOY_LEFT;
//				if(dijs.lY > 500)
//					d0|=JOY_DOWN;
//				if(dijs.lY < -500)
//					d0|=JOY_UP;
//				if(dijs.rgbButtons[0] & 0x80)
//					d0|=JOY_A;
//				if(dijs.rgbButtons[1] & 0x80)
//					d0|=JOY_B;
//			}
//			else if(ret == DIERR_INPUTLOST){
//				pDInputDevice2->Acquire();
//			}
//		}
//		ret = pDInputDevice->GetDeviceState(256,di_key);		//?????¥‚???
//	}
//	if(ret == DI_OK){
//		if(di_key[DIK_LEFT] & 0x80)
//			d0|=JOY_LEFT;
//		if(di_key[DIK_UP] & 0x80)
//			d0|=JOY_UP;
//		if(di_key[DIK_RIGHT] & 0x80)
//			d0|=JOY_RIGHT;
//		if(di_key[DIK_DOWN] & 0x80)
//			d0|=JOY_DOWN;
//		//if(di_key[DIK_Z] & 0x80)
//		//	d0|=JOY_A;
//		//if(di_key[DIK_X] & 0x80)
//		//	d0|=JOY_B;
//		//if(di_key[DIK_G] & 0x80)
//		//	d0|=JOY_A;
//		//if(di_key[DIK_H] & 0x80)
//		//	d0|=JOY_B;
//			
//		// ???? ?????????
//		// ESC ??
//		if(di_key[DIK_ESCAPE] & 0x80){
//			// ?????????§ó??þ†??
//			//if( ImeBufferBak2 == NULL ) 
//			d0|=JOY_ESC;
//		}
//		
//		// ??????? Ctrl ???????????
//		if( di_key[DIK_RCONTROL] & 0x80 || di_key[DIK_LCONTROL] & 0x80 ){ 
//			// Ctrl + M
//			if( di_key[DIK_M] & 0x80 )	d0|=JOY_CTRL_M;
//			// Ctrl + S
//			if( di_key[DIK_S] & 0x80 )	d0|=JOY_CTRL_S;
//			// Ctrl + P
//			if( di_key[DIK_P] & 0x80 )	d0|=JOY_CTRL_P;
//			// Ctrl + I
//			if( di_key[DIK_I] & 0x80 )	d0|=JOY_CTRL_I;
//			// Ctrl + E
//			if( di_key[DIK_E] & 0x80 )	d0|=JOY_CTRL_E;
//			// Ctrl + A
//			if( di_key[DIK_A] & 0x80 )	d0|=JOY_CTRL_A;
//			// Ctrl + C
//			if( di_key[DIK_C] & 0x80 )	d0|=JOY_CTRL_C;
//			// Ctrl + V
//			if( di_key[DIK_V] & 0x80 )	d0|=JOY_CTRL_V;
//			// Ctrl + Z
//			if( di_key[DIK_Z] & 0x80 )	d0|=JOY_A;
//			// Ctrl + X
//			if( di_key[DIK_X] & 0x80 )	d0|=JOY_B;
//			// Ctrl + T
//			//if( di_key[DIK_T] & 0x80 )	d0|=JOY_CTRL_T;			
//
//			
//		}
//		
//		// ¥{ûì??
//		if( di_key[DIK_PRIOR]	& 0x80)		d0|=JOY_P_UP;
//		if( di_key[DIK_NEXT] 	& 0x80)		d0|=JOY_P_DOWN;
//		if( di_key[DIK_HOME] 	& 0x80)		d0|=JOY_HOME;
//		if( di_key[DIK_END] 	& 0x80)		d0|=JOY_END;
//		if( di_key[DIK_INSERT] 	& 0x80)		d0|=JOY_INS;
//		if( di_key[DIK_DELETE] 	& 0x80)		d0|=JOY_DEL;
//		
//		
//		
//		joy_con[0] = d0;
//
//		d0=0;
//		
//		//??¢Dûè???
//		// ???¨Á???þ†??
//		if( GetImeString() == NULL ){
//			
//			// ?????????
//			if(di_key[DIK_F1] & 0x80) d0|=JOY_F1;
//			if(di_key[DIK_F2] & 0x80) d0|=JOY_F2;
//			if(di_key[DIK_F3] & 0x80) d0|=JOY_F3;
//			if(di_key[DIK_F4] & 0x80) d0|=JOY_F4;
//			
//			if(di_key[DIK_F5] & 0x80) d0|=JOY_F5;
//			if(di_key[DIK_F6] & 0x80) d0|=JOY_F6;
//			if(di_key[DIK_F7] & 0x80) d0|=JOY_F7;
//			if(di_key[DIK_F8] & 0x80) d0|=JOY_F8;
//			
//			if(di_key[DIK_F9] & 0x80) d0|=JOY_F9;
//			if(di_key[DIK_F10] & 0x80) d0|=JOY_F10;
//			if(di_key[DIK_F11] & 0x80) d0|=JOY_F11;
//			if(di_key[DIK_F12] & 0x80) d0|=JOY_F12;
//		}
//		// ??????? Ctrl ???????????
//		if( di_key[DIK_RCONTROL] & 0x80 || di_key[DIK_LCONTROL] & 0x80 ){ 
//			// ??????????????
//			if(di_key[DIK_1] & 0x80) d0|=JOY_CTRL_1;
//			if(di_key[DIK_2] & 0x80) d0|=JOY_CTRL_2;
//			if(di_key[DIK_3] & 0x80) d0|=JOY_CTRL_3;
//			if(di_key[DIK_4] & 0x80) d0|=JOY_CTRL_4;
//			if(di_key[DIK_5] & 0x80) d0|=JOY_CTRL_5;
//			if(di_key[DIK_6] & 0x80) d0|=JOY_CTRL_6;
//			if(di_key[DIK_7] & 0x80) d0|=JOY_CTRL_7;
//			if(di_key[DIK_8] & 0x80) d0|=JOY_CTRL_8;
//			if(di_key[DIK_9] & 0x80) d0|=JOY_CTRL_9;
//			if(di_key[DIK_0] & 0x80) d0|=JOY_CTRL_0;
//			if(di_key[DIK_MINUS] & 0x80) 	d0|=JOY_CTRL_MINUS;
//			//cary Ê®Áù
//			if(di_key[DIK_EQUALS] & 0x80) 	d0|=JOY_CTRL_CIRCUMFLEX;
//			if(di_key[DIK_BACKSLASH] & 0x80) d0|=JOY_CTRL_YEN;
//			if(di_key[DIK_J] & 0x80) d0|=JOY_CTRL_J;
//			if(di_key[DIK_Q] & 0x80) d0|=JOY_CTRL_Q;
//			if(di_key[DIK_W] & 0x80) d0|=JOY_CTRL_W;
//			if(di_key[DIK_G] & 0x80) d0|=JOY_CTRL_G;
//			
//			// Robin 0607 channel
//			if(di_key[DIK_C] & 0x80) d0|=JOY_CTRL_C;
//
//			#ifdef _TELLCHANNEL				//ROG ADD ÃÜÓïÆµµÀ
//			if( di_key[DIK_R] & 0x80 ) d0|=JOY_CTRL_R;
//			#endif
//
//		}
//		
//		// ?????????
//		//if(di_key[DIK_SYSRQ] & 0x80) d0|=JOY_PRINT_SCREEN;
//		// ????
//		if(di_key[DIK_TAB] & 0x80) d0|=JOY_TAB;
//		// ?????
//		if(di_key[DIK_RSHIFT] & 0x80) d0|=JOY_RSHIFT;
//		if(di_key[DIK_LSHIFT] & 0x80) d0|=JOY_LSHIFT;
//		// ??????
//		//if(di_key[DIK_RETURN] & 0x80) d0|=JOY_RETURN;
//
//#ifdef _MIND_ICON		
//		if( (pc.ptAct) && ((di_key[DIK_LMENU] & 0x80) || (di_key[DIK_RMENU] & 0x80)) && pc.ptAct->pYobi ){
//			CHAREXTRA *ext;	
//			ext = (CHAREXTRA *)pc.ptAct->pYobi;
//			if(!ext->ptMindIcon){
//				if(!m_bt && (di_key[DIK_Q] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_heart);			    
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_heart);
//				}else if(!m_bt && (di_key[DIK_W] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_broken);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_broken);
//				}else if(!m_bt && (di_key[DIK_E] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_m_clip);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_m_clip);
//				}else if(!m_bt && (di_key[DIK_R] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_m_stone);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_m_stone);
//				}else if(!m_bt && (di_key[DIK_T] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_m_burlap);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_m_burlap);
//				}else if(!m_bt && (di_key[DIK_Y] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_screamer);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_screamer);
//				}else if(!m_bt && (di_key[DIK_U] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_question_mark);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_question_mark);
//				}else if(!m_bt && (di_key[DIK_I] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_cramp);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_cramp);
//				}else if(!m_bt && (di_key[DIK_O] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_black_squall);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_black_squall);
//				}else if(!m_bt && (di_key[DIK_P] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_perspiration);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_perspiration);
//				}else if(!m_bt && (di_key[DIK_A] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_silence);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_silence);
//				}else if(!m_bt && (di_key[DIK_S] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_asleep);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_asleep);
//				}else if(!m_bt && (di_key[DIK_D] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_cafe);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_cafe);
//				}else if(!m_bt && (di_key[DIK_F] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_lightbulb);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_lightbulb);
//				}else if(!m_bt && (di_key[DIK_G] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_accent);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_accent);
//				}else if(!m_bt && (di_key[DIK_H] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_money);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_money);
//				}else if(!m_bt && (di_key[DIK_J] & 0x80)){
//					m_bt = true; setCharMind( pc.ptAct, SPR_pk);				
//					lssproto_MA_send( sockfd, nowGx, nowGy, SPR_pk);
//				}
//				if(!(di_key[0x10]&0x80) && !(di_key[0x11]&0x80) && !(di_key[0x12]&0x80) &&
//				   !(di_key[0x13]&0x80) && !(di_key[0x14]&0x80) && !(di_key[0x15]&0x80) &&
//				   !(di_key[0x16]&0x80) && !(di_key[0x17]&0x80) && !(di_key[0x18]&0x80) &&
//				   !(di_key[0x19]&0x80) && !(di_key[0x1E]&0x80) && !(di_key[0x1F]&0x80) &&
//				   !(di_key[0x20]&0x80) && !(di_key[0x21]&0x80) && !(di_key[0x22]&0x80) &&
//				   !(di_key[0x23]&0x80) && !(di_key[0x24]&0x80)
//				  )
//					m_bt = false;
//			}
//		}		
//#endif
//
//#if 0		
//		if(di_key[DIK_NUMPAD4] & 0x80)
//			d0|=JOY_LEFT;
//		if(di_key[DIK_NUMPAD8] & 0x80)
//			d0|=JOY_UP;
//		if(di_key[DIK_NUMPAD6] & 0x80)
//			d0|=JOY_RIGHT;
//		if(di_key[DIK_NUMPAD2] & 0x80)
//			d0|=JOY_DOWN;
//		if(di_key[DIK_A] & 0x80)
//			d0|=JOY_A;
//		if(di_key[DIK_S] & 0x80)
//			d0|=JOY_B;
//		if(di_key[DIK_T] & 0x80)
//			d0|=JOY_A;
//		if(di_key[DIK_Y] & 0x80)
//			d0|=JOY_B;
//#endif			
//		
//		
//		joy_con[1] = d0;
//
//		d1 = ~(joy_buf[0]);				//??¢D???
//		joy_trg[0] = d1 & joy_con[0];
//
//		d1 = ~(joy_buf[1]);				//??¢D???
//		joy_trg[1] = d1 & joy_con[1];
//
//		c = ~(joy_con[0]);				//??¢D¢¬úÇ?
//		a = ~(joy_buf[0]);
//		a ^= c;
//		a &= c;
//		joy_pul[0] = a;
//
//		c = ~(joy_con[1]);				//??¢D¢¬úÇ?
//		a = ~(joy_buf[1]);
//		a ^= c;
//		a &= c;
//		joy_pul[1] = a;
//
//		joy_buf[0] = joy_con[0];
//		joy_buf[1] = joy_con[1];
//
//		joy_auto[0] = joy_auto_repeat(0);
//		joy_auto[1] = joy_auto_repeat(1);
//	}
//	else if(ret == DIERR_INPUTLOST){
//		pDInputDevice->Acquire();
//	}
}
#endif
/*??????????  ??????????????????????  ??????????*/
/*-------------------------------------------
	Direct Input ???
---------------------------------------------*/
int InitDInput(void)
{
    return 0;
//	int ret;
//
//	if( GetAsyncKeyState( VK_CONTROL ) & 0x8000 ){		//????????????
//		return false;
//	}
//#ifndef DI_2
//	ret = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
//	if(ret != DI_OK){
//		return false;
//	}
//#else
//	ret = DirectInputCreate(hInst,DIRECTINPUT_VERSION,&pDInput,NULL);
//	if(ret != DI_OK){
//		return false;
//	}
//#endif
//	//???????????????????þÎ
//	ret = pDInput->CreateDevice(GUID_SysKeyboard,&pDInputDevice,NULL);
//	if(ret != DI_OK){
//		RELEASE(pDInput);
//		return false;
//	}
//
//	//??????????¡@¤e
//	ret = pDInputDevice->SetDataFormat(&c_dfDIKeyboard);
//	if(ret != DI_OK){
//		RELEASE(pDInputDevice);
//		RELEASE(pDInput);
//		return false;
//	}
//
//	//????¡@¤e
//	ret = pDInputDevice->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//	if(ret != DI_OK){
//		RELEASE(pDInputDevice);
//		RELEASE(pDInput);
//		return false;
//	}
//
//	pDInputDevice->Acquire();
//
//#ifndef DI_2
//	ret = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
//	if(ret != DI_OK){
//		return false;
//	}
//#else
//	ret = DirectInputCreate(hInst,DIRECTINPUT_VERSION,&pDInput2,NULL);
//	if(ret != DI_OK){
//		return false;
//	}
//#endif
//	keyboad_flg = true;		//?????£Ï£Ë
//
//	joy_flg = false;		//????????þtúé?
//	return true;
//
//	//?????????£S?
//	pDInputDevice2 = NULL;
//
//#ifndef DI_2
//	pDInput2->EnumDevices( DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)GetJoystickCallback, NULL, DIEDFL_ATTACHEDONLY);
//	if(pDInputDevice2 == NULL){
//		RELEASE(pDInput2);
//		return false;
//	}
//#else
//	pDInput2->EnumDevices(DIDEVTYPE_JOYSTICK,(LPDIENUMDEVICESCALLBACK)GetJoystickCallback,NULL,DIEDFL_ATTACHEDONLY);
//	if(pDInputDevice2 == NULL){
//		RELEASE(pDInput2);
//		return false;
//	}
//#endif
//	//??????????¡@¤e
//	ret = pDInputDevice2->SetDataFormat(&c_dfDIJoystick);
//	if(ret != DI_OK){
//		RELEASE(pDInputDevice2);
//		RELEASE(pDInput2);
//		return false;
//	}
//
//	//????¡@¤e
//	ret = pDInputDevice2->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
//	if(ret != DI_OK){
//		RELEASE(pDInputDevice2);
//		RELEASE(pDInput2);
//		return false;
//	}
//
//	DIPROPRANGE diprg; 
// 
//	// û´?£k?§d??¡@¤e
//	diprg.diph.dwSize	= sizeof(diprg); 
//	diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
//	diprg.diph.dwObj	= DIJOFS_X; 
//	diprg.diph.dwHow	= DIPH_BYOFFSET; 
//	diprg.lMin	= -1000; 
//	diprg.lMax	= +1000; 
// 	pDInputDevice2->SetProperty(DIPROP_RANGE, &diprg.diph);
//	diprg.diph.dwObj	= DIJOFS_Y; 
// 	pDInputDevice2->SetProperty(DIPROP_RANGE, &diprg.diph);
//
//	pDInputDevice2->Acquire();
//
//	joy_flg = true;		//????????£Ï£Ë
//	return true;
}
/*-------------------------------------------
	Direct Input ??
---------------------------------------------*/
int EndDInput(void)
{
	//?????£Ï£Ë??
	/*if(keyboad_flg == true){
		pDInputDevice->Unacquire();
		RELEASE(pDInputDevice);
		RELEASE(pDInput);
	}
	????????£Ï£Ë??
	if(joy_flg == true){
		pDInputDevice2->Unacquire();
		RELEASE(pDInputDevice2);
		RELEASE(pDInput2);
	}*/

	return true;
}

