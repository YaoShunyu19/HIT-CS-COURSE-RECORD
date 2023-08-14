#include "stdafx.h"
#include "common.h"
#include "scriptClass.h"
#include "scriptParser.h"
#include "Client.h"

UINT32 test_server_ip_address = 0;

UINT32 test_client_logic_ip = 0;
UINT32 test_server_logic_ip = 0;
UINT8  test_client_mac[6];
UINT8  test_server_mac[6];

UINT16 test_Item;

UINT16 test_Case;

struct stud_rip_route_node *g_rip_route_table = NULL;
//added by xiaojun for arp test
 struct stud_arp_cache_node *g_arp_cache_table = NULL;
//remove by wangyl for delete variable g_rip_route_table_count
unsigned int g_rip_route_table_count = 0;

unsigned int g_rip_packet_type = 0;

//for bgp fsm
struct BgpPeer  stPeer;
unsigned int g_bgp_tcp_seqNo = 0;
unsigned int g_bgp_tcp_ackNo = 0;

BOOL stud_test_initialize = false;

ipv6_addr test_client_logic_ipv6;
ipv6_addr test_server_logic_ipv6;

char *operateType[] = {"","INPUT","OUTPUT",""};
char *packetType[] = {"","IPv4","IPv6","TCP","UDP","SET","ROUTEADD","IPV6ROUTEADD","REQ","REP","RIPTIMEOUT","RIPDELETROUTE","RIPROUTEINIT","RIPROUTEPRINT","RIPROUTESEND","FRAME","BGPSTATUE","BGPTIMEOUT","BGPEXCEPT","ARP","ARPCACHESEND","ARPCACHETIMEOUT","ARPCACHEINIT",""};
char *functionName[] = {"",
"stud_ip_recv", /*  1 */
"stud_ip_Upsend",  /* 2 */
"stud_route_add", /*3*/
"stud_Route_Init", /*4*/
"stud_fwd_deal", /*5*/
"stud_tcp_input",/*6*/
"stud_tcp_output",/*7*/
"stud_tcp_socket",/*8*/
"stud_tcp_connect",/*9*/
"stud_tcp_send",/*10*/
"stud_tcp_recv",/*11*/
"stud_tcp_close",/*12*/
"stud_ipv6_recv",/*13*/
"stud_ipv6_Upsend",/*14*/
"stud_ipv6_route_add",/*15*/
"stud_ipv6_Route_Init",/*16*/
"stud_ipv6_fwd_deal",/*17*/
"stud_slide_window",/*18*/
"stud_rip_packet_recv", /*19*/
"stud_rip_route_timeout",/*20*/
"stu_bgp_FsmEventOpen",/*21*/
"stu_bgp_FsmEventKeepAlive",/*22*/
"stu_bgp_FsmEventNotification",/*23*/
"stu_bgp_FsmEventUpdate",/*24*/
"stu_bgp_FsmEventTcpException",/*25*/
"stu_bgp_FsmEventTimerProcess",/*26*/
"stu_bgp_FsmEventStart",/*27*/
"stu_bgp_FsmEventStop",/*28*/
"stu_bgp_FsmEventConnect",/*29*/
"stud_arp_recv",/*30*/
"stud_arp_cache_timeout",/*31*/
"stud_rarp_recv_rep",/*32*/
""};


char *paraIPv4[] = {"",
"srcMac",
"destMac",
"version",
"headLen",
"totalLen",
"ttl",
"protocol",
"checksum",
"data",
"srcAddr",
"destAddr",
""};

char *paraIPv6[] = {"",
"srcMac",
"destMac",
"version",
"payload",
"nextHead",
"hopLimit",
"srcAddr",
"destAddr",
"data",
""};

char *paraTCP[] ={"",
"srcMac",
"destMac",
"version",
"headLen",
"totalLen",
"ttl",
"protocol",
"checksum",
"data",
"srcAddr",
"destAddr",
"srcPort",
"destPort",
"seqno",
"ackno",
"flag",
"tcpchecksum",
"tcpdata",
"socket",
"timeout",
"bgptype"
""};

char *paraUDP[] ={"",
"srcMac",
"destMac",
"version",
"headLen",
"totalLen",
"ttl",
"protocol",
"checksum",
"data",
"srcAddr",
"destAddr",
"srcPort",
"destPort",
"udpchecksum",
"udpdata",
"socket",
"ifno",
"packettype",
"checkflag",
""};

char *paraREP[] = {"",
"result",
""
};


char *paraSet[] = {"","var1","var2","var3","var4","var5","var6","var7","var8","var9","var10",""};

char *paraAddRoute[] = {"","data",""};

char *paraFRAME[] = {"","seqno","timeout","ackno","flag","nakno",""};

char *paraBGPSTATUE[] = {"","type",""};

char *paraBGPTIMEOUT[] = {"","type",""};

char *paraBGPEXCEPT[] = {"","type",""};

