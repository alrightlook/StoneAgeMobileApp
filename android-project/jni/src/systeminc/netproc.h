#ifndef _NETPROC_H_
#define _NETPROC_H_

extern short selectServerIndex;
extern short clientLoginStatus;
extern short charListStatus;
extern short charDelStatus;

extern char gamestate_chooseserver_name[];
extern char gamestate_login_charname[];
extern char gamestate_deletechar_charname[];

extern char netprocErrmsg[];

extern int connectServer2Counter;
/*
#ifdef _TELLCHANNEL				// (不可开) ROG ADD 密语频道
extern char ReTellName[];
#endif
*/
void initConnectServer( void );
int ConnectWGS();
int connectServer( void );

void charListStart( void );
int  charListProc( void );
void charLoginStart( void );
int charLoginProc( void );

void createNewCharStart( void );
int createNewCharProc( void );

void charLogoutStart( void );
int charLogoutProc( void );

void walkSendForServer( int, int, char * );
void noChecksumWalkSendForServer( int, int, char * );

void chatStrSendForServer( char *, int );

void delCharStart( void );
int delCharProc( void );

#ifdef _PK2007
void pkListStart( void );
int  pkListProc( void );
#endif

#endif  /* ifndef _NETPROC_H_ */
