#include<iostream>
#include<thread>
#include<mutex>
#include<list>
#include<algorithm>
//展示如何使用RAII方式来进行简化在析构函数中使用
class thread_guard
{
    
    public:
        explicit thread_guard(std::thread &t_) :t(t_)//防止隐式转化
         {}                       //只对有一个参数的类构造函数有效，类构造函数参数大于或等于两个，
         ~thread_guard()
         {
             if(t.joinable())
                t.join();
         }
    private:
        std::thread &t;
};
class scoped_thread
{
    std::thread t;
    public://把线程控制权串
        explicit scoped_thread(std::thread t_): t(std::move(t_))
        {

        }
};
std::list<int> some_list;
std::mutex some_mutex;
void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex); //线程安全
    some_list.push_back(new_value);
}
int main()
{
    
    return 0;
}
