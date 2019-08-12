// A[n,m]是一个 n 行 m 列的矩阵， a[i,j]表示 A 的第 i 行 j 列的元
// 素， 定义 x[i,j]为 A 的第 i 行和第 j 列除了 a[i,j]之外所有元素(共
// n+m-2 个)的乘积， 即
// x[i,j]=a[i,1]*a[i,2]*...*a[i,j-1]*...*a[i,m]*a[1,j]*a[2,j]..
// .*a[i-1,j]*a[i+1,j]...*a[n,j],现输入非负整形的矩阵 A[n,m]，
// 求 MAX(x[i,j])， 即所有的 x[i,j]中的最大值。 （ 题目来源： 搜狗面
// 试题）
//动态规划
#include<iostream>
#include<vector>
#include<algorithm>
int main()
{
    int N,M;
    while(std::cin >> N >> M)
    {
        std::vector<std::vector<int>> vec(M,std::vector<int>(N,0));
        long max = 0; //遍历每一个除了她相乘的最大值，和max进行对比
        for(int i = 0 ; i< N; i++)
            for(int  j = 0 ; j < M ;j++)
             {
                 std::cin >> vec[i][j];
             }
        for(int i = 0 ; i < N ;i++)
        {
            for(int j = 0 ; j < M ; j++)
            {
                int row = 1;
                for(int l = 0 ; l < M ; l++)
                {
                    if(l != j)
                    {
                        row *=vec[i][l];
                    }
                }

                int ligh = 1;
                for(int r = 0 ;r < N ;r++)
                {
                    if(r != i)
                    {
                        ligh *= vec[i][j];
                    }
                }
                max = max > (ligh * row) ? max : (ligh * row);
            }
        }
        std::cout << max << std::endl;
    }
}
