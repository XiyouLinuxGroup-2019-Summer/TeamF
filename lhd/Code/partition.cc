//对于partition() 来进行重新排列，符合条件的元素放在首部，其他放在后面
#include<iostream>
#include<algorithm>
#include<iterator>
using namespace std;
int main()
{
   // std::cout<<" jjj" << std::endl;
    int a[] = {1,2,3,4,5,6};
    std::partition(a,a+6,[](int x){return x% 2 != 0;});
    std::copy(a,a+6,ostream_iterator<int>(std::cout ,"\n"));
    std::cout << std::endl;
    return 0;
}
