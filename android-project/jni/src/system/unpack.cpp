#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../systeminc/version.h"
#include"../systeminc/unpack.h"

#ifdef _DEBUG
extern int g_iMallocCount;
#endif

#define BIT_CMP			(unsigned char)0x80
#define BIT_ZERO		(unsigned char)0x40
#define BIT_REP_LARG	(unsigned char)0x10
#define BIT_REP_LARG2	(unsigned char)0x20
unsigned char *encoder( unsigned char *buf, unsigned char **disBuf,
	unsigned int width, unsigned int height, unsigned int *len, int cmpFlag )
{
	unsigned char *wBuf, *ewBuf, *eBuf;
	unsigned char *wBuf1, *buf1;
	unsigned char *wBuf2, *buf2;
	unsigned char idx;
	int flag = 1;
	RD_HEADER *header;
	unsigned int cnt, cnt2;
	unsigned char repData;
	int mode;
	unsigned int l, el;
	int addWBuf;

	if( *disBuf == NULL ){
		if( (wBuf = (unsigned char *)malloc( width * height + sizeof( RD_HEADER ) )) == NULL )
		{
			return NULL;
		}
#ifdef _DEBUG
		g_iMallocCount++;
#endif
		*disBuf = wBuf;
	}else{
		wBuf = *disBuf;
	}

	if( cmpFlag ){
		flag = 1;
	}else{
		flag = 0;
	}
	eBuf = buf + width * height;
	ewBuf = wBuf + width * height + sizeof( RD_HEADER );
	wBuf1 = wBuf+sizeof( RD_HEADER );
	buf1 = buf;
	while( cmpFlag ){
		if( buf1 >= eBuf ){
			flag = 1;
			break;
		}
		if( wBuf1 >= ewBuf ){
			flag = 0;
			break;
		}

		if( *buf1 == 0 && *(buf1+1) == 0 ){
			idx = BIT_CMP | BIT_ZERO;
			cnt = 2;
			buf1 += 2;
			while( buf1 < eBuf && cnt < 0xfffff ){
				if( *buf1 != 0 ){
					break;
				}
				buf1++;
				cnt++;
			}
			if( cnt <= 0xf ){
				addWBuf = 0;
			}else
				if( cnt <= 0xfff ){
					addWBuf = 1;
				}else{
					addWBuf = 2;
				}
			if( wBuf1+addWBuf >= ewBuf ){
				flag = 0;
				break;
			}
			if( addWBuf == 0 ){
				idx |= (cnt & 0x0f);
				*wBuf1++ = idx;
				continue;
			}else
				if( addWBuf == 1 ){
					idx |= (BIT_REP_LARG | ((cnt >> 8) & 0xf));
					*wBuf1++ = idx;
					*wBuf1++ = (unsigned char)(cnt & 0xff);
					continue;
				}else{
					idx |= (BIT_REP_LARG2 | ((cnt >> 16) & 0xf));
					*wBuf1++ = idx;
					*wBuf1++ = (unsigned char)((cnt >> 8) & 0xff);
					*wBuf1++ = (unsigned char)(cnt & 0xff);
					continue;
				}
		}
		if( *buf1 == *(buf1+1) && *buf1 == *(buf1+2) ){
			repData = *buf1;
			idx = BIT_CMP;
			cnt = 3;
			buf1 += 3;
			while( buf1 < eBuf && cnt < 0xfffff ){
				if( *buf1 != repData ){
					break;
				}
				buf1++;
				cnt++;
			}
			if( cnt <= 0xf ){
				addWBuf = 1;
			}else
				if( cnt <= 0xfff ){
					addWBuf = 2;
				}else{
					addWBuf = 3;
				}
				if( wBuf1+addWBuf >= ewBuf ){
					flag = 0;
					break;
				}
			if( addWBuf == 1 ){
				idx |= (cnt & 0x0f);
				*wBuf1++ = idx;
				*wBuf1++ = repData;
				continue;
			}else
				if( addWBuf == 2 ){
					idx |= (BIT_REP_LARG | ((cnt >> 8) & 0xf));
					*wBuf1++ = idx;
					*wBuf1++ = repData;
					*wBuf1++ = (unsigned char)(cnt & 0xff);
					continue;
				}else{
					idx |= (BIT_REP_LARG2 | ((cnt >> 16) & 0xf));
					*wBuf1++ = idx;
					*wBuf1++ = repData;
					*wBuf1++ = (unsigned char)((cnt >> 8) & 0xff);
					*wBuf1++ = (unsigned char)(cnt & 0xff);
					continue;
				}
		}
		idx = 0;
		cnt2 = 0;
		buf2 = buf1;
		wBuf2 = wBuf1;
		while( 1 ){
			if( buf2 >= eBuf || cnt2 >= 0xfff ){
				mode = 0;
				break;
			}
			if( wBuf2 >= ewBuf ){
				mode = 1;
				break;
			}
			if( buf2+2 < eBuf ){
				if( *buf2 == 0 && *(buf2+1) == 0 ){
					mode = 0;
					break;
				}
				if( *buf2 != 0 && *buf2 == *(buf2+1) && *(buf2+1) == *(buf2+2) ){
					mode = 0;
					break;
				}

			}
			buf2++;
			wBuf2++;
			cnt2++;
		}
		if( cnt2 <= 0xf ){
			addWBuf = 0;
		}else
			if( cnt2 <= 0xfff ){
				addWBuf = 1;
			}else{
				addWBuf = 2;
			}

		if( mode == 1 || wBuf2+addWBuf >= ewBuf ){
			flag = 0;
			break;
		}
		if( addWBuf == 0 ){
			idx = (cnt2 & 0xf);
			*wBuf1++ = idx;
		}else
			if( addWBuf == 1 ){
				idx = BIT_REP_LARG | ((cnt2 >> 8) & 0xf);
				*wBuf1++ = idx;
				*wBuf1++ = (unsigned char)(cnt2 & 0xff);
			}else{
				idx = BIT_REP_LARG2 | ((cnt2 >> 16) & 0xf);
				*wBuf1++ = idx;
				*wBuf1++ = (unsigned char)((cnt2 >> 8) & 0xff);
				*wBuf1++ = (unsigned char)(cnt2 & 0xff);
			}
		for( cnt = 0; cnt < cnt2; cnt++ ){
			*wBuf1++ = *buf1++;
		}
	}


	header = (RD_HEADER *)wBuf;
	header->id[0] = 'R';
	header->id[1] = 'D';
	header->width = width;
	header->height = height;

	if( flag == 1 )
	{
		header->compressFlag = 1;
		header->size = wBuf1 - wBuf;
		l = header->size;
	}
	else
	// ?üÎ??????
	{
		header->compressFlag = 0;
//		header->size = (int)wBuf + width * height + sizeof( RD_HEADER );
		wBuf1 = wBuf + sizeof( RD_HEADER );
		buf1 = buf;
		el = width * height;
		for( l = 0; l < el; l++ )
			*wBuf1++ = *buf1++;
		l += sizeof( RD_HEADER );
	}

	*len = l;
	return wBuf;
}

