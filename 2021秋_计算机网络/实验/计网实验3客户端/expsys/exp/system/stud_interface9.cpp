#include "sysinclude.h"

extern unsigned int getMNHaddress();
extern unsigned int getHAHaddress();
extern unsigned int getFAHaddress();
extern void submitCareofadd(unsigned int careofadd);
extern bool isMNaddressRegister(unsigned int address);
extern int icmp_sendIpPkt(unsigned char* pData, unsigned short len);
extern int MN_regi_sendIpPkt(unsigned char* pData, unsigned short len);
extern int FA_sendregi_rep(char* pData,unsigned short len,unsigned int destadd,unsigned int srcadd);
extern int FA_sendregi_req(char* pData,unsigned short len,unsigned int destadd,unsigned int srcadd);
extern int HA_regi_sendIpPkt(unsigned char type,unsigned char code,unsigned int destadd,unsigned int srcadd,unsigned int Mn_addr,unsigned int Ha_addr);
extern int HA_send_Encap_packet(char* pbuffer,unsigned short len,unsigned int dstAddr,unsigned int srcAddr);
extern int FA_send_ipv4_toMN(char *pBuffer, int length);
extern int FA_forward_ipv4packet(char *pBuffer, int length);
extern int HA_forward_ipv4packet(char *pBuffer, int length);
extern void HA_DiscardPkt(char * pBuffer, int length);
extern void FA_DiscardPkt(char * pBuffer, int length);

extern struct  stud_MN_route_node *g_MN_route_table;
extern struct  stud_FA_route_node *g_FA_route_table;
extern struct  stud_HA_route_node *g_HA_route_table;


int stud_MN_icmp_send()
{
	return 0;
}

int stud_MN_icmp_recv(char *buffer,unsigned short  len)
{
	return 0;
}

 int stud_MN_send_Regi_req(char*pdata,unsigned char len,unsigned short ttl,unsigned int HA_addr,unsigned int FA_addr,unsigned int MN_Haddr,unsigned int iden_low,unsigned int iden_high)
{
	return 0;
}

int stud_MN_recv_Regi_rep(char *pbuffer,unsigned short len,unsigned int if_no)
{
	return 0;
}

int stud_FA_recv_Regi_req(char *pbuffer,unsigned short len, unsigned int mask,unsigned int if_no)
{
	return 0;
}

int stud_FA_recv_Regi_rep(char *pbuffer,unsigned short len)
{
	return 0;
}

int stud_HA_recv_Regi_req(char *pbuffer,unsigned short len,unsigned int mask,unsigned int if_no)
{
	return 0;
}

int stud_packertrans_HA_recv(char *pbuffer,unsigned short len)
{
	return 0;
}

int stud_packertrans_FA_recv(char *pbuffer,unsigned short len)
{
	return 0;
}

