#include <stdio.h>
#include <stdlib.h>

int fib_dg_int(int i) {
	if (i==0) return 0;
	else if (i==1 || i==2) return 1;
	else return fib_dg_int(i-1)+fib_dg_int(i-2);
}
long fib_dg_long(int i) {
	if (i==0) return 0;
	else if (i==1 || i==2) return 1;
	else return fib_dg_long(i-1)+fib_dg_long(i-2);
}
unsigned int fib_dg_ui(int i) {
	if (i==0) return 0;
	else if (i==1 || i==2) return 1;
	else return fib_dg_ui(i-1)+fib_dg_ui(i-2);
} 
unsigned long fib_dg_ul(int i) {
	if (i==0) return 0;
	else if (i==1 || i==2) return 1;
	else return fib_dg_ul(i-1)+fib_dg_ul(i-2);
} 

int main()
{
	int i=0;
	for (;;i++) {
		printf("%d:  %lu\n",i,fib_dg_ul(i));
	}
    return 0;
}