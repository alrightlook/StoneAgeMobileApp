#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/battleProc.h"
#include "../systeminc/produce.h"
#include "../systeminc/anim_tbl.h"
#include "../systeminc/loadrealbin.h"

// ?��??????
int ProduceInitFlag = true;
extern SDL_Surface* screenSurface;
extern SDL_Surface* battleSurface;
#ifdef _READ16BITBMP
extern bool g_bUseAlpha;
#endif
// ????????????�� **************************************************/
void CopyBackBuffer( void )
{
	// ??????????????????????
	//lpBattleSurface->BltFast( 0, 0, lpDraw->lpBACKBUFFER, NULL, DDBLTFAST_WAIT );
#ifdef _READ16BITBMP
	//if(g_bUseAlpha) lpBattleSurfaceSys->BltFast(0,0,lpDraw->lpBACKBUFFERSYS,NULL,DDBLTFAST_WAIT);
#endif
}	

//Syu Mark δʹ��
//�����ƶ�
bool ProduceAccele( int mode )
{
	SDL_Rect rect = { 0, 0, DEF_APPSIZEX, DEF_APPSIZEY };
	int endFlag = false;
	static float x = 0, x2 = 0, y = 0, y2 = 0; 
	static float a = 0;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		x = 0; x2 = 0; y = 0;a = 0;
	}
	// ?�h��??��?
	switch( mode ){
	case 0:	// ��?�L?�h
		y -= a;	// ?�h
		// ?�L�T???
		a += 0.3F; 
		// ????????
		if( y <= -480 ) endFlag = true;
		break;
	case 1:	// ??�L?�h
		y += a;	// ?�h
		// ?�L�T???
		a += 0.3F; 
		// ????????
		if( y >= 480 ) endFlag = true;
		break;
	case 2:	// ??�L?�h
		x -= a;	// ?�h
		// ?�L�T???
		a += 0.4F; 
		// ????????
		if( x <= -640 ) endFlag = true;
		break;
	case 3:	// ??�L?�h
		x += a;	// ?�h
		// ?�L�T???
		a += 0.4F; 
		// ????????
		if( x >= 640 ) endFlag = true;
		break;
	case 4:	// ???�L?�h
		x += a;		// ?�h
		x2 -= a;	// ?�h
		// ?�L�T???
		a += 0.4F; 
		// ????????
		if( x >= 640 ) endFlag = true;
		break;
	case 5:	// ��??�L?�h
		y += a;	// ?�h
		y2 -= a;	// ?�h
		// ?�L�T???
		a += 0.3F; 
		// ????????
		if( y >= 480 ) endFlag = true;
		break;
	}
	// ???????????????
//	ClearBackSurface();	
	// ??��???
	if( mode == 4 || mode == 5 ){
		int i;
		// ??��???
		for( i = 0 ; i < 480 ; i++ ){
			rect.y = i;
			rect.h = 1;
			rect.x = 0;
			rect.w = DEF_APPSIZEX;
			// ???????
			if( mode == 4 ){
				// ????
				if( i & 1 ){
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( (int)x, i, &rect, lpBattleSurface );
				}else{
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( (int)x2, i, &rect, lpBattleSurface );
				}
			}else{
				// ????
				if( i & 1 ){
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( 0, i + (int)y, &rect, lpBattleSurface );
				}else{
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( 0, i + (int)y2, &rect, lpBattleSurface );
				}
			}
		}
	}else{
		// ??????????�L��??��?���e?
		//DrawSurfaceFast2( (int)x, (int)y, &rect, lpBattleSurface );
	}
	if( endFlag == true ){
		// ???
		x = 0;
		x2 = 0;
		y = 0; 
		y2 = 0; 
		a = 0;
		// ????�֢t��?
		if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
		else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
		return true;
	}
	return false;
}	
	

