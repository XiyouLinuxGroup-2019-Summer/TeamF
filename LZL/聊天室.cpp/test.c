#include<stdio.h>
#include<string.h>

char one[50]="111122";
char second[50]="111145";
char third[100];
int judge(char *a,char *b,char *c)
{
    
    if(strcmp(a,b)<=0)
    {
        strcat(c,a);
        strcat(c,b);
    }else
    {
        strcat(c,b);
        strcat(c,a);
    }
}
int main()
{
    judge(one,second,third);
    printf("%s\n",third);
}