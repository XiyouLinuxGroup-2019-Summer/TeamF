#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
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
        perror("send");
        exit(1);
    }
}

int login(recv_t *sock,const char *string)  //sock_fd是要被发送数据的套接字
{
    int ret;
    char recv_buf[MAX_USERNAME];//登录时默认使用字符串
    int flag_recv=USERNAME;
    if(1) //在数据库中检测账号密码是否匹配 返回名称　密码在message中
    send_data(sock->send_fd,"zhaolonga");
    else 
    send_data(sock->send_fd,"@@@"); //错误的请求
}

int register_server(recv_t * sock)
{
    char account[MAX_ACCOUNT]="111111";
    if(1) //如果存在合法的未使用账号
    send_data(sock->send_fd,account);
    else 
    send_data(sock->send_fd,"@@@");  //错误的请求
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
        case REGISTER :
            register_server(recv_buf);
            break;
        default:
            printf("error\n");
            break;
    }
    printf("end of pthread!\n");
    struct epoll_event ev;
    ev.data.fd = recv_buf->conn_fd;
    ev.events = EPOLLIN | EPOLLONESHOT;
    epoll_ctl(recv_buf->epfd, EPOLL_CTL_MOD,recv_buf->conn_fd, &ev);
    free(recv_buf);
}