//added by xiaojun for arp protocol
char *paraARP[] = {"","flagnum","kind","dest_ethernet_macadd","src_ethernet_macadd","hardwarekind","framekind","protocolkind","macaddr_len","ipadd_len","destaddr","dest_mac","srcaddress","","src_mac_addr",""};
char *paraARPCACHESEND[] = {"","flagno",""};
char *paraARPCACHEINIT[] = {"","flagno",""};
scriptcommonDefine* scriptcommonDefine::scriptdefine = NULL;

// following the input string return corresponding index value
int scriptcommonDefine::getVectorIndex(char *vect[], char * myStr){
	int retIndex = 0;
	int loopIndex = 1;
	char *str1;
	char *str2;
	int i2;

       str2 = myStr;
	i2 = strlen(str2);
	//printf("i2=============%d\n",i2);
	while (vect[loopIndex] != ""){
		str1 = vect[loopIndex];
		

	/*if(strlen(myStr) > strlen(vect[loopIndex]))
		{
			len = strlen(myStr);
		}
		else
		{
			len = strlen(vect[loopIndex]);
		}*/
		//if((strlen(myStr) == strlen(vect[loopIndex]))  && (memcmp(vect[loopIndex],myStr,strlen(vect[loopIndex])) == 0)){
	
		if(i2 == strlen(str1))
		{
			if(memcmp(str1,str2,i2) == 0)
			{
				retIndex = loopIndex;
				break;
			}
		}
		loopIndex ++;
	}

	return retIndex;
}


// following the index return corresponding name string
char * scriptcommonDefine::getVectorString(char *vect[], UINT16 index){
	return vect[index];
}

int scriptcommonDefine::getOperateTypeIndex(char * mystr){
	return getVectorIndex(operateType,mystr);
}


int scriptcommonDefine::getFunctionNameIndex(char * mystr){
	return getVectorIndex(functionName,mystr);
}


char * scriptcommonDefine::getOperateTypeString(int index){
	return getVectorString(operateType,index);
}

int scriptcommonDefine::getPacketTypeIndex(char * mystr){
	return getVectorIndex(packetType,mystr);
}

char * scriptcommonDefine::getPacketTypeString(int index){
	return getVectorString(packetType,index);
}

int scriptcommonDefine::getParaIndex(int myType, char * mystr){

	switch(myType){
		case PACKET_TYPE_IPV4:// ipv4
			return getVectorIndex(paraIPv4,mystr);
			break;

		case PACKET_TYPE_IPV6: // ipv6
			return getVectorIndex(paraIPv6,mystr);
			break;

		case PACKET_TYPE_TCP:
			return getVectorIndex(paraTCP,mystr);
			break;

		case PACKET_TYPE_UDP:
			return getVectorIndex(paraUDP,mystr);
			break;

		case PACKET_TYPE_SET:
			return getVectorIndex(paraSet,mystr);
			break;

		case PACKET_TYPE_ADDROUTE:
			return getVectorIndex(paraAddRoute,mystr);
			break;

		case PACKET_TYPE_REP:
			return getVectorIndex(paraREP,mystr);
			break;
		case PACKET_TYPE_FRAME:
			return getVectorIndex(paraFRAME,mystr);
			break;
		case PACKET_TYPE_BGPSTATUE:
			return getVectorIndex(paraBGPSTATUE,mystr);
			break;
		case PACKET_TYPE_BGPTIMEOUT:
			return getVectorIndex(paraBGPTIMEOUT,mystr);
			break;
		case PACKET_TYPE_BGPEXCEPT:
			return getVectorIndex(paraBGPEXCEPT,mystr);
			break;
		case PACKET_TYPE_ARP:
			return getVectorIndex(paraARP,mystr);
			break;
		//added by xiaojun for arp protocol 
		case PACKET_TYPE_ARPCACHESEND:
			return getVectorIndex(paraARPCACHESEND,mystr);
			break;
		case PACKET_TYPE_ARPCACHEINIT:
			return getVectorIndex(paraARPCACHEINIT,mystr);
			break;
		//add end
		default:
			break;
	}
	
	return 0;
}


char * scriptcommonDefine::getParaString(int myType, int index){
	
	switch(myType){
		case 1:// ipv4
			return getVectorString(paraIPv4,index);
			break;
		case 2: // ipv6
			return getVectorString(paraIPv6,index);
			break;
		case 3:
			return getVectorString(paraTCP,index);
			break;
		case 4:
			return getVectorString(paraUDP,index);
			break;
		case 5:
			return getVectorString(paraSet,index);
			break;
		case 6:
			return getVectorString(paraAddRoute,index);
			break;
		case 15:
			return getVectorString(paraFRAME,index);
			break;
		case 16:
			return getVectorString(paraBGPSTATUE,index);
			break;
		case 17:
			return getVectorString(paraBGPTIMEOUT,index);
			break;
		case 18:
			return getVectorString(paraBGPEXCEPT,index);
			break;
		default:
			break;
	}

	return NULL;
}

void scriptcommonDefine::regist_ip_recv(int (*myFun)(char * , UINT16)){

		Func_ip_recv = myFun;
}


void scriptcommonDefine::regist_ip_upsend(int (*myFun)(char* , UINT16, UINT32, UINT32, UINT8, UINT8)){
		Func_ip_Upsend = myFun;
}

