#include"EventLoop.hpp"
#include<thread>
void threadFunc()
{
    printf("pid = %d , tid = %d\n",getpid(),CurrentThread::tid());
    EventLoop loop;
    loop.loop();
}
int main()
{
    printf("pid = %d , tid = %d\n",getpid(),CurrentThread::tid());
    EventLoop loop;

    std::thread t1(threadFunc);
    loop.loop();
    t1.join();
    return 0;
}
