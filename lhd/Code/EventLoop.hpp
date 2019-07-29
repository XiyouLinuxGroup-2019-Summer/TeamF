#ifndef EventLoop_HPP
#define EventLoop_HPP
#include<iostream>
//#include<boost/noncopyable.hpp>
#include<muduo/net/EventLoop.h>
#include<muduo/base/CurrentThread.h>
#include<atomic>
#include<sys/poll.h>
using namespace muduo;
class EventLoop
{ 
    public:
    EventLoop() ;
    ~EventLoop();
    private:
     EventLoop( const EventLoop& ) = delete;
     EventLoop& operator=( const EventLoop& ) = delete;
    public:
     void loop();//运行创建的是一个IO线程，主要运行事件循环
     void assertInLoopThread()
     {
         if(!isInLoopThread()) //有没有在其他线程的时间循环之中
         {
             abortNotInLoopThread();
         }
     }
     bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }
     //记住本对象所属的线程
    static EventLoop *getEventLoopOfCurrentThread(); 
    EventLoop* startloop();
    private:
      const pid_t threadId_;
      void abortNotInLoopThread();
      std::atomic<bool> looping;
};
__thread EventLoop* t_loopINthisThread = 0;
EventLoop::EventLoop():looping(false),threadId_(CurrentThread::tid())
{
    if(!t_loopINthisThread)
    {
        t_loopINthisThread = this;
    }
}
EventLoop * EventLoop::startloop()
{
   // thread_.start();
   {
       {
           // MutexLockGurad lock(mutex_); // 实现RAII方式的对于资源的初始化
           //线程安全
        /*    while(loop_  == NULL) // 如果循环为空的话，一直等待
           {
               cond.wait();//条件变量
           }
        */
       }
   }
   return loop;
}
EventLoop * EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopINthisThread;
}
EventLoop::~EventLoop()
{
    assert(!looping);
    t_loopINthisThread = NULL;
}
void EventLoop::loop ()
{
    looping = true;
    ::poll(NULL,0,5 * 1000);

    std::cout << "EventLoop" << "this stop looping" << std::endl;
    looping = false;
}
#endif // DEBUG
