/************************/
/*	chat.c				*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/font.h"
//#include "../caryIme.h"
#include "../systeminc/netproc.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/savedata.h"
#include "time.h"
#include "../systeminc/menu.h"
#include "../systeminc/t_music.h"
#include "../systeminc/t_music.h"
#include "../systeminc/tool.h"
#include "../systeminc/lssproto_cli.h"
#include "../systeminc/netmain.h"
#include "../systeminc/chat.h"
#include "../NewProto/protocol.h"
#ifdef _TALK_WINDOW
#include "../systeminc/talkwindow.h"
#endif
#include "../systeminc/EncryptClient.h"

extern INPUT_HISTORY InputHistory ;
STR_BUFFER SubBuffer; 



// ?????????
CHAT_BUFFER ChatBuffer[ MAX_CHAT_LINE ];

// ����?�V????????
STR_BUFFER MyChatBuffer;

// ???�V????????????????
STR_BUFFER *pNowStrBuffer = NULL;

// ??????????
int NowChatLine = 0;
// ????
int NowMaxChatLine = DEF_CHAT_LINE;
// ??��?��??
int NowMaxVoice = DEF_VOICE;
// ????����????
int CursorFlashCnt = 0;
// ??????????��
int ChatLineSmoothY = 0 ;

#define CAHT_HISTORY_STR_FILE_NAME 	"/sdcard/jerrysa/chathis.dat" 	// ??????????????
// ???????????�B�l
CHAT_HISTORY ChatHistory;

#define MAX_SHIELD_SIZE 5000
//typedef CEncryptClient<0x55, 0x168, 0xC9, 0x3C, 0x6B, 0x49, 0x81, 0x65>	ENCRYPTCONF;
char SayShieldList[MAX_SHIELD_SIZE][24];
char NameShieldList[MAX_SHIELD_SIZE][24];

// ??????? 
void KeyboardTab( void );

// ??????
FILE *chatLogFile = NULL;
char chatLogFileName[] = "CHAT_000000.TXT";
void openChatLogFile( void );

/*
#ifdef _TELLCHANNEL				// (���ɿ�) ROG ADD ����Ƶ��
char ReTellName[] = "";
#endif 
*/

void InitChat( void )
{
	memset( ChatBuffer, 0, sizeof( CHAT_BUFFER ) * MAX_CHAT_LINE );
	memset( &MyChatBuffer, 0, sizeof( STR_BUFFER ) );
	MyChatBuffer.len = 86;
#ifndef _CHANNEL_MODIFY
	MyChatBuffer.fontPrio = FONT_PRIO_BACK;
#else
	MyChatBuffer.fontPrio = FONT_PRIO_CHATBUFFER;
#endif
	MyChatBuffer.x = 8;
	MyChatBuffer.y = 432 + DISPLACEMENT_Y;
	getUserChatOption();
	openChatLogFile();
#ifdef _CHANNEL_MODIFY
	FILE *pf;
	char buf;

	if ((pf = fopen("/sdcard/jerrysa/channel.dat","r")) == NULL)
	{
		pc.etcFlag |= PC_ETCFLAG_CHAT_TELL;
		if ((pf = fopen("/sdcard/jerrysa/channel.dat", "w+")) == NULL)
			return;
		else
		{
			char buf[3];

			buf[0] = (pc.etcFlag & PC_ETCFLAG_CHAT_TELL) ? 1:0;
			buf[1] = (pc.etcFlag & PC_ETCFLAG_CHAT_SAVE) ? 1:0;
			buf[2] = (pc.etcFlag & PC_ETCFLAG_CHAT_OCC) ? 1:0;
			fwrite(buf, 1, sizeof(buf), pf);
			fclose(pf);
		}
	}
	else
	{
		fread(&buf,1,sizeof(char),pf);
		if (buf)
			pc.etcFlag |= PC_ETCFLAG_CHAT_TELL;
		fread(&buf,1,sizeof(char),pf);
		if (buf)
			pc.etcFlag |= PC_ETCFLAG_CHAT_SAVE;
		fread(&buf,1,sizeof(char),pf);
		if (buf)
			pc.etcFlag |= PC_ETCFLAG_CHAT_OCC;
		fclose(pf);
	}

	if (pc.familyleader <= 0) pc.etcFlag &= ~PC_ETCFLAG_CHAT_FM;
	else pc.etcFlag |= PC_ETCFLAG_CHAT_FM;

	if (pc.profession_class == 0) pc.etcFlag &= ~PC_ETCFLAG_CHAT_OCC;

	//else pc.etcFlag |= PC_ETCFLAG_CHAT_OCC;

	if (bNewServer) lssproto_FS_send(sockfd,pc.etcFlag);
	else old_lssproto_FS_send(sockfd,pc.etcFlag);
#endif
}

void openChatLogFile( void )
{
	if( chatLogFile == NULL ){
#ifdef _DEBUG
		chatLogFile = fopen( chatLogFileName, "wt" );
#else
		struct tm *nowTime;
		time_t longTime;
		time( &longTime );
		nowTime = localtime( &longTime );
		sprintf( chatLogFileName, "CHAT_%02d%02d%02d.TXT",
			(nowTime->tm_year % 100), nowTime->tm_mon+1, nowTime->tm_mday );

		chatLogFile = fopen( chatLogFileName, "a" );
		if( chatLogFile ){
			fprintf( chatLogFile, "----- Login: %02d/%02d/%02d %02d:%02d:%02d -----\n",
				(nowTime->tm_year % 100), nowTime->tm_mon+1, nowTime->tm_mday,
				nowTime->tm_hour, nowTime->tm_min, nowTime->tm_sec );
		}
#endif
	}
}

