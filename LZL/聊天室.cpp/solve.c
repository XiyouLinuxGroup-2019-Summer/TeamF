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
#include<mysql/mysql.h>
#include"Data.h"

void send_data(int conn_fd,const char *string) //传入一个连接套接字和字符串数据   
{
    if(send(conn_fd,string,strlen(string),0)<0)
    {
        perror("send");
        exit(1);
    }
}

int login(recv_t *sock,MYSQL *mysql)  //sock_fd是要被发送数据的套接字
{
    int ret;
    char recv_buf[MAX_USERNAME];//登录时默认使用字符串
    int flag_recv=USERNAME;
    char buf[256];
    sprintf(buf,"select *from Data where Account = %s",sock->send_Account);
    mysql_query(mysql,buf);
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row;
    row=mysql_fetch_row(result);
    printf("%s || %s\n",sock->message,row[1]);
    if(!strcmp(sock->message,row[1]))//在数据库中检测账号密码是否匹配 返回名称　密码在message中
    {
        send_data(sock->send_fd,row[3]);//发送名称
        sprintf(buf,"update Data set status = \"1\" where Account = \"%s\"",sock->send_Account);
        mysql_query(mysql,buf); //改变登录状态
    }
    else 
    send_data(sock->send_fd,"@@@");//错误的请求
}

int register_server(recv_t * sock,MYSQL *mysql)
{
    char account[MAX_ACCOUNT];
    char buf[256];
    memset(account,0,sizeof(account));
    mysql_query(mysql,"select *from Account");
    //perror("error in mysql_query\n");
    MYSQL_RES *result = mysql_store_result(mysql);
    MYSQL_ROW row=mysql_fetch_row(result);
    //itoa(row[0]+1,account,10);    //atoi字符串转数字
    //数字转化为字符串必须用sprintf itoa不标准
    sprintf(account,"%d",atoi(row[0])+1);
    sprintf(buf,"update Account set Account = \"%s\" where Account = \"%s\"",account,row[0]);
    mysql_query(mysql,buf);
    send_data(sock->send_fd,account);//注册时返回一个账号                                       //存一次昵称
    sprintf(buf,"insert into Data values('%s','%s','%s','%s',0,%d)",account,sock->message,sock->message_tmp,sock->recv_Acount,sock->send_fd);
    printf("%s\n",buf);
    mysql_query(mysql,buf);
    mysql_free_result(result);
}

int *solve(void *arg)
{
    MYSQL mysql;
    mysql_init(&mysql);  //初始化一个句柄
    mysql_library_init(0,NULL,NULL);//初始化数据库
    mysql_real_connect(&mysql,"127.0.0.1","root","lzl213260C","Login_Data",0,NULL,0);//连接数据库
    mysql_set_character_set(&mysql,"utf8");//调整为中文字符
    recv_t *recv_buf=(recv_t *)arg;
    int recv_flag=recv_buf->type;
    switch (recv_flag)
    {
        case LOGIN :
            login(recv_buf,&mysql);
            break;
        case REGISTER :
            register_server(recv_buf,&mysql);
            break;
        default:
            printf("error\n");
            break;
    }
    printf("end of pthread!\n");
    struct epoll_event ev;
    ev.data.fd = recv_buf->conn_fd;
    ev.events = EPOLLIN | EPOLLONESHOT;
    //设置这个的目的是客户端在挂掉以后会发送一个信息　LT模式下没有接到包会不停的发　就会导致服务器epoll收到很多消息
    //解决方案是开始时事件类型改为那三个　然后设置EPOLLONESHOT　一个套接字只接受一次信息　在线程中在加上即可
    epoll_ctl(recv_buf->epfd, EPOLL_CTL_MOD,recv_buf->conn_fd, &ev);
    mysql_close(&mysql);
    free(recv_buf);
}