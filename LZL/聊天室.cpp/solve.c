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

int *solve(void *arg)
{
    recv_t *recv_buf=(recv_t *)arg;
    int recv_flag=recv_buf->type;
    switch (recv_flag)
    {
        case LOGIN :
            login(recv_buf->send_fd,"have a people login!");
            break;
        default:
            break;
    }
    return 0;
}

void send_data(int conn_fd,const char *string) //传入一个连接套接字和字符串数据
{
    if(send(conn_fd,string,strlen(string),0)<0)
    {
        perror("send\n");
        exit(1);
    }
}

int login(int sock_fd,const char *string)  //sock_fd是要发送数据的套接字
{
    int ret;
    recv_t recv_buf;
    int flag_recv=USERNAME;
    int ans=3;
    printf("%s\n",string);
    while(1)
    {
        if((ret=recv(sock_fd,&recv_buf,sizeof(recv_buf),0))<0)
        {
            perror("error in recv\n");
            return 0;  //错误退出
        }
        if(flag_recv==USERNAME)
        {
            //数据库中寻找数据
            if("找到数据")   //伪代码
            {
                send_data(sock_fd,"y\n");
                flag_recv=PASSWORD;
            }else if("未找到数据")
            {
                exit(1);
            }
        }else  
        {
            if("根据账号查找密码是否正确")
            {
                send_data(sock_fd,"y\n");
                send_data(sock_fd,"welcome to zhaolong's chat!\n");
                printf("%s login!"); //使用用户名
                break;
            }else if(ans)
            {
                send_data(sock_fd,"password error\n");
                ans--;
            }
            if(!ans)
            {
                send_data(sock_fd,"please enter again\n");//超过三次错误
                break;
            }
        }
    }
}