bool SaveChatHistoryStr( int no )
{
	FILE *fp; 
	// ????????????
	if( ( fp = fopen( CAHT_HISTORY_STR_FILE_NAME, "r+b" ) ) == NULL )
		return false;
	// ??????��????????????��???
	fseek( fp, sizeof( ChatHistory.str[ 0 ] ) * no, SEEK_SET );
	// ��?????????
	if( fwrite( &ChatHistory.str[ no ], sizeof( ChatHistory.str[ 0 ] ), 1, fp ) < 1 ){
	
		fclose( fp );// ????????
		return false;
	}
	
	// ?��???�k??��???????????��???
	fseek( fp, sizeof( ChatHistory.str[ 0 ] ) * MAX_CHAT_HISTORY, SEEK_SET );
	// ?��???�k??��V
	if( fwrite( &no, sizeof( int ), 1, fp ) < 1 ){
	
		fclose( fp );// ????????
		return false;
	}
	
	// ????????
	fclose( fp );
	
	return true;
}

// ???????��??��??? ****************************************************************/
bool LoadChatHistoryStr( void )
{
	FILE *fp;

	// ?????????�k?????
	ChatHistory.nowNo = -1;

	// ��???????????
	if( ( fp = fopen( CAHT_HISTORY_STR_FILE_NAME, "rb" ) ) == NULL ){
		// ?????�ɤ�??��
		if( ( fp = fopen( CAHT_HISTORY_STR_FILE_NAME, "wb" ) ) != NULL ){
			// ???????
			fwrite( &ChatHistory, sizeof( CHAT_HISTORY ) - sizeof( int ), 1, fp );
			fclose( fp );	// ????????
		}
		
		// ?��???????�k?????
		ChatHistory.nowNo = MAX_CHAT_HISTORY - 1;
		
		return false;
	}
	
	// ???��???
	if( fread( &ChatHistory, sizeof( CHAT_HISTORY ) - sizeof( int ), 1, fp ) < 1 ){
		
		fclose( fp );	// ????????
		return false;
	}
	// ????????
	fclose( fp );
	
	return true;
}

#define MAX_PATH          260
/* ��??????�V?��????? ****************************************************/
//����������
void* EncryptFileName( char* pszResFile,unsigned int &nSize )
{
//	ENCRYPTCONF	myConfFileCncryp;
//	if( NULL != pszResFile ) ; //  [Chancy2008-4-16 21:51]
//
//	char szFile[MAX_PATH] = "";
//	char szResFileTemp[MAX_PATH] = "" ;
//	//strcpy( szResFileTemp , pszResFile , sizeof( szResFileTemp ) ) ;
//
//	char* pszReadName = szResFileTemp; //  [Chancy2008-4-16 21:52]
//
//	if ( !pszReadName )
//		return NULL;
//
//	sprintf(szFile,"data/%s.bin",pszReadName);
//
//	FILE* fp = fopen(szFile, "rb");
//
//	if ( !fp )
//	{
//		return false;
//	}
//
//	(myConfFileCncryp).Init();
//
//	fseek(fp,0,SEEK_END);//move file pointer to file end
//	unsigned int unFileSize =  ftell( fp );
//	nSize = ftell( fp );
//	fseek( fp,0,SEEK_SET );// RESTORE
//
//	void* pBuffer = new char[ unFileSize + 1 ]; // apply 1 char memory for '\0' because char pointer
//
//	if( !pBuffer )
//		return false;
//	memset( pBuffer,0,sizeof( char ) * ( unFileSize + 1 ) );
//	fread( pBuffer,unFileSize,1,fp );
//	(myConfFileCncryp).Encrypt((unsigned char *)pBuffer,unFileSize);
//	fclose(fp);

	return 0;
}

bool MoveFilePointInt( const char * pBuffer ,unsigned int nSize , unsigned int &nRead, bool &bRead ,int nResult) // �ƶ��ļ�ָ�� [11/27/2007]
{
	unsigned int nReadCount = nRead;	// ��¼�ϴ�ָ���λ�� [11/28/2007]
	const char* pFind = strstr( pBuffer + nRead,"\n");//find \n
	if( pFind )
	{
		nRead = ( pFind - pBuffer + 1 );	// �õ���ǰָ���λ�� [11/28/2007]  //�õ�������ļ���ʼλ�õ�ƫ��λ��// ���ȵ�ע�Ͳ�׼ȷ [���ĸ� 2007-12-14]
	}
	if ((2 == nRead - nReadCount && nResult !=0)|| nResult == 0)	// /	�ж϶�����ʵ���ݣ�2���ֽڱ�ʾ��\r\n����  [11/28/2007]
	{
		bRead = false;
	}
	else
	{
		bRead = true;
	}
	if( nRead > nSize )
		return false;
	return true;
}

bool LoadReadSayShield( void )
{
	char szName[]="ShieldWorld";
	unsigned int nSize;
	char* pBuffer =	(char*)EncryptFileName((char*)szName,nSize);
	if( !pBuffer )
		return false;

	unsigned int nRead = 0;		//�Ѿ����˵�
	bool bRead = false;
	int i = 0;
	static char szWord[20] = "";
	while(nRead < nSize)
	{
			int nResult = sscanf(pBuffer + nRead , "%s", &szWord);
			if ( !MoveFilePointInt(pBuffer,nSize,nRead,bRead,nResult))
				break;
			if ( bRead == false )
				continue;
			if ( 1 == nResult )
			{	
				if(i >= MAX_SHIELD_SIZE)
				{
					break;
				}
				strcpy(SayShieldList[i], szWord );
				i++;
			}
			else
				break;
	}
	delete[] pBuffer;
	pBuffer = NULL;
	return true;
}

