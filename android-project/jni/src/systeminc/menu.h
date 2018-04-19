/************************/
/*	menu.h				*/
/************************/
#ifndef _MENU_H_
#define _MENU_H_

#include "pc.h"
#include "chat.h"

#ifdef _TELLCHANNEL				//ROG ADD ����Ƶ��
#include "netproc.h"
#endif

// ??????��????�R�e??
#define MENU_SYSTEM_0	13
#define MENU_MAP_0		3

#define MAIL_STR_LEN 140		// ???��??��?
#define MAIL_DATE_STR_LEN 20	// �T��?��???��?
#define MAIL_MAX_HISTORY 20		// ?????
#define MAX_CHAT_REGISTY_STR		8		// ?????��?�K???

// ??????????�B�l
typedef struct{
	int 	x, y;			// ����?��
	int 	defX, defY;		// ??????��
	int 	bmpNo;			// ??�k?
	int 	dispPrio;		// ����?�I
	bool	dragFlag;		// ???????
	int	mixFlag;		// ?��???
}ITEM_BUFFER;

// ???????�B�l
typedef struct{
	char 	str[ MAIL_MAX_HISTORY ][ MAIL_STR_LEN + 1 ];			// ??????��??
	char 	dateStr[ MAIL_MAX_HISTORY ][ MAIL_DATE_STR_LEN + 1 ];	// �T��?��??
	int 	noReadFlag[ MAIL_MAX_HISTORY ];							// �t��????����??????????�k??
	int 	petLevel[ MAIL_MAX_HISTORY ];							// ����??????????�k?
	char 	petName[ MAIL_MAX_HISTORY ][ PET_NAME_LEN + 1 ];		// ????���q
	int 	itemGraNo[ MAIL_MAX_HISTORY ];							// ��?????????????�k?
	int 	newHistoryNo;											// ?��???�k?
}MAIL_HISTORY;

// ?????����??????��?�B�l
typedef struct{
	int sizeX,sizeY;	// ???
	int titleNo;		// ????�k?
	int titleX, titleY;	// ??????��
	int mx, my;			// ?��?��
	int nowX, nowY;		// ?��?????
	int cnt;			// ?��?��
	int wndType;		// ??????��?
	int hitFlag;		// ???�R�e??��?
	int yesDispNo;		// ??????�R�e�k?
	int noDispNo;		// ???????�R�e�k?
	int boxDispPrio;	// ?��?????����?�I??
	int yesNoResult;	// YesNo?????��???  ?��???
}WINDOW_DISP;

#if defined(__ALBUM_47)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND_45 553
#define MAX_PET_KIND_46 555
//#define MAX_PET_KIND    557
//#define MAX_PET_KIND    558 // Robin fix
//#define MAX_PET_KIND    564 // Robin fix
//#define MAX_PET_KIND    566 // Robin fix
#define MAX_PET_KIND    570 // Robin fix

#elif defined(__ALBUM_46)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND_45 553
#define MAX_PET_KIND    555

#elif defined(__ALBUM_45)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND_44 551
#define MAX_PET_KIND    553

#elif defined(__ALBUM_44)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND_43 549
#define MAX_PET_KIND    551

#elif defined(__ALBUM_43)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND_42 545
#define MAX_PET_KIND    549

#elif defined(__ALBUM_42)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND_41 543
#define MAX_PET_KIND    545

#elif defined(__ALBUM_41)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND_40 542
#define MAX_PET_KIND    543

#elif defined(__ALBUM_40)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND_39 540 
#define MAX_PET_KIND    542

#elif defined(__ALBUM_39)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND_38 521 
#define MAX_PET_KIND    540

#elif defined(__ALBUM_38)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND_37 498 
#define MAX_PET_KIND    521 

#elif defined(__ALBUM_37)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND_36 483 
#define MAX_PET_KIND    496 

#elif defined(__ALBUM_36)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND_35 462 
#define MAX_PET_KIND    483 
#elif defined(__ALBUM_35)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND_34 436
#define MAX_PET_KIND    462 //460

#elif defined(__ALBUM_34)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND_33 432
#define MAX_PET_KIND    436

#elif defined(__ALBUM_33)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND_32 431
#define MAX_PET_KIND    432

