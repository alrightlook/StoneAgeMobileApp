#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<direct.h>
#include<math.h>

#include "../systeminc/version.h"
#include"../systeminc/system.h"
#include"../systeminc/map.h"
#include"../systeminc/anim_tbl.h"
#include"../systeminc/pc.h"
#include"../systeminc/netproc.h"
#include"../systeminc/character.h"
#include"../systeminc/loadrealbin.h"
#include"../systeminc/loadsprbin.h"
#include"../systeminc/netmain.h"
#include"../systeminc/lssproto_cli.h"
#include"../systeminc/battleProc.h"
#include"../systeminc/t_music.h"
#include"../systeminc/field.h"
#include"../systeminc/login.h"
#include"../systeminc/menu.h"
#include"../systeminc/battleMenu.h"
#include "../systeminc/handletime.h"
#include "../systeminc/savedata.h"
#include "../systeminc/tool.h"

// ?????�L��???(1 ... ?�L��?)
const short fastDrawTile = 1;

short fastDrawTileFlag = 1;
int amountXFastDraw = 0, amountYFastDraw = 0;
int nowXFastDraw, nowYFastDraw;
int baseXFastDraw, baseYFastDraw;
int amountXFastDraw2 = 0, amountYFastDraw2 = 0;
int nowXFastDraw2, nowYFastDraw2;
int baseXFastDraw2, baseYFastDraw2;
#define SEARCH_AREA		11		// �եޥåפΥ����å����죨����餫��ξ��x��

enum
{
	MOUSE_CURSOR_MODE_NORMAL,
	MOUSE_CURSOR_MODE_MOVE
};

// ??????��?
char nowFloorName[FLOOR_NAME_LEN+1];
int nowFloor;
int nowFloorGxSize, nowFloorGySize;
int nowGx, nowGy;
float nowX = (float)nowGx*GRID_SIZE, nowY = (float)nowGy * GRID_SIZE;
float nowVx, nowVy, nowSpdRate;
int nextGx, nextGy;
int oldGx = -1, oldGy = -1;
int oldNextGx = -1, oldNextGy = -1;
int mouseMapGx, mouseMapGy;
int mouseMapX, mouseMapY;
#define MOVE_MODE_CHANGE_TIME	1000
short mouseCursorMode = MOUSE_CURSOR_MODE_NORMAL;
int mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2;
int mapAreaWidth, mapAreaHeight;

unsigned short tile[MAP_X_SIZE * MAP_Y_SIZE];	// �ޥåפΥ�����
unsigned short parts[MAP_X_SIZE * MAP_Y_SIZE];	// �ޥåפΥѩ`��
unsigned short event[MAP_X_SIZE * MAP_Y_SIZE];	// ���٥��
unsigned short hitMap[MAP_X_SIZE * MAP_Y_SIZE];	// �ޥåפ������ж�



// �ޥåץǩ`���o���r���Ƅ��W�ӄI����
int mapEmptyFlag;
short mapEmptyDir;
int mapEmptyGx, mapEmptyGy;
short getMapAreaX1[2], getMapAreaY1[2], getMapAreaX2[2], getMapAreaY2[2];
short getMapAreaCnt;
unsigned int mapEmptyStartTime;


// �ե���������줿��
bool floorChangeFlag = false;


// ������r�˱�Ҫ�ʄI����
bool loginFlag;

// ��`�ץ��ե����Ȥ�2�ؤǌg�Ф��ʤ�����Υե饰
bool warpEffectFlag = false;
bool warpEffectStart = false;	// ��`�ץ��ե������_ʼ
bool warpEffectOk = false;		// ��`�ץե��`�ɥ���ϣ�

// �ޥåפ�עҕ���
float viewPointX;
float viewPointY;
int viewOffsetX = SCREEN_WIDTH_CENTER;
int viewOffsetY = SCREEN_HEIGHT_CENTER;

// �ƄӄI��
#define MOVE_CLICK_WAIT_TIME	250		// �ߥ���
short moveAddTbl[8][2] =
{
	{ -1,  1 }, // 0
	{ -1,  0 }, // 1
	{ -1, -1 }, // 2
	{  0, -1 }, // 3
	{  1, -1 }, // 4
	{  1,  0 }, // 5
	{  1,  1 }, // 6
	{  0,  1 }  // 7
};
bool moveStackFlag = false;
int moveStackGx, moveStackGy;
#define MOVE_MAX	100
short moveRoute[MOVE_MAX];
short moveRouteCnt = 0;
short moveRouteGx[MOVE_MAX];
short moveRouteGy[MOVE_MAX];
char moveRouteDir[MOVE_MAX];

#define MOVE_MAX2	2			// ���`�Ф��ͤ��ƄӽU·�����
short moveRouteCnt2 = 0;
short moveRoute2[MOVE_MAX2];

short moveLastDir = -1;

// ??????
bool mouseLeftCrick = false;
bool mouseLeftOn = false;
bool mouseRightCrick = false;
bool mouseRightOn = false;
#ifdef _MOUSE_DBL_CLICK
bool mouseDblRightOn = false;
#endif
unsigned int mouseLeftPushTime;
unsigned int beforeMouseLeftPushTime;

// ????????����?�I???�e?
// ����顢�ѩ`�Ĥα�ʾ�����λ�Q����

enum
{
	CHAR_PARTS_PRIO_TYPE_CHAR,
	CHAR_PARTS_PRIO_TYPE_PARTS,
	CHAR_PARTS_PRIO_TYPE_ANI
};

typedef struct TAG_CHAR_PARTS_PRIORITY
{
	unsigned int graNo;
	int x, y;
	int dx, dy;
	int depth;
	float mx, my;
	short type;
	TAG_CHAR_PARTS_PRIORITY *pre;
	TAG_CHAR_PARTS_PRIORITY *next;
} CHAR_PARTS_PRIORITY;

#define MAX_CHAR_PRIO_BUF	2048
CHAR_PARTS_PRIORITY charPrioBufTop;
CHAR_PARTS_PRIORITY charPrioBuf[MAX_CHAR_PRIO_BUF];
int charPrioCnt;



// ���󥫥���ȄI����
short nowEncountPercentage;	// ���󥫥������
short nowEncountExtra;		// ���󥫥�������a����
short minEncountPercentage;	// ��С���󥫥������
short maxEncountPercentage;	// ��󥨥󥫥������
short sendEnFlag;			// EN�ץ�ȥ����ͤä��ե饰
short encountNowFlag;		// ���󥫥�����Ф�

// ���٥�ȄI����
int eventId = 0;			// ���٥�ȹ���ID
short eventWarpSendFlag;	// ��`�ץ��٥�����ťե饰
short eventWarpSendId;		// ��`�ץ��٥�����ŕr��ID
short eventEnemySendFlag;	// �̶����󥫥���ȥ��٥�����ťե饰
short eventEnemySendId;		// �̶����󥫥���ȥ��٥�����ŕr��ID
short eventEnemyFlag;		// �̶����˥��󥫥���Ȥ����飱
short etcEventFlag = 0;
short vsLookFlag;			// ?�P????

// �Զ�ӳ��
#define AUTO_MAPPING_W		54
#define AUTO_MAPPING_H		54
bool autoMappingInitFlag = true;
unsigned char autoMappingBuf[AUTO_MAPPING_H][AUTO_MAPPING_W];
/*
#define AUTO_MAPPING_SEE_W		30
#define AUTO_MAPPING_SEE_H		30
unsigned int readMapAfterFrame = 10000;		// �ޥå��i���z����νU�^�ե�`����
unsigned short autoMapSeeFlagBuf[AUTO_MAPPING_SEE_H*AUTO_MAPPING_SEE_W];
bool autoMapSeeFlag = false;
int autoMapSeeFloor;
int autoMapSeeGx, autoMapSeeGy;
*/

// ����????????
//   1 ... ����??????
//   0 ... �G��
int transmigrationEffectFlag;

// ����?????????????????????��?��?
//   1 ... ????��??
//   0 ... ��?
int transEffectPaletteStatus;

// ����??????????????��?????
// ??????��????�r????��??
unsigned int transEffectPaletteAfterWaitTime;


// �ƄӄI��
void onceMoveProc(void);
void partyMoveProc(void);
void getPartyTbl(void);

// �ޥåץ���å���I��
//#define MAP_CACHE_PROC
#ifdef MAP_CACHE_PROC

#define MAX_MAP_CACHE_SIZE	3		// ����å��夹�����ե���
#define MAP_CACHE_X_SIZE	800		// ���ե��Σط������
#define MAP_CACHE_Y_SIZE	1200		// ���ե��Σٷ������
#define MAP_CACHE_X_BYTE	((MAP_CACHE_X_SIZE+7)/8)
#define MAP_CACHE_Y_BYTE	MAP_CACHE_Y_SIZE
int mapCacheFloorNo[MAX_MAP_CACHE_SIZE];
int mapCacheFloorGxSize[MAX_MAP_CACHE_SIZE];
int mapCacheFloorGySize[MAX_MAP_CACHE_SIZE];
unsigned char mapCacheFlag[MAX_MAP_CACHE_SIZE][MAP_CACHE_X_BYTE*MAP_CACHE_Y_BYTE];
int mapCacheUse;
unsigned int mapCacheLastTime[MAX_MAP_CACHE_SIZE];

void initMapCache(void);
bool checkMapCache(int, int, short *, int, int, int, int);
void clearMapCacheFlag(int);
bool checkMapCacheFlag(int, int, int, int, int, int, int);
bool checkMapCacheEvent(int, int, int, int);
#endif
bool readMap(int, int, int, int, int, unsigned short *, unsigned short *, unsigned short *);
void readHitMap(int, int, int, int, unsigned short *, unsigned short *, unsigned short *, unsigned short *);
void getRouteMap(void);
void shiftRouteMap(void);
void shiftRouteMap2(void);
int getDirData(int, int, int, int);
void checkAreaLimit(short *, short *, short *, short *);
void turnAround(void);
void turnAround2(int);
bool checkPrioPartsVsChar(CHAR_PARTS_PRIORITY *, CHAR_PARTS_PRIORITY *);
void insertCharPartsPrio(CHAR_PARTS_PRIORITY *, CHAR_PARTS_PRIORITY *);
void addCharPartsPrio(CHAR_PARTS_PRIORITY *, CHAR_PARTS_PRIORITY *);
void delCharPartsPrio(CHAR_PARTS_PRIORITY *);
bool createAutoMap(int, int, int);

///////////////////////////////////////////////////////////////////////////
// �ޥåׄI����ڻ�
void initMap(void)
{
#ifndef _DEBUG
	nowFloor = 0;
	nowFloorGxSize = 0;
	nowFloorGySize = 0;
	nowGx = 0;
	nowGy = 0;
	nowX = 0;
	nowY = 0;
	nextGx = 0;
	nextGy = 0;
	oldGx = 0, oldGy = 0;
	oldNextGx = 0, oldNextGy = 0;

	loginFlag = true;
#else
	if (offlineFlag)
	{
		setMap(400, 20, 20);
		nowFloorGxSize = 100;
		nowFloorGySize = 100;
	}
	else
	{
		nowFloor = 0;
		nowFloorGxSize = 0;
		nowFloorGySize = 0;
		nowGx = 0;
		nowGy = 0;
		nowX = 0;
		nowY = 0;
		nextGx = 0;
		nextGy = 0;
		oldGx = 0, oldGy = 0;
		oldNextGx = 0, oldNextGy = 0;

		loginFlag = true;
	}
#endif

	mapAreaX1 = nowGx+MAP_TILE_GRID_X1;
	mapAreaY1 = nowGy+MAP_TILE_GRID_Y1;
	mapAreaX2 = nowGx+MAP_TILE_GRID_X2;
	mapAreaY2 = nowGy+MAP_TILE_GRID_Y2;

	if (mapAreaX1 < 0)
		mapAreaX1 = 0;
	if (mapAreaY1 < 0)
		mapAreaY1 = 0;
	if (mapAreaX2 > nowFloorGxSize)
		mapAreaX2 = nowFloorGxSize;
	if (mapAreaY2 > nowFloorGySize)
		mapAreaY2 = nowFloorGySize;

	mapAreaWidth  = mapAreaX2 - mapAreaX1;
	mapAreaHeight = mapAreaY2 - mapAreaY1;
	nowVx = 0;
	nowVy = 0;
	nowSpdRate = 1;
	moveRouteCnt = 0;
	moveRouteCnt2 = 0;
	moveStackFlag = false;
	mapEmptyFlag = false;
	mouseCursorMode = MOUSE_CURSOR_MODE_NORMAL;
	nowEncountPercentage = 0;
	nowEncountExtra = 0;
	sendEnFlag = 0;
	encountNowFlag = 0;
	eventWarpSendFlag = 0;
	eventEnemySendFlag = 0;
	eventEnemyFlag = 0;
	etcEventFlag = 0;
	mouseLeftPushTime = 0;
	beforeMouseLeftPushTime = 0;
	warpEffectFlag = false;
	warpEffectStart = false;
	warpEffectOk = false;
//	autoMapSeeFlag = false;
	eventWarpSendId = -1;
	eventEnemySendId = -1;
	moveLastDir = -1;
	amountXFastDraw = 0;
	amountYFastDraw = 0;
	nowXFastDraw = 0;
	nowYFastDraw = 0;
	baseXFastDraw = 0;
	baseYFastDraw = 0;
	amountXFastDraw2 = 0;
	amountYFastDraw2 = 0;
	nowXFastDraw2 = 0;
	nowYFastDraw2 = 0;
	baseXFastDraw2 = 0;
	baseYFastDraw2 = 0;
	vsLookFlag = 0;
	fastDrawTileFlag = 1;
	transmigrationEffectFlag = 0;
	transEffectPaletteStatus = 0;
	transEffectPaletteAfterWaitTime = 0;
#ifdef MAP_CACHE_PROC
	initMapCache();
#endif
}

///////////////////////////////////////////////////////////////////////////
// ?????????
void resetMap(void)
{
	// ?�h????�h�I????????
	nowGx = (int)(nowX / GRID_SIZE);
	nowGy = (int)(nowY / GRID_SIZE);
	nextGx = nowGx;
	nextGy = nowGy;
	nowX = (float)nowGx * GRID_SIZE;
	nowY = (float)nowGy * GRID_SIZE;
	oldGx = -1;
	oldGy = -1;
	oldNextGx = -1;
	oldNextGy = -1;
	mapAreaX1 = nowGx + MAP_TILE_GRID_X1;
	mapAreaY1 = nowGy + MAP_TILE_GRID_Y1;
	mapAreaX2 = nowGx + MAP_TILE_GRID_X2;
	mapAreaY2 = nowGy + MAP_TILE_GRID_Y2;

	if (mapAreaX1 < 0)
		mapAreaX1 = 0;
	if (mapAreaY1 < 0)
		mapAreaY1 = 0;
	if (mapAreaX2 > nowFloorGxSize)
		mapAreaX2 = nowFloorGxSize;
	if (mapAreaY2 > nowFloorGySize)
		mapAreaY2 = nowFloorGySize;

	mapAreaWidth  = mapAreaX2 - mapAreaX1;
	mapAreaHeight = mapAreaY2 - mapAreaY1;
	nowVx = 0;
	nowVy = 0;
	nowSpdRate = 1;
	viewPointX = nowX;
	viewPointY = nowY;
	moveRouteCnt = 0;
	moveRouteCnt2 = 0;
	moveStackFlag = false;
	mouseCursorMode = MOUSE_CURSOR_MODE_NORMAL;
	mouseLeftPushTime = 0;
	beforeMouseLeftPushTime = 0;
//	autoMapSeeFlag = false;
}

///////////////////////////////////////////////////////////////////////////
// �¤����ޥåפ�������ޥåץե��������ɤ����������{����
void createMap(int fl, int maxx, int maxy)
{
	FILE *fp;
	char floorname[255];
	int i = 0, j, k, ox, oy;
	short l = 0;

	if (fl == 0)
		return;

	sprintf(floorname, "/sdcard/jerrysa/map/%d.dat", fl);
	if ((fp = fopen(floorname, "rb")) == NULL)
	{
		// ?????��????��??
		// �ե����뤬�o���Τ����ɤ���
		//_mkdir("map");
		if ((fp = fopen(floorname, "wb")) == NULL)
			return;
		fseek(fp, 0, SEEK_SET);
		fwrite(&maxx, sizeof(int), 1, fp);
		fwrite(&maxy, sizeof(int), 1, fp);
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < maxx; j++)
			{
				for (k=0; k < maxy; k++)
				{
					fwrite(&l, sizeof(short), 1, fp);
				}
			}
		}
		fclose(fp);
	}
	else
	{
		// �ե����뤬����r���������`���ʤ�ޥåפ����äƤ�
		fread(&ox, sizeof(int), 1, fp);
		fread(&oy, sizeof(int), 1, fp);
		if (maxx != ox || maxy != oy)
		{
			fclose(fp);
			// ���������`���ΤǼȴ�ե��������������Ҏ�ˤ���
			if ((fp = fopen(floorname, "wb")) == NULL)
				return;
			fwrite(&maxx, sizeof(int), 1, fp);
			fwrite(&maxy, sizeof(int), 1, fp);
			for (i = 0; i < 3; i++)
			{
				for (j = 0; j < maxx; j++)
				{
					for (k = 0; k < maxy; k++)
					{
						fwrite(&l, sizeof(short), 1, fp);
					}
				}
			}
		}
		fclose (fp);
	}
}


// �����ڤΥ��٥�Ȥ����ͤ���Ƥ������������������
void setEventMemory(int x, int y, unsigned short ev)
{
	event[(y - mapAreaY1) * mapAreaWidth + (x - mapAreaX1)] = ev;
}


