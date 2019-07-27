#ifndef Accptor_HPP
#define Accptor_HPP
#include<boost/noncopyable.hpp>
#include<functional>
#include<muduo/net/InetAddress.h>
#include"EventLoop.hpp"
#include<atomic>

//这个class 主要用来accept 新连接 ，并且可以通过回调函数来通知使用者
//内部class  //只是为了建立新的连接
// using namespace muduo;
// using namespace net;
class Accptor : boost::noncopyable
{
    public: 
        typedef  std::function<void (int sockfd, const muduo::net::InetAddress &) > NewconnectionCallback;
        Accptor(EventLoop * loop, const muduo::net::InetAddress & listenAddr);
        void setNewConnetcionCallback(const NewconnectionCallback cb)
        {
            NewconnectionCallback_ = cb;
        }
        bool listenning() const {
            return listenning_;
        }
        void listening();
    private:
        NewconnectionCallback NewconnectionCallback_;
        std::atomic<bool> listenning_;
        void handleRead();
        EventLoop* loop_;
        //Socket accpetSocket_; // RAII 方式来进行管理
       // Channel acceptChannel_; // 文件描述符的IO事件分发
        NewconnectionCallback NewconnectionCallback_;
};  // Socket 是RAII handle 所以是server socket , Channel 可以观察套接字上的readable时间
Accptor::Accptor(EventLoop * loop, const muduo::net::InetAddress & listenAddr)
: loop_(loop) , accpetSocket_(sockets::createNonblockingOrDie()),acceptChannel_(loop,acceptSocket_.fd()),listenning_(false)
{
    acceptSocket_.setReuseAddr(true);//设置地址
    acceptSocket_.bindAddress(listenAddr);// 绑定地址                           可读事件回调      //绑定地址
    acceptSocket_.setReadCallback(std::bind(&Accptor::handleRead,this));
    //类似于模板类函数，std::bind 在functional 中
}
void Accptor::listening()
{
    loop->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}
/* int sockets::createNonblockingOrDie() 创建一个非阻塞的套接字
{
    int sockfd = ::socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    return sockfd;
}
*/
void Accptor::handleRead()
{
    loop->assertInLoopThread();
    InetAddress peerAddr(0);
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd > = 0 )
    {
        if(NewconnectionCallback_)
        {
            NewconnectionCallback_(connfd,peerAddr);//接受的套机子和地址传递进去
        }
    }
    else
    {
        sock.close();
        //关闭socket套接字
    }
}
// 我们可以使用值语义，传递socket 对象过去使用move ，传递给回调函数，确保安全的释放
#endif // !Accptor_HPP