#elif defined(__ALBUM_32)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND_15 357
#define MAX_PET_KIND_16 364
#define MAX_PET_KIND_17 389
#define MAX_PET_KIND_18	391
#define MAX_PET_KIND_19 395
#define MAX_PET_KIND_20 398
#define MAX_PET_KIND_21 401
#define MAX_PET_KIND_22 405
#define MAX_PET_KIND_23 407
#define MAX_PET_KIND_24 414
#define MAX_PET_KIND_25 415
#define MAX_PET_KIND_26 420
#define MAX_PET_KIND_27 422
#define MAX_PET_KIND_28 423
#define MAX_PET_KIND_29 424
#define MAX_PET_KIND_30 425
#define MAX_PET_KIND_31 429
#define MAX_PET_KIND    431

#elif defined(__ALBUM_15)
#define MAX_PET_KIND_1 	124
#define MAX_PET_KIND_4 	224
#define MAX_PET_KIND_5 	228
#define MAX_PET_KIND_6 	229
#define MAX_PET_KIND_7 	230
#define MAX_PET_KIND_8 	237
#define MAX_PET_KIND_9 	240
#define MAX_PET_KIND_10 297 
#define MAX_PET_KIND_11	301
#define MAX_PET_KIND_12	305
#define MAX_PET_KIND_13	306
#define MAX_PET_KIND_14 353
#define MAX_PET_KIND    357
#else
#define MAX_PET_KIND 	124		// ????��?
#define MAX_PET_TBL 	181		// ???????????
#endif

#define MAX_PET_PAGE_SIZE 	8	// ?????������????????
#ifdef _PET_ITEM
#define MAX_PET_SPECIES		42	// ������������
#endif

// ????????????�h�k?
enum{
	ANIM_DISP_PET,				// ???����
	ANIM_DISP_PET_SEND,			// ???????��???��???�h
	ANIM_DISP_PET_RECIEVE,		// ??????����???����??�h
	
	ANIM_DISP_PET_MAIN,			// ?????�h�k��??
	ANIM_DISP_PET_WALK,			// ��?
	ANIM_DISP_PET_STAND,		// �r?
	ANIM_DISP_PET_ATTACK,		// ??
	ANIM_DISP_PET_GUARD,		// ???
	ANIM_DISP_PET_DAMAGE,		// ????
	ANIM_DISP_PET_DEAD,			// ?��
	ANIM_DISP_PET_ITEM,			// ???????����
	
	ANIM_DISP_LETTER_MOVE,		// ��??�h
	ANIM_DISP_LETTER,			// ��????????
	
	ANIM_DISP_MIX_PET_INIT,		// ?��??????
	ANIM_DISP_MIX_PET_MOVE,		// ?��???��?????�h
	ANIM_DISP_MIX_PET_MOJI,		// ?��????????????��??

#ifdef _AniCrossFrame	  // Syu ADD �������ι���������
	ANIM_DISP_CROSSFRAME = 18,
#endif
	ANIM_DISP_MIX_PET_END,		// ?��????��??
};

// ?????�B�l
typedef struct{
	char name[ PET_NAME_LEN + 1 ];	// ????���q
	char freeName[ PET_NAME_LEN + 1 ];	// ??????��
	int faceGraNo;					// ???????????�k?
	int level;						// ???
	int maxHp;						// ?��??
	int str;						// ???
	int quick;						// ��??
	int def;						// �\??
	int earth, water, fire, wind;	// ��???
	int flag;						// �K??????��?????? ?��?? ?��������???��?? ?���K??? ?
}PET_ALBUM;

// ????????????????�B�l
typedef struct{
	int albumNo;					// ?????????�k?
	char name[ PET_NAME_LEN + 1 ];	// ????���q
	int faceGraNo;					// ???????????�k?
}PET_ALBUM_TBL;


// ????????
extern bool TaskBarFlag;
// ??????
extern unsigned int MenuToggleFlag;
// ?????????
extern ITEM_BUFFER ItemBuffer[];
// ????????????
extern int StatusUpPoint;
// �ӡP???��???????��???
extern bool BattlePetReceiveFlag;
#ifdef _STANDBYPET		
extern bool StandbyPetSendFlag;
#endif
// ???����?????�k?
extern int BattlePetReceivePetNo; 
// ??????��??
extern MAIL_HISTORY MailHistory[];	
// ?????��?�K??�V??????
extern STR_BUFFER chatRegistryStr[];
// ????�V?��??
extern STR_BUFFER MailStr;		
// ??????????????
extern ACTION *pActMenuWnd3;
// ?????��?�K??�V??????
extern STR_BUFFER chatRegistryStr[];
// ??????��??��??
extern STR_BUFFER shougouChange;

#ifdef _MONEYINPUT //Syu ADD �ֶ������Ǯ��
extern STR_BUFFER MymoneyBuffer;
#endif

