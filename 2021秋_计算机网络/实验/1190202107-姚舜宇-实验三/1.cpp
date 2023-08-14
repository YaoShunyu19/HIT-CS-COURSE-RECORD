﻿/*
IPV4 分组收发实验
*/
#include "sysInclude.h"
#include <stdio.h>
#include <malloc.h>
extern void ip_DiscardPkt(char* pBuffer, int type);

extern void ip_SendtoLower(char* pBuffer, int length);

extern void ip_SendtoUp(char* pBuffer, int length);

extern unsigned int getIpv4Address();

/*
接收接口函数
输入：pBuffer为指向接收缓冲区的指针，指向IPv4 分组头部
	  length为IPv4为分组长度
返回：0：成功接收IP 分组并交给上层处理
	  1：IP 分组接收失败
*/
int stud_ip_recv(char* pBuffer, unsigned short length)
{
	int version = pBuffer[0] >> 4;                             // 第0个字节内的最开始4个bits为版本号
	int head_length = pBuffer[0] & 0xf;                        // 第0个字节内的紧接着4个bits为头部长度
	short ttl = (unsigned short)pBuffer[8];                    // 第8个字节内的8个bits为生存时间ttl
	short checksum = ntohs(*(unsigned short*)(pBuffer + 10));  // 第10个字节后的16bits为头检验和（short int）ntohs:将一个无符号短整形数从网络字节顺序转换为主机字节顺序
	int destination = ntohl(*(unsigned int*)(pBuffer + 16));   // 第16个字节后的32bits为目的ip地址（long int）ntohl:将一个无符号长整形数从网络字节顺序转换为主机字节顺序

	if (version != 4)  //版本号不为4，错误
	{
		ip_DiscardPkt(pBuffer, STUD_IP_TEST_VERSION_ERROR);
		return 1;
	}
	if (head_length < 5 || head_length > 15)  //首部长度小于5或大于15，错误
	{
		ip_DiscardPkt(pBuffer, STUD_IP_TEST_HEADLEN_ERROR);
		return 1;
	}
	if (ttl <= 0)  //生存时间小于或等于0从，错误
	{
		ip_DiscardPkt(pBuffer, STUD_IP_TEST_TTL_ERROR);
		return 1;
	}
	if (destination != getIpv4Address() && destination != 0xffff)  //目的IP地址错误
	{
		ip_DiscardPkt(pBuffer, STUD_IP_TEST_DESTINATION_ERROR);
		return 1;
	}

	//计算校验和
	unsigned long sum = 0;
	unsigned long t = 0;
	int i;
	for (i = 0; i < head_length * 2; i++)
	{
		t += (unsigned char)pBuffer[i * 2] << 8;
		t += (unsigned char)pBuffer[i * 2 + 1];
		sum += t;
		t = 0;
	}
	unsigned short low_of_sum = sum & 0xffff;
	unsigned short high_of_sum = sum >> 16;
	if (low_of_sum + high_of_sum != 0xffff)  //校验和发生错误
	{
		ip_DiscardPkt(pBuffer, STUD_IP_TEST_CHECKSUM_ERROR);
		return 1;
	}
	ip_SendtoUp(pBuffer, length); 
	return 0;
}

/*
发送接口函数
输入：pBuffer为指向接收缓冲区的指针，指向IPv4 分组头部
	  len为IPv4上层协议数据长度
	  srcAddr为源IPv4地址
	  dstAddr为目的IPv4地址
	  protocol为IPv4上层协议号
	  ttl为生存时间
返回：0：成功发送IP分组
	  1：发送IP分组失败
*/
int stud_ip_Upsend(char* pBuffer, unsigned short len, unsigned int srcAddr,
	unsigned int dstAddr, byte protocol, byte ttl)
{
	short ip_length = len + 20; //得到这层的数据长度
	char* buffer = (char*)malloc(ip_length * sizeof(char));
	memset(buffer, 0, ip_length);
	buffer[0] = 0x45; //规定版本号和首部长度为4和20
	buffer[8] = ttl;  //规定生存时间
	buffer[9] = protocol;  //规定协议号

	// 将数据长度转换为网络字节序
	unsigned short network_length = htons(ip_length);

	memcpy(buffer + 2, &network_length, 2);
	unsigned int src = htonl(srcAddr);  //解析源IPv4地址
	unsigned int dst = htonl(dstAddr);  //解析目的IPv4地址

	memcpy(buffer + 12, &src, 4);
	memcpy(buffer + 16, &dst, 4);

	//计算校验和
	unsigned long sum = 0;
	unsigned long t = 0;
	int i;
	for (i = 0; i < 20; i += 2)
	{
		t += (unsigned char)buffer[i] << 8;
		t += (unsigned char)buffer[i + 1];
		sum += t;
		t = 0;
	}
	unsigned short low_of_sum = sum & 0xffff;
	unsigned short high_of_sum = sum >> 16;
	unsigned short checksum = low_of_sum + high_of_sum;  //低16位与高16位相加
	checksum = ~checksum;  //取反得到校验和
	unsigned short header_checksum = htons(checksum);  //将校验和更新

	memcpy(buffer + 10, &header_checksum, 2);
	memcpy(buffer + 20, pBuffer, len);

	ip_SendtoLower(buffer, len + 20); //发送分组
	return 0;
}

