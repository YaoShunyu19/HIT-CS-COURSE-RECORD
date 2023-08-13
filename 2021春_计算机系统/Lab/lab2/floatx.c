#include <stdio.h>
#include <stdlib.h>

void show_16(unsigned char str[]) {
	int i=3;
	printf("Hexadecimal representation is  ");
	for (;i>=0;i--) printf("0x%02x ",str[i]);
	printf("\n\n");
}
int main()
{
    float *p;
    unsigned char zero_p[] ={0x00,0x00,0x00,0x00};//+0.0
    p=(float *)zero_p;
    printf("positive zero:   %f\n",*p);
    show_16(zero_p);
    
    unsigned char zero_n[] ={0x00,0x00,0x00,0x80};//-0.0
    p=(float *)zero_n;
    printf("negative zero:   %f\n",*p);
    show_16(zero_n);
    
    unsigned char min_float_p[] ={0x01,0x00,0x00,0x00};//minimun float positive
    p=(float *)min_float_p;
    printf("min float positive:   %E\n",*p);
    show_16(min_float_p);
    
    unsigned char max_float_p[] ={0xff,0xff,0x7f,0x7f};//maximum float positive
    p=(float *)max_float_p;
    printf("max float positive:   %E\n",*p);
    show_16(max_float_p);
    
    unsigned char min_normalized_p[] ={0x00,0x00,0x80,0x00};//minimum positive normalized float
    p=(float *)min_normalized_p;
    printf("min positive normalized:   %E\n",*p);
    show_16(min_normalized_p);
    
    unsigned char p_infinity[] ={0x00,0x00,0x80,0x7f};//positive infinity
    p=(float *)p_infinity;
    printf("positive infinity:   %E\n",*p);
    show_16(p_infinity);
    
    unsigned char nan[] ={0x01,0x00,0x80,0x7f};//not a number
    p=(float *)nan;
    printf("nan:   %E\n",*p);
    show_16(nan);
    return 0;
}
