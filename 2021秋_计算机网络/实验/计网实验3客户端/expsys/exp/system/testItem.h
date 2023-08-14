#ifndef _TEST_ITEM_H_
#define _TEST_ITEM_H_

#pragma once

#include "common.h"
#include "scriptOneLine.h"

// this is test item class
class testCase{
public:
	// ���캯��
	testCase(){

	}

	// �ӻ����������ɲ�����������, 
	// only for client
	// type 0: for server
	//      1: for client
	testCase(char *pBuffer,int type){
		int totalNum = *(UINT16*)pBuffer;
		int sliceLen = 0;
		
		pBuffer += 2;

		caseDesc = "";
		if (type == PARSER_CLIENT){
			serverScriptNum = 0;
			clientScriptNum = totalNum;
		}
		else{
			serverScriptNum = totalNum;
			clientScriptNum = 0;
		}

		for (int i = 0; i < totalNum; i ++){
			sliceLen = *(UINT16*)(pBuffer);

			if (type == PARSER_CLIENT){
				clientScript[i] = new scriptOneLine(pBuffer);
			}
			else{
				serverScript[i] = new scriptOneLine(pBuffer);
			}

			// move to next test case
			pBuffer += sliceLen;
		}
	}

	~testCase(){
		int i = 0;

		for (i = 0; i < clientScriptNum; i ++){
			delete clientScript[i];
			clientScript[i] = NULL;
		}

		for (i = 0; i < serverScriptNum; i++){
			delete serverScript[i];
			serverScript[i] = NULL;
		}
	}


	char *caseDesc; // ����������
	int	   serverScriptNum; // ���������Խű�����
	int    clientScriptNum; // �ͻ��˲��Խű�����
	scriptOneLine* serverScript[100];  // ���Խű������֧��100�����
	scriptOneLine* clientScript[100];  // ���Խű������֧��100�����

};

class testItem{
public:
	int testId; /* ������Ŀid*/
	int testcaseNum; // ����������
	char *testDesc; // ������Ŀ����
	testCase* testcase[20]; 

};

#endif
