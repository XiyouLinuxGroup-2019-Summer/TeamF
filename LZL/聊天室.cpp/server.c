#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<pthread.h>
#include"solve.h"

int main()
{
    int sock_fd,conn_fd;
    int optval;
    int flag_recv=USERNAME;
    int name_num;
    pid_t pid;
    socklen_t cli_len;
    struct sockaddr_in cli_addr,serv_addr;
    size_t ret;
    int connect_size=0;
    pthread_t pth1;
    recv_t recv_buf;

    int epfd,nfds;
    struct epoll_event ev,events[EVENTS_MAX_SIZE];

    sock_fd=socket(AF_INET,SOCK_STREAM,0);  //服务器套接字
    if(sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    optval=1;            //通用套接字　　socket退出后可正常连接　待设置的套接字选项的值及其大小
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int))<0)
    {
        perror("setsocket\n");
        exit(1);
    }
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET;      //协议族 ipv4 tcp/ip
    serv_addr.sin_port=htons(SERV_POT);//服务器端口
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); //IP地址
    
    if(bind(sock_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))<0)
    {
        perror("bind\n");
        exit(1);
    }

    if(listen(sock_fd,LISTENQ)<0)
    {
        perror("listen\n");
        exit(1);
    }

    epfd=epoll_create(EVENTS_MAX_SIZE);
    ev.data.fd= sock_fd;
    ev.events =EPOLLIN;     //设置为监听读的状态
    //使用默认的LT模式
    epoll_ctl(epfd,EPOLL_CTL_ADD,sock_fd,&ev);
    connect_size++;
    for(;;)
    {   
        nfds = epoll_wait(epfd,events,EVENTS_MAX_SIZE,-1);
        for(int i=0;i<nfds;i++)
        {
            connect_size++;
            if(events[i].data.fd==sock_fd)       //服务器套接字
            {
                if(connect_size>MAX_CONTECT_SIZE)
                {
                    perror("到达最大连接数!\n");
                    continue;
                }

                conn_fd=accept(sock_fd,(struct sokcaddr*)&cli_addr,&cli_len);
                //网络字节序转换成字符串输出
                printf("accept a new client ! ip:%s\n",inet_ntoa(cli_addr.sin_addr));

                if(conn_fd<=0)
                {
                    perror("error in accept\n");
                    continue;
                }
                ev.data.fd= sock_fd;
                ev.events =EPOLLIN;  //设置事件可写
                epoll_ctl(epfd,EPOLL_CTL_ADD,conn_fd,&ev); //新增服务器套接字

            }else if(events[i].events & EPOLLIN)  //接收到可写事件
            {
                if((ret=recv(events[i].data.fd,&recv_buf,sizeof(recv_buf),0))<0)
                //包的格式已经提前制定好
                {
                    perror("recv\n");
                    continue;
                }
                recv_buf.send_fd = events[i].data.fd;   //发送者的套接字已经改变
                pth1=pthread_create(&pth1,NULL,solve,&recv_buf);
            }  
        }
    }
    close(sock_fd);
    //还要关闭其他打开的套接字　
}