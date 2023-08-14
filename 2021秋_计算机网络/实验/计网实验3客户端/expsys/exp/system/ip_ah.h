/*
 *		ip_ah.h - AH header.
 */
/*
 * 		Copyright (c) 2002 by Tsinghua Univ., Beijing, P.R.C.
 */
/*
	Modification History
	--------------------
	15Apr2002, by Yinghua Wu, create
*/

#ifndef _IP_AH_H
#define _IP_AH_H

#include "ike_crypto.h"
#include "ip_ipsec.h"


typedef struct ahAlgorithm 
{
	unsigned int  digestSize;                              /* in bits          */
	unsigned int  dataLen;                                 /* the authentication len used actually, in bits */	
	const char *name;                                /* name of algorithm*/

	void (*hashInit)(IKE_MD5_CTX *ctx_hash);	             /* initialize context */
    void (*hashUpdate)(IKE_MD5_CTX *ctx_hash, const unsigned char *input, unsigned int len);   /* add input to hash */
    void (*hashFinal)(unsigned char *output, IKE_MD5_CTX *ctx_hash);   /* finalize hash */
}AH_ALGORITHM;

typedef struct ipsec_ah 
{
	BYTE  ah_nxt;                 /* Next Header */
	BYTE  ah_len;                 /* Length of auth data field +1, in 32 bits */
	unsigned short ah_reserved;            /* Reserved for future use */
	DWORD ah_spi;                 /* Security parameter index */
	DWORD ah_seq;                 /* Sequence number field */
	/* Authentication Data */
}IPSEC_AH;

/* external function definition for both IPv4 IPSec and IPv6 IPSec */
extern AH_ALGORITHM *ahAlgorithmLookup(BYTE algAuth);
extern unsigned int ahICVCheckLength(AH_ALGORITHM *pAlg,  IPSEC_AH *pAHHdr);
extern void ahGetLenFromAlgorithm(AH_ALGORITHM *pAlg, unsigned int *icvLen, unsigned int *ahLen);




#endif  /* _IP_AH_H */
