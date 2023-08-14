#ifndef _MESSAGE_BLOCK_H
#define _MESSAGE_BLOCK_H
#pragma once

class MessageBlock
{
public:
	MessageBlock(UINT16 length = 0);
	MessageBlock(char* pData, UINT16 length);
	MessageBlock(const MessageBlock& mb);
	MessageBlock(char *pBuffer);
	~MessageBlock();

	static MessageBlock MB_TestReq(char expNum, char testCaseNum);
	static MessageBlock MB_Report(char report);
	static MessageBlock MB_TestFinish();

	int Copy(char* pData, int length);

	char* GetDataPtr();
	int GetLength();
	void SetLength(int len); 

	int SetType(char type);
	int SetSubType(char subType);
	bool SetOverLoadData(char* data, UINT16 len);

	char GetType();
	char GetSubType();

	int Print();

public:
	char* m_pData;
	UINT16 m_length;
};

#endif
