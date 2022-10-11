#include <stdio.h>
#include <stdlib.h>
#define N 100000

int main() {
	FILE *fp = fopen("hello.c","r");
	char str[N+1];
	int i=0;
	char ch;
	while ((ch=fgetc(fp))!=EOF) {
		str[i]=ch;
		i++;
	}
	

	int sum=i;
	
	
	i=0;
	int index=0;
	while (i<sum) {
		if (str[i]=='\n') {
			printf("\\n ");
		}
		else if (str[i]=='\t') {printf("\\t ");}
		else printf(" %c ",str[i]);
		index++;
		i++;
		if (index==16) {
			index=0;
			i-=16;
			printf("\n");
			int j;
			for (j=0;j<16;j++,i++)  printf("%.2x ",str[i]);
			printf("\n");
		}
		if (i==sum) {
			printf("\n");
			i-=index;
			int j;
			for (j=0;j<index;j++,i++)  printf("%.2x ",str[i]);
			printf("\n");
		}
	}
	
	fclose(fp);

	return 0;
}
