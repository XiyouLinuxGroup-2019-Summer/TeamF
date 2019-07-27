#ifndef Poller_HPP
#define Poller_HPP
#include<boost/noncopyable.hpp>
#include<vector>
#include"Channel.hpp"
//Poller class 是一个IO多路复用的一个封装，无须加锁
class Poller : boost::  noncopyable
{
    public:
     typedef std::vector<Channel* > Channellist;

     Poller(EventLoop * loop);
     ~Poller();
     Timestamp poll(int timeoutMs , Channellist* active);

    //Changes the interested I/O events
    //Must be Called in the loop threadId_
    //void  updateChannel(................);
};
#endif
