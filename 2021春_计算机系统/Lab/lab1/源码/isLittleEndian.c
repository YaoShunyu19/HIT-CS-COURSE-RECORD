#include <stdio.h>
#include <stdlib.h>

typedef enum __bool { false = 0, true = 1, } bool;

bool isLittleEndian() {
	union isLittleEndian {
		int i;
		char c;
	}a;
	a.i=1;
	if (a.c==1) return true;
	return false;
}

int main() {
	printf("%d",isLittleEndian());
	return 0;
}