void scriptcommonDefine::regist_route_add(void (*myFun)(stud_route_msg *)){
		Func_route_add = myFun;
}

void scriptcommonDefine::regist_route_init(void (*myFun)()){
		Func_Route_Init = myFun;
}
	
void scriptcommonDefine::regist_fwd_deal(int (*myFun)(char *, int)){
		Func_fwd_deal = myFun;
}
	
void scriptcommonDefine::regist_tcp_input(int (*myFun)(char *,UINT16, UINT32, UINT32)){
		Func_tcp_input = myFun;
}
	
void scriptcommonDefine::regist_tcp_output(void (*myFun)(char *, UINT16, unsigned char, UINT16, UINT16, UINT32, UINT32)){
		Func_tcp_output = myFun;
}
	
void scriptcommonDefine::regist_tcp_socket(int (*myFun)(int,	int, int)){
		Func_tcp_socket = myFun;
}
	
void scriptcommonDefine::regist_tcp_connect(int (*myFun)(int, sockaddr_in*, int)){
		Func_tcp_connect = myFun;
}
	
void scriptcommonDefine::regist_tcp_send(int (*myFun)(int, const unsigned char*, UINT16, int)){
		Func_tcp_send = myFun;
}
	
void scriptcommonDefine::regist_tcp_recv(int (*myFun)(int, unsigned char*, UINT16, int)){
		Func_tcp_recv = myFun;
}
	
void scriptcommonDefine::regist_tcp_close(int (*myFun)( int)){
		Func_tcp_close = myFun;
}

// add some codes to support the ipv6 test
void scriptcommonDefine::regist_ipv6_recv(int (*myFun)(char * , UINT16)){

	Func_ipv6_recv = myFun;
}

void scriptcommonDefine::regist_ipv6_upsend(int (*myFun)(char* , UINT16, ipv6_addr *, ipv6_addr *, char, char)){
	Func_ipv6_Upsend = myFun;
}

void scriptcommonDefine::regist_ipv6_route_add(void (*myFun)(stud_ipv6_route_msg *)){
	Func_ipv6_route_add = myFun;
}

void scriptcommonDefine::regist_ipv6_route_init(void (*myFun)()){
	Func_ipv6_Route_Init = myFun;
}

void scriptcommonDefine::regist_ipv6_fwd_deal(int (*myFun)(char *, int)){
	Func_ipv6_fwd_deal = myFun;
}

void scriptcommonDefine::regist_slide_window(int (*myFun)(char *,int,UINT8))
{
	Func_slide_window = myFun;
}

void scriptcommonDefine::regist_rip_packet_recv(int (*myFun)(char *,int,UINT8,UINT32))
{
	Func_rip_packet_recv = myFun;
}

