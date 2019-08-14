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
#include<map>
#include<iostream>
using namespace std;
char gl_account[MAX_ACCOUNT]; //以一个全局变量记录账号
int  fact_fd;          //记录服务器套接字
char fact_name[MAX_USERNAME];
int tt=1;  //更新映射表主键
list_friend_t head;//存储一次登录的好友信息
map<int,int>mp; //主键为账号　键值为头指针

list_messages_t Messages[1024]; //与每一个好友的消息链表

list_messages_t Message_BOX;





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


//接包
//执行这个函数以后为在此文件中存储一个好友信息链表　可以进行各种操作
//计划把数据接收来以后进行存放　以链表形式存储　分页形式显示
//最后有一个标记位为　EOF　的结束包　
int FetchAll_for_Friend_List()
{
    //List_DelNode(head);
    recv_t pacage;
    int ans=0;
    while(1)
    {
        if(recv(fact_fd,&pacage,sizeof(recv_t),0)<0)
        perror("error in recv\n");//收包

        if(pacage.type==EOF_OF_BOX)
        break;//接收到EOF结束符　退出接收循环

        //利用数据包中数据对链表结点进行赋值
        list_friend_t temp=(list_friend_t)malloc(sizeof(node_friend_t));
        temp->status=pacage.conn_fd;//状态
        strcpy(temp->recv_account,pacage.message_tmp);//好友账号
        strcpy(temp->nickname,pacage.message);//昵称
        //printf("%s\n",temp->nickname);
        //printf("%d::%d\n",++ans,temp->status);
        List_AddTail(head,temp);//建立链表
    }
    //getchar();
}


