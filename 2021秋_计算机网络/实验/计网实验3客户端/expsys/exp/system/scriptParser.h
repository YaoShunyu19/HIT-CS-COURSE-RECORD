#ifndef _SCRIPT_PARSER_H
#define _SCRIPT_PARSER_H
#pragma once

#include "common.h"
#include "testItem.h"
#include "scriptOneLine.h"

//add by wangyl for username auth 20070919
typedef struct IpAndMacMsgUI{
	commonHead commonhead;
	UINT8 flag;
	
	char usrID[EXPE_STUDID_LEN];////corrected
	
	
};

typedef struct IpAndMacMsg{
	commonHead commonhead;
	UINT8 flag;
	ip_addr clientAddr;
	UINT8 clientMac[6];
	ip_addr serverAddr;
	UINT8 serverMac[6];
};

// �ű���������
// �̳��߳���
class scriptParser {
	// ����ִ�нű�
	
private:
	int testItemIndex;// ��������Ŀ��
	int testCaseIndex; // ��������
	testCase *downloadScript; // �������ű�
	int currentLine;  // ��ǰ������
	bool testResult;
	int mysocket[5];	
	UINT8 parserType;	
	
	
public:
	ip_addr clientAddr;
	ip_addr serverAddr;
	ip_addr tunnelClientAddr;
	ip_addr tunnelServerAddr;

	UINT8 clientMac[6];
	UINT8 serverMac[6];
	UINT8 tunnelClientMac[6];
	UINT8 tunnelServerMac[6];


	// ���캯�����û�ָ��Ҫ������Ŀ�źͲ�������
	scriptParser(int itemIndex = 0, int caseIndex = 0, UINT8 parserType = PARSER_CLIENT);

	// ��������
	~scriptParser();

	// �ӷ�����������Ӧ�Ľű� only for client
	// if success : true
	// else :   false
	bool downloadScriptFromServer();

	bool downloadScriptOnServer();

	bool downloadListFromServer();

	bool autoRunScript();	

	int runOneScript(scriptOneLine *pOneScript);

//	int runOneScript(scriptOneLine *pOneScript,char *pBuffer);

	bool getInputPacket(char *pBuffer, int val);

	int getPacketFromSocket(char *pBuffer);

	bool checkPacket(char *pData,UINT16 *pPara, UINT32 srcAddr, UINT32 destAddr,char *);

	bool checkIPv6Packet(char *pData,UINT16 *pPara, UINT32 srcAddr, UINT32 destAddr,char *);

	bool checkTcpPacket(char *, UINT16 *pPara, char *,int);

	void sendMsgToServer(UINT8 *pBuffer);

	void sendMsgToClient(UINT8 *pBuffer);

	bool getIpAndMacFromServer();

};

#endif