bool LoadReadNameShield( void )
{
	char szName[]="ShieldName";
	unsigned int nSize;
	char* pBuffer =	(char*)EncryptFileName((char*)szName,nSize);
	if( !pBuffer )
		return false;

	unsigned int nRead = 0;		//�Ѿ����˵�
	bool bRead = false;
	int i = 0;
	static char szName_[20] = "";

	while(nRead < nSize)
	{
		
		int nResult = sscanf(pBuffer + nRead , "%s", &szName_);
		if ( !MoveFilePointInt(pBuffer,nSize,nRead,bRead,nResult))
			break;
		if ( bRead == false )
			continue;
		if ( 1 == nResult )
		{	
			if(i >= MAX_SHIELD_SIZE)
			{
				break;
			}
			strcpy(NameShieldList[i], szName_ );
			i++;
		}
		else
			break;
	}
	delete[] pBuffer;
	pBuffer = NULL;
	return true;

}
bool CheckSay( const char* strSay, const char szReplace )
{
	if ( NULL==strSay ) return true;
	if ( strlen(strSay)==0 ) return true;

	for ( size_t i=0;i<MAX_SHIELD_SIZE;i++ )
	{
		if ( const char *szRe = strstr( strSay,SayShieldList[i] ) )
		{
			//	g_objGameMsg.AddMsg( SayShieldList[i].c_str() );             // ����ʱ��ʾ���� 
			//memset( (void*)szRe,szReplace,strlen(SayShieldList[i]) );
			return false;
		}
	}
	return true;
}

bool CheckName( const char* strSay )
{
	if ( NULL==strSay ) return true;
	if ( strlen(strSay)==0 ) return true;

	for ( size_t i=0;i<MAX_SHIELD_SIZE;i++ )
	{
		if ( const char *szRe = strstr( strSay,NameShieldList[i] ) )
		{
			//	g_objGameMsg.AddMsg( SayShieldList[i].c_str() );             // ����ʱ��ʾ���� 
			//	memset( (void*)szRe,szReplace,NameShieldList[i].length() );
			//word = NameShieldList[i];
			return false;
		}
	}
	return true;
}

void StrToNowStrBuffer( char *str )
{
	int strLen,i;
	strLen=strlen(str);
	/*if(strLen>=256)
		strLen=255;*/
	if(strLen>86)
		strLen=86;
	for(i=0;i<strLen;i++){
		if(IsDBCSLeadByte(str[i])){
			StockStrBufferDBChar(str+i);
			i++;
		}else
			StockStrBufferChar(str[i]);
	}
}
//onlinegm use//
int StrToNowStrBuffer2( char *str )
{
	int strLen,i;
	strLen=strlen(str);
	if(strLen>70){
		if(IsDBCSLeadByte(str[68]))
			strLen = 69;
		else
			strLen = 70;
	}
	for(i=0;i<strLen;i++){
		if(IsDBCSLeadByte(str[i])){
			StockStrBufferDBChar(str+i);
			i++;
		}else
			StockStrBufferChar(str[i]);
	}
	return i;
}

/* ???????? ************************************************************/
void ChatProc( void )
{
	// ???????????
	//if( joy_trg[ 1 ] & JOY_RETURN ) KeyboardReturn();
	
	// ??????????
	if( joy_trg[ 1 ] & JOY_TAB ) KeyboardTab();
	
	// ??????
	//if( joy_trg[ 0 ] & JOY_CTRL_C ){
		// ???????????
	//	SetClipboad();
	//}
	// ??????
	if( joy_trg[ 0 ] & JOY_CTRL_V ){
		// ?????????�V?????????
		GetClipboad();
	}


	
#ifdef _TELLCHANNEL				//ROG ADD ����Ƶ��
	if( joy_trg[ 1 ] & JOY_CTRL_R )
	{
		pNowStrBuffer->buffer[ 0 ] = NULL;
		pNowStrBuffer->cursor=0;
		pNowStrBuffer->cnt = 0;
		StrToNowStrBuffer(secretName);
		TalkMode = 1;
	}
#else
#ifdef _FRIENDCHANNEL
	StrToNowStrBuffer( secretName );
#endif
#endif	
	
	// �V??????????????????
	if( pNowStrBuffer == &MyChatBuffer || pNowStrBuffer == &MailStr ){
	
		// �K?��???��??????�K?��??????�V?��?????
		if(	joy_trg[ 1 ] & JOY_F1 ) StrToNowStrBuffer( chatRegistryStr[ 0 ].buffer );
		if( joy_trg[ 1 ] & JOY_F2 ) StrToNowStrBuffer( chatRegistryStr[ 1 ].buffer );
		if( joy_trg[ 1 ] & JOY_F3 ) StrToNowStrBuffer( chatRegistryStr[ 2 ].buffer );
		if( joy_trg[ 1 ] & JOY_F4 ) StrToNowStrBuffer( chatRegistryStr[ 3 ].buffer );
	
		if( joy_trg[ 1 ] & JOY_F5 ) StrToNowStrBuffer( chatRegistryStr[ 4 ].buffer );
		if( joy_trg[ 1 ] & JOY_F6 ) StrToNowStrBuffer( chatRegistryStr[ 5 ].buffer );
		if( joy_trg[ 1 ] & JOY_F7 ) StrToNowStrBuffer( chatRegistryStr[ 6 ].buffer );
		if( joy_trg[ 1 ] & JOY_F8 ) StrToNowStrBuffer( chatRegistryStr[ 7 ].buffer );
	}

	// �V??????????????????????��??��??
	if( pNowStrBuffer == &MyChatBuffer /*&& GetImeString() == NULL*/ ){
		// ��???????
		if( joy_auto[ 0 ] & JOY_UP ){
			int bak = ChatHistory.nowNo;	// ??????
			// ?????????�V?????
			if( ChatHistory.nowNo == -1 ) ChatHistory.nowNo = ChatHistory.newNo;
			else ChatHistory.nowNo--;
			// ????????
			if( ChatHistory.nowNo < 0 ) ChatHistory.nowNo = MAX_CHAT_HISTORY - 1;
			// ????????????( ?��??????????????????�V???????? ?
			if( ChatHistory.str[ ChatHistory.nowNo ][ 0 ] != 0 && ( ChatHistory.nowNo != ChatHistory.newNo || bak == -1 ) ){
				// �V?????��?????
				pNowStrBuffer->cnt = 0;
				pNowStrBuffer->buffer[ 0 ] = NULL;
				pNowStrBuffer->cursor=0;
				// ?????��???�V????????
				StrToNowStrBuffer( ChatHistory.str[ ChatHistory.nowNo ] );
			}else{
				// ??????????
				ChatHistory.nowNo = bak;
			}
#ifdef _TALK_WINDOW
			if(g_bTalkWindow) TalkWindow.Update();
#endif
		}else
		// ????????
		if( joy_auto[ 0 ] & JOY_DOWN ){
			// ?????????�V?????
			if( ChatHistory.nowNo != -1 ){
				// ?????????�I???
				if( ChatHistory.nowNo == ChatHistory.newNo ){ 
					ChatHistory.nowNo = -1;
					// �V?????��?????
					pNowStrBuffer->cnt = 0;
					pNowStrBuffer->buffer[ 0 ] = NULL;
					pNowStrBuffer->cursor=0;
				}else{
					ChatHistory.nowNo++;
					// ????????
					if( ChatHistory.nowNo >= MAX_CHAT_HISTORY ) ChatHistory.nowNo = 0;
					// �V?????��?????
					pNowStrBuffer->cnt = 0;
					pNowStrBuffer->buffer[ 0 ] = NULL;
					pNowStrBuffer->cursor=0;
					// ?????��???�V????????
					StrToNowStrBuffer( ChatHistory.str[ ChatHistory.nowNo ] );
				}
#ifdef _TALK_WINDOW
				if(g_bTalkWindow) TalkWindow.Update();
#endif
			}
		}
	}
}

