//
// ?????????k????
//   ??????????????????????????
//
#include<string.h>
#include <math.h>
#include"../systeminc/version.h"
#include"../systeminc/system.h"
#include"../systeminc/process.h"
#include"../systeminc/netproc.h"
#include"../systeminc/netmain.h"
#include"../systeminc/pc.h"
#include"../systeminc/tool.h"
#include"../systeminc/character.h"
#include"../systeminc/loadsprbin.h"
#include"../systeminc/savedata.h"
#include"../systeminc/chat.h"
//#include "../caryIme.h"
//#include "../systeminc/ime_sa.h"
#include"../systeminc/menu.h"
#include"../systeminc/t_music.h"
#include"../systeminc/netmain.h"
#include"../systeminc/map.h"
#include"../systeminc/lssproto_cli.h"
#include"../systeminc/field.h"
#include"../systeminc/anim_tbl.h"
#include"../systeminc/login.h"
#include"../systeminc/produce.h"
#include"../wgs/message.h"
#include"../wgs/descrypt.h"

#ifdef _SAHOOK //Syu ADD Hook???
#include "..\Sa_Hk.h"
#define UM_KEYEVENT (WM_APP + 0)
#endif

#define SA_VERSION ""
#define SA_VERSION_NUM	300

int nGroup=0;
int nServerGroup=0;

bool logOutFlag = false;
unsigned int MsgCooltime = 0;

short createCharFlag = 0;
char szUser[32];
char szPassword[32];
///add by zhu 2011-03-03

void initInputIdPassword( void );
int inputIdPassword( bool );
void initCommonMsgWin( void );
int commonMsgWin( char * );
void initCertifyIdPassword( void );
void initSelectServer( void );
int selectGroup();
int selectServer();
void initConnecGameServer( void );
int connecGameServer( void );
void initDownloadCharList( void );
int downloadCharList( void );
void initDeleteCharacter( void );
int deleteCharacter( void );
int commonYesNoWindow( int, int );
// add code by shan
int fmYesNoWindow( int x, int y, int id);
int fmselectdetuyWindow( int x, int y, int id);
void initStrBuffer( STR_BUFFER *, int, int, int, int, int );
void getStrSplit( char *, char *, int, int, int );
int DelCharGraColorWin( void );

#ifdef _PET_TRACE_MOUSE
SCPlayPet PlayPet;
#endif
#ifdef _75_LOGIN
static ACTION *aEffect,*aEffect1;
#define RAND(x,y)   ((x-1)+1+ (int)( (double)(y-(x-1))*rand()/(RAND_MAX+1.0)) )
#endif
// Terry add 2001/12/07
#ifdef __EDEN_EFFECT
void BankProc(void);
#endif
// Terry end
// Terry add 2002/01/03
#ifdef __EDEN_AUCTION
void AuctionNewWT(void);
void AuctionListWT(void);
void AuctionSurveyModifyWT(void);
void AuctionModifyWT(void);
// server????????????
typedef struct _AUCTION_LIST_DATA
{
	int  index;												// ??е?????????????
	char cdkey[CHAR_NAME_LEN+1];			// ????cdkey
	char account[101];								// ???????
	char name[CHAR_NAME_LEN+1];				// ??????????
	char money[8];										// ???
	int  kind;												// ???????????
	char sellname[ITEM_NAME_LEN+1];		// ????????????????
	int  grp_no;											// ???????????????
	int  lv,act,def,dex,hp,loyal;			// ?????????
	int  name_color;									// ????????????
	char item_effect_string[64];			// ????????
}ALD,*pALD;

char *szpALD = NULL;  // server??????????????
bool bNewData = false;        // ???????????????
void AuctionGetString(int nWhichOne,pALD ald); //????????????
void CheckNumber(char *buf,int num);	// ??????????????
void CheckSpace(char *buf);		// ???????????
#endif

#ifdef _SAHOOK //Syu ADD Hook???
bool hookflag = false;
#endif

static int selCharGraColorWinProcNo;

short skillShopWindow4ProcNo;
short skillShopWindow1ProcNo;


STR_BUFFER idKey;
STR_BUFFER passwd;
static STR_BUFFER *idPasswordFocus[] =
	{ &idKey, &passwd };
STR_BUFFER *idPasswordStr = &idKey;
short idKeyReturn = 0;
static int idPasswordGraId[] = { -2, -2 };
static int idPasswordFocusSw = 0;
static short idKeyBoxX, idKeyBoxY;
static short passwdBoxX, passwdBoxY;
static bool bAgain=false;
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
static ACTION *pActPet20;
int BornPetNum = 0;
#endif
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
ACTION *PkServerSel;
short PkMenuflag = 0 ; 
int VersionXYZ = 1 ; //Pk Server?汾??
#endif
// Nuke 0615: Avoid 7's lock
extern int isWGS7;
#ifdef _LOGINKICK			
unsigned long StartTime = -1 ; 
#endif
#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif

void idPasswordProc( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;

	bool flag = false;
	int ret;
	static char msg[256];
	static char szWGSState[]="登入游戏中，请稍后！";

	if( SubProcNo == 0 ){
		SDL_StartTextInput();
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
#ifndef _DEBUG
		SDL_Delay(0);
#endif
		PaletteChange( DEF_PAL, 0 );
		SubProcNo++;
		initInputIdPassword();
		idKeyReturn = 0;
#ifdef _PET_TRACE_MOUSE
		PlayPet.SetActionStep(0);
#endif
	}
	if( SubProcNo == 1 ){
		if(!bAgain)
			bAgain=true;
		else{
//??Hook???
#ifdef _SAHOOK //Syu ADD Hook???
			if( hookflag == false)
			{
				hookflag = true;
				KeyboardHook_Start(hWnd, UM_KEYEVENT);
			}
//????????
			memcpy(idKey.buffer,szUser,32);
			ecb_crypt("f;encor1c",idKey.buffer,32,DES_DECRYPT);
			memcpy(passwd.buffer,szPassword,32);
			ecb_crypt("f;encor1c",passwd.buffer,32,DES_DECRYPT);
			idKey.cursor=idKey.cnt=strlen(idKey.buffer);
			passwd.cursor=passwd.cnt=strlen(passwd.buffer);
#else
			memcpy(idKey.buffer,szUser,32);
			ecb_crypt("f;encor1c",idKey.buffer,32,DES_DECRYPT);
			memcpy(passwd.buffer,szPassword,32);
			ecb_crypt("f;encor1c",passwd.buffer,32,DES_DECRYPT);
			idKey.cursor=idKey.cnt=strlen(idKey.buffer);
			passwd.cursor=passwd.cnt=strlen(passwd.buffer);
#endif
		}
		idPasswordFocusSw = 0;
		GetKeyInputFocus( idPasswordFocus[idPasswordFocusSw] );
		SubProcNo++;
	}

	// ????????
	if( SubProcNo == 2 )
		flag = true;	// ????????

	ret = inputIdPassword( flag );

	if( ret == 1 ){
		SubProcNo=3;
		play_se( 217, 320, 240 );	// ?????
	}
	else if( ret == 2 ){
	//????????
		play_se( 217, 320, 240 );	// ?????
		// ????????? WM_CLOSE ??????????
		//PostMessage( hWnd, WM_CLOSE, 0, 0L );
	}else if( ret < 0 ){
	// ID,PASSWORD?????????????
		SubProcNo = 100;
		strcpy( msg, "请输入您的账号与密码" );
	}
	if(SubProcNo==3){
#ifdef _DEBUG
		w = (strlen( szWGSState )+23)*9/64+2;
#else
		w = (strlen( szWGSState )+3)*9/64+2;
#endif
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640 -w*64)/2;
		y = (480 -h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );

#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		initConnectServer();
		SubProcNo=4;
#ifdef _PET_TRACE_MOUSE
		PlayPet.SetActionStep(2);
#endif
#ifdef _75_LOGIN
		if(g_bUseAlpha){
			DeathAction(aEffect);
			DeathAction(aEffect1);
		}
#endif
	}
	if(SubProcNo==4){
		if( ptActMenuWin != NULL ){
			if( ptActMenuWin->hp >= 1 ){
				int len;
				int xx, yy;
				if (iWGS==7) isWGS7=1; // Nuke 0615: Avoid 7's lock

#ifdef _DEBUG
				switch(iWGS){
				case 0:
					sprintf(msg,"%s",szWGSState);
					break;
				case 1: //????WGS
					sprintf(msg,"%s(%s)",szWGSState,"Connect to WGS!!");
					break;
				case 2: //???CSIP
					sprintf(msg,"%s(%s)",szWGSState,"Get the Redirect!!");
					break;
				case 3: //????CS
					sprintf(msg,"%s(%s)",szWGSState,"Connect to CS!!");
					break;
				case 4: //???Public key(????)
					sprintf(msg,"%s(%s)",szWGSState,"Get the Public key!!");
					break;
				case 5: //???Session key(????)
					sprintf(msg,"%s(%s)",szWGSState,"Send the Session key!!");
					break;
				case 6: //???????GS??
					sprintf(msg,"%s(%s)",szWGSState,"Get Continue!!");
					break;
				case 7: //????????????
					sprintf(msg,"%s(%s)",szWGSState,"Send UserName!!");
					break;
				case 8:
					sprintf(msg,"%s(%s)",szWGSState,"Get Goto!!");
					break;
				}
#else
				sprintf(msg,"%s(%d)",szWGSState,iWGS);
#endif
				len = strlen( msg );
				xx = (w*64-len*8)/2;
				yy = (h*48-16)/2;
				StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
			}
		}
#ifdef NO_WGS
	int ret2 = 1;
	dwServer = GS;
#else
	int ret2 = ConnectWGS();
#endif
		if( ret2 == 1 ){
		// ????Title
			memcpy(szUser,idKey.buffer,32);
			ecb_crypt("f;encor1c",szUser,32,DES_ENCRYPT);
			memcpy(szPassword,passwd.buffer,32);
			ecb_crypt("f;encor1c",szPassword,32,DES_ENCRYPT);
			idKey.cnt = 0;
			idKey.cursor=0;
			memset(idKey.buffer,0, sizeof(idKey.buffer));
			passwd.cnt = 0;
			passwd.cursor=0;
			memset(passwd.buffer,0, sizeof(passwd.buffer));
			ChangeProc( PROC_TITLE_MENU );
			if( ptActMenuWin ){
				DeathAction( ptActMenuWin );
				ptActMenuWin = NULL;
			}
		}else if( ret2 == -7 ){
			SubProcNo = 100;
			strcpy( msg, szError );
		}else if( ret2 < 0 ){
			SubProcNo = 100;
			strcpy( msg, netprocErrmsg );
		}else if( ret2 == -8 ){
			SubProcNo = 100;
			strcpy( msg, szError );
		}
	}
	if(SubProcNo == 5)
	{
		SubProcNo = 100;
		strcpy( msg, "输入的账号密码错误\n请再次输入!" );
	}
	if( SubProcNo == 100 ){
		cleanupNetwork();
		if( ptActMenuWin ){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 ){
		if( commonMsgWin( msg ) ){
			// ???????????
//?????????Title??????????Hook
#ifdef _SAHOOK //Syu ADD Hook???
			if( hookflag == false)
			{
				hookflag = true;
				KeyboardHook_Start(hWnd, UM_KEYEVENT);
			}
#endif
			SubProcNo = 2;
			//Sleep(1000);
		}
	}
}

// ?????????V????????
void initInputIdPassword( void )
{
	int i;

	for( i = 0; i < sizeof( idPasswordGraId )/sizeof( int ); i++ )
	{
		idPasswordGraId[i] = -2;
	}

#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
#ifdef _BACK_LOGO
  idKeyBoxX = 320;
	idKeyBoxY = 185;
#else
	#ifdef _75_LOGIN
		idKeyBoxX = 458;
		idKeyBoxY = 99;
	#else
		idKeyBoxX = 478;
		idKeyBoxY = 155;
	#endif //_75_LOGIN
#endif
#endif
#ifdef _80_LOGIN
		idKeyBoxX = 376;
		idKeyBoxY = 94;
#endif
#ifdef _v4_0
	idKeyBoxX = 95;
	idKeyBoxY = 229;
#endif

	idKeyBoxX = 376;
	idKeyBoxY = 94;


	initStrBuffer( &idKey, idKeyBoxX, idKeyBoxY, 12, FONT_PAL_WHITE, FONT_PRIO_BACK );

	idKey.cnt = 0;
	idKey.cursor=0;

#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
#ifdef _BACK_LOGO
	passwdBoxX = 320;
	passwdBoxY = 216;
#else
	#ifdef _75_LOGIN
		passwdBoxX = 458;
		passwdBoxY = 139;
	#else
		passwdBoxX = 478;
		passwdBoxY = 191;
	#endif //_75_LOGIN
#endif
#endif
#ifdef _80_LOGIN
		passwdBoxX = 376;
		passwdBoxY = 132;
#endif
#ifdef _v4_0
	passwdBoxX = 95;
	passwdBoxY = 265;
#endif


	passwdBoxX = 376;
	passwdBoxY = 132;


	initStrBuffer( &passwd, passwdBoxX, passwdBoxY, 16, FONT_PAL_WHITE, FONT_PRIO_BACK );

	// ????????????????????@?e
	passwd.cnt = 0;
	passwd.cursor=0;
#ifdef _SAHOOK //Syu ADD Hook???
	passwd.filterFlag = HOOK_TYPE;
#else
	passwd.filterFlag = BLIND_TYPE;
#endif
	// ??????@?e

	idPasswordFocusSw = 0;
	GetKeyInputFocus( idPasswordFocus[idPasswordFocusSw] );
#ifdef _75_LOGIN
	if(g_bUseAlpha){
		aEffect = MakeAnimDisp(300,244,101817,0);
		aEffect1 = MakeAnimDisp(300,244,101818,0);
	}
#endif
}


// ?V???????????
//
//  ???k??	 0 ... ?V??
//				 1 ... ?V???
//				 2 ... ??????
//				-1 ... ?V????
int inputIdPassword( bool flag )
{
	int id;
	int selOkFlag;
	static bool flag2 = false;
	static int oldId = 0;
	int ret = 0;
	int x1, y1, x2, y2, cx, cy;

	// ???????????R?e
	id = selGraId( idPasswordGraId, sizeof( idPasswordGraId )/sizeof( int ) );

	// ???????????
	if( id == 0 )
	{
		// CD??????????@?e??????
		if( strlen( idKey.buffer ) > 0 /*&& strlen( passwd.buffer ) > 0*/ )
		{

//????OK Button???Dll??????????????
#ifdef _SAHOOK //Syu ADD Hook???
			keyboardHook_Send(passwd.buffer);
			passwd.cnt = strlen(passwd.buffer);
			passwd.buffer[passwd.cnt] = '\0';
#else
			idKey.buffer[idKey.cnt] = '\0';
			passwd.buffer[passwd.cnt] = '\0';
#endif
			ret = 1;
		}
		else
		{
			// ?@?e??????
			ret = -1;
		}
	}
	else
	// ???????????
	if( id == 1 )
	{
		ret = 2;
	}


	// ??????k?????????@?e
	if( flag )
	{
		selOkFlag = 2;
	}
	else
	{
		selOkFlag = 0;
	}

	StockFontBuffer2( &idKey );
	StockFontBuffer2( &passwd );
	id = -1;

	if( selOkFlag )
	{
		x1 = idKeyBoxX - 4 -2;
		y1 = idKeyBoxY - 2 -1;
#ifdef _v4_0
		x2 = x1 + 92 +4;
		y2 = y1 + 20 +2;
#endif

#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
#ifdef _BACK_LOGO
		x2 = x1 + 92 +26;
		y2 = y1 + 20 +2;
#else
		x2 = x1 + 92 +4;
		y2 = y1 + 20 +2;
#endif
#endif


		if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX ) )
		{
			id = 0;
		}
		x1 = passwdBoxX - 4 -2;
		y1 = passwdBoxY - 2 -1;
#ifdef _v4_0
		x2 = x1 + 92 +4;
		y2 = y1 + 20 +2;
#endif
#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
#ifdef _BACK_LOGO
		x2 = x1 + 92 +26;
		y2 = y1 + 20 +2;
#else
		x2 = x1 + 92 +4;
		y2 = y1 + 20 +2;
#endif
#endif


		if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX ) )
		{
			id = 1;
		}
	}


	// ?V?????????
	if( idKeyReturn )
	{
		id = 1;
		idKeyReturn = 0;
	}

	if( joy_trg[ 1 ] & JOY_TAB )
	{
		if( oldId == 0 )
		{
			id = 1;
		}
		else
		if( oldId == 1 )
		{
			id = 0;
		}
	}

	if( !flag )
	{
		// ????false???V???????
		GetKeyInputFocus( NULL );
		flag2 = true;
	}
	else
	if( (0 <= id && id <= 1)
	 || flag2 )
	{
		if( flag2 )
			id = oldId;
//Hook???????????????
#ifdef _SAHOOK //Syu ADD Hook???
		if ( (id == 1) && (hookflag == false ))
		{
			hookflag = true;
			KeyboardHook_Start(hWnd, UM_KEYEVENT);

		}
		else if ( (id == 0) && (hookflag == true))
		{
			hookflag = false;
			KeyboardHook_Stop();
			extern int HOOK_PASSWD_NUM;
			HOOK_PASSWD_NUM = 0;
		}
#endif
		GetKeyInputFocus( idPasswordFocus[id] );
		flag2 = false;
		oldId = id;
	}

#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
	#ifdef _NEWPANEL_70
		#ifdef _BACK_LOGO
			cx = 317;
			cy = 288;
		#else
			#ifdef _75_LOGIN
				cx = 320;
				cy = 240;
			#else
				cx = 431;
				cy = 265;
			#endif //_75_LOGIN
		#endif //_BACK_LOGO
	#else
		cx = 432;
		cy = 265;
	#endif
#endif
#ifdef _v4_0
	cx = 49;
	cy = 339;
#endif
#ifndef _75_LOGIN
	x1 = cx - 50;
	y1 = cy - 30;
	x2 = cx + 50;
	y2 = cy + 30;
#else
	x1 = 402;
	y1 = 212;
	x2 = x1 + 70;
	y2 = y1 + 50;
#endif
#ifdef _80_LOGIN
	cx = 320;
	cy = 240;
	x1 = cx;
	y1 = cy - 15;
	x2 = cx + 90;
	y2 = cy + 110;
#endif


	if (MakeHitBox(x1, y1, x2, y2, -1) && selOkFlag)
		idPasswordGraId[0] = StockDispBuffer(cx, cy, DISP_PRIO_BG, CG_TITLE_ID_PASS_OK, 1);
	else
		idPasswordGraId[0] = -2;
#ifdef _v4_0
	cx = 171;
	cy = 348;
#endif

#ifdef _NEWLOGO				   // Syu ADD 6.0????????logo ???_v4_0
	#ifdef _BACK_LOGO
		cx = 361;
		cy = 372;
	#else
		#ifdef _75_LOGIN
			cx = 320;
		#else
			cx = 554;
			cy = 274;
		#endif
	#endif
#endif
#ifndef _75_LOGIN
	x1 = cx - 50;
	y1 = cy - 30;
	x2 = cx + 50;
	y2 = cy + 30;
#else
	x1 = 520;
	x2 = x1 + 70;
#endif
#ifdef _80_LOGIN
	x1 = 412;
	y1 = 232;
	x2 = 494;
	y2 = 372;
#endif

	if (MakeHitBox(x1, y1, x2, y2, -1) && selOkFlag)
		idPasswordGraId[1] = StockDispBuffer(cx - 140, cy + 20, DISP_PRIO_BG, CG_TITLE_ID_PASS_QUIT, 1);
	else
		idPasswordGraId[1] = -2;
#ifdef __SKYISLAND
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_TITLE_ID_PASS, 0 );
#endif
	return ret;
}

static short commonMsgWinProcNo = 0;

void initCommonMsgWin( void )
{
	commonMsgWinProcNo = 0;
}


int commonMsgWin( char *msg )
{
	static int fontId[] = { -2 };
	static ACTION *ptActMenuWin = NULL;
	int id;
	int i;
	static int x, y, w, h;
	int ret = 0;
#ifdef _NEW_WGS_MSG				// WON ADD WGS????????
	char temp[256], out_msg[5][128];
	int j, count=0, temp_size=0;
#endif

	// ???
	if( commonMsgWinProcNo == 0 )
	{
		commonMsgWinProcNo = 1;

		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}

#ifdef _NEW_WGS_MSG				// WON ADD WGS??????????
			
		memset( temp, 0, sizeof(temp) );
		strcpy( temp, msg);		
		
		i=0; j=0;
		while( temp[count] != '\0' ){
			j++;
			if( temp[count] == '\n' ){
				if( j > temp_size )		temp_size = j;
				i++;	j=0;
			}
			count++;
		}

		if( temp_size == 0 ){ 
			temp_size = strlen( temp );
			i = 1;
		}

		h = ((36+32)/48) * (i+2);
		w = temp_size*8/64+2;

#else

		w = strlen(msg)*8/64+2;
		h = (36+32)/48;
#endif

		if( h < 2 )
			h = 2;
		x = (640 -w*64)/2;
		y = (480 -h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );

#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}


	// ??????k???R?e
	id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

	// ???
	if( id == 0 )
	{
		ret = 1;
		play_se( 217, 320, 240 );	// ?????
	}


	// ?????????????????
	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return ret;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx;

#ifdef _NEW_WGS_MSG				// WON ADD WGS????????

			memset( temp, 0, sizeof(temp) );
			strcpy( temp, msg);		
		
			for(i=0; i<5; i++)
				memset( out_msg[i], 0, sizeof(out_msg[i]) );

			i=0; j=0;
			while( temp[count] != '\0' ){
				out_msg[i][j]=temp[count];	j++;
				if( temp[count] == '\n' ){
					out_msg[i][j-1] = '\0';
					i++;	j=0;	
				}
				count++;
			}
		
			for( j=0; j<=i; j++){
				len = strlen( out_msg[j] )+1;
				xx = (w*64-len*8)/2;
				StockFontBuffer( x+xx, y+(j*30)+30, FONT_PRIO_FRONT, FONT_PAL_WHITE, out_msg[j], 0 );
			}

#else

			len = strlen( msg )+1;
			xx = (w*64-len*8)/2;

			StockFontBuffer( x+xx, y+30, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
#endif

			xx = (w*64-strlen( "???" )*8)/2;
#ifdef _SAHOOK //Syu ADD Hook???
			if ( hookflag == true)
			{
				hookflag = false;
				KeyboardHook_Stop();
				extern int HOOK_PASSWD_NUM;
				HOOK_PASSWD_NUM = 0;
			}
#endif

#ifdef _NEW_WGS_MSG				// WON ADD WGS????????
			fontId[0] =	StockFontBuffer( x+xx, y+(j*30)+56, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "???", 2 );
#else
			fontId[0] =	StockFontBuffer( x+xx, y+56, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "???", 2 );
#endif
		}
	}
	return ret;
}








// ????V???????????
//  ??:	STR_BUFFER *pt;	... ??????????B?l?????
//			int x, y;		... ???????
//			int len;		... ?V?????????
//			int color;		... ??????
//			int prio;		... ??????I?T
void initStrBuffer( STR_BUFFER *pt, int x, int y, int len, int color, int prio )
{
	pt->x        = x;
	pt->y        = y;
	pt->len      = len;
	pt->color    = color;
	pt->fontPrio = prio;
}


// ????????k???R?e
//   ?????k?????????????k?????
//  ??:	int *id;	... ?????????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int selGraId( int *id, int cnt )
{
	int i;

	// ???????????????
	if( (mouse.onceState & MOUSE_LEFT_CRICK) == 0 )
		return -1;

	for( i = 0; i < cnt; i++ )
	{
		if( id[i] == HitDispNo )
		{
			return i;
		}
	}

	return -1;
}


// ????????k???R?e?????????
//   ?????k?????????????k?????
//  ??:	int *id;	... ?????????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int selRepGraId( int *id, int cnt )
{
	int i;

	// ???????????????
	if( (mouse.autoState & MOUSE_LEFT_CRICK) == 0 )
		return -1;

	for( i = 0; i < cnt; i++ )
	{
		if( id[i] == HitDispNo )
		{
			return i;
		}
	}

	return -1;
}


// ????????k???R?e
//   ?????k????????????????k?????
//  ??:	int *id;	... ?????????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int pushGraId( int *id, int cnt )
{
	int i;

	// ?????????????
	if( (mouse.state & MOUSE_LEFT_CRICK) == 0 )
		return -1;

	for( i = 0; i < cnt; i++ )
	{
		if( id[i] == HitDispNo )
		{
			return i;
		}
	}

	return -1;
}


// ??????k???R?e
//   ?????k?????????????k?????
//  ??:	int *id;	... ???????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int selFontId( int *id, int cnt )
{
	int i;
	// ???????????????
	if( (mouse.onceState & MOUSE_LEFT_CRICK) == 0 )
		return -1;
	for( i = 0; i < cnt; i++ ){
		if( id[i] == HitFontNo )
			return i;
	}
	return -1;
}


// ?????????????R?e
//   ???????????????k?????
//  ??:	int *id;	... ?????????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int focusGraId( int *id, int cnt )
{
	int i;

	for( i = 0; i < cnt; i++ )
	{
		if( id[i] == HitDispNo )
		{
			return i;
		}
	}

	return -1;
}


// ???????????R?e
//   ???????????????k?????
//  ??:	int *id;	... ???????????????
//			int cnt;	... ????
//  ???k:	-1			... ??????????
int focusFontId( int *id, int cnt )
{
	int i;

	for( i = 0; i < cnt; i++ )
	{
		if( id[i] == HitFontNo )
		{
			return i;
		}
	}

	return -1;
}

// ????_????????????????
// 
static short userCertifyErrorMsgWinProcNo = 0;



extern char szAnnouncement[];
extern struct gamegroup  gmgroup[];

void titleProc( void )
{
	int ret;
	static char msg[256];
	if( SubProcNo == 0 ){
		char str[128];

		sprintf( str, "%s %s", DEF_APPNAME, SA_VERSION );

		//SetWindowText( hWnd, str );
		if(0<strlen(szAnnouncement))
			SubProcNo=200;
		else
			SubProcNo++;
	}
	if( SubProcNo == 1 ){
		if( initNet() ){
			if(nGroup>1)
				SubProcNo=2;
			else
				SubProcNo=3;
			ProduceInitFlag = true;
			initSelectServer();
		//	disconnectServerFlag = false;
		}else{
			SubProcNo=100;
			strcpy( netprocErrmsg, NET_ERRMSG_SOCKLIBERROR );
		}
	}
	// ???Group
	if(SubProcNo==2){
		ret=selectGroup();
		if(ret==1){
			SubProcNo++;
		}else if(ret==2){
			idPasswordFocusSw=0;
			GetKeyInputFocus(idPasswordFocus[idPasswordFocusSw]);
			ChangeProc(PROC_ID_PASSWORD);
			SubProcNo=0;
			cleanupNetwork();
		}else if(ret==3){
			SubProcNo = 100;
			sprintf(msg,"[%s]??????汾?????????????",gmgroup[nServerGroup].name);
			cleanupNetwork();
		}
	}
	// ???Server
	if(SubProcNo==3){
		ret=selectServer();
		if(ret==1){
			SubProcNo++;
		}else if(ret==2){
			if(nGroup<2){
				idPasswordFocusSw=0;
				GetKeyInputFocus(idPasswordFocus[idPasswordFocusSw]);
				ChangeProc(PROC_ID_PASSWORD);
				SubProcNo=0;
			}else
				SubProcNo=2;
		}
	}
	if( SubProcNo == 4 ){
		// ????Game server??????
		initConnecGameServer();
		SubProcNo++;
	}
	if( SubProcNo == 5 ){
		// ????Game server
		ret = connecGameServer();
		if( ret == 1 ){
#ifdef _PK2007
			ChangeProc( PROC_PKSERVER_SELECT );
#else
			ChangeProc( PROC_CHAR_SELECT );
#endif
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
			PkMenuflag = 0 ;
#endif
		}else if( ret == -2 ){
			SubProcNo = 100;
			strcpy( msg, netprocErrmsg );
			cleanupNetwork();
		}
	}
	// ????????
	if( SubProcNo == 100 ){
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 ){
		if( commonMsgWin( msg ) ){
			char str[128];
			SubProcNo = 1;

			sprintf( str, "%s %s", DEF_APPNAME, SA_VERSION );

			//SetWindowText( hWnd, str );
		}
	}
	if( SubProcNo == 200 ){
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 201 ){
		if( commonMsgWin( szAnnouncement ) )
			SubProcNo = 1;
	}
	// ??????????
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_TITLE, 0 );
	// ?????????

	StockFontBuffer( 570, 460, FONT_PRIO_BACK, FONT_PAL_BLUE, SHOW_VERSION, 0 );

}


// ????k???????

ACTION *ptActSelectServerWin = NULL;

// ???
void initSelectServer( void )
{
	ptActSelectServerWin = NULL;
}

/*return:	0 ... ?????
			1 ... ????Group
			2 ... ????????
			3 ... ????????????????	*/
int selectGroup()
{
	static int fontId[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
	static int x,y;
	int row;
	int id;
	int i;
	int ret = 0;
	if( ptActSelectServerWin == NULL ){
		y=270 + DISPLACEMENT_Y;
		x=0 ;
		row=(nGroup>>2)+1;
		if(row==1)
			x=256;
		else if(row==2)
			x=192;
		else if(row==3)
			x=128;
		else if(row==4)
			x=64;
		ptActSelectServerWin = MakeWindowDisp( x, y, row<<1, 3, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActSelectServerWin->x+22;
		y = ptActSelectServerWin->y+28;
#else
		x+=22;
		y+=28;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
	}
	if( ptActSelectServerWin != NULL ){
		// ?α??????????
		id = selFontId( fontId, nGroup+1 );
		if( id >= 0 && id < nGroup ){
			nServerGroup=id;
			char title[256];
			sprintf( title, "%s %s [%s]", DEF_APPNAME, SA_VERSION, gmgroup[nServerGroup].name );
			//SetWindowText( hWnd, title );
			DeathAction( ptActSelectServerWin );
			ptActSelectServerWin = NULL;
			play_se( 217, 320, 240 );	// click??
			if(gmgroup[nServerGroup].used==1)
				return 1;
			else
				return 3;
		}else if(id==nGroup){
				DeathAction( ptActSelectServerWin );
				ptActSelectServerWin = NULL;
				play_se( 217, 320, 240 );	// click??
				return 2;
		}
		if(ptActSelectServerWin->hp >= 1){
			int Hit=0;
			for(i=0;i<nGroup;i++){
				Hit=2;
				fontId[i]=StockFontBuffer(x+(i>>2)*128,y+(i%4)*25,FONT_PRIO_FRONT,FONT_PAL_YELLOW,gmgroup[i].name,Hit);	
			}
			fontId[i]=StockFontBuffer(x+(i>>2)*128,y+75,FONT_PRIO_FRONT,FONT_PAL_YELLOW,"  回上一页  ",2);
		}
	}
	return 0;
}

//
//  ???k??	0 ... ?k???
//				1 ... ?????e
//				2 ... ??????
int selectServer()
{
	static int fontId[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
	static int x,y;
	int row;
	int id;
	int i;
	int ret = 0;
	if( ptActSelectServerWin == NULL ){
		y=270 + DISPLACEMENT_Y ;
		x=0;
		row = (gmgroup[nServerGroup].num>>2)+1;
		if(row==1)
			x=256;
		else if(row==2)
			x=192;
		else if(row==3)
			x=128;
		else if(row==4)
			x=64;
		ptActSelectServerWin = MakeWindowDisp( x, y, row<<1, 3, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActSelectServerWin->x+22;
		y = ptActSelectServerWin->y+28;
#else
		x+=22;
		y+=28;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
	}
	if( ptActSelectServerWin != NULL ){
		// ?α??????????
		id = selFontId( fontId, gmgroup[nServerGroup].num+1 );
		if( id >= 0 && id < gmgroup[nServerGroup].num ){
			char title[256];

			selectServerIndex=gmgroup[nServerGroup].startindex+id;
#ifdef _DEBUG
			sprintf( title, "%s %s [%s  %s:%s]", DEF_APPNAME, SA_VERSION,
				gmsv[selectServerIndex].name,
				gmsv[selectServerIndex].ipaddr, gmsv[selectServerIndex].port );
#else
			sprintf( title, "%s %s [%s]", DEF_APPNAME, SA_VERSION, gmsv[selectServerIndex].name );
#endif
			//SetWindowText( hWnd, title );
			DeathAction( ptActSelectServerWin );
			ptActSelectServerWin = NULL;
			play_se( 217, 320, 240 );	// click??
			return 1;
		}else if(id==gmgroup[nServerGroup].num){
			DeathAction( ptActSelectServerWin );
			ptActSelectServerWin = NULL;
			play_se( 217, 320, 240 );	// click??
			return 2;
		}
		if(ptActSelectServerWin->hp >= 1){
			int j=gmgroup[nServerGroup].startindex;
			int Hit=0;
			for(i=0;i<gmgroup[nServerGroup].num;i++,j++){
				if('1'==gmsv[j].used)
					Hit=2;
				else
					Hit=0;


				fontId[i]=StockFontBuffer(x+(i>>2)*128,y+(i%4)*25,FONT_PRIO_FRONT,FONT_PAL_YELLOW,gmsv[j].name,Hit);

			}
			fontId[i]=StockFontBuffer(x+(i>>2)*128,y+75,FONT_PRIO_FRONT,FONT_PAL_YELLOW,"  回上一页  ",2);
		}
	}
	return 0;
}





// ??????Q?????

static short connecGameServerProcNo = 0;

void initConnecGameServer( void )
{
	connecGameServerProcNo = 0;
}


// ??????Q??
//
//  ???k??	 0 ... ???
//				 1 ... ???Q??
//				-1 ... ??????????????????
//				-2 ... ???????Q??????
int connecGameServer( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	int ret2;
	static char msg[256];

	// ???
	if( connecGameServerProcNo == 0 )
	{
		connecGameServerProcNo = 1;

		sprintf( msg, "%s服务器连线中", gmsv[selectServerIndex].name );

		// ????????
		w = (strlen( msg )*9+63)/64;
		if( w < 2 )
			w = 2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640  -w*64)/2;
		y = (480  -h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ?????????
	if( connecGameServerProcNo == 1 )
	{
		cleanupNetwork();
		if( initNet() )
		{
			dwServer = GS;
			connecGameServerProcNo = 2;
		}
		else
		{
			ret = -1;
		}
	}
	// ??????Q?????
	if( connecGameServerProcNo == 2 )
	{
		initConnectServer();
		connecGameServerProcNo = 3;
	}
	if( connecGameServerProcNo == 3 )
	{
		ret2 = connectServer();
		if( ret2 == 1 )
		{
			ret = 1;
		}
		else
		if( ret2 < 0 )
		{
			ret = -2;
		}
	}

	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;

			len = strlen( msg );
			xx = (w*64-len*8)/2 + 25;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}


///////////////////////////////////////////////////////////////////////////
//
// ????k??????????????
//
//cary ?????????saac???????
void selectCharacterProc( void )
{
	int ret;
	int i, j;
	int login;
	int x;
	static char msg[256];
	static int btnGraId[] = { -2, -2, -2, -2, -2, -2, -2 };
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
	static int PkBtn[1] = { -2 } ; 
	int x3 , y3 ;
	char ServerName[13][32] = {
		{"???"} , 
		{"?????????"} , 
		{"??·?????"} , 
		{"????????"} , 
		{"?????????"} , 
		{"?????????"} , 
		{"??????"} , 
		{"??????????"} , 
		{"???????"} , 
		{"?????????"} , 
		{"??????????"} , 
		{"????????"} , 
		{"?????硡"} };
#endif
	int btnUseFlag = 0;
	int attrColor[4][2] =
	{
		{ SYSTEM_PAL_GREEN,  SYSTEM_PAL_GREEN2  },
		{ SYSTEM_PAL_AQUA,   SYSTEM_PAL_AQUA2   },
		{ SYSTEM_PAL_RED,    SYSTEM_PAL_RED2    },
		{ SYSTEM_PAL_YELLOW, SYSTEM_PAL_YELLOW2 }
	};
	int x1, y1, x2, y2;

	// ??????????x?????
	if( SubProcNo == 0 )
	{
		SubProcNo++;

		initDownloadCharList();
	}
	// ??????????x??
	if( SubProcNo == 1 )
	{
		ret = downloadCharList();
		if( ret == 1 ){
			SubProcNo = 10;
			play_bgm(2);
		}
		else if( ret == -1 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"无法与服务器取得联系");
		}
		else if( ret == -2 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"网路忙线中，请稍后再试！");
		}
		else if( ret == -3 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"账号或密码输入错误");
		}
		else if( ret == -4 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"?????????????????????????????????????????????????");
		}
		else if( ret == -5 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"?????????????????????????????????????????????????????");
		}
		else if( ret == -6 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"??????????????????????????????????");
		}
		else if( ret == -7 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"??????δ???????????????????????");
		}
		else if( ret == -8 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"??????????????????");
		}
		else if( ret == -9 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"??????????У???????????");
		}
		else if( ret == -10 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"???????"/*"??????У???????????"*/);
		}
		else if( ret == -11 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"????????????????С?");
		}
#ifdef _CHANGEGALAXY
		else if( ret == -12 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"????????????????????????");
		}
#endif
#ifdef _ERROR301
		else if( ret == -13 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"??????δ???????δ??л??????????");
		}
#endif
		else if( ret == -101 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"101 ???????????????????????");
		}
		else if( ret == -102 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"102 ?????????????????????");
		}
		else if( ret == -103 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"103 ?????????????У??????????????");
		}
		else if( ret == -201 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"201 ?????????????????????????δ????????????????????????????????");
		}
		else if( ret == -202 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"202 ?????????????????");
		}
		else if( ret == -203 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"203 ???????????????????????ε????????????????????????????10??????????");
		}
		else if( ret == -204 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"204 ?????????????????");
		}
		else if( ret == -205 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"205 δ????????????????");
		}
		else if( ret == -301 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"301 ??????δ???????????????????????");
		}
		else if( ret == -302 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"302 ???????????δ??????????????????????????????????????????????п?????");
		}
		else if( ret == -303 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"303 ??????????????????");
		}
		else if( ret == -304 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"304 ????????????С?");
		}
		else if( ret == -401 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"401 ??δ???WGS????????????????????????????");
		}
		else if( ret == -402 ){
			SubProcNo = 100;
			cleanupNetwork();
			strcpy(msg,"402 WGS????????????????????????????????");
		}
	}

	if( SubProcNo == 10 )
	{
		for( i = 0; i < sizeof( btnGraId )/sizeof( int ); i++ )
		{
			btnGraId[i] = -2;
		}
		SubProcNo++;
	}
	if( SubProcNo == 11 )
	{
		btnUseFlag = 2;
	}

	// ???????
	if( SubProcNo == 20 )
	{
		//cary ????????????
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
		PkMenuflag = 0 ;
#endif
		
		ret = DelCharGraColorWin();
//		ret = commonYesNoWindow( 320, 240 );
		// ??
		if( ret == 1 )
		{
			SubProcNo++;
		}
		else
		// ???
		if( ret == 2 )
		{
			SubProcNo = 10;
		}
	}
	if( SubProcNo == 21 )
	{
		initDeleteCharacter();
		SubProcNo++;
	}
	// ????
	if( SubProcNo == 22 )
	{
		ret = deleteCharacter();
		if( ret == 1 )
		{
			resetCharacterList( selectPcNo );
			clearUserSetting( selectPcNo );
			if( saveNowState() )
			{
			}
			if( maxPcNo > 0 )
				maxPcNo--;
			SubProcNo = 10;
		}
		else
		if( ret == -2 )
		{
			SubProcNo = 300;
		}
	}

	// ???
	if( SubProcNo == 100 )
	{
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 )
	{
		if( commonMsgWin( msg ) ){
			ChangeProc( PROC_TITLE_MENU );
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
			PkMenuflag = 0 ;
#endif
			SubProcNo=1;
		}
	}
	//??????????
	if( SubProcNo == 300 )
	{
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 301 )
	{
		if( commonMsgWin( "人物无法删除" ) )
			SubProcNo = 10;
	}

	ret = selGraId( btnGraId, sizeof( btnGraId )/sizeof( int ) );
	// ????????
	if( ret == 0 )
	{
		fade_out_bgm();
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
		PkMenuflag = 0 ;
#endif

		ChangeProc( PROC_TITLE_MENU );
		SubProcNo=1;
		play_se( 217, 320, 240 );	// click??
	}
	else
	// ?????????????
	if( 1 <= ret && ret <= 2 )
	{
		selectPcNo = ret - 1;
		ChangeProc( PROC_CHAR_MAKE );
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
		PkMenuflag = 0 ;
#endif

		play_se( 217, 320, 240 );	// click??
	}
	else
	// ???????????
	if( 3 <= ret && ret <= 4 )
	{
		char name[CHAR_NAME_LEN+1];

		// ????????e
		strcpy( name, chartable[ret-3].name );
//cary test		sjisStringToEucString( name );
		strcpy( gamestate_login_charname, name );
		selectPcNo = ret - 3;

		// ?????pc.faceGraNo??V????
		newCharacterFaceGraNo = chartable[selectPcNo].faceGraNo;
		loginDp = chartable[selectPcNo].dp;
#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
		PkMenuflag = 0 ;
#endif

		ChangeProc( PROC_CHAR_LOGIN_START );
		play_se( 217, 320, 240 );	// click??

		// shan 2002/01/18
		{
			map_bgm_no = 0;
			if( ret==3 ){
				sCharSide = 1;
			}else{
				sCharSide = 2;
			}
		}
	}
	else
	// ?????????
	if( 5 <= ret && ret <= 6 )
	{
		char name[CHAR_NAME_LEN+1];

		// ???????e
		strcpy( name, chartable[ret-5].name );
//cary test		sjisStringToEucString( name );
		strcpy( gamestate_deletechar_charname, name );
		selectPcNo = ret - 5;
		SubProcNo = 20;
		selCharGraColorWinProcNo = 0;
		play_se( 217, 320, 240 );	// click??
	}


	if( SubProcNo >= 10 )
	{
		// ??????????
		for( i = 0; i < MAXCHARACTER; i++ )
		{
			if( existCharacterListEntry( i ) )
			{
				// ???
				StockDispBuffer( 168+i*304, 84, DISP_PRIO_CHAR, chartable[i].faceGraNo, 0 );

				// ???q
				x = (144-strlen( chartable[i].name )*9)/2;
				StockFontBuffer( 93+i*304+x, 127,FONT_PRIO_BACK, FONT_PAL_WHITE, chartable[i].name, 0 );

				// ???
				sprintf( msg, "%3d", chartable[i].level );
				StockFontBuffer( 104+i*304, 152,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ??? ??
				sprintf( msg, "%3d", chartable[i].hp );
				StockFontBuffer( 104+i*304, 174,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ???
				sprintf( msg, "%3d", chartable[i].str );
				StockFontBuffer( 104+i*304, 196,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ?\??
				sprintf( msg, "%3d", chartable[i].def );
				StockFontBuffer( 104+i*304, 218,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ?????
				sprintf( msg, "%3d", chartable[i].agi );
				StockFontBuffer( 104+i*304, 240,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ?u?
				sprintf( msg, "%3d", chartable[i].app );
				StockFontBuffer( 104+i*304, 262,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ??
				sprintf( msg, "%8d", chartable[i].dp );
				StockFontBuffer( 188+i*304, 262,FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ?N??????
				for( j = 0; j < 4; j++ )
				{
					if( chartable[i].attr[j] > 0 )
					{
						x1 = 208+i*304;
						y1 = 177+j*22;
						x2 = x1 + chartable[i].attr[j] * 8;
						y2 = y1 + 8;
						StockBoxDispBuffer( x1,   y1,   x2,   y2,   DISP_PRIO_IME2, attrColor[j][0], 1 );
						StockBoxDispBuffer( x1+1, y1+1, x2+1, y2+1, DISP_PRIO_IME1, attrColor[j][1], 0 );
						StockBoxDispBuffer( x1+2, y1+2, x2+2, y2+2, DISP_PRIO_IME1, attrColor[j][1], 0 );
					}
				}

				// ??????
				login = chartable[i].login;
				if( login >= 1000 )
					login = 9999;	// ?????????
				sprintf( msg, "%4d", login );
				StockFontBuffer( 156+i*304, 284,
					FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

				// ???????
				// ???????????
				btnGraId[3+i] =
					StockDispBuffer( 100+i*304, 342,
						DISP_PRIO_CHAR, CG_CHR_SEL_LOGIN_BTN, btnUseFlag );
				// ?????????
				btnGraId[5+i] =
					StockDispBuffer( 230+i*304, 342,
						DISP_PRIO_CHAR, CG_CHR_SEL_DEL_BTN, btnUseFlag );
			}
			else
			if( SubProcNo < 100 || 300 <= SubProcNo )
			{
				// ???????????????????
				btnGraId[1+i] =
					StockDispBuffer( 160+i*304,
						342, DISP_PRIO_CHAR, CG_CHR_SEL_NEW_BTN, btnUseFlag );
			}
		}

		// ?????
		btnGraId[0] =
			StockDispBuffer( 320, 432, DISP_PRIO_CHAR, CG_CHR_SEL_BACK_BTN, btnUseFlag );
	}

	// ??????
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_SEL_BG, 0 );

#ifdef _LOGINKICK			
	if ( StartTime == -1 ) 
		StartTime = TimeGetTime();
	if ( TimeGetTime() > StartTime + 30 * 1000 /*1000????*/) {
		ChangeProc( PROC_TITLE_MENU );
		StartTime = -1 ; 
	}
#endif

#ifdef _PKSERVERCHARSEL			// (?????) Syu ADD PK???????????????
	if ( VersionXYZ == 1 ) {
		PkBtn[0] = StockDispBuffer( 160, 430, DISP_PRIO_CHAR, CG_PKSERVER_BTN_UP + PkMenuflag , 2 );
		if( HitDispNo == PkBtn[ 0 ] ) {
			if( mouse.onceState & MOUSE_LEFT_CRICK ) {
				( PkMenuflag == 0 ) ? PkMenuflag = 1 : PkMenuflag = 0 ; 
				play_se( 217, 320, 240 );
			}
		}
		if ( PkMenuflag == 1 ) {
			if ( PkServerSel == NULL ) {
				PkServerSel = MakeWindowDisp( 255 , 5 , 2 , 7 , 0 , -1 ) ; 
			} else {
				if( PkServerSel->hp > 0 ) {
					x3 = PkServerSel->x ; 
					y3 = PkServerSel->y ;
					StockDispBuffer( x3 + 60 , y3 + 200 , DISP_PRIO_IME3 , CG_PKSERVER_PANEL , 0 ) ; 
					StockFontBuffer( x3 + 20 , y3 + 155, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "??????????" , 1 );
					for ( i = 0 ; i < 4 ; i ++ ) {
						for ( j = 0 ; j < 3 ; j ++ ) {
							StockFontBuffer( x3 - 95 + i * 83 , y3 + 178 + j * 22  , FONT_PRIO_FRONT, FONT_PAL_WHITE, ServerName[ i * 3 + j + 1] , 2 );
							if ( mouse.onceState & MOUSE_LEFT_CRICK ) {
							//???i * 4 + j + 1??Gmsv????????AC????
							}
						}
					}
				}
			}
		} else {
			if ( PkServerSel != NULL ) {
				DeathAction(PkServerSel);
				PkServerSel = NULL;
			}
		}
	}
#endif

	RunAction();			// ?????????
	StockTaskDispBuffer();	// ???????????????????
}
//end


// ?????????

static short deleteCharacterProcNo = 0;

// ???
void initDeleteCharacter( void )
{
	deleteCharacterProcNo = 0;
}

// ?????
//
//  ???k??	 0 ... ???
//				 1 ... ????
//				-1 ... ??????
//				-2 ... ???
int deleteCharacter( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	static char msg[] = "人物删除中";

	// ???
	if( deleteCharacterProcNo == 0 )
	{
		deleteCharacterProcNo = 1;

		// ????????
		w = strlen( msg )*9/64+2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640 -w*64)/2;
		y = (480 -h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ????????x
	if( deleteCharacterProcNo == 1 )
	{
		delCharStart();
		deleteCharacterProcNo++;
	}
	else
	if( deleteCharacterProcNo == 2 )
	{
		ret = delCharProc();
	}

	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;

			len = strlen( msg );
			xx = (w*64-len*8)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}



// ??????????x??

static short downloadCharListProcNo = 0;
#ifdef _PK2007
static short downloadpkListProcNo = 0;
#endif
// ???
void initDownloadCharList( void )
{
	downloadCharListProcNo = 0;
}

// ?????
//
//  ???k??	 0 ... ?????
//				 1 ... ??????
//				-1 ... ??????
//				-2 ... ???
//              -3 ... ?????????
//              -4 ... ????_??????
int downloadCharList( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	static char msg[256];

	// ???
	if( downloadCharListProcNo == 0 )
	{
		downloadCharListProcNo = 1;

		strcpy( msg, "人物名单取得中" );

		// ????????
		w = strlen( msg )*8/64+2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ????????x
	if( downloadCharListProcNo == 1 )
	{
		charListStart();
		downloadCharListProcNo++;
	}
	else
	if( downloadCharListProcNo == 2 )
	{
		ret = charListProc();
	}

	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;

			len = strlen( msg );
			xx = (w*64-len*8)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}
#ifdef _PK2007
int downloadpkList( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	static char msg[256];
	if( downloadpkListProcNo == 0 )
	{
		downloadpkListProcNo = 1;
		strcpy( msg, "星系列表取得中" );
		w = strlen( msg )*8/64+2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	if( downloadpkListProcNo == 1 )
	{
		pkListStart();
		downloadpkListProcNo++;
	}
	else if( downloadpkListProcNo == 2 )
	{
		ret = pkListProc();
	}

	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;
			len = strlen( msg );
			xx = (w*64-len*8)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}
#endif

// ????????k???????
//
//  ???k??	0 ... ???
//				1 ... ??????????
//				2 ... ???????????
int commonYesNoWindow( int x, int y )
{
	int i;
	int ret = 0;
	int id;
	static int btnGraId[] = { -2, -2 };

	id = selGraId( btnGraId, sizeof( btnGraId )/sizeof( int ) );
	// ??????????
	if( id == 0 )
	{
		ret = 1;
		play_se( 217, 320, 240 );	// ?????
	}
	else
	// ???????????
	if( id == 1 )
	{
		ret = 2;
		play_se( 217, 320, 240 );	// ?????
	}

	btnGraId[0] = StockDispBuffer( x, y, DISP_PRIO_YES_NO_BTN, CG_COMMON_YES_BTN, 2 );
	btnGraId[1] = StockDispBuffer( x, y, DISP_PRIO_YES_NO_BTN, CG_COMMON_NO_BTN, 2 );

	StockDispBuffer( x, y, DISP_PRIO_YES_NO_WND, CG_COMMON_WIN_YORO, 0 );

	if( ret != 0 )
	{
		for( i = 0; i < sizeof( btnGraId )/sizeof( int ); i++ )
		{
			btnGraId[i] = -2;
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////
//
// ??????
//

// ??????k???

#define	MaxSelectChar  	12
static ACTION *ptActSelChar[MaxSelectChar];
static int selectGraNoTbl[MaxSelectChar] =
{
	SPR_001em,
	SPR_011em,
	SPR_021em,
	SPR_031em,
	SPR_041em,
	SPR_051em,
	SPR_061em,
	SPR_071em,
	SPR_081em,
	SPR_091em,
	SPR_101em,
	SPR_111em
};
int selectGraLocate[MaxSelectChar][2] =
{
	{ 198, 301 },
	{ 113, 258 },
	{ 248, 396 },
	{ 130, 370 },
	{ 137, 160 },
	{ 343, 379 },
	{ 479, 371 },
	{ 420, 310 },
	{ 376, 231 },
	{ 297, 198 },
	{ 381, 146 },
	{ 242, 116 }
};
int selectGraHitArea[MaxSelectChar][4] =
{
//	    x,   y,   w,   h
	{ -32, -56,  63,  65 },
	{ -21, -72,  42,  88 },
	{ -19, -78,  38,  94 },
	{ -20, -67,  41,  80 },
	{ -30, -80,  60, 100 },
	{ -36, -84,  72, 100 },
	{ -30, -51,  59,  64 },
	{ -19, -77,  40,  88 },
	{ -18, -72,  34,  83 },
	{ -22, -72,  43,  89 },
	{ -18, -78,  35,  90 },
	{ -20, -76,  40,  90 }//
};
static int selCharColor[MaxSelectChar];	// ??????
static int selCharDir[MaxSelectChar];	// ??????
static int selCharGraNoProcNo;
static int nowSelCharGraNo;			// ???????
static int nowSelCharMouthNo;		// ???????
static int nowSelCharEyeNo;			// ???????
static int nowSelCharStatusPoint;	// ????????????????????
static int nowSelCharStatus[4];		// ?????????(?l??????????????)
static int nowSelCharAttrPoint;		// ?????N?????????????
static int nowSelCharAttr[4];		// ?????N??(?????)

// ????????????????
//static int selCharGraColorWinProcNo;

// ????????????@?e?
static int editCharParamProcNo;
STR_BUFFER selCharName;
static int selCharNameBoxX, selCharNameBoxY;

// ????????k???
static int selHomeTownProcNo;
static int nowSelHomeTownNo;

// ???????????
static int createCharProcNo;



void initMakeCharacter( void );

void initSelCharGraNo( void );
void initSelCharGraNo2( void );
int selCharGraNo( void );
void initSelCharGraColorWin( void );
int selCharGraColorWin( void );
void initEditCharParam( void );
void initEditCharParam2( void );
int editCharParam( void );
void initCreateChar( void );
int createChar( void );
void initSelHomeTown( void );
int selHomeTown( void );



// ??????????????
void initMakeCharacter( void )
{
}

// ???????????
void makeCharacterProc( void )
{
	int ret;

	// ???
	if( SubProcNo == 0 )
	{
		initMakeCharacter();
		SubProcNo++;
	}

	// ??????k??????
	if( SubProcNo == 1 )
	{
		initSelCharGraNo();
		SubProcNo = 3;
	}
	else
	if( SubProcNo == 2 )
	{
		initSelCharGraNo2();
		SubProcNo++;
	}
	// ??????k????
	if( SubProcNo == 3 )
	{
		ret = selCharGraNo();
		// ????k????e
		if( ret == 1 )
		{
			SubProcNo = 10;
		}
		else
		// ?????????
		if( ret == 2 )
		{
			ChangeProc( PROC_CHAR_SELECT, 10 );
//			return;
		}
	}

	// ??????????@?e????
	if( SubProcNo == 10 && ret == 1 )
	{
		initEditCharParam();
		SubProcNo = 12;
	}
	else
	if( SubProcNo == 11 )
	{
		initEditCharParam2();
		SubProcNo = 12;
	}
	if( SubProcNo == 12 )
	{
		ret = editCharParam();
		if( ret == 1 )
		{
			SubProcNo = 20;
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
			if ( pActPet20 != NULL)
			{
				DeathAction( pActPet20 );
				pActPet20 = NULL;
			}
#endif
		}
		// ?????
		if( ret == 2 )
		{
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
			if ( pActPet20 != NULL)
			{
				DeathAction( pActPet20 );
				pActPet20 = NULL;
			}
#endif
			SubProcNo = 2;
		}
	}

	// ????????k??
	if( SubProcNo == 20 )
	{
#ifndef _DELBORNPLACE //Syu ADD 6.0 ?????????????
		initSelHomeTown();
#endif
		SubProcNo = 21;
	}
	if( SubProcNo == 21 )
	{
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
		ret = 1;
#else
		ret = selHomeTown();
#endif
		// ??e
		if( ret == 1 )
		{
			SubProcNo = 30;
			// ??q???????????
			resetCharacterList( selectPcNo );
			clearUserSetting( selectPcNo );
		}
		else
		// ?????
		if( ret == 2 )
		{
			SubProcNo = 11;
		}
	}

	// ???????????????
	if( SubProcNo == 30 )
	{
		initCreateChar();
		SubProcNo = 31;
	}
	if( SubProcNo == 31 )
	{
		ret = createChar();
		if( ret == 1 )
		{
			maxPcNo++;
			strcpy( gamestate_login_charname, newCharacterName );
			ChangeProc( PROC_CHAR_LOGIN_START );
			createCharFlag = 1;
			//return;
		}
		else
		if( ret == 2 )
		{
			SubProcNo = 100;
		}
		selHomeTown();	// ???????
	}

	// ???
	if( SubProcNo == 100 )
	{
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 )
	{
		if( commonMsgWin( "人物创造失败" ) )
		{
			// ???????????
			ChangeProc( PROC_CHAR_SELECT, 10 );
			//return;
		}
		selHomeTown();	// ???????
	}

	RunAction();			// ?????????
	StockTaskDispBuffer();	// ???????????????????
}




// ??????k??????
void initSelCharGraNo( void )
{
	int i;

	selCharGraNoProcNo = 0;

	// ??????k??????
	nowSelCharGraNo = -1;	// ???k????t?-1
	nowSelCharMouthNo = 0;
	nowSelCharEyeNo = 0;

	for( i = 0; i < MaxSelectChar; i++ )
	{
		selCharColor[i] = 0;	// ??????
		selCharDir[i] = 0;		// ??????
	}

	// ????????????
	for( i = 0; i < MaxSelectChar; i++ )
	{
		if( selectGraNoTbl[i] < 0 )
		{
			ptActSelChar[i] = NULL;
			continue;
		}

		ptActSelChar[i] = GetAction( PRIO_CHR, 0 );
		if( ptActSelChar[i] != NULL )
		{
			ptActSelChar[i]->anim_chr_no = selectGraNoTbl[i];
			ptActSelChar[i]->anim_no = ANIM_STAND;
			ptActSelChar[i]->dispPrio = DISP_PRIO_CHAR;
			ptActSelChar[i]->x = selectGraLocate[i][0];
			ptActSelChar[i]->y = selectGraLocate[i][1];
			ptActSelChar[i]->anim_ang = selCharDir[i];
		}
	}

}

// ??????k??????
// ??q????????????????????????????????
void initSelCharGraNo2( void )
{
	int i;

	selCharGraNoProcNo = 0;
	nowSelCharMouthNo = 0;
	nowSelCharEyeNo = 0;

	// ??????k??????
	nowSelCharGraNo = -1;	// ???k????t?-1

	// ????????????
	for( i = 0; i < MaxSelectChar; i++ )
	{
		if( selectGraNoTbl[i] < 0 )
		{
			ptActSelChar[i] = NULL;
			continue;
		}

		ptActSelChar[i] = GetAction( PRIO_CHR, 0 );
		if( ptActSelChar[i] != NULL )
		{
			ptActSelChar[i]->anim_chr_no = selectGraNoTbl[i];
			ptActSelChar[i]->anim_no = ANIM_STAND;
			ptActSelChar[i]->dispPrio = DISP_PRIO_CHAR;
			ptActSelChar[i]->x = selectGraLocate[i][0];
			ptActSelChar[i]->y = selectGraLocate[i][1];
			ptActSelChar[i]->anim_ang = selCharDir[i];
		}
	}
}

// ??????k????
//
//  ???k??	0 ... ???
//				1 ... ??e
//				2 ... ?????
int selCharGraNo( void )
{
	int i;
	int ret = 0;
	int ret2;
	int x1, y1, x2, y2;
	int id;
	static int selCharCanselGraId[] = { -2 };


	// ???
	if( selCharGraNoProcNo == 0 )
	{
		for( i = 0; i < sizeof( selCharCanselGraId )/sizeof( int ); i++ )
		{
			selCharCanselGraId[i] = -2;
		}
		selCharGraNoProcNo++;
	}

	// ????k??
	if( selCharGraNoProcNo == 1 )
	{
		for( i = 0; i < MaxSelectChar; i++ )
		{
			if( ptActSelChar[i] != NULL )
			{
				x1 = ptActSelChar[i]->x + selectGraHitArea[i][0];
				y1 = ptActSelChar[i]->y + selectGraHitArea[i][1];
				x2 = x1 + selectGraHitArea[i][2];
				y2 = y1 + selectGraHitArea[i][3];
				if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX ) )
				{

					ptActSelChar[i]->anim_no = ANIM_WALK;
					if( mouse.onceState & MOUSE_LEFT_CRICK )
					{
						// ?????k?????
						selCharGraNoProcNo = 2;
						nowSelCharGraNo = i;
						play_se( 217, 320, 240 );	// ?????
					}

				}
				else
				{
					ptActSelChar[i]->anim_no = ANIM_STAND;
				}
			}
		}
	}

	// ????k??????
	if( selCharGraNoProcNo == 2 )
	{
		initSelCharGraColorWin();
		selCharGraNoProcNo++;
	}
	if( selCharGraNoProcNo == 3 )
	{
		ret2 = selCharGraColorWin();
		// ??e
		if( ret2 == 1 )
		{
			ret = 1;
		}
		else
		// ????????
		if( ret2 == 2 )
		{
			selCharGraNoProcNo = 1;
			nowSelCharGraNo = -1;
		}
	}

	// ???????????R?e
	id = selGraId( selCharCanselGraId, sizeof( selCharCanselGraId )/sizeof( int ) );
	// ?????
	if( id == 0 )
	{
		ret = 2;
		play_se( 217, 320, 240 );	// ?????
	}

	// ????????????
	if( ret != 0 )
	{
		for( i = 0; i < MaxSelectChar; i++ )
		{
			if( ptActSelChar[i] != NULL )
			{
				DeathAction( ptActSelChar[i] );
				ptActSelChar[i] = NULL;
			}
		}
	}


	// ????????
	for( i = 0; i < MaxSelectChar; i++ )
	{
		if( ptActSelChar[i] != NULL )
		{
			if( nowSelCharGraNo == i )
			{
				ptActSelChar[i]->anim_no = ANIM_WALK;
				x1 = ptActSelChar[i]->x + selectGraHitArea[i][0];
				y1 = ptActSelChar[i]->y + selectGraHitArea[i][1];
				x2 = x1 + selectGraHitArea[i][2];
				y2 = y1 + selectGraHitArea[i][3];
				StockBoxDispBuffer( x1, y1, x2, y2, DISP_PRIO_BOX, BoxColor, 0 );
				//MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX );
			}
			ptActSelChar[i]->anim_chr_no = selectGraNoTbl[i] + selCharColor[i]*5;
			ptActSelChar[i]->anim_ang = selCharDir[i];
			pattern( ptActSelChar[i], ANM_NOMAL_SPD, ANM_LOOP );
		}
	}

	if( selCharGraNoProcNo == 1 )
	{
		id = focusGraId( selCharCanselGraId, sizeof( selCharCanselGraId )/sizeof( int ) );
		if( id == 0 )
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"??????????档");
		}

		else
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"????????????");
		}

		// ????k?????
		selCharCanselGraId[0] =
			StockDispBuffer( 64, 400, DISP_PRIO_BG, CG_CHR_MAKE_BACK_BTN, 2 );
	}

	if( ret == 0 )
	{
		// ??????
		StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_SEL_BG, 0 );
	}

	RunAction();			// ?????????
	StockTaskDispBuffer();	// ???????????????????

	return ret;
}




// ??????????????????
void initSelCharGraColorWin( void )
{
	selCharGraColorWinProcNo = 0;
}

// ??????????????????
//
//  ???k??	0 ... ???
//				1 ... OK????????
//				2 ... ?????????????
int selCharGraColorWin( void )
{
	static int fontId[] = { -2, -2, -2, -2 };
	static ACTION *ptActMenuWin = NULL;
	int id;
	int i;
	static int x, y, w, h;
	int ret = 0;
	char msg[][32] =
	{
		"   决定   ",
		" 改变颜色 ",
		" 转动方向 ",
		"   取消   "
	};

	// ???
	if( selCharGraColorWinProcNo == 0 )
	{
		selCharGraColorWinProcNo = 1;

		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}

		// ????????
		w = 2;
		h = 3;
		x = 496;
		y = 80;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ??????k???R?e
	id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

	// ??e
	if( id == 0 )
	{
		ret = 1;
		play_se( 217, 320, 240 );	// ?????
	}
	else
	// ????k??
	if( id == 1 )
	{
		selCharColor[nowSelCharGraNo]++;
		if( selCharColor[nowSelCharGraNo] > 3 )
			selCharColor[nowSelCharGraNo] = 0;
		play_se( 217, 320, 240 );	// ?????
	}
	else
	// ??????
	if( id == 2 )
	{
		selCharDir[nowSelCharGraNo]++;
		if( selCharDir[nowSelCharGraNo] > 7 )
			selCharDir[nowSelCharGraNo] = 0;
		play_se( 217, 320, 240 );	// ?????
	}
	else
	// ?????
	if( id == 3 )
	{
		ret = 2;
		play_se( 217, 320, 240 );	// ?????
	}

	// ?????????????????
	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}


	// ???????????????
	id = focusFontId( fontId, sizeof( fontId )/sizeof( int ) );

	// ??e
	if( id == 0 )
	{
		//ShowBottomLineString(FONT_PAL_WHITE,"????????????????");
	}
	else
	// ????k??
	if( id == 1 )
	{
		//ShowBottomLineString(FONT_PAL_WHITE,"?????????????");
	}
	else
	// ??????
	if( id == 2 )
	{
		//ShowBottomLineString(FONT_PAL_WHITE,"???????????");
	}
	else
	// ?????
	if( id == 3 )
	{
		//ShowBottomLineString(FONT_PAL_WHITE,"?????????????档");
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			{
				fontId[i] =
					StockFontBuffer( x+22, y+i*22+32,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, msg[i], 2 );
			}
		}
	}

	return ret;
}



void initEditCharParam( void )
{
	editCharParamProcNo = 0;

	selCharNameBoxX = 104;
	selCharNameBoxY = 102;
	initStrBuffer( &selCharName, selCharNameBoxX, selCharNameBoxY,
		CHAR_NAME_LEN, FONT_PAL_WHITE, FONT_PRIO_BACK );
	selCharName.buffer[0] = '\0';
	selCharName.cnt = 0;
	selCharName.cursor=0;
	GetKeyInputFocus( &selCharName );

#ifdef __TEST_SERVER
	if( (bNewServer & 0xf100000) == 0xf100000){
		nowSelCharStatusPoint = 550;
	}else{
		nowSelCharStatusPoint = 20;
	}
#else
	//nowSelCharStatusPoint = 20;
	nowSelCharStatusPoint = 0;
#endif
//	nowSelCharStatus[0]   =  0;
//	nowSelCharStatus[1]   =  0;

	nowSelCharStatus[0]   =  10;
	nowSelCharStatus[1]   =  10;

	nowSelCharStatus[2]   =  0;
	nowSelCharStatus[3]   =  0;
	nowSelCharAttrPoint   = 10;
	nowSelCharAttr[0]     =  0;
	nowSelCharAttr[1]     =  0;
	nowSelCharAttr[2]     =  0;
	nowSelCharAttr[3]     =  0;
}


void initEditCharParam2( void )
{
	editCharParamProcNo = 0;

	selCharNameBoxX = 104;
	selCharNameBoxY = 102;
	initStrBuffer( &selCharName, selCharNameBoxX, selCharNameBoxY,
		CHAR_NAME_LEN, FONT_PAL_WHITE, FONT_PRIO_BACK );
	GetKeyInputFocus( &selCharName );
}
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
typedef struct{
	int spr_num;	 //SPR???
	int earth;		 //??????
	int water;		 //?????
	int fire;		 //??????
	int wind;		 //??????
	char name[16];	 //????????
} BORNPETINFO;
#endif
// ????????????@?e
//
//  ???k??	0 ... ???
//				1 ... ??e
//				2 ... ?????
int editCharParam( void )
{
	int ret = 0;
	char msg[64];
	int x1, y1, x2, y2;
	int cnt =1;
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
	BORNPETINFO petinfo[4] = {

		{ 100251 , 0 , 4 , 6 , 0 , "????????"},
		{ 100296 , 2 , 0 , 0 , 8 , "????"},
		{ 100292 , 0 , 7 , 3 , 0 , "????"},
		{ 100266 , 6 , 0 , 0 , 4 , "????"}};

/*		{ 101490 , 7 , 5 , 8 , 3 , "??????"},
		{ 100444 , 10 , 10 , 10 , 10 , "?????????"},
		{ 100725 , 10 , 9 , 10 , 9 , "??????"},
		{ 100451 , 1 , 1 , 1 , 1 , "???????"}};
*/	int BornPreBtn , BornNextBtn ;
	int attrx1 , attry1 , attrx2 , attry2;
	int BornBaseX = 430 , BornBaseY = 110 ;
#endif
	static int graId[] =
	{ -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	  -2, -2, -2, -2, -2, -2, -2, -2,
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
	  -2, -2, -2, -2, -2, -2};
#else
	  -2, -2, -2, -2 };
#endif
	int i;
	int id, id2, id3;


	int statusLocate[4][2] =
	{
		{ 111, 250 },
		{ 111, 278 },
		{ 111, 305 },
		{ 111, 334 }
	};
	int attrLocate[4][2] =
	{
		{ 297, 255 },
		{ 297, 279 },
		{ 297, 303 },
		{ 297, 327 }
	};
	int attrColor[4][2] =
	{
		{ SYSTEM_PAL_GREEN,  SYSTEM_PAL_GREEN2  },
		{ SYSTEM_PAL_AQUA,   SYSTEM_PAL_AQUA2   },
		{ SYSTEM_PAL_RED,    SYSTEM_PAL_RED2    },
		{ SYSTEM_PAL_YELLOW, SYSTEM_PAL_YELLOW2 }
	};
	int upDownBtnGraNo[][2] =
	{
		{ CG_PREV_BTN, CG_PREV_BTN_DOWN },
		{ CG_NEXT_BTN, CG_NEXT_BTN_DOWN }
	};
	int selUseFlag = 0;
	int btn1, btn2;



	//GetKeyInputFocus( NULL );

	// ???
	if( editCharParamProcNo == 0 )
	{
		for( i = 0; i < sizeof( graId )/sizeof( int ); i++ )
		{
			graId[i] = -2;
		}
		editCharParamProcNo++;
	}

	// ??????@?e??
	id = -1;
	id3 = -1;
	if( editCharParamProcNo == 1 )
	{
		selUseFlag = 2;
		GetKeyInputFocus( &selCharName );

		// ???????????R?e
		//id = selGraId( graId, sizeof( graId )/sizeof( int ) );
		id = selRepGraId( graId, sizeof( graId )/sizeof( int ) );
		id3 = pushGraId( graId, sizeof( graId )/sizeof( int ) );


		// ??e
		if( id == 0 )
		{
			// ???q??V??????
			if( selCharName.cnt <= 0 )
			{
				editCharParamProcNo = 100;
			}
			else
			// ??????????????????????
			if( nowSelCharStatusPoint > 0 )
			{
				editCharParamProcNo = 104;
			}
			else
			// ?N???????????????????
			if( nowSelCharAttrPoint > 0 )
			{
				editCharParamProcNo = 106;
			}
			else
			// ?i?????????
			{
				strcpy( newCharacterName, selCharName.buffer );
				newCharacterName[selCharName.cnt] = '\0';

				if( cmpNameCharacterList( selCharName.buffer ) )
				{
					// ?i????q???????
					editCharParamProcNo = 102;
				}
				else
				// ???????????????q???
				if( strstr( newCharacterName, " " ) != NULL
				 || strstr( newCharacterName, "  " ) != NULL )
				{
					editCharParamProcNo = 108;
				}
				/*else
				if(!CheckName(newCharacterName))
				{
					editCharParamProcNo = 110;
				}*/
				else
				{
					// ??e
					ret = 1;
					newCharacterGraNo =
						selectGraNoTbl[nowSelCharGraNo]+selCharColor[nowSelCharGraNo]*5;
					newCharacterFaceGraNo =
						CG_CHR_MAKE_FACE+nowSelCharGraNo*100
						+selCharColor[nowSelCharGraNo]*25
						+nowSelCharEyeNo*5+nowSelCharMouthNo;
					newCharacterVit = nowSelCharStatus[0];
					newCharacterStr = nowSelCharStatus[1];
					newCharacterTgh = nowSelCharStatus[2];
					newCharacterDex = nowSelCharStatus[3];
					newCharacterEarth = nowSelCharAttr[0];
					newCharacterWater = nowSelCharAttr[1];
					newCharacterFire  = nowSelCharAttr[2];
					newCharacterWind  = nowSelCharAttr[3];
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
					//????????????????????????
					newCharacterHomeTown = BornPetNum;
#endif
					loginDp = 0;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		// ??
		if( id == 1 )
		{
			ret = 2;
			play_se( 217, 320, 240 );	// ?????
		}
		else
		// ????????????????
		if( 2 <= id && id <= 9 )
		{
			id2 = id - 2;
			// ?????????
			if( (id2 % 2) == 0 )
			{
				if( nowSelCharStatus[id2/2] > 0 )
				{
					nowSelCharStatus[id2/2]--;
					nowSelCharStatusPoint++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
			else
			// ?????????
			{
				if( nowSelCharStatusPoint > 0 )
				{

#ifdef __TEST_SERVER
					if( nowSelCharStatusPoint > 100 )
						cnt = 10;
					else
						cnt = 1;
					nowSelCharStatus[id2/2] += cnt;
					nowSelCharStatusPoint -= cnt;
					play_se( 217, 320, 240 );
#else
					nowSelCharStatus[id2/2]++;
					nowSelCharStatusPoint--;
					play_se( 217, 320, 240 );
#endif

				}
			}
		}
		else
		// ?N?????????
		if( 10 <= id && id <= 17 )
		{
			id2 = id - 10;
			// ?????????
			if( (id2 % 2) == 0 )
			{
				if( nowSelCharAttr[id2/2] > 0 )
				{
					nowSelCharAttr[id2/2]--;
					nowSelCharAttrPoint++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
			else
			// ?????????
			{
				int no, no2;

				no = id2/2;
				if( no < 2 )
				{
					no2 = no+2;
				}
				else
				{
					no2 = no-2;
				}

				if( nowSelCharAttr[no2] == 0 )
				{
					if( nowSelCharAttrPoint > 0 )
					{
						nowSelCharAttr[no]++;
						nowSelCharAttrPoint--;
						play_se( 217, 320, 240 );	// ?????
					}
					else
					// ????????????????????????????????
					// ?????
					{
						// ??????????????????????????????????S?
						for( i = 0; i < 4; i++ )
						{
							if( i != no && nowSelCharAttr[i] > 0 )
							{
								break;
							}
						}
						if( i < 4 )
						{
							nowSelCharAttr[no]++;
							nowSelCharAttr[i]--;
							play_se( 217, 320, 240 );	// ?????
						}
					}
				}
			}
		}
		else
		if( id == 18 || id == 19 )
		{
			// ?????????
			if( id == 18 )
			{
				if( nowSelCharEyeNo > 0 )
				{
					nowSelCharEyeNo--;
				}
				else
				{
					nowSelCharEyeNo = 4;
				}
			}
			else
			// ?????????
			{
				if( nowSelCharEyeNo < 4 )
				{
					nowSelCharEyeNo++;
				}
				else
				{
					nowSelCharEyeNo = 0;
				}
			}
			play_se( 217, 320, 240 );	// ?????
		}
		else
		if( id == 20 || id == 21 )
		{
			// ?????????
			if( id == 20 )
			{
				if( nowSelCharMouthNo > 0 )
				{
					nowSelCharMouthNo--;
				}
				else
				{
					nowSelCharMouthNo = 4;
				}
			}
			else
			// ?????????
			{
				if( nowSelCharMouthNo < 4 )
				{
					nowSelCharMouthNo++;
				}
				else
				{
					nowSelCharMouthNo = 0;
				}
			}
			play_se( 217, 320, 240 );	// ?????
		}
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
		else
		if( id == 22 || id == 23 )
		{
			if ( pActPet20 != NULL)
			{
				DeathAction( pActPet20 );
				pActPet20 = NULL;
			}
			if ( id == 22 )
			{
				if ( BornPetNum > 0 )
					BornPetNum -- ;
				else
					BornPetNum = 3 ;
			}
			else
			{
				if ( BornPetNum < 3 )
					BornPetNum ++ ;
				else
					BornPetNum = 0 ;
			}
			play_se( 217, 320, 240 );
		}
#endif
	}


	// ????????
	// ???q?V????
	if( editCharParamProcNo == 100 )
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 101 )
	{
		if( commonMsgWin( "没有输入名字！" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}
	// ?i??????
	if( editCharParamProcNo == 102 )
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 103 )
	{
		if( commonMsgWin( "已经有同名的人物存在了！" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}
	// ??????????????????????
	if( editCharParamProcNo == 104 )
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 105 )
	{
		if( commonMsgWin( "请将状态的点数分配完毕！" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}
	// ?N???????????????????
	if( editCharParamProcNo == 106 )
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 107 )
	{
		if( commonMsgWin( "请将属性的点数分配完毕" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}
	// ???q??????
	if( editCharParamProcNo == 108 )
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 109 )
	{
		if( commonMsgWin( "名字里面不可以有空白！" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}
	if(editCharParamProcNo == 110)
	{
		initCommonMsgWin();
		editCharParamProcNo++;
		play_se( 220, 320, 240 );	// ???
	}
	if( editCharParamProcNo == 111 )
	{
		if( commonMsgWin( "你输入的名字有不允许字符" ) )
		{
			// ???????????
			editCharParamProcNo = 1;
		}
	}


	// ?C??
	if( editCharParamProcNo == 1 )
	{
		id = focusGraId( graId, sizeof( graId )/sizeof( int ) );
		if( id == 0 )
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"??????????趨??");
		}
		else
		if( id == 1 )
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"??????????档");
		}
		else
		if( 2 <= id && id <= 9 )
		{
			id -= 2;
			id /= 2;
			if( id == 0 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"???????????????й??");
			}
			else
			if( id == 1 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"??????????????й??");
			}
			else
			if( id == 2 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"???????????????й??");
			}
			else
			if( id == 3 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"????????????????ж?????й??");
			}
		}
		else
		if( 10 <= id && id <= 17 )
		{
			id -= 10;
			id /= 2;
			if( id == 0 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"???????????????????????????????");
			}
			else
			if( id == 1 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"??????????????????????????????");
			}
			else
			if( id == 2 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"???????????????????????????????");
			}
			else
			if( id == 3 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"???????????????????????????????");
			}
		}
		else
		if( id == 18 || id == 19 )
		{
			if( nowSelCharGraNo != 0 && nowSelCharGraNo != 6 )
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"?????????????");
			}
			else
			{
				//ShowBottomLineString(FONT_PAL_WHITE,"????????????");
			}
		}
		else
		if( id == 20 || id == 21 )
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"????????????");
		}
		else
		{
			//ShowBottomLineString(FONT_PAL_WHITE,"?趨???????????????????????");
		}
	}



	StockFontBuffer2( &selCharName );

	// ???????
	StockDispBuffer( 93, 172, DISP_PRIO_BG,
		CG_CHR_MAKE_FACE+nowSelCharGraNo*100+selCharColor[nowSelCharGraNo]*25
		+nowSelCharEyeNo*5+nowSelCharMouthNo,
			0 );

	// ????????????????????
	sprintf( msg, "%3d", nowSelCharStatusPoint );
	StockFontBuffer( 161, 224, FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );

	// ?????????
	for( i = 0; i < 4; i++ )
	{
		sprintf( msg, "%3d", nowSelCharStatus[i] );
		StockFontBuffer( statusLocate[i][0], statusLocate[i][1],
			FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );
	}


	// ?????N?????????????
	sprintf( msg, "%3d", nowSelCharAttrPoint );
	StockFontBuffer( 350, 224, FONT_PRIO_BACK, FONT_PAL_WHITE, msg, 0 );


	// ?N??????
	for( i = 0; i < 4; i ++ )
	{
		if( nowSelCharAttr[i] > 0 )
		{
			x1 = attrLocate[i][0];
			y1 = attrLocate[i][1];
			x2 = x1 + nowSelCharAttr[i] * 8;
			y2 = y1 + 8;
			StockBoxDispBuffer( x1,   y1,   x2,   y2,   DISP_PRIO_IME2, attrColor[i][0], 1 );
			StockBoxDispBuffer( x1+1, y1+1, x2+1, y2+1, DISP_PRIO_IME1, attrColor[i][1], 0 );
			StockBoxDispBuffer( x1+2, y1+2, x2+2, y2+2, DISP_PRIO_IME1, attrColor[i][1], 0 );
		}
	}


	// ?????????

	graId[0] = StockDispBuffer(  96, 410, DISP_PRIO_CHAR, CG_CHR_MAKE_OK_BTN, selUseFlag );
	// ????k?????
	graId[1] = StockDispBuffer( 192, 410, DISP_PRIO_CHAR, CG_CHR_MAKE_BACK_BTN, selUseFlag );

	for( i = 0; i < 4; i++ )
	{
		btn1 = 0;
		btn2 = 0;
		if( 2 <= id3 && id3 <= 9
		 && (id3-2)/2 == i )
		{
			if( ((id3-2)%2) == 0 )
			{
				btn1 = 1;
			}
			else
			{
				btn2 = 1;
			}
		}
		// < ???
		graId[i*2+2] = StockDispBuffer( statusLocate[i][0]+51, statusLocate[i][1]+8,
						DISP_PRIO_CHAR, upDownBtnGraNo[0][btn1], selUseFlag );

		// > ???
		graId[i*2+3] = StockDispBuffer( statusLocate[i][0]+87, statusLocate[i][1]+8,
						DISP_PRIO_CHAR, upDownBtnGraNo[1][btn2], selUseFlag );
	}

	for( i = 0; i < 4; i++ )
	{
		if( i == 0 && nowSelCharAttr[i+2] == 0
		 || i == 1 && nowSelCharAttr[i+2] == 0
		 || i == 2 && nowSelCharAttr[i-2] == 0
		 || i == 3 && nowSelCharAttr[i-2] == 0 )
		{
			btn1 = 0;
			btn2 = 0;
			if( 10 <= id3 && id <= 17
			 && (id3-10)/2 == i )
			{
				if( ((id3-10)%2) == 0 )
				{
					btn1 = 1;
				}
				else
				{
					btn2 = 1;
				}
			}

			// < ???
			graId[i*2+10] = StockDispBuffer( attrLocate[i][0]+101, attrLocate[i][1]+5,
							DISP_PRIO_CHAR, upDownBtnGraNo[0][btn1], selUseFlag );

			// > ???
			graId[i*2+11] = StockDispBuffer( attrLocate[i][0]+137, attrLocate[i][1]+5,
							DISP_PRIO_CHAR, upDownBtnGraNo[1][btn2], selUseFlag );
		}
		else
		{
			graId[i*2+10] = -2;
			graId[i*2+11] = -2;
		}
	}

	for( i = 0; i < 2; i++ )
	{
		btn1 = 0;
		btn2 = 0;
		if( 18 <= id3 && id3 <= 21
		 && (id3-18)/2 == i )
		{
			if( ((id3-18)%2) == 0 )
			{
				btn1 = 1;
			}
			else
			{
				btn2 = 1;
			}
		}

		// < ???
		graId[i*2+18] =
			StockDispBuffer( 156, 172+i*24,
				DISP_PRIO_CHAR, upDownBtnGraNo[0][btn1], selUseFlag );
		// > ???
		graId[i*2+19] =
			StockDispBuffer( 260, 172+i*24,
				DISP_PRIO_CHAR, upDownBtnGraNo[1][btn2], selUseFlag );
	}

	if( nowSelCharGraNo != 0 && nowSelCharGraNo != 6 )
	{
		StockDispBuffer( 320, 240, DISP_PRIO_CHAR, CG_CHR_MAKE_EYE_SEL, 0 );
	}
	else
	{
		StockDispBuffer( 320, 240, DISP_PRIO_CHAR, CG_CHR_MAKE_NOSE_SEL, 0 );
	}
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
	BornPreBtn = 0;
	BornNextBtn = 0;
	if(  id3 == 22 || id3 == 23 )
	{
		if( id3 == 22 )
			BornPreBtn = 1;
		else
			BornNextBtn = 1;
	}
	attrx1 = BornBaseX + 35 ; 
	if ( petinfo[BornPetNum].earth > 0 )
	{
		attry1 = BornBaseY  ; 
		attrx2 = attrx1 + petinfo[BornPetNum].earth * 8 ;
		attry2 = attry1 + 8 ; 
		StockBoxDispBuffer( attrx1 , attry1 , attrx2 , attry2 , DISP_PRIO_IME2 , SYSTEM_PAL_GREEN , 1);
		StockBoxDispBuffer( attrx1+1 , attry1+1 , attrx2+1 , attry2+1 , DISP_PRIO_IME1 , SYSTEM_PAL_GREEN2 , 0);
		StockBoxDispBuffer( attrx1+2 , attry1+2 , attrx2+2 , attry2+2 , DISP_PRIO_IME1 , SYSTEM_PAL_GREEN2 , 0);
	}
	if ( petinfo[BornPetNum].water > 0 )
	{
		attry1 = BornBaseY + 20 ; 
		attrx2 = attrx1 + petinfo[BornPetNum].water * 8 ;
		attry2 = attry1 + 8 ; 
		StockBoxDispBuffer( attrx1 , attry1 , attrx2 , attry2 , DISP_PRIO_IME2 , SYSTEM_PAL_AQUA , 1);
		StockBoxDispBuffer( attrx1+1 , attry1+1 , attrx2+1 , attry2+1 , DISP_PRIO_IME1 , SYSTEM_PAL_AQUA2 , 0);
		StockBoxDispBuffer( attrx1+2 , attry1+2 , attrx2+2 , attry2+2 , DISP_PRIO_IME1 , SYSTEM_PAL_AQUA2 , 0);
	}
	if ( petinfo[BornPetNum].fire > 0 )
	{
		attry1 = BornBaseY + 40 ; 
		attrx2 = attrx1 + petinfo[BornPetNum].fire * 8 ;
		attry2 = attry1 + 8 ; 
		StockBoxDispBuffer( attrx1 , attry1 , attrx2 , attry2 , DISP_PRIO_IME2 , SYSTEM_PAL_RED , 1);
		StockBoxDispBuffer( attrx1+1 , attry1+1 , attrx2+1 , attry2+1 , DISP_PRIO_IME1 , SYSTEM_PAL_RED2 , 0);
		StockBoxDispBuffer( attrx1+2 , attry1+2 , attrx2+2 , attry2+2 , DISP_PRIO_IME1 , SYSTEM_PAL_RED2 , 0);
	}
	if ( petinfo[BornPetNum].wind > 0 )
	{
		attry1 = BornBaseY + 60 ; 
		attrx2 = attrx1 + petinfo[BornPetNum].wind * 8 ;
		attry2 = attry1 + 8 ; 
		StockBoxDispBuffer( attrx1 , attry1 , attrx2 , attry2 , DISP_PRIO_IME2 , SYSTEM_PAL_YELLOW , 1);
		StockBoxDispBuffer( attrx1+1 , attry1+1 , attrx2+1 , attry2+1 , DISP_PRIO_IME1 , SYSTEM_PAL_YELLOW2 , 0);
		StockBoxDispBuffer( attrx1+2 , attry1+2 , attrx2+2 , attry2+2 , DISP_PRIO_IME1 , SYSTEM_PAL_YELLOW2 , 0);
	}
	
	StockFontBuffer( BornBaseX + 15 , BornBaseY - 33, FONT_PRIO_FRONT, FONT_PAL_WHITE , petinfo[BornPetNum].name , 0 );
	graId[22] = StockDispBuffer( BornBaseX - 105 , BornBaseY + 85 , DISP_PRIO_CHAR, upDownBtnGraNo[0][BornPreBtn], selUseFlag );
	graId[23] = StockDispBuffer( BornBaseX - 30 , BornBaseY + 85 , DISP_PRIO_CHAR, upDownBtnGraNo[1][BornNextBtn], selUseFlag );
	StockDispBuffer( BornBaseX - 3 , BornBaseY - 4  , DISP_PRIO_CHAR , CG_NEWBORNPET_WND , 0 );
	if( pActPet20 == NULL )
	{
		//3~9
		pActPet20 = MakeAnimDisp( BornBaseX - 60 , BornBaseY + 45 , petinfo[BornPetNum].spr_num , 0 );
	}
#endif
	// ??????
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_BG, 0 );

	RunAction();			// ?????????
	StockTaskDispBuffer();	// ???????????????????
	FlashKeyboardCursor();	// ???????????????
	//ImeProc();				// ???????

	return ret;
}




// ????????k??
// ???
void initSelHomeTown( void )
{
	selHomeTownProcNo = 0;
	nowSelHomeTownNo = 0;
}


// ????????k????
//
//  ???k??	0 ... ???
//				1 ... ??e
//				2 ... ?????
int selHomeTown( void )
{
	static int btnId[] = { -2, -2, -2, -2, -2 };
	int id;
	int ret = 0;
	int ret2;
	char *homeTownName[] =
	{
		"萨姆吉尔村",
		"玛丽娜丝村",
		"加加村",
		"卡鲁它那村"
	};
	int selUseFlag = 0;
	int i;

	// ???
	if( selHomeTownProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		selHomeTownProcNo++;
	}

	// ??????k???
	if( selHomeTownProcNo == 1 )
	{
		selUseFlag = 2;
	}

	// ??e??_??
	if( selHomeTownProcNo == 10 )
	{
		ret2 = commonYesNoWindow( 320, 240 );
		// ??
		if( ret2 == 1 )
		{
			ret = 1;
			selHomeTownProcNo = 11;	// ???
		}
		else
		// ???
		if( ret2 == 2 )
		{
			selHomeTownProcNo = 1;
		}
	}

	id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
	// ?????
	if( id == 0 )
	{
		ret = 2;
		play_se( 217, 320, 240 );	// ?????
	}

	if( 1 <= id && id <= 4 )

	{
		nowSelHomeTownNo = id - 1;
		newCharacterHomeTown = nowSelHomeTownNo;
		selHomeTownProcNo = 10;
		play_se( 217, 320, 240 );	// ?????
	}


	if( selHomeTownProcNo == 1 )
	{
		id = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );
	}
	// ??????Z??C??
#ifdef _DELBORNPLACE //Syu ADD 6.0 ?????????????
#else
	if( id == 1 )
	{
		StockFontBuffer( 444, 96,
			FONT_PRIO_BACK, FONT_PAL_YELLOW, homeTownName[0], 0 );

		StockFontBuffer( 386, 156,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "位于萨伊那斯的东边", 0 );
		StockFontBuffer( 386, 176,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "是四个村子中最大", 0 );
		StockFontBuffer( 386, 196,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "也是最整齐的村子", 0 );

	}
	else
	// ??????Z??C??
	if( id == 2 )
	{
		StockFontBuffer( 444, 96,
			FONT_PRIO_BACK, FONT_PAL_YELLOW, homeTownName[1], 0 );
		StockFontBuffer( 390, 166,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "位于萨伊那斯的西边", 0 );
		StockFontBuffer( 390, 186,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "面向美丽海滩的村子", 0 );
	}
	else
	// ??????Z??C??
	if( id == 3 )
	{
		StockFontBuffer( 444, 96,
			FONT_PRIO_BACK, FONT_PAL_YELLOW, homeTownName[2], 0 );

		StockFontBuffer( 390, 156,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "位于加鲁卡的东边", 0 );
		StockFontBuffer( 390, 176,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "四周被森林围绕着", 0 );
		StockFontBuffer( 390, 196,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "绿意盎然的村子", 0 );

	}
	else
	// ???????Z??C??
	if( id == 4 )
	{
		StockFontBuffer( 444, 96,
			FONT_PRIO_BACK, FONT_PAL_YELLOW, homeTownName[3], 0 );

		StockFontBuffer( 390, 156,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "位于加鲁卡的西边", 0 );
		StockFontBuffer( 390, 176,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "村子实在好几个小小", 0 );
		StockFontBuffer( 390, 196,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "的岛上建立起来的", 0 );

	}
	else
	// ??????????C??
	if( id == 0 )
	{
		StockFontBuffer( 390, 176,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "回到前一个画面。", 0 );
	}
	else
	{
		StockFontBuffer( 390, 176,
			FONT_PRIO_BACK, FONT_PAL_BLUE, "请选择出生地。", 0 );
	}

	btnId[0] = StockDispBuffer( 412, 296, DISP_PRIO_CHAR, CG_CHR_MAKE_BACK_BTN, selUseFlag );

	btnId[1] = StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_HOME_NAME0, selUseFlag );
	btnId[2] = StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_HOME_NAME1, selUseFlag );
	btnId[3] = StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_HOME_NAME2, selUseFlag );
	btnId[4] = StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_HOME_NAME3, selUseFlag );

	// ??????
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_CHR_MAKE_SEL2_BG, 0 );
#endif
	return ret;
}




// ??????????
// ???
void initCreateChar( void )
{
	createCharProcNo = 0;
}


// ????????
//  ???k??	0 ... ???
//				1 ... ??
//				2 ... ????
int createChar( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	int ret2;
	char msg[] = "人物制作中...";

	// ???
	if( createCharProcNo == 0 )
	{
		createCharProcNo++;

		// ????????
		w = (strlen( msg )*9+63)/64;
		if( w < 2 )
			w = 2;
		h = 2;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ???????????
	if( createCharProcNo == 1 )
	{
		createNewCharStart();
		createCharProcNo++;
	}
	if( createCharProcNo == 2 )
	{
		ret2 = createNewCharProc();
		// ??
		if( ret2 == 1 )
		{
			ret = 1;
		}
		else
		// ????
		if( ret2 < 0 )
		{
			ret = 2;
		}
	}


	// ?????????????????
	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;

			len = strlen( msg )/2;
			xx = (w*64-len*17)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}

void initCharLogin( void );
int charLogin( void );

void characterLoginProc( void )
{
	int ret;
	static char msg[256];

	if( SubProcNo == 0 ){
		initCharLogin();
		SubProcNo++;
	}
	if( SubProcNo == 1 ){
		ret = charLogin();
		if( ret == 0 ) {
			// ?????
		}
		else if( ret == 1 ){

//andy_reedit initchat			
			InitChat();
			ChangeProc( PROC_GAME );
		}else
		if( ret == -1 )
		{
			SubProcNo = 100;
			strcpy( msg, NET_ERRMSG_LOGINTIMEOUT );
		}
		else
		if( ret == -2 ){
			SubProcNo = 100;
			strcpy( msg, "签入处理失败。(1001)" );
		}


#ifdef _NEW_WGS_MSG				// WON ADD WGS???????

		else if( ret == 101 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "????????????????????" );
		}		
		else if( ret == 102 ){
			SubProcNo = 100;
			strcpy( msg, "???????????????????" );
		}
		else if( ret == 103 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????????,????????????" );
		}		
		else if( ret == 104 ){
			SubProcNo = 100;
			strcpy( msg, "????????????????" );
		}
		else if( ret == 201 ){
			SubProcNo = 100;
			strcpy( msg, "?????????????????????????δ????????????????????????????????" );
		}
		else if( ret == 202 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "???????????????????" );
		}
		else if( ret == 203 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????????????" );
		}
		else if( ret == 204 ){ // 2.1 New  	// ???????????
			SubProcNo = 100;
			//strcpy( msg, "??????????????" );				
			strcpy( msg, "???????????????" );
		}
		else if( ret == 205 ){
			SubProcNo = 100;
			strcpy( msg, "δ????????????????" );
		}
		else if( ret == 206 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "???????????" );
		}
		else if( ret == 207 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "???????????" );
		}
		else if( ret == 208 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????????" );
		}
		else if( ret == 209 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "????????δ??" );
		}
		else if( ret == 210 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????????????????????????????" );
		}
		else if( ret == 211 ){
			SubProcNo = 100;
			strcpy( msg, "??????????, ?????????????????" );
		}
		else if( ret == 212 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "????????????????????????????, ????????" );
		}
		else if( ret == 213 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "???????, ?????????" );
		}
		else if( ret == 214 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????δ????" );
		}
		else if( ret == 220 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "???????????????????" );
		}
		else if( ret == 222 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "?????????????????????????????????, ????????" );
		}
		else if( ret == 301 ){
			SubProcNo = 100;
			strcpy( msg, "??????δ???????????????????????" );
		}
		else if( ret == 402 ){
			SubProcNo = 100;
			strcpy( msg, "WGS???????????????, ????????????????????????????" );
		}
		else if( ret == 405 ){
			SubProcNo = 100;

			strcpy( msg, "WGS????????, ????WGS??????????????????????" );
		}
		else if( ret == 999 ){ // 2.1 New
			SubProcNo = 100;
			strcpy( msg, "??·???????" );
		}
		else if( ret >= 2030 && ret < 2044 ){	// ?????????????
			int star=-1;
			star = ret - 2030;
			SubProcNo = 100;
				
			// ???? cs ?? acid ???????
			switch( star ){
				case 0: strcpy( msg, "?????????????????" );	break;	
				case 1:	strcpy( msg, "????????????????" );	break;
				case 2:	strcpy( msg, "?????????????????" );	break;
				case 3:	strcpy( msg, "????????????????" );	break;
				case 4:	strcpy( msg, "????????????????" );	break;
				case 5:	strcpy( msg, "????????????????" );	break;
				case 6:	strcpy( msg, "?????????????????" );	break;
				case 7:	strcpy( msg, "????????????????" );	break;
				case 8:	strcpy( msg, "??????????????????" );	break;
				case 9:	strcpy( msg, "????????·??????????" );	break;
				case 10: strcpy( msg, "????????????????" );	break;
				case 11: strcpy( msg, "????????????????" );	break;				
				case 12: strcpy( msg, "????????????????" );	break;
				case 13: strcpy( msg, "????????????????" );	break;
				default: strcpy( msg, "?????????(1002)" );		break;
			}
		}

		else {
			SubProcNo = 100;
			sprintf( msg, "????????(%d)??", ret );
		}
#endif

#ifdef _DEBUG
		char msg2[1024];
		sprintf( msg2, "%d %s", ret, msg);
		strncpy( msg, msg2, sizeof(msg) );
#endif

	}
	if( SubProcNo == 100 ){
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 ){
		if( commonMsgWin( msg ) ){
			// ???????????
			ChangeProc( PROC_TITLE_MENU );
		}
	}
	RunAction();
	StockTaskDispBuffer();
}


static short charLoginProcNo = 0;

// ???
void initCharLogin( void )
{
	charLoginProcNo = 0;
}

// ?????
//
//  ???k??	 0 ... ?????
//				 1 ... ??????
//				-1 ... ??????
//				-2 ... ???
int charLogin( void )
{
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	static char msg[] = "签入中";

	// ???
	if( charLoginProcNo == 0 )
	{
		charLoginProcNo = 1;

		// ????????
		w = strlen( msg )*9/64+2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}

	// ??????????
	if( charLoginProcNo == 1 )
	{
		charLoginStart();
		charLoginProcNo++;
	}
	else
	if( charLoginProcNo == 2 )
	{
		ret = charLoginProc();
	}

	if( ret != 0 )
	{
		if( ptActMenuWin )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}

	if( ptActMenuWin != NULL )
	{
		// ?????????
		if( ptActMenuWin->hp >= 1 )
		{
			int len;
			int xx, yy;

			len = strlen( msg );
			xx = (w*64-len*8)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}




static int produce_logout(void);
static int produce_vct_no = 0;

///////////////////////////////////////////////////////////////////////////
//
// ????????
//

void initCharLogout( void );
int charLogout( void );

void characterLogoutProc( void )
{
	int ret;
	static char msg[256];

	if( SubProcNo == 0 ){
		logOutFlag = true;

		if(!produce_logout()){		//??????
			return;
		}
#ifdef __SKYISLAND
		extern void SkyIslandRelease();
		SkyIslandRelease();
#endif
		//????k????
		produce_vct_no = 0;

#ifdef _AniRandom   // Syu ADD ???????????????
		extern void AniRandomRelease();
		AniRandomRelease();
#endif
#ifdef _AniCrossFrame	   // Syu ADD ???????ι?????????
		extern void crossAniRelease();
		crossAniRelease();
#endif
#ifdef _AniCharBubble	   // Syu ADD ?????????????????
		extern void CharbubbleRelease();
		CharbubbleRelease();
#endif
#ifdef _AniImmobile	 // Syu ADD ??????????????
		extern void ImmobileAniRelease();
		ImmobileAniRelease();
#endif
#ifdef _SPECIALSPACEANIM	// Syu ADD ??????????????
		extern void ReleaseSpecAnim();
		ReleaseSpecAnim();
#endif
		// PC????
		resetPc();

		// ?????????????
		initCharObj();

		// ????????
		DeathAllAction();

		// ???????
//		fade_out_bgm();

#ifdef _DEBUG
		if( !offlineFlag )
		{
			SubProcNo++;
		}
		else
		{
			// ????????? WM_CLOSE ??????????
			//PostMessage( hWnd, WM_CLOSE, 0, 0L );
		}
#else
		SubProcNo++;
#endif
	}

	if( SubProcNo == 1 )
	{
		initCharLogout();
		SubProcNo++;
	}
	else
	if( SubProcNo == 2 ){
		ret = charLogout();
		if( ret == 1 ){
			cleanupNetwork();
			DeathAllAction();
			//cary
			ChangeProc( PROC_ID_PASSWORD );
			SubProcNo=0;
		}
		else
		// ??????
		if( ret == -1 )
		{
			// ?????????
			cleanupNetwork();
			SubProcNo = 100;
			strcpy( msg, NET_ERRMSG_LOGOUTTIMEOUT );
		}
		else
		// ???
		if( ret == -2 )
		{
			// ?????????
			cleanupNetwork();
			SubProcNo = 100;
			strcpy( msg, "签出处理失败。" );
		}
	}

	// ???????
	if( SubProcNo == 100 )
	{
		initCommonMsgWin();
		SubProcNo++;
	}
	if( SubProcNo == 101 )
	{
		if( commonMsgWin( msg ) )
		{
			// ???????????
			//cary
			ChangeProc( PROC_ID_PASSWORD );	// ?????
			SubProcNo=0;
		}
	}

	RunAction();			// ?????????
	StockTaskDispBuffer();	// ???????????????????
}


static int charLogoutProcNo;

// ??????????
void initCharLogout( void )
{
	charLogoutProcNo = 0;
}


// ???????
//  ???k??	 0 ... ??????
//				 1 ... ????????
//				-1 ... ??????
//				-2 ... ???
int charLogout( void )
{
	extern unsigned int  systemWndNo;						// ??????k?
	static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h;
	int ret = 0;
	static char msg[] = "签出中";

	if( systemWndNo != 1){
#ifdef _CHAR_NEWLOGOUT
		lssproto_CharLogout_send( sockfd, 0);
#endif
		return 1;
	}
	if( charLogoutProcNo == 0 ){
		charLogoutProcNo = 1;
		w = strlen( msg )*9/64+2;
		h = (16+47)/48;
		if( h < 2 )
			h = 2;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
	}
	if( charLogoutProcNo == 1 ){
		charLogoutStart();
		charLogoutProcNo++;
	}else
		if( charLogoutProcNo == 2 ){
			ret = charLogoutProc();
		}

	if( ret != 0 ){
		if( ptActMenuWin ){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}
	if( ptActMenuWin != NULL ){
		if( ptActMenuWin->hp >= 1 ){
			int len;
			int xx, yy;
			len = strlen( msg );
			xx = (w*64-len*8)/2;
			yy = (h*48-16)/2;
			StockFontBuffer( x+xx, y+yy, FONT_PRIO_FRONT, FONT_PAL_WHITE, msg, 0 );
		}
	}

	return ret;
}



























///////////////////////////////////////////////////////////////////////////
// ?????????[??????
//
//   ?????:	?S?40??? ? 8?
//
// ????????????????openServerWindowProc?????
int windowTypeWN = -1;	// ????????  -1??t???
int buttonTypeWN = 0;	// ??????(??????)
int indexWN;			// ??????k?
int idWN;				// ??????
// shan begin
//char msgWN[25][70];		// ??????????
char msgWN[25][100];
//shan end
int msgWNLen;			// ????????????????
int msgWN_W;			// ????????????
int msgWN_H;			// ?????????????
ACTION *ptActMenuWin = NULL;

int selStartLine;		// ?k????????????????k???????
int wnOpenX, wnOpenY;	// ?[????????????????????
short wnCloseFlag = 0;	// ?[??????????????


#define MAX_SHOP_ITEM	8	// ??????????????????????
#define MAX_SHOP_PAGE	4	// ?????????????????????
short shopWindowMode;		// ??????????????0...?c????,1...???????
char shopWindow1Title[27];	// ??????????????
char shopWindow1Msg[27];	// ???????????????

bool isShopItemHover = false;
typedef struct
{
	int level;
	int price;
	int graNo;
	short sealFlag;
	short tbl;
	char name[ITEM_NAME_LEN+1];
	char info[3][29];
	int num;
#ifdef _NEW_MANOR_LAW
	int costfame;
#endif
} ITEM_SHOP;

int menuIndexWN;									// ??????k?(???????????k?)
char shopWindow2Msg[2][45];							// ???????????????
short shopWondow2Page, shopWondow2MaxPage;			// ????????????
ITEM_SHOP sealItem[MAX_SHOP_ITEM*MAX_SHOP_PAGE];	// ?c?????
short selShopItemNo;								// ?k????????

char shopWindow3Msg[41];
short sealItemCnt;

char shopWindow4Msg[2][39];							// ???????????????
char shopWindow5Msg[2][39];							// ???????????????
char shopWindow6Msg[2][39];							// ???????????????

short nowUserItemCnt;
char shopWindow7Msg[2][39];							// ???????????????
char shopWindow8Msg[2][39];							// ???????????????


// Shan Add  (Begin)
int    FMmsgWN[16];
int    FMelderFlag[16];
int    FMpidWN[16]; 
int    FMdengonidex;
char   FMnameWN[11][20];
time_t FMHoldTime;
// end

#ifdef _BLACK_MARKET
struct BlackMarketItem{
	int  iIndex;
	char iName[128];
	int  iGraphicsNum;	
};
struct BlackMarketItem BMItem[12];
int sItem=-1;
#endif

ITEM_SHOP userItem[MAX_SHOP_ITEM*MAX_SHOP_PAGE];	// ?????????????

short buyOffFlag;									// ???M???


#define MAX_SKILL_SHOP_SKILL	8					// ?????????????????????
// Robin fix ???????
//#define MAX_SKILL_SHOP_PAGE		2					// ??????????????
#define MAX_SKILL_SHOP_PAGE		10						// ??????????????

typedef struct
{
	int price;
	int icon;
	char name[SKILL_NAME_LEN+1];
	char info[3][25];
} SKILL_SHOP;

SKILL_SHOP sealSkill[MAX_SKILL_SHOP_SKILL*MAX_SKILL_SHOP_PAGE];


short skillShopWindowProcNo;
short selShopSkillNo;
short selShopSkillPetNo;
short selShopSkillSlotNo;


#define MAX_POOL_SHOP_ITEM		8					// ??????????????????

#ifdef _ADD_POOL_ITEM			   // WON ADD ?????????
//#define MAX_POOL_SHOP_PAGE		10
#define MAX_POOL_SHOP_PAGE		13
#else
#define MAX_POOL_SHOP_PAGE		3					// ???????
#endif

typedef struct
{
	int level;
	int price;
	int graNo;
	short poolFlag;
	short tbl;
	char name[ITEM_NAME_LEN+1];
	char info[3][29];
#ifdef _ITEM_PILENUMS
	int num;
#endif
} POOL_SHOP;

int restPoolSlot;		// ????????????????

POOL_SHOP poolItem[MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE];


void initServerWindowType0( char * );
void initServerWindowType1( char * );
void initServerWindowType2( char * );
void initServerWindowType3( char * );
void initServerWindowType4( char * );
void initServerWindowType5( char * );
void initServerWindowType6( char * );
void initServerWindowType7( char * );
void serverWindowType0( int );
void serverWindowType1( void );
void serverWindowType2( void );
void serverWindowType3( void );
void serverWindowType4( void );
void serverWindowType5( void );
void serverWindowType6( void );
void serverWindowType7( void );
void serverWindowType9( void );

#ifdef _CHAR_PROFESSION			// WON ADD ??????????
void initServerWindowProfession( char *data );
void profession_windows();
int profession_windows_1();
int profession_windows_2();
char ProfessionShopTitle[27];
char ProfessionShopMsg[2][45];
#endif

#ifdef _NPC_WELFARE_2				// WON ADD ??NPC-2
void initServerWindowProfession2( char *data );
void profession_windows2();
int profession_windows_12();
int profession_windows_22();
char ProfessionShopTitle2[27];
char ProfessionShopMsg2[2][45];
#endif

#ifdef _PETSKILL_CANNEDFOOD
void initPetSkillWindowType1( char *date);
void PetSkillShowType1( void );
#endif

void initFamilyAddWN( char * );
void familyAddWN( void );
void initFamilyList( char * );
void familyListWN( void );
void initFamilyDetail( char * );
int familyDetailWN( void );
void initMineFamilyDetail( char * );
void mineFamilyDetailWN( void );

void initItemman( char * );
int itemmanWN( void );

void initFamilyTaxWN( char * );
void familyTaxWN( void );

void initFamilyLeaderChange( char * );
void familyLeaderChangeWN( void );
void familyLeaderChangeQWN( void );
void familyLeaderChangeAWN( void );
void initFamilyLeaderChangeA( char * );

#ifdef _BLACK_MARKET
void initBlackMarket( char * );
void BMWindowType( void );
#endif

#ifdef _ADD_FAMILY_TAX			   // WON ADD ?????????
void FMTAXWindowsType( void );
#endif

// Shan Add
void initFMWindowType( char * );
void FMWindowType( void );
void initFMWindowType1( char * );
void FMWindowType1( void );
void initFMWindowType2( char * );
void FMWindowType2( void );
void initFMWindowType3( char * );
#ifdef _NEW_MANOR_LAW
void FMWindowType3(int king);
#else
void FMWindowType3( void );
#endif
#ifdef _FMVER21
void initFMWindowType4( char * ,int king);
#ifdef _NEW_MANOR_LAW
void FMWindowType4(int king);
#else
void FMWindowType4( void );
#endif
#endif
// Robin Family PK
void initFMPKListWN( char * );
void FMPKListWN( int mode );
void initFMPKSelectWN( char * );
void FMPKSelectWN( int mode );
void initFMPKDetailWN( char * );
void FMPKDetailWN( int mode );

void initShowRidePetWN( void );
void showRidePetWN( void );


#ifdef _NPC_SELLSTH
void InitSellSth_Menu( char* data );
void InitSellSth_View( char* data );

void InitSellSth_Sell( char* data );
//void SellSth_Menu( void );
void SellSth_Sell( void );
#endif

#ifdef _MOUSE_DBL_CLICK
void initMouseGetNameWN( char* data);
void showMouseGetNameWN( void);
#endif

#ifdef _CONTRACT
void initContractWN( char* data);
void showContractWN();
#endif

#ifdef _RACEMAN
void initRacemanRankWN( char* data);
void showRacemanRankWN( void );
#endif


// ???
void initOpenServerWindow( void )
{
	windowTypeWN = -1;
	ptActMenuWin = NULL;
}

// Robin 0720 Login Announce
int LoginAnnounce = 0;
char AnnounceBuf[8192];


void openServerWindow( int windowtype, int buttontype, int index, int id, char *data )
{

#ifdef _ANGEL_SUMMON
	if( windowtype == WINDOW_MESSAGETYPE_ANGELMESSAGE ) {
		if( index == 520 /*CHAR_WINDOWTYPE_ANGEL_ASK*/ ) {
			angelFlag = true;
			angelRecvType = index;
			angelRecvButton = buttontype; 
			strcpy( angelMsg, data);
			return;
		}
		else if( index = 521 /*CHAR_WINDOWTYPE_ANGEL_CLEAN*/) {
			angelFlag = false;
			strcpy( angelMsg, "");
			return;
		}
	}
#endif

	if( windowTypeWN < 0 || windowtype == 33 || windowtype == 0 || windowtype == 36
		|| windowtype == 39 || windowtype == 42 ){

		if( windowtype==0 && windowTypeWN==13){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
#ifdef _BLACK_MARKET
		if( windowtype==0 && windowTypeWN==WINDOW_MESSAGETYPE_BLACKMARKET){
			closeBlackMarketWN();
		}
#endif
		wnOpenX = nowGx;
		wnOpenY = nowGy;
		wnCloseFlag = 0;

		windowTypeWN = windowtype;
		buttonTypeWN = buttontype;
		indexWN = index;
		idWN = id;

		buyOffFlag = 0;

		// ???????
		if( windowTypeWN == WINDOW_MESSAGETYPE_MESSAGE
			|| windowTypeWN == WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT 
#ifdef _NEW_MANOR_LAW
			|| windowTypeWN == WINDOW_FMMESSAGETYPE_MANOR_SCHEDULE	// ?????????
#endif
		 )
		{
			// ???
			// ???????
			// ?????????V?
			initServerWindowType0( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_SELECT )
		{
			// ?k??
			initServerWindowType1( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMENU )
		{
			// ?????M??????
			initServerWindowType2( data );
		}else if( windowTypeWN == WINDOW_MESSAGETYPE_ITEMSHOPMAIN ){
			initServerWindowType3( data );
		}
		else
		// ???M??????
		if( windowTypeWN == WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN )
		{
			// ?????M??????
			initServerWindowType3( data );
			buyOffFlag = 1;
		}
		else
		// ?????????????
		if( windowTypeWN == WINDOW_MESSAGETYPE_PETSKILLSHOP )
		{
			initServerWindowType5( data );
		}
		else
#ifdef _CHAR_PROFESSION			// WON ADD ??????????
		if( windowTypeWN == WINDOW_MESSAGETYPE_PROFESSIONSHOP )
		{
			initServerWindowProfession( data );
		}else
#endif
#ifdef _NPC_WELFARE_2				// WON ADD ??NPC-2
		if( windowTypeWN == WINDOW_MESSAGETYPE_PROFESSIONSHOP2 )
		{
			initServerWindowProfession2( data );
		}else
#endif
#ifdef _PETSKILL_CANNEDFOOD
		if( windowTypeWN == WINDOWS_MESSAGETYPE_PETSKILLSHOW ){
			initPetSkillWindowType1( data );
		}else
#endif
		// ????Y???????????
		if( windowTypeWN == WINDOW_MESSAGETYPE_WIDEMESSAGE
		 || windowTypeWN == WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT )
		{
			// ???
			// ???????
			// ?????????V?
			initServerWindowType4( data );
		}
		else
		// ????????????????
		if( windowTypeWN == WINDOW_MESSAGETYPE_POOLITEMSHOPMAIN ){
			initServerWindowType6( data );
		}
		else
		// ????????????????????
		if( windowTypeWN == WINDOW_MESSAGETYPE_POOLITEMSHOPMENU )
		{
			initServerWindowType7( data );
		}
		else
		// Robin 0518 Family
		if( windowTypeWN == WINDOW_MESSAGETYPE_FAMILYADD )
		{
			initFamilyAddWN( data );
		}

#ifdef _BLACK_MARKET
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_BLACKMARKET )
		{
			initBlackMarket( data );
		}    
#endif
#ifdef _ADD_FAMILY_TAX			   // WON ADD ?????????
		else
		if( windowTypeWN == WINDOWS_MESSAGETYPE_FAMILY_TAX )
		{
			initServerWindowType0( data );
		}  
#endif
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_FAMILYJOIN )
		{
			initFamilyList( data );
		}

		if( windowTypeWN == WINDOW_MESSAGETYPE_FAMILYDETAIL )
		{
			initMineFamilyDetail( data );
		}

        // Shan Family Dengon
		// Member List
		else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_SELECT )
		{
			initFMWindowType( data );
		}
#ifdef _FMVER21
		// TOP 30 DP List
		else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_TOP30DP )
		{
			initFMWindowType4(data,0);
		}
#endif
#ifdef _NEW_MANOR_LAW
		else if(windowTypeWN == WINDOW_FMMESSAGETYPE_FM_MEMONTUM) initFMWindowType4(data,1);
#endif
		// Dp List
	  else
#ifdef _NEW_MANOR_LAW
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_DP || windowTypeWN == WINDOW_FMMESSAGETYPE_10_MEMONTUM)
#else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_DP )
#endif
		{
			initFMWindowType3( data );
		}
		// Point List
		else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_POINTLIST )
		{
			initFMWindowType2( data );
		}
		// Family Memo
        else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_DENGON )
		{
			initFMWindowType1( data );
		}
        // Familys Memo
		else
		if( windowTypeWN == WINDOW_FMMESSAGETYPE_FMSDENGON )
		{
			initFMWindowType1( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_PKSCHEDULELIST )
		{
			initFMPKListWN( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_PKSCHEDULESELECTFAMILY )
		{
			initFMPKSelectWN( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_PKSCHEDULEDETAIL )
		{
			initFMPKDetailWN( data );
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_SHOWRIDEPET )
		{
			initShowRidePetWN();
		}
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_LOGINMESSAGE )
		{
			windowTypeWN = -1;
			LoginAnnounce = 1;
			strcpy( AnnounceBuf, data );
			//windowTypeWN = WINDOW_MESSAGETYPE_MESSAGE;
			//initServerWindowType0( data );
		}else if( windowTypeWN == WINDOW_MESSAGETYPE_LEADERSELECT ){
			initFamilyLeaderChange( data );
		}else if( windowTypeWN == WINDOW_MESSAGETYPE_LEADERSELECTA ){
			initFamilyLeaderChangeA( data );
		}
#ifdef _NPC_SELLSTH
		if( windowTypeWN == WINDOWS_MESSAGETYPE_SELLSTHMENU ){
			InitSellSth_Menu( data);
		}else if( windowTypeWN == WINDOWS_MESSAGETYPE_SELLSTHVIEW ){
			InitSellSth_View( data);
		}else if( windowTypeWN == WINDOWS_MESSAGETYPE_SELLSTHSELL ){
			InitSellSth_Sell( data);
		}
#endif

// Terry add 2001/12/06
// ??????--????????
#ifdef __EDEN_EFFECT
		else if(windowTypeWN == WINDOW_MESSAGETYPE_BANK)
		{
			// ?趨???????????д??
			pc.personal_bankgold = atoi(data);
			if(pc.personal_bankgold < 0)			          					pc.personal_bankgold = 0;
			else if(pc.personal_bankgold > MAX_PERSONAL_BANKGOLD) pc.personal_bankgold = MAX_PERSONAL_BANKGOLD;
		}
#endif
// Terry end
// Terry add 2002/01/03
#ifdef __EDEN_AUCTION
		else if(windowTypeWN >= WINDOW_MESSAGETYPE_AUCTIONNEW && 
			      windowTypeWN <= WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY)
		{
			switch(windowTypeWN)
			{
				// ??????????
			case WINDOW_MESSAGETYPE_AUCTIONNEW:
				break;
				// ????????б????
			case WINDOW_MESSAGETYPE_AUCTIONLIST_BUY:
			case WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY:
				szpALD = data;
				//szpALD = TESTDATA;
				bNewData = true;
				break;
				// ?????????????????
			case WINDOW_MESSAGETYPE_AUCTIONSURVEY:
				// ?????????????????
			case WINDOW_MESSAGETYPE_AUCTIONMODIFY:
				szpALD = data;
				break;
			}
		}
#endif
// Terry end

#ifdef _MOUSE_DBL_CLICK
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_MOUSEGETNAME )
		{
			initMouseGetNameWN( data);
		}
#endif

#ifdef _CONTRACT
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_CONTRACT )
		{
			initContractWN( data);
		}
#endif

#ifdef _RACEMAN
		else
		if( windowTypeWN == WINDOW_MESSAGETYPE_RACEMAN_RANK )
		{
			initRacemanRankWN( data );
		}
#endif


		if( CheckMenuFlag() )
		{
			InitMenu2();
		}

		closeEtcSwitch();
		closeCharActionAnimeChange();
		closeJoinChannelWN();
	}
}


// ????????????????????
// ?????????
void openServerWindowProc( void )
{
#if 0
	// 
	if( joy_trg[ 1 ] & JOY_F6 )
	{
#if 1
		openServerWindow( 9, 0x3c, 0, 0,
		"1|???????????????|???????????????????????|"
		"????????|500|?????????????????????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		"???????|1000|???????????????????|"
		);
#endif
#if 0
		openServerWindow( 1, 0x3c, 0, 0,
			"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n"
			"BBBBBBBBBBBBBBBB\n"
			"CCCC\n"
			"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n"
			"EE\n"
			"FFFFF\n"
			"GGGGGGGGGGGG\n"
			"H\n"
			"IIIIIIIIIIIIIIIIIIIIIIIII\n"
			"JJJJJJJJJJJ\n"
			"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n"
			"LLLLLLLLLLLLLLL\n"
			"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n"
			"BBBBBBBBBBBBBBBB\n"
			"CCCC\n"
			"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n"
			"EE\n"
			"FFFFF\n"
			"GGGGGGGGGGGG\n"
			"H\n"
			"IIIIIIIIIIIIIIIIIIIIIIIII\n"
			"JJJJJJJJJJJ\n"
			"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n"
			"LLLLLLLLLLLLLLL\n"	 );
#endif
#if 0
		openServerWindow( 7, 0x3c, 0, 0,
			"1|1|0|????????|?????????????|??????????????????|"
			"?????????????????|????????|"
			"???S|0|30|24000|???S???????|"
			"?t??|0|100|24001|?t?????????|"
			"????|0|200|24001|???????????|"
			"?????|0|300|24001|?t?????????|"
			"????|0|50|24001|???????????|"
			"??????|0|100|24001|???????????|"
			"?~??|0|100|24001|?~?????????|"
			"?~??|0|100|24001|?~?????????|"
			"?~??|0|100|24001|?~?????????|"
			"??????|0|100|24001|??????????????????????\n???????????" );
#endif
#if 0
		openServerWindow( 7, 0x3c, 0, 0,
			"0|1|0|????????|?????????????|???????????|"
			"????????|???????????????K?????\n??????|?o???????|"
			"???????????????|"
			"???S|0|1|30|24000|???S???????|"
			"?t??|0|3|100|24001|?t?????????|"
			"????|0|5|200|24001|???????????|"
			"?????|0|10|300|24001|?t?????????|"
			"????|0|1|10|24001|???????????|"
			"??????|0|2|100|24001|???????????|"
			"?~??|0|3|100|24001|?~?????????|"
			"?~??|0|4|100|24001|?~?????????|"
			"?~??|0|1|100|24001|?~?????????|"
			"??????|1|100|9999999|24001|??????????????????????\n???????????" );
#endif
	}
#endif

	isShopItemHover = false;
	// Robin 0720 Login Announce
	if( LoginAnnounce )
	{
		LoginAnnounce = 0;
		lssproto_WN_recv( sockfd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OK
						, -1, -1, AnnounceBuf);
	}

	// ?????@?e????????????
	if( windowTypeWN < 0 )
		return;

	// ??????????t?????????????????????
	if( ABS( wnOpenX - nowGx ) >= 2
	 || ABS( wnOpenY - nowGy ) >= 2 )
	{
		wnCloseFlag = 1;	// ?[???????????
	}
	switch( windowTypeWN )
	{
		// ???????
		case WINDOW_MESSAGETYPE_MESSAGE:
		case WINDOW_MESSAGETYPE_WIDEMESSAGE:
			serverWindowType0( 0 );
			break;

		// ?????????V?
		case WINDOW_MESSAGETYPE_MESSAGEANDLINEINPUT:
		case WINDOW_MESSAGETYPE_WIDEMESSAGEANDLINEINPUT:
			serverWindowType0( 1 );
			break;
#ifdef _NEW_MANOR_LAW
		case WINDOW_FMMESSAGETYPE_MANOR_SCHEDULE:	// ?????????
			serverWindowType0( 2 );
			break;
#endif
		// ?k??
		case WINDOW_MESSAGETYPE_SELECT:
			serverWindowType1();
			break;

		// ????k??
		case WINDOW_MESSAGETYPE_PETSELECT:
			serverWindowType2();
			break;
#ifdef _NPC_FUSION
		case WINDOWS_MESSAGETYPE_PETFUSION://andy_add ???????
			serverWindowType9();
			break;
#endif
		// ???k??
		case WINDOW_MESSAGETYPE_PARTYSELECT:
			serverWindowType3();
			break;

		// ???????k??
		case WINDOW_MESSAGETYPE_PETANDPARTYSELECT:
			serverWindowType4();
			break;

		// ?????M??????
		case WINDOW_MESSAGETYPE_ITEMSHOPMENU:
		case WINDOW_MESSAGETYPE_ITEMSHOPMAIN:
		case WINDOW_MESSAGETYPE_LIMITITEMSHOPMAIN:
			isShopItemHover = true;
			serverWindowType5();
			break;

		// ??????????
		case WINDOW_MESSAGETYPE_PETSKILLSHOP:
			isShopItemHover = true;
			serverWindowType6();
			break;

#ifdef _CHAR_PROFESSION			// WON ADD ??????????
		case WINDOW_MESSAGETYPE_PROFESSIONSHOP:
			isShopItemHover = true;
			profession_windows();
			break;
#endif
#ifdef _NPC_WELFARE_2				// WON ADD ??NPC-2
		case WINDOW_MESSAGETYPE_PROFESSIONSHOP2:
			isShopItemHover = true;
			profession_windows2();
			break;
#endif

#ifdef _PETSKILL_CANNEDFOOD
		case WINDOWS_MESSAGETYPE_PETSKILLSHOW:
			PetSkillShowType1();
			break;

#endif
		// ????????????????
		case WINDOW_MESSAGETYPE_POOLITEMSHOPMENU:
		case WINDOW_MESSAGETYPE_POOLITEMSHOPMAIN:
			isShopItemHover = true;
			serverWindowType7();
			break;

		// Robin 0517 family:
		case WINDOW_MESSAGETYPE_FAMILYADD:
			isShopItemHover = true;
			familyAddWN();
			break;

		case WINDOW_MESSAGETYPE_FAMILYJOIN:
			isShopItemHover = true;
			familyListWN();
			break;

#ifdef _NPC_SELLSTH
		case WINDOWS_MESSAGETYPE_SELLSTHMENU:
//			SellSth_Menu();
		case WINDOWS_MESSAGETYPE_SELLSTHVIEW:
		case WINDOWS_MESSAGETYPE_SELLSTHSELL:
			isShopItemHover = true;
			SellSth_Sell();
			break;
#endif
		case WINDOW_MESSAGETYPE_FAMILYDETAIL:
			isShopItemHover = true;
			mineFamilyDetailWN();
			break;

		/*
		case WINDOW_MESSAGETYPE_ITEMMAN:
			itemmanWN();
			break;
        */	
#ifdef _ADD_FAMILY_TAX			   // WON ADD ?????????			
		case WINDOWS_MESSAGETYPE_FAMILY_TAX:
			isShopItemHover = true;
			FMTAXWindowsType();
			break;
#endif
#ifdef _BLACK_MARKET
        case WINDOW_MESSAGETYPE_BLACKMARKET:
			isShopItemHover = true;		
			BMWindowType();
			break;
#endif
		// Shan family memberlist
		case WINDOW_FMMESSAGETYPE_SELECT:
			FMWindowType();
			break;		
#ifdef _FMVER21
			// Shan family dplist top30
#ifdef _NEW_MANOR_LAW
		case WINDOW_FMMESSAGETYPE_FM_MEMONTUM:
			FMWindowType4(1);
			break;
    case WINDOW_FMMESSAGETYPE_TOP30DP:
			FMWindowType4(0);
			break;
#else
		case WINDOW_FMMESSAGETYPE_TOP30DP:
			FMWindowType4();
			break;
#endif
#endif
		// Shan family dplist
#ifdef _NEW_MANOR_LAW
		case WINDOW_FMMESSAGETYPE_10_MEMONTUM:
			FMWindowType3(1);
			break;
		case WINDOW_FMMESSAGETYPE_DP:
			FMWindowType3(0);
			break;
#else
		case WINDOW_FMMESSAGETYPE_DP:
			FMWindowType3();
			break;
#endif
		// Shan family pointlist
	    case WINDOW_FMMESSAGETYPE_POINTLIST:
			FMWindowType2();
			break;	     
	    // Shan family dengon
		case WINDOW_FMMESSAGETYPE_DENGON:
        case WINDOW_FMMESSAGETYPE_FMSDENGON:
			FMWindowType1();
			break;

		// Robin FM_PK
        case WINDOW_MESSAGETYPE_PKSCHEDULELIST:
			FMPKListWN(0);
			break;
		case WINDOW_MESSAGETYPE_PKSCHEDULESELECTFAMILY:
			FMPKSelectWN(0);
			break;
        case WINDOW_MESSAGETYPE_PKSCHEDULEDETAIL:
			FMPKDetailWN(0);
			break;

        case WINDOW_MESSAGETYPE_FAMILYTAX:
			familyTaxWN();
			break;

        case WINDOW_MESSAGETYPE_SHOWRIDEPET:
			showRidePetWN();
			break;

        case WINDOW_MESSAGETYPE_LEADERSELECT:
			familyLeaderChangeWN();
			break;

        case WINDOW_MESSAGETYPE_LEADERSELECTQ:
			familyLeaderChangeQWN();
			break;

        case WINDOW_MESSAGETYPE_LEADERSELECTA:
			familyLeaderChangeAWN();
			break;
// Terry 2001/12/07
#ifdef __EDEN_EFFECT
		case WINDOW_MESSAGETYPE_BANK:
			BankProc();
			break;
#endif
// Terry end

// Terry add 2002/01/03
#ifdef __EDEN_AUCTION
		// WT-->(W)indow(T)ype
		// ??????????
		case WINDOW_MESSAGETYPE_AUCTIONNEW: AuctionNewWT(); break;
		// ????????б????
		case WINDOW_MESSAGETYPE_AUCTIONLIST_BUY:
		case WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY: AuctionListWT(); break;
		// ?????????????????
		case WINDOW_MESSAGETYPE_AUCTIONSURVEY:
		// ?????????????????
		case WINDOW_MESSAGETYPE_AUCTIONMODIFY: AuctionSurveyModifyWT(); break;
			break;
#endif
// Terry end

#ifdef _MOUSE_DBL_CLICK
		case WINDOW_MESSAGETYPE_MOUSEGETNAME:
			showMouseGetNameWN();
			break;
#endif

#ifdef _CONTRACT
		case WINDOW_MESSAGETYPE_CONTRACT:
			showContractWN();
			break;
#endif

#ifdef _RACEMAN
		case WINDOW_MESSAGETYPE_RACEMAN_RANK:
			showRacemanRankWN();
			break;
#endif



	}
}


// ??????????????????????
//   ????????
//   ????????????????????????????
//   ???????]???????
//  ???k?? dist ... ???I????????(2?????)
//           src  ... ??????????
//           distSize ... dist??
//           line ... ????????
//           strLen ... ?????????????
void getStrSplit( char *dist, char *src, int distSize, int line, int strLen )
{
	int i, j;
	int flag;
	char msg[256];
	char *ptMsg;
	char *dis;

	dis = dist;
	if( distSize-1 < strLen )
	{
		strLen = distSize-1;
	}

	for( i = 0, j = 0; i < line && j < line; i++, j++, dis += distSize )
	{
		// 1???????
		flag = getStringToken( src, '\n', i+1, sizeof( msg )-1, msg );

		ptMsg = msg;
		while( 1 )
		{
			// ?????strLen??????????
			if( strlen( ptMsg ) > (unsigned int)strLen )
			{
				strncpy( dis, ptMsg, strLen );
				if( GetStrLastByte( dis ) != 3 )
				{
					dis[strLen] = '\0';
					ptMsg += strLen;
				}
				else
				{
					dis[strLen-1] = '\0';
					ptMsg += (strLen-1);
				}
				j++;
				dis += distSize;
				if( j >= line )
					break;
			}
			else
			// ???????????????
			{
				strcpy( dis, ptMsg );
				break;
			}
		}

		if( flag == 1 )
		{
			j++;
			dis += distSize;
			break;
		}
	}
	// ????????????NULL??V???
	for( ; j < line; j++, dis += distSize )
		dis[0] = '\0';
}


// ????????
//  ??????????????????????
void initServerWindowType0( char *data )
{
	msgWN_W = 7;
	msgWN_H = 5;
	msgWNLen = 40;

	makeStringFromEscaped( data );
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), msgWNLen );
}


// ??????k?????????
//  ??????????????????????
void initServerWindowType1( char *data )
{
	makeStringFromEscaped( data );
	if( '0' <= *data && *data <= '7' )
	{
		selStartLine = getIntegerToken( data, '\n', 1 );
		data += 2;
	}
	else
	{
		selStartLine = 1;
	}
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 40 );
}



// ???????

short shopWindowProcNo;

// ?????????????
void initServerWindowType2( char *data )
{
	char msg[256];

	shopWindowProcNo = 0;

	getStringToken( data, '|', 1, sizeof( msg )-1, msg );
	makeStringFromEscaped( msg );
	strncpy( shopWindow1Title, msg, sizeof( shopWindow1Title )-1 );
	shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';

	getStringToken( data, '|', 2, sizeof( msg )-1, msg );
	makeStringFromEscaped( msg );
	strncpy( shopWindow1Msg, msg, sizeof( shopWindow1Msg )-1 );
	shopWindow1Msg[sizeof( shopWindow1Msg )-1] = '\0';
#ifdef _NEW_MANOR_LAW
	getStringToken(data,'|',3,sizeof(msg) - 1,msg);
	pc.fame = atoi(msg);
#endif

	menuIndexWN = indexWN;
}

void initServerWindowType3( char *data )
{
	char title1[128];
	char msg1[128];
	char msg2[256];
	char msg3[128];
	char msg4[128];
	char msg5[128];
	char msg6[128];
	char itemName[64];
	char info[256];
	int i, j;
	int flag;
	int datalen;
#ifdef _NEW_MANOR_LAW
	int offset = 7;
#else
	int offset = 6;
#endif

	datalen = 6;

	shopWindowMode = getIntegerToken( data, '|', 1 );
	if( shopWindowMode == 0 ){
		shopWindowProcNo = 10;
		shopWondow2Page = 0;

		if( getIntegerToken( data, '|', 2 ) == 0 )
			return;

		menuIndexWN = getIntegerToken( data, '|', 3 );

		getStringToken( data, '|', 4, sizeof( title1 )-1, title1 );
		makeStringFromEscaped( title1 );
		getStringToken( data, '|', 5, sizeof( msg1 )-1, msg1 );
		makeStringFromEscaped( msg1 );
		getStringToken( data, '|', 6, sizeof( msg2 )-1, msg2 );
		makeStringFromEscaped( msg2 );
		getStringToken( data, '|', 7, sizeof( msg3 )-1, msg3 );
		makeStringFromEscaped( msg3 );
		getStringToken( data, '|', 8, sizeof( msg4 )-1, msg4 );
		makeStringFromEscaped( msg4 );
		getStringToken( data, '|', 9, sizeof( msg5 )-1, msg5 );
		makeStringFromEscaped( msg5 );
		getStringToken( data, '|', 10, sizeof( msg6 )-1, msg6 );
		makeStringFromEscaped( msg6 );

		strncpy( shopWindow1Title, title1, sizeof( shopWindow1Title )-1 );
		shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';
		strncpy( shopWindow1Msg, msg1, sizeof( shopWindow1Msg )-1 );
		shopWindow1Msg[sizeof( shopWindow1Msg )-1] = '\0';

		getStrSplit( (char *)shopWindow2Msg, msg2,
			sizeof( shopWindow2Msg[0] ),
				sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ),
					sizeof( shopWindow2Msg[0] )-1 );

		strncpy( shopWindow3Msg, msg3, sizeof( shopWindow3Msg )-1 );
		shopWindow3Msg[sizeof( shopWindow3Msg )-1] = '\0';

		getStrSplit( (char *)shopWindow4Msg, msg4,
			sizeof( shopWindow4Msg[0] ),
				sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ),
					sizeof( shopWindow4Msg[0] )-1 );

		getStrSplit( (char *)shopWindow5Msg, msg5,
			sizeof( shopWindow5Msg[0] ),
				sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ),
					sizeof( shopWindow5Msg[0] )-1 );

		getStrSplit( (char *)shopWindow6Msg, msg6,
			sizeof( shopWindow6Msg[0] ),
				sizeof( shopWindow6Msg )/sizeof( shopWindow6Msg[0] ),
					sizeof( shopWindow6Msg[0] )-1 );


		shopWondow2MaxPage = MAX_SHOP_ITEM*MAX_SHOP_PAGE;
		for( i = 0, j = 0; i < shopWondow2MaxPage; i++ ){
			flag = getStringToken( data, '|', 11+i*offset, sizeof( itemName )-1, itemName );
			makeStringFromEscaped( itemName );
			if( flag ){
				sealItem[i].name[0] = '\0';
				continue;
			}

			j++;
			if( strlen( itemName ) <= ITEM_NAME_LEN ){
				strcpy( sealItem[i].name, itemName );
			}else{
				strcpy( sealItem[i].name, "???" );
			}
			sealItem[i].sealFlag = getIntegerToken( data, '|', 12+i*offset );
			sealItem[i].level = getIntegerToken( data, '|', 13+i*offset );
			sealItem[i].price = getIntegerToken( data, '|', 14+i*offset );
			sealItem[i].graNo = getIntegerToken( data, '|', 15+i*offset );
			getStringToken( data, '|', 16+i*offset, sizeof( info )-1, info );
			makeStringFromEscaped( info );
			getStrSplit( (char *)(&sealItem[i].info), info,
				sizeof( sealItem[0].info[0] ),
					sizeof( sealItem[0].info )/sizeof( sealItem[0].info[0] ),
						sizeof( sealItem[0].info[0] )-1 );
#ifdef _NEW_MANOR_LAW
			sealItem[i].costfame = getIntegerToken(data,'|',17+i*offset);
#endif
		}
		if( j > 0 ){
			shopWondow2MaxPage = (j+7)/MAX_SHOP_ITEM;
		}else{
			shopWondow2MaxPage = 1;
		}
	}else{
#ifdef _ITEM_PILENUMS
		datalen = 7;
#endif

		shopWindowProcNo = 100;
		shopWondow2Page = 0;
		if( getIntegerToken( data, '|', 2 ) == 0 )
			return;
		menuIndexWN = getIntegerToken( data, '|', 3 );
		getStringToken( data, '|', 4, sizeof( title1 )-1, title1 );
		makeStringFromEscaped( title1 );
		getStringToken( data, '|', 5, sizeof( msg1 )-1, msg1 );
		makeStringFromEscaped( msg1 );
		getStringToken( data, '|', 6, sizeof( msg2 )-1, msg2 );
		makeStringFromEscaped( msg2 );
		getStringToken( data, '|', 7, sizeof( msg3 )-1, msg3 );
		makeStringFromEscaped( msg3 );
		getStringToken( data, '|', 8, sizeof( msg4 )-1, msg4 );
		makeStringFromEscaped( msg4 );

		strncpy( shopWindow1Title, title1, sizeof( shopWindow1Title )-1 );
		shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';

		strncpy( shopWindow1Msg, msg1, sizeof( shopWindow1Msg )-1 );
		shopWindow1Msg[sizeof( shopWindow1Msg )-1] = '\0';

		getStrSplit( (char *)shopWindow7Msg, msg2,
			sizeof( shopWindow7Msg[0] ),
				sizeof( shopWindow7Msg )/sizeof( shopWindow7Msg[0] ),
					sizeof( shopWindow7Msg[0] )-1 );

		getStrSplit( (char *)shopWindow8Msg, msg3,
			sizeof( shopWindow8Msg[0] ),
				sizeof( shopWindow8Msg )/sizeof( shopWindow8Msg[0] ),
					sizeof( shopWindow8Msg[0] )-1 );

		getStrSplit( (char *)shopWindow5Msg, msg4,
			sizeof( shopWindow5Msg[0] ),
				sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ),
					sizeof( shopWindow5Msg[0] )-1 );


		shopWondow2MaxPage = MAX_SHOP_ITEM*MAX_SHOP_PAGE;
		for( i = 0, j = 0; i < shopWondow2MaxPage; i++ ){
			flag = getStringToken( data, '|', 9+i*datalen, sizeof( itemName )-1, itemName );
			makeStringFromEscaped( itemName );
			if( flag ){
				userItem[i].name[0] = '\0';
				continue;
			}

			j++;
			if( strlen( itemName ) <= ITEM_NAME_LEN ){
				strcpy( userItem[i].name, itemName );
			}else{
				strcpy( userItem[i].name, "???" );
			}
			userItem[i].sealFlag = getIntegerToken( data, '|', 10+i*datalen );
			userItem[i].price = getIntegerToken( data, '|', 11+i*datalen );
			userItem[i].graNo = getIntegerToken( data, '|', 12+i*datalen );
			getStringToken( data, '|', 13+i*datalen, sizeof( info )-1, info );
			makeStringFromEscaped( info );
			getStrSplit( (char *)(&userItem[i].info), info,
				sizeof( userItem[i].info[0] ),
					sizeof( userItem[i].info )/sizeof( userItem[i].info[0] ),
						sizeof( userItem[i].info[0] )-1 );
			userItem[i].tbl = getIntegerToken( data, '|', 14+i*datalen );
#ifdef _ITEM_PILENUMS
			userItem[i].num = getIntegerToken( data, '|', 15+i*datalen );
#endif
		}
		if( j > 0 ){
			shopWondow2MaxPage = (j+7)/MAX_SHOP_ITEM;
		}else{
			shopWondow2MaxPage = 1;
		}
	}
}


// ????????
//  ??????????????????????
void initServerWindowType4( char *data )
{
	msgWN_W = 9;
	msgWN_H = 9;
	msgWNLen = (msgWN_W*64-48)/9;

	makeStringFromEscaped( data );
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), msgWNLen );
}

#ifdef _PETSKILL_CANNEDFOOD
short PetskillShpwWindowProcNo;
void initPetSkillWindowType1( char *data)
{
	selShopSkillPetNo = atoi( data);
	PetskillShpwWindowProcNo = 0;
}
#endif

void initServerWindowType5(char *data)
{
	char title1[128], msg2[128], skillName[64], info[256];
	int i, j, flag;
	int iReadOffset = 4;
	
	skillShopWindowProcNo = 0;
	shopWondow2Page = 0;
	
	// ??q?????????????????????????????
	if (getIntegerToken(data, '|', 1) == 0)
		return;
	
	getStringToken(data, '|', 2, sizeof(title1) - 1, title1);
	makeStringFromEscaped(title1);
	getStringToken(data, '|', 3, sizeof(msg2) - 1, msg2);
	makeStringFromEscaped(msg2);
	strncpy(shopWindow1Title, title1, sizeof(shopWindow1Title) - 1);
	shopWindow1Title[sizeof(shopWindow1Title) - 1] = '\0';
	
	getStrSplit((char*)shopWindow2Msg, msg2,
		sizeof(shopWindow2Msg[0]), sizeof(shopWindow2Msg) / sizeof(shopWindow2Msg[0]),
		sizeof(shopWindow2Msg[0]) - 1);
	
	shopWondow2MaxPage = MAX_SKILL_SHOP_SKILL*MAX_SKILL_SHOP_PAGE;
	for (i = 0, j = 0; i < shopWondow2MaxPage; i++)
	{
		// ?????
		flag = getStringToken(data, '|', 4 + i * iReadOffset, sizeof(skillName) - 1, skillName);
		makeStringFromEscaped(skillName);
		if (flag)
		{
			sealSkill[i].name[0] = '\0';
			continue;
		}
		
		j++;
		if (strlen(skillName) <= SKILL_NAME_LEN)
			strcpy(sealSkill[i].name, skillName);
		else
			strcpy( sealSkill[i].name, "???" );
		
		// ?k?h
		sealSkill[i].price = getIntegerToken(data, '|', 5 + i * iReadOffset);
		
		// ?????????
		getStringToken(data, '|', 6 + i * iReadOffset, sizeof(info) - 1, info);
		makeStringFromEscaped(info);
		getStrSplit((char*)(&sealSkill[i].info), info,
			sizeof(sealSkill[0].info[0]),
			sizeof(sealSkill[0].info) / sizeof(sealSkill[0].info[0]),
			sizeof(sealSkill[0].info[0]) - 1);
	}
	if( j > 0 )
		shopWondow2MaxPage = (j + 7) / MAX_SKILL_SHOP_SKILL;
	else
		shopWondow2MaxPage = 1;
}


// ????????????????????
void initServerWindowType6( char *data )
{
	char title1[128];
	char msg1[128];
	char msg2[256];
	char msg3[128];
	char itemName[64];
	char info[256];
	int i, j;
	int flag;

#ifdef _ITEM_PILENUMS
	int datalen = 7;
#else
	int datalen = 6;
#endif

	shopWindowMode = getIntegerToken( data, '|', 1 );
	if( shopWindowMode == 0 ){
		shopWindowProcNo = 10;
		shopWondow2Page = 0;
		restPoolSlot = getIntegerToken( data, '|', 2 );
		getStringToken( data, '|', 3, sizeof( title1 )-1, title1 );
		makeStringFromEscaped( title1 );
		getStringToken( data, '|', 4, sizeof( msg1 )-1, msg1 );
		makeStringFromEscaped( msg1 );
		getStringToken( data, '|', 5, sizeof( msg2 )-1, msg2 );
		makeStringFromEscaped( msg2 );
		getStringToken( data, '|', 6, sizeof( msg3 )-1, msg3 );
		makeStringFromEscaped( msg3 );

		strncpy( shopWindow1Title, title1, sizeof( shopWindow1Title )-1 );
		shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';

		getStrSplit( (char *)shopWindow2Msg, msg1,
			sizeof( shopWindow2Msg[0] ),
				sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ),
					sizeof( shopWindow2Msg[0] )-1 );

		getStrSplit( (char *)shopWindow4Msg, msg2,
			sizeof( shopWindow4Msg[0] ),
				sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ),
					sizeof( shopWindow4Msg[0] )-1 );

		getStrSplit( (char *)shopWindow5Msg, msg3,
			sizeof( shopWindow5Msg[0] ),
				sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ),
					sizeof( shopWindow5Msg[0] )-1 );


		shopWondow2MaxPage = MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE;
		for( i = 0, j = 0; i < shopWondow2MaxPage; i++ ){
			flag = getStringToken( data, '|', 7+i*datalen, sizeof( itemName )-1, itemName );
			makeStringFromEscaped( itemName );
			if( flag ){
				poolItem[i].name[0] = '\0';
				continue;
			}

			j++;
			if( strlen( itemName ) <= ITEM_NAME_LEN ){
				strcpy( poolItem[i].name, itemName );
			}else{
				strcpy( poolItem[i].name, "???" );
			}
			poolItem[i].poolFlag = getIntegerToken( data, '|', 8+i*datalen );
			poolItem[i].price = getIntegerToken( data, '|', 9+i*datalen );
			poolItem[i].graNo = getIntegerToken( data, '|', 10+i*datalen );
			getStringToken( data, '|', 11+i*datalen, sizeof( info )-1, info );

#ifdef _ITEM_PILENUMS
			poolItem[i].num = getIntegerToken( data, '|', 12+i*datalen );
#endif
			makeStringFromEscaped( info );
			getStrSplit( (char *)(&poolItem[i].info), info,
				sizeof( poolItem[0].info[0] ),
					sizeof( poolItem[0].info )/sizeof( poolItem[0].info[0] ),
						sizeof( poolItem[0].info[0] )-1 );
#ifdef _ITEM_PILENUMS
			poolItem[i].tbl = getIntegerToken( data, '|', 13+i*datalen );
#else
			poolItem[i].tbl = getIntegerToken( data, '|', 12+i*datalen );
#endif
		}
		if( j > 0 ){
			shopWondow2MaxPage = (j+7)/MAX_POOL_SHOP_ITEM;
		}else{
			shopWondow2MaxPage = 1;
		}
	}else{
		shopWindowProcNo = 100;
		shopWondow2Page = 0;
		getStringToken( data, '|', 2, sizeof( title1 )-1, title1 );
		makeStringFromEscaped( title1 );
		getStringToken( data, '|', 3, sizeof( msg1 )-1, msg1 );
		makeStringFromEscaped( msg1 );
		getStringToken( data, '|', 4, sizeof( msg2 )-1, msg2 );
		makeStringFromEscaped( msg2 );
		getStringToken( data, '|', 5, sizeof( msg3 )-1, msg3 );
		makeStringFromEscaped( msg3 );

		strncpy( shopWindow1Title, title1, sizeof( shopWindow1Title )-1 );
		shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';
		getStrSplit( (char *)shopWindow2Msg, msg1,
			sizeof( shopWindow2Msg[0] ),
				sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ),
					sizeof( shopWindow2Msg[0] )-1 );

		getStrSplit( (char *)shopWindow4Msg, msg2,
			sizeof( shopWindow4Msg[0] ),
				sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ),
					sizeof( shopWindow4Msg[0] )-1 );

		getStrSplit( (char *)shopWindow5Msg, msg3,
			sizeof( shopWindow5Msg[0] ),
				sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ),
					sizeof( shopWindow5Msg[0] )-1 );


		shopWondow2MaxPage = MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE;
		for( i = 0, j = 0; i < shopWondow2MaxPage; i++ ){
			// ??????
			flag = getStringToken( data, '|', 6+i*datalen, sizeof( itemName )-1, itemName );
			makeStringFromEscaped( itemName );
			if( flag ){
				poolItem[i].name[0] = '\0';
				continue;
			}

			j++;
			if( strlen( itemName ) <= ITEM_NAME_LEN ){
				strcpy( poolItem[i].name, itemName );
			}else{
				strcpy( poolItem[i].name, "???" );
			}
			poolItem[i].poolFlag = getIntegerToken( data, '|', 7+i*datalen );
			poolItem[i].level = getIntegerToken( data, '|', 8+i*datalen );
			poolItem[i].price = getIntegerToken( data, '|', 9+i*datalen );
			poolItem[i].graNo = getIntegerToken( data, '|', 10+i*datalen );
			getStringToken( data, '|', 11+i*datalen, sizeof( info )-1, info );
#ifdef _ITEM_PILENUMS
			poolItem[i].num = getIntegerToken( data, '|', 12+i*datalen );
#endif
			makeStringFromEscaped( info );
			getStrSplit( (char *)(&poolItem[i].info), info,
				sizeof( poolItem[i].info[0] ),
					sizeof( poolItem[i].info )/sizeof( poolItem[i].info[0] ),
						sizeof( userItem[i].info[0] )-1 );
		}
		if( j > 0 ){
			shopWondow2MaxPage = (j+7)/MAX_POOL_SHOP_ITEM;
		}else{
			shopWondow2MaxPage = 1;
		}
	}
}

void initServerWindowType7( char *data )
{
	char msg[256];

	shopWindowProcNo = 0;

	getStringToken( data, '|', 1, sizeof( msg )-1, msg );
	makeStringFromEscaped( msg );
	strncpy( shopWindow1Title, msg, sizeof( shopWindow1Title )-1 );
	shopWindow1Title[sizeof( shopWindow1Title )-1] = '\0';

	getStringToken( data, '|', 2, sizeof( msg )-1, msg );
	makeStringFromEscaped( msg );
	strncpy( shopWindow1Msg, msg, sizeof( shopWindow1Msg )-1 );
	shopWindow1Msg[sizeof( shopWindow1Msg )-1] = '\0';
}



// ?????????
// ?????????u??????????
// ???????u??????????????V?
void serverWindowType0( int mode )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static STR_BUFFER input;

	int id;
	int i, j;
	int mask;
	int btn;

	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	static int totalMsgLine;
	static int lineSkip;


	if( ptActMenuWin == NULL )
	{
		winW = msgWN_W;
		winH = msgWN_H;
#ifdef _NEW_MANOR_LAW
		if(mode == 2) winH = 7;
#endif
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif

		lineSkip = 20;
		totalMsgLine = (winH*48-56)/lineSkip;

		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = ((winH*48-56)/totalMsgLine)*(totalMsgLine-1)
					+(winH*48-totalMsgLine*lineSkip)/2+8;
			}
		}

		// ?????????????????
		if( mode == 0 || mode == 2)
		{
			msgLine = totalMsgLine - 1;
			input.buffer[0] = '\0';
		}
		else
		if( mode == 1 )
		{
			msgLine = totalMsgLine - 2;
			input.buffer[0] = '\0';
			input.cnt = 0;
			input.cursor=0;
			initStrBuffer( &input,
				winX+(msgWN_W*64-msgWNLen*9)/2,
					winY+((winH*48-56)/totalMsgLine)*(totalMsgLine-2)+(winH*48-totalMsgLine*lineSkip)/2,
						msgWNLen, FONT_PAL_WHITE, FONT_PRIO_FRONT );
			GetKeyInputFocus( &input );
		}

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || ((joy_trg[ 0 ] & JOY_ESC) /*&& GetImeString() == NULL*/)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		// ???
		if( 0 <= id && id < 6 )
		{
			char msg[256];
			btn = 1;
			btn <<= id;

			makeEscapeString( input.buffer, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}

		if( id >= 0 )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( mode == 1 )
			{
				GetKeyInputFocus( &MyChatBuffer );
			}
			return;
		}


		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{
					StockFontBuffer( winX+(msgWN_W*64-msgWNLen*9)/2,
						winY+((winH*48-56)/totalMsgLine)*i+(winH*48-totalMsgLine*lineSkip)/2,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
			// ?V???????
			if( mode == 1 )
			{
				StockFontBuffer2( &input );
			}
		}
	}
}


// ?????????
// ??????k?????u??????????
void serverWindowType1( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id, id2;
	int i, j;
	int mask;
	int btn;
	int x1, y1, x2, y2;

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = 196;
			}
		}

		msgLine = 10; //msgLine = 8;

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ?????k???R?e
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;
		if( ptActMenuWin->hp >= 1 )	// ???????????k????????
		{
			for( i = selStartLine; i <  msgLine; i++ )
			{
				// ??????????????????
				if( strlen( msgWN[i] ) > 0 )
				{
					x1 = winX+41;
					y1 = winY+18+i*21;
					x2 = x1 + 366;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							id2 = i - selStartLine + 1;
						}
					}
				}
			}
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		if( 0 <= id && id < 6
		 || 0 <= id2 && id2 < 10
		 || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d", id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)

				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}

		if( id >= 0 || id2 >= 0 )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{
					StockFontBuffer( winX+44, winY+21+i*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}

void serverWindowType2( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id, id2;
	int i, j;
	int mask;
	int btn;
	int x1, y1, x2, y2;
	int len;

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 ){
			if( buttonTypeWN & mask ){
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 ){
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = 196;
			}
		}
		strcpy( msgWN[0], "＝ ＝ ＝         请选择宠物         ＝ ＝ ＝" );
		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL ){
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );
		id2 = -1;
		if( ptActMenuWin->hp >= 1 ){
			for( i = 1, j = 1; i <= MAX_PET; i++ ){
				if(pet[i-1].useFlag != 0 ){
					x1 = winX+41;
					y1 = winY+18+(j+1)*21;
					x2 = x1 + 366;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							id2 = i;
						}
					}
					j++;
				}
			}
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		if( 0 <= id && id < 6
		 || 0 <= id2 && id2 < 8 ){
			char data[256];
			char msg[256];
			if( 0 <= id && id < 6 ){
				btn = 1;
				btn <<= id;
			}else{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d", id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}

		if( id >= 0 || id2 >= 0 ){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 ){
			StockFontBuffer( winX+54, winY+21,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[0], 0 );
			for( i = 1, j = 1; i <= MAX_PET; i++ ){
				len = strlen( pet[i-1].name );
				if( pet[i-1].useFlag != 0 ){
					len = strlen( pet[i-1].freeName );
					if( len > 0 ){
						strcpy( msgWN[1], pet[i-1].freeName );
					}else{
						len = strlen( pet[i-1].name );
						strcpy( msgWN[1], pet[i-1].name );
					}
					StockFontBuffer( winX+152+(144-len*9)/2, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					sprintf( msgWN[1], "LV.%d", pet[i-1].level );
					StockFontBuffer( winX+260, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					sprintf( msgWN[1], "MaxHP %d", pet[i-1].maxHp );
					StockFontBuffer( winX+314, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					j++;
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 ){
				btnId[i] = -2;
				if( buttonTypeWN & mask ){
					if( j < btnCnt ){
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}

void serverWindowType3( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id, id2;
	int i, j;
	int mask;
	int btn;
	int x1, y1, x2, y2;
	int len;

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = 196;
			}
		}

		strcpy( msgWN[0], "＝ ＝ ＝         请选择队友       ＝ ＝ ＝" );

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ?????k???R?e
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;
		if( ptActMenuWin->hp >= 1 )	// ???????????k????????
		{
			for( i = 1; i <= MAX_PARTY; i++ )
			{
				// ??????????????????
				if( partyModeFlag == 0 && i == 1 )
				{
					strcpy( msgWN[1], pc.name );
				}
				else
				if( party[i-1].useFlag != 0 )
				{
						strcpy( msgWN[1], party[i-1].name );
				}
				else
				{
					msgWN[1][0] = '\0';
				}
				len = strlen( msgWN[1] );
				if( len > 0 )
				{
					x1 = winX+41;
					y1 = winY+18+(i+1)*21;
					x2 = x1 + 366;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							id2 = i + 5;
						}
					}
				}
			}
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		if( 0 <= id && id < 6
		 || 0 <= id2 && id2 < 8 )
		{
			char data[256];
			char msg[256];
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d", id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}

		if( id >= 0 || id2 >= 0 )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+62, winY+21,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[0], 0 );
			for( i = 1; i <= MAX_PARTY; i++ )
			{
				if( partyModeFlag == 0 && i == 1 )
				{
					strcpy( msgWN[1], pc.name );
				}
				else
				if( party[i-1].useFlag != 0 )
				{
					strcpy( msgWN[1], party[i-1].name );
				}
				else
				{
					msgWN[1][0] = '\0';
				}
				len = strlen( msgWN[1] );
				if( len > 0 )
				{
					StockFontBuffer( winX+152+(144-len*9)/2, winY+21+(i+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}


// ?????????
// ???????k??
void serverWindowType4( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id, id2;
	int i, j;
	int mask;
	int btn;
	int x1, y1, x2, y2;
	int len;

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = 196;
			}
		}

		strcpy( msgWN[0], "＝ ＝ ＝    请选择宠物或是队友    ＝ ＝ ＝" );

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ?????k???R?e
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;
		if( ptActMenuWin->hp >= 1 )	// ???????????k????????
		{
			for( i = 1; i <= MAX_PET; i++ )
			{
				// ??????????????????
				if(pet[i-1].useFlag != 0 )
				{
					x1 = winX+41;
						y1 = winY+18+(i+1)*21;
					x2 = x1 + 149;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							id2 = i;
						}
					}
				}
			}
		}

		for( i = 1; i <= MAX_PARTY; i++ )
		{
			// ??????????????????
			if( partyModeFlag == 0 && i == 1 )
			{
				strcpy( msgWN[1], pc.name );
			}
			else
			if( party[i-1].useFlag != 0 )
			{
				strcpy( msgWN[1], party[i-1].name );
			}
			else
			{
				msgWN[1][0] = '\0';
			}
			len = strlen( msgWN[1] );
			if( len > 0 )
			{
				x1 = winX+257;
				y1 = winY+18+(i+1)*21;
				x2 = x1 + 149;
				y2 = y1 + 20;
				if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
				{
					if( mouse.onceState & MOUSE_LEFT_CRICK )
					{
						id2 = i + 5;
					}
				}
			}
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		if( 0 <= id && id < 6
		 || 0 <= id2 && id2 < 10 )
		{
			char data[256];
			char msg[256];
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d", id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}

		if( id >= 0 || id2 >= 0 )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+62, winY+21,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[0], 0 );

			// ???????
			for( i = 1; i <= MAX_PET; i++ )
			{
				len = strlen( pet[i-1].name );
				if( pet[i-1].useFlag != 0 )
				{
					len = strlen( pet[i-1].freeName );
					if( len > 0 )
					{
						strcpy( msgWN[1], pet[i-1].freeName );
					}
					else
					{
						len = strlen( pet[i-1].name );
						strcpy( msgWN[1], pet[i-1].name );
					}
					StockFontBuffer( winX+44+(144-len*9)/2, winY+21+(i+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );
				}
			}

			// ????????
			for( i = 1; i <= MAX_PARTY; i++ )
			{
				if( partyModeFlag == 0 && i == 1 )
				{
					strcpy( msgWN[1], pc.name );
				}
				else
				if( party[i-1].useFlag != 0 )
				{
					strcpy( msgWN[1], party[i-1].name );
				}
				else
				{
					msgWN[1][0] = '\0';
				}
				len = strlen( msgWN[1] );
				if( len > 0 )
				{
					StockFontBuffer( winX+260+(144-len*9)/2, winY+21+(i+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}


void serverWindowType9( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	char *btnTitle[] = { 
				" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
		};
	
	static int selectID[3]={-1,-1,-1};
	static int selects=0;
	
	int id, id2;
	int i, j;
	int mask, btn, len;
	int x1, y1, x2, y2;

	if( ptActMenuWin == NULL ){
		winW = 7;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 ){
			if( buttonTypeWN & mask ){
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 ){
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ ){
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = 196;
			}
		}
		if( selects == 0 )	{
			strcpy( msgWN[0], "＝ ＝ ＝       请选择主体宠物     ＝ ＝ ＝" );
		}else	{
			strcpy( msgWN[0], "＝ ＝ ＝       请选择客体宠物     ＝ ＝ ＝" );
		}
		play_se( 202, 320, 240 );	// ????????
	}
	if( ptActMenuWin != NULL ){
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );
		id2 = -1;
		if( ptActMenuWin->hp >= 1 ){
			for( i = 1, j = 1; i <= MAX_PET; i++ ){
				if(pet[i-1].useFlag != 0 ){
					x1 = winX+41;
					y1 = winY+18+(j+1)*21;
					x2 = x1 + 366;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							id2 = i;
							if( selects < 3 ){
								int k;
								for( k=0; k<3; k++)	{
									if( selectID[k] == i )
										break;
								}
								if( k>= 3 )	{
									selectID[selects++] = i;
								}
							}
						}
					}
					j++;
				}
			}
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		if( 0 <= id && id < 6 ){
		 //|| 0 <= id2 && id2 < 8 ){
			char data[256];
			char msg[256];
			if( 0 <= id && id < 6 ){
				btn = 1;
				btn <<= id;
			}else{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			//sprintf( data, "%d", id2 );//selectID
			sprintf( data, "%d|%d|%d", selectID[0], selectID[1], selectID[2] );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			for( i=0; i<3; i++)	{
				selectID[i]=-1;
			}
			selects = 0;
			windowTypeWN = -1;
		}

		if( id >= 0 || id2 >= 0 ){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 ){
			StockFontBuffer( winX+54, winY+21,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[0], 0 );
			for( i = 1, j = 1; i <= MAX_PET; i++ ){
				len = strlen( pet[i-1].name );
				if( pet[i-1].useFlag != 0 ){
					len = strlen( pet[i-1].freeName );
					if( len > 0 ){
						strcpy( msgWN[1], pet[i-1].freeName );
					}else{
						len = strlen( pet[i-1].name );
						strcpy( msgWN[1], pet[i-1].name );
					}
					StockFontBuffer( winX+62, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					//selectID
					if( selectID[0] == i )	{
						sprintf( msgWN[1], "主体");
						StockFontBuffer( winX+220, winY+21+(j+1)*21,
							FONT_PRIO_FRONT, FONT_PAL_RED, msgWN[1], 0 );
					}else if( selectID[1] == i || selectID[2] == i )	{
						sprintf( msgWN[1], "客体");
						StockFontBuffer( winX+220, winY+21+(j+1)*21,
							FONT_PRIO_FRONT, FONT_PAL_GREEN, msgWN[1], 0 );
					}
					sprintf( msgWN[1], "LV.%d", pet[i-1].level );
					StockFontBuffer( winX+260, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );
					sprintf( msgWN[1], "MaxHP %d", pet[i-1].maxHp );
					StockFontBuffer( winX+314, winY+21+(j+1)*21,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					j++;
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 ){
				btnId[i] = -2;
				if( buttonTypeWN & mask ){
					if( j < btnCnt ){
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}


// ?????????

void initShopWindow1( void );
int shopWindow1( void );
void initShopWindow2( void );
int shopWindow2( void );
void initShopWindow3( void );
int shopWindow3( void );
void initShopWindow4( void );
int shopWindow4( void );
void initShopWindow5( void );
int shopWindow5( void );
void initShopWindow6( void );
int shopWindow6( void );
void initShopWindow7( void );
int shopWindow7( void );
void initShopWindow8( void );
int shopWindow8( void );

#ifdef _ITEM_PILENUMS
void initShopWindow10( void );
int shopWindow10( void );
#endif

void serverWindowType5( void )
{
	int ret;

	if( shopWindowProcNo == 0 ){
		initShopWindow1();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 1 ){
		ret = shopWindow1();
	}

	if( shopWindowProcNo == 15 ){
		initShopWindow2();
		shopWindowProcNo = 11;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 10 ){
		int i, j;
		for( i = MAX_ITEMSTART, j= 0; i < MAX_ITEM; i++ ){
			if( pc.item[i].useFlag == 0 )
				j++;
		}
		nowUserItemCnt = j;
	
		initShopWindow2();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 11 ){
		ret = shopWindow2();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "0" );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			windowTypeWN = -1;
		}else if( ret == 2 ){
			shopWindowProcNo = 30;
		}else if( ret == 3 ){
			shopWindowProcNo = 20;
		}
		if( ret == 4 ){
			shopWindowProcNo = 50;
		}
	}
	if( shopWindowProcNo == 20 ){
		initShopWindow4();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 21 ){
		ret = shopWindow4();
		if( ret == 1 ){
			shopWindowProcNo = 30;
		}else if( ret == 2 ){
			shopWindowProcNo = 15;
		}
	}
	if( shopWindowProcNo == 30 ){
		initShopWindow3();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 31 ){
		ret = shopWindow3();
		if( ret == 1 ){
			shopWindowProcNo = 40;
		}else if( ret == 2 ){
			shopWindowProcNo = 15;
		}
	}
	if( shopWindowProcNo == 40 ){
		initShopWindow5();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 41 ){
		ret = shopWindow5();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "%d|%d", selShopItemNo+1, sealItemCnt );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			nowUserItemCnt--;
			pc.gold -= sealItem[selShopItemNo].price*sealItemCnt;
#ifdef _NEW_MANOR_LAW
			if(sealItem[selShopItemNo].costfame > 0)
				pc.fame -= sealItem[selShopItemNo].costfame * sealItemCnt;
#endif
			shopWindowProcNo = 15;
		}else if( ret == 2 ){
			shopWindowProcNo = 30;
		}
	}
	if( shopWindowProcNo == 50 ){
		initShopWindow6();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 51 ){
		ret = shopWindow6();
		if( ret == 1 ){
			shopWindowProcNo = 15;
		}
	}

	if( shopWindowProcNo == 100 ){
		initShopWindow7();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 101 ){
		ret = shopWindow7();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "0" );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			windowTypeWN = -1;
		}else if( ret == 2 ){
			shopWindowProcNo = 110;
		}else if( ret == 3 ){
			shopWindowProcNo = 120;
		}
	}

	if( shopWindowProcNo == 110 ){
#ifdef _ITEM_PILENUMS
		initShopWindow10();
#else
		initShopWindow5();
#endif
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 111 ){
#ifdef _ITEM_PILENUMS
		ret = shopWindow10();
#else
		ret = shopWindow5();
#endif
		if( ret == 1 ){
			char data[256];
			char msg[256];

#ifdef _ITEM_PILENUMS
			sprintf( data, "%d|%d", userItem[selShopItemNo].tbl, sealItemCnt);
#else
			sprintf( data, "%d|%d", userItem[selShopItemNo].tbl, userItem[selShopItemNo].price );
#endif
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			
			pc.gold += userItem[selShopItemNo].price;
			int i, j, k;
#ifdef _ITEM_PILENUMS
			shopWindowProcNo = 100;
			userItem[selShopItemNo].num -= sealItemCnt;
			if( userItem[selShopItemNo].num <= 0 ){
#else
			shopWindowProcNo = 100;
#endif
				userItem[selShopItemNo].name[0] = '\0';
				for( i = 0; i < MAX_SHOP_ITEM*MAX_SHOP_PAGE-1; i++ ){
					if( userItem[i].name[0] == '\0' ){
						for( j = i+1; j < MAX_SHOP_ITEM*MAX_SHOP_PAGE; j++ ){
							if( userItem[j].name[0] != '\0' ){
								strcpy( userItem[i].name, userItem[j].name );
								userItem[i].sealFlag = userItem[j].sealFlag;
								userItem[i].level = userItem[j].level;
								userItem[i].price = userItem[j].price;
								userItem[i].graNo = userItem[j].graNo;
								userItem[i].tbl = userItem[j].tbl;
#ifdef _ITEM_PILENUMS
								userItem[i].num = userItem[j].num;
#endif
								for( k = 0; k < 3; k++ ){
									strcpy( userItem[i].info[k], userItem[j].info[k] );
								}
								userItem[j].name[0] = '\0';
								break;
							}
						}
					}
				}
#ifdef _ITEM_PILENUMS
			}
#endif
			for( i = 0; i < MAX_SHOP_ITEM*MAX_SHOP_PAGE; i++ ){
				if( userItem[i].name[0] == '\0' ) break;
			}
			if( i > 0 ){
				shopWondow2MaxPage = (i+7)/MAX_SHOP_ITEM;
			}else{
				shopWondow2MaxPage = 1;
			}
			if( shopWondow2Page >= shopWondow2MaxPage )
				shopWondow2Page = shopWondow2MaxPage-1;

		}else if( ret == 2 ){
			shopWindowProcNo = 100;
		}
	}

	if( shopWindowProcNo == 120 ){
		initShopWindow8();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 121 ){
		ret = shopWindow8();
		if( ret == 1 ){
			shopWindowProcNo = 100;
		}
	}
}

short shopWindow1ProcNo;

void initShopWindow1( void )
{
	shopWindow1ProcNo = 0;
}

int shopWindow1( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id = 0;
	int i;
	int ret = 0;

	if( shopWindow1ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 4;
		h = 3;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow1ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			ret = id + 1;
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			ret = 3;	// ????
			wnCloseFlag = 0;
		}

		if( 0 <= id && id <= 2 )
		{
#if 1
			char data[256];
			char msg[256];
			sprintf( data, "%d", ret );
			makeEscapeString( data, msg, sizeof( msg )-1 );
#if 0
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, menuIndexWN, idWN, 0, msg ) ;
			else
				oldlssproto_WN_send( sockfd, nowGx, nowGy, menuIndexWN, idWN, 0, msg ) ;
#else
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
#endif
#endif
		}

		if( id >= 0 )
		{
			if( ret == 3 )
			{
				play_se( 203, 320, 240 );	// ??????????
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			windowTypeWN = -1;
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			xx = (w*64-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+12, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			xx = (w*64-strlen( shopWindow1Msg )/2*17)/2;
			StockFontBuffer( x+xx, y+32, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Msg, 0 );

			btnId[0] =
				StockDispBuffer( x+w*64/2, y+16/2+66, DISP_PRIO_IME3, CG_BUY_BTN, 2 );
			btnId[1] =
				StockDispBuffer( x+w*64/2, y+16/2+90, DISP_PRIO_IME3, CG_SEAL_BTN, 2 );
			btnId[2] =
				StockDispBuffer( x+w*64/2, y+16/2+114, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
		}
	}

	return ret;
}

short shopWindow2ProcNo;
void initShopWindow2( void )
{
	shopWindow2ProcNo = 0;
}

int shopWindow2( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( shopWindow2ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow2ProcNo++;
	}


	if( ptActMenuWin != NULL ){
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ ){
				if( sealItem[j+i].name[0] != '\0' ){
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){

							if(sealItem[j+i].price <= pc.gold && 
								sealItem[j+i].sealFlag == 0
#ifdef _NEW_MANOR_LAW
								&& sealItem[j+i].costfame <= pc.fame
#endif
								){							
								selId = j+i;
							}else{
								play_se( 220, 320, 240 );
							}
						}
						focusId = j+i;
					}
				}
			}
		}

		if( joy_con[0] & JOY_A ){
			pushId = 0;
			if( joy_auto[0] & JOY_A ){
				id = 0;
			}
		}
		if( pushId == 0 ){
			prevBtn = 1;
			if( id == 0 ){
				if( shopWondow2Page > 0 ){
					shopWondow2Page--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}else{
			prevBtn = 0;
		}
		if( joy_con[0] & JOY_B ){
			pushId = 1;
			if( joy_auto[0] & JOY_B ){
				id = 1;
			}
		}
		if( pushId == 1 ){
			nextBtn = 1;
			if( id == 1 ){
				if( shopWondow2Page+1 < shopWondow2MaxPage ){
					shopWondow2Page++;
					play_se( 217, 320, 240 );
				}
			}
		}else{
			nextBtn = 0;
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 2	 || selId >= 0 ){
			if( id == 2 ){
				ret = 1;
			}else if( id == 100 ){
				ret = 100;
			}else{
				selShopItemNo = selId;
				sealItemCnt = 1;
				if( nowUserItemCnt <= 0 ){
					ret = 4;
				}
				else if( sealItem[selId].level > pc.level ){
					ret = 3;
				}else{
					ret = 2;
				}
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 ){
			int xx;
			xx = (w-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );
			for( i = 0; i < sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ); i++ ){
				if( shopWindow2Msg[0][0] != '\0' ){
					StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow2Msg[i], 0 );
				}
			}
#ifdef _NEW_MANOR_LAW

			sprintf(tmsg,"声望 %8dF",pc.fame);
			StockFontBuffer(x+244,y+64,FONT_PRIO_FRONT,FONT_PAL_WHITE,tmsg,0);
#endif
			sprintf( tmsg, "金钱  %8dS", pc.gold );
			StockFontBuffer( x+244, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ ){
				if( strlen( sealItem[j+i].name ) == 0 )	continue;
				color = itemColor[0];
				if( sealItem[j+i].level > pc.level )
					color = itemColor[2];
				if( sealItem[j+i].sealFlag )
					color = itemColor[1];
				if( sealItem[j+i].price > pc.gold ){
					color = itemColor[1];
				}
#ifdef _NEW_MANOR_LAW
				if(sealItem[j+i].costfame > pc.fame){
					color = itemColor[1];
				}
#endif
				StockFontBuffer( x+34, y+118+i*21, FONT_PRIO_FRONT, color, sealItem[j+i].name, 0 );
#ifdef _NEW_MANOR_LAW
				if(sealItem[j+i].costfame > -1){
						sprintf(tmsg,"%8dF%8dS",sealItem[j+i].costfame,sealItem[j+i].price);
					StockFontBuffer(x+242,y+118+i*21,FONT_PRIO_FRONT,color,tmsg,0);	
				}
				else
#endif
				{
					sprintf( tmsg, "%8dS", sealItem[j+i].price );
					StockFontBuffer( x+312, y+118+i*21,	FONT_PRIO_FRONT, color, tmsg, 0 );
				}
			}
			if( focusId >= 0 ){
				StockDispBuffer( x+64, y+350, DISP_PRIO_IME3, sealItem[focusId].graNo, 0 );
				for( i = 0; i < sizeof( sealItem[0].info )/sizeof( sealItem[0].info[0] ); i++ ){
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						sealItem[focusId].info[i], 0 );
				}
			}
			btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_WIN, 1 );
		}
	}
	return ret;
}


short shopWindow3ProcNo;
void initShopWindow3( void )
{
	shopWindow3ProcNo = 0;
}

int shopWindow3( void )
{
	static int x, y, w, h;
	static int btnId[4];
	int upBtnGraNo[] = { CG_UP_BTN, CG_UP_BTN_DOWN };
	int downBtnGraNo[] = { CG_DOWN_BTN, CG_DOWN_BTN_DOWN };
	int upBtn, downBtn;
	int id;
	int pushId = 0;
	int i;
	int ret = 0;
	char tmsg[256];

	if( shopWindow3ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 440;
		h = 160;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif		
		shopWindow3ProcNo++;
	}


	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		if( joy_con[0] & JOY_A ){
			pushId = 0;
			if( joy_auto[0] & JOY_A ){
				id = 0;
			}
		}
		if( pushId == 0 ){
			downBtn = 1;
			if( id == 0 ){
				if( sealItemCnt > 1 ){
					sealItemCnt--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}else{
			downBtn = 0;
		}
		if( joy_con[0] & JOY_B ){
			pushId = 1;
			if( joy_auto[0] & JOY_B ){
				id = 1;
			}
		}
		if( pushId == 1 ){
			upBtn = 1;
			if( id == 1 ){
				if( sealItemCnt+1 <= nowUserItemCnt
				 && sealItem[selShopItemNo].price*(sealItemCnt+1) <= pc.gold 
#ifdef _NEW_MANOR_LAW
				 && sealItem[selShopItemNo].costfame * (sealItemCnt+1) <= pc.fame
#endif
				 ){
					sealItemCnt++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}else{
			upBtn = 0;
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 2 ){
			if( id == 2 ){
				ret = 1;
			}else if( id == 3 ){
				ret = 2;
			}else if( id == 100 ){
				ret = 100;
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}
		if( ptActMenuWin->hp >= 1 ){
			int xx;
			xx = (w-strlen( shopWindow3Msg )/2*17)/2;
			StockFontBuffer( x+xx, y+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow3Msg, 0 );
			StockFontBuffer( x+16, y+52, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				sealItem[selShopItemNo].name, 0 );
			sprintf( tmsg, "%2d个", sealItemCnt );
			StockFontBuffer( x+300, y+52, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			sprintf( tmsg, "%8dS x %2d = %8dS",	sealItem[selShopItemNo].price,
				sealItemCnt, sealItem[selShopItemNo].price*sealItemCnt );
			StockFontBuffer( x+16, y+82, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				tmsg, 0 );
#ifdef _NEW_MANOR_LAW
			if(sealItem[selShopItemNo].costfame > -1){
				sprintf(tmsg,"%8dF x %2d = %8dF",sealItem[selShopItemNo].costfame,sealItemCnt,
								sealItem[selShopItemNo].costfame * sealItemCnt);
				StockFontBuffer(x+16,y+102,FONT_PRIO_FRONT,FONT_PAL_WHITE,tmsg,0);
			}
			sprintf(tmsg,"声望 %8dF",pc.fame);
			StockFontBuffer(x+248,y+100,FONT_PRIO_FRONT,FONT_PAL_WHITE,tmsg,0);
#endif
			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+248, y+120, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			btnId[0] = StockDispBuffer( x+364, y+60, DISP_PRIO_IME3, downBtnGraNo[downBtn], 2 );
			btnId[1] = StockDispBuffer( x+404, y+60, DISP_PRIO_IME3, upBtnGraNo[upBtn], 2 );

			btnId[2] = StockDispBuffer( x +64, y+140, DISP_PRIO_IME3, CG_OK_BTN, 2 );
			btnId[3] = StockDispBuffer( x+164, y+140, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_KOSU_WIN, 1 );
		}
	}

	return ret;
}
short shopWindow4ProcNo;
void initShopWindow4( void )
{
	shopWindow4ProcNo = 0;
}

int shopWindow4( void )
{
	static int x, y, w, h;
	static int btnId[2];
	int id;
	int i;
	int ret = 0;
	if( shopWindow4ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 6;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow4ProcNo++;
	}


	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 ){
			if( id == 0 ){
				ret = 1;
			}else if( id == 1 ){
				ret = 2;
			}else if( id == 100 ){
				ret = 100;
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 ){
			for( i = 0; i < sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ); i++ ){
				if( shopWindow4Msg[0][0] != '\0' ){
					StockFontBuffer( x+20, y+20+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow4Msg[i], 0 );
				}
			}

			btnId[0] = StockDispBuffer( x+128, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+256, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}

short shopWindow5ProcNo;

void initShopWindow5( void )
{
	shopWindow5ProcNo = 0;
}

int shopWindow5( void )
{
	static int x, y, w, h;
	static int btnId[2];
	int id;
	int i;
	int ret = 0;

	if( shopWindow5ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 6;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow5ProcNo++;
	}

	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}
		if( id >= 0 ){
			if( id == 0 ){
				ret = 1;
			}else if( id == 1 ){
				ret = 2;
			}else if( id == 100 ){
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 || ret == 1 ){
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 ){
			for( i = 0; i < sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ); i++ ){
				if( shopWindow5Msg[0][0] != '\0' ){
					StockFontBuffer( x+20, y+20+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow5Msg[i], 0 );
				}
			}
			btnId[0] = StockDispBuffer( x+128, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+256, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}
short shopWindow6ProcNo;
void initShopWindow6( void )
{
	shopWindow6ProcNo = 0;
}

int shopWindow6( void )
{
	static int x, y, w, h;
	static int btnId[1];
	int id;
	int i;
	int ret = 0;

	if( shopWindow6ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 6;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow6ProcNo++;
	}
	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 ){
			if( id == 0 ){
				ret = 1;
			}else if( id == 100 ){
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			// ?????????
			for( i = 0; i < sizeof( shopWindow6Msg )/sizeof( shopWindow6Msg[0] ); i++ )
			{
				if( shopWindow6Msg[0][0] != '\0' )
				{
					StockFontBuffer( x+20, y+20+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow6Msg[i], 0 );
				}
			}

			btnId[0] = StockDispBuffer( x+192, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
		}
	}

	return ret;
}


// ??????????
//   ????????
short shopWindow7ProcNo;

// ???
void initShopWindow7( void )
{
	shopWindow7ProcNo = 0;
}


// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ??e
//           3 ... ?????
int shopWindow7( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( shopWindow7ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow7ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			//id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ )
			{
				if( userItem[j+i].name[0] != '\0' )
				{
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							if( userItem[j+i].sealFlag == 0 )
							{
								selId = j+i;
							}
						}
						focusId = j+i;
					}
				}
			}
		}


		// ?q??????
		if( joy_con[0] & JOY_A )
		{
			pushId = 0;
			if( joy_auto[0] & JOY_A )
			{
				id = 0;
			}
		}
		if( pushId == 0 )
		{
			prevBtn = 1;
			if( id == 0 )
			{
				if( shopWondow2Page > 0 )
				{
					shopWondow2Page--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			prevBtn = 0;
		}
		// ???????
		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			nextBtn = 0;
		}


		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		// id = 0, id = 1 ????????????????
		if( id >= 2
		 || selId >= 0 )
		{
			// ?????
			if( id == 2 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopItemNo = selId;
				sealItemCnt = 1;
				if( userItem[selShopItemNo].price + pc.gold > CHAR_getMaxHaveGold() )
				{
					ret = 3;
				}
				else
				{
					ret = 2;
				}
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || (buyOffFlag == 1 && ret == 1) )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			// ????????
			xx = (w-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			// ?????????
			for( i = 0; i < sizeof( shopWindow7Msg )/sizeof( shopWindow7Msg[0] ); i++ )
			{
				if( shopWindow7Msg[0][0] != '\0' )
				{
					StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow7Msg[i], 0 );
				}
			}


			// ???
			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+244, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ ){
				if( strlen( userItem[j+i].name ) == 0 )
					continue;
				color = itemColor[0];
				if( userItem[j+i].sealFlag )
					color = itemColor[1];

				if( (pc.gold + userItem[j+i].price) > CHAR_getMaxHaveGold() )
					color = 7;
				StockFontBuffer( x+34, y+118+i*21,
					FONT_PRIO_FRONT, color, userItem[j+i].name, 0 );
#ifdef _ITEM_PILENUMS
				sprintf( tmsg, "x%d", userItem[j+i].num );
				StockFontBuffer( x+34+250, y+118+i*21,
					FONT_PRIO_FRONT, color, tmsg, 0 );
#endif
					sprintf( tmsg, "%8dS", userItem[j+i].price );
					StockFontBuffer( x+312, y+118+i*21,
						FONT_PRIO_FRONT, color, tmsg, 0 );
			}
			if( focusId >= 0 ){
				StockDispBuffer( x+64, y+350, DISP_PRIO_IME3, userItem[focusId].graNo, 0 );
				for( i = 0; i < sizeof( userItem[0].info )/sizeof( userItem[0].info[0] ); i++ ){
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						userItem[focusId].info[i], 0 );
				}
			}
			if( buyOffFlag == 0 ){
				btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
			}else{
				btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
			}
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_WIN, 1 );
		}
	}
	return ret;
}


// ??????????
//   ?????????????????
short shopWindow8ProcNo;

// ???
void initShopWindow8( void )
{
	shopWindow8ProcNo = 0;
}

int shopWindow8( void )
{
	static int x, y, w, h;
	static int btnId[1];
	int id;
	int i;
	int ret = 0;

	if( shopWindow8ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 6;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow8ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 )
		{
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			// ?????????
			for( i = 0; i < sizeof( shopWindow8Msg )/sizeof( shopWindow8Msg[0] ); i++ )
			{
				if( shopWindow8Msg[0][0] != '\0' )
				{
					StockFontBuffer( x+20, y+20+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow8Msg[i], 0 );
				}
			}

			btnId[0] = StockDispBuffer( x+192, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
		}
	}

	return ret;
}
#ifdef _ITEM_PILENUMS
short shopWindow10ProcNo;
void initShopWindow10( void )
{
	shopWindow10ProcNo = 0;
}

int shopWindow10( void )
{
	static int x, y, w, h;
	static int btnId[4];
	int upBtnGraNo[] = { CG_UP_BTN, CG_UP_BTN_DOWN };
	int downBtnGraNo[] = { CG_DOWN_BTN, CG_DOWN_BTN_DOWN };
	int upBtn, downBtn;
	int id;
	int pushId;
	int i;
	int ret = 0;
	char tmsg[256];

	if( shopWindow10ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
	/*
		w = 5;///440;
		h = 2;//160;
		x = (640-w*70)/2;//(640-w)/2;
		y = (456-h*40)/2;//(456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
*/
		w = 440;
		h = 160;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		shopWindow10ProcNo++;
	}

	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		if( joy_con[0] & JOY_A ){
			pushId = 0;
			if( joy_auto[0] & JOY_A ){
				id = 0;
			}
		}
		if( pushId == 0 ){
			downBtn = 1;
			if( id == 0 ){
				if( sealItemCnt > 1 ){
					sealItemCnt--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}else{
			downBtn = 0;
		}
		if( joy_con[0] & JOY_B ){
			pushId = 1;
			if( joy_auto[0] & JOY_B ){
				id = 1;
			}
		}
		if( pushId == 1 ){
			upBtn = 1;
			if( id == 1 ){
				if( sealItemCnt+1 <= userItem[selShopItemNo].num
				 && (userItem[selShopItemNo].price*(sealItemCnt+1) + pc.gold) <= CHAR_getMaxHaveGold()){
					sealItemCnt++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}else{
			upBtn = 0;
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 2 ){
			if( id == 2 ){
				ret = 1;
			}else if( id == 3 ){
				ret = 2;
			}else if( id == 100 ){
				ret = 100;
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}
		if( ptActMenuWin->hp >= 1 ){//userItem
			int xx;
			xx = (w-strlen( shopWindow3Msg )/2*17)/2;
			StockFontBuffer( x+xx, y+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow3Msg, 0 );
			StockFontBuffer( x+16, y+52, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				userItem[selShopItemNo].name, 0 );
			sprintf( tmsg, "%2d个", sealItemCnt );
			StockFontBuffer( x+300, y+52, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			sprintf( tmsg, "%8dS x %2d = %8dS",
				userItem[selShopItemNo].price,
				sealItemCnt, userItem[selShopItemNo].price*sealItemCnt );
			StockFontBuffer( x+146, y+82, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				tmsg, 0 );
			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+248, y+120, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );
			btnId[0] = StockDispBuffer( x+364, y+60, DISP_PRIO_IME3, downBtnGraNo[downBtn], 2 );
			btnId[1] = StockDispBuffer( x+404, y+60, DISP_PRIO_IME3, upBtnGraNo[upBtn], 2 );

			btnId[2] = StockDispBuffer( x +64, y+120, DISP_PRIO_IME3, CG_OK_BTN, 2 );
			btnId[3] = StockDispBuffer( x+164, y+120, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_KOSU_WIN, 1 );
		}
	}

	return ret;
}
#endif


void initSkillShopWindow1( void );
int skillShopWindow1( void );
void initSkillShopWindow2( void );
int skillShopWindow2( void );
void initSkillShopWindow3( void );
int skillShopWindow3( void );
void initSkillShopWindow4( void );
int skillShopWindow4( void );

void serverWindowType6( void )
{
	int ret;

	// ??????
	if( skillShopWindowProcNo == 0 )
	{
		initSkillShopWindow1();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );	// ????????
	}
	if( skillShopWindowProcNo == 1 )
	{
		ret = skillShopWindow1();
		if( ret == 2 )
		{
			skillShopWindowProcNo = 10;
		}
	}
	if( skillShopWindowProcNo == 10 ){
		initSkillShopWindow2();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 11 ){
		ret = skillShopWindow2();
		if( ret == 1 ){
			skillShopWindowProcNo = 0;
		}else if( ret == 2 ){
			skillShopWindowProcNo = 20;
		}
	}
	if( skillShopWindowProcNo == 20 ){
		initSkillShopWindow3();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 21 ){
		ret = skillShopWindow3();
		if( ret == 1 ){
			skillShopWindowProcNo = 10;
		}else if( ret == 2 ){
			skillShopWindowProcNo = 30;
		}
	}
	if( skillShopWindowProcNo == 30 ){
		initSkillShopWindow4();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 31 ){
		ret = skillShopWindow4();
		if( ret == 1 ){
#if 1
			char data[256];
			char msg[256];
			sprintf( data, "%d|%d|%d|%d",
				selShopSkillNo+1,
				selShopSkillPetNo,
				selShopSkillSlotNo,
				sealSkill[selShopSkillNo].price );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
#endif
			pc.gold -= sealSkill[selShopSkillNo].price;

			windowTypeWN = -1;
		}else if( ret == 2 ){
			skillShopWindowProcNo = 20;
		}
	}
}


#ifdef _CHAR_PROFESSION			// WON ADD ??????????

void initServerWindowProfession( char *data )
{
	char title1[128];
	char msg2[128];
	char skillName[64];
	char info[256];
	int i, j;
	int flag;

	skillShopWindowProcNo = 0;
	shopWondow2Page = 0;

	if( getIntegerToken( data, '|', 1 ) == 0 )
		return;

	getStringToken( data, '|', 2, sizeof( title1 )-1, title1 );
	makeStringFromEscaped( title1 );
	getStringToken( data, '|', 3, sizeof( msg2 )-1, msg2 );
	makeStringFromEscaped( msg2 );

	strncpy( ProfessionShopTitle, title1, sizeof( ProfessionShopTitle )-1 );
	ProfessionShopTitle[sizeof( ProfessionShopTitle )-1] = '\0';

	getStrSplit( (char *)ProfessionShopMsg, msg2,
		sizeof( ProfessionShopMsg[0] ),
			sizeof( ProfessionShopMsg )/sizeof( ProfessionShopMsg[0] ),
				sizeof( ProfessionShopMsg[0] )-1 );

	// Robin fix 20040707 ???????
	//shopWondow2MaxPage = MAX_SKILL_SHOP_SKILL* 9;
	shopWondow2MaxPage = MAX_SKILL_SHOP_SKILL*MAX_SKILL_SHOP_PAGE;

	for( i = 0, j = 0; i < shopWondow2MaxPage; i++ )
	{
		// ????????
		flag = getStringToken( data, '|', 4+i*4, sizeof( skillName )-1, skillName );
		makeStringFromEscaped( skillName );
		if( flag )
		{
			sealSkill[i].name[0] = '\0';
			continue;
		}

		j++;

		if( strlen( skillName ) <= SKILL_NAME_LEN )
			strcpy( sealSkill[i].name, skillName );
		else
			strcpy( sealSkill[i].name, "???" );
		
		// ???
		sealSkill[i].price = getIntegerToken( data, '|', 5+i*4 );
		
		// ???
		getStringToken( data, '|', 6+i*4, sizeof( info )-1, info );
		makeStringFromEscaped( info );
		getStrSplit( (char *)(&sealSkill[i].info), info,
			sizeof( sealSkill[0].info[0] ),
				sizeof( sealSkill[0].info )/sizeof( sealSkill[0].info[0] ),
					sizeof( sealSkill[0].info[0] )-1 );
		
		// ??
		sealSkill[i].icon = getIntegerToken( data, '|', 7+i*4 );

	}
	if( j > 0 )
	{
		shopWondow2MaxPage = (j+7)/MAX_SKILL_SHOP_SKILL;
	}
	else
	{
		shopWondow2MaxPage = 1;
	}
}

void profession_windows( void )
{
	int ret;

	if( skillShopWindowProcNo == 0 )
	{
		initSkillShopWindow1();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 1 )
	{
		ret = profession_windows_1();
		if( ret == 2 )
		{
			skillShopWindowProcNo = 10;
		}
	}
	if( skillShopWindowProcNo == 10 ){
		initSkillShopWindow4();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 11 ){
		ret = profession_windows_2();
		if( ret == 1 ){

			char data[256];
			char msg[256];
			sprintf( data, "%d|%d",
				selShopSkillNo+1,
				sealSkill[selShopSkillNo].price );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;

			// Robin fix 20040707 ???Client??????bug
			//pc.gold -= sealSkill[selShopSkillNo].price;

			windowTypeWN = -1;
		}else if( ret == 2 ){
			skillShopWindowProcNo = 0;
		}
	}

}


int profession_windows_1( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( skillShopWindow1ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		skillShopWindow1ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 )
		{
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( sealSkill[j+i].name[0] != '\0' )
				{
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							if( sealSkill[j+i].price <= pc.gold )
							{
								selId = j+i;
							}
							else
							{
								play_se( 220, 320, 240 );
							}
						}
						focusId = j+i;
					}
				}
			}
		}

		if( joy_con[0] & JOY_A )
		{
			pushId = 0;
			if( joy_auto[0] & JOY_A )
			{
				id = 0;
			}
		}
		if( pushId == 0 )
		{
			prevBtn = 1;
			if( id == 0 )
			{
				if( shopWondow2Page > 0 )
				{
					shopWondow2Page--;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			prevBtn = 0;
		}

		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			nextBtn = 0;
		}

		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 2
		 || selId >= 0 )
		{
			if( id == 2 )
			{
				ret = 1;
				play_se( 203, 320, 240 );
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopSkillNo = selId;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			xx = (w-strlen( ProfessionShopTitle )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, ProfessionShopTitle, 0 );

			for( i = 0; i < sizeof( ProfessionShopMsg )/sizeof( ProfessionShopMsg[0] ); i++ )
			{
				if( ProfessionShopMsg[0][0] != '\0' )
				{
					StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						ProfessionShopMsg[i], 0 );
				}
			}

			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+244, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( strlen( sealSkill[j+i].name ) == 0 )
					continue;

				color = itemColor[0];

				if( sealSkill[j+i].price > pc.gold )
				{
					color = itemColor[1];
				}

				StockFontBuffer( x+34, y+118+i*21,
					FONT_PRIO_FRONT, color, sealSkill[j+i].name, 0 );

				sprintf( tmsg, "%8dS", sealSkill[j+i].price );
				StockFontBuffer( x+312, y+118+i*21,
					FONT_PRIO_FRONT, color, tmsg, 0 );
			}

			if( focusId >= 0 )
			{
				// ??ICON
				StockDispBuffer( x+58, y+340, DISP_PRIO_IME3, sealSkill[focusId].icon, 0 );

				for( i = 0; i < sizeof( sealSkill[0].info )/sizeof( sealSkill[0].info[0] ); i++ )
				{
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						sealSkill[focusId].info[i], 0 );

				}
			}

			btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );

			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_SKILLSHOP_WIN, 1 );
		}
	}

	return ret;
}


int profession_windows_2( void )
{
	static int x, y, w, h;
	static int btnId[2];
	static char msg1[128];
	int id;
	int i;
	int ret = 0;

	if( skillShopWindow4ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 8;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif

		char name[32];
		if( strlen( pc.freeName ) > 0 )
		{
			strcpy( name, pc.freeName );
		}
		else
		{
			strcpy( name, pc.name );
		}

		sprintf( msg1, "让[%s]将[%s]", name,
			sealSkill[selShopSkillNo].name );

		skillShopWindow4ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 )
		{
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 1 )
			{
				ret = 2;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			// ??_?????
			StockFontBuffer( x+20, y+20+0*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				msg1, 0 );
			StockFontBuffer( x+20, y+20+1*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				"学起来吗？", 0 );

			btnId[0] = StockDispBuffer( x+w*64/3, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+w*64/3*2, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}


#endif


#ifdef _NPC_WELFARE_2				// WON ADD ??NPC-2
void initServerWindowProfession2( char *data )
{
	char title1[128];
	char msg2[128];
	char skillName[64];
	char info[256];
	int i, j;
	int flag;

	skillShopWindowProcNo = 0;
	shopWondow2Page = 0;

	if( getIntegerToken( data, '|', 1 ) == 0 )
		return;

	getStringToken( data, '|', 2, sizeof( title1 )-1, title1 );
	makeStringFromEscaped( title1 );
	getStringToken( data, '|', 3, sizeof( msg2 )-1, msg2 );
	makeStringFromEscaped( msg2 );

	strncpy( ProfessionShopTitle2, title1, sizeof( ProfessionShopTitle2 )-1 );
	ProfessionShopTitle2[sizeof( ProfessionShopTitle2 )-1] = '\0';

	getStrSplit( (char *)ProfessionShopMsg2, msg2,
		sizeof( ProfessionShopMsg2[0] ),
			sizeof( ProfessionShopMsg2 )/sizeof( ProfessionShopMsg2[0] ),
				sizeof( ProfessionShopMsg2[0] )-1 );

	shopWondow2MaxPage = MAX_SKILL_SHOP_SKILL* 9;

	for( i = 0, j = 0; i < shopWondow2MaxPage; i++ )
	{
		// ????????
		flag = getStringToken( data, '|', 4+i*4, sizeof( skillName )-1, skillName );
		makeStringFromEscaped( skillName );
		if( flag )
		{
			sealSkill[i].name[0] = '\0';
			continue;
		}

		j++;

		if( strlen( skillName ) <= SKILL_NAME_LEN )
			strcpy( sealSkill[i].name, skillName );
		else
			strcpy( sealSkill[i].name, "???" );
		
		// ???
		sealSkill[i].price = getIntegerToken( data, '|', 5+i*4 );
		
		// ???
		getStringToken( data, '|', 6+i*4, sizeof( info )-1, info );
		makeStringFromEscaped( info );
		getStrSplit( (char *)(&sealSkill[i].info), info,
			sizeof( sealSkill[0].info[0] ),
				sizeof( sealSkill[0].info )/sizeof( sealSkill[0].info[0] ),
					sizeof( sealSkill[0].info[0] )-1 );
		
		// ??
		sealSkill[i].icon = getIntegerToken( data, '|', 7+i*4 );

	}
	if( j > 0 )
	{
		shopWondow2MaxPage = (j+7)/MAX_SKILL_SHOP_SKILL;
	}
	else
	{
		shopWondow2MaxPage = 1;
	}
}

void profession_windows2( void )
{
	int ret;

	if( skillShopWindowProcNo == 0 )
	{
		initSkillShopWindow1();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 1 )
	{
		ret = profession_windows_12();
		if( ret == 2 )
		{
			skillShopWindowProcNo = 10;
		}
	}
	if( skillShopWindowProcNo == 10 ){
		initSkillShopWindow4();
		skillShopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( skillShopWindowProcNo == 11 ){
		ret = profession_windows_22();
		if( ret == 1 ){

			char data[256];
			char msg[256];
			sprintf( data, "%d|%d",
				selShopSkillNo+1,
				sealSkill[selShopSkillNo].price );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;

			pc.gold -= sealSkill[selShopSkillNo].price;

			windowTypeWN = -1;
		}else if( ret == 2 ){
			skillShopWindowProcNo = 0;
		}
	}

}


int profession_windows_12( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( skillShopWindow1ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		skillShopWindow1ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 )
		{
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( sealSkill[j+i].name[0] != '\0' )
				{
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							if( sealSkill[j+i].price <= pc.gold )
							{
								selId = j+i;
							}
							else
							{
								play_se( 220, 320, 240 );
							}
						}
						focusId = j+i;
					}
				}
			}
		}

		if( joy_con[0] & JOY_A )
		{
			pushId = 0;
			if( joy_auto[0] & JOY_A )
			{
				id = 0;
			}
		}
		if( pushId == 0 )
		{
			prevBtn = 1;
			if( id == 0 )
			{
				if( shopWondow2Page > 0 )
				{
					shopWondow2Page--;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			prevBtn = 0;
		}

		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			nextBtn = 0;
		}

		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 2
		 || selId >= 0 )
		{
			if( id == 2 )
			{
				ret = 1;
				play_se( 203, 320, 240 );
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopSkillNo = selId;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			xx = (w-strlen( ProfessionShopTitle2 )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, ProfessionShopTitle2, 0 );

			for( i = 0; i < sizeof( ProfessionShopMsg2 )/sizeof( ProfessionShopMsg2[0] ); i++ )
			{
				if( ProfessionShopMsg2[0] != '\0' )
				{
					StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						ProfessionShopMsg2[i], 0 );
				}
			}

			sprintf( tmsg, "????%s  ??????%3d", pc.profession_class_name, pc.profession_skill_point );
			StockFontBuffer( x+235, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d ?", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( strlen( sealSkill[j+i].name ) == 0 )
					continue;

				color = itemColor[0];

				if( sealSkill[j+i].price > pc.gold )
				{
					color = itemColor[1];
				}

				StockFontBuffer( x+34, y+118+i*21,
					FONT_PRIO_FRONT, color, sealSkill[j+i].name, 0 );

				sprintf( tmsg, "???????%5d", sealSkill[j+i].price );
				StockFontBuffer( x+280, y+118+i*21,
					FONT_PRIO_FRONT, color, tmsg, 0 );
			}

			if( focusId >= 0 )
			{
				// ??ICON
				StockDispBuffer( x+58, y+340, DISP_PRIO_IME3, sealSkill[focusId].icon, 0 );

				for( i = 0; i < sizeof( sealSkill[0].info )/sizeof( sealSkill[0].info[0] ); i++ )
				{
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						sealSkill[focusId].info[i], 0 );

				}
			}

			btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );

			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_SKILLSHOP_WIN, 1 );
		}
	}

	return ret;
}


int profession_windows_22( void )
{
	static int x, y, w, h;
	static int btnId[2];
	static char msg1[128];
	int id;
	int i;
	int ret = 0;

	if( skillShopWindow4ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 8;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif

		char name[32];
		if( strlen( pc.freeName ) > 0 )
		{
			strcpy( name, pc.freeName );
		}
		else
		{
			strcpy( name, pc.name );
		}

		sprintf( msg1, "?[%s]??[%s]", name,
			sealSkill[selShopSkillNo].name );

		skillShopWindow4ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 )
		{
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 1 )
			{
				ret = 2;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			// ??_?????
			StockFontBuffer( x+20, y+20+0*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				msg1, 0 );
			StockFontBuffer( x+20, y+20+1*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				"??????", 0 );

			btnId[0] = StockDispBuffer( x+w*64/3, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+w*64/3*2, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}

#endif





#ifdef _PETSKILL_CANNEDFOOD
void PetSkillShowType1( void )
{
	static int x, y, w, h;
	static int btnId[1];
	int id=-1;
	int i;
	int ret = 0;
	int x1, y1, x2, y2;
	char skillName[128];

	if( PetskillShpwWindowProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 5;
		h = 6;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		PetskillShpwWindowProcNo++;
	}
	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 ){
			if( (id = selGraId( btnId, sizeof( btnId )/sizeof( int ) )) < 0 ){
				for( i = 0; i < MAX_SKILL && i < pet[selShopSkillPetNo].maxSkill; i++ ){
					x1 = x+41;
					y1 = y+33+(i+1)*26;
					x2 = x1 + 238;
					y2 = y1 + 22;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							id = i;
						}
					}
				}
			}else{
				id = 100;
			}

		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 0 ){
			if( id == 100 ){
				ret = 100;
			}else{
				selShopSkillSlotNo = id;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 )
			{
				windowTypeWN = -1;
			}else if( ret == 2 ){
				char data[256];
				char msg[256];
//				sprintf( data, "%d|%d|%d|%d",
				sprintf( data, "%d",
//					selShopSkillNo+1
//					selShopSkillPetNo,
					selShopSkillSlotNo
//					sealSkill[selShopSkillNo].price );
					);
				makeEscapeString( data, msg, sizeof( msg )-1 );
				if( bNewServer)
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
				else
					old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
				pc.gold -= sealSkill[selShopSkillNo].price;
				windowTypeWN = -1;
			}
			return;
		}

		if( ptActMenuWin->hp >= 1 ){
			StockFontBuffer( x+75, y+33,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, "要在哪个位置做呢？", 0 );
			for( i = 0; i < MAX_SKILL && i < pet[selShopSkillPetNo].maxSkill; i++ ){
				sprintf( skillName, "技 %d: ", i+1 );
				if( petSkill[selShopSkillPetNo][i].useFlag != 0 ){
					strcat( skillName, petSkill[selShopSkillPetNo][i].name );
				}
				StockFontBuffer( x+46, y+36+(i+1)*26,
					FONT_PRIO_FRONT, FONT_PAL_WHITE, skillName, 0 );
			}
			btnId[0] = StockDispBuffer( x+160, y+256, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
		}
	}
	return;
}
#endif


void initSkillShopWindow1( void )
{
	skillShopWindow1ProcNo = 0;
}

int skillShopWindow1( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( skillShopWindow1ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		skillShopWindow1ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 )
		{
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );


			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( sealSkill[j+i].name[0] != '\0' )
				{
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							if (sealSkill[j + i].price <= pc.gold)
							{
								selId = j+i;
							}
							else
							{
								play_se( 220, 320, 240 );
							}
						}
						focusId = j+i;
					}
				}
			}
		}

		if( joy_con[0] & JOY_A )
		{
			pushId = 0;
			if( joy_auto[0] & JOY_A )
			{
				id = 0;
			}
		}
		if( pushId == 0 )
		{
			prevBtn = 1;
			if( id == 0 )
			{
				if( shopWondow2Page > 0 )
				{
					shopWondow2Page--;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			prevBtn = 0;
		}

		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );
				}
			}
		}
		else
		{
			nextBtn = 0;
		}

		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 2
		 || selId >= 0 )
		{
			if( id == 2 )
			{
				ret = 1;
				play_se( 203, 320, 240 );
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopSkillNo = selId;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			xx = (w-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			for( i = 0; i < sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ); i++ )
			{
				if( shopWindow2Msg[0][0] != '\0' )
				{
					StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow2Msg[i], 0 );
				}
			}

			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer(x + 244, y + 84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0);

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SKILL_SHOP_SKILL;
			for( i = 0; i < MAX_SKILL_SHOP_SKILL; i++ )
			{
				if( strlen( sealSkill[j+i].name ) == 0 )
					continue;

				color = itemColor[0];

				if (sealSkill[j + i].price > pc.gold)
					color = itemColor[1];

				StockFontBuffer(x + 34, y + 118 + i * 21, FONT_PRIO_FRONT, color, sealSkill[j + i].name, 0);

				sprintf(tmsg, "%8dS", sealSkill[j + i].price);
				StockFontBuffer(x + 312, y + 118 + i * 21, FONT_PRIO_FRONT, color, tmsg, 0);

			}

			if( focusId >= 0 )
			{
				for( i = 0; i < sizeof( sealSkill[0].info )/sizeof( sealSkill[0].info[0] ); i++ )
				{
					StockFontBuffer( x+27, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						sealSkill[focusId].info[i], 0 );
				}
			}

			btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );

			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_SKILLSHOP_WIN, 1 );
		}
	}

	return ret;
}

short skillShopWindow2ProcNo;

void initSkillShopWindow2( void )
{
	skillShopWindow2ProcNo = 0;
}

// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ??e
int skillShopWindow2( void )
{
	static int x, y, w, h;
	static int btnId[1];
	int id;
	int i, j;
	int ret = 0;
	int x1, y1, x2, y2;
	int len;

	if( skillShopWindow2ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 7;
		h = 5;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		skillShopWindow2ProcNo++;
	}

	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );

			for( i = 0, j = 0; i < MAX_PET; i++ )
			{
				// ??????????????????
				if( pet[i].useFlag != 0 )
				{
					x1 = x+41;
					y1 = y+25+(j+1)*26;
					x2 = x1 + 366;
					y2 = y1 + 22;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							id = i + 1;
						}
					}
					j++;
				}
			}
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 0 )
		{
			// ?????
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopSkillPetNo = id;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( x+145, y+25,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, "要让谁学呢？", 0 );
			for( i = 0, j = 0; i < MAX_PET; i++ )
			{
				if( pet[i].useFlag != 0 )
				{
					len = strlen( pet[i].freeName );
					if( len > 0 )
					{
						strcpy( msgWN[1], pet[i].freeName );
					}
					else
					{
						len = strlen( pet[i].name );
						strcpy( msgWN[1], pet[i].name );
					}
#if 0
					StockFontBuffer( x+88+(144-len*9)/2, y+28+(j+1)*26,
#else
					StockFontBuffer( x+62, y+28+(j+1)*26,
#endif
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );


					sprintf( msgWN[1], "LV.%d", pet[i].level );
					StockFontBuffer( x+260, y+28+(j+1)*26,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					sprintf( msgWN[1], "MaxHP %d", pet[i].maxHp );
					StockFontBuffer( x+314, y+28+(j+1)*26,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[1], 0 );

					j++;
				}
			}
			btnId[0] = StockDispBuffer( x+w*64/2, y+204, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
		}
	}

	return ret;
}


// ???????  ?????k???????
short skillShopWindow3ProcNo;
// ???
void initSkillShopWindow3( void )
{
	skillShopWindow3ProcNo = 0;
}

// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ??e
int skillShopWindow3( void )
{
	static int x, y, w, h;
	static int btnId[1];
	int id;
	int i;
	int ret = 0;
	int x1, y1, x2, y2;
	char skillName[128];

	if( skillShopWindow3ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 5;
		h = 6;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		skillShopWindow3ProcNo++;
	}

	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );

			for( i = 0; i < MAX_SKILL && i < pet[selShopSkillPetNo-1].maxSkill; i++ )
			{
				x1 = x+41;
				y1 = y+33+(i+1)*26;
				x2 = x1 + 238;
				y2 = y1 + 22;
				if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
				{
					if( mouse.onceState & MOUSE_LEFT_CRICK )
					{
						id = i + 1;
					}
				}
			}
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		if( id >= 0 )
		{
			// ?????
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopSkillSlotNo = id;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( x+75, y+33,
				FONT_PRIO_FRONT, FONT_PAL_WHITE, "要在哪个位置做呢？", 0 );
			for( i = 0; i < MAX_SKILL && i < pet[selShopSkillPetNo-1].maxSkill; i++ )
			{
				sprintf( skillName, "技 %d: ", i+1 );
				if( petSkill[selShopSkillPetNo-1][i].useFlag != 0 )
				{
					strcat( skillName, petSkill[selShopSkillPetNo-1][i].name );
				}
				StockFontBuffer( x+46, y+36+(i+1)*26,
					FONT_PRIO_FRONT, FONT_PAL_WHITE, skillName, 0 );
			}
			btnId[0] = StockDispBuffer( x+160, y+256, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
		}
	}

	return ret;
}


void initSkillShopWindow4( void )
{
	skillShopWindow4ProcNo = 0;
}


int skillShopWindow4( void )
{
	static int x, y, w, h;
	static int btnId[2];
	static char msg1[128];
	int id;
	int i;
	int ret = 0;

	if( skillShopWindow4ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 8;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif

		char name[32];
		if( strlen( pet[selShopSkillPetNo-1].freeName ) > 0 )
		{
			strcpy( name, pet[selShopSkillPetNo-1].freeName );
		}
		else
		{
			strcpy( name, pet[selShopSkillPetNo-1].name );
		}

		sprintf( msg1, "让[%s]将[%s]", name,
			sealSkill[selShopSkillNo].name );

		skillShopWindow4ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 )
		{
			if( id == 0 )
			{
				ret = 1;
			}
			else
			if( id == 1 )
			{
				ret = 2;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}


		if( ptActMenuWin->hp >= 1 )
		{
			// ??_?????
			StockFontBuffer( x+20, y+20+0*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				msg1, 0 );
			StockFontBuffer( x+20, y+20+1*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				"学起来吗？", 0 );

			btnId[0] = StockDispBuffer( x+w*64/3, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+w*64/3*2, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}




#if 1


// ?????????

void initPoolShopWindow1( void );
int poolShopWindow1( void );
void initPoolShopWindow2( void );
int poolShopWindow2( void );
void initPoolShopWindow3( void );
int poolShopWindow3( void );

void initPoolShopWindow4( void );
int poolShopWindow4( void );


// ????????????M???
void serverWindowType7( void )
{
	int ret;

	if( shopWindowProcNo == 0 ){
		initPoolShopWindow1();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 1 ){
		ret = poolShopWindow1();
	}

	if( shopWindowProcNo == 10 ){
		initPoolShopWindow2();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 11 ){
		ret = poolShopWindow2();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "0" );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			windowTypeWN = -1;
		}else if( ret == 2 ){
			shopWindowProcNo = 20;
		}
	}

	if( shopWindowProcNo == 20 ){
		initPoolShopWindow3();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );	// ????????
	}
	if( shopWindowProcNo == 21 ){
		ret = poolShopWindow3();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "%d", poolItem[selShopItemNo].tbl );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;

			pc.gold -= poolItem[selShopItemNo].price;
			restPoolSlot--;
			if( restPoolSlot < 0 )
				restPoolSlot = 0;

			poolItem[selShopItemNo].name[0] = '\0';
			poolItem[selShopItemNo].info[0][0] = '\0';
			poolItem[selShopItemNo].info[1][0] = '\0';
			poolItem[selShopItemNo].info[2][0] = '\0';
#ifdef _ITEM_PILENUMS
			poolItem[selShopItemNo].num = 0;
#endif
			int i, j, k;
			for( i = 0; i < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE-1; i++ ){
				if( poolItem[i].name[0] == '\0' ){
					for( j = i+1; j < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE; j++ ){
						if( poolItem[j].name[0] != '\0' ){
							strcpy( poolItem[i].name, poolItem[j].name );
							poolItem[i].poolFlag = poolItem[j].poolFlag;
							poolItem[i].level = poolItem[j].level;
							poolItem[i].price = poolItem[j].price;
							poolItem[i].graNo = poolItem[j].graNo;
							poolItem[i].tbl = poolItem[j].tbl;
#ifdef _ITEM_PILENUMS
							poolItem[i].num = poolItem[j].num;
#endif

							for( k = 0; k < 3; k++ ){
								strcpy( poolItem[i].info[k], poolItem[j].info[k] );
							}
							poolItem[j].name[0] = '\0';
							break;
						}
					}
				}
			}
			for( i = 0; i < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE; i++ ){
				if( poolItem[i].name[0] == '\0' )
					break;
			}
			if( i > 0 ){
				shopWondow2MaxPage = (i+7)/MAX_POOL_SHOP_ITEM;
			}else{
				shopWondow2MaxPage = 1;
			}
			if( shopWondow2Page >= shopWondow2MaxPage )
				shopWondow2Page = shopWondow2MaxPage-1;

			shopWindowProcNo = 10;

		}else if( ret == 2 ){
			shopWindowProcNo = 10;
		}
	}

	if( shopWindowProcNo == 100 ){
		initPoolShopWindow4();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );	// ????????
	}
	if( shopWindowProcNo == 101 ){
		ret = poolShopWindow4();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "0" );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			windowTypeWN = -1;
		}else if( ret == 2 ){
			shopWindowProcNo = 110;
		}else if( ret == 3 ){
			shopWindowProcNo = 120;
		}
	}

	if( shopWindowProcNo == 110 ){
		initPoolShopWindow3();
		shopWindowProcNo++;
		play_se( 202, 320, 240 );
	}
	if( shopWindowProcNo == 111 ){
		ret = poolShopWindow3();
		if( ret == 1 ){
			char data[256];
			char msg[256];
			sprintf( data, "%d", selShopItemNo+1);
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;

			poolItem[selShopItemNo].name[0] = '\0';
			poolItem[selShopItemNo].info[0][0] = '\0';
			poolItem[selShopItemNo].info[1][0] = '\0';
			poolItem[selShopItemNo].info[2][0] = '\0';
#ifdef _ITEM_PILENUMS
			poolItem[selShopItemNo].num = 0;
#endif
			int i, j, k;
				for( i = 0; i < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE-1; i++ ){
					if( poolItem[i].name[0] == '\0' ){
						for( j = i+1; j < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE; j++ ){
							if( poolItem[j].name[0] != '\0' ){
								strcpy( poolItem[i].name, poolItem[j].name );
								poolItem[i].poolFlag = poolItem[j].poolFlag;
								poolItem[i].level = poolItem[j].level;
								poolItem[i].price = poolItem[j].price;
								poolItem[i].graNo = poolItem[j].graNo;
								poolItem[i].tbl = poolItem[j].tbl;
#ifdef _ITEM_PILENUMS
								poolItem[i].num = poolItem[j].num;
#endif
								for( k = 0; k < 3; k++ ){
									strcpy( poolItem[i].info[k], poolItem[j].info[k] );
								}
								poolItem[j].name[0] = '\0';
								break;
							}
						}
					}
				}
			for( i = 0; i < MAX_POOL_SHOP_ITEM*MAX_POOL_SHOP_PAGE; i++ ){
				if( poolItem[i].name[0] == '\0' )
					break;
			}
			if( i > 0 ){
				shopWondow2MaxPage = (i+7)/MAX_POOL_SHOP_ITEM;
			}else{
				shopWondow2MaxPage = 1;
			}
			if( shopWondow2Page >= shopWondow2MaxPage )
				shopWondow2Page = shopWondow2MaxPage-1;

			shopWindowProcNo = 100;

		}else if( ret == 2 ){
			shopWindowProcNo = 100;
		}
	}
}


// ?????????????????????

short poolShopWindow1ProcNo;

// ???
void initPoolShopWindow1( void )
{
	poolShopWindow1ProcNo = 0;
}


// ???
//   ???k??0 ... ???
//           1 ... ???
//           2 ... ?????
//           3 ... ???
int poolShopWindow1( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id = 0;
	int i;
	int ret = 0;

	if( poolShopWindow1ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 4;
		h = 3;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		poolShopWindow1ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		if( ptActMenuWin->hp >= 1 )
		{
			// ?????k???R?e
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			ret = id + 1;
		}

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			ret = 3;	// ????
			wnCloseFlag = 0;
		}

		if( 0 <= id && id <= 2 )
		{
			char data[256];
			char msg[256];
			sprintf( data, "%d", ret );
			makeEscapeString( data, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
		}

		if( id >= 0 )
		{
			if( ret == 3 )
			{
				play_se( 203, 320, 240 );	// ??????????
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			windowTypeWN = -1;
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			xx = (w*64-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+12, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			xx = (w*64-strlen( shopWindow1Msg )/2*17)/2;
			StockFontBuffer( x+xx, y+32, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Msg, 0 );

			btnId[0] =
				StockDispBuffer( x+w*64/2, y+16/2+66, DISP_PRIO_IME3, CG_SEAL_BTN, 2 );
			btnId[1] =
				StockDispBuffer( x+w*64/2, y+16/2+90, DISP_PRIO_IME3, CG_BUY_BTN, 2 );
			btnId[2] =
				StockDispBuffer( x+w*64/2, y+16/2+114, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
		}
	}

	return ret;
}


// ???????????????????

short poolShopWindow2ProcNo;

// ???
void initPoolShopWindow2( void )
{
	poolShopWindow2ProcNo = 0;
}


// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ??e
int poolShopWindow2( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;

	if( poolShopWindow2ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		// ????????
		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		poolShopWindow2ProcNo++;
	}


	if( ptActMenuWin != NULL ){
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 ){
			// ?????k???R?e
			//id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_POOL_SHOP_ITEM; i++ ){
				if( poolItem[j+i].name[0] != '\0' ){
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){
						if( mouse.onceState & MOUSE_LEFT_CRICK ){
							// ?????K???????k???
							if( poolItem[j+i].price <= pc.gold
							 && poolItem[j+i].poolFlag == 0
							 && restPoolSlot > 0 ){
								selId = j+i;
							}else{
								play_se( 220, 320, 240 );	// ???
							}
						}
						focusId = j+i;
					}
				}
			}
		}


		// ?q??????
		if( joy_con[0] & JOY_A ){
			pushId = 0;
			if( joy_auto[0] & JOY_A ){
				id = 0;
			}
		}
		if( pushId == 0 ){
			prevBtn = 1;
			if( id == 0 ){
				if( shopWondow2Page > 0 ){
					shopWondow2Page--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			prevBtn = 0;
		}
		// ???????
		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			nextBtn = 0;
		}


		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		// id = 0, id = 1 ????????????????
		if( id >= 2
		 || selId >= 0 )
		{
			// ?????
			if( id == 2 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopItemNo = selId;
				ret = 2;
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			// ????????
			xx = (w-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			// ?????????
			if( restPoolSlot > 0 )
			{
				for( i = 0; i < sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ); i++ )
				{
					if( shopWindow2Msg[i][0] != '\0' )
					{
						StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
							shopWindow2Msg[i], 0 );
					}
				}
			}
			else
			{
				for( i = 0; i < sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ); i++ )
				{
					if( shopWindow4Msg[i][0] != '\0' )
					{
						StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
							shopWindow4Msg[i], 0 );
					}
				}
			}


			// ???
			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+244, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ )
			{
				// ?????????????
				if( strlen( poolItem[j+i].name ) == 0 )
					continue;

				// ???????
				color = itemColor[0];
				// ?????????????????
				if( poolItem[j+i].poolFlag )
					color = itemColor[1];
				// ??????????
				if( poolItem[j+i].price > pc.gold )
				{
					color = itemColor[1];
				}
				// ??????????????
				if( restPoolSlot <= 0 )
				{
					color = itemColor[1];
				}

				// ??????
				StockFontBuffer( x+34, y+118+i*21,
					FONT_PRIO_FRONT, color, poolItem[j+i].name, 0 );
#ifdef _ITEM_PILENUMS
				sprintf( tmsg, "x%d", poolItem[j+i].num ); 
				StockFontBuffer( x+34+250, y+118+i*21, FONT_PRIO_FRONT, color, tmsg, 0 );
#endif
				// ??
				sprintf( tmsg, "%8dS", poolItem[j+i].price );
				StockFontBuffer( x+312, y+118+i*21,
					FONT_PRIO_FRONT, color, tmsg, 0 );
			}

			if( focusId >= 0 )
			{
				// ??????
				StockDispBuffer( x+64, y+350, DISP_PRIO_IME3, poolItem[focusId].graNo, 0 );

				for( i = 0; i < sizeof( sealItem[0].info )/sizeof( poolItem[0].info[0] ); i++ )
				{
					// ?C??
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						poolItem[focusId].info[i], 0 );
				}
			}

			// ?????
			btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );

			// ?????
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_WIN, 1 );
		}
	}

	return ret;
}


// ???????????????_?????
short poolShopWindow3ProcNo;

// ???
void initPoolShopWindow3( void )
{
	poolShopWindow3ProcNo = 0;
}


// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ???
int poolShopWindow3( void )
{
	static int x, y, w, h;
	static int btnId[2];
	int id;
	int i;
	int ret = 0;

	if( poolShopWindow3ProcNo == 0 ){
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ ){
			btnId[i] = -2;
		}
		w = 6;
		h = 2;
		x = (640-w*64)/2;
		y = (456-h*48)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		poolShopWindow3ProcNo++;
	}


	if( ptActMenuWin != NULL ){
		id = -1;
		if( ptActMenuWin->hp >= 1 )	{
			id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
		}
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){
			id = 100;
			wnCloseFlag = 0;
		}


		if( id >= 0 ){
			if( id == 0 ){
				ret = 1;
			}else
			if( id == 1 ){
				ret = 2;
			}
			else
			if( id == 100 ){
				ret = 100;
			}
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100 ){
				windowTypeWN = -1;
			}
			return ret;
		}
		if( ptActMenuWin->hp >= 1 ){
			for( i = 0; i < sizeof( shopWindow5Msg )/sizeof( shopWindow5Msg[0] ); i++ )	{
				if( shopWindow5Msg[i][0] != '\0' ){
					StockFontBuffer( x+20, y+20+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						shopWindow5Msg[i], 0 );
				}
			}

			btnId[0] = StockDispBuffer( x+128, y+72, DISP_PRIO_IME3, CG_YES_BTN, 2 );
			btnId[1] = StockDispBuffer( x+256, y+72, DISP_PRIO_IME3, CG_NO_BTN, 2 );
		}
	}

	return ret;
}

short poolShopWindow4ProcNo;

// ???
void initPoolShopWindow4( void )
{
	poolShopWindow4ProcNo = 0;
}


// ???
//   ???k??0 ... ???
//           1 ... ??
//           2 ... ??e
//           3 ... ?????????
int poolShopWindow4( void )
{
	static int x, y, w, h;
	static int btnId[3];
	int id;
	int pushId;
	int focusId;
	int selId;
	int i, j;
	int ret = 0;
	char tmsg[256];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int color;
	int prevBtn, nextBtn;
	int x1, y1, x2, y2;
	short itemMaxFlag;

	if( poolShopWindow4ProcNo == 0 )
	{
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		// ????????
		w = 432;
		h = 428;
		x = (640-w)/2;
		y = (456-h)/2;
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		poolShopWindow4ProcNo++;
	}


	if( ptActMenuWin != NULL )
	{
		id = -1;
		pushId = -1;
		focusId = -1;
		selId = -1;

		if( ptActMenuWin->hp >= 1 )
		{
			// ????????????????
			for( i = MAX_ITEMSTART, j = 0; i < MAX_ITEM; i++ )
			{
				if( pc.item[i].useFlag )
				{
					j++;
				}
			}
			itemMaxFlag = 0;
			if( j >= MAX_ITEM - MAX_ITEMSTART )
				itemMaxFlag = 1;

			// ?????k???R?e
			//id = selGraId( btnId, sizeof( btnId )/sizeof( int ) );
			id = selRepGraId( btnId, sizeof( btnId )/sizeof( int ) );
			pushId = pushGraId( btnId, sizeof( btnId )/sizeof( int ) );

			j = shopWondow2Page*MAX_POOL_SHOP_ITEM;
			for( i = 0; i < MAX_POOL_SHOP_ITEM; i++ )
			{
				if( poolItem[j+i].name[0] != '\0' )
				{
					x1 = x+32;
					y1 = y+i*21+116;
					x2 = x1 + 372;
					y2 = y1 + 20;
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{
							if( poolItem[j+i].poolFlag == 0
							 && !itemMaxFlag )
							{
								selId = j+i;
							}
							else
							{
								play_se( 220, 320, 240 );	// ???
							}
						}
						focusId = j+i;
					}
				}
			}
		}


		// ?q??????
		if( joy_con[0] & JOY_A )
		{
			pushId = 0;
			if( joy_auto[0] & JOY_A )
			{
				id = 0;
			}
		}
		if( pushId == 0 )
		{
			prevBtn = 1;
			if( id == 0 )
			{
				if( shopWondow2Page > 0 )
				{
					shopWondow2Page--;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			prevBtn = 0;
		}
		// ???????
		if( joy_con[0] & JOY_B )
		{
			pushId = 1;
			if( joy_auto[0] & JOY_B )
			{
				id = 1;
			}
		}
		if( pushId == 1 )
		{
			nextBtn = 1;
			if( id == 1 )
			{
				if( shopWondow2Page+1 < shopWondow2MaxPage )
				{
					shopWondow2Page++;
					play_se( 217, 320, 240 );	// ?????
				}
			}
		}
		else
		{
			nextBtn = 0;
		}


		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			wnCloseFlag = 0;
		}

		// id = 0, id = 1 ????????????????
		if( id >= 2
		 || selId >= 0 )
		{
			// ?????
			if( id == 2 )
			{
				ret = 1;
			}
			else
			if( id == 100 )
			{
				ret = 100;
			}
			else
			{
				selShopItemNo = selId;
#if 0
				if( ????????? )
				{
					ret = 3;
				}
				else
#endif
				{
					ret = 2;
				}
			}

			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			if( ret == 100
			 || ret == 1 )
			{
				windowTypeWN = -1;
			}
			return ret;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			int xx;

			// ????????
			xx = (w-strlen( shopWindow1Title )/2*17)/2;
			StockFontBuffer( x+xx, y+18, FONT_PRIO_FRONT, FONT_PAL_WHITE, shopWindow1Title, 0 );

			// ?????????
			if( !itemMaxFlag )
			{
				for( i = 0; i < sizeof( shopWindow2Msg )/sizeof( shopWindow2Msg[0] ); i++ )
				{
					if( shopWindow2Msg[0][0] != '\0' )
					{
						StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
							shopWindow2Msg[i], 0 );
					}
				}
			}
			else
			{
				for( i = 0; i < sizeof( shopWindow4Msg )/sizeof( shopWindow4Msg[0] ); i++ )
				{
					if( shopWindow4Msg[0][0] != '\0' )
					{
						StockFontBuffer( x+18, y+48+i*18, FONT_PRIO_FRONT, FONT_PAL_WHITE,
							shopWindow4Msg[i], 0 );
					}
				}
			}


			// ???
			sprintf( tmsg, "金钱 %8dS", pc.gold );
			StockFontBuffer( x+244, y+84, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			// ?????v?
			btnId[0] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[1] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
			sprintf( tmsg, "%2d/%2d 页", shopWondow2Page+1, shopWondow2MaxPage );
			StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, tmsg, 0 );

			j = shopWondow2Page*MAX_SHOP_ITEM;
			for( i = 0; i < MAX_SHOP_ITEM; i++ ){
				if( strlen( poolItem[j+i].name ) == 0 )
					continue;

				color = itemColor[0];
				if( poolItem[j+i].poolFlag )
					color = itemColor[1];
				if( itemMaxFlag )
					color = itemColor[1];
				StockFontBuffer( x+34, y+118+i*21,
					FONT_PRIO_FRONT, color, poolItem[j+i].name, 0 );

#ifdef _ITEM_PILENUMS
				char tmsg[256];
				sprintf( tmsg, "x%d", poolItem[j+i].num ); 
				StockFontBuffer( x+34+250, y+118+i*21, FONT_PRIO_FRONT, color, tmsg, 0 );
#endif
			}

			if( focusId >= 0 ){
				StockDispBuffer( x+64, y+350, DISP_PRIO_IME3, poolItem[focusId].graNo, 0 );

				for( i = 0; i < sizeof( poolItem[0].info )/sizeof( poolItem[0].info[0] ); i++ ){
					StockFontBuffer( x+120, y+308+i*20, FONT_PRIO_FRONT, FONT_PAL_WHITE,
						poolItem[focusId].info[i], 0 );
				}
			}

			if( buyOffFlag == 0 ){
				btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_RETURN_BTN, 2 );
			}else{
				btnId[2] = StockDispBuffer( x+216, y+402, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
			}
			StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_WIN, 1 );
		}
	}

	return ret;
}



#endif




// ????????
int produce_logout()
{
	switch(produce_vct_no){
	case 0:
		// ?????????????? 
		CopyBackBuffer();
		// ??????????????????
		BackBufferDrawType = DRAW_BACK_PRODUCE; 
		// ?????????????
		fade_out_bgm();
		// ????????
		NowTime = TimeGetTime();
		produce_vct_no++;
		// ???????
		InitMenu();
		break;
	case 1:
		// ????
		if( DrawProduce( PRODUCE_BRAN_SMALL ) == true ){
			BackBufferDrawType = DRAW_BACK_NORMAL; 
			// ????????????
			PaletteChange( DEF_PAL, 0 );
			return 1;		//??????
		}
//		MenuProc();	// ??????
//		//ImeProc();	// ???????
		break;
	}
	return 0;		//????
}




void StockDispHLine( int x, int y, int width )
{
	int i=0;
	for( i=0 ; i<width ; i++ )
		StockDispBuffer( x+32+(i*64), y+24, DISP_PRIO_IME3, 26022, 0 );
}

void StockDispVLine( int x, int y, int height )
{
	int i=0;
	for( i=0 ; i<height ; i++ )
		StockDispBuffer( x+32, y+24+(i*48), DISP_PRIO_IME3, 26024, 0 );
}

// Robin 0521 Family Add
static ACTION *pActPet10;
static int familyPetIndex = 0;
static char familyName[STR_BUFFER_SIZE];
int familyElf = 2;
STR_BUFFER familyNameInput;
STR_BUFFER familyRuleInput;
STR_BUFFER familyRuleShow;


static char familySpriteName[3][40] = {
	"光明精灵",
	"黑暗精灵",
	"        ",
};

/*
static char StrYesNo[2][10] = {
	"?Ρ???",
	"????",
};
*/
static int joinflg = 1;

// Robin 0518 Family
void initFamilyAddWN( char *data )
{
	makeStringFromEscaped( data );
	DeathAction( pActPet10 );
	pActPet10 = NULL;
	familyElf = 2;
}

void closeFamilyAddWN( void )
{
	play_se( 203, 320, 240 );
	DeathAction( ptActMenuWin );
	DeathAction( pActPet10 );
	pActPet10 = NULL;
	ptActMenuWin = NULL;
	windowTypeWN = -1;
	GetKeyInputFocus( &MyChatBuffer );
}


// ???????? ??? new
void familyAddWN( void )
{
	static int x=0, y=0, w=0, h=0;
	static int btnId[5];
	static int fontId[5];
	static int prevBtn =0, nextBtn =0;
	static int Inputforce = 0;
	char buf[1024], buf2[1024], buf3[1024];

	int selBtnId;
	int selFontBtnId;
	int i;
	int ret = 0;
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };


	if( CheckMenuFlag()
	 || joy_trg[ 0 ] & JOY_ESC
	 || actBtn == 1
	 || menuBtn == 1
	 || disconnectServerFlag == true
	 || wnCloseFlag == 1 )
	{
		closeFamilyAddWN();
		return;
	}
	
	
	// ??????
	if( ptActMenuWin == NULL )
	{
		//initSkillShopWindow1();
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		w=571; h=295;
		x = (640-w)/2;
		y = (480-h)/2;

		Inputforce = 0;

		familyRuleInput.buffer[0] = NULL;
		familyRuleInput.cnt = 0;
		familyRuleInput.cursor = 0;
		familyRuleInput.len = 120;
		familyRuleInput.lineLen = 46;
		familyRuleInput.lineDist = 30;
		familyRuleInput.color = 0;
		familyRuleInput.x = x+20;
		familyRuleInput.y = y+160;
		familyRuleInput.fontPrio = FONT_PRIO_FRONT;

		familyNameInput.buffer[0] = NULL;
		familyNameInput.cnt = 0;
		familyNameInput.cursor = 0;
		familyNameInput.len = 16;
		familyNameInput.color = 0;
		familyNameInput.x = x+1000;
		familyNameInput.y = y+40;
		familyNameInput.fontPrio = FONT_PRIO_FRONT;


		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		play_se( 202, 320, 240 );	// ????????	

	}
	else if( ptActMenuWin->hp >= 1 )
	{
		

		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );
		selFontBtnId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( mouse.onceState & MOUSE_LEFT_CRICK )		
		{
			switch( selBtnId )
			{
				case 0:
					//
					if( !familyNameInput.buffer[0] ) {
						StockChatBufferLine( "请输入家族名称！", FONT_PAL_WHITE );
						break;
					}
					if( !familyRuleInput.buffer[0] ) {
						StockChatBufferLine( "请输入家族主旨！", FONT_PAL_WHITE );
						break;
					}
					if( familyElf == 2 ) {
						StockChatBufferLine( "请选择守护精灵！", FONT_PAL_WHITE );
						break;
					}
					if( pet[familyPetIndex].useFlag == NULL ) {
						StockChatBufferLine( "请选择家族守护兽！", FONT_PAL_WHITE );
						break;
					}

					makeEscapeString( familyNameInput.buffer, buf2, sizeof(buf2) );
					makeEscapeString( familyRuleInput.buffer, buf3, sizeof(buf3) );
					sprintf( buf, "A|%s|%d|%d|%s"
						, buf2, familyPetIndex, familyElf, buf3);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
					closeFamilyAddWN();
					return;
					break;
				case 1:
					closeFamilyAddWN();
					return;
					break;
				case 2:
					prevBtn = true;
					i =0;
					while(1) {
						familyPetIndex--;
						if( familyPetIndex <0 ) familyPetIndex =4;
						if( pet[familyPetIndex].useFlag != NULL)
							break;
						if( (i++) >5 ) {
							familyPetIndex = -1;
							break;
						}
					}
					if( pActPet10 != NULL ) {
						DeathAction( pActPet10 );
						pActPet10 = NULL;
					}
					break;
				case 3:
					nextBtn = true;
					i=0;
					while(1) {
						familyPetIndex++;
						if( familyPetIndex >=5 ) familyPetIndex =0;
						if( pet[familyPetIndex].useFlag != NULL)
							break;
						if( (i++) >5 ) {
							familyPetIndex = -1;
							break;
						}					
					}
					if( pActPet10 != NULL ) {
						DeathAction( pActPet10 );
						pActPet10 = NULL;
					}

					break;

			}
			switch( selFontBtnId )
			{
				case 0:
					Inputforce = 1;
					break;
				case 1:
					if( familyElf == 0)	familyElf = 1;
					else	familyElf = 0;
					break;
				case 2:
					Inputforce = 2;
					break;

			}
		
		}

		if( mouse.state & MOUSE_LEFT_CRICK && prevBtn == true )	
			prevBtn = true;
		else	prevBtn = false;
 
		if( mouse.state & MOUSE_LEFT_CRICK && nextBtn == true )
			nextBtn = true;
		else	nextBtn = false;

		btnId[0] = StockDispBuffer( x+90, y+274, DISP_PRIO_IME3, CG_OK_BTN, 2 );
		btnId[1] = StockDispBuffer( x+270, y+274, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
		btnId[2] = StockDispBuffer( x+435, y+270, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
		btnId[3] = StockDispBuffer( x+495, y+270, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );

		// ?????????
		fontId[0] = StockFontBuffer( x+100, y+40, FONT_PRIO_FRONT, FONT_PAL_WHITE, "                  ", 2 );
		// ?峤?????
		StockFontBuffer( x+100, y+70, FONT_PRIO_FRONT, FONT_PAL_WHITE, pc.name, 0 );
		// ????
		StockFontBuffer( x+100, y+100, FONT_PRIO_FRONT, FONT_PAL_WHITE, "??", 0 );
		// pet
		if( pet[familyPetIndex].useFlag != NULL )
			if( pet[familyPetIndex].freeName[0] == NULL )
				sprintf( buf, " %s", pet[familyPetIndex].name );
			else
				sprintf( buf, " %s", pet[familyPetIndex].freeName );
		else
			sprintf( buf, "" );
		StockFontBuffer( x+436, y+80, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
		
		// sprite
		sprintf( buf, "  %s  ", familySpriteName[familyElf]);
		fontId[1] = StockFontBuffer( x+436, y+36, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 2 );

		// rule
		sprintf( buf, "%46s", "" );
		fontId[2] = StockFontBuffer( x+20, y+160, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 2 );

		familyNameInput.x = x+100;
		familyNameInput.y = y+40;
		familyRuleInput.x = x+20;
		familyRuleInput.y = y+160;

		if( Inputforce == 1 ) {
			StockFontBuffer2( &familyNameInput );
			GetKeyInputFocus( &familyNameInput );
			if( familyRuleInput.buffer[0] != NULL )
				StockFontBuffer2( &familyRuleInput );
		} else if( Inputforce == 2 ) {
			StockFontBuffer2( &familyRuleInput );
			GetKeyInputFocus( &familyRuleInput );
			if( familyNameInput.buffer[0] != NULL )
				StockFontBuffer2( &familyNameInput );
		} else {
			GetKeyInputFocus( &MyChatBuffer );
			if( familyRuleInput.buffer[0] != NULL )
				StockFontBuffer2( &familyRuleInput );
			if( familyNameInput.buffer[0] != NULL )
				StockFontBuffer2( &familyNameInput );
		}


		if( pActPet10 == NULL && (pet[familyPetIndex].useFlag != NULL) )
//			pActPet10 = MakeAnimDisp( x+460, y+240, pet[ familyPetIndex ].graNo, 3 );
			pActPet10 = MakeAnimDisp( x+460, y+240, pet[ familyPetIndex ].graNo, 0 );
		StockDispBuffer( x+312, y+79, DISP_PRIO_IME3, pc.faceGraNo, 0 );
		StockDispBuffer( 320, 240, DISP_PRIO_IME3, CG_FAMILY_DETAIL_WIN, 1 );

	}

}





static short familyListProcNo = 0;
static int pagenum = 0;
static int familytotal = 0;
static int familynum = 0;
static ACTION *pActPet11;
typedef struct {
	int index;
	int tempindex;
	char name[512];
	char leadername[512];
	int popu;
	int DP;
	int setupflag;
	int acceptflag;
} TfamilyList;

static TfamilyList familyList[10];

void closeFamilyList( void )
{
	play_se( 203, 320, 240 );	// ??????????
	familyListProcNo = 0;
	windowTypeWN = -1;
	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	DeathAction( pActPet11 );
	pActPet11 = NULL;
	//GetKeyInputFocus( &MyChatBuffer );
}

#ifdef PK_SERVICE_MAN
typedef struct
{
	int index;
	char name[128];
	char family[128];
	int trans;
	int level;
}TPKPlayerList;

static short pkListProcNo = 0;
static int pkPlayerTotal = 0;
static int pkPageNum = 0;
static int pkPlayerNum = 0;
static TPKPlayerList pkPlayerList[10];

typedef struct
{
	int  index;
	char rank[16];
	int  score;
	char name[128];
	char family[128];
}TPKHistoryList;

static short pkHistoryListProcNo = 0;
static int pkHistoryMaxSeason = 0;
static int pkHistoryPageNum = 0;
static int pkHistoryPlayerNum = 0;
static char pkHistoryInfo[64] = {'\0'};
static TPKHistoryList pkHistoryList[10];

void closePKList( void )
{
	play_se( 203, 320, 240 );	// ??????????
	pkListProcNo = 0;
	windowTypeWN = -1;
	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	DeathAction( pActPet11 );
	pActPet11 = NULL;
	//GetKeyInputFocus( &MyChatBuffer );
}

void closePKHistoryList( void )
{
	play_se( 203, 320, 240 );	// ??????????
	pkHistoryListProcNo = 0;
	windowTypeWN = -1;
	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	DeathAction( pActPet11 );
	pActPet11 = NULL;
	//GetKeyInputFocus( &MyChatBuffer );
}

void initPKList(char* data)
{
	int i=0,j=0;
	char pkbuf[128];
	char ibuf[16];

	pkListProcNo = 1;

	if( windowTypeWN != WINDOW_MESSAGETYPE_DAYACTION_PKLIST )
	{
		windowTypeWN = WINDOW_MESSAGETYPE_DAYACTION_PKLIST;
		DeathAction( ptActMenuWin );
		ptActMenuWin = NULL;
	}

	for( i=0;i<10;i++ )
	{
		pkPlayerList[i].index = -1;
		pkPlayerList[i].name[0] = NULL;
		pkPlayerList[i].level = -1;
		pkPlayerList[i].family[0] = NULL;
		pkPlayerList[i].trans = -1;
	}

	getStringToken( data, '|', 3, sizeof( ibuf )-1, ibuf );
	pkPlayerTotal = atoi( ibuf );
	getStringToken( data, '|', 4, sizeof( ibuf )-1, ibuf );
	pkPageNum = atoi( ibuf );
	getStringToken( data, '|', 5, sizeof( ibuf )-1, ibuf );
	pkPlayerNum = atoi( ibuf );

	for( i = 0; i < pkPlayerNum ; i++ )
	{
		getStringToken( data, '|', 6+i, sizeof( pkbuf )-1, pkbuf );

		getStringToken( pkbuf, ' ', 1, sizeof( ibuf )-1, ibuf );
		pkPlayerList[i].index = atoi( ibuf );

		getStringToken( pkbuf, ' ', 2, sizeof( pkPlayerList[i].name )-1, pkPlayerList[i].name );
		makeStringFromEscaped( pkPlayerList[i].name );

		getStringToken(pkbuf, ' ', 3, sizeof(pkPlayerList[i].family) - 1, pkPlayerList[i].family);
		makeStringFromEscaped(pkPlayerList[i].family);

		getStringToken(pkbuf, ' ', 4, sizeof(ibuf) - 1, ibuf);
		pkPlayerList[i].trans = atoi(ibuf);

		getStringToken( pkbuf, ' ', 5, sizeof( ibuf )-1, ibuf );
		pkPlayerList[i].level = atoi( ibuf );
	}
}

void initPKHistoryList(char* data)
{
	int i=0,j=0;
	char pkbuf[128];
	char ibuf[16];

	pkHistoryListProcNo = 1;

	if( windowTypeWN != WINDOW_MESSAGETYPE_DAYACTION_PKHISTORYLIST )
	{
		windowTypeWN = WINDOW_MESSAGETYPE_DAYACTION_PKHISTORYLIST;
		DeathAction( ptActMenuWin );
		ptActMenuWin = NULL;
	}

	for( i=0;i<10;i++ )
	{
		pkHistoryList[i].index = -1;
		pkHistoryList[i].name[0] = NULL;
		pkHistoryList[i].family[0] = NULL;
		pkHistoryList[i].rank[0] = NULL;
		pkHistoryList[i].score = 0;
	}

	getStringToken( data, '|', 3, sizeof( ibuf )-1, ibuf );
	pkHistoryMaxSeason = atoi( ibuf );
	getStringToken( data, '|', 4, sizeof( ibuf )-1, ibuf );
	pkHistoryPageNum = atoi( ibuf );

	getStringToken(data, '|', 5, sizeof(pkHistoryInfo) - 1, pkHistoryInfo);
	makeStringFromEscaped(pkHistoryInfo);

	getStringToken( data, '|', 6, sizeof( ibuf )-1, ibuf );
	pkHistoryPlayerNum = atoi( ibuf );


	for( i = 0; i < pkHistoryPlayerNum ; i++ )
	{
		getStringToken( data, '|', 7+i, sizeof( pkbuf ) - 1, pkbuf );

		getStringToken( pkbuf, ' ', 1, sizeof( ibuf ) - 1, ibuf );
		pkHistoryList[i].index = atoi( ibuf );

		getStringToken( pkbuf, ' ', 2, sizeof( pkHistoryList[i].rank ) - 1, pkHistoryList[i].rank);
		makeStringFromEscaped( pkHistoryList[i].rank );

		getStringToken(pkbuf, ' ', 3, sizeof(ibuf) - 1, ibuf);
		pkHistoryList[i].score = atoi(ibuf);
		
		getStringToken(pkbuf, ' ', 4, sizeof(pkHistoryList[i].name) - 1, pkHistoryList[i].name);
		makeStringFromEscaped(pkHistoryList[i].name);

		getStringToken(pkbuf, ' ', 5, sizeof(pkHistoryList[i].family) - 1, pkHistoryList[i].family);
		makeStringFromEscaped(pkHistoryList[i].family);
	}
}
#endif

void initFamilyList( char* data )
{
	int i=0,j=0;
	char familybuf[128];
	char ibuf[16];

	familyListProcNo = 1;

	if( windowTypeWN != WINDOW_MESSAGETYPE_FAMILYJOIN )
	{
		windowTypeWN = WINDOW_MESSAGETYPE_FAMILYJOIN;
		DeathAction( ptActMenuWin );
		ptActMenuWin = NULL;
	}

	for( i=0;i<10;i++ )
	{
		familyList[i].index = -1;
		familyList[i].name[0] = NULL;
		familyList[i].leadername[0] = NULL;
		familyList[i].popu = 0;
		familyList[i].DP = 0;
		familyList[i].tempindex = -1;
		familyList[i].setupflag = 0;
		familyList[i].acceptflag = 0;
	}

	getStringToken( data, '|', 3, sizeof( ibuf )-1, ibuf );
	familytotal = atoi( ibuf );
	getStringToken( data, '|', 4, sizeof( ibuf )-1, ibuf );
	pagenum = atoi( ibuf );
	//makeStringFromEscaped( pagenum );
	getStringToken( data, '|', 5, sizeof( ibuf )-1, ibuf );
	familynum = atoi( ibuf );
	//makeStringFromEscaped( familynum );


	for( i=0; i<familynum ; i++ )
	{
		getStringToken( data, '|', 6+i, sizeof( familybuf )-1, familybuf );


		getStringToken( familybuf, ' ', 1, sizeof( ibuf )-1, ibuf );
		familyList[i].index = atoi( ibuf );

		getStringToken( familybuf, ' ', 2, sizeof( familyList[i].name )-1, familyList[i].name );
		makeStringFromEscaped( familyList[i].name );

		getStringToken( familybuf, ' ', 3, sizeof( familyList[i].leadername )-1, familyList[i].leadername );
		makeStringFromEscaped( familyList[i].leadername );

		getStringToken( familybuf, ' ', 5, sizeof( ibuf )-1, ibuf );
		familyList[i].popu = atoi( ibuf );

		getStringToken( familybuf, ' ', 4, sizeof( ibuf )-1, ibuf );
		familyList[i].DP = atoi( ibuf );

		getStringToken( familybuf, ' ', 6, sizeof( ibuf )-1, ibuf );
		familyList[i].tempindex = atoi( ibuf );

		getStringToken( familybuf, ' ', 7, sizeof( ibuf )-1, ibuf );
		familyList[i].setupflag = atoi( ibuf );

		getStringToken( familybuf, ' ', 8, sizeof( ibuf )-1, ibuf );
		familyList[i].acceptflag = atoi( ibuf );
		
	}

}


void familyListWN( void )
{
	//static ACTION *ptActMenuWin = NULL;
	static int x, y, w, h, i, j;
	static int btnId[5];
	static int fontId[12];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int prevBtnGraNo10[] = { CG_PREV_BTN2, CG_PREV_BTN2_DOWN };
	int nextBtnGraNo10[] = { CG_NEXT_BTN2, CG_NEXT_BTN2_DOWN };

	static int prevBtn =0, nextBtn =0, prevBtn10 =0, nextBtn10 =0;
	int selBtnId = -1;
	int selFontBtnId = -1;
	int ret = 0;
	char buf[1024];
	char buf2[512];



	if( CheckMenuFlag()
	 || joy_trg[ 0 ] & JOY_ESC
	 || actBtn == 1
	 || menuBtn == 1
	 || disconnectServerFlag == true
	 || wnCloseFlag == 1
	 )
	{
		closeFamilyList();
		/*
		windowTypeWN = -1;
		DeathAction( ptActMenuWin );
		ptActMenuWin = NULL;
		*/
		return;
	}

	if( familyListProcNo == 10 || familyListProcNo == 20 ) {
		familyDetailWN();
	}

   if( familyListProcNo == 1 ) {
	if( ptActMenuWin == NULL )
	{
		familyListProcNo = 1;

		w=7; h=8;
		x = (640-w*64)/2;
		y = (480-h*48)/2;
		 
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		play_se( 202, 320, 240 );	// ????????	


	}
	//else if( ptActMenuWin != NULL )
	//if( familyListProcNo == 1 )
	else if( ptActMenuWin->hp >= 1 )
	{
		
		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );
		selFontBtnId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
				
		if( mouse.onceState & MOUSE_LEFT_CRICK )
		{
			switch( selBtnId ) {
			case 0:
				windowTypeWN = -1;
				DeathAction( ptActMenuWin );
				ptActMenuWin = NULL;
				return;
			case 1:
				prevBtn = true;
				if( pagenum > 1 ) {
					sprintf(buf, "S|F|%d|0", pagenum-1);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				else {
					sprintf(buf, "S|F|%d|0", (familytotal-1)/10 +1 );
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				return;
				break;
			case 2:
				nextBtn = true;
				if( pagenum < (familytotal-1)/10 +1 ) {
					sprintf(buf, "S|F|%d|0", pagenum+1);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				else {
					sprintf(buf, "S|F|1|0" );
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				return;
				break;
			case 3:
				prevBtn10 = true;
				if( pagenum > 10 ) {
					sprintf(buf, "S|F|%d|0", pagenum-10);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				else {
					sprintf(buf, "S|F|%d|0", (familytotal-1)/10 +1 );
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				return;
				break;
			case 4:
				nextBtn10 = true;
				if( pagenum < (familytotal-1)/10 +1 -9 ) {
					sprintf(buf, "S|F|%d|0", pagenum+10);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				else {
					sprintf(buf, "S|F|1|0" );
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}
				return;
				break;

			}

			if( selFontBtnId != -1 ) {
				sprintf( buf, "S|D|%s|%d|%d",
					makeEscapeString(familyList[selFontBtnId].name, buf2, sizeof(buf2) ),
					familyList[selFontBtnId].index, familyList[selFontBtnId].tempindex);
				if( bNewServer)
					lssproto_FM_send( sockfd, buf );
				else
					old_lssproto_FM_send( sockfd, buf );
			}
			
		}


		if( mouse.state & MOUSE_LEFT_CRICK && prevBtn == true )
			prevBtn = true;
		else	prevBtn = false;
		if( mouse.state & MOUSE_LEFT_CRICK && nextBtn == true )
			nextBtn = true;
		else	nextBtn = false;
		if( mouse.state & MOUSE_LEFT_CRICK && prevBtn10 == true )
			prevBtn10 = true;
		else	prevBtn10 = false;
		if( mouse.state & MOUSE_LEFT_CRICK && nextBtn10 == true )
			nextBtn10 = true;
		else	nextBtn10 = false;


		StockFontBuffer( x+190, y+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, "?????б?", 0 );
#ifdef _READ16BITBMP
		if(!g_bUseAlpha) StockDispHLine( x+150, y+40, 2 );
#endif
		btnId[0] = StockDispBuffer( x+220, y+350, DISP_PRIO_IME3, CG_EXIT_BTN, 2 );
		btnId[1] = StockDispBuffer( x+16+60, y+8+60, DISP_PRIO_IME4, prevBtnGraNo[prevBtn], 2 );
		btnId[2] = StockDispBuffer( x+16+210, y+8+60, DISP_PRIO_IME4, nextBtnGraNo[nextBtn], 2 );
		btnId[3] = StockDispBuffer( x+16+20, y+8+60, DISP_PRIO_IME4, prevBtnGraNo10[prevBtn10], 2 );
		btnId[4] = StockDispBuffer( x+16+250, y+8+60, DISP_PRIO_IME4, nextBtnGraNo10[nextBtn10], 2 );
		sprintf( buf, "%3d / %3d ?", pagenum, (familytotal-1)/10 +1  );
		StockFontBuffer( x+100, y+60, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );


		sprintf( buf, "%-4s %-16s %-16s %6s %10s", "", "[家 族 名 称]", "[族   长]", "[人数]", "[家族声望]" );
		StockFontBuffer( x+25, y+90, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );


		for( i=0; i<10 ; i++ )	{
			if( familyList[i].name[0] == NULL )	break;
			sprintf( buf, "%4d: %-16s %-16s %6d %10d", (pagenum-1)*10+1+i, familyList[i].name, familyList[i].leadername,
					familyList[i].popu, (familyList[i].DP /100));
			if( familyList[i].acceptflag != 1 )	j = FONT_PAL_AQUA;
			else if( familyList[i].setupflag != 1 )	j = FONT_PAL_GREEN;
			else	j = FONT_PAL_WHITE;
			fontId[i] = StockFontBuffer( x+15, y+110+(i*20), FONT_PRIO_FRONT, j, buf, 2 );
			
		}

	}
   }
	
}

struct {
	int index;
	int tempindex;
	char name[20];
	char leadername[20];
	int leadergraph;
	char petname[20];
	char petattr[64];
	int joinnum;
	int joinflag;
	char rule[130];
	int petgra;
	int sprite;
} familyDetail;


static int changeData[2] = {0,0};
void initFamilyDetail( char* data )
{
	char ibuf[64];

	if( familyListProcNo == 1 )
		familyListProcNo = 10;
	else if( familyListProcNo == 0 ){
		familyListProcNo = 20;
		lssproto_WN_recv( sockfd, WINDOW_MESSAGETYPE_MESSAGE, WINDOW_BUTTONTYPE_OK
						, -1, -1, AnnounceBuf);
	}

	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	
	familyRuleShow.buffer[0] = NULL;
	familyRuleShow.cnt = 0;
	familyRuleShow.cursor = 0;
	familyRuleShow.len = 120;
	familyRuleShow.lineLen = 46;
	familyRuleShow.lineDist = 30;
	familyRuleShow.color = 0;
	//familyRuleShow.x = x+20;
	//familyRuleShow.y = y+160;
	familyRuleShow.fontPrio = FONT_PRIO_FRONT;
	
	getStringToken( data, '|', 3, sizeof( ibuf )-1, ibuf );
	familyDetail.tempindex = atoi( ibuf );
	getStringToken( data, '|', 4, sizeof( ibuf )-1, ibuf );
	familyDetail.index = atoi( ibuf );
	getStringToken( data, '|', 5, sizeof( familyDetail.name )-1, familyDetail.name );
	makeStringFromEscaped( familyDetail.name );
	getStringToken( data, '|', 6, sizeof( familyDetail.leadername )-1, familyDetail.leadername );
	makeStringFromEscaped( familyDetail.leadername );
	getStringToken( data, '|', 7, sizeof( ibuf )-1, ibuf );
	familyDetail.leadergraph = atoi( ibuf );
	getStringToken( data, '|', 8, sizeof( familyDetail.petname )-1, familyDetail.petname );
	makeStringFromEscaped( familyDetail.petname );
	getStringToken( data, '|', 9, sizeof( familyDetail.petattr )-1, familyDetail.petattr );
	makeStringFromEscaped( familyDetail.petattr );
	getStringToken( data, '|', 10, sizeof( ibuf )-1, ibuf );
	familyDetail.joinnum = atoi( ibuf );
	getStringToken( data, '|', 11, sizeof( ibuf )-1, ibuf );
	familyDetail.joinflag = atoi( ibuf );
	getStringToken( data, '|', 12, sizeof( familyDetail.rule )-1, familyDetail.rule );
	makeStringFromEscaped( familyDetail.rule );
	strcpy( familyRuleShow.buffer, familyDetail.rule );
	familyRuleShow.cnt = strlen(familyRuleShow.buffer);
	familyRuleShow.cursor = strlen(familyRuleShow.buffer);

	getStringToken( data, '|', 13, sizeof( ibuf )-1, ibuf );
	familyDetail.sprite = atoi( ibuf );
	getStringToken( familyDetail.petattr, ' ', 1, sizeof( ibuf )-1, ibuf );
	familyDetail.petgra = atoi( ibuf );

	changeData[0] = 0;
	changeData[1] = 0;
}


int familyDetailWN( void )
{
	static int w, h, x, y, xx, yy, i;
	static int btnId[5];
	static int fontId[5];
	static int prevBtn =0, nextBtn =0;
	static int familyPetIndex = 0;
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };

	//static int changeData[2] = {0,0};
	int selBtnId = -1;
	int selFontBtnId = -1;
	char buf[1024];
	char buf2[512];

	if( CheckMenuFlag()
	 || joy_trg[ 0 ] & JOY_ESC
	 || actBtn == 1
	 || menuBtn == 1
	 || disconnectServerFlag == true
	 || wnCloseFlag == 1
	 )
	{
		GetKeyInputFocus( &MyChatBuffer );
		closeFamilyList();
		return 0;
	}

	if( ptActMenuWin == NULL )
	{

		//familyListProcNo = 11;
		for( int i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}

		w=571; h=295;
		x = (640-w)/2;
		y = (480-h)/2;
		//xx = 0;
		//yy = 0;

		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		familyRuleShow.x = x+20;
		familyRuleShow.y = y+160;

		play_se( 202, 320, 240 );	// ????????	

	}

	if( ptActMenuWin->hp >= 1 )
	{

		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );
		selFontBtnId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( mouse.onceState & MOUSE_LEFT_CRICK )
		{
			switch( selBtnId ) {
			case 0 :
				if( pc.familyName[0] != NULL ) {
					lssproto_TK_recv( sockfd, 0, "P|你已经加入其它家族了喔！", 0);
					break;
				}

				sprintf( buf, "J|%d|%d|%s|%d",
						familyDetail.tempindex, familyDetail.index, 
						makeEscapeString(familyDetail.name, buf2, sizeof(buf2)),
						familyDetail.sprite);
				if( bNewServer)
					lssproto_FM_send( sockfd, buf );
				else
					old_lssproto_FM_send( sockfd, buf );
				familyListProcNo = 1;
				closeFamilyList();
				GetKeyInputFocus( &MyChatBuffer );
				return 0;
				break;
			case 1 :
				familyListProcNo = 1;
				DeathAction( ptActMenuWin );
				ptActMenuWin = NULL;
				DeathAction( pActPet11 );
				pActPet11 = NULL;
				GetKeyInputFocus( &MyChatBuffer );
				return 0;
				break;

			case 2:
					prevBtn = true;
					i =0;
					while(1) {
						familyPetIndex--;
						if( familyPetIndex <0 ) familyPetIndex =4;
						if( pet[familyPetIndex].useFlag != NULL)
							break;
						if( (i++) >5 ) {
							familyPetIndex = -1;
							break;
						}
					}
					if( pActPet11 != NULL ) {
						DeathAction( pActPet11 );
						pActPet11 = NULL;
					}
					changeData[1] = 1;
					break;
			case 3:
					nextBtn = true;
					i=0;
					while(1) {
						familyPetIndex++;
						if( familyPetIndex >=5 ) familyPetIndex =0;
						if( pet[familyPetIndex].useFlag != NULL)
							break;
						if( (i++) >5 ) {
							familyPetIndex = -1;
							break;
						}
					}
					if( pActPet11 != NULL ) {
						DeathAction( pActPet11 );
						pActPet11 = NULL;
					}
					changeData[1] = 1;
					break;
			case 4:
				if( (changeData[0] == 1) && 
					(strcmp(familyRuleShow.buffer,familyDetail.rule)!=0) ) {
					sprintf( buf, "X|R|%s", familyRuleShow.buffer);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}

				if( changeData[1] == 1 ) {
					sprintf( buf, "X|P|%d", familyPetIndex);
					if( bNewServer)
						lssproto_FM_send( sockfd, buf );
					else
						old_lssproto_FM_send( sockfd, buf );
				}

				familyListProcNo = 1;
				DeathAction( ptActMenuWin );
				ptActMenuWin = NULL;
				DeathAction( pActPet11 );
				pActPet11 = NULL;
				GetKeyInputFocus( &MyChatBuffer );
				return 0;
				break;

			}

			switch( selFontBtnId ) {
			case 0:
				changeData[0] = 1;
				break;
			}

		}

		if( mouse.state & MOUSE_LEFT_CRICK && prevBtn == true )
			prevBtn = true;
		else	prevBtn = false;

		if( mouse.state & MOUSE_LEFT_CRICK && nextBtn == true )
			nextBtn = true;
		else	nextBtn = false;

		//StockFontBuffer( x+60, y+60, FONT_PRIO_FRONT, FONT_PAL_WHITE, "", 0 );
		//sprintf( buf, "", pagenum, (familytotal-1)/8 +1  );
		//StockFontBuffer( x+160, y+30, FONT_PRIO_FRONT, FONT_PAL_WHITE, "????????", 0 );
		//StockDispHLine( x+140, y+ );

		//sprintf( buf, "?????????%s", familyDetail.name );
		StockFontBuffer( x+100, y+40, FONT_PRIO_FRONT, FONT_PAL_WHITE, familyDetail.name, 0 );
		//sprintf( buf, "?峤?????%s", familyDetail.leadername );
		StockFontBuffer( x+100, y+70, FONT_PRIO_FRONT, FONT_PAL_WHITE, familyDetail.leadername, 0 );
		sprintf( buf, "%d ??", familyDetail.joinnum );
		StockFontBuffer( x+100, y+100, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

		
		// ???????
		sprintf( buf, "%46s", "" );
#ifdef _FMVER21
		if( (pc.familyleader == FMMEMBER_LEADER) && ( strcmp( pc.familyName, familyDetail.name) ==0) ) {
#else
		if( (pc.familyleader == 1) && ( strcmp( pc.familyName, familyDetail.name) ==0) ) {
#endif
			fontId[0] = StockFontBuffer( x+20, y+160, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 2 );
			btnId[2] = StockDispBuffer( x+435, y+270, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
			btnId[3] = StockDispBuffer( x+495, y+270, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
		}
		else
			fontId[0] = StockFontBuffer( x+20, y+160, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
		
		//strcpy( familyRuleShow.buffer, familyDetail.rule );

		if( changeData[0] == 1 ) {
			StockFontBuffer2( &familyRuleShow );
			GetKeyInputFocus( &familyRuleShow );
		} else {
			GetKeyInputFocus( &MyChatBuffer );
			if( familyRuleShow.buffer[0] != NULL )
				StockFontBuffer2( &familyRuleShow );
		}
		
		//StockFontBuffer( x+20, y+160, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
		//StockFontBuffer( x+20, y+190, FONT_PRIO_FRONT, FONT_PAL_WHITE, familyDetail.rule, 0 );
		//StockFontBuffer( x+20, y+220, FONT_PRIO_FRONT, FONT_PAL_WHITE, familyDetail.rule, 0 );

		
		//sprintf( buf, "?? ?? ???%s", familyDetail.petname );
		if( changeData[1] == 0 )
			StockFontBuffer( x+436, y+80, FONT_PRIO_FRONT, FONT_PAL_WHITE, familyDetail.petname, 0 );
		else
			StockFontBuffer( x+436, y+80, FONT_PRIO_FRONT, FONT_PAL_WHITE, pet[familyPetIndex].name , 0 );
		
		StockFontBuffer( x+436, y+36, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				familySpriteName[familyDetail.sprite], 0 );
		
		//fontId[0] = StockFontBuffer( x+200, y+250, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "?? ??", 2 );
		//fontId[1] = StockFontBuffer( x+300, y+250, FONT_PRIO_FRONT, FONT_PAL_YELLOW, "?? ??", 2 );
		if( pc.familyName[0] == NULL && familyDetail.joinflag == 1 ) {
			btnId[0] = StockDispBuffer( x+90, y+274, DISP_PRIO_IME3, CG_JOIN_BTN, 2 );
			btnId[1] = StockDispBuffer( x+270, y+274, DISP_PRIO_IME3, CG_EXIT_BTN2, 2 );
		}
		else if( (changeData[0]==1) || (changeData[1]==1) ){
			btnId[4] = StockDispBuffer( x+90, y+274, DISP_PRIO_IME3, 26093, 2 );
			btnId[1] = StockDispBuffer( x+270, y+274, DISP_PRIO_IME3, CG_EXIT_BTN2, 2 );
		}
		else
			btnId[1] = StockDispBuffer( x+180, y+274, DISP_PRIO_IME3, CG_EXIT_BTN2, 2 );

		if( pActPet11 == NULL )
			if( changeData[1] == 0 )
				pActPet11 = MakeAnimDisp( x+460, y+240, familyDetail.petgra, 0 );
			else
				pActPet11 = MakeAnimDisp( x+460, y+240, pet[familyPetIndex].graNo, 0 );

		StockDispBuffer( x+312, y+79, DISP_PRIO_IME3, familyDetail.leadergraph, 0 );
		StockDispBuffer( 320, 240, DISP_PRIO_IME3, CG_FAMILY_DETAIL_WIN, 1 );

	}

	return 0;

}

#ifdef _FMVER21
/*----------  Shan (BEGIN) ----------*/
/*---------  AD 2001/05/31  ---------*/
// shan Family MemberList
void initFMWindowType( char *data )
{
	makeStringFromEscaped( data );
	
	selStartLine = 0;
	FMdengonidex = 0;

	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 64 );
    
	time(&FMHoldTime);
    
	char namebuf[32],lvbuf[8],statusbuf[32],onlinebuf[32],dpbuf[64];	
	int int_status,onlineflag;
	char tmp[32];
#ifdef _FM_MODIFY
	int iGSId;
	char szGSname[64],szPlanetId[8];
#endif
#ifdef _NEW_MANOR_LAW
	char momentum[12] = {0};
#endif
	// ???崫???????? 11 ??????б?????????????(???EVENT???)
	// ?????????????????????????????????
	for( int i = 0; i < 11;  i++ ){
		if( strlen( msgWN[i] ) > 0 ){					
			if( i>=selStartLine ){				
				if( i != 10){
					getStringToken(msgWN[i], '|', 6, sizeof(statusbuf)-1, tmp);
					getStringToken(msgWN[i], '|', 7, sizeof(statusbuf)-1, statusbuf);
				}
				else
					getStringToken(msgWN[i], '|', 3, sizeof(statusbuf)-1, statusbuf);
				// ??? INDEX -> 1~10  
				FMmsgWN[i-(selStartLine-1)] = atoi(statusbuf);
				FMelderFlag[i-(selStartLine-1)] = atoi(tmp);
				getStringToken(msgWN[i], '|', 1, sizeof(tmp)-1, tmp);				
				FMpidWN[i-(selStartLine-1)] = atoi(tmp);
				getStringToken(msgWN[i], '|', 2, sizeof(FMnameWN[i-(selStartLine-1)])-1, 
					           FMnameWN[i-(selStartLine-1)]);								
				//???????
				if(i<10){
					getStringToken(msgWN[i], '|', 2, sizeof(namebuf)-1, namebuf);
					getStringToken(msgWN[i], '|', 3, sizeof(lvbuf)-1, lvbuf);
					getStringToken(msgWN[i], '|', 4, sizeof(onlinebuf)-1, onlinebuf);
                    onlineflag = atoi(onlinebuf);
					if( onlineflag > 0 )
						strcpy(onlinebuf, "OnLine");
					else	
						strcpy(onlinebuf, "OffLine");
                    getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);					
					getStringToken(msgWN[i], '|', 7, sizeof(statusbuf)-1, statusbuf);
					int_status = atoi(statusbuf);
					switch( int_status ){
																 // ?? ?? ??	
                            case FMMEMBER_MEMBER:		   	     // ??????
								strcpy(statusbuf, "一般族员");
								break;
							case FMMEMBER_LEADER:				 // ?????峤
								strcpy(statusbuf, "族    长");
								break;
							case FMMEMBER_ELDER:				 // ????
								strcpy(statusbuf, "长    老");
								break;
/*							case FMMEMBER_ELDER:				 // ????
								strcpy(statusbuf, "??    ??");
								break;
							case FMMEMBER_INVITE:				 // ???
								strcpy(statusbuf, "??    ?");
								break;
                            case FMMEMBER_BAILEE:				 // ????
								strcpy(statusbuf, "?? ?? ??");
								break;*/
							case FMMEMBER_APPLY:				 // ???????								
								strcpy(statusbuf, "申请加入");
								break;					
/*							case FMMEMBER_VICELEADER:            // ???峤
                                strcpy(statusbuf, "?? ?? ??");
								break;*/
							default:							 // ??    ??	
								strcpy(statusbuf, "退    出");
                                break;							
					}
#ifdef _FM_MODIFY
					// ????????????
					getStringToken(msgWN[i],'|',8,sizeof(szGSname) - 1,szGSname);
					iGSId = atoi(szGSname);
					if(iGSId > 0){
						for(int j = 0;j < MAX_GMSV;j++){
							if(gmsv[j].used == '1'){
								getStringToken(gmsv[j].ipaddr,'.',4,sizeof(szPlanetId) - 1,szPlanetId);
								if(iGSId == atoi(szPlanetId)){
									sprintf(szGSname,"%s",gmsv[j].name);
									break;
								}
							}
						}
					}
					else sprintf(szGSname,"无");

#ifndef _NEW_MANOR_LAW
					sprintf(msgWN[i], "%-9s%-18s%6s%13s%10s%6s", onlinebuf, namebuf, lvbuf, dpbuf, statusbuf,szGSname);
#else
					getStringToken(msgWN[i],'|',9,sizeof(momentum) - 1,momentum);
					sprintf(msgWN[i], "%-9s%-18s%3s%12s%12s%10s%7s", onlinebuf, namebuf, lvbuf, dpbuf,momentum,statusbuf,szGSname);
#endif

#else
					sprintf(msgWN[i], "%-12s%-18s%3s%13s%10s", onlinebuf, namebuf, lvbuf, dpbuf, statusbuf);
#endif
				}else{
					getStringToken(msgWN[i], '|', 1, sizeof(namebuf)-1, namebuf);
					getStringToken(msgWN[i], '|', 2, sizeof(tmp)-1, tmp);
                    FMdengonidex = atoi(tmp);
					getStringToken(msgWN[i], '|', 3, sizeof(statusbuf)-1, statusbuf);
					int_status = atoi(statusbuf);
					switch( int_status ){						    
					    case 1:  // ???????
							strcpy(statusbuf, "继续招募");
							break;
						case 0:  // ??????								
							strcpy(statusbuf, "终止招募");
							break;						
						default:								
							break;							
					}
					sprintf(msgWN[i], " %-20s                         %s", namebuf, statusbuf);
				}
			}
		}
	}
}

/*----------  Shan (BEGIN) ----------*/
/*---------  AD 2001/05/31  ---------*/
// ????????б?
int fmDutyproc  = 0;    // select number duty
int fmDutyRet   = 0;
int fmYesNoproc = 0;    // delete number
int fmYesNoRet  = 0;

#ifdef _FM_MODIFY
extern int TalkMode;
extern char secretName[32];
extern STR_BUFFER *pNowStrBuffer;
#endif

void FMWindowType( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static int fmdelid;	
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;
	int  x1, y1, x2, y2;		
  char buf[1024];
#ifdef _FM_MODIFY
	int iNameX1,iNameY1,iNameX2,iNameY2;
#endif
	
	if( fmDutyproc == 1) {
		fmDutyRet = fmselectdetuyWindow(200, 100, fmdelid);
		return;
	}
	if(fmDutyRet != 0)
		ptActMenuWin = NULL;

	if( ptActMenuWin == NULL )
	{
#ifndef _FM_MODIFY
		winW = 7;
#else
	#ifdef _NEW_MANOR_LAW
		winW = 9;
	#else
		winW = 8;
	#endif
#endif
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 335;
			}
		}
        msgLine = 11;
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		

		if(fmDutyRet != 0)
		{ 								
			if(fmDutyRet != FMMEMBER_LEADER || fmDutyRet != FMMEMBER_APPLY){
				id2 = fmdelid;				
				FMmsgWN[id2] = fmDutyRet;
			}
			fmDutyRet = 0;		
		}

		if( ptActMenuWin->hp >= 1 )	
		{
			for( i = selStartLine; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{
					if(i==10){
						x1 = winX+25;					
						y1 = winY+28+(i+3)*21;
						x2 = x1 + 405;
					  y2 = y1 + 20;
					}
					else{
#ifdef _NEW_MANOR_LAW
						x1 = winX+420;
#else
						x1 = winX+364;
#endif
						y1 = winY+18+(i+3)*21;
					  x2 = x1 + 70;					    
					  y2 = y1 + 20;
#ifdef _FM_MODIFY
						iNameX1 = winX + 95;
						iNameY1 = winY + 18 + (i+3) * 21;
						iNameX2 = iNameX1 + 118;
						iNameY2 = iNameY1 + 20;
#endif
					}							

					// ????峤?????????????
					/*if( pc.familyleader == FMMEMBER_LEADER && 
						FMmsgWN[i-selStartLine+1] > 0 && FMmsgWN[i-selStartLine+1] < FMMEMBER_NUMBER &&
						FMmsgWN[i-selStartLine+1] != FMMEMBER_LEADER ||
						( pc.familyleader == FMMEMBER_ELDER || pc.familyleader == FMMEMBER_VICELEADER ) &&
						( FMmsgWN[i-selStartLine+1] == FMMEMBER_MEMBER || FMmsgWN[i-selStartLine+1] == FMMEMBER_APPLY ) ||
						(i-selStartLine+1) == 11  &&
						(pc.familyleader == FMMEMBER_LEADER || pc.familyleader == FMMEMBER_ELDER || pc.familyleader == FMMEMBER_VICELEADER))*/
          if( pc.familyleader == FMMEMBER_LEADER && 
						FMmsgWN[i-selStartLine+1] > 0 && FMmsgWN[i-selStartLine+1] < FMMEMBER_NUMBER &&
						FMmsgWN[i-selStartLine+1] != FMMEMBER_LEADER ||
						pc.familyleader == FMMEMBER_ELDER &&
						( FMmsgWN[i-selStartLine+1] == FMMEMBER_MEMBER || FMmsgWN[i-selStartLine+1] == FMMEMBER_APPLY ) ||
						(i-selStartLine+1) == 11  &&
						(pc.familyleader == FMMEMBER_LEADER || pc.familyleader == FMMEMBER_ELDER ))
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{           
						time_t now_time;
						time(&now_time);
						if( mouse.onceState & MOUSE_LEFT_CRICK && (now_time - FMHoldTime)>=1 )
						{   
							time(&FMHoldTime);
							id2 = i - selStartLine + 1;
                            
							if( id2>=1 && id2<=10)	
							switch( FMmsgWN[id2] ){
							case FMMEMBER_MEMBER:		// ?????
							case FMMEMBER_APPLY:		// ???????
							case FMMEMBER_ELDER:		  // ????
							//case FMMEMBER_ELDER:		  // ????
							//case FMMEMBER_INVITE:		  // ???
							//case FMMEMBER_BAILEE:	 	  // ????							
							//case FMMEMBER_VICELEADER:   // ???峤
								fmDutyproc = 1;
								fmdelid = id2;
								id2 = -1;								
								DeathAction( ptActMenuWin );
								ptActMenuWin = NULL;
								return;
								break;							
							default:
                                break;
							}
							else if(id2==11)
							switch( FMmsgWN[id2] ){						    
							case 1:  // YES								
								FMmsgWN[id2] = 0;
								break;
							case 0:  // N O																
								FMmsgWN[id2] = 1; 
								break;							
							default:								
                                break;							
							}
						}
					}
#ifdef _FM_MODIFY
					//?????????????????????
					if(i < 10){
						if(MakeHitBox(iNameX1,iNameY1,iNameX2,iNameY2,DISP_PRIO_BOX2)){
							time_t now_time;
							time(&now_time);
							if(mouse.onceState & MOUSE_LEFT_CRICK && (now_time - FMHoldTime) >= 1){
								//??????
#ifdef _TELLCHANNEL
								TalkMode = 1;
								sprintf(secretName,"%s ",FMnameWN[i+1]);
#endif
								pNowStrBuffer->buffer[ 0 ] = NULL;
								pNowStrBuffer->cursor=0;
								pNowStrBuffer->cnt = 0;
#ifdef _FRIENDCHANNEL
								StrToNowStrBuffer(secretName);
#endif
							}
						}
					}
#endif
				}
			}
		}		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}
        		
		if( 0 <= id && id < 6
		    || 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d|%d|%d", FMdengonidex, id2, FMmsgWN[id2]);
			makeEscapeString( data, msg, sizeof( msg )-1 );
						
      if(1 <= id2 && id2 < 11){
				sprintf(buf, "M|%s|%d|%d",FMnameWN[id2],FMpidWN[id2],FMmsgWN[id2]);
				if( bNewServer)
					lssproto_FM_send(sockfd,buf);
				else
					old_lssproto_FM_send(sockfd,buf);
			}
			if( id2 == 11){				
				sprintf(buf, "T|%d",FMmsgWN[id2]);
				if( bNewServer)
					lssproto_FM_send(sockfd,buf);
				else
					old_lssproto_FM_send(sockfd,buf);
			}    
			/*if( FMmsgWN[id2] != FMMEMBER_ELDER && FMmsgWN[id2] != FMMEMBER_INVITE && 
				FMmsgWN[id2] != FMMEMBER_BAILEE && FMmsgWN[id2] != FMMEMBER_VICELEADER )*/
			if( FMmsgWN[id2] == FMMEMBER_NONE || FMmsgWN[id2] == FMMEMBER_MEMBER || FMmsgWN[id2] == 0){
				if( bNewServer)
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
				else
					old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			}
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
#ifndef _FM_MODIFY
			StockFontBuffer( winX+34, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "              ?????????б?", 0 );
			StockFontBuffer( winX+24, winY+21+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ?????? ?? ?? ? ?? ?? ?? ??????? ???????????? ??????", 0 );
#else
			StockFontBuffer( winX+34, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "                         ‘家族成员列表’", 0 );
	#ifdef _NEW_MANOR_LAW
			StockFontBuffer( winX+24, winY+21+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "‘状态’  ‘成员姓名’  ‘等级’  ‘个人声望’  ‘’个人气势‘  ’加入‘  ’伺服器‘", 0 );
	#else
			StockFontBuffer( winX+24, winY+21+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ?????? ?? ?? ? ?? ?? ?? ??????? ???????????? ???????????????", 0 );
	#endif
#endif
			for( i = 0; i < msgLine; i++ ){
				if( strlen( msgWN[i] ) > 0 ){					
					if( i<selStartLine ){						
						StockFontBuffer( winX+34, winY+21+(i+3)*21,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
					}
					else if((i-selStartLine+1)==11){				
            int Font_Color = FONT_PAL_GREEN;
						if(FMmsgWN[i+1] == 1) Font_Color = FONT_PAL_GREEN;
						else if(FMmsgWN[i+1] == 0) Font_Color = FONT_PAL_RED;
						StockFontBuffer( winX+34, winY+31+(i+3)*21,
							FONT_PRIO_FRONT, Font_Color, msgWN[i], 0 );
					}
					else{
						int Font_Color = FONT_PAL_WHITE;						
						if(FMmsgWN[i+1] == 2) Font_Color = FONT_PAL_BLUE2;						
						else if(FMmsgWN[i+1] == -1) Font_Color = FONT_PAL_RED;
						
						StockFontBuffer( winX+34, winY+21+(i+3)*21,
							FONT_PRIO_FRONT, Font_Color, msgWN[i], 0 );
					}
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}
#else
/*----------  Shan (BEGIN) ----------*/
/*---------  AD 2001/05/31  ---------*/
// shan Family MemberList
void initFMWindowType( char *data )
{
	makeStringFromEscaped( data );
	
	selStartLine = 0;
	FMdengonidex = 0;

	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 40 );
    
	time(&FMHoldTime);

    char getstatus[3];	
	char namebuf[30],lvbuf[5],statusbuf[16],onlinebuf[12],dpbuf[64];	
	int int_status,onlineflag;
	char tmp[32];
	// ???崫???????? 11 ??????б?????????????(???EVENT???)
	// ?????????????????????????????????
	for( int i = 0; i < 11;  i++ ){
		if( strlen( msgWN[i] ) > 0 ){					
			if( i>=selStartLine ){
				strcpy( getstatus, msgWN[i]+(strlen(msgWN[i])-1));
				// ??? INDEX -> 1~10
				FMmsgWN[i-(selStartLine-1)] = atoi(getstatus);
				getStringToken(msgWN[i], '|', 1, sizeof(tmp)-1, tmp);				
				FMpidWN[i-(selStartLine-1)] = atoi(tmp);
				getStringToken(msgWN[i], '|', 2, sizeof(FMnameWN[i-(selStartLine-1)])-1, 
					           FMnameWN[i-(selStartLine-1)]);								
				//???????
				if(i<10){
					getStringToken(msgWN[i], '|', 2, sizeof(namebuf)-1, namebuf);
					getStringToken(msgWN[i], '|', 3, sizeof(lvbuf)-1, lvbuf);
					getStringToken(msgWN[i], '|', 4, sizeof(onlinebuf)-1, onlinebuf);
                    onlineflag = atoi(onlinebuf);
					if( onlineflag > 0 )
						strcpy(onlinebuf, "OnLine");
					else	
						strcpy(onlinebuf, "OffLine");
                    getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);					
					getStringToken(msgWN[i], '|', 6, sizeof(statusbuf)-1, statusbuf);
					sprintf(msgWN[i], "%-12s%-18s%-7s%9s%5s", onlinebuf, namebuf, lvbuf, dpbuf, statusbuf);
				}else{
					getStringToken(msgWN[i], '|', 1, sizeof(namebuf)-1, namebuf);
					getStringToken(msgWN[i], '|', 2, sizeof(tmp)-1, tmp);
                    FMdengonidex = atoi(tmp);
					getStringToken(msgWN[i], '|', 3, sizeof(statusbuf)-1, statusbuf);
					int_status = atoi(statusbuf);
					switch( int_status ){						    
					    case 1:  // ???????
							strcpy(statusbuf, "???????");
							break;
						case 0:  // ??????								
							strcpy(statusbuf, "??????");
							break;						
						default:								
							break;							
					}
					sprintf(msgWN[i], " %-20s                         %s", namebuf, statusbuf);
				}
			}
		}
	}
}

/*----------  Shan (BEGIN) ----------*/
/*---------  AD 2001/05/31  ---------*/
// ????????б?
int fmYesNoproc = 0;    // delete number
int fmYesNoRet  = 0;
void FMWindowType( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static int fmdelid;	
	char *btnTitle[] =
	{
		" ??? ",
		"CANCEL",
		"?  ??",
		"?  ??",
		"????",
		"????"
	};	
	char getstatus[3];	
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;
	int  x1, y1, x2, y2;		
    char buf[1024];		

	if( fmYesNoproc == 1) {
		fmYesNoRet = fmYesNoWindow(170, 170, fmdelid);
		return;
	}
	if(fmYesNoRet != 0)
		ptActMenuWin = NULL;	

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 335;
			}
		}
        msgLine = 11;
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		

		if(fmYesNoRet != 0)
		{ 
			if(fmYesNoRet==1){
				id2 = fmdelid;
				FMmsgWN[id2] = 4;
			}			
			fmYesNoRet = 0;		
		}

		if( ptActMenuWin->hp >= 1 )	
		{
			for( i = selStartLine; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{
					if(i==10){
						x1 = winX+25;					
						y1 = winY+28+(i+3)*21;
						x2 = x1 + 405;
					    y2 = y1 + 20;
					}
					else{
						x1 = winX+364;					
						y1 = winY+18+(i+3)*21;
					    x2 = x1 + 70;					    
					    y2 = y1 + 20;
					}							

					// ????峤????????

					if( pc.familyleader == 1 && 
						FMmsgWN[i-selStartLine+1] > 0 &&
						FMmsgWN[i-selStartLine+1] < 4 &&
						FMpidWN[i-selStartLine+1] != 0 ||
						(i-selStartLine+1) == 11  &&
						pc.familyleader == 1 )
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{           
						time_t now_time;
					    time(&now_time);						
						if( mouse.onceState & MOUSE_LEFT_CRICK && (now_time - FMHoldTime)>=1 )
						{   
							time(&FMHoldTime);
							id2 = i - selStartLine + 1;
                            
							if( id2>=1 && id2<=10)
							switch( FMmsgWN[id2] ){
							case 1:  // ?? ?? ??
								fmYesNoproc = 1;
								fmdelid = id2;
								id2 = -1;								
								DeathAction( ptActMenuWin );																
								break;
							case 2:  // ???????
								FMmsgWN[id2] = 1;
								break;
							case 3:  // ???????								
								fmYesNoproc = 1;
								fmdelid = id2;
								id2 = -1;								
								DeathAction( ptActMenuWin );								
								break;
							case 4:  // ??    ??
								break;
							default:
                                break;
							}
							else if(id2==11)
							switch( FMmsgWN[id2] ){						    
							case 1:  // YES								
								FMmsgWN[id2] = 0;
								break;
							case 0:  // N O																
								FMmsgWN[id2] = 1; 
								break;							
							default:								
                                break;							
							}
						}
					}
				}
			}
		}		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}
        		
		if( 0 <= id && id < 6
		    || 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d|%d", FMdengonidex, id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
						
            if(1 <= id2 && id2 < 11){
				sprintf(buf, "M|%s|%d|%d",FMnameWN[id2],FMpidWN[id2],FMmsgWN[id2]);
				if( bNewServer)
					lssproto_FM_send(sockfd,buf);
				else
					old_lssproto_FM_send(sockfd,buf);
			}
			if( id2 == 11){				
				sprintf(buf, "T|%d",FMmsgWN[id2]);
				if( bNewServer)
					lssproto_FM_send(sockfd,buf);
				else
					old_lssproto_FM_send(sockfd,buf);
			}
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+34, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "              ?????????б?", 0 );
			StockFontBuffer( winX+24, winY+21+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ?????? ?? ?? ? ?? ?? ?? ??????? ???????????? ??????", 0 );
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{					
					if( i<selStartLine ){						
					    StockFontBuffer( winX+34, winY+21+(i+3)*21,
						    FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
					}
					else if((i-selStartLine+1)==11){				
                        int Font_Color = FONT_PAL_GREEN;
						if(FMmsgWN[i+1] == 1) Font_Color = FONT_PAL_GREEN;
						else if(FMmsgWN[i+1] == 0) Font_Color = FONT_PAL_RED;
					    StockFontBuffer( winX+34, winY+31+(i+3)*21,
						    FONT_PRIO_FRONT, Font_Color, msgWN[i], 0 );
					}
					else{
						int  int_status;						
						strcpy( getstatus, msgWN[i]+(strlen(msgWN[i])-1));
					    int_status = atoi(getstatus);
						// ??????Χ
						if(int_status>=1 && int_status<=4)
						switch( int_status ){						    
							case 1:  // ?? ?? ??								
								strcpy(msgWN[i]+(strlen(msgWN[i])-3), "?? ?? ??");
								break;
							case 2:  // ???????								
								strcpy(msgWN[i]+(strlen(msgWN[i])-3), "???????");
								break;
							case 3:  // ???????								
								strcpy(msgWN[i]+(strlen(msgWN[i])-3), "???????");
								break;							
							case 4:  // ??    ??								
								strcpy(msgWN[i]+(strlen(msgWN[i])-3), "??    ??");
								break;							
							default:								
                                break;							
						}
						int_status = -1;
                        int Font_Color = FONT_PAL_WHITE;
						if(FMmsgWN[i+1] == 1) Font_Color = FONT_PAL_WHITE;
						else if(FMmsgWN[i+1] == 2) Font_Color = FONT_PAL_BLUE2;
						else if(FMmsgWN[i+1] == 3) Font_Color = FONT_PAL_GRAY;
						else if(FMmsgWN[i+1] == 4) Font_Color = FONT_PAL_RED;

					    StockFontBuffer( winX+34, winY+21+(i+3)*21,
						    FONT_PRIO_FRONT, Font_Color, msgWN[i], 0 );
					}
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
} 
#endif

// shan Family Dengon
#define IS_2BYTEWORD( _a_ ) ( (char)(0x80) <= (_a_) && (_a_) <= (char)(0xFF) )
void initFMWindowType1( char *data )
{	
	char m_buf[128], m_buf1[512];
	FMdengonidex = 0;
	msgWN_W  = 9;
	msgWN_H  = 9;
	msgWNLen = (msgWN_W*64-48)/9;   
	makeStringFromEscaped( data );	
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), msgWNLen+40 );    
	FMdengonidex = atoi(msgWN[7]);
    for( int i = 7; i < 14; i++ ){
		if( strlen( msgWN[i-7] ) > 0 ){					
			if( i>=selStartLine ){	
				int n = 0;
				char *p = msgWN[i-7];
				for( size_t j = 0; j < strlen(msgWN[i-7]); j++){
					if( IS_2BYTEWORD(*(p+j)) == 1){
						j++;
						continue;
					}
					else if( *(p+j) == '|')
						n++;
				}
				getStringToken(msgWN[i-7], '|', (n+1), sizeof(msgWN[i])-1, msgWN[i]);
			    getStringToken(msgWN[i-7], '|', 1, sizeof(m_buf1)-1, m_buf1);							
				for(int k=2;k<(n+1);k++){					
					getStringToken(msgWN[i-7], '|', k, sizeof(m_buf)-1, m_buf);
					strcat(m_buf1, "|");
				    strcat(m_buf1, m_buf);
				}
				strcpy(msgWN[i-7], m_buf1);
			}
		}
	}
}


// ?????????
void FMWindowType1( void )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static STR_BUFFER input;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id;
	int i, j;
	int mask;
	int btn;

	static int totalMsgLine;
	static int lineSkip;

	if( ptActMenuWin == NULL )
	{
		winW = msgWN_W;
		winH = msgWN_H;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		lineSkip = 20;
		totalMsgLine = (winH*48-56)/lineSkip;

		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}		
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = ((winH*48-56)/totalMsgLine)*(totalMsgLine-1)
					+(winH*48-totalMsgLine*lineSkip)/2+8;
			}
		}				
		msgLine = totalMsgLine - 2;
		input.buffer[0] = '\0';
		input.cnt = 0;
		input.cursor=0;
		initStrBuffer( &input,
			winX+(msgWN_W*64-msgWNLen*9)/2,
			winY+((winH*48-56)/totalMsgLine)*(totalMsgLine-2)+(winH*48-totalMsgLine*lineSkip)/2,
					msgWNLen, FONT_PAL_WHITE, FONT_PRIO_FRONT );
		GetKeyInputFocus( &input );
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || ((joy_trg[ 0 ] & JOY_ESC) /*&& GetImeString() == NULL*/)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

		// OK		
		if( id == 0 && strcmp(input.buffer,"")!=0 || id != 0)
		if( 0 <= id && id < 6 )
		{
			char msg[256],msg1[256];
			btn = 1;
			btn <<= id;

			makeEscapeString( input.buffer, msg1, sizeof( msg1 )-1 );
			sprintf( msg, "%d|%s", FMdengonidex, msg1);
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;

			windowTypeWN = -1;
		}
		if( id >= 0 )
		{
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			GetKeyInputFocus( &MyChatBuffer );			
			return;
		}
		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+200,
						winY+25,
						FONT_PRIO_FRONT, FONT_PAL_GREEN, "家 族 留 言 板", 0 );
			for( i = 0; i < 7; i++ )
			{				
				if( strlen( msgWN[i] ) > 0 )
				{
					StockFontBuffer( winX+(msgWN_W*64-msgWNLen*9)/2,
						winY+((winH*48-56)/totalMsgLine)*(i*2)+(winH*48-totalMsgLine*lineSkip)/2+30,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
				}				
				if( strlen( msgWN[i+7] ) > 0 )
				{
					StockFontBuffer( winX+(msgWN_W*64-msgWNLen*9)/2+300,
						winY+((winH*48-56)/totalMsgLine)*(i*2+1)+(winH*48-totalMsgLine*lineSkip)/2+30,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, msgWN[i+7], 0 );
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
			StockFontBuffer2( &input );			
		}
	}
}

//shan Family Point
void initFMWindowType2( char *data )
{
	makeStringFromEscaped( data );
	
	selStartLine = 0;
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 100 );
    
    char getstatus[4];
	char buf0[9],buf1[9],buf2[9],buf3[20],buf4[20];;
	char tmp[8];
	int  tmp_i;

#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
	for( int i = 0; i < fm_point_num; i++ ){
#else
	for( int i = 0; i < 4; i++ ){
#endif
		if( strlen( msgWN[i] ) > 0 ){					
			if( i>=selStartLine ){				
				getStringToken(msgWN[i], '|', 5, sizeof(getstatus)-1, getstatus);
				FMmsgWN[i-(selStartLine-1)] = atoi(getstatus);
				getStringToken(msgWN[i], '|', 1, sizeof(tmp)-1, tmp);				
				FMpidWN[i-(selStartLine-1)] = atoi(tmp);
				getStringToken(msgWN[i], '|', 2, sizeof(tmp)-1, tmp);
				getStringToken(msgWN[i], '|', 6, sizeof(buf4)-1, buf4);
									           
        tmp_i = atoi(tmp);
				//(tmp_i==100)?(strcpy(buf0,"???????")):(strcpy(buf0,"?? ? ??")); 
				if(tmp_i==100)      strcpy(buf0,"萨姆吉尔");
				else if(tmp_i==200) strcpy(buf0,"玛丽娜斯");
				else if(tmp_i==300) strcpy(buf0,"加   加");
				else if(tmp_i==400) strcpy(buf0,"卡鲁它那");
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
				else if(tmp_i==500) strcpy(buf0,"伊    甸");
				else if(tmp_i==600) strcpy(buf0,"塔 尔 塔");
				else if(tmp_i==700) strcpy(buf0,"尼 克 斯");
				else if(tmp_i==800) strcpy(buf0,"弗 烈 顿");
				else if(tmp_i==900) strcpy(buf0,"亚 伊 欧");
#endif
#ifdef _FIX_10_FMPOINT
				else if(tmp_i==1000)strcpy(buf0,"瑞尔亚斯");
#endif
				(FMmsgWN[i-(selStartLine-1)]>0)?(strcpy(buf3,buf4)):(strcpy(buf3,"未使用")); 
				//???????
				if(i<10){										
					getStringToken(msgWN[i], '|', 3, sizeof(buf1)-1, buf1);
					getStringToken(msgWN[i], '|', 4, sizeof(buf2)-1, buf2);
					sprintf(msgWN[i], "%9s%5s %5s%18s", buf0, buf1, buf2, buf3);
					sprintf(FMnameWN[i-(selStartLine-1)], "%s|%s|%s", tmp, buf1, buf2);
				} 
			}
		}
	}
}

// ??????
void FMWindowType2( void )
{
	static int winX, winY;
	static int winW, winH;
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
#ifdef _FIX_10_FMPOINT
	static int btnId[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
	static int btnLoc[12][2];
#else
	static int btnId[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
	static int btnLoc[11][2];
#endif
#else
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
#endif

	static int btnCnt;
	static int msgLine;
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};		
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;
	int  x1, y1, x2, y2;
    char buf[1024];	
	if( ptActMenuWin == NULL )
	{
		winW = 5;
		winH = 7;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
#ifdef _FIX_10_FMPOINT
		for( i = 0; i < 16; i++, mask <<= 1 )
#else
		for( i = 0; i < 15; i++, mask <<= 1 )
#endif
#else
		for( i = 0; i < 10; i++, mask <<= 1 )
#endif
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
				if( btnCnt == fm_point_num )
#else
				if( btnCnt == 4 )
#endif
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 185;
			}
		}

#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
        msgLine = fm_point_num;
#else
        msgLine = 4;
#endif
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		
		if( ptActMenuWin->hp >= 1 )	
		{
			for( i = selStartLine; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{
					x1 = winX+30;					
					y1 = winY+18+(i+3)*21;
					x2 = x1 + 260;					    
					y2 = y1 + 20;
					
					// ????峤????????
#ifdef _FMVER21
					if( pc.familyleader == FMMEMBER_LEADER && FMmsgWN[i-selStartLine+1] <= 0 )
#else
					if( pc.familyleader == 1 && FMmsgWN[i-selStartLine+1] <= 0 )
#endif
					if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) )
					{   
						if( mouse.onceState & MOUSE_LEFT_CRICK )
						{   						
							id2 = i - selStartLine + 1;                            							
						}
					}
				}
			}
		}		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}

#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
#ifdef _FIX_10_FMPOINT
		if( 0<= id && id < 16
		|| 0 <= id2 && id2 < 17
#else
		if( 0 <= id && id < 11
		 || 0 <= id2 && id2 < 16
#endif
#else
		if( 0 <= id && id < 6
		 || 0 <= id2 && id2 < 11
#endif
		 || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
#ifdef _FIX_10_FMPOINT
			if( 0 <= id && id < 12 )
#else
			if( 0 <= id && id < 11 )
#endif
#else
			if( 0 <= id && id < 6 )
#endif
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d", id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
						
			sprintf(buf, "P|%d|%s", FMpidWN[id2], FMnameWN[id2]);

#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
            if(1 <= id2 && id2 <= fm_point_num +1){
#else
            if(1 <= id2 && id2 < 5){
#endif
				if( bNewServer)
					lssproto_FM_send(sockfd,buf);
				else
					old_lssproto_FM_send(sockfd,buf);
			}

#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
			if( 0 <= id && id < fm_point_num + 2 ){
#else
			if( 0 <= id && id < 6 ){
#endif
				if( bNewServer)
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
				else
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			}
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+34, winY+28,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "         ’家族据点申请列表‘", 0 );
			StockFontBuffer( winX+30, winY+18+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_GREEN, "’地点‘  ’东‘  ’南‘        ’状  态‘", 0 );
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{					
					if( i<selStartLine ){						
					    StockFontBuffer( winX+30, winY+21+(i+3)*21,
						    FONT_PRIO_FRONT, FONT_PAL_WHITE, msgWN[i], 0 );
					}					
					else{						
                        int Font_Color = FONT_PAL_WHITE;						
						if(FMmsgWN[i+1] <= 0) Font_Color = FONT_PAL_GRAY;

					    StockFontBuffer( winX+30, winY+21+(i+3)*21,
						    FONT_PRIO_FRONT, Font_Color, msgWN[i], 0 );
					}
				}
			}
			j = 0;
#ifdef _FIX_9_FMPOINT			   // WON ADD ?????
#ifdef _FIX_10_FMPOINT
			for( i = 0, mask = 1; i < 12; i++, mask <<= 1 )
#else
			for( i = 0, mask = 1; i < 11; i++, mask <<= 1 )
#endif
#else
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
#endif
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+110+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}

#ifdef _FMVER21
// (member list) select duty
int fmselectdetuyWindow( int x, int y, int id)
{	
	static int btnId[] = {-2, -2, -2 ,-2};
	static int btnLoc[4][2];	
	static ACTION *ptActMenuWin = NULL;	
	int ret = 0;
	int dutytobtn = -1;
	char buff[64];
	char *btnTitle[] =
	{		
		"一般族员",
		"退    出",
		"长    老",			
		//"??    ??",	
		//"??    ?",
		//"?? ?? ??",
		//"?? ?? ??",
		"取    消"
	};		
	
	if( fmYesNoproc == 1){
		fmYesNoRet = fmYesNoWindow(170, 170, id);
		return 0;
	}
	if(fmYesNoRet != 0){				
		if(fmYesNoRet == 1){
			ret = FMMEMBER_NONE; 							
			fmDutyproc = 0;
			fmYesNoRet = 0;	
			return ret;
		}
	}

	if( ptActMenuWin == NULL ){
		ptActMenuWin = MakeWindowDisp( x, y, 3, 5, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		for(int i=0; i<4; i++)
			btnId[i] = -2;
		for(int j=0; j<4; j++){
			btnLoc[j][0] = x + 65;
			btnLoc[j][1] = y + 45 + 30 * (j+1);
		}
		play_se( 202, 320, 240 );
	}	    

	/*switch(FMmsgWN[id]){
	case FMMEMBER_ELDER:
		dutytobtn = 2;
		break;
	case FMMEMBER_INVITE:
		dutytobtn = 3;
		break;
	case FMMEMBER_BAILEE:
		dutytobtn = 4;
		break;
	case FMMEMBER_VICELEADER:
		dutytobtn = 5;
	default:
		break;
	}*/
		switch(FMmsgWN[id]){
	case FMMEMBER_ELDER:
		dutytobtn = 2;
		break;	
	default:
		break;
	}

	sprintf( buff, "请选择");	
	StockFontBuffer(x+20, y+25, FONT_PRIO_FRONT, FONT_PAL_WHITE, buff, 0 );
	sprintf( buff, " %s ", FMnameWN[id]);	
	StockFontBuffer(x+65, y+25, FONT_PRIO_FRONT, FONT_PAL_GREEN, buff, 0 );
	sprintf( buff, "所担任的家族职务：");	
	StockFontBuffer(x+55, y+45, FONT_PRIO_FRONT, FONT_PAL_WHITE, buff, 0 );
	if( ptActMenuWin!=NULL ){
		for(int i=0; i<4; i++)			
			/*if( pc.familyleader == FMMEMBER_ELDER && i >= 2 && i<= 5 )
				StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_GRAY, btnTitle[i], 0);*/
			if( FMelderFlag[id] == 0 && i == 2)
				StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_GRAY, btnTitle[i], 0);		
			else if( pc.familyleader == FMMEMBER_ELDER && i == 2 )
				StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_GRAY, btnTitle[i], 0);
			else if( dutytobtn == i )
				StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_GRAY, btnTitle[i], 0);					
			else if( i == 3)
				StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_WHITE, btnTitle[i], 0);					
			else
			    StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
				   			FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 0);		
	}

	for(int i=0; i<4; i++){
		int x1 = x + 50;			
		int y1 = y + 30 * (i+1) + 42;
		int x2 = x1 + 85;					    
		int y2 = y1 + 20;
		if( dutytobtn != i)
	    /*if( pc.familyleader == FMMEMBER_LEADER || 
			(pc.familyleader == FMMEMBER_ELDER || 
			pc.familyleader == FMMEMBER_VICELEADER )
			&& i >= 0 && i <= 1)*/
		if( !(FMelderFlag[id] == 0 && i == 2) )
		if( pc.familyleader == FMMEMBER_LEADER || 
			pc.familyleader == FMMEMBER_ELDER && (i >= 0 && i <= 1 || i == 3 ))
		if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){   
			if( mouse.onceState & MOUSE_LEFT_CRICK ){			
				/*switch(i){					   
				case 0:                         // ?????
					ret = FMMEMBER_MEMBER;   
					fmDutyproc = 0;								    
					break;
				case 1:							// ??? 					
			   	    fmYesNoproc = 1;		
					break;
				case 2:							// ????
					ret = FMMEMBER_ELDER;    
					fmDutyproc = 0;								    
					break;
				case 3:							// ???
					ret = FMMEMBER_INVITE;   
					fmDutyproc = 0;								    
					break;
				case 4:							// ????
					ret = FMMEMBER_BAILEE;    
					fmDutyproc = 0;								    
					break;				
				case 5:                         // ???峤
					ret = FMMEMBER_VICELEADER;
					fmDutyproc = 0;
					break;
				default:
					break;
				}*/
				switch(i){					   
				case 0:                         // ?????
					ret = FMMEMBER_MEMBER;   
					fmDutyproc = 0;								    
					break;
				case 1:							// ??? 					
			   	    fmYesNoproc = 1;		
					break;
				case 2:							// ????
					ret = FMMEMBER_ELDER;    
					fmDutyproc = 0;								    
					break;				
				case 3:
					fmDutyproc = 0;
					break;
				default:
					break;
				}
				DeathAction( ptActMenuWin );
	            ptActMenuWin = NULL;
			}
		}
	}			
	if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 ){	
		    fmDutyproc = 0;
			DeathAction( ptActMenuWin );
	        ptActMenuWin = NULL;
	}
	return ret;
}
#endif

// (member list) delete member
int fmYesNoWindow( int x, int y, int id)
{	
	static int btnId[] = {-2, -2};
	static int btnLoc[2][2];	
	static ACTION *ptActMenuWin = NULL;
	int ret = 0;
	char buff[64];
	char *btnTitle[] =
	{		
		"确   定",
		"取   消"	
	};		

	if( ptActMenuWin==NULL ){
		ptActMenuWin = MakeWindowDisp( x, y, 5, 2, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif
		for(int i=0; i<2; i++)
			btnId[i] = -2;
		for(int j=0; j<2; j++){
			btnLoc[j][0] = x + 83 * (j+1);
			btnLoc[j][1] = y + 60;
		}
		play_se( 202, 320, 240 );
	}	

	sprintf( buff, "确定要让 ％s 退出家族吗？", FMnameWN[id]);	
	StockFontBuffer(x+30, y+25, FONT_PRIO_FRONT, FONT_PAL_WHITE, buff, 0 );
	if( ptActMenuWin!=NULL ){
		for(int i=0; i<2; i++)
			StockFontBuffer( btnLoc[i][0], btnLoc[i][1],
							FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 0);		
	}

	for(int i=0; i<2; i++){
		int x1 = x + 83 * (i+1)-10;			
		int y1 = y + 58;
		int x2 = x1 + 60;					    
		int y2 = y1 + 20;
		if( MakeHitBox( x1, y1, x2, y2, DISP_PRIO_BOX2 ) ){   
			if( mouse.onceState & MOUSE_LEFT_CRICK ){			
				ret = i+1;
				fmYesNoproc = 0;				
				DeathAction( ptActMenuWin );
	            ptActMenuWin = NULL;
			}
		}
	}			
	return ret;
}

#ifdef _FMVER21
// shan Family DpList
short lnmake;
void initFMWindowType4( char *data ,int king)
{
	makeStringFromEscaped( data );

	FMdengonidex = 0;
	selStartLine = 0;
	lnmake = 0;
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 80 );
    
	time(&FMHoldTime);
    
	char lvbuf[8],fmnamebuf[36],namebuf[36],dpbuf[20],makecolor[4];	
	int sumdp = 0, taldp = 0;
#ifdef _NEW_MANOR_LAW
	int momentum = 0,fmmomentum = 0;
#endif

	FMdengonidex = atoi(msgWN[10]);
	for( int i = 0; i < 10; i++ ){
		if( strlen( msgWN[i] ) > 0 ){
			if( i>=selStartLine ){
				if(king == 0){
					//???????
					getStringToken(msgWN[i], '|', 2, sizeof(lvbuf)-1, lvbuf);
					getStringToken(msgWN[i], '|', 3, sizeof(fmnamebuf)-1, fmnamebuf);
					makeStringFromEscaped( fmnamebuf );
					getStringToken(msgWN[i], '|', 4, sizeof(namebuf)-1, namebuf);
					makeStringFromEscaped( namebuf );
					getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);
					sumdp = atoi(dpbuf);
					getStringToken(msgWN[i], '|', 6, sizeof(dpbuf)-1, dpbuf);
					taldp = atoi(dpbuf);
					getStringToken(msgWN[i], '|', 7, sizeof(makecolor)-1, makecolor);
					if( strcmp(makecolor,"1") == 0 ) lnmake = i;
					sprintf(msgWN[i]," %-7s%-18s%-16s%13d%12d%14d%", lvbuf, fmnamebuf, namebuf, sumdp, taldp-sumdp, taldp);
				}
				else if(king == 1){
#ifdef _NEW_MANOR_LAW
					getStringToken(msgWN[i], '|', 2, sizeof(lvbuf)-1, lvbuf);
					getStringToken(msgWN[i], '|', 3, sizeof(fmnamebuf)-1, fmnamebuf);
					makeStringFromEscaped( fmnamebuf );
					getStringToken(msgWN[i], '|', 4, sizeof(namebuf)-1, namebuf);
					makeStringFromEscaped( namebuf );
					getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);
					fmmomentum = atoi(dpbuf);
					getStringToken(msgWN[i], '|', 6, sizeof(dpbuf)-1, dpbuf);
					momentum = atoi(dpbuf);
					sprintf(msgWN[i],"%-7s%-16s%-16s%8d%12d",lvbuf,fmnamebuf,namebuf,fmmomentum,momentum);
#endif
				}
			}
		}
	}
}

// ????????
#ifdef _NEW_MANOR_LAW
void FMWindowType4(int king)
#else
void FMWindowType4( void )
#endif
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static int fmdelid;	
	char *btnTitle[] =
	{
				" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};		
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;	
	
	if( ptActMenuWin == NULL )
	{
#ifdef _NEW_MANOR_LAW
		if(king == 0){
			winW = 10;
			winH = 7;
		}
		else if(king == 1){
			winW = 8;
			winH = 7;
		}
#else
		winW = 10;
		winH = 7;
#endif
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 295;
			}
		}
        msgLine = 10;
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		
		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}
        		
		if( 0 <= id && id < 6
		    || 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d|%d", FMdengonidex, id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
      
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
#ifdef _NEW_MANOR_LAW
			if(king == 0){
				StockFontBuffer( winX+235, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "’史上最强之家族强者排行榜‘", 0 );
				StockFontBuffer( winX+25, winY+16+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_GREEN, "’名次‘ ’家 族 名 称‘  ’族 长 名 称‘   ’综 合 声 望‘   ’成 员 声 望‘  ’家 族 总 声 望‘", 0 );
			}
			else if(king == 1){
				StockFontBuffer( winX+155, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "’史上最强之家族强者排行榜‘", 0 );
				StockFontBuffer( winX+25, winY+16+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_GREEN, "’名次‘ ’家 族 名 称‘  ’族 长 名 称‘ ’家族总气势‘  ’个人气势‘", 0 );
			}
#else
			StockFontBuffer( winX+235, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "???????????????????а?", 0 );
			StockFontBuffer( winX+25, winY+16+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_GREEN, "?????Ρ? ????  ??  ??  ??? ????  ??  ??  ??? ????????????????????????????????????", 0 );
#endif
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{		
					int fontcolor = FONT_PAL_WHITE;
                    if( lnmake != 0){
						fontcolor = FONT_PAL_GRAY;
						if( lnmake == i ) fontcolor = FONT_PAL_WHITE;
					}
					if( i<selStartLine ){
#ifdef _NEW_MANOR_LAW
						if(king == 0) StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
						else if(king == 1) StockFontBuffer( winX+60, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
#else
					  StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
#endif
					}
#ifdef _NEW_MANOR_LAW
					if(king == 0) StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
					else if(king == 1) StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
#else
					StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
#endif
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}

void initFMWindowType3( char *data )
{
	makeStringFromEscaped( data );

	FMdengonidex = 0;
	selStartLine = 0;
	lnmake = 0;
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 50 );
    
	time(&FMHoldTime);
    
	char lvbuf[8],fmnamebuf[36],namebuf[36],dpbuf[20],makecolor[4];	
	
	FMdengonidex = atoi(msgWN[10]);
	for( int i = 0; i < 10; i++ ){
		if( strlen( msgWN[i] ) > 0 ){
			if( i>=selStartLine ){
				//???????
				getStringToken(msgWN[i], '|', 2, sizeof(lvbuf)-1, lvbuf);
				getStringToken(msgWN[i], '|', 3, sizeof(fmnamebuf)-1, fmnamebuf);
				makeStringFromEscaped( fmnamebuf );
				getStringToken(msgWN[i], '|', 4, sizeof(namebuf)-1, namebuf);
				makeStringFromEscaped( namebuf );
				getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);
                getStringToken(msgWN[i], '|', 6, sizeof(makecolor)-1, makecolor);
				if( strcmp(makecolor,"1") == 0 ) lnmake = i;
				sprintf(msgWN[i], " %-9s%-19s%-17s%12s", lvbuf, fmnamebuf, namebuf, dpbuf);
			}
		}
	}
}

// ???????? top30
#ifdef _NEW_MANOR_LAW
void FMWindowType3(int king)
#else
void FMWindowType3( void )
#endif
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static int fmdelid;	
#ifdef _NEW_MANOR_LAW
	char szMessage[128];
#endif
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};		
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;	
	
	if( ptActMenuWin == NULL )
	{
		winW = 8;
		winH = 7;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 295;
			}
		}
        msgLine = 10;
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		
		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}
        		
		if( 0 <= id && id < 6
		    || 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d|%d", FMdengonidex, id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
      
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+34, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "                  ‘史上最强之家族强者排行榜’", 0 );
			
#ifdef _NEW_MANOR_LAW
			if(king == 0) sprintf(szMessage,"‘名词’  ‘家 族 名 称’  ‘族 长 名 称’   ‘家族声望值’");
			else if(king == 1) sprintf(szMessage,"‘名词’  ‘家 族 名 称’  ‘族 长 名 称’   ‘家族气势值’");
			StockFontBuffer( winX+39, winY+16+2*21,FONT_PRIO_FRONT, FONT_PAL_GREEN,szMessage,0);
#else
			StockFontBuffer( winX+39, winY+16+2*21,FONT_PRIO_FRONT, FONT_PAL_GREEN, "???? ?Ρ? ????  ??  ??  ??? ????  ??  ??  ???  ?????????????", 0 );
#endif
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{		
					int fontcolor = FONT_PAL_WHITE;
                    if( lnmake != 0){
						fontcolor = FONT_PAL_GRAY;
						if( lnmake == i ) fontcolor = FONT_PAL_WHITE;
					}
					if( i<selStartLine ){						
					    StockFontBuffer( winX+58, winY+14+(i+3)*21,
						    FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
					}
					 StockFontBuffer( winX+58, winY+14+(i+3)*21,
					                  FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );					
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}
#else
short lnmake;
void initFMWindowType3( char *data )
{
	makeStringFromEscaped( data );

	FMdengonidex = 0;
	selStartLine = 0;
	lnmake = 0;
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
		sizeof( msgWN )/sizeof( msgWN[0] ), 50 );
    
	time(&FMHoldTime);
    
	char lvbuf[8],fmnamebuf[36],namebuf[36],dpbuf[20],makecolor[4];	
	
	FMdengonidex = atoi(msgWN[10]);
	for( int i = 0; i < 10; i++ ){
		if( strlen( msgWN[i] ) > 0 ){
			if( i>=selStartLine ){
				//???????
				getStringToken(msgWN[i], '|', 2, sizeof(lvbuf)-1, lvbuf);
				getStringToken(msgWN[i], '|', 3, sizeof(fmnamebuf)-1, fmnamebuf);
				makeStringFromEscaped( fmnamebuf );
				getStringToken(msgWN[i], '|', 4, sizeof(namebuf)-1, namebuf);
				makeStringFromEscaped( namebuf );
				getStringToken(msgWN[i], '|', 5, sizeof(dpbuf)-1, dpbuf);
                getStringToken(msgWN[i], '|', 6, sizeof(makecolor)-1, makecolor);
				if( strcmp(makecolor,"1") == 0 ) lnmake = i;
				sprintf(msgWN[i], " %-9s%-19s%-17s%12s", lvbuf, fmnamebuf, namebuf, dpbuf);
			}
		}
	}
}

// ????????
void FMWindowType3( void )
{
	static int winX, winY;
	static int winW, winH;
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	static int msgLine;
	static int fmdelid;	
	char *btnTitle[] =
	{
		" ??? ",
		"CANCEL",
		"?  ??",
		"?  ??",
		"????",
		"????"
	};		
	int  id, id2;
	int  i, j;
	int  mask;
	int  btn;	
	
	if( ptActMenuWin == NULL )
	{
		winW = 8;
		winH = 7;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( btnId )/sizeof( int ); i++ )
		{
			btnId[i] = -2;
		}
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 295;
			}
		}
        msgLine = 10;
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		id2 = -1;		
		
		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
		}
        		
		if( 0 <= id && id < 6
		    || 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			char data[256];
			char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			if( id2 < 0 )
				id2 = 0;
			sprintf( data, "%d|%d", FMdengonidex, id2 );
			makeEscapeString( data, msg, sizeof( msg )-1 );
      
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, msg ) ;			
			windowTypeWN = -1;
		}
		if( id >= 0 || id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+34, winY+21,
						    FONT_PRIO_FRONT, FONT_PAL_YELLOW, "                  ???????????????????а?", 0 );
			StockFontBuffer( winX+39, winY+16+2*21,
						    FONT_PRIO_FRONT, FONT_PAL_GREEN, "???? ?Ρ? ????  ??  ??  ??? ????  ??  ??  ???  ?????????????", 0 );
			for( i = 0; i < msgLine; i++ )
			{
				if( strlen( msgWN[i] ) > 0 )
				{		
					int fontcolor = FONT_PAL_WHITE;
                    if( lnmake != 0){
						fontcolor = FONT_PAL_GRAY;
						if( lnmake == i ) fontcolor = FONT_PAL_WHITE;
					}
					if( i<selStartLine ){						
					    StockFontBuffer( winX+58, winY+14+(i+3)*21,
						    FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );
					}
					 StockFontBuffer( winX+58, winY+14+(i+3)*21,
					                  FONT_PRIO_FRONT, fontcolor, msgWN[i], 0 );					
				}
			}
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}
		}
	}
}
#endif
/*----------   Shan(END)   ----------*/

// Robin ???????
#if 0	

typedef struct {
	int index;
	char name[64];
	int graNo;
	char memo[128];
} itemPool;

static int havenum;
itemPool myPool[15];
itemPool fmPool[15];
static int poolPage = 0;

void initItemman( char* data )
{
	int i;
	char buf[1024];

	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	windowTypeWN = WINDOW_MESSAGETYPE_ITEMMAN;

	havenum = getIntegerToken( data, '|',  3 );

	//poolPage = 1;
	for( i=0; i < havenum; i++ ) 
	{
		myPool[i].index = getIntegerToken( data, '|',  3+((i*4)+1) );
		getStringToken( data, '|', 3+((i*4)+2), sizeof( buf )-1, buf );
		strcpy( myPool[i].name, buf);
		myPool[i].graNo = getIntegerToken( data, '|',  3+((i*4)+3) );
		getStringToken( data, '|', 3+((i*4)+4), sizeof( buf )-1, buf );
		strcpy( myPool[i].memo, buf);
	}
	for( i=havenum; i < 15; i++ )
	{
		myPool[i].index = 0;
		strcpy( myPool[i].name, "");
		myPool[i].graNo = 0;
		strcpy( myPool[i].memo, "");
	}

}

void closeItemmanWN( void )
{

	play_se( 203, 320, 240 );	// ??????????
	windowTypeWN = -1;
	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;

}


int itemmanWN( void )
{
	static int w, h, x, y, xx, yy;
	static int btnId[5];
	static int fontId[15];
	int i;
	int selBtnId = -1;
	int selFontBtnId = -1;
	char buf[1024];
	int prevBtnGraNo[] = { CG_PREV_BTN, CG_PREV_BTN_DOWN };
	int nextBtnGraNo[] = { CG_NEXT_BTN, CG_NEXT_BTN_DOWN };
	int itemColor[] = { FONT_PAL_WHITE, FONT_PAL_GRAY, FONT_PAL_RED };
	int prevBtn=0, nextBtn=0;


	if( CheckMenuFlag()
	 || joy_trg[ 0 ] & JOY_ESC
	 || actBtn == 1
	 || menuBtn == 1
	 || disconnectServerFlag == true
	 || wnCloseFlag == 1 )
	{
		closeItemmanWN();
		return 0;
	}

	if( ptActMenuWin == NULL )
	{
		//familyListProcNo = 11;

		w=432; h=420;
		x = (640-w)/2;
		y = (480-h)/2;
		 
		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, -1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif	
		//InitItem4( 325, 230 );
		play_se( 202, 320, 240 );	// ????????	
		
	}

	if( ptActMenuWin->hp >= 1 )
	{

		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );
		selFontBtnId = focusFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( mouse.onceState & MOUSE_LEFT_CRICK )
		{
			switch( selBtnId ) {
			case 0:
				closeItemmanWN();
				return 0;
			case 1:
				if( poolPage+1 > 1 )
					poolPage --;
				break;
			case 2:
				if( poolPage+1 < (havenum/7)+1 )
					poolPage ++;
				break;
			}

			if( selFontBtnId != -1 ) {
				sprintf( buf, "B|I|%d", myPool[(poolPage*7)+selFontBtnId].index );
				if( bNewServer)
					lssproto_FM_send( sockfd, buf );
				else
					old_lssproto_FM_send( sockfd, buf );
				return 0;
			}
		}

		if( (selFontBtnId>=0)&&(selFontBtnId<15) ) {
			StockFontBuffer( x+110, y+300, FONT_PRIO_FRONT, 0, myPool[(poolPage*7)+selFontBtnId].name, 0 );
			StockFontBuffer( x+110, y+320, FONT_PRIO_FRONT, 0, myPool[(poolPage*7)+selFontBtnId].memo, 0 );
			StockDispBuffer( x + 65, y + 345, DISP_PRIO_IME3, myPool[(poolPage*7)+selFontBtnId].graNo, 0 );
		}

		for( i= 0 ; i< 7 ; i++ )
		{
				fontId[i] = StockFontBuffer( x+34, y+120+(i*21),
							FONT_PRIO_FRONT, 0, myPool[(poolPage*7)+i].name, 2 );
		}
			
		btnId[0] = StockDispBuffer( x + 220, y + 400, DISP_PRIO_IME3, CG_CANCEL_BTN, 2 );
		btnId[1] = StockDispBuffer( x +32, y+100, DISP_PRIO_IME3, prevBtnGraNo[prevBtn], 2 );
		btnId[2] = StockDispBuffer( x+200, y+100, DISP_PRIO_IME3, nextBtnGraNo[nextBtn], 2 );
		sprintf( buf, "%2d/%2d ?", poolPage+1, (havenum/7)+1 );
		StockFontBuffer( x +66, y+92, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

		StockFontBuffer( x+175, y+18, FONT_PRIO_FRONT, 0, "???????", 0 );
		StockDispBuffer( x+w/2, y+h/2, DISP_PRIO_MENU, CG_ITEMSHOP_WIN, 1 );
	}
	return 0;
}

#endif

// ???????????????????
// return:		0 ... ??????
//				1 ... ????OK
//				2 ... ????Cancel
int DelCharGraColorWin( void )
{
	static int fontId[] = { -2, -2};
	static bool bErr = false;
	static ACTION *ptActMenuWin = NULL;
	int id;
	int i;
	static int x, y, w, h;
	int ret = 0;
	char msg[][16] ={
		" 确定 ",
		" 取消 "
	};
	// ?????
	if( selCharGraColorWinProcNo == 0 ){
		selCharGraColorWinProcNo = 1;
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
		//???????
		w = 3;
		h = 2;
		x = 250;
		y = 180;

		//Change fix ????????λ??
		y = 355;
		if( selectPcNo == 0 )
			x = 110;
		else
			x = 410;

		ptActMenuWin = MakeWindowDisp( x, y, w, h, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x;
		y = ptActMenuWin->y;
#endif	
		initStrBuffer( &passwd, x+20, y+44, 20, FONT_PAL_WHITE, FONT_PRIO_FRONT );
		// ?趨?????????StrBuffer
		passwd.cnt = 0;
		passwd.cursor=0;
		passwd.filterFlag = BLIND_TYPE;
		GetKeyInputFocus( &passwd );
		bErr = false;
	}
	// ?ж??????????
	id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
	// ???????
	if( id == 0 ){
		char sztemp[32];
		memcpy(sztemp,szPassword,32);
		ecb_crypt("f;encor1c",sztemp,32,DES_DECRYPT);
		if( passwd.cnt==strlen(sztemp) && !strncmp( sztemp, passwd.buffer, passwd.cnt)){
			ret = 1;
			bErr = false;
			play_se( 217, 320, 240 );	// click??
		}else{
			passwd.cnt = 0;
			passwd.cursor=0;
			bErr = true;
		}
	}else
	// ???????
	if( id == 1 ){
		ret = 2;
		play_se( 217, 320, 240 );	// click??
	}
	// ?????????
	if( ret != 0 ){
		if( ptActMenuWin ){
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
	}
	// ??????????
	id = focusFontId( fontId, sizeof( fontId )/sizeof( int ) );
	// ???
    if( id == 0 ) {}
		//ShowBottomLineString(FONT_PAL_WHITE,"???????????????????");
	else
	 {}
	// ???
	if( id == 1 )
	{}
		//ShowBottomLineString(FONT_PAL_WHITE,"??????????");
	if( ptActMenuWin != NULL ){
		// ?????????
		if( ptActMenuWin->hp >= 1 ){
			for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ ){

				fontId[i] = StockFontBuffer( x+50+i*48, y+64,
					FONT_PRIO_FRONT, FONT_PAL_YELLOW, msg[i], 2 );
			}
			if( bErr)
				StockFontBuffer( x+20, y+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, "输入错误请再试", 0);
			else
				StockFontBuffer( x+20, y+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, "请输入密码", 0);
			StockFontBuffer2( &passwd);
			FlashKeyboardCursor();	// ???????????????
		}
	}
	return ret;
}




// Robin 0712

struct FMPKData {
	char time[6];
	char atkFMName[20];
	char defFMName[20];
	int	readyTime;
	int member;
	int flag;
	int win;
};
FMPKData FMPKDataList[4];
int serverTime;

char winStr[][32] =
{
	"乱斗生存战",
	"满场打飞战"
};


void initFMPKListWN( char *data )
{
	int i;
	char buf2[256];

	getStringToken( data, '|', 1, sizeof( buf2 )-1, buf2 );
	serverTime = atoi(buf2);

	for( i=0; i<4; i++ )
	{
		getStringToken( data, '|', i*7+2, sizeof( buf2 )-1, buf2 );
		strcpy( FMPKDataList[i].time, buf2);
		getStringToken( data, '|', i*7+3, sizeof( buf2 )-1, buf2 );
		strcpy( FMPKDataList[i].atkFMName, makeStringFromEscaped(makeStringFromEscaped(buf2)) );
		getStringToken( data, '|', i*7+4, sizeof( buf2 )-1, buf2 );
		strcpy( FMPKDataList[i].defFMName, makeStringFromEscaped(makeStringFromEscaped(buf2)) );
		getStringToken( data, '|', i*7+5, sizeof( buf2 )-1, buf2 );
		FMPKDataList[i].readyTime = atoi(buf2);
		getStringToken( data, '|', i*7+6, sizeof( buf2 )-1, buf2 );
		FMPKDataList[i].member = atoi(buf2);
		getStringToken( data, '|', i*7+7, sizeof( buf2 )-1, buf2 );
		FMPKDataList[i].flag = atoi(buf2);
		getStringToken( data, '|', i*7+8, sizeof( buf2 )-1, buf2 );
		FMPKDataList[i].win = atoi(buf2);

	}

}



void FMPKListWN( int mode )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	//static int msgLine;
	//static STR_BUFFER input;
	char dataBuf[512];
	char dataBuf2[256];
	char buf[256];

	char *dataStatusStr[] =
	{
		" 新  增 ",
		" 修  改 ",
		" 接  受 ",
		"设 定 中",
		"等待回答",
		"已 排 定"
	};
	int showcolor;
	int pushStatus;
	
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id ;
	int i, j;
	int mask;
	int btn;


	static int totalMsgLine;
	static int lineSkip;


	if( ptActMenuWin == NULL )
	{
		winW = 8;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif	
		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		

		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}

		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = winH*48-40;
			}
		}


		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}

		// ???
		if( 0 <= id && id < 6 )
		{
			//char msg[256];
			btn = 1;
			btn <<= id;

			//makeEscapeString( input.buffer, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;

			windowTypeWN = -1;

		} 
		else if( (6 <= id) && (id < 10) )
		{

			int dataSel = id -6;
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, FMPKDataList[dataSel].time ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, FMPKDataList[dataSel].time ) ;
		}


		if( id >= 0 )
		{
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}
		

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{

			
			j = 0;
			
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}


			StockFontBuffer( winX+200, winY+30,	FONT_PRIO_FRONT, FONT_PAL_WHITE,
							"家族对战时间表", 0 );
			sprintf( dataBuf, "目前时间： %d:%d", serverTime/100, serverTime%100 );
			StockFontBuffer( winX+360, winY+45,	FONT_PRIO_FRONT, FONT_PAL_WHITE,
							dataBuf, 0 );
			//StockDispHLine( winX+80, winY+30, 2 );

			sprintf( dataBuf, "%10s  %16s  %16s  %8s  %8s"
					, "开战时间", "挑战家族", "受邀家族" 
					, "准备时间", "参加人数" );

			StockFontBuffer( winX+20, winY+70,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			for( i=0; i<4; i++ )
			{
				//char min[3], sec[3];
				int pktime = atoi(FMPKDataList[i].time);
				if( pktime >= 10000 ) {
				//if( strlen(FMPKDataList[i].time) > 4 ) {
					//sscanf(FMPKDataList[i].time+1, "%2s%2s", min, sec);
					//sprintf( buf, "???? %d:%d", (pktime-10000)/100, pktime%100 );
					sprintf( buf, "明天 %d:00", (pktime-10000)/100 );
					//sprintf( buf, "???? %s:%s", min, sec );
				}
				else {
					//sscanf(FMPKDataList[i].time, "%2s%2s", min, sec);
					//sprintf( buf, "???? %d:%d", pktime/100, pktime%100 );
					sprintf( buf, "今天 %d:00", pktime/100 );
					//sprintf( buf, "???? %s:%s", min, sec );
				}
#ifdef _READ16BITBMP
				if(!g_bUseAlpha) StockDispHLine( winX+36, winY+90+(i*60), 7 );
#endif

				if( FMPKDataList[i].flag != -1 )
				{
					sprintf( dataBuf, "%10s  %16s  %16s  %4d????  %6d??"
						, buf, FMPKDataList[i].atkFMName, FMPKDataList[i].defFMName
						, FMPKDataList[i].readyTime, FMPKDataList[i].member );
					sprintf( dataBuf2, "胜利条件： %s", winStr[FMPKDataList[i].win] );

					switch ( FMPKDataList[i].flag ) {
					case -1:
						showcolor = FONT_PAL_WHITE;
						pushStatus = 2;
						break;
					case 0:
					case 1:
						showcolor = FONT_PAL_YELLOW;
						pushStatus = 2;
						break;
					case 2:
					case 3:
						showcolor = FONT_PAL_GREEN;
						pushStatus = 0;
						break;
					case 4:
						showcolor = FONT_PAL_RED;
						pushStatus = 0;
						break;
					}
#ifdef _FMVER21
					if( pc.familyleader != FMMEMBER_LEADER)	pushStatus = 0;
#else
					if( pc.familyleader != 1)	pushStatus = 0;
#endif					
					StockFontBuffer( winX+20, winY+106+(i*60),
								FONT_PRIO_FRONT, showcolor, dataBuf, 0 );
					StockFontBuffer( winX+50, winY+130+(i*60),
								FONT_PRIO_FRONT, showcolor, dataBuf2, 0 );

					fontId[i+6] = StockFontBuffer( winX+400, winY+100+(i*60)+30,
								FONT_PRIO_FRONT, showcolor,
								dataStatusStr[FMPKDataList[i].flag+1], pushStatus );
				}
				else
				{
					sprintf( dataBuf, "%10s" , buf );
					StockFontBuffer( winX+20, winY+106+(i*60),
								FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

#ifdef _FMVER21
					if( pc.familyleader != FMMEMBER_LEADER)	pushStatus = 0;
#else
					if( pc.familyleader != 1)	pushStatus = 0;
#endif
					else pushStatus = 2;
					fontId[i+6] = StockFontBuffer( winX+400, winY+100+(i*60)+30,
								FONT_PRIO_FRONT, FONT_PAL_WHITE,
								dataStatusStr[0], pushStatus );
				}

			} // for List end





		}
	}
}

struct FMPKSelect {
	char fmname[16+1];
	int	fmindex;
};
FMPKSelect FMPKSelectData[8];
int FMPKSelectNum;

void initFMPKSelectWN( char *data )
{

	int i;
	char buf[256], buf2[256];


	getStringToken( data, '|', 1, sizeof( buf )-1, buf );
	FMPKSelectNum = atoi(buf);

	for( i=0; i<FMPKSelectNum; i++ )
	{

		getStringToken( data, '|', i*2+2, sizeof( buf2 )-1, buf2 );
		FMPKSelectData[i].fmindex = atoi(buf2);
		getStringToken( data, '|', i*2+3, sizeof( buf2 )-1, buf2 );
		strcpy( FMPKSelectData[i].fmname, makeStringFromEscaped(buf2) );

	}

}

void FMPKSelectWN( int mode )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2,
							-2, -2, -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	//static int msgLine;
	//static STR_BUFFER input;
	char dataBuf[256];
	char buf[256];

	//int	dataStatus;
	/*
	char *dataStatusStr[] =
	{
		"δ???",
		" ??? ",
		" ???? ",
		"?趨??",
		"??????",
		"?????"
	};
	*/

	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id ;
	int i, j;
	int mask;
	int btn;


	static int totalMsgLine;
	static int lineSkip;


	if( ptActMenuWin == NULL )
	{
		//winW = msgWN_W;
		//winH = msgWN_H;
		winW = 4;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		//lineSkip = 20;
		//totalMsgLine = (winH*48-56)/lineSkip;

		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}

		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = winH*48-56;
			}
		}

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}

		// ???
		if( 0 <= id && id < 6 )
		{
			//char msg[256];
			btn = 1;
			btn <<= id;

			//makeEscapeString( input.buffer, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;

			windowTypeWN = -1;

		} 
		else if( (6 <= id) && (id < 6+8) )
		{

			
			int dataSel = id -6;
			sprintf( dataBuf, "%d|%s", FMPKSelectData[dataSel].fmindex, 
						makeEscapeString(FMPKSelectData[dataSel].fmname, buf, sizeof(buf)) );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, dataBuf ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, dataBuf ) ;
		}

		
		if( id >= 0 )
		{
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}
		

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{

			
			j = 0;
			
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}

			sprintf( dataBuf, "%s", "请选择项挑战的家族：" );
			StockFontBuffer( winX+30, winY+40,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			for( i=0; i<FMPKSelectNum; i++ )
			{
				sprintf( dataBuf, "%-16s", FMPKSelectData[i].fmname );

				fontId[i+6] = StockFontBuffer( winX+40, winY+60+(i*30)+30,
								FONT_PRIO_FRONT, FONT_PAL_WHITE,
								dataBuf, 2 );

			} // for List end

		}
	}

}

struct FMPKDetail {
	char time[4+1];
	char atkFMName[16+1];
	char defFMName[16+1];
	int readyTime;
	int member;
	int win;
};
FMPKDetail FMPKDetailData;


void initFMPKDetailWN( char *data )
{

	char buf2[256];

	getStringToken( data, '|', 1, sizeof( buf2 )-1, buf2 );
	strcpy( FMPKDetailData.time, makeStringFromEscaped(buf2) );
	getStringToken( data, '|', 2, sizeof( buf2 )-1, buf2 );
	strcpy( FMPKDetailData.atkFMName, makeStringFromEscaped(buf2) );
	getStringToken( data, '|', 3, sizeof( buf2 )-1, buf2 );
	strcpy( FMPKDetailData.defFMName, makeStringFromEscaped(buf2) );
	getStringToken( data, '|', 4, sizeof( buf2 )-1, buf2 );
	FMPKDetailData.readyTime = atoi(buf2);
	getStringToken( data, '|', 5, sizeof( buf2 )-1, buf2 );
	FMPKDetailData.member = atoi(buf2);
	getStringToken( data, '|', 6, sizeof( buf2 )-1, buf2 );
	FMPKDetailData.win = atoi(buf2);

}



void FMPKDetailWN( int mode )
{

	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2, -2};
	static int btnId[] = { -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;
	//static int msgLine;
	//static STR_BUFFER input;
	static int pushBtnFlag[4];
	int selBtnId;
	char dataBuf[256];
	char buf[256];
	char buf2[256];


	char *btnTitle[] =
	{
				" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id ;
	int i, j;
	int mask;
	int btn;


	static int totalMsgLine;
	static int lineSkip;


	if( ptActMenuWin == NULL )
	{
		winW = 6;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;

		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}

		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = winH*48-56;
			}
		}

		play_se( 202, 320, 240 );
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1
		 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}

		// ???
		if( 0 <= id && id < 6 )
		{
			btn = 1;
			btn <<= id;

			sprintf( dataBuf, "%s|%s|%s|%d|%d|%d",
					FMPKDetailData.time,
					makeEscapeString( FMPKDetailData.atkFMName, buf, sizeof(buf) ),
					makeEscapeString( FMPKDetailData.defFMName, buf2, sizeof(buf2) ),
					FMPKDetailData.readyTime,
					FMPKDetailData.member,
					FMPKDetailData.win
					);
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, dataBuf ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, dataBuf ) ;

			windowTypeWN = -1;

		}
		else if( id == 6 )
		{
			FMPKDetailData.win ++;
			if( FMPKDetailData.win > 1 )
				FMPKDetailData.win = 0;
		}
		
		if( mouse.onceState & MOUSE_LEFT_CRICK )
		switch( selBtnId ) {
		case 0:
			pushBtnFlag[0] = true;
			FMPKDetailData.readyTime ++;
			if( FMPKDetailData.readyTime > 40 )
				FMPKDetailData.readyTime = 1;
			break;
		case 1:
			pushBtnFlag[1] = true;
			FMPKDetailData.readyTime --;
			if( FMPKDetailData.readyTime < 1 )
				FMPKDetailData.readyTime = 40;
			break;
		case 2:
			pushBtnFlag[2] = true;
			FMPKDetailData.member ++;
			if( FMPKDetailData.member > 50 )
				FMPKDetailData.member = 1;
			break;
		case 3:
			pushBtnFlag[3] = true;
			FMPKDetailData.member --;
			if( FMPKDetailData.member < 1 )
				FMPKDetailData.member = 50;
			break;

		}


		if( id >= 0 && id < 6 )
		{
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}


		for( int i=0 ; i<4 ; i++ )
		{
			if( mouse.state & MOUSE_LEFT_CRICK && pushBtnFlag[i] == true )
				pushBtnFlag[i] = true;
			else	pushBtnFlag[i] = false;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{


			j = 0;

			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}

			StockFontBuffer( winX+140, winY+30,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, "家族对战申请表", 0 );
#ifdef _READ16BITBMP
			if(!g_bUseAlpha) StockDispHLine( winX+100, winY+50, 3 );
#endif

			sprintf( dataBuf, "%s %s", "挑战家族", FMPKDetailData.atkFMName );
			StockFontBuffer( winX+40, winY+90,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			sprintf( dataBuf, "%s %s", "受邀家族", FMPKDetailData.defFMName );
			StockFontBuffer( winX+40, winY+90+40,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			sprintf( dataBuf, "准备时间 %d 分钟", FMPKDetailData.readyTime );
			StockFontBuffer( winX+40, winY+90+80,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			btnId[0] = StockDispBuffer( winX+190, winY+98+80, DISP_PRIO_IME3,
									CG_UP_BTN + pushBtnFlag[0], 2 );
			btnId[1] = StockDispBuffer( winX+190+40, winY+98+80, DISP_PRIO_IME3,
									CG_DOWN_BTN + pushBtnFlag[1], 2 );

			sprintf( dataBuf, "参加人数： %d 人", FMPKDetailData.member );
			StockFontBuffer( winX+40, winY+90+120,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );
			
			btnId[2] = StockDispBuffer( winX+190, winY+98+120, DISP_PRIO_IME3,
									CG_UP_BTN + pushBtnFlag[2], 2 );
			btnId[3] = StockDispBuffer( winX+190+40, winY+98+120, DISP_PRIO_IME3,
									CG_DOWN_BTN + pushBtnFlag[3], 2 );

			sprintf( dataBuf, "胜利条件 %s", winStr[FMPKDetailData.win] );
			fontId[6] = StockFontBuffer( winX+40, winY+90+160,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 2 );

		}
	}


}

int familyTax;
int familyTaxChange;
int haveGold;

void initFamilyTaxWN( char * data )
{
	char buf[256];
	getStringToken( data, '|', 3, sizeof( buf )-1, buf );
	familyTax = atoi(buf);
	
	familyTaxChange = familyTax;
	haveGold = pc.gold;

	if( windowTypeWN != WINDOW_MESSAGETYPE_FAMILYTAX )
	{
		windowTypeWN = WINDOW_MESSAGETYPE_FAMILYTAX;
		DeathAction( ptActMenuWin );
		ptActMenuWin = NULL;
	}

}

void closeFamilyTaxWN()
{
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
}

void familyTaxWN()
{
	
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2 };
	static int btnId[] = { -2, -2, -2, -2 };
	static int pushBtnFlag[4];
	static int taxGoldInc =0, taxGoldDec =0;
	static int taxGoldCnt =0;
	int selBtnId;
	char dataBuf[256];
	char buf[256];
	int id ;
	int i;


	if( ptActMenuWin == NULL )
	{
		winW = 5;
		winH = 4;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		play_se( 202, 320, 240 );
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			closeFamilyTaxWN();
		}


		if( mouse.onceState & MOUSE_LEFT_CRICK )

		switch( selBtnId ) {
		case 0:
			sprintf( buf, "B|T|%d", familyTaxChange - familyTax );
			//sprintf( buf, "B|T|%d", 79000000 );
			if( bNewServer)
				lssproto_FM_send( sockfd, buf );
			else
				old_lssproto_FM_send( sockfd, buf );
			closeFamilyTaxWN();
			return;
			break;
		case 1:
			closeFamilyTaxWN();
			return;
			break;
		}



#if 1
		switch( selBtnId ) {
		
		// Add Gold
		case 2:
			if( mouse.onceState & MOUSE_LEFT_CRICK_UP && pushBtnFlag[2] == true ){
							
				pushBtnFlag[2] = false;
			}

			if( ( haveGold < CHAR_getMaxHaveGold() )&&( familyTaxChange > 0 ) ) {
							
				if( pushBtnFlag[2] == true ){

					haveGold += taxGoldInc;
					//bankGold -= bankGoldInc;
					taxGoldCnt++;

					// ???????
					if( taxGoldCnt >= 30 ){ 
						// ?????????????????
						taxGoldCnt = 0;		
						// ????????
						if( taxGoldInc == 0 ) {
							taxGoldInc = 1;
						}else{
							// ???????
							taxGoldInc *= 5;
							// ????????
							if( taxGoldInc > 10000 ) {
								taxGoldInc = 10000;		
							}
						}
					}
					// ????????
					if( haveGold >= CHAR_getMaxHaveGold() ) {
						haveGold = CHAR_getMaxHaveGold();
						play_se( 220, 320, 240 );
					}
					if( haveGold >= pc.gold + familyTax ) {
						haveGold = pc.gold + familyTax;
						play_se( 220, 320, 240 );
					}
								
				}

				// ????????
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					// ???
					haveGold++;
					// ????????
					if( haveGold >= CHAR_getMaxHaveGold() ) { 
						haveGold = CHAR_getMaxHaveGold();
						// ???
						play_se( 220, 320, 240 );
					} else {
						// ?????????
						taxGoldInc = 0;		
						// ?????????????????
						taxGoldCnt = 0;		
						// ???????
						pushBtnFlag[2] = true;
						// ?????
						play_se( 217, 320, 240 ); 
					}
				}
			}
			break;

		case 3:
			if( mouse.onceState & MOUSE_LEFT_CRICK_UP && pushBtnFlag[3] == true ){
							
				pushBtnFlag[3] = false;
			}

			if(  haveGold > 0  && (familyTax + ( pc.gold - haveGold )) < MAX_FMBANKGOLD ) {
							
				if( pushBtnFlag[3] == true ){

					haveGold -= taxGoldDec;
					//bankGold -= bankGoldInc;
					taxGoldCnt++;

					// ???????
					if( taxGoldCnt >= 30 ){ 
						// ?????????????????
						taxGoldCnt = 0;
						// ????????
						if( taxGoldDec == 0 ) {
							taxGoldDec = 1;
						}else{
							// ???????
							taxGoldDec *= 5;
							// ????????
							if( taxGoldDec > 10000 ) {
								taxGoldDec = 10000;
							}
						}
					}
					// ????????
					if( haveGold < 0 ) {
						haveGold = 0;
						play_se( 220, 320, 240 );
					}
					if( (familyTax + ( pc.gold - haveGold )) > MAX_FMBANKGOLD ) {
						haveGold = familyTax + pc.gold - MAX_FMBANKGOLD;

						play_se( 220, 320, 240 );
					}
								
				}

				// ????????
				if( mouse.onceState & MOUSE_LEFT_CRICK ) {
					// ???
					haveGold--;
					// ????????
					if( haveGold < 0 ) { 
						haveGold = 0;
						// ???
						play_se( 220, 320, 240 );
					} else {
						// ?????????
						taxGoldDec = 0;		
						// ?????????????????
						taxGoldCnt = 0;		
						// ???????
						pushBtnFlag[3] = true;
						// ?????
						play_se( 217, 320, 240 ); 
					}
				}
			}
			break;

		}


#endif

		for( int i=0 ; i<4 ; i++ )
		{
			if( mouse.state & MOUSE_LEFT_CRICK && pushBtnFlag[i] == true )
				pushBtnFlag[i] = true;
			else	pushBtnFlag[i] = false;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			sprintf( dataBuf, "%s  家族的账户", pc.familyName );
			StockFontBuffer( winX+80, winY+30,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );
#ifdef _READ16BITBMP
			if(!g_bUseAlpha) StockDispHLine( winX+64, winY+50, 3 );
#endif
			StockFontBuffer( winX+60, winY+70,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, "存  款", 0 );

			familyTaxChange = familyTax + ( pc.gold - haveGold );
			sprintf( dataBuf, "%10d", familyTaxChange );
			StockFontBuffer( winX+200, winY+70,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			StockFontBuffer( winX+60, winY+120,
					FONT_PRIO_FRONT, FONT_PAL_WHITE, "现  金", 0 );
			sprintf( dataBuf, "%10d", haveGold );
			StockFontBuffer( winX+200, winY+120,
					FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			btnId[3] = StockDispBuffer( winX+190+40, winY+105, DISP_PRIO_IME3,
					CG_UP_BTN + pushBtnFlag[3], 2 );

#ifdef _FMVER21
			/*if( pc.familyleader == FMMEMBER_LEADER || 
				pc.familyleader == FMMEMBER_VICELEADER ||
				pc.familyleader == FMMEMBER_BAILEE) {*/
			if( pc.familyleader == FMMEMBER_LEADER || 
				pc.familyleader == FMMEMBER_ELDER) {
#else
			if( pc.familyleader == 1 ) {
#endif
			
				btnId[2] = StockDispBuffer( winX+190, winY+105, DISP_PRIO_IME3,
									CG_DOWN_BTN + pushBtnFlag[2], 2 );

				btnId[0] = StockDispBuffer( winX+100, winY+160, DISP_PRIO_IME3,
									CG_TRADE_OK_BTN, 2 );
				btnId[1] = StockDispBuffer( winX+100+120, winY+160, DISP_PRIO_IME3,
									CG_TRADE_CANCEL_BTN, 2 );
			}
			else
			{
				btnId[0] = StockDispBuffer( winX+100, winY+160, DISP_PRIO_IME3,
									CG_TRADE_OK_BTN, 2 );
				btnId[1] = StockDispBuffer( winX+100+120, winY+160, DISP_PRIO_IME3,
									CG_TRADE_CANCEL_BTN, 2 );
				//btnId[1] = StockDispBuffer( winX+160, winY+160, DISP_PRIO_IME3,
				//					CG_TRADE_OK_BTN, 2 );
				//StockFontBuffer( winX+60, winY+120,
				//		FONT_PRIO_FRONT, FONT_PAL_WHITE, "????峤???????", 0 );
			}
			

		}
	}


}

int rideablePet[3];
int showNum;

void initShowRidePetWN( void )
{
	//int baseimageNo = pc.graNo - (pc.graNo % 5);
	int leaderimageNo = 100700 + ((pc.baseGraNo-100000)/20)*10 + (pc.familySprite)*5;
	int i =0, j;

	rideablePet[0] = 0;
	rideablePet[1] = 0;
	rideablePet[2] = 0;


	/*
	if( pc.ridePetNo != -1 )
	{
		for( j=0 ; j< sizeof(ridePetTable)/sizeof(tagRidePetTable) ; j++ )
		{
			if( ridePetTable[j].rideNo == pc.graNo )
			{
				baseimageNo = ridePetTable[j].charNo;
				break;
			}
		}

	}else
		baseimageNo = pc.graNo - (pc.graNo % 5);
	*/

	for( j=0 ; j< sizeof(ridePetTable)/sizeof(tagRidePetTable) ; j++ )
	{
#ifdef _LEADERRIDE
		if( ridePetTable[j].charNo == pc.baseGraNo ||
			( ridePetTable[j].charNo == leaderimageNo && pc.big4fm != 0 && pc.familyleader != FMMEMBER_APPLY && pc.familyleader != FMMEMBER_NONE ) )
#else
		if( ridePetTable[j].charNo == pc.baseGraNo )
#endif
		{
			rideablePet[i] = ridePetTable[j].petNo;
			i++;
			showNum = i;
			if( i >= 3 ) break;
		}
	}
}

static ACTION *pActPet12;
static ACTION *pActPet13;
static ACTION *pActPet14;

void showRidePetWN( void )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2 };

	static int btnLoc[6][2];
	static int btnCnt;
	//static int msgLine;
	//static STR_BUFFER input;
	char dataBuf[256];
	char buf[256];

	//int	dataStatus;
	/*
	char *dataStatusStr[] =
	{
		"δ???",
		" ??? ",
		" ???? ",
		"?趨??",
		"??????",
		"?????"
	};
	*/

	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};
	int id ;
	int i, j;
	int mask;
	int btn;


	static int totalMsgLine;
	static int lineSkip;


	if( ptActMenuWin == NULL )
	{
		//winW = msgWN_W;
		//winH = msgWN_H;
		winW = 7;
		winH = 6;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		//lineSkip = 20;
		//totalMsgLine = (winH*48-56)/lineSkip;

		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 6; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}

		// ???????????????????
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;
				btnLoc[i][1] = winH*48-35;
			}
		}

		play_se( 202, 320, 240 );	// ????????
	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		// ?\?????????????????????????
		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			DeathAction( pActPet12 );
			pActPet12 = NULL;
			DeathAction( pActPet13 );
			pActPet13 = NULL;
			DeathAction( pActPet14 );
			pActPet14 = NULL;
		}

		// ???
		if( 0 <= id && id < 6 )
		{
			//char msg[256];
			btn = 1;
			btn <<= id;

			//makeEscapeString( input.buffer, msg, sizeof( msg )-1 );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;

			windowTypeWN = -1;

		}
		else if( (6 <= id) && (id < 6+8) )
		{

			
			int dataSel = id -6;
			sprintf( dataBuf, "%d|%s", FMPKSelectData[dataSel].fmindex, 
						makeEscapeString(FMPKSelectData[dataSel].fmname, buf, sizeof(buf)) );
			if( bNewServer)
				lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, dataBuf ) ;
			else
				old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN,
								WINDOW_BUTTONTYPE_OK, dataBuf ) ;
		}

		
		if( id >= 0 )
		{
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			DeathAction( pActPet12 );
			pActPet12 = NULL;
			DeathAction( pActPet13 );
			pActPet13 = NULL;
			DeathAction( pActPet14 );
			pActPet14 = NULL;
			return;
		}
		

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{

			
			j = 0;
			
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				fontId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						fontId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}

			sprintf( dataBuf, "%s", "适合你骑乘的宠物种类如下：" );
			StockFontBuffer( winX+30, winY+40,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			if( pActPet12 == NULL )
				if( rideablePet[0] != 0 )
					pActPet12 = MakeAnimDisp( winX+115, winY+180, rideablePet[0], 0 );

			if( pActPet13 == NULL )
				if( rideablePet[1] != 0 )
					pActPet13 = MakeAnimDisp( winX+335, winY+180, rideablePet[1], 0 );

			if( pActPet14 == NULL )
				if( rideablePet[2] != 0 )
					pActPet14 = MakeAnimDisp( winX+225, winY+180, rideablePet[2], 0 );

			sprintf( dataBuf, "%s", "赶快去把她们找出来吧！" );
			StockFontBuffer( winX+30, winY+220,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

		}
	}

}

char memberName[100][36];
int	memberIndex[100];
int memberNum;
//int pagenum;
int changeWho;

void initFamilyLeaderChange( char* data )
{

	int i;
	char buf[256], buf2[256];
	
	pagenum = 1;
	changeWho = -1;
	getStringToken( data, '|', 4, sizeof( buf )-1, buf );
	memberNum = atoi(buf);

	for( i=0; i<memberNum; i++ )
	{
		getStringToken( data, '|', 5+i*2, sizeof( buf2 )-1, buf2 );
		memberIndex[i] = atoi(buf2);
		getStringToken( data, '|', 5+i*2+1, sizeof( buf2 )-1, buf2 );
		strcpy( memberName[i], makeStringFromEscaped(buf2) );

	}
	
}

void closeFamilyLeaderChangeWN()
{
			windowTypeWN = -1;
			//wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			changeWho = -1;
}

void familyLeaderChangeWN()
{
	
	static int winX, winY;
	static int winW, winH;
	static int fontId[30];
	static int fontId2[3];
	int selId, selId2;
	//char buf[256];
	int i;


	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
		for( i = 0; i < sizeof( fontId2 )/sizeof( int ); i++ )
			fontId2[i] = -2;
		
		play_se( 202, 320, 240 );
	}


	if( ptActMenuWin != NULL )
	{
		selId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
		selId2 = selFontId( fontId2, sizeof( fontId2 )/sizeof( int ) );
		//selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			//id = 100;
			closeFamilyLeaderChangeWN();
		}

		if( mouse.onceState & MOUSE_LEFT_CRICK )
			if( selId >= 0 && selId < 30 ) {
				closeFamilyLeaderChangeWN();
				changeWho = (pagenum-1)*30 + selId;
				windowTypeWN = WINDOW_MESSAGETYPE_LEADERSELECTQ;
				return;
			}

		switch( selId2 ) {
		case 0:
			closeFamilyLeaderChangeWN();
			return;
		case 1:
			pagenum --;
			return;
		case 2:
			pagenum ++;
			return;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+50, winY+30, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				"请选择家族成员", 0 );

			fontId2[0] = StockFontBuffer( winX+300, winY+350, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  离   开 " , 2 );

			switch( pagenum )
			{
			case 1:
				for( i=0; i< memberNum ; i++)
					fontId[i] =	StockFontBuffer( winX+30+(i/10)*50, winY+60+(i%10)*20,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, memberName[i], 2 );
				if( memberNum > 30 )
					fontId2[2] = StockFontBuffer( winX+100, winY+350, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
						"下一页", 2 );
				break;
			case 2:
				for( i=30; i<memberNum; i++)
					fontId[i] =	StockFontBuffer( winX+30+((i-30)/10)*50, winY+60+(i%10)*20,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, memberName[i], 2 );
				fontId2[1] = StockFontBuffer( winX+30, winY+350, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
					"上一页", 2 );
				if( memberNum > 60 )
					fontId2[2] = StockFontBuffer( winX+100, winY+350, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
						"下一页", 2 );
				break;
			case 3:
				for( i=60; i<memberNum; i++)
					fontId[i] =	StockFontBuffer( winX+30+((i-60)/10)*50, winY+60+(i%10)*20,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, memberName[i], 2 );
				fontId2[1] = StockFontBuffer( winX+30, winY+350, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
					"上一页", 2 );
				break;
			}

			

		}


	}


}


void familyLeaderChangeQWN()
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[2];
	int selId;
	char buf[256];
	int i;

	if( ptActMenuWin == NULL )
	{
		winW = 6;
		winH = 2;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
		
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{
		selId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			//id = 100;
			closeFamilyLeaderChangeWN();
		}

		if( mouse.onceState & MOUSE_LEFT_CRICK )

		switch( selId ) {
		case 0:
			closeFamilyLeaderChangeWN();
			return;
		case 1:
			sprintf( buf, "L|CHANGE|Q|%d|%s", memberIndex[changeWho], memberName[changeWho] );
			if( bNewServer)
				lssproto_FM_send( sockfd, buf );
			else
				old_lssproto_FM_send( sockfd, buf );
			closeFamilyLeaderChangeWN();
			return;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			sprintf( buf, "你确定要将族长的位子交给%s吗？", memberName[changeWho] );
			StockFontBuffer( winX+50, winY+30, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				buf, 0 );

			fontId[0] = StockFontBuffer( winX+200, winY+60, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  离   开  " , 2 );
			fontId[1] = StockFontBuffer( winX+280, winY+60, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  确   定  " , 2 );

		}

	}

}

char oldLeaderName[36];
int	oldLeaderIndex;

void initFamilyLeaderChangeA( char *data )
{
	char buf[64];

	//windowTypeWN = -1;
	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;

	getStringToken( data, '|', 1, sizeof( buf )-1, buf );
	strcpy( oldLeaderName, makeStringFromEscaped(buf) );
	getStringToken( data, '|', 2, sizeof( buf )-1, buf );
	oldLeaderIndex = atoi(buf);
}

void familyLeaderChangeAWN()
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[2];
	int selId;
	char buf[256], buf2[64];
	int i;

	if( ptActMenuWin == NULL )
	{
		winW = 6;
		winH = 3;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
		
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{
		selId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			//id = 100;
			closeFamilyLeaderChangeWN();
		}

		if( mouse.onceState & MOUSE_LEFT_CRICK )

		switch( selId ) {
		case 0:
			sprintf( buf, "L|CHANGE|A|0|%s|%d",
				makeEscapeString( oldLeaderName, buf2, sizeof(buf2)),
				oldLeaderIndex );
			if( bNewServer)
				lssproto_FM_send( sockfd, buf );
			else
				old_lssproto_FM_send( sockfd, buf );
			closeFamilyLeaderChangeWN();
			return;
		case 1:
			sprintf( buf, "L|CHANGE|A|1|%s|%d",
				makeEscapeString( oldLeaderName, buf2, sizeof(buf2)),
				oldLeaderIndex );
			if( bNewServer)
				lssproto_FM_send( sockfd, buf );
			else
				old_lssproto_FM_send( sockfd, buf );
			closeFamilyLeaderChangeWN();
			return;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			sprintf( buf, "%s 希望将族长的位子交给你，", oldLeaderName );
			StockFontBuffer( winX+50, winY+30, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				buf, 0 );
			StockFontBuffer( winX+50, winY+60, FONT_PRIO_FRONT, FONT_PAL_WHITE,
				"你要接受吗？", 0 );

			fontId[0] = StockFontBuffer( winX+200, winY+105, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  不愿意  " , 2 );
			fontId[1] = StockFontBuffer( winX+280, winY+105, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  愿  意  " , 2 );

		}

	}

}


struct {
	char	name[64];
	int		member;
	char	leadername[64];
	int		rank;
	int		fmdp;
	int		personaldp;
	int		job;
	int		sprite;
	char	pktime[32];
	int		pkaddress;
	char	pkopp[64];
	char	pkdata[256];
#ifdef _NEW_MANOR_LAW
	int		fmMomentum;
	int		momentum;
#endif
} mineFamily;

void initMineFamilyDetail( char* data )
{

	char ibuf[64];

	DeathAction( ptActMenuWin );
	ptActMenuWin = NULL;
	
	makeStringFromEscaped( data );

	getStringToken( data, '|', 1, sizeof( mineFamily.name )-1, mineFamily.name );
	makeStringFromEscaped( mineFamily.name );
	getStringToken( data, '|', 2, sizeof( ibuf )-1, ibuf );
	mineFamily.member = atoi( ibuf );
	getStringToken( data, '|', 3, sizeof( mineFamily.leadername )-1, mineFamily.leadername );
	makeStringFromEscaped( mineFamily.leadername );
	getStringToken( data, '|', 4, sizeof( ibuf )-1, ibuf );
	mineFamily.rank = atoi( ibuf );
	getStringToken( data, '|', 5, sizeof( ibuf )-1, ibuf );
	mineFamily.fmdp = atoi( ibuf );
	getStringToken( data, '|', 6, sizeof( ibuf )-1, ibuf );
	mineFamily.personaldp = atoi( ibuf );
	getStringToken( data, '|', 7, sizeof( ibuf )-1, ibuf );
	mineFamily.job = atoi( ibuf );
	if( mineFamily.job < 0 )	mineFamily.job = 0;
	getStringToken( data, '|', 8, sizeof( ibuf )-1, ibuf );
	mineFamily.sprite = atoi( ibuf );
	/*
	getStringToken( data, '|', 9, sizeof( buf )-1, buf );
	makeStringFromEscaped( buf );
		getStringToken( buf, ' ', 1, sizeof( mineFamily.pktime )-1, mineFamily.pktime );
		makeStringFromEscaped( mineFamily.pktime );
		getStringToken( buf, ' ', 2, sizeof( ibuf )-1, ibuf );
		mineFamily.pkaddress = atoi( ibuf );
		getStringToken( buf, ' ', 3, sizeof( mineFamily.pkopp )-1, mineFamily.pkopp );
		makeStringFromEscaped( mineFamily.pkopp );
	*/
	getStringToken( data, '|', 9, sizeof( mineFamily.pkdata )-1, mineFamily.pkdata );
	makeStringFromEscaped( mineFamily.pkdata );
#ifdef _NEW_MANOR_LAW
	getStringToken(data,'|',10,sizeof(ibuf) - 1,ibuf);
	mineFamily.fmMomentum = atoi(ibuf);
	getStringToken(data,'|',11,sizeof(ibuf) - 1,ibuf);
	mineFamily.momentum = atoi(ibuf);
#endif
}

void closeMineFamilyDetailWN()
{
			windowTypeWN = -1;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
}

char	*familyJobName[] =
{
	"为加入家族",
    "一般成员",
	"待 审 核",
    " 族   长 ",
    " 长   老 ",	
//    " ??  ?? ",
//    " ??  ? ",
//    "?? ?? ??",
};

char *familyTownName[] =
{
	"",
	"萨姆吉尔庄园",
	"玛丽娜丝庄园",
	"加加庄园",
	"卡鲁它那庄园"
};


void mineFamilyDetailWN()
{
	
	static int winX, winY;
	static int winW, winH;
	static int fontId[2];
	int selId;
	char buf[256];
	//char timebuf[14];
	int i;

	if( ptActMenuWin == NULL )
	{
		winW = 7;
		winH = 6;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
			fontId[i] = -2;
		
		play_se( 202, 320, 240 );
	}

	if( ptActMenuWin != NULL )
	{
		selId = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			closeMineFamilyDetailWN();
		}

		if( mouse.onceState & MOUSE_LEFT_CRICK )

		switch( selId ) {
		case 0:
			closeFamilyLeaderChangeWN();
			return;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{

			StockFontBuffer( winX+190, winY+20, FONT_PRIO_FRONT, FONT_PAL_WHITE, "家族资料", 0 );
#ifdef _READ16BITBMP
			if(!g_bUseAlpha) StockDispHLine( winX+160, winY+40, 2 );
#endif

			sprintf( buf, "家族名称：%s", mineFamily.name );
			StockFontBuffer( winX+30, winY+60, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			sprintf( buf, "族长名称：%s", mineFamily.leadername );
			StockFontBuffer( winX+240, winY+60, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "家族人数：%d", mineFamily.member );
			StockFontBuffer( winX+30, winY+90, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			sprintf( buf, "守护精灵：%s", familySpriteName[mineFamily.sprite] );
			StockFontBuffer( winX+240, winY+90, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
#ifdef _NEW_MANOR_LAW
			sprintf( buf, "声望排名：%d", mineFamily.rank );
#else
			sprintf( buf, "家族排名：%d", mineFamily.rank );
#endif
			StockFontBuffer( winX+30, winY+120, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			sprintf( buf, "家族声望：%d", mineFamily.fmdp );
			StockFontBuffer( winX+240, winY+120, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "家族职位：%s", familyJobName[mineFamily.job] );
			StockFontBuffer( winX+30, winY+150, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			sprintf( buf, "个人声望：%d", mineFamily.personaldp );
			StockFontBuffer( winX+240, winY+150, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

#ifdef _NEW_MANOR_LAW
			sprintf( buf, "家族气势：%d", mineFamily.fmMomentum );
			StockFontBuffer( winX+240, winY+180, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			sprintf( buf, "个人气势：%d", mineFamily.momentum );
			StockFontBuffer( winX+240, winY+210, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
#endif
			StockFontBuffer( winX+30, winY+180, FONT_PRIO_FRONT, FONT_PAL_WHITE, "????????", 0 );
#ifdef _NEW_MANOR_LAW
			StockFontBuffer( winX+45, winY+230, FONT_PRIO_FRONT, FONT_PAL_WHITE, mineFamily.pkdata , 0 );
#else
			StockFontBuffer( winX+45, winY+210, FONT_PRIO_FRONT, FONT_PAL_WHITE, mineFamily.pkdata , 0 );
#endif
			
			/*
			if( atoi( mineFamily.pktime ) >= 10000 ) {
				sprintf( timebuf, "???? %d:00", ( atoi( mineFamily.pktime )-10000)/100 );
			}
			else {
				sprintf( timebuf, "???? %d:00", atoi( mineFamily.pktime )/100 );
			}
			sprintf( buf, "%s  %s  %s", timebuf, familyTownName[mineFamily.pkaddress], mineFamily.pkopp );
			StockFontBuffer( winX+50, winY+240, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
			*/

			fontId[0] = StockFontBuffer( winX+180, winY+250, FONT_PRIO_FRONT, FONT_PAL_YELLOW,
				"  离    开  " , 2 );
		}

	}

}

//Terry add 2001/12/06
#ifdef __EDEN_EFFECT
ACTION *pBankProcWnd = NULL;

void BankProc(void)
{
	static int BankBtNo[20]; 		// ???????
	static int BankBtFlag[20];	// ???????
	static int Gold = 0;									// ????????????
	static int BankGoldInc = 0;						// ????????ε??
	static int BankGoldCnt = 0;						// ???????
	int x,y;
	char msg[256];
		
	if( CheckMenuFlag()
		|| (joy_trg[0] & JOY_ESC)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == true
		|| wnCloseFlag == 1)
	{
		if(pBankProcWnd != NULL)
		{
			windowTypeWN = -1;
			DeathAction(pBankProcWnd);
			pBankProcWnd = NULL;
			play_se(217,320,240);
			return;
		}
	}
		
	// ??????????????н???
	if(pBankProcWnd == NULL)
	{
		// ??????н???
#ifdef _READ16BITBMP
		if(g_bUseAlpha) pBankProcWnd = MakeWindowDisp(185,160,270,160,OLD_GRAPHICS_START+46,-1);  //x=(640-w)/2 y=(480-h)/2 w=270 h=160
		else
#endif
		pBankProcWnd = MakeWindowDisp(185,160,270,160,26287,-1);  //x=(640-w)/2 y=(480-h)/2 w=270 h=160
		Gold = 0;
		// ????Ч
		play_se(202,320,240);
	}
	else
	{
		// ???????????????
		if(pBankProcWnd->hp > 0)
		{
			if(mouse.onceState & MOUSE_LEFT_CRICK)
			{
				// ?????????????,??????
				if(HitDispNo == BankBtNo[0])
				{
					memset(msg,0,sizeof(msg));
					if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,msg);
					else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,0,msg);
					windowTypeWN = -1;
					DeathAction(pBankProcWnd);
					pBankProcWnd = NULL;
					play_se(217,320,240);
					return;
				}
				// ?????????????,????????????????gs
				if(HitDispNo == BankBtNo[1])
				{
					sprintf(msg,"%d",Gold);
					pc.gold -= Gold;
					pc.personal_bankgold += Gold;
					if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,4,msg);
					else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,4,msg);

					windowTypeWN = -1;
					DeathAction(pBankProcWnd);
					pBankProcWnd = NULL;
					play_se(217,320,240);
					return;
				}
			}
			
			// ?????
			if(HitDispNo == BankBtNo[3])
			{
				// ?????????????(??????????),?趨??????????
				if(mouse.onceState & MOUSE_LEFT_CRICK_UP && BankBtFlag[3] == true) BankBtFlag[3] = false;
				
				// ????????????????????????????????????С?????????????С?????????
				if(Gold + pc.personal_bankgold <= MAX_PERSONAL_BANKGOLD && pc.gold - Gold > 0)
				{
					// ??????????true(???????????????)
					if(BankBtFlag[3] == true)
					{
						Gold += BankGoldInc;
						BankGoldCnt++;
						
						// ???BankGoldCnt????30,??????ε??????屶
						if(BankGoldCnt >= 30)
						{ 
							// ??count???0
							BankGoldCnt = 0;
							if(BankGoldInc == 0) BankGoldInc = 1;
							else
							{
								// ???????屶
								BankGoldInc *= 5;
								// ?????μ?10000
								if(BankGoldInc > 10000) BankGoldInc = 10000;		
							}
						}
						// ????????????????????
						if(Gold + pc.personal_bankgold >= MAX_PERSONAL_BANKGOLD)
						{
							Gold = MAX_PERSONAL_BANKGOLD - pc.personal_bankgold;
							play_se(220,320,240);
						}
						// ??????????????С??0
						if(pc.gold - Gold < 0)
						{
							Gold = pc.gold;
							play_se(220,320,240);
						}
					}
					
					// ????????
					if(mouse.onceState & MOUSE_LEFT_CRICK)
					{
						// ??????????
						Gold++;
						// ????????????????????????????????????????
						if(Gold + pc.personal_bankgold >= MAX_PERSONAL_BANKGOLD)
						{ 
							Gold = MAX_PERSONAL_BANKGOLD - pc.personal_bankgold;
							// ????Ч
							play_se(220,320,240);
						}
						// ??????????????С??0
						if(pc.gold - Gold < 0)
						{
							Gold = pc.gold;
							play_se(220,320,240);
						}
						else
						{
							// ????????????0
							BankGoldInc = 0;		
							// ???????0
							BankGoldCnt = 0;		
							// ?趨?????????
							BankBtFlag[3] = true;
							// ????Ч
							play_se(217,320,240);
						}
					}
				}
				else
				{
					BankGoldInc = 0;
					BankGoldCnt = 0;
				}
			}
			else
			{
				// ?趨???????????
				BankBtFlag[3] = false;
			}
			
			// ??????
			// ????????,????Gold?????
			if(HitDispNo == BankBtNo[2])
			{
				// ?????????????(??????????),?趨??????????
				if(mouse.onceState & MOUSE_LEFT_CRICK_UP && BankBtFlag[2] == true) BankBtFlag[2] = false;
				// ??????????????????????????????????????????????????????????????????????
				if(pc.personal_bankgold + Gold > 0 && pc.gold - Gold <= CHAR_getMaxHaveGold())
				{
					// ????????????
					if(BankBtFlag[2] == true)
					{
						// ??????????
						Gold -= BankGoldInc;
						BankGoldCnt++;
						
						// ???BankGoldCnt????30,??????ε??????屶
						if(BankGoldCnt >= 30)
						{
							// ??count???0
							BankGoldCnt = 0;	
							if(BankGoldInc == 0) BankGoldInc = 1;
							else
							{
								// ???????屶
								BankGoldInc *= 5;
								// ?????μ?10000
								if(BankGoldInc > 10000) BankGoldInc = 10000;
							}
						}
						// ?????????????????????????????
						if(pc.personal_bankgold + Gold < 0)
						{
							Gold = pc.personal_bankgold * (-1);
							play_se(220,320,240);
						}
						// ????????????????????????????????????????
						if(pc.gold - Gold >= CHAR_getMaxHaveGold())
						{
							Gold = (CHAR_getMaxHaveGold() - pc.gold) * (-1);
							play_se(220,320,240);
						}
					}
					// ????????
					if(mouse.onceState & MOUSE_LEFT_CRICK)
					{
						Gold--;
						
						// ?????????????????????????????
						if(pc.personal_bankgold + Gold < 0)
						{
							Gold = pc.personal_bankgold * (-1);
							play_se(220,320,240);
						}
						else 
						// ????????????????????????????????????????
						if(pc.gold - Gold >= CHAR_getMaxHaveGold())
						{
							Gold = (CHAR_getMaxHaveGold() - pc.gold) * (-1);
							play_se(220,320,240);
						}
						else
						{
							// ????????????0
							BankGoldInc = 0;		
							// ???????0
							BankGoldCnt = 0;		
							// ?趨?????????
							BankBtFlag[2] = true;
							play_se(217,320,240); 
						}
					}
				}
				else
				{
					BankGoldInc = 0;
					BankGoldCnt = 0;
				}
			}
			else
			{
				// ?趨???????????
				BankBtFlag[2] = false;
			}
			
			if(pBankProcWnd != NULL)
			{
				// ?????趨
				x = pBankProcWnd->x ;
				y = pBankProcWnd->y ;
			
				BankBtNo[0] = StockDispBuffer(x+200,y+133,DISP_PRIO_IME3,CG_TRADE_CANCEL_BTN,2);
				BankBtNo[1] = StockDispBuffer(x+ 75,y+133,DISP_PRIO_IME3,CG_TRADE_OK_BTN,2);
				BankBtNo[2] = StockDispBuffer(x+140,y+ 70,DISP_PRIO_IME3,CG_UP_BTN + BankBtFlag[2],2);
				BankBtNo[3] = StockDispBuffer(x+200,y+ 70,DISP_PRIO_IME3,CG_DOWN_BTN + BankBtFlag[3],2);
				
				sprintf(msg,"%7d",pc.gold - Gold);
				StockFontBuffer(x+180,y+40,FONT_PRIO_FRONT,0,msg,0);
				sprintf(msg,"%7d",pc.personal_bankgold + Gold);
				StockFontBuffer(x+180,y+86,FONT_PRIO_FRONT,0,msg,0 );
#ifdef _READ16BITBMP
				if(g_bUseAlpha) StockDispBuffer(320,240,DISP_PRIO_IME3,OLD_GRAPHICS_START+46,1);
				else
#endif
				StockDispBuffer(320,240,DISP_PRIO_IME3,26287,1);
			}
		}
	}
}
#endif
//Terry end

// Terry add 2002/01/03
#ifdef __EDEN_AUCTION
#define AUCTION_NEW     		26266 //??????????
#define AUCTION_OK_U				26267 //ok???(δ????)
#define AUCTION_OK_D				26268 //ok???(????)
#define AUCTION_PLANK				26269 //?????????????
#define AUCTION_UP_U				26188 //???????(δ????)
#define AUCTION_UP_D				26189 //???????(????)
#define AUCTION_DOWN_U			26190 //???????(δ????)
#define AUCTION_DOWN_D			26191 //???????(????)
#define AUCTION_LIST    		26270 //?????б?(????)????
#define AUCTION_LIST_1      26271 //?????б?(???)????
#define AUCTION_GET_BACK_U	26272 //?????(δ????)
#define AUCTION_GET_BACK_D	26273 //?????(????)
#define AUCTION_GET_BACK_N	26274 //?????(??????)
#define AUCTION_SEE_DATA_U	26275 //???????(δ????)
#define AUCTION_SEE_DATA_D	26276 //???????(????)
#define AUCTION_SEE_DATA_N	26277 //???????(??????)
#define AUCTION_PAGE_UP_U   26278 //????(δ????)
#define AUCTION_PAGE_UP_D   26279 //????(????)
#define AUCTION_PAGE_UP_N   26280 //????(??????)
#define AUCTION_PAGE_DOWN_U 26281 //????(δ????)
#define AUCTION_PAGE_DOWN_D 26282 //????(????)
#define AUCTION_PAGE_DOWN_N 26283 //????(??????)
#define AUCTION_CANCEL_Y_U	26284 //???(δ????)(?????)
#define AUCTION_CANCEL_Y_D	26285 //???(????)(?????)
#define AUCTION_CANCEL_Y_N	26286 //???(??????)(?????)
#define AUCTION_DECIDE_U		26289 //???(δ????)
#define AUCTION_DECIDE_D		26288 //???(????)
#define AUCTION_CANCEL_R_U	26291 //???(δ????)(?????)
#define AUCTION_CANCEL_R_D	26290 //???(????)(?????)
#define AUCTION_SURVEY			26292 //???(????)????
#define AUCTION_SURVEY_1		26293 //???(???)????
#define AUCTION_PET_STAT    26297 //???????????????

STR_BUFFER AuctionStr;		// ??????????,50????
STR_BUFFER AuctionMoney;	// ??????????????
STR_BUFFER AuctionItem;	  // ????????
ACTION *pActAuctionWT = NULL;

void AuctionNewWT(void)
{
	static ACTION *pShowPet1 = NULL,*pShowPet2 = NULL;
	static int nButton1[3][2]; // nBotton1[x][y]-> x:0->???? x:1->ok x:2->????  y:0->?????????????
														 //                                                   y:1->????? 0->???????? 1->???????
	static int nButton2[2][2]; // nBotton2[x][y]-> x:0->??? x:1->??? y:0->?????????????
                             //                                      y:1->????? 0->???????? 1->???????
	static int nItem[MAX_ITEM-MAX_ITEMSTART][2];	 // nItem[x][y]-> x:->????????(??????????????0~14) y:0->?????????????
																		 //                                                    y:1->????? 0->???????? 1->??????? 
	static int nPet,nPetShow;	 // nPet:????б?????????? nPetShow:???????
	static int nItemSelect;		 // ????????????????
	static int nAuctionKind;	 // ???????????????? 1:Pet 2:Item
	static bool bPress;
	int x,y,i,j,k;
	char temp[8];
	char szSendMsg[3][260];			 // szSendMsg[x][256]-> x:0->???????? x:1->???server???????

	if( CheckMenuFlag()
		|| (joy_trg[0] & JOY_ESC)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == true
		|| wnCloseFlag == 1)
	{
		if(pActAuctionWT != NULL)
		{
			windowTypeWN = -1;
			DeathAction(pActAuctionWT);
			pActAuctionWT = NULL;
			DeathAction(pShowPet1);
			pShowPet1 = NULL;
			DeathAction(pShowPet2);
			pShowPet2 = NULL;
			play_se(217,320,240);
			GetKeyInputFocus(&MyChatBuffer);
			return;
		}
	}
	// ???????в?????н???
	if(pActAuctionWT == NULL)
	{
		// ??????н???
		pActAuctionWT = MakeWindowDisp(50,15,540,450,AUCTION_NEW,-1);
		memset(szSendMsg,0,sizeof(szSendMsg));
		memset(nButton1,0,sizeof(nButton1));
		memset(nButton2,0,sizeof(nButton2));
		memset(nItem,0,sizeof(nItem));
		for(i=0;i<MAX_ITEM;i++) nItem[i][0] = -1;
		nPet = nPetShow = 0;
		nItemSelect = 0;
		pShowPet1 = pShowPet2 = NULL;
		bPress = true;
		// ????????????
		memset(AuctionStr.buffer,0,sizeof(AuctionStr.buffer));
		AuctionStr.cnt       = 0;
		AuctionStr.cursor    = 0;
		AuctionStr.color		 = 0;
		AuctionStr.len       = 100;
		AuctionStr.lineLen   = 24;
		AuctionStr.lineDist  = 20;
		AuctionStr.x				 = pActAuctionWT->x + 30;
		AuctionStr.y				 = pActAuctionWT->y + 260;
		AuctionStr.fontPrio  = FONT_PRIO_FRONT;
		// ???????????
		memset(AuctionMoney.buffer,0,sizeof(AuctionMoney.buffer));
		AuctionMoney.cnt       = 0;
		AuctionMoney.cursor    = 0;
		AuctionMoney.color		 = 0;
		AuctionMoney.len       = 7;
		AuctionMoney.lineLen   = 7;
		AuctionMoney.lineDist  = 0;
		AuctionMoney.x				 = pActAuctionWT->x + 95;
		AuctionMoney.y				 = pActAuctionWT->y +387;
		AuctionMoney.fontPrio  = FONT_PRIO_FRONT;
		// ?????????????
		memset(AuctionItem.buffer, 0, sizeof(AuctionItem.buffer));
		AuctionItem.cnt        = 0;
		AuctionItem.cursor     = 0;
		AuctionItem.color	  	 = 0;
		AuctionItem.len        = 100;
		AuctionItem.lineLen    = 28;
		AuctionItem.lineDist   = 20;
		AuctionItem.x				   = pActAuctionWT->x + 26;
		AuctionItem.y				   = pActAuctionWT->y +170;
		AuctionItem.fontPrio   = FONT_PRIO_FRONT;
		// ????Ч
		play_se(202,320,240);
	}
	else
	{
		// ???????????????
		if(pActAuctionWT->hp > 0)
		{
			// ?????趨
			x = pActAuctionWT->x;
			y = pActAuctionWT->y;

			// ??????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK)
			{
				for(i=0;i<3;i++)
				{
					if(HitDispNo == nButton1[i][0] && !nButton1[i][1])
					{
						nButton1[i][1] = 1;
						bPress = true;
					}
					if(i<2)
					{
						if(HitDispNo == nButton2[i][0] && !nButton2[i][1])
						{
							nButton2[i][1] = 1;
							bPress = true;
						}
					}
				}
			}
			// ?????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK_UP)
			{
				for(i=0;i<3;i++)
				{
					if(nButton1[i][1]) nButton1[i][1] = 0;
					if(i<2) if(nButton2[i][1]) nButton2[i][1] = 0;
				}
				bPress = false;
			}
			
			for(i=0;i<3;i++)
			{
				for(j=0;j<5;j++)
				{
					if(MakeHitBox(x+261+j*50-24,y+272+i*48-24,x+261+j*50+25,y+272+i*48+23,DISP_PRIO_IME3) == true)
					{
						// ??????????double-click
						if(mouse.onceState & MOUSE_LEFT_DBL_CRICK)
						{
							for(k=0;k<MAX_ITEM-MAX_ITEMSTART;k++) nItem[k][1] = 0;
							// ?????????
							nItem[i*5+j][1] = 1;
							// ???????????????
							nItemSelect = i*5+j+5;
							nAuctionKind = 2;
							DeathAction(pShowPet2);
							pShowPet2 = NULL;
							play_se(217,320,240); 
							break;
						}
					}
				}
			}

			// ?????????
			if(nButton2[0][1] || nButton2[1][1])
			{
				if(nButton2[0][1])
				{
					// ??????????
					if(nAuctionKind == 1)
					{
						CheckSpace(AuctionStr.buffer);
						// ?????????
						if(pet[nPetShow].useFlag == 1)
						{
							makeEscapeString(AuctionStr.buffer,szSendMsg[0],sizeof(AuctionStr.buffer));
							sprintf(szSendMsg[1],"1|%d|%s|%d",nPetShow,szSendMsg[0],atoi(AuctionMoney.buffer));
							makeEscapeString(szSendMsg[1],szSendMsg[2],sizeof(szSendMsg[1]));
							if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_YES,szSendMsg[2]);
							else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_YES,szSendMsg[2]);
						}
					}
					// ??????????
					if(nAuctionKind == 2)
					{
						// ?????????
						if(pc.item[nItemSelect].useFlag == 1)
						{
							makeEscapeString(AuctionStr.buffer,szSendMsg[0],sizeof(AuctionStr.buffer));
							sprintf(szSendMsg[1],"2|%d|%s|%d",nItemSelect,szSendMsg[0],atoi(AuctionMoney.buffer));
							makeEscapeString(szSendMsg[1],szSendMsg[2],sizeof(szSendMsg[1]));
							if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_YES,szSendMsg[2]);
							else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_YES,szSendMsg[2]);
						}
					}
				}
				windowTypeWN = -1;
				DeathAction(pActAuctionWT);
				pActAuctionWT = NULL;
				DeathAction(pShowPet1);
				pShowPet1 = NULL;
				DeathAction(pShowPet2);
				pShowPet2 = NULL;
				play_se(217,320,240);
				GetKeyInputFocus(&MyChatBuffer);
				return;
			}

			// ??????????
			if(nButton1[0][1] && bPress)
			{
        bPress = false;
				nPet = (nPet-1 < 0 ? 4:nPet-1);
				DeathAction(pShowPet1);
				pShowPet1 = NULL;
				// ?????
				for(i=0;i<MAX_PET;i++)
				{
					if(pet[nPet].useFlag == 1) 
					{
						pShowPet1 = MakeAnimDisp(x+235+70,y+55+120,pet[nPet].graNo,0);
						break;
					}
					else nPet = (nPet-1 < 0 ? 4:nPet-1);
				}
				play_se(217,320,240);
			}
			// ??????ok
			if(nButton1[1][1] && bPress) 
			{
				nAuctionKind = 1;
				bPress = false;
				nItem[nItemSelect-5][1] = 0;
				//if(nPetShow != nPet && pShowPet1 != NULL)
				if(pShowPet1 != NULL)
				{
					nPetShow = nPet;
					DeathAction(pShowPet2);
					pShowPet2 = NULL;
					// ?????????????
				  if(pet[nPetShow].useFlag == 1) pShowPet2 = MakeAnimDisp(x+15+70,y+55+120,pet[nPetShow].graNo,0);
				}
				play_se(217,320,240);
			}
			// ??????????
			if(nButton1[2][1] && bPress)
			{
				bPress = false;
				nPet = (nPet+1 >= 5 ? 0:nPet+1);
				DeathAction(pShowPet1);
				pShowPet1 = NULL;
				// ?????
				for(i=0;i<MAX_PET;i++)
				{
					if(pet[nPet].useFlag == 1)
					{
						pShowPet1 = MakeAnimDisp(x+235+70,y+55+120,pet[nPet].graNo,0);
						break;
					}
					else nPet = (nPet+1 >= 5 ? 0:nPet+1);
				}
				play_se(217,320,240);
			}

			// ??????????????
			if(MakeHitBox(AuctionStr.x,AuctionStr.y,AuctionStr.x+170,AuctionStr.y+95,DISP_PRIO_BOX2) == true)
			{
				GetKeyInputFocus(&AuctionStr);
			}

			// ????????????
			if(MakeHitBox(AuctionMoney.x,AuctionMoney.y,AuctionMoney.x+115,AuctionMoney.y+18,DISP_PRIO_BOX2) == true)
			{
				GetKeyInputFocus(&AuctionMoney);
			}

			if(pActAuctionWT != NULL)
			{
				// ??????趨button?????λ??
				nButton1[0][0] = StockDispBuffer(x+485,y+ 70,DISP_PRIO_IME3,AUCTION_UP_U      +nButton1[0][1],2);
				nButton1[1][0] = StockDispBuffer(x+484,y+100,DISP_PRIO_IME3,AUCTION_OK_U      +nButton1[1][1],2);
				nButton1[2][0] = StockDispBuffer(x+485,y+130,DISP_PRIO_IME3,AUCTION_DOWN_U    +nButton1[2][1],2);
				nButton2[0][0] = StockDispBuffer(x+312,y+422,DISP_PRIO_IME3,AUCTION_DECIDE_U  +nButton2[0][1],2);
				nButton2[1][0] = StockDispBuffer(x+432,y+422,DISP_PRIO_IME3,AUCTION_CANCEL_R_U,2);

				// ??????????????
				for(i=0;i<3;i++)
				{
					for(j=0;j<5;j++)
					{
						// ?????????е?????????????,???????????????????
						if(!nItem[i*5+j][1])
						{
							// ??????
							if(pc.item[i*5+j+5].useFlag == 1) nItem[i*5+j][0] = StockDispBuffer(x+262+j*50,y+273+i*48,DISP_PRIO_ITEM,pc.item[i*5+j+5].graNo,0);
						}
						// ???????????????????,????????????????????λ
						else 
						{
							// ??????
							StockDispBuffer(x+120,y+125,DISP_PRIO_IME3,pc.item[i*5+j+5].graNo,0);
							// ?????????
							StockDispBuffer(x+120,y+125,DISP_PRIO_IME3,AUCTION_PLANK,0);
							// ?????????
							StockFontBuffer(x+36,y+ 28,FONT_PRIO_FRONT,0,pc.item[i*5+j+5].name,0);
							// ?????????
							strncpy(AuctionItem.buffer,pc.item[i*5+j+5].memo,64);
							StockFontBuffer2(&AuctionItem);
						}
					}
				}
				// ??????????????????
				// ?????????????????????
				if(pet[nPet].useFlag == 1)
				{
					if(pShowPet1 != NULL)
					{
						// ?????????
						StockFontBuffer(x+256,y+28,FONT_PRIO_FRONT,0,pet[nPet].name,0);
						// ??????????
//						itoa(pet[nPet].level,temp,10);
                        sprintf(temp, "%d", pet[nPet].level);
						StockFontBuffer(x+400,y+58    ,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPet].atk,temp,10);
                        sprintf(temp, "%d", pet[nPet].atk);
						StockFontBuffer(x+400,y+58+ 28,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPet].def,temp,10);
                        sprintf(temp, "%d", pet[nPet].def);
						StockFontBuffer(x+400,y+58+ 56,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPet].quick,temp,10);
                        sprintf(temp, "%d", pet[nPet].quick);
						StockFontBuffer(x+400,y+58+ 84,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPet].maxHp,temp,10);
                        sprintf(temp, "%d", pet[nPet].maxHp);
						StockFontBuffer(x+400,y+58+112,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPet].ai,temp,10);
                        sprintf(temp, "%d", pet[nPet].ai);
						StockFontBuffer(x+400,y+58+140,FONT_PRIO_FRONT,0,temp,0);
					}
					// ????а???ok ????????????????
					if(nAuctionKind == 1 && pShowPet2 != NULL)
					{
						// ?????????
						StockFontBuffer(x+36,y+28,FONT_PRIO_FRONT,0,pet[nPetShow].name,0);
						// ???????????????
						StockDispBuffer(x+180,y+137,DISP_PRIO_IME3,AUCTION_PET_STAT,0);
						// ??????????
//						itoa(pet[nPetShow].level,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].level);
						StockFontBuffer(x+180,y+58    ,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPetShow].atk,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].atk);
						StockFontBuffer(x+180,y+58+ 28,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPetShow].def,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].def);
						StockFontBuffer(x+180,y+58+ 56,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPetShow].quick,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].quick);
						StockFontBuffer(x+180,y+58+ 84,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPetShow].maxHp,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].maxHp);
						StockFontBuffer(x+180,y+58+112,FONT_PRIO_FRONT,0,temp,0);
//						itoa(pet[nPetShow].ai,temp,10);
                        sprintf(temp, "%d", pet[nPetShow].ai);
						StockFontBuffer(x+180,y+58+140,FONT_PRIO_FRONT,0,temp,0);
					}
				}
				StockFontBuffer2(&AuctionStr);
				CheckNumber(AuctionMoney.buffer,-1);
				StockFontBuffer2(&AuctionMoney);
				StockDispBuffer(320,240,DISP_PRIO_IME3,AUCTION_NEW,1);
			}
		}
	}
}


void AuctionListWT(void)
{
	static ALD aldArea[10];		// server??δ?????????е????????
	static int nButton[5][2];	// nButton[x][y]-> x:0->??? x:1->??????? x:2->???? x:3->???? x:4->???  
                            //                 y:0->????????????? y:1->????? 0->???? 1->?е?? 2->??????
	static int nList[10][2];	// ?????????????????????? nList[x][y]-> x:0~9->??????????  
                            //															            y:0->????????????? y:1->????? 0->???? 1->?е??
	static int nListSelect;   // ?????????б?
	int x,y,i;
	char temp[16],szSendMsg[2][260];
	static char id[16];							// ????cd key
	static bool bPress;
	static bool bIsThisPlayerAuction; // ??????????????????????
	static int nListNum;			// ?б????
	static int nCurrentPage;	// ?????

	if( CheckMenuFlag()
		|| (joy_trg[0] & JOY_ESC)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == true
		|| wnCloseFlag == 1)
	{
		if(pActAuctionWT != NULL)
		{
			windowTypeWN = -1;
			DeathAction(pActAuctionWT);
			pActAuctionWT = NULL;
			bNewData = false;
			play_se(217,320,240);
			return;
		}
	}

	if(bNewData == true) 
	{
		memset(aldArea,0,sizeof(aldArea));
		AuctionGetString(0,aldArea);
		bIsThisPlayerAuction = false;
		bNewData = false;
		nButton[2][1] = 2;
		nButton[3][1] = 2;
		if(buttonTypeWN & WINDOW_BUTTONTYPE_PRE)  nButton[2][1] = 0;
		if(buttonTypeWN & WINDOW_BUTTONTYPE_NEXT) nButton[3][1] = 0;
	}

	// ???????в?????н???
	if(pActAuctionWT == NULL)
	{
		// ??????н???
		pActAuctionWT = MakeWindowDisp(50,15,540,450,AUCTION_LIST,-1);
		memset(nList,0,sizeof(nList));
		nButton[0][1] = 2; // ?????趨???????
		nButton[1][1] = 2;
		nButton[2][1] = 2;
		if(!(buttonTypeWN & WINDOW_BUTTONTYPE_NEXT)) nButton[3][1] = 2;
		else																				 nButton[3][1] = 0;
		nButton[4][1] = 0;
		nListSelect = -1;
		bPress = true;
		nListNum = 0;
		bIsThisPlayerAuction = false;
		extern char szUser[];
		memcpy(id,szUser,16);
		ecb_crypt("f;encor1c",id,16,DES_DECRYPT);
		// ????Ч
		play_se(202,320,240);
	}
	else
	{
		// ???????????????
		if(pActAuctionWT->hp > 0)
		{
			// ?????趨
			x = pActAuctionWT->x;
			y = pActAuctionWT->y;

			// ??????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK)
			{
				for(i=0;i<10;i++)
				{
					nList[i][1] = 0;
					if(HitFontNo == nList[i][0])
					{
						// ?????????????е???????????е???????
						if(!strcmp(aldArea[i].cdkey,id)) 
						{
							bIsThisPlayerAuction = true;
							nButton[0][1] = 0;
						}
						else bIsThisPlayerAuction = false;
						nListSelect = i;
						nList[i][1] = 1;
						nButton[1][1] = 0;
						bPress = true;
					}
					if(i<5)
					{
						if(HitDispNo == nButton[i][0] && nButton[i][1] == 0)
						{
							nButton[i][1] = 1;
							bPress = true;
						}
					}
				}
			}
			// ?????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK_UP)
			{
				if(!bIsThisPlayerAuction) nButton[0][1] = 2;
				else  									  nButton[0][1] = 0;
				nButton[1][1] = 2;
				for(i=0;i<10;i++) 
				{
					if(nList[i][1])
					{
						nButton[1][1] = 0;
						break;
					}
				}
			}
			// ????????????????  ??aldArea[nListSelect].index???server
			if(nButton[0][1] == 1 || nButton[1][1] == 1)
			{
				int type;

				if(nButton[0][1] == 1) type = WINDOW_BUTTONTYPE_OK;
				else                   type = WINDOW_BUTTONTYPE_YES;

				memset(szSendMsg,0,sizeof(szSendMsg));
				sprintf(szSendMsg[1],"%d|",aldArea[nListSelect].index);
				makeEscapeString(szSendMsg[1],szSendMsg[0],sizeof(szSendMsg[0]));
				if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,type,szSendMsg[0]);
				else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,type,szSendMsg[0]);

				windowTypeWN = -1;
				bNewData = false;
				DeathAction(pActAuctionWT);
				pActAuctionWT = NULL;
				play_se(217,320,240);
				return;
			}
			// ???????????????? ??aldArea[0].index???server
			if((nButton[2][1] == 1 || nButton[3][1] == 1) && bPress)
			{
				int type;

				if(nButton[2][1] == 1) 
				{
					type = WINDOW_BUTTONTYPE_PRE;
					nListNum -= 10;
					nCurrentPage = nListNum/10+1;
				}
				else				 					 
				{
					type = WINDOW_BUTTONTYPE_NEXT;
					nListNum += 10;
					nCurrentPage = nListNum/10+1;
				}
				memset(szSendMsg,0,sizeof(szSendMsg));
				sprintf(szSendMsg[1],"%d|",aldArea[0].index);
				makeEscapeString(szSendMsg[1],szSendMsg[0],sizeof(szSendMsg[0]));
				if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,type,szSendMsg[0]);
				else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,type,szSendMsg[0]);
				bPress = false;
			}
			// ??????????
			if(nButton[4][1])
			{
				StockDispBuffer(x+439,y+386,DISP_PRIO_IME3,AUCTION_CANCEL_Y_U+nButton[4][1],2);
				windowTypeWN = -1;
				bNewData = false;
				DeathAction(pActAuctionWT);
				pActAuctionWT = NULL;
				play_se(217,320,240);
				return;
			}

			// ??????趨button?????λ??
			nButton[0][0] = StockDispBuffer(x+102,y+386,DISP_PRIO_IME3,AUCTION_GET_BACK_U +nButton[0][1],nButton[0][1] == 2 ? 0:2);
			nButton[1][0] = StockDispBuffer(x+172,y+386,DISP_PRIO_IME3,AUCTION_SEE_DATA_U +nButton[1][1],nButton[1][1] == 2 ? 0:2);
			nButton[2][0] = StockDispBuffer(x+299,y+386,DISP_PRIO_IME3,AUCTION_PAGE_UP_U  +nButton[2][1],nButton[2][1] == 2 ? 0:2);
			nButton[3][0] = StockDispBuffer(x+369,y+386,DISP_PRIO_IME3,AUCTION_PAGE_DOWN_U+nButton[3][1],nButton[3][1] == 2 ? 0:2);
			nButton[4][0] = StockDispBuffer(x+439,y+386,DISP_PRIO_IME3,AUCTION_CANCEL_Y_U,2);
			// ????????
			sprintf(temp,"%d/9 页",nCurrentPage);
			StockFontBuffer(x+420,y+350,FONT_PRIO_FRONT,0,temp,0);
			for(i=0;i<10;i++)
			{
				if(strlen(aldArea[i].account) > 0)
				{
					// ??????
//                    itoa(nListNum+i,temp,10)
                    sprintf(temp, "%d", nListNum + i);
					StockFontBuffer(x+92,y+110+i*25,FONT_PRIO_FRONT,0, temp,0);
					// ???????
					// ????б?
					if(windowTypeWN == WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY)
					{
						// ???????????????,?????????;??????????
						if(!strcmp(aldArea[i].cdkey,id)) nList[i][0] = StockFontBuffer(x+132,y+110+i*25,FONT_PRIO_FRONT,nListSelect == i ? FONT_PAL_YELLOW:FONT_PAL_RED,aldArea[i].account,2);
						// ?????????????????????
						else StockFontBuffer(x+132,y+110+i*25,FONT_PRIO_FRONT,0,aldArea[i].account,0);
					}
					// ?????б?
					else nList[i][0] = StockFontBuffer(x+132,y+110+i*25,FONT_PRIO_FRONT,nListSelect == i ? FONT_PAL_YELLOW:0,aldArea[i].account,2);
				}
			}
			if(windowTypeWN == WINDOW_MESSAGETYPE_AUCTIONLIST_MODIFY) StockDispBuffer(244,72,DISP_PRIO_IME3,AUCTION_LIST_1,1);
			StockDispBuffer(320,240,DISP_PRIO_IME3,AUCTION_LIST,1);
		}
	}
}

void AuctionSurveyModifyWT(void)
{
	static ACTION *pShowPet = NULL;
	static ALD ald;	// ?????????????
	static int nButton[2][2]; // nButton[x][y]-> x:0->??? x:1->???
                            //                 y:0->????????????? y:1->????? 0->???? 1->?е??
	static bool bPress;
	int x,y,i;
	char szSendMsg[2][8];
	char szPetAttrib[8];

	if( CheckMenuFlag()
		|| (joy_trg[0] & JOY_ESC)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == true
		|| wnCloseFlag == 1)
	{
		if(pActAuctionWT != NULL)
		{
			windowTypeWN = -1;
			DeathAction(pActAuctionWT);
			pActAuctionWT = NULL;
			if(pShowPet != NULL)
			{
				DeathAction(pShowPet);
				pShowPet = NULL;
			}
			GetKeyInputFocus(&MyChatBuffer);
			play_se(217,320,240);
			return;
		}
	}
	// ???????в??????????
	if(pActAuctionWT == NULL)
	{
		// ???????????
		pActAuctionWT = MakeWindowDisp(50,15,540,450,AUCTION_SURVEY,-1);
		memset(&ald,0,sizeof(ald));
		memset(nButton,0,sizeof(nButton));
		AuctionGetString(1,&ald);
		bPress = false;
		// ????????????
		strcpy(AuctionStr.buffer,ald.account);
		AuctionStr.cnt       = strlen(AuctionStr.buffer);
		AuctionStr.cursor    = strlen(AuctionStr.buffer);
		AuctionStr.color		 = 0;
		AuctionStr.len       = 100;
		AuctionStr.lineLen   = 24;
		AuctionStr.lineDist  = 20;
		AuctionStr.x				 = pActAuctionWT->x + 230;
		AuctionStr.y				 = pActAuctionWT->y + 265;
		AuctionStr.fontPrio  = FONT_PRIO_FRONT;
		// ???????????
		strcpy(AuctionMoney.buffer,ald.money);
		AuctionMoney.cnt       = strlen(AuctionMoney.buffer);
		AuctionMoney.cursor    = strlen(AuctionMoney.buffer);
		AuctionMoney.color		 = 0;
		AuctionMoney.len       = 100;
		AuctionMoney.lineLen   = 28;
		AuctionMoney.lineDist  = 20;
		AuctionMoney.x				 = pActAuctionWT->x +270;
		AuctionMoney.y				 = pActAuctionWT->y +385;
		AuctionMoney.fontPrio  = FONT_PRIO_FRONT;
		// ?????????????
		if(ald.kind == 2)
		{
			strcpy(AuctionItem.buffer,ald.item_effect_string);
			AuctionItem.cnt        = 0;
			AuctionItem.cursor     = 0;
			AuctionItem.color	  	 = 0;
			AuctionItem.len        = 100;
			AuctionItem.lineLen    = 28;
			AuctionItem.lineDist   = 20;
			AuctionItem.x				   = pActAuctionWT->x +220;
			AuctionItem.y				   = pActAuctionWT->y +200;
			AuctionItem.fontPrio   = FONT_PRIO_FRONT;
		}
		// ????Ч
		play_se(202,320,240);
	}
	else
	{
		// ???????????????
		if(pActAuctionWT->hp > 0)
		{
			// ?????趨
			x = pActAuctionWT->x;
			y = pActAuctionWT->y;

			// ??????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK)
			{
				for(i=0;i<2;i++) if(HitDispNo == nButton[i][0]) nButton[i][1] = 1;
				bPress = true;
			}
			// ?????????????
			if(mouse.onceState & MOUSE_LEFT_CRICK_UP)
			{
				for(i=0;i<2;i++) nButton[i][1] = 0;
				bPress = false;
			}
			// ???????????????
			if(nButton[0][1] || nButton[1][1])
			{
				if(nButton[0][1])
				{
					memset(szSendMsg,0,sizeof(szSendMsg));
					sprintf(szSendMsg[0],"%d|",ald.index);
					makeEscapeString(szSendMsg[0],szSendMsg[1],sizeof(szSendMsg[0]));
					if(bNewServer) lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_OK,szSendMsg[1]);
					else   				 old_lssproto_WN_send(sockfd,nowGx,nowGy,indexWN,idWN,WINDOW_BUTTONTYPE_OK,szSendMsg[1]);
				}
				windowTypeWN = -1;
				DeathAction(pActAuctionWT);
				pActAuctionWT = NULL;
				if(pShowPet != NULL)
				{
					DeathAction(pShowPet);
					pShowPet = NULL;
				}
				GetKeyInputFocus(&MyChatBuffer);
				play_se(217,320,240);
				return;
			}
			
			if(windowTypeWN == WINDOW_MESSAGETYPE_AUCTIONMODIFY)
			{
				// ??????????????
				if(MakeHitBox(AuctionStr.x,AuctionStr.y,AuctionStr.x+170,AuctionStr.y+95,DISP_PRIO_BOX2) == true)
				{
					GetKeyInputFocus(&AuctionStr);
				}
				
				// ????????????
				if(MakeHitBox(AuctionMoney.x,AuctionMoney.y,AuctionMoney.x+115,AuctionMoney.y+18,DISP_PRIO_BOX2) == true)
				{
					GetKeyInputFocus(&AuctionMoney);
				}
			}

			// ??????趨button?????λ??
			nButton[0][0] = StockDispBuffer(x+210,y+433,DISP_PRIO_IME3,AUCTION_DECIDE_U,2);
			nButton[1][0] = StockDispBuffer(x+320,y+433,DISP_PRIO_IME3,AUCTION_CANCEL_R_U,2);
			// ????????????
			StockFontBuffer(x+235,y+22,FONT_PRIO_FRONT,0,ald.name,1);
			// ???????
			if(ald.kind == 1)
			{
				if(pShowPet == NULL) pShowPet = MakeAnimDisp(x+280,y+200,ald.grp_no,0);
				// ???????????
				StockFontBuffer(x+235,y+55,FONT_PRIO_FRONT,0,ald.sellname,1);
				// ???????????????
				StockDispBuffer(x+375,y+170,DISP_PRIO_IME3,AUCTION_PET_STAT,0);
				// ?????????
//				itoa(ald.lv,szPetAttrib,10);
                sprintf(szPetAttrib, "%d", ald.lv);
				StockFontBuffer(x+375,y+ 90,FONT_PRIO_FRONT,0,szPetAttrib,1);
//				itoa(ald.act,szPetAttrib,10);
                sprintf(szPetAttrib, "%d", ald.act);
				StockFontBuffer(x+375,y+120,FONT_PRIO_FRONT,0,szPetAttrib,1);
//				itoa(ald.def,szPetAttrib,10);
                sprintf(szPetAttrib, "%d", ald.def);
				StockFontBuffer(x+375,y+148,FONT_PRIO_FRONT,0,szPetAttrib,1);
//				itoa(ald.dex,szPetAttrib,10);
                sprintf(szPetAttrib, "%d", ald.dex);
				StockFontBuffer(x+375,y+174,FONT_PRIO_FRONT,0,szPetAttrib,1);
//				itoa(ald.hp,szPetAttrib,10);
                sprintf(szPetAttrib, "%d", ald.hp);
				StockFontBuffer(x+375,y+202,FONT_PRIO_FRONT,0,szPetAttrib,1);
//				itoa(ald.loyal,szPetAttrib,loyal);
                sprintf(szPetAttrib, "%d", ald.lv);
				StockFontBuffer(x+375,y+230,FONT_PRIO_FRONT,0,szPetAttrib,1);
			}
			else 
			{
				// ???????????
				StockFontBuffer(x+235,y+55,FONT_PRIO_FRONT,ald.name_color,ald.sellname,1);
				// ???????????
				StockFontBuffer2(&AuctionItem);
				// ??????
				StockDispBuffer(x+315,y+155,DISP_PRIO_IME3,ald.grp_no,0);
				// ?????????
				StockDispBuffer(x+315,y+155,DISP_PRIO_IME3,AUCTION_PLANK,0);
			}
			// ????????????
			StockFontBuffer2(&AuctionStr);
			CheckNumber(AuctionMoney.buffer,-1);
			StockFontBuffer2(&AuctionMoney);
			
			// ???????????????,???"???"?????"????"??????
			if(windowTypeWN == WINDOW_MESSAGETYPE_AUCTIONMODIFY) StockDispBuffer(x+160,y,DISP_PRIO_IME3,AUCTION_SURVEY_1,1);
			StockDispBuffer(320,240,DISP_PRIO_IME3,AUCTION_SURVEY,1);
		}
	}
}

void AuctionGetString(int nWhichOne,pALD ald)
{
	int i;
	char buf[256];
	char temp[256];

	memset(buf,0,sizeof(buf));
	memset(temp,0,sizeof(temp));
	makeStringFromEscaped(szpALD);
	// ???????list
	if(nWhichOne == 0)
	{
		for(i=0;i<10;i++)
		{
			getStringToken(szpALD,'|',i+1,sizeof(buf),buf);
			makeStringFromEscaped(buf);
			getStringToken(buf,'|',1,sizeof(temp),temp);												// ?????
			ald[i].index = atoi(temp);
			getStringToken(buf,'|',2,sizeof(ald[i].cdkey),ald[i].cdkey);				// ??????cdkey
			makeStringFromEscaped(ald[i].cdkey);
			getStringToken(buf,'|',3,sizeof(ald[i].account),ald[i].account);		// ????
			makeStringFromEscaped(ald[i].account);
		}
	}
	// ?????????????
	if(nWhichOne == 1)
	{
		getStringToken(szpALD,'|',1,sizeof(temp),temp);											// ?????
		ald->index = atoi(temp);
		getStringToken(szpALD,'|',2,sizeof(ald->name),ald->name);					// ?????????
		makeStringFromEscaped(ald->name);								
		getStringToken(szpALD,'|',3,sizeof(ald->account),ald->account);			// ????
		makeStringFromEscaped(ald->account);
		getStringToken(szpALD,'|',4,sizeof(ald->money),ald->money);				// ???
		getStringToken(szpALD,'|',5,sizeof(temp),temp);										// ???????????
		ald->kind = atoi(temp);
		getStringToken(szpALD,'|',6,sizeof(buf),buf);								  		// ??ó???????????????
		makeStringFromEscaped(buf);
		getStringToken(buf,'|',1,sizeof(ald->sellname),ald->sellname);		// ????????????????
		getStringToken(buf,'|',2,sizeof(temp),temp);		           				// ???????????????
		ald->grp_no = atoi(temp);
		// ?????????
		if(ald[0].kind == 1)
		{
			getStringToken(buf,'|',3,sizeof(temp),temp);									// ???????
			ald->lv = atoi(temp);
			getStringToken(buf,'|',4,sizeof(temp),temp);									// ??????
			ald->act = atoi(temp);
			getStringToken(buf,'|',5,sizeof(temp),temp);									// ??????
			ald->def = atoi(temp);
			getStringToken(buf,'|',6,sizeof(temp),temp);									// ???????
			ald->dex = atoi(temp);
			getStringToken(buf,'|',7,sizeof(temp),temp);									// ??????
			ald->hp = atoi(temp);
			getStringToken(buf,'|',8,sizeof(temp),temp);									// ????????
			ald->loyal = atoi(temp);
		}
		// ?????
		else 
		{
			getStringToken(buf,'|',3,sizeof(temp),temp);									// ???????????? 0:???? 4:???? 5:????
			ald->name_color = atoi(temp);
			getStringToken(buf,'|',4,sizeof(temp),temp);									// ????????
			makeStringFromEscaped(temp);
			strncpy(ald->item_effect_string,temp,64);
		}
	}
}

// ?????????????
void CheckNumber(char *buf,int num)
{
	int i;

	for(i=0;i<(int)strlen(buf);i++) if(*(buf+i) < '0' || *(buf+i) > '9') *(buf+i) = '0';
	if(num > -1){
		if(atoi(buf) > num) sprintf(buf,"%d",num);
	}
	else if(atoi(buf) > CHAR_getMaxHaveGold()) sprintf(buf,"%d",CHAR_getMaxHaveGold());
}


// ?????????????
void CheckSpace(char *buf)
{
	int i,j;
	char *temp;

	for(i=0;i<(int)strlen(buf);i++) 
	{
		if(*(buf+i) == 0x20) 
		{
			temp = (buf+i);
			for(j=0;j<(int)strlen(buf+i);j++)
			{
				*temp = *(temp+1);
				temp++;
			}
			i = 0;
		}
	}
	strcat(buf,"\0");
}
#endif
// Terry end

#ifdef _BLACK_MARKET
void closeBlackMarketWN( void )
{
	play_se( 203, 320, 240 );
	DeathAction( ptActMenuWin );	             		
	ptActMenuWin = NULL;
	windowTypeWN = -1;
	sItem = -1;
	GetKeyInputFocus( &MyChatBuffer );
}
void initBlackMarket( char *data ){
	int  i=0;	
	char sTmp[128]="";
	makeStringFromEscaped( data);
	getStrSplit( (char *)msgWN, data, sizeof( msgWN[0]),
		         sizeof( msgWN)/sizeof( msgWN[0]), sizeof( msgWN[0])-1);
	for(i=0; i<12; i++){
		getStringToken( msgWN[i], '|', 1, sizeof( sTmp)-1, sTmp);
		BMItem[i].iIndex = atoi(sTmp);
		getStringToken( msgWN[i], '|', 2, sizeof( sTmp)-1, sTmp);
        BMItem[i].iGraphicsNum = atoi(sTmp);
		getStringToken( msgWN[i], '|', 3, sizeof( BMItem[i].iName)-1, BMItem[i].iName);		
	}
}

#ifdef _ADD_FAMILY_TAX			   // WON ADD ?????????		
void FMTAXWindowsType( void )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2 };
	static int btnId[] = { -2, -2, -2, -2 };
	static int pushBtnFlag[4];
	
	int selBtnId;
	char dataBuf[256];
	char buf[256];
	int id ;
	int i;

	static int fm_tax_num = 0;

	if( ptActMenuWin == NULL )
	{
		winW = 5;
		winH = 4;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );

#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		play_se( 202, 320, 240 );
	}


	if( ptActMenuWin != NULL )
	{
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );
		selBtnId = focusGraId( btnId, sizeof( btnId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			closeFamilyTaxWN();
		}


		if( mouse.onceState & MOUSE_LEFT_CRICK )

		switch( selBtnId ) {
		case 0:
			sprintf( buf, "L|TAX|%d|%d", 1, fm_tax_num);			
			if( bNewServer)
				lssproto_FM_send( sockfd, buf );
			else
				old_lssproto_FM_send( sockfd, buf );
			closeFamilyTaxWN();
			return;
			break;
		case 1:
			closeFamilyTaxWN();
			return;
			break;
		}

		switch( selBtnId ) {
		
		// Dec TAX
		case 2:

			if( mouse.onceState & MOUSE_LEFT_CRICK_UP && pushBtnFlag[2] == true ){
							
				pushBtnFlag[2] = false;
			}

			if( mouse.onceState & MOUSE_LEFT_CRICK ) {
				fm_tax_num-- ;
				if(fm_tax_num <=0){
					fm_tax_num = 0;
					play_se( 220, 320, 240 );
				}
			}

			break;

		// Add TAX
		case 3:

			if( mouse.onceState & MOUSE_LEFT_CRICK_UP && pushBtnFlag[3] == true ){
							
				pushBtnFlag[3] = false;
			}

			if( mouse.onceState & MOUSE_LEFT_CRICK ) {
				fm_tax_num++ ;
				if(fm_tax_num >= 30){
					fm_tax_num = 30;
					play_se( 220, 320, 240 );
				}
			}

			break;

		}

		for( int i=0 ; i<4 ; i++ )
		{
			if( mouse.state & MOUSE_LEFT_CRICK && pushBtnFlag[i] == true )
				pushBtnFlag[i] = true;
			else	pushBtnFlag[i] = false;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			sprintf( dataBuf, "%s  ????????", pc.familyName );
			StockFontBuffer( winX+80, winY+30,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );
#ifdef _READ16BITBMP
			if(!g_bUseAlpha) StockDispHLine( winX+64, winY+50, 3 );
#endif
			StockFontBuffer( winX+60, winY+70,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, "?  ???", 0 );

			sprintf( dataBuf, "%5d	??", fm_tax_num );
			StockFontBuffer( winX+200, winY+70,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			btnId[3] = StockDispBuffer( winX+190+40, winY+105, DISP_PRIO_IME3,
					CG_UP_BTN + pushBtnFlag[3], 2 );

#ifdef _FMVER21
			if( pc.familyleader == FMMEMBER_LEADER || 
				pc.familyleader == FMMEMBER_ELDER) {
#else
			if( pc.familyleader == 1 ) {
#endif
			
				btnId[2] = StockDispBuffer( winX+190, winY+105, DISP_PRIO_IME3,
									CG_DOWN_BTN + pushBtnFlag[2], 2 );

				btnId[0] = StockDispBuffer( winX+100, winY+160, DISP_PRIO_IME3,
									CG_TRADE_OK_BTN, 2 );
				btnId[1] = StockDispBuffer( winX+100+120, winY+160, DISP_PRIO_IME3,
									CG_TRADE_CANCEL_BTN, 2 );
			}
			else
			{
				btnId[0] = StockDispBuffer( winX+100, winY+160, DISP_PRIO_IME3,
									CG_TRADE_OK_BTN, 2 );
				btnId[1] = StockDispBuffer( winX+100+120, winY+160, DISP_PRIO_IME3,
									CG_TRADE_CANCEL_BTN, 2 );

			}
			
		}
	}


}
#endif 

void BMWindowType( void )
{	
	static int btnId[2];	
	int  selBtnId;	
	int  x, y;
	int  i;	
		
	x = 0;
	y = 0;
	
	if( CheckMenuFlag()
	 || joy_trg[ 0 ] & JOY_ESC
	 || actBtn == 1
	 || menuBtn == 1
	 || disconnectServerFlag == true
	 || wnCloseFlag == 1)
	{
		closeBlackMarketWN();
		return;
	}

	if(ptActMenuWin==NULL)
	{			
		for( i=0; i<2; i++)	
			btnId[i] = -2;		
		ptActMenuWin = MakeWindowDisp( x+100, y+100, 100, 100, NULL, -1);
#ifdef _NEW_RESOMODE  //800 600??
		x = ptActMenuWin->x - 100;
		y = ptActMenuWin->y - 100;
#endif
		play_se( 202, 320, 240 );
	}
	else if(ptActMenuWin->hp>=1)
	{	
		selBtnId = focusGraId( btnId, sizeof( btnId)/sizeof( int ) );
		if(mouse.onceState&MOUSE_LEFT_CRICK)
		{
			switch(selBtnId)
			{
				case 0:
					if(sItem!=-1){
						lssproto_BM_send( sockfd, BMItem[sItem].iIndex);
						closeBlackMarketWN();
						return;
					}
					break;
				case 1:
					closeBlackMarketWN();
				    return;
					break;
			}
		}		
		
		for( i=0; i<12; i++){
		    if(sItem==i)
				StockBoxDispBuffer( x+sItem%4*62+18, y+sItem/4*48+28, x+sItem%4*62+68, y+sItem/4*48+68, DISP_PRIO_BOX2, 213, 0 );
			if(MakeHitBox( x+i%4*62+18, y+i/4*48+28, x+i%4*62+68, y+i/4*48+68, DISP_PRIO_BOX2)){
				char iName[64]="";
				char *sp= BMItem[i].iName;								
				int  j=0;
				int  ip=0;
				char cp;
				char Tmp[256]="";
				char *iip;	
				
				iip= Tmp;

	            while(1){
					cp = BMItem[i].iName[ip++];
		
					if(cp==NULL){
						*iip = '\0';
						StockFontBuffer( x+20, y+205+j*20, FONT_PRIO_FRONT, FONT_PAL_WHITE, Tmp, 1);
						break;
					}
		
					if(IsDBCSLeadByte(cp)){
						*iip++ = cp;
						*iip++ = BMItem[i].iName[ip++];
					}else{
						*iip++ = cp;
					}

					if((iip-Tmp)>=33){
						*iip = '\0';
						StockFontBuffer( x+20, y+205+j*20, FONT_PRIO_FRONT, FONT_PAL_WHITE, Tmp, 1);					
						strcpy( Tmp, "");
						iip= Tmp;						
						j++;
					}
				}

				if(mouse.onceState&MOUSE_LEFT_CRICK){
					sItem = i;
				}	
				StockDispBuffer( x+i%4*62+46, y+i/4*48+48, DISP_PRIO_IME3, BMItem[i].iGraphicsNum+1, 1);
				
				if(BMItem[i].iGraphicsNum==22268) strcpy( iName, "????????");
				else if(BMItem[i].iGraphicsNum==22270) strcpy( iName, "????????");
				else if(BMItem[i].iGraphicsNum==22272) strcpy( iName, "????????");
				else if(BMItem[i].iGraphicsNum==22274) strcpy( iName, "?????????");
				else if(BMItem[i].iGraphicsNum==22276) strcpy( iName, "?????????");
				else if(BMItem[i].iGraphicsNum==22278) strcpy( iName, "????????????");
				else if(BMItem[i].iGraphicsNum==22280) strcpy( iName, "????????");
				else if(BMItem[i].iGraphicsNum==22282) strcpy( iName, "??????????");
				else if(BMItem[i].iGraphicsNum==22284) strcpy( iName, "?????????");
				else if(BMItem[i].iGraphicsNum==22286) strcpy( iName, "???????????");
				else if(BMItem[i].iGraphicsNum==22288) strcpy( iName, "????????");
				else if(BMItem[i].iGraphicsNum==22290) strcpy( iName, "??????????");
				StockFontBuffer( x+20, y+180, FONT_PRIO_FRONT, FONT_PAL_YELLOW, iName, 1);
			}
			StockDispBuffer( x+i%4*62+46, y+i/4*48+48, DISP_PRIO_IME3, BMItem[i].iGraphicsNum, 1);
		}		

        btnId[0] = StockDispBuffer(  x+80, y+272, DISP_PRIO_IME3,   CG_OK_BTN, 2);
		btnId[1] = StockDispBuffer( x+190, y+272, DISP_PRIO_IME3, CG_EXIT_BTN, 2);
		
		StockDispBuffer( x+140, y+150, DISP_PRIO_IME3,  CG_BM_WND, 1);		
	}	
}
#endif

#ifdef _NPC_SELLSTH
static int SellSthPage = 0;
static char SellHeadString[256];
static char SellHeadMessage[256];

static int SellSthBtNo[5];
static int SellSthBtFlag[5];
static int SellSthSPetNum = -1;

#define MAX_LISTNUM		12
typedef struct _tagNSellSthList{
	int use;
	char name[256];
	char headmess[256];
	char sellmess[256];
}NSellSthList;
static NSellSthList SellSthList[MAX_LISTNUM];

typedef struct _tagNSellSthListView{
	int num;
	char username[256];
	char headmess[256];
	char sellmess[256];
	int type;
	char name[256];
	char freename[256];
	int tmp[7];
	char buf[7][256];
}NSellSthListVies;
static NSellSthListVies SellSthListView;

//ACTION *pSellSthFindProcWnd = NULL;
ACTION *pSellSthSellProcWnd = NULL;

static int Inputsellforce=0;
STR_BUFFER SellSthInput;
STR_BUFFER SellSthMesInput;
static int SellSubCMD = 0;
static ACTION *SellSthActPet=NULL;
static int SelectSellSthLook = -1;

void InitSellSth_Menu( char* data )
{
	char buf[256], buf1[256];
	int i, count=0, ti;

	SellSubCMD = 400;
	SelectSellSthLook = -1;

	for( i=0; i<MAX_LISTNUM; i++){
		SellSthList[i].use = 0;
	}

	getStringToken( data, '|', 1, sizeof( buf )-1, buf );
	count = atoi( buf);
	getStringToken( data, '|', 2, sizeof( buf )-1, buf );
	SellSthPage = atoi( buf);

	if( count <= 0 ) return;
	for( i=3; i<count+3; i++){
		getStringToken( data, '|', i, sizeof( buf )-1, buf );
		if( strlen( buf) <= 0 ) continue;

		getStringToken( buf, ',', 1, sizeof( buf1 )-1, buf1 );
		ti = atoi( buf);
		getStringToken( buf, ',', 2, sizeof( buf1 )-1, buf1 );
		sprintf( SellSthList[ti].name, "%s", buf1);
		getStringToken( buf, ',', 3, sizeof( buf1 )-1, buf1 );
		sprintf( SellSthList[ti].headmess, "%s", buf1);
		getStringToken( buf, ',', 4, sizeof( buf1 )-1, buf1 );
		sprintf( SellSthList[ti].sellmess, "%s", buf1);
		SellSthList[ti].use = 1;
	}
	
}

void InitSellSth_View( char* data )
{
	
	char buf[256];
	int i, ti;
	SellSubCMD = 500;
/*
typedef struct _tagNSellSthListView{
	int num;
	char username[256];
	char headmess[256];
	char sellmess[256];
	int type;
	char name[256];
	char freename[256];
	int tmp[6];
	char buf[7][256];
}NSellSthListVies;
static NSellSthListVies SellSthListView;
*/
	getStringToken( data, '|', 1, sizeof( buf )-1, buf );
	SellSthListView.num = atoi( buf);
	getStringToken( data, '|', 2, sizeof( buf )-1, buf );
	sprintf( SellSthListView.username, "%s", buf );
	getStringToken( data, '|', 3, sizeof( buf )-1, buf );
	sprintf( SellSthListView.headmess, "%s", buf );
	getStringToken( data, '|', 4, sizeof( buf )-1, buf );
	sprintf( SellSthListView.sellmess, "%s", buf );

	getStringToken( data, '|', 5, sizeof( buf )-1, buf );
	SellSthListView.type = atoi( buf);
	getStringToken( data, '|', 6, sizeof( buf )-1, buf );
	sprintf( SellSthListView.name, "%s", buf );
	getStringToken( data, '|', 7, sizeof( buf )-1, buf );
	sprintf( SellSthListView.freename, "%s", buf);

	ti = 0;
	for( i=8; i<15; i++){
		getStringToken( data, '|', i, sizeof( buf )-1, buf );
		SellSthListView.tmp[ti++] = atoi( buf);
	}
	ti = 0;
	for( i=15; i<22; i++){
		getStringToken( data, '|', i, sizeof( buf )-1, buf );
		sprintf( SellSthListView.buf[ti++], "%s", buf);
	}
	ti=0;
}

void InitSellSth_Sell( char* data )
{
	memset( SellHeadString, 0, sizeof( SellHeadString));
	memset( SellHeadMessage, 0, sizeof( SellHeadMessage));
	Inputsellforce = 0;
	SellSubCMD = 100;
}

void SellSth_SellSub_MessView( void)
{


	if(pSellSthSellProcWnd == NULL){
		int i;
		pSellSthSellProcWnd = MakeWindowDisp( 640 /2-32*7, 10, 7, 9, NULL, 1);
		play_se(202,320,240);

		for( i=0; i<10; i++){
			SellSthBtNo[i] = -1;
			SellSthBtFlag[i] = 0;
		}
		return;
	}else {
		char buf[256];
		int x, y, x1, y1;
		int selSSthBtnId;
		x = pSellSthSellProcWnd->x ;
		y = pSellSthSellProcWnd->y ;

		SellSthBtNo[0] = StockDispBuffer( x+280, y+410, DISP_PRIO_IME3, CG_TRADE_OK_BTN, 2);
		SellSthBtNo[1] = StockDispBuffer( x+380, y+410, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN, 2);

		selSSthBtnId = focusGraId( SellSthBtNo, sizeof( SellSthBtNo )/sizeof( int ) );
//		selSSthFontBtnId = selFontId( SSthfontId, sizeof( SSthfontId )/sizeof( int ) );

		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			switch( selSSthBtnId){
			case 0:
				{
					char msg[256], data[256];
					sprintf( data, "%d", SellSthListView.num );
					makeEscapeString( data, msg, sizeof( msg )-1 );
					if( bNewServer)
						lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_YES, msg ) ;
					else
						old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_YES, msg ) ;
					wnCloseFlag = 1;
				}
				break;
			case 1:
				{
					char msg[256], data[256];
					sprintf( data, "");
					makeEscapeString( data, msg, sizeof( msg )-1 );
					if( bNewServer)
						lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_OK, msg ) ;
					else
						old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_OK, msg ) ;
					wnCloseFlag = 1;
				}
				
				return;
			}

		}
		sprintf( buf, "????????%s", SellSthListView.username);
		StockFontBuffer( x+220, y+50, FONT_PRIO_FRONT, 0, buf, 0 );
		StockDispBuffer( x+100, y+30, DISP_PRIO_DRAG, CG_TRADE_LINE, 0 );
		StockDispBuffer( x+230, y+30, DISP_PRIO_DRAG, CG_TRADE_LINE, 0 );

		StockFontBuffer( x+220, y+386, FONT_PRIO_FRONT, 0, "?????????OK???????????", 0 );

		StockFontBuffer( x+44, y+22, FONT_PRIO_FRONT, 0, SellSthListView.headmess, 0 );
		char *splitPoint = SellSthListView.sellmess;
		y1 = y+80;
		while( 1 ){
			if( strlen( splitPoint ) > 28 ){
				strncpy( buf, splitPoint, 28 );
				buf[ 28 ] = NULL;
				if( GetStrLastByte( buf ) == 3 ){
					buf[ 27 ] = NULL;
					splitPoint += 27;
				}else{
					buf[ 28 ] = NULL;
					splitPoint += 28;
				}
				StockFontBuffer( x+220, y1, FONT_PRIO_FRONT, 0, buf, 0 ); y1 += 20;
			}else{
				strcpy( buf, splitPoint );
				StockFontBuffer( x+220, y1, FONT_PRIO_FRONT, 0, buf, 0 );
				break;
			}
		}

		StockFontBuffer( x+60, y+260, FONT_PRIO_FRONT, 0, SellSthListView.name, 0 );
		if( strlen( SellSthListView.freename) > 0 &&
			strcmp( SellSthListView.name, SellSthListView.freename ) )
			StockFontBuffer( x+60, y+280, FONT_PRIO_FRONT, 0, SellSthListView.freename, 0 );

		if( SellSthListView.type == 1 ){//????
			StockDispBuffer( x+80, y+200, DISP_PRIO_DRAG, SellSthListView.tmp[1], 0 );
			y1 = y+300;
			x1 = x+40;
			char *splitPoint = SellSthListView.buf[0];
			while( 1 ){
				if( strlen( splitPoint ) > 28 ){
					strncpy( buf, splitPoint, 28 );
					buf[ 28 ] = NULL;
					if( GetStrLastByte( buf ) == 3 ){
						buf[ 27 ] = NULL;
						splitPoint += 27;
					}else{
						buf[ 28 ] = NULL;
						splitPoint += 28;
					}
					StockFontBuffer( x1, y1, FONT_PRIO_FRONT, 0, buf, 0 ); y1 += 20;
				}else{
					strcpy( buf, splitPoint );
					StockFontBuffer( x1, y1, FONT_PRIO_FRONT, 0, buf, 0 );
					break;
				}
			}
		}else if( SellSthListView.type == 2 ){//????
			if ( SellSthActPet == NULL ) {
				SellSthSPetNum = SellSthListView.tmp[0];
				SellSthActPet = MakeAnimDisp( x+120, y+220, SellSthSPetNum , 0 );
			}

			StockDispBuffer( x+50, y+320, DISP_PRIO_DRAG, CG_TRADE_LV_LINE, 0 );
			StockDispBuffer( x+140, y+320, DISP_PRIO_DRAG, CG_TRADE_HP_LINE, 0 );
			StockDispBuffer( x+50, y+342, DISP_PRIO_DRAG, CG_TRADE_ATK_LINE, 0 );
			StockDispBuffer( x+140, y+342, DISP_PRIO_DRAG, CG_TRADE_DEF_LINE, 0 );
			StockDispBuffer( x+50, y+366, DISP_PRIO_DRAG, CG_TRADE_DEX_LINE, 0 );

#ifdef _PET_2TRANS
			if( SellSthListView.tmp[2] == 1 )
				StockFontBuffer( x+140, y+260, FONT_PRIO_FRONT, 0, "??", 0 );
			else if( SellSthListView.tmp[2] == 2 )
				StockFontBuffer( x+140, y+260, FONT_PRIO_FRONT, 0, "???", 0 );
#else
			if( SellSthListView.tmp[2] == 1 )
				StockFontBuffer( x+140, y+260, FONT_PRIO_FRONT, 0, "?", 0 );
#endif
			x1 = x+50;
			y1 = y+100;

			sprintf( buf, "%d", SellSthListView.tmp[1] );
			StockFontBuffer( x1+6, y1+212, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", SellSthListView.tmp[3] );
			StockFontBuffer( x1+88, y1+212, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", SellSthListView.tmp[4] );
			StockFontBuffer( x1+6, y1+232, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", SellSthListView.tmp[5] );
			StockFontBuffer( x1+96, y1+232, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", SellSthListView.tmp[6] );
			StockFontBuffer( x1+6, y1+256, FONT_PRIO_FRONT, 0, buf, 0 );

			x1 = x+300;
			y1 = y+240;
			int i;
			for( i=0; i<7; i++){
				if( i != 0 ) y1+=22;
				StockDispBuffer( x1, y1, DISP_PRIO_DRAG, CG_TRADE_SK1_LINE+i, 0 );
				StockFontBuffer( x1-26, y1-6, FONT_PRIO_FRONT, 0, SellSthListView.buf[i], 0 );
			}
		}
	}
}

void SellSth_Menu( void )
{
	int i, selSSthBtnId=-1;

	if(pSellSthSellProcWnd == NULL){
		pSellSthSellProcWnd = MakeWindowDisp( 185, 160, 270, 160, CG_AUCWND_DETAIL, -1);
		play_se(202,320,240);

		for( i=0; i<10; i++){
			SellSthBtNo[i] = -1;
			SellSthBtFlag[i] = 0;
		}
		return;
	}else {
		int x, y, x1, y1, h;
		char buf[256];

		x = pSellSthSellProcWnd->x ;
		y = pSellSthSellProcWnd->y ;

		SellSthBtNo[0] = StockDispBuffer( x+360, y+220, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN,2);
		SellSthBtNo[2] = StockDispBuffer( x+300, y-120, DISP_PRIO_IME3, CG_PREV_BTN + SellSthBtFlag[2],2);
		SellSthBtNo[3] = StockDispBuffer( x+360, y-120, DISP_PRIO_IME3, CG_NEXT_BTN + SellSthBtFlag[3],2);

		sprintf( buf, "%d", SellSthPage);
		StockFontBuffer( x+324, y-124, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

		x1 = x-160;
		y1 = y-102;
		h= 26;
		

		for( i=0; i<MAX_LISTNUM; i++){
			if( i!= 0 ) y1+=h;
			if( SellSthList[i].use != 1 ) continue;
			if( MakeHitBox( x1, y1, x1+580, y1+20, DISP_PRIO_BOX2 ) == true ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					char msg[256], data[256];
					sprintf( data, "%d", i);
					makeEscapeString( data, msg, sizeof( msg )-1 );
					if( bNewServer)
						lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
					else
						old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, 0, msg ) ;
					wnCloseFlag = 1;
					return;
				}
			}
			sprintf( buf, "%d.[%s] %s", i, SellSthList[i].name, SellSthList[i].headmess);

//			sprintf( buf, "%s", "????");
		 	StockFontBuffer( x1, y1, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );
		}

		selSSthBtnId = focusGraId( SellSthBtNo, sizeof( SellSthBtNo )/sizeof( int ) );
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			switch( selSSthBtnId){
			case 0:
				wnCloseFlag = 1;
				break;
			case 1:
				break;
			case 2:
				if( SellSthPage > 0 ){
					char msg[256], data[256];
					sprintf( data, "");
					makeEscapeString( data, msg, sizeof( msg )-1 );
					if( bNewServer)
						lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_PRE, msg ) ;
					else
						old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_PRE, msg ) ;
					SellSthBtFlag[2] = 1;
				}
				break;
			case 3:
				if( SellSthPage < 20 ){
					char msg[256], data[256];

					sprintf( data, "");
					makeEscapeString( data, msg, sizeof( msg )-1 );
					if( bNewServer)
						lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_NEXT, msg ) ;
					else
						old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_NEXT, msg ) ;
					SellSthBtFlag[3] = 1;
				}
				break;
			}
		}else if( mouse.onceState & MOUSE_LEFT_CRICK_UP ){
			switch( selSSthBtnId){
			case 1:
				break;
			case 2:
				SellSthBtFlag[2] = 0;
				break;
			case 3:
				SellSthBtFlag[3] = 0;
				break;
			}
		}
		StockDispBuffer( 320, 200, DISP_PRIO_IME3, CG_AUCWND_COLUMN, 1);
	}


}
void SellSth_SellSub_MessUpItem( void)
{
	int selSSthBtnId;
	int x, y, w, h, i;
	if( pSellSthSellProcWnd == NULL){

		x = 640/2;
		y = 480/2;
		w=500;
		h=400;
		pSellSthSellProcWnd = MakeWindowDisp( x-w/2, y-h/2, w, h, CG_TRADE_VIEWWND, -1);
		play_se( 202, 320, 240);

		for( i=0; i<10; i++)
			SellSthBtNo[i] = -1;
		selSSthBtnId = -1;
		return;
	}else {
		x = pSellSthSellProcWnd->x;
		y = pSellSthSellProcWnd->y;

		int x1, y1, x2, y2;
		int itemX=x+296, itemY=y+242;
		int w=52, h=50;
		int ls = 0;
		static int selectI=-1;
		static int selectP=-1;

		SellSthBtNo[0] = StockDispBuffer( x+ 10, y+354, DISP_PRIO_IME3, CG_TRADE_OK_BTN,2);
		SellSthBtNo[1] = StockDispBuffer( x+ 100, y+354, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN,2);

		x1 = itemX - w/2;
		x2 = itemX+w/2;
		for( i=0; i<15; i++){
			if( i!=0 && i%5==0) itemY+=h;
			if( pc.item[i+MAX_ITEMSTART].useFlag == 0 ) continue;
			y1 = itemY-h/2;
			y2 = itemY+h/2;
			if( MakeHitBox( x1+(i%5)*w, y1, x2+(i%5)*w, y2, DISP_PRIO_BOX2 ) == true ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					selectI = i+MAX_ITEMSTART;
					selectP = -1;
					if( SellSthActPet != NULL ){
						DeathAction( SellSthActPet);
						SellSthActPet = NULL ; 
					}
				}
			}
			if( selectI == i )StockBoxDispBuffer( x1+(i%5)*w, y1, x2+(i%5)*w, y2, DISP_PRIO_BOX2, 249, 0 );
			StockDispBuffer( itemX+(i%5)*w, itemY, DISP_PRIO_DRAG, pc.item[i+MAX_ITEMSTART].graNo, 0);
		}
		char buf[256];
		x1 = x+280;//-110;
		y1 = y-9;//-48;
		for( i=0; i<5; i++){
			if( i!= 0 ) y1 += 41;
			if( pet[i].useFlag == 0 ) continue;
			if( MakeHitBox( x1-20, y1, x1+256, y1+40, DISP_PRIO_BOX2 ) == true ){
				if( mouse.onceState & MOUSE_LEFT_CRICK ){
					selectI = -1;
					selectP = i;
				}
			}
			if( selectP == i ) StockBoxDispBuffer( x1-20, y1, x1+256, y1+40, DISP_PRIO_BOX2, 249, 0 );
			sprintf( buf, "%s", pet[i].name);
		 	StockFontBuffer( x1, y1, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "%3d", pet[i].atk );
		 	StockFontBuffer( x1, y1+21, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "%3d", pet[i].def );
		 	StockFontBuffer( x1+60, y1+21, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "%3d", pet[i].quick );
		 	StockFontBuffer( x1+110, y1+21, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "%3d", pet[i].maxHp );
		 	StockFontBuffer( x1+160, y1+21, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

			sprintf( buf, "%3d", pet[i].level );
		 	StockFontBuffer( x1+220, y1+21, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 0 );

		}
		StockFontBuffer( x, y+2, FONT_PRIO_FRONT, FONT_PAL_WHITE, "????????????????????", 0 );
		if( selectI != -1 ){
			int kk = MAX_ITEMSTART;
			StockDispBuffer( x+10, y+60, DISP_PRIO_DRAG, pc.item[selectI].graNo, 0 );
			StockFontBuffer( x+80, y+60, FONT_PRIO_FRONT, 0, pc.item[selectI].name, 0 );
			StockFontBuffer( x+80, y+80, FONT_PRIO_FRONT, 0, pc.item[selectI].name2, 0 );
			sprintf( buf, "??????%2d", pc.item[selectI].pile);
			StockFontBuffer( x, y+100, FONT_PRIO_FRONT, 0, buf, 0 );

			sprintf( buf, "?????%s", pc.item[selectI].damage );
			StockFontBuffer( x+80, y+100, FONT_PRIO_FRONT, 0, buf, 0 );

			y1 = y;
			char *splitPoint = pc.item[selectI].memo;
			while( 1 ){
				if( strlen( splitPoint ) > 28 ){
					strncpy( buf, splitPoint, 28 );
					buf[ 28 ] = NULL;
					if( GetStrLastByte( buf ) == 3 ){
						buf[ 27 ] = NULL;
						splitPoint += 27;
					}else{
						buf[ 28 ] = NULL;
						splitPoint += 28;
					}
					StockFontBuffer( x, y1 + 140, FONT_PRIO_FRONT, 0, buf, 0 ); y1 += 20;
				}else{
					strcpy( buf, splitPoint );
					StockFontBuffer( x, y1 + 140, FONT_PRIO_FRONT, 0, buf, 0 );
					break;
				}
			}

		}else if( selectP != -1 ) {
			if ( SellSthActPet == NULL ) {
				SellSthSPetNum = pet[selectP].graNo; 
				SellSthActPet = MakeAnimDisp( x+20, y+170, SellSthSPetNum , 0 );
			}else if ( SellSthActPet != NULL && SellSthSPetNum !=  pet[selectP].graNo ) {
				DeathAction( SellSthActPet);
				SellSthActPet = NULL ; 
				SellSthSPetNum = pet[selectP].graNo; 
				SellSthActPet = MakeAnimDisp( x+20, y+170, pet[selectP].graNo, 0 );
			}
			StockDispBuffer( x+160, y+70, DISP_PRIO_DRAG, CG_TRADE_LINE, 0 );

			StockFontBuffer( x+100, y+60, FONT_PRIO_FRONT, 0, pet[selectP].name, 0 );
			if( strlen( pet[selectP].freeName) > 0 && strcmp( pet[selectP].name, pet[selectP].freeName) )
				StockFontBuffer( x+100, y+80, FONT_PRIO_FRONT, 0, pet[selectP].freeName, 0 );

			StockDispBuffer( x, y+220, DISP_PRIO_DRAG, CG_TRADE_LV_LINE, 0 );
			StockDispBuffer( x+90, y+220, DISP_PRIO_DRAG, CG_TRADE_HP_LINE, 0 );
			StockDispBuffer( x, y+242, DISP_PRIO_DRAG, CG_TRADE_ATK_LINE, 0 );
			StockDispBuffer( x+90, y+242, DISP_PRIO_DRAG, CG_TRADE_DEF_LINE, 0 );
			StockDispBuffer( x, y+266, DISP_PRIO_DRAG, CG_TRADE_DEX_LINE, 0 );

			sprintf( buf, "%d", pet[selectP].level );
			StockFontBuffer( x+6, y+214, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", pet[selectP].maxHp );
			StockFontBuffer( x+88, y+214, FONT_PRIO_FRONT, 0, buf, 0 );

			sprintf( buf, "%d", pet[selectP].atk );
			StockFontBuffer( x+6, y+234, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", pet[selectP].def );
			StockFontBuffer( x+96, y+234, FONT_PRIO_FRONT, 0, buf, 0 );
			sprintf( buf, "%d", pet[selectP].quick );
			StockFontBuffer( x+6, y+258, FONT_PRIO_FRONT, 0, buf, 0 );
		}

		selSSthBtnId = focusGraId( SellSthBtNo, sizeof( SellSthBtNo )/sizeof( int ) );
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			char data[256], msg[256];
			switch( selSSthBtnId){
			case 0:
				if( selectI != -1 ) {
					sprintf( data, "%s|%s|1|%d", SellSthInput.buffer, SellSthMesInput.buffer, selectI);
				}else if( selectP != -1 ) {
					sprintf( data, "%s|%s|2|%d", SellSthInput.buffer, SellSthMesInput.buffer, selectP);
				}
				makeEscapeString( data, msg, sizeof( msg )-1 );
				if( bNewServer)
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_OK, msg ) ;
				else
					old_lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, WINDOW_BUTTONTYPE_OK, msg ) ;
			case 1:
				wnCloseFlag = 1;
				break;
			}
		}

		StockDispBuffer( 320, 220, DISP_PRIO_IME3, CG_TRADE_VIEWWND, 1);
	}
}

void SellSth_SellSub_MessRegist( void )
{
	static int SSthfontId[2];
	int selSSthBtnId;
	int selSSthFontBtnId;
	int x, y, w, h, i;

	// ??????????????н???
	if( pSellSthSellProcWnd == NULL){
		x = 640/2;
		y = 480/2;
		w=500;
		h=400;
		pSellSthSellProcWnd = MakeWindowDisp( x-w/2, y-h/2, w, h, CG_AUCWND_DETAIL, -1);
		play_se( 202, 320, 240);
		Inputsellforce = 0;
		SellSthInput.buffer[0] = NULL;
		SellSthInput.cnt = 0;
		SellSthInput.cursor = 0;
		SellSthInput.len = 32;
		SellSthInput.lineDist = 30;
		SellSthInput.color = 0;
		SellSthInput.x = pSellSthSellProcWnd->x+10;
		SellSthInput.y = pSellSthSellProcWnd->y+42;
		SellSthInput.fontPrio = FONT_PRIO_FRONT;

		SellSthMesInput.buffer[0] = NULL;
		SellSthMesInput.cnt = 0;
		SellSthMesInput.cursor = 0;
		SellSthMesInput.len = 128;
		SellSthMesInput.lineLen = 64;
		SellSthMesInput.lineDist = 26;
		SellSthMesInput.color = 0;
		SellSthMesInput.x = pSellSthSellProcWnd->x+10;
		SellSthMesInput.y = pSellSthSellProcWnd->y+66;
		SellSthMesInput.fontPrio = FONT_PRIO_FRONT;

		for( i=0; i<10; i++)
			SellSthBtNo[i] = -1;
		selSSthBtnId = -1;
		return;
	}else {

		x = pSellSthSellProcWnd->x;
		y = pSellSthSellProcWnd->y;

		int frontX=x-36, frontY=y+42;
		char buf[256];

		StockFontBuffer( frontX, frontY, FONT_PRIO_FRONT, FONT_PAL_WHITE, "????", 1);
		sprintf( buf, "%32s", "");
		SSthfontId[0] = StockFontBuffer( x+10, frontY, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 2 );
		StockFontBuffer( frontX, frontY+26, FONT_PRIO_FRONT, FONT_PAL_WHITE, "?????", 1);
		sprintf( buf, "%64s", "");
		SSthfontId[1] = StockFontBuffer( x+10, frontY+26, FONT_PRIO_FRONT, FONT_PAL_WHITE, buf, 2 );

		SellSthBtNo[0] = StockDispBuffer( x+ 410, y+180, DISP_PRIO_IME3, CG_TRADE_OK_BTN,2);
		SellSthBtNo[1] = StockDispBuffer( x+ 495, y+180, DISP_PRIO_IME3, CG_TRADE_CANCEL_BTN,2);

		selSSthBtnId = focusGraId( SellSthBtNo, sizeof( SellSthBtNo )/sizeof( int ) );
		selSSthFontBtnId = selFontId( SSthfontId, sizeof( SSthfontId )/sizeof( int ) );
		if( mouse.onceState & MOUSE_LEFT_CRICK ){
			switch( selSSthBtnId){
			case 0:
				SellSubCMD = 200;
				DeathAction( pSellSthSellProcWnd);
				pSellSthSellProcWnd = NULL;
				play_se(217,320,240);
				break;
			case 1:
				wnCloseFlag = 1;
				break;
			}

			switch( selSSthFontBtnId ){
			case -1: Inputsellforce = 0; break;
			case 0: Inputsellforce = 1; break;
			case 1: Inputsellforce = 2;	break;
			case 2:	break;
			}
		}
		if( Inputsellforce == 1 ) {
			StockFontBuffer2( &SellSthInput );
			GetKeyInputFocus( &SellSthInput );
			if( SellSthMesInput.buffer[0] != NULL ) StockFontBuffer2( &SellSthMesInput );
		}else if( Inputsellforce == 2 ) {
			StockFontBuffer2( &SellSthMesInput );
			GetKeyInputFocus( &SellSthMesInput );
			if( SellSthInput.buffer[0] != NULL ) StockFontBuffer2( &SellSthInput );		}else {
			GetKeyInputFocus( &MyChatBuffer );
			if( SellSthInput.buffer[0] != NULL ) StockFontBuffer2( &SellSthInput );
			if( SellSthMesInput.buffer[0] != NULL ) StockFontBuffer2( &SellSthMesInput );
		}
/*
SellSthInput
SellSthMesInput
#define CG_AUCWND_DETAIL		26381	// ??????????
#define CG_AUCWND_MESSAGE		26382	// ?????????
#define CG_AUCWND_COLUMN		26383	// ???????
#define CG_AUCWND_CHANGE		26384	// ????????
*/
		StockDispBuffer( 320, 220, DISP_PRIO_IME3, CG_AUCWND_DETAIL, 1);
	}

}


void SellSth_Sell( void )
{
	if( CheckMenuFlag()
		|| (joy_trg[0] & JOY_ESC)
		|| actBtn == 1
		|| menuBtn == 1
		|| disconnectServerFlag == true
		|| wnCloseFlag == 1){

		GetKeyInputFocus( &MyChatBuffer );
		if(pSellSthSellProcWnd != NULL){
			windowTypeWN = -1;
			DeathAction( pSellSthSellProcWnd);
			pSellSthSellProcWnd = NULL;
			play_se(217,320,240);
		}
		if( SellSthActPet != NULL ){
			DeathAction( SellSthActPet);
			SellSthActPet = NULL ; 
		}
		return;
	}

	switch( SellSubCMD){
	case 100:
		SellSth_SellSub_MessRegist();
		break;
	case 200:
		SellSth_SellSub_MessUpItem();
		break;
	case 400:
		SellSth_Menu();
		break;
	case 500:
		SellSth_SellSub_MessView();
		break;
	}
}
#endif

#ifdef _PET_TRACE_MOUSE

#define MAX_SHOW_PET_SET 29 
#define KIND_OF_CHANGE_TIME 5
#define TIME_TO_CHANGE_TIME 30000

int iKindOfPet[MAX_SHOW_PET_SET][2] = {
	{100250,100298},{100307,100310},{100319,100330},{100335,100362},{100367,100396},
	{100430,100430},{100820,100919},{101152,101156},{101177,101180},{101278,101287},
	{101412,101414},{101423,101428},{101437,101489},{101493,101501},{101530,101578},
	{101607,101610},{101612,101622},{101705,101729},{101736,101737},{101741,101744},
	{101746,101748},{101759,101765},{101767,101768},{101813,101816},{101819,101823},
	{101866,101876},{101879,101882},{101884,101886},{101906,101935}
};

int iChangeTime[KIND_OF_CHANGE_TIME] = { 5000,10000,15000,20000,25000 };

int iPetAction[5] = {
	ANIM_ATTACK,
	ANIM_DEAD,
	ANIM_STAND,
	ANIM_WALK,
	ANIM_GUARD
};

SCPlayPet::SCPlayPet()
{
	m_pPet = NULL;
	m_bMove = m_bAIWalk = false;
	m_bChangeDir = true;
	m_iTimeToChangeAction = 0;
	m_iTimeToChangeTime = m_iTimeToChangeAction + TIME_TO_CHANGE_TIME;
	m_iKindOfChangeTime = 0;
}

SCPlayPet::~SCPlayPet()
{
	/*if(m_pPet){
		DeathAction(m_pPet);
		m_pPet = NULL;
	}*/
}

void SCPlayPet::CnangePetAction(bool bUserMove)
{
	if(bUserMove) m_pPet->anim_no = iPetAction[2];
	else{
		m_pPet->anim_no = iPetAction[rand() % 5];
		if(m_pPet->anim_no == ANIM_DEAD) m_pPet->actNo = 9;
		else if(m_pPet->anim_no == ANIM_WALK) 
			m_bAIWalk = true;
	}
}

void SCPlayPet::Proc()
{
	float tmpX,tmpY,tmpDir;

	switch(m_iActionStep){
	// ?????
	case 0:
		srand((unsigned)time(NULL));
		if(m_pPet == NULL){
			int iShowSet;
			int iShowPet;

			iShowSet = rand() % MAX_SHOW_PET_SET;
			iShowPet = iKindOfPet[iShowSet][1] - iKindOfPet[iShowSet][0];
			if(iShowPet == 0) iShowPet = iKindOfPet[iShowSet][0];
			else iShowPet = (rand() % iShowPet) + iKindOfPet[iShowSet][0];
			m_pPet = MakeAnimDisp(rand() % DEF_APPSIZEX,rand() % DEF_APPSIZEY,iShowPet,0);
		}
		m_iActionStep++;
		break;
	case 1:
		if(mouse.onceState & MOUSE_LEFT_CRICK_UP || m_bAIWalk){
			if(m_bAIWalk){
				m_iDestX = rand() % DEF_APPSIZEX;
				m_iDestY = rand() % DEF_APPSIZEY;
				m_bAIWalk = false;
			}
			else{
				m_iDestX = mouse.crickLeftUpPoint.x;
				m_iDestY = mouse.crickLeftUpPoint.y;
			}
			m_iX = m_iDestX - m_pPet->x;
			m_iY = m_iDestY - m_pPet->y;
			if(m_iX > 0) m_iDirx = 1;
			else{
				m_iX = -m_iX;
				m_iDirx = -1;
			}
			if(m_iY > 0) m_iDiry = 1;
			else{
				m_iY = -m_iY;
				m_iDiry = -1;
			}
			if(m_iX >= m_iY) m_iCount = m_iX;
			else m_iCount = m_iY;
			if(m_iX > 20 || m_iY > 20){
				m_pPet->actNo = 0;
				m_bMove = true;
				m_bChangeDir = true;
				b = 0;
			}
		}
		if(m_bMove){
			if(m_iCount == 0){
				CnangePetAction(true);
				m_bMove = false;
			}
			else{
				if(m_pPet->x == m_iDestX || m_pPet->y == m_iDestY || m_bChangeDir){
					tmpX = (float)(m_pPet->x - m_iDestX);
					tmpY = (float)(m_pPet->y - m_iDestY);
					tmpDir = Atan(tmpX,tmpY) + 22.5F - 45.0F * 3;
					AdjustDir(&tmpDir);
					m_pPet->dir = m_pPet->anim_ang = (int)(tmpDir/45);
					m_pPet->anim_no = ANIM_WALK;
					m_bChangeDir = false;
				}
			}
			if(m_iX >= m_iY){
				m_pPet->x += m_iDirx;
				b += m_iY;
				if(b > (m_iX >> 1)){
					b -= m_iX;
					m_pPet->y += m_iDiry;
				}				
				m_iCount--;
			}
			else{
				m_pPet->y += m_iDiry;
				b += m_iX;
				if(b > (m_iY >> 1)){
					b -= m_iY;
					m_pPet->x += m_iDirx;
				}				
				m_iCount--;
			}
		}
		break;
	case 2:
		DeathAction(m_pPet);
		m_pPet = NULL;
		m_iActionStep++;
		break;
	default:break;
	}
	if(m_pPet != NULL){
		if(m_iTimeToChangeTime < TimeGetTime()){
			m_iKindOfChangeTime = rand() % KIND_OF_CHANGE_TIME;
			m_iTimeToChangeTime = TimeGetTime() + TIME_TO_CHANGE_TIME;
			m_iTimeToChangeAction += iChangeTime[m_iKindOfChangeTime];
		}
		if(m_iTimeToChangeAction < TimeGetTime()){
			if(!m_bMove) CnangePetAction(false);
			m_iTimeToChangeAction += iChangeTime[m_iKindOfChangeTime];
		}
	}
}
#endif


#ifdef _MOUSE_DBL_CLICK

char mouseGetName[20];
int checkInBlacklist( char* name);
void addBlacklist( char* name);
void removeBlacklist( char* name);
void cleanBlacklist();

void initMouseGetNameWN( char* name )
{
	strncpy( mouseGetName, name, 20);
	//windowTypeWN = WINDOW_MESSAGETYPE_MOUSEGETNAME;
}


//showRidePetWN
void showMouseGetNameWN( void )
{
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2 };
	int selFontBtnId = -1;
	char dataBuf[256];
	char sendName[64];

	int id ;
	int i;

	static int totalMsgLine;
	static int lineSkip;

	static inBlacklist = false;


	if( ptActMenuWin == NULL )
	{
		//winW = msgWN_W;
		//winH = msgWN_H;
		winW = 5;
		winH = 5;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		//lineSkip = 20;
		//totalMsgLine = (winH*48-56)/lineSkip;

		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		play_se( 202, 320, 240 );	// ????????


		if( checkInBlacklist( mouseGetName) )
			inBlacklist = true;
		else
			inBlacklist = false;

	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			mouseGetName[0] = NULL;
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}
		
		if( id >= 0 )
		{
			switch( id) {
#ifdef _TELLCHANNEL
			case 1: // ????
				//pc.etcFlag |= PC_ETCFLAG_CHAT_TELL;
				pNowStrBuffer->buffer[ 0 ] = NULL;
				pNowStrBuffer->cursor=0;
				pNowStrBuffer->cnt = 0;
				sprintf( sendName, "%s ", mouseGetName);
				StrToNowStrBuffer( sendName);
				TalkMode = 1;
				break;
#endif
			case 2: // ??????
				if( inBlacklist)
					removeBlacklist( mouseGetName);
				else {
					char sendName[64];
					sprintf(" IceCloud go Shit!!! ");
					addBlacklist( mouseGetName);
				}
				break;
			case 3: // ?????????
				cleanBlacklist();
				break;
			case 4: // ???
				// Black Jack
				break;
			case 5: // ????
				//
				break;

			}
			mouseGetName[0] = NULL;

			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}
		

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{


			sprintf( dataBuf, "?????? %s ??", mouseGetName);
			StockFontBuffer( winX+40, winY+30,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );

			fontId[1] =
					StockFontBuffer( winX+110, winY+60,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ??????? ", 2 );
			if( inBlacklist)
				fontId[2] =
					StockFontBuffer( winX+110, winY+90,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, "?????????", 2 );
			else
				fontId[2] =
					StockFontBuffer( winX+110, winY+90,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, "?????????", 2 );
			
			fontId[3] =
					StockFontBuffer( winX+110, winY+120,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, "?????????", 2 );

			fontId[4] =
					StockFontBuffer( winX+110, winY+120,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ??    ? ", 2 );

			fontId[5] =
					StockFontBuffer( winX+110, winY+120,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ??    ?? ", 2 );

			
			fontId[0] =
					StockFontBuffer( winX+110, winY+200,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, " ??????? ", 2 );

		}
	}

}
#endif


#ifdef _CONTRACT

typedef struct{
	int used;
	char name[32];
	char fmname[32];
} contractSignTag;

contractSignTag contractSign[4];
char	contractDetail[2048];
int		contractArgnum;
char	contractTime[512];
int		contractSignIndex;
int		contractSigned;

void initContractWN( char* data )
{
	char	buf[512], token[64];
	int		i, index;
	char	contractArg[512] ="";

	strcpy( contractTime, "");

	getStringToken( data, '|', 1, sizeof( contractDetail )-1, contractDetail );
	getStringToken( data, '|', 2, sizeof( buf )-1, buf );
	contractArgnum = atoi( buf);
	getStringToken( data, '|', 3, sizeof( contractArg )-1, contractArg );
	getStringToken( data, '|', 4, sizeof( contractTime )-1, contractTime );
	
	for( i=0; i<4; i++) {
		contractSign[i].used = false;
	}
	contractSignIndex = -1;
	contractSigned = false;

	for( i=0; i<4; i++) {
		if( getStringToken( contractArg, ',', i+1, sizeof( buf )-1, buf ) != 0)
			break;
		if( strlen( buf) == 0)	break;
		getStringToken( buf, ':', 1, sizeof( token )-1, token );
		index = atoi( token);
		if( index <0 || index >=4)	break;
		getStringToken( buf, ':', 2, sizeof( contractSign[index].name )-1, contractSign[index].name );
		getStringToken( buf, ':', 3, sizeof( contractSign[index].fmname )-1, contractSign[index].fmname );
		contractSign[index].used = true;
		// ?ж?????????
		if( !strcmp( contractSign[index].name, pc.name) )
			contractSigned = true;
	}

}

void showContractWN()
{
#define MAX_LINE	40
#define MAX_CHAR	56
#define LINE_PER_PAGE	8
	static int winX, winY;
	static int winW, winH;
	static int fontId[] = { -2, -2, -2, -2, -2, -2, -2, -2 };
	static int page = 0;
	static int max_page = 0;
	static char msgLine[MAX_LINE][MAX_CHAR+1];
	char* pStart;
	int selFontBtnId = -1;
	char dataBuf[1024];

	int id ;
	int i;

	static int totalMsgLine;
	static int lineSkip;
 
	static int inBlacklist = false;


	if( ptActMenuWin == NULL )
	{
		//winW = msgWN_W;
		//winH = msgWN_H;
		winW = 9;
		winH = 8;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;


		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
#ifdef _NEW_RESOMODE  //800 600??
		winX = ptActMenuWin->x;
		winY = ptActMenuWin->y;
#endif
		
		// ??????????
		memset( msgLine, 0, sizeof( msgLine));
		page =0;
		max_page =0;
		pStart = contractDetail;
		for( i =0; i <MAX_LINE; i++) {
			char* pFind;
			pFind = strstr( pStart, "\\n");
			if( pFind != NULL && pFind-pStart < MAX_CHAR ) { // ?л??з?????δ????
				strncpy( msgLine[i], pStart, pFind - pStart);
				pStart = pFind +2;
			}
			else if( pFind != NULL && pFind-pStart >= MAX_CHAR ) { // ?л??з?????????
				strncpy( msgLine[i], pStart, MAX_CHAR);
				pStart += MAX_CHAR;
			}
			else if( strlen( pStart) > MAX_CHAR ) {
				strncpy( msgLine[i], pStart, MAX_CHAR);
				pStart += MAX_CHAR;
			}
			else {
				strncpy( msgLine[i], pStart, strlen( pStart) );
				pStart += strlen( pStart);
				break;
			}

		}
		max_page = i / LINE_PER_PAGE;

		//lineSkip = 20;
		//totalMsgLine = (winH*48-56)/lineSkip;
		
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}
		
		play_se( 202, 320, 240 );	// ????????


	}


	if( ptActMenuWin != NULL )
	{
		// ??????k???R?e
		id = selFontId( fontId, sizeof( fontId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || (joy_trg[ 0 ] & JOY_ESC)
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
				id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}

		if( id >= 0 ) {
			switch( id) {
			case 0:
			case 1:
			case 2:
			case 3:
				// ???
				if( contractSignIndex != id)
					contractSignIndex = id;
				else
					contractSignIndex = -1;
				break;
			case 6: // ????
				if( page > 0 )	page--;
				break;
			case 7: // ????
				if( page < max_page )	page++;
				break;
			case 4: // ???
				if( contractSignIndex != -1 )
					lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, contractSignIndex, "" );
			default:
				windowTypeWN = -1;
				wnCloseFlag = 0;
				DeathAction( ptActMenuWin );
				ptActMenuWin = NULL;
				return;
			}
		}
		for( i = 0; i < sizeof( fontId )/sizeof( int ); i++ )
		{
			fontId[i] = -2;
		}

		if( ptActMenuWin != NULL && ptActMenuWin->hp >= 1 )
		{
			char sideComment[4][20] = { "甲方", "乙方", "丙方", "丁方"};
			
			// ??????????
			for( i =0; i <LINE_PER_PAGE; i++) {
				StockFontBufferExt( winX+30, winY+(i*26)+30,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, msgLine[(page*LINE_PER_PAGE)+i], 0, 18 );
			}

			for( i =0; i <contractArgnum; i++) {
				int x, y;

				// ?????????
				//x = winX+40+(i%2)*240;
				//y = winY+(winH*48)-130+((i/2)*28);
				// ?????????
				x = winX + 40;
				y = winY +(winH*48) -110 +(i*24);

				if( contractSign[i].used ) {
					// ???????????λ
					sprintf( dataBuf, "%s： %s  %s", sideComment[i],
						contractSign[i].fmname, contractSign[i].name);
					StockFontBuffer( x, y,
							FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );
				}
				else {
					// ??????????λ
					if( contractSignIndex == i) {
						sprintf( dataBuf, "%s： %s  %s", sideComment[i], pc.familyName, pc.name );
						fontId[i] =
							StockFontBuffer( x, y, FONT_PRIO_FRONT, FONT_PAL_RED, dataBuf, 2 );
					}
					else {
						// ?????????λ
						if( contractSigned == false ) {
							sprintf( dataBuf, "%s：   在此签名", sideComment[i] );
							fontId[i] =
								StockFontBuffer( x, y, FONT_PRIO_FRONT, FONT_PAL_BLUE2, dataBuf, 2 );
						}
						// ??????????????
						else {
							sprintf( dataBuf, "%s??   尚未签名", sideComment[i] );
							fontId[i] =
								StockFontBuffer( x, y, FONT_PRIO_FRONT, FONT_PAL_BLUE2, dataBuf, 0 );
						}
					}
				}
			}
			
			fontId[4] =
					StockFontBuffer( winX+100, winY+(winH*48)-50,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, "确  定", 2 );

			if( contractTime[0] != NULL ) {
				sprintf( dataBuf, "立书日：%s", contractTime);
				StockFontBuffer( winX+(winW*64)-200, winY+(winH*48)-50,
						FONT_PRIO_FRONT, FONT_PAL_WHITE, dataBuf, 0 );
			}
			else {
				fontId[5] =
					StockFontBuffer( winX+(winW*64)-150, winY+(winH*48)-50,
						FONT_PRIO_FRONT, FONT_PAL_YELLOW, "取  消", 2 );
			}
			
			if( page > 0 )
				fontId[6] =
					StockFontBuffer( winX+150, winY+248,
					FONT_PRIO_FRONT, FONT_PAL_YELLOW, "上一页", 2 );
			
			if( page < max_page )
				fontId[7] =
					StockFontBuffer( winX+(winW*64)-200, winY+248,
					FONT_PRIO_FRONT, FONT_PAL_YELLOW, "下一页", 2 );

		}
	}

}

#endif

//?????
#ifdef _RACEMAN
char linemsgWN[10][200];   //????????????
char ranktitle[60];		   //???а?????	

void initRacemanRankWN( char* data)   //??????????
{
	int i,j;

	makeStringFromEscaped( data );

	selStartLine = 0;
	char namebuf[256],catchbuf[32],rankbuf[8];	

	memset(linemsgWN,0,sizeof(linemsgWN));
	ranktitle[0]='\0';
	//lnmake = 0;
	//getStrSplit( (char *)msgWN, data, sizeof( msgWN[0] ),
	//	sizeof( msgWN )/sizeof( msgWN[0] ), 80 );
 	
	getStringToken( data , ' ', 1 , sizeof(ranktitle)-1, ranktitle);
	j =0 ;
	for( i = 1; i < 31 ; i++ ){	
		//???????
		if(i%3==1)
			getStringToken( data , ' ', i+1 , sizeof(rankbuf)-1, rankbuf);
		else if(i%3==2)
			getStringToken( data , ' ', i+1 , sizeof(namebuf)-1, namebuf);
		else{
			getStringToken( data , ' ', i+1, sizeof(catchbuf)-1, catchbuf);
			//sprintf(msgWN[j]," %-7s%-18s%-16s%13d%12d%14d%", lvbuf, fmnamebuf, namebuf, sumdp, taldp-sumdp, taldp);				
			sprintf(linemsgWN[j]," %-10s%-27s%s", rankbuf , namebuf , catchbuf );				
			j++;
		}					
	}
	
}

void showRacemanRankWN( void )
{
	static int winW,winH;
	static int winX,winY;	
	static int btnId[] = { -2, -2, -2, -2, -2, -2 };
	static int btnLoc[6][2];
	static int btnCnt;

	int  id;// id2 ;
	int  i, j;
	int  mask;
	int  btn;	
	char *btnTitle[] =
	{
		" OK ",
		"CANCEL",
		"确  定",
		"取  消",
		"上一页",
		"下一页"
	};		


	if( ptActMenuWin == NULL )
	{
		winW = 10-2-2;
		winH = 7;
		winX = (640 - winW*64)/2;
		winY = (456 - winH*48)/2;
		ptActMenuWin = MakeWindowDisp( winX, winY, winW, winH, NULL, 1 );
		mask = 1;
		btnCnt = 0;
		for( i = 0; i < 10; i++, mask <<= 1 )
		{
			if( buttonTypeWN & mask )
			{
				btnCnt++;
				if( btnCnt == 4 )
					break;
			}
		}
		if( btnCnt > 0 )
		{
			int w;
			w = winW*64/(btnCnt+1);
			for( i = 0; i < btnCnt; i++ )
			{
				btnLoc[i][0] = w * (i+1) - 27;				
				btnLoc[i][1] = 295;
			}
		}
		play_se( 202, 320, 240 );

	}
	if( ptActMenuWin != NULL )
	{		
		id = selFontId( btnId, sizeof( btnId )/sizeof( int ) );

		if( CheckMenuFlag()
		 || joy_trg[ 0 ] & JOY_ESC
		 || actBtn == 1
		 || menuBtn == 1
		 || disconnectServerFlag == true
		 || wnCloseFlag == 1 )
		{
			id = 100;
			windowTypeWN = -1;
			wnCloseFlag = 0;
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
		}

		if( 0 <= id && id < 6
		    //|| 0 <= id2 && id2 < 12
		    || id == 100 )
		{
			//char data[256];
			//char msg[256];
			if( id == 100 )
			{
				btn = WINDOW_BUTTONTYPE_CANCEL;
			}
			else
			if( 0 <= id && id < 6 )
			{
				btn = 1;
				btn <<= id;
			}
			else
			{
				btn = 0;
			}
			
			
			//makeEscapeString( data, msg, sizeof( msg )-1 );
      
			lssproto_WN_send( sockfd, nowGx, nowGy, indexWN, idWN, btn, "" ) ;			
			windowTypeWN = -1;
		}
		if( id >= 0 ){ //|| id2 >= 0) {
			DeathAction( ptActMenuWin );
			ptActMenuWin = NULL;
			return;
		}

		if( ptActMenuWin->hp >= 1 )
		{
			StockFontBuffer( winX+235-130 , winY+21+10,
					    FONT_PRIO_FRONT, FONT_PAL_YELLOW, ranktitle , 0 );//"?????????????????а?", 0 );
			StockFontBuffer( winX+25, winY+16+2*21,
					    FONT_PRIO_FRONT, FONT_PAL_GREEN, "‘排名’      ‘   名    称  ’         ‘抓宠数量’ " , 0 );
																	  	
			//????????
			int fontcolor = FONT_PAL_WHITE;
			for( i = 0; i < 10 ; i++ )			//??????
			{
				if( strlen( linemsgWN[i] ) != NULL )
				{		
					StockFontBuffer( winX+40, winY+14+(i+3)*21,FONT_PRIO_FRONT, fontcolor,linemsgWN[i], 0 );
				}
			}
			//?????
			j = 0;
			for( i = 0, mask = 1; i < 6; i++, mask <<= 1 )
			{
				btnId[i] = -2;
				if( buttonTypeWN & mask )
				{
					if( j < btnCnt )
					{
						btnId[i] =
							StockFontBuffer( winX+btnLoc[j][0], winY+btnLoc[j][1],
								FONT_PRIO_FRONT, FONT_PAL_YELLOW, btnTitle[i], 2 );
						j++;
					}
				}
			}

		}
	}
}

#endif

#ifdef _80_LOGIN_PLAY
void _80LoginProc(void)
{
	static ACTION	*pMainChar = NULL, *pFace = NULL, *pPet[5], *pFacePet = NULL;
	static int		iWaitCount = 0, iX = DEF_APPSIZEX >> 1, iY = DEF_APPSIZEY >> 1, iBackBMP = CG_LOGO;

	StockDispBuffer(iX, iY, DISP_PRIO_BG, iBackBMP, 0);
	if (joy_trg[0] & JOY_ESC)
		SubProcNo = 8;
	switch (SubProcNo)
	{
	// ????????
	case 0:
		pMainChar = MakeAnimDisp(DEF_APPSIZEX, DEF_APPSIZEY >> 1, 100400, 0);
		ATR_CHR_ACT(pMainChar) = ANIM_WALK;
		ATR_CHR_ANG(pMainChar) = 2;
		ATR_CHR_H_POS(pMainChar) = ATR_H_POS(pMainChar);
		SubProcNo++;
		break;
	// ????????????м?
	case 1:
		ATR_H_POS(pMainChar) -= 2;
		// ??????м???
		if (ATR_H_POS(pMainChar) < DEF_APPSIZEX >> 1)
		{
			// ????????????
			ATR_CHR_ACT(pMainChar) = ANIM_ANGRY;
			ATR_CHR_ANG(pMainChar) = 0;
			SubProcNo++;
		}
		break;
	// ????????
	case 2:
		if (ATR_CHR_TIM(pMainChar) <= 0)
			iWaitCount++;
		if (iWaitCount > 5)
		{
			iWaitCount = 0;
			SubProcNo++;
		}
		break;
	// ???????????
	case 3:
		ATR_CHR_ACT(pMainChar) = ANIM_STAND;
		ATR_CHR_ANG(pMainChar) = 6;
		pFace = GetAction(PRIO_CHR, 0);
		ATR_CHR_ACT(pFace) = ANIM_STAND;
		ATR_CHR_NO(pFace) = SPR_screamer;
		ATR_H_POS(pFace) = ATR_H_POS(pMainChar);
		ATR_V_POS(pFace) = ATR_V_POS(pMainChar) - 100;
		ATR_DISP_PRIO(pFace) = DISP_PRIO_BOX3;
		SubProcNo++;
		break;
	// ????????
	case 4:
		if (pattern(pFace, ANM_NOMAL_SPD, ANM_NO_LOOP))
		{
			int iShowSet, iShowPet;

			// ???????
			ATR_CHR_ANG(pMainChar) = 2;
			ATR_CHR_ACT(pMainChar) = ANIM_WALK;
			// ???
			ATR_CHR_NO(pFace) = SPR_perspiration;
			for (int i = 0; i < 5; i++)
			{
				iShowSet = RAND(0, MAX_SHOW_PET_SET);
				iShowPet = iKindOfPet[iShowSet][1] - iKindOfPet[iShowSet][0];
				if (iShowPet == 0)
					iShowPet = iKindOfPet[iShowSet][0];
				else
					iShowPet = RAND(0, iShowPet) + iKindOfPet[iShowSet][0];
				pPet[i] = MakeAnimDisp(DEF_APPSIZEX, DEF_APPSIZEY >> 1, iShowPet, 0);
				ATR_CHR_ACT(pPet[i]) = ANIM_WALK;
				ATR_CHR_ANG(pPet[i]) = 2;
				ATR_H_POS(pPet[i]) += RAND(10, 100);
				ATR_CHR_H_POS(pPet[i]) = ATR_H_POS(pPet[i]);
			}
			pFacePet = GetAction(PRIO_CHR, 0);
			ATR_CHR_ACT(pFacePet) = ANIM_STAND;
			ATR_CHR_NO(pFacePet) = SPR_cramp;
			ATR_H_POS(pFacePet) = ATR_H_POS(pPet[0]);
			ATR_V_POS(pFacePet) = ATR_V_POS(pPet[0]) - 100;
			ATR_DISP_PRIO(pFacePet) = DISP_PRIO_BOX3;
			iX = 320;
			SubProcNo++;
		}
		break;
	// ???????????
	case 5:
		pattern(pFace, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(pFacePet, ANM_NOMAL_SPD, ANM_LOOP);
		iX += 4;
		if (iX > 960)
		{
			// ????????
			iBackBMP = CG_TITLE_ID_PASS;
			iX = -320;
			SubProcNo++;
		}
		break;
	// ?????????????
	case 6:
		pattern(pFace, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(pFacePet, ANM_NOMAL_SPD, ANM_LOOP);
		iX += 4;
		if (iX >= 320)
			SubProcNo++;
		break;
	// ?????治??,??????????????
	case 7:
		pattern(pFace, ANM_NOMAL_SPD, ANM_LOOP);
		pattern(pFacePet, ANM_NOMAL_SPD, ANM_LOOP);
		{
			int iOutOf0 = 0;

			if (ATR_H_POS(pMainChar) > -24)
			{
				ATR_H_POS(pMainChar) -= 4;
				ATR_H_POS(pFace) -= 4;
			}
			for (int i = 0; i < 5; i++)
			{
				if (ATR_H_POS(pPet[i]) < -24)
					iOutOf0++;
				ATR_H_POS(pPet[i]) -= 4;
			}
			ATR_H_POS(pFacePet) -= 4;
			if (iOutOf0 == 5)
				SubProcNo++;
		}
		break;
	default:
		DeathAction(pMainChar);
		DeathAction(pFace);
		DeathAction(pFacePet);
		DeathAction(pPet[0]);
		DeathAction(pPet[1]);
		DeathAction(pPet[2]);
		DeathAction(pPet[3]);
		DeathAction(pPet[4]);
		SubProcNo = 0;
		ProcNo = PROC_ID_PASSWORD;
		break;
	}
}
#endif




#ifdef _PK2007
void selectpkProc( void )
{
	int ret;
	if( SubProcNo == 0 )
	{
		SubProcNo++;
		downloadpkListProcNo = 0;
	}
	if( SubProcNo == 1 )
	{
		ret = downloadpkList();
		switch( ret )
		{
		case 1://??? pk server ?б?
			SubProcNo = 2;
		break;
		case 2:// pk server ?б??0
			SubProcNo = 3;
		break;
		default:
		break;
		}
	}
	if( SubProcNo == 2 )
	{
		
	}
	if( SubProcNo == 3 )
	{
		ChangeProc( PROC_CHAR_SELECT );
	}
	StockDispBuffer( 320, 240, DISP_PRIO_BG, CG_TITLE, 0 );
	RunAction();		
	StockTaskDispBuffer();
}
#endif