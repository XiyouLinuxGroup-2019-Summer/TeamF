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

void input_userinfo(int conn_fd,const char *string)//循环　直到接收到消息为“ｙ“
{
    char input_buf[MAX_USERNAME];
    char recv_buf[BUFSIZ];
    int flag_userinfo;
    do{
        printf("%s:",string);
        if(get_userinfo(input_buf,32)<0)
        {
            perror("get_userinfo\n");
            exit(1);
        }
        printf("%s\n",input_buf);
        if(send(conn_fd,input_buf,MAX_USERNAME,0)<0)
        {
            perror("send\n");
            exit(1);
        }
        printf("发送成功\n");
        if(my_recv(conn_fd,recv_buf,strlen(recv_buf))<0)
        {
            perror("my_recv\n");
            exit(1);
        }
        printf("接收到信息\n");
        if(recv_buf[0]==VAILD)
        flag_userinfo=VAILD;
        else
        {
            printf("error! please enter again\n");
            flag_userinfo=INVAILD;
        }
    }while(flag_userinfo==INVAILD);
}

int login_client(int conn_fd,char *username)
{
    char             buf[MAX_RECV];
    int              ret;
    //先告诉服务器有一个登录请求 初始化请求要用的包
    recv_t           Package;
    Package.type   = LOGIN;
    Package.send_fd= conn_fd;
    if(send(conn_fd,&Package,sizeof(recv_t),0)<0)//发送一个登录请求
    {
        perror("error in send\n");
        return 0;  //错误退出
    }
/*     if(Package.message[0]!=VAILD)
    {
        printf("登录请求错误！\n");
        return 0;  //服务器未返回‘ｙ’
    } */
    //如果登录请求正确
    input_userinfo(conn_fd,"username:");
    input_userinfo(conn_fd,"password:");

    if((ret=my_recv(conn_fd,username,MAX_USERNAME)<0))//希望登录成功后返回本身用户名
    {
        perror("my_recv\n");
        exit(1);
    }
    printf("welcome to zhaolonga-chat\n");
    //close(conn_fd);     //应该是不关闭的　现在先测试登录
    return 1;  //登陆成功　进入服务界面
}

int main(int argc,char **argv)  //暂时无全局变量
{
    //setbuf(stdin,NULL);
    int i;
    int ret;
    int conn_fd;
    int serv_port;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];
    int Port=SERV_POT;    //宏中自定义的端口号　后面还要改
    char client_IP[32]="127.0.0.1";//ＩＰ地址
    char username[MAX_USERNAME];
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

    if(!login_client(conn_fd,username))  //希望指针返回一个用户名
    {
        printf("密码或账号错误！\n");
        return 0;
    }else
    {      //登录成功后显示的页面
	system("clear");
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
    
}