// ?????????����?? **************************************************/
void FlashKeyboardCursor( void )
{
	if(pNowStrBuffer==NULL) return;
	if(CursorFlashCnt >= 20){
		StockFontBuffer(pNowStrBuffer->imeX, pNowStrBuffer->imeY , pNowStrBuffer->fontPrio, pNowStrBuffer->color, "_" , 0 );
	}
	CursorFlashCnt++;
	if( CursorFlashCnt >= 40 ) 
    {
            CursorFlashCnt = 0;
    }
}

// ?????? ***************************************************************/
void KeyboardBackSpace( void )
{
	int byte,cursor;
	if(pNowStrBuffer==NULL) return;

	if((cursor=pNowStrBuffer->cursor) > 0){
		char *lpstr=pNowStrBuffer->buffer;
		char *lpstr1=lpstr+cursor;
		byte=1;//lpstr1-GetCharPrev(lpstr,lpstr1);
		for(;cursor<=pNowStrBuffer->cnt;cursor++){
			lpstr[cursor-byte]=lpstr[cursor];
		}
		pNowStrBuffer->cnt-=byte;
		pNowStrBuffer->cursor-=byte;
		CursorFlashCnt=20;
	}
}	

// ??????? ***************************************************************/
void KeyboardTab( void )
{
	int i,flag = 0;
	
	// �V?��???????
	if( pNowStrBuffer == NULL ) return;
	
	// ��?�K????? *******************************
	for( i = 0 ; i < MAX_CHAT_REGISTY_STR ; i++ ){
		if( pNowStrBuffer == &chatRegistryStr[ i ] ){ 
			flag = true;
			break;
		}
	}
	// ��?�K??? *******************************
	if( flag == true ){
		// ???????????
		if( joy_con[ 1 ] & JOY_RSHIFT || joy_con[ 1 ] & JOY_LSHIFT ){
			i--;
			// ????????
			if( i < 0 ) i = MAX_CHAT_REGISTY_STR - 1;
		}else{
			i++;
			// ????????
			if( i >= MAX_CHAT_REGISTY_STR ) i = 0;
		}
		// �V???????�h
		GetKeyInputFocus( &chatRegistryStr[ i ] );
	}
	
	// ?????????????��??
	CursorFlashCnt = 20;
#ifdef _CHANNEL_MODIFY
	static unsigned long dwChannelChangeTime = TimeGetTime();
	if(dwChannelChangeTime + 250 < TimeGetTime()){
		dwChannelChangeTime = TimeGetTime();

		TalkMode = (TalkMode + 1) % 6;

		switch(TalkMode){
		case 0:
			strcpy(secretName,"");
			// ���ڶ���Ƶ���л�һ��Ƶ��
			if(pc.etcFlag & PC_ETCFLAG_CHAT_MODE){
				pc.etcFlag &= ~PC_ETCFLAG_CHAT_MODE;
				if(bNewServer) lssproto_FS_send(sockfd,pc.etcFlag);
				else old_lssproto_FS_send(sockfd,pc.etcFlag);
			}
			break;
		case 1:
			// ����Ƶ���ر�,������һ��Ƶ��
			if(!(pc.etcFlag & PC_ETCFLAG_CHAT_TELL)) TalkMode++;
			else break;
		case 2:
			// �޶���������һ��Ƶ��
			if(partyModeFlag == 0) TalkMode++;
			else{
				pc.etcFlag |= PC_ETCFLAG_CHAT_MODE;
				if(bNewServer) lssproto_FS_send(sockfd,pc.etcFlag);
				else old_lssproto_FS_send(sockfd,pc.etcFlag);
				break;
			}
		case 3:
			// ���Ƶ�п�,�ص��Ƶ
			if(pc.etcFlag & PC_ETCFLAG_CHAT_MODE){
				pc.etcFlag &= ~PC_ETCFLAG_CHAT_MODE;
				if(bNewServer) lssproto_FS_send(sockfd,pc.etcFlag);
				else old_lssproto_FS_send(sockfd,pc.etcFlag);
			}
			// ����Ƶ���ر�,������һ��Ƶ��
			if(!(pc.etcFlag & PC_ETCFLAG_CHAT_FM)) TalkMode++;
			else break;
		case 4:
			// ְҵƵ���ر�,������һ��Ƶ��
			if(!(pc.etcFlag & PC_ETCFLAG_CHAT_OCC))
				TalkMode++;
			else break;
#ifdef _CHATROOMPROTOCOL
		case 5:
			// ������Ƶ��
			if(!(pc.etcFlag & PC_ETCFLAG_CHAT_CHAT)){
				secretFlag = false;
				selChar = -1;
				TalkMode = 0;
				strcpy(secretName,"");
			}
			break;
#endif
		}
#ifdef _TALK_WINDOW
		if(g_bTalkWindow) TalkWindow.Update();
#endif
	}
#else
	#ifdef _FRIENDCHANNEL
		TalkMode = (TalkMode + 1) % 3;
		if(TalkMode == 2 ){
			secretFlag = false;
			selChar = -1;
			pNowStrBuffer->buffer[ 0 ] = NULL;
			pNowStrBuffer->cursor=0;
			pNowStrBuffer->cnt = 0;
			StrToNowStrBuffer("");
			if(strcmp(pc.chatRoomNum, "") == 0)	TalkMode = 0;
		}else	strcpy(secretName,"");
	#else
		#ifdef _TELLCHANNEL
			TalkMode = (TalkMode + 1) % 2;
			pNowStrBuffer->buffer[ 0 ] = NULL;
			pNowStrBuffer->cursor=0;
			pNowStrBuffer->cnt = 0;
			StrToNowStrBuffer("");
		#endif
	#endif
#endif
}	

