#include<iostream>
#include<string>
#include<functional>
using namespace std;
using namespace placeholders;
void test(int i , double d,const string &s)
{
    cout << "i = " << i << "d = " << d << " s = " << s << endl;
    i++; 
}
int  test1(int i , double d,const string &s)
{
    cout << "i = " << i << "d = " << d << " s = " << s << endl; 
    return i;
}
int main()
{
    // function<void(void) > fp;
    // string s = "f";
    // int a = 3;
    // double b = 2.3;
    // fp = bind(&test,a,b,s); // ref 可以在模板传递参数的时候传递引用，不然无法传递,相当于一次引用，会修改当前的值
    // fp();
    // cout << a << endl;
    function 
    < void(int , double,const string&)> fp1;
    fp1 = bind(test,_1,_2,_3);
    fp1(4,7.1,"kitte");
    return 0;
}
