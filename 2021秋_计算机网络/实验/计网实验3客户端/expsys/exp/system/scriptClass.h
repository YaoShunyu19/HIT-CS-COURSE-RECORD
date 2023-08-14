
// this is a class to describe the scriptParser
#pragma once

#include "common.h"
#include "testItem.h"
#include "scriptOneLine.h"

class scriptClass
{
	// public variable
public:
		testItem* testitem[20];

	// private varible
private:
	static scriptClass *m_scriptclass;
	FILE *m_filestr;
	int testItemId;
	int testCaseIndex;
	int scriptIndex;
	int testItemNum;
	testCase * tempTestCase;
	scriptOneLine *tempScript;
	
	scriptClass(){
		testItemNum = 0;
	}

	~scriptClass(){
	
	}


public:
	// get the instance
	static scriptClass* getInstance(){
		if (m_scriptclass == NULL){
			m_scriptclass = new scriptClass();
		}
		return m_scriptclass;
	}

	bool loadScript();

	// when get the <testItem> into the function 
	// arrive the </testItem> out the function
	void enterTestItem();

	// when get the <testCase> into the function 
	// arrive the </testCase> out the function
	void enterTestCase();

	// when get the <server> into the function 
	// arrive the </server> out the function
	void enterServer();

	// when get the <client> into the function 
	// arrive the </client> out the function
	void enterClient();

	// when get the <client> into the function 
	// arrive the </client> out the function
	void enterPara();

	// get the testItem describe 
	// buffer structure is :
	// length(16)
	int getScriptList(char *pBuffer);

	// get the script of special test case
	// flag = 1: client
	// flag = 0; server
	// return: buffer size
	int getScript(UINT8 itemIdex, UINT8 caseIdex, UINT8 flag, char* pBuffer);
};
