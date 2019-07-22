#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<dirent.h>

#define normal       0   //正常
#define out_redirect 1   //输出重定向
#define in_redirect  2   //输入重定向
#define have_pipe    3   //有管道

void print_promt();
void get_input(char a[]);
void explain_input(char *,int *,char a[100][256]);
void do_cmd(int , char a[100][256]);
int find_command(char *);

int main(int argc,char **argv)
{
    int i;
    int argcount;
    char arglist[100][256];
    char **arg=NULL;
/*     buf=(char*)malloc(sizeof(char)*256);
    if(buf==NULL)
    {
        perror("error in malloc\n");
        exit(1);
    } */
    char buf[256];
    while(1){
        memset(buf,0,sizeof(buf));
        print_promt();
        //printf("%d\n",sizeof(buf));
        //get_input(buf);
        scanf("%s",buf);
        printf("fdg");
        if(!strcmp(buf,"exit") || !strcmp(buf,"logout")) break;
        printf("asd");
        for(i=0;i<100;i++) arglist[i][0]='\0';
        printf("sadd");
        argcount=0;
        explain_input(buf,&argcount,arglist);
        do_cmd(argcount,arglist);    
    }
    if(buf!=NULL)
    free(buf);
    exit(0);
}

void print_promt()
{
    printf("myshell$$:");
}

void get_input(char buf[])
{
/*     int len=0;
    char ch;
    buf[100]='p';
    printf("%c\n",buf[100]);
    ch=getchar();
    while(len<256)
    {
        if(ch=='\n') break;
        buf[len++]=ch;
        ch=getchar();
    }
    printf("asdasdasd");
    if(len==256)
    {
        perror("command is to long !\n");
        exit(1);
    }
    printf("1232");
    buf[len++]='\n';       //加个回车的意义在哪里
    printf("asdasd");
    buf[len]='\0';
    printf("asd"); */
    gets(buf);
}

void explain_input(char *buf,int *argcount,char arglist[100][256])
{
    char *q=buf;
    char *p=buf;
    int number=0;
    while(1)
    {
        if(p[0]=='\0')
        break;
        if(p[0]==' ')
        p++;
        else
        {
            q=p;
            number=0;
            while(q[0]!='\0' && q[0]!=' '){
                q++;
                number++;
            }
            strncpy(arglist[*argcount],p,number+1);
            arglist[*argcount][number]='\0';//最后一项为'\0'
            *argcount=*argcount+1;
            p=q;
        }
    }
}

void do_cmd(int argcount,char arglist[100][256])
{
    int flag=0;
    int how=0;     //检测是否存在重定向 管道
    int background=0;  //判断是否存在后台运行符
    int status=0;
    int i;
    int fd;
    char *arg[argcount+1];
    char *argnext[argcount+1];
    char *file;
    pid_t pid;

    for(i=0;i<argcount;i++)
    {
        arg[i]=arglist[i];
    }
    arg[argcount]=NULL;
    for(i=0;i<argcount;i++)
    {
        if(strncmp(arg[i],'&',1)==0)
        {
            if(i==argcount-1)
            {
                background=1;
                arg[argcount-1]=NULL;
                break;
            }else
            {
                printf("wrong commond!\n");
                return;
            }
        }
    }
    for(i=0;arg[i]!=NULL;i++)
    {
        if(!strcmp(arg[i],">"))
        {
            flag++;
            how=out_redirect;
            if(arg[i+1]==NULL)
            flag++;     //错误的格式
        }
    }
    for(i=0;arg[i]!=NULL;i++)
    {
        if(!strcmp(arg[i],"<"))
        {
            flag++;
            how=in_redirect;
            if(i==0)
            flag++;     //错误的格式
        }
    }  
    for(i=0;arg[i]!=NULL;i++)
    {
        if(!strcmp(arg[i],"|"))
        {
            flag++;
            how=have_pipe;
            if(arg[i+1]==NULL)
            flag++;     //错误的格式
            if(i==0)
            flag++;
        }
    } 
    if(flag>1)
    {
        perror("wrong command!\n");
        return;
    }
    if(how==out_redirect)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
            }
        }
    }
    if(how==in_redirect)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"<")==0)
            {
                file=arg[i+1];
                arg[i]=NULL;
            }
        }
    }
    if(how==have_pipe)
    {
        for(i=0;arg[i]!=NULL;i++)
        {
            if(strcmp(arg[i],"|")==0)
            {
                arg[i]=NULL;
                int j;
                for(j=i+1;arg[j]!=NULL;j++)
                {
                    argnext[j-i-1]=arg[j];
                }
                argnext[j-i-1]=arg[j]; //其实就是NULL
                break;
            }
        }
    }
    if((pid=fork())<0)
    {
        printf("fork error\n");
        return;
    }
    switch (how)
    {
        case 0:     //没有什么参数
            if(pid==0)
            {
                if(!find_command(arg))
                {
                    perror("can't find this command!\n");
                    exit(0);
                }
                execvp(arg[0],arg);   //子进程运行参数命令
                exit(0);              //退出子进程
            }
            break;
        case 1:    //输出重定向
            if(pid==0)
            {
                if(!find_command(arg))
                {
                    perror("can't find this command!\n");
                    exit(0);
                }
                fd=open(file,O_RDWR|O_CREAT|O_TRUNC,0644); //输出重定向的目标文件有可能不存在
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 2:    //输出重定向
            if(pid==0)
            {
                if(!find_command(arg))
                {
                    perror("can't find this command!\n");
                    exit(0);
                }
                fd=open(file,O_RDONLY);
                dup2(fd,0);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        case 3:
            if(pid==0)
            {
                int pid2=0;
                int fd2;
                int status;
                if((pid2=fork())<0)   //子进程再次分离一个子进程
                {
                    perror("error in fork\n");
                    exit(0);
                }
                if(pid2==0)
                {
                    if(!find_command(arg))
                    {
                        perror("can't find this command!\n");
                        exit(1);
                    }
                    fd2=open("/tmp/youdontnofile",O_WRONLY|O_CREAT|O_TRUNC,0644);
                    dup2(fd2,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                if(waitpid(pid2,&status,0)==-1)  //第三个参数未被设置 所以阻塞父进程
                {
                    printf("wait child process error\n");
                    exit(1);
                }
                if(!find_command(argnext[0]))
                {
                    perror("can't find this command!\n");
                    exit(1);
                }
                fd2=open("/tmp/youdontnofile",O_RDONLY);
                dup2(fd2,0);
                execvp(argnext[0],argnext);
                if(remove("/tmp/youdontnofile"))
                {
                    perror("error in remove\n");
                    exit(1);
                }
            }
            break;
        default:
            break;
    }
    if(background==1)  //子进程不会走到这里来
    {
        return; //父进程直接返回
    }
    if(waitpid(pid,&status,0)<0)
    {
        perror("error in wairpid\n");
        exit(1);
    }
}

int find_command(char *command)
{
    DIR* dp;
    struct dirent* dirp;
    char *path[]={"./","/bin","/usr/bin",NULL};
    if(strncmp(command,"./",2)==0)
    {
        command=command+2;//指针向后移动
    }
    int i=0;
    while(path[i]!=NULL){
        if(dp=opendir(path[i])==-1)
        {
            perror("error in opendir\n");
            exit(1);
        }
        while(dirp=readdir(dp)!=NULL){
            if(!strcmp(dirp->d_name,command))
            {
                close(dp);
                return 1;
            }
        }
        close(dp);
        i++;
    }
    return 0;
}