#ifndef	ENCRYPTCLIENT_H
#define ENCRYPTCLIENT_H

#include <stdlib.h>
//#include "Profile.h"
//#ifdef __BORLANDC__
//#define NANOBEGIN     __emit__ (0xEB,0x03,0xD6,0xD7,0x01)
//#define NANOEND       __emit__ (0xEB,0x03,0xD6,0xD7,0x00)
//#else
//#define NANOBEGIN     __asm _emit 0xEB __asm _emit 0x03 __asm _emit 0xD6 __asm _emit 0xD7 __asm _emit 0x01
//#define NANOEND       __asm _emit 0xEB __asm _emit 0x03 __asm _emit 0xD6 __asm _emit 0xD7 __asm _emit 0x00
//#endif

template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
					unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
class	CEncryptClient
{
public:
	CEncryptClient(){ Init(); }
public:
	void Init() { m_nPos1 = m_nPos2 = 0; }
	void Encrypt(unsigned char * bufMsg, int nLen, bool bMove = true);
	void ChangeCode(unsigned long dwData);
protected:
	int		m_nPos1;
	int		m_nPos2;
protected:
	class	CEncryptCode
	{
	public:
		CEncryptCode()
		{
		//* 与下面ELSE算法相同，只为加强复杂性：)
				unsigned char	nCode = fst1;
				int i;
				for(i = 0; i < 256; i++)
				{
					m_bufEncrypt1[i] = nCode;
					int	nTemp = (a1*nCode) % 256;
					nCode = (c1 + nTemp*nCode + b1*nCode) % 256;
				}
				//@@@	assert(nCode == fst1);

					nCode = fst2;
				for( i = 0; i < 256; i++)
				{
					m_bufEncrypt2[i] = nCode;
					int	nTemp = a2*nCode;
					nCode = ((b2 + nTemp)*nCode + c2) & 0xFF;
				}
				//@@@	assert(nCode == fst2);
		}
		unsigned char m_bufEncrypt1[256];
		unsigned char m_bufEncrypt2[256];
	}m_cGlobalEncrypt;		//??? 应改成静态共享的成员对象，以节约资源
};

template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
					unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
inline void CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::Encrypt(unsigned char * bufMsg, int nLen, bool bMove /*= true*/)
{
		int		nOldPos1 = m_nPos1;
		int		nOldPos2 = m_nPos2;
		for(int i = 0; i < nLen; i++)
		{
			bufMsg[i] ^= m_cGlobalEncrypt.m_bufEncrypt1[m_nPos1];
			bufMsg[i] ^= m_cGlobalEncrypt.m_bufEncrypt2[m_nPos2];
			if(++m_nPos1 >= 256)
			{
				m_nPos1 = 0;
				if(++m_nPos2 >= 256)
					m_nPos2 = 0;
			}
			//@@@	assert(m_nPos1 >=0 && m_nPos1 < 256);
			//@@@	assert(m_nPos2 >=0 && m_nPos2 < 256);
		}

		if(!bMove)
		{
			// 恢复指针
			m_nPos1 = nOldPos1;
			m_nPos2 = nOldPos2;
		}
	   /*@@@	LOGCATCH("Encrypt fail."); exit(3);*/ 
}

template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
					unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
inline void CEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::ChangeCode(unsigned long dwData)
{
	
		unsigned int dwData2 = dwData*dwData;
		for(int i = 0; i < 256; i += 4)
		{
			*(unsigned int*)(&m_cGlobalEncrypt.m_bufEncrypt1[i]) ^= dwData;
			*(unsigned int*)(&m_cGlobalEncrypt.m_bufEncrypt2[i]) ^= dwData2;
		}
	 /*LOGCATCH("ChangeCode fail.");*/ 
}



	

#endif