#include <stdlib.h>
#include <assert.h>

#define SERV_POT           8888    //服务器端口
#define LISTENQ            12        //连接请求队列的最大长度

#define IP                 (127.0.0.1)       //本地ip

#define FRIEND_PAGE_SIZE   5      //好友列表分页最大数
#define EVENTS_MAX_SIZE    1024   //epoll接收事件最大数 
#define MAX_CONTECT_SIZE   1024   //服务器最大连接数
#define MAX_USERNAME       64     //名称长度最大数
#define MAX_ACCOUNT        32     //账号长度最大值
#define MAX_PASSWORD       32     //密码最大长度
#define MAX_TELEPHONE      32     //找回密码使用的电话最长长度

#define MAX_RECV           8096    //发包的大小 与接包大小相同

#define ERROR_IN_LOGIN     '@'      //登录失败
#define INVAILD            'n'      //用户信息无效 
#define VAILD              'y'      //用户信息有效
#define USERNAME            0
#define PASSWORD            1
#define BOX_NO_MESSAGES    "@@@@@@@@@@@@@"
#define BOX_HAVE_MESSAGS   "$$$$$$$$$$$$$"
/*-----------------------------------------------*/
#define EOF_OF_BOX          -1      //发包结束符
#define LOGIN               0       //登录请求
#define REGISTER            1       //注册请求
#define RETRIEVE            2       //找回密码
#define ADD_FRIENDS         3       //添加好友
#define ADD_FRIENDS_QUERY   4       //收到添加好友请求后做出答复的数据类型
#define DEL_FRIENDS         5       //删除好友
#define LIST_FRIENDS        6       //显示好友列表


/*-----------------------------------------------*/

typedef struct {
    int  type;   //事件类型
    int send_fd;
    char send_Account[MAX_ACCOUNT];  //账号　用账号来区分不同的人
    int recv_fd;
    char recv_Acount[MAX_ACCOUNT];    //
    char message[MAX_RECV];
    char message_tmp[MAX_RECV];//放两个缓冲区　以备不时之需
    int epfd;
    int conn_fd;
}recv_t; //客户端第一次recv中包的格式

typedef struct 
{
    char account[MAX_ACCOUNT];
    char message[MAX_RECV];
    int type;    //事件类型
}Box_t;



typedef struct friend_node//好友链表类型
{
    int status;//是否在线
    char nickname[MAX_USERNAME];//昵称
    char recv_account[MAX_ACCOUNT];//朋友账号　接收者
    char send_account[MAX_ACCOUNT];//本人账号　发送者
    struct friend_node *next;
    struct friend_node *prev;
    //不需要加套接字　在服务器会进行查询
}node_friend_t,*list_friend_t;


//包含分页器与链表操作
#define List_Init(list, list_node_t) {					\
		list=(list_node_t*)malloc(sizeof(list_node_t)); \
		(list)->next=(list)->prev=list;					\
	}

#define List_Free(list, list_node_t) {			\
		assert(NULL!=list);						\
		list_node_t *tmpPtr;					\
		(list)->prev->next=NULL; 				\
		while(NULL!=(tmpPtr=(list)->next)){ 	\
			(list)->next=tmpPtr->next;			\
			free(tmpPtr);						\
		}										\
		(list)->next=(list)->prev=list;			\
	}

#define List_Destroy(list, list_node_t) {		\
		assert(NULL!=list);						\
		List_Free(list, list_node_t)			\
		free(list);								\
		(list)=NULL;							\
	}

#define List_AddHead(list, newNode) {			\
		(newNode)->next=(list)->next;		 	\
		(list)->next->prev=newNode;			 	\
		(newNode)->prev=(list);				 	\
		(list)->next=newNode;				 	\
	}

#define List_AddTail(list, newNode) {			\
		(newNode)->prev=(list)->prev; 		 	\
		(list)->prev->next=newNode;			 	\
		(newNode)->next=list;				 	\
		(list)->prev=newNode;				 	\
	}

#define List_InsertBefore(node, newNode) {		\
		(newNode)->prev=(node)->prev; 		 	\
		(newNode)->next=node;			 		\
		(newNode)->prev->next=newNode;			\
		(newNode)->next->prev=newNode;			\
	}

