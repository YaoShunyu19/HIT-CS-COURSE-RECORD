#include "common.h"
#include "scriptClass.h"
#include "scriptParser.h"
#include "Client.h"
#include "sysinclude.h"
extern char acUsername[EXPE_STUDID_LEN];

extern UINT16 test_Item;

extern UINT16 test_Case;

main(int argc, _TCHAR* argv[])
{
	scriptClass *myInstance = scriptClass::getInstance();
	scriptParser *pParser = NULL;
	Client *clientSender = Client::getInstance();
	scriptcommonDefine *myScript = scriptcommonDefine::getInstance();

	// initialize the socket
	if (clientSender->Init(argv[1]) != 1){
		clientSender->Close();
		//return false;
	}

	if (!clientSender->initUISocket()){
		printf("can not connect to ui\n");
	}

	// bind some function
	// for ip test
	myScript->regist_ip_recv(stud_ip_recv);

	myScript->regist_ip_upsend(stud_ip_Upsend);

	myScript->regist_route_init(stud_Route_Init);

	myScript->regist_route_add(stud_route_add);

	myScript->regist_fwd_deal(stud_fwd_deal);

	myScript->regist_tcp_input(stud_tcp_input);

	myScript->regist_tcp_output(stud_tcp_output);

	myScript->regist_tcp_socket(stud_tcp_socket);

	myScript->regist_tcp_connect(stud_tcp_connect);

	myScript->regist_tcp_send(stud_tcp_send);

	myScript->regist_tcp_recv(stud_tcp_recv);

	myScript->regist_tcp_close(stud_tcp_close);

	myScript->regist_ipv6_recv(stud_ipv6_recv);

	myScript->regist_ipv6_upsend(stud_ipv6_Upsend);

	myScript->regist_ipv6_route_init(stud_ipv6_Route_Init);

	myScript->regist_ipv6_route_add(stud_ipv6_route_add);

	myScript->regist_ipv6_fwd_deal(stud_ipv6_fwd_deal);
	
	myScript->regist_rip_packet_recv(stud_rip_packet_recv);

	myScript->regist_mobileip_icmp_send(stud_MN_icmp_send);//added by xiaojun for mobile ip

	myScript->regist_mobileip_icmp_recv(stud_MN_icmp_recv);

	myScript->regist_MN_send_Regi_req(stud_MN_send_Regi_req);

	myScript->regist_MN_recv_Regi_rep(stud_MN_recv_Regi_rep);

	myScript->regist_FA_recv_Regi_req(stud_FA_recv_Regi_req);

	myScript->regist_FA_recv_Regi_rep(stud_FA_recv_Regi_rep);

	myScript->regist_HA_recv_Regi_req(stud_HA_recv_Regi_req);

	myScript->regist_packertrans_HA_recv(stud_packertrans_HA_recv);

	myScript->regist_packertrans_FA_recv(stud_packertrans_FA_recv);

	myScript->regist_rip_route_timeout(stud_rip_route_timeout);
	
	myScript->regist_bgp_FsmEventConnect(stud_bgp_FsmEventConnect);

	myScript->regist_bgp_FsmEventKeepAlive(stud_bgp_FsmEventKeepAlive);

	myScript->regist_bgp_FsmEventNotification(stud_bgp_FsmEventNotification);

	myScript->regist_bgp_FsmEventOpen(stud_bgp_FsmEventOpen);

	myScript->regist_bgp_FsmEventStart(stud_bgp_FsmEventStart);

	myScript->regist_bgp_FsmEventStop(stud_bgp_FsmEventStop);

	myScript->regist_bgp_FsmEventTcpException(stud_bgp_FsmEventTcpException);

	myScript->regist_bgp_FsmEventTimerProcess(stud_bgp_FsmEventTimerProcess);

	myScript->regist_bgp_FsmEventUpdate(stud_bgp_FsmEventUpdate);

	myScript->regist_ipsec_InPut(stud_ipsec_input);

	myScript->regist_ipsec_OutPut(stud_ipsec_output);
	// for client 
	// for client cx
  	test_Item = atoi(argv[3]);
	test_Case = 0;

	memset(acUsername,0x00,16);
	strcpy(acUsername, argv[2]);
	//printf("The Username of student is %s\n", acUsername);
	

	for (int i = 4; i < argc; i ++){////changed from 3 to 4, because added the username in args
		test_Case= atoi(argv[i]);

		if (test_Item == 0)
		{
			switch(test_Case)
			{
			case 0: // stop and wait
				myScript->regist_slide_window(stud_slide_window_stop_and_wait);
				break;
			case 1: // back N frame
				myScript->regist_slide_window(stud_slide_window_back_n_frame);
				break;
                        case 2://choice to resend
                                myScript->regist_slide_window(stud_slide_window_choice_frame_resend);							  
                      		break;
			default:
			      break;
			}
		}

		printf("begin test!, testItem = %d  testcase = %d\n",test_Item,test_Case);

		pParser = new scriptParser(test_Item,test_Case,PARSER_CLIENT);
		
		//printf("client %x  server %x\n",pParser->clientAddr,pParser->serverAddr);

		// download script from server
		stud_test_initialize = false;
		if (!pParser->downloadScriptFromServer()){
			delete pParser;
			printf("can not download script from server!\n");
			break;
		}

		// auto run the script
		pParser->autoRunScript();

		delete pParser;

		// maybe we should use sleep to delay some time
	}

	clientSender->sendFinishMsg();
        printf("Test over!\n");
        Sleep(500);
	getchar();
}