int login_client(int conn_fd,char *username)
{
    List_Init(Message_BOX,node_messages_t); //初始化消息盒子链表　后面只需要添加即可
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
    strcpy(fact_name,username);//给全局变量赋值
    if(number==-1) return 0;
    //登录成功以后开始接收离线消息盒子里的消息
    my_recv(conn_fd,buf,MAX_RECV);
    if(!strcmp(buf,BOX_NO_MESSAGES))
    printf("消息盒子无记录！\n");//登陆成功 离线消息盒子无记录　进入服务界面
    else
    {
        Box_t box;
        char buffer[32];
        printf("进行到循环\n");
        while(1)
        {
            if(recv(conn_fd,&box,sizeof(box),0)<0)
            perror("error in recv\n");
            if(box.type==EOF_OF_BOX)
            {
                printf("%s %d %s \n",box.message,box.type,box.account);
                break;
            }
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
            
            //说明这是一个消息类型的包　服务器会先发　你发送的消息　后发你接收的消息
            //把这些接收来的包放入账号主键唯一对应的键值
        }
    }
    //下面这行代码是当时调错用的　错误原因为服务器逻辑出现问题

    //Box_t box;
    //recv(conn_fd,&box,sizeof(box),0);   //处理多接收了一个包
    //printf("%s %d %s \n",box.message,box.type,box.account);
    //接收完离线消息盒子记录开始接收好友信息列表
    //实现为直接开始收包　最后一个结束包其中无数据　标记位为EOF
    FetchAll_for_Friend_List();
    //好友关系记录发送完成　结尾为一个结尾包　
    printf("好友列表加载完成\n");
    //接收到一个标记包　表示是否有消息记录
    if(recv(conn_fd,buf,sizeof(buf),0)<0)
    perror("error in client recv messages record\n");
    if(!strcmp(buf,BOX_NO_MESSAGES))
    printf("消息记录无记录！\n");
    else{
        //若有消息记录开始加载
        Box_t box;
        while(1)
        {
            if(recv(conn_fd,&box,sizeof(box),0)<0)
            perror("error in recv\n");
/*             printf("%s\n",box.message);
            getchar(); */
            if(box.type==EOF_OF_BOX) //接收到结束包会退出
            break;
            if(box.type==SEND_MESSAGES)//顺序为　链表前为老信息　链表后为新信息
            {
                if(!mp[atoi(box.account)])//表中值为零说明第一次
                {
                    mp[atoi(box.account)]=tt++; //对应唯一二维数组的索引
/*                     printf("%d %s %d\n",strlen(box.account),box.account,tt-1);
                    getchar(); */
                    List_Init(Messages[mp[atoi(box.account)]],node_messages_t);
                }
                list_messages_t temp=(list_messages_t)malloc(sizeof(node_messages_t)); //销毁链表
                strcpy(temp->messages,box.message);
                strcpy(temp->send_account,box.account);//好友账号
                strcpy(temp->nickname,box.usename); //username 必为好友本人的
                temp->type=box.type;
                List_AddTail(Messages[mp[atoi(box.account)]],temp);//将消息加入链表
            }
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
    Package.type   = DEL_FRIENDS;
    Package.send_fd= conn_fd;
    char Account[MAX_ACCOUNT];
    list_friend_t curpos;
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
    List_ForEach(head,curpos)
    {
        if(!strcpy(Account,curpos->recv_account));
        {
            List_DelNode(curpos);
            break;
        }
    }
    printf("%s have been delete!\n"); //没检测是否存在
    getchar();
    return 1;
}


int send_friend_messages(char *account,char *Message)
{
    recv_t package;
    package.type=SEND_MESSAGES;
    strcpy(package.message,Message);         //消息
    strcpy(package.recv_Acount,account);     //消息接收者
    strcpy(package.send_Account,gl_account); //消息发送者
    if(send(fact_fd,&package,sizeof(recv_t),0)<0)
    perror("error in send friend messages\n");  //相当于只发不收 收在一个专门收的函数中
    return 0;
}



//这个函数的意义是打开一个窗口　你可以与你输入的账号的好友聊天
//除非点击输入　否则每隔0.5秒刷新一次页面　防止有新的信息收到而无法显示
//单独开的那个线程把所有收到的消息放到一个链表中　对于好友请求没有读就不会从数据库中删除
//每次刷新页面时在这个链表中搜索　有新的消息就打印出来　
//服务器如何存储消息信息　与好友请求同存储于一张表中　在每次登录时发送　对于每一个好友建立一个以其
//账号为主键的map　键值为指针　指向一个链表 //在每次搜寻和发送时都在链表中中加入消息　同时服务器进行存储　

int Chat(char *account)//参数为好友账号
{
    char Message[MAX_RECV];
    Pagination_t paging;
    node_messages_t *pos;
    char acc_tmp[MAX_ACCOUNT];
    strcpy(acc_tmp,account);
    int i;
    char choice;
    int flag=0;
    list_messages_t curos;
    paging.totalRecords=0;
    if(Messages[mp[atoi(account)]]==NULL) //说明没有消息　链表还未初始化
    {
        printf("初始化\n");
        mp[atoi(account)]=++tt;
        List_Init(Messages[mp[atoi(account)]],node_messages_t);
    }
    List_ForEach(Messages[mp[atoi(account)]],curos) paging.totalRecords++;
    //遍历消息链表
	paging.offset = paging.totalRecords;
	paging.pageSize = MESSADES_PAGE_SIZE;

            while(1){
            Paging_Locate_FirstPage(Messages[mp[atoi(account)]], paging);
            system("clear");
            printf("链表长度：%d\n",paging.totalRecords);

            //在消息盒子中查找是否有正在发消息的好友发送来的消息
            List_ForEach(Message_BOX,curos) 
            {
                //消息肯定是发送者是正在聊天的好友的账号
                if(curos->type==SEND_MESSAGES && !strcmp(curos->send_account,account))
                {
                    list_messages_t temp = (list_messages_t)malloc(sizeof(node_messages_t));
                    strcpy(temp->messages,curos->messages);
                    strcpy(temp->recv_account,curos->recv_account);
                    strcpy(temp->nickname,curos->nickname);
                    List_AddTail(Messages[mp[atoi(account)]],temp);
                    paging.totalRecords+=1;//更新消息链表
                    List_FreeNode(curos); //这个消息已经载入消息链表　可以删除了
                }
            }
            List_ForEach(Messages[mp[atoi(account)]],curos)
            {
                cout << curos->messages << endl;
            }

            printf(
                    "\n==================================================================\n");
            printf(
                    "**************************** %s ****************************\n",account);//有消息可以用这个　Messages[mp[account]]->nickname
                               //没有消息不就凉了
            printf(
                    "------------------------------------------------------------------\n");
                    //printf("((((%d,%s,%d\n",mp[atoi(acc_tmp)],acc_tmp,strlen(acc_tmp));
            Paging_ViewPage_ForEach(Messages[mp[atoi(acc_tmp)]], paging, node_messages_t, pos, i){
                //链表中名称必为好友昵称
                //printf("%s :%s :\n",pos->nickname,fact_name);
                if(strcmp(pos->nickname,fact_name))//怎么比都可以
                {
                    printf("%-65s\n",pos->nickname);
                    printf("%-65s\n",pos->messages);
                }else{
                    printf("%65s\n",fact_name);
                    printf("%65s\n",pos->messages);
                }
                putchar('\n');
            }

            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[P]revPage | [N]extPage | [I]uput | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            fflush(stdin);
            scanf("%c", &choice);
            getchar();
            fflush(stdin);

            switch (choice) {
                case 'I':
                case 'i':
                {
                    //将消息在发向服务器的同时把消息存入　本好友账号映射的消息链表
                    printf("please enter:\n");
                    cin.getline(Message,sizeof(Message)); //发送者　接收者　消息　昵称不急
                    list_messages_t temp=(list_messages_t)malloc(sizeof(node_messages_t));
                    strcpy(temp->messages,Message);
                    strcpy(temp->send_account,gl_account);
                    strcpy(temp->nickname,fact_name);//随便即可　只要不和自己的名字重合
                    strcpy(temp->recv_account,account);
/*                     if(mp[account]==0)
                    mp[account]=++tt; */
                    List_AddTail(Messages[mp[atoi(account)]],temp);//加入到消息链表
                    paging.totalRecords++;
                    send_friend_messages(account,Message);
                }
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, -1, node_friend_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, 1, node_friend_t);
                    }
                    break;
                case 'r':
                case 'R':
                    flag=1;
                    break;
            }
            if(flag) break;
            usleep(500); //阻塞0.5秒　重新从消息链表加载
        } 
}



