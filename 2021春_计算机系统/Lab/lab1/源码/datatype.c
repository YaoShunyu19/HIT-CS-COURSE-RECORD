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
	
	
	printf("int���ͱ�������var_int ���ݣ�%-4d\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_int,&var_int);
    show_int(var_int);
    printf("\n");
    
    printf("float���ͱ�������var_float ���ݣ�%-4f\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_float,&var_float);
    show_int(var_float);
    printf("\n");
    
    printf("double���ͱ�������var_double ���ݣ�%-4f\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_double,&var_double);
    show_int(var_double);
    printf("\n");
    
    printf("long���ͱ�������var_long ���ݣ�%-4ld\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_long,&var_long);
    show_int(var_long);
    printf("\n");
    
    printf("short���ͱ�������var_short ���ݣ�%-4d\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_short,&var_short);
    show_int(var_short);
    printf("\n");
    
    printf("char���ͱ�������var_int ���ݣ�%-4c\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_char,&var_char);
    show_int(var_char);
    printf("\n");
    
    printf("pointer���ͱ�������var_pointer ���ݣ�%p\t  ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_pointer,&var_pointer);
    show_int(var_int);
    printf("\n");
    
    printf("struct���ͱ������� var_struct.num  ���ݣ�%ld\t ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_struct.num,&var_struct.num);
    show_long(var_struct.num);
    printf("                   var_struct.age  ���ݣ�%d\t ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_struct.age,&var_struct.age);
    show_int(var_struct.age);
    printf("\n");
    
    printf("union���ͱ������� var_union.index  ���ݣ�%d\t ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_union.index,&var_union.index);
    show_int(var_union.index);
    printf("                  var_union.i  ���ݣ�%c\t ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",var_union.i,&var_union.i);
    show_char(var_union.i);
    printf("\n");
    
    printf("enum���ͱ����� w ���ݣ�%d\t ��ַ��%p\t ��Ӧ16���Ƶ��ڴ���ֽڣ�",w,&w);
    show_int(w);
    printf("\n");
    
    printf("main�����ĵ�ַΪ %x\n",&main);
    printf("\n");

    printf("printf�����ĵ�ַΪ %x\n",&printf);
	return 0;
}
