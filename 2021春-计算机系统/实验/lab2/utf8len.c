#include <stdio.h>
#include <stdlib.h>

int utf8len(char *s)
{
    int res=0;
    while (*s != '\0') {
    	res++;
        if ((*s & 0x80) == 0) {
        	s++;
		} 
        else {
        	while ((*s & 0x40) != 0) {
        		s++;
			}
			s++;
		}
    }
    return res;
}
int main()
{
    char str[1000];
    scanf("%s",str);
    char *p=str;
    int length=utf8len(p);
    printf("length=%d",length);
    return 0;
}
