/*
* THIS FILE IS FOR TCP TEST
*/

/*
struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};
*/

#include "sysInclude.h"

extern void tcp_DiscardPkt(char *pBuffer, int type);

extern void tcp_sendReport(int type);

extern void tcp_sendIpPkt(unsigned char *pData, UINT16 len, unsigned int  srcAddr, unsigned int dstAddr, UINT8	ttl);

extern int waitIpPacket(char *pBuffer, int timeout);

extern unsigned int getIpv4Address();

extern unsigned int getServerIpv4Address();

int stud_tcp_input(char *pBuffer, unsigned short len, unsigned int srcAddr, unsigned int dstAddr)
{
	return 0;
}

void stud_tcp_output(char *pData, unsigned short len, unsigned char flag, unsigned short srcPort, unsigned short dstPort, unsigned int srcAddr, unsigned int dstAddr)
{

}

int stud_tcp_socket(int domain, int type, int protocol)
{
	return 2;
}

int stud_tcp_connect(int sockfd, struct sockaddr_in *addr, int addrlen)
{
	return 0;
}

int stud_tcp_send(int sockfd, const unsigned char *pData, unsigned short datalen, int flags)
{
	return 0;
}

int stud_tcp_recv(int sockfd, unsigned char *pData, unsigned short datalen, int flags)
{
	return 0;
}

int stud_tcp_close(int sockfd)
{
	return 0;
}
