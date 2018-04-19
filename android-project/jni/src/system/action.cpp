/************************/
/*	action.c			*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"

ACTION *pActTop;
ACTION *pActBtm;

#ifdef _DEBUG		
int ActCnt;
extern int g_iMallocCount;
#endif

void InitAction( void )
{
	/* ?????????? */
	pActTop = ( ACTION * )malloc( sizeof( ACTION ) );
	pActBtm = ( ACTION * )malloc( sizeof( ACTION ) );
#ifdef _DEBUG
	g_iMallocCount += 2;
#endif

	pActTop->pPrev = NULL;	 	/* ?????䥺? */
	pActTop->pNext = pActBtm;	/* ???????? */
	pActTop->func  = NULL;		/* ????䥺? */
	pActTop->prio  = PRIO_TOP;	/* ???? */
	pActTop->bmpNo = -1;		/* ??????? */
	
	pActBtm->pPrev = pActTop;	/* ????? */
	pActBtm->pNext = NULL;		/* ??????䥺? */
	pActBtm->func  = NULL;		/* ????䥺? */
	pActBtm->prio  = PRIO_BTM;	/* ???? */
	pActBtm->bmpNo = -1;		/* ??????? */
}

/* ?????????? *****************************************************/
ACTION *GetAction( unsigned char prio, unsigned int yobiSize )
{
	ACTION *pAct;
	ACTION *pActLoop;
	/* ?????????? */
	pAct = ( ACTION * )malloc( sizeof( ACTION ) );
    memset(pAct, 0, sizeof(ACTION));
#ifdef _DEBUG
	g_iMallocCount++;
#endif
	if( pAct == NULL ){
		//MessageBox( hWnd, "沒有足夠的硬盤空間！", "GetAction Erorr", MB_OK );
		return NULL;
	}
	/* ???? */
	if( yobiSize > 0 ){
		/* ?????? */
		pAct->pYobi = malloc( yobiSize );
        memset(pAct->pYobi, 0, yobiSize);
#ifdef _DEBUG
		g_iMallocCount++;
#endif
		// ???
		if( pAct->pYobi == NULL ){
			/* ???????????? */
			FREE( pAct );
#ifdef _DEBUG
			g_iMallocCount--;
#endif
			////MessageBox( hWnd, "沒有足夠的硬盤空間！", "GetYobi Erorr", MB_OK );
			return NULL;
		}
	}
    else
    {
        //pAct->pYobi = 0;
    }
	/* ??? */
	pAct->func  = NULL;			/* ????䥺? */
	pAct->prio  = prio;			/* ??? */
	pAct->bmpNo = -1;			/* ??????? */
	pAct->hitDispNo = -2;		/* ?????????????? */
	/* ????? */
	for( pActLoop = pActTop->pNext ; pActLoop != pActBtm->pNext ; pActLoop = pActLoop->pNext ){
		/* ???? */
		if( pActLoop->prio > prio ){
			/* ???? */
			/* ????????? */
			pAct->pPrev = pActLoop->pPrev;	/* ???? */
			pAct->pNext = pActLoop;			/* ????? */
			/* ????? */
			pActLoop->pPrev->pNext = pAct;	/* ???? */
			pActLoop->pPrev = pAct;			/* ????? */
#ifdef _DEBUG
			/* ?????????? */
   			ActCnt++;
#endif
			break;
		}
	}
    pAct->deathFlag = false;
	return pAct;
}

/* ????????? *********************************************************/
// 执行所有的动作，当deathFlag设定为true时，会造成此动作的释放

void RunAction( void )
{
	ACTION *pActLoop = pActTop->pNext; 	/* ???????? */
	ACTION *pActLoopBak;  /* ????????? */
	// ?????	
	while(1){
		/* ?????????? */	
		if( pActLoop == pActBtm ) break;
		/* ????? */
		if( pActLoop->deathFlag == false ){
			/* ?????????? */
			if( pActLoop->func != NULL ){
				pActLoop->func( pActLoop );
			}
			/* ?????????? */
			pActLoop = pActLoop->pNext;
		}else{	/* ?????? */
			/* ????? */
			pActLoop->pPrev->pNext = pActLoop->pNext;
			pActLoop->pNext->pPrev = pActLoop->pPrev;
            
			/* ?????? */
			pActLoopBak = pActLoop->pNext;
			/* ?????? */
			ClearAction( pActLoop );
			/* ???????? */
			pActLoop = pActLoopBak;
		}
	}
}

/* ???????????? ***********************************************/
void DeathAction( ACTION *pAct )
{
	if( pAct == NULL ) return;
	pAct->deathFlag = true;
}

/* ???????????? ***********************************************/
void ClearAction( ACTION *pAct )
{
	// ???????
	if( pAct->pYobi != NULL ){
		// ????
		//FREE( pAct->pYobi );
#ifdef _DEBUG
		g_iMallocCount--;
#endif
	}
	// ????????
	FREE( pAct );
	pAct = NULL;
#ifdef _DEBUG
	g_iMallocCount--;
#endif
#ifdef _DEBUG		
	/* ?????????? */
	ActCnt--;
#endif		

}
////#include <tlhelp32.h>

/* ?????? ***********************************************************/
void DeathAllAction( void )
{
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
	ACTION *pActLoop = pActTop->pNext;
	/* ???????? */
	while( pActLoop != pActBtm ){
		/* ??????? */
		pActLoop->deathFlag = true;
		/* ??????? */
		pActLoop = pActLoop->pNext;
	}
}

/* ???????? *********************************************************/
void EndAction( void )
{
	// ??????
	DeathAllAction();
	//?????????????
	RunAction();
	/* ??????????? */
	FREE( pActTop );
	FREE( pActBtm );
#ifdef _DEBUG
	g_iMallocCount -= 2;
#endif
}

