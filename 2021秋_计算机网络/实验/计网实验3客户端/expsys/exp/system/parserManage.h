#ifndef _PARSER_MANAGE_H_
#define  _PARSER_MANAGE_H_

#pragma once
#include "common.h"
#include "scriptParser.h"
// define the global variables to support script parser management

typedef struct parserLink{
	scriptParser *pParser;
	struct parserLink *pNext;
};

typedef struct parserLinkHead{
	parserLink *pHead;
	parserLink *pTail;
	UINT16 len;
};

class parserManage{
private:
	parserLinkHead pHead;

	static parserManage *m_instance;

public:
	parserManage(){
		pHead.len = 0;
		pHead.pHead = NULL;
		pHead.pTail = NULL;
	}

	~parserManage(){
		clearParser();
	}

	static parserManage *get_instance(){
		if (m_instance == NULL){
			m_instance = new parserManage();
		}
		return m_instance;
	}

	// insert a new parser
	void insertParser(scriptParser *pParser);

	scriptParser * getParser(ip_addr ipaddr);

	void deleteParser(ip_addr ipaddr);

	// delete all parsers
	void clearParser(void);
};
#endif
