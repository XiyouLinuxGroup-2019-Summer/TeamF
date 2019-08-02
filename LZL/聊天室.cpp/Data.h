#define SERV_POT           8888    //服务器端口
#define LISTENQ            12        //连接请求队列的最大长度

#define IP                 (127.0.0.1)       //本地ip

#define EVENTS_MAX_SIZE    1024   //epoll接收事件最大数 
#define MAX_CONTECT_SIZE   1024   //服务器最大连接数
#define MAX_USERNAME       64     //名称长度最大数

#define MAX_RECV           8096    //发包的大小 与接包大小相同

#define INVAILD            'n'      //用户信息无效
#define VAILD              'y'      //用户信息有效
#define USERNAME            0
#define PASSWORD            1
/*-----------------------------------------------*/
#define LOGIN               0       //登录请求


/*-----------------------------------------------*/

typedef struct {
    int type;   //事件类型
    int send_fd;
    int send_Account;  //账号　用账号来区分不同的人
    int recv_fd;
    int recv_Acount;    //
    char message[MAX_RECV];
}recv_t; //客户端第一次recv中包的格式

typedef struct{
    char name[64];
    char password[64];
}login_t;  //登录时使用的包