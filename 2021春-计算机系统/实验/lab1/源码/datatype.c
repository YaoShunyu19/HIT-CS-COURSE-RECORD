#include <stdio.h>
#include <stdlib.h>

typedef unsigned char *byte_pointer;

struct STU {
	long num;
	int age;
};

union UNI {
	int index;
	char i;
};

enum week{ Mon=1, Tues, Wed, Thurs, Fri, Sat, Sun};

void show_bytes(byte_pointer start, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++)
		printf(" %.2x",start[i]);
	printf("\n");
}

void show_int(int x)
{
	show_bytes((byte_pointer) &x,sizeof(int));
}

void show_float(float x)
{
	show_bytes((byte_pointer) &x,sizeof(float));
}

void show_double(double x)
{
	show_bytes((byte_pointer) &x,sizeof(double));
}

void show_long(long x)
{
	show_bytes((byte_pointer) &x,sizeof(long));
}

void show_short(short x)
{
	show_bytes((byte_pointer) &x,sizeof(short));
}

void show_pointer(void *x)
{
	show_bytes((byte_pointer) &x,sizeof(void *));
}

void show_char(char x)
{
	show_bytes((byte_pointer) &x,sizeof(char));
}




int main(int argc, char *argv[]) {
	
	int var_int=255;
	float var_float=1.68;
	double var_double=1.24;
	long var_long=456;
	short var_short=14;
	char var_char='s';
	int * var_pointer=&var_int;
	struct STU var_struct;
	var_struct.num=1190202107;
	var_struct.age=19;
	union UNI var_union;
	var_union.index=19;
	var_union.i='l';
	enum week w;
	
	
	printf("int类型变量名：var_int 内容：%-4d\t  地址：%p\t 对应16进制的内存各字节：",var_int,&var_int);
    show_int(var_int);
    printf("\n");
    
    printf("float类型变量名：var_float 内容：%-4f\t  地址：%p\t 对应16进制的内存各字节：",var_float,&var_float);
    show_int(var_float);
    printf("\n");
    
    printf("double类型变量名：var_double 内容：%-4f\t  地址：%p\t 对应16进制的内存各字节：",var_double,&var_double);
    show_int(var_double);
    printf("\n");
    
    printf("long类型变量名：var_long 内容：%-4ld\t  地址：%p\t 对应16进制的内存各字节：",var_long,&var_long);
    show_int(var_long);
    printf("\n");
    
    printf("short类型变量名：var_short 内容：%-4d\t  地址：%p\t 对应16进制的内存各字节：",var_short,&var_short);
    show_int(var_short);
    printf("\n");
    
    printf("char类型变量名：var_int 内容：%-4c\t  地址：%p\t 对应16进制的内存各字节：",var_char,&var_char);
    show_int(var_char);
    printf("\n");
    
    printf("pointer类型变量名：var_pointer 内容：%p\t  地址：%p\t 对应16进制的内存各字节：",var_pointer,&var_pointer);
    show_int(var_int);
    printf("\n");
    
    printf("struct类型变量名： var_struct.num  内容：%ld\t 地址：%p\t 对应16进制的内存各字节：",var_struct.num,&var_struct.num);
    show_long(var_struct.num);
    printf("                   var_struct.age  内容：%d\t 地址：%p\t 对应16进制的内存各字节：",var_struct.age,&var_struct.age);
    show_int(var_struct.age);
    printf("\n");
    
    printf("union类型变量名： var_union.index  内容：%d\t 地址：%p\t 对应16进制的内存各字节：",var_union.index,&var_union.index);
    show_int(var_union.index);
    printf("                  var_union.i  内容：%c\t 地址：%p\t 对应16进制的内存各字节：",var_union.i,&var_union.i);
    show_char(var_union.i);
    printf("\n");
    
    printf("enum类型变量名 w 内容：%d\t 地址：%p\t 对应16进制的内存各字节：",w,&w);
    show_int(w);
    printf("\n");
    
    printf("main函数的地址为 %x\n",&main);
    printf("\n");

    printf("printf函数的地址为 %x\n",&printf);
	return 0;
}
