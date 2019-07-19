#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<dirent.h>
#include<pwd.h>
#include<errno.h>
#include<grp.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define PARAM_NONE 0
#define PARAM_A    1
#define PARAM_L    2
#define PARAM_R    4
#define MAXROWLWN  80

int g_leave_len = MAXROWLWN;
int g_maxlen;  //存放某目录下最长文件名的长度

int yy;
void recursion(int flag_param,char * path)//本函数的意义是递归显示目录下的文件
{
    //printf("%d %s\n",errno,strerror(errno));
    //printf("递归 : %d \n",yy);
    DIR *dir;
    struct dirent *ptr;
    int count=0;
    char filename[7300][256],temp[256];
    char book[7300][256];
    int ans=0;
    if(strcmp(path,"/usr/share/ghostscript/9.22/Resource/CIDFSubst/DroidSansFallback.ttf/")==0)
    return 0;
    if(strcmp(path,"/usr/src/linux-headers-4.15.0-29deepin-generic/scripts/dtc/include-prefixes/powerpc/")==0)
    return 0;
    if(strcmp)
    //printf("%s:\n",path);
    dir=opendir(path);  //先打开一遍统计文件名数量
    if(dir==NULL)
    {
        my_err("opendir",__LINE__);
    }
    //统计出最长文件名长度和文件总数
    while((ptr=readdir(dir))!=NULL)
    {
        if(g_maxlen<strlen(ptr->d_name))
        {
            g_maxlen=strlen(ptr->d_name);
        }
        count++;
    }
    printf("number : %d\n",count);
    closedir(dir);
    if(count>7300)
    {
        my_err("too many file under this dir!\n",__LINE__);
    }
    //printf("%s\n",path);
    int i,j,len=strlen(path);
    struct stat buf;
    if((dir=opendir(path))==-1)
    {
        printf("error in opendir!\n");
        return ;
    }
    for(int i=0;i<count;i++)
    {
        ptr=readdir(dir);
        if(ptr==NULL) my_err("readdir",__LINE__);
        strncpy(filename[i],path,len);
        filename[i][len]='\0';  //因为strcat的实现需要最后一位是‘\0’
        if(filename[i][0]=='.') continue;
        strcat(filename[i],ptr->d_name);
        filename[i][len+strlen(ptr->d_name)]='\0';
        //printf("%d   stat:  %s\n",flag_param,filename[i]);
        display(flag_param-4,filename[i]);
        lstat(filename[i],&buf);
        if(S_ISDIR(buf.st_mode))  //此文件是一个目录 应该递归显示
        {
            strcpy(book[ans++],filename[i]);
            //printf("hello:  %s\n",book[ans-1]);
        }  //保存下所有的目录文件
    }
    closedir(dir);
    if(flag_param & PARAM_L ==0)  //没有l的时候打印一个换行符
    putchar('\n');
    for(int k=0;k<ans;k++)
    {
        //printf("12: %s\n",book[k]);
        int flag=0;
        for(int i=0;i<strlen(book[k])-1;i++)
        {
            if(book[k][i]=='/' && book[k][i+1]=='.')
            {
                flag=1;
                break;
            }
        }
        if(flag) continue;
        printf("\n%s:\n",book[k]);
        yy++;
        //printf("yy:%d\n",yy);
        recursion(flag_param,strcat(book[k],"/"));
        yy--;
        putchar('\n');
    }
}

void my_err(const char *err_string, int line)
{
    fprintf(stderr,"line:%d",line);
    perror(err_string);
    exit(1);
}

void display_attribute(struct stat buf,char * name)  /*显示所有的状态信息*/
{
    char   buf_time[32];
    struct passwd *psd;
    struct group  *grp;

    if(S_ISLNK(buf.st_mode)) {   //链接文件
        printf("l");
    }else if(S_ISREG(buf.st_mode)){  //普通文件
        printf("-");
    }else if(S_ISDIR(buf.st_mode)){  //目录
        printf("d");
    }else if(S_ISCHR(buf.st_mode)){  //字符设备
        printf("c");
    }else if(S_ISBLK(buf.st_mode)){   //块设备
        printf("b");
    }else if(S_ISFIFO(buf.st_mode)){  //FIFO文件
        printf("f");
    }else if(S_ISSOCK(buf.st_mode)){   //socket文件
        printf("s");
    }

    if(buf.st_mode & S_IRUSR){ //用户的权限
        printf("r");
    }else 
    printf("-");
    if(buf.st_mode & S_IWUSR){
        printf("w");
    }else
    printf("-");
    if(buf.st_mode & S_IXUSR){
        printf("x");
    }else
    printf("-");


    if(buf.st_mode & S_IRGRP){ //用户组的权限
        printf("r");
    }else 
    printf("-");
    if(buf.st_mode & S_IWGRP){
        printf("w");
    }else
    printf("-");
    if(buf.st_mode & S_IXGRP){
        printf("x");
    }else
    printf("-");


    if(buf.st_mode & S_IROTH){ //其他用户的权限
        printf("r");
    }else 
    printf("-");
    if(buf.st_mode & S_IWOTH){
        printf("w");
    }else
    printf("-");
    if(buf.st_mode & S_IXOTH){
        printf("x");
    }else
    printf("-");

    printf(" ");


    psd=getpwuid(buf.st_uid);
    grp=getgrgid(buf.st_gid);

    printf("%4d  ",buf.st_nlink);
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);

    printf("%6d",buf.st_size);//文件的大小
    strcpy(buf_time,ctime(&buf.st_mtime));  //文件的最后修改时间
    //ctime函数的作用为把时间转化为字符串
    buf_time[strlen(buf_time)-1]='\0';
    printf("  %s",buf_time);
}

