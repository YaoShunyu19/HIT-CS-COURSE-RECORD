#include <stdio.h>

/*
4.45题
A 不能 sub $8,%rsp将%rsp的值减去0x8，压入栈中的值是%rsp-0x8。
B movq REG,-0x8(%rsp)
  subq $8,%rsp 
*/

/*
4.46题
A 不能 movq指令将栈顶的值赋给%rsp，所以%rsp+0x8不少指向栈的指针了
B addq $8,,%rsp
  movq -0x8(%rsp),REG 
*/

/*
4.51题
取指    icode:ifun<-M1[PC]
	    rA:rB<-M1[PC+1]
	    valC<-M8[PC+2]
	    valP<-PC+10
译码    valB<-R[rB]
执行    valE<-valB+valC
访存
写回    R[rB]<-valE
更新PC  PC<-valP 
*/



int main() {
	return 0;
} 