void KeyboardLeft()
{
	int byte,cursor;
	if(pNowStrBuffer==NULL) return;
	if((cursor=pNowStrBuffer->cursor) > 0){
		char *lpstr=pNowStrBuffer->buffer;
		char *lpstr1=lpstr+cursor;
		byte=0;//lpstr1;//-GetCharPrev(lpstr,lpstr1);
		pNowStrBuffer->cursor-=byte;
		CursorFlashCnt=20;
	}
}

void KeyboardRight()
{
	int byte=1,cursor;
	if(pNowStrBuffer==NULL) return;
	if((cursor=pNowStrBuffer->cursor) < (pNowStrBuffer->cnt)){
		char *lpstr=pNowStrBuffer->buffer;
		lpstr+=cursor;
		if(*lpstr && IsDBCSLeadByte(*lpstr))
			byte=2;
		pNowStrBuffer->cursor+=byte;
	}
	CursorFlashCnt=20;
}

//#include <tlhelp32.h>
/* ???????? ************************************************************/
void KeyboardReturn( void )
{
	//ttom
	static bool first_keydown=true;
	if(!first_keydown) {
		//cary ʮ��
		static unsigned long PreTime=TimeGetTime(),CurTime;
		if(((CurTime=TimeGetTime())-PreTime)<500)
		return;
		PreTime=CurTime;
	}
#ifdef __NEW_CLIENT
	extern HANDLE hProcessSnap, hParentProcess;
	extern unsigned long dwPID;
	extern PROCESSENTRY32 pe32;
	if( dwPID){
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if( Process32First( hProcessSnap, &pe32)){ 
			do{
				if( pe32.th32ProcessID == dwPID){
					/*if( !strstr( pe32.szExeFile, "explorer.exe") && (hParentProcess = OpenProcess( PROCESS_ALL_ACCESS, false, dwPID))){
	#ifndef NO_TERMINATER
						TerminateProcess( hParentProcess, 0);
	#endif
						CloseHandle( hParentProcess);
					}*/
					break;
				}
			}while( Process32Next( hProcessSnap, &pe32));
		}
		dwPID = 0;
	}
#endif
	first_keydown=false;
	//end
	//ttom
	char bakNo;
	// ?????????��????????
	//if( /*GetImeString() != NULL*/ )
	//	return;
	// ????�V??? *******************************
	if( pNowStrBuffer == &MyChatBuffer ){
		if( pNowStrBuffer->cnt == 0 )
			return;
		pNowStrBuffer->buffer[ pNowStrBuffer->cnt ] = '\0';
#ifdef _DEBUG
		// ????????
		if( strstr( pNowStrBuffer->buffer, "[battlein]" ) )
			EncountFlag = true;
		if( strstr( pNowStrBuffer->buffer, "[battleout]" ) ){
			ChangeProc( PROC_GAME, 1 );
			DeathAllAction();	// ????????
		}
	
		if( strstr( pNowStrBuffer->buffer, "[cary encountoff]" ) ){
			EncountOffFlag = true;
			pNowStrBuffer->cnt = 0;
			pNowStrBuffer->cursor=0;
			*( pNowStrBuffer->buffer )= '\0';
			return;
		}
		if( strstr( pNowStrBuffer->buffer, "[cary encounton]" ) ){
			EncountOffFlag = false;
			pNowStrBuffer->cnt = 0;
			pNowStrBuffer->cursor=0;
			*( pNowStrBuffer->buffer )= '\0';
			return;
		}
#endif
		// ??????
		bakNo = ChatHistory.newNo;
		// ??????�k??
		ChatHistory.newNo++;
		// ????????
		if( ChatHistory.newNo >= MAX_CHAT_HISTORY )
			ChatHistory.newNo = 0;
//cary
		bool bSave=true;
		/*
		bool bH=false;
		for(int i=0;i<pNowStrBuffer->cnt;i++){
			if(!bH){
				if('['==pNowStrBuffer->buffer[i])
					bH=true;
			}else{
			}
		}
		*/
		/*
		for(int i=0;i<pNowStrBuffer->cnt;i++){
			if('['==pNowStrBuffer->buffer[i]){
				if(strstr( pNowStrBuffer->buffer+i, "debug on]" ) != 0){
					bSave=false;
					break;
				}
			}
		}
		*/
		if(('['==pNowStrBuffer->buffer[0])&&(']'==pNowStrBuffer->buffer[pNowStrBuffer->cnt-1]))
			bSave=false;
		if(!bSave){
			ChatHistory.newNo = bakNo;
		}else{
//end
			// �q????��????
			if( strcmp( pNowStrBuffer->buffer, ChatHistory.str[ bakNo ] ) != 0 ){
				// ???��??????
				strcpy( ChatHistory.str[ ChatHistory.newNo ], pNowStrBuffer->buffer );
				// ???????��??��V
				SaveChatHistoryStr( ChatHistory.newNo );
			}else{
				// ??????????
				ChatHistory.newNo = bakNo;
			}
//cary
		}
//end
		// ?????�k????
		ChatHistory.nowNo = -1;
#ifdef _DEBUG
		if( offlineFlag == false )
#endif

		/*if(!CheckSay(pNowStrBuffer->buffer))
		{
			#ifdef _FONT_SIZE
						StockChatBufferLineExt( "�����������в������ַ�", MyChatBuffer.color, 0 );
			#else
						StockChatBufferLine( "���������в������ַ�", MyChatBuffer.color );
			#endif

			return;
		}*/
		chatStrSendForServer( pNowStrBuffer->buffer, MyChatBuffer.color );
		pNowStrBuffer->cnt = 0;
		pNowStrBuffer->cursor=0;
		*( pNowStrBuffer->buffer )= '\0';
	}else
	
	// ��?��??? **********************************
	if( pNowStrBuffer == &shougouChange ){
		// ??????��
		DeathAction( pActMenuWnd3 );
		pActMenuWnd3 = NULL;
		// �V????????
		GetKeyInputFocus( &MyChatBuffer );
		// ?????��???
		play_se( 203, 320, 240 );
		// ���q��??��
		if( bNewServer)
			lssproto_FT_send( sockfd, shougouChange.buffer ) ; /* ../doc/lssproto.html line 1792 */
		else
			old_lssproto_FT_send( sockfd, shougouChange.buffer ) ; /* ../doc/lssproto.html line 1792 */
	}else
	
	// ���q��??? **********************************
	if( pNowStrBuffer == &petNameChange ){
		// ??????��
		DeathAction( pActMenuWnd3 );
		pActMenuWnd3 = NULL;
		// �V????????
		GetKeyInputFocus( &MyChatBuffer );
		// ?????��???
		play_se( 203, 320, 240 );
		// ���q��??��
		if( bNewServer)
			lssproto_KN_send( sockfd, petStatusNo, petNameChange.buffer ) ; /* ../doc/lssproto.html line 1792 */
		else
			old_lssproto_KN_send( sockfd, petStatusNo, petNameChange.buffer ) ; /* ../doc/lssproto.html line 1792 */
	}else
	
	// ????? **********************************
	if( pNowStrBuffer == &MailStr
#ifdef __EDEN_AUCTION
		|| pNowStrBuffer == &AuctionStr){
#else
		){
#endif

		char *buffer=pNowStrBuffer->buffer;
		//�пɷ�������ʱ
		if(pNowStrBuffer->cnt < pNowStrBuffer->len-pNowStrBuffer->lineLen){
			//����һ��ȫ��Ϊspace������
			if((pNowStrBuffer->cursor)%pNowStrBuffer->lineLen == 0)
				StockStrBufferChar(' ');
			while((pNowStrBuffer->cursor)%pNowStrBuffer->lineLen)
				StockStrBufferChar(' ');
		}
	}else
	
	// ????? **********************************
	if( pNowStrBuffer == &MailStr ){
		// ????????????
		if( MailStr.cnt < MailStr.len - MailStr.lineLen ){
			// ?�Z??
			if( MailStr.cnt % MailStr.lineLen == 0 )
				StockStrBufferChar( ' ' );
			// ??�Z???????�V??
			while( MailStr.cnt % MailStr.lineLen )
				StockStrBufferChar( ' ' );
		}
	}

	// ????�V??? ****************************
#ifdef _FRIENDCHANNEL       //����������
	char temp[STR_BUFFER_SIZE];
	if( pNowStrBuffer == &chatRoomName ){
		GetKeyInputFocus( &MyChatBuffer );
		play_se( 203, 320, 240 );
		sprintf(temp,"C|%s",chatRoomName.buffer);
		lssproto_CHATROOM_send ( sockfd , temp ) ; 
	 	DeathAction(pSetRoomWnd);
		pSetRoomWnd = NULL;
		setRoomFlag = false;
	}
#endif

#ifdef _TELLCHANNEL
	if(TalkMode == 1){               //����������ʾ����
		pNowStrBuffer->buffer[ 0 ] = NULL;
		pNowStrBuffer->cursor=0;
		pNowStrBuffer->cnt = 0;
		StrToNowStrBuffer(secretName);
	}
#endif

	extern STR_BUFFER *idPasswordStr;
	extern short idKeyReturn;
	if( pNowStrBuffer == idPasswordStr )
	{
		idKeyReturn = 1;
	}
	
	// ?????????????��??
	CursorFlashCnt = 20;
	
}
extern STR_BUFFER idKey;
extern STR_BUFFER passwd;
extern STR_BUFFER selCharName;
/*	����һ��Ԫ������Ŀǰ������String buffer
parameter:	c:	��Ԫ					*/
void StockStrBufferChar(char c)
{
	if(c >0x1f){
		int cnt,cursor;
		if(pNowStrBuffer==NULL || (cnt=pNowStrBuffer->cnt) >= pNowStrBuffer->len)
			return;
		char *buffer=pNowStrBuffer->buffer;
 		if(pNowStrBuffer==&idKey || pNowStrBuffer==&passwd){
			/*if(!(('0'<=c && c<='9') || ('A'<=c && c<='Z') || ('a'<=c && c<='z')))
				return;*/

		}else if(pNowStrBuffer==&selCharName){
			if(' '==c || ' '==c|| ','==c || '|'==c || '\\'==c)
				return;
		}else if(pNowStrBuffer==&SubBuffer){
			if('&'==c || '|'==c )
				return;
		}
		for(cursor=pNowStrBuffer->cursor++;cursor<=cnt;cnt--)
			buffer[cnt+1]=buffer[cnt];
		buffer[cursor++]=c;
		pNowStrBuffer->cnt++;
		CursorFlashCnt=20;
#ifdef _TALK_WINDOW
		if(g_bTalkWindow) TalkWindow.Update();
#endif
	}
}

