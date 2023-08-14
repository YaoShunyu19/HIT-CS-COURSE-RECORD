#ifndef _COMMON_H_
#define _COMMON_H

#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <io.h>
#include <stdexcept>

using namespace std;


#define MAX_PARA_NUM  30

#define TEST_FINISH_SUCCESS 0
#define TEST_FINISH_FAIL 1
#define TEST_CREATE_SOCKET_FAIL 2

// const variable define

#define DEFINE_BASE 0
#define REQ_SCRIPT (DEFINE_BASE++)
#define REP_ACK (DEFINE_BASE++)
#define REP_ERROR (DEFINE_BASE++)
#define REP_CORRECT (DEFINE_BASE++)

#define ETHER_HEAD_LEN 14
#define IP_HEAD_LEN 20
#define TCP_HEAD_LEN 20

// define the data type
#define byte    char
#define UINT8   char
#define uint8   char
#define UINT16  unsigned short
#define uint16  unsigned short
#define UINT32  unsigned int
#define uint32  unsigned int
#define UINT64  unsigned long
#define uint64  unsigned long


#define MSG_TYPE_BASE 0
#define MSG_TYPE_CONTROL 1
#define MSG_TYPE_DATA 2
#define MSG_TYPE_TIMEOUT 3
#define MSG_TYPE_RESEND 4

#define MSG_SUBTYPE_TYPE_BASE 0
#define MSG_SUBTYPE_REQ 4
#define MSG_SUBTYPE_REP 5
#define MSG_SUBTYPE_ROUTEADD 6
#define MSG_SUBTYPE_RESULT 7
#define MSG_SUBTYPE_LIST 8
#define MSG_SUBTYPE_GET_SCRIPT 9
#define MSG_SUBTYPE_IP_MAC  10
#define MSG_SUBTYPE_FINISH_TEST 11
#define MSG_SUBTYPE_ROUTEADDIPV6 12
#define MSG_SUBTYPE_RIPROUTESEND 13
#define MSG_SUBTYPE_RIPTIMEOUT 14
#define MSG_SUBTYPE_RIPROUTEINIT 15
#define MSG_SUBTYPE_RIPROUTEPRINT 16
#define MSG_SUBTYPE_BGPSTATUE  17
#define MSG_SUBTYPE_BGPTIMEOUT 18
#define MSG_SUBTYPE_BGPEXCEPT  19
#define MSG_SUBTYPE_ARPCACHESEND 20 //added by xiaojun
#define MSG_CACHE_TIMEOUT 21 //added by xiaojun
#define MSG_SUBTYPE_ARPCACHEINIT 22 //added by xiaojun
#define MSG_SUBTYPE_IPSECINIT 23 //added by wangyl 20080919
#define MSG_SUBTYPE_IPSECDATA 24
#define MSG_SUBTYPE_MOBILEIP_REP 25
#define MSG_SUBTYPE_MNROUTEINIT 26 //added  by xiaojun for mobile ip protocol
#define MSG_SUBTYPE_MNROUTESEND 27
#define MSG_SUBTYPE_FAROUTEINIT 28
#define MSG_SUBTYPE_FAROUTESEND 29
#define MSG_SUBTYPE_HAROUTEINIT 30
#define MSG_SUBTYPE_HAROUTESEND 31
#define MSG_SUBTYPE_HATRANROUINIT 32
#define MSG_SUBTYPE_FATRANROUINIT 33

#define MSG_SUBTYPE_PACKETRETURN 34 //add by xiangxj for ipv6_fwd

#define DATA_SUBTYPE_RECV 0
#define DATA_SUBTYPE_SEND 1

// define the ip_addr structure
#define ip_addr UINT32

//add for GUI test
#define GUI_TEST_START_NUMBER 10
enum GUI_TEST_CASE_NUMBER{GUI_IP=GUI_TEST_START_NUMBER,GUI_RIP,GUI_IPV6,GUI_TCP};

//add for CODE test
#define CODE_TEST_START_NUMBER 0
#define CODE_TEST_END_NUMBER 9
enum CODE_TEST_CASE_NUMBER{CODE_SLIDE=CODE_TEST_START_NUMBER,CODE_IP,CODE_IP_FW,CODE_IPV6,CODE_IPV6_FW,CODE_RIP,CODE_TCP,CODE_BGP,CODE_IPSEC};

// define the ipv6 structure
typedef union
{
	BYTE 	bAddr[16];
	unsigned short wAddr[8];
	unsigned int   dwAddr[4];
}ipv6_addr;

#define MSG_REQUEST 0
#define MSG_REPLY 1

#define MSG_REPLY_SUCCESS 1


#define PARSER_CLIENT 0
#define PARSER_SERVER 1

