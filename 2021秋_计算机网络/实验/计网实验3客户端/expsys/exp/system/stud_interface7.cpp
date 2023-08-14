#include "sysinclude.h"

extern void bgp_FsmTryToConnectPeer();
extern void bgp_FsmSendTcpData(char *pBuf,DWORD dwLen);

BYTE stud_bgp_FsmEventOpen(BgpPeer *pPeer,BYTE *pBuf,unsigned int len) 
{
	return 0;
}

BYTE stud_bgp_FsmEventKeepAlive(BgpPeer *pPeer,BYTE *pBuf,unsigned int len)    
{
	return 0;
}

BYTE stud_bgp_FsmEventNotification(BgpPeer *pPeer,BYTE *pBuf,unsigned int len) 
{
	return 0;
}

BYTE stud_bgp_FsmEventUpdate(BgpPeer *pPeer,BYTE *pBuf,unsigned int len)
{
	return 0;
}
        
BYTE stud_bgp_FsmEventTcpException(BgpPeer *pPeer,BYTE msgType)           
{
	return 0;
}
	
BYTE stud_bgp_FsmEventTimerProcess(BgpPeer *pPeer,BYTE msgType)
{
	return 0;
}
        
BYTE stud_bgp_FsmEventStart(BgpPeer *pPeer)      
{
	return 0;
}
       
BYTE stud_bgp_FsmEventStop(BgpPeer *pPeer)       
{
	return 0;
}
       
BYTE stud_bgp_FsmEventConnect(BgpPeer *pPeer)   
{
	return 0;
}

