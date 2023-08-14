#ifndef _SCRIPT_ONE_LINE_H
#define _SCRIPT_ONE_LINE_H
#pragma once

#include "common.h"

class scriptOneLine{
public:

	UINT16 op;
	UINT16 timeout;
	UINT16 packetType;
	UINT16 funcName;
	UINT16 para[MAX_PARA_NUM];
	UINT32 srcAddr;
	UINT32 destAddr;
	char *data;
	UINT32 dataLen;


	// use for cut the script into slices
	char *keyStr[10];

	scriptOneLine();

	scriptOneLine(char *script,UINT8 type);

	// use for create the instance from buffer
	scriptOneLine(char *pBuffer);

	~scriptOneLine(){
		if (data != NULL){
			free(data);
		}
	}

	void setOp(UINT16 opStr);

	void setFunction(UINT16 funcStr);

	bool setPara(char *fieldStr, char *valStr);

	// get the operate string from script string
	UINT16 checkOp(char *scriptStr);

	// get the operate string from script string
	UINT16 checkPacketType(char *scriptStr);

	UINT16 checkFunctionName(char *scriptStr);

		// convert the HEX string to char *
	//e.g. (0x)01 02 03 04 
	// return the buffer size
	static int hexToBuffer(char *hexStr, char *pBuffer){
		int buffSize = 0;
		int pos = 0;
		int val = 0;
		char hexSubStr [2];

#if 0
		while (pos < strlen(hexStr)){
			if (hexStr[pos] == ' '){
				pos ++;
			}
			else{
				hexSubStr[0] = hexStr[pos];
				hexSubStr[1] = hexStr[pos + 1];
				sscanf(hexSubStr,"%2x",&val);
				*(pBuffer + buffSize) = val;
				buffSize ++;
				pos += 2;
			}
			
		}
#endif

		char *p = NULL;

		p = strtok(hexStr," ");

		while (p){
			sscanf(p,"%2x",&val);
			*(pBuffer + buffSize) = val;
			buffSize ++;
			p = strtok(NULL," ");
		}

		return buffSize;
	}

	// convert the script to string
	int scriptToString(char *pBuffer);

};
#endif