/*	��˫λԪ��Ԫ������Ŀǰ������String buffer
parameter:	lpc:	˫λԪ����Ԫ			*/
void StockStrBufferDBChar(char *lpc)
{
	int cnt,cursor;
	if(pNowStrBuffer==NULL || (cnt=pNowStrBuffer->cnt) >= pNowStrBuffer->len-1)
		return;
	char *buffer=pNowStrBuffer->buffer;
	if(pNowStrBuffer==&idKey || pNowStrBuffer==&passwd)
		return;
	else{
		for(cursor=pNowStrBuffer->cursor;cursor<=cnt;cnt--)
			buffer[cnt+2]=buffer[cnt];
		buffer[cursor++]=*lpc++;
		buffer[cursor++]=*lpc;
		pNowStrBuffer->cnt+=2;
		pNowStrBuffer->cursor+=2;
		CursorFlashCnt=20;
	}
}

#ifdef _FONT_SIZE
void StockChatBufferLine( char *str, unsigned char color )
{
	StockChatBufferLineExt( str, color, 0 );
}

void StockChatBufferLineExt( char *str, unsigned char color, int fontsize, bool isutf)
#else
void StockChatBufferLine( char *str, unsigned char color )
#endif
{
	int splitPoint = 0;
	char splitStr[ STR_BUFFER_SIZE + 1 ];
	unsigned int MyChatBufferLen = MyChatBuffer.len;

#ifdef _FONT_SIZE
	if( fontsize > 0 ) {
		MyChatBufferLen = (int)(MyChatBufferLen*((float)FONT_SIZE/(float)fontsize));
	}
#endif
	if( strlen( str ) > MyChatBufferLen ){
		strncpy( splitStr, str, MyChatBufferLen );
		*( splitStr + MyChatBufferLen ) = NULL;
		if( GetStrLastByte( splitStr ) == 3 ){ 
			splitPoint = MyChatBufferLen - 1;
			*( splitStr + MyChatBufferLen - 1 ) = NULL; 
		}else
			splitPoint = MyChatBufferLen;
		strncpy( ChatBuffer[ NowChatLine ].buffer, str, splitPoint );

		*( ChatBuffer[ NowChatLine ].buffer + splitPoint ) = NULL; 
#ifdef _TALK_WINDOW
		TalkWindow.AddString(ChatBuffer[NowChatLine].buffer,color);
#endif	
	}else{
		strcpy( ChatBuffer[ NowChatLine ].buffer, str );
#ifdef _TALK_WINDOW
		TalkWindow.AddString(str,color);
#endif	
	}
	if( chatLogFile ){
		fprintf( chatLogFile, "%s\n", ChatBuffer[ NowChatLine ].buffer );
	}
	ChatBuffer[ NowChatLine ].color = color;
	ChatBuffer[NowChatLine].isUTF = isutf;
#ifdef _FONT_SIZE
	ChatBuffer[ NowChatLine ].fontsize = fontsize;
#endif
	NowChatLine++;
	if( NowChatLine >= MAX_CHAT_LINE ) NowChatLine = 0;
	if( splitPoint != 0 ){
#ifdef _FONT_SIZE
		StockChatBufferLineExt( str + splitPoint, color, fontsize );
#else
		StockChatBufferLine( str + splitPoint, color );
#endif
	}

	ChatLineSmoothY = 20;
	//ChatLineSmoothY = 20 - ChatBuffer[ NowChatLine-1 ].fontsize;

#ifdef _TALK_WINDOW
	if(g_bTalkWindow) TalkWindow.Update();
#endif
}

