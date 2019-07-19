#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

int biaozhi; //等于0打印隐藏文件，等于一不打印隐藏文件；

//自定义的错误处理函数
void my_err(const char *err_string , int line)
{
	fprintf(stderr,"line:%d",line);
	perror(err_string);
	exit(1);
}

//打印文件列表的一行
int print_file(struct stat buf ,char *name)
{
	char buff[64];
	struct passwd *pwd; //从该结构体获取文件所有者的名字
	struct group *grp; //从该结构体获取文件组名
	//判断文件类型
	if(S_ISLNK(buf.st_mode))	//判断是否为链接
		printf("l");
	if(S_ISREG(buf.st_mode))	//判断是否为一般文件
		printf("-");
	if(S_ISDIR(buf.st_mode))	//判断是否为目录
		printf("d");
	if(S_ISCHR(buf.st_mode))	//字符设备文件
		printf("c");
	if(S_ISBLK(buf.st_mode))	//块设备文件
		printf("b");
	if(S_ISFIFO(buf.st_mode))	//先进先出FIFO
		printf("f");
	if(S_ISSOCK(buf.st_mode))	//socket
		printf("s");
	
	//判断权限
	if((buf.st_mode & S_IRUSR))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWUSR))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXUSR))
		printf("x");
	else
		printf("-");
	
	if((buf.st_mode & S_IRGRP))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWGRP))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXGRP))
		printf("x");
	else
		printf("-");

	if((buf.st_mode & S_IROTH))
		printf("r");
	else
		printf("-");

	if((buf.st_mode & S_IWOTH))
		printf("w");
	else
		printf("-");

	if((buf.st_mode & S_IXOTH))
		printf("x");
	else
		printf("-");

	//打印链接数
	printf(" %4d ",buf.st_nlink);
	
	//打印文件用户和用户组
	pwd = getpwuid(buf.st_uid);
	grp = getgrgid(buf.st_gid);
	printf("%-8s",pwd->pw_name);
	printf("%-8s",grp->gr_name);
	
	//打印文件大小
	printf("%8d",buf.st_size);

	//打印文件最后一次修改的时间
	strcpy(buff,ctime(&buf.st_ctime));
	buff[strlen(buff)-1] = '\0';
	printf(" %s",buff);
	printf(" %s\n",name);
	return 0;
}

//只打印文件名的
void printf_name(char name[][PATH_MAX+1],int num)
{
	int i,j=0;
	for(i=0;i<num;i++)
	{
		j++;
		printf("%-20s",name[i]);
		if(j==5)
		{
			printf("\n");
			j = 0;
		}
	}
	printf("\n");
}

//从目录中获得文件列表
void have(char *path)
{
	DIR *dir;
	int i = 0;
	int number = 0;
	char a[256][PATH_MAX+1];
	struct dirent *ptr;
	dir = opendir(path);
	if(dir == NULL)
		my_err("opendir",__LINE__);
	while((ptr = readdir(dir))!=NULL)
	{
		if(biaozhi==0 && ptr->d_name[0] == '.')
		{
			i++;
			continue;
		}
		strcpy(a[i],ptr->d_name);
		a[i][strlen(a[i])+1] = '\0';
		i++;
		number++;
	}
	printf_name(a,number);
}

void have2(char *path)
{
	DIR *dir;
	int i = 0;
	int number = 0;
	struct stat buf;
	char a[256][PATH_MAX+1];
	struct dirent *ptr;
	dir = opendir(path);
	if(dir == NULL)
		my_err("opendir",__LINE__);
	while((ptr = readdir(dir))!=NULL)
	{
		if(biaozhi==0 && ptr->d_name[0] == '.')
		{
			i++;
			continue;
		}
		strcpy(a[i],ptr->d_name);
		a[i][strlen(a[i])+1] = '\0';
		i++;
		number++;
	}
	for(i=0;i<number;i++)
	{	
		if(biaozhi==0 && a[i][0]=='.')
		lstat(a[i],&buf);
		print_file(buf,a[i]);
	}
	
}

int main(int argc,char *argv[])
{
	char path[32];
	int i,j,k,m=0,num=0,c;
	struct stat buf;
	char a[10];
	if(argc==1)
 	{
		biaozhi = 0;
		strcpy(path,"./");
		have2(path);
		return 0;
	}
}
