#ifndef _TEST_ITEM_H_
#define _TEST_ITEM_H_

#pragma once

#include "common.h"
#include "scriptOneLine.h"

// this is test item class
class testCase{
public:
	// 构造函数
	testCase(){

	}

	// 从缓冲区中生成测试命令序列, 
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


	char *caseDesc; // 测试例描述
	int	   serverScriptNum; // 服务器测试脚本行数
	int    clientScriptNum; // 客户端测试脚本行数
	scriptOneLine* serverScript[100];  // 测试脚本，最大支持100个语句
	scriptOneLine* clientScript[100];  // 测试脚本，最大支持100个语句

};

class testItem{
public:
	int testId; /* 测试项目id*/
	int testcaseNum; // 测试例数量
	char *testDesc; // 测试项目描述
	testCase* testcase[20]; 

};

#endif
