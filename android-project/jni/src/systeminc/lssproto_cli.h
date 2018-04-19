/* output by ./lsgen.perl 0.41 ( 1998 May)
 * made Mon Jun 12 11:17:33 2000
 * user kawata
 * host unicorn.lan.titan.co.jp
 * file /opt/kawata/sa/bin/output/lssproto_cli.h
 * util output/lssproto_util.c , output/lssproto_util.h
 * src  /opt/kawata/sa/bin/../doc/lssproto.html
 */
#ifndef _LSSPROTOCLI_H_
#define _LSSPROTOCLI_H_

#include "../NewProto/protocol.h"
/*#ifdef __NEW_PROTOCOL
#include "lssproto_util.h"

#ifdef MAXLSRPCARGS
#if ( MAXLSRPCARGS <= ( 13 + 1 ) )
#undef MAXLSRPCARGS
#define MAXLSRPCARGS ( 13 + 1 )
#endif
#else
#define MAXLSRPCARGS ( 13 + 1 )
#endif

int lssproto_InitClient( int(*)(int,char*,int)  , int bufsiz , int fd);
void lssproto_SetClientLogFiles( char *read , char *write);
void lssproto_CleanupClient(void);


#else
  */


#include "lssproto_util.h"

#ifdef MAXLSRPCARGS
#if ( MAXLSRPCARGS <= ( 13 + 1 ) )
#undef MAXLSRPCARGS
#define MAXLSRPCARGS ( 13 + 1 )
#endif
#else
#define MAXLSRPCARGS ( 13 + 1 )
#endif

