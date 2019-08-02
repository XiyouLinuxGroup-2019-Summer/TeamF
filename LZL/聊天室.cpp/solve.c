#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<errno.h>
#include<pthread.h>
#include<stdio.h>
#include"Data.h"

void send_data(int conn_fd,const char *string) //传入一个连接套接字和字符串数据 
{
    if(send(conn_fd,string,strlen(string),0)<0)
    {
        perror("send\n");
        exit(1);
    }
}

int login(recv_t *sock,const char *string)  //sock_fd是要被发送数据的套接字
{
    int ret;
    char recv_buf[MAX_USERNAME];//登录时默认使用字符串
    int flag_recv=USERNAME;
    int ans=3;
    printf("%s\n",string);
    while(1)
    {
        if((ret=recv(sock->send_fd,&recv_buf,sizeof(recv_buf),0))<0)
        {
            perror("error in recv\n");
            return 0;  //错误退出
        }
        if(flag_recv==USERNAME)
        {
            //数据库中寻找数据
            if(1)   //伪代码  找到数据  测试登录
            {
                send_data(sock->send_fd,"y\n");
                flag_recv=PASSWORD;
            }else if("未找到数据")
            {
                exit(1);
            }
        }else  
        {
            if(1)  //伪代码　根据账号查找密码是否正确　测试登录
            {
                send_data(sock->send_fd,"y\n");
                send_data(sock->send_fd,"zhaolonga\n");//成功后应该发送一个姓名
                //后面可能还要发很多数据　这里先留着
                printf("aaa login!"); //使用用户名  测试登录
                break;
            }else if(ans)
            {
                send_data(sock->send_fd,"password error\n");
                ans--;
            }
            if(!ans)
            {
                send_data(sock->send_fd,"please enter again\n");//超过三次错误
                break;
            }
        }
    }
}

int *solve(void *arg)
{
    recv_t *recv_buf=(recv_t *)arg;
    int recv_flag=recv_buf->type;
    switch (recv_flag)
    {
        case LOGIN :
            login(recv_buf,"have a people login!");
            break;
        default:
            break;
    }
    return 0;
}