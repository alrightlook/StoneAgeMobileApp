#define UNPACK2	0
#define WIN32_LEAN_AND_MEAN
#include "../systeminc/version.h"
#include "../systeminc/system.h"
#include "../systeminc/loadrealbin.h"
#include "../systeminc/unpack.h"

//#include <io.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char autoMapColorTbl[MAX_GRAPHICS];	// ?????????????????

#ifdef _DEBUG
extern int g_iMallocCount;
#endif
/*
#ifdef __CARYTEST

#include "../MemDll.h"

unsigned long		g_dwHeapCache			 = -1;
unsigned long		g_dwCurFrame			 = 10;
LPREALBIN	g_lpRealAdrn			 = NULL;
LPDWORD		g_lpdwBitmapNumberTable	 = NULL;
HANDLE		g_hRealbin				 = NULL;
int			g_iRealbinNum;						//图档数量
int			g_iTableNum;						//图号表的数量
LPREALBIN	g_lpRealHead			 = NULL;
LPREALBIN	g_lpRealTail			 = NULL;

void initAutoMapColor( char *addrbinfilename )
{
	char *filename = "data\\auto.dat";
	if( readAutoMapColor( filename, addrbinfilename ) == 0 ){
		makeAutoMapColor();
		writeAutoMapColor( filename, addrbinfilename );
	}
}

void makeAutoMapColor( void )
{
	int chgTbl[20] = {
		166,  61,  80, 112,  96, 189, 112,  60, 164, 164,
		111,   0,   0,   0,   0,   0,   0,   0,   0,   0	};
	for( int i = 0; i < g_iRealbinNum; i++ ){
		if( g_lpRealAdrn[i].bmpnumber != 0 ){
			if( 100 <= g_lpRealAdrn[i].bmpnumber && g_lpRealAdrn[i].bmpnumber <= 19999 ){
				autoMapColorTbl[g_lpRealAdrn[i].bmpnumber] = getAutoMapColor( g_lpRealAdrn[i].bitmapno );
			}else if( 60 <= g_lpRealAdrn[i].bmpnumber && g_lpRealAdrn[i].bmpnumber <=79 ){
				autoMapColorTbl[g_lpRealAdrn[i].bmpnumber] = (unsigned char)chgTbl[g_lpRealAdrn[i].bmpnumber-60];
			}else{
				autoMapColorTbl[g_lpRealAdrn[i].bmpnumber] = 0;
			}
		}else{
			autoMapColorTbl[g_lpRealAdrn[i].bmpnumber] = 0;
		}
	}
}
int writeAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *wfp, *rfp;
	int rfh;
	struct _stat statBuf;
	int adrnNo;
	unsigned int adrnTime;
	unsigned short autoMapColorVersion = 4;	
	char *tmpStr;

	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??????
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = _fileno( rfp );
	if( _fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = statBuf.st_ctime;
	fclose( rfp );
	if( (wfp = fopen( wFName, "wb" )) == NULL )
		return 0;
	fwrite( &autoMapColorVersion, sizeof( autoMapColorVersion ), 1, wfp );
	fwrite( &adrnNo, sizeof( adrnNo ), 1, wfp );
	fwrite( &adrnTime, sizeof( adrnTime ), 1, wfp );
	fwrite( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, wfp );
	fclose( wfp );
	return 1;
}

int readAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *rfp;
	int rfh;
	struct _stat statBuf;
	int adrnNo, rAdrnNo;
	unsigned int adrnTime, rAdrnTime;
	unsigned short autoMapColorVersion = 4, rAutoMapColorVersion;
	char *tmpStr;
	// adrn.bin???????
	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = _fileno( rfp );
	if( _fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = statBuf.st_ctime;
	fclose( rfp );
	if( (rfp = fopen( wFName, "rb" )) == NULL )
		return 0;

	if( fread( &rAutoMapColorVersion, sizeof( rAutoMapColorVersion ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( autoMapColorVersion != rAutoMapColorVersion ){
		fclose( rfp );
		return 0;
	}
	if( fread( &rAdrnNo, sizeof( rAdrnNo ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnNo != rAdrnNo ){
		fclose( rfp );
		return 0;
	}
	if( fread( &rAdrnTime, sizeof( rAdrnTime ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnTime != rAdrnTime ){
		fclose( rfp );
		return 0;
	}
	if( fread( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	fclose( rfp );
	return true;
}

void CalcualRealNum( HANDLE file, int &numReal, int &numTable)
{
#if 1
	numReal = 0;
	numTable = 0;
	unsigned long readbyte;
	ADRNBIN tmpadrnbuff;
	while( ReadFile( file, &tmpadrnbuff, sizeof(tmpadrnbuff), &readbyte, NULL) && readbyte){
		if( (unsigned long)numReal < tmpadrnbuff.bitmapno)
			numReal = tmpadrnbuff.bitmapno;
		if( (unsigned long)numTable < tmpadrnbuff.attr.bmpnumber)
			numTable = tmpadrnbuff.attr.bmpnumber;
	}
	numReal++;
	numTable++;
#else
	numReal = MAX_GRAPHICS;
	numTable = MAX_GRAPHICS;
#endif
}

bool initRealbinFileOpen(char *realbinfilename, char *addrbinfilename)
{
	bool	ret = false;
	HANDLE	addrbin;
	unsigned long	readbyte;
	ADRNBIN tmpadrnbuff;
	g_lpRealHead = NULL;
	g_lpRealTail = NULL;
	int		memsize = 1024*1024*16;		//10M
	if( ERROR_MEMORY_OK == MemoryInit()){
		if( HeapCreate( &g_dwHeapCache, memsize)){		//建立Heap
			if( INVALID_HANDLE_VALUE != (g_hRealbin=CreateFile( realbinfilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL))){
				if( INVALID_HANDLE_VALUE != (addrbin=CreateFile( addrbinfilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL))){
					int sizeReal, sizeTable;
					CalcualRealNum( addrbin, g_iRealbinNum, g_iTableNum);
					sizeReal = g_iRealbinNum * sizeof(REALBIN);
					sizeTable = g_iTableNum * sizeof(unsigned long);
					//配置全部所需的记忆体
					if( g_lpRealAdrn = (LPREALBIN)VirtualAlloc( NULL, sizeReal+sizeTable+31, MEM_COMMIT, PAGE_READWRITE)){
						ZeroMemory( g_lpRealAdrn, sizeReal+sizeTable+31);
						g_lpdwBitmapNumberTable = (LPDWORD)((sizeReal + (unsigned long)g_lpRealAdrn + 31) & -32);
						SetFilePointer( addrbin, 0, NULL, FILE_BEGIN);
						while( ReadFile( addrbin, &tmpadrnbuff, sizeof(tmpadrnbuff), &readbyte, NULL) && readbyte){
							//填入所有的资料
							g_lpRealAdrn[tmpadrnbuff.bitmapno].bitmapno	 = tmpadrnbuff.bitmapno;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].adder	 = tmpadrnbuff.adder;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].size		 = tmpadrnbuff.size;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].xoffset	 = tmpadrnbuff.xoffset;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].yoffset	 = tmpadrnbuff.yoffset;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].width	 = (short)tmpadrnbuff.width;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].height	 = (short)tmpadrnbuff.height;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].bmpnumber = tmpadrnbuff.attr.bmpnumber;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].hit		 = tmpadrnbuff.attr.hit;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].height1	 = tmpadrnbuff.attr.height;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].effect1	 = tmpadrnbuff.attr.effect1;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].effect2	 = tmpadrnbuff.attr.effect2;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].atari_x	 = tmpadrnbuff.attr.atari_x;
							g_lpRealAdrn[tmpadrnbuff.bitmapno].atari_y	 = tmpadrnbuff.attr.atari_y;
							//建table
							if( tmpadrnbuff.attr.bmpnumber<=33 && tmpadrnbuff.bitmapno>230000){//防堵魔法图号覆盖声音的bug
								continue;
							}
							g_lpdwBitmapNumberTable[tmpadrnbuff.attr.bmpnumber] = tmpadrnbuff.bitmapno;
						}
						g_lpdwBitmapNumberTable[0] = 0;
						ret = true;
					}
					CloseHandle( addrbin);
				}
			}
		}
	}
	if( !ret)
		cleanupRealbin();
	return ret;
}

void cleanupRealbin()
{
	LPREALBIN	pre;
	while( g_lpRealTail){
		g_lpRealTail->cache = NULL;
		g_lpRealTail->decode_cache = NULL;
		pre = g_lpRealTail->pre;
		g_lpRealTail->pre = NULL;
		g_lpRealTail->next = NULL;
		g_lpRealTail = pre;
	}
	g_lpRealHead = NULL;
	if( g_dwHeapCache != -1){
		HeapRelease( &g_dwHeapCache);
		g_dwHeapCache = -1;
	}
	if( g_hRealbin){
		CloseHandle( g_hRealbin);
		g_hRealbin = NULL;
	}
	if( g_lpRealAdrn){
		VirtualFree( g_lpRealAdrn, NULL, MEM_RELEASE);
		g_lpRealAdrn = NULL;
	}
	MemoryRelease();
}

inline void MoveNodeToCacheHead( LPREALBIN node)
{
	//调整cache的顺序****************************
	if( node != g_lpRealHead){
		//调整Tail node
		if( g_lpRealTail == NULL)
			g_lpRealTail = node;
		else if( g_lpRealTail==node && node->pre){
			g_lpRealTail = node->pre;
		}
		//调整双向link
		if( node->pre)
			node->pre->next = node->next;
		if( node->next)
			node->next->pre = node->pre;
		node->pre = NULL;
		node->next = g_lpRealHead;
		//调整Head node
		if( g_lpRealHead)
			g_lpRealHead->pre = node;
		g_lpRealHead = node;
	}
}

void CacheRLU( int howlong)
{
	unsigned long baseframe = SurfaceDate - howlong;	//比较frame cnt的基准cnt
	LPREALBIN	pre;
	while( g_lpRealTail && g_lpRealTail->cache_frame <= baseframe){
		g_lpRealTail->cache = NULL;
		if( g_lpRealTail->decode_cache)
			HeapFree( g_dwHeapCache, (LPVOID*)&g_lpRealTail->decode_cache);
		pre = g_lpRealTail->pre;
		g_lpRealTail->pre = NULL;
		g_lpRealTail->next = NULL;
		g_lpRealTail = pre;
	}
	if( g_lpRealTail)
		g_lpRealTail->next = NULL;
	else//所有cache list都释放掉时
		g_lpRealHead = NULL;
}

LPBYTE ReadRealBin( unsigned long GraphicNo)
{
	int			bufsize;
	unsigned long		readbyte;
	LPREALBIN	node = &g_lpRealAdrn[GraphicNo];
	//读出图档资料
	if( !node->cache){
		//没有cache此图
		SetFilePointer( g_hRealbin, node->adder, NULL, FILE_BEGIN);
		//配置记忆体
		bufsize = node->width*node->height*2;
		if( !(node->decode_cache = (LPWORD)HeapAlloc( g_dwHeapCache, node->size+bufsize))){
			//cache 不够了，把最久没使用的图释放
#if 1
			HeapReset( g_dwHeapCache);
			LPREALBIN node;
			while( g_lpRealHead){
				node = g_lpRealHead->next;
				g_lpRealHead->cache = NULL;
				g_lpRealHead->decode_cache = NULL;
				g_lpRealHead->state = 0;
				g_lpRealHead->next = NULL;
				g_lpRealHead->pre = NULL;
				g_lpRealHead = node;
			}
			g_lpRealTail = NULL;
#else
			CacheRLU( 0);
#endif
//			node = &g_lpRealAdrn[GraphicNo];
			if( !(node->decode_cache = (LPWORD)HeapAlloc( g_dwHeapCache, node->size+bufsize)))
				return NULL;	//还是没配置到记忆体
		}
		node->cache = (LPBYTE)((unsigned long)node->decode_cache + bufsize);
		//读入图档资料
		ReadFile( g_hRealbin, node->cache, node->size, &readbyte, NULL);
		if( node->cache[0]!='R' || node->cache[1]!='D'){
			return NULL;
		}
	}
	//同一个frame中用到数次时，不用再sort
	node->cache_frame = SurfaceDate;
	MoveNodeToCacheHead( node);
	return node->cache;
}

LPBYTE _ReadRealBin( unsigned long GraphicNo)
{
	unsigned long		readbyte;
	LPREALBIN	node = &g_lpRealAdrn[GraphicNo];
	//读出图档资料
	if( !node->cache){
		//没有cache此图
		SetFilePointer( g_hRealbin, node->adder, NULL, FILE_BEGIN);
		//配置记忆体
		if( !(node->cache = (LPBYTE)HeapAlloc( g_dwHeapCache, node->size))){
			//cache 不够了，把最久没使用的图释放
			HeapReset( g_dwHeapCache);
			LPREALBIN node;
			while( g_lpRealHead){
				node = g_lpRealHead->next;
				g_lpRealHead->cache = NULL;
				g_lpRealHead->decode_cache = NULL;
				g_lpRealHead->state = 0;
				g_lpRealHead->next = NULL;
				g_lpRealHead->pre = NULL;
				g_lpRealHead = node;
			}
			g_lpRealTail = NULL;
			if( !(node->cache = (LPBYTE)HeapAlloc( g_dwHeapCache, node->size)))
				return NULL;	//还是没配置到记忆体
		}
		//读入图档资料
		ReadFile( g_hRealbin, node->cache, node->size, &readbyte, NULL);
		if( node->cache[0]!='R' || node->cache[1]!='D'){
			return NULL;
		}
	}
	//同一个frame中用到数次时，不用再sort
	node->cache_frame = SurfaceDate;
	MoveNodeToCacheHead( node);
	return node->cache;
}

bool realGetNo( unsigned long CharAction, unsigned long *GraphicNo)
{
	if( CharAction >= (unsigned long)g_iTableNum ){
		*GraphicNo = 0;
		return false;
	}
	*GraphicNo = g_lpdwBitmapNumberTable[CharAction];
	return true;
}

unsigned long realGetNo2( unsigned long bnum)
{
	if( bnum >= (unsigned long)g_iTableNum)
		return -1;
	return g_lpdwBitmapNumberTable[bnum];
}

bool realIsValid( unsigned long CharAction)
{
	if( CharAction >= (unsigned long)g_iTableNum)
		return false;
	return (g_lpdwBitmapNumberTable[CharAction]>0);
}

unsigned long realGetBitmapNo( unsigned long GraphicNo)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum)
		return -1;
	return g_lpRealAdrn[GraphicNo].bmpnumber;
}

bool realGetPos( unsigned long GraphicNo, short *x, short *y)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*x = 0;
		*y = 0;
		return false;
	}
	*x = g_lpRealAdrn[GraphicNo].xoffset;
	*y = g_lpRealAdrn[GraphicNo].yoffset;
	return true;
}

bool realGetWH( unsigned long GraphicNo, short *w, short *h)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*w = 0;
		*h = 0;
		return false;
	}
	*w = (short)g_lpRealAdrn[GraphicNo].width;
	*h = (short)g_lpRealAdrn[GraphicNo].height;
	return true;
}

bool realGetHitPoints( unsigned long GraphicNo, short *HitX, short *HitY)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*HitX = 0;
		*HitY = 0;
		return false;
	}
	*HitX = g_lpRealAdrn[GraphicNo].atari_x;
	*HitY = g_lpRealAdrn[GraphicNo].atari_y;
	return true;
}

bool realGetPrioType( unsigned long GraphicNo , short *prioType)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*prioType = 0;
		return false;
	}
	*prioType = (g_lpRealAdrn[GraphicNo].hit / 100);
	return true;
}

bool realGetHitFlag( unsigned long GraphicNo, short *Hit)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*Hit = 0;
		return false;
	}
	*Hit = (g_lpRealAdrn[GraphicNo].hit % 100);
	return true;
}
*/
/*
bool realGetHeightFlag( unsigned long GraphicNo, short *Height)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum){
		*Height = 0;
		return false;
	}
	*Height = g_lpRealAdrn[GraphicNo].height1;
	return true;
}

int realGetSoundEffect( unsigned long GraphicNo)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum)
		return 0;
	return g_lpRealAdrn[GraphicNo].effect1;
}

int realGetWalkSoundEffect( unsigned long GraphicNo)
{
	if( GraphicNo >= (unsigned long)g_iRealbinNum)
		return 0;
	return g_lpRealAdrn[GraphicNo].effect2;
}
*/
/*
unsigned char					g_lpbyBuf[4096];
static int				index_d;
static int				iCntWidth, iCntHeight, x, y, w, h;
static LPBYTE			image;
static LPRD_HEADER		header;
static int				destx, desty, sx, sy, ex, ey, destrealw;
static int				cnt, idx, color, index;
bool DrawScaleImage( unsigned long GraphicNo, int screenx, int screeny, int screenw, int screenh, LPWORD dest, int destw, int desth, int pitch, LPWORD pal)
{
	if( GraphicNo < (unsigned long)g_iRealbinNum){
		if( header = (LPRD_HEADER)ReadRealBin( GraphicNo)){
			image = (LPBYTE)header + sizeof(RD_HEADER);
			if( header->id[0]=='R' && header->id[1]=='D'){
				w = header->width;
				h = header->height;
				//不缩放
				_asm{
					mov		eax, dword ptr[destw]	;if( destw == 0)
					mov		ebx, dword ptr[desth]	;if( desth == 0)
					cmp		eax, 0
#if __PII
					cmove	eax, dword ptr[w]		;	destw = w
#else
					je		IF_1
					jmp		ENDIF_1
IF_1:
					mov		eax, dword ptr[w]		;	destw = w
ENDIF_1:
#endif
					cmp		ebx, 0
#if __PII
					cmove	ebx, dword ptr[h]		;	desth = h
#else
					je		IF_2
					jmp		ENDIF_2
IF_2:
					mov		ebx, dword ptr[h]		;	desth = h
ENDIF_2:
#endif
					mov		dword ptr[destw], eax
					mov		dword ptr[desth], ebx
				}
				//判断在荧幕上的实际范围
				_asm{
					mov		eax, dword ptr[screenx]
					mov		ecx, dword ptr[screeny]
					add		eax, dword ptr[destw]	;ex=screenx+destw
					add		ecx, dword ptr[desth]	;sy=screeny+desth
					mov		ebx, dword ptr[screenw]
					mov		edx, dword ptr[screenh]
					cmp		eax, ebx				;if( ex > (screenw-1))
#if __PII
					cmovg	eax, ebx				;	ex = screenw - 1
#else
					jg		IF_3
					jmp		ENDIF_3
IF_3:
					mov		eax, ebx				;	ex = screenw - 1
ENDIF_3:
#endif
					cmp		ecx, edx				;if( sy > (screenh-1))
#if __PII
					cmovg	ecx, edx				;	sy = screenh - 1
#else
					jg		IF_4
					jmp		ENDIF_4
IF_4:
					mov		ecx, edx				;	sy = screenh - 1
ENDIF_4:
#endif
					dec		eax
					dec		ecx
					mov		dword ptr[ex], eax
					mov		dword ptr[sy], ecx
					mov		eax, dword ptr[screenx]	;if( (sx=screenx) < 0)
					mov		ebx, dword ptr[screeny]	;if( (ey=screeny) < 0)
					xor		ecx, ecx
					cmp		eax, 0
#if __PII
					cmovl	eax, ecx				;	sx = 0
#else
					jl		IF_5
					jmp		ENDIF_5
IF_5:
					mov		eax, ecx				;	sx = 0
ENDIF_5:
#endif
					cmp		ebx, 0
#if __PII
					cmovl	ebx, ecx				;	ey = 0
#else
					jl		IF_6
					jmp		ENDIF_6
IF_6:
					mov		ebx, ecx				;	ey = 0
ENDIF_6:
#endif
					mov		dword ptr[sx], eax
					mov		dword ptr[ey], ebx
				}
				iCntHeight = 0;
				destx = sx - screenx;
				destrealw = ex - sx + 1;
				desty = screeny + desth - 1;
				//计算荧幕上的起点
				dest += (sy * (pitch>>1) + sx);
//不使用压缩法*************************************************************************************
				if( !header->compressFlag){
					index = (desty-sy)*w;
					desty = sy;
					iCntHeight = 0;
					while( desty >= ey){
						iCntWidth = 0;
						index_d = 0;
						x = 0;
						while( x++ < w){
							color = image[index++];
							iCntWidth += destw;
							while( iCntWidth >= w){
								iCntWidth -= w;
								g_lpbyBuf[index_d++] = (unsigned char)color;
							}
						}
						iCntHeight += desth;
						while( iCntHeight >= h){
							iCntHeight -= h;
							for( cnt = destx; cnt < destrealw; cnt++){
								if( idx = g_lpbyBuf[cnt])
									dest[cnt] = pal[idx];
							}
							dest -= (pitch>>1);
						}
						desty--;
					}
				}else{
//使用RLE压缩**************************************************************************************
					cnt = 0;
					idx = 0;
					index = 0;
					for( y = 0; y < h; y++){
						_asm{
							xor		ebx, ebx
							mov		ecx, dword ptr[cnt]
							mov		dword ptr[iCntWidth], 0
							mov		dword ptr[index_d], ebx		;index_d = 0
							mov		dword ptr[x], 0
							cmp		ecx, 0
							jz		EXIT_NO_CNT					;上一行没有留下cnt未执行完
							mov		esi, dword ptr[w]
							mov		eax, ecx
							lea		edi, dword ptr[g_lpbyBuf]
							cmp		ecx, esi					;if( cnt > w)
#if __PII
							cmovg	ecx, esi					;	ecx = w
#else
							jg		IF_7
							jmp		ENDIF_7
IF_7:
							mov		ecx, esi					;	ecx = w
ENDIF_7:
#endif
							mov		edx, dword ptr[idx]
							sub		eax, ecx
							mov		dword ptr[cnt], eax			;cnt -= ecx
							mov		dword ptr[x], ecx			;x = ecx
							test	edx, 0x80					;if( idx & BIT_CMP)
							jz		BLOCK_MOVE					;搬移cnt个范围的block
//处理cnt个未处理的重覆color///////////////////////////////////////////////////////////////////////
							mov		edx, dword ptr[color]
							mov		eax, dword ptr[iCntWidth]
REP_BIT_CMP:				;重覆填入cnt个color
							add		eax, dword ptr[destw]		;iCntWidth += destw
							mov		dword ptr[iCntWidth], eax
							sub		eax, esi					;iCntWidth -= w
							jl		SKIP_CMP_MOVE				;因缩小而skip pixel
REP_ZOOM_CMP:				;有可能因放大而重覆此pixel
							mov		byte ptr[edi+ebx], dl		;g_lpbyBuf[index_d] = color
							mov		dword ptr[iCntWidth], eax
							inc		ebx							;index_d++
							sub		eax, esi					;iCntWidth -= w
							jge		REP_ZOOM_CMP				;有可能因放大而重覆此pixel
SKIP_CMP_MOVE:				;因缩小而skip pixel
							mov		eax, dword ptr[iCntWidth]
							dec		ecx
							jg		REP_BIT_CMP					;while( iCntWidth > w)
							jmp		EXIT_CNT_MOVE
//处理cnt个未处理的重覆block搬移///////////////////////////////////////////////////////////////////
BLOCK_MOVE:					;准备搬移区块资料
							mov		esi, dword ptr[index]
REP_BLOCK:					;重覆搬移区块资料
							mov		eax, dword ptr[image]
							mov		dl, byte ptr[eax+esi]		;color = image[index]
							mov		eax, dword ptr[iCntWidth]
							inc		esi							;index++
							add		eax, dword ptr[destw]		;iCntWidth += destw
							mov		dword ptr[iCntWidth], eax
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jl		SKIP_BLOCK_MOVE				;因缩小而skip pixel
REP_ZOOM_BLOCK:				;有可能因放大而重覆此pixel
							mov		byte ptr[edi+ebx], dl		;g_lpbyBuf[index_d] = color
							mov		dword ptr[iCntWidth], eax
							inc		ebx							;index_d++
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jge		REP_ZOOM_BLOCK				;有可能因放大而重覆此pixel
SKIP_BLOCK_MOVE:			;因缩小而skip pixel
							dec		ecx
							jg		REP_BLOCK					;while( iCntWidth > w)
							mov		dword ptr[index], esi		;储存index
EXIT_CNT_MOVE:				;储存index_d
							mov		dword ptr[index_d], ebx
EXIT_NO_CNT:
						}
						_asm{
							mov		eax, dword ptr[x]
							mov		esi, dword ptr[image]
							cmp		eax, dword ptr[w]
							jge		EXIT_LINE_OK				;此行已画完
REP_DRAW_LINE:				;画出一整行的回圈
							mov		ebx, dword ptr[index]
							mov		eax, dword ptr[esi+ebx]		;idx = image[index]
							mov		ecx, eax					;cnt = idx
							inc		ebx							;index++
							mov		dword ptr[idx], eax
							and		ecx, 0x0f					;cnt &= 0x0f;
							test	eax, 0x80					;if( idx & BIT_CMP)
							jz		MOVE_LINE_BLOCK				;	此packet是搬一区块
//处理一连续重覆的pixel////////////////////////////////////////////////////////////////////////////
							mov		edx, eax
							xor		edi, edi					;color = 0
							shr		edx, 8						;最低位元组放image[index]
							test	eax, 0x40					;if( !(idx & BIT_ZERO))
							jnz		NO_COLOR					;	使用透明压缩
							mov		edi, edx					;color = image[index];
							inc		ebx							;index++
							and		edi, 0xff					;取出color
							shr		edx, 8						;最低位元组放image[index]
NO_COLOR:					;使用透明色压缩
							mov		dword ptr[color], edi
							test	eax, 0x30					;if( idx & (BIT_REP_LARG2|BIT_REP_LARG))
							jz		BEGIN_LINE_CMP				;	使用4 bits的byte数压缩
							test	eax, 0x20					;if( idx & BIT_REP_LARG2)
							jnz		SHIFT_16					;	使用20 bits的byte数压缩
							shl		ecx, 8						;cnt << 8
							and		edx, 0xff
							inc		ebx							;index++
							or		ecx, edx					;cnt |= image[index]
							jmp		BEGIN_LINE_CMP
SHIFT_16:					;使用20 bits的byte数压缩
							mov		eax, edx
							shl		ecx, 16						;cnt << 16
							shr		eax, 8
							shl		edx, 8						;cnt的16~9 bit
							and		eax, 0xff					;cnt的8~0 bit
							and		edx, 0xff00					;cnt的16~9 bit
							or		ecx, eax					;cnt |= image[index+1]
							add		ebx, 2						;index+=2
							or		ecx, edx					;cnt |= image[index]
BEGIN_LINE_CMP:				;计算copy多少个bytes及初使registers
							mov		edx, edi					;color
							mov		eax, dword ptr[w]
							mov		edi, ecx
							sub		eax, dword ptr[x]			;w - x
							mov		dword ptr[index], ebx
							cmp		ecx, eax					;if( cnt > (w-x))
#if __PII
							cmovg	ecx, eax					;	ecx = (w-x)
#else
							jg		IF_8
							jmp		ENDIF_8
IF_8:
							mov		ecx, eax					;	ecx = (w-x)
ENDIF_8:
#endif
							mov		ebx, dword ptr[index_d]
							sub		edi, ecx					;cnt -= ecx
							mov		eax, dword ptr[iCntWidth]
							mov		dword ptr[cnt], edi
							add		dword ptr[x], ecx			;x += ecx
							lea		edi, dword ptr[g_lpbyBuf]
REP_LINE_BIT_CMP:			;重覆填入cnt个color
							add		eax, dword ptr[destw]		;iCntWidth += destw
							mov		dword ptr[iCntWidth], eax
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jl		SKIP_LINE_CMP_MOVE			;因缩放而skip pixel
REP_ZOOM_LINE_CMP:			;有可能因放大而重覆此pixel
							mov		byte ptr[edi+ebx], dl		;g_lpbyBuf[index_d] = color
							mov		dword ptr[iCntWidth], eax
							inc		ebx							;index_d++
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jge		REP_ZOOM_LINE_CMP			;有可能因放大而重覆此pixel
SKIP_LINE_CMP_MOVE:			;因缩放而skip pixel
							mov		eax, dword ptr[iCntWidth]
							dec		ecx
							jg		REP_LINE_BIT_CMP			;while( iCntWidth > w)
							mov		eax, dword ptr[x]
							mov		dword ptr[index_d], ebx
							cmp		eax, dword ptr[w]			;while( x < w)
							jl		REP_DRAW_LINE				;继续画完此行
							jmp		EXIT_LINE_OK
//处理区块的copy///////////////////////////////////////////////////////////////////////////////////
MOVE_LINE_BLOCK:
							test	eax, 0x10					;if( idx & BIT_REP_LARG)
							jz		BEGIN_LINE_BLOCK			;	使用4 bits的区块大小
							shr		eax, 8						;使用12 bits的大小
							shl		ecx, 8						;cnt << 8
							and		eax, 0xff
							inc		ebx							;index++
							or		ecx, eax					;cnt |= image[index]
BEGIN_LINE_BLOCK:			;计算copy多少个bytes及初使registers
							mov		eax, dword ptr[w]
							mov		edx, ecx
							sub		eax, dword ptr[x]			;w - x
							mov		esi, dword ptr[index_d]
							cmp		ecx, eax					;if( cnt > (w-x))
#if __PII
							cmovg	ecx, eax					;	ecx = (w-x)
#else
							jg		IF_9
							jmp		ENDIF_9
IF_9:
							mov		ecx, eax					;	ecx = (w-x)
ENDIF_9:
#endif
							lea		edi, dword ptr[g_lpbyBuf]
							sub		edx, ecx					;cnt -= ecx
							mov		dword ptr[cnt], edx
							add		dword ptr[x], ecx			;x += ecx
REP_LINE_BLOCK:				;重覆搬移区块资料
							mov		eax, dword ptr[image]
							mov		dl, byte ptr[eax+ebx]		;color = image[index]
							mov		eax, dword ptr[iCntWidth]
							inc		ebx							;index++
							add		eax, dword ptr[destw]		;iCntWidth += destw
							mov		dword ptr[iCntWidth], eax
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jl		SKIP_LINE_BLOCK_MOVE		;因缩放而skip pixel
REP_ZOOM_LINE_BLOCK:		;有可能因放大而重覆此pixel
							mov		byte ptr[edi+esi], dl		;g_lpbyBuf[index_d++] = color
							mov		dword ptr[iCntWidth], eax
							inc		esi							;index_d++
							sub		eax, dword ptr[w]			;iCntWidth -= w
							jge		REP_ZOOM_LINE_BLOCK			;有可能因放大而重覆此pixel
SKIP_LINE_BLOCK_MOVE:		;因缩放而skip pixel
							dec		ecx
							jg		REP_LINE_BLOCK				;while( iCntWidth > w)
							mov		dword ptr[index], ebx		;储存index
							mov		dword ptr[index_d], esi
							mov		eax, dword ptr[x]
							mov		esi, dword ptr[image]
							cmp		eax, dword ptr[w]			;while( x > w)
							jl		REP_DRAW_LINE				;继续画完此行
EXIT_LINE_OK:
						}
						_asm{
							mov		ecx, dword ptr[iCntHeight]
							mov		edi, dword ptr[dest]
							add		ecx, dword ptr[desth]		;iCntHeight += desth;
							lea		esi, dword ptr[g_lpbyBuf]
							mov		dword ptr[iCntHeight], ecx
							mov		ebx, dword ptr[pal]
							sub		ecx, dword ptr[h]			;if( iCntHeight < h)
							jl		EXIT_MOVE_LINE				;exit
							add		esi, dword ptr[destx]		;g_lpbyBuf[destx]
REP_HEIGHT:					;计算此行要重覆的次数
							mov		edx, dword ptr[desty]
							mov		dword ptr[iCntHeight], ecx	;iCntHeight -= h
							cmp		edx, dword ptr[sy]
							jg		NEXT_LINE
							xor		ecx, ecx
MOVE_LINE:					;复制本行
							xor		eax, eax
							mov		al, byte ptr[esi+ecx]		;g_lpbyBuf[i]
							cmp		eax, 0
							jz		TRANS						;if( 0 == g_lpbyBuf[i])
							mov		dx, word ptr[ebx+eax*2]		;pal[tmp]
							mov		word ptr[edi+ecx*2], dx		;dest[i] = pal[tmp]
TRANS:						;透明色不需搬移
							inc		ecx							;i++
							cmp		ecx, dword ptr[destrealw]
							jl		MOVE_LINE					;i<=ex
							sub		edi, dword ptr[pitch]		;dest -= pitch
							mov		edx, dword ptr[desty]
							mov		dword ptr[dest], edi
NEXT_LINE:					;判断是否画下一行，或return true
							dec		edx							;desty-=1
							mov		ecx, dword ptr[iCntHeight]
							mov		dword ptr[desty], edx
							cmp		edx, dword ptr[ey]			;if( desty < ey)
							jl		RETURN_TRUE					;return true
							sub		ecx, dword ptr[h]
							jge		REP_HEIGHT					;while( iCntHeight >= h)
							jmp		EXIT_MOVE_LINE
RETURN_TRUE:				;y已超出要秀的范围，return true
							mov		eax, dword ptr[h]			;强制离开
							mov		dword ptr[y], eax
EXIT_MOVE_LINE:				;此行的缩放功能处理完毕
						}
					}
				}
			}
			return true;
		}
	}
	return false;
}
*/
/*
static LPWORD	dest;
bool DrawImage( unsigned long GraphicNo, LPWORD pal)
{
//	if( GraphicNo < (unsigned long)g_iRealbinNum){
		if( header = (LPRD_HEADER)ReadRealBin( GraphicNo)){
			LPREALBIN	node = &g_lpRealAdrn[GraphicNo];
			dest = node->decode_cache;
			image = (LPBYTE)header + sizeof(RD_HEADER);
		//	if( header->id[0]=='R' && header->id[1]=='D'){
				w = header->width;
				h = header->height;
				size = header->size;
				desty = h - 1;
				//计算荧幕上的起点
				dest += (desty * w);
//不使用压缩法*************************************************************************************
				if( !header->compressFlag){
					index = 0;
					for( ; desty >= 0; desty--){
						for( destx = 0; destx < w; destx++){
							if( color = image[destx+index])
								dest[destx] = pal[color];
						}
						index += w;
						dest -= w;
					}
				}else{
//使用RLE压缩**************************************************************************************
					cnt = 0;
					index = 0;
					for( ; desty >= 0; desty--){
						_asm{
							mov		ecx, dword ptr[cnt]
							mov		dword ptr[x], 0
							cmp		ecx, 0
							jz		EXIT_NO_CNT					;上一行没有留下cnt未执行完
							mov		esi, dword ptr[w]
							mov		eax, ecx
							mov		edi, dword ptr[dest]
							cmp		ecx, esi					;if( cnt > w)
							jle		ENDIF_8
							mov		ecx, esi					;	ecx = w
ENDIF_8:
							sub		eax, ecx
							mov		edx, dword ptr[idx]
							mov		dword ptr[cnt], eax			;cnt -= ecx
							mov		dword ptr[x], ecx			;x = ecx
							test	edx, 0x80					;if( idx & BIT_CMP)
							jz		BLOCK_MOVE					;搬移cnt个范围的block
//处理cnt个未处理的重覆color///////////////////////////////////////////////////////////////////////
#if 1
							mov		edx, dword ptr[color]
							sub		ecx, 2
							jl		REP_BIT_CMP_1
REP_BIT_CMP_2:				;重覆填入cnt个color
							mov		dword ptr[edi], edx			;*dest = color
							add		edi, 4						;dest++
							sub		ecx, 2
							jge		REP_BIT_CMP_2				;while( cnt > 0)
REP_BIT_CMP_1:
							add		ecx, 2
							je		EXIT_CNT_MOVE
							xor		ecx, ecx
							mov		ecx, edx
							mov		word ptr[edi], cx			;*dest = color
							add		edi, 2						;dest++
							jmp		EXIT_CNT_MOVE
#else
							xor		edx, edx
							mov		edx, dword ptr[color]
REP_BIT_CMP:				;重覆填入cnt个color
							mov		word ptr[edi], dx			;*dest = color
							add		edi, 2						;dest++
							dec		ecx
							jg		REP_BIT_CMP					;while( cnt > 0)
							jmp		EXIT_CNT_MOVE
#endif
//处理cnt个未处理的重覆block搬移///////////////////////////////////////////////////////////////////
BLOCK_MOVE:					;准备搬移区块资料
							mov		esi, dword ptr[index]
							mov		ebx, dword ptr[pal]
REP_BLOCK:					;重覆搬移区块资料
							mov		edx, dword ptr[image]
							mov		edx, dword ptr[edx+esi]		;col_index = image[index]
							and		edx, 0xff
							mov		ax, word ptr[ebx+edx*2]		;color = pal[col_index]
							mov		word ptr[edi], ax			;*dest = color
							inc		esi							;index++
							add		edi, 2						;dest++
							dec		ecx
							jg		REP_BLOCK					;while( cnt > 0)
							mov		dword ptr[index], esi		;储存index
EXIT_CNT_MOVE:
							mov		dword ptr[dest], edi		;储存dest
EXIT_NO_CNT:
						}
						_asm{
							mov		eax, dword ptr[x]
							mov		esi, dword ptr[image]
							cmp		eax, dword ptr[w]
							jge		EXIT_LINE_OK				;此行已画完
							mov		ebx, dword ptr[index]
REP_DRAW_LINE:				;画出一整行的回圈
							mov		eax, dword ptr[esi+ebx]		;idx = image[index]
							inc		ebx							;index++
							mov		ecx, eax					;cnt = idx
							mov		dword ptr[idx], eax
							and		ecx, 0x0f					;cnt &= 0x0f;
							test	eax, 0x80					;if( idx & BIT_CMP)
							jz		MOVE_LINE_BLOCK				;	此packet是搬一区块
//处理一连续重覆的pixel////////////////////////////////////////////////////////////////////////////
							mov		edx, eax
							xor		edi, edi					;col_index = 0
							shr		edx, 8						;最低位元组放image[index]
							test	eax, 0x40					;if( !(idx & BIT_ZERO))
							jnz		NO_COLOR					;	使用透明压缩
							mov		edi, edx					;col_index = image[index];
							inc		ebx							;index++
							and		edi, 0xff					;取出col_index
							shr		edx, 8						;最低位元组放image[index]
							shl		edi, 1
							add		edi, dword ptr[pal]
							mov		edi, dword ptr[edi]			;color = pal[col_index]
							and		edi, 0xffff
							mov		dword ptr[color], edi
							shl		edi, 16
							or		edi, dword ptr[color]
NO_COLOR:					;使用透明色压缩
							mov		dword ptr[color], edi
							test	eax, 0x30					;if( idx & (BIT_REP_LARG2|BIT_REP_LARG))
							jz		BEGIN_LINE_CMP				;	使用4 bits的byte数压缩
							test	eax, 0x20					;if( idx & BIT_REP_LARG2)
							jnz		SHIFT_16					;	使用20 bits的byte数压缩
							shl		ecx, 8						;cnt << 8
							and		edx, 0xff
							inc		ebx							;index++
							or		ecx, edx					;cnt |= image[index]
							jmp		BEGIN_LINE_CMP
SHIFT_16:					;使用20 bits的byte数压缩
							mov		eax, edx
							shl		ecx, 16						;cnt << 16
							shr		eax, 8
							shl		edx, 8						;cnt的16~9 bit
							and		eax, 0xff					;cnt的8~0 bit
							and		edx, 0xff00					;cnt的16~9 bit
							or		ecx, eax					;cnt |= image[index+1]
							add		ebx, 2						;index+=2
							or		ecx, edx					;cnt |= image[index]
BEGIN_LINE_CMP:				;计算copy多少个bytes及初使registers
							mov		edx, edi					;color
							mov		eax, dword ptr[w]
							mov		edi, ecx
							sub		eax, dword ptr[x]			;w - x
							cmp		ecx, eax					;if( cnt > (w-x))
							jle		ENDIF_9
							mov		ecx, eax					;	ecx = (w-x)
ENDIF_9:
							sub		edi, ecx					;cnt -= ecx
							mov		dword ptr[cnt], edi
							add		dword ptr[x], ecx			;x += ecx
							mov		edi, dword ptr[dest]
#if 1
							sub		ecx, 2
							jl		REP_LINE_BIT_CMP_1
REP_LINE_BIT_CMP_2:			;重覆填入cnt个color
							mov		dword ptr[edi], edx			;*dest = color
							add		edi, 4						;dest++
							sub		ecx, 2
							jge		REP_LINE_BIT_CMP_2			;while( cnt > 0)
REP_LINE_BIT_CMP_1:
							add		ecx, 2
							je		EXIT_LINE_BIT_CMP
							xor		ecx, ecx
							mov		ecx, edx
							mov		word ptr[edi], cx			;*dest = color
							add		edi, 2						;dest++
EXIT_LINE_BIT_CMP:
#else
REP_LINE_BIT_CMP:			;重覆填入cnt个color
							mov		word ptr[edi], dx			;*dest = color
							add		edi, 2						;dest++
							dec		ecx
							jg		REP_LINE_BIT_CMP			;while( cnt > 0)
#endif
							mov		eax, dword ptr[x]
							mov		dword ptr[dest], edi		;储存dest
							cmp		eax, dword ptr[w]			;while( x < w)
							jl		REP_DRAW_LINE				;继续画完此行
							jmp		EXIT_LINE_BLOCK
//处理区块的copy///////////////////////////////////////////////////////////////////////////////////
MOVE_LINE_BLOCK:
							test	eax, 0x10					;if( idx & BIT_REP_LARG)
							jz		BEGIN_LINE_BLOCK			;	使用4 bits的区块大小
							shr		eax, 8						;使用12 bits的大小
							shl		ecx, 8						;cnt << 8
							and		eax, 0xff
							inc		ebx							;index++
							or		ecx, eax					;cnt |= image[index]
BEGIN_LINE_BLOCK:			;计算copy多少个bytes及初使registers
							mov		eax, dword ptr[w]
							mov		edx, ecx
							sub		eax, dword ptr[x]			;w - x
							cmp		ecx, eax					;if( cnt > (w-x))
							jle		ENDIF_10
							mov		ecx, eax					;	ecx = (w-x)
ENDIF_10:
							mov		edi, dword ptr[dest]
							sub		edx, ecx					;cnt -= ecx
							add		dword ptr[x], ecx			;x += ecx
							mov		esi, dword ptr[pal]
							mov		dword ptr[cnt], edx
REP_LINE_BLOCK:				;重覆搬移区块资料
							mov		edx, dword ptr[image]
							mov		edx, dword ptr[edx+ebx]		;col_index = image[index]
							and		edx, 0xff
							mov		ax, word ptr[esi+edx*2]		;color = pal[col_index]
							mov		word ptr[edi], ax			;*dest = color
							inc		ebx							;index++
							add		edi, 2						;dest++
							dec		ecx
							jg		REP_LINE_BLOCK				;while( cnt > 0)
							mov		esi, dword ptr[image]
							mov		eax, dword ptr[x]
							mov		dword ptr[dest], edi		;储存dest
							cmp		eax, dword ptr[w]			;while( x > w)
							jl		REP_DRAW_LINE				;继续画完此行
EXIT_LINE_BLOCK:
							mov		dword ptr[index], ebx		;储存index
EXIT_LINE_OK:
							mov		eax, dword ptr[w]
							shl		eax, 2
							sub		dword ptr[dest], eax
						}
					}
				}
		//	}
			return true;
		}
//	}
	return false;
}
*/
/*
#define CHECK_WIDTH(); if( (cnt+=j) > w){ bak = cnt - w; cnt = w;}
#define GET_BITCMP_CNT();	cnt = idx & 0x0f;							\
							if( idx & (BIT_REP_LARG2|BIT_REP_LARG))	{	\
								cnt = (cnt<<8) | *image++;				\
								if( idx & BIT_REP_LARG2)				\
									cnt = (cnt<<8) | *image++;			\
							}
#define GET_NOTBITCMP_CNT();	cnt = idx & 0x0f;				\
								if( idx & BIT_REP_LARG)			\
									cnt = (cnt<<8) | *image++;

bool DrawImage( unsigned long GraphicNo, int screenx, int screeny, int screenw, int screenh, LPWORD dest, int pitch, LPWORD pal)
{
	int j, i, tmp, bak, repData, skin;
	if( GraphicNo < (unsigned long)g_iRealbinNum){
		if( header = (LPRD_HEADER)_ReadRealBin( GraphicNo)){
			LPREALBIN	node = &g_lpRealAdrn[GraphicNo];
			image = (LPBYTE)header + sizeof(RD_HEADER);
			if( header->id[0]=='R' && header->id[1]=='D'){
				ex = w = header->width;
				ey = h = header->height;
				if( (screenx+w) > screenw)
					ex = screenw - screenx;
				sy = 0;
				if( (j=screeny+h) > screenh)
					sy = j - screenh;
				sx = 0;
				if( screenx < 0){
					sx = -screenx;
					screenx = 0;
				}
				if( screeny < 0){
					ey = h + screeny;
					screeny = 0;
				}
				dest += ((screeny+(ey-sy)-1) * (pitch>>=1) + screenx);
				pitch = -pitch - (ex-sx);
				if( !header->compressFlag){
					image += sy * w;
					tmp = w - ex;
					for( i = sy; i < ey; i++){
						image += sx;
						for( j = sx; j < ex; j++, dest++){
							if( color = *image++)
								*dest = pal[color];
						}
						image += tmp;
						dest += pitch;
					}
				}else{
					bak = 0;
					for( i = 0; i < sy; i++){
						j = 0;
						if( bak){
							if( (j=bak) > w)
								j = w;
							if( !(idx & BIT_CMP))
								image += j;
							if( bak -= j)
								continue;
						}
						for( ; j < w;){
							idx = *image++;
							if( idx & BIT_CMP){
								color = 0;
								if( !(idx & BIT_ZERO))
									repData = pal[color=*image++];
								GET_BITCMP_CNT();
								CHECK_WIDTH();
								j = cnt;
							}else{
								GET_NOTBITCMP_CNT();
								CHECK_WIDTH();
								image += (cnt-j);
								j = cnt;
							}
						}
					}
					for( ; i < ey; i++, dest += pitch){
						j = 0;
						if( bak){
							if( (cnt=bak) > w)   cnt = w;
							if( cnt > sx){
								tmp = cnt;
								skin = 0;
								if( cnt > ex){
									tmp = ex;
									skin = cnt - tmp;
								}
								if( idx & BIT_CMP){
									if( color){
										for( j = sx; j < tmp; j++)
											*dest++ = repData;
									}else
										dest += (tmp-sx);
								}else{
									image += sx;
									for( j = sx; j < tmp; j++, dest++){
										if( color = *image++)
											*dest = pal[color];
									}
									image += skin;
								}
								if( bak -= cnt)
									continue;
							}else if( !(idx & BIT_CMP))
								image += cnt;
							bak = 0;
							j = cnt;
						}
						for( ; j < sx;){
							idx = *image++;
							if( idx & BIT_CMP){
								color = 0;
								if( !(idx & BIT_ZERO))
									repData = pal[color=*image++];
								GET_BITCMP_CNT();
								CHECK_WIDTH();
								if( cnt > sx){
									tmp = cnt;
									if( cnt > ex)   tmp = ex;
									if( color){
										for( j = sx; j < tmp; j++, dest++)
											*dest = repData;
									}else
										dest += (tmp-sx);
								}
							}else{
								GET_NOTBITCMP_CNT();
								CHECK_WIDTH();
								if( cnt > sx){
									tmp = cnt;
									if( cnt > ex)   tmp = ex;
									image += (sx-j);
									for( j = sx; j < tmp; j++, dest++){
										if( color = *image++)
											*dest = pal[color];
									}
								}else
									image += (cnt-j);
							}
							j = cnt;
						}
						for( ; j < w;){
							idx = *image++;
							if( idx & BIT_CMP){
								color = 0;
								if( !(idx & BIT_ZERO))
									repData = pal[color = *image++];
								GET_BITCMP_CNT();
								CHECK_WIDTH();
								tmp = cnt;
								if( cnt > ex)   tmp = ex;
								if( color){
									for( ; j < tmp; j++)
										*dest++ = repData;
								}else if( j < tmp)
									dest += (tmp-j);
							}else{
								GET_NOTBITCMP_CNT();
								CHECK_WIDTH();
								tmp = cnt;
								skin = 0;
								if( cnt > ex){
									tmp = ex;
									if( tmp > j)
										skin = cnt - tmp;
									else
										skin = cnt - j;
								}
								for( ; j < tmp; j++, dest++){
									if( color = *image++)
										*dest = pal[color];
								}
								image += skin;
							}
 							j = cnt;
						}
					}
				}
				return true;
			}
		}
	}
	return false;
}
*/
/*
LPBYTE ReadSkyIslandRealBin( unsigned long GraphicNo)
{
	unsigned long		readbyte;
	LPREALBIN	node = &g_lpRealAdrn[GraphicNo];
	//读出图档资料
	if( !node->cache){
		//没有cache此图
		SetFilePointer( g_hRealbin, node->adder, NULL, FILE_BEGIN);
		//配置记忆体
		if( !(node->cache = (LPBYTE)HeapAlloc( g_dwHeapCache, node->size))){
			//cache 不够了，把最久没使用的图释放
#if 1
			HeapReset( g_dwHeapCache);
			LPREALBIN node;
			while( g_lpRealHead){
				node = g_lpRealHead->next;
				g_lpRealHead->cache = NULL;
				g_lpRealHead->decode_cache = NULL;
				g_lpRealHead->state = 0;
				g_lpRealHead->next = NULL;
				g_lpRealHead->pre = NULL;
				g_lpRealHead = node;
			}
			g_lpRealTail = NULL;
#else
			CacheRLU( 0);
#endif
			node = &g_lpRealAdrn[GraphicNo];
			if( !(node->cache = (LPBYTE)HeapAlloc( g_dwHeapCache, node->size)))
				return NULL;	//还是没配置到记忆体
		}
		//读入图档资料
		ReadFile( g_hRealbin, node->cache, node->size, &readbyte, NULL);
		if( node->cache[0]!='R' || node->cache[1]!='D'){
			return NULL;
		}
	}
	//同一个frame中用到数次时，不用再sort
	node->cache_frame = SurfaceDate;
	MoveNodeToCacheHead( node);
	return node->cache;
}

bool DrawSkyIslandImage( unsigned long GraphicNo, LPWORD buf, LPWORD pal)
{
	if( header = (LPRD_HEADER)ReadSkyIslandRealBin( GraphicNo)){
		dest = buf;
		image = (LPBYTE)header + sizeof(RD_HEADER);
		w = header->width;
		h = header->height;
		sy = h - 1;
		desty = sy;
		//计算荧幕上的起点
		dest += (sy * w);
//不使用压缩法*************************************************************************************
		if( !header->compressFlag){
			index = 0;
			for( ; desty >= 0; desty--){
				for( destx = 0; destx < w; destx++){
					if( color = image[destx+index])
						dest[destx] = pal[color];
				}
				index += w;
				dest -= w;
			}
		}else{
//使用RLE压缩**************************************************************************************
			cnt = 0;
			index = 0;
			for( ; desty >= 0; desty--){
				_asm{
					mov		ecx, dword ptr[cnt]
					mov		dword ptr[x], 0
					cmp		ecx, 0
					jz		EXIT_NO_CNT					;上一行没有留下cnt未执行完
					mov		esi, dword ptr[w]
					mov		eax, ecx
					mov		edi, dword ptr[dest]
					cmp		ecx, esi					;if( cnt > w)
					jle		ENDIF_8
					mov		ecx, esi					;	ecx = w
ENDIF_8:
					sub		eax, ecx
					mov		edx, dword ptr[idx]
					mov		dword ptr[cnt], eax			;cnt -= ecx
					mov		dword ptr[x], ecx			;x = ecx
					test	edx, 0x80					;if( idx & BIT_CMP)
					jz		BLOCK_MOVE					;搬移cnt个范围的block
//处理cnt个未处理的重覆color///////////////////////////////////////////////////////////////////////
#if 1
					mov		edx, dword ptr[color]
					sub		ecx, 2
					jl		REP_BIT_CMP_1
REP_BIT_CMP_2:				;重覆填入cnt个color
					mov		dword ptr[edi], edx			;*dest = color
					add		edi, 4						;dest++
					sub		ecx, 2
					jge		REP_BIT_CMP_2				;while( cnt > 0)
REP_BIT_CMP_1:
					add		ecx, 2
					je		EXIT_CNT_MOVE
					xor		ecx, ecx
					mov		ecx, edx
					mov		word ptr[edi], cx			;*dest = color
					add		edi, 2						;dest++
					jmp		EXIT_CNT_MOVE
#else
					xor		edx, edx
					mov		edx, dword ptr[color]
REP_BIT_CMP:				;重覆填入cnt个color
					mov		word ptr[edi], dx			;*dest = color
					add		edi, 2						;dest++
					dec		ecx
					jg		REP_BIT_CMP					;while( cnt > 0)
					jmp		EXIT_CNT_MOVE
#endif
//处理cnt个未处理的重覆block搬移///////////////////////////////////////////////////////////////////
BLOCK_MOVE:					;准备搬移区块资料
					mov		esi, dword ptr[index]
					mov		ebx, dword ptr[pal]
REP_BLOCK:					;重覆搬移区块资料
					mov		edx, dword ptr[image]
					mov		edx, dword ptr[edx+esi]		;col_index = image[index]
					and		edx, 0xff
					mov		ax, word ptr[ebx+edx*2]		;color = pal[col_index]
					mov		word ptr[edi], ax			;*dest = color
					inc		esi							;index++
					add		edi, 2						;dest++
					dec		ecx
					jg		REP_BLOCK					;while( cnt > 0)
					mov		dword ptr[index], esi		;储存index
EXIT_CNT_MOVE:
					mov		dword ptr[dest], edi		;储存dest
EXIT_NO_CNT:
				}
				_asm{
					mov		eax, dword ptr[x]
					mov		esi, dword ptr[image]
					cmp		eax, dword ptr[w]
					jge		EXIT_LINE_OK				;此行已画完
					mov		ebx, dword ptr[index]
REP_DRAW_LINE:				;画出一整行的回圈
					mov		eax, dword ptr[esi+ebx]		;idx = image[index]
					inc		ebx							;index++
					mov		ecx, eax					;cnt = idx
					mov		dword ptr[idx], eax
					and		ecx, 0x0f					;cnt &= 0x0f;
					test	eax, 0x80					;if( idx & BIT_CMP)
					jz		MOVE_LINE_BLOCK				;	此packet是搬一区块
//处理一连续重覆的pixel////////////////////////////////////////////////////////////////////////////
					mov		edx, eax
					xor		edi, edi					;col_index = 0
					shr		edx, 8						;最低位元组放image[index]
					test	eax, 0x40					;if( !(idx & BIT_ZERO))
					jnz		NO_COLOR					;	使用透明压缩
					mov		edi, edx					;col_index = image[index];
					inc		ebx							;index++
					and		edi, 0xff					;取出col_index
					shr		edx, 8						;最低位元组放image[index]
					shl		edi, 1
					add		edi, dword ptr[pal]
					mov		edi, dword ptr[edi]			;color = pal[col_index]
					and		edi, 0xffff
					mov		dword ptr[color], edi
					shl		edi, 16
					or		edi, dword ptr[color]
NO_COLOR:					;使用透明色压缩
					mov		dword ptr[color], edi
					test	eax, 0x30					;if( idx & (BIT_REP_LARG2|BIT_REP_LARG))
					jz		BEGIN_LINE_CMP				;	使用4 bits的byte数压缩
					test	eax, 0x20					;if( idx & BIT_REP_LARG2)
					jnz		SHIFT_16					;	使用20 bits的byte数压缩
					shl		ecx, 8						;cnt << 8
					and		edx, 0xff
					inc		ebx							;index++
					or		ecx, edx					;cnt |= image[index]
					jmp		BEGIN_LINE_CMP
SHIFT_16:					;使用20 bits的byte数压缩
					mov		eax, edx
					shl		ecx, 16						;cnt << 16
					shr		eax, 8
					shl		edx, 8						;cnt的16~9 bit
					and		eax, 0xff					;cnt的8~0 bit
					and		edx, 0xff00					;cnt的16~9 bit
					or		ecx, eax					;cnt |= image[index+1]
					add		ebx, 2						;index+=2
					or		ecx, edx					;cnt |= image[index]
BEGIN_LINE_CMP:				;计算copy多少个bytes及初使registers
					mov		edx, edi					;color
					mov		eax, dword ptr[w]
					mov		edi, ecx
					sub		eax, dword ptr[x]			;w - x
					cmp		ecx, eax					;if( cnt > (w-x))
					jle		ENDIF_9
					mov		ecx, eax					;	ecx = (w-x)
ENDIF_9:
					sub		edi, ecx					;cnt -= ecx
					mov		dword ptr[cnt], edi
					add		dword ptr[x], ecx			;x += ecx
					mov		edi, dword ptr[dest]
#if 1
					sub		ecx, 2
					jl		REP_LINE_BIT_CMP_1
REP_LINE_BIT_CMP_2:			;重覆填入cnt个color
					mov		dword ptr[edi], edx			;*dest = color
					add		edi, 4						;dest++
					sub		ecx, 2
					jge		REP_LINE_BIT_CMP_2			;while( cnt > 0)
REP_LINE_BIT_CMP_1:
					add		ecx, 2
					je		EXIT_LINE_BIT_CMP
					xor		ecx, ecx
					mov		ecx, edx
					mov		word ptr[edi], cx			;*dest = color
					add		edi, 2						;dest++
EXIT_LINE_BIT_CMP:
#else
REP_LINE_BIT_CMP:			;重覆填入cnt个color
					mov		word ptr[edi], dx			;*dest = color
					add		edi, 2						;dest++
					dec		ecx
					jg		REP_LINE_BIT_CMP			;while( cnt > 0)
#endif
					mov		eax, dword ptr[x]
					mov		dword ptr[dest], edi		;储存dest
					cmp		eax, dword ptr[w]			;while( x < w)
					jl		REP_DRAW_LINE				;继续画完此行
					jmp		EXIT_LINE_BLOCK
//处理区块的copy///////////////////////////////////////////////////////////////////////////////////
MOVE_LINE_BLOCK:
					test	eax, 0x10					;if( idx & BIT_REP_LARG)
					jz		BEGIN_LINE_BLOCK			;	使用4 bits的区块大小
					shr		eax, 8						;使用12 bits的大小
					shl		ecx, 8						;cnt << 8
					and		eax, 0xff
					inc		ebx							;index++
					or		ecx, eax					;cnt |= image[index]
BEGIN_LINE_BLOCK:			;计算copy多少个bytes及初使registers
					mov		eax, dword ptr[w]
					mov		edx, ecx
					sub		eax, dword ptr[x]			;w - x
					cmp		ecx, eax					;if( cnt > (w-x))
					jle		ENDIF_10
					mov		ecx, eax					;	ecx = (w-x)
ENDIF_10:
					mov		edi, dword ptr[dest]
					sub		edx, ecx					;cnt -= ecx
					add		dword ptr[x], ecx			;x += ecx
					mov		esi, dword ptr[pal]
					mov		dword ptr[cnt], edx
REP_LINE_BLOCK:				;重覆搬移区块资料
					mov		edx, dword ptr[image]
					mov		edx, dword ptr[edx+ebx]		;col_index = image[index]
					and		edx, 0xff
					mov		ax, word ptr[esi+edx*2]		;color = pal[col_index]
					mov		word ptr[edi], ax			;*dest = color
					inc		ebx							;index++
					add		edi, 2						;dest++
					dec		ecx
					jg		REP_LINE_BLOCK				;while( cnt > 0)
					mov		esi, dword ptr[image]
					mov		eax, dword ptr[x]
					mov		dword ptr[dest], edi		;储存dest
					cmp		eax, dword ptr[w]			;while( x > w)
					jl		REP_DRAW_LINE				;继续画完此行
EXIT_LINE_BLOCK:
					mov		dword ptr[index], ebx		;储存index
EXIT_LINE_OK:
					mov		eax, dword ptr[w]
					shl		eax, 2
					sub		dword ptr[dest], eax
				}
			}
		}
		return true;
	}
	return false;
}
*/
/*
void DrawDecodedImage( LPREALBIN node, int screenx, int screeny, int screenw, int screenh, LPWORD dest, int pitch)
{
	LPWORD srcimage = node->decode_cache;
	w = node->width;
	h = node->height;
	_asm{
		//判断在荧幕上的实际范围
		mov		eax, dword ptr[screenx]
		mov		ecx, dword ptr[screeny]
		add		eax, dword ptr[w]		;ex=screenx+w
		add		ecx, dword ptr[h]		;ey=screeny+h
		mov		ebx, dword ptr[screenw]
		mov		edx, dword ptr[screenh]
		cmp		eax, ebx				;if( ex > (screenw-1))
		jle		ENDIF_1
		mov		eax, ebx				;	ex = screenw - 1
ENDIF_1:
		cmp		ecx, edx				;if( ey > (screenh-1))
		jle		ENDIF_2
		mov		ecx, edx				;	ey = screenh - 1
ENDIF_2:
		dec		eax
		dec		ecx
		mov		dword ptr[ex], eax
		mov		dword ptr[ey], ecx
		mov		eax, dword ptr[screenx]	;if( (sx=screenx) < 0)
		mov		ebx, dword ptr[screeny]	;if( (sy=screeny) < 0)
		xor		ecx, ecx
		cmp		eax, 0
		jge		ENDIF_3
		mov		eax, ecx				;	sx = 0
ENDIF_3:
		cmp		ebx, 0
		jge		ENDIF_4
		mov		ebx, ecx				;	sy = 0
ENDIF_4:
		mov		dword ptr[sx], eax
		mov		dword ptr[sy], ebx
	}
	destrealw = (ex - sx + 1)<<1;
	//计算荧幕上的起点
	dest += (sy*(pitch>>1)+sx);
	srcimage += ((sy-screeny)*w+(sx-screenx));
	w <<= 1;
	_asm{
		mov		edi, dword ptr[dest]
		mov		esi, dword ptr[srcimage]
		mov		ebx, dword ptr[destrealw];搬移的位元数
		pxor	mm7, mm7
BEGIN_LINE:
		mov		eax, edi
		xor		ecx, ecx				;counter
		mov		edx, 32
		and		eax, 1fh
		je		MOVE_32					;目的位址已对齐32 bytes
		sub		edx, eax				;算出要对齐32，要使用多少个位元
		cmp		edx, ebx
		jle		HEAD_REP				;是否小于搬移的宽度
		mov		edx, ebx
HEAD_REP:
		mov		ax, word ptr[esi+ecx]
		cmp		ax, 0
		jz		SKIP_TRANS_HEAD
		mov		word ptr[edi+ecx], ax
SKIP_TRANS_HEAD:
		add		ecx, 2
		cmp		edx, ecx
		jnz		HEAD_REP
MOVE_32:
		lea		eax, [ecx+32]			;预计搬移32 bytes后的位址
		cmp		ebx, eax
		jl		TAIL_REP				;搬移的位元小于32 bytes
BODY_REP:
		movq	mm0, [esi+ecx+0*8]		;搬移32 bytes
		movq	mm1, [esi+ecx+1*8]
		movq	mm2, [edi+ecx+0*8]
		movq	mm3, [edi+ecx+1*8]
		movq	mm4, mm0
		movq	mm5, mm1
		pcmpeqw	mm4, mm7
		pcmpeqw	mm5, mm7
		pand	mm4, mm2
		pand	mm5, mm3
		por		mm4, mm0
		por		mm5, mm1
		movq	[edi+ecx+0*8], mm4
		movq	[edi+ecx+1*8], mm5
		movq	mm0, [esi+ecx+2*8]
		movq	mm1, [esi+ecx+3*8]
		movq	mm2, [edi+ecx+2*8]
		movq	mm3, [edi+ecx+3*8]
		movq	mm4, mm0
		movq	mm5, mm1
		pcmpeqw	mm4, mm7
		pcmpeqw	mm5, mm7
		pand	mm4, mm2
		pand	mm5, mm3
		por		mm4, mm0
		por		mm5, mm1
		movq	[edi+ecx+2*8], mm4
		movq	[edi+ecx+3*8], mm5
		add		eax, 32
		add		ecx, 32
		cmp		ebx, eax
		jge		BODY_REP				;搬移的位元小于32 bytes
TAIL_REP:
		cmp		ebx, ecx
		jle		LINE_EXIT				;搬完了
TAIL_REP_1:
		mov		ax, word ptr[esi+ecx]
		cmp		ax, 0
		jz		SKIP_TRANS_TAIL
		mov		word ptr[edi+ecx], ax
SKIP_TRANS_TAIL:
		add		ecx, 2
		cmp		ebx, ecx
		jg		TAIL_REP_1
LINE_EXIT:
		mov		edx, dword ptr[sy]
		add		edi, dword ptr[pitch]
		inc		edx
		add		esi, dword ptr[w]
		mov		dword ptr[sy], edx
		cmp		edx, dword ptr[ey]
		jle		BEGIN_LINE
		emms
	}
}
#else
*/
unsigned long bitmapnumbertable[MAX_GRAPHICS];