// �ޥåץǩ`���Ε����z��
//
//   ����å�����(x1,y1)-(x2,y2)�ι���Υޥåץǩ`��������z��
//
//     tile, parts, event �Ͼ��Υ��ꘋ��ˤʤäƤ���
//
bool writeMap(int floor, int x1, int y1, int x2, int y2, unsigned short *tile , unsigned short *parts, unsigned short *event)
{
	FILE *fp;
	char filename[255];
	int fWidth, fHeight, fOffset, mWidth, width, height, fx, fy, mx, my, len, len2, i, j;
	

	// �i���z�ߥե�����������
	sprintf(filename, "/sdcard/jerrysa/map/%d.dat", floor);

	// ????????
	if ((fp = fopen(filename, "rb+")) == NULL)
	{
		// ʧ������Τ�����ǥޥåץե�������o������
		// ������ե����������
		//_mkdir("map");
		// �i���z�ߥǩ`��������
		fp = fopen(filename, "wb");
		fclose(fp);
		// �i���z�ߥ�`�ɤǤ⤦һ�ȥ��`�ץ�
		if ((fp = fopen(filename, "rb+")) == NULL)
			return false;
	}

	fseek(fp, 0, SEEK_SET);
	fread(&fWidth,  sizeof(int), 1, fp);
	fread(&fHeight, sizeof(int), 1, fp);
	mWidth = x2 - x1;
	width = mWidth;
	height = y2 - y1;
	mx = 0;
	fx = x1;
	if (x1 < 0)
	{
		width += x1;
		fx = 0;
		mx -= x1;
	}
	if (x2 > fWidth)
		width -= (x2 - fWidth);
	my = 0;
	fy = y1;
	if (y1 < 0)
	{
		height += y1;
		fy = 0;
		my -= y1;
	}
	if (y2 > fHeight)
		height -= (y2 - fHeight);
	fOffset = sizeof(int) * 2;
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fwrite(&tile[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fwrite(&parts[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}	
	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		// �i���z��?Ҋ���ե饰�����Ƥ�
		for (j = 0; j < width; j++)
		{
			event[len2+j] |= (MAP_SEE_FLAG | MAP_READ_FLAG);
			if (nowFloor == floor && (mapAreaX1 <= x1 + j && x1 + j < mapAreaX2 && mapAreaY1 <= y1 + i && y1 + i < mapAreaY2))
				// �����ڤΥ��٥�Ȥ����ͤ���Ƥ������������������
				setEventMemory(x1 + j, y1 + i, event[len2 + j]);
		}
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fwrite(&event[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fclose (fp);

	return true;
}


// �ޥåץǩ`�����i���z��
//
//   ����å�����(x1,y1)-(x2,y2)�ι���Υޥåץǩ`�����i���z��
//
bool readMap(int floor, int x1, int y1, int x2, int y2, unsigned short *tile, unsigned short *parts, unsigned short *event)
{
	FILE *fp;
	char filename[255];
#ifdef _FIX_DEL_MAP           // WON ������ҳ��ͼ
	char list[10];
#endif
	int fWidth, fHeight, fOffset, mWidth, width, height, fx, fy, mx, my, len, len2, i;    

	// �i���z�ߥե�����������
	sprintf(filename, "/sdcard/jerrysa/map/%d.dat", floor);

	// �ե����륪�`�ץ�
	if ((fp = fopen(filename, "rb")) == NULL)
	{
#ifdef _FIX_DEL_MAP	//andy_add
		memset(tile, 193, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
		//memset(parts, 193, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
#else
		// ������ե����������
		_mkdir("map");
		// �i���z�ߥǩ`��������
		fp = fopen(filename, "wb");     
		fclose(fp);
		// �i���z�ߥ�`�ɤǤ⤦һ�ȥ��`�ץ�
#endif
		// ��??????????�T????
		if ((fp = fopen(filename, "rb")) == NULL)
			return false;
	}

#ifdef _FIX_DEL_MAP           // WON ������ҳ��ͼ    
    fseek(fp, 0, SEEK_SET);
	if (!fread(list, sizeof(char), 1, fp)) {   // ��ҳ���ͼ�ͼ���
		//andy_add
		memset(tile, 193, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
		//memset(parts, 193, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
         fclose(fp);
         //lssproto_DM_send(sockfd);
         return true;
	}
#endif

	memset(tile, 0, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
	memset(parts, 0, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
	memset(event, 0, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));
	fseek(fp, 0, SEEK_SET);
	fread(&fWidth,  sizeof(int), 1, fp);
	fread(&fHeight, sizeof(int), 1, fp);
	mWidth = x2 - x1;
	width = mWidth;
	height = y2 - y1;
	mx = 0;
	fx = x1;
	if (x1 < 0)
	{
		width += x1;
		fx = 0;
		mx -= x1;
	}
	if (x2 > fWidth)
		width -= (x2 - fWidth);
	my = 0;
	fy = y1;
	if (y1 < 0)
	{
		height += y1;
		fy = 0;
		my -= y1;
	}
	if (y2 > fHeight)
		height -= (y2 - fHeight);

	fOffset = sizeof(int) * 2;
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&tile[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&parts[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&event[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fclose (fp);

	return true;
}


//
// �ޥåױ�ʾλ���O��
//
void setMap(int floor, int gx, int gy)
{
	nowFloor = floor;
	setWarpMap(gx, gy);
}
//
// �ޥå�λ�å�`�׉��
//
void setWarpMap(int gx, int gy)
{
	nowGx = gx;
	nowGy = gy;
	nowX = (float)nowGx * GRID_SIZE;
	nowY = (float)nowGy * GRID_SIZE;
	nextGx = nowGx;
	nextGy = nowGy;
	nowVx = 0;
	nowVy = 0;
	nowSpdRate = 1;
	oldGx = -1;
	oldGy = -1;
	oldNextGx = -1;
	oldNextGy = -1;
	viewPointX = nowX;
	viewPointY = nowY;
	wnCloseFlag = 1;	// ���å�����ɥ����]����
#ifdef _AniCrossFrame	   // Syu ADD �������ι���������
	extern void crossAniRelease();
	crossAniRelease();
#endif
#ifdef _SURFACE_ANIM       //ROG ADD ��̬����
	extern void ReleaseSpecAnim ();
	ReleaseSpecAnim();
#endif
}

#ifdef _NEWDRAWBATTLEMAP		   // (���ɿ���) Syu ADD �Զ�����BattleMap
//ROG ADD �Զ�����
typedef struct{
	int x;
	int y;
	float mx;
	float my;
	int bmpNO;
}PARTS;

typedef int CMPFUNC(const void * , const void *);
int sort_parts(PARTS *ptc, PARTS *ptp)
{
	S2 w, h;
	if (ptc->mx > ptp->mx && ptc->my < ptp->my)
		return 1;
	else
	{
		realGetHitPoints(ptp->bmpNO, &w, &h);
		if (ptc->x > ptp->x)
		{
			if (ptp->y - (w - 1) * SURFACE_HEIGHT / 2 <= ptc->y)
				return -1;
		}
		else if (ptc->x < ptp->x)
		{
			if (ptp->y - (h - 1) * SURFACE_HEIGHT / 2 <= ptc->y)
				return -1;
		}
		else
		{
			if (ptp->y <= ptc->y)
				return -1;
		}
	}
	return 1;

}
//ROG ADD end
void ddrawBattleMap(void)
{
	int i, j , count = 0, x, y, tx, ty, partsCnt = 0, TilesCnt = 0, ti, tj;
	float dx, dy;
	U4 bmpNo;
	PARTS tempParts[MAX_CHAR_PRIO_BUF / 2];
	PARTS tempTiles[MAX_CHAR_PRIO_BUF / 2];

	draw_map_bgm_flg = 0;
	// ????????�t?��??????????��???
	if (nowGx != oldGx || nowGy != oldGy)
	{
		if (readMap(nowFloor, mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0]))
		{
			// hitMap[]????�R�e????�@�e
			readHitMap(mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0], &hitMap[0]);
			if (mapEmptyFlag)
			{
				if (!checkEmptyMap(mapEmptyDir))
				{
					mapEmptyFlag = false;
					autoMappingInitFlag = true;	// ??????��???
				}
			}
			else
				autoMappingInitFlag = true;	// ??????��???
//			readMapAfterFrame = 0;	// ???????��?????????????????
		}
		else
			return;
	}
	camMapToGamen(0.0, 0.0, &dx, &dy);
	baseXFastDraw = (int)(dx + .5);
	baseYFastDraw = (int)(dy + .5);
	//???????????????
	if (ResoMode == 1)
	{
		nowXFastDraw = (int)(dx / 2 + .5);
		nowYFastDraw = (int)(dy / 2 + .5);
	}
	else
	{
		nowXFastDraw = baseXFastDraw;
		nowYFastDraw = baseYFastDraw;
	}
	nowXFastDraw2 = baseXFastDraw;
	nowYFastDraw2 = baseYFastDraw;
	amountXFastDraw = 0;
	amountYFastDraw = 0;
	// 1 5 12 16 ... 
	tx = nowXFastDraw2 + (mapAreaX1 + mapAreaY2 - 1) * SURFACE_WIDTH / 2;
	ty = nowYFastDraw2 + (-mapAreaX1 + mapAreaY2 - 1) * SURFACE_HEIGHT / 2;
	ti = mapAreaHeight - 1;
	tj = 0;
	while (ti >= 0)
	{
		i = ti;
		j = tj;
		x = tx;
		y = ty;
		while (i >= 0 && j >= 0)
		{
		// ???����
			if (tile[i * mapAreaWidth+j] > CG_INVISIBLE || parts[i * mapAreaWidth+j] > CG_INVISIBLE)
			{
				// ?����??????
				if (x >= (-SURFACE_WIDTH >> 1) && x < DEF_APPSIZEX + (SURFACE_WIDTH >> 1) &&
					y >= (-SURFACE_HEIGHT >> 1) && y < DEF_APPSIZEY + (SURFACE_HEIGHT >> 1))
				{
//ROG ADD ��������ʾ
					if (parts[i * mapAreaWidth+j] > CG_INVISIBLE)
					{
						realGetNo(parts[i * mapAreaWidth + j], &bmpNo);
						tempParts[partsCnt].bmpNO = parts[i * mapAreaWidth + j];
						tempParts[partsCnt].x     = x;
						tempParts[partsCnt].y     = y;
						tempParts[partsCnt].mx    = (float)(mapAreaX1 + j) * GRID_SIZE;
						tempParts[partsCnt].my    = (float)(mapAreaY1 + i) * GRID_SIZE;
						partsCnt++;
					}
					tempTiles[TilesCnt].x = x;
					tempTiles[TilesCnt].y = y;
					tempTiles[TilesCnt].bmpNO = tile[i * mapAreaWidth + j];
					TilesCnt++;
					//StockDispBuffer(x, y, DISP_PRIO_TILE, tile[i*mapAreaWidth+j], 0);
		//			count++;
				}
			}
//ROG ADD end			}
			i--;
			j--;
			x -= SURFACE_WIDTH;
		}
		if (tj < mapAreaWidth - 1)
		{
			tj++;
			tx += SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
		else
		{
			ti--;
			tx -= SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
	}
//ROG ADD ��������ʾ
	if (partsCnt > 0)
	{
		qsort(tempParts, partsCnt, sizeof(PARTS), (CMPFUNC*)sort_parts);
		for (i = 0; i < partsCnt; i++)
			StockDispBuffer(tempParts[i].x , tempParts[i].y, DISP_PRIO_TILE, tempParts[i].bmpNO, 0);
	}
	SortDispBuffer();
	for (i = 0; i < TilesCnt; i++)
		StockDispBuffer(tempTiles[i].x, tempTiles[i].y, DISP_PRIO_TILE, tempTiles[i].bmpNO, 0);
	SortDispBuffer();
//ROG ADD end
//	stockCharParts();
	oldGx = nowGx;
	oldGy = nowGy;
	if (fMapBgm >= 40 && fMapBgm <= 53 && map_bgm_no == 2)
	{
		play_map_bgm(fMapBgm);
		draw_map_bgm_flg = 1;
		fMapBgm = 0;
	}
}
#endif

void drawMap(void)
{
	int i, j, x, y, tx, ty, rainFlag = 0, snowFlag = 0, tryFlag = 0;;
	S2 xx, yy, ww, hh;
	float dx, dy;
	U4 bmpNo;

	draw_map_bgm_flg = 0;
//	readMapAfterFrame++;
	// ????????�t?��??????????��???
	if (nowGx != oldGx || nowGy != oldGy)
	{
		if (readMap(nowFloor, mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0]))
		{
			// hitMap[]????�R�e????�@�e
			readHitMap(mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0], &hitMap[0]);
			if (mapEmptyFlag)
			{
				if (!checkEmptyMap(mapEmptyDir))
				{
					mapEmptyFlag = false;
					autoMappingInitFlag = true;	// ??????��???
				}
			}
			else
				autoMappingInitFlag = true;	// ??????��???
//			readMapAfterFrame = 0;	// ???????��?????????????????
		}
		else
			return;
	}
	camMapToGamen(0.0, 0.0, &dx, &dy);
	baseXFastDraw = (int)(dx + .5);
	baseYFastDraw = (int)(dy + .5);
	//???????????????
	if (ResoMode == 1)
	{
		nowXFastDraw = (int)(dx / 2 + .5);
		nowYFastDraw = (int)(dy / 2 + .5);
	}
	else
	{
		nowXFastDraw = baseXFastDraw;
		nowYFastDraw = baseYFastDraw;
	}
	nowXFastDraw2 = baseXFastDraw;
	nowYFastDraw2 = baseYFastDraw;
	amountXFastDraw = 0;
	amountYFastDraw = 0;
	// 1 5 12 16 ... 
	tx = nowXFastDraw2 + (mapAreaX1 + mapAreaY2 - 1) * SURFACE_WIDTH / 2;
	ty = nowYFastDraw2 + (-mapAreaX1 + mapAreaY2 - 1) * SURFACE_HEIGHT / 2;

#if 1
	// ????�k???????????
	//
	// [map]
	//               16
	//            15    14
	//         13    12    11
	//      10     9     8     7
	//          6     5     4
	//             3     2
	//                1

	int ti, tj;

	ti = mapAreaHeight - 1;
	tj = 0;

	while (ti >= 0)
	{
		i = ti;
		j = tj;
		x = tx;
		y = ty;
//		if (i==30)
		while (i >= 0 && j >= 0)
		{
			// ???����
			if (tile[i * mapAreaWidth+j] > CG_INVISIBLE)
			{
#if 0
				// ??????��??(???)
				if (193 <= tile[i * mapAreaWidth + j] && tile[i * mapAreaWidth+j] <= 196)
					play_environment(0, x, y);
#endif
				// ?����??????
				if (x >= (-SURFACE_WIDTH >> 1) && x < DEF_APPSIZEX + (SURFACE_WIDTH >> 1) &&
					y >= (-SURFACE_HEIGHT >> 1) && y < DEF_APPSIZEY + (SURFACE_HEIGHT >> 1))
					StockDispBuffer(x, y, DISP_PRIO_TILE, tile[i * mapAreaWidth + j], 0);
			}
			else
			{
				// �{��?????????
				// ????????��??
				if (20 <= tile[i * mapAreaWidth + j] && tile[i * mapAreaWidth + j] <= 39)
					play_environment(tile[i * mapAreaWidth + j], x, y);
				else if (40 <= tile[i * mapAreaWidth + j] && tile[i * mapAreaWidth + j] <= 59)// ?????????��??
				{
					play_map_bgm(tile[i * mapAreaWidth + j]);
                    if ( map_bgm_no != t_music_bgm_no) {
                        stop_bgm();
                        play_bgm( map_bgm_no );
                    }
					draw_map_bgm_flg = 1;
				}
			}
			// ???����
			if (parts[i * mapAreaWidth+j] > CG_INVISIBLE)
			{
#if 0
				// ??????��??(???)
				if (parts[i * mapAreaWidth+j] == 10011)
					play_environment(2, x, y);
				else if (parts[i * mapAreaWidth + j] == 10012)
					play_environment(1, x, y);
				else if (parts[i * mapAreaWidth + j] == 10203)
					play_environment(4, x, y);
				else if (parts[i * mapAreaWidth + j] == 10048)
				{
					play_map_bgm(2);
					draw_map_bgm_flg = 1;
				}
#endif
				realGetNo(parts[i * mapAreaWidth + j], &bmpNo);
				// ?����??????
				realGetPos(bmpNo, &xx, &yy);
				realGetWH(bmpNo, &ww, &hh);
				xx += x;
				yy += y;
				if (xx < DEF_APPSIZEX && xx + ww - 1 >= 0 && yy < DEF_APPSIZEY && yy + hh - 1 >= 0)
					// ����?�I�T?�e????�K???
					setPartsPrio(bmpNo, x, y, 0, 0, (float)(mapAreaX1 + j) * GRID_SIZE, (float)(mapAreaY1 + i) * GRID_SIZE, -1);
			}
			else
			{
				// �{��?????????
				// ????????��??
				if (20 <= parts[i * mapAreaWidth + j] && parts[i * mapAreaWidth + j] <= 39)
					play_environment(parts[i * mapAreaWidth + j], x, y);
				else if (40 <= parts[i * mapAreaWidth + j] && parts[i * mapAreaWidth + j] <= 59)// ?????????��??
				{
					play_map_bgm(parts[i * mapAreaWidth + j]);
                    if ( map_bgm_no != t_music_bgm_no) {
                        stop_bgm();
                        play_bgm( map_bgm_no );
                    }
					draw_map_bgm_flg = 1;
				}
			}
			i--;
			j--;
			x -= SURFACE_WIDTH;
		}
		if (tj < mapAreaWidth - 1)
		{
			tj++;
			tx += SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
		else
		{
			ti--;
			tx -= SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
	}
#else
	// ????�k???????????
	//
	// [map] 
	//               16
	//            15    12
	//         14    11     8
	//      13    10     7     4
	//          9     6     3
	//             5     2
	//                1
	x = (int)(dx + .5);
	y = (int)(dy + .5);
	for (i = mapAreaHeight - 1; i >= 0; i--)
	{
		tx = x;
		ty = y;
		for (j = 0; j < mapAreaWidth; j++)
		{
			// ???����
			if (tile[i * mapAreaWidth + j] > CG_INVISIBLE)
			{
				// ?����??????
				if (x >= (-SURFACE_WIDTH >> 1) && x < DEF_APPSIZEX + (SURFACE_WIDTH >> 1) &&
					y >= (-SURFACE_HEIGHT >> 1) && y < DEF_APPSIZEY + (SURFACE_HEIGHT >> 1))
					StockDispBuffer(x, y, DISP_PRIO_TILE, tile[i * mapAreaWidth + j], 0);
			}
			// ???����
			if (parts[i * mapAreaWidth + j] > CG_INVISIBLE)
			{
				realGetNo(parts[i * mapAreaWidth + j], &bmpNo);
				// ?����??????
				realGetPos(bmpNo, &xx, &yy);
				realGetWH(bmpNo, &ww, &hh);
				xx += x;
				yy += y;
				if (xx < DEF_APPSIZEX && xx + ww - 1 >= 0 && yy < DEF_APPSIZEY && yy + hh - 1 >= 0)
					// ����?�I�T?�e????�K???
					setPartsPrio(bmpNo, x, y, 0, 0, (float)(mapAreaX1 + j) * GRID_SIZE, (float)(mapAreaY1 + i) * GRID_SIZE, -1);
			}
			x += SURFACE_WIDTH >> 1;
			y -= SURFACE_HEIGHT >> 1;
		}
		x = tx-SURFACE_WIDTH >> 1;
		y = ty-SURFACE_HEIGHT >> 1;
	}
#endif
	stockCharParts();
	oldGx = nowGx;
	oldGy = nowGy;

	// shan 2002/01/18
	if (fMapBgm >= 40 && fMapBgm <= 53 && map_bgm_no == 2)
	{
		play_map_bgm(fMapBgm);
		draw_map_bgm_flg = 1;
		fMapBgm = 0;
	}
}

// ?????�L��??
void drawMap2(void)
{
	int i, j, x, y, tx, ty;
	S2 xx, yy, ww, hh;
	float dx, dy;
	U4 bmpNo;
	short tileDrawFlag;

	if (!fastDrawTile)
	{
		drawMap();
		return;
	}
	draw_map_bgm_flg = 0;
//	readMapAfterFrame++;
	// ????????�t?��??????????��???
	if (nowGx != oldGx || nowGy != oldGy)
	{
		if (readMap(nowFloor, mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0]))
		{
			// hitMap[]????�R�e????�@�e??
			readHitMap(mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0], &hitMap[0]);
			if (mapEmptyFlag)
			{
				if (!checkEmptyMap(mapEmptyDir))
				{
					mapEmptyFlag = false;
					autoMappingInitFlag = true;	// ??????��???
				}
			}
			else
				autoMappingInitFlag = true;	// ??????��???
//			readMapAfterFrame = 0;	// ???????��?????????????????
		}
		else
			return;
	}
		camMapToGamen(0.0, 0.0, &dx, &dy);
	nowXFastDraw = (int)(dx + .5);
	nowYFastDraw = (int)(dy + .5);
	nowXFastDraw2 = nowXFastDraw;
	nowYFastDraw2 = nowYFastDraw;
	//???????????????
	if (ResoMode == 1)
	{
		nowXFastDraw = (int)(dx / 2 + .5);
		nowYFastDraw = (int)(dy / 2 + .5);
	}
	amountXFastDraw = nowXFastDraw - baseXFastDraw;
	amountYFastDraw = nowYFastDraw - baseYFastDraw;
	amountXFastDraw2 = nowXFastDraw2 - baseXFastDraw2;
	amountYFastDraw2 = nowYFastDraw2 - baseYFastDraw2;
	tx = nowXFastDraw2 + (mapAreaX1 + mapAreaY2 - 1) * SURFACE_WIDTH / 2;
	ty = nowYFastDraw2 + (-mapAreaX1 + mapAreaY2 - 1) * SURFACE_HEIGHT / 2;
	// ????�k???????????
	//
	// [map]
	//               16
	//            15    14
	//         13    12    11
	//      10     9     8     7
	//          6     5     4
	//             3     2
	//                1
	int ti, tj;

	ti = mapAreaHeight - 1;
	tj = 0;
	while (ti >= 0)
	{
		i = ti;
		j = tj;
		x = tx;
		y = ty;
		while (i >= 0 && j >= 0)
		{
			// ???����
			if (tile[i * mapAreaWidth + j] > CG_INVISIBLE)
			{
				if (amountXFastDraw2 != 0 || amountYFastDraw2 != 0)
				{
					// ?����?�V?????????????
					if ((-SURFACE_WIDTH >> 1) < x && x < DEF_APPSIZEX + (SURFACE_WIDTH >> 1) &&
						(-SURFACE_HEIGHT >> 1) < y && y < DEF_APPSIZEY + (SURFACE_HEIGHT >> 1))
					{
						tileDrawFlag = 0;
						if (amountXFastDraw2 > 0)
						{
							if ((x - amountXFastDraw2) <= (SURFACE_WIDTH >> 1))
								tileDrawFlag = 1;
						}
						else if (amountXFastDraw2 < 0)
						{
							if (DEF_APPSIZEX - (SURFACE_WIDTH >> 1) <= (x - amountXFastDraw2))
								tileDrawFlag = 1;
						}
						if (amountYFastDraw2 > 0)
						{
							if ((y - amountYFastDraw2) <= (SURFACE_HEIGHT >> 1))
								tileDrawFlag = 1;
						}
						else if (amountYFastDraw2 < 0)
						{
							if (DEF_APPSIZEY - (SURFACE_HEIGHT >> 1) <= (y - amountYFastDraw2))
								tileDrawFlag = 1;
						}
						if (tileDrawFlag)
							StockDispBuffer(x, y, DISP_PRIO_TILE, tile[i * mapAreaWidth + j], 0);
					}
				}
			}
			else
			{
				// �{��?????????
				// ????????��??
				if (20 <= tile[i * mapAreaWidth + j] && tile[i * mapAreaWidth + j] <= 39)
					play_environment(tile[i * mapAreaWidth + j], x, y);
				else if (40 <= tile[i * mapAreaWidth + j] && tile[i * mapAreaWidth + j] <= 59)// ?????????��??
				{
					play_map_bgm(tile[i * mapAreaWidth + j]);
					draw_map_bgm_flg = 1;
				}
			}
			// ???����
			if (parts[i * mapAreaWidth + j] > CG_INVISIBLE)
			{
				realGetNo(parts[i * mapAreaWidth + j], &bmpNo);
				// ?����??????
				realGetPos(bmpNo, &xx, &yy);
				realGetWH(bmpNo, &ww, &hh);
				xx += x;
				yy += y;
				if (xx < DEF_APPSIZEX && xx + ww - 1 >= 0 && yy < DEF_APPSIZEY && yy + hh - 1 >= 0)
					// ����?�I�T?�e????�K???
					setPartsPrio(bmpNo, x, y, 0, 0, (float)(mapAreaX1 + j) * GRID_SIZE, (float)(mapAreaY1 + i) * GRID_SIZE, -1);
			}
			else
			{
				// �{��?????????
				// ????????��??
				if (20 <= parts[i * mapAreaWidth + j] && parts[i * mapAreaWidth + j] <= 39)
					play_environment(parts[i * mapAreaWidth + j], x, y);
				else if (40 <= parts[i * mapAreaWidth + j] && parts[i * mapAreaWidth + j] <= 59)// ?????????��??
				{
					play_map_bgm(parts[i * mapAreaWidth + j]);
					draw_map_bgm_flg = 1;
				}
			}
			i--;
			j--;
			x -= SURFACE_WIDTH;
		}
		if (tj < mapAreaWidth - 1)
		{
			tj++;
			tx += SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
		else
		{
			ti--;
			tx -= SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
	}
	// shan 2002/01/18
	if (fMapBgm >= 40 && fMapBgm <= 53 && map_bgm_no == 2)
	{
		play_map_bgm(fMapBgm);
		draw_map_bgm_flg = 1;
		fMapBgm = 0;
	}
	stockCharParts();
	oldGx = nowGx;
	oldGy = nowGy;
}

//  ?????����
void drawTile(void)
{
	int i, j, x, y, tx, ty;

	// ??????��???
	if (readMap(nowFloor, mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0]))
		readHitMap(mapAreaX1, mapAreaY1, mapAreaX2, mapAreaY2, &tile[0], &parts[0], &event[0], &hitMap[0]);		// hitMap[]????�R�e????�@�e??
	tx = nowXFastDraw2 + (((mapAreaX1 + mapAreaY2 - 1) * SURFACE_WIDTH) >> 1);
	ty = nowYFastDraw2 + (((-mapAreaX1 + mapAreaY2 - 1) * SURFACE_HEIGHT) >> 1);
	// ????�k???????????
	//
	// [map]
	//               16
	//            15    14
	//         13    12    11
	//      10     9     8     7
	//          6     5     4
	//             3     2
	//                1

	int ti, tj;

	ti = mapAreaHeight - 1;
	tj = 0;
	while (ti >= 0)
	{
		i = ti;
		j = tj;
		x = tx;
		y = ty;
		while (i >= 0 && j >= 0)
		{
			// ???����
			if (tile[i * mapAreaWidth + j] > CG_INVISIBLE)
			{
				// ?����??????
				if (x >= (-SURFACE_WIDTH >> 1) && x < (DEF_APPSIZEX + (SURFACE_WIDTH >> 1)) &&
					y >= (-SURFACE_HEIGHT >> 1) && y < (DEF_APPSIZEY + (SURFACE_HEIGHT >> 1)))
					StockDispBuffer(x, y, DISP_PRIO_TILE, tile[i * mapAreaWidth + j], 0);
			}
			i--;
			j--;
			x -= SURFACE_WIDTH;
		}
		if (tj < mapAreaWidth - 1)
		{
			tj++;
			tx += SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
		else
		{
			ti--;
			tx -= SURFACE_WIDTH >> 1;
			ty -= SURFACE_HEIGHT >> 1;
		}
	}
}

void redrawMap(void)
{
	oldGx = -1;
	oldGy = -1;
}
#if 1
static unsigned char BitTable[] =	/*	????��???????????	*/
{
		0x00 , 0x80 , 0x40 , 0xC0 , 0x20 , 0xA0 , 0x60 , 0xE0 , 
		0x10 , 0x90 , 0x50 , 0xD0 , 0x30 , 0xB0 , 0x70 , 0xF0 , 
		0x08 , 0x88 , 0x48 , 0xC8 , 0x28 , 0xA8 , 0x68 , 0xE8 , 
		0x18 , 0x98 , 0x58 , 0xD8 , 0x38 , 0xB8 , 0x78 , 0xF8 , 
		0x04 , 0x84 , 0x44 , 0xC4 , 0x24 , 0xA4 , 0x64 , 0xE4 , 
		0x14 , 0x94 , 0x54 , 0xD4 , 0x34 , 0xB4 , 0x74 , 0xF4 , 
		0x0C , 0x8C , 0x4C , 0xCC , 0x2C , 0xAC , 0x6C , 0xEC , 
		0x1C , 0x9C , 0x5C , 0xDC , 0x3C , 0xBC , 0x7C , 0xFC , 
		0x02 , 0x82 , 0x42 , 0xC2 , 0x22 , 0xA2 , 0x62 , 0xE2 , 
		0x12 , 0x92 , 0x52 , 0xD2 , 0x32 , 0xB2 , 0x72 , 0xF2 , 
		0x0A , 0x8A , 0x4A , 0xCA , 0x2A , 0xAA , 0x6A , 0xEA , 
		0x1A , 0x9A , 0x5A , 0xDA , 0x3A , 0xBA , 0x7A , 0xFA , 
		0x06 , 0x86 , 0x46 , 0xC6 , 0x26 , 0xA6 , 0x66 , 0xE6 , 
		0x16 , 0x96 , 0x56 , 0xD6 , 0x36 , 0xB6 , 0x76 , 0xF6 , 
		0x0E , 0x8E , 0x4E , 0xCE , 0x2E , 0xAE , 0x6E , 0xEE , 
		0x1E , 0x9E , 0x5E , 0xDE , 0x3E , 0xBE , 0x7E , 0xFE , 
		0x01 , 0x81 , 0x41 , 0xC1 , 0x21 , 0xA1 , 0x61 , 0xE1 , 
		0x11 , 0x91 , 0x51 , 0xD1 , 0x31 , 0xB1 , 0x71 , 0xF1 , 
		0x09 , 0x89 , 0x49 , 0xC9 , 0x29 , 0xA9 , 0x69 , 0xE9 , 
		0x19 , 0x99 , 0x59 , 0xD9 , 0x39 , 0xB9 , 0x79 , 0xF9 , 
		0x05 , 0x85 , 0x45 , 0xC5 , 0x25 , 0xA5 , 0x65 , 0xE5 , 
		0x15 , 0x95 , 0x55 , 0xD5 , 0x35 , 0xB5 , 0x75 , 0xF5 , 
		0x0D , 0x8D , 0x4D , 0xCD , 0x2D , 0xAD , 0x6D , 0xED , 
		0x1D , 0x9D , 0x5D , 0xDD , 0x3D , 0xBD , 0x7D , 0xFD , 
		0x03 , 0x83 , 0x43 , 0xC3 , 0x23 , 0xA3 , 0x63 , 0xE3 , 
		0x13 , 0x93 , 0x53 , 0xD3 , 0x33 , 0xB3 , 0x73 , 0xF3 , 
		0x0B , 0x8B , 0x4B , 0xCB , 0x2B , 0xAB , 0x6B , 0xEB , 
		0x1B , 0x9B , 0x5B , 0xDB , 0x3B , 0xBB , 0x7B , 0xFB , 
		0x07 , 0x87 , 0x47 , 0xC7 , 0x27 , 0xA7 , 0x67 , 0xE7 , 
		0x17 , 0x97 , 0x57 , 0xD7 , 0x37 , 0xB7 , 0x77 , 0xF7 , 
		0x0F , 0x8F , 0x4F , 0xCF , 0x2F , 0xAF , 0x6F , 0xEF , 
		0x1F , 0x9F , 0x5F , 0xDF , 0x3F , 0xBF , 0x7F , 0xFF
};
static unsigned short crctab16[] =	/*	crc  ??��????		*/
{
		0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
		0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
		0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
		0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
		0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
		0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
		0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
		0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
		0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
		0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
		0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
		0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
		0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
		0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
		0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
		0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
		0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
		0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
		0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
		0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
		0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
		0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
		0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
		0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
		0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
		0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
		0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
		0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
		0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
		0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
		0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
		0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0,
};
unsigned short CheckCRC(unsigned char *p , int size)
{
	unsigned short	crc = 0;
	int		i;
	
	for (i = 0; i < size; i ++)
	{
		crc = (crctab16[(crc >> 8) & 0xFF] ^ (crc << 8) ^ BitTable[p[i]]);
	}
	return crc;
}

bool mapCheckSum(int floor, int x1, int y1, int x2, int y2, int tileSum, int partsSum, int eventSum)
{
	int tilesum = 0, objsum = 0, eventsum = 0, databufferindex = 0, width = x2 - x1, height = y2 - y1, i, j;
	unsigned short tile[MAP_X_SIZE*MAP_Y_SIZE];
	unsigned short parts[MAP_X_SIZE*MAP_Y_SIZE];
	unsigned short event[MAP_X_SIZE*MAP_Y_SIZE];

	// ???????????��?��?

	readMap(floor, x1, y1, x2, y2, tile, parts, event);
	for (i = 0; i < height; i++)
	{
        for (j = 0; j < width; j++)
			event[i * width + j] &= 0x0fff;
    }

	tilesum  = CheckCRC((unsigned char*)tile,  27 * 27 * sizeof(short));
	objsum   = CheckCRC((unsigned char*)parts, 27 * 27 * sizeof(short));
	eventsum = CheckCRC((unsigned char*)event, 27 * 27 * sizeof(short));

	if (tileSum == tilesum && partsSum == objsum && eventSum == eventsum)
	{
		if (loginFlag)
		{
			redrawMap();
			loginFlag = false;
		}
		return true;
	}
	else
	{
		if (bNewServer)
			lssproto_M_send(sockfd, floor, x1, y1, x2, y2);
		else
			old_lssproto_M_send(sockfd, floor, x1, y1, x2, y2);
		//������ͼ�쳣
		if (loginFlag)
			loginFlag = false;

		return false;
	}
}
#else
bool mapCheckSum(int floor, int x1, int y1, int x2, int y2, int tileSum, int partsSum, int eventSum)
{
	int tilesum = 0, objsum = 0, eventsum = 0, databufferindex = 0, width = x2 - x1, height = y2 - y1, i, j;
	unsigned short tile[MAP_X_SIZE*MAP_Y_SIZE];
	unsigned short parts[MAP_X_SIZE*MAP_Y_SIZE];
	unsigned short event[MAP_X_SIZE*MAP_Y_SIZE];
	// ???????????��?��?

	readMap(floor, x1, y1, x2, y2, tile, parts, event);
	for (i = 0; i < height; i++)
	{
        for (j = 0; j < width; j++)
		{
			tilesum += (tile[i * width + j] % (27 * 27)) ^ databufferindex;
			objsum += (parts[i * width + j] % (27 * 27)) ^ databufferindex;
#if 1
			eventsum +=	((event[i * width + j] & 0x0fff) % (27 * 27)) ^ databufferindex;
#else
			eventsum += (event[i * width + j] % (27 * 27)) ^ databufferindex;
#endif
        	databufferindex++;
        }
    }

	if (tileSum == tilesum && partsSum == objsum && eventSum == eventsum)
	{
		// ??????????��????????
		if (loginFlag)
		{
			redrawMap();
			loginFlag = false;
		}

		return true;
	}
	else
	{
		if (bNewServer)
			lssproto_M_send(sockfd, floor, x1, y1, x2, y2);
		else
			old_lssproto_M_send(sockfd, floor, x1, y1, x2, y2);
#if 0
		// ??????????????false???
		if (loginFlag)
			loginFlag = false;
#endif
		return false;
	}
}
#endif


// ????????????�R�e�ѩ�?��?��??
#if 1
void readHitMap(int x1, int y1, int x2, int y2, unsigned short *tile, unsigned short *parts, unsigned short *event, unsigned short *hitMap)
{
	int width, height, i, j, k, l;
	S2 hit, hitX, hitY;
	U4 bmpNo;

	memset(hitMap, 0, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));

	width = x2 - x1;
	height = y2 - y1;
	if (width < 1 || height < 1)
		return;

	// ???
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// ???????�R�e
			if (tile[i * width + j] > CG_INVISIBLE || (60 <= tile[i * width + j] && tile[i * width + j] <= 79))
			{
				realGetNo(tile[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0 && hitMap[i * width + j] != 2)
					hitMap[i * width + j] = 1;
				else if (hit == 2) // hit?2??????�R�e???
					hitMap[i * width + j] = 2;
			}
			else
			{
				// 0??11??????????????
				switch (tile[i * width + j])
				{
					case 0:	// 0.bmp(��???)???????�R�e?????
						// ��????????????????????
						if ((event[i * width + j] & MAP_SEE_FLAG) == 0)
							break;
					case 1:
					case 2:
					case 5:
					case 6:
					case 9:
					case 10:
						// ???�R�e???????�@�e???
						if (hitMap[i * width + j] != 2)
							hitMap[i * width + j] = 1;
						break;

					case 4:
						hitMap[i * width + j] = 2;
						break;
				}
			}
		}
	}

	// ???
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{ 
			// ???????�R�e
			if (parts[i * width + j] > CG_INVISIBLE)
			{
				realGetNo(parts[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0)
				{
					realGetHitPoints(bmpNo, &hitX, &hitY);
					for (k = 0; k < hitY; k++)
					{
						for (l = 0; l < hitX; l++)
						{
							if ((i - k) >= 0 && (j + l) < width && hitMap[(i - k) * width + j + l] != 2)
								hitMap[(i - k) * width + j + l] = 1;
						}
					}
				}
				// ???�R�e?????????��?????
				// ?�ɤ�?��???????
				else if (hit == 2)
				{
					realGetHitPoints(bmpNo, &hitX, &hitY);
					for (k = 0; k < hitY; k++)
					{
						for (l = 0; l < hitX; l++)
						{
							if ((i - k) >= 0 && (j + l) < width)
								hitMap[(i - k) * width + j + l] = 2;
						}
					}
				}
				else if (hit == 1 && parts[i * width + j] >= 15680 && parts[i * width + j] <= 15732)
				{
					realGetHitPoints(bmpNo, &hitX, &hitY);
					for (k = 0; k < hitY; k++)
					{
						for (l = 0; l < hitX; l++)
						{
							//if ((i - k) >= 0 && (j + l) < width)
								//hitMap[(i-k)*width+j+l] = 0;
							if (k == 0 && l == 0)
								hitMap[(i - k) * width + j + l] = 1;
						}
					}					
				}	
			}
			else if (60 <= parts[i * width + j] && parts[i * width + j] <= 79)
			{
				realGetNo(parts[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0 && hitMap[i * width + j] != 2)
					hitMap[i * width + j] = 1;
				// hit?2??????�R�e???
				else if (hit == 2)
					hitMap[i * width + j] = 2;
			}
			else
			{
				// 0??11??????????????
				switch (parts[i * width + j])
				{	
					case 1:
					case 2:
					case 5:
					case 6:
					case 9:
					case 10:
						// ???�R�e???????�@�e???
						if (hitMap[i * width + j] != 2)
							hitMap[i * width + j] = 1;
						break;

					case 4:
						hitMap[i * width + j] = 2;
						break;
				}
			}

			// ?�e????????��?��?????
			if ((event[i * width + j] & 0x0fff) == EVENT_NPC)
				hitMap[i * width + j] = 1;
		}
	}
}
#else
void readHitMap(int x1, int y1, int x2, int y2, unsigned short *tile, unsigned short *parts, unsigned short *event, unsigned short *hitMap)
{
	int width, height, i, j, k, l;
	S2 hit, hitX, hitY;
	U4 bmpNo;

	memset(hitMap, 0, MAP_X_SIZE * MAP_Y_SIZE * sizeof(short));

	width = x2 - x1;
	height = y2 - y1;

	if (width < 1 || height < 1)
		return;
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			// ��????????????????????
			if ((event[i * width + j] & MAP_READ_FLAG) == 0)
				continue;

			// ???????�R�e
			if (tile[i * width + j] > CG_INVISIBLE
#if 0
			 || (60 <= tile[i * width + j] && tile[i * width + j] <= 79))
#else
			)
#endif
			{
				realGetNo(tile[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0 && hitMap[i * width + j] != 2)
					hitMap[i*width+j] = 1;
				// hit?2??????�R�e???
				else if (hit == 2)
					hitMap[i * width + j] = 2;
			}
#if 0
			else
			{
				// 0??11??????????????
				switch (tile[i * width + j])
				{
					case 0:	// 0.bmp(��???)???????�R�e?????
					case 1:
					case 2:
					case 5:
					case 6:
					case 9:
					case 10:
						// ???�R�e???????�@�e???
						if (hitMap[i * width + j] != 2)
							hitMap[i * width + j] = 1;
						break;
					case 4:
						hitMap[i * width + j] = 2;
						break;
				}
			}
#endif
			// ???????�R�e
			if (parts[i * width + j] > CG_INVISIBLE)
			{
				realGetNo(parts[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0)
				{
					realGetHitPoints(bmpNo, &hitX, &hitY);
					for (k = 0; k < hitY; k++)
					{
						for (l = 0; l < hitX; l++)
						{
							if ((i - k) >= 0 && (j + l) < width && hitMap[(i - k) * width + j + l] != 2)
								hitMap[(i - k) * width + j + l] = 1;
						}
					}
				}
				// ???�R�e?????????��?????
				// ?�ɤ�?��???????
				else if (hit == 2)
				{
					realGetHitPoints(bmpNo, &hitX, &hitY);
					for (k = 0; k < hitY; k++)
					{
						for (l = 0; l < hitX; l++)
						{
							if ((i - k) >= 0 && (j + l) < width)
								hitMap[(i - k) * width + j + l] = 2;
						}
					}
				}
			}
#if 0
			else if (60 <= parts[i * width + j] && parts[i * width + j] <= 79)
			{
				realGetNo(parts[i * width + j], &bmpNo);
				// ???�R�e??��
				realGetHitFlag(bmpNo, &hit);
				// ???�R�e?????????�@�e
				if (hit == 0 && hitMap[i * width + j] != 2)
					hitMap[i * width + j] = 1;
				// hit?2??????�R�e???
				else if (hit == 2)
					hitMap[i * width + j] = 2;
			}
#endif
#if 0
			else
			{
				// 0??11??????????????
				switch (parts[i * width + j])
				{
					case 1:
					case 2:
					case 5:
					case 6:
					case 9:
					case 10:
						// ???�R�e???????�@�e???
						if (hitMap[i * width + j] != 2)
							hitMap[i * width + j] = 1;
						break;
					case 4:
						hitMap[i * width + j] = 2;
						break;
				}
			}
#endif
			// ?�e????????��?��?????
			if ((event[i * width + j] & 0x0fff) == EVENT_NPC)
				hitMap[i * width + j] = 1;
		}
	}
}
#endif
//
// ???�R�e?????
//   ??�k��true ... ���V?��
//
#ifdef _ITEM_PATH
bool ITEMPATHFLAG = false;
#endif
bool checkHitMap(int gx, int gy)
{
	int x, y;

	x = gx - mapAreaX1;
	y = gy - mapAreaY1;

	if (pc.skywalker)
		return false;

	// ????�d???���V?��
	if (x < 0 || mapAreaWidth <= x || y < 0 || mapAreaHeight <= y)
		return true;
#ifdef _ITEM_PATH
	if (ITEMPATHFLAG)
	{
		ITEMPATHFLAG = false;
		return false;
	}
#endif
	// ???�R�e????���V?��
	if (hitMap[y * mapAreaWidth + x] == 1)
		return true;

	return false;
}

//
// ?�h�I?????��????�h????????????�r??
//
bool checkEmptyMap(int dir)
{
	// ?????�t???11?????��??????��???true?��?
	int i, gx, gy, tx, ty, len;
	bool flag = false;

	if (mapAreaWidth < MAP_X_SIZE || mapAreaHeight < MAP_Y_SIZE)
		return false;

	getMapAreaCnt = 0;

	if (dir == 0 || dir == 1 || dir == 2)
	{
		gx = nowGx - SEARCH_AREA;
		gy = nowGy - SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if (event[ty * mapAreaWidth + tx] == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = gx + 1;
					getMapAreaY2[getMapAreaCnt] = gy + (SEARCH_AREA << 1) + 1;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gy++;
			ty++;
		}
	}
	if (dir == 2 || dir == 3 || dir == 4)
	{
		gx = nowGx - SEARCH_AREA;
		gy = nowGy - SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if (event[ty * mapAreaWidth + tx] == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = gx + (SEARCH_AREA << 1) + 1;
					getMapAreaY2[getMapAreaCnt] = gy + 1;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gx++;
			tx++;
		}
	}
	if (dir == 4 || dir == 5 || dir == 6)
	{
		gx = nowGx + SEARCH_AREA;
		gy = nowGy - SEARCH_AREA;
		tx = SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if (event[ty * mapAreaWidth + tx] == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = gx + 2;
					getMapAreaY2[getMapAreaCnt] = gy + (SEARCH_AREA << 1) + 1;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gy++;
			ty++;
		}
	}
	if (dir == 6 || dir == 7 || dir == 0)
	{
		gx = nowGx - SEARCH_AREA;
		gy = nowGy + SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if (event[ty * mapAreaWidth + tx] == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy;
					getMapAreaX2[getMapAreaCnt] = gx + (SEARCH_AREA << 1) + 1;
					getMapAreaY2[getMapAreaCnt] = gy + 2;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gx++;
			tx++;
		}
	}

	return flag;
}

// �d??????????
void checkAreaLimit(short *x1, short *y1, short *x2, short *y2)
{
	if (*x1 < 0)
		*x1 = 0;
	if (*y1 < 0)
		*y1 = 0;
	if (*x2 > nowFloorGxSize)
		*x2 = nowFloorGxSize;
	if (*y2 > nowFloorGySize)
		*y2 = nowFloorGySize;
}

///////////////////////////////////////////////////////////////////////////
// ????��??????????����??
//
void drawGrid(void)
{
	float x, y;
	int xx, yy;
	extern int mapWndFontNo[];
	extern int resultWndFontNo[];

	// ????????????????
	if ((MenuToggleFlag & JOY_CTRL_M) == 0)
		mapWndFontNo[0] = -2;
	// �P�f???????????????
	if (BattleResultWndFlag <= 0)
		resultWndFontNo[0] = -2;

	camGamenToMap((float)mouse.crickLeftDownPoint.x, (float)mouse.crickLeftDownPoint.y, &x, &y);
	mouseMapX = (int)(x + .5);
	mouseMapY = (int)(y + .5);
	mouseMapGx = (mouseMapX + (GRID_SIZE >> 1)) / GRID_SIZE;
	mouseMapGy = (mouseMapY + (GRID_SIZE >> 1)) / GRID_SIZE;
	xx = mouseMapGx * GRID_SIZE;
	yy = mouseMapGy * GRID_SIZE;
	camMapToGamen((float)xx, (float)yy, &x, &y);

	// ??????????????��?????����???
#if 0
	if (mouse.level < DISP_PRIO_MENU || mapWndFontNo[0] == HitDispNo || resultWndFontNo[0] == HitDispNo)
#else
	if (mouse.level < DISP_PRIO_MENU)
#endif
	{
		// ?????????����
		if (mouseCursorMode == MOUSE_CURSOR_MODE_NORMAL)
			StockDispBuffer((int)(x + .5), (int)(y + .5), DISP_PRIO_GRID, CG_GRID_CURSOR, 0);
	//cary 2002.1.15	else
	//	{
	//		StockDispBuffer((int)(x+.5), (int)(y+.5), DISP_PRIO_GRID, 1610, 0);
	//	}
	}

	// ?????�l
	// fieldProc(); moveProc(); ?����??????????��???
	mouseLeftCrick = false;
	mouseLeftOn = false;
	mouseRightCrick = false;
	mouseRightOn = false;
#ifdef _MOUSE_DBL_CLICK
	mouseDblRightOn = false;
#endif
	if ((mouse.level < DISP_PRIO_MENU && mouse.itemNo == -1) || mapWndFontNo[0] == HitDispNo || resultWndFontNo[0] == HitDispNo)
	{
		if ((mouse.onceState & MOUSE_LEFT_CRICK))
		{
			{
				mouseLeftCrick = true;
				mouseLeftPushTime = 0;
				beforeMouseLeftPushTime = TimeGetTime();
			}
		}
		if ((mouse.state & MOUSE_LEFT_CRICK))
		{
			{
				mouseLeftOn = true;
				if (beforeMouseLeftPushTime > 0)
					mouseLeftPushTime = TimeGetTime() - beforeMouseLeftPushTime;
			}
		}
		else
		{
			mouseLeftPushTime = 0;
			beforeMouseLeftPushTime = 0;
		}
		if ((mouse.onceState & MOUSE_RIGHT_CRICK))
			mouseRightCrick = true;
		if ((mouse.state & MOUSE_RIGHT_CRICK))
			mouseRightOn = true;
#ifdef _MOUSE_DBL_CLICK
		if ((mouse.onceState & MOUSE_RIGHT_DBL_CRICK))
			mouseDblRightOn = true;
#endif
		// ???????????????????�h???????
		// �P�f??????????????????�h???????
		if (mapWndFontNo[0] == HitDispNo || resultWndFontNo[0] == HitDispNo)
			mouseLeftCrick = false;
	}
	else
	{
		mouseCursorMode = MOUSE_CURSOR_MODE_NORMAL;
		mouseLeftPushTime = 0;
		beforeMouseLeftPushTime = 0;
	}
}

///////////////////////////////////////////////////////////////////////////
// ?�h??
//Terry add 2003/11/25
bool g_bTradesystemOpen = false;
//end
void moveProc(void)
{
	static unsigned int befortime = -1;

#ifdef _DEBUG
	{
		static short tglSw = 0;

		if (tglSw == 1)
		{
			char msg[256];
			sprintf(msg, "EN send        : %d", sendEnFlag);
			StockFontBuffer(240, 16, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "EV (Warp) send : %d", eventWarpSendFlag);
			StockFontBuffer(240, 32, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "EV (Enemy) send: %d", eventEnemySendFlag);
			StockFontBuffer(240, 48, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "Empty          : %d", mapEmptyFlag);
			StockFontBuffer(240, 64, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "  nowGx      = %3d/ nowGy      = %3d", nowGx, nowGy);
			StockFontBuffer(240, 80, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "  mapEmptyGx = %3d/ mapEmptyGy = %3d", mapEmptyGx, mapEmptyGy);
			StockFontBuffer(240, 96, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "  mapEmptyDir = %3d", mapEmptyDir);
			StockFontBuffer(240, 112, FONT_PRIO_FRONT, 0, msg, 0);
		}
		else if (tglSw == 2)
		{
			char msg[256];
			sprintf(msg, "nowEncountPercentage : %d", nowEncountPercentage);
			StockFontBuffer(240, 16, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "nowEncountExtra      : %d", nowEncountExtra);
			StockFontBuffer(240, 32, FONT_PRIO_FRONT, 0, msg, 0);
		}
		else if (tglSw == 3 || tglSw == 4)
		{
			// ???�R�e����??????
			// ????�R�e����??????
			char msg[256];
			int x, y, xx, yy, color;

			xx = -MAP_TILE_GRID_X1;
			yy = -MAP_TILE_GRID_Y1;

			if (MAP_X_SIZE > mapAreaWidth)
			{
				if (mapAreaX1 == 0)
					xx -= (MAP_X_SIZE - mapAreaWidth);
			}
			if (MAP_Y_SIZE > mapAreaHeight)
			{
				if (mapAreaY1 == 0)
					yy -= (MAP_Y_SIZE - mapAreaHeight);
			}

			for (y = 0; y < mapAreaHeight && y < 26; y++)
			{
				for (x = 0; x < mapAreaWidth; x++)
				{
					if (tglSw == 3)
					{
						sprintf(msg, "%d", hitMap[y * mapAreaWidth + x]);
						if (x == xx && y == yy)
							color = FONT_PAL_RED;
						else
							color = FONT_PAL_WHITE;
					}
					else
					{
						sprintf(msg, "%d", (event[y * mapAreaWidth + x] & 0xfff));
						if (x == xx && y == yy)
							color = FONT_PAL_RED;
						else
						{
							if (event[y * mapAreaWidth + x] != 0)
								color = FONT_PAL_YELLOW;
							else
								color = FONT_PAL_WHITE;
						}
					}
					StockFontBuffer(x * 10, y * 18, FONT_PRIO_FRONT, color, msg, 0);
				}
			}
			sprintf(msg, "gx = %5d / gy = %5d / hit = %d", mouseMapGx, mouseMapGy, checkHitMap(mouseMapGx, mouseMapGy));
			StockFontBuffer(332, 40, FONT_PRIO_FRONT, 0, msg, 0);
		}
		else if (tglSw == 5)
		{
			char msg[256];
			int x, y;

			x = mouseMapGx - mapAreaX1;
			y = mouseMapGy - mapAreaY1;
			sprintf(msg, "Tile  Bmp = %d", tile[y * mapAreaWidth + x]);
			StockFontBuffer(240, 34, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "Parts Bmp = %d", parts[y * mapAreaWidth + x]);
			StockFontBuffer(240, 54, FONT_PRIO_FRONT, 0, msg, 0);
			sprintf(msg, "Gx = %d / Gy = %d", mouseMapGx, mouseMapGy);
			StockFontBuffer(240, 74, FONT_PRIO_FRONT, 0, msg, 0);
		}
		else if (tglSw == 6)
		{
			char msg[256];

			if ((joy_trg[ 0 ] & JOY_RIGHT) && mapEffectRainLevel < 5)
				mapEffectRainLevel++;
			else if ((joy_trg[ 0 ] & JOY_LEFT) && mapEffectRainLevel > 0)
				mapEffectRainLevel--;
			sprintf(msg, "mapEffectRainLevel = %d", mapEffectRainLevel);
			StockFontBuffer(240, 34, FONT_PRIO_FRONT, 0, msg, 0);
		}
		else if (tglSw == 7)
		{
			char msg[256];

			if ((joy_trg[ 0 ] & JOY_RIGHT) && mapEffectSnowLevel < 5)
				mapEffectSnowLevel++;
			else if ((joy_trg[ 0 ] & JOY_LEFT) && mapEffectSnowLevel > 0)
				mapEffectSnowLevel--;
			sprintf(msg, "mapEffectSnowLevel = %d", mapEffectSnowLevel);
			StockFontBuffer(240, 34, FONT_PRIO_FRONT, 0, msg, 0);
		}
	}
#endif

	// ?��????????????
	if (sendEnFlag == 0 && eventWarpSendFlag == 0 && eventEnemySendFlag == 0)
		etcEventFlag = 0;


	extern bool isMouseHoverItem;
	if (mouseLeftCrick && isMouseHoverItem)
	{
		// ??��??��?
		turnAround();
		// ????��?
		getItem();
	}
	// ?��???���Y??????????
	// ??????????????
	if (mouseLeftCrick && !isMouseHoverItem)
	{
		if (lookAtAround())
			mouseLeftCrick = false;
#ifdef __TALK_TO_NPC
		bool TalkToNPC();
		if (TalkToNPC())
			mouseLeftCrick = false;
#endif
//Terry add 2003/11/25
		if (g_bTradesystemOpen)
			mouseLeftCrick = false;
//end
	}
	// ??��???��??????��?
	
#ifdef _MOUSE_DBL_CLICK
	if (mouseDblRightOn)
	{
	}
#endif
	// ?�h�I?�k��
	// �O��?????????�V??��?��??
	// ?????????????????
	if ((partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0) && etcSendFlag == 0 && etcEventFlag == 0)
	{
		// ??????��????�h??????
		if (mouseLeftPushTime >= MOVE_MODE_CHANGE_TIME)
			mouseCursorMode = MOUSE_CURSOR_MODE_MOVE;
		// ?�h??????
		if (mouseCursorMode == MOUSE_CURSOR_MODE_MOVE)
		{
			// ????????�G��??????
			if (mouseLeftCrick)
				mouseCursorMode = MOUSE_CURSOR_MODE_NORMAL;
			else
			// ?????��??�t??�h�I???
			{
				// ?????��??��
				if (befortime + MOVE_CLICK_WAIT_TIME <= TimeGetTime())
				{
					befortime = TimeGetTime();
					moveStackGx = mouseMapGx;
					moveStackGy = mouseMapGy;
					moveStackFlag = true;
				}
			}
		}
		else
		// �G��??????
		if (mouseLeftCrick && !isMouseHoverItem)
		{
			// ?????��??�t??�h�I???
			// ?????��??��
			if (befortime + MOVE_CLICK_WAIT_TIME <= TimeGetTime())
			{
				befortime = TimeGetTime();
				moveStackGx = mouseMapGx;
				moveStackGy = mouseMapGy;
				moveStackFlag = true;
			}
		}
	}

	// ?????�k????��?��?
	getPartyTbl();
	// ?��?????????�h??
	if (partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0)
		onceMoveProc();
	else
	// ?????��?????�h??
		partyMoveProc();

	return;
}

short _encountFlag = 0;
short _warpEventFlag = 0;
short _enemyEventFlag = 0;
int  _enemyEventDir;
short justGoalFlag = 0;	// ?����???????????
short _partyTbl[MAX_PARTY];	// ?????�ѩ�????????

void _etcEventCheck(void);
bool _execEtcEvent(void);
void _getMoveRoute2(void);
void setPcMovePointToChar(int, int);
void setPartyMovePoint(void);
void _mapMove(void);
void _partyMapMove(void);
void _setMapMovePoint(int, int);
bool _checkEncount(void);
void _sendEncount(void);
void _sendMoveRoute(void);
bool _checkWarpEvent(int, int);
void _sendWarpEvent(void);
bool _checkEnemyEvent(int, int);
void _sendEnemyEvent(void);
//void _checkEmptyMap(void);
bool checkEmptyMapData(int, int, int);

void updateMapArea(void)
{
	mapAreaX1 = nowGx+MAP_TILE_GRID_X1;
	mapAreaY1 = nowGy+MAP_TILE_GRID_Y1;
	mapAreaX2 = nowGx+MAP_TILE_GRID_X2;
	mapAreaY2 = nowGy+MAP_TILE_GRID_Y2;

	if (mapAreaX1 < 0)
		mapAreaX1 = 0;
	if (mapAreaY1 < 0)
		mapAreaY1 = 0;
	if (mapAreaX2 > nowFloorGxSize)
		mapAreaX2 = nowFloorGxSize;
	if (mapAreaY2 > nowFloorGySize)
		mapAreaY2 = nowFloorGySize;

	mapAreaWidth  = mapAreaX2 - mapAreaX1;
	mapAreaHeight = mapAreaY2 - mapAreaY1;
}

void onceMoveProc(void)
{
	int dir;
	// ??????��?????�L??��?��?
	nowSpdRate = 1.0F;
	// ????????�h??????
	if (mapEmptyFlag)
		return;
	if (sendEnFlag == 0	&& eventWarpSendFlag == 0 && eventEnemySendFlag == 0)
	{
		//cary say ����event��ִ�У��͸�server
		if (_execEtcEvent())
			return;
		if ((float)nextGx * GRID_SIZE == nowX && (float)nextGy * GRID_SIZE == nowY)
		{
			if (moveStackFlag && moveRouteCnt2 == 0)
			{
				moveStackFlag = false;
				getRouteMap();
				if (moveRouteCnt == 0)
					turnAround();
			}
			if (moveRouteCnt == 0 && moveRouteCnt2 == 0)
			{
				turnAround2(moveLastDir);
				moveLastDir = -1;
			}
			if (moveRouteCnt > 0 && moveRouteCnt2 == 0)
			{
				_getMoveRoute2();
				if (moveRouteCnt2 > 0)
					_etcEventCheck();
				_sendMoveRoute();
			}
			if (moveRouteCnt2 > 0)
			{
				_checkEmptyMap();
				dir = moveRoute2[0];
				shiftRouteMap2();
				setMapMovePoint(nowGx + moveAddTbl[dir][0], nowGy + moveAddTbl[dir][1]);
				setPcMovePointToChar(nowGx, nowGy);
			}
		}
	}
	// ??????????�\????????�h�I?�@�e??
	setPartyMovePoint();
	// ????????????��???????????
	justGoalFlag = 0;
	// ????????�h??????
	if (mapEmptyFlag)
		return;
	// ???�h????�h????????
	_mapMove();
	// ???????�h??
	_partyMapMove();
	updateMapArea();
	viewPointX = nowX;
	viewPointY = nowY;
}

// ?????????????
void partyMoveProc(void)
{
	int i;
	ACTION *ptAct, *ptActNext;

	// ??????????��??????�h�I?????�@�e?
	// ??????????????????��?�@�e?????
	for (i = 0; i < MAX_PARTY; i++)
	{
		if (_partyTbl[i] >= 0)
		{
			ptAct = party[_partyTbl[i]].ptAct;

			// ?????
			if (party[_partyTbl[i]].id != pc.id)
			{
				// ??????��???????�h�I?�@�e??
				if ((float)ptAct->nextGx * GRID_SIZE == ptAct->mx && (float)ptAct->nextGy * GRID_SIZE == ptAct->my)
				{
					// ????????��?????�@�e
					if (ptAct->bufCount > 0)
					{
						// ????????????�L??
						if (i == 0)
						{
							nowSpdRate = 1.0F;
							if (ptAct->bufCount > 5)
								nowSpdRate = 2.0F;
							else if (ptAct->bufCount >= 4)
								nowSpdRate = 1.6F;
							else if (ptAct->bufCount >= 2)
								nowSpdRate = 1.2F;
						}
						_setCharMovePoint(ptAct, ptAct->bufGx[0], ptAct->bufGy[0]);
						shiftBufCount(ptAct);
						if (_partyTbl[i + 1] >= 0 && (i + 1) < MAX_PARTY)
						{
							ptActNext = party[_partyTbl[i + 1]].ptAct;
							stockCharMovePoint(ptActNext, ptAct->gx, ptAct->gy);
						}
					}
				}
			}
			// ????
			else
			{
				// ??????��???????�h�I?�@�e??
				if ((float)nextGx * GRID_SIZE == nowX && (float)nextGy * GRID_SIZE == nowY)
				{
					// ????????��?????�@�e
					if (ptAct->bufCount > 0)
					{
						_setMapMovePoint(ptAct->bufGx[0], ptAct->bufGy[0]);
						shiftBufCount(ptAct);
						if (_partyTbl[i + 1] >= 0 && (i + 1) < MAX_PARTY)
						{
							ptActNext = party[_partyTbl[i + 1]].ptAct;
							stockCharMovePoint(ptActNext, ptAct->gx, ptAct->gy);
						}
					}
				}
			}
		}
		else
			break;
	}
	// ?�h??
	for (i = 0; i < MAX_PARTY; i++)
	{
		if (_partyTbl[i] >= 0)
		{
			if (party[_partyTbl[i]].id != pc.id)
				_charMove(party[_partyTbl[i]].ptAct);
			else
			{
				//mapMove2();
				_mapMove();
			}
		}
		else
			break;
	}
	updateMapArea();
	viewPointX = nowX;
	viewPointY = nowY;
}

///////////////////////////////////////////////////////////////////////////
// ?????�h�I?�@�e
void _setMapMovePoint(int _nextGx, int _nextGy)
{
	float dx, dy, len, dir1;
	int dir;

	nextGx = _nextGx;
	nextGy = _nextGy;
	// ?�h?????
	dx = nextGx * GRID_SIZE - nowX;
	dy = nextGy * GRID_SIZE - nowY;
	len = (float)sqrt((double)(dx * dx + dy * dy));
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	else
	{
		dx = 0;
		dy = 0;
	}
	nowVx = dx * MOVE_SPEED;
	nowVy = dy * MOVE_SPEED;
	// PC????��?�@�e
	if (dx != 0 || dy != 0)
	{
		dir1 = Atan(dx, dy) + 22.5F;
		AdjustDir(&dir1);
		dir = (int)(dir1 / 45);
		setPcDir(dir);
		setPcWalkFlag();
	}
	// PC?????????�t?�i???
	setPcPoint();
}

///////////////////////////////////////////////////////////////////////////
// ???????????�h??��??
// ??????????????��?�@�e????
void setPcMovePointToChar(int gx, int gy)
{
	if (partyModeFlag != 0 && (pc.status & CHR_STATUS_LEADER) != 0)
	{
		if (_partyTbl[1] >= 0)
			stockCharMovePoint(party[_partyTbl[1]].ptAct, gx, gy);
	}
}

///////////////////////////////////////////////////////////////////////////
// ???????????�h???�\??????�h???
void setPartyMovePoint(void)
{
	int i;
	ACTION *ptAct, *ptActNext;

	if (partyModeFlag != 0 && (pc.status & CHR_STATUS_LEADER) != 0)
	{
		for (i = 1; i < MAX_PARTY; i++)
		{
			if (_partyTbl[i] >= 0)
			{
				ptAct = party[_partyTbl[i]].ptAct;

				// ??????????��?
				if ((float)ptAct->nextGx * GRID_SIZE == ptAct->mx && (float)ptAct->nextGy * GRID_SIZE == ptAct->my)
				{
					// ????????��?????�@�e
					if (ptAct->bufCount > 0)
					{
						setCharMovePoint(ptAct, ptAct->bufGx[0], ptAct->bufGy[0]);
						shiftBufCount(ptAct);
						// ????????��?�J?
						if (_partyTbl[i + 1] >= 0 && (i + 1) < MAX_PARTY)
						{
							ptActNext = party[_partyTbl[i + 1]].ptAct;
							stockCharMovePoint(ptActNext, ptAct->gx, ptAct->gy);
						}
					}
				}
			}
			else
				break;
		}
	}
}

void _mapMove(void)
{
	float nGx, nGy, vx, vy;

	if (nowVx != 0 || nowVy != 0)
	{
		nGx = (float)nextGx * GRID_SIZE;
		nGy = (float)nextGy * GRID_SIZE;

		vx = nowVx * nowSpdRate;
		vy = nowVy * nowSpdRate;
		// ??��??����?????�h??????����???
		if (pointLen2(nowX, nowY, nGx, nGy) <= vx * vx + vy * vy)
		{
			// ?����?????????????
			nowX = nGx;
			nowY = nGy;
			nowVx = 0;
			nowVy = 0;
			justGoalFlag = 1;	// ???����????
		}
		else
		{
			// ?�h
			nowX += vx;
			nowY += vy;
		}



		setPcAction(ANIM_WALK);
		setPcWalkFlag();

		if (pc.ptAct != NULL)
			mapEffectMoveDir = pc.ptAct->anim_ang;
	}
	else
	{
		// PC????????????WALK???????????STAND???
		if (checkPcWalkFlag() == 1)
		{
			setPcAction(ANIM_STAND);
			delPcWalkFlag();
		}
		mapEffectMoveDir = -1;
	}

	{
		nowGx = (int)(nowX / GRID_SIZE);
		nowGy = (int)(nowY / GRID_SIZE);
	}
	// PC?????????�t?�i???
	setPcPoint();
}

///////////////////////////////////////////////////////////////////////////
// ???????????????
void _partyMapMove(void)
{
	int i;

	// ???????????�h???�\??????�h??
	if (partyModeFlag != 0 && (pc.status & CHR_STATUS_LEADER) != 0)
	{
		for (i = 1; i < MAX_PARTY; i++)
		{
			if (_partyTbl[i] >= 0)
				charMove2(party[_partyTbl[i]].ptAct);
			else
				break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// ?????�ѩ�????
void getPartyTbl(void)
{
	int i, j;

	if (partyModeFlag != 0)
	{
		// ???????
		for (i = 0; i < MAX_PARTY; i++)
			_partyTbl[i] = -1;
		// ?????�k?????��?��?
		for (i = 0, j = 0; i < MAX_PARTY; i++)
		{
			if (party[i].useFlag != 0 && party[i].ptAct != NULL)
			{
				_partyTbl[j] = i;
				j++;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// ?��?????????��?
enum
{
	etcEventMode_None,
	etcEventMode_Warp,
	etcEventMode_LocalEncount,
	etcEventMode_Enemy
};

bool _etcEventFlag = false;
short _etcEventStep = 0;
short _etcEventMode = etcEventMode_None;
short _eventWarpNo;

void _etcEventCheck(void)
{
	int i, dir, gx, gy, ogx, ogy;
	bool breakFlag;
	float tmpDir;

	gx = nowGx;
	gy = nowGy;
	ogx = gx;
	ogy = gy;
	breakFlag = false;

	for (i = 0; i < moveRouteCnt2; i++)
	{
		gx += moveAddTbl[moveRoute2[i]][0];
		gy += moveAddTbl[moveRoute2[i]][1];
		tmpDir = Atan((float)(gx - ogx), (float)(gy - ogy)) + 22.5F;
		AdjustDir(&tmpDir);
		dir = (int)(tmpDir / 45);

		if (_checkEnemyEvent(gx, gy))
		{
			_etcEventFlag = true;
			_etcEventStep = i;
			_etcEventMode = etcEventMode_Enemy;
			_enemyEventDir = dir - 3;
			if (_enemyEventDir < 0)
				_enemyEventDir += 8;
			i--;
			breakFlag = true;
			break;
		}
		else if (_checkWarpEvent(gx, gy))
		{
			_etcEventFlag = true;
			_etcEventStep = i + 1;
			_etcEventMode = etcEventMode_Warp;
			breakFlag = true;
			break;
		}
		else if (_checkEncount())
		{
			//cary �ɰ�����
			if (!bNewServer)
			{
				_etcEventFlag = true;
				_etcEventStep = i + 1;
				_etcEventMode = etcEventMode_LocalEncount;
				breakFlag = true;
			}
			break;
			//end cary
		}
		ogx = gx;
		ogy = gy;
	}
	if (i < MOVE_MAX2 && breakFlag)
	{
		moveRouteCnt2 = i + 1;
		moveRouteCnt = 0;
	}
}

bool _execEtcEvent(void)
{
	if (_etcEventFlag)
	{
		if (justGoalFlag)
		{
			if (_etcEventStep > 0)
				_etcEventStep--;
		}
		if (_etcEventStep <= 0)
		{
			switch (_etcEventMode)
			{
				case etcEventMode_Warp:
					_sendWarpEvent();
					break;
				case etcEventMode_LocalEncount:
					//cary �ɰ�����
					if (!bNewServer)
						_sendEncount();
					//end test
					break;
				case etcEventMode_Enemy:
					_sendEnemyEvent();
					break;
			}
			_etcEventFlag = false;
			_etcEventStep = 0;
			return true;
		}
	}
	return false;
}

void _getMoveRoute2(void)
{
	int i;

	for (i = 0; i < MOVE_MAX2 && moveRouteCnt > 0; i++)
	{
		moveRoute2[moveRouteCnt2] = moveRoute[0];
		moveRouteCnt2++;
		shiftRouteMap();
	}
}

void _sendMoveRoute(void)
{
	int i;
	if (moveRouteCnt2 <= 0)
		return;

	for (i = 0; i < moveRouteCnt2; i++)
		moveRouteDir[i] = cnvServDir(moveRoute2[i], 0);
#ifdef _DEBUG
	if (!offlineFlag)
#endif
	{
		if (partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0)
		{
			moveRouteDir[i] = '\0';

#ifdef MAP_CACHE_PROC
			if (checkMapCache(nowGx, nowGy, moveRoute2, moveRouteCnt2, nowFloor, nowFloorGxSize, nowFloorGySize))
				noChecksumWalkSendForServer(nowGx, nowGy, moveRouteDir);
			else
				walkSendForServer(nowGx, nowGy, moveRouteDir);
#else
			walkSendForServer(nowGx, nowGy, moveRouteDir);
#endif
		}
	}
}

bool _checkEncount(void)
{
	bool ret = false;

	if (EncountOffFlag)
		return false;

	if (partyModeFlag == 0 || (pc.status & CHR_STATUS_LEADER) != 0)
	{
		if (nowEncountPercentage > rand2())
		{
			ret = true;
			nowEncountExtra = 0;
		}
	}
	if (6 > nowEncountExtra)
		nowEncountExtra++;
	else
	{
		if (maxEncountPercentage > nowEncountPercentage)
			nowEncountPercentage++;
	}

	return ret;
}

void _sendEncount(void)
{
	resetMap();
	sendEnFlag = 1;
	etcEventFlag = 1;
	eventEnemyFlag = 0;

	if (bNewServer)
		lssproto_EN_send(sockfd, nowGx, nowGy);
	else
		old_lssproto_EN_send(sockfd, nowGx, nowGy);
}

bool _checkWarpEvent(int gx, int gy)
{
	int x, y, timeZoneNo;

	x = gx - mapAreaX1;
	y = gy - mapAreaY1;
	timeZoneNo = getLSTime (&SaTime);
	_eventWarpNo = (event[y * mapAreaWidth + x] & 0x0fff);
	// ???
	if (_eventWarpNo == EVENT_WARP)
		return true;
	// ��?????
	else if (_eventWarpNo == EVENT_WARP_MONING && timeZoneNo == LS_MORNING)
		return true;
	// ??????
	else if (_eventWarpNo == EVENT_WARP_NOON && (timeZoneNo == LS_NOON || timeZoneNo == LS_EVENING))
		return true;
	// ??????
	else if (_eventWarpNo == EVENT_WARP_NIGHT && timeZoneNo == LS_NIGHT)
		return true;

	return false;
}

void _sendWarpEvent(void)
{
	resetMap();
	eventWarpSendFlag = 1;
	etcEventFlag = 1;
	eventWarpSendId = eventId;
	if (bNewServer)
		lssproto_EV_send(sockfd, _eventWarpNo, eventId, nowGx, nowGy, -1);
	else
		old_lssproto_EV_send(sockfd, _eventWarpNo, eventId, nowGx, nowGy, -1);
	eventId++;
	wnCloseFlag = 1;	// �[???????��??
#ifdef __AI
	void AI_CloseWnd();
//			AI_CloseWnd();
#endif
	closeEtcSwitch();	// ?��@�e?????��??
	closeCharActionAnimeChange();	// ???????????��??
	closeJoinChannelWN();
	// ????����?
	SubProcNo = 200;
	// ??��??��???
	warpEffectProc();
	warpEffectFlag = true;
	floorChangeFlag = true;
	// ?????????????????
	if (MenuToggleFlag & JOY_CTRL_M)
		MapWmdFlagBak = true;
}

///////////////////////////////////////////////////////////////////////////
// ?�e????????????
//
// ????????
bool _checkEnemyEvent(int gx, int gy)
{
	int x, y, ev;

	x = gx - mapAreaX1;
	y = gy - mapAreaY1;

	ev = (event[y * mapAreaWidth + x] & 0x0fff);
	// ??????
	if (ev == EVENT_ENEMY)
		return true;

	return false;
}

void _sendEnemyEvent(void)
{
	resetMap();
	eventEnemySendFlag = 1;
	etcEventFlag = 1;
	eventEnemySendId = eventId;
	if (bNewServer)
		lssproto_EV_send(sockfd, EVENT_ENEMY, eventId,	nowGx, nowGy, _enemyEventDir);
	else
		old_lssproto_EV_send(sockfd, EVENT_ENEMY, eventId,	nowGx, nowGy, _enemyEventDir);
	eventId++;
	wnCloseFlag = 1;
	eventEnemyFlag = 1;
}

void _checkEmptyMap(void)
{
	int dir, i, j, gx, gy;
	
	i = 0;
	gx = nowGx;
	gy = nowGy;
	
	dir = moveRoute2[i];
	if (checkEmptyMapData(gx, gy, dir))
	{
		for (j = 0; j < getMapAreaCnt; j++)
		{
			if (bNewServer)
				lssproto_M_send(sockfd, nowFloor, getMapAreaX1[j], getMapAreaY1[j], getMapAreaX2[j], getMapAreaY2[j]);
			else
				old_lssproto_M_send(sockfd, nowFloor, getMapAreaX1[j], getMapAreaY1[j], getMapAreaX2[j], getMapAreaY2[j]);
		}
		mapEmptyFlag = true;
		mapEmptyDir = dir;
		mapEmptyGx = nowGx;
		mapEmptyGy = nowGy;
		mapEmptyStartTime = TimeGetTime();
	}
}

bool checkEmptyMapData(int _gx, int _gy, int dir)
{
	// ?????�t???11?????��??????��???true?��?
	int i, gx, gy, tx, ty, len, egx, egy;
	bool flag = false;

	if (mapAreaWidth < MAP_X_SIZE || mapAreaHeight < MAP_Y_SIZE)
		return false;

	getMapAreaCnt = 0;
	if (dir == 0 || dir == 1 || dir == 2)
	{
		gx = _gx - SEARCH_AREA;
		gy = _gy - SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		egx = gx + 1;
		egy = gy + (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if ((event[ty*mapAreaWidth+tx] & MAP_READ_FLAG) == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = egx;
					getMapAreaY2[getMapAreaCnt] = egy;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gy++;
			ty++;
		}
	}
	if (dir == 2 || dir == 3 || dir == 4)
	{
		gx = _gx - SEARCH_AREA;
		gy = _gy - SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		egx = gx + (SEARCH_AREA << 1) + 1;
		egy = gy + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if ((event[ty*mapAreaWidth+tx] & MAP_READ_FLAG) == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = egx;
					getMapAreaY2[getMapAreaCnt] = egy;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gx++;
			tx++;
		}
	}
	if (dir == 4 || dir == 5 || dir == 6)
	{
		gx = _gx + SEARCH_AREA;
		gy = _gy - SEARCH_AREA;
		tx = SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = -SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		egx = gx + 2;
		egy = gy + (SEARCH_AREA << 1) + 1;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if ((event[ty*mapAreaWidth+tx] & MAP_READ_FLAG) == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx;
					getMapAreaY1[getMapAreaCnt] = gy - 1;
					getMapAreaX2[getMapAreaCnt] = egx;
					getMapAreaY2[getMapAreaCnt] = egy;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gy++;
			ty++;
		}
	}
	if (dir == 6 || dir == 7 || dir == 0)
	{
		gx = _gx - SEARCH_AREA;
		gy = _gy + SEARCH_AREA;
		tx = -SEARCH_AREA - MAP_TILE_GRID_X1;
		ty = SEARCH_AREA - MAP_TILE_GRID_Y1;
		len = (SEARCH_AREA << 1) + 1;
		egx = gx + (SEARCH_AREA << 1) + 1;
		egy = gy + 2;
		for (i = 0; i < len; i++)
		{
			if ((0 <= gx && gx < nowFloorGxSize) && (0 <= gy && gy < nowFloorGySize))
			{
				if ((event[ty*mapAreaWidth+tx] & MAP_READ_FLAG) == 0)
				{
					getMapAreaX1[getMapAreaCnt] = gx - 1;
					getMapAreaY1[getMapAreaCnt] = gy;
					getMapAreaX2[getMapAreaCnt] = egx;
					getMapAreaY2[getMapAreaCnt] = egy;
					checkAreaLimit(&getMapAreaX1[getMapAreaCnt], &getMapAreaY1[getMapAreaCnt], &getMapAreaX2[getMapAreaCnt], &getMapAreaY2[getMapAreaCnt]);
					getMapAreaCnt++;
					flag = true;
					break;
				}
			}
			gx++;
			tx++;
		}
	}

	return flag;
}

// ?????no�k??????(x,y)??�h?
// ???????�q????
void goFrontPartyCharacter(int no, int x, int y)
{
	int i, ox, oy;
	ACTION *ptAct;

	// ????�ѩ�?��???????????��??
	if (party[0].ptAct == NULL)
		return;
	// ?????�h???��??
	ptAct = party[0].ptAct;
	if (ptAct->bufCount > 0 || (float)ptAct->nextGx*GRID_SIZE != ptAct->mx || (float)ptAct->nextGy*GRID_SIZE != ptAct->my)
		return;
	// �I??????�q????????��??��??
	for (i = no-1; i >= 0; i--)
	{
		if (party[i].useFlag && party[i].ptAct != NULL)
		{
			if (ABS(party[i].ptAct->nextGx - party[no].ptAct->nextGx) < 2 && ABS(party[i].ptAct->nextGy - party[no].ptAct->nextGy) < 2)
				return;
			i = -1;
			break;
		}
	}
	if (i >= 0)
		return;
	nowSpdRate = 1.0F;
	i = no;
	while (i < MAX_PARTY)
	{
		if (party[i].useFlag && party[i].ptAct != NULL)
		{
			ptAct = party[i].ptAct;
			ox = party[i].ptAct->nextGx;
			oy = party[i].ptAct->nextGy;
			stockCharMovePoint(ptAct, x, y);
			x = ox;
			y = oy;
		}
		i++;
	}
}

// ?????�h�I?�@�e
void setMapMovePoint(int _nextGx, int _nextGy)
{
	float dx, dy, len, dir1, rate = 1.0F;
	int dir;

	nextGx = _nextGx;
	nextGy = _nextGy;
	// ?�h?????
	dx = nextGx * GRID_SIZE - nowX;
	dy = nextGy * GRID_SIZE - nowY;
	len = (float)sqrt((double)(dx * dx + dy * dy));
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	else
	{
		dx = 0;
		dy = 0;
	}
	nowVx = dx * MOVE_SPEED * rate;
	nowVy = dy * MOVE_SPEED * rate;

	// PC????��?�@�e
	if (dx != 0 || dy != 0)
	{
		dir1 = Atan(dx, dy) + 22.5F;
		AdjustDir(&dir1);
		dir = (int)(dir1 / 45);
		setPcDir(dir);
		setPcWalkFlag();
	}
	// PC?????????�t?�i???
	setPcPoint();
}

// ?????�h�I?�@�e??�L?????
void setMapMovePoint2(int _nextGx, int _nextGy)
{
	float dx, dy, len, dir1, rate = 1.0F;
	int dir;

	if (pc.ptAct != NULL)
	{
		if (pc.ptAct->bufCount > 5)
			rate = 2.0F;
		else if (pc.ptAct->bufCount >= 4)
			rate = 1.6F;
		else if (pc.ptAct->bufCount >= 2)
			rate = 1.2F;
	}

	nextGx = _nextGx;
	nextGy = _nextGy;
	// ?�h?????
	dx = nextGx * GRID_SIZE - nowX;
	dy = nextGy * GRID_SIZE - nowY;
	len = (float)sqrt((double)(dx * dx + dy * dy));
	if (len > 0)
	{
		dx /= len;
		dy /= len;
	}
	else
	{
		dx = 0;
		dy = 0;
	}
	nowVx = dx * MOVE_SPEED * rate;
	nowVy = dy * MOVE_SPEED * rate;
	// PC????��?�@�e
	if (dx != 0 || dy != 0)
	{
		dir1 = Atan(dx, dy) + 22.5F;
		AdjustDir(&dir1);
		dir = (int)(dir1 / 45);
		setPcDir(dir);
		setPcWalkFlag();
	}
	// PC?????????�t?�i???
	setPcPoint();
}

// ????�h??
void mapMove2(void)
{
	float dx, dy;

	// ?�h???
	if (nowVx != 0 || nowVy != 0)
	{
		// ?����?????????????
		dx = (float)nextGx * GRID_SIZE;
		dy = (float)nextGy * GRID_SIZE;
		if (pointLen2(nowX, nowY, dx, dy) <= nowVx * nowVx + nowVy * nowVy)
		{
			nowX = dx;
			nowY = dy;
			nowVx = 0;
			nowVy = 0;
		}
		// ?�h
		else
		{
			nowX += nowVx;
			nowY += nowVy;
		}
		setPcAction(ANIM_WALK);
		setPcWalkFlag();
	}
	else
	{
		// PC????????????WALK???????????STAND???
		if (checkPcWalkFlag() == 1)
		{
			setPcAction(ANIM_STAND);
			delPcWalkFlag();
		}
	}

	nowGx = (int)(nowX / GRID_SIZE);
	nowGy = (int)(nowY / GRID_SIZE);
	// PC?????????�t?�i???
	setPcPoint();
}

///////////////////////////////////////////////////////////////////////////
// ??��????��??
//
void turnAround(void)
{
	float tmpDir, tmpX, tmpY;
	int dir;
	char dir2[2];
	static unsigned int turnSendTime = 0;

	// PC?????????????
	if (pc.ptAct == NULL)
		return;
	// ?�h?�N????????
	if (nowVx != 0 || nowVy != 0)
		return;
	// ???????????????????
	if (etcSendFlag != 0 || etcEventFlag != 0)
		return;
	{
		tmpX = (float)(mouseMapGx - nowGx);
		tmpY = (float)(mouseMapGy - nowGy);
	}
	tmpDir = Atan(tmpX, tmpY) + 22.5F;
	AdjustDir(&tmpDir);
	dir = (int)(tmpDir / 45);
	// ???�i????????
	if (pc.ptAct->anim_ang == dir)
		return;
	// ?��?��
	if (turnSendTime+FIELD_BTN_PUSH_WAIT < TimeGetTime())
	{
		setPcDir(dir);
		dir2[0] = cnvServDir(dir, 1);
		dir2[1] = '\0';
		walkSendForServer(nowGx, nowGy, dir2);
		turnSendTime = TimeGetTime();
	}
}

void turnAround2(int dir)
{
	char dir2[2];
	static unsigned int turnSendTime = 0;

	// ��??????????????
	if (dir < 0 || 7 < dir)
		return;
	// PC?????????????
	if (pc.ptAct == NULL)
		return;
	// ?�h?�N????????
	if (nowVx != 0 || nowVy != 0)
		return;
	// ???????????????????
	if (etcSendFlag != 0 || etcEventFlag != 0)
		return;
	// ???�i????????
	if (pc.ptAct->anim_ang == dir)
		return;
	// ?��?��
	if (turnSendTime + FIELD_BTN_PUSH_WAIT < TimeGetTime())
	{
		setPcDir(dir);
		dir2[0] = cnvServDir(dir, 1);
		dir2[1] = '\0';
		walkSendForServer(nowGx, nowGy, dir2);
		turnSendTime = TimeGetTime();
	}
}

///////////////////////////////////////////////////////////////////////////
// ?�h???����??
//
// ??��??dir��??1��?�h?????????��x??
void getRouteData(int dir, int *x, int *y)
{
	switch (dir)
	{
		case 0:
			*x = -1;
			*y = 1;
			break;

		case 1:
			*x = -1;
			*y = 0;
			break;

		case 2:
			*x = -1;
			*y = -1;
			break;

		case 3:
			*x = 0;
			*y = -1;
			break;

		case 4:
			*x = 1;
			*y = -1;
			break;

		case 5:
			*x = 1;
			*y = 0;
			break;

		case 6:
			*x = 1;
			*y = 1;
			break;

		case 7:
			*x = 0;
			*y = 1;
			break;

		default:
			*x = 0;
			*y = 0;
			break;
	}
}

// (gx1,gy1)??(gx2,gy2)????
int getDirData(int gx1, int gy1, int gx2, int gy2)
{
	float tmpDir, tmpX, tmpY;

	// ?����?��??��??
	tmpX = (float)(gx2 - gx1);
	tmpY = (float)(gy2 - gy1);
	tmpDir = Atan(tmpX, tmpY) + 22.5F;
	AdjustDir(&tmpDir);

	return (int)(tmpDir / 45);
}

// (gx1,gy2)??����??(gx2,gy2)??��????????��??
// ??�k��true ... ?��????
bool checkGridAround(int gx1, int gy1, int gx2, int gy2)
{
	if (((gx1 == gx2) && ABS(gy2 - gy1) == 1) || (ABS(gx2 - gx1) == 1 && (gy1 == gy2)) || (ABS(gx2 - gx1) == 1 && ABS(gy2 - gy1) == 1))
		return true;

	return false;
}

void getRouteMap(void)
{
	int mx = moveStackGx, my = moveStackGy, nowx = nowGx, nowy = nowGy, dir, dx, dy, targetDir, nowDir, checkDir1, checkDir2, i;
	int dirTbl[] = { 0, 1, -1 };
	float tmpDir;
	float tmpX, tmpY;
	bool flag;

	moveLastDir = -1;
	if (nowx == mx && nowy == my)
		return;
	tmpX = (float)(mx - nowx);
	tmpY = (float)(my - nowy);
	tmpDir = Atan(tmpX, tmpY) + 22.5F;
	AdjustDir(&tmpDir);
	targetDir = (int)(tmpDir / 45);

	moveRouteCnt = 0;

	while (nowx != mx || nowy != my)
	{
		if (moveRouteCnt >= MOVE_MAX)
		{
			moveRouteCnt = 0;
			return;
		}

		tmpX = (float)(mx - nowx);
		tmpY = (float)(my - nowy);
		tmpDir = Atan(tmpX, tmpY) + 22.5F;
		AdjustDir(&tmpDir);
		nowDir = (int)(tmpDir / 45);
		checkDir1 = targetDir + 1;
		checkDir1 &= 7;
		checkDir2 = targetDir - 1;
		checkDir2 &= 7;
		if (nowDir != targetDir && nowDir != checkDir1 && nowDir != checkDir2)
			break;
		flag = true;
		for (i = 0; i < sizeof(dirTbl) / sizeof(int); i++)
		{
			dir = nowDir + dirTbl[i];
			dir &= 7;
			checkDir1 = targetDir + 1;
			checkDir1 &= 7;
			checkDir2 = targetDir - 1;
			checkDir2 &= 7;
			if (dir != targetDir && dir != checkDir1 && dir != checkDir2)
				continue;
			getRouteData(dir, &dx, &dy);
			if (checkHitMap(nowx+dx, nowy+dy))
			{
				if ((nowx+dx) == mx && (nowy+dy) == my)
				{
					if ((dir % 2) == 0)
						continue;
					flag = true;
					break;
				}
				else
					continue;
			}
			if ((dir % 2) == 0)
			{
				int dir2, dx2, dy2, dir3, dx3, dy3;
				bool flag2, flag3;

				dir2 = dir + 1;
				dir2 &= 7;
				getRouteData(dir2, &dx2, &dy2);
				dir3 = dir - 1;
				dir3 &= 7;
				getRouteData(dir3, &dx3, &dy3);
				flag2 = checkHitMap(nowx + dx2, nowy + dy2);
				flag3 = checkHitMap(nowx + dx3, nowy + dy3);
				if (flag2 && flag3)
					break;
				if (flag2)
				{
					moveRoute[moveRouteCnt] = dir3;
					moveRouteGx[moveRouteCnt] = nowx + dx3;
					moveRouteGy[moveRouteCnt] = nowy + dy3;
					moveRouteCnt++;
					dx = (nowx + dx) - (nowx + dx3);
					dy = (nowy + dy) - (nowy + dy3);
					dir = getDirData(0, 0, dx, dy);
					nowx += dx3;
					nowy += dy3;
					flag = false;
					break;
				}
				if (flag3)
				{
					moveRoute[moveRouteCnt] = dir2;
					moveRouteGx[moveRouteCnt] = nowx + dx2;
					moveRouteGy[moveRouteCnt] = nowy + dy2;
					moveRouteCnt++;
					dx = (nowx + dx) - (nowx + dx2);
					dy = (nowy + dy) - (nowy + dy2);
					dir = getDirData(0, 0, dx, dy);
					nowx += dx2;
					nowy += dy2;
					flag = false;
					break;
				}
			}
			flag = false;
			break;
		}
		if (flag)
			break;
		nowx += dx;
		nowy += dy;
		moveRoute[moveRouteCnt] = dir;
		moveRouteGx[moveRouteCnt] = nowx;
		moveRouteGy[moveRouteCnt] = nowy;
		moveRouteCnt++;
	}
	dx = 0;
	if (mx - nowx > 0)
		dx = 1;
	else if (mx - nowx < 0)
		dx = -1;
	if (dx != 0)
	{
		while (!checkHitMap(nowx + dx, nowy) && mx != nowx)
		{
			nowx += dx;
			moveRoute[moveRouteCnt] = getDirData(0, 0, dx, 0);
			moveRouteGx[moveRouteCnt] = nowx;
			moveRouteGy[moveRouteCnt] = nowy;
			moveRouteCnt++;
		}
	}
	dy = 0;
	if (my - nowy > 0)
		dy = 1;
 	else if (my - nowy < 0)
		dy = -1;
	if (dy != 0)
	{
		while (!checkHitMap(nowx, nowy + dy) && my != nowy)
		{
			nowy += dy;
			moveRoute[moveRouteCnt] = getDirData(0, 0, 0, dy);
			moveRouteGx[moveRouteCnt] = nowx;
			moveRouteGy[moveRouteCnt] = nowy;
			moveRouteCnt++;
		}
	}
	if (moveRouteCnt > 0 && (nowx != mx || nowy != my))
	{
		tmpX = (float)(mx - nowx);
		tmpY = (float)(my - nowy);
		tmpDir = Atan(tmpX, tmpY) + 22.5F;
		AdjustDir(&tmpDir);
		moveLastDir = (int)(tmpDir / 45);
	}

	return;
}

void shiftRouteMap(void)
{
	int i;

	if (moveRouteCnt <= 0)
		return;

	moveRouteCnt--;
	for (i = 0; i < moveRouteCnt; i++)
	{
		moveRoute[i] = moveRoute[i + 1];
		moveRouteGx[i] = moveRouteGx[i + 1];
		moveRouteGy[i] = moveRouteGy[i + 1];
	}
}

void shiftRouteMap2(void)
{
	int i;

	if (moveRouteCnt2 <= 0)
		return;

	moveRouteCnt2--;
	for (i = 0; i < moveRouteCnt2; i++)
	{
		moveRoute2[i] = moveRoute2[i + 1];
	}
}

char cnvServDir(int dir, int mode)
{
	char ret = 'f';

	if (0 <= dir && dir <= 2)
	{
		if (mode == 0)
			ret = 'f' + dir;
		else
			ret = 'F' + dir;
	}
	else if (dir < 8)
	{
		if (mode == 0)
			ret = 'a' + dir - 3;
		else
			ret = 'A' + dir - 3;
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////
// ????????����?�I???�e?
// ???
void initCharPartsPrio(void)
{
	charPrioCnt = 0;
	charPrioBufTop.pre = NULL;
	charPrioBufTop.next = NULL;
}

// ???????????????
void setPartsPrio(int graNo, int x, int y, int dx, int dy, float mx, float my, int dispPrio)
{
	int i;
	CHAR_PARTS_PRIORITY *ptc, *prePtc;
	bool flag;

	// ?????��???��??
	if (charPrioCnt >= MAX_CHAR_PRIO_BUF)
		return;

	// ?????????????
	charPrioBuf[charPrioCnt].graNo = graNo;
	charPrioBuf[charPrioCnt].x     = x;
	charPrioBuf[charPrioCnt].y     = y;
	charPrioBuf[charPrioCnt].dx    = dx;
	charPrioBuf[charPrioCnt].dy    = dy;
	charPrioBuf[charPrioCnt].mx    = mx;
	charPrioBuf[charPrioCnt].my    = my;
	if (dispPrio == 0)
		charPrioBuf[charPrioCnt].type = CHAR_PARTS_PRIO_TYPE_ANI;
	else
		charPrioBuf[charPrioCnt].type  = CHAR_PARTS_PRIO_TYPE_PARTS;
	charPrioBuf[charPrioCnt].pre   = NULL;
	charPrioBuf[charPrioCnt].next  = NULL;
	charPrioBuf[charPrioCnt].depth = y;

	if (charPrioCnt == 0)
		addCharPartsPrio(&charPrioBufTop, &charPrioBuf[charPrioCnt]);
	else
	{
		// ???�K??????????
		flag = false;
		ptc = charPrioBufTop.next;
		for (i = 0; i < charPrioCnt && ptc != NULL; i++, ptc = ptc->next)
		{
			if (ptc->type == CHAR_PARTS_PRIO_TYPE_CHAR)
			{
				if (checkPrioPartsVsChar(ptc, &charPrioBuf[charPrioCnt]))
				{
					insertCharPartsPrio(ptc, &charPrioBuf[charPrioCnt]);
					flag = true;
					break;
				}
			}
			prePtc = ptc;
		}
		if (!flag)
			addCharPartsPrio(prePtc, &charPrioBuf[charPrioCnt]);
	}
	charPrioCnt++;
}

// ????????????
void setCharPrio(int graNo, int x, int y, int dx, int dy, float mx, float my)
{
	int i;
	CHAR_PARTS_PRIORITY *ptc, *prePtc;
	bool flag;

	// ?????��???��??
	if (charPrioCnt >= MAX_CHAR_PRIO_BUF)
		return;
	// ?????????????
	charPrioBuf[charPrioCnt].graNo = graNo;
	charPrioBuf[charPrioCnt].x     = x;
	charPrioBuf[charPrioCnt].y     = y;
	charPrioBuf[charPrioCnt].dx    = dx;
	charPrioBuf[charPrioCnt].dy    = dy;
	charPrioBuf[charPrioCnt].mx    = mx;
	charPrioBuf[charPrioCnt].my    = my;
	charPrioBuf[charPrioCnt].type  = CHAR_PARTS_PRIO_TYPE_CHAR;
	charPrioBuf[charPrioCnt].pre   = NULL;
	charPrioBuf[charPrioCnt].next  = NULL;
	charPrioBuf[charPrioCnt].depth = y;
	if (charPrioCnt == 0)
		addCharPartsPrio(&charPrioBufTop, &charPrioBuf[charPrioCnt]);
	else
	{
		// ???�K??????????
		flag = false;
		ptc = charPrioBufTop.next;
		for (i = 0; i < charPrioCnt && ptc != NULL; i++, ptc = ptc->next)
		{
			if (charPrioBuf[charPrioCnt].depth > ptc->depth)
			{
				insertCharPartsPrio(ptc, &charPrioBuf[charPrioCnt]);
				flag = true;
				break;
			}
			prePtc = ptc;
		}
		if (!flag)
			addCharPartsPrio(prePtc, &charPrioBuf[charPrioCnt]);
	}
	charPrioCnt++;
}

// ????????��??�I??????
//   ??�k��?????????��q?? true
bool checkPrioPartsVsChar(CHAR_PARTS_PRIORITY *ptc, CHAR_PARTS_PRIORITY *ptp)
{
	short hit, prioType;
	S2 w, h;

	// ��??�I???�e????��?��?
	realGetPrioType(ptp->graNo, &prioType);
	// ???�R�e??��
	realGetHitFlag(ptp->graNo, &hit);
	// ��??�Ģl? prioType == 3 ??????
	if (hit != 0 && prioType == 3)
		return false;
	// ????
	//  ????????
/*	if (329585 <= ptp->graNo && ptp->graNo <= 329590)
		return false;*/
	if (prioType == 1)
	{
		if (ptc->mx <= ptp->mx || ptc->my >= ptp->my)
			return false;
		else
			return true;
	}
#if 0
	// ????
	//  ????�i??????
	else if (prioType == 2)
	{
		// ??�@�e??????????
		if ((ptc->mx <= ptp->mx && ptc->my >= ptp->my) || (ptc->mx < ptp->mx-GRID_SIZE || ptc->my > ptp->my+GRID_SIZE))
			return false;
		else
			return true;
	}
#endif
	// ??????????????
#if 1
	if (ptc->mx > ptp->mx && ptc->my < ptp->my)
		return true;
	else
	{
		realGetHitPoints(ptp->graNo, &w, &h);
		if (ptc->x > ptp->x)
		{
			// PC??��??�D???
			if (ptp->y - (w - 1) * SURFACE_HEIGHT / 2 <= ptc->y)
				return false;
		}
		else if (ptc->x < ptp->x)
		{
			// PC??��??�D???
			if (ptp->y - (h - 1) * SURFACE_HEIGHT / 2 <= ptc->y)
				return false;
		}
		else
		{
			if (ptp->y <= ptc->y)
				return false;
		}
	}
#else
	realGetHitPoints(ptp->graNo, &w, &h);
	if (ptc->x >= ptp->x)
	{
		// PC??��??�D???
		if (ptp->y - (w - 1) * SURFACE_HEIGHT / 2 < ptc->y)
			return false;
	}
	else
	{
		// PC??��??�D???
		if (ptp->y - (h - 1) * SURFACE_HEIGHT / 2 < ptc->y)
			return false;
	}
#endif

	return true;
}

// pt1?��??pt2?���V??
void insertCharPartsPrio(CHAR_PARTS_PRIORITY *pt1, CHAR_PARTS_PRIORITY *pt2)
{
	if (pt1 == NULL || pt2 == NULL)
		return;

	pt2->pre = pt1->pre;
	pt2->next = pt1;
	(pt1->pre)->next = pt2;
	pt1->pre = pt2;
}

// pt1????p2?�D???
void addCharPartsPrio(CHAR_PARTS_PRIORITY *pt1, CHAR_PARTS_PRIORITY *pt2)
{
	if (pt1 == NULL || pt2 == NULL)
		return;

	pt2->pre = pt1;
	pt2->next = pt1->next;
	if (pt1->next != NULL)
		(pt1->next)->pre = pt2;
	pt1->next = pt2;
}

// pt1???
void delCharPartsPrio(CHAR_PARTS_PRIORITY *pt1)
{
	if (pt1 == NULL)
		return;
	// �I�d????????????
	if (pt1->pre == NULL)
		return;
	(pt1->pre)->next = pt1->next;
	if (pt1->next != NULL)
		(pt1->next)->pre = pt1->pre;
}

// ????����??
void stockCharParts(void)
{
	int i;
	CHAR_PARTS_PRIORITY *pt;

	pt = charPrioBufTop.next;
	if (pt == NULL)
		return;

	for (i = 0; i < charPrioCnt && pt != NULL; i++, pt = pt->next)
	{
		if (pt->type == CHAR_PARTS_PRIO_TYPE_ANI)
			StockDispBuffer2(pt->x + pt->dx, pt->y + pt->dy, 0, pt->graNo, 0);
		else
			StockDispBuffer2(pt->x + pt->dx, pt->y + pt->dy, DISP_PRIO_PARTS, pt->graNo, 0);
#if 0
		// ?????
		if (pt->type == CHAR_PARTS_PRIO_TYPE_CHAR)
			break;
#endif
	}
}

///////////////////////////////////////////////////////////////////////////
// ??????
//
#if 0	// ???????��?��??????�R�e?????
//
// NPC???????��??????
// ???????????????????
//
//   ??�k��true  ... ????�D��
//           false ... ??��?
bool checkNpcEvent(int gx, int gy, int dx, int dy)
{
	int x, y, ev;

	x = gx - mapAreaX1;
	y = gy - mapAreaY1;

	ev = (event[(y + dy) * mapAreaWidth + (x + dx)] & 0x0fff);
	// ?�h�I?NPC
	if (ev == EVENT_NPC)
	{
		// ?�h?��??
		resetMap();
		return true;
	}

	return false;
}
#endif

///////////////////////////////////////////////////////////////////////////
// ??????????
//
void drawAutoMap(int x, int y)
{
	if (autoMappingInitFlag)
	{
		createAutoMap(nowFloor, nowGx, nowGy);
		autoMappingInitFlag = false;
	}
	DrawAutoMapping(x, y, (unsigned char *)autoMappingBuf, AUTO_MAPPING_W, AUTO_MAPPING_H);
}

// (gx,gy)?????��??��?????????????
// autoMappingBuf??�t??
bool createAutoMap(int floor, int gx, int gy)
{
	FILE *fp;
	char filename[255];
	int fWidth, fHeight, fOffset, mWidth, width, height, fx, fy, mx, my, len, len2, i, j;
	int x1, y1, x2, y2, index;
	unsigned short tile[AUTO_MAPPING_W*AUTO_MAPPING_H];
	unsigned short parts[AUTO_MAPPING_W*AUTO_MAPPING_H];
	unsigned short event[AUTO_MAPPING_W*AUTO_MAPPING_H];

	memset(autoMappingBuf, 0, sizeof(autoMappingBuf));
	// ��???????��?��
	sprintf(filename, "/sdcard/jerrysa/map/%d.dat", floor);
	// ????????
	if ((fp = fopen(filename, "rb"))==NULL)
	{
		// ����???????????????��???
		// ??????????
		//_mkdir("map");
		// ��?????????
		fp = fopen(filename, "wb");
		fclose(fp);
		// ��??????????�T????
		if ((fp = fopen(filename, "rb")) == NULL)
			return false;
	}

	memset(tile, 0, sizeof(tile));
	memset(parts, 0, sizeof(parts));
	memset(event, 0, sizeof(event));
	fseek(fp, 0, SEEK_SET);
	fread(&fWidth,  sizeof(int), 1, fp);
	fread(&fHeight, sizeof(int), 1, fp);
	x1 = gx - (AUTO_MAPPING_W >> 1);
	y1 = gy - (AUTO_MAPPING_H >> 1);
	x2 = x1 + AUTO_MAPPING_W;
	y2 = y1 + AUTO_MAPPING_H;
	mWidth = x2 - x1;
	width = mWidth;
	height = y2 - y1;
	mx = 0;
	fx = x1;
	if (x1 < 0)
	{
		width += x1;
		fx = 0;
		mx -= x1;
	}
	if (x2 > fWidth)
		width -= (x2 - fWidth);
	my = 0;
	fy = y1;
	if (y1 < 0)
	{
		height += y1;
		fy = 0;
		my -= y1;
	}
	if (y2 > fHeight)
		height -= (y2 - fHeight);

	fOffset = sizeof(int) << 1;
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&tile[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}

	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&parts[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fOffset += sizeof(short) * (fWidth * fHeight);
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short) * len + fOffset, SEEK_SET);
		fread(&event[len2], sizeof(short) * width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}
	fclose (fp);
	// ???�@�e
	for (i = 0; i < AUTO_MAPPING_H; i++)
	{
		for (j = 0; j < AUTO_MAPPING_W; j++)
		{
			// ??????��?��?????????����???
			if (event[i * AUTO_MAPPING_W + j] & MAP_SEE_FLAG)
			{
				autoMappingBuf[i][j] = autoMapColorTbl[tile[i * AUTO_MAPPING_W + j]];
			}
		}
	}
	// ???�@�e
	for (i = 0; i < AUTO_MAPPING_H; i++)
	{
		for (j = 0; j < AUTO_MAPPING_W; j++)
		{
			// ??????��?��?????????����???
			if (event[i*AUTO_MAPPING_W+j] & MAP_SEE_FLAG)
			{
				index = autoMapColorTbl[parts[i * AUTO_MAPPING_W + j]];
				if (index != 0)
				{
					U4 bmpNo;
					S2 hit, hitX, hitY;
					int k, l;

					realGetNo(parts[i * AUTO_MAPPING_W + j], &bmpNo);
					// ???�R�e??��
					realGetHitFlag(bmpNo, &hit);
					// ???�R�e?????????�@�e
					if (hit == 0)
					{
						realGetHitPoints(bmpNo, &hitX, &hitY);
						for (k = 0; k < hitY; k++)
						{
							for (l = 0; l < hitX; l++)
							{
								if ((i - k) >= 0 && (j + l) < AUTO_MAPPING_W)
									autoMappingBuf[i - k][j + l] = index;
							}
						}
					}
					else
						autoMappingBuf[i][j] = index;
				}
			}
		}
	}

	return true;
}

/*
// ��????????��???
void readAutoMapSeeFlag(void)
{
	FILE *fp;
	char filename[255];
	int fWidth, fHeight, fOffset;
	int mWidth;
	int width, height;
	int fx, fy;
	int mx, my;
	int len, len2;
	int i, j;
	int x1, y1, x2, y2;


	// ��???????��?��
	sprintf(filename, "/sdcard/jerrysa/map/%d.dat", nowFloor);

	// ????????
	if ((fp = fopen(filename, "rb"))==NULL)
	{
		return;
	}

	memset(autoMapSeeFlagBuf, 0, sizeof(autoMapSeeFlagBuf));

	fseek(fp, 0, SEEK_SET);
	fread(&fWidth,  sizeof(int), 1, fp);
	fread(&fHeight, sizeof(int), 1, fp);

	x1 = nowGx - AUTO_MAPPING_SEE_W/2;
	y1 = nowGy - AUTO_MAPPING_SEE_H/2;
	x2 = x1 + AUTO_MAPPING_SEE_W;
	y2 = y1 + AUTO_MAPPING_SEE_H;


	mWidth = x2 - x1;
	width = mWidth;
	height = y2 - y1;

	mx = 0;
	fx = x1;
	if (x1 < 0)
	{
		width += x1;
		fx = 0;
		mx -= x1;
	}
	if (x2 > fWidth)
	{
		width -= (x2 - fWidth);
	}
	my = 0;
	fy = y1;
	if (y1 < 0)
	{
		height += y1;
		fy = 0;
		my -= y1;
	}
	if (y2 > fHeight)
	{
		height -= (y2 - fHeight);
	}


	fOffset = sizeof(int) * 2 + sizeof(short) * (fWidth * fHeight) * 2;
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short)*len+fOffset, SEEK_SET);
		fread(&autoMapSeeFlagBuf[len2], sizeof(short)*width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}

	fclose (fp);

	// �G??????????????????����????????
	for (i = 0; i < AUTO_MAPPING_SEE_H; i++)
	{
		for (j = 0; j < AUTO_MAPPING_SEE_W; j++)
		{
			autoMapSeeFlagBuf[i*AUTO_MAPPING_SEE_W+j] |= MAP_SEE_FLAG;
		}
	}

	autoMapSeeFloor = nowFloor;
	autoMapSeeGx = nowGx;
	autoMapSeeGy = nowGy;

	autoMapSeeFlag = true;
}


// ��????????????
void writeAutoMapSeeFlag(void)
{
	FILE *fp;
	char filename[255];
	int fWidth, fHeight, fOffset;
	int mWidth;
	int width, height;
	int fx, fy;
	int mx, my;
	int len, len2;
	int i;
	int x1, y1, x2, y2;


	// ?????????��?????????
	if (!autoMapSeeFlag)
		return;

	autoMapSeeFlag = false;

	// ��???????��?��
	sprintf(filename, "/sdcard/jerrysa/map/%d.dat", autoMapSeeFloor);

	// ????????
	if ((fp = fopen(filename, "rb+"))==NULL)
	{
		return;
	}


	fseek(fp, 0, SEEK_SET);
	fread(&fWidth,  sizeof(int), 1, fp);
	fread(&fHeight, sizeof(int), 1, fp);

	x1 = autoMapSeeGx - AUTO_MAPPING_SEE_W/2;
	y1 = autoMapSeeGy - AUTO_MAPPING_SEE_H/2;
	x2 = x1 + AUTO_MAPPING_SEE_W;
	y2 = y1 + AUTO_MAPPING_SEE_H;

	mWidth = x2 - x1;
	width = mWidth;
	height = y2 - y1;

	mx = 0;
	fx = x1;
	if (x1 < 0)
	{
		width += x1;
		fx = 0;
		mx -= x1;
	}
	if (x2 > fWidth)
	{
		width -= (x2 - fWidth);
	}
	my = 0;
	fy = y1;
	if (y1 < 0)
	{
		height += y1;
		fy = 0;
		my -= y1;
	}
	if (y2 > fHeight)
	{
		height -= (y2 - fHeight);
	}

	fOffset = sizeof(int) * 2 + sizeof(short) * (fWidth * fHeight) * 2;
	len = fy * fWidth + fx;
	len2 = my * mWidth + mx;
	for (i = 0; i < height; i++)
	{
		fseek(fp, sizeof(short)*len+fOffset, SEEK_SET);
		fwrite(&autoMapSeeFlagBuf[len2], sizeof(short)*width, 1, fp);
		len  += fWidth;
		len2 += mWidth;
	}

	fclose (fp);
}
*/


///////////////////////////////////////////////////////////////////////////
// ??????????
#ifdef MAP_CACHE_PROC


// ???
void initMapCache(void)
{
	int i;

	for (i = 0; i < MAX_MAP_CACHE_SIZE; i++)
	{
		mapCacheFloorNo[i] = -1;
		mapCacheFloorGxSize[i] = 0;
		mapCacheFloorGySize[i] = 0;
		clearMapCacheFlag(i);
		mapCacheLastTime[i] = 0;
	}
	mapCacheUse = 0;
}

// ?�h�I????????????????????
bool checkMapCache(int gx, int gy, short *buf, int size, int floor, int gxSize, int gySize)
{
	#define MAP_CACHE_RANGE		14
	#define MAP_CACHE_RANGE2	13
	int x1[2], y1[2], x2[2], y2[2];
	int tx1[2] = { 0xffff, 0xffff };
	int ty1[2] = { 0xffff, 0xffff };
	int tx2[2] = { 0, 0 };
	int ty2[2] = { 0, 0 };
	int dir, dx, dy, cnt, i, mask;
	unsigned char bit, tbit;

	// ?�h????��????????�d???��??
	tbit = 0;
	for (cnt = 0; cnt < size; cnt++)
	{
		dir = buf[cnt];
		bit = 0;
		if (0 <= dir && dir <= 2)
		{
			x1[0] = gx - MAP_CACHE_RANGE;
			x2[0] = x1[0] + 1;
			y1[0] = gy - MAP_CACHE_RANGE2;
			y2[0] = gy + MAP_CACHE_RANGE2 + 1;
			bit |= 1;
		}
		else if (4 <= dir && dir <= 6)
		{
			x1[0] = gx + MAP_CACHE_RANGE;
			x2[0] = x1[0] + 1;
			y1[0] = gy - MAP_CACHE_RANGE2;
			y2[0] = gy + MAP_CACHE_RANGE2 + 1;
			bit |= 1;
		}
		if (2 <= dir && dir <= 4)
		{
			x1[1] = gx - MAP_CACHE_RANGE2;
			x2[1] = gx + MAP_CACHE_RANGE2 + 1;
			y1[1] = gy - MAP_CACHE_RANGE;
			y2[1] = y1[1] + 1;
			bit |= 2;
		}
		else if (6 <= dir && dir <= 7 || dir == 0)
		{
			x1[1] = gx - MAP_CACHE_RANGE2;
			x2[1] = gx + MAP_CACHE_RANGE2 + 1;
			y1[1] = gy + MAP_CACHE_RANGE;
			y2[1] = y1[1] + 1;
			bit |= 2;
		}
		if ((bit & 1) != 0)
		{
			if (x1[0] < tx1[0])
				tx1[0] = x1[0];
			if (x2[0] > tx2[0])
				tx2[0] = x2[0];
			if (y1[0] < ty1[0])
				ty1[0] = y1[0];
			if (y2[0] > ty2[0])
				ty2[0] = y2[0];
			tbit |= 1;
		}
		if ((bit & 2) != 0)
		{
			if (x1[1] < tx1[1])
				tx1[1] = x1[1];
			if (x2[1] > tx2[1])
				tx2[1] = x2[1];
			if (y1[1] < ty1[1])
				ty1[1] = y1[1];
			if (y2[1] > ty2[1])
				ty2[1] = y2[1];
			tbit |= 2;
		}
		getRouteData(dir, &dx, &dy);
		gx += dx;
		gy += dy;
	}
	// ????�d?????��????????
	bit = 0;
	for (i = 0, mask = 1; i < 2; i++, mask <<= 1)
	{
		if ((tbit & mask) != 0)
		{
			if (checkMapCacheFlag(floor, gxSize, gySize, tx1[i], ty1[i], tx2[i], ty2[i]))
			{
				if (!checkMapCacheEvent(tx1[i], ty1[i], tx2[i], ty2[i]))
					bit |= mask;
			}
		}
	}
	if (tbit == bit)
		return true;
	else
		return false;
}

// ???????
void clearMapCacheFlag(int no)
{
	if (no < 0 || MAX_MAP_CACHE_SIZE <= no)
		return;
	memset(mapCacheFlag[no], 0, MAP_CACHE_X_BYTE*MAP_CACHE_Y_BYTE);
}

// ���e?�t???????????
//  ??�k��true  ... ????????
//			false ... ????????
bool checkMapCacheFlag(int floor, int gxSize, int gySize, int x1, int y1, int x2, int y2)
{
	int xByteStart, xByteEnd, yByteStart, yByteEnd, i, j;
	unsigned char bits, mask;
	bool ret = true;
	bool thisFloorCacheFlag = false;

	// ���e????�k?????��??
	j = mapCacheUse;
	for (i = 0; i < MAX_MAP_CACHE_SIZE; i++)
	{
		if (mapCacheFloorNo[j] == floor)
		{
			// ???????
			mapCacheUse = j;
			thisFloorCacheFlag = true;
			break;
		}
		j++;
		if (j >= MAX_MAP_CACHE_SIZE)
			j = 0;
	}
	// ??????��??
	if (!thisFloorCacheFlag)
	{
		bool flag;
		unsigned int tmpTime;

		// ??????????????�t??????????
		if (gxSize < 100 || gySize < 100)
			return false;
		// ?????????????????�S?
		flag = false;
		for (i = 0; i < MAX_MAP_CACHE_SIZE; i++)
		{
			if (mapCacheFloorNo[i] == -1)
			{
				flag = true;
				break;
			}
		}
		// ???��????�k????????��?
		if (!flag)
		{
			i = 0;
			tmpTime = 0xffffffff;
			for (j = 0; j < MAX_MAP_CACHE_SIZE; j++)
			{
				if (tmpTime > mapCacheLastTime[j])
				{
					i = j;
					tmpTime = mapCacheLastTime[j];
				}
			}
		}
		// ?????�K?
		mapCacheUse = i;
		mapCacheFloorNo[i] = floor;
		mapCacheFloorGxSize[i] = gxSize;
		mapCacheFloorGySize[i] = gySize;
		clearMapCacheFlag(i);
		ret = false;
	}
	// ?????????????�K?
	mapCacheLastTime[mapCacheUse] = TimeGetTime();
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 > mapCacheFloorGxSize[mapCacheUse])
		x2 = mapCacheFloorGxSize[mapCacheUse];
	if (y2 > mapCacheFloorGySize[mapCacheUse])
		y2 = mapCacheFloorGySize[mapCacheUse];
	// ???�d??
	if (x1 > x2 || y1 > y2)
		return false;

	xByteStart = x1 >> 3;	// xByteStart = x1 / 8
	xByteEnd = x2 >> 3;		// xByteEnd = x2 / 8;
	yByteStart = y1;
	yByteEnd = y2;
	for (i = xByteStart; i <= xByteEnd; i++)
	{
		bits = 0xff;
		if (i == xByteStart)
		{
			mask = ((unsigned char)0xff >> (x1 % 8));
			bits &= mask;
		}
		if (i == xByteEnd)
		{
			mask = ((unsigned char)0xff << (7 - (x1 % 8)));
			bits &= mask;
		}
		for (j = yByteStart; j <= yByteEnd; j++)
		{
			if ((mapCacheFlag[mapCacheUse][j * MAP_CACHE_X_BYTE + i] & bits) != bits)
				// ???????��??????
				ret = false;
			mapCacheFlag[mapCacheUse][j * MAP_CACHE_X_BYTE + i] |= bits;
		}
	}

	return ret;
}

// ????�ѩ���?EVENT_ALTERRATIVE?????true?��??
bool checkMapCacheEvent(int x1, int y1, int x2, int y2)
{
	int x, y, w, h, i, j;

	// ??????��??�d?????????��??
	// ???????????????
	if (x1 >= mapAreaX2 || x2 < 0 || y1 >= mapAreaY2 || y2 < 0)
		return true;

	w = x2 - x1;
	h = y2 - y1;
	x = x1 - mapAreaX1;
	y = y1 - mapAreaY1;
	if (x < 0)
	{
		w += x;
		x = 0;
	}
	if (y < 0)
	{
		h += y;
		y = 0;
	}
	if (mapAreaX1 + w > mapAreaX2)
		w -= (mapAreaX1 + w - mapAreaX2);
	if (mapAreaY1 + h > mapAreaY2)
		h -= (mapAreaY1 + h - mapAreaY2);
	for (i = 0; i < h; i++, y++)
	{
		for (j = 0; j < w; j++, x++)
		{
			if ((event[y * mapAreaWidth + x] & 0x0fff) == EVENT_ALTERRATIVE)
				return true;
		}
	}

	return false;
}
#endif

///////////////////////////////////////////////////////////////////////////
// ????????

#define MAX_MAP_EFFECT_BUF	500

MAP_EFFECT masterBufMapEffect[MAX_MAP_EFFECT_BUF];
MAP_EFFECT *emptyBufMapEffect;
MAP_EFFECT *useBufMapEffect;

short mapEffectDrawFlag = 0;
short mapEffectRainLevel = 0;
short oldMapEffectRainLevel = 0;
short mapEffectRainCnt = 0;
short mapEffectSnowLevel = 0;
short oldMapEffectSnowLevel = 0;
short mapEffectSnowCnt = 0;
short mapEffectMoveDir = -1;
short mapEffectStarFlag = 0;
short mapEffectFallingStarFlag = 0;
int mapEffectFallingStarTime;
#ifdef _HALLOWEEN_EFFECT
int mapEffectHalloween = 0;
#endif
short mapEffectKamiFubukiLevel = 0;
//short oldMapEffectKamiFubukiLevel = 0;
short mapEffectKamiFubukiCnt = 0;
#ifdef __EDEN_DICE
bool bMapEffectDice = false;
#endif

// ????????????
void initMapEffect(bool bFirstRun)
{
	int i;

	emptyBufMapEffect = &masterBufMapEffect[0];
	masterBufMapEffect[0].pre = (MAP_EFFECT *)NULL;
	for (i = 1; i < MAX_MAP_EFFECT_BUF; i++)
	{
		masterBufMapEffect[i - 1].next = &masterBufMapEffect[i];
		masterBufMapEffect[i].pre = &masterBufMapEffect[i - 1];
#ifdef _HALLOWEEN_EFFECT
		if (bFirstRun)
			masterBufMapEffect[i - 1].pAction = NULL;
		else
		{
			if (masterBufMapEffect[i - 1].pAction)
			{
				DeathAction(masterBufMapEffect[i - 1].pAction);
				masterBufMapEffect[i - 1].pAction = NULL;
			}
		}
#endif
	}
	masterBufMapEffect[i - 1].next = (MAP_EFFECT*)NULL;
	useBufMapEffect = (MAP_EFFECT*)NULL;
	mapEffectRainLevel = 0;
	mapEffectRainCnt = 0;
	mapEffectSnowLevel = 0;
	mapEffectSnowCnt = 0;
	mapEffectMoveDir = -1;
	mapEffectStarFlag = 0;
	mapEffectFallingStarFlag = 1;
	mapEffectKamiFubukiLevel = 0;
	mapEffectKamiFubukiCnt = 0;
#ifdef __EDEN_DICE
	bMapEffectDice = false;
#endif
#ifdef _HALLOWEEN_EFFECT
	mapEffectHalloween = 0;
#endif
}

// ??????????��x?�����@�e???
MAP_EFFECT *getMapEffectBuf(void)
{
	MAP_EFFECT *buf;

	if (emptyBufMapEffect == (MAP_EFFECT *)NULL)
		return (MAP_EFFECT*)NULL;
	buf = emptyBufMapEffect;
	emptyBufMapEffect = emptyBufMapEffect->next;
	if (emptyBufMapEffect != (MAP_EFFECT*)NULL)
		emptyBufMapEffect->pre = (MAP_EFFECT*)NULL;
	buf->next = useBufMapEffect;
	buf->pre  = (MAP_EFFECT*)NULL;
	if (useBufMapEffect != (MAP_EFFECT*)NULL)
		useBufMapEffect->pre = buf;
	useBufMapEffect = buf;
	return buf;
}

// ???????????��??
void delMapEffectBuf(MAP_EFFECT *buf)
{
	if (buf == (MAP_EFFECT*)NULL)
		return;
	if (buf->pre != (MAP_EFFECT*)NULL)
		buf->pre->next = buf->next;
	if (buf->next != (MAP_EFFECT*)NULL)
		buf->next->pre = buf->pre;
	if (useBufMapEffect == buf)
		useBufMapEffect = buf->next;
	buf->next = emptyBufMapEffect;
	buf->pre = (MAP_EFFECT*)NULL;
	emptyBufMapEffect = buf;
	buf->type = MAP_EFFECT_TYPE_NONE;
}

// ?�h???����
void mapEffectDirHosei(int *x, int *y)
{
	if (mapEffectMoveDir < 0)
		return;
	if (mapEffectMoveDir == 0)
		(*y) -= 2;
	else if (mapEffectMoveDir == 1)
	{
		(*x)++;
		(*y)--;
	}
	else if (mapEffectMoveDir == 2)
		(*x) += 2;
	else if (mapEffectMoveDir == 3)
	{
		(*x)++;
		(*y)++;
	}
	else if (mapEffectMoveDir == 4)
		(*y) += 2;
	else if (mapEffectMoveDir == 5)
	{
		(*x)--;
		(*y)++;
	}
	else if (mapEffectMoveDir == 6)
		(*x) -= 2;
	else if (mapEffectMoveDir == 7)
	{
		(*x)--;
		(*y)--;
	}
}

// sprdisp.cpp��?PutBmp()?????
void drawMapEffect(void)
{
	if (mapEffectDrawFlag)
	{
		DrawMapEffect();
		mapEffectDrawFlag = 0;
	}
}

// ????????��
void mapEffectProc(void)
{
	mapEffectRain();
	mapEffectSnow();
	mapEffectStar();
	mapEffectFallingStar();
	mapEffectKamiFubuki();	
#ifdef __EDEN_DICE
	mapEffectDice();
#endif
#ifdef _HALLOWEEN_EFFECT
	mapEffectHalloweenProc();
#endif
	if (transEffectPaletteStatus == 2 && transEffectPaletteAfterWaitTime == 0)
		transEffectPaletteAfterWaitTime = SDL_GetTicks();
	// ?????????�r??�V??????????��?
	else if (transEffectPaletteStatus == 2 && (SDL_GetTicks() - transEffectPaletteAfterWaitTime) > 2500)
	{
		transEffectPaletteStatus = 0;
		transEffectPaletteAfterWaitTime = 0;
	}
}

void mapEffectProc2(int n)
{
	mapEffectRain2(n);
	mapEffectSnow2(n);
}

// ???????
void mapEffectRain(void)
{
	short mapEffectRainWaitTime = 0;
	short mapEffectRainFrameMax = 0;
	int hosei1 = 840, hosei2 = -200, ww = DEF_APPSIZEX, hh = DEF_APPSIZEY, i;
	static unsigned int time = TimeGetTime();
	MAP_EFFECT *buf;

	// ???????????????
	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 370;
		hosei2 = -50;
	}
	// ?�D��
	if (mapEffectRainLevel)
	{
		if (mapEffectRainLevel < 3)
		{
			mapEffectRainWaitTime = (51 - 25 * mapEffectRainLevel);
			if (time + mapEffectRainWaitTime < TimeGetTime())
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_RAIN;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->dx = 2 + (rand() % 2);
					buf->dy = 9;
					time = TimeGetTime();
					mapEffectRainCnt++;
				}
			}
		}
		else
		{
			mapEffectRainFrameMax = (mapEffectRainLevel - 2) << 2;
			if (ResoMode == 1)
				mapEffectRainFrameMax >>= 1;
			for (i = 0; i < mapEffectRainFrameMax; i++)
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_RAIN;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->dx = 2 + (rand() % 2);
					buf->dy = 9;
					time = TimeGetTime();
					mapEffectRainCnt++;
				}
			}
		}
	}
	// ?�h??
	buf = useBufMapEffect;
	while (buf != (MAP_EFFECT *)NULL)
	{
		if (buf->type == MAP_EFFECT_TYPE_RAIN)
		{
			buf->x += buf->dx;
			buf->y += buf->dy;
			mapEffectDirHosei(&buf->x, &buf->y);
			if (ww <= buf->x || hh <= buf->y)
			{
				// ?�h��?
				MAP_EFFECT *buf2 = buf->next;
				delMapEffectBuf(buf);
				buf = buf2;
				mapEffectRainCnt--;
				continue;
			}
		}
		buf = buf->next;
		mapEffectDrawFlag = 1;
	}
}

// �D??????
void mapEffectSnow(void)
{
	short mapEffectSnowWaitTime = 0;
	short mapEffectSnowFrameMax = 0;
	int hosei1 = 940;
	int hosei2 = -200;
	int ww = DEF_APPSIZEX;
	int hh = DEF_APPSIZEY;
	static unsigned int time = TimeGetTime();
	MAP_EFFECT *buf;
	int i, j;
	// ???????????????
	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 370;
		hosei2 = -50;
	}
	// �D�D��
	if (mapEffectSnowLevel)
	{
		if (mapEffectSnowLevel < 3)
		{
			mapEffectSnowWaitTime = (51 - 25 * mapEffectSnowLevel);
			if (time + mapEffectSnowWaitTime < TimeGetTime())
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_SNOW;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->mode = (rand() % 2);
					j = (rand() % 2);
					if (j == 0)
					{
						buf->dx = 0 + (rand() % 2);
						buf->dy = 3 + (rand() % 2);
					}
					else if (j == 1)
					{
						buf->dx = 1;
						buf->dy = 4 + (rand() % 2);
					}
					time = TimeGetTime();
					mapEffectSnowCnt++;
				}
			}
		}
		else
		{
			mapEffectSnowFrameMax = (mapEffectSnowLevel - 2) << 1;
			if (ResoMode == 1)
				mapEffectSnowFrameMax >>= 1;
			for (i = 0; i < mapEffectSnowFrameMax; i++)
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_SNOW;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->mode = (rand() % 2);
					j = (rand() % 2);
					if (j == 0)
					{
						buf->dx = 0 + (rand() % 2);
						buf->dy = 3 + (rand() % 2);
					}
					else if (j == 1)
					{
						buf->dx = 1;
						buf->dy = 4 + (rand() % 2);
					}
					time = TimeGetTime();
					mapEffectSnowCnt++;
				}
			}
		}
	}
	// ?�h??
	buf = useBufMapEffect;
	while (buf != (MAP_EFFECT*)NULL)
	{
		if (buf->type == MAP_EFFECT_TYPE_SNOW)
		{
			buf->x += buf->dx;
			buf->y += buf->dy;
			mapEffectDirHosei(&buf->x, &buf->y);
			if (ww + 100 <= buf->x || hh <= buf->y)
			{
				// ?�h��?
				MAP_EFFECT *buf2 = buf->next;
				delMapEffectBuf(buf);
				buf = buf2;
				mapEffectSnowCnt--;
				continue;
			}
		}
		buf = buf->next;
		mapEffectDrawFlag = 1;
	}
}

// ��???????��??�e?
void setEffectStar(MAP_EFFECT *buf, int gx, int gy, int ggx, int ggy, int type)
{
	if (buf)
	{
		buf->type = MAP_EFFECT_TYPE_STAR;
		buf->gx = gx;
		buf->gy = gy;
		buf->ggx = ggx;
		buf->ggy = ggy;
		buf->type2 = type;
	}
}

void calEffectStar(MAP_EFFECT *buf)
{
	float mx, my;
	// ?������?�t
	camMapToGamen((float)buf->gx * GRID_SIZE, (float)buf->gy * GRID_SIZE, &mx, &my);
	buf->x = (int)(mx + .5) + buf->ggx;
	buf->y = (int)(my + .5) + buf->ggy;
	//???????????????
	if (ResoMode == 1)
	{
		buf->x >>= 1;
		buf->y >>= 1;
	}
}

void mapEffectStar(void)
{
	static MAP_EFFECT *buf[30];
	int i;

	if ((nowFloor == 1200 || nowFloor == 20105 || nowFloor == 10920 || nowFloor == 20406)
	 && NOON_TO_EVENING + 10 <= SaTime.hour && SaTime.hour < NIGHT_TO_MORNING + 80)
	{
		if (nowFloor == 1200 && mapEffectStarFlag == 0)
		{
			for (i = 0; i < sizeof(buf) / sizeof(void*); i++)
				buf[i] = NULL;
			buf[0] = getMapEffectBuf();
			setEffectStar(buf[0], 62, 28,   0,   0, 0);
			buf[1] = getMapEffectBuf();
			setEffectStar(buf[1], 63, 27,  -5,  -5, 1);
			buf[2] = getMapEffectBuf();
			setEffectStar(buf[2], 63, 28,  16, -10, 0);
			buf[3] = getMapEffectBuf();
			setEffectStar(buf[3], 63, 29,  10,   5, 1);
			buf[4] = getMapEffectBuf();
			setEffectStar(buf[4], 64, 29,   0,  -8, 1);

			buf[5] = getMapEffectBuf();
			setEffectStar(buf[5], 65, 29,  -8,   5, 0);
			buf[6] = getMapEffectBuf();
			setEffectStar(buf[6], 65, 30,  16,  10, 0);
			buf[7] = getMapEffectBuf();
			setEffectStar(buf[7], 66, 31,   0,   0, 1);
			buf[8] = getMapEffectBuf();
			setEffectStar(buf[8], 66, 30,   8, -12, 1);
			buf[9] = getMapEffectBuf();
			setEffectStar(buf[9], 67, 31,  20,   0, 0);

			buf[10] = getMapEffectBuf();
			setEffectStar(buf[10], 69, 34,  -8,  -8, 0);
			buf[11] = getMapEffectBuf();
			setEffectStar(buf[11], 70, 34,   5,  10, 1);
			buf[12] = getMapEffectBuf();
			setEffectStar(buf[12], 70, 35,   2,  24, 0);
			buf[13] = getMapEffectBuf();
			setEffectStar(buf[13], 70, 36, -20, -16, 1);
			buf[14] = getMapEffectBuf();
			setEffectStar(buf[14], 71, 36,   2,   4, 1);

			buf[15] = getMapEffectBuf();
			setEffectStar(buf[15], 71, 37,   5, -10, 1);
			buf[16] = getMapEffectBuf();
			setEffectStar(buf[16], 72, 37,   0,  12, 0);

			mapEffectStarFlag = 1;
		}
		else if (nowFloor == 20105 && mapEffectStarFlag == 0)
		{
			for (i = 0; i < sizeof(buf) / sizeof(void*); i++)
				buf[i] = NULL;
			buf[0] = getMapEffectBuf();
			setEffectStar(buf[0], 21,  8,   5,   0, 1);
			buf[1] = getMapEffectBuf();
			setEffectStar(buf[1], 20, 10,  -5,   5, 1);
			buf[2] = getMapEffectBuf();
			setEffectStar(buf[2], 18, 11,   5,  10, 0);
			buf[3] = getMapEffectBuf();
			setEffectStar(buf[3], 20, 12,   0,  -5, 1);
			buf[4] = getMapEffectBuf();
			setEffectStar(buf[4], 23, 11,   0,   0, 0);

			buf[5] = getMapEffectBuf();
			setEffectStar(buf[5], 23, 14,   0,   0, 0);
			buf[6] = getMapEffectBuf();
			setEffectStar(buf[6], 25, 13,   8,   6, 1);
			buf[7] = getMapEffectBuf();
			setEffectStar(buf[7], 25, 16,   0,   0, 1);
			buf[8] = getMapEffectBuf();
			setEffectStar(buf[8], 26, 18,   4,  -8, 0);
			buf[9] = getMapEffectBuf();
			setEffectStar(buf[9], 28, 16, -16,   0, 0);

			buf[10] = getMapEffectBuf();
			setEffectStar(buf[10], 21, 14,  -5,   0, 1);
			buf[11] = getMapEffectBuf();
			setEffectStar(buf[11], 27, 14,   0,   0, 1);
			buf[12] = getMapEffectBuf();
			setEffectStar(buf[12], 24, 15,   4, -10, 1);
			buf[13] = getMapEffectBuf();
			setEffectStar(buf[13], 28, 12,   5,  -8, 0);
			buf[14] = getMapEffectBuf();
			setEffectStar(buf[14], 32, 14,   0,   0, 0);

			buf[15] = getMapEffectBuf();
			setEffectStar(buf[15], 30, 17,   0,   0, 0);
			buf[16] = getMapEffectBuf();
			setEffectStar(buf[16], 32, 19,   0,   0, 1);
			buf[17] = getMapEffectBuf();
			setEffectStar(buf[17], 36, 19,   5,   2, 1);
			buf[18] = getMapEffectBuf();
			setEffectStar(buf[18], 34, 22,  -4,   4, 0);
			buf[19] = getMapEffectBuf();
			setEffectStar(buf[19], 36, 25,   1,   1, 0);

			buf[20] = getMapEffectBuf();
			setEffectStar(buf[20], 39, 23,   0,   0, 0);
			buf[21] = getMapEffectBuf();
			setEffectStar(buf[21], 38, 25,  -8,   2, 1);
			buf[22] = getMapEffectBuf();
			setEffectStar(buf[22], 40, 24,  -5,  -5, 1);
			buf[23] = getMapEffectBuf();
			setEffectStar(buf[23], 39, 27,   0,   0, 0);
			buf[24] = getMapEffectBuf();
			setEffectStar(buf[24], 39, 30,   0,   0, 1);

			buf[25] = getMapEffectBuf();
			setEffectStar(buf[25], 43, 29,   0,   0, 0);
			buf[26] = getMapEffectBuf();
			setEffectStar(buf[26], 42, 32,   2,   0, 0);
			buf[27] = getMapEffectBuf();
			setEffectStar(buf[27], 44, 33,   0,   0, 1);
			buf[28] = getMapEffectBuf();
			setEffectStar(buf[28], 43, 34,   8,   2, 0);
			buf[29] = getMapEffectBuf();
			setEffectStar(buf[29], 45, 36,   0,   0, 0);

			mapEffectStarFlag = 1;
		}
		else if (nowFloor == 10920 && mapEffectStarFlag == 0)
		{
			for (i = 0; i < sizeof(buf) / sizeof(void *); i++)
				buf[i] = NULL;
			buf[0] = getMapEffectBuf();
			setEffectStar(buf[0], 71,  2,   0,   0, 1);
			buf[1] = getMapEffectBuf();
			setEffectStar(buf[1], 71,  5,   0,   0, 0);
			buf[2] = getMapEffectBuf();
			setEffectStar(buf[2], 70,  7,   0,   0, 1);
			buf[3] = getMapEffectBuf();
			setEffectStar(buf[3], 73,  7,   0,   0, 1);
			buf[4] = getMapEffectBuf();
			setEffectStar(buf[4], 75,  6,   0,   0, 0);

			buf[5] = getMapEffectBuf();
			setEffectStar(buf[5], 75,  9,   0,   0, 0);
			buf[6] = getMapEffectBuf();
			setEffectStar(buf[6], 75, 11,   0,   0, 1);
			buf[7] = getMapEffectBuf();
			setEffectStar(buf[7], 77,  9,   0,   0, 1);
			buf[8] = getMapEffectBuf();
			setEffectStar(buf[8], 76, 13,   0,   0, 0);
			buf[9] = getMapEffectBuf();
			setEffectStar(buf[9], 79, 12,   0,   0, 0);

			buf[10] = getMapEffectBuf();
			setEffectStar(buf[10], 78, 15,   0,   0, 1);
			buf[11] = getMapEffectBuf();
			setEffectStar(buf[11], 80, 14,   0,   0, 0);
			buf[12] = getMapEffectBuf();
			setEffectStar(buf[12], 79, 16,   0,   0, 1);
			buf[13] = getMapEffectBuf();
			setEffectStar(buf[13], 80, 18,   0,   0, 0);
			buf[14] = getMapEffectBuf();
			setEffectStar(buf[14], 83, 18,   0,   0, 0);

			buf[15] = getMapEffectBuf();
			setEffectStar(buf[15], 83, 20,   0,   0, 1);
			buf[16] = getMapEffectBuf();
			setEffectStar(buf[16], 84, 20,   0,   0, 1);
			buf[17] = getMapEffectBuf();
			setEffectStar(buf[17], 84, 22,   0,   0, 0);
			buf[18] = getMapEffectBuf();
			setEffectStar(buf[18], 85, 19,   0,   0, 0);
			buf[19] = getMapEffectBuf();
			setEffectStar(buf[19], 87, 20,   0,   0, 1);

			buf[20] = getMapEffectBuf();
			setEffectStar(buf[20], 89, 21,   0,   0, 0);
			buf[21] = getMapEffectBuf();
			setEffectStar(buf[21], 89, 23,   0,   0, 1);
			buf[22] = getMapEffectBuf();
			setEffectStar(buf[22], 89, 26,   0,   0, 1);
			buf[23] = getMapEffectBuf();
			setEffectStar(buf[23], 91, 25,   0,   0, 0);
			buf[24] = getMapEffectBuf();
			setEffectStar(buf[24], 91, 27,   0,   0, 1);

			buf[25] = getMapEffectBuf();
			setEffectStar(buf[25], 93, 26,   0,   0, 1);
			buf[26] = getMapEffectBuf();
			setEffectStar(buf[26], 93, 30,   0,   0, 0);

			mapEffectStarFlag = 1;
		}
		else if (nowFloor == 20406 && mapEffectStarFlag == 0)
		{
			for (i = 0; i < sizeof(buf) / sizeof(void *); i++)
				buf[i] = NULL;
			buf[0] = getMapEffectBuf();
			setEffectStar(buf[0], 16,  8,   0,   0, 1);
			buf[1] = getMapEffectBuf();
			setEffectStar(buf[1], 16, 10,   0,   0, 0);
			buf[2] = getMapEffectBuf();
			setEffectStar(buf[2], 18, 10,   0,   0, 0);
			buf[3] = getMapEffectBuf();
			setEffectStar(buf[3], 19, 12,   0,   0, 1);
			buf[4] = getMapEffectBuf();
			setEffectStar(buf[4], 20, 12,   0,   0, 1);

			buf[5] = getMapEffectBuf();
			setEffectStar(buf[5], 21, 14,   0,   0, 0);
			buf[6] = getMapEffectBuf();
			setEffectStar(buf[6], 22, 16,   0,   0, 0);
			buf[7] = getMapEffectBuf();
			setEffectStar(buf[7], 23, 16,   0,   0, 1);
			buf[8] = getMapEffectBuf();
			setEffectStar(buf[8], 22, 11,   0,   0, 0);
			buf[9] = getMapEffectBuf();
			setEffectStar(buf[9], 26, 15,   0,   0, 0);

			buf[10] = getMapEffectBuf();
			setEffectStar(buf[10], 25, 18,   0,   0, 1);
			buf[11] = getMapEffectBuf();
			setEffectStar(buf[11], 26, 17,   0,   0, 1);
			buf[12] = getMapEffectBuf();
			setEffectStar(buf[12], 27, 13,   0,   0, 0);
			buf[13] = getMapEffectBuf();
			setEffectStar(buf[13], 29, 14,   0,   0, 1);
			buf[14] = getMapEffectBuf();
			setEffectStar(buf[14], 29, 12,   0,   0, 1);

			buf[15] = getMapEffectBuf();
			setEffectStar(buf[15], 31, 17,   0,   0, 0);
			buf[16] = getMapEffectBuf();
			setEffectStar(buf[16], 31, 19,   0,   0, 1);
			buf[17] = getMapEffectBuf();
			setEffectStar(buf[17], 34, 18,   0,   0, 1);
			buf[18] = getMapEffectBuf();
			setEffectStar(buf[18], 33, 21,   0,   0, 1);
			buf[19] = getMapEffectBuf();
			setEffectStar(buf[19], 34, 23,   0,   0, 0);

			buf[20] = getMapEffectBuf();
			setEffectStar(buf[20], 38, 26,   0,   0, 0);
			buf[21] = getMapEffectBuf();
			setEffectStar(buf[21], 41, 25,   0,   0, 1);
			buf[22] = getMapEffectBuf();
			setEffectStar(buf[22], 41, 26,   0,   0, 1);
			buf[23] = getMapEffectBuf();
			setEffectStar(buf[23], 41, 32,   0,   0, 0);
			buf[24] = getMapEffectBuf();
			setEffectStar(buf[24], 44, 31,   0,   0, 0);

			buf[25] = getMapEffectBuf();
			setEffectStar(buf[25], 44, 33,   0,   0, 0);
			buf[26] = getMapEffectBuf();
			setEffectStar(buf[26], 42, 43,   0,   0, 1);
			buf[27] = getMapEffectBuf();
			setEffectStar(buf[27], 45, 36,   0,   0, 0);
			buf[28] = getMapEffectBuf();
			setEffectStar(buf[28], 38, 21,   0,   0, 0);
			buf[29] = getMapEffectBuf();
			setEffectStar(buf[29], 37, 19,   0,   0, 1);

			mapEffectStarFlag = 1;
		}
		for (i = 0; i < sizeof(buf) / sizeof(void*); i++)
		{
			if (buf[i])
			{
				calEffectStar(buf[i]);
				// ????
				// ??��?
				if (SaTime.hour < NOON_TO_EVENING + 20 || NIGHT_TO_MORNING + 70 < SaTime.hour)
					buf[i]->mode = 5;
				// ????
				// ??��?
				else if ((NOON_TO_EVENING + 20 <= SaTime.hour && SaTime.hour < NOON_TO_EVENING +30) || NIGHT_TO_MORNING + 60 < SaTime.hour)
					buf[i]->mode = 6;
				// ????
				// ??��?
				else if ((NOON_TO_EVENING + 30 <= SaTime.hour && SaTime.hour < NOON_TO_EVENING + 40) || NIGHT_TO_MORNING + 40 < SaTime.hour)
					buf[i]->mode = 8;
				else
				{
					if (buf[i]->type2 == 0)
					{
						// ???����?��?��????
						// ??����???��?��????
						if (SaTime.hour < NOON_TO_EVENING + 50 || NIGHT_TO_MORNING + 30 < SaTime.hour)
							buf[i]->mode = 4;
						// ???��???��?��????
						// ??������?��?��????
						else if (SaTime.hour < NOON_TO_EVENING + 80 || NIGHT_TO_MORNING + 20 < SaTime.hour)
							buf[i]->mode = 1;
						else
							buf[i]->mode = (rand() % 2);
					}
					else if (buf[i]->type2 == 1)
						buf[i]->mode = 8;
				}
			}
		}
		mapEffectDrawFlag = 1;
	}
	else
	{
		if (mapEffectStarFlag)
		{
			for (i = 0; i < sizeof(buf) / sizeof(void*); i++)
			{
				delMapEffectBuf(buf[i]);
				buf[i] = NULL;
			}
			mapEffectStarFlag = 0;
		}
	}
}

// ??��?????
void mapEffectFallingStar(void)
{
	static MAP_EFFECT *buf = NULL;

	if (mapEffectFallingStarFlag != 0 && (SaTime.hour < EVENING_TO_NIGHT || NIGHT_TO_MORNING < SaTime.hour))
	{
		mapEffectFallingStarFlag = 0;
		mapEffectFallingStarTime = (rand() % 300) + 400;
	}
	if (!mapEffectFallingStarFlag && mapEffectFallingStarTime == SaTime.hour)
	{
		if (nowFloor == 10920)
		{
			buf = getMapEffectBuf();
			if (buf)
			{
				int i = (rand() % 5);

				switch (i)
				{
					case 0:
						setEffectStar(buf, 76, 6, 0, 0, 0);
						buf->dx = 10;
						buf->dy = 6;
						buf->ey = 120;
						break;
					case 1:
						setEffectStar(buf, 77, 7, 0, 0, 0);
						buf->dx = 8;
						buf->dy = 7;
						buf->ey = 120;
						break;
					case 2:
						setEffectStar(buf, 80, 13, 0, 0, 0);
						buf->dx = -10;
						buf->dy = 9;
						buf->ey = 120;
						break;
					case 3:
						setEffectStar(buf, 84, 19, 0, 0, 0);
						buf->dx = 12;
						buf->dy = 7;
						buf->ey = 90;
						break;
					case 4:
						setEffectStar(buf, 95, 26, 0, 0, 0);
						buf->dx = -10;
						buf->dy = 7;
						buf->ey = 120;
						break;
				}
				// ��?��?
				if ((rand() % 10) < 6)
					buf->mode = 8;
				else
					buf->mode = 1;
			}
			mapEffectFallingStarFlag = 1;
		}
		else if (nowFloor == 20105)
		{
			buf = getMapEffectBuf();
			if (buf)
			{
				int i = (rand() % 8);

				i = 7;
				switch (i)
				{
					case 0:
						setEffectStar(buf, 22, 9, 0, 0, 0);
						buf->dx = 10;
						buf->dy = 7;
						buf->ey = 120;
						break;
					case 1:
						setEffectStar(buf, 27, 15, 0, 0, 0);
						buf->dx = -8;
						buf->dy = 9;
						buf->ey = 120;
						break;
					case 2:
						setEffectStar(buf, 31, 14, 0, 0, 0);
						buf->dx = -10;
						buf->dy = 6;
						buf->ey = 120;
						break;
					case 3:
						setEffectStar(buf, 33, 18, 0, 0, 0);
						buf->dx = 12;
						buf->dy = 8;
						buf->ey = 120;
						break;
					case 4:
						setEffectStar(buf, 38, 22, 0, 0, 0);
						buf->dx = -11;
						buf->dy = 7;
						buf->ey = 120;
						break;
					case 5:
						setEffectStar(buf, 43, 27, 0, 0, 0);
						buf->dx = -9;
						buf->dy = 6;
						buf->ey = 120;
						break;
					case 6:
						setEffectStar(buf, 41, 29, 0, 0, 0);
						buf->dx = 12;
						buf->dy = 6;
						buf->ey = 120;
						break;
					case 7:
						setEffectStar(buf, 43, 33, 0, 0, 0);
						buf->dx = 14;
						buf->dy = 8;
						buf->ey = 80;
						break;
				}
				// ��?��?
				if ((rand() % 10) < 6)
					buf->mode = 8;
				else
					buf->mode = 1;
			}
			mapEffectFallingStarFlag = 1;
		}
		else if (nowFloor == 1200)
		{
			buf = getMapEffectBuf();
			if (buf)
			{
				int i = (rand() % 3);

				i = 2;
				switch (i)
				{
					case 0:
						setEffectStar(buf, 63, 26, 0, 0, 0);
						buf->dx = 10;
						buf->dy = 7;
						buf->ey = 80;
						break;
					case 1:
						setEffectStar(buf, 66, 30, 0, 0, 0);
						buf->dx = -10;
						buf->dy = 6;
						buf->ey = 70;
						break;
					case 2:
						setEffectStar(buf, 70, 35, 0, 0, 0);
						buf->dx = 8;
						buf->dy = 6;
						buf->ey = 40;
						break;
				}
				// ��?��?
				if ((rand() % 10) < 6)
					buf->mode = 8;
				else
					buf->mode = 1;
			}
			mapEffectFallingStarFlag = 1;
		}
		else if (nowFloor == 20406)
		{
			buf = getMapEffectBuf();
			if (buf)
			{
				int i = (rand() % 6);

				i = 5;
				switch (i)
				{
					case 0:
						setEffectStar(buf, 20, 11, 0, 0, 0);
						buf->dx = -12;
						buf->dy = 9;
						buf->ey = 80;
						break;
					case 1:
						setEffectStar(buf, 23, 11, 0, 0, 0);
						buf->dx = 10;
						buf->dy = 8;
						buf->ey = 120;
						break;
					case 2:
						setEffectStar(buf, 32, 14, 0, 0, 0);
						buf->dx = -9;
						buf->dy = 6;
						buf->ey = 120;
						break;
					case 3:
						setEffectStar(buf, 36, 20, 0, 0, 0);
						buf->dx = -10;
						buf->dy = 9;
						buf->ey = 120;
						break;
					case 4:
						setEffectStar(buf, 41, 24, 0, 0, 0);
						buf->dx = 11;
						buf->dy = 7;
						buf->ey = 120;
						break;
					case 5:
						setEffectStar(buf, 44, 32, 0, 0, 0);
						buf->dx = 10;
						buf->dy = 8;
						buf->ey = 120;
						break;
				}
				// ��?��?
				if ((rand() % 10) < 6)
					buf->mode = 8;
				else
					buf->mode = 1;
			}
			mapEffectFallingStarFlag = 1;
		}
	}
	if (buf)
	{
		buf->ggx += buf->dx;
		buf->ggy += buf->dy;
		if (buf->ggy > buf->ey)
		{
			delMapEffectBuf(buf);
			buf = NULL;
		}
		else
		{
			calEffectStar(buf);
			mapEffectDrawFlag = 1;
		}
	}
}

// �D??????
void mapEffectSnow2(int n)
{
	short mapEffectSnowWaitTime = 0;
	short mapEffectSnowFrameMax = 0;
	int hosei1 = 940, hosei2 = -200, ww = DEF_APPSIZEX, hh = DEF_APPSIZEY, time = 0, i, j, k;
	MAP_EFFECT *buf;
	
	// ???????????????
	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 420;
		hosei2 = -50;
	}
	for (k = 0; k < n; k++)
	{
		// �D�D��
		if (mapEffectSnowLevel)
		{
			if (mapEffectSnowLevel < 3)
			{
				mapEffectSnowWaitTime = (2 - mapEffectSnowLevel) << 1;
				if (time >= mapEffectSnowWaitTime)
				{
					MAP_EFFECT *buf = getMapEffectBuf();
					if (buf != (MAP_EFFECT*)NULL)
					{
						buf->type = MAP_EFFECT_TYPE_SNOW;
						buf->x = (rand() % hosei1) + hosei2;
						buf->y = 0;
						buf->mode = (rand() % 2);
						j = (rand() % 2);
						if (j == 0)
						{
							buf->dx = 0 + (rand() % 2);
							buf->dy = 3 + (rand() % 2);
						}
						else if (j == 1)
						{
							buf->dx = 1;
							buf->dy = 4 + (rand() % 2);
						}
						time = 0;
						mapEffectSnowCnt++;
					}
				}
			}
			else
			{
				mapEffectSnowFrameMax = (mapEffectSnowLevel - 2) << 1;
				if (ResoMode == 1)
					mapEffectSnowFrameMax >>= 1;
				for (i = 0; i < mapEffectSnowFrameMax; i++)
				{
					MAP_EFFECT *buf = getMapEffectBuf();
					if (buf != (MAP_EFFECT*)NULL)
					{
						buf->type = MAP_EFFECT_TYPE_SNOW;
						buf->x = (rand() % hosei1) + hosei2;
						buf->y = 0;
						buf->mode = (rand() % 2);
						j = (rand() % 2);
						if (j == 0)
						{
							buf->dx = 0 + (rand() % 2);
							buf->dy = 3 + (rand() % 2);
						}
						else if (j == 1)
						{
							buf->dx = 1;
							buf->dy = 4 + (rand() % 2);
						}
						time = 0;
						mapEffectSnowCnt++;
					}
				}
			}
		}
		// ?�h??
		i = 0;
		buf = useBufMapEffect;
		while (buf != (MAP_EFFECT*)NULL)
		{
			if (buf->type == MAP_EFFECT_TYPE_SNOW)
			{
				buf->x += buf->dx;
				buf->y += buf->dy;
				mapEffectDirHosei(&buf->x, &buf->y);
				if (ww + 100 <= buf->x || hh <= buf->y)
				{
					// ?�h��?
					MAP_EFFECT *buf2 = buf->next;
					delMapEffectBuf(buf);
					buf = buf2;
					mapEffectSnowCnt--;
					continue;
				}
			}
			buf = buf->next;
			mapEffectDrawFlag = 1;
			i++;
		}
		time++;
	}
}

// ???????
void mapEffectRain2(int n)
{
	short mapEffectRainWaitTime = 0;
	short mapEffectRainFrameMax = 0;
	int hosei1 = 840, hosei2 = -200, ww = DEF_APPSIZEX, hh = DEF_APPSIZEY, time = 0, i, k;
	MAP_EFFECT *buf;

	// ???????????????
	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 370;
		hosei2 = -50;
	}
	for (k = 0; k < n; k++)
	{
		// ?�D��
		if (mapEffectRainLevel)
		{
			if (mapEffectRainLevel < 3)
			{
				mapEffectRainWaitTime = (2 - mapEffectRainLevel) << 1;
				if (time >= mapEffectRainWaitTime)
				{
					MAP_EFFECT *buf = getMapEffectBuf();
					if (buf != (MAP_EFFECT*)NULL)
					{
						buf->type = MAP_EFFECT_TYPE_RAIN;
						buf->x = (rand() % hosei1) + hosei2;
						buf->y = 0;
						buf->dx = 2 + (rand() % 2);
						buf->dy = 9;
						time = 0;
						mapEffectRainCnt++;
					}
				}
			}
			else
			{
				mapEffectRainFrameMax = (mapEffectRainLevel - 2) << 2;
				if (ResoMode == 1)
					mapEffectRainFrameMax >>= 1;
				for (i = 0; i < mapEffectRainFrameMax; i++)
				{
					MAP_EFFECT *buf = getMapEffectBuf();
					if (buf != (MAP_EFFECT*)NULL)
					{
						buf->type = MAP_EFFECT_TYPE_RAIN;
						buf->x = (rand() % hosei1) + hosei2;
						buf->y = 0;
						buf->dx = 2 + (rand() % 2);
						buf->dy = 9;
						time = 0;
						mapEffectRainCnt++;
					}
				}
			}
		}
		// ?�h??
		i = 0;
		buf = useBufMapEffect;
		while (buf != (MAP_EFFECT*)NULL)
		{
			if (buf->type == MAP_EFFECT_TYPE_RAIN)
			{
				buf->x += buf->dx;
				buf->y += buf->dy;
				mapEffectDirHosei(&buf->x, &buf->y);
				if (ww <= buf->x || hh <= buf->y)
				{
					// ?�h��?
					MAP_EFFECT *buf2 = buf->next;
					delMapEffectBuf(buf);
					buf = buf2;
					mapEffectRainCnt--;
					continue;
				}
			}
			buf = buf->next;
			mapEffectDrawFlag = 1;
			i++;
		}
	}
}

// ??????????
void mapEffectKamiFubuki(void)
{
	short mapEffectKamiFubukiWaitTime = 0;
	short mapEffectKamiFubukiFrameMax = 0;
	int hosei1 = 1240, hosei2 = -100, ww = DEF_APPSIZEX, hh = DEF_APPSIZEY, i, j;
	static unsigned int time = TimeGetTime();
	MAP_EFFECT *buf;

	// ???????????????
	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 470;
		hosei2 = -50;
	}
	// ????�D��
	if (mapEffectKamiFubukiLevel)
	{
		if (mapEffectKamiFubukiLevel < 3)
		{
			mapEffectKamiFubukiWaitTime = (51 - 25 * mapEffectKamiFubukiLevel);
			if (time + mapEffectKamiFubukiWaitTime < TimeGetTime())
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_KAMIFUBUKI;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->mode = (rand() % 4);
					buf->cnt = 4;
					j = (rand() % 2);
					if (j == 0)
					{
						buf->dx = -1;
						buf->dy = 2;
					}
					else if (j == 1)
					{
						buf->dx = -1;
						buf->dy = 3;
					}
					time = TimeGetTime();
					mapEffectKamiFubukiCnt++;
				}
			}
		}
		else
		{
			mapEffectKamiFubukiFrameMax = (mapEffectKamiFubukiLevel - 2) << 1;
			if (ResoMode == 1)
				mapEffectKamiFubukiFrameMax >>= 1;
			for (i = 0; i < mapEffectKamiFubukiFrameMax; i++)
			{
				MAP_EFFECT *buf = getMapEffectBuf();
				if (buf != (MAP_EFFECT*)NULL)
				{
					buf->type = MAP_EFFECT_TYPE_KAMIFUBUKI;
					buf->x = (rand() % hosei1) + hosei2;
					buf->y = 0;
					buf->mode = (rand() % 4);
					buf->cnt = 4;
					j = (rand() % 2);
					if (j == 0)
					{
						buf->dx = -1;
						buf->dy = 2;
					}
					else if (j == 1)
					{
						buf->dx = -1;
						buf->dy = 3;
					}
					time = TimeGetTime();
					mapEffectKamiFubukiCnt++;
				}
			}
		}
	}
	// ?�h??
	i = 0;
	buf = useBufMapEffect;
	while (buf != (MAP_EFFECT*)NULL)
	{
		if (buf->type == MAP_EFFECT_TYPE_KAMIFUBUKI)
		{
			buf->x += (buf->dx + (rand() % 3) - 1);
			buf->y += (buf->dy + (rand() % 3) - 1);
			if (buf->cnt <= 0)
			{
				buf->type2 = (rand() % 3);
				buf->w = (rand() % 8) + 1;
				buf->h = (rand() % 8) + 1;
				buf->cnt = 4;
			}
			else
				buf->cnt--;
			mapEffectDirHosei(&buf->x, &buf->y);
			if (hh <= buf->y)
			{
				// ?�h��?
				MAP_EFFECT *buf2 = buf->next;
				delMapEffectBuf(buf);
				buf = buf2;
				mapEffectKamiFubukiCnt--;
				continue;
			}
		}
		buf = buf->next;
		mapEffectDrawFlag = 1;
		i++;
	}
}

///////////////////////////////////////////////////////////////////////////
// ?????��?????????????��?��????
//
void camMapToGamen(float sx, float sy, float *ex, float *ey)
{
	float x0, y0, x, y, tx = (float)(SURFACE_WIDTH >> 1), ty = (float)(SURFACE_HEIGHT >> 1);

	x0 = (sx - viewPointX) / GRID_SIZE;
	y0 = (sy - viewPointY) / GRID_SIZE;
	x = + x0 * tx + y0 * tx;
	y = - x0 * ty + y0 * ty;
	//?��?��
	*ex = x + viewOffsetX;
	*ey = y + viewOffsetY;
}

//
// ?��?��???????��(????????)????
//
void camGamenToMap(float sx, float sy, float *ex, float *ey)
{
	float x0, y0, x, y;

	// ?��?��
	x0 = sx - viewOffsetX; 
	y0 = sy - viewOffsetY; 
	x = x0 - (float)SURFACE_WIDTH / (float)SURFACE_HEIGHT * y0;
	y = x0 + (float)SURFACE_WIDTH / (float)SURFACE_HEIGHT * y0;
	*ex = x + viewPointX;
	*ey = y + viewPointY;
}

#ifdef __EDEN_DICE
extern char *pCommand;
extern unsigned long dwDiceTimer;

void mapEffectDice(void)
{
	char temp[16];
	static int d1, d2, i = 0, nState = 0;

	if (bMapEffectDice)
	{
		switch (nState)
		{
		case 0:
			getStringToken(pCommand, '|', 3, sizeof(temp), temp);
			d1 = atoi(temp);
			getStringToken(pCommand, '|', 6, sizeof(temp), temp);
			d2 = atoi(temp);
			FREE(pCommand);
#ifdef _DEBUG
			g_iMallocCount--;
#endif
			pCommand = NULL;
			nState++;
			delCharObj(1);
			break;
		case 1:
			if (TimeGetTime() - dwDiceTimer > 100)
			{
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
    #ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, gm_name);
    #else
#ifdef _NPC_PICTURE
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, 0);
#else
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
#endif
    #endif
#else
    #ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, gm_name);
    #else
		#ifdef _NPC_PICTURE
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
		#else
				setNpcCharObj(0, 26400 + i, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48);
		#endif
    #endif
#endif
				dwDiceTimer = TimeGetTime();
				if (++i > 10)
				{
					i = 0;
					nState++;
				}
			}
			break;
		case 2:
#ifdef _CHAR_PROFESSION			// WON ADD ����ְҵ
    #ifdef _GM_IDENTIFY		// Rog ADD GMʶ��
			setNpcCharObj(0, 26410 + d1, 12, 8, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, "");
			setNpcCharObj(1, 26416 + d2, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, "");
    #else
		#ifdef _NPC_PICTURE
			setNpcCharObj(0, 26410 + d1, 12, 8, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, 0);
			setNpcCharObj(1, 26416 + d2, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0, 0);
		#else
			setNpcCharObj(0, 26410 + d1, 12, 8, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
			setNpcCharObj(1, 26416 + d2, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
		#endif
    #endif
#else
			setNpcCharObj(0, 26410 + d1, 12, 8, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
			setNpcCharObj(1, 26416 + d2, 13, 9, 0, "", "", "", 1, "", 1, 0, 1, 0, 48, 0);
#endif
			bMapEffectDice = false;
			nState = 0;
			break;
		}
	}
}
#endif

#ifdef _HALLOWEEN_EFFECT
void mapEffectHalloweenProc(void)
{
	int hosei1 = 940, hosei2 = -200, ww = DEF_APPSIZEX, hh = DEF_APPSIZEY, j;
	static unsigned int time = TimeGetTime();
	MAP_EFFECT *buf;

	if (ResoMode == 1)
	{
		ww >>= 1;
		hh >>= 1;
		hosei1 = 370;
		hosei2 = -50;
	}
	if (mapEffectHalloween)
	{
		if (time + 100 < TimeGetTime())
		{
			MAP_EFFECT *buf = getMapEffectBuf();
			if (buf != (MAP_EFFECT*)NULL)
			{
				buf->type = MAP_EFFECT_TYPE_HALLOWEEN;
				buf->x = (rand() % hosei1) + hosei2;
				buf->y = 0;
				buf->mode = (rand() % 2);
				buf->pAction = MakeAnimDisp(buf->x, buf->y, mapEffectHalloween, 0);
				if (buf->pAction != NULL)
				{
					buf->pAction->hitDispNo = 0;
					buf->pAction->dispPrio = DISP_PRIO_GRID;
					j = (rand() % 2);
					if (j == 0)
					{
						buf->dx = rand() % 2;
						buf->dy = 3 + (rand() % 2);
					}
					else if (j == 1)
					{
						buf->dx = 1;
						buf->dy = 4 + (rand() % 2);
					}
				}
			}
			time = TimeGetTime();
		}
	}
	buf = useBufMapEffect;
	while (buf != (MAP_EFFECT*)NULL)
	{
		if (buf->type == MAP_EFFECT_TYPE_HALLOWEEN)
		{
			buf->pAction->x = buf->x += buf->dx;
			buf->pAction->y = buf->y += buf->dy;
			mapEffectDirHosei(&buf->x, &buf->y);
			if (ww + 100 <= buf->x || hh <= buf->y)
			{
				MAP_EFFECT *buf2 = buf->next;
				DeathAction(buf->pAction);
				buf->pAction = NULL;
				delMapEffectBuf(buf);
				buf = buf2;
				continue;
			}
		}
		buf = buf->next;
		mapEffectDrawFlag = 1;
	}
}
#endif

