// 牛牛回答 1到n之间最小的数 
//求出最大公约数和n的成绩
//线性筛素数
#include<iostream>
#include<algorithm>
#include<numeric>
using namespace std;
class K
{
    public:
        long long int  get(int n) //防止溢出
        {
            auto sum = 0 ;
            auto b = 0 ;
            auto d = n;
            if(n == 1) 
                return  1;
            else
               {
                    b =  get(d-1);
                    sum = get(d-1) * d ;
                    auto c = ([&] () ->int {auto temp = n; while(n){temp = b % n;b = n ; n= temp;} return b;});
                    sum = sum / c() ;
               }
            return sum;
        }
};
int main()
{
    K a1;
    auto b = 0 ;
    cin >> b;
    auto c = a1.get(b);
    cout << c%987654321 << endl;
    return 0;
}
