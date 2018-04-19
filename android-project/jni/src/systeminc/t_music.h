/************************/
/*	t_music.h			*/
/************************/
#ifndef _T_MUSIC_
#define _T_MUSIC_

#define T_MUSIC_MONO	0
#define T_MUSIC_STEREO	1

#define TONE_MAX	500//400//256

typedef struct{
	int voice_place;	//????ýÍ???????????
	int voice_cnt;		//?????
	int voice_loop;		//??????
	int voice_volume;		//?ýè?????
	int voice_note;		//???§k?
	int play_time;		//?þØ??
	int voice_rate;
	int voice_address;
} TONE_EQU;

typedef struct
{
	char *fname;
	int volume;
	char loop_flg;
	int loop_point;
} T_MUSIC_BGM;

extern T_MUSIC_BGM bgm_tbl[];

extern TONE_EQU tone_tbl[];

int t_music_init(void);
void t_music_end(void);
extern int cdda_no;
extern int stereo_flg;
extern int t_music_se_volume;
extern int t_music_bgm_volume;
extern int t_music_bgm_no;
extern int t_music_se_no;
extern char t_music_bgm_pitch[16];
extern int draw_map_bgm_flg;
extern int map_bgm_no;

bool cdda_play(int);
bool cdda_stop(void);
int play_se(int tone, int x, int y);
int play_bgm(int bgm_no);
void bgm_volume_change(void);
void set_gbm_pitch(void);
void stop_bgm(void);
void fade_out_bgm(void);
int play_environment(int tone, int x, int y);
int play_map_bgm(int tone);

#endif
