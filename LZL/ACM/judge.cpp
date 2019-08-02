#include <cstdio>
#include <cstring>
#include <iostream>
#define LL long long
using namespace std;
int pre[151000];
int find(int x){
    return x==pre[x]?x:pre[x]=find(pre[x]);
}
void judge(int x,int y){
    int xx=find(x),yy=find(y);
    if(xx!=yy){
        pre[xx]=yy;
    }
}
LL t[151000];
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++){
        pre[i]=i;
    }
    for(int i=0;i<m;i++){
        int x,y;
        scanf("%d%d",&x,&y);
        judge(x,y);
    }
    for(int i=1;i<=n;i++){
        t[find(i)]++;
    }
    LL sum=0;
    for(int i=1;i<=n;i++){
        if(t[i]!=0&&t[i]!=1){
            sum+=(t[i]*(t[i]-1)/2);
        }
    }
    if(sum!=m){
        printf("NO\n");
    }
    else{
        printf("YES\n");
    }
    return 0;
}
