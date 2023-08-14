/* GLOBAL.H - RSAREF types and constants
 */
#ifndef _IP_MD5_H
#define _IP_MD5_H

/* MD5 context. */
typedef struct
{
  unsigned int state[4];                                  
  unsigned int count[2];       
  unsigned char buffer[64];                       
} IKE_MD5_CTX;

#endif