#define PACKET_TYPE_BASE 0
#define	PACKET_TYPE_IPV4 1
#define PACKET_TYPE_IPV6 2
#define PACKET_TYPE_TCP 3
#define PACKET_TYPE_UDP 4
#define PACKET_TYPE_SET 5
#define PACKET_TYPE_ADDROUTE 6
#define PACKET_TYPE_IPV6ROUTEADD 7
#define PACKET_TYPE_REQ 8
#define PACKET_TYPE_REP 9
#define PACKET_TYPE_RIPTIMEOUT 10
#define PACKET_TYPE_RIPDELETROUTE 11
#define PACKET_TYPE_RIPROUTEINIT 12
#define PACKET_TYPE_RIPROUTEPRINT 13
#define PACKET_TYPE_RIPROUTESEND 14
#define PACKET_TYPE_FRAME 15
#define PACKET_TYPE_BGPSTATUE 16
#define PACKET_TYPE_BGPTIMEOUT 17
#define PACKET_TYPE_BGPEXCEPT 18
#define PACKET_TYPE_ARP 19//added by xiao jun
#define PACKET_TYPE_ARPCACHESEND 20
#define PACKET_TYPE_ARPCACHETIMEOUT 21
#define PACKET_TYPE_ARPCACHEINIT 22
#define PACKET_TYPE_IPSEC 23 //added by wangyl 20080919
#define PACKET_TYPE_IPSECINIT 24
#define PACKET_TYPE_ICMP 26 //added by xiaojun 20081121
#define PACKET_TYPE_MIREP 27
#define PACKET_TYPE_MNROUTEINIT 28
#define PACKET_TYPE_MNROUTESEND 29
#define PACKET_TYPE_FAROUTEINIT 30
#define PACKET_TYPE_FAROUTESEND 31
#define PACKET_TYPE_HAROUTEINIT 32
#define PACKET_TYPE_HAROUTESEND 33
#define PACKET_TYPE_HATRANROUINIT 34
#define PACKET_TYPE_FATRANROUINIT 35
#define PACKET_TYPE_PACKETRETURN 36 //add by xiangxj for ipv6_fwd

#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17
#define PROTOCOL_ICMP 1
// define the test result
#define STUD_IP_TEST_CORRECT 0
#define STUD_IP_TEST_CHECKSUM_ERROR 1
#define STUD_IP_TEST_TTL_ERROR 2
#define STUD_IP_TEST_VERSION_ERROR 3
#define STUD_IP_TEST_HEADLEN_ERROR 4
#define STUD_IP_TEST_DESTINATION_ERROR 5

#define STUD_FORWARD_TEST_LOCALHOST 6
#define STUD_FORWARD_TEST_TTLERROR 7
#define STUD_FORWARD_TEST_NOROUTE  8

#define STUD_TCP_TEST_SEQNO_ERROR 9
#define STUD_TCP_TEST_SRCPORT_ERROR 10
#define STUD_TCP_TEST_DSTPORT_ERROR 11

#define STUD_IPV6_TEST_CORRECT 12
#define STUD_IPV6_TEST_HOPLIMIT_ERROR 13
#define STUD_IPV6_TEST_VERSION_ERROR 14
#define STUD_IPV6_TEST_DESTINATION_ERROR 15

#define STUD_IPV6_FORWARD_TEST_LOCALHOST 16
#define STUD_IPV6_FORWARD_TEST_HOPLIMIT_ERROR 17
#define STUD_IPV6_FORWARD_TEST_NOROUTE  18

#define STUD_RIP_VERSION_ERROR  19
#define STUD_RIP_COMMAND_ERROR  20
#define STUD_RIP_LENGTH_ERROR   21

#define STUD_ARP_TEST_OP_ERROR 1
#define STUD_ARP_TEST_HARDWAREKIND_ERROR 2

#define DEFAULT_TCP_SRC_PORT 2007
#define DEFAULT_TCP_DST_PORT 2006
#define DEFAULT_BGP_PORT 179
#define MAX_BUFFER_SIZE 4096

/*
// define the error code returned by test server
#define ERROR_INVAIL_DESTADDR 2
#define ERROR_INVAIL_SRCADDR 3
#define ERROR_IP_CHECKSUM 4
#define ERROR_IP_VERSION 5
#define ERROR_IP_HEADLEN 6
#define ERROR_IP_TOTALLEN 7
#define ERROR_IP_TTL 8
#define ERROR_IP_PROTOCOL 9
#define ERROR_IP_NEXTHOP 10
#define ERROR_TCP_CHECKSUM 11
#define ERROR_TCP_SRCPORT 12
#define ERROR_TCP_DESTPORT 13
#define ERROR_TCP_SEQNO 14
#define ERROR_TCP_ACKNO 15
#define ERROR_TCP_FLAG 16
#define ERROR_TCP_DATA 17

#define ERROR_IPV6_DESTADDR 18
#define ERROR_IPV6_SRCADDR 19
#define ERROR_IPV6_VERSION 20
#define ERROR_IPV6_PAYLOAD 21
#define ERROR_IPV6_NEXTHEAD 22
#define ERROR_IPV6_HOPLIMIT 23
#define ERROR_IPV6_NEXTHOP 24
#define ERROR_AVAIL_JUDGE 25
#define ERROR_TIMEOUT 26
#define ERROR_BGPFSM  27
#define ERROR_BGPPACKET  28
*/

