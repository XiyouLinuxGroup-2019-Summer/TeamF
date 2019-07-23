//经典动态规划求(n++,n*2)
#include<iostream>
#include<algorithm>
#include<string.h>
#include<cmath>
#include<vector>
using namespace std;
class K
{
    public:
        K():n(0)
        {

        }
        ~K(){};
        int  get()
        {
            
            cin >> n;
            
            if(n < 0)
                n = abs(n);
            if(n < 2 )
                return n;
            vector<int> dp(n+1);
            dp[0] = 0;
            dp[1] = 1;
            for(int i = 2 ; i<= n ;i++)
            {
                if(i % 2 == 0)
                {
                    dp[i] = 1 + min(dp[i-1] , dp[i/2]); 
                }
                else
                {
                    dp[i] = 1 + min(dp[i-1],1 + dp[(i+1) / 2]);
                }
            }
            return dp[n];      
        }
    private:
        int n;
};
int main()
{
    K a1;
    int k = a1.get();
    cout <<  k << endl;
}
