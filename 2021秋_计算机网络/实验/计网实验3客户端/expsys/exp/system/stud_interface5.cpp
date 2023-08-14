#include "sysinclude.h"

extern void rip_sendIpPkt(unsigned char *pData, UINT16 len,unsigned short dstPort,UINT8 iNo);

extern struct stud_rip_route_node *g_rip_route_table;

int stud_rip_packet_recv(char *pBuffer,int bufferSize,UINT8 iNo,UINT32 srcAdd)
{	
	return 0;
}

void stud_rip_route_timeout(UINT32 destAdd, UINT32 mask, unsigned char msgType)
{
		
}
