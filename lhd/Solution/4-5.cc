// 字符迷阵是一种经典的智力游戏。 玩家需要在给定的矩形的字符迷
// 阵中寻找特定的单词。
// 在这题的规则中， 单词是如下规定的：
// 1. 在字符迷阵中选取一个字符作为单词的开头；
// 2. 选取右方、 下方、 或右下 45 度方向作为单词的延伸方向；
// 3. 以开头的字符， 以选定的延伸方向， 把连续得到的若干字符拼接
// 在一起， 则称为一个单词。 
#include<iostream>
#include<string>
#include<vector>
using namespace std;

int main()
{
    int T;
    cin >> T;
    while(--T)
    {
        int m,n;
        cin >> m >> n; // 行列
        vector<string> svec(m);
        int count = 0;
        for(int i = 0 ; i < m ;i++)
        {
             cin >> svec[i];
        }
        string word; // 需要查找的单词
        cin >> word;
        for(int i = 0 ; i < m ; i++)
        {
            for(int j = 0 ; j < n ; j++)
            {
                if(svec[i][j] == word[0])
                {
                    int x = i , y = j , z = 0;
                    //行列分别匹配
                    while( y  < n && z < word.size())
                    {
                        if(svec[i][y] == word[z])
                        {
                            y++;
                            z++;
                        }
                        else
                        {
                            break;
                        }
                        if(z == word.size())
                        {
                            count++;
                            break;
                        }
                    }
                    x = i ;
                    y = j ;
                    z = 0 ;
                    while( x < n && z < word.size())
                    {
                         if(svec[x][j] == word[z])
                        {
                            x++;
                            z++;
                        }
                        else
                        {
                            break;
                        }
                         if(z == word.size())
                        {
                            count++;
                            break;
                        }
                    }
                     x = i, y = j, z = 0;
                     while (x < m && y < n && z < word.size())
                    {
                        if (svec[x][y] == word[z])
                        {
                            x++;
                            y++;
                            z++;
                        }
                        else
                        {
                            break;
                        }
                        if (z == word.size())
                        {
                            count++;
                            break;
                        }
                    }
                }
            }
        }
        std::cout << count << std::endl;
    }
    return 0;
}
