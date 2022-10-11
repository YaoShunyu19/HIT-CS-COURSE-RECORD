#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[100];
	int number;
	scanf("%d",&number);
	int num=number;
	int len=0;
	while (num!=0) {
		num/=10;
		len++;
	}
	str[len]='\0';
	int i=len-1;
	for (;i>=0;i--) {
		str[i]=(char)(number%10+48);
		number/=10;
	}
	printf("%s",str);
    return 0;
}
