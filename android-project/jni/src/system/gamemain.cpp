/************************/	
/*	gamemain.cpp		*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
//#include <winuser.h>
//#include <winbase.h>
#include "../systeminc/loadrealbin.h"
#include "../systeminc/loadsprbin.h"
#include "../oft/sys2.h"
//#include "../caryIme.h"
//#include "../systeminc/ime_sa.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/battleMap.h"
#include "../systeminc/netmain.h"
#include "../systeminc/savedata.h"
#include "../systeminc/produce.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/t_music.h"
#include "../systeminc/menu.h"
#include "../systeminc/handletime.h"
#include "../systeminc/battleMenu.h"
#include "../systeminc/character.h"
#include "SDL.h"
#include <stdlib.h>

#include "../systeminc/pc.h"
#include "../systeminc/map.h"
#include "../NewProto/autil.h"
#include "../NewProto/protocol.h"
//#include <tlhelp32.h>
#ifdef _REMAKE_20
#include "../MMOGprotect.h"
#endif
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif

#include "../systeminc/field.h"

int blitFlag = 0;
extern bool isShopItemHover;
//#include   <Nb30.h> 
//#pragma comment(lib,"netapi32.lib")

#define NO_DRAW_MAX_CNT 2	// ��?????��?????
//#define FRAME_SKIP	1		// ???????? ??��?  ???��?

// ????????��?�v???�L?
void ChangeWindowMode( void );
// ??�T???�@�e??
void SetResoMode(int Mode);
void StrToNowStrBuffer( char *str );

//#ifndef _DEBUG
//	#define _DEBUG	// ????????????
//#endif

int testCnt;
static char no_wait_cnt = 0;
int debug_info_flag = 0;

//---------------------------------------------------------------------------//
// ?????��?�e?                                                        //
//---------------------------------------------------------------------------//

// ????????��?
int	  FrameRate;				// ???????
int	  DrawFrameCnt;				// ?��????��???????
unsigned long DrawFrameTime;			// ?��??????????
#ifdef _DEBUG
static int palNo = 0;			// ????�k?
static int fontNo = 0;			// ????�k?
#endif

int	GameState;
unsigned long ProcTime = 14;
unsigned long SystemTime = 14;	//14 msc
extern SDL_Surface* screenSurface;
extern SDL_Surface* battleSurface;

unsigned long NowTime;
int	  NoDrawCnt = 1;
int BackBufferDrawType;
#ifdef _READ16BITBMP
bool	g_bMMXMode;	// CPU �Ƿ���MMXָ�
#endif
#ifdef _REMAKE_20
static unsigned long dwDisableInputScriptTime = 0;	// ÿ�����ִ��һ��DisableInputScriptTime()
#endif

//unsigned long WINAPI CheckTickCount(LPVOID lpParam);

float getDistOfPoints(int x1, int y1, int x2, int y2)
{
	return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

bool isTextInputHit(int x, int y)
{
	if(pNowStrBuffer == 0) {
		return false;
	}

	if ( x > pNowStrBuffer->x && x < pNowStrBuffer->x + 12 * pNowStrBuffer->len && y > pNowStrBuffer->y && y < pNowStrBuffer->y+12)
	{
		return true;
	}
	return false;
}
#ifdef _CHECK_GAMESPEED
void HighSpeedCheck( void )
{
#define HIGH_SPEED_CHECK_INTERVAL 60*60
	static int SpeedCnt=0;
	static float delays=0;
	if( init_net == false ) return;
	SpeedCnt++;
	if( SpeedCnt >= HIGH_SPEED_CHECK_INTERVAL ){
		int delaytimes = lssproto_getdelaytimes();
		if( delaytimes > 0 ){
			lssproto_setdelaytimes( 0);
			delays += delaytimes*10;
		}else{
			if( delays > 0 ) delays--;
		}
		SpeedCnt = 0;
		lssproto_CS_send( sockfd);	//��̽���server
	}
	if( (int)delays > 0 ){
		for( int i=0; i<((int)delays+1000); i++){}
	}
}
#endif
int HideSa = 0;
static unsigned long nowtime = 0;
extern SDL_Window* mainWindow;
extern SDL_Renderer* mainRenderer;
bool isRightClick = false;
bool GameMain( void )
{
	if( InitGame() == false ){
		////MessageBox( hWnd, "��Ϸ�趨��ʼ��ʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
		return false;
	}
	SDL_SetHint(SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1");
    SetResoMode( ResoMode );
    InitDirectDraw();
	char sz[32];
	
//	itoa( TimeGetTime()^0xffffbcde, sz, 10);
    sprintf(sz, "%d", TimeGetTime()^0xffffbcde);
#ifdef SDL_SA
    bool quit = false;
    InitOffScreenSurface();
    ChangeProc(PROC_ID_PASSWORD);
	static int fps = SDL_GetTicks();
	isRightClick = false;
    while(!quit)
    {
		static long lastClickTime = SDL_GetTicks();
		fps = SDL_GetTicks();
        SDL_RenderClear(mainRenderer);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN)
            {
                //quit = true;
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    quit = true;
                }
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					SDL_StopTextInput();
					KeyboardReturn();
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE)
				{
					KeyboardBackSpace();
				}
            }
            else if(event.type == SDL_QUIT) {
                quit = true;
            }
			else if (event.type == SDL_FINGERDOWN) {
				int x, y;
				long dt = SDL_GetTicks() - lastClickTime;
				lastClickTime = SDL_GetTicks();
				x = event.tfinger.x * 640;
				y = event.tfinger.y * 480;
				if (isTextInputHit(x, y))
				{
					SDL_StartTextInput();
				}
				//SDL_Log("Jerry %d", x);
				//SDL_Log("Jerry %d", y);
				if (getDistOfPoints(x, y, mouse.nowPoint.x, mouse.nowPoint.y) < 20 && dt < 500) {
                    MouseDblCrickLeftUpPoint(x, y);
					MouseNowPoint(x, y);
                }
                else {					
					
					MouseNowPoint(x , y);
					
					if (isShopItemHover) 
					{	
					}
					else
					{
						MouseCrickLeftDownPoint(x, y);
					}
					
                    //MouseCrickLeftDownPoint(x, y);
					//MouseCrickLeftUpPoint(x, y);
                }
				HitMouseCursor();	
			}
			else if (event.type == SDL_FINGERUP) {
				int x, y;
				x = event.tfinger.x * 640;
				y = event.tfinger.y * 480;
				MouseCrickLeftUpPoint(x, y);
				isRightClick = ~isRightClick;
				//HitMouseCursor();
				//MouseNowPoint(x, y);
				//MouseCrickLeftUpPoint(x, y);
				
			}
			else if (event.type == SDL_FINGERMOTION)
			{
				int x, y;
				x = event.tfinger.x * 640;
				y = event.tfinger.y * 480;
				MouseNowPoint(x, y);
			}
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // if (event.button.clicks == 2) {
                //     MouseDblCrickLeftUpPoint(event.button.x, event.button.y);
                // }
                // else {
                //     MouseCrickLeftDownPoint(event.button.x, event.button.y);
                // }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                // MouseCrickLeftUpPoint(event.button.x, event.button.y);
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                //MouseNowPoint(event.button.x, event.button.y);
            }
            else if(event.type == SDL_TEXTEDITING)
            {
                //OutputDebugStringA(event.text.text);
            }
            else if(event.type == SDL_TEXTINPUT)
            {
                //OutputDebugStringA(event.text.text);
				//SDL_Log("Jerry, Key input Text: %c", event.text.text[0]);
                //StockStrBufferChar( event.text.text[0] );
				StrToNowStrBuffer(event.text.text);
            }
        }	
			
		for (blitFlag = 0; blitFlag < 8; blitFlag++)
		{
			memset(&DispBuffer,0,sizeof(DispBuffer));
			FontCnt = 0;
			Process();
			MouseProc();
			HitMouseCursor();
		}
		

        SortDispBuffer();
        
        PaletteProc();

		switch( BackBufferDrawType ){
		
			case DRAW_BACK_NON:
				break;
				
			case DRAW_BACK_NORMAL:
				ClearBackSurface();	
				break;
				
			case DRAW_BACK_PRODUCE:
				break;
				
			case DRAW_BACK_BATTLE:	// ??????��?
				ClearBackSurface();	
				DrawBattleMap();
				break;
				
		}
		
        if( BackBufferDrawType != DRAW_BACK_PRODUCE){
            PutBmp();
        }
        
		baseXFastDraw = nowXFastDraw;
		baseYFastDraw = nowYFastDraw;
		baseXFastDraw2 = nowXFastDraw2;
		baseYFastDraw2 = nowYFastDraw2;

        networkLoop();

        SDL_DestroyTexture(screenTexture);
        screenTexture = SDL_CreateTextureFromSurface(mainRenderer, screenSurface);
        SDL_RenderCopy(mainRenderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(mainRenderer);
        SurfaceDate++;

		

		int dt = SDL_GetTicks() - fps;
		
		//SDL_Log("Jerry The FPS is %d", dt);

        static unsigned long reset = SDL_GetTicks() + 120000;
		static unsigned long flip = 1;



    }
#else

	while( 1 ){         //Jerry ��Ϸ��ѭ��

		if( SystemTask() == false ){
			EndGame();
			return false;
		}
#ifdef SA_25
		if( !lpDraw){
			Sleep( 1);
			continue;
		}
#endif
//cary
#ifdef _CHECK_GAMESPEED
		HighSpeedCheck();
#endif
//end
#ifdef _REMAKE_20
#ifndef _DEBUG
		if(dwDisableInputScriptTime + 300000 < TimeGetTime()){
			DisableInputScript();
			dwDisableInputScriptTime = TimeGetTime();
		}
#endif
#endif
		RealTimeToSATime( &SaTime );
		DispBuffer.DispCnt = 0;
		FontCnt = 0;
#ifdef _DEBUG
		if( !offlineFlag )
			networkLoop();
#else
		networkLoop();
#endif



		joy_read();
#if 1
		if( joy_trg[ 1 ] & JOY_F12 ){
			static unsigned int prePushTime = 0;
			if( prePushTime + 500 <= TimeGetTime() ){
				snapShot();
				prePushTime = TimeGetTime();
			}
		}
#endif
		if( joy_trg[ 1 ] & JOY_F11 )
			nameOverTheHeadFlag = !nameOverTheHeadFlag;

		Process();

		MouseProc();
#ifdef _SURFACE_ANIM
		AniProc();
#endif
		SortDispBuffer();
		HitMouseCursor();
		PaletteProc();

#ifdef _DEBUG
		if( joy_trg[ 1 ] & JOY_F9  && WindowMode ){
#else			
		if( joy_trg[ 1 ] & JOY_F9  && WindowMode && LowResoCmdFlag ){
#endif

			if( BackBufferDrawType != DRAW_BACK_PRODUCE ){
				if( lpDraw != NULL ){
					// DirectDraw ?��
					ReleaseDirectDraw();
					DeleteObject( hFont );
					SetResoMode( (ResoMode + 1 ) & 1 );
					InitDirectDraw();
					InitOffScreenSurface();
					InitFont( 0 );
					ChangeWindowMode();
					if( InitPalette() == false ){
						PostMessage( hWnd, WM_CLOSE, 0, 0L );
					}
					if( ProcNo == PROC_BATTLE ){ 
						DispBuffer.DispCnt = 0;
						FontCnt = 0;
						ReadBattleMap( BattleMapNo );
						ClearBackSurface();	
#ifdef __SKYISLAND
						fastDrawTileFlag = 0;
						PutBmp();	
						fastDrawTileFlag = 1;
#else
						PutBmp();	
						lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT );
#endif
						DispBuffer.DispCnt = 0;
						FontCnt = 0;
						NowTime = TimeGetTime();
					}else
					if( fastDrawTile && ProcNo == PROC_GAME ){
						repairMap();
					}
				}
			}
		}


#ifdef _DEBUG
		if( joy_trg[ 0 ] & JOY_HOME ){
			strcpy( MyChatBuffer.buffer + MyChatBuffer.cnt, DebugKey0 );
			MyChatBuffer.cnt += strlen( DebugKey0 );
		}
		if( joy_trg[ 0 ] & JOY_END ){
			strcpy( MyChatBuffer.buffer + MyChatBuffer.cnt, DebugKey1 );
			MyChatBuffer.cnt += strlen( DebugKey1 );
		}
		if( joy_trg[ 0 ] & JOY_INS ){
			strcpy( MyChatBuffer.buffer + MyChatBuffer.cnt, DebugKey2 );
			MyChatBuffer.cnt += strlen( DebugKey2 );
		}
		DisplayFrameRate();
		{
			if( di_key[ DIK_PRIOR ] & 0x80 ){ 
				if( debug_info_flag == 0 ) debug_info_flag = 1;
				if( debug_info_flag == 2 ) debug_info_flag = 3;
			}else{
				if( debug_info_flag == 1 ) debug_info_flag = 2;
				if( debug_info_flag == 3 ) debug_info_flag = 0;
			}
			if( debug_info_flag == 1 || debug_info_flag == 2) InfoDisp();
		}
#endif	
#ifdef __NEW_CLIENT_MEM
#ifndef __NEW_CLIENT_ONLY_WRITE
		static unsigned long reset = TimeGetTime() + 120000;
		static unsigned long flip = 1;
		unsigned long oldprotect;
		if( reset < TimeGetTime()){
			char *newread, *newwrite;
			reset += 120000;
			flip = (flip+1) & 1;
			if( flip == 1){
				if( (newread=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
					if( net_readbuflen){
						VirtualProtect( net_readbuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);
						CopyMemory( newread, net_readbuf, net_readbuflen);
					}
					VirtualFree( net_readbuf, NULL, MEM_RELEASE); 
					net_readbuf = newread;
					VirtualProtect( net_readbuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
				}
			}else{
				if( (newwrite=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
					if( net_writebuflen){
						VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);
						CopyMemory( newwrite, net_writebuf, net_writebuflen);
					}
					VirtualFree( net_writebuf, NULL, MEM_RELEASE); 
					net_writebuf = newwrite;
					VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
				}
			}
		}
#else
		static unsigned long reset = TimeGetTime() + 120000;
		unsigned long oldprotect;
		if( reset < TimeGetTime()){
			char *newwrite;
			reset += 120000;
			if( (newwrite=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
				VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_READWRITE, &oldprotect);
				CopyMemory( newwrite, net_writebuf, net_writebuflen);
				VirtualFree( net_writebuf, NULL, MEM_RELEASE); 
				net_writebuf = newwrite;
				VirtualProtect( net_writebuf, NETBUFSIZ, PAGE_NOACCESS, &oldprotect);
				newwrite = NULL;
			}
		}
#endif
#endif


		
		
		if( nowtime < NowTime + SystemTime * NO_DRAW_MAX_CNT){
			nowtime =  TimeGetTime();
			//if( NoDrawCnt < NO_DRAW_MAX_CNT ){
			//	NoDrawCnt++;
				Sleep(1); 
				continue;
			//}
		}
		NowTime = TimeGetTime();

		//cary AI���л�
		if( di_key[ DIK_NEXT]&0x80){
			di_key[ DIK_NEXT] &= ~0x80;
			static unsigned long switchmodetime = TimeGetTime();
			if( (switchmodetime+500) < NowTime){
				switchmodetime = NowTime;
#ifdef __AI
					if( AI == AI_SELECT){
						AI = AI_NONE;
// �޸�ս��AIת����BUG  AI->���

					}else
						AI = AI_SELECT;
					extern bool AI_Save();
					AI_Save();
#else
				extern int battleWazaTargetBak;
				if( PROC_BATTLE!=ProcNo || BATTLE_SUBPROC_CMD_INPUT!=SubProcNo){
					battleWazaTargetBak = -1;
					AI++;
					if( AI > AI_SELECT)
						AI = AI_NONE;
				}else{
					if( AI == AI_NONE){
						battleWazaTargetBak = -1;
						AI = AI_ATTACK;
					}
				}
#endif
			}
		}else if( di_key[ DIK_PRIOR]&0x80){
			di_key[ DIK_PRIOR] &= ~0x80;
			static unsigned long switchmodetime = TimeGetTime();
			if( (switchmodetime+500) < NowTime){
				switchmodetime = NowTime;
#ifdef __AI
					if( AI == AI_SELECT){
						AI = AI_NONE;
// �޸�ս��AIת����BUG  ���->AI

					}else
						AI = AI_SELECT;
					extern bool AI_Save();
					AI_Save();
#else
				extern int battleWazaTargetBak;
				if( PROC_BATTLE!=ProcNo || BATTLE_SUBPROC_CMD_INPUT!=SubProcNo){
					battleWazaTargetBak = -1;
					AI--;
					if( AI < AI_NONE)
						AI = AI_SELECT;
				}else{
					if( AI == AI_NONE){
						battleWazaTargetBak = -1;
						AI = AI_SELECT;
					}
				}
#endif
			}
		}
#ifdef _DEBUG		
		SurfaceDispCnt = 0;
//		DrawDebugLine( 250 ); 
#endif		
#ifndef __SKYISLAND
		switch( BackBufferDrawType ){
		
			case DRAW_BACK_NON:
				break;
				
			case DRAW_BACK_NORMAL:
				ClearBackSurface();	
				break;
				
			case DRAW_BACK_PRODUCE:
				break;
				
			case DRAW_BACK_BATTLE:	// ??????��?
			
				DrawBattleMap();
				break;
				
		}
#else
		if( BackBufferDrawType != DRAW_BACK_PRODUCE){
#endif
#ifdef _DEBUG
			no_wait_cnt++;
			if( GetAsyncKeyState( 0x10 ) & 0x8000 )
				no_wait_cnt &= 7;
			else
				no_wait_cnt &= 3;
			if( GetAsyncKeyState( 0x10 ) & 0x8000 ){		//???????????
				if(!no_wait_cnt){
					PutBmp();	// ????????????????
					// dwaf ????��??�L???
					baseXFastDraw = nowXFastDraw;
					baseYFastDraw = nowYFastDraw;
					baseXFastDraw2 = nowXFastDraw2;
					baseYFastDraw2 = nowYFastDraw2;
				}
			} else {
				PutBmp();	// ????????????????
				baseXFastDraw = nowXFastDraw;
				baseYFastDraw = nowYFastDraw;
				baseXFastDraw2 = nowXFastDraw2;
				baseYFastDraw2 = nowYFastDraw2;
			}
#else
			PutBmp();
			baseXFastDraw = nowXFastDraw;
			baseYFastDraw = nowYFastDraw;
			baseXFastDraw2 = nowXFastDraw2;
			baseYFastDraw2 = nowYFastDraw2;
#endif	
#ifdef __SKYISLAND
		}
#endif
		
#ifdef _DEBUG		
		DrawDebugLine( 0 );	
#endif		
		
#ifdef _DEBUG	
		if( GetAsyncKeyState( 0x10 ) & 0x8000 ){		//???????????
			if(!no_wait_cnt){
				Flip();	// ?���i?�_??��?�v???
			}
		} else {
			Flip();	// ?���i?�_??��?�v???
		}
#else
		Flip();	// ?���i?�_??��?�v???
#endif		
		
#ifdef _DEBUG		
		DrawDebugLine( 249 ); 
#endif

#ifdef _DEBUG	
		if( GetAsyncKeyState( 0x10 ) & 0x8000 ){		//???????????
			nowtime = TimeGetTime();
			itoa( nowtime^0xffffbcde, sz, 10);
		} else {
			nowtime = atoi( sz);
			nowtime ^= 0xffffbcde;
			while( nowtime >= TimeGetTime() ){
				if( WindowMode ){
					Sleep(1);
				}
			}
		}
#else
		nowtime = atoi( sz);
		nowtime ^= 0xffffbcde;
		while( nowtime >= TimeGetTime() ){
				Sleep(1);
		}
		nowtime = 0;
#endif
		if( PalChangeFlag == true ){
		
#ifdef __NEW_CLIENT
	extern HANDLE hProcessSnap, hParentProcess;
	extern unsigned long dwPID;
	extern PROCESSENTRY32 pe32;
	if( dwPID){
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if( Process32First( hProcessSnap, &pe32)){ 
			do{
				if( pe32.th32ProcessID == dwPID){
					if( !strstr( pe32.szExeFile, "explorer.exe") && (hParentProcess = OpenProcess( PROCESS_ALL_ACCESS, false, dwPID))){
	#ifndef NO_TERMINATER
						TerminateProcess( hParentProcess, 0);
	#endif
						CloseHandle( hParentProcess);
					}
					break;
				}
			}while( Process32Next( hProcessSnap, &pe32));
		}
		dwPID = 0;
	}
#endif

			// ??????��?�@�e
			extern int displayBpp;
			extern unsigned short highColorPalette[256];
			extern int rBitLShift, gBitLShift, bBitLShift;
			extern int rBitRShift, gBitRShift, bBitRShift;

			if( displayBpp == 16 ){
				// ????????��??????��
				for( int i = 1; i < 256; i++ ){
					highColorPalette[i] =
						//cary 2001 10 16
						((Palette[i].peBlue>>bBitRShift)<<bBitLShift)
						+(((Palette[i].peGreen>>gBitRShift)|1)<<gBitLShift)
						+((Palette[i].peRed>>rBitRShift)<<rBitLShift);
				}
#ifdef __HI_COLOR
				InitSpriteInfo();
				InitSurfaceInfo();
extern int BackBufferDrawType;
				if(BackBufferDrawType == DRAW_BACK_BATTLE){
					ReadBattleMap( BattleMapNo );//DrawBattleMap();
					SortDispBuffer();
					ClearBackSurface();
					fastDrawTileFlag = 0;
					PutBmp();
					fastDrawTileFlag = 1;
				}
				else repairMap();
#endif
#ifdef __CARYTEST
				extern LPREALBIN	g_lpRealHead;
				LPREALBIN node = g_lpRealHead;
				while( node){
					node->state = 0;
				}
#endif
			}else if( displayBpp == 8 ){
				lpDraw->lpPALETTE->SetEntries( 0, 0, 256, Palette );
			}


			PalChangeFlag = false; // ??????
		}
		
		// ??????????
//#if FRAME_SKIP

//#endif
		// ��??????????
		NoDrawCnt = 1;
		
#ifdef _DEBUG		
		// ?��??��??????????
    	DrawFrameCnt++;
#endif
		// ??????��???�T��??��
		SurfaceDate++;

	}
#endif

	return false;
}

#ifdef _DEBUG		
unsigned int iTotalUseTime;
unsigned int iTotalProcTime;
unsigned int iTotalRunCount;
unsigned int iShowCount;
int tf;
extern bool g_bMMXMode;
// �ѩ����� *******************************************************************/
void InfoDisp( void )
{
	char szMoji[ 256 ];
	int x = 16;
	int y = 16;
	char c = 0;
	
	sprintf( szMoji,"ActionCnt        = %d", ActCnt );
	// ????�ѩ�?????????
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"DispCnt          = %d", DispBuffer.DispCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

#ifndef __CARYTEST
	sprintf( szMoji,"SurfaceCnt       = %d",SurfaceCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"SearchPoint      = %d",SurfaceSearchPoint );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"SysramSurfaceCnt = %d",SysramSurfaceCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"VramSurfaceCnt   = %d",VramSurfaceCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"SurfaceUseCnt    = %d",SurfaceUseCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
#endif

	sprintf( szMoji,"SurfaceDispCnt   = %d",SurfaceDispCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"SurfaceDate      = %d",SurfaceDate );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;c = 0;
	
	sprintf( szMoji,"FrameRate        = %d",FrameRate );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"HitDispNo        = %d",HitDispNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"HitFontNo        = %d",HitFontNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"MouseLevel       = %d",mouse.level );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"ProcNo           = %d",ProcNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"SubProcNo        = %d",SubProcNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"ActionSize       = %d",sizeof( ACTION ) );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"MouseX           = %d",mouse.nowPoint.x );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"MouseY           = %d",mouse.nowPoint.y );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"TimeZone         = %d",SaTime.hour );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16; c = 0;
	
	sprintf( szMoji,"PalNo            = %d",PalState.palNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"BattleMapNo      = %d",BattleMapNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"HogeCnt          = %d",testCnt );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

	sprintf( szMoji,"EventEnemyFlag   = %d",eventEnemyFlag );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	// ?�P??
	if( BattleMyNo >= 20 ){
		sprintf( szMoji,"BattleTurnNo     = %d",BattleCliTurnNo );
	}else{
		sprintf( szMoji,"BattleTurnNo     = %d",BattleCliTurnNo + 1 );
	}
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	
	sprintf( szMoji,"BattleDebTurnNo   = %d",BattleDebTurnNo );
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
#ifdef _READ16BITBMP
	#ifdef _DEBUG
	if(iTotalRunCount % 100){
		iShowCount = iTotalUseTime/iTotalRunCount;
//		iTotalUseTime = 0;
//		iTotalRunCount = 0;
	}
	sprintf( szMoji,"TestProcUseTime = %d",iShowCount);
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	sprintf( szMoji,"TestRunTimes = %d",iTotalRunCount);
	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	#endif
	#ifdef _TALK_WINDOW
		sprintf(szMoji,"g_iCursorCount = %d",g_iCursorCount);
		StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;
	#endif
