#include <stdio.h>
#include <string.h>

int main()
{
	char a[201];
	int k;
	int i,j=0;
	while(1)
	{
		fgets(a,201,stdin);
		k = strlen(a);
		a[k-1] = '\0';
		if(strcmp(a,"START")==0  && j==0)
		{
			j++;
			continue;
		}
		if(strcmp(a,"END")==0 && j==1)
		{
			j = 0;
			continue;
		}
		if(strcmp(a,"ENDOFINPUT")==0 && j == 0)
			break;
		k = strlen(a);
		for(i=0;i<k;i++)
		{
			if(a[i] == 'A')
				a[i] = 'V';
			else if(a[i] == 'B')
				a[i] = 'W';
			else if(a[i] == 'C')
				a[i] = 'X';
			else if(a[i] == 'D')
				a[i] = 'Y';
			else if(a[i] == 'E')
				a[i] = 'Z';
			else if(a[i]>90 || a[i]<65)
				continue;
			else if(a[i]>=70 && a[i]<=90)
				a[i] = a[i]-5;
		}
		if(i==k)
			puts(a);
	}
	return 0;
}
