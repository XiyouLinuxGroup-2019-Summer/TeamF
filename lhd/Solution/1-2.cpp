#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
class K
{
    public:
        void init()
        {
          cin >> n ;
          int s  = 0;
          for(int i = 0 ; i < n ;i ++)
          {
            cin >> s;
            a.push_back(s);
          }
        }
        void number()
        {
            sort(a.rbegin(),a.rend());
            int sumA= 0,sumB = 0 ;
            for(vector<int>::iterator it  = a.begin() ; it != a.end() ; it++)
            {
                sumA += *it;
                it++;
                if(it == a.end())
                {
                    break;
                }
                sumB += *it;
            }
            cout << sumA - sumB<< endl;
        }
    private:
        int n; // 表示纸牌的数量
        vector<int> a;
};
int main()
{
    K a1;
    a1.init();
    a1.number();
    return 0;
}
