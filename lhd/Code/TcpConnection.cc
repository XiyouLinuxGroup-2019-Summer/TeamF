#include"TCPConnection.hpp"
void TCPConnection::shutdown ()
{
    //use compare and  swap 状态机的交换
    if(state_ == KConnected) // 已经连接的状态
    {
        setState(Disconnecting) ; //改变状态 
        //shared_from_this 
        loop->runInLoop(std::bind(&TCPConnection->shutdownInloop,this));
        //事件循环函数，传递进入this 当前的函数指针
    }   
}
void TCPConnection::shutdownInloop ()
{
    loop->assertInLoopThread(); //事件是不是在当前线程中执行
    //判断有没有正在进行写操作
    //如果没有在进行写操作的话，现在对于写端发送FIN ，来进行关系操作
  //  if(////)
    {
        ////
    }
}
//send 发送数据，也让其在IO线程中进行调用，sendInloop事件循环之中
//如果message 如果有性能的损耗 可以使用移动语义 std::move 等操作
void TCPConnection::send(const std::string & message) // 传递进来左值引用
{
    if(state_ == KConnected) // 状态机的转化 // 如果在连接的状态之下
    {
        // 判断是不是在当前的进程之中
        //首先尝试发送部分数据，如果一次可以发送完毕，不启用后来的回调函数
        //不然关注socket writeable 可写事件，可以发送剩余的数据
        //不然会造成数据乱序
    }
    else
    {
        loop_->runInLoop(std::bind(&TcpConnection::sendInloop,this,message));
    }
}

void TCPConnection::sendInloop (const std::string & message)
{
    loop_->assertInLoopThread();
    ssize_t nwrote = 0 ;
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0 )
    {
        nwrote == ::write(channel_->fd(), message.data(),message.size()) ;
        if(nwrote >= 0 )
        {
            if(implicit_cast <size_t> (nwrote ) < message.size())
            {
                 //... 类型转换比较，没有发送完之后会重新发送内容
            }
        }
        else // 说明发送失败
        {
            // 重新恢复相应的值，并且写出错误
        }   
    }
    assert(nwrote >= 0) ;
    if(implicit_cast<size_t> (nwrote) < message.size() ) // 如果缓冲区中有内容所以我们应该等待添加到缓冲区的数据之后
    {

    }
}