//Syu Mark δʹ��
//�����ƶ�
bool ProduceBrake( int mode )
{
	SDL_Rect rect = { 0, 0, DEF_APPSIZEX, DEF_APPSIZEY };
	static int endFlag = -1;
	static float x = 0,x2 = 0, y = 0, y2 = 0; 
	static float a;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		endFlag = -1;
	}
	// ?�h��??��?
	switch( mode ){
	case 0:	// ��?�L?�h
		if( endFlag == -1 ){
			x = 0;
			y = 513.2F;
			a = 17.4F;
			endFlag = false;
		}
		y -= a;	// ?�h
		// ?�L�T???
		a -= 0.3F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	case 1:	// ??�L?�h
		if( endFlag == -1 ){
			x = 0;
			y = -513.2F;
			a = 17.4F;
			endFlag = false;
		}
		y += a;	// ?�h
		// ?�L�T???
		a -= 0.3F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	case 2:	// ??�L?�h
		if( endFlag == -1 ){
			x = 661;
			y = 0;
			a = 22.8F;
			endFlag = false;
		}
		x -= a;	// ?�h
		// ?�L�T???
		a -= 0.4F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	case 3:	// ??�L?�h
		if( endFlag == -1 ){
			x = -661;
			y = 0;
			a = 22.8F;
			endFlag = false;
		}
		x += a;	// ?�h
		// ?�L�T???
		a -= 0.4F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	case 4:	// ???�L?�h
		if( endFlag == -1 ){
			x = -661;
			x2 = 661;
			y = 0;
			a = 22.8F;
			endFlag = false;
		}
		x += a;		// ?�h
		x2 -= a;	// ?�h
		// ?�L�T???
		a -= 0.4F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	case 5:	// ??�L?�h
		if( endFlag == -1 ){
			x = 0;
			y = -513.2F;
			y2 = 513.2F;
			a = 17.4F;
			endFlag = false;
		}
		y += a;	// ?�h
		y2 -= a;	// ?�h
		// ?�L�T???
		a -= 0.3F; 
		// ????????
		if( a <= 0 ) endFlag = true;
		break;
	}
	// ???????????????
//	ClearBackSurface();	
	// ??��???
	if( mode == 4 || mode == 5 ){
		int i;
		// ??��???
		for( i = 0 ; i < 480 ; i++ ){
			rect.y = i;
			rect.h = 1;
			rect.x = 0;
			rect.w = DEF_APPSIZEX;
			// ??????
			if( mode == 4 ){
				// ????
				if( i & 1 ){
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( (int)x, i, &rect, lpBattleSurface );
				}else{
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( (int)x2, i, &rect, lpBattleSurface );
				}
			}else{
				// ????
				if( i & 1 ){
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( 0, i + (int)y, &rect, lpBattleSurface );
				}else{
					// ??????????�L��??��?���e?
					//DrawSurfaceFast2( 0, i + (int)y2, &rect, lpBattleSurface );
				}
			}
		}
		
	}else{
		// ??????????�L��??��?���e?
		//DrawSurfaceFast2( (int)x, (int)y, &rect, lpBattleSurface );
	}
	if( endFlag == true ){
		// ????�֢t��?
		if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
		else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
		// ??????
		endFlag = -1;
		return true;
	}
	return false;
}

//�ķ����ƶ�
bool Produce4Way( int mode )
{
	SDL_Rect rect[ 4 ] = {	
		{ 0 , 0 , 320 + (DISPLACEMENT_X >> 1) , 240 + (DISPLACEMENT_Y >> 1) }, 
		{ 320 + (DISPLACEMENT_X >> 1) , 0 , 640 + DISPLACEMENT_X , 240 + (DISPLACEMENT_Y >> 1) },
		{ 0 , 240 + (DISPLACEMENT_Y >> 1) , 320 + (DISPLACEMENT_X >> 1) , 480 + DISPLACEMENT_Y }, 
		{ 320 + (DISPLACEMENT_X >> 1) , 240 + (DISPLACEMENT_Y >> 1) , 640 + DISPLACEMENT_X , 480 + DISPLACEMENT_Y } };

        rect[0].w = rect[0].w - rect[0].x;
        rect[1].w = rect[1].w - rect[1].x;
        rect[2].w = rect[2].w - rect[2].x;
        rect[3].w = rect[3].w - rect[3].x;

        rect[0].h = rect[0].h - rect[0].y;
        rect[1].h = rect[1].h - rect[1].y;
        rect[2].h = rect[2].h - rect[2].y;
        rect[3].h = rect[3].h - rect[3].y;


	static int endFlag = -1;
	static int x[ 4 ], y[ 4 ]; 
	int d = 4,i;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		endFlag = -1;
	}
	// ?�h��??��?
	switch( mode ){
	case 0:	// ?��??
		if( endFlag == -1 ){
			x[ 0 ] = 0;
			y[ 0 ] = 0;
			x[ 1 ] = 320 + (DISPLACEMENT_X >> 1);
			y[ 1 ] = 0;
			x[ 2 ] = 0;
			y[ 2 ] = 240 + (DISPLACEMENT_Y >> 1);
			x[ 3 ] = 320 + (DISPLACEMENT_X >> 1);
			y[ 3 ] = 240 + (DISPLACEMENT_Y >> 1);
			endFlag = false;
		}
		// ?�h
		x[ 0 ] -= d;
		y[ 0 ] -= d;
		x[ 1 ] += d;
		y[ 1 ] -= d;
		x[ 2 ] -= d;
		y[ 2 ] += d;
		x[ 3 ] += d;
		y[ 3 ] += d;
		// ????????
		if( x[ 0 ] <= -320 - (DISPLACEMENT_X >> 1) ) endFlag = true;
		break;
	case 1:	// ?����?
		if( endFlag == -1 ){
			x[ 0 ] = -280;
			y[ 0 ] = -280;
			x[ 1 ] = 320 + 280 + (DISPLACEMENT_X >> 1);
			y[ 1 ] = -280;
			x[ 2 ] = -280;
			y[ 2 ] = 240 + 280 + (DISPLACEMENT_Y >> 1);
			x[ 3 ] = 320 + 280 + (DISPLACEMENT_X >> 1);
			y[ 3 ] = 240 + 280 + (DISPLACEMENT_Y >> 1);
			endFlag = false;
		}
		// ?�h
		x[ 0 ] += d;
		y[ 0 ] += d;
		x[ 1 ] -= d;
		y[ 1 ] += d;
		x[ 2 ] += d;
		y[ 2 ] -= d;
		x[ 3 ] -= d;
		y[ 3 ] -= d;
		// ????????
		if( x[ 0 ] >= 0 ) endFlag = true;
		break;
	}
	// ???????????????