#define List_InsertAfter(node, newNode) {		\
		(newNode)->next=node->next;			 	\
		(newNode)->prev=node; 				 	\
		(newNode)->next->prev=newNode;			\
		(newNode)->prev->next=newNode;			\
	}

#define List_IsEmpty(list)  ((list != NULL)	\
	&& ((list)->next == list)				\
	&& (list == (list)->prev))

#define List_DelNode(node) {\
			assert(NULL!=node && node!=(node)->next && node!=(node)->prev);				\
			(node)->prev->next=(node)->next; 	\
			(node)->next->prev=(node)->prev;	\
	}

#define List_FreeNode(node) {	\
		List_DelNode(node);		\
		free(node);				\
	}


#define List_ForEach(list, curPos) 		\
	 for (   curPos = (list)->next;  	\
		  	  	  curPos != list;       \
		  	  	  curPos=curPos->next	\
	    )


typedef struct
{
	int totalRecords;	//�ܼ�¼��
	int offset;			//��ǰҳ��ʼ��¼����ڵ�һ����¼��ƫ�Ƽ�¼��
	int pageSize;		//ҳ���С
	void *curPos;		//��ǰҳ��ʼ��¼�������еĽ���ַ
}Pagination_t;

#define List_Paging(list, paging, list_node_t) {			\
		if(paging.offset+paging.pageSize>=paging.totalRecords){	\
			Paging_Locate_LastPage(list, paging, list_node_t);	}\
		else {													\
			int i;												\
			list_node_t * pos=(list)->next;						\
			for( i=0; i<paging.offset && pos!=list ; i++) 		\
			   pos=pos->next;		 							\
			paging.curPos=(void*)pos;							\
		}														\
	}

#define Paging_Locate_FirstPage(list, paging) { \
		paging.offset=0;						\
		paging.curPos=(void *)((list)->next);	\
	}

#define Paging_Locate_LastPage(list, paging, list_node_t) {	\
	int i=paging.totalRecords % paging.pageSize;	\
	if (0==i && paging.totalRecords>0)				\
		i=paging.pageSize;							\
	paging.offset=paging.totalRecords-i;			\
	list_node_t * pos=(list)->prev;					\
	for(;i>1;i--)									\
		pos=pos->prev;								\
	paging.curPos=(void*)pos;						\
													\
}


#define Paging_ViewPage_ForEach(list, paging, list_node_t, pos, i) 	\
	for (i=0, pos = (list_node_t *) (paging.curPos);	\
			pos != list && i < paging.pageSize; 		\
			i++, pos=pos->next)							\



#define Paging_Locate_OffsetPage(list, paging, offsetPage, list_node_t) {\
	int offset=offsetPage*paging.pageSize;			\
	list_node_t *pos=(list_node_t *)paging.curPos;	\
	int i;											\
	if(offset>0){									\
		if( paging.offset + offset >= paging.totalRecords )	{\
			Paging_Locate_LastPage(list, paging, list_node_t);	\
		}else {												\
			for(i=0; i<offset; i++ )						\
				pos=pos->next;								\
			paging.offset += offset;						\
			paging.curPos= (void *)pos;						\
		}													\
	}else{													\
		if( paging.offset + offset <= 0 ){					\
			Paging_Locate_FirstPage(list, paging);			\
		}else {												\
			for(i=offset; i<0; i++ )						\
				pos	= pos->prev;							\
			paging.offset += offset;						\
			paging.curPos= pos;								\
		}													\
	}														\
}

//���ݷ�ҳ��paging���㵱ǰ��ҳ��
#define Pageing_CurPage(paging) 	(0==(paging).totalRecords?0:1+(paging).offset/(paging).pageSize)

//���ݷ�ҳ��paging������ܵ�ҳ��
#define Pageing_TotalPages(paging)	(((paging).totalRecords%(paging).pageSize==0)?\
	(paging).totalRecords/(paging).pageSize:\
	(paging).totalRecords/(paging).pageSize+1)

//����paging�жϵ�ǰҳ���Ƿ�Ϊ��һҳ�����Ϊtrue��ʾ�ǣ�����false
#define Pageing_IsFirstPage(paging) (Pageing_CurPage(paging)<=1)

//����paging�жϵ�ǰҳ���Ƿ�Ϊ���һҳ�����Ϊtrue��ʾ�ǣ�����false
#define Pageing_IsLastPage(paging) 	(Pageing_CurPage(paging)>=Pageing_TotalPages(paging))
