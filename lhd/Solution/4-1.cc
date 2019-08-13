// 现在我们需要查出一些作弊的问答社区中的 ID， 作弊有两种： 1.A
// 回答了 B 的问题， 同时 B 回答了 A 的问题。 那么 A 和 B 都是作弊。 2.
// 作弊 ID 用户 A 和作弊 ID 用户 B 同时回答了 C 的问题， 那么 C 也是作
// 弊。 已知每个用户的 ID 是一串数字， 一个问题可能有多个人回答。
// （ 题目来源： 搜狗面试题）
//3
//1 1 2
//2 1 1
//3 2 1 2
//3
// 1 2 3
//两种状态回调函数没有回答
#include<iostream>
#include<vector>
#include<map>
#include<set> //set　里面存放的是犯罪Ｉｄ
//维护一个有向连接图， ij 要等于 ji 说明作弊
int main()
{
    int N,p;
    int count = 0;
    std::map<int,std::vector<int>> a1;
    std::set<int> test;
    scanf("%d",&N);
    while(N != 0)
    {
        int questtionID  = 0;
        int answernumber = 0;
        std::cin >> questtionID >> answernumber; // 输入问题ID和回答的人数
        for(int i = 0; i < answernumber ; i++)
        {
            std::cin >> p;
            a1[questtionID].push_back(p);
        }
        N--;
    }
    for(auto &c : a1)
    {
        for(auto &g : c.second)
        {
            if(a1[c.first][g] == a1[g][c.first])
            {
                test.insert(c.first);
                test.insert(g);
            }
            if(test.find(g) != test.end())
            {
                count++;
            }
           
        }
        if(count > 2)
        {
            test.insert(c.first);
        }
    }
    for(auto &b : test)
    {
        std::cout << b << std::endl;
    }
    return 0;
}









