#ifndef _WORK_H_
#define _WORK_H_

//#include <dinput.h>
#include	"vg410.h"

/*----------  ???  ----------*/
extern Uint32_ joy_con[2];
extern Uint32_ joy_trg[2];
extern Uint32_ joy_pul[2];
extern Uint32_ joy_buf[2];
extern Uint32_ joy_auto[2];
extern Uint16 joy_timer[2];
extern Uint16 s_timer;
extern unsigned char di_key[256];
#ifndef DI_2
//extern LPDIRECTINPUTDEVICE8 pDInputDevice;
//extern LPDIRECTINPUTDEVICE8 pDInputDevice2;
#else
//extern LPDIRECTINPUTDEVICE pDInputDevice;
//extern LPDIRECTINPUTDEVICE2 pDInputDevice2;
#endif
extern char joy_flg;
extern char keyboad_flg;
extern int slow_flg;
extern int flash_vct_no;
extern int quake_flg;
extern int quake_vct_no;
extern ACTION *p_party[];
extern int action_inf;
// 
extern bool DInputActiveFlag;	

#endif