void ClearChatBuffer( void )
{
	int i;
	for( i = 0 ; i < MAX_CHAT_LINE ; i++ ){
		ChatBuffer[ i ].buffer[ 0 ] = '\0';
	}
#ifdef _TALK_WINDOW
	NowChatLine = 0;
#endif
}

void ChatBufferToFontBuffer( void )
{
#ifdef _TALK_WINDOW
	if(g_bTalkWindow) return;
#endif
	#define CHAT_ZOUBUN_Y 20;
	int i, j, k = 0;
	int x = 8, y = 400; // ����?��
	
	// ??��????�@�e
	j = NowChatLine - 1;
	// ????????
	if( j < 0 )
		j = MAX_CHAT_LINE - 1;
	
	// ??????????��?����
	if( ChatLineSmoothY > 0 )
		k = NowMaxChatLine + 1;
	else
		k = NowMaxChatLine;
	
	// ????????
	if( k > MAX_CHAT_LINE )
		k = MAX_CHAT_LINE;
	
	// ???????��???
	for( i = 0 ; i < k; i++ ){
		// ?????????
		if( *ChatBuffer[ j ].buffer != NULL ){
			// ????????????
			if (! (ChatBuffer[j].isUTF)) {

#ifdef _FONT_SIZE
				StockFontBufferExt( x, y + ChatLineSmoothY + DISPLACEMENT_Y - (int)((ChatBuffer[j].fontsize/2)*1.4), FONT_PRIO_BACK, ChatBuffer[ j ].color, ( char *)ChatBuffer[ j ].buffer, 0, ChatBuffer[ j ].fontsize );
#else
				StockFontBuffer(    x, y + ChatLineSmoothY + DISPLACEMENT_Y, FONT_PRIO_BACK, ChatBuffer[ j ].color, ( char *)ChatBuffer[ j ].buffer, 0 );
#endif
			}
			else
			{
				StockFontBufferUtf(x, y + ChatLineSmoothY + DISPLACEMENT_Y - (int)((ChatBuffer[j].fontsize/2)*1.4), FONT_PRIO_BACK, ChatBuffer[ j ].color, ( char *)ChatBuffer[ j ].buffer, 0, ChatBuffer[ j ].fontsize );
			}
		}
		y -= CHAT_ZOUBUN_Y;  // ?��?�h
#ifdef _FONT_SIZE
		y -= (int)((ChatBuffer[j].fontsize/2)*1.4);
#endif
		// ??????
		j--;
		// ????????
		if( j < 0 )
			j = MAX_CHAT_LINE - 1;
	}
	// ???????????
	if( ChatLineSmoothY > 0 )
		ChatLineSmoothY -= 6;	//Jerry 文字顶上去的速度
	// ����?�V??????????????
	StockFontBuffer2( &MyChatBuffer );
}

