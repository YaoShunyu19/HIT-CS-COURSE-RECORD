/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain
*/
#ifndef _SHA1_H
#define  _SHA1_H
#pragma once

 extern void SHA1Transform(unsigned int state[5], const unsigned char buffer[64]);
 extern void SHA1Init(SHA1_CTX* context);
 extern void SHA1Update(SHA1_CTX* context, const unsigned char* data, unsigned int len);
 extern void SHA1Final(unsigned char digest[20], SHA1_CTX* context);
 
#endif