FILE *Realbinfp;

typedef struct tagRGBQUAD {
        unsigned char    rgbBlue;
        unsigned char    rgbGreen;
        unsigned char    rgbRed;
        unsigned char    rgbReserved;
} RGBQUAD;

FILE *Addrbinfp;
ADRNBIN adrnbuff[MAX_GRAPHICS];
#ifdef _READ16BITBMP
AddressBin_s adrntruebuff[MAX_GRAPHICS_24];
void* hAdrntrueFile;
void* hRealtrueFile;
RGBQUAD g_rgbPal[256];
#endif
void initAutoMapColor( char *addrbinfilename )
{
	char *filename = "data\\auto.dat";
	if( readAutoMapColor( filename, addrbinfilename ) == 0 ){
		makeAutoMapColor();
		writeAutoMapColor( filename, addrbinfilename );
	}
}

void makeAutoMapColor( void )
{
	unsigned int i;
	int chgTbl[20] = {
		166,  61,  80, 112,  96, 189, 112,  60, 164, 164,
		111,   0,   0,   0,   0,   0,   0,   0,   0,   0	};
	for( i = 0; i < MAX_GRAPHICS; i++ ){
		if( adrnbuff[i].attr.bmpnumber != 0 ){
			if( 100 <= adrnbuff[i].attr.bmpnumber && adrnbuff[i].attr.bmpnumber <= 19999 ){
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = getAutoMapColor( adrnbuff[i].bitmapno );
			}else if( 60 <= adrnbuff[i].attr.bmpnumber && adrnbuff[i].attr.bmpnumber <= 79 ){
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = (unsigned char)chgTbl[adrnbuff[i].attr.bmpnumber-60];
			}else{
				autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = 0;
			}
		}else{
			autoMapColorTbl[adrnbuff[i].attr.bmpnumber] = 0;
		}
	}
}

int writeAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *wfp, *rfp;
	int rfh;
	struct stat statBuf;
	int adrnNo;
	unsigned int adrnTime;
	unsigned short autoMapColorVersion = 4;	
	char *tmpStr;

	// adrn.bin???????????
	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??????
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = fileno( rfp );
	if( fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = statBuf.st_ctime;
	fclose( rfp );
	// ???????
	if( (wfp = fopen( wFName, "wb" )) == NULL )
		return 0;
	fwrite( &autoMapColorVersion, sizeof( autoMapColorVersion ), 1, wfp );
	fwrite( &adrnNo, sizeof( adrnNo ), 1, wfp );
	fwrite( &adrnTime, sizeof( adrnTime ), 1, wfp );
	fwrite( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, wfp );
	fclose( wfp );
	return 1;
}

int readAutoMapColor( char *wFName, char *addrbinfilename )
{
	FILE *rfp;
	int rfh;
	struct stat statBuf;
	int adrnNo, rAdrnNo;
	unsigned int adrnTime, rAdrnTime;
	unsigned short autoMapColorVersion = 4, rAutoMapColorVersion;
	char *tmpStr;
	// adrn.bin???????
	tmpStr = strstr( addrbinfilename, "adrn" );
	if( tmpStr == NULL )
		return 0;	// ??䥺??????
	if( tmpStr[4] == '.' )
		adrnNo = 0;	// ?????????
	else{
		adrnNo = -1;
		sscanf( tmpStr, "adrn_%d.bin", &adrnNo );
		if( adrnNo < 0 )
			return 0;
	}
	// adrn.bin??
	if( (rfp = fopen( addrbinfilename, "rb" )) == NULL )
		return 0;
	rfh = fileno( rfp );
	if( fstat( rfh, &statBuf ) < 0 ){
		fclose( rfp );
		return 0;
	}
	adrnTime = statBuf.st_ctime;
	fclose( rfp );
	if( (rfp = fopen( wFName, "rb" )) == NULL )
		return 0;

	if( fread( &rAutoMapColorVersion, sizeof( rAutoMapColorVersion ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( autoMapColorVersion != rAutoMapColorVersion ){
		fclose( rfp );
		return 0;
	}
	// adrn.bin????????
	if( fread( &rAdrnNo, sizeof( rAdrnNo ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnNo != rAdrnNo ){
		fclose( rfp );
		return 0;
	}
	// adrn.bin?????
	if( fread( &rAdrnTime, sizeof( rAdrnTime ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	if( adrnTime != rAdrnTime ){
		fclose( rfp );
		return 0;
	}
	if( fread( &autoMapColorTbl, sizeof( autoMapColorTbl ), 1, rfp ) != 1 ){
		fclose( rfp );
		return 0;
	}
	fclose( rfp );
	return true;
}

bool initRealbinFileOpen(char *realbinfilename, char *addrbinfilename)
{
	ADRNBIN tmpadrnbuff;
	if ((Addrbinfp = fopen(addrbinfilename, "rb"))==NULL)
		return false;

	if ((Realbinfp = fopen(realbinfilename, "rb"))==NULL)
		return false;

	//adrn.bin
	while(!feof(Addrbinfp)){
		fread(&tmpadrnbuff, sizeof(tmpadrnbuff), 1, Addrbinfp);
		adrnbuff[tmpadrnbuff.bitmapno] = tmpadrnbuff;


		//if (tmpadrnbuff.bitmapno == 437719)
		//{
		//	int debug = 0;
		//}
		if( tmpadrnbuff.attr.bmpnumber != 0 ){
			if( (12802 <= tmpadrnbuff.attr.bmpnumber && tmpadrnbuff.attr.bmpnumber <= 12811)
			 || (10132 <= tmpadrnbuff.attr.bmpnumber && tmpadrnbuff.attr.bmpnumber <= 10136) ){
				adrnbuff[tmpadrnbuff.bitmapno].attr.hit =
					300 + (adrnbuff[tmpadrnbuff.bitmapno].attr.hit % 100);
			}
			if( tmpadrnbuff.attr.bmpnumber<=33 && tmpadrnbuff.bitmapno>230000){//防堵魔法图号覆盖声音的bug
				continue;
			}
			bitmapnumbertable[tmpadrnbuff.attr.bmpnumber] = tmpadrnbuff.bitmapno;
		}else
			bitmapnumbertable[tmpadrnbuff.attr.bmpnumber] = 0;
	}
	fclose(Addrbinfp);
	return true;
}

void cleanupRealbin(void)
{

	fclose(Realbinfp);


#ifdef _READ16BITBMP
	//CloseHandle(hRealtrueFile);
#endif
}

bool realGetPos(U4 GraphicNo , S2 *x , S2 *y)
{
#ifndef _READ16BITBMP
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*x=0;*y=0;return false;}
	*x = adrnbuff[GraphicNo].xoffset;
	*y = adrnbuff[GraphicNo].yoffset;
#else
	if(GraphicNo < 0) {*x=0;*y=0;return false;}
	if(GraphicNo >= OLD_GRAPHICS_START){
		if(GraphicNo > MAX_GRAPHICS) {*x=0;*y=0;return false;}
		*x = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].xoffset;
		*y = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].yoffset;
	}
	else{
		*x = adrnbuff[GraphicNo].xoffset;
		*y = adrnbuff[GraphicNo].yoffset;
	}
#endif
	return true;
}

bool realGetWH(U4 GraphicNo , S2 *w , S2 *h)
{
#ifndef _READ16BITBMP
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*w=0;*h=0;return false;}
	*w = adrnbuff[GraphicNo].width;
	*h = adrnbuff[GraphicNo].height;
#else
	if(GraphicNo < 0){*w=0;*h=0;return false;}
	if(GraphicNo >= OLD_GRAPHICS_START){
		if(GraphicNo > MAX_GRAPHICS) {*w=0;*h=0;return false;}
		*w = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].width;
		*h = adrntruebuff[GraphicNo - OLD_GRAPHICS_START].height;
	}
	else{
		*w = adrnbuff[GraphicNo].width;
		*h = adrnbuff[GraphicNo].height;
	}
#endif

	return true;
}

bool realGetHitPoints(U4 GraphicNo , S2 *HitX , S2 *HitY)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*HitX=0;*HitY=0;return false;}

	*HitX = adrnbuff[GraphicNo].attr.atari_x;
	*HitY = adrnbuff[GraphicNo].attr.atari_y;

	return true;
}

bool realGetHitFlag(U4 GraphicNo , S2 *Hit)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){
		*Hit=0;
		return false;
	}

	if ((GraphicNo >= 369715 && GraphicNo <= 369847) || GraphicNo == 369941)//强制地表可走
		*Hit = 1;
	else if (GraphicNo >= 369641 && GraphicNo <= 369654)
		*Hit = 1;
	else
		*Hit = (adrnbuff[GraphicNo].attr.hit % 100);

	return true;
}