void scriptcommonDefine::regist_rip_route_timeout(void (*myFun)(UINT32,UINT32 ,unsigned char))
{
	Func_rip_route_timeout = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventOpen(BYTE (*myFun)(BgpPeer *,BYTE * ,unsigned int))
{
	Func_bgp_FsmEventOpen = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventKeepAlive(BYTE (*myFun)(BgpPeer *,BYTE * ,unsigned int))
{
	Func_bgp_FsmEventKeepAlive = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventNotification(BYTE (*myFun)(BgpPeer *,BYTE *,unsigned int ))
{
	Func_bgp_FsmEventNotification = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventUpdate(BYTE (*myFun)(BgpPeer *,BYTE * ,unsigned int))
{
	Func_bgp_FsmEventUpdate = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventTcpException(BYTE (*myFun)(BgpPeer *,BYTE ))
{
	Func_bgp_FsmEventTcpException = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventTimerProcess(BYTE (*myFun)(BgpPeer *,BYTE))
{
	Func_bgp_FsmEventTimerProcess = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventStart(BYTE (*myFun)(BgpPeer *))
{
	Func_bgp_FsmEventStart = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventStop(BYTE (*myFun)(BgpPeer *))
{
	Func_bgp_FsmEventStop = myFun;
}

void scriptcommonDefine::regist_bgp_FsmEventConnect(BYTE (*myFun)(BgpPeer *))
{
	Func_bgp_FsmEventConnect = myFun;
}


void scriptcommonDefine::regist_arp_recv(int (*myFun)(char * , UINT16)){

		Func_arp_recv = myFun;
}


void scriptcommonDefine::regist_arp_cache_timeout(void (*myFun)(UINT32,unsigned char))
{
	Func_arp_cache_timeout = myFun;
}



void scriptcommonDefine::regist_rarp_recv_rep(int (*myFun)(char * , UINT16)){

		Func_rarp_recv_rep = myFun;
}
            
 //description: use in client to initialize the parser and relation classes
void clientParseInit(void){
	scriptcommonDefine *myScript = scriptcommonDefine::getInstance();
}

// description: use in server to initialize the parser and relation classes
void serverParseInit(void){
	scriptClass *myInstance = scriptClass::getInstance();
	scriptcommonDefine *myScript = scriptcommonDefine::getInstance();

	// load script to memory from hard disk
	myInstance->loadScript();
}


// offer the functions for students
UINT16 IpCalcChecksum(char *pIpHead, UINT16 len)
{
	UINT32 sum = 0;
	UINT16* p = (UINT16*)pIpHead;

	for(int i=0; i<len; i++)
	{
		sum = sum + ntohs(p[i]);
	}
	sum = sum - ntohs(p[5]);
	sum = ( sum >> 16 ) + ( sum & 0xFFFF );
	sum = ( sum >> 16 ) + ( sum & 0xFFFF );

	return (UINT16)(~sum);
}

// offer the functions for students
UINT16 tcpCalcChecksum(char *pIpHead, UINT16 len)
{
	UINT32 sum = 0;
	UINT16* p = (UINT16*)pIpHead;

	for(int i=0; i<len; i++)
	{
		sum = sum + ntohs(p[i]);
	}
	sum = ( sum >> 16 ) + ( sum & 0xFFFF );
	sum = ( sum >> 16 ) + ( sum & 0xFFFF );

	return (UINT16)(~sum);
}

// only used for client
// there are other functions in server.
int SendIpPacket(unsigned char* pData, unsigned short len, unsigned int
	srcAddr, unsigned int dstAddr, unsigned char ttl,unsigned char protocol){
		char outBuffer[MAX_BUFFER_SIZE] = {0};
		int buffsize = 0;

		// Ethernet frame 
		EthernetHead *pHead1 = (EthernetHead *)outBuffer;
		IPHead *pHead2 = (IPHead *)(outBuffer + sizeof(EthernetHead));

		memcpy(pHead1->destMac,test_server_mac,6);
		memcpy(pHead1->srcMac,test_client_mac,6);
		pHead1->protocol = htons(0x800);

		buffsize += sizeof(EthernetHead);

		pHead2->ver = 0x45;
		pHead2->tos = 0;
		pHead2->totalLen = htons(20 + len);
		pHead2->ignore = 0;
		pHead2->ttl = ttl;
		pHead2->protocol = protocol;
		pHead2->srcAddr = htonl(srcAddr);
		pHead2->destAddr = htonl(dstAddr);
		pHead2->checksum = htons(IpCalcChecksum((char *)pHead2,10));

		buffsize += 20;

		memcpy(pHead2->pData,pData,len);

		buffsize += len;

		MessageBlock mb(outBuffer,buffsize);

		Client *pInstance = Client::getInstance();

		pInstance->Send(mb);

		return 0;
	}


int SendIpv6Packet(unsigned char* pData, unsigned short len, ipv6_addr
	srcAddr, ipv6_addr dstAddr, unsigned char hoplimit,unsigned char nexthead){
		char outBuffer[MAX_BUFFER_SIZE] = {0};
		int buffsize = 0;

		// Ethernet frame 
		EthernetHead *pHead1 = (EthernetHead *)outBuffer;
		IPv6Head *pHead2 = (IPv6Head *)(outBuffer + sizeof(EthernetHead));

		memcpy(pHead1->destMac,test_server_mac,6);
		memcpy(pHead1->srcMac,test_client_mac,6);
		pHead1->protocol = htons(0x86dd);

		buffsize += sizeof(EthernetHead);

		pHead2->verTraffic = 0x6 << 12;
		pHead2->payLoad = htons(len);
		pHead2->nextHead = nexthead;
		pHead2->hopLimit = hoplimit;
		memcpy(pHead2->srcAddr.bAddr,srcAddr.bAddr,16);
		memcpy(pHead2->destAddr.bAddr,dstAddr.bAddr,16);

		buffsize += 40;

		memcpy(pHead2->pData,pData,len);

		buffsize += len;

		MessageBlock mb(outBuffer,buffsize);

		Client *pInstance = Client::getInstance();

		pInstance->Send(mb);

		return 0;
	}


void sendReportFunction(unsigned short type){
	char outBuff[100];
	replyHead *pHead = (replyHead *)outBuff;

	//free(pBuffer);

	pHead->commonhead.msgLen = htons(sizeof(replyHead) -2);
	pHead->commonhead.type = MSG_TYPE_CONTROL;
	pHead->commonhead.subType = MSG_SUBTYPE_REP;
	pHead->result = htons(type);

	MessageBlock mb(outBuff);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);
}
//added by xiaojun for arp test
void arp_DiscardPkt(char * pBuffer, int type){
	if(pBuffer!=NULL)
	sendReportFunction(type);
	else
    sendReportFunction(0);
}

// for ip test
void ip_DiscardPkt(char * pBuffer, int type){
	sendReportFunction(type);
}

// for ipv6 test
void ipv6_DiscardPkt(char * pBuffer, int type){
	sendReportFunction(type);
}

// only used for client
void ip_SendtoLower(char *pBuffer, int length){
	char outBuffer[MAX_BUFFER_SIZE] = {0};
	int buffsize = 0;

	if((length >MAX_BUFFER_SIZE - 14)||(length < 0))
	{
		printf("invalid the size of IP packet,send failed!\n");
	       return;
	}
	// Ethernet frame 
	EthernetHead *pHead1 = (EthernetHead *)outBuffer;

	memcpy(pHead1->destMac,test_server_mac,6);
	memcpy(pHead1->srcMac,test_client_mac,6);
	pHead1->protocol = htons(0x800);

	buffsize += sizeof(EthernetHead);

	memcpy(outBuffer + sizeof(EthernetHead),pBuffer,length);

	buffsize += length;

	MessageBlock mb(outBuffer,buffsize);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);	
}

// only used for client
void ipv6_SendtoLower(char *pBuffer, int length){
	char outBuffer[MAX_BUFFER_SIZE] = {0};
	int buffsize = 0;

	if((length >MAX_BUFFER_SIZE - 14)||(length < 0))
	{
		printf("invalid the size of IPV6 packet,send failed!\n");
	    return;
	}

	// Ethernet frame 
	EthernetHead *pHead1 = (EthernetHead *)outBuffer;

	memcpy(pHead1->destMac,test_server_mac,6);
	memcpy(pHead1->srcMac,test_client_mac,6);
	pHead1->protocol = htons(0x86dd);

	buffsize += sizeof(EthernetHead);

	memcpy(outBuffer + sizeof(EthernetHead),pBuffer,length);

	buffsize += length;

	MessageBlock mb(outBuffer,buffsize);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);	
}


// send back a correct reply message
void ip_SendtoUp(char *pBuffer, int length){
	char outBuff[100];
	replyHead *pHead = (replyHead *)outBuff;

	//free(pBuffer);

	pHead->commonhead.msgLen = htons(sizeof(replyHead) -2);
	pHead->commonhead.type = MSG_TYPE_CONTROL;
	pHead->commonhead.subType = MSG_SUBTYPE_REP;
	pHead->result = htons(STUD_IP_TEST_CORRECT);

	MessageBlock mb(outBuff);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);
}

// send back a correct reply message
void ipv6_SendtoUp(char *pBuffer, int length){
	char outBuff[100];
	replyHead *pHead = (replyHead *)outBuff;

	//free(pBuffer);

	pHead->commonhead.msgLen = htons(sizeof(replyHead) -2);
	pHead->commonhead.type = MSG_TYPE_CONTROL;
	pHead->commonhead.subType = MSG_SUBTYPE_REP;
	pHead->result = htons(STUD_IPV6_TEST_CORRECT);

	MessageBlock mb(outBuff);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);
}

ip_addr getIpv4Address(){
	return test_client_logic_ip;
}

void getIpv6Address(ipv6_addr* pAddr){
	assert(pAddr);
	memcpy(pAddr,&test_client_logic_ipv6,sizeof(ipv6_addr));
}

void fwd_SendtoLower(char *pBuffer, int length, unsigned int nexthop){
	char outBuffer[MAX_BUFFER_SIZE] = {0};
	int buffsize = 0;
	UINT8 destMac[6];

	// Ethernet frame 
	EthernetHead *pHead1 = (EthernetHead *)outBuffer;

	destMac[0] = 0;
	destMac[1] = 0x0d;
	memcpy(destMac + 2,&nexthop,4);
	memcpy(pHead1->destMac,destMac,6);
	memcpy(pHead1->srcMac,test_client_mac,6);
	pHead1->protocol = htons(0x800);

	buffsize += sizeof(EthernetHead);

	memcpy(outBuffer + sizeof(EthernetHead),pBuffer,length);

	buffsize += length;

	MessageBlock mb(outBuffer,buffsize);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);
}

// for ip forwarding test
void fwd_DiscardPkt(char * pBuffer, int type){
	sendReportFunction(type);
}

// when the destination is local host, then call this function
void fwd_LocalRcv(char *pBuffer, int length){
	sendReportFunction(STUD_FORWARD_TEST_LOCALHOST);
}


void ipv6_fwd_SendtoLower(char *pBuffer, int length, ipv6_addr* nexthop){
	char outBuffer[MAX_BUFFER_SIZE] = {0};
	int buffsize = 0;
	UINT8 destMac[6];

	// Ethernet frame 
	if((length >MAX_BUFFER_SIZE - 14)||(length < 0))
	{
		printf("invalid the size of IPV6 packet,send failed!\n");
	    return;
	}
	
	EthernetHead *pHead1 = (EthernetHead *)outBuffer;

	destMac[0] = 0;
	destMac[1] = 0x0d;
	memcpy(destMac + 2,&nexthop->bAddr[12],4);
	memcpy(pHead1->destMac,destMac,6);
	memcpy(pHead1->srcMac,test_client_mac,6);
	pHead1->protocol = htons(0x86dd);

	buffsize += sizeof(EthernetHead);

	memcpy(outBuffer + sizeof(EthernetHead),pBuffer,length);

	buffsize += length;

	MessageBlock mb(outBuffer,buffsize);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);
}

// for ipv6 forwarding test
void ipv6_fwd_DiscardPkt(char * pBuffer, int type){
	sendReportFunction(type);
}

// when the destination is local host, then call this function
void ipv6_fwd_LocalRcv(char *pBuffer, int length){
	sendReportFunction(STUD_IPV6_FORWARD_TEST_LOCALHOST);
}


// for tcp test
void tcp_DiscardPkt(char * pBuffer, int type){
	sendReportFunction(type);
}

void tcp_sendReport(int type){
	sendReportFunction(type);
}

void tcp_sendIpPkt(unsigned char* pData, UINT16 len, unsigned int  srcAddr,
				   unsigned int dstAddr, UINT8	ttl = 10){
	SendIpPacket((unsigned char *)pData,len,srcAddr,dstAddr,ttl,PROTOCOL_TCP);
	Sleep(2);
}


int waitIpPacket(char *pBuffer, int timeout){
	MessageBlock mb(MAX_BUFFER_SIZE);
	commonHead *myhead = NULL;
	replyHead *pRepHead = NULL;
	Client *pClient = Client::getInstance();

	Client *pInstance = Client::getInstance();

	if (pInstance->Recv(mb,timeout)){
		myhead = (commonHead *)mb.m_pData;
		if (myhead->type == MSG_TYPE_DATA){
			memcpy(pBuffer,mb.m_pData + 4 + 14 + 20 ,mb.m_length -  4 - 14 - 20);
			return mb.m_length -  4 - 14 - 20;			
		}
		else{
			pRepHead = (replyHead *)mb.m_pData;
			pClient->sendMsgToUI(6,1,pRepHead->result);
			return -1;
		}
	}
	else{
		return -1;
	}
}

UINT32 getServerIpv4Address(){
	return test_server_logic_ip;
}

// some public function
char *getSubStr(const char *str,int beg,int len){
	int totallen = strlen(str) - 1;
	char *retStr = NULL;

	if (beg > totallen){
		return NULL;
	}

	if (beg + len >= totallen){
		// get the substr to tail
		retStr = (char *)malloc(totallen - beg + 1);
		strncpy(retStr,(str + beg),totallen - beg);
		retStr[totallen - len] = '\0';
	}
	else{
		retStr = (char *)malloc(len + 1);
		strncpy(retStr,(str + beg),len);
		retStr[len] = '\0';
	}

	return retStr;
}

// lTrim function
char *lTrim(char * mystr){
	int i = 0;
	while (*(mystr + i) == ' '){
		i ++;
	}

	return (mystr + i);
}

// rTrim function
char *rTrim(char *mystr){
	int i = strlen(mystr) - 1;
	while (*(mystr + i) == ' '){
		i --;
	}

	*(mystr + i + 1) = '\0';

	return mystr;
}

// trim function
char * Trim(char * mystr){
	// delete the '\n'
	char *retStr = "";

	if (*(mystr + strlen(mystr) - 1) == '\n')
	{
		*(mystr + strlen(mystr) -1 ) = '\0';
	}
	retStr = lTrim(mystr);
	retStr = rTrim(retStr);

	return retStr;

}

// covert the hex string to buffer
UINT16 getBufferFromHexString(char *hexStr,char *pBuffer){
	int len = strlen(hexStr);
	int i = 0;
	int pos = 0;

	while(i < len)
	{
		sscanf(hexStr + i,"%2x",pBuffer+pos);
		i += 2;
		while (hexStr[i] == ' '){
			i ++;
		}
		pos ++;
	}
	return pos;
}


void rip_sendIpPkt(unsigned char* pData, unsigned short len,unsigned short dstPort, UINT8 iNo)
{

			char outBuffer[MAX_BUFFER_SIZE] = {0};	
			char *tempBuffer;
			int buffsize = 0; 
			unsigned char ttl;
			unsigned short srcPort;			
			unsigned int srcAddr;
			unsigned int dstAddr;

			if((iNo != 1)&&(iNo != 2))
			{
				printf("invalid interface No.,send rip packet fail!\n");
				return;
			}

			if(len >124)
			{
				printf("invalid packet length,send rip packet fail!\n");
				return;
			}
			udpHead *pHead3 =(udpHead *)outBuffer;
			RipPacket *pHead4 = (RipPacket *)pHead3->pData;

			ttl = 255;
			srcPort =520;			
			

			pHead3->srcPort = htons(srcPort);
			pHead3->destPort = htons(dstPort);
			pHead3->len = htons(8 + len);
			pHead3->checksum = 0;
			buffsize += 8;
			tempBuffer = outBuffer + buffsize;
			memcpy(tempBuffer,pData,len);
			buffsize += len;
			if((pHead4->rip_cmd == 2)&&(g_rip_packet_type == 1))
			{
				if(iNo == 1)
				{
					srcAddr = 0x0a000001;
					dstAddr = 0x0a000002;
				}
				else if(iNo == 2)
				{
					srcAddr = 0x14000001;
					dstAddr = 0x14000002;
				}
			}
			else
			{
				if(iNo == 1)
				{
					srcAddr = 0x0a000001;
					dstAddr = 0xe0000009;
				}
				else if(iNo == 2)
				{
					srcAddr = 0x14000001;
					dstAddr = 0xe0000009;
				}

			}			
			SendIpPacket((unsigned char *)outBuffer,buffsize,srcAddr,dstAddr,ttl,PROTOCOL_UDP);
			Sleep(2);
	
}
//added by xiaojun for arp test
void sys_Arp_ClearcacheTable()
{
	while(g_arp_cache_table != NULL)
	{
		struct stud_arp_cache_node * tem = g_arp_cache_table;

		g_arp_cache_table = g_arp_cache_table->next;

		free(tem);
	}
}

void sys_Arp_Addcache(UINT32 dest, UINT8 macaddr[6], UINT16 elementAge, UINT8 maxRetries)
{
	struct stud_arp_cache_node *tmp = (struct stud_arp_cache_node *)malloc(sizeof(struct stud_arp_cache_node));

	if(tmp == NULL)
		return;
	memcpy(tmp->dest_ethernet_macadd,macaddr,6);
	tmp->dest_ip_address= dest;
	tmp->elementAge = elementAge;
	tmp->maxRetries = maxRetries;
	tmp->next = g_arp_cache_table;

	g_arp_cache_table = tmp;
}



void sys_Rip_ClearRouteTable()
{
	while(g_rip_route_table != NULL)
	{
		struct stud_rip_route_node * tmp = g_rip_route_table;

		g_rip_route_table = g_rip_route_table->next;

		free(tmp);
	}
/*remove by wangyl for delete variable g_rip_route_table_count
	g_rip_route_table_count = 0;
*/
}




void sys_Rip_AddRoute(unsigned int dest, unsigned int mask, unsigned int nexthop, unsigned int metric, unsigned int if_no)
{
	struct stud_rip_route_node * tmp = (struct stud_rip_route_node *)malloc(sizeof(struct stud_rip_route_node));

	if(tmp == NULL)
		return;
	tmp->dest = dest;
	tmp->mask = mask;
	tmp->nexthop = nexthop;
	tmp->metric = metric;
	tmp->if_no = if_no;
	tmp->next = g_rip_route_table;

	g_rip_route_table = tmp;
/*remove by wangyl for delete variable g_rip_route_table_count
	g_rip_route_table_count++;
*/
}

void sys_Rip_UpdateRoute(unsigned int dest, unsigned int mask, unsigned int nexthop, unsigned int metric, unsigned int if_no)
{
	struct stud_rip_route_node * tmp = g_rip_route_table;

	while(tmp != NULL)
	{
		if((tmp->dest == dest) && (tmp->mask == mask))
		{
			tmp->nexthop = nexthop;
			tmp->metric = metric;
			tmp->if_no = if_no;
			return;
		}

		tmp = tmp->next;
	}
}

void sys_Rip_DelRoute(unsigned int dest, unsigned int mask)
{
	struct stud_rip_route_node * tmp = g_rip_route_table;
	struct stud_rip_route_node * pre = NULL;

	while(tmp != NULL)
	{
		if((tmp->dest == dest) && (tmp->mask == mask))
		{
			if(pre != NULL)
			{
				pre->next = tmp->next;
			}
			else
			{
				g_rip_route_table = tmp->next;
			}

			free(tmp);
/*remove by wangyl for delete variable g_rip_route_table_count
			g_rip_route_table_count--;
*/

			return;
		}

		pre = tmp;
		tmp = tmp->next;
	}
}

void sys_Rip_Route_timeout(unsigned int dest, unsigned int mask)
{
	struct stud_rip_route_node * tmp = g_rip_route_table;
	struct stud_rip_route_node * pre = NULL;

	while(tmp != NULL)
	{
		if((tmp->dest == dest) && (tmp->mask == mask))
		{
			tmp->metric = 16;
			return;
		}

		pre = tmp;
		tmp = tmp->next;
	}
}
int sys_Rip_LookupRoute(unsigned int dest, unsigned int mask)
{
	struct stud_rip_route_node * tmp = g_rip_route_table;

	while(tmp != NULL)
	{
		if((tmp->dest == dest) && (tmp->mask == mask))
		{
			return 1;
		}

		tmp = tmp->next;
	}

	return 0;
}

int sys_Rip_GetRouteMetric(unsigned int dest, unsigned int mask)
{
	struct stud_rip_route_node * tmp = g_rip_route_table;

	while(tmp != NULL)
	{
		if((tmp->dest == dest) && (tmp->mask == mask))
		{
			return tmp->metric;
		}

		tmp = tmp->next;
	}

	return 0;
}

void SendFRAMEPacket(unsigned char* pData, unsigned int len){
			char outBuffer[MAX_BUFFER_SIZE] = {0};			
			int buffsize = 0;
			int i;
			unsigned char *printbuf;

			printbuf = pData;			
			if((len < 0)||(len > 40))
			{
				printf("invalid packet length");
				return;
			}
			// Ethernet frame 
			sys_frame_send *pHead1 = (sys_frame_send *)outBuffer;
			pHead1->flag = htonl(0xffffffff);
			
			memcpy(outBuffer+sizeof(sys_frame_send),pData,len);		

			pHead1->len = htonl(len);			
			buffsize = len + sizeof(sys_frame_send);			

			//printf("buffersize================%d\n",buffsize);
			MessageBlock mb(outBuffer,buffsize);

			Client *pInstance = Client::getInstance();

			pInstance->Send(mb);

			//printf("send out a frame\n");
			
}
//added by xiaojun for arp test
void SendARPPacket(unsigned char* pData, unsigned int len){
			char outBuffer[MAX_BUFFER_SIZE] = {0};			
			unsigned int buffsize = 0;
			int i;
			unsigned char *printbuf;

			printbuf = pData;			
			/*if((len < 0)||(len > 40))
			{
				printf("invalid packet length");
				return;
			}*/
			memcpy(outBuffer,pData,len);	
			buffsize = len;
            
			 arp_or_rarp_pkt *precvarp;
	         precvarp = (arp_or_rarp_pkt *)outBuffer;
			//printf("buffersize================%d\n",buffsize);
			 //printf("precvarp->kind==========%d\n",precvarp->kind);
			MessageBlock mb(outBuffer,buffsize);

			Client *pInstance = Client::getInstance();

			pInstance->Send(mb);

			//printf("send out a frame\n");
			
}

void bgp_FsmSendTcpData(char *pBuf,DWORD dwLen){

	
	UINT16 srcPort = DEFAULT_TCP_DST_PORT;
	UINT16 dstPort = DEFAULT_BGP_PORT;
	UINT32 seq = g_bgp_tcp_seqNo;
	UINT32 ack = g_bgp_tcp_ackNo;
	char flag = 16;	
	if  (dwLen >100)
	{
		printf("the length of data too long,send failed\n");
		return;
	}
	else if(dwLen< 0)
	{
		printf("the length of data too short,send failed\n");
		return;
	}
	clientTcpPacket(srcPort,dstPort,seq,ack,flag,pBuf,dwLen);
}

void clientSendTcpPacket(char flag,UINT32 seqNo, UINT32 ackNo){	
	UINT16 srcPort = DEFAULT_TCP_DST_PORT;
	UINT16 dstPort = DEFAULT_BGP_PORT;
	UINT32 seq = seqNo;
	UINT32 ack = ackNo;
	char cflag = flag;
	clientTcpPacket(srcPort,dstPort,seq,ack,cflag,NULL,0);
	
}
 void bgp_FsmTryToConnectPeer()
 {
	UINT16 srcPort = DEFAULT_TCP_DST_PORT;
	UINT16 dstPort = DEFAULT_BGP_PORT;
	UINT32 seq = g_bgp_tcp_seqNo;
	UINT32 ack = g_bgp_tcp_ackNo;
	char cflag = 2;
	clientTcpPacket(srcPort,dstPort,seq,ack,cflag,NULL,0);
 }
int clientTcpPacket(unsigned short srcPort, unsigned short destPort, unsigned
	int seqNum, unsigned int ackNum, unsigned char flag, char* pData,int len){
		char outBuffer[MAX_BUFFER_SIZE] = {0};
		char tempBuffer[MAX_BUFFER_SIZE] = {0};
		int buffsize = 0;

	// Ethernet frame 
	EthernetHead *pHead1 = (EthernetHead *)outBuffer;
	IPHead *pHead2 = (IPHead *)(outBuffer + sizeof(EthernetHead));
	tcpHead *pHead3 =(tcpHead *)pHead2->pData;

	memcpy(pHead1->destMac,test_client_mac,6);
	memcpy(pHead1->srcMac,test_server_mac,6);
	pHead1->protocol = htons(0x800);

	buffsize += sizeof(EthernetHead);

	pHead2->ver = 0x45;
	pHead2->tos = 0;
	pHead2->totalLen = htons(20 + 20 + len);
	pHead2->ignore = 0;
	pHead2->ttl = 64;
	pHead2->protocol = PROTOCOL_TCP;
	pHead2->srcAddr = htonl(test_client_logic_ip);
	pHead2->destAddr = htonl(test_server_logic_ip);
	pHead2->checksum = htons(IpCalcChecksum((char *)pHead2,10));

	buffsize += 20;

	pHead3->srcPort = htons(srcPort);
	pHead3->destPort = htons(destPort);
	pHead3->seqNo = htonl(seqNum);
	pHead3->ackNo = htonl(ackNum);
	pHead3->headLen = 0x50;
	pHead3->flag = flag;
	pHead3->windowsize = htons(1000);
	pHead3->checksum = 0;
	pHead3->urgentPointer = 0;

	buffsize += 20;

	if  (pData != NULL){
		memcpy(pHead3->pData,pData,len);
	}

	buffsize += len;


	// calculate the tcp checksum
	// construct the head
	memcpy(tempBuffer,&pHead2->srcAddr,8);
	tempBuffer[8] = 0;
	tempBuffer[9] = 6;
	int tempLen = 0;

	if (len % 2 != 0){
		tempLen = len + 1;
		//buffsize ++;
	}
	else{
		tempLen = len;
	}
	
	*(UINT16 *)(&tempBuffer[10]) = htons(20 + len);
	memcpy(tempBuffer + 12, pHead3, 20 + tempLen);
	
	pHead3->checksum = htons(tcpCalcChecksum(tempBuffer,(12 + 20 + tempLen)/2));

	MessageBlock mb(outBuffer,buffsize);

	Client *pInstance = Client::getInstance();

	pInstance->Send(mb);

	//for seq ack maintain
	if(len >0)
	{
		g_bgp_tcp_seqNo +=len;
	}
	else
	{
		g_bgp_tcp_seqNo++;
	}
	return 0;
}

