#include <stdio.h>
#include <stdlib.h> 
/*
3.59题
(%rdi):*dest
%rsi:x  x=2^64*x_h+x_l
%rdx:y  y=2^64*y_h+y_l

store_prod
	movq %rdx,%rax      rax=y       rax:y_l
	cqto                符号位扩展  rdx:y_h 
	movq %rsi,%rcx		rcx=x       rcx:x_l
	sarq $63,%rcx       rcx>>=63    rcx:x_h
	imulq %rax,%rcx     rcx*=rax    rcx:x_h*y_l
	imulq %rsi,%rdx     rdx*=rsi    rdx:y_h*x_l
	addq %rdx,%rcx      rcx+=rdx    rcx:x_h*y_l+y_h*x_l
	mulq %rsi           (rdx:rax)=rax*rsi    y_l*x_l rdx:进位 rax:低64位 
	addq %rcx,%rdx      rdx+=rcx    rdx:进位+x_h*y_l+y_h*x_l
	movq %rax,(%rdi)    结果的低64位 
	movq %rdx,8(%rdi)   结果的高64位 
	
64位的数符号位扩展位128位，值不变
x_h,y_h为x,y的符号位扩展后的高64位，全0或全1
p_l=x_l*y_l的低64位
p_h=x_l*y_l的进位+x_h*y_l+y_h*x_l
p=2^64*p_h+p_l
*/

//3.61题
long cread_alt(long *xp) {
	long t=0;
	xp=(xp?xp:&t);
	return *xp;
}

//3.63题
long switch_prob(long x,long n) {
	long result=x;
	switch(n) {
		case 60:
		case 62:
			result=8*x;
			break;
		case 63:
			result=x;
			result>>=3;
			break;
		case 64:
			result=x;
			result<<=4;
			result-=x;
			x=result;
		case 65:
			x*=x;
		case 61:
		default:
			result=x+75;
			break;
	}
	return result;
} 

/*
3.65题
A:rdx  因为第6行addq $8,%rdx,rdx的地址加8，在内循环中，对应的是列的元素，j+1所以是A[i][j] 
B:rax  因为第7行addq $120,%rax,rax的地址加120，在内循环中，对应的是行的元素，j+1所以是A[j][i] 
C :15  因为由B可以知道，一行有120个字节，每8个字节保存一个地址，所以一行有120/8=15个元素的地址，故M=15 
*/

/*
3.67题
A:       _________________
        |                |
        |                |
        .                .
        .                .
        .                .
        |                |
        |                |
        |    rdi=z       |-->rsp+24
        |    rax=&z      |-->rsp+16
        |    rsi=y       |-->rsp+8
        |    rdi=x       |-->rsp+0
        ------------------
B:  传递了结构体strA
C:  通过寄存器rsp的地址偏移，将栈中元素取出到寄存器中进行调用
D:  首先在函数eval中设置结构r的首地址为64(%rsp)，通过寄存器rdi传递给函数process，
    然后再通过rdi的地址偏移将结果压入栈中
E:       _________________
        |                |
        |                |
        |                |-->rsp+104
        |                |-->rsp+96
        |                |-->rsp+88
        |        z       |-->rsp+80
        |        x       |-->rsp+72
        |        y       |-->rsp+64 = rdi
        .                .
        .                .
        .                .
        |                |
        |                |
        |    指向rdi=z   |-->rsp+24
        |    rax=&z      |-->rsp+16
        |    rsi=y       |-->rsp+8
        |    rdi=x       |-->rsp+0
        ------------------
    通过寄存器rsp的地址偏移，将栈中元素取出到寄存器中进行调用
F:将结构里的元素压入栈中进行传递，调用使通过栈指针rsp的偏移来取出 
*/ 

/*
3.69题
汇编代码中第2、3行，ecx=(%rsi)+0x120(%rsi),ecx就是test函数里的n，(%rsi)=bp->first,
0x120(%rsi)=bp->last,0x120=288=8+CNT*sizeof(a_struct).
汇编代码中第4到6行，%rax=5*i,%rax=bp+40*i,%rdx=*(bp+40*i+8),即为&bp->a[i],所以
sizeof(a_struct)=40,带入上式，得到CNT=7
A:
	CNT=7

汇编代码中第8行，0x10(%rax,%rdx,8)表示ap->x[ap->idx],说明a_struct里有一个long型数组x，有一个
整数idx，数组的大小只能为4，因为有idx的存在，不能大于4，5*8=40，也不能小于4，3*8=24，整数不能
有16个字节，所以除了长度为4的long型数组外，还要8个字节的整数，一定为long型
ap=%rax+8,ap->x[ap->idx]=ap+8+8*%rdx,%rdx=*(bp+8+40*i)=ap->idx,证明了上述猜想
所以a_struct的完整声明为：
B:
	typedef struct {
		long idx;
		long x[4];
	} a_struct;

*/ 

//3.71题
void good_echo() {
	int MAX=10;
	char str[MAX];
	while (fgets(str,MAX,stdin)!=NULL) {
		int i;
		for (i=0;str[i]!='\0' && str[i]!='\n';i++) {
			putchar(str[i]);
		}
		if (i<MAX-1) break;
	}
	return;
} 

int main()
{
	
	good_echo();
	 
	return 0;
} 
