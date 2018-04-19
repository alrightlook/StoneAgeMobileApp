#ifndef _MAP_H_ 
#define _MAP_H_

// ??????����??????�d???��????�n�d??

#define MAP_TILE_GRID_X1	-20
#define MAP_TILE_GRID_X2	+17		// �Z?????
#define MAP_TILE_GRID_Y1	-16
#define MAP_TILE_GRID_Y2	+21		// �Z?????

// ???����???
#define MAP_X_SIZE	(MAP_TILE_GRID_X2 - MAP_TILE_GRID_X1)
#define MAP_Y_SIZE	(MAP_TILE_GRID_Y2 - MAP_TILE_GRID_Y1)

// ????????
#define GRID_SIZE		64

// ?�h�L�T
#define MOVE_SPEED			8
#define MOVE_FRAME			(GRID_SIZE/MOVE_SPEED)

// ????�k?
enum
{
	EVENT_NONE,			// ?????�D��???
	EVENT_NPC,			// ?�eNPC
	EVENT_ENEMY,		// ?�e��??????
	EVENT_WARP,			// ???
	EVENT_DOOR,			// ??
	EVENT_ALTERRATIVE,	// ??????�l????????????��???
	EVENT_WARP_MONING,	// ��?�e???
	EVENT_WARP_NOON,	// ??�e???
	EVENT_WARP_NIGHT,	// ??�e???

	EVENT_END		// ????�k????
};

// ???????
#define MAP_READ_FLAG	0x8000		// ???????????????????
#define MAP_SEE_FLAG	0x4000		// ??????��??????????

// ???��?��?
#define FLOOR_NAME_LEN	24

extern char nowFloorName[];
extern int nowFloor;
extern int nowFloorGxSize, nowFloorGySize;
extern int nowGx, nowGy;
extern float nowX, nowY;
extern float nowVx, nowVy, nowSpdRate;
extern int nextGx, nextGy;
extern int oldGx, oldGy;
extern int oldNextGx, oldNextGy;
extern int mouseMapGx, mouseMapGy;
extern int mouseMapX, mouseMapY;

extern short moveRouteCnt;

extern int mapEmptyFlag;

extern float viewPointX;
extern float viewPointY;
extern int viewOffsetX;
extern int viewOffsetY;

extern short moveAddTbl[8][2];


extern bool mouseLeftCrick;
extern bool mouseLeftOn;
extern bool mouseRightCrick;
extern bool mouseRightOn;
#ifdef _MOUSE_DBL_CLICK
extern bool mouseDblRightOn;
#endif
extern unsigned int mouseLeftPushTime;
extern unsigned int beforeMouseLeftPushTime;


extern short nowEncountPercentage;
extern short nowEncountExtra;
extern short minEncountPercentage;
extern short maxEncountPercentage;
extern short sendEnFlag;
extern short encountNowFlag;

extern short eventWarpSendFlag;
extern short eventWarpSendId;
extern short eventEnemySendFlag;
extern short eventEnemySendId;
extern short eventEnemyFlag;

extern bool loginFlag;

extern short vsLookFlag;

extern bool floorChangeFlag;

extern bool warpEffectFlag;
extern bool warpEffectStart;
extern bool warpEffectOk;

extern int transmigrationEffectFlag;
extern int transEffectPaletteStatus;

void initMap( void );
void resetMap( void );
void createMap( int, int, int );
bool writeMap( int, int, int, int, int,
	unsigned short *, unsigned short *, unsigned short * );
void setMap( int, int, int );
void setWarpMap( int, int );
bool mapCheckSum( int, int, int, int, int, int, int, int );

#ifdef _NEWDRAWBATTLEMAP		   // (���ɿ���) Syu ADD �Զ�����BattleMap
void ddrawBattleMap( void );
#endif
void drawMap( void );
void drawMap2( void );
void drawTile( void );
void redrawMap( void );
void drawGrid( void );
void moveProc( void );
bool checkEmptyMap( int );
void setMapMovePoint( int, int );
void setMapMovePoint2( int, int );
void mapMove2( void );
void updateMapArea( void );

void getRouteData( int, int *, int * );

char cnvServDir( int, int );

void goFrontPartyCharacter( int, int, int );


void initCharPartsPrio( void );
void setPartsPrio( int, int, int, int, int, float, float, int);
void setCharPrio( int, int, int, int, int, float, float );
void stockCharParts( void );


#if 0
bool checkNpcEvent( int, int, int, int );
#endif

void drawAutoMap( int x, int y );
void readAutoMapSeeFlag( void );
void writeAutoMapSeeFlag( void );

bool checkHitMap( int, int );

void camMapToGamen( float, float, float *, float * );
void camGamenToMap( float, float, float *, float * );


#if 1

enum
{
	MAP_EFFECT_TYPE_NONE,
	MAP_EFFECT_TYPE_RAIN,
	MAP_EFFECT_TYPE_SNOW,
	MAP_EFFECT_TYPE_STAR,
	MAP_EFFECT_TYPE_KAMIFUBUKI,
	MAP_EFFECT_TYPE_HOTARU
#ifdef _HALLOWEEN_EFFECT
	,MAP_EFFECT_TYPE_HALLOWEEN
#endif
};

typedef struct _MAP_EFFECT
{
	int x, y;
	int dx, dy;
	int gx, gy;
	int ggx, ggy;
	int ex, ey;
	int w, h;
	int cnt;
	_MAP_EFFECT *pre;
	_MAP_EFFECT *next;
	short type;
	short type2;
	short mode;
#ifdef _HALLOWEEN_EFFECT
	ACTION *pAction;
#endif
} MAP_EFFECT;

extern MAP_EFFECT *useBufMapEffect;

extern short mapEffectRainLevel;
extern short oldMapEffectRainLevel;
extern short mapEffectSnowLevel;
extern short oldMapEffectSnowLevel;
extern short mapEffectMoveDir;
extern short mapEffectKamiFubukiLevel;
#ifdef _HALLOWEEN_EFFECT
extern int mapEffectHalloween;
#endif
#ifdef __EDEN_DICE
extern bool bMapEffectDice;
#endif


extern const short fastDrawTile;
extern short fastDrawTileFlag;
extern int amountXFastDraw, amountYFastDraw;
extern int nowXFastDraw, nowYFastDraw;
extern int baseXFastDraw, baseYFastDraw;
extern int amountXFastDraw2, amountYFastDraw2;
extern int nowXFastDraw2, nowYFastDraw2;
extern int baseXFastDraw2, baseYFastDraw2;

void initMapEffect( bool bFirstRun );
void drawMapEffect( void );
void mapEffectProc( void );
void mapEffectRain( void );
void mapEffectSnow( void );
void mapEffectStar( void );
void mapEffectFallingStar( void );
void mapEffectKamiFubuki( void );
void mapEffectDice(void);

#ifdef _HALLOWEEN_EFFECT
void mapEffectHalloweenProc(void);
#endif

void mapEffectProc2( int );
void mapEffectRain2( int );
void mapEffectSnow2( int );

void _checkEmptyMap( void );

#endif

#ifdef _ITEM_PATH
	extern bool ITEMPATHFLAG;
#endif

#endif
