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

// 脚本解析器类
// 继承线程类
class scriptParser {
	// 用于执行脚本
	
private:
	int testItemIndex;// 测试例项目号
	int testCaseIndex; // 测试例号
	testCase *downloadScript; // 测试例脚本
	int currentLine;  // 当前的行数
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


	// 构造函数，用户指定要测试项目号和测试例号
	scriptParser(int itemIndex = 0, int caseIndex = 0, UINT8 parserType = PARSER_CLIENT);

	// 析构函数
	~scriptParser();

	// 从服务器下载相应的脚本 only for client
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
