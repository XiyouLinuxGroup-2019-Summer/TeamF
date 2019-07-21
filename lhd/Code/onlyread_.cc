//只读算法
//学习闭包的概念
#include<iostream>
#include<numeric>
#include<vector>
#include<algorithm>
using namespace std;
int main()
{
     vector<int> a;
     a.push_back(1);
     a.push_back(2);
     a.push_back(3);
     vector<int> b ;
     b.push_back(1);
     b.push_back(2);
     b.push_back(3);
     int sum = accumulate(a.cbegin(),a.cend(),0);
     cout << sum  << endl;
     bool trus = equal(a.cbegin(),a.cend(),b.cbegin());
     cout << trus << endl;
    //accumualte 作为使用运算符和返回值的类型
     //fill表示把一个迭代器赋值给一个东西
     //不能在一个空容器中使用fill_n
    return 0;
}