//	ClearBackSurface();	
	for( i = 0 ; i < 4 ; i++ ){
		//???????????????
		if(ResoMode == 1){
			rect[i].x >>= 1;
			rect[i].y >>= 1;
			rect[i].w >>= 1;
			rect[i].h >>= 1;
            DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect[ i ], battleSurface );
			// ??????????�L��??��?���e?
			//DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect[ i ], lpBattleSurface );
		} else {
			// ??????????�L��??��?���e?
            DrawSurfaceFast2( x[ i ], y[ i ], &rect[ i ], battleSurface );
			//DrawSurfaceFast2( x[ i ], y[ i ], &rect[ i ], lpBattleSurface );
		}
	}
	if( endFlag == true ){
		// ????�֢t��?
		if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
		else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
		// ??????
		endFlag = -1;
		return true;
	}
	return false;
}	
#ifdef _NEW_RESOMODE

//����ս������
bool ProduceHagare( int mode )
{
	//�����иλ��С
	//1024 * 768 
	//int CutX = 128 ; 
	//int CutY = 96 ; 
	// 800 * 600 
	int CutX = 100 ; 
	int CutY = 75 ; 

	RECT rect;
	static int endFlag = -1;
	static int x[ 64 ], y[ 64 ], a[ 64 ], cnt,cnt2; 
	static int posX[ 64 ], posY[ 64 ]; 
	static bool flag[ 64 ]; 
	int i, j, nowX = 0, nowY = 0;
	int rnd = Rnd( 0, 63 );
	int no = 0;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		endFlag = -1;
	}
	// ?�h��??��?
	switch( mode ){
	case 0:	// ��??
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = true;
					nowX += CutX;
				}
				nowY += CutY;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
		}
		while( flag[ rnd ] == false ){
			rnd = Rnd( 0, 63 );
		}
		// ????��?
		flag[ rnd ] = false;
		cnt++;
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 1:	// ��???
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = false;
					nowX += CutX;
				}
				nowY += CutY;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
		}
		while( flag[ rnd ] == true ){
			rnd = Rnd( 0, 63 );
		}
		// ????��?
		flag[ rnd ] = true;
		cnt++;
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 2:	// ��???????
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = 2;
					nowX += CutX;
				}
				nowY += CutY;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
			cnt2 = 0;
		}
		// ?????????
		if( cnt2 < 64 ){
			while( flag[ rnd ] <= 1 ){
				rnd = Rnd( 0, 63 );
			}
			// ???????
			flag[ rnd ] = 1;
			cnt2++;
		}
		// ?�h
		for( i = 0 ; i < 64 ; i++ ){
			if( flag[ i ] == 1 ){
				a[ i ]++;
				y[ i ] += a[ i ];
				if( y[ i ] >= 800 ){ 
					flag[ i ] = 0;
					cnt++;
				}
			}
		}
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 3:	// ��??????
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = -CutY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = false;
					nowX += CutX;
				}
				nowY += CutY;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
			cnt2 = 0;
		}
		// ?????????
		if( cnt2 < 64 ){
			while( flag[ rnd ] >= 1 ){
				rnd = Rnd( 0, 63 );
			}
			// ???????
			flag[ rnd ] = 1;
			cnt2++;
		}
		// ?�h
		for( i = 0 ; i < 64 ; i++ ){
			if( flag[ i ] == 1 ){
				a[ i ]++;
				y[ i ] += a[ i ];
				if( y[ i ] >= posY[ i ] ){ 
					y[ i ] = posY[ i ];
					flag[ i ] = 2;
					cnt++;
				}
			}
		}
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	}
	// ???????????????
