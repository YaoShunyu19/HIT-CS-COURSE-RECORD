#include <stdio.h>
#include <stdlib.h>

int main()
{
    float a=1;
	printf("%f\n",a/0);
	unsigned char min_float_p[] ={0x01,0x00,0x00,0x00};
    float *p=(float *)min_float_p;
    printf("%f\n",a/(*p));
    unsigned char min_nor_p[] ={0x00,0x00,0x80,0x00};
    p=(float *)min_nor_p;
    printf("%f\n",a/(*p));
    return 0;
}
