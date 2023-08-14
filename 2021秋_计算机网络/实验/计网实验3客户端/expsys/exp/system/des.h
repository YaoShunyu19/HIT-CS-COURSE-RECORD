/* des.h -
 *
 *	Headers and defines for d3des.c
 *	Graven Imagery, 1992.
 *
 * Copyright (c) 1988,1989,1990,1991,1992 by Richard Outerbridge
 *	(GEnie : OUTER; CIS : [71755,204])
 */
#ifndef HEADER_DES_H
#define HEADER_DES_H



/* If this is set to 'unsigned int' on a DEC Alpha, this gives about a
 * %20 speed up (longs are 8 bytes, int's are 4). */
#ifndef DES_LONG
/* RCSID $Id: des.h,v 1.1.1.1 2003/12/01 19:42:18 wq Exp $ */
/* This conditional for FreeS/WAN project. */
#define DES_LONG unsigned long
#endif

typedef unsigned char des_cblock[8];
typedef struct des_ks_struct
	{
	union	{
		des_cblock _;
		/* make sure things are correct size on machines with
		 * 8 byte longs */
		DES_LONG pad[2];
		} ks;
#undef _
#define _	ks._
	} des_key_schedule[16];

#define DES_KEY_SZ 	(sizeof(des_cblock))
#define DES_SCHEDULE_SZ (sizeof(des_key_schedule))

#define DES_ENCRYPT	1
#define DES_DECRYPT	0

#define DES_CBC_MODE	0
#define DES_PCBC_MODE	1

#define des_ede2_cbc_encrypt(i,o,l,k1,k2,iv,e) \
	ipsec_des_ede3_cbc_encrypt((i),(o),(l),(k1),(k2),(k1),(iv),(e))

#define C_Block des_cblock
#define Key_schedule des_key_schedule
#ifdef KERBEROS
#define ENCRYPT DES_ENCRYPT
#define DECRYPT DES_DECRYPT
#endif
#define KEY_SZ DES_KEY_SZ
#define set_key ipsec_des_set_key
#define key_sched ipsec_des_key_sched

#define ncbc_encrypt ipsec_des_ncbc_encrypt

/* For compatibility with the MIT lib - eay 20/05/92 */
typedef des_key_schedule bit_64;
#define des_fixup_key_parity des_set_odd_parity
#define des_check_key_parity check_parity

extern int des_check_key;	/* defaults to false */
extern int des_rw_mode;		/* defaults to DES_PCBC_MODE */

/* The next line is used to disable full ANSI prototypes, if your
 * compiler has problems with the prototypes, make sure this line always
 * evaluates to true :-) */

#if defined(MSDOS) || defined(__STDC__)
#undef NOPROTO
#endif

void des_ncbc_encrypt(des_cblock *input,des_cblock *output,long length,
	des_key_schedule schedule,des_cblock *ivec,int enc);


void ipsec_des_encrypt(DES_LONG *data,des_key_schedule ks, int enc);
void des_encrypt2(DES_LONG *data,des_key_schedule ks, int enc);
void des_encrypt3(DES_LONG *data, des_key_schedule ks1,
	des_key_schedule ks2, des_key_schedule ks3);
void des_decrypt3(DES_LONG *data, des_key_schedule ks1,
	des_key_schedule ks2, des_key_schedule ks3);
void ipsec_des_ede3_cbc_encrypt(des_cblock *input, des_cblock *output, 
	long length, des_key_schedule ks1, des_key_schedule ks2, 
	des_key_schedule ks3, des_cblock *ivec, int enc);


void des_set_odd_parity(des_cblock *key);
int des_is_weak_key(des_cblock *key);
int ipsec_des_set_key(des_cblock *key,des_key_schedule schedule);
int ipsec_des_key_sched(des_cblock *key,des_key_schedule schedule);

#endif