// for slide window test
#define MSG_TYPE_SEND 1
#define MSG_TYPE_RECEIVE 2
#define MSG_TYPE_TIMEOUT 3
#define MSG_TYPE_RESEND 4


//for rip 
#define RIP_MAX_ROUTES 	25		/* rip max routes number */
#define RIP_UPDATE_TIMEOUT 1    /*send route table*/
#define RIP_MSG_DELE_ROUTE 1
#define RIP_MSG_SEND_ROUTE 2 

#define STUD_RIP_TEST_VERSION_ERROR 1
#define STUD_RIP_TEST_COMMAND_ERROR 2
#define STUD_RIP_TEST_MZERO_ERROR  3

#define EXPE_STUDID_LEN 16

//added by wangyl for ipsec

#define IPSEC_TRANSPORT_MODE 1
#define IPSEC_TUNNEL_MODE 2
#define IPSEC_COMB_MODE 3

#define IPSEC_AH_PROTOCOL 1
#define IPSEC_ESP_PROTOCOL 2
#define IPSEC_COMB_PROTOCOL 3

#define IPSEC_AH_DISABLE 0
#define IPSEC_AH_ENABLE 1

#define IPSEC_ESP_DISABLE 0
#define IPSEC_ESP_ENABLE 1

#define IPSEC_ESP_3DES 1
#define IPSEC_ESP_DES 2


/****************************************************/

// for every table item of MSG_SUBTYPE_ROUTEADD 
typedef struct stud_route_msg
{
	UINT32  dest;
	UINT32  masklen;
	UINT32  nexthop;
};

// for every table item of MSG_SUBTYPE_ROUTEADD 
typedef struct stud_ipv6_route_msg
{
	ipv6_addr  dest;
	UINT32  masklen;
	ipv6_addr  nexthop;
};

// for rip route table
typedef struct rip_route_table
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int metric;
	unsigned int if_no;
};

typedef struct MN_route_table
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
};

typedef struct FA_route_table
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
};

typedef struct HA_route_table
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
};


//added by xiaojun for arp test
typedef struct arp_cache_table
{
	UINT32 dest_ip_address;//协议地址
	UINT8 dest_ethernet_macadd[6];//目的硬件地址
	UINT16	elementAge;//生存时间
	UINT8	maxRetries;//最大请求次数
};
typedef struct commonHead {
	UINT16 msgLen; // message length
	char type;   // msg type MSG_TYPE_DATA OR  MSG_TYPE_CONTROL
	char subType; // SUB TYPE
};

// for MSG_SUBTYPE_REP
typedef struct replyHead{
	commonHead commonhead;
	unsigned short result;
};

//xiangxj add for ipv6packt return
typedef struct returnHead{
	commonHead commonhead;
	int returnlen;
	char tembuffer[100];
};

// for MSG_SUBTYPE_RIPROUTESEND
typedef struct ripRouteHead{
	commonHead commonhead;
	unsigned int iCount;
	UINT32 destaddress;
	char data[1];
};

// for MSG_SUBTYPE_FAROUTESEND
typedef struct FARouteHead{
	commonHead commonhead;
	unsigned char iCount;
	char data[1];
};

// for MSG_SUBTYPE_HAROUTESEND
typedef struct HARouteHead{
	commonHead commonhead;
	unsigned char iCount;
	char data[1];
};

// for MSG_SUBTYPE_MNROUTESEND
typedef struct MNRouteHead{
	commonHead commonhead;
	unsigned int iCount;
};


//for MSG_SUBTYPE_ARPCACHESEND
typedef struct arpcacheHead{
	commonHead commonhead;
	UINT8 Count;
	char data[1];
};



//for MSG_CACHE_TIMEOUT
typedef struct arpcacheTimeoutHead{
	commonHead commonhead;
	UINT32  destipaddr;
};

//for MSG_SUBTYPE_RIPTIMEOUT
typedef struct ripTimeoutHead{
	commonHead commonhead;
	char MsgType;
	UINT32 destAddr;
	UINT32 masklen;
};

