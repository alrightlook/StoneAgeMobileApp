/*
 * DES encryption/decryption library interface
 * 
 *	Written by Koichiro Mori (kmori@lsi-j.co.jp)
 */

#define DES_DIRMASK	001
#define DES_ENCRYPT	(0 * DES_DIRMASK)
#define DES_DECRYPT	(1 * DES_DIRMASK)

#define DES_DEVMASK	002
#define DES_HW		(0 * DES_DEVMASK)
#define DES_SW		(1 * DES_DEVMASK)

#define DESERR_NONE	0
#define DESERR_NOHWDEVICE 1
#define DESERR_HWERROR	2
#define DESERR_BADPARAM	3

#define	DES_FAILED(err)	((err) > DESERR_NOHWDEVICE)

//#if __STDC__ || LSI_C

int ecb_crypt(char *key, char *buf, unsigned len, unsigned mode);
int cbc_crypt(char *key, char *buf, unsigned len, unsigned mode, char *ivec);
void des_setparity(char *key);

/* Not defined by Sun Microsystems - internally used by desbench program */
void des_setkey(char *key);
void des_crypt(char *buf, int dflag);
/*
#else

 int ecb_crypt();
 int cbc_crypt();
 void des_setparity();

// Not defined by Sun Microsystems - internally used by desbench program 
 void des_setkey();
 void des_crypt();

#endif // __STDC__ || LSI_C 
*/