#include "stdafx.h"
#include "common.h"
#include "scriptParser.h"
#include "scriptClass.h"
#include "Client.h"
#include "mydhcp.h"

int n;
extern UINT32 test_server_logic_ip;
extern UINT32 test_client_logic_ip;
extern UINT8  test_client_mac[6];
extern UINT8  test_server_mac[6];
extern UINT16 IpCalcChecksum(char *, UINT16);
// to support the ipv6 test
extern ipv6_addr test_server_logic_ipv6;
extern ipv6_addr test_client_logic_ipv6;
extern unsigned int g_rip_packet_type;

//for bgp fsm
extern struct BgpPeer  stPeer;
extern unsigned int g_bgp_tcp_seqNo;
extern unsigned int g_bgp_tcp_ackNo;

//add by wangyl for username auth
char acUsername[EXPE_STUDID_LEN];

extern void sys_Arp_ClearcacheTable();
extern void sys_Arp_Addcache(UINT32 dest, UINT8 macaddr[6], UINT16 elementAge, UINT8 maxRetries);

extern void sys_Rip_ClearRouteTable();
extern void sys_Rip_AddRoute(unsigned int dest, unsigned int mask, unsigned int nexthop, unsigned int metric, unsigned int if_no);


scriptParser::scriptParser(int itemIndex, int caseIndex,UINT8 parserType){
	testItemIndex = itemIndex;
	testCaseIndex = caseIndex;
	clientAddr = 0;
	serverAddr = 0;
	memset(clientMac,0,6);
	memset(serverMac,0,6);
	downloadScript = NULL;
	testResult = true;
	currentLine = 0;
	for (int i = 0; i < 5; i ++){
		mysocket[i] = 0;
	}
	//for bgp fsm
	g_bgp_tcp_seqNo = 1234;
	g_bgp_tcp_ackNo = 0;
	stPeer.bgp_bAuth = 0;
	stPeer.bgp_byState = 1;
	stPeer.bgp_dwCfgHoldtime = 180;
	stPeer.bgp_dwMyRouterID = 0x7f000001;
	stPeer.bgp_wMyAS = 2;
	stPeer.bVersion = 4;
	//end

	parserType = parserType;	
	if (parserType == PARSER_SERVER){
		// get the avail ip and mac
		mydhcp *pInstance = mydhcp::getInstance();
		IpAndMac *pItem = pInstance->getAvailItem();

		clientAddr = pItem->ip;
		memcpy(clientMac,pItem->macAddr,6);

		serverAddr = 0x0a000001;
		serverMac[0] = 0;
		serverMac[1] = 0x0d;
		memcpy(&serverMac[2],&serverAddr,4);
	}
	else
	{
		// must get the ip and mac from server
		getIpAndMacFromServer();
	}
}

// 析构函数
scriptParser::~scriptParser(){
	if (downloadScript != NULL){
		delete(downloadScript);
	}

	// free the ip address
	if (parserType == PARSER_SERVER){
		mydhcp *pInstance = mydhcp::getInstance();

		pInstance->insertAvailItem(clientAddr);
	}
}

	// 从服务器下载相应的脚本 only for client
	// if success : true
	// else :   false
bool scriptParser::downloadScriptFromServer(){
		//TODO:
		// 1. send the request
		// 2. wait for reply
		// 3. create the testcase from packet
		char pBuffer[MAX_BUFFER_SIZE] = {0};
		char outBuffer[MAX_BUFFER_SIZE] = {0};

		getScriptHead *pScript;

		// send MSG_SUBTYPE_GET_SCRIPT message
		pScript = (getScriptHead *)outBuffer;

		pScript->commonhead.msgLen = htons(sizeof(struct getScriptHead) - sizeof(UINT16));
		pScript->commonhead.type = MSG_TYPE_CONTROL;
		pScript->commonhead.subType = MSG_SUBTYPE_GET_SCRIPT;
		pScript->flag = MSG_REQUEST;
		pScript->itemIndex = testItemIndex;
		pScript->caseIndex = testCaseIndex;

		sendMsgToServer(outBuffer);
		
		if (!getInputPacket(pBuffer,10)) // timeout is 10 second, maybe we need more time to get input packet.
		{
			return false;
		}

		pScript = (getScriptHead *)pBuffer;
		if ((pScript->commonhead.subType == MSG_SUBTYPE_GET_SCRIPT)
			&& (pScript->flag == MSG_REPLY)){
				// accept script file

				if (downloadScript != NULL){
					delete downloadScript;
				}

				downloadScript = new testCase(pScript->data,PARSER_CLIENT);
			}

			return true;
	}


	// 从服务器下载相应的脚本 only for server
	// if success : true
	// else :   false
bool scriptParser::downloadScriptOnServer(){
		scriptClass *myInstance = scriptClass::getInstance();
		char buffer[MAX_BUFFER_SIZE] = {0};

		if (downloadScript != NULL){
			delete(downloadScript);
		}

		downloadScript = NULL;

		myInstance->getScript(testItemIndex,testCaseIndex,PARSER_SERVER,buffer);

		downloadScript = new testCase(buffer,PARSER_SERVER);

		testResult = true;
		currentLine = 0;
		for (int i = 0; i < 5; i ++){
			mysocket[i] = 0;
		}		
		return true;
	}

bool scriptParser::downloadListFromServer(){
		//TODO:
		// 1. send the request
		// 2. wait for reply
		// 3. create the list from packet and send to main UI
	char pBuffer[MAX_BUFFER_SIZE] = {0};
	char outBuffer[MAX_BUFFER_SIZE] = {0};

		getListHead *pList;

		// send MSG_SUBTYPE_GET_SCRIPT message
		pList = (getListHead *)outBuffer;

		pList->commonhead.msgLen = htons(sizeof(struct getListHead) - sizeof(UINT16));
		pList->commonhead.type = MSG_TYPE_CONTROL;
		pList->commonhead.subType = MSG_SUBTYPE_LIST;
		pList->flag = MSG_REQUEST;

		sendMsgToServer(outBuffer);

		getInputPacket(pBuffer,10); // timeout is 10 second, maybe we need more time to get input packet.

		pList = (getListHead *)pBuffer;
		if ((pList->commonhead.subType == MSG_SUBTYPE_LIST)
			&& (pList->flag == MSG_REPLY)){
				// accept list buffer
				// submit pList->data to main UI
				// TODO:

		}

		return true;
	}


	// 自动执行脚本, this function is used in client