//added by xiaojun for mobileip protocol
//for MSG_SUBTYPE_MOBILEIP_REP
typedef struct mirephead{
	commonHead commonhead;
	UINT8 MsgCode;
};

// for MSG_SUBTYPE_IPSECDATA
typedef struct ipsecDataHead{
	commonHead commonhead;
	DWORD dwLen;
};


//for MSG_SUBTYPE_BGPTIMEOUT
typedef struct BGPTimeoutHead{
	commonHead commonhead;
	char MsgType;	
};
//for MSG_SUBTYPE_BGPEXCEPT
typedef struct BGPExceptHead{
	commonHead commonhead;
	char MsgType;	
};

//for MSG_SUBTYPE_RIPROUTEPRINT
#pragma pack(1)
typedef struct ripRoutePrintHead{
	commonHead commonhead;
	unsigned char pflag;
	unsigned int iCount;	
	char cData[1];
	//rip_route_table rt[10];
};
#pragma pack()


// for MSG_SUBTYPE_REP
typedef struct resultHead{
	commonHead commonhead;
	char testitem;
	char testcase;
	UINT16 result;
};

// for MSG_SUBTYPE_LIST
typedef struct getListHead{
	commonHead commonhead;
	UINT8 flag; 
	char data[1];
};

// for every table item MSG_SUBTYPE_LIST
typedef struct listField{
	UINT16 strLen;
	UINT16 index;
	UINT16 father;
	char pStr[1];
};


// for MSG_SUBTYPE_GET_SCRIPT
typedef struct getScriptHead{
	commonHead commonhead;
	UINT8 flag; // 0 : request; 1: response
	char itemIndex;
	char caseIndex;
	char data[1];
};

// for every list item of MSG_SUBTYPE_GET_SCRIPT
typedef struct scriptField{
	UINT16 num;
	UINT16 len;
	UINT16 op;
	UINT16 packetType;
	UINT16 timeout;
    //UINT16 nakno;
	UINT16 funcName;
	UINT16 para[MAX_PARA_NUM];
	ip_addr srcAddr;
	ip_addr destAddr;
	char pStr[1];
};

typedef struct EthernetHead{
	UINT8 destMac[6];
	UINT8 srcMac[6];
	UINT16 protocol;
};

typedef struct IPHead{
	UINT8 ver;
	UINT8 tos;
	UINT16 totalLen;
	UINT32 ignore;
	UINT8 ttl;
	UINT8 protocol;
	UINT16 checksum;
	UINT32 srcAddr;
	UINT32 destAddr;
	char pData[1];
};

typedef struct IPv6Head{
	UINT16 verTraffic;
	UINT16 flowLabel;
	UINT16 payLoad;
	UINT8 nextHead;
	UINT8 hopLimit;
	ipv6_addr srcAddr;
	ipv6_addr destAddr;
	char pData[1];
};

typedef struct tcpHead{
	UINT16 srcPort;
	UINT16 destPort;
	UINT32 seqNo;
	UINT32 ackNo;
	UINT8  headLen;
	UINT8  flag;
	UINT16 windowsize;
	UINT16 checksum;
	UINT16 urgentPointer;
	char pData[1];
};

typedef struct udpHead{
	UINT16 srcPort;
	UINT16 destPort;
	UINT16 len;
	UINT16 checksum;
	char pData[1];
};

//移动代理广播扩展
typedef struct mobileip_ExpendField
{
	UINT8 type;
	UINT8 datalen;
	UINT16 sequence;
	UINT16 max_ttl;
	UINT16 ignore;
	char pdata[1];
};

typedef struct  RipIpAddr{
	union{
		unsigned long ul;	/* long type */
		unsigned short us[2];	/* short type */
		unsigned char uc[4];	/* char type */
	}u;
#define u_l u.ul
#define u_s u.us
#define u_c u.uc
};
typedef struct RipRt{ 	
	short rr_family;		/* rip route family */
	union{
		short u1_mbz;		/* ripv1 must-be-zero */
		short u1_rttag;		/* ripv2 route-tag */
		short u1_atype;		/* ripv2 authentication mode */
	}u1;
#define rr_mbz	u1.u1_mbz
#define rr_rttag u1.u1_rttag
#define rr_atype u1.u1_atype
	union{
		char u2_key[16];	/* ripv2 authentication key */
		struct{
			RipIpAddr s1_addr;	/* rip dst address */
			union{
				char u3_data[8];
				struct{
					RipIpAddr s2_mask;	/* ripv2 mask address */
					RipIpAddr s2_nexthop;	/* ripv2 nexthop address */
				}s2;
			}u3;
			int s1_metric;	/* rip route metric */
		} s1;
		
	}u2;
#define rr_akey		u2.u2_key		/* rip route authentication key for v2 */
#define rr_addr		u2.s1.s1_addr		/* rip route address */
#define rr_data		u2.s1.u3.u3_data	/* rip route must-be-zero for v1 */
#define rr_mask		u2.s1.u3.s2.s2_mask	/* rip route mask for v2 */
#define rr_nexthop	u2.s1.u3.s2.s2_nexthop	/* rip route nexthop for v2 */
#define rr_metric	u2.s1.s1_metric		/* rip route metric */
};