//可以把在登录以后收到的信息包装成一个消息盒子 
//好友请求　好友消息　群聊消息
//在其中处理所有的请求　建立一个消息盒子链表　存储所有数据包　根据其中标记位来辨别请求类型
void *method_client(void *arg)                       
{
    recv_t buf;
    while(1)
    {
        if(recv(fact_fd,&buf,sizeof(recv_t),0)<0)
        perror("error in recv\n");
        list_messages_t temp = (list_messages_t)malloc(sizeof(node_messages_t));
        temp->type=buf.type;//标记位　
        strcpy(temp->send_account,buf.send_Account);//发送者
        strcpy(temp->messages,buf.message);//消息
        strcpy(temp->nickname,buf.message_tmp);//昵称
        strcpy(temp->recv_account,buf.recv_Acount);//就是本人账号
        List_AddTail(Message_BOX,temp);
    }
}


int show_friend_list()//套接字为全局变量
{
    char account[MAX_ACCOUNT];
    Pagination_t paging;
    node_friend_t *pos;
    int i;
    char choice;
    list_friend_t curos;
    paging.totalRecords=0;
    List_ForEach(head,curos) paging.totalRecords++;
	paging.offset = 0;
	paging.pageSize = FRIEND_PAGE_SIZE;
    //paging.totalRecords = FetchAll_for_Friend_List(head);
    Paging_Locate_FirstPage(head, paging);
    do {
            system("clear");
            printf("链表长度：%d\n",paging.totalRecords);
            printf(
                    "\n==============================================================\n");
            printf(
                    "********************** Friend  List **********************\n");
            printf("%10s  %20s %15s\n", "account", "Name","status");
            printf(
                    "------------------------------------------------------------------\n");
            Paging_ViewPage_ForEach(head, paging, node_friend_t, pos, i){
                printf("%10s  %20s  %15d     \n",pos->recv_account,pos->nickname,pos->status);
            }

            printf(
                    "------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",
                    paging.totalRecords, Pageing_CurPage(paging),
                    Pageing_TotalPages(paging));
            printf(
                    "******************************************************************\n");
            printf(
                    "[P]revPage | [N]extPage | [Q]uery | [R]eturn");
            printf(
                    "\n==================================================================\n");
            printf("Your Choice:");
            fflush(stdin);
            scanf("%c", &choice);
            fflush(stdin);

            switch (choice) {
                case 'q':
                case 'Q':
                    printf("Please enter an account you want to chat with:\n");
                    scanf("%s",account);
                    getchar();
                    Chat(account);
                    break;
                case 'p':
                case 'P':
                    if (!Pageing_IsFirstPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, -1, node_friend_t);
                    }
                    break;
                case 'n':
                case 'N':
                    if (!Pageing_IsLastPage(paging)) {
                        Paging_Locate_OffsetPage(head, paging, 1, node_friend_t);
                    }
                    break;
            }
        } while (choice != 'r' && choice != 'R');   
        //链表在客户端退出时进行销毁
}