void old_lssproto_W_send( int fd,int x,int y,char* direction ) ; /* ../doc/lssproto.html line 166 */
void old_lssproto_w_send( int fd,int x,int y,char* direction ) ; /* ../doc/lssproto.html line 198 */
void lssproto_XYD_recv( int fd,int x,int y,int dir ) ; /* ../doc/lssproto.html line 209 */
void old_lssproto_EV_send( int fd,int event,int seqno,int x,int y,int dir ) ; /* ../doc/lssproto.html line 212 */
void lssproto_EV_recv( int fd,int seqno,int result ) ; /* ../doc/lssproto.html line 247 */
void old_lssproto_EN_send( int fd,int x,int y ) ; /* ../doc/lssproto.html line 261 */
void old_lssproto_DU_send( int fd,int x,int y ) ; /* ../doc/lssproto.html line 273 */
void lssproto_EN_recv( int fd,int result,int field ) ; /* ../doc/lssproto.html line 285 */
void old_lssproto_EO_send( int fd,int dummy ) ; /* ../doc/lssproto.html line 298 */
void old_lssproto_BU_send( int fd,int dummy ) ; /* ../doc/lssproto.html line 309 */
void old_lssproto_JB_send( int fd,int x,int y ) ; /* ../doc/lssproto.html line 320 */
void old_lssproto_LB_send( int fd,int x,int y ) ; /* ../doc/lssproto.html line 331 */
void lssproto_RS_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 342 */
void lssproto_RD_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 349 */
void old_lssproto_B_send( int fd,char* command ) ; /* ../doc/lssproto.html line 354 */
void lssproto_B_recv( int fd,char* command ) ; /* ../doc/lssproto.html line 377 */
void old_lssproto_SKD_send( int fd,int dir,int index ) ; /* ../doc/lssproto.html line 542 */
void old_lssproto_ID_send( int fd,int x,int y,int haveitemindex,int toindex ) ; /* ../doc/lssproto.html line 633 */
void old_lssproto_PI_send( int fd,int x,int y,int dir ) ; /* ../doc/lssproto.html line 662 */
void old_lssproto_DI_send( int fd,int x,int y,int itemindex ) ; /* ../doc/lssproto.html line 676 */
void old_lssproto_DG_send( int fd,int x,int y,int amount ) ; /* ../doc/lssproto.html line 693 */
void old_lssproto_DP_send( int fd,int x,int y,int petindex ) ; /* ../doc/lssproto.html line 706 */
void lssproto_I_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 722 */
void old_lssproto_MI_send( int fd,int fromindex,int toindex ) ; /* ../doc/lssproto.html line 741 */
void lssproto_SI_recv( int fd,int fromindex,int toindex ) ; /* ../doc/lssproto.html line 763 */
void old_lssproto_MSG_send( int fd,int index,char* message,int color ) ; /* ../doc/lssproto.html line 788 */
void lssproto_MSG_recv( int fd,int aindex,char* text,int color ) ; /* ../doc/lssproto.html line 806 */
void old_lssproto_PMSG_send( int fd,int index,int petindex,int itemindex,char* message,int color ) ; /* ../doc/lssproto.html line 830 */
void lssproto_PME_recv( int fd,int objindex,int graphicsno,int x,int y,int dir,int flg,int no,char* cdata ) ; /* ../doc/lssproto.html line 851 */
void old_lssproto_AB_send( int fd ) ; /* ../doc/lssproto.html line 877 */
void lssproto_AB_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 882 */
void lssproto_ABI_recv( int fd,int num,char* data ) ; /* ../doc/lssproto.html line 913 */
void old_lssproto_DAB_send( int fd,int index ) ; /* ../doc/lssproto.html line 921 */
void old_lssproto_AAB_send( int fd,int x,int y ) ; /* ../doc/lssproto.html line 935 */
void old_lssproto_L_send( int fd,int dir ) ; /* ../doc/lssproto.html line 948 */
void old_lssproto_TK_send( int fd,int x,int y,char* message,int color,int area ) ; /* ../doc/lssproto.html line 962 */
void lssproto_TK_recv( int fd,int index,char* message,int color ) ; /* ../doc/lssproto.html line 1011 */
void lssproto_MC_recv( int fd,int fl,int x1,int y1,int x2,int y2,int tilesum,int objsum,int eventsum,char* data ) ; /* ../doc/lssproto.html line 1043 */
void old_lssproto_M_send( int fd,int fl,int x1,int y1,int x2,int y2 ) ; /* ../doc/lssproto.html line 1071 */
void lssproto_M_recv( int fd,int fl,int x1,int y1,int x2,int y2,char* data ) ; /* ../doc/lssproto.html line 1091 */
void old_lssproto_C_send( int fd,int index ) ; /* ../doc/lssproto.html line 1128 */
void lssproto_C_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 1135 */
void lssproto_CA_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 1253 */
void lssproto_CD_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 1305 */
void lssproto_R_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 1315 */
void old_lssproto_S_send( int fd,char* category ) ; /* ../doc/lssproto.html line 1345 */
void lssproto_S_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 1369 */
void lssproto_D_recv( int fd,int category,int dx,int dy,char* data ) ; /* ../doc/lssproto.html line 1715 */
void old_lssproto_FS_send( int fd,int flg ) ; /* ../doc/lssproto.html line 1745 */
void lssproto_FS_recv( int fd,int flg ) ; /* ../doc/lssproto.html line 1760 */
void old_lssproto_HL_send( int fd,int flg ) ; /* ../doc/lssproto.html line 1774 */
void lssproto_HL_recv( int fd,int flg ) ; /* ../doc/lssproto.html line 1785 */
void old_lssproto_PR_send( int fd,int x,int y,int request ) ; /* ../doc/lssproto.html line 1795 */
void lssproto_PR_recv( int fd,int request,int result ) ; /* ../doc/lssproto.html line 1808 */
void old_lssproto_KS_send( int fd,int petarray ) ; /* ../doc/lssproto.html line 1824 */
void lssproto_KS_recv( int fd,int petarray,int result ) ; /* ../doc/lssproto.html line 1835 */

