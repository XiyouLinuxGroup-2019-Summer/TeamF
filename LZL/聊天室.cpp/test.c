#include<stdio.h>
#include<stdlib.h>
char row[200]="111";
char account[200];
int main()
{
    sprintf(account,"%d",atoi(row)+1);
    printf("%s\n",account);
}