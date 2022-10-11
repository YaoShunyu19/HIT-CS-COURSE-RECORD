#include <stdio.h>
#include <stdlib.h>
#define e 1e-10
long long main()
{
    char str[100];
	float number;
	scanf("%f",&number);
	int n=0;
	int num=(int)number;
	float li=number-num;
	while (num!=0) {
		n++;
		num/=10;
	}
	int i=n-1;
	num=(int)number;
	for (;i>=0;i--){
		str[i]=(char)(num%10+48);
		num/=10;
	}
	str[n]='.';
	i=1;
	while (li>e) {
		li*=10.0;
		str[n+i]=(char)((int)li+48);
		li-=(int)li;
		i++;
	}
	str[n+i]='\0';
	printf("%s",str);
    return 0;
}
