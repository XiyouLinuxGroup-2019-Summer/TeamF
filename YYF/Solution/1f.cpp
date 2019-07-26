#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main()
{
	int n;
	cin>>n;
	int i;
	char day[5];
	int dayd=0;
	int year;
	int month;
	char name[7];
	for(i=0;i<n;i++)
	{
		dayd=0;
		scanf("%s %s %d",day,name,&year);
		int k = strlen(day);
		while(--k)
		{
			if(k==2)
				dayd += day[k-1];
			else if(k==1 && dayd != 0)
		       		dayd += day[k-1]*10;
		 	else
				dayd += day[k-1];	
		}
		if(strcmp(name,"no")==0)
			dayd += 20;
		if(strcmp(name,"zip")==0)
			dayd += 40;
		if(strcmp(name,"zotz")==0)
			dayd += 60;
		if(strcmp(name,"tzec")==0)
			dayd += 80;
		if(strcmp(name,"xul")==0)
			dayd += 100;
		if(strcmp(name,"yoxkin")==0)
			dayd += 120;
		if(strcmp(name,"mol")==0)
			dayd += 140;
		if(strcmp(name,"chen")==0)
			dayd+=160;
		if(strcmp(name,"yax")==0)
			dayd+=180;
		if(strcmp(name,"zac")==0)
			dayd+=200;
		if(strcmp(name,"ceh")==0)
			dayd+=220;
		if(strcmp(name,"mac")==0)
			dayd+=240;
		if(strcmp(name,"kankin")==0)
			dayd+=260;
		if(strcmp(name,"muan")==0)
			dayd+=280;
		if(strcmp(name,"pax")==0)
			dayd+=300;
		if(strcmp(name,"koyab")==0)
			dayd+=320;
		if(strcmp(name,"cumhu")==0)
			dayd+=340;
		if(strcmp(name,"uayet")==0)
			dayd+=360;
		dayd += year*365;
		year = dayd/260;
		dayd = dayd%260;
		month = dayd/20;
		dayd = dayd%20;
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
		if(dayd==0)
			printf("%d %s %d",month,"",year);
	}
}
