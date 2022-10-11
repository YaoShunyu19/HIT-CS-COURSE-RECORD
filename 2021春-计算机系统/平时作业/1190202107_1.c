#include <stdio.h>
#include <stdlib.h>

//2.63题
unsigned srl(unsigned x,int k)
{
    /*Perform shift arithmetically*/
    unsigned xsra=(int)x>>k;
    unsigned m=0x0;
    m+=1<<(8*sizeof(int)-k);
    m--;
    xsra&=m;
    return xsra;
}
int sra(int x,int k)
{
    /*Perform shift logically*/
    int xsrl=(unsigned)x>>k;
    int flag=(xsrl&(1<<(8*sizeof(int)-k-1)));
    int p=xsrl | (-1<<(8*sizeof(int)-k));
    return (((~(!flag))+1) & xsrl) + (((~(!!flag))+1) & p);
}


//2.67题 C
int bad_int_size_is_32()
{
    int before_msb=2<<15;
    int set_msb=before_msb<<15;
    int beyond_msb=set_msb<<1;
    return before_msb && set_msb && !beyond_msb;
}

//2.71题 C
typedef unsigned packed_t;
int xbyte(packed_t word,int bytenum)
{
    int y=word<<((3-bytenum)*8);
    return y>>24;
}

int main()
{
    /*2.59题：
    x&=0x000000ff;
    y&=0xffffff00;
    printf("0x%x",x|y);

    2.67题
    A. 第6行int型数据<<32
    B. 1<<32改为2<<31
    C. 见函数int bad_int_size_is_32()

    2.71题
    A. 错在没有将符号位扩展为一个32位的int型
    B. 见函数xbyte()

    2.83题
    A. value=(Y>>k)*(2^k/(2^k-1))=Y/(2^k-1)
    B. (a) 5/7     (b) 6/15=2/5     (c) 19/63

    2.87题
    描述                    Hex         M         E         V         D
    -0                      8000        0         -14       -0        -0.0
    最小的>2的值            4001     1025/1024     1    1025*2^(-9)   2.00195313
    512                     6000        1          9        512       512.0
    最大的非规格化数        03FF    1023/1024     -14   1023*2^(-24)   0.000060976
    负无穷                  FC00        --        --       负无穷     负无穷
    十六进制表示为3BB0的数  3BB0      59/32        -1    59*2^(-5)     1.84375
    */
    return 0;
}
