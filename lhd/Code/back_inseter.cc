//输出迭代器back_inserter
#include<iterator>
#include<iostream>
#include<vector>
using namespace std;
//back_inseter  接受一个指向容器的引用，返回一个与容器绑定的插入迭代器
int main()
{
    vector<int> vec;
    auto it  = back_inserter(vec); //插入迭代器
    *it = 42;
    it++;
    *it = 45;
    cout << vec[0] << vec[1] <<  endl;
    int a1[] = {0,1,2,3,4};
    int a2[5];
    auto ret = copy(begin(a1),end(a1),a2); // copy 赋值函数 第三个为起始范围
    //replace_copy(begin,end,back_inserter,0,42)//赋值函数 
    //unique 重新排列数据
    cout << a2[2] << endl;
    return 0;
}
