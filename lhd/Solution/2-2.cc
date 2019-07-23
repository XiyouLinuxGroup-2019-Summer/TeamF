//字符串匹配
#include<iostream>
#include<algorithm>
#include<numeric>
#include<string>
#include<vector>
using namespace std;
class K
{
    public:
        int sum()
        {
           vector<string> s1;
           int sum = 0 ,flag = 0;
           cin >> A >>B;
           for(size_t i  = 0 ; i < A.size() ; i++)
           {
               size_t  j = i + B.size() - 1;//切割字符串从A 开始需要切割多少，末尾
               if(j >= A.size())
                    continue;
               string  C = A.substr(i , j + 1 - i);
               s1.push_back(C);
           }  
           //进行去重操作
           sort(s1.begin(),s1.end());
           auto end_unique = unique(s1.begin() , s1.end());
           s1.erase(end_unique,s1.end());
           for(auto ed : s1)
           {
               flag = 0;
               for(size_t i = 0; i < B.size() ; i++)
               {
                   if(B[i] == '?')
                        continue;
                    if(B[i] != ed[i])
                    {
                        flag = 1;
                    }
               }
               if(flag == 0)
               {
                   sum++;
               }
           }       
           return sum;
        }
    private:
        string A;
        string B;
};
int main()
{
    K a1;
    int sum = a1.sum();
    cout << sum << endl;
    return 0;
}