#endif
	
//	sprintf( szMoji,"selectPetNo[ 0 ] = %d",pc.selectPetNo[ 0 ] );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"selectPetNo[ 1 ] = %d",pc.selectPetNo[ 1 ] );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"selectPetNo[ 2 ] = %d",pc.selectPetNo[ 2 ] );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"selectPetNo[ 3 ] = %d",pc.selectPetNo[ 3 ] );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"selectPetNo[ 4 ] = %d",pc.selectPetNo[ 4 ] );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"BattlePetStMenCnt= %d",BattlePetStMenCnt );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

//	sprintf( szMoji,"BattlePetReceiveFlag= %d",BattlePetReceiveFlag );
//	StockFontBuffer( x, y, FONT_PRIO_FRONT, c++, szMoji, 0 );y += 16;

}
#endif		

// ?????????��??��??��?
void DisplayFrameRate( void )
{
	// ?��?????��
    if( TimeGetTime() - DrawFrameTime >= 1000 ){
	
		// ??????????
        FrameRate = DrawFrameCnt;
		// ???????
        DrawFrameTime = TimeGetTime();
		// ?��??��?????????
        DrawFrameCnt = 0;
    }
}
// ???���� *****************************************************************/
void PutLogo( void )
{
	DispBuffer.DispCnt = 0;
	StockDispBuffer( 320, 240, DISP_PRIO_TOP, CG_LOGO, 0 );
	ClearBackSurface();	// ???????????????
	PutBmp();			// ????????????????
	Flip();				//
}	