//	ClearBackSurface();	
	for( i = 0 ; i < 64 ; i++ ){
		if( flag[ i ] == 2 ){
			rect.left = posX[ i ];
			rect.top = posY[ i ];
			rect.right = posX[ i ] + CutX;
			rect.bottom = posY[ i ] + CutY;
			//???????????????
			if(ResoMode == 1){
				rect.top >>= 1;
				rect.left >>= 1;
				rect.right >>= 1;
				rect.bottom >>= 1;
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect, lpBattleSurface );
			} else {
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ], y[ i ], &rect, lpBattleSurface );
			}
		}
	}
	for( i = 0 ; i < 64 ; i++ ){
		if( flag[ i ] == 1 ){
			rect.left = posX[ i ];
			rect.top = posY[ i ];
			rect.right = posX[ i ] + CutX;
			rect.bottom = posY[ i ] + CutY;
			//???????????????
			if(ResoMode == 1){
				rect.top >>= 1;
				rect.left >>= 1;
				rect.right >>= 1;
				rect.bottom >>= 1;
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect, lpBattleSurface );
			} else {
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ], y[ i ], &rect, lpBattleSurface );
			}
		}
	}
	if( endFlag == true ){
		// ????�֢t��?
		if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
		else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
		// ??????
		endFlag = -1;
		return true;
	}
	return false;
}
#else
//����ս������
//3 ����ս��
//2 �뿪ս��
bool ProduceHagare( int mode )
{
	SDL_Rect rect;
	static int endFlag = -1;
	static int x[ 64 ], y[ 64 ], a[ 64 ], cnt,cnt2; 
	static int posX[ 64 ], posY[ 64 ]; 
	static int flag[ 64 ]; 
	int i, j, nowX = 0, nowY = 0;
	int rnd = Rnd( 0, 63 );
	int no = 0;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		endFlag = -1;
	}
	// ?�h��??��?
	switch( mode ){
	case 0:	// ��??
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = true;
					nowX += 80;
				}
				nowY += 60;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
		}
		while( flag[ rnd ] == false ){
			rnd = Rnd( 0, 63 );
		}
		// ????��?
		flag[ rnd ] = false;
		cnt++;
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 1:	// ��???
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = false;
					nowX += 80;
				}
				nowY += 60;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
		}
		while( flag[ rnd ] == true ){
			rnd = Rnd( 0, 63 );
		}
		// ????��?
		flag[ rnd ] = true;
		cnt++;
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 2:	// ��???????
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = nowY;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = 2;
					nowX += 80;
				}
				nowY += 60;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
			cnt2 = 0;
		}
		// ?????????
		if( cnt2 < 64 ){
			while( flag[ rnd ] <= 1 ){
				rnd = Rnd( 0, 63 );
			}
			// ???????
			flag[ rnd ] = 1;
			cnt2++;
		}
		// ?�h
		for( i = 0 ; i < 64 ; i++ ){
			if( flag[ i ] == 1 ){
				a[ i ]++;
				y[ i ] += a[ i ];
				if( y[ i ] >= 500 ){ 
					flag[ i ] = 0;
					cnt++;
				}
			}
		}
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	case 3:	// ��??????
		// ???
		if( endFlag == -1 ){
			for( i = 0 ; i < 8 ; i++ ){
				for( j = 0 ; j < 8 ; j++ ){
					x[ no ] = nowX;
					y[ no ] = -60;
					posX[ no ] = nowX;
					posY[ no ] = nowY;
					a[ no ] = 0;
					flag[ no++ ] = false;
					nowX += 80;
				}
				nowY += 60;
				nowX = 0;
			}
			endFlag = false;
			cnt = 0;
			cnt2 = 0;
		}
		// ?????????
		if( cnt2 < 64 ){
			while( flag[ rnd ] >= 1 ){
				rnd = Rnd( 0, 63 );
			}
			// ???????
			flag[ rnd ] = 1;
			cnt2++;
		}
		// ?�h
		for( i = 0 ; i < 64 ; i++ ){
			if( flag[ i ] == 1 ){
				a[ i ]++;
				y[ i ] += a[ i ];
				if( y[ i ] >= posY[ i ] ){ 
					y[ i ] = posY[ i ];
					flag[ i ] = 2;
					cnt++;
				}
			}
		}
		// ????????
		if( cnt >= 64 ) endFlag = true;
		break;
	}
	// ???????????????
