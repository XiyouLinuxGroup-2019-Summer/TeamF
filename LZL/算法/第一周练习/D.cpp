#include<cstdio>
#include<cstring>
#include<cstdio>
using namespace std;
 
int main()
{
    char a[210];
    char s[40]="VWXYZABCDEFGHIJKLMNOPQRSTU";
    memset(a,0,sizeof(a));
    while(1)
    {
        //gets(a);
        if(!strcmp(a,"START")) continue;
        if(!strcmp(a,"END")) continue;
        if(!strcmp(a,"ENDOFINPUT")) break;
        int l=strlen(a);
        for(int i=0; i<l; i++)
        {
            if(a[i]>='A'&&a[i]<='Z')
            {
                a[i]=s[a[i]-'A'];
            }
            else
            {
                continue;
            }
        }
        printf("%s\n",a);
    }
    return 0;
}