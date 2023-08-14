// define the data type
#include "common.h"
#include "ip_ah.h"
#include "ip_esp.h"

#define PACKET_TYPE_DATA 0
#define PACKET_TYPE_SYN   0x02
#define PACKET_TYPE_SYN_ACK 0x12
#define PACKET_TYPE_ACK  0x10
#define PACKET_TYPE_FIN  0x01
#define PACKET_TYPE_FIN_ACK 0x11

// for ip test
extern int stud_ip_recv(char * pBuffer,unsigned short length);

extern int stud_ip_Upsend(char* pBuffer,unsigned short len,unsigned int srcAddr,unsigned int dstAddr,byte protocol,byte ttl);

// for ip forward test
extern void stud_Route_Init();

extern int stud_fwd_deal(char * pBuffer, int length);

extern void stud_route_add(stud_route_msg *proute);

// for ipv6 test
extern int stud_ipv6_recv(char * pBuffer, unsigned short length);

extern int stud_ipv6_Upsend(char* pData, unsigned short len, 
							ipv6_addr* srcAddr, ipv6_addr* dstAddr, 
							char hoplimit,char nexthead);

extern void stud_ipv6_Route_Init();

extern void stud_ipv6_route_add(stud_ipv6_route_msg *proute);

extern int stud_ipv6_fwd_deal(char * pBuffer, int length);

// for tcp test
extern int stud_tcp_input(char *pBuff, uint16 len, uint32 srcAddr, uint32 destAddr);

extern void stud_tcp_output(char *pData, uint16 len, unsigned char flag, uint16 sourcePort, uint16 destPort, uint32 srcAddr, uint32 destAddr);

extern int stud_tcp_socket(int  domain,	int  type, int  protocol);

extern int stud_tcp_connect(int sockfd, sockaddr_in*	addr, int addrlen);

extern int stud_tcp_send(int sockfd, const unsigned char* pData, uint16 dataLen, int	flags);

extern int stud_tcp_recv(int sockfd, unsigned char* pData, uint16 dataLen, int	flags);

extern int stud_tcp_close( int sockfd );

extern int stud_slide_window_stop_and_wait(char *pBuffer, int bufferSize, UINT8 messageType);

extern int stud_slide_window_back_n_frame(char *pBuffer, int bufferSize, UINT8 messageType);

extern int stud_slide_window_choice_frame_resend(char *pBuffer, int bufferSize, UINT8 messageType);

extern int stud_rip_packet_recv(char *pBuffer,int bufferSize,UINT8 iNo,UINT32 srcAdd);

extern void stud_rip_route_timeout(UINT32 destAdd, UINT32 mask, unsigned char msgType);

extern BYTE stud_bgp_FsmEventOpen(BgpPeer *pPeer,BYTE *pBuf,unsigned int);      

extern BYTE stud_bgp_FsmEventKeepAlive(BgpPeer *pPeer,BYTE *pBuf,unsigned int);   

extern BYTE stud_bgp_FsmEventNotification(BgpPeer *pPeer,BYTE *pBuf,unsigned int);

extern BYTE stud_bgp_FsmEventUpdate(BgpPeer *pPeer,BYTE *pBuf,unsigned int);

extern BYTE stud_bgp_FsmEventTcpException(BgpPeer *pPeer,BYTE msgType);

extern BYTE stud_bgp_FsmEventTimerProcess(BgpPeer *pPeer,BYTE msgType);

extern BYTE stud_bgp_FsmEventStart(BgpPeer *pPeer);

extern BYTE stud_bgp_FsmEventStop(BgpPeer *pPeer);

extern BYTE stud_bgp_FsmEventConnect(BgpPeer *pPeer);

extern int stud_ipsec_input(char * pBuffer,unsigned int len);

extern int stud_ipsec_output(char * pBuffer,unsigned int len);

extern int stud_MN_icmp_send();
extern int stud_MN_icmp_recv(char *buffer,unsigned short len);
extern int stud_MN_send_Regi_req(char*pdata,unsigned char len,unsigned short ttl,unsigned int HA_addr,unsigned int FA_addr,unsigned int MN_Haddr,unsigned int iden_low,unsigned int iden_high);
extern int stud_MN_recv_Regi_rep(char *pbuffer,unsigned short len,unsigned int if_no);
extern int stud_HA_recv_Regi_req(char *pbuffer,unsigned short len,unsigned int mask,unsigned int if_no);
extern int stud_packertrans_HA_recv(char *pbuffer,unsigned short len);
extern int stud_packertrans_FA_recv(char *pbuffer,unsigned short len);
extern int stud_FA_recv_Regi_req(char *pbuffer,unsigned short len,unsigned int mask,unsigned int if_no);
extern int stud_FA_recv_Regi_rep(char *pbuffer,unsigned short len);





