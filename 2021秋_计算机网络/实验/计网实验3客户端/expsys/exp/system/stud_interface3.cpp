/*
* THIS FILE IS FOR IPv6 TEST
*/
// system support
#include "sysinclude.h"

extern void ipv6_DiscardPkt(char* pBuffer,int type);

extern void ipv6_SendtoLower(char*pBuffer,int length);

extern void ipv6_SendtoUp(char *pBuffer,int length);

extern void getIpv6Address(ipv6_addr *paddr);

// implemented by students

int stud_ipv6_recv(char *pBuffer, unsigned short length)
{
	return 0;
}

int stud_ipv6_Upsend(char *pData, unsigned short len, 
					 ipv6_addr *srcAddr, ipv6_addr *dstAddr, 
					 char hoplimit, char nexthead)
{
	return 0;
}