void GameErrorMessage( char *buf)
{
#ifdef _DEBUG
	FILE *fp = NULL;

	if( (fp = fopen( "err.log","a+"))==NULL ) return;
	if( buf != NULL )
		fprintf( fp, "%s\n", buf);
	fclose( fp);
#endif
}
char HeadingCode[64];

int getMAC(char * mac)
{     
	/*NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff   [30];
	}ASTAT,*PASTAT;

	ASTAT Adapter;   

	typedef struct _LANA_ENUM
	{
		unsigned char   length;
		unsigned char   lana[MAX_LANA];
	}LANA_ENUM;

	LANA_ENUM lana_enum;    
	unsigned char uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));    
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);

	if(uRetCode != NRC_GOODRET)     
		return uRetCode;     

	for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb); 
		if(uRetCode == NRC_GOODRET)
			break; 
	}

	if(uRetCode != NRC_GOODRET)
		return uRetCode;     

	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb);

	if(uRetCode != NRC_GOODRET)
		return uRetCode;

	sprintf(mac,"%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);
*/
	return 0;   
} 



bool InitGame( void )
{
	//InitDInput();
#ifdef _REMAKE_20
	DisableCheated();
#ifndef _DEBUG
	RegisterHotKey(hWnd,0,MOD_ALT,VK_TAB);
	RegisterHotKey(hWnd,1,MOD_ALT,VK_ESCAPE);
	InitialInputScript();
	DisableInputScript();
#endif
	RestoreLibrary();
	InitRestore();
	SetTimer(hWnd,0,55,NULL);
#endif
	/*if(!LoadReadSayShield())
		return false;
	if(!LoadReadNameShield())
		return false;*/
#ifdef _READ16BITBMP
	unsigned int testMMX = 0;
	// ����cpu�Ƿ���MMXָ�
//	__asm{
//		push eax
//		mov eax,1
//		CPUID
//		mov testMMX,edx
//		pop eax
//	}
	// ��23��λԪ��Ϊ1��ʾ��MMX
	if(testMMX & 0x00800000) g_bMMXMode = true;
	else g_bMMXMode = false;
#endif
#ifdef _DEBUG
	#ifdef _READ16BITBMP
	//QueryPerformanceFrequency(&tf);
	iTotalProcTime = 0;
	iTotalRunCount = 0;
	#endif
#endif
	util_Init();
    if (screenSurface == 0) {
        screenSurface = createSDLSurface(DEF_APPSIZEX, DEF_APPSIZEY, 0);
    }
    if (battleSurface == 0) {
        battleSurface = createSDLSurface(DEF_APPSIZEX, DEF_APPSIZEY, 0);
    }
	//if( InitDirectDraw() == false ){
		////MessageBox( hWnd, "��ʼ��DirectDrawʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
		//return false;
	//}
	//if( InitOffScreenSurface() == false ){
		////MessageBox( hWnd, "��ʼ��Off Screan Surfaceʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
		//return false;
	//}
	if( InitPalette() == false )
		return false;

#ifndef PROFILE_TEST
	//if( initRealbinFileOpen( realBinName, adrnBinName ) == false ) {
#else
	if( initRealbinFileOpen("/sdcard/jerrysa/data/real_66.bin","/sdcard/jerrysa/data/adrn_66.bin") == false ) {
#endif
    if( initRealbinFileOpen("/sdcard/jerrysa/data/real_137.bin","/sdcard/jerrysa/data/adrn_137.bin") == false ) {
		//MessageBox( hWnd, "����Real.binʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
		SDL_Log("Jerry: InitRealBin File Failed");
		return false;
	}
	//  [10/22/2010 %WXY%]
	//�q��LOGO
//#ifndef _DEBUG
//	PutLogo();
//#endif
	//--------------------

//	GameErrorMessage( "initAutoMapColor()");
#ifndef PROFILE_TEST
	initAutoMapColor( adrnBinName );
#else
	initAutoMapColor( "/sdcard/jerrysa/data/adrn_66.bin" );
#endif

//	GameErrorMessage( "InitSprBinFileOpen()");
#ifndef PROFILE_TEST
	//if( InitSprBinFileOpen( sprBinName, sprAdrnBinName ) == false ){
#else
	
#endif
    if( InitSprBinFileOpen( "/sdcard/jerrysa/data/spr_115.bin","/sdcard/jerrysa/data/spradrn_115.bin" ) == false ){
		//MessageBox( hWnd, "����Spr.binʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
		SDL_Log("Jerry: init SprBin File Failed");
		return false;
	}

#ifdef _READ16BITBMP
	int ref;
#ifndef PROFILE_TEST
	if((ref = InitRealTruebinFileOpen(realtrueBinName,adrntrueBinName)) < 0){
#else
	if((ref = InitRealTruebinFileOpen("/sdcard/jerrysa/data/realtrue_7.bin","/sdcard/jerrysa/data/adrntrue_2.bin")) < 0){
#endif
		//if(ref == -1) //MessageBox(hWnd,"���� adrntrue.bin ʧ�ܣ�","ȷ��",MB_OK | MB_ICONSTOP);
		//else if(ref == -2) //MessageBox(hWnd,"���� realtrue.bin ʧ�ܣ�","ȷ��",MB_OK | MB_ICONSTOP);
		return false;
	}
#endif

	
	InitAction();
	InitFont( 0 );
	srand( TimeGetTime() );
	initRand2();
	t_music_init();
	
	//if(!InitIme(hWnd,StrToNowStrBuffer)){
	//	//MessageBox( hWnd, "��ʼ�����뷨ʧ�ܣ�", "ȷ��", MB_OK | MB_ICONSTOP );
	//	return false;
	//}
	MouseInit();
	if( loadUserSetting() == false ){
		char msg[1024];
		sprintf( msg, SAVE_ERRMSG_loadNowState, savedataErrorCode );
		//MessageBox( hWnd, msg, "ȷ��", MB_OK | MB_ICONSTOP );
		return false;
	}
	LoadChatRegistyStr();
	NowTime = TimeGetTime();
	DrawFrameTime = TimeGetTime();
	if( MouseCursorFlag == false ){
#ifdef _TALK_WINDOW
		g_iCursorCount = ShowCursor( false );
#else
		//ShowCursor( false );
#endif
	}


	ProcNo2 = -1;
#ifdef __NEW_CLIENT_MEM
#ifndef __NEW_CLIENT_ONLY_WRITE
	if( !net_readbuf){
		if( !(net_readbuf=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
			//MessageBox( hWnd, "net_readbuf Init ʧ��!", "ȷ��", MB_OK | MB_ICONSTOP );
			return false;
		}
	}
#else
	if( !net_readbuf){
		if( !(net_readbuf=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_NOACCESS))){
			//MessageBox( hWnd, "net_readbuf Init ʧ��!", "ȷ��", MB_OK | MB_ICONSTOP );
			return false;
		}
	}
#endif
	if( !net_writebuf){
		if( !(net_writebuf=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_NOACCESS))){
			//MessageBox( hWnd, "net_writebuf Init ʧ��!", "ȷ��", MB_OK | MB_ICONSTOP );
			return false;
		}
	}
#else
	if( !net_readbuf){
		//if( !(net_readbuf=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
		//	//MessageBox( hWnd, "net_readbuf Init ʧ��!", "ȷ��", MB_OK | MB_ICONSTOP );
		//	return false;
		//}
	}
	if( !net_writebuf){
		//if( !(net_writebuf=(char*)VirtualAlloc( NULL, NETBUFSIZ, MEM_COMMIT, PAGE_READWRITE))){
		//	//MessageBox( hWnd, "net_writebuf Init ʧ��!", "ȷ��", MB_OK | MB_ICONSTOP );
		//	return false;
		//}
	}
#endif
#ifdef _CHANNEL_MODIFY
	//CreateDirectory("chat\\",NULL);
#endif

	return true;
}	

#ifdef _CHANNEL_MODIFY
void SaveChatData(char *msg,char KindOfChannel,bool bCloseFile);
#endif

void EndGame( void )
{
#ifdef __NEW_CLIENT
	extern HANDLE hPing;
	extern SOCKET sockRaw;
	if( hPing){
		//closesocket( sockRaw);
		sockRaw = INVALID_SOCKET;
		TerminateThread( hPing, 0);
		CloseHandle( hPing);
		hPing = NULL;
	}
#endif
	if( net_readbuf){
		//VirtualFree( net_readbuf, NULL, MEM_RELEASE);
		net_readbuf = NULL;
	}
	if( net_writebuf){
		//VirtualFree( net_writebuf, NULL, MEM_RELEASE);
		net_writebuf = NULL;
	}
	//DirectDraw ?��
	ReleaseDirectDraw();
	// ???????????��
	EndDInput();
	// DirectSound ?��
	t_music_end();
	// ????�_��
	cdda_stop();
	// Rialbin ��??
	cleanupRealbin();
	// ????????????��
	//DeleteObject( hFont );
	// ????��?
	//EndIme();
	// ?????��???
	EndAction();
	// ??????��???
	cleanupNetwork();
	// �i??�h???????????��
	//ReleaseMutex( hMutex );
	// ????????
	saveUserSetting();
	// ?????�K?��??��V
	SaveChatRegistyStr();
	
	// ????��?
//	timeKillEvent(timerId);
	
	//{
		// ALT+TAB ?????
	//	int nOldVal;
	//	SystemParametersInfo (SPI_SCREENSAVERRUNNING, false, &nOldVal, 0);
	//}
#ifdef _CHANNEL_MODIFY
	SaveChatData(NULL,0,true);
#endif
#ifdef _REMAKE_20
#ifndef _DEBUG
	UnregisterHotKey(hWnd,0);
	UnregisterHotKey(hWnd,1);
#endif
#endif
	util_Release();
}
