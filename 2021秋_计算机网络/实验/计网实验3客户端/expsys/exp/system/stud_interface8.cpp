#include "sysinclude.h"

extern struct ipsecSpdHeader *g_ipsec_spd_in;
extern struct ipsecSpdHeader *g_ipsec_spd_out;


extern void ipsec_Receive_process(char *pBuffer,int dwLen);
extern void ipsec_Send_process(char *pBuffer, int iLen);
extern  AH_ALGORITHM *ahAlgorithmLookup(unsigned char algAuth);
extern ESP_ALGORITHM *espAlgorithmLookup(unsigned char  algEncry);
extern int ah4CalculateICV(unsigned char *pPacketCpy, unsigned short  totalLen,unsigned char  *pOutput,	const AH_ALGORITHM  *pAlg,IPSEC_ASSOC  *pSA);
extern int espAuthCalculate(unsigned char  *pData,unsigned short  dataLen,unsigned char  *pOutput,AH_ALGORITHM  *pAlg,IPSEC_ASSOC  *pSA);




int stud_ipsec_input(char * pBuffer,unsigned int len)
{
	return 1;
}

int stud_ipsec_output(char * pBuffer,unsigned int len)
{
	return 1;
}