//	ClearBackSurface();	
	for( i = 0 ; i < 64 ; i++ ){
		if( flag[ i ] == 2 ){
			rect.x = posX[ i ];
			rect.y = posY[ i ];
			rect.w = posX[ i ] + 80 - rect.x;
			rect.h = posY[ i ] + 60 - rect.y;
			//???????????????
			if(ResoMode == 1){
				rect.x >>= 1;
				rect.y >>= 1;
				rect.w >>= 1;
				rect.h >>= 1;
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect, battleSurface );
			} else {
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ], y[ i ], &rect, battleSurface );
			}
		}
	}
	for( i = 0 ; i < 64 ; i++ ){
		if( flag[ i ] == 1 ){
			rect.x = posX[ i ];
			rect.y = posY[ i ];
            rect.w = posX[ i ] + 80 - rect.x;
			rect.h = posY[ i ] + 60 - rect.y;
			//???????????????
			if(ResoMode == 1){
				rect.x >>= 1;
				rect.y >>= 1;
				rect.w >>= 1;
				rect.h >>= 1;
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ] / 2, y[ i ] / 2, &rect, battleSurface );
			} else {
				// ??????????�L��??��?���e?
				DrawSurfaceFast2( x[ i ], y[ i ], &rect, battleSurface );
			}
		}
	}
	if( endFlag == true ){
		// ????�֢t��?
		if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
		else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
		// ??????
		endFlag = -1;
		return true;
	}
	return false;
}
#endif	

#ifdef _NEW_RESOMODE
//�軻�ɿ������������ȵ�X , Y ֵ
//1024 * 768 
//#define BRAN_SIZE_X 32
//#define BRAN_SIZE_Y 16
//800 * 600 
#define BRAN_SIZE_X 20
#define BRAN_SIZE_Y 15
#else
#define BRAN_SIZE_X 20
#define BRAN_SIZE_Y 16
#endif
static int bran_cnt;
static int pos_tbl[( ( 480 + DISPLACEMENT_Y ) /BRAN_SIZE_Y)*( ( 640+ DISPLACEMENT_X )/BRAN_SIZE_X)*2];
static int bran_flg = 0;
//static int bran_flg = 0;

//�ɻ����������ķ������෴
bool Produce_bran_small(int ang)
{
	SDL_Rect rect;
	int d0,d6,d7;
	int *a0,x,y;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		bran_flg = 0;
	}
	if(bran_flg == 0){		//????
		bran_flg = 1;		//?????��
		d0 = 0;
		for( d7= -240 - (DISPLACEMENT_Y >> 1) + (BRAN_SIZE_Y >> 1); d7<240 + (DISPLACEMENT_Y >> 1) + (BRAN_SIZE_Y >> 1); d7+=BRAN_SIZE_Y){
			for(d6=-320 - (DISPLACEMENT_X >> 1) + (BRAN_SIZE_X >> 1); d6<320 + (DISPLACEMENT_X >> 1) + (BRAN_SIZE_X >> 1); d6+=BRAN_SIZE_X){
				pos_tbl[d0++] = d6;
				pos_tbl[d0++] = d7;
			}
		}
		bran_cnt = ang < 0 ? 64:0;
	}
	// ???????????????
//	ClearBackSurface();	
	a0 = pos_tbl + (((DISPLACEMENT_Y + 480) / BRAN_SIZE_Y) * ((DISPLACEMENT_X + 640) / BRAN_SIZE_X) << 1) - 2;
	for(d7=480 + DISPLACEMENT_Y -BRAN_SIZE_Y; d7>=0; d7-=BRAN_SIZE_Y){
		for(d6=640 + DISPLACEMENT_X-BRAN_SIZE_X; d6>=0; d6-=BRAN_SIZE_X){
			//???????????????
			if(ResoMode == 1){
				rect.x = d6 >> 1;
				rect.w = (d6+BRAN_SIZE_X) >> 1 - rect.x;
				rect.y = d7 >> 1;
				rect.h = (d7+BRAN_SIZE_Y) >> 1 - rect.y;
			} else {
				rect.x = d6;
				rect.w = (d6+BRAN_SIZE_X) - rect.x;
				rect.y = d7;
				rect.h = (d7+BRAN_SIZE_Y) - rect.y;
			}
			//???????????????
			if(ResoMode == 1){
				x = (((*a0 * bran_cnt) >> 6) + 320 - (BRAN_SIZE_X >> 1)) >> 1;
				y = (((*(a0 + 1) * bran_cnt) >> 6) + 240 - (BRAN_SIZE_Y >> 1)) >> 1;
                SDL_Rect destRect = {x, y, 0,0};
                SDL_BlitSurface(battleSurface, &rect, screenSurface, &destRect);
				//lpDraw->lpBACKBUFFER->BltFast(x,y,lpBattleSurface,&rect,DDBLTFAST_WAIT);
			} else {
				x = ((*a0 * bran_cnt) >> 6) + 320 + (DISPLACEMENT_X >> 1) - (BRAN_SIZE_X >> 1);
				y = ((*(a0 + 1) * bran_cnt) >> 6) + 240 + (DISPLACEMENT_Y >> 1) - (BRAN_SIZE_Y >> 1);
                SDL_Rect destRect = {x, y, 0,0};
                SDL_BlitSurface(battleSurface, &rect, screenSurface, &destRect);
				//lpDraw->lpBACKBUFFER->BltFast(x,y,lpBattleSurface,&rect,DDBLTFAST_WAIT);
			}
			a0 -= 2;
		}
	}
	bran_cnt += ang;
	if(ang < 0){
		if(bran_cnt < 0){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;
			return true;
		}
	} else {
		if(bran_cnt > 64){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;
			return true;
		}
	}
	return false;
}

