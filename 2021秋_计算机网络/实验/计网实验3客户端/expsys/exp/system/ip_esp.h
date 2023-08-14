/*
 *		ip_esp.h - ESP header.
 */
/*
 * 		Copyright (c) 2002 by Tsinghua Univ., Beijing, P.R.C.
 */
/*
	Modification History
	--------------------
	15Apr2002, by Yinghua Wu, create
*/
#ifndef _IP_ESP_H
#define _IP_ESP_H

#include "ip_ah.h"
#include "ip_ipsec.h"

typedef struct ipsec_esp
{
	unsigned int  spi;            /* Security parameter index */
	unsigned int  seq;            /* Sequence number field    */
}IPSEC_ESP;

typedef struct espTail
{
	unsigned char  padLen;          /* pad length  */
	unsigned char  nxtHeader;       /* next header */
}ESP_TAIL;

typedef struct espAlgorithm
{
	unsigned int ivLen;                 /* the actual length of IV, in bytes  */
	const char *name;               /* name of algorithm */

	/* encrypt function, if using IV, add to encryText */
	int (* encrypt)(unsigned char *plainText, unsigned int plainLen, unsigned char *encryText, unsigned int *encryLen, char *pKeys, char *value);
	/* decrypt function, if using IV, remove from encryText */
	int (* decrypt)(unsigned char *pEncryText, unsigned int encryLen, unsigned char *pPlainText, unsigned int *pPlainLen, char *pKeys, char *value);
	/* padding function, making the length of plainText equal the one of encryText */
	int (* padding)(unsigned char *pOriginalText, unsigned int originalLen, unsigned char **pPaddingText, unsigned int *paddedLen, unsigned char nxtHdr);
}ESP_ALGORITHM;

/* internal functions */


extern unsigned int ipsecESPPadding(unsigned char *pOriginalText, unsigned short originalLen, unsigned char **pPaddingText, unsigned int *paddedLen, unsigned char nxtHdr);





#endif /* _IP_ESP_H */
