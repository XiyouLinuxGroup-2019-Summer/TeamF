//如果擦掉x中的字母可以得到y，说明y是x的子序列
#include<iostream>
#include<string>
#include<algorithm>
using namespace std;
class K
{
    public:
        void get()
        {     
            cin >> A;   
            string::iterator ps;
            unsigned int count = 0 ;
            while(A.size() > count)
            {
                ps = max_element(A.begin()+count,A.end()); //找出最大的元素在这个迭代器中
                A.erase(A.begin()+count,ps);
                count++;
            }
            cout << A << endl;
        }
    private:
        string A;
};
int main()
{
    K a1;
    a1.get();
    return 0;
}