void old_lssproto_AC_send( int fd,int x,int y,int actionno ) ; /* ../doc/lssproto.html line 1852 */
void old_lssproto_MU_send( int fd,int x,int y,int array,int toindex ) ; /* ../doc/lssproto.html line 1883 */
void old_lssproto_PS_send( int fd,int havepetindex,int havepetskill,int toindex,char* data ) ; /* ../doc/lssproto.html line 1907 */
void lssproto_PS_recv( int fd,int result,int havepetindex,int havepetskill,int toindex ) ; /* ../doc/lssproto.html line 1935 */
void old_lssproto_ST_send( int fd,int titleindex ) ; /* ../doc/lssproto.html line 1963 */
void old_lssproto_DT_send( int fd,int titleindex ) ; /* ../doc/lssproto.html line 1979 */
void old_lssproto_FT_send( int fd,char* data ) ; /* ../doc/lssproto.html line 1993 */
void lssproto_SKUP_recv( int fd,int point ) ; /* ../doc/lssproto.html line 2011 */
void old_lssproto_SKUP_send( int fd,int skillid ) ; /* ../doc/lssproto.html line 2019 */
void old_lssproto_KN_send( int fd,int havepetindex,char* data ) ; /* ../doc/lssproto.html line 2030 */
void lssproto_WN_recv( int fd,int windowtype,int buttontype,int seqno,int objindex,char* data ) ; /* ../doc/lssproto.html line 2047 */
void old_lssproto_WN_send( int fd,int x,int y,int seqno,int objindex,int select,char* data ) ; /* ../doc/lssproto.html line 2166 */
void lssproto_EF_recv( int fd,int effect,int level,char* option ) ; /* ../doc/lssproto.html line 2196 */
void lssproto_SE_recv( int fd,int x,int y,int senumber,int sw ) ; /* ../doc/lssproto.html line 2218 */
void old_lssproto_SP_send( int fd,int x,int y,int dir ) ; /* ../doc/lssproto.html line 2238 */
void old_lssproto_ClientLogin_send( int fd,char* cdkey,char* passwd ) ; /* ../doc/lssproto.html line 2253 */
void lssproto_ClientLogin_recv( int fd,char* result ) ; /* ../doc/lssproto.html line 2271 */
void old_lssproto_CreateNewChar_send( int fd,int dataplacenum,char* charname,int imgno,int faceimgno,int vital,int str,int tgh,int dex,int earth,int water,int fire,int wind,int hometown ) ; /* ../doc/lssproto.html line 2284 */
void lssproto_CreateNewChar_recv( int fd,char* result,char* data ) ; /* ../doc/lssproto.html line 2314 */
void old_lssproto_CharDelete_send( int fd,char* charname ) ; /* ../doc/lssproto.html line 2338 */
void lssproto_CharDelete_recv( int fd,char* result,char* data ) ; /* ../doc/lssproto.html line 2349 */
void old_lssproto_CharLogin_send( int fd,char* charname ) ; /* ../doc/lssproto.html line 2366 */
void lssproto_CharLogin_recv( int fd,char* result,char* data ) ; /* ../doc/lssproto.html line 2376 */
void old_lssproto_CharList_send( int fd ) ; /* ../doc/lssproto.html line 2391 */
void lssproto_CharList_recv( int fd,char* result,char* data ) ; /* ../doc/lssproto.html line 2396 */
void old_lssproto_CharLogout_send( int fd ) ; /* ../doc/lssproto.html line 2438 */
void lssproto_CharLogout_recv( int fd,char* result,char* data ) ; /* ../doc/lssproto.html line 2445 */
void old_lssproto_ProcGet_send( int fd ) ; /* ../doc/lssproto.html line 2459 */
void lssproto_ProcGet_recv( int fd,char* data ) ; /* ../doc/lssproto.html line 2465 */
void old_lssproto_PlayerNumGet_send( int fd ) ; /* ../doc/lssproto.html line 2477 */
void lssproto_PlayerNumGet_recv( int fd,int logincount,int player ) ; /* ../doc/lssproto.html line 2483 */
void old_lssproto_Echo_send( int fd,char* test ) ; /* ../doc/lssproto.html line 2495 */
void lssproto_Echo_recv( int fd,char* test ) ; /* ../doc/lssproto.html line 2506 */
void old_lssproto_Shutdown_send( int fd,char* passwd,int min ) ; /* ../doc/lssproto.html line 2517 */
int lssproto_InitClient( int(*)(int,char*,int)  , int bufsiz , int fd);
void lssproto_SetClientLogFiles( char *read , char *write);
void lssproto_CleanupClient(void);
int lssproto_ClientDispatchMessage(int fd ,char*line);
// Robin 2001/04/06
void lssproto_NU_recv( int fd, int AddCount ) ; /* ../doc/lssproto.html line  */
void old_lssproto_TD_send( int fd, char *data ) ;
void lssproto_TD_recv( int fd, char *data ) ;
// Robin 05/25
void old_lssproto_FM_send( int fd, char *data ) ;
void lssproto_FM_recv( int fd, char *data ) ;
#ifdef _FIX_DEL_MAP
void lssproto_DM_send(int fd);  // WON ADD Íæ¼Ò³éµØÍ¼ËÍ¼àÓü
#endif
//#endif

#endif
