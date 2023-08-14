#ifndef _CLIENT_H_
#define _CLIENT_H_
#pragma once

#define BUF_SIZE 2048

#define TEST_SERVER_PORT 2006
#define MAIN_UI_PORT 2006

#include <stdio.h>
//#include <winsock2.h>
#include "MessageBlock.h"

class Client
{
public:
	Client(){};
	int Init(char *);
	bool initUISocket();
	int Close();


	static Client* getInstance(){
		if (m_client == NULL){
			m_client = new Client();
		}

		return m_client;
	}

	int Send(MessageBlock& mb);
	bool Recv(MessageBlock &mb,int ti);
	void sendMsgToUI(UINT8 testitem, UINT8 testcase, UINT16 result);
	void sendDataToUI(char *result,int len,int type);
	int SendToUI(MessageBlock &mb,int type);
	void sendFinishMsg();

private:
	static Client *m_client;
	WSADATA m_wsd;
	SOCKET m_socket;
	SOCKET m_socketUI;
	char m_recvbuf[4096];
	int m_recvlen;
	sockaddr_in m_server;
};

#endif
