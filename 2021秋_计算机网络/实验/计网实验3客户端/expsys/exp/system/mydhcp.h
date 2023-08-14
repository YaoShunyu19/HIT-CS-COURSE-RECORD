#ifndef _MY_DHCP_H_
#define _MY_DHCP_H_
#pragma once

#include "common.h"

typedef struct IpAndMac{
	ip_addr ip;
	UINT8  macAddr[6]; 
	struct IpAndMac *pNext;
};

typedef struct IpAndMacHead{
	UINT16 len;
	struct IpAndMac *pHead;
};

class mydhcp{
private:
	IpAndMacHead queueHead;


	static mydhcp *m_instance;

public:

	mydhcp();

	~mydhcp();

	static mydhcp *getInstance(){
		if (m_instance == NULL){
			m_instance = new mydhcp();
		}

		return m_instance;
	}

	IpAndMac *getAvailItem();

	void insertAvailItem(ip_addr ip);
	
};
#endif