//Syu Mark δʹ��
#define BREAK_UP 8
bool Produce_break_up(int ang)
{
	SDL_Rect rect = { 0, 0, 640, 0 };
	int d7;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		bran_flg = 0;
	}
	if(bran_flg == 0){		//????
		bran_flg = 1;		//��????
		if(ang < 0){
			bran_cnt = 480;
		} else {
			bran_cnt = 0;
		}
	}
	// ???????????????
//	ClearBackSurface();	
	for(d7=0; d7<bran_cnt; d7++){
		rect.y = d7*480/bran_cnt;
		rect.h = 1;
		//lpDraw->lpBACKBUFFER->BltFast( 0, d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
	}
	if(ang < 0){
		bran_cnt -= BREAK_UP;
		if(bran_cnt < 0){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;		//��????
			return true;
		}
	} else {
		bran_cnt += BREAK_UP;
		if(bran_cnt > 480){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;		//��????
			return true;
		}
	}
	return false;
}

//Syu Mark δʹ��
// ��????? ******************************************************************/
bool Produce_break_up2(int ang)
{
	SDL_Rect rect = { 0, 0, 640, 0 };
	int d7;
	// ??????
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		bran_flg = 0;
	}
	if(bran_flg == 0){		//????
		bran_flg = 1;		//��????
		if(ang < 0){
			bran_cnt = 480;
		} else {
			bran_cnt = 0;
		}
	}
	// ???????????????
//	ClearBackSurface();	
	for(d7=0; d7<bran_cnt; d7++){
		rect.y = 480-d7*480/bran_cnt;
		rect.h = 1;
		//lpDraw->lpBACKBUFFER->BltFast( 0, 480-d7, lpBattleSurface, &rect, DDBLTFAST_WAIT );
	}
	if(ang < 0){
		bran_cnt -= BREAK_UP;
		if(bran_cnt < 0){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;		//��????
			return true;
		}
	} else {
		bran_cnt += BREAK_UP;
		if(bran_cnt > 480){
			// ????�֢t��?
			if( GameState == GAME_FIELD_TO_ENCOUNT ) GameState = GAME_ENCOUNT_TO_BATTLE; 
			else if( GameState == GAME_ENCOUNT_TO_BATTLE ) GameState = GAME_BATTLE; 
			bran_flg = 0;		//��????
			return true;
		}
	}
	return false;
}

//����ѹ��
bool ProduceCenterPress( int mode )
{
	static int line;
	SDL_Rect rect1, rect2;
	int i;
	int h = 240 + (DISPLACEMENT_Y >> 1);
	int spd = 8;
	// ???
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		if( mode == 0 ){
			line = 240;
			if( ResoMode == 1 ) line >>= 1;
		}else	line = 0;
	}
	// ???????????????
	//ClearBackSurface();	
	rect1.x  = 0;
	rect1.w = DEF_APPSIZEX;
	rect2.x  = 0;
	rect2.w = DEF_APPSIZEX;
	if( ResoMode == 1 ){
		h >>= 1;
		rect1.w >>= 1;
		rect2.w >>= 1;
		spd >>= 1;
	}
	for( i = 0; i < line; i++ ){
		rect1.y = h-i*h/line;
		rect1.h = 1;
        SDL_Rect destRect1 = {0, h - i, 0,0};
        SDL_BlitSurface(battleSurface, &rect1, screenSurface, &destRect1);
		//lpDraw->lpBACKBUFFER->BltFast( 0, h-i, lpBattleSurface, &rect1, DDBLTFAST_WAIT );
		rect2.y = i*h/line+h;
		rect2.h = 1;
        SDL_Rect destRect2 = {0, h + i, 0,0};
        SDL_BlitSurface(battleSurface, &rect2, screenSurface, &destRect2);
		//lpDraw->lpBACKBUFFER->BltFast( 0, i+h, lpBattleSurface, &rect2, DDBLTFAST_WAIT );
	}
	if( mode == 0 ){
		line -= spd;
		if( line < 0 ) return true;
	}else{
		line += spd;
		if( line > h ) return true;
	}
	return false;
}


