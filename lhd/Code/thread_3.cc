//异步线程 std::async启动一个异步任务，返回一个std::future对象
//这个对象持有最终计算出来的结果，需要的时候就调用一个这个对象的get（）成员函数
//阻塞到就绪之中
#include<future>
#include<iostream>
#include<thread>
//int find_to_anweer();
//void do_other_stuff();
int main()
{
    std::cout << "thread number   " << std::thread::hardware_concurrency() << std::endl;
    return 0;
    // std::future<int> the_answer = std::async(find_to_anweer);
    // do_other_stuff();
    // std::cout << "The answer is " << the_answer.get() << std::endl;
    //还可以向std::async 传递一个额外的参数，这个参数类型式std::launch
}