typedef struct RipPacket{ 	
	char rip_cmd;				/* rip command field */
	char rip_vers;				/* rip version field */
	short rip_mbz;				/* rip must-be-zero field */
	struct RipRt rip_rts[RIP_MAX_ROUTES];	/* rip individual route entry */
};

typedef struct stud_rip_route_node
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int metric;
	unsigned int if_no;
	struct stud_rip_route_node *next;
};
//added by xiaojun for arp test
typedef struct stud_arp_cache_node
{
	UINT32 dest_ip_address;//协议地址
	UINT8 dest_ethernet_macadd[6];//目的硬件地址
	UINT16	elementAge;//生存时间
	UINT8	maxRetries;//最大请求次数
	struct stud_arp_cache_node *next;
};
//add end

typedef struct stud_MN_route_node
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
	struct stud_MN_route_node *next;
};

typedef struct stud_FA_route_node
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
	struct stud_FA_route_node *next;
};

typedef struct stud_HA_route_node
{
	unsigned int dest;
	unsigned int mask;
	unsigned int nexthop;
	unsigned int if_no;
	struct stud_HA_route_node *next;
};

//for slide window
typedef enum {sysdata,sysack,sysnak} sys_frame_kind;
typedef struct {
	sys_frame_kind kind;
	unsigned int seq;
	unsigned int ack;
}sys_frame;

typedef struct {
	unsigned int flag;
	unsigned int len;	
}sys_frame_send;


//for bgp fsm
//bgp状态机宏定义
#define BGP_STATE_IDLE			1	// idle state - ignore everything 
#define BGP_STATE_CONNECT		2	// connect state - trying to connect 
#define BGP_STATE_ACTIVE		3	// waiting for a connection 
#define BGP_STATE_OPENSENT		4	// open packet has been sent 
#define BGP_STATE_OPENCONFIRM	5	// waiting for a keepalive or notify 
#define BGP_STATE_ESTABLISHED	6	// connection has been established

// BGP PACKET types
#define BGP_OPEN		    1		/* open message */
#define BGP_UPDATE		    2		/* update message */
#define BGP_NOTIFY		    3		/* notification message */
#define BGP_KEEPALIVE	    4		/* keepalive message */    

//BGP timeout message type
#define BGP_RETRANSMISSION_TIMEOUT      0      /*retransmission timeout*/  
#define BGP_CONNECTRETRY_TIMEOUT        1      /*connectretry timeout*/
#define BGP_HOLD_TIMEOUT                2      /*hold timeout*/
#define BGP_KEEPALIVE_TIMEOUT           3      /*keepalive timeout*/

//BGP global struct
struct BgpPeer
{       
        unsigned int bgp_dwMyRouterID;    // 本路由器的路由器ID;        
        unsigned short  bgp_wMyAS ;  //  向对方建立时，所用的AS。联盟相关
		unsigned int bgp_dwCfgHoldtime;	//设置的 holdtime时间值   
        unsigned char  bgp_byState;			//协议状态
		unsigned char bgp_bAuth;       //    是否有认证信息
		unsigned char bVersion;        //当前bgp的版本号
};
//end BGP FSM


typedef struct arp_or_rarp_pkt
{
      UINT8 dest_ethernet_macadd[6];//以太网目的地址
	  UINT8 src_ethernet_macadd[6];//以太网源地址 
	  UINT16 framekind; //帧类型
	  UINT8  hardwarekind; //硬件地址的类型，为1表示以太地址
      UINT16  protocolkind; //协议类型字段
      UINT8  macaddr_len;//硬件地址长度
      UINT8  ipadd_len; //IP地址长度
      UINT16  kind;//帧的类型
      UINT8   src_mac_addr[6]; //发送方以太网地址
      UINT32 srcaddress;
	  UINT8   dest_mac_addr[6]; //目的以太网地址     
	  UINT32 destaddress;
};