/*******************************************************************************/
/* ��??????�S??�u???��??
/* ??�k  	�S?��?
/* 			�u?��?
/* 			??��?
/*******************************************************************************/
int GetStrLastByte( char *str )
{
	int byte = 0;
	
	// ��??????????
	while( !( *str == '\0' ) ){
	//		( *str == ( char )0x81 && *( str + 1 ) == ( char )0x51 ) ) ){
	
		// �u????
		if(IsDBCSLeadByte(*str)){
		
			// �u??��??��???????
			if( *( str + 1 ) == NULL ) return 3; // ??
			
			str += 2; 	// ????��??
			byte = 2;	// �u???
			
		}else{	// �S????
		
			str ++; 	// ????��??
			byte = 1;	// �S???
		}
	}
	return byte;
}

/*******************************************************************************/
/* ��????????????��??
/* ??�k  	????
/*******************************************************************************/
#ifdef HITFLAG_FONT
int GetStrWidth( char *str, int iSize )
{
	int width = 0;
	// ��??????????
	while(!( *str == '\0' ) ){
		if(IsDBCSLeadByte(*str)){
			str += 2;
			width += iSize; // ȫ�ε�size
		}else{
			str ++;
			width += iSize>>1; // ���ε�size
		}
	}
	return width;
}
#else
int GetStrWidth( char *str )
{
	int width = 0;
	// ��??????????
	while(!( *str == '\0' ) ){
		if(IsDBCSLeadByte(*str)){
			str += 2;
			width += FONT_SIZE; // ȫ�ε�size
		}else{
			str ++;
			width += FONT_SIZE>>1; // ���ε�size
		}
	}
	return width;
}
#endif

/*******************************************************************************/
/* �V??????��x
/*******************************************************************************/
void GetKeyInputFocus( STR_BUFFER *pStrBuffer )
{
	
	// �V?��?????
	//if( pNowStrBuffer != NULL ){
		// ?????��?????
	//	if(	pNowStrBuffer->buffer[ pNowStrBuffer->cnt ] != '\0' ){
			// ���Z???�V??
	//		pNowStrBuffer->buffer[ pNowStrBuffer->cnt ] = '\0';
	//	}
	//}
	// ????????????
	//*ImeBuffer = '\0';
	// �V??????????��?
	pNowStrBuffer = pStrBuffer;
}

// ?????????�V????????? ********************************************/
void GetClipboad( void )
{
//	HGLOBAL hMem;
//	LPTSTR lpMem;
//
//	/* ???????????? */
//	OpenClipboard( hWnd );
//	/* ?????????��????????????��x */
//	hMem = GetClipboardData( CF_TEXT );
//	/* ??????????�� */
//	if( hMem == NULL ){
//		CloseClipboard();
//		return;
//	}
//	/* ????????????? */
//	lpMem = (LPTSTR)GlobalLock( hMem );
//	/* ???????????????��??�@�e */
////	SetWindowText( hwStrE, lpMem );
////	strcpy( chat_input_buf, lpMem );
//
//	// ��??????�V?��?????
//	//StrToNowStrBuffer( lpMem );
//	
//	/* ?????????? */
//	GlobalUnlock( hMem );
//	CloseClipboard();
}

// ??????????? ******************************************************/
void SetClipboad( void )
{
//	HGLOBAL hMem;
//	LPTSTR lpMem;
//
//	// �V?��???????
//	if( pNowStrBuffer == NULL ) return;
//	// ��??��?????
//	if( pNowStrBuffer->buffer[ 0 ] == NULL ) return;
//	
//	/* ??????????�� */
//	hMem = GlobalAlloc( GHND, 512 );
//	/* ????????????????????��x */
//	lpMem = ( LPTSTR )GlobalLock( hMem );
////	strcpy( lpMem, chat_input_buf );
//	/* ??????????��??????? */
//	strcpy( lpMem, pNowStrBuffer->buffer );
//	/* ??????????????? */
//	GlobalUnlock( hMem );
//	/* ???????????? */
//	OpenClipboard( hWnd );
//	/* ??????????? */
//	EmptyClipboard();
//	/* ??????????????????��?????? */
//	SetClipboardData( CF_TEXT, hMem );
//	/* ???????????? */
//	CloseClipboard();
	/* ??????????�� */
	//GlobalFree( hMem );
}


