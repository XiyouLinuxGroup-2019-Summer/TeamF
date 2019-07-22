#include<iostream>

using namespace std;

class solution
{
public:
    int fun(int c,int n){
        if(c==1){
            return n;
        }
        int sta,end,mid;
        sta=1;end=n;
        while(sta<end && sta+1!=end){
            mid=(sta+end)/2;
            int i=index(mid,c);
            if(i==n){
                return mid;
            }
            else if(i>n){
                end=mid-1;
            }
            else{
                sta=mid;
            }
        }
        if(index(end,c)<=n){
             return end;
        }
        return sta;     
    }
    int index(int first,int c){
        int sum=0;
        for(int i=0;i<c;i++){
            sum=sum+first;
            first=(first+1)/2;
        }
        return sum;
    } 

};

int main(){
    int n,c;
    cin>>c>>n;
    solution a;
    cout<<a.fun(c,n)<<endl;
    return 0;
}
