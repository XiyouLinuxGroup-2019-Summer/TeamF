#ifndef TCPConnection_HPP
#define TCPConnection_HPP
#include<memory>
//#include<muduo/net/TcpConnection.h>
#include<boost/noncopyable.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<muduo/net/EventLoop.h>
#include<boost/scoped_ptr.hpp>
#include<muduo/net/Channel.h>
#include<muduo/net/InetAddress.h>
#include<string>
class TCPConnection;

typedef std::shared_ptr<TCPConnection> TCPConnectionPtr;
class TCPConnection : boost::noncopyable , public boost::enable_shared_from_this<TCPConnection>
{
 //KConnectiong 和 KConnected    Socket  会析构函数关闭close(fd)
    private:
    enum StateE {kConnecting ,KConnected,DisConnected,KDisconnected} ;

    void setState(StateE s) { state_ = s;}
    void handleRead();
    muduo::net::EventLoop * loop_;
    StateE state_; 
   // boost:scoped_ptr<Socket> socket_; // 使用scoped_ptr 来进行管理socket
   //boost::scoped_ptr < channel > // 分发io事件，每一个事件循环，和一个channel联系
   //一起
   void send(const void * message,size_t len);//
   void send(const std::string & message);
   void shutdown();
   //thread safe thing 线程安全事件  // 把实际工作放入到shutdownInloop()之中来做，后者保证其在IO线程中进行调用
   void handleClose() ; // 事件处理函数
   void handleError() ; //事件处理函数
   void sendInloop(const std::string & message);
   void shutdownInloop();

   // Buffer intpuBuffer_;
   // Buffer outputBuffer_;
};


#endif // !TCPConnection_HPP

