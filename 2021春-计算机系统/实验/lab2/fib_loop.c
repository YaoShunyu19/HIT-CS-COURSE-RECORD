#include <stdio.h>
#include <stdlib.h>

int fib_loop_int(int n) {
	int fib[100];
	fib[0]=0;
	fib[1]=1;
	if (n==0) return 0;
	else if (n==1) return 1;
	int i=2;
	for (;i<=n;i++) {
		fib[i]=fib[i-1]+fib[i-2];
	}
	return fib[i-1];
}
long fib_loop_long(int n) {
	long fib[100];
	fib[0]=0;
	fib[1]=1;
	if (n==0) return 0;
	else if (n==1) return 1;
	int i=2;
	for (;i<=n;i++) {
		fib[i]=fib[i-1]+fib[i-2];
	}
	return fib[i-1];
}
unsigned int fib_loop_ui(int n) {
	unsigned int fib[100];
	fib[0]=0;
	fib[1]=1;
	if (n==0) return 0;
	else if (n==1) return 1;
	int i=2;
	for (;i<=n;i++) {
		fib[i]=fib[i-1]+fib[i-2];
	}
	return fib[i-1];
}
unsigned long fib_loop_ul(int n) {
	long fib[100];
	fib[0]=0;
	fib[1]=1;
	if (n==0) return 0;
	else if (n==1) return 1;
	int i=2;
	for (;i<=n;i++) {
		fib[i]=fib[i-1]+fib[i-2];
	}
	return fib[i-1];
}

int main()
{
	int i=0;
	for (;i<=50;i++) {
		printf("%d:  %lu\n",i,fib_loop_ul(i));
	}
    return 0;
}