bool realGetPrioType(U4 GraphicNo , S2 *prioType)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){
		*prioType=0;
		return false;
	}

	*prioType = (adrnbuff[GraphicNo].attr.hit / 100);
	return true;
}

bool realGetHeightFlag(U4 GraphicNo , S2 *Height)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){*Height=0;return false;}
	*Height = adrnbuff[GraphicNo].attr.height;

	return true;
}

int realGetSoundEffect(U4 GraphicNo)
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS)return false;
	return adrnbuff[bitmapnumbertable[GraphicNo]].attr.effect1;

}

int realGetWalkSoundEffect(U4 GraphicNo )
{
	if(GraphicNo<0 || GraphicNo>=MAX_GRAPHICS){return false;}
	return adrnbuff[GraphicNo].attr.effect2;

}

bool realGetNo( U4 CharAction , U4 *GraphicNo )
{
#ifndef _READ16BITBMP
	if(CharAction<0 || CharAction>=MAX_GRAPHICS){*GraphicNo=0;return false;}
	*GraphicNo = bitmapnumbertable[CharAction];
	return true;
#else
	if(CharAction < 0){
		*GraphicNo = 0;
		return false;
	}
	if(CharAction >= OLD_GRAPHICS_START){
		if(CharAction >= MAX_GRAPHICS){
			*GraphicNo = 0;
			return false;
		}
		*GraphicNo = CharAction;
		return true;
	}
	else *GraphicNo = bitmapnumbertable[CharAction];
	return true;
#endif
}

