#include<muduo/net/TcpConnection.h>
#include<boost/noncopyable.hpp>
#include<memory>
#include<atomic>
#include<boost/scoped_ptr.hpp>
//Tcpserver为了管理TCPConnection
class TcpServer : boost::noncopyable
{
    public:
        TcpServer(EventLoop * loop , const InetAddress& listenAddr);
        ~TcpServer(); // force out-line dtor for scoped_ptr members
        // Starts the Server if it is not listening

        void start();

        //Set connection callback
        //Not thread safe
        void setNewConnetcionCallback(const ConnectionCallback & cb)
        {ConnectionCallback_ = cb ; }
        void setMessageCallback(const MessageCallback & cb)
        {
            MessageCallback_ = Cb;
        }
    private:
        void newConnection(int sockfd, const InetAddress& peerAddr);
        typdef std::map<std::string,TcpConnection> ConnectionMap;
        EventLoop * loop_;
        conat std::string name_;
        boost::scoped_ptr<Accptor> accetor_; //对于accptor 进行管理 接受新的TCP链接，并且通知使用者，内部的class 
        ConnectionCallback connectionCallback_; // 使用链接回调函数
        setMessageCallback setMessageCallback_; // Message 回调函数
        std::atomic<bool> started_;  // 开始结束标记
        int nextConnID_ ; //  
        ConnectionMap connections;
};
int main()
{
    return 0;
}
