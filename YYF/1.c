#include <stdio.h>

int main()
{
	char a[5] = "12";
	char *b = "34";
	sprintf(a,"%s%s",a,b);
	printf("%s",a);
}
