/*
* THIS FILE IS FOR IP FORWARD TEST
*/
#include "sysInclude.h"

// system support
extern void fwd_LocalRcv(char *pBuffer, int length);

extern void fwd_SendtoLower(char *pBuffer, int length, unsigned int nexthop);

extern void fwd_DiscardPkt(char *pBuffer, int type);

extern unsigned int getIpv4Address( );

// implemented by students

void stud_Route_Init()
{
	return;
}

void stud_route_add(stud_route_msg *proute)
{
	return;
}


int stud_fwd_deal(char *pBuffer, int length)
{
	return 0;
}

