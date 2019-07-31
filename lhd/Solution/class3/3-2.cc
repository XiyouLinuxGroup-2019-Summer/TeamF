// 有两个用字符串表示的非常大的大整数,算出他们的乘积， 也是用
// 字符串表示。 不能用系统自带的大整数类型。 （ 题目来源： 拼多多面
// 试题）
//使用分支法和循环法
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
using namespace std;
string f(string a, string b)
{
    int * sum1 = new int [a.size() + b.size()];
    for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < b.size(); j++) {
                sum1[i+j] += (a[a.size() -1 -i]-'0') * (b[b.size() - 1 - j]-'0');
            }
            for (int j = 0; j < (a.size()+b.size()) - 1 ; j++) {
                if(sum1[j]>9){
                    sum1[j+1]+=sum1[j]/10;
                    sum1[j] = sum1[j]%10;
                }
            }
        }
        string str = "";
        bool t = false;
        for (int i = (a.size() + b.size() -1); i >=0; i--) {
            if(sum1[i]!=0) t = true;
            if(t) str = str + to_string(sum1[i]);
        }
        return str;
}
int main()
{
    string a,b;
    cin >> a ; // 逐位进位保留
    cin >> b ;
    string c = f(a,b);
    cout << c <<endl;
    return 0;
}
