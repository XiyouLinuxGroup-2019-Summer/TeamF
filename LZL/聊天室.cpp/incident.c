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
char gl_account[MAX_ACCOUNT]; //以一个全局变量记录账号
int  fact_fd;

void *method_client(void *arg)
{
    recv_t buf;
    while(1)
    {
        if(recv(fact_fd,&buf,sizeof(recv_t),0)<0)
        perror("error in recv\n");
    }
}

int my_recv(int conn_fd,char *data_buf,int len)
{
    static char recv_buf[BUFSIZ];  //8192  
    static char *phread;
    static int len_remain = 0;
    int i;
    if(len_remain<=0) //能够第二次接着发　保存第一次没发完的数据  
    {
        if((len_remain=recv(conn_fd,recv_buf,sizeof(recv_buf),0))<0)
        {
            perror("recv\n");
            exit(1);
        }else if(len_remain==0){
            return 0;
        }
        phread=recv_buf;
    }
    for(i=0;*phread!='\n';i++)      //防止一次发送没有发送完　所以设置为static 
    {
        if(i>len) return 0;
        data_buf[i]=*phread;
        phread++;
        len_remain--;
    }
    len_remain--;    //回车结束符号
    phread++;        //为了与上面进行对应
    return i;
}

int get_userinfo(char *buf,int len)
{
    int i,c;
    if(buf==NULL)
    return -1;
    i=0;
    while((c=getchar())!='\n' && c!=EOF && i<=len-2)
    {
        buf[i++]=c;
    }
    buf[i++]='\0';
    return 0;
}

int input_userinfo(recv_t *temp)
{
    int flag_userinfo=0;
    char password[MAX_RECV];
    fflush(stdin);
    printf("account:");
    if(get_userinfo(gl_account,MAX_ACCOUNT)<0)
    {
        perror("get_userinfo\n");
        exit(1);
    }
    strcpy(temp->send_Account,gl_account);
    printf("password:");
    if(get_userinfo(password,MAX_PASSWORD)<0)
    {
        perror("get_userinfo\n");
        exit(1);
    }
    strcpy(temp->message,password);
}

int login_client(int conn_fd,char *username)
{
    fact_fd=conn_fd; //给全局变量赋值
    int              ret;
    recv_t           Package;
    Package.type   = LOGIN;
    Package.send_fd= conn_fd;
    char buf[MAX_RECV];
    int number=3;
    getchar();
    system("clear");
    while(number--)  //三次机会
    {
        input_userinfo(&Package);
        if(send(conn_fd,&Package,sizeof(recv_t),0)<0)//发送一个登录请求
        {
            perror("error in send\n");
            return 0;  //错误退出
        }
        //如果登录请求正确
        if((ret=my_recv(conn_fd,username,MAX_USERNAME))<0) //默认阻塞
        {
            perror("error in my_recv\n");
            exit(1);
        }
        //printf("%s::\n",username);
        if(username[0]==ERROR_IN_LOGIN)
        {
            perror("account or password error!\n");
            continue;
        }else break;
    }
    if(number==-1) return 0;

    //登录成功以后开始接收离线消息盒子里的消息
    my_recv(conn_fd,buf,MAX_RECV);
    if(!strcmp(buf,BOX_NO_MESSAGES))
    return 1;//登陆成功 离线消息盒子无记录　进入服务界面
    Box_t box;
    char buffer[32];
    while(1)
    {
        if(recv(conn_fd,&box,sizeof(box),0)<0)
        perror("error in recv\n");
        if(box.type==EOF_OF_BOX)
        break;
        printf("%s:\n%s\n",box.account,box.message); //显示离线发送来的信息
        if(box.type==ADD_FRIENDS)
        {
            printf("YES   [Y]    NO    [N]\n");
            scanf("%s",buffer);
            getchar();
            Package.type=ADD_FRIENDS_QUERY;
            strcpy(Package.message,buffer);

            //有发送者和接收者就可以确定好友关系　加入数据库
            strcpy(Package.recv_Acount,box.account);//发送者
            strcpy(Package.send_Account,gl_account);//接收者　//一种新的事件　epoll来判断
            if(send(conn_fd,&Package,sizeof(recv_t),0)<0)
            perror("error in send friend request\n"); //根据首字母判断
        }
    }


    return 1;  //登陆成功　进入服务界面
}

