// 小云正在参与开发一个即时聊天工具， 他负责其中的会话列表部
// 分。 当用户在一个会话中发送或接收信息时， 如果该会话已经在会
// 话列表中， 则会从原来的位置移到列表的最上方； 如果没有在会话列
// 表中， 则在会话列表最上方插入该会话。 小云在现在要做的工作是
// 测试， 他会先把会话列表清空等待接收信息。 当接收完大量来自不同
// 会话的信息后， 就输出当前的会话列表， 以检查其中是否有 bug。（题
// 目来源： 网易游戏面试题）
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;
int main()
{
    int N,number;
    std::cin >> N;
    while(std::cin >> number  && N != 0)
    {
        std::vector<int> vec;
        std::vector<int> se;
        for(int i = 0 ; i< number ;i++)
        {
            int p = 0 ;
            std::cin >> p;
            vec.push_back(p);
        }
        for(int j = vec.size() - 1 ; j >= 0 ;j--)
        {
            std::vector<int>::iterator it = find(se.begin(),se.end(),vec[j]);
            if(it == se.end())
            {
                se.push_back(vec[j]);
            }
        }
        for(auto &c : se)
        {
            std::cout << c << " ";
        }
        std::endl;
        --N;
    }
    return 0;
}
