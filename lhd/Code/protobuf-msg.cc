#include<assert.h>
#include <iostream>
#include "msg.pb.h"
using namespace std;
int main()
{
    test::student student_;

    std::cout << "类所占用的字节数 =  "  << student_.ByteSize() << std::endl;
    
    //
    student_.set_stamp(123); // 生成的api都是小写

    //消息中的自定义类型，并没有set 接口，通过mutable_*接口
    //来得到对应指针进而执行操作
    auto usrLoginPtr = student_.mutable_userlogin();
    //set_ .. 表示赋值操作
    auto usrInfoPtr = student_.mutable_usrinfo();

    string name("lhd");
    string passwd("12345");
    usrLoginPtr->set_username(name);
    usrLoginPtr->set_passwd(passwd);
    
    string id("24");
    usrInfoPtr->set_classroom(5);
    usrInfoPtr->set_studentid(id);
    for(int i  = 0 ; i < 5 ; ++i)
    {
        //对于repeated的类型,add_*（）接口
        //添加一个数据的指针，用来做相关的操作
        ////这里只是使用一个int 
        student_.add_array(i);
    }
    return 0;
}