bool DrawProduce( int no )
{
#ifdef _DEBUG
	// pass
	if( joy_con[ 0 ] & JOY_UP ){ 
		// ???�w?��????
		NowTime = TimeGetTime();
		return false;
	}
#endif
	bool ret = true;
	ClearBackSurface();	

	switch( no ){
	case PRODUCE_UP_ACCELE:
		ret = ProduceAccele( 0 );
		break;
	case PRODUCE_DOWN_ACCELE:
		ret = ProduceAccele( 1 );
		break;
	case PRODUCE_LEFT_ACCELE:
		ret = ProduceAccele( 2 );
		break;
	case PRODUCE_RIGHT_ACCELE:	// ??�L?�h
		ret = ProduceAccele( 3 );
		break;
	case PRODUCE_LEFT_RIGHT_ACCELE:	// ???�L?�h
		ret = ProduceAccele( 4 );
		break;
	case PRODUCE_UP_DOWM_ACCELE:	// ��??�L?�h
		ret = ProduceAccele( 5 );
		break;
	case PRODUCE_UP_BRAKE:		// ��?�L?�h
		ret = ProduceBrake( 0 );
		break;
	case PRODUCE_DOWN_BRAKE:	// ??�L?�h
		ret = ProduceBrake( 1 );
		break;
		
	case PRODUCE_LEFT_BRAKE:	// ??�L?�h
		ret = ProduceBrake( 2 );
		break;
		
	case PRODUCE_RIGHT_BRAKE:	// ??�L?�h
		ret = ProduceBrake( 3 );
		break;
		
	case PRODUCE_LEFT_RIGHT_BRAKE:	// ??�L?�h
		ret = ProduceBrake( 4 );
		break;
		
	case PRODUCE_UP_DOWM_BRAKE:	// // ��??�L?�h
		ret = ProduceBrake( 5 );
		break;
	case PRODUCE_4WAY_OUT:		// �??�h?��??
		ret = Produce4Way( 0 );
		break;
	case PRODUCE_4WAY_IN:		// �??�h?����?
		ret = Produce4Way( 1 );
		break;
	case PRODUCE_HAGARE_OUT:	// ??????��???
		ret = ProduceHagare( 0 );
		break;
	case PRODUCE_HAGARE_IN:		// ??????��????
		ret = ProduceHagare( 1 );
		break;
	case PRODUCE_HAGARE_OCHI_OUT:	// ??????��???
		ret = ProduceHagare( 2 );
		break;
	case PRODUCE_HAGARE_OCHI_IN:	// ??????��????
		ret = ProduceHagare( 3 );
		break;
	case PRODUCE_BRAN_SMALL:	// ???����
		ret = Produce_bran_small(-1);
		break;
	case PRODUCE_BRAN_BIG:		// ????��
		ret = Produce_bran_small(1);
		break;
	case PRODUCE_BREAK_UP1:		//��????��???
		ret = Produce_break_up(-1);
		break;
	case PRODUCE_BREAK_UP2:		//��????��??
		ret = Produce_break_up(1);
		break;
	case PRODUCE_BREAK_UP3:		//��????��???
		ret = Produce_break_up(-1);
		break;
	case PRODUCE_BREAK_UP4:		//��???????
		ret = Produce_break_up2(1);
		break;
	case PRODUCE_BREAK_UP5:		//��????????
		ret = Produce_break_up2(-1);
		break;
	case PRODUCE_BREAK_UP6:		//��???????
		ret = Produce_break_up2(1);
		break;
	case PRODUCE_BREAK_UP7:		//��????????
		ret = Produce_break_up2(-1);
		break;
	case PRODUCE_BREAK_UP8:		//��????��??
		ret = Produce_break_up(1);
		break;
	case PRODUCE_CENTER_PRESSIN:
		ret = ProduceCenterPress( 0 );
		break;
	case PRODUCE_CENTER_PRESSOUT:
		ret = ProduceCenterPress( 1 );
		break;

	}
#ifdef __SKYISLAND
	if( ret){
		ClearSurface( battleSurface);
#ifdef _READ16BITBMP
		if(g_bUseAlpha) ClearSurface( battleSurface);
#endif
	}
#endif
	return ret;
}
void TitleProduce( void )
{
	static float x[ 10 ], y[ 10 ], a[ 10 ];
	static int flag = 0, cnt,cnt2, cnt3;
	static int fall[ 8 ];
	static int time;
	int i,bmpNo;
#ifdef _DEBUG		
	// pass
	if( joy_con[ 0 ] & JOY_UP ){ 
		NowTime = TimeGetTime();
		return;
	}
#endif
	if( ProduceInitFlag == true ){
		ProduceInitFlag = false;
		flag = 0;
	}
	if( flag == 0 ){
		for( i = 0 ; i < 8 ; i++ ){
			x[ i ] = 320;
			y[ i ] = -100;
			a[ i ] = 1;
			fall[ i ] = 0;
			cnt = 0;
			cnt2 = 0;
			cnt3 = 0;
			time = 0;
		}
		// ��???????
		x[ 9 ] = -319;
		//cary 2001 07 10
		y[ 9 ] = 410 + 5;
//		y[ 9 ] = 410;
		a[ 9 ] = 25.5;
		x[ 8 ] = 959;
		y[ 8 ] = 444;
		a[ 8 ] = 25.5;
		// ?????????
		for( bmpNo = CG_TITLE_NAME_S ; bmpNo <= CG_TITLE_DREAM_LOGO ; bmpNo++ ){
			unsigned long no;
			realGetNo( bmpNo , &no );
			LoadBmp( no );
		}
		// ???�w?��????
		NowTime = TimeGetTime();
		flag = 1;
	}
	// ??�r?
	if( flag == 1 ){
		//time++;
		//if( time >= 120 ) 
		flag = 2;
	}
	// ��???�h
	if( flag == 2 ){
		a[ 8 ] -= 0.5;
		x[ 8 ] -= a[ 8 ];
		if( a[ 8 ] == 0 ){
			x[ 8 ] = 320;
			flag = 3;
		}
		// ?????�h
		a[ 9 ] -= 0.5;
		x[ 9 ] += a[ 9 ];
		if( a[ 9 ] == 0 ){
			x[ 9 ] = 320;
			flag = 3;
		}
	}
	// ??????
	if( cnt < 8 && flag == 3 ){
		if( cnt2 % 4 == 0 ){
			fall[ cnt ] = 1;
			cnt++;
		}
		cnt2++;
	}
	// ?�h
	if( flag == 3 ){
		for( i = 0 ; i < 8 ; i++ ){
			if( fall[ i ] >= 1 && fall[ i ] <= 3 ){
				a[ i ] += 1.0;
				y[ i ] += a[ i ];
				if( y[ i ] >= 160 ){
					y[ i ] = 160;
					fall[ i ]++;
					if( fall[ i ] == 4 ){ 
						cnt3++;
						continue;
					}
					a[ i ] *= -0.5F;
					y[ i ] += a[ i ];
				}
			}
		}
	}
	// ��???????����
	StockDispBuffer( ( int )x[ 8 ], ( int )y[ 8 ], DISP_PRIO_CHAR, CG_TITLE_JSS_LOGO, 0 );
	//Syu Cancel 06/24/2002
	//StockDispBuffer( ( int )x[ 9 ], ( int )y[ 9 ], DISP_PRIO_CHAR, CG_TITLE_DREAM_LOGO, 0 );
	if( cnt3 < 8 ){
		// ����
		for( i = 0 ; i < 8 ; i++ ){
			StockDispBuffer( ( int )x[ i ], ( int )y[ i ], DISP_PRIO_CHAR, CG_TITLE_NAME_S + i, 0 );
		}
	}else if( cnt3 >= 8 && cnt3 < 31 ){
		if( cnt3 < 26 ){
			StockDispBuffer( ( int )x[ 0 ], ( int )y[ 0 ], DISP_PRIO_CHAR, CG_TITLE_NAME_FLASH, 0 );
		}else{
			StockDispBuffer( ( int )x[ 0 ], ( int )y[ 0 ], DISP_PRIO_CHAR, CG_TITLE_NAME_FLASH1 + cnt3 - 26 , 0 );
		}	
		cnt3++;
	}else
		StockDispBuffer( ( int )x[ 0 ], ( int )y[ 0 ], DISP_PRIO_CHAR, CG_TITLE_NAME, 0 );
}

