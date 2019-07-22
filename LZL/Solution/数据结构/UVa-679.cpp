//二叉树模拟
#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstdlib>
#include<cstring>
using namespace std;

int m,tmpa,tmpb,flag;
int book[1<<20];
int main()
{
    while(cin >> m)
    {
        if(m==-1)break;
        for(int i=0;i<m;i++)
        {
            cin >> tmpa >> tmpb;   //深度与小球个数
            memset(book,0,sizeof(book));
            while(tmpb--)
            {
                flag=1;
                while(1)
                {
                    book[flag]=!book[flag];
                    flag=book[flag]?(flag*2):flag*2+1;  //优雅的代码 
                    if(flag>(1<<tmpa-1)) break;
                }
            }
            cout << flag/2 << endl;
        }
    }
    return 0;
}