typedef enum{
	MAIL_WND_VIEW,
	MAIL_WND_SELECT,
	MAIL_WND_SEND,
	MAIL_WND_PET_SEND,
	MAIL_WND_ITEM,
	MAIL_WND_HISTORY,
}MAIL_WND_TYPE;

extern STR_BUFFER TradeBuffer;
// ????���q��??��??
extern STR_BUFFER petNameChange;
// Terry add 2002/01/04
#ifdef __EDEN_AUCTION
extern STR_BUFFER AuctionStr;		// ����������,50����
extern STR_BUFFER AuctionMoney;	// �����Ǯ���һ����
#endif
// Terry end
// ��???????����?????????�k?
extern int  petStatusNo;

extern int mailHistoryWndPageNo;	// ???????????�k?
extern int mailHistoryWndSelectNo;	// �k��?????����k?	
//extern int mailReceivePetGraNo;		// ??????����????????????�k?

// �P�f??���h????????����?????
extern int MapWmdFlagBak;

// �P�f????????��??????????
#define RESULT_WND_CLOSE_TIME 	300	// ?��
// �P�f????????��?????????
extern int ResultWndTimer;

// ?����?????
extern bool ItemMixRecvFlag;

// ???????
extern int MouseCursorFlag;							
#ifdef _TRADETALKWND				// (���ɿ�) Syu ADD ���������Ի����
void TradeTalk ( char *msg ) ; 
#endif

void InitItem( int x, int y , bool bPetItemFlag);

// ?????????? *******************************************************/
void InitMenu( void );

// ??????????? *******************************************************/
void InitMenu2( void );

// ????��??????? *******************************************************/
bool CheckMenuFlag( void );

// ?????? ***************************************************************/
void MenuProc( void );

// ?????����????�� ***************************************************/
ACTION *MakeWindowDisp( int x, int y, int sizeX, int sizeY, int titleNo, int wndType );

// ???�R�e?????�� ******************************************************/
bool MakeHitBox( int x1, int y1, int x2, int y2, int dispPrio );

// ???????��V ****************************************************************/
bool SaveMailHistory( int no );

// ???????��??? ****************************************************************/
bool LoadMailHistory( void );

// ???�t��???? ****************************************************************/
bool CheckMailNoReadFlag( void );

// ??????????�^���M?? **************************************************/
void InitOhtaParam( void );

// ��?????????�� **********************************************************/
void DeathLetterAction( void ); 

// ?????�K?��??��V ****************************************************************/
bool SaveChatRegistyStr( void );

// ?????�K?��??��??? ****************************************************************/
bool LoadChatRegistyStr( void );

// ��?????????? *****************************************************/
void CenteringStr( char *inStr, char *outStr, int max );

// ��????????? ****************************************************************/
void CheckNewPet( int sprNo );

// �P�f??��???????? **********************************************************/
void CheckBattleNewPet( void );

// ?????��??? ****************************************************************/
bool LoadAlbum( void );

// ?????��V ****************************************************************/
bool SaveAlbum( int no );
ACTION *MakeAnimDisp( int x, int y, int sprNo, int mode );

// Robin 04/16 trade
void tradeInit( void );

void BankmanInit( char* data );
void BankmanWN( void );
void closeBankman( void );
#endif

#ifdef _TELLCHANNEL				//ROG ADD ����Ƶ��
void InitSelectChar( char *msg, bool endFlag);
void SelectChar( void );
void DeathTellChannel( void );
extern int TalkMode;
extern char secretName[32];
#endif


#ifdef _FRIENDCHANNEL      //ROG ADD ����Ƶ��
#define MAX_ROOM_NUM  50
extern bool setRoomFlag;
extern bool secretFlag;				   //�������
extern int selChar;
extern ACTION *pChtChanlWnd;            //�������Ӵ�
extern ACTION *pSelChanlWnd;
void SelectChatRoom( void );
void InitSelChatRoom(char *msg);
void InitCreateChatRoom(char *msg);
void ChatRoomWnd(void);
void initAssentWnd(char *data);
void AssentWnd(void);
extern STR_BUFFER chatRoomName;            //���������� 
extern ACTION *pSetRoomWnd;
void setRoomName(void);					   //�趨����������

#endif


/////////////////////////////////////////
#ifdef _TIMEBAR_FUNCTION
int SetTimeBarPos(int timBarIdent, int CurPos);
int SetTimeBar(char *title, int range);
#endif

#ifdef _STANDBYPET
void send_StandBy_Pet( void);
#endif