unsigned char *decoder( unsigned char *buf, unsigned char **disBuf,
	unsigned int *width1, unsigned int *height1, unsigned int *len )
{
	RD_HEADER *header;
	unsigned char *wBuf, *ewBuf, *eBuf;
	unsigned char *wBuf1, *buf1;
	unsigned int width, height;
	unsigned int cnt;
	unsigned int l, el;
	unsigned char repData;
	unsigned char idx;
	if( *disBuf == NULL )
		return NULL;
	wBuf = *disBuf;
	header = (RD_HEADER *)buf;
	if( header->id[0] != 'R' || header->id[1] != 'D' ){
		return NULL;
	}

	width  = header->width;
	height = header->height;

	if( header->compressFlag == 0 ){
		wBuf1 = wBuf;
		buf1 = buf+sizeof( RD_HEADER );
		el = width * height;
		for( l = 0; l < el; l++ )
			*wBuf1++ = *buf1++;

		*len = l;
		*width1 = width;
		*height1 = height;
		return wBuf;
	}
	eBuf = buf + header->size;
	ewBuf = wBuf + width * height;
	wBuf1 = wBuf;
	buf1 = buf+sizeof( RD_HEADER );
	while( buf1 < eBuf ){
		idx = *buf1++;
		if( (idx & BIT_CMP) != 0 ){
			if( (idx & BIT_ZERO) != 0 ){
				repData = 0;
			}else{
				repData = *buf1++;
			}
			if( (idx & BIT_REP_LARG2) != 0 ){
				cnt = ((idx & 0x0f)<<16);
				cnt |= ((*buf1) << 8);
				buf1++;
				cnt |= *buf1++;
			}else
				if( (idx & BIT_REP_LARG) != 0 ){
					cnt = ((idx & 0x0f)<<8);
					cnt |= *buf1++;
				}else{
					cnt = (idx & 0x0f);
				}
			memset( wBuf1, repData, cnt );
			wBuf1 += cnt;
		}else{
			if( (idx & BIT_REP_LARG) != 0 )	{
				cnt = ((idx & 0x0f)<<8);
				cnt |= *buf1++;
			}else{
				cnt = (idx & 0x0f);
			}
			if( cnt >= 0xfffff ){
				return NULL;
			}
			for( l = 0; l < cnt; l++ )
				*wBuf1++ = *buf1++;
		}
	}

	*len = width * height;
	*width1  = width;
	*height1 = height;
	return wBuf;
}
