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
    buf[i++]='\n';
    buf[i++]='\0';
    return 0;
}

int input_userinfo(recv_t *temp)
{
    int flag_userinfo=0;
    char account[MAX_ACCOUNT];
    char password[MAX_RECV];
    fflush(stdin);
    printf("account:");
    if(get_userinfo(account,MAX_ACCOUNT)<0)
    {
        perror("get_userinfo\n");
        exit(1);
    }
    strcpy(temp->send_Account,account);
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
    int              ret;
    recv_t           Package;
    Package.type   = LOGIN;
    Package.send_fd= conn_fd;
    int number=3;
    getchar();
    system("clear");
    while(number--)  //三次机会
    {
        input_userinfo(&Package);
        printf("send    \n");
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
        if(username[0]==ERROR_IN_LOGIN)
        {
            perror("account or password error!\n");
            continue;
        }else break;
        printf("down to while\n");
    }
    printf("welcome to zhaolonga-chat\n");
    return 1;  //登陆成功　进入服务界面
}

int register_client(int conn_fd,char *account)  //注册请求　返回一个账号
{
    int ret;
    recv_t           Package;
    Package.type   = REGISTER;
    Package.send_fd= conn_fd;
    char password[MAX_PASSWORD];
    system("clear");
    printf("Welcome to register account!\n");
    printf("please enter your password,we will give your a unique account.\n");
    printf("password:");
    getchar();
    get_userinfo(password,MAX_PASSWORD);
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

int main(int argc,char **argv)  //暂时无全局变量
{
    setbuf(stdin,NULL);
    int i;
    int ret;
    int conn_fd;
    int serv_port;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];
    int Port=SERV_POT;    //宏中自定义的端口号　后面还要改
    char client_IP[32]="127.0.0.1";//ＩＰ地址
    char username[MAX_USERNAME];
    char register_tmp[MAX_ACCOUNT];
    recv_t Package;//登录请求时要发的包
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family=AF_INET; //ipv4 tcp

    serv_port=Port;  //设置端口
    if(serv_port<0 || serv_port>65535)
    {
        perror("invaild port\n"); 
        exit(1);
    }else
    {
        serv_addr.sin_port=htons(serv_port);//转化为网络字节序
    }

    if(inet_aton(client_IP,&serv_addr.sin_addr)==0)//设置IP
    {
        perror("invaild server ip!\n");
        exit(1);
    }
    if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr==0) //unsigned long
    {
        perror("invaild param!\n");
        exit(1);
    }
    //ipv4 tcp/ip协议    　　　tcp流套接字
    conn_fd=socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd<0)
    {
        perror("socket\n");
        exit(1);
    }
    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))<0)
    {
        perror("目前用户数量太多！\n");//当用户数量太多时不连接
        exit(1);
    }

    printf("连接请求已运行\n");
    char ch;
    int flag=0;
    int ans=0;
    do{
        if(ans==1) break;
        ans=0;
        system("clear");
        printf("Register[R]          Enter[E]\n");
        printf("Quit    [Q]\n");
        scanf("%c",&ch);
        switch (ch)
        {
            case 'R':
            case 'r':
                register_client(conn_fd,register_tmp);
                break;
            case 'E':
            case 'e':
                if(!login_client(conn_fd,username))  //希望指针返回一个用户名
                {
                    printf("please login again.\n");
                    flag=1;
                }else ans=1;   //登录成功
                break;
            case 'Q':
            case 'q':
            default:
                break;
        }
    }while(ch !='q' && ch!='Q');
    if(!flag)
    {      //登录成功后显示的页面
	char choice;
	 do { 
		system("clear");
		printf("\n\n====================================================================\n");
		printf("\n========您好，[%s]先生/女士,欢迎来到zhaolonga-happychat=================\n",username);
		printf("\n==================================================================\n");
		printf("     **************** zhaolonga-happychat ****************\n\n");
		printf("      [S]演出厅管理界面                [P]剧目管理界面\n");
		printf("\n");
		printf("      [T]售票管理界面                  [C]查询演出票\n");
		printf("                      \n");
		printf("      [R]退票界面                      [Q]查询演出界面\n");
		printf("                       \n");
		printf("      [N]统计销售额界面                [L]统计票房界面\n");
		printf("                       \n");
		printf("      [F]维护个人资料                  [A]管理系统用户\n");
		printf("                       \n");
		printf("\n");
		printf("      [E]退出登录\n");
		printf("\n==================================================================\n");
		printf("\n请输入您要进行的操作");
		choice = getchar();
		switch (choice)
		 {
		case 'S':
		case 's':
            system("clear");
			break;
         case 'P': 
		 case 'p':
		 	system("clear");
			 break; 
		case 'F':
		case 'f':
            system("clear");
			break;
		case 'c': 
		case 'C':
		 	system("clear");
			 break; 
		case 'Q': 
		case 'q': 
			system("clear");
			 break; 
		case 'T': 
		case 't': 
			system("clear");
			break; 
		case 'R': 
		case 'r': 
			break; 
		case 'N': 
		case 'n': 
			break; 
		case 'A': 
		case 'a':
			system("clear");
			break;
		case 'L': 
		case 'l':
			system("clear");
			break;
		}
	} while ('E' != choice && 'e' != choice); 
    } 
    close(conn_fd);
    return 0;
}