int register_client(int conn_fd,char *account)  //注册请求　返回一个账号
{
    int ret;
    recv_t           Package;
    Package.type   = REGISTER;
    Package.send_fd= conn_fd;
    char password[MAX_PASSWORD];
    char telephone[MAX_TELEPHONE];
    char nickname[MAX_USERNAME];
    system("clear");

    //收集注册信息
    printf("Welcome to register account!\n");
    printf("please enter your password,we will give your a unique account.\n");
    printf("password:");
    getchar();
    get_userinfo(password,MAX_PASSWORD);
    printf("Get it back for your password\n");
    printf("telephone:");
    get_userinfo(telephone,MAX_TELEPHONE);
    printf("please enter you nickname!\n");
    get_userinfo(nickname,MAX_USERNAME);
    strcpy(Package.recv_Acount,nickname);   //缓冲区不够了　这个用一下
    strcpy(Package.message_tmp,telephone);
    strcpy(Package.message,password);

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in register send\n");
        return 0;
    }
    if((ret=my_recv(conn_fd,account,MAX_ACCOUNT))<0)
    {
        perror("error in register send\n");
        return 0;
    }
    if(account[0]==ERROR_IN_LOGIN)
    {
        perror("error in server data\n");
        return 0;
    }
    printf("This is your Account ,Don't forget it!\n");
    printf("Account:%s\nPlease enter again!\n",account);
    printf("please enter enter key for quit!\n");
    getchar();
    return 1;   //发送正确且收到账号消息返回１
}

int Retrieve_client(int conn_fd)
{
    //变量声明
    int              ret=0;
    recv_t           Package;
    Package.type   = RETRIEVE;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    char telephone[MAX_TELEPHONE];
    char new_password[MAX_PASSWORD];
    char flag[32]; //仅用作收一次数据
    system("clear");
    //数据获取
    getchar();
    printf("Please enter your Account:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("Please enter your telephone:");
    get_userinfo(telephone,MAX_TELEPHONE);
    printf("enter your new password:");
    get_userinfo(new_password,MAX_PASSWORD);
    strcpy(Package.send_Account,Account);
    strcpy(Package.message_tmp,telephone);
    strcpy(Package.message,new_password);
    //发包
    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in retrieve send\n");
        return 0;
    }
    if((ret=my_recv(conn_fd,flag,32))<0)
    {
        perror("error in retrieve send\n");
        return 0;
    }
    if(flag[0]=='@')  //正确的话发送一个"y“
    {
        printf("Password change failed\n");
        return 0;
    }
    printf("Password changed successfully\n");
    return 1;
}

int Add_Friend(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = ADD_FRIENDS;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    system("clear");
    getchar();
    printf("Please enter a Account you want to add:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("please enter your friendly greeting:\n");
    get_userinfo(message,MAX_RECV);
    strcpy(Package.recv_Acount,Account);
    strcpy(Package.message,message);
    strcpy(Package.send_Account,gl_account);  //全局变量

    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in add friend send\n");
        return 0;
    }
    printf("The message has been sent,please wait for it to be accepted\n");
    getchar();
    return 1;
}

int Del_Friend(int conn_fd)
{
    int              ret=0;
    recv_t           Package;
    Package.type   = ADD_FRIENDS;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    char message[MAX_RECV];   //添加好友时给对方发送的话
    char temp[64];   //就是一个接收消息的缓冲区
    system("clear");
    getchar();
    printf("Please enter a Account you want to delete:");
    get_userinfo(Account,MAX_ACCOUNT);
    printf("Do you sure delete %s?[Y]yes / [N]no\n",Account);
    scanf("%s",temp);
    if(!(temp[0]=='Y'||temp[0]=='y'?1:0)) return 0;
    strcpy(Package.recv_Acount,Account);
    strcpy(Package.send_Account,gl_account);  //全局变量　代表本身的账号
    if((ret=send(conn_fd,&Package,sizeof(recv_t),0))<0)
    {
        perror("error in del friend send\n");
        return 0;
    }
    printf("%s have been delete!\n"); //没检测是否存在
    getchar();
    return 1;
}