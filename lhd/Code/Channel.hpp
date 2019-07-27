#ifndef Channel_HPP
#define Channel_HPP
#include"EventLoop.hpp"
//#include<boost/noncopyable.hpp>
#include "muduo/base/Timestamp.h"
#include<functional>
using namespace muduo;
//每一个Channel 都只是属于一个Eventloop ,对于每一个Channel 对象来说
//都只是属于一个IO线程，也不会在析构函数负责对于一个文件描述符的IO时间的分法
class Channel
{
    public:
        typedef std::function<void()> EventCallback;
        typedef std::function<void(Timestamp)> ReadEventCallback;
        Channel(EventLoop * loop, int fd);

        void handleEvent();
        void SetReadCallback(const EventCallback &cb)
        { readCallback_ = cb;}
        void SetWriteCallback(const EventCallback &cb)
        {
            writeCallback_ = cb;
        }
    private:
        Channel & operator=(const Channel &) = delete;
        Channel( const Channel& ) = delete;
        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvent;
        EventLoop* loop_;
        const int  fd_;
        int        events_;
        int        revents_; // it's the received event types of epoll or poll
        int        index_; // used by Poller.
        bool       logHup_;
        std::weak_ptr<void> tie_;
        bool tied_;
        bool eventHandling_;
        bool addedToLoop_;
        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback closeCallback_;
        EventCallback errorCallback_;
};
#endif // !CHANNEL_HPP
