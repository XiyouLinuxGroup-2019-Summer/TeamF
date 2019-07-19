#include <stdio.h>

int main()
{
	int t;
	int n;
	int i,j,k,m,c,d;
	int cont;
	long b;
	long a[101];
	scanf("%d",&t);
	for(i=0;i<t;i++)
	{
		cont = 0;
		k = 0;
		scanf("%d",&n);
		for(j=0;j<n;j++)
		{
			scanf("%ld",&b);
			if(b%3==0 && b!=0)
			{
				cont++;	
				continue;
			}
			a[k] = b;
			k++;
		}
		for(j=0;j<k-1;j++)
		{
			for(m=j+1;m<k;m++)
			{
				if((a[j]+a[k])%3 == 0)
				{
					
				}
			}
		}
	}
}