bool scriptParser::autoRunScript(){
		scriptOneLine *pTemp;
		int scriptNum;
		bool ret = true;


		scriptNum = downloadScript->clientScriptNum;
		//printf("scriptNum==============%d\n",scriptNum);

		for (int i = 0; i < scriptNum; i ++){
				pTemp = downloadScript->clientScript[i];

			// run
			if (!runOneScript(pTemp)){
				//printf("i========================%d\n",i);
				ret = false;
				break;
			}
		}

		return ret;
	}


// 执行一条脚本语句,for client
//  return 0   error happen
//         1   success, can run next script
int scriptParser::runOneScript(scriptOneLine *pOneScript){
		scriptcommonDefine *myScript = scriptcommonDefine::getInstance();
		scriptClass *myInstance = scriptClass::getInstance();
		Client *pClient = Client::getInstance();
		Client *psClient = Client::getInstance();
		int myret = 1;
		UINT32 ifno = 0;
		char mt = 0;
		UINT32 destAdd = 0;
		UINT32  maskLen;
		int i;
		char pBuffer[MAX_BUFFER_SIZE] = {0};
		char outBuffer[MAX_BUFFER_SIZE] ={0};
		commonHead *pHead, *pTemp;
		replyHead *pRepHead;
		ripRouteHead *pRouteHead;
		arpcacheHead *pcacheHead;
		ripTimeoutHead *pTimeoutHead;
		arpcacheTimeoutHead *pcacheTimeoutHead;
		BGPTimeoutHead *pBgptimeoutHead;
		BGPExceptHead  *pBgpExceptHead;
		EthernetHead *pHead1;
		IPHead *pHead2;
		udpHead *pHead3;
		RipPacket *pHead4;
		tcpHead  *tcphead;
		struct ripRoutePrintHead *pRoutePrHead;
		UINT8 a[6] = {0x00,0xb4,0xc0,0x91,0xd2,0x91};
		UINT8 b[6] = {0x00,0xb4,0xc0,0x91,0xd2,0x92};
		UINT8 c[6] = {0x00,0xb4,0xc0,0x91,0xd2,0x93};
		UINT8 d[6] = {0x00,0xb4,0xc0,0x91,0xd2,0x94};
		UINT8 e[6] = {0x00,0xb4,0xc0,0x91,0xd2,0x95};
		
		getScriptHead *pScriptHead;
		getListHead *pListHead;
		struct stud_rip_route_node *pstrrn;
		struct stud_arp_cache_node *pstac;
		rip_route_table *strrtRig;
		int ret;
		unsigned char messageType;
		int len;

		if (pOneScript->op == myScript->getOperateTypeIndex("INPUT")){
			if (!getInputPacket(pBuffer,pOneScript->timeout)){
				myret = 0;
				//printf("Test interrupted, Please restart the test process!\n");
				return myret;
			}

			pHead = (commonHead *)pBuffer;

			// if packetType is reserved to system, deal with the packet by default
			switch (pHead->type){
				case MSG_TYPE_DATA:
					// deal with the packet
					// check the correctness
					if(pOneScript->packetType == myScript->getPacketTypeIndex("FRAME"))
					{
						//当前报文为FRAME
						printf("receive a frame\n");
					}
					else if (pOneScript->packetType == myScript->getPacketTypeIndex("IPv4"))
					{
						if (!checkPacket(pBuffer + sizeof(commonHead),
							pOneScript->para,       // parameters described in script
							pOneScript->srcAddr,    // source address 
							pOneScript->destAddr,   // destination address
							pOneScript->data
							))
						{
								myret = 0;
								testResult = false;
								break;
						}						
					}					
					else if (!checkIPv6Packet(pBuffer + sizeof(commonHead),
							pOneScript->para,       // parameters described in script
							pOneScript->srcAddr,    // source address 
							pOneScript->destAddr,   // destination address
							pOneScript->data
							))
							{
								myret = 0;
								testResult = false;
								break;
							}
					//如果是bgp fsm，则需要对tcp报文进行ack、seq进行累加
					if((pOneScript->packetType == myScript->getPacketTypeIndex("TCP"))&&(testItemIndex==CODE_BGP))
					{
						//printf("receive an bgp fsm packet\n");
						pHead2 = (IPHead *)(pBuffer + 4 + 14);
						tcphead = (tcpHead *)pHead2->pData;
						if ((tcphead->flag != 0x01) && (tcphead->flag != 0x02))
						{
							g_bgp_tcp_ackNo = ntohl(tcphead->seqNo)  + 1;
						}
						else
						{
							g_bgp_tcp_ackNo = ntohl(tcphead->seqNo) + ntohs(pHead2->totalLen) - 40;
						}
						//printf("current g_bgp_tcp_ackNo ====  %d\n",g_bgp_tcp_ackNo);
					}
					// submit the packet to user's function
					if (pOneScript->funcName > 0){
						// call the function by function pointer
						//printf("call function %d\n",pOneScript->funcName);
						//printf("pOneScript->funcName===============================%d\n",pOneScript->funcName);
						switch (pOneScript->funcName){
							case 1: //stud_ip_recv
								ret = (*myScript->Func_ip_recv)(pBuffer + sizeof(commonHead) + sizeof(EthernetHead),ntohs(pHead->msgLen) - 2 - sizeof(EthernetHead));

								break;

							case 5: //stud_fwd_deal
								ret = (*myScript->Func_fwd_deal)(pBuffer + sizeof(commonHead) + sizeof(EthernetHead),ntohs(pHead->msgLen) - 2 - sizeof(EthernetHead));
								break;

							case 6: //stud_tcp_recv
								{
								// we must ignore the ethereal frame head
								IPHead *ipHead = (IPHead *)(pBuffer + sizeof(commonHead) + 14);
								
								ret = (*myScript->Func_tcp_input)(
									ipHead->pData,
									ntohs(pHead->msgLen) - 2 - 14 - 20, // ignore ethereal frame and ip head
									ipHead->srcAddr,
									ipHead->destAddr
									);
								}
								if(ret != 0)
											psClient->sendMsgToUI(6,1,256);
								break;

							case 13: // stud_ipv6_recv
								ret = (*myScript->Func_ipv6_recv)(pBuffer + sizeof(commonHead) + sizeof(EthernetHead),ntohs(pHead->msgLen) - 2 - sizeof(EthernetHead));
								break;

							case 17: // stud_ipv6_fwd_deal
								ret = (*myScript->Func_ipv6_fwd_deal)(pBuffer + sizeof(commonHead) + sizeof(EthernetHead),ntohs(pHead->msgLen) - 2 - sizeof(EthernetHead));
								break;

							case 18: // stud_slide_window
								{														
								    ret = (*myScript->Func_slide_window)(pBuffer + sizeof(commonHead) + sizeof(sys_frame_send) ,ntohs(pHead->msgLen) - 2 - sizeof(sys_frame_send),MSG_TYPE_RECEIVE);
									
								}
								break;
							case 19:  //stud_rip_packet_recv
								//printf("stud_rip_packet_recv: recv a rip packet\n");
								pHead1 = (EthernetHead *)(pBuffer+4);
								pHead2 = (IPHead *)(pBuffer+4 + sizeof(EthernetHead));
								pHead3 =(udpHead *)pHead2->pData;
								pHead4 = (RipPacket *)pHead3->pData;
								//printf("rip packet cmd = %d version = %d mbz = %d \n",pHead4->rip_cmd,pHead4->rip_vers,pHead4->rip_mbz);
								if(pHead4->rip_cmd == 1)
									g_rip_packet_type = 1;
								n = rand();
								n = n % 2;
								switch(n)
								{
								case 0:
									   ifno = 1;
									   break;
								case 1: 
									   ifno = 1;
									   break;
                                }	
								//printf("ifno = 555555555555555555555555555%d \n",ifno);
								ret = (*myScript->Func_rip_packet_recv)(pBuffer + sizeof(commonHead) + sizeof(EthernetHead) + 20 +8,ntohs(pHead->msgLen) - 2 - sizeof(EthernetHead)-20-8,ifno,ntohl(pHead2->srcAddr));
								break;
							case 21://stu_bgp_FsmEventOpen
								//printf("call back function stu_bgp_FsmEventOpen\n");
								len = ntohs(pHead->msgLen) - 2 - ETHER_HEAD_LEN - IP_HEAD_LEN - TCP_HEAD_LEN;
								ret = (*myScript->Func_bgp_FsmEventOpen)((BgpPeer *)&stPeer,(BYTE *)(pBuffer + sizeof(commonHead) + ETHER_HEAD_LEN + IP_HEAD_LEN + TCP_HEAD_LEN),len);
								break;
							case 22://stu_bgp_FsmEventKeepAlive
								//printf("call back function stu_bgp_FsmEventKeepAlive\n");
								len = ntohs(pHead->msgLen) - 2 - ETHER_HEAD_LEN - IP_HEAD_LEN - TCP_HEAD_LEN;
								ret = (*myScript->Func_bgp_FsmEventKeepAlive)((BgpPeer *)&stPeer,(BYTE *)(pBuffer + sizeof(commonHead) + ETHER_HEAD_LEN + IP_HEAD_LEN + TCP_HEAD_LEN),len);
								break;
							case 23://stu_bgp_FsmEventNotification
								//printf("call back function stu_bgp_FsmEventNotification\n");
								len = ntohs(pHead->msgLen) - 2 - ETHER_HEAD_LEN - IP_HEAD_LEN - TCP_HEAD_LEN;
								ret = (*myScript->Func_bgp_FsmEventNotification)((BgpPeer *)&stPeer,(BYTE *)(pBuffer + sizeof(commonHead) + ETHER_HEAD_LEN + IP_HEAD_LEN + TCP_HEAD_LEN),len);
								break;
							case 24://stu_bgp_FsmEventUpdate
								//printf("call back function stu_bgp_FsmEventUpdate\n");
								len = ntohs(pHead->msgLen) - 2 - ETHER_HEAD_LEN - IP_HEAD_LEN - TCP_HEAD_LEN;
								ret = (*myScript->Func_bgp_FsmEventUpdate)((BgpPeer *)&stPeer,(BYTE *)(pBuffer + sizeof(commonHead) + ETHER_HEAD_LEN + IP_HEAD_LEN + TCP_HEAD_LEN),len);
								break;							
							case 28://stu_bgp_FsmEventStop
								//printf("call back function stu_bgp_FsmEventStop\n");
								ret = (*myScript->Func_bgp_FsmEventStop)((BgpPeer *)&stPeer);
								break;
							//added by xiaojun for arp protocol
							
							case 30://stud_arp_recv
                                //printf("9999999999999999999999999\n");
                                ret = (*myScript->Func_arp_recv)(pBuffer + sizeof(commonHead),ntohs(pHead->msgLen));
							    break;

                            case 32://stud_rarp_recv_rep
                                //printf("666666666666666666666666\n");
                                ret = (*myScript->Func_rarp_recv_rep)(pBuffer + sizeof(commonHead),ntohs(pHead->msgLen));
							    break;

							default:
								ret = -1;
								break;

						}
						
					}
					break;

				case MSG_TYPE_CONTROL:
					// follow the control type to apply the corresponding policy
					switch (pHead->subType)
					{
					case MSG_SUBTYPE_REQ:
						// run the next script
						break;

					case MSG_SUBTYPE_REP:
						// send some message to main UI
						pRepHead = (replyHead *)pBuffer;
						/*
						if (ntohs(pRepHead->result) == TEST_FINISH_FAIL){
						*/
							testResult = false;
							myret = false;
							/*
						}
						else{
							testResult = true;
							myret = true;
							// return the result to main UI
						}
						*/
						pClient->sendMsgToUI(testItemIndex,testCaseIndex,pRepHead->result);
						
						break;

						// only for server
					case MSG_SUBTYPE_LIST:
						// client get the information

						break;

					case MSG_SUBTYPE_ROUTEADD:
						// call the routeAdd function
						{
							UINT8 *pData;
							pData = (pBuffer + sizeof(commonHead));

							(*myScript->Func_Route_Init)();

							while (pData < pBuffer + ntohs(pHead->msgLen)){
								(*myScript->Func_route_add)((stud_route_msg *)pData);

								// insert the route table into memory including mac address
								pData += sizeof(stud_route_msg);

								if ((pData + sizeof(stud_route_msg)) > (pBuffer + ntohs(pHead->msgLen))){
									break;
								}
							}

						}
						break;

					case MSG_SUBTYPE_ROUTEADDIPV6:
						// call the routeAdd function
						{
							UINT8 *pData;
							stud_ipv6_route_msg temp;

							memset(&temp,0,sizeof(temp));

							// create the ipv6 address
							// 2001:da8:bf: + ipv4 address
							temp.dest.bAddr[0] = 0x07;
							temp.dest.bAddr[1] = 0xd1;
							temp.dest.bAddr[2] = 0x0d;
							temp.dest.bAddr[3] = 0xa8;
							temp.dest.bAddr[4] = 0x00;
							temp.dest.bAddr[5] = 0xbf;

							temp.nexthop.bAddr[0] = 0x07;
							temp.nexthop.bAddr[1] = 0xd1;
							temp.nexthop.bAddr[2] = 0x0d;
							temp.nexthop.bAddr[3] = 0xa8;
							temp.nexthop.bAddr[4] = 0x00;
							temp.nexthop.bAddr[5] = 0xbf;

							pData = (pBuffer + sizeof(commonHead));

							(*myScript->Func_ipv6_Route_Init)();

							while (pData < pBuffer + ntohs(pHead->msgLen)){
								// construct the ipv6 address
								*(UINT32 *)(&temp.dest.bAddr[12]) = ((stud_route_msg *)pData)->dest;
								*(UINT32 *)(&temp.nexthop.bAddr[12]) = ((stud_route_msg *)pData)->nexthop;
								temp.masklen = 96 + ntohl(((stud_route_msg *)pData)->masklen);

								(*myScript->Func_ipv6_route_add)(&temp);

								// insert the route table into memory including mac address
								pData += sizeof(stud_route_msg);

								if ((pData + sizeof(stud_route_msg)) > (pBuffer + ntohs(pHead->msgLen))){
									break;
								}
							}

						}
						break;
					case MSG_CACHE_TIMEOUT:
                        pcacheTimeoutHead = (arpcacheTimeoutHead *)pBuffer;
						//printf("141414141414\n");
                        //printf("destipaddr==================%x\n",ntohl(pcacheTimeoutHead->destipaddr));
						(*myScript->Func_arp_cache_timeout)(ntohl(pcacheTimeoutHead->destipaddr),MSG_CACHE_TIMEOUT);
						
                         

					case MSG_SUBTYPE_RIPTIMEOUT:
						//operation user timeout process
						pTimeoutHead = (ripTimeoutHead *)pBuffer;				 		
						mt = pTimeoutHead->MsgType;
                        destAdd = pTimeoutHead->destAddr;
						maskLen = pTimeoutHead->masklen;

						if(mt == RIP_MSG_DELE_ROUTE)
						{	 

							//added by xiaojun
								  (*myScript->Func_rip_route_timeout)(destAdd,maskLen,mt);
						}
						else if(mt == RIP_MSG_SEND_ROUTE)
						{							
							(*myScript->Func_rip_route_timeout)(0x0,0x00000000,mt);
						}						
						break;					
					case MSG_SUBTYPE_RIPROUTEINIT:
						//inital rip route table and interface table
						sys_Rip_ClearRouteTable();
						sys_Rip_AddRoute(0x0a000000, 0xffffff00, 0x0, 1, 1);
						sys_Rip_AddRoute(0x14000000, 0xffffff00, 0x0, 1, 2);
						sys_Rip_AddRoute(0x1e000000, 0xffffff00, 0x14000002, 5, 2);
						sys_Rip_AddRoute(0x28000000, 0xffffff00, 0x0a000002, 6, 1);
						sys_Rip_AddRoute(0x32000000, 0xffffff00, 0x14000002, 2, 2);
						sys_Rip_AddRoute(0x28000000, 0xff000000, 0x0a000002, 2, 1);
						//printf("rip initialization success!\n");
						break;
					case MSG_SUBTYPE_RIPROUTEPRINT:
						pRoutePrHead = (ripRoutePrintHead *)pBuffer;
						unsigned int len;
						if(pRoutePrHead->pflag == 1)
						{
							strrtRig = (struct rip_route_table *)(pBuffer+sizeof(ripRoutePrintHead)-1);
							len = ntohl(pRoutePrHead->iCount);
							printf("right routing table is :\n");							
							if((len >10)&&(len == 0))
								break;
							for(i = 0; i< len;i++)
							{							
								printf("dest:%ld.%ld.%ld.%ld ",(ntohl(strrtRig->dest)>>24)&0xFF,(ntohl(strrtRig->dest)>>16)&0xFF,(ntohl(strrtRig->dest)>>8)&0xFF,ntohl(strrtRig->dest)&0xFF);
								printf("mask:%ld.%ld.%ld.%ld ",(ntohl(strrtRig->mask)>>24)&0xFF,(ntohl(strrtRig->mask)>>16)&0xFF,(ntohl(strrtRig->mask)>>8)&0xFF,ntohl(strrtRig->mask)&0xFF);
								printf("nexthop:%ld.%ld.%ld.%ld ",(ntohl(strrtRig->nexthop)>>24)&0xFF,(ntohl(strrtRig->nexthop)>>16)&0xFF,(ntohl(strrtRig->nexthop)>>8)&0xFF,ntohl(strrtRig->nexthop)&0xFF);
								printf("metric: %d ",ntohl(strrtRig->metric));
								printf("ifno: %d\n",ntohl(strrtRig->if_no));
								strrtRig += 1;
								
							}

							printf("your routing table is : \n");
							pstrrn = g_rip_route_table;	
							while(pstrrn != NULL)
							{
								printf("dest:%ld.%ld.%ld.%ld ",(pstrrn->dest>>24)&0xFF,(pstrrn->dest>>16)&0xFF,(pstrrn->dest>>8)&0xFF,pstrrn->dest&0xFF);
								printf("mask:%ld.%ld.%ld.%ld ",(pstrrn->mask>>24)&0xFF,(pstrrn->mask>>16)&0xFF,(pstrrn->mask>>8)&0xFF,pstrrn->mask&0xFF);
								printf("nexthop:%ld.%ld.%ld.%ld ",(pstrrn->nexthop>>24)&0xFF,(pstrrn->nexthop>>16)&0xFF,(pstrrn->nexthop>>8)&0xFF,pstrrn->nexthop&0xFF);
								printf("metric: %d ",pstrrn->metric);
								printf("ifno: %d \n",pstrrn->if_no);
								pstrrn = pstrrn->next;
							}
						}						
						break;
                    			case MSG_SUBTYPE_ARPCACHEINIT:					
						sys_Arp_ClearcacheTable();
						sys_Arp_Addcache(0x0a000000,a, 600, 5);
						sys_Arp_Addcache(0x14000000,b, 600, 5);
						sys_Arp_Addcache(0x1e000000,c, 600, 5);
						sys_Arp_Addcache(0x28000000,d, 600, 5);
						sys_Arp_Addcache(0x32000000,e, 600, 5);
						//printf("arpcache initialization success!\n");
						break;
					case MSG_SUBTYPE_BGPTIMEOUT:
						//printf("reveive MSG_SUBTYPE_BGPTIMEOUT===============\n");
						pBgptimeoutHead = (BGPTimeoutHead *)pBuffer;
						mt = pBgptimeoutHead->MsgType;
						if (pOneScript->funcName > 0)
						{
							switch (pOneScript->funcName)
							{
								case 26:
									ret = (*myScript->Func_bgp_FsmEventTimerProcess)((BgpPeer *)&stPeer,mt);
									break;
								default:
									break;
							}
						}
						break;
					case MSG_SUBTYPE_BGPEXCEPT:
						//printf("reveive MSG_SUBTYPE_BGPEXCEPT===============\n");
						pBgpExceptHead = (BGPExceptHead *)pBuffer;
						mt = pBgpExceptHead->MsgType;
						if (pOneScript->funcName > 0)
						{
							switch (pOneScript->funcName)
							{
								case 25:
									ret = (*myScript->Func_bgp_FsmEventTcpException)((BgpPeer *)&stPeer,mt);
									break;
								default:
									break;
							}
						}
						break;
					default:
						// error message send to main UI
						printf("wrong message type!\n");
					}

					break;

				default:
					break;

			}
		}
		else if (pOneScript->op == myScript->getOperateTypeIndex("OUTPUT")){
				if (pOneScript->timeout > 0){
						// task delay
						Sleep(pOneScript->timeout);
				}

				// send packet or message
				if (pOneScript->packetType == myScript->getPacketTypeIndex("IPv4")){
					// send ipv4 packet 
					if (pOneScript->funcName > 0){
						// must call user function
						switch(pOneScript->funcName){
							case 2: // stud_ip_Upsend
								(*myScript->Func_ip_Upsend)(
									pOneScript->data,
									((pOneScript->data == NULL)?0:strlen(pOneScript->data)),
									clientAddr,
									serverAddr,
									pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"protocol")],
									pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"ttl")]);
								break;

							default:
								break;
						}
					}
				}
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("IPv6"))
				{
						// send ipv6 packet
						// TODO:
						// send ipv4 packet 
						if (pOneScript->funcName > 0){
							// must call user function
							switch(pOneScript->funcName){
							case 14: // stud_ipv6_Upsend
								{
									(*myScript->Func_ipv6_Upsend)(
										pOneScript->data,
										((pOneScript->data == NULL)?0:strlen(pOneScript->data)),
										&test_client_logic_ipv6,
										&test_server_logic_ipv6,
										pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV6,"hopLimit")],
										pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV6,"nextHead")]
										);
									    //printf("hopLimit====%d\n",pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV6,"hopLimit")]);
								}
								break;

							default:
								break;
							}
						}
					}
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("TCP")){
							// send tcp packet
							if (pOneScript->funcName > 0){
								// must call user function
								switch(pOneScript->funcName){
										case 8: //stud_tcp_socket
											mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]] = 
												(*myScript->Func_tcp_socket)(AF_INET,SOCK_STREAM,IPPROTO_TCP);
											if (mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]] <= 0){
												//printf("stud_tcp_socket return error!\n");
												myret = 0;
											}
											/*printf("tcp_socket = %d  index = %d\n",
												mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]],
												pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]);*/
											break;

										case 9: //stud_tcp_connect
											commonHead *pScript;
											sockaddr_in myaddr;
											myaddr.sin_family = AF_INET;
											myaddr.sin_port = htons(pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"destPort")]);
											myaddr.sin_addr.s_addr = htonl(serverAddr);
											ret = (*myScript->Func_tcp_connect)(
												mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]],
												(sockaddr_in*)&myaddr, 
												sizeof(myaddr));

											if (ret != 0){
												myret = 0;
												testResult = false;
											}
											/*if(myret == 0)
											{
											  		pScript = (commonHead *)outBuffer;

													pScript->msgLen = htons(sizeof(struct commonHead) - sizeof(UINT16));
													pScript->type = MSG_TYPE_TIMEOUT;
													pScript->subType = 0;

													sendMsgToServer(outBuffer);
													myret = 1;
													return 0;
											
											}*/
											if(myret == 0)
											psClient->sendMsgToUI(6,1,256);
											break;

										case 7: 
											{
												UINT16 tempScrPort = DEFAULT_TCP_SRC_PORT, tempDestPort = DEFAULT_TCP_DST_PORT;
												UINT32 tempSrcAddr = clientAddr, tempDestAddr = serverAddr;

												if (pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"srcPort")] > 0){
													tempScrPort = pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"srcPort")];
												}

												if (pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"destPort")] > 0){
													tempDestPort = pOneScript->para[myScript->getParaIndex(PACKET_TYPE_IPV4,"destPort")];
												}

												if (pOneScript->srcAddr > 0)												{
													tempSrcAddr = pOneScript->srcAddr;
												}

												if (pOneScript->destAddr > 0){
													tempDestAddr = pOneScript->destAddr;
												}
											
												(*myScript->Func_tcp_output)(
													pOneScript->data,
													pOneScript->dataLen,
													pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")],
													tempScrPort,
													tempDestPort,
													tempSrcAddr,
													tempDestAddr);
											}
											break;

										case 10: //stud_tcp_send
											if (mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]] > 0){
												ret = (*myScript->Func_tcp_send)(
													mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]],
													(unsigned char *)pOneScript->data,
													pOneScript->dataLen,
													16);
												if (ret != 0){
													myret = 0;
													testResult = false;
												}
											}
											else{
												//cout << "socket init have not run" << endl;
											}
											break;

										case 11: //stud_tcp_recv
											// pData表示纯粹的数据指针，已剥离ip头和tcp头
											{
												unsigned char tempBuffer[1000] = {0};

												ret = (*myScript->Func_tcp_recv)(
													mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]],
													tempBuffer,
													1000,
													MSG_DONTROUTE);

												if (memcmp(tempBuffer,pOneScript->data,pOneScript->dataLen) != 0){
													myret = 0;
													//printf("accept a wrong tcp data!\n");
												}
											}
											break;

										case 12:
											ret = (*myScript->Func_tcp_close)(mysocket[pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"socket")]]);
											if (ret != 0)
											{
												myret = 0;
												testResult = false;
											}
											//if(myret == 0)
											//psClient->sendMsgToUI(6,1,256);
											break;
										case 27://stud_bgp_FsmEventStart
											//printf("call back function stu_bgp_FsmEventStart\n");
											ret = (*myScript->Func_bgp_FsmEventStart)(&stPeer);
											break;
										case 29://stu_bgp_FsmEventConnect
											//printf("call back function stu_bgp_FsmEventConnect\n");
											ret = (*myScript->Func_bgp_FsmEventConnect)(&stPeer);
											break;
                    					default:
											break;
								}
							}
							else if((testItemIndex == CODE_BGP)&&(testCaseIndex == 0))
							{//需要系统自动完成tcp报文的发送,而不是调用学生接口
								char flag;								
								if(pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")]>0)
								{
									flag = pOneScript->para[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")];
								}

								clientSendTcpPacket(flag,g_bgp_tcp_seqNo,g_bgp_tcp_ackNo);
								//printf("system send out tcp packet\n");
							}
						}
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("FRAME")){  
							// stud_slide_window							
								char tempBuffer[1000];
								UINT32 tempSeqno;
								memset(tempBuffer,0x00,1000);
								sys_frame *pFrameHead = (sys_frame *)tempBuffer;
								if (pOneScript->para[myScript->getParaIndex(PACKET_TYPE_FRAME,"timeout")] == 1)
								{
									//printf("33333333333333333333333333333\n");
									tempSeqno = pOneScript->para[myScript->getParaIndex(PACKET_TYPE_FRAME,"seqno")];
									//printf("a frame timeout\n");
									//printf("frame seq ======================%d\n",tempSeqno);
									ret = (*myScript->Func_slide_window)((char *)&tempSeqno,sizeof(UINT32),MSG_TYPE_TIMEOUT);
									
								}
								else
								{
									//printf("222222222222222222222222222222222222222222\n");
									pFrameHead->kind = sysdata;
									pFrameHead->ack = htonl(pOneScript->para[myScript->getParaIndex(PACKET_TYPE_FRAME,"ackno")]);									
									pFrameHead->seq = htonl(pOneScript->para[myScript->getParaIndex(PACKET_TYPE_FRAME,"seqno")]);
									tempSeqno = pOneScript->para[myScript->getParaIndex(PACKET_TYPE_FRAME,"seqno")];
                                    printf("frame seq ======================%d\n",tempSeqno);
              						ret = (*myScript->Func_slide_window)(tempBuffer,20,MSG_TYPE_SEND);	
								
								}

								
							}
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("REQ")){
									pTemp = (commonHead *)outBuffer;
									pTemp->msgLen = htons(sizeof(struct commonHead) - 2);
									pTemp->type = MSG_TYPE_CONTROL;
									pTemp->subType = MSG_SUBTYPE_REQ;

									sendMsgToServer(outBuffer);
								}
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("REP")){
										pRepHead = (replyHead *)outBuffer;
										pRepHead->commonhead.msgLen = htons(sizeof(struct replyHead) - 2);
										pRepHead->commonhead.type = MSG_TYPE_CONTROL;
										pRepHead->commonhead.subType = MSG_SUBTYPE_REP;
										pRepHead->result = htons(MSG_REPLY_SUCCESS);

										sendMsgToServer(outBuffer);
									}
				//added by xiaojun for arp test
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("ARPCACHESEND")){
					                        UINT8 iCount;
                                            unsigned char length;
                                            struct arp_cache_table strcah[10];
											pstac = g_arp_cache_table;

                                            iCount = 0;

											//printf("7777777777777777777\n");
											while(pstac != NULL)
											{
												//if(pstac->elementAge == 0)
												//break;
												//printf("787878787878\n");
											  strcah[iCount].dest_ip_address = htonl(pstac->dest_ip_address);
                                              memcpy(strcah[iCount].dest_ethernet_macadd,pstac->dest_ethernet_macadd,6);
											  strcah[iCount].elementAge =  htons(pstac->elementAge);
											  strcah[iCount].maxRetries = pstac->maxRetries;
											 
                                              
											  iCount++;
											  pstac = pstac->next;
											}
                                            //printf("909090909090\n");
											  pcacheHead = (arpcacheHead *)outBuffer;
											if((iCount < 10) && (iCount>0))
											{
												//printf("iCount==========%d\n",iCount);
												memcpy(pcacheHead->data,strcah,(sizeof(struct arp_cache_table)*iCount));
												pcacheHead->Count = iCount;
												pcacheHead->commonhead.msgLen = htons(sizeof(struct arpcacheHead)+(sizeof(struct arp_cache_table)*iCount)-2-4);
											}
											else if(iCount == 0)
											{	
												//printf("22222222222222\n");
												pcacheHead->Count = 0;
												pcacheHead->commonhead.msgLen = htons(sizeof(struct arpcacheHead)-2-4);
											}
							                 //printf("55858585858585885\n");
											pcacheHead->commonhead.type = MSG_TYPE_CONTROL;
											pcacheHead->commonhead.subType = MSG_SUBTYPE_ARPCACHESEND;
											sendMsgToServer(outBuffer);
                           


                                            }
				//add end
				else if (pOneScript->packetType == myScript->getPacketTypeIndex("RIPROUTESEND")){
											unsigned char routeCount;
											unsigned char length;
											char *temp;
											struct rip_route_table strrt[10];
											int k=0;
											UINT32 destIPADDR=0;
											
											
											pstrrn = g_rip_route_table;	
											routeCount = 0;		
							/*remove by wangyl for delete variable g_rip_route_table_count
											if(g_rip_route_table_count >=10)
											{
												myret = 0;
												testResult = false;
												return myret;
											}
							*/
											while(pstrrn != NULL)
											{
												strrt[routeCount].dest = htonl(pstrrn->dest);
												//printf("pstrrn->dest================%x\n",pstrrn->dest);
												strrt[routeCount].if_no = htonl(pstrrn->if_no);
												strrt[routeCount].mask = htonl(pstrrn->mask);
												strrt[routeCount].metric = htonl(pstrrn->metric);
												//printf("pstrrn->metric=======%d\n",pstrrn->metric);
												if(pstrrn->metric == 16)
												{
												    destIPADDR = pstrrn->dest;
													//printf("pstrrn->dest3333333333333333======================%x\n",pstrrn->dest);
													//printf("destIPADDR=================%x\n",destIPADDR);
												}
												strrt[routeCount].nexthop = htonl(pstrrn->nexthop);
												routeCount++;
												pstrrn = pstrrn->next;
											}
											//add by wangyl for delete variable g_rip_route_taqble_count
											if(routeCount >= 10)
											{
												myret = 0;
												testResult = false;
												return myret;
											}
											//add end
											pRouteHead = (ripRouteHead *)outBuffer;
											if(routeCount < 10)
											{
												memcpy(pRouteHead->data,strrt,(sizeof(struct rip_route_table)*routeCount));
												pRouteHead->iCount = htonl(routeCount);
												pRouteHead->commonhead.msgLen = htons(sizeof(struct ripRouteHead)+(sizeof(struct rip_route_table)*routeCount)-2-4);
											}
											else
											{												
												pRouteHead->iCount = 0;
												pRouteHead->commonhead.msgLen = htons(sizeof(struct ripRouteHead)-2-4);
											}
											pRouteHead->destaddress = destIPADDR;
											pRouteHead->commonhead.type = MSG_TYPE_CONTROL;
											pRouteHead->commonhead.subType = MSG_SUBTYPE_RIPROUTESEND;
											//printf("pRouteHead->destaddress================%x\n",pRouteHead->destaddress);
											sendMsgToServer(outBuffer);
                           
                                             
				}
				else if(pOneScript->packetType == myScript->getPacketTypeIndex("BGPSTATUE")){
					//printf("client system sent out bgp statue\n");
					pRepHead = (replyHead *)outBuffer;
					pRepHead->commonhead.msgLen = htons(sizeof(struct replyHead) - 2);
					pRepHead->commonhead.type = MSG_TYPE_CONTROL;
					pRepHead->commonhead.subType = MSG_SUBTYPE_BGPSTATUE;
					if(&stPeer !=NULL)
						pRepHead->result = htons(stPeer.bgp_byState);
					else
						pRepHead->result = htons(10);
					//printf("student bgp statue is ====================%d\n",htons(pRepHead->result));
					sendMsgToServer(outBuffer);
				}
				else{
					//printf("unkonw type packet type index\n");
				}					
		}
		else{
			//printf("Unknow keyword index\n");
		}
		// return the success or fail
		return myret;
	}

	// get the input packet，
	// return false: timeout happened
	// return true:  get the packet
	bool scriptParser::getInputPacket(char *pBuffer, int val){
		Client *temp = Client::getInstance();
		MessageBlock mb(MAX_BUFFER_SIZE);

		if (temp->Recv(mb,val)){
			memcpy(pBuffer,mb.m_pData,mb.m_length);
			return true;
		}
		else
		{
			temp->sendMsgToUI(testItemIndex,testCaseIndex,256);
			return false;
		}
	}

	// check the tcp packet available
	// pData是从以太帧开始的
	bool scriptParser::checkTcpPacket(char *pBuffer,UINT16 pPara[],char *pData, int len){
		scriptcommonDefine *myScript = scriptcommonDefine::getInstance();
		IPHead *pIp = (IPHead *)(pBuffer + sizeof(EthernetHead));
		tcpHead *pTcp = (tcpHead *)pIp->pData;
		char tempBuffer[MAX_BUFFER_SIZE] = {0};

		// checksum 
		memcpy(tempBuffer,&pIp->srcAddr,8);
		tempBuffer[8] = 0;
		tempBuffer[9] = 6;
		memset(&tempBuffer[10],htons(20 + len),2);
		memcpy(tempBuffer + 12, pTcp, 20 + len);

		if (ntohs(pTcp->checksum) != IpCalcChecksum(tempBuffer,(12 + 20 + len)/2)){
			printf("tcp checksum error! checksum = %  recompute = %d\n",ntohs(pTcp->checksum),IpCalcChecksum(tempBuffer,(12 + 20 + len)/2));
			return false;
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"srcPort")] > 0){
			if (ntohs(pTcp->srcPort) != pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"srcPort")]){
				printf("tcp srcPort error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"destPort")] > 0){
			if (ntohs(pTcp->destPort) != pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"destPort")]){
				printf("tcp destPort error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"seqno")] > 0){
			if (ntohl(pTcp->seqNo) != pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"seqno")]){
				printf("tcp seqno error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"ackno")] > 0){
			if (ntohl(pTcp->ackNo) != pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"ackno")]){
				printf("tcp srcPort error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")] > 0){
			if (pTcp->flag != pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")]){
				printf("tcp flag error! accept = %d  need = %d\n",pTcp->flag,pPara[myScript->getParaIndex(PACKET_TYPE_TCP,"flag")]);
				return false;
			}
		}

		if (len > 0){
			if (strncmp(pData,pTcp->pData,len) != 0){
				printf("get a wrong tcp data!\n");
				return false;
			}
		}

		return true;
	}

	// check the packet available
	// pData是从以太帧开始的
	bool scriptParser::checkPacket(char *pData,UINT16 *pPara, UINT32 srcAddr, UINT32 destAddr, char *overload){
		scriptcommonDefine *myScript = scriptcommonDefine::getInstance();
		EthernetHead *pEth = (EthernetHead *)pData;
		IPHead *pIp = (IPHead *)(pData + sizeof(EthernetHead));
		UINT16 inputChecksum =  ntohs(pIp->checksum);

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"checksum")] > 0){
			pIp->checksum = 0;

			if (inputChecksum != IpCalcChecksum((char *)pIp,10)){
				// checksum error
				printf("check sum error! input checksum = %d  recompute checksum = %d\n",ntohs(pIp->checksum),IpCalcChecksum((char *)pIp,10));
				return false;
			}

			pIp->checksum = htons(inputChecksum);
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"version")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"version")] != ((pIp->ver & 0xF0) >> 4)){
				printf("version error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"headLen")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"headLen")] != (pIp->ver & 0x0F)){
				printf("packet head len error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"totalLen")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"totalLen")] != ntohs(pIp->totalLen)){
				printf("packet total len error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"ttl")] > 0){
			if ((pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"ttl")] & 0xFF) != pIp->ttl){
				printf("packet ttl error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"protocol")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"protocol")] != pIp->protocol){
				printf("packet protocol error!\n");
				return false;
			}
		}
		
		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"srcAddr")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"srcAddr")] != ntohl(srcAddr)){
				printf("packet srcAddr error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"destAddr")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"destAddr")] != ntohl(destAddr)){
				printf("packet destAddr error!\n");
				return false;
			}
		}

		if (overload != NULL){
			if(strncmp(pIp->pData,overload,strlen(overload)) != 0){
				printf("data error!\n");
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"destMac")] > 0){
			if (pEth->destMac[5] != pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"destMac")]){
				printf("next hop mac address error! dest mac = %d  should be %d\n",pEth->destMac[5],pPara[myScript->getParaIndex(PACKET_TYPE_IPV4,"destMac")]);
			}
		}
		return true;
	}


	// check the packet available
	// pData是从以太帧开始的
	bool scriptParser::checkIPv6Packet(char *pData,UINT16 *pPara, UINT32 srcAddr, UINT32 destAddr, char *overload){
		scriptcommonDefine *myScript = scriptcommonDefine::getInstance();
		EthernetHead *pEth = (EthernetHead *)pData;
		IPv6Head *pIp = (IPv6Head *)(pData + sizeof(EthernetHead));

		/*check ip address*/

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"version")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"version")] != (pIp->verTraffic >> 12)){
					return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"nextHead")] > 0){
			if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"nextHead")] != pIp->nextHead){
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"hopLimit")] > 0){
			if ((pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"hopLimit")]) != pIp->hopLimit){
				return false;
			}
		}

		if (pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"destMac")] > 0){
			if (pEth->destMac[5] != pPara[myScript->getParaIndex(PACKET_TYPE_IPV6,"destMac")]){
				return false;
			}
		}

		return true;
	}

	// send msg between client and server
	void scriptParser::sendMsgToServer(UINT8 *pBuffer){
			// send the msg to server
			Client *temp = Client::getInstance();
			MessageBlock mb(pBuffer);
			temp->Send(mb);	
	}

	void scriptParser::sendMsgToClient(UINT8 *pBuffer){
		// send the msg to server

	}

	bool scriptParser::getIpAndMacFromServer(){
		char pBuffer[MAX_BUFFER_SIZE] = {0};
		int tryTime = 5;
		int i = 0;
		IpAndMacMsg *pHead = (IpAndMacMsg *)pBuffer;
		IpAndMacMsgUI *pHeadUI = (IpAndMacMsgUI *)pBuffer;

		pHeadUI->commonhead.msgLen = htons(sizeof(struct IpAndMacMsgUI) - sizeof(UINT16));
		pHeadUI->commonhead.type = MSG_TYPE_CONTROL;
		pHeadUI->commonhead.subType = MSG_SUBTYPE_IP_MAC;
		pHeadUI->flag = MSG_REQUEST;
		
		memset(pHeadUI->usrID,0x00,EXPE_STUDID_LEN);
		memcpy(pHeadUI->usrID,acUsername,EXPE_STUDID_LEN);	
		
		sendMsgToServer(pBuffer);

		while (i < tryTime){
			memset(pBuffer,0,MAX_BUFFER_SIZE);
			// timeout is 10 second, maybe we need more time to get input packet.

			if (!getInputPacket(pBuffer,10)){
				return false;
			} 

			pHead = (IpAndMacMsg *)pBuffer;
			if ((pHead->commonhead.subType == MSG_SUBTYPE_IP_MAC)
				&&(pHead->flag == MSG_REPLY)){
					// accept back packet
					test_client_logic_ip = clientAddr = pHead->clientAddr;
					test_server_logic_ip = serverAddr = pHead->serverAddr;

					// create the ipv6 address
					// 2001:da8:bf: + ipv4 address
					test_server_logic_ipv6.bAddr[0] = 0x07;
					test_server_logic_ipv6.bAddr[1] = 0xd1;
					test_server_logic_ipv6.bAddr[2] = 0x0d;
					test_server_logic_ipv6.bAddr[3] = 0xa8;
					test_server_logic_ipv6.bAddr[4] = 0x00;
					test_server_logic_ipv6.bAddr[5] = 0xbf;
					*(UINT32 *)(&test_server_logic_ipv6.bAddr[12]) = htonl(test_server_logic_ip);

					memcpy(&test_client_logic_ipv6.bAddr[0],&test_server_logic_ipv6.bAddr[0],16);

					*(UINT32 *)(&test_client_logic_ipv6.bAddr[12]) = htonl(test_client_logic_ip);

					memcpy(serverMac,pHead->serverMac,6);
					memcpy(clientMac,pHead->clientMac,6);

					memcpy(test_client_mac,clientMac,6);
					memcpy(test_server_mac,serverMac,6);
					break;
			}

			i++;

		}

		if (i < tryTime){
			return true;
		}
		else{
			return false;
		}
	}	

