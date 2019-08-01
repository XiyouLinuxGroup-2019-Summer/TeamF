// 迷宫是通过
// 一个二维矩阵表示的， 每个元素的值的含义如下 0-墙， 1-路， 2-探
// 险家的起始位置， 3-迷宫的出口， 大写字母-门， 小写字母-对应大写
// 字母所代表的门的钥匙。 （ 题目来源： 拼多多面试
#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

int main()
{
    int M,N;
    cin >> M >> N;
    char map[M][N];
    pair<int,int> a1; // 起始位置
    for(int i = 0 ; i < M ;i++)
    {
        for(int j = 0 ;j < N ; j++)
        {
            cin >> map[i][j];
            if(map[i][j] == '2')
            {
                a1.first = i;
                a1.second = j;
            }    
        }
    }
    return 0;
}
