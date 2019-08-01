// j 的重量为 w[j]， 对于每个小朋友 i， 当他分到的巧克力大小达到 h[i]
// (即 w[j]>=h[i])， 他才会上去表演节目。 老师的目标是将巧克力分
// 发给孩子们， 使得最多的小孩上台表演。 可以保证每个 w[i]> 0 且不
// 能将多块巧克力分给一个孩子或将一块分给多个孩子。 （ 题目来源：
// 拼多多面试题）
// 输入描述:
// 第一行： n， 表示 h 数组元素个数
// 第二行： n 个 h 数组元素
// 第三行： m， 表示 w 数组元素个数
// 第四行： m 个 w 数组元素
// 输出描述:
// 上台表演学生人数
#include<iostream>
#include<algorithm>
using namespace std;
int main()
{
    int n,m;
    cin >> n;
    int *h = new int [n];
    for(int i = 0 ; i < n ; i++)
    {
        cin >> h[i]; // 人
    }
    cin >> m;
    int *w = new int [m];
    for(int i = 0 ; i < m ; i++)
    {
        cin >> w[i];//巧克力
    }
    int sum = 0;
    int j = 0 ;
    sort(h,h+n);
    sort(w,w+m);
    for(int i = 0 ;i< m;i++)
    {
        if(w[i]>=h[j] && i < n)
        {
            sum++;
            j++;
        }
    }
    cout << sum << endl;
    delete []h;
    delete []w;
    return 0;
}
