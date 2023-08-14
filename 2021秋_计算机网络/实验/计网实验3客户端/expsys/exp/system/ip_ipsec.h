
#ifndef _IP_IPSEC_H
#define _IP_IPSEC_H

#include "common.h"




#define IPSEC_TRUE 1
#define IPSEC_FALSE 0

#define IPSEC_MODE_TUNNEL 1
#define IPSEC_MODE_TRANSPORT 2



#define IPSEC_AUTH_MD5 1
#define IPSEC_AUTH_SHA1 2
#define IPSEC_AUTH_MAX 2

#define IPSEC_ENCRY_DES 1
#define IPSEC_ENCRY_3DES 2
#define IPSEC_ENCRY_TUC  4
#define IPSEC_ENCRY_MAX 2

#define IPSEC_PROTO_AH 1
#define IPSEC_PROTO_ESP 2

#define IPSEC_IPSECAPPLY 0
#define IPSEC_PACKETDROP 1

/* error codes */
#define IPSEC_EINVAL 1
#define IPSEC_EALG 2
#define IPSEC_EMODE 3
#define IPSEC_ESEQOVERF 4
#define IPSEC_EFRAGINTRANS 5
#define IPSEC_EICV 6
#define IPSEC_EREPLAY 7
#define IPSEC_EENCRYPT 8
#define IPSEC_EDECRYPT 9
#define IPSEC_ESAEXPIRE 10

/* Some Assigned Protocol Numbers */
#define	IPT_ICMP	1	/* protocol type for ICMP packets	*/
#define	IPT_TCP		6	/* protocol type for TCP packets	*/
#define	IPT_EGP		8	/* protocol type for EGP packets	*/
#define	IPT_UDP		17	/* protocol type for UDP packets	*/
#define IPT_IPIP6	41	/* protocol type for Tunnel packets	*/
#define IPT_ESP 	50	/* protocol type for IPSec ESP packets */
#define IPT_AUTH 	51	/* protocol type for IPSec AH packets */

typedef struct IPhdr 
{
	BYTE	verlen;
	BYTE	tos;
	unsigned short	tot_len;
	unsigned short	id;
	unsigned short	frag_off;
	BYTE	ttl;
	BYTE	protocol;
	unsigned short	check;
	DWORD	saddr;
	DWORD	daddr;
	/*The options start here. */
}IPSEC_HEAD;


typedef struct ipsecSaKey 
{
	BYTE  keyNum;		/* the number of keys */
	unsigned short  keyBits;		/* bits of one key */
	BYTE  key[1];			/* keybits/8 */
}IPSEC_SA_KEY;


/* ip security association */
typedef struct ipsecAssoc 
{	
	struct ipsecAssoc  *next; 

	DWORD  srcAddr;                   /* source address for SA */
	DWORD  dstAddr;                   /* destination address for SA */
	BYTE  proto;                             /* ESP or AH */
	BYTE  mode;                              /* TUNNEL or TRANSPORT */	
	
	DWORD  spi;                              /* SPI */
	
	BYTE  algAuth;                          /* authentication algorithm */
	BYTE  algEncry;                         /* encryption algorithm */
	struct ipsecSaKey  *keyAuth;                   /* key for authentication */
	struct ipsecSaKey  *keyEncry;                  /* key for encryption */
	struct ipsecSaKey *keyEncry2;                /*key for encryption 2*/
	DWORD  ivLen;                           /* the length of IV, 4n */
	BYTE  iv[20];                           /* the initial vector */
	
}IPSEC_ASSOC;

#define IPSEC_ASSOC_SAD IPSEC_ASSOC
/* ipsec policy selector */
typedef struct ipsecPolicySelector
{	
	struct ipsecPolicySelector  *next;		/* selector chain */
	DWORD  srcAddr;		/* source address */
	DWORD  dstAddr;		/* destination address */
	DWORD   srcPref;		 /* prefix of src addr */
	DWORD  dstPref;		 /* prefix of dst addr */	
	unsigned short  upperProto;      /* upper protocol */	

	IPSEC_ASSOC *sa;
}IPSEC_POLICY_SELECTOR;


/* SPD header */
typedef struct ipsecSpdHeader 
{
	struct ipsecPolicySelector  *spSelector;    

}IPSEC_SPD_HEADER;


/* functions for both IPv4 IPSec and IPv6 IPSec */
extern void ipsecInitVector(char *iv, int len);





#endif /* _IP_IPSEC_H */