void display_single(char *name)
{
    int i,len;
    if(g_leave_len<g_maxlen)
    {
        putchar('\n');
        g_leave_len=MAXROWLWN;
    }

    len=strlen(name);
    len=g_maxlen-len;
    printf("%-s",name);
    for(int i=0;i<len;i++) putchar(' ');
    printf("  ");  //两个空格
    g_leave_len-=(g_maxlen+2);//一行中剩下的字符数
}

void display(int flag,char *pathname)  //传入一个路径名 
{
    int i,j;
    struct stat buf;
    char name[256];  //代表名称的最长值 不同系统可能不同
    //printf("ol  %s:",pathname);
    if(lstat(pathname,&buf)==-1)
    {
        my_err("stat",__LINE__);  //stat函数出现错误 进行精确到行的报错
    } 
    for(i=0,j=0;i<strlen(pathname);i++)
    {
        if(pathname[i]=='/')  //目录之间的分隔符
        {
            j=0;continue;
        }
        name[j++]=pathname[i];
    }
    name[j]='\0';
    //printf("%d name: %s\n",flag,name);
    char tmp[100];
    strcpy(tmp,name);
    //仅支持-a -l选项 即四种情况
    switch (flag)
    {
        case PARAM_NONE:
            if(tmp[0]!='.')    //一般情况不显示隐藏文件
            display_single(tmp);
            break;
        case PARAM_A:
            display_single(tmp);
            break;
        case PARAM_L:
            if(tmp[0]!='.')
            {
                display_attribute(buf,tmp);
                printf("  %s\n",tmp);
            }
            break;
        case PARAM_A+PARAM_L:
                display_attribute(buf,name);
                printf("  %s\n",tmp);
                break;
        default:
            break;
    }
}

void display_dir(int flag_param,char * path)
{
    if(flag_param>=4) //证明有-R选项      //上面已经遍历此目录
    {
        recursion(flag_param,path);
    }else{
        DIR *dir;
        struct dirent *ptr;
        int count=0;
        char filename[256][_PC_PATH_MAX+1],temp[_PC_PATH_MAX+1];
        dir=opendir(path);  //先打开一遍统计文件名数量
        if(dir==NULL)
        {
            my_err("opendir",__LINE__);
        }
        //统计出最长文件名长度和文件总数
        while((ptr=readdir(dir))!=NULL)
        {
            if(g_maxlen<strlen(ptr->d_name))
            {
                g_maxlen=strlen(ptr->d_name);
            }
            count++;
        }
        closedir(dir);
        if(count>256)
        {
            my_err("too many file under this dir!\n",__LINE__);
        }

        int i,j,len=strlen(path);

        dir=opendir(path);

        for(int i=0;i<count;i++)
        {
            ptr=readdir(dir);
            if(ptr==NULL) my_err("readdir",__LINE__);
            
            strncpy(filename[i],path,len);
            filename[i][len]='\0';  //因为strcat的实现需要最后一位是‘\0’
            strcat(filename[i],ptr->d_name);
            filename[i][len+strlen(ptr->d_name)]='\0';
            display(flag_param,filename[i]);
        }
        closedir(dir);
        if(flag_param & PARAM_L ==0)  //没有l的时候打印一个换行符
        putchar('\n');
    }
}

int main(int argc ,char ** argv)
{
    int i=0,j=0,k=0,num=0;
    char path[_PC_PATH_MAX+1];
    char param[40];//保存命令行参数
    int flag_param=0;
    struct stat buf;

    for(i=0;i<argc;i++)
    {
        if(argv[i][0]=='-')
        {
            for(k=1;k<strlen(argv[i]);k++,j++)   //j是出现的参数总数
            param[j]=argv[i][k];  //保存命令行参数
        num++;  //保存 ’-‘ 的数量
        }
    }
    for(i=0;i<j;i++)
    {
        if(param[i]=='a')
        {
            flag_param|=PARAM_A;
        }else if(param[i]=='l')
        {
            flag_param|=PARAM_L;
        }else if(param[i]=='R')
        {
            flag_param|=PARAM_R;
        }else
        {
            printf("is a invaild param!\n");
            exit(1);
        }
    }
    const int yy=flag_param;
    param[j]='\0';
    if(num+1==argc)
    {
        strcpy(path,"./");  //没有参数的话默认为当前目录
        path[2]='\0';
        //printf(";;;%d,\n",flag_param);
        display_dir(yy,path);
        return 0;
    }
    i=1;
    do{
        //if(i==argc-1) return 0;; //防止无参数时的段错误
        if(argv[i][0]=='-') 
        {
            i++;
            continue;
        }else
        {
            //printf("%s\n",path);
            strcpy(path,argv[i]);
            if(stat(path,&buf)==-1)
            {
                my_err("stat",__LINE__);
            }
            if(S_ISDIR(buf.st_mode))
            {
                if(path[strlen(argv[i])-1]!='/')
                {
                    path[strlen(argv[i])]='/';
                    path[strlen(argv[i])+1]='\0';//保证字符串末尾为空零 方便进行字符串操作
                }else
                {
                    path[strlen(argv[i])]='\0';
                }
                //printf("canshu   %s  : %d\n",path,flag_param);
                //printf(";;;%d,\n",flag_param);
                display_dir(yy,path);
            }else
            {
                //printf(";;;%d,\n",flag_param);
                display(yy,path);//参数为一个文件
            }
            i++;
        }
    }while(i<argc);
    return 0;
}