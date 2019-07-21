#include<iostream>
#include<algorithm>
#include<string>
#include<map>
#include<vector>
#include<cstring>
using namespace std;
int m;
int tmpa,tmpb;
int main()
{
    cin >> m;
    while(m--)
    {
        cin >> tmpa >> tmpb;
        if(tmpa%(tmpb+1)==0)
        cout << "second\n";
        else 
        cout << "first\n";
    }
    return 0;
}