bool realGetBitmapNo( int num )
{
#ifndef _READ16BITBMP
	if( num < 0 || num >= MAX_GRAPHICS ) return -1;
	return adrnbuff[num].attr.bmpnumber;
#else
	if(num < 0) return -1;
	if(num >= OLD_GRAPHICS_START){
		if(num >= MAX_GRAPHICS) return -1;
		return adrntruebuff[num - OLD_GRAPHICS_START].bitmapno;
	}
	else return adrnbuff[num].attr.bmpnumber;
#endif
}

///////////////////////////////////////////////////////////////////??????????
#define REALGETIMAGEMAXSIZE 1600*1600
unsigned char g_realgetimagebuf[REALGETIMAGEMAXSIZE];
unsigned char g_realgetimagebuf2[REALGETIMAGEMAXSIZE];

//???????????????
bool realGetImage( int graphicNo, unsigned char **bmpdata, int *width, int *height)
{

	ADRNBIN adrdata;
	if(graphicNo<0 || graphicNo>=MAX_GRAPHICS)return false;
	adrdata=adrnbuff[graphicNo];

	fseek(Realbinfp, adrdata.adder, SEEK_SET);//real.bin??????????? 
	if( fread(&g_realgetimagebuf, adrdata.size, 1, Realbinfp ) != 1 )
		return false;

	unsigned int len;
	*bmpdata = g_realgetimagebuf2;
	if( decoder( g_realgetimagebuf, bmpdata,
			(unsigned int*)width, (unsigned int*)height, &len ) == NULL ){
		return false;
	}
	return true;
}

#ifdef _READ16BITBMP
// open realtrue.bin and adrntrue.bin file
int InitRealTruebinFileOpen(char *szRealTrueBinFileName,char *szAdrnTruebinFileName)
{
	//bool bReadReturn;
	//AddressBin_s Addr;
	//unsigned long dwReadByte;

	//ZeroMemory(adrntruebuff,sizeof(adrntruebuff));
	//// 开档
	//hAdrntrueFile = CreateFile(szAdrnTruebinFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	//if(hAdrntrueFile == INVALID_HANDLE_VALUE) return -1;
	//hRealtrueFile = CreateFile(szRealTrueBinFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	//if(hRealtrueFile == INVALID_HANDLE_VALUE){
	//	CloseHandle(hAdrntrueFile);
	//	return -2;
	//}
	//
	//while(1){
	//	// 把档案内的资料读入
	//	bReadReturn = ReadFile(hAdrntrueFile,&Addr,sizeof(AddressBin_s),&dwReadByte,NULL);
	//	// 资料读完了
	//	if(bReadReturn && dwReadByte == 0) break;
	//	// 错误
	//	if(dwReadByte == 0) break;
	//	memcpy(&adrntruebuff[Addr.bitmapno],&Addr,sizeof(AddressBin_s));
	//}

	//CloseHandle(hAdrntrueFile);
	return 0;
}

// 传入的BmpNo 值必须是减去 OLD_GRAPHICS_START 后的值,才能在全彩的图档里找到图
bool Read16BMP(int BmpNo,unsigned char **BmpData,int *width,int *height,unsigned char **AlphaData,bool *useAlpha)
{
	bool bRet = true,bReadReturn;
	AddressBin_s *pAddr;
	unsigned char *pBmpData;
	unsigned int len,iw = 0,ih = 0;
	unsigned long dwReadByte;

	if(BmpNo > MAX_GRAPHICS_24) return false;
	pAddr = &adrntruebuff[BmpNo];
	// 移到要读取的图档资料位置上
	//SetFilePointer(hRealtrueFile,pAddr->adder,NULL,0);
	pBmpData = (unsigned char*)malloc(pAddr->size);
#ifdef _DEBUG
	g_iMallocCount++;
#endif
	if(pBmpData == NULL) return false;
	else{
		memset(g_rgbPal,0,sizeof(g_rgbPal));
		// 先读入色盘资料
		//bReadReturn = ReadFile(hRealtrueFile,g_rgbPal,pAddr->palSize+sizeof(RGBQUAD),&dwReadByte,NULL);
		// 读档失败
		if(bReadReturn && dwReadByte == 0) bRet = false;
		// 把图档资料读入
		//bReadReturn = ReadFile(hRealtrueFile,pBmpData,pAddr->size,&dwReadByte,NULL);
		// 读档失败
		if(bReadReturn && dwReadByte == 0) bRet = false;
		else{
			*BmpData = g_realgetimagebuf2;
			if(decoder(pBmpData,BmpData,(unsigned int*)width,(unsigned int*)height,&len) == NULL) bRet = false;
		}
		FREE(pBmpData);
#ifdef _DEBUG
		g_iMallocCount--;
#endif
		// 把alpha资料读入
		*useAlpha = false;
		if(pAddr->alpha_size > 0){
			pBmpData = (unsigned char*)malloc(pAddr->alpha_size);
#ifdef _DEBUG
			g_iMallocCount++;
#endif
			if(pBmpData == NULL) return false;
			//bReadReturn = ReadFile(hRealtrueFile,pBmpData,pAddr->alpha_size,&dwReadByte,NULL);
			// 读档失败
			if(bReadReturn && dwReadByte == 0) bRet = false;
			else{
				*AlphaData = g_realgetimagebuf;
				if(decoder(pBmpData,AlphaData,&iw,&ih,&len) == NULL) bRet = false;
			}
			FREE(pBmpData);
#ifdef _DEBUG
			g_iMallocCount--;
#endif
			*useAlpha = true;
		}
	}

	return bRet;
}
#endif
