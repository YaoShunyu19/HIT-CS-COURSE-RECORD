#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[100];
	scanf("%s",str);
	float res=0;
	char *p=str;
	while (*p != '\0') {
		if (*p == '.') break;
		res=res*10+((int)*p-48);
		p++;
	}
	float c=0.1;
	if (*p == '.') {
		p++;
		while (*p !='\0') {
			res+=c*((int)*p-48);
			c/=10;;
			p++;
		}
	}
	printf("%f",res);
    return 0;
}
