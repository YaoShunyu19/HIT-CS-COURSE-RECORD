 #include <stdio.h>
#include <stdlib.h> 

/*
7.7题
bar5.c

void f() {
	double x=-0.0;
} 
*/

/*
7.9题
bar6.c中的char main为弱符号，foo6.c中的main为函数，强符号，所以函数p2引用的main为函数main
输出的是main函数的十六进制表示 
*/

/*
7.11题
.bss节存储未初始化的全局和静态C变量，以及所有被初始化为0的全局的静态变量。在目标文件中这个节
不占据实际的空间，在运行时，在内存中分配这些变量，初始值为0
总的内存大小为0x230字节，包括0x228字节的.data里的值，和0x8字节.bss里的还为分配空间的值 
*/


int main()
{

	return 0;
} 