// extern function and variable declare
extern UINT32 test_server_ip_address;
extern UINT32 test_client_logic_ip;
extern UINT32 test_server_logic_ip;
extern UINT8  test_client_mac[6];
extern UINT8  test_server_mac[6];
extern ipv6_addr test_client_logic_ipv6;
extern ipv6_addr test_server_logic_ipv6;
extern struct stud_rip_route_node *g_rip_route_table;
extern  struct stud_arp_cache_node *g_arp_cache_table;
extern struct  stud_MN_route_node *g_MN_route_table;
extern struct  stud_FA_route_node *g_FA_route_table;
extern struct  stud_HA_route_node *g_HA_route_table;
//remove by wangyl for delete variable g_rip_route_table_count
extern unsigned int g_rip_route_table_count;


extern void clearMacTable();
extern void clientParseInit(void);
extern void serverParseInit(void);
extern void ip_DiscardPkt(char * pBuffer, int type);
extern void ipv6_DiscardPkt(char * pBuffer, int type);
extern void ip_SendtoLower(char *pBuffer,int length);
extern void ip_SendtoUp(char *pBuffer, int length);
extern void ipv6_SendtoLower(char *pBuffer,int length);
extern void ipv6_SendtoUp(char *pBuffer, int length);
extern unsigned int getIpv4Address();
extern ipv6_addr* getIpv6Address();
extern void arp_DiscardPkt(char * pBuffer, int type);

extern int SendIpPacket(unsigned char* pData, unsigned short len, unsigned int
				 srcAddr, unsigned int dstAddr, unsigned char ttl);

extern int HA_send_Encap_packet(char* pbuffer,unsigned short len,unsigned int dstAddr,unsigned int srcAddr);

extern int SendIpv6Packet(unsigned char* pData, unsigned short len, ipv6_addr
				   srcAddr, ipv6_addr dstAddr, unsigned char hoplimit,unsigned char nexthead);

extern void ipv6_fwd_SendtoLower(char *pBuffer, int length, ipv6_addr* nexthop);
// for ip forward test
extern void fwd_DiscardPkt(char * pBuffer, int type);
extern void HA_DiscardPkt(char * pBuffer, int length);
extern void FA_DiscardPkt(char * pBuffer, int length);
extern void fwd_LocalRcv(char *pBuffer, int length);

// for ipv6 forward test
extern void ipv6_fwd_DiscardPkt(char * pBuffer, int type);

extern void ipv6_fwd_LocalRcv(char *pBuffer, int length);

// for tcp test
extern void tcp_sendIpPkt(unsigned char* pData, UINT16 len, unsigned int  srcAddr,
				   unsigned int dstAddr, UINT8	ttl);

extern void tcp_DiscardPkt(char * pBuffer, int type);

extern void tcp_sendReport(int type);

extern int waitIpPacket(char *pBuffer, int timeout);

extern UINT32 getServerIpv4Address();

extern char *getSubStr(const char *str,int beg,int len);

extern char *lTrim(char *);

extern char *rTrim(char *);

extern char *Trim(char *);

extern UINT16 getBufferFromHexString(char *hexStr,char *pBuffer);

extern BOOL stud_test_initialize;

extern void rip_sendIpPkt(unsigned char* pData, UINT16 len,unsigned short dstPort,UINT8 iNo);
extern int icmp_sendIpPkt(unsigned char* pData, UINT16 len);
extern int MN_regi_sendIpPkt(unsigned char* pData, unsigned short len);
extern int FA_sendregi_rep(char* pData,unsigned short len,unsigned int destadd,unsigned int srcadd);
extern int FA_sendregi_req(char* pData,unsigned short len,unsigned int destadd,unsigned int srcadd);
extern int HA_regi_sendIpPkt(unsigned char type,unsigned char code,unsigned int destadd,unsigned int srcadd,unsigned int Mn_addr,unsigned int Ha_addr);
extern void SendFRAMEPacket(unsigned char* pData, unsigned int len);
extern UINT32 getMNHaddress();
extern int FA_send_ipv4_toMN(char *pBuffer, int length);
extern int HA_forward_ipv4packet(char *pBuffer, int length);

extern void SendARPPacket(unsigned char* pData, unsigned int len);

extern void clientSendTcpPacket(char flag,UINT32 seqNo, UINT32 ackNo);

extern void bgp_FsmSendTcpData(char *pBuf,DWORD dwLen);

extern int clientTcpPacket(unsigned short srcPort, unsigned short destPort, unsigned
	int seqNum, unsigned int ackNum, unsigned char flag, char* pData,int len);

extern  void bgp_FsmTryToConnectPeer();

class scriptcommonDefine{
private:
	static scriptcommonDefine *scriptdefine;

	// initialize the vector variables
	scriptcommonDefine(){

	}

