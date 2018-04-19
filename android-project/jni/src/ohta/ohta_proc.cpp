/************************/
/*	ohta_proc.cpp		*/
/************************/
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "ohta.h"
//#include "../caryIme.h"
//#include "../systeminc/ime_sa.h"
#include "../systeminc/map.h"
#include "../systeminc/menu.h"

void OhtaTestProc( void )
{
	switch( SubProcNo ){
		case 0:
			MakeJiki();
			SubProcNo++;
			InitChat();
			GetKeyInputFocus( &MyChatBuffer );
			break;
		
		case 1:
			FlashKeyboardCursor();
			if( mouse.level < DISP_PRIO_MENU && TaskBarFlag == false ){
				drawGrid();
				moveProc();
			}

			RunAction();
			StockTaskDispBuffer();
			drawMap();
			ChatProc();
			ChatBufferToFontBuffer(); 
			MenuProc();
			//ImeProc();		
			PaletteProc();
			break;
	}
}

