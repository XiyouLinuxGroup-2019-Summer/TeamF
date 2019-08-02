#include<iostream>
#include<algorithm>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cstring>
#include<unordered_map>
using namespace std;

int pre[150000+10];
int m,n;
int tmpa,tmpb;
int a,b;
int find(int i)
{
    return pre[i]==i?i:pre[i]=find(pre[i]);
}
int tag[150000+10];
unordered_map<int,int>mp;
int main()
{
    cin >> m >> n;
    for(int i=1;i<=m;i++)
    {
        pre[i]=i;
    }
    for(int i=0;i<n;i++)
    {
        cin >> tmpa >> tmpb;
        a=find(tmpa);
        b=find(tmpb);
        if(a!=b)
        {
            pre[a]=b;
        }
    }
    for(int i=1;i<=m;i++)
    {
/*         if(mp.find(pre[i])==mp.end())
        {
            mp.emplace(make_pair(pre[i],1));
        }else
        {
            mp[pre[i]]++;
        } */
        tag[pre[i]]++;
    }
    long long ans=0;
    int tmp=0;
    int dd=0;
/*     for(const auto &x : mp)
    {
        dd++;
        tmp=x.second;
        ans+=(((tmp*(tmp-1))/2));
    } */
    for(int i=1;i<=m;i++)
    {
        cout << pre[i] << endl;
        if(tag[pre[i]]!=0 && tag[pre[i]]!=1)
        ans+=(((tag[pre[i]]*(tag[pre[i]]-1))/2));
    }
    if(n==ans)
    cout << "YES\n";
    else 
    cout << "NO\n";
    return 0;
}