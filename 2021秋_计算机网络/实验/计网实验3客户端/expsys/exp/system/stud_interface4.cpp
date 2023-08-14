/*
* THIS FILE IS FOR IPv6 FORWARD TEST
*/
// system support

#include "sysinclude.h"

extern void ipv6_fwd_DiscardPkt(char *pBuffer, int type);
extern void ipv6_fwd_SendtoLower(char *pBuffer, int length, ipv6_addr *nexthop);
extern void getIpv6Address(ipv6_addr *pAddr);
extern void ipv6_fwd_LocalRcv(char *pBuffer, int length);

void stud_ipv6_Route_Init()
{
	return;
}

void stud_ipv6_route_add(stud_ipv6_route_msg *proute)
{
	return;
}

int stud_ipv6_fwd_deal(char *pBuffer, int length)
{
	return 0;
}
