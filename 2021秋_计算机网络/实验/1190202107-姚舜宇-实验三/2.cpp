﻿/*
IPV4 分组转发实验
*/
#include "sysInclude.h"
#include <stdio.h>
#include <vector>
using std::vector;

// system support
extern void fwd_LocalRcv(char* pBuffer, int length);

extern void fwd_SendtoLower(char* pBuffer, int length, unsigned int nexthop);

extern void fwd_DiscardPkt(char* pBuffer, int type);

extern unsigned int getIpv4Address();

vector<stud_route_msg> route;

void stud_Route_Init()
{
	return;
}

/*
用路由表添加路由的函数
输入：proute ：指向需要添加路由信息的结构体头部，
    其数据结构
	stud_route_msg 的定义如下：
    typedef struct stud_route_msg
    {
      unsigned int dest;  //目的地址
      unsigned int masklen;  //子网掩码长度
      unsigned int nexthop;  //下一跳
    } stud_route_msg;
*/
void stud_route_add(stud_route_msg* proute)
{

	stud_route_msg temp;  //用来将待添加的路由表项转化为本地字节序
	unsigned int dest = ntohl(proute->dest); 
	unsigned int masklen = ntohl(proute->masklen);   
	unsigned int nexthop = ntohl(proute->nexthop);  //依次将proute的所有字段转化为字节序储存
	temp.dest = dest;
	temp.masklen = masklen;
	temp.nexthop = nexthop;
	route.push_back(temp);
	return;
}

/*
系统处理收到的IP分组的函数
输入：pBuffer：指向接收到的IPv4 分组头部
      length：IPv4 分组的长度
返回：0 为成功，1 为失败；
*/
int stud_fwd_deal(char* pBuffer, int length)
{
	int version = pBuffer[0] >> 4;							   //第0个字节内的最开始4个bits为版本号
	int head_length = pBuffer[0] & 0xf;						   //第0个字节内的紧接着4个bits为头部长度
	short ttl = (unsigned short)pBuffer[8];					   //第8个字节内的8个bits为生存时间ttl
	short checksum = ntohs(*(unsigned short*)(pBuffer + 10));  //第10个字节后的16bits为头检验和（short int）
	int destination = ntohl(*(unsigned int*)(pBuffer + 16));   //第10个字节后的16bits为头检验和（short int）
	if (ttl <= 0)
	{
		fwd_DiscardPkt(pBuffer, STUD_FORWARD_TEST_TTLERROR);
		return 1;
	}

	if (destination == getIpv4Address())
	{
		fwd_LocalRcv(pBuffer, length);
		return 0;
	}
	stud_route_msg* ans_route = NULL;
	int temp_dest = destination;
	for (int i = 0; i < route.size(); i++)
	{
		unsigned int temp_sub_net = route[i].dest & ((1 << 31) >> (route[i].masklen - 1));
		if (temp_sub_net == temp_dest)
		{
			ans_route = &route[i];
			break;
		}
	}
	if (!ans_route)
	{
		fwd_DiscardPkt(pBuffer, STUD_FORWARD_TEST_NOROUTE);
		return 1;
	}
	else
	{
		char* buffer = new char[length];
		memcpy(buffer, pBuffer, length);
		buffer[8] = ttl - 1;
		memset(buffer + 10, 0, 2);
		unsigned long sum = 0;
		unsigned long temp = 0;
		int i;

		for (i = 0; i < head_length * 2; i++)
		{
			temp += (unsigned char)buffer[i * 2] << 8;
			temp += (unsigned char)buffer[i * 2 + 1];
			sum += temp;
			temp = 0;
		}
		unsigned short low_of_sum = sum & 0xffff;
		unsigned short high_of_sum = sum >> 16;
		unsigned short checksum = low_of_sum + high_of_sum; 
		checksum = ~checksum;
		unsigned short header_checksum = htons(checksum);
		memcpy(buffer + 10, &header_checksum, 2);
		fwd_SendtoLower(buffer, length, ans_route->nexthop);
	}
	return 0;
}