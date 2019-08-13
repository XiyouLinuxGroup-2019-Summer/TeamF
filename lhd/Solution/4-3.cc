// 小 W 有一个电子时钟用于显示时间， 显示的格式为 HH:MM:SS， HH，
// MM， SS 分别表示时， 分， 秒。 其中时的范围为[‘00’ ,‘01’ …‘ 23’ ]，
// 分的范围为[‘ 00’ ,‘ 01’ …‘ 59’ ]， 秒的范围为[‘ 00’ ,‘ 01’ …
// ‘ 59’ ]。
//贪心算法，从左边往右边遍历
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
int main()
{
    int N;
    std::vector<std::string> data;
    std::cin >> N;
    while(N != 0)
    {
        std::string temp;
        std::cin >> temp;
        data.emplace_back(temp);
        --N;
    }
    for( auto & c :data)
    {
        std::string temp1,temp2;
        temp1 = c.substr(0,c.find(':'));
        c = c.substr(c.find(":")+1,c.size() - 1);
        temp2 = c.substr(0,c.find(':'));
        c = c.substr(c.find(":") + 1,c.size()-1);
        if(temp1[0] > '2')
        {
            temp1[0] = '0';
        }
        if(temp2[0] > '5')
        {
            temp2[0] = '0';
        }
        if(c[0] > '5')
        {
            c[0] = '0';
        }
        std::cout << temp1 << ":" <<temp2 <<":" << c <<std::endl;
    }
    return 0;
}
