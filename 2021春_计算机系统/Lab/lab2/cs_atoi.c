#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[100];
	scanf("%s",str);
	int len=0,res=0;
	char *p=str;
	while (*p != '\0') {
		res=res*10+((int)*p-48);
		p++;
	}
	printf("%d",res);
    return 0;
}