	~scriptcommonDefine(){
	}

public:
		int (*Func_ip_recv)(char * , UINT16);
		int (*Func_ip_Upsend)(char* , UINT16, UINT32, UINT32, UINT8, UINT8);
		void (*Func_route_add)(stud_route_msg *);
		void (*Func_Route_Init)();
		int (*Func_fwd_deal)(char *, int);
		int (*Func_tcp_input)(char *,UINT16, UINT32, UINT32);
		void (*Func_tcp_output)(char *, UINT16,unsigned char, UINT16, UINT16,UINT32,UINT32);
		int (*Func_tcp_socket)(int,	int, int);
		int (*Func_tcp_connect)(int, sockaddr_in*, int);
		int (*Func_tcp_send)(int, const unsigned char*, UINT16, int);
		int (*Func_tcp_recv)(int, unsigned char*, UINT16, int);
		int (*Func_tcp_close)( int);
		int (*Func_ipv6_recv)(char * , UINT16);
		int (*Func_ipv6_Upsend)(char* , UINT16, ipv6_addr *, ipv6_addr *, char, char);
		void (*Func_ipv6_route_add)(stud_ipv6_route_msg *);
		void (*Func_ipv6_Route_Init)();
		int (*Func_ipv6_fwd_deal)(char *, int);
		int (*Func_slide_window)(char *,int, UINT8);
		int (*Func_rip_packet_recv)(char *,int,UINT8,UINT32);
		void (*Func_rip_route_timeout)(UINT32,UINT32 ,unsigned char);
		int (*Func_mobileip_icmp_recv)(char *,UINT16);
		int (*Func_mobileip_icmp_send)();
		int (*Func_MN_send_Regi_req)(char *,unsigned char,unsigned short,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int);
		int (*Func_MN_recv_Regi_rep)(char *,unsigned short,unsigned int);
		int (*Func_FA_recv_Regi_req)(char *,unsigned short,unsigned int,unsigned int);
		int (*Func_FA_recv_Regi_rep)(char *,unsigned short);
		int (*Func_HA_recv_Regi_req)(char *,unsigned short,unsigned int,unsigned int);
		int (*Func_packertrans_HA_recv)(char *,unsigned short);
		int (*Func_packertrans_FA_recv)(char *,unsigned short);
		BYTE (*Func_bgp_FsmEventOpen)(BgpPeer *,BYTE *,unsigned int);
		BYTE (*Func_bgp_FsmEventKeepAlive)(BgpPeer *,BYTE *,unsigned int);
		BYTE (*Func_bgp_FsmEventNotification)(BgpPeer *,BYTE *,unsigned int);
		BYTE (*Func_bgp_FsmEventUpdate)(BgpPeer *,BYTE *,unsigned int);
		BYTE (*Func_bgp_FsmEventTcpException)(BgpPeer *,BYTE);
		BYTE (*Func_bgp_FsmEventTimerProcess)(BgpPeer *,BYTE);
		BYTE (*Func_bgp_FsmEventStart)(BgpPeer *);
		BYTE (*Func_bgp_FsmEventStop)(BgpPeer *);
		BYTE (*Func_bgp_FsmEventConnect)(BgpPeer *);

		int (*Func_arp_recv)(char * , UINT16);//added by xiaojun
        void (*Func_arp_cache_timeout)(UINT32,unsigned char);
		int (*Func_rarp_recv_rep)(char * , UINT16);//added by xiaojun

		int (*Func_ipsec_InPut)(char *, UINT32);//added by wangyl for ipsec 20080924
		int (*Func_ipsec_OutPut)(char *,UINT32);




	static scriptcommonDefine* getInstance(){
		if (scriptdefine == NULL){
			scriptdefine = new scriptcommonDefine();
		}

		return scriptdefine;
	}

	// following the input string return corresponding index value
	int getVectorIndex(char *myVector[], char * myStr);

	// following the index return corresponding name string
	char * getVectorString(char *myVector[], UINT16 index);

	int getOperateTypeIndex(char * mystr);

	char * getOperateTypeString(int index);

	int getPacketTypeIndex(char * mystr);

	char * getPacketTypeString(int index);


	int getFunctionNameIndex(char * mystr);

	int getParaIndex(int myType, char * mystr);

	char * getParaString(int myType, int index);

	void scriptcommonDefine::regist_ip_recv(int (*myFun)(char * , UINT16));

	void scriptcommonDefine::regist_ip_upsend(int (*myFun)(char* , UINT16, UINT32, UINT32, UINT8, UINT8));

	void scriptcommonDefine::regist_route_add(void (*myFun)(stud_route_msg *));

	void scriptcommonDefine::regist_route_init(void (*myFun)());

	void scriptcommonDefine::regist_fwd_deal(int (*myFun)(char *, int));

	void scriptcommonDefine::regist_tcp_input(int (*myFun)(char *,UINT16, UINT32, UINT32));

	void scriptcommonDefine::regist_tcp_output(void (*myFun)(char *, UINT16, unsigned char, UINT16, UINT16, UINT32,UINT32));

