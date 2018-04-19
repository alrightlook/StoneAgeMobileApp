#ifndef _NETMAIN_H_
#define _NETMAIN_H_

#include "../systeminc/version.h"
#include "SDL_net.h"

struct gameserver
{
	char used;
	char ipaddr[127];
	char port[64];
	char name[64];

};

struct gamegroup{
	char	used;
	char	num;
	char	startindex;
	char	name[61];
};

#define SUCCESSFULSTR	"successful"        // ??????��?��?�e??
#define FAILEDSTR		"failed" 
#define OKSTR           "ok"
#define CANCLE          "cancle"

#define TIMEOUT (600*1000)                   //  ????????????????????????

//#define NETBUFSIZ (1024*16)
//#define NETBUFSIZ (1024*8)
#define NETBUFSIZ (1024*16) // Robin 2004.05.04

extern char *net_writebuf;
extern char *net_readbuf;
extern int net_readbuflen;
extern int net_writebuflen;

extern unsigned int sockfd;
extern int server_choosed;
extern int init_net;

extern struct gameserver gmsv[];

#ifdef _LOG_MSG
extern char debugLogFileName[];
#endif


extern bool disconnectServerFlag;
extern bool oldDisconnectServerFlag;


int getServerInfo( int, char *, short *, IPaddress*);


void networkLoop(void);
void networkMycharWalk( int gx , int gy , char *direction );

bool initNet(void);
void cleanupNetwork( void );
int appendReadBuf( char *buf , int size );
int appendWriteBuf( int index , char *buf , int size );
int shiftReadBuf(  int size );
int shiftWriteBuf( int size );
int getLineFromReadBuf( char *output  , int maxlen );
int sendn( unsigned int s , char *buffer , int len );



int getServerInfoByServerName( char *servername , char *hostname , short *port );


// ????????
#define NET_ERRMSG_SOCKLIBERROR		"��·��δ׼���á�"
#define NET_ERRMSG_BADNAME          "�ŷ������Ʋ���ȷ��"
#define NET_ERRMSG_SOCKETERROR      "�޷�ִ��socket��TCP��"
#define NET_ERRMSG_NOTGETADDR		"�޷�����ŷ����ģɣ�λ�á�(%s)"
#define NET_ERRMSG_NOTCONNECT_S		"�޷���ʼ�����ŷ��������ӡ�"
#define NET_ERRMSG_NOTCONNECT		"��������δ�����볢����������"
#define NET_ERRMSG_CONNECTTIMEOUT	"�ŷ�������ʱ���ѵ���"
#define NET_ERRMSG_LOGINTIMEOUT		"���봦��ʱ���ѵ���"
#define NET_ERRMSG_CHARLISTTIMEOUT	"��ȡ�����ʱ���ѵ���"
#define NET_ERRMSG_LOGOUTTIMEOUT	"�ǳ�����ʱ���ѵ���"
#define NET_ERRMSG_LOGINFAIL		"�޷������ŷ�����"
#define NET_ERRMSG_CREATECHARTIMEOUT	"��������ʱ���ѵ���"
#define NET_ERRMSG_DELETECHARTIMEOUT	"��������ʱ���ѵ���"
#define NET_ERRMSG_VERSIONERROR		"ʯ��ʱ���汾��ͬ"

#define GAMESTATE_ERRMSG_LOGOUTDENY	"�ŷ����ܾ�����ǳ���"


#endif
