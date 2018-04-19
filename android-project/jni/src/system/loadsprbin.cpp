
#define sprVERSION	3				//SPR ?  �n??????
#include "../systeminc/version.h"
#include 	"../systeminc/system.h"
#include 	"../systeminc/loadsprbin.h"
#include 	"../systeminc/anim_tbl.h"

#include	"../oft/vg410.h"
#include	"../oft/work.h"

#define ERRPRINT(a);

//cary 2001 07 10
#define maxBUFFER	1024*1024*6
//#define maxBUFFER	(0x2fffff)
U2 TBbuffer[maxBUFFER];
U2 *ptAnim;

SPRITEDATA SpriteData[mxSPRITE];


bool InitSprBinFileOpen( char *SPR_BIN, char *SPRADRN_BIN )
{
	char useFlag[mxSPRITE];
	SPRADRN spradrn[mxSPRITE];
	SPRADRN _spradrn;
	U2 buf[30000];
	U2 *ptBuf;
	FILE *fp1, *fp2;
	int i, j;
	unsigned int k;
	int size;
	if( (fp1 = fopen( SPR_BIN, "rb" )) == NULL )
		return false;
	if( (fp2 = fopen( SPRADRN_BIN, "rb" )) == NULL )
		return false;

	FILE* markFile;
	markFile = fopen("/sdcard/jerrysa/version.txt", "w");
	fwrite("1", 1, 1, markFile);
	fclose(markFile);
	memset( TBbuffer, 0, sizeof( TBbuffer ) );
	memset( SpriteData, 0, sizeof( SpriteData ) );
	memset( spradrn, 0, sizeof( spradrn ) );
	memset( useFlag, 0, sizeof( useFlag ) );
//	int max = -1;
	for( i = 0; i < mxSPRITE; i++ ){
//		if( i == 1177)
//			int debug = 0;
		fread( &_spradrn, 1, sizeof( _spradrn ), fp2 );
		if( feof( fp2 ) != 0 )
			break;
		int no = _spradrn.sprNo - SPRSTART;
//		if( no > max)
//			max = no;
		spradrn[no].sprNo = no;
		spradrn[no].offset = _spradrn.offset;
		spradrn[no].animSize = _spradrn.animSize;
		useFlag[no] = 1;
	}
	if( i >= mxSPRITE ){
		fclose( fp1 );
		fclose( fp2 );
		return false;
	}
	ptAnim = TBbuffer;
	for( i = 0; i < mxSPRITE; i++ ){
		if( i == 250)
			int debug = 0;
		if( useFlag[i] != 0 ){
			SpriteData[i].animSize = spradrn[i].animSize;
			SpriteData[i].ptAnimlist = (ANIMLIST *)ptAnim;
			ptAnim += sizeof( ANIMLIST ) / sizeof( U2 ) * SpriteData[i].animSize;
			fseek( fp1, spradrn[i].offset, SEEK_SET );
			for( j = 0; j < SpriteData[i].animSize; j++ ){
				fread( buf, 1, 12, fp1 );
				ptBuf = buf;

				SpriteData[i].ptAnimlist[j].dir = *ptBuf++;	// ??
				SpriteData[i].ptAnimlist[j].no = *ptBuf++;	// ????????????????�k?
				SpriteData[i].ptAnimlist[j].dtAnim = *ptBuf++;
				SpriteData[i].ptAnimlist[j].dtAnim |= ((*ptBuf)<<16);
				ptBuf++;
				SpriteData[i].ptAnimlist[j].frameCnt = *ptBuf++;
				SpriteData[i].ptAnimlist[j].frameCnt |= ((*ptBuf)<<16);
				ptBuf++;

				//???����??????��
				if( SpriteData[i].ptAnimlist[j].frameCnt ){
					SpriteData[i].ptAnimlist[j].dtAnim =
						SpriteData[i].ptAnimlist[j].dtAnim /
							(SpriteData[i].ptAnimlist[j].frameCnt << 4);
					if( SpriteData[i].ptAnimlist[j].dtAnim < 1 )
						SpriteData[i].ptAnimlist[j].dtAnim = 1;
				}
				SpriteData[i].ptAnimlist[j].ptFramelist = (FRAMELIST *)ptAnim;
				ptAnim += sizeof( FRAMELIST ) / sizeof( U2 )
					* SpriteData[i].ptAnimlist[j].frameCnt;

				size = 5 * SpriteData[i].ptAnimlist[j].frameCnt;
				fread( buf, 1, size*sizeof( U2 ), fp1 );
				ptBuf = buf;

				for( k = 0; k < SpriteData[i].ptAnimlist[j].frameCnt; k++ ){
					SpriteData[i].ptAnimlist[j].ptFramelist[k].BmpNo   = *ptBuf++;
					SpriteData[i].ptAnimlist[j].ptFramelist[k].BmpNo   |= ((*ptBuf)<<16);
					ptBuf++;
					SpriteData[i].ptAnimlist[j].ptFramelist[k].PosX    = *((S2 *)ptBuf);
					ptBuf++;
					SpriteData[i].ptAnimlist[j].ptFramelist[k].PosY    = *((S2 *)ptBuf);
					ptBuf++;
					SpriteData[i].ptAnimlist[j].ptFramelist[k].SoundNo = *ptBuf++;
					//Change fix ��ȫ��ѹͼ��350000��ʼ����,��350000��������,�ĳ���400000,���Ա�Ż����ѹ��ͼ�ظ���,ֻ�ð�ȫ�ʵı������Ӽ�50000,�˵�400000��
					if( (i >= 1769 && i <= 1812) || (i >= 1817 && i <= 1818) )//�������Ϊ101769��101818
					    SpriteData[i].ptAnimlist[j].ptFramelist[k].BmpNo += OLD_GRAPHICS_START-350000;
				}
			}
		}
	}
	fclose( fp1 );
	fclose( fp2 );

	// shan		
	SpriteData[1059].ptAnimlist[80].ptFramelist[8].BmpNo   = 283639;
	SpriteData[1059].ptAnimlist[93].ptFramelist[8].BmpNo   = 283640;	
	SpriteData[1283].ptAnimlist[7].ptFramelist[6].SoundNo  = 10100;
	SpriteData[1283].ptAnimlist[49].ptFramelist[6].SoundNo = 10100;
	SpriteData[1404].ptAnimlist[0].ptFramelist[6].BmpNo    = 284436;
	SpriteData[1409].ptAnimlist[0].ptFramelist[6].BmpNo    = 284476;
	// 
	for (int kk=0;kk < 8; kk++) {
		SpriteData[373].ptAnimlist[kk*7].ptFramelist[8].SoundNo  = 254;
		SpriteData[373].ptAnimlist[kk*7].ptFramelist[10].SoundNo  = 254;
		SpriteData[373].ptAnimlist[kk*7].ptFramelist[15].SoundNo  = 250;
	}
	//cary
	SpriteData[102].ptAnimlist[82].ptFramelist[0].BmpNo   = 126235;
	SpriteData[102].ptAnimlist[83].ptFramelist[0].BmpNo   = 126236;
	SpriteData[102].ptAnimlist[83].ptFramelist[1].BmpNo   = 126237;
	SpriteData[102].ptAnimlist[84].ptFramelist[1].BmpNo   = 126238;
	SpriteData[1058].ptAnimlist[80].ptFramelist[8].BmpNo   = 232475;
	SpriteData[1058].ptAnimlist[93].ptFramelist[8].BmpNo   = 232476;

	SpriteData[260].ptAnimlist[21].ptFramelist[5].SoundNo=10001;
	SpriteData[502].animSize=1;
	SpriteData[502].ptAnimlist = (ANIMLIST *)ptAnim;
	ptAnim += sizeof( ANIMLIST ) / sizeof( U2 ) * SpriteData[502].animSize;
	SpriteData[502].ptAnimlist[0].dir = 0;
	SpriteData[502].ptAnimlist[0].no = 0;
	SpriteData[502].ptAnimlist[0].dtAnim = 4;
	SpriteData[502].ptAnimlist[0].frameCnt = 3;

	SpriteData[502].ptAnimlist[0].ptFramelist = (FRAMELIST *)ptAnim;
	ptAnim += sizeof( FRAMELIST ) / sizeof( U2 ) * SpriteData[502].ptAnimlist[0].frameCnt;
	int iTemp=8590;
	for( k = 0; k < SpriteData[502].ptAnimlist[0].frameCnt; k++ ){
		SpriteData[502].ptAnimlist[0].ptFramelist[k].BmpNo   = iTemp++;
		SpriteData[502].ptAnimlist[0].ptFramelist[k].PosX    = 0;
		SpriteData[502].ptAnimlist[0].ptFramelist[k].PosY    = 0;
		SpriteData[502].ptAnimlist[0].ptFramelist[k].SoundNo = 0;
	}
	//end*/

	//Change fix Ͷ������û���趨SoundNo,ս��ʱ�ᵱ
	for(int kk=1965;kk<1987;kk++)
	{
		SpriteData[kk].ptAnimlist[9].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[22].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[35].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[48].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[61].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[74].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[87].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[100].ptFramelist[4].SoundNo = 10006;
	}
	for(int kk=1988;kk<1990;kk++)
	{
		SpriteData[kk].ptAnimlist[9].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[22].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[35].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[48].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[61].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[74].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[87].ptFramelist[4].SoundNo = 10006;
		SpriteData[kk].ptAnimlist[100].ptFramelist[4].SoundNo = 10006;
	}

	/*int kk = 2017;
	SpriteData[kk].ptAnimlist[0].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[13].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[26].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[39].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[52].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[65].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[78].ptFramelist[16].SoundNo = 10000;
	SpriteData[kk].ptAnimlist[91].ptFramelist[16].SoundNo = 10000;*/


	if( ptAnim >= &TBbuffer[maxBUFFER] )
		return false;

	return true;
}