	void scriptcommonDefine::regist_tcp_socket(int (*myFun)(int,	int, int));

	void scriptcommonDefine::regist_tcp_connect(int (*myFun)(int, sockaddr_in*, int));

	void scriptcommonDefine::regist_tcp_send(int (*myFun)(int, const unsigned char*, UINT16, int));

	void scriptcommonDefine::regist_tcp_recv(int (*myFun)(int, unsigned char *, UINT16, int));

	void scriptcommonDefine::regist_tcp_close(int (*myFun)( int));

	// support the ipv6 test
	void scriptcommonDefine::regist_ipv6_recv(int (*myFun)(char * , UINT16));

	void scriptcommonDefine::regist_ipv6_upsend(int (*myFun)(char* , UINT16, ipv6_addr *, ipv6_addr *, char, char));

	void scriptcommonDefine::regist_ipv6_route_add(void (*myFun)(stud_ipv6_route_msg *));

	void scriptcommonDefine::regist_ipv6_route_init(void (*myFun)());

	void scriptcommonDefine::regist_ipv6_fwd_deal(int (*myFun)(char *, int));

	// support the slide windows protocol
	void scriptcommonDefine::regist_slide_window(int (*myFun)(char *,int, UINT8));

	// support the rip protocol

	void scriptcommonDefine::regist_rip_packet_recv(int (*myFun)(char *,int,UINT8,UINT32));

	void scriptcommonDefine::regist_rip_route_timeout(void (*myFun)(UINT32,UINT32 ,unsigned char ));
	
	//support mobileip by xiaojun

	void scriptcommonDefine::regist_mobileip_icmp_recv(int (*myFun)(char *,UINT16));

	void scriptcommonDefine::regist_mobileip_icmp_send(int (*myFun)());

	void scriptcommonDefine::regist_MN_send_Regi_req(int (*myFun)(char *,unsigned char,unsigned short,unsigned int,unsigned int,unsigned int,unsigned int,unsigned int));

	void scriptcommonDefine::regist_MN_recv_Regi_rep(int (*myFun)(char *,unsigned short,unsigned int));

	void scriptcommonDefine::regist_FA_recv_Regi_req(int (*myFun)(char *,unsigned short,unsigned int,unsigned int));

	void scriptcommonDefine::regist_FA_recv_Regi_rep(int (*myFun)(char *,unsigned short));

	void scriptcommonDefine::regist_HA_recv_Regi_req(int (*myFun)(char *,unsigned short,unsigned int,unsigned int));
	
	void scriptcommonDefine::regist_packertrans_HA_recv(int (*myFun)(char *,unsigned short));

	void scriptcommonDefine::regist_packertrans_FA_recv(int (*myFun)(char *,unsigned short));
	//support bgp fsm 
	void scriptcommonDefine::regist_bgp_FsmEventOpen(BYTE (*myFun)(BgpPeer *,BYTE * ,unsigned int));

	void scriptcommonDefine::regist_bgp_FsmEventKeepAlive(BYTE (*myFun)(BgpPeer *,BYTE * ,unsigned int));

	void scriptcommonDefine::regist_bgp_FsmEventNotification(BYTE (*myFun)(BgpPeer *,BYTE *,unsigned int ));

	void scriptcommonDefine::regist_bgp_FsmEventUpdate(BYTE (*myFun)(BgpPeer *,BYTE *,unsigned int ));

	void scriptcommonDefine::regist_bgp_FsmEventTcpException(BYTE (*myFun)(BgpPeer *,BYTE));

	void scriptcommonDefine::regist_bgp_FsmEventTimerProcess(BYTE (*myFun)(BgpPeer *,BYTE));

	void scriptcommonDefine::regist_bgp_FsmEventStart(BYTE (*myFun)(BgpPeer *));

	void scriptcommonDefine::regist_bgp_FsmEventStop(BYTE (*myFun)(BgpPeer *));

	void scriptcommonDefine::regist_bgp_FsmEventConnect(BYTE (*myFun)(BgpPeer *));
	// added by xiao jun for arp protocol
    void scriptcommonDefine::regist_arp_recv(int (*myFun)(char * , UINT16));
	void scriptcommonDefine::regist_arp_cache_timeout(void (*myFun)(UINT32,unsigned char ));
    void scriptcommonDefine::regist_rarp_recv_rep(int (*myFun)(char * , UINT16));

	// add by wangyl for ipsec 20080924

	void scriptcommonDefine::regist_ipsec_InPut(int (*myFun)(char * ,unsigned int));
	void scriptcommonDefine::regist_ipsec_OutPut(int (*myFun)(char * ,unsigned int));
};

#endif
