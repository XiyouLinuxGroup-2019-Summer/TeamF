#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>

struct dd{
	char haha[2][256];
	int ha;
}cd;

struct history{
	char a[100];
	struct history *next;
};

//打印提示符
void print_prompt()
{
	struct passwd *usrname;
	usrname = getpwuid(getuid());
	char buf[256];
	memset(buf,0,sizeof(buf));
	printf("my_shell:%s",getcwd(buf,256));
	if(strcmp(usrname->pw_name,"root")==0)
		printf("#");
	else
		printf("$");
}

//获得命令行输入
void get_input(char *buf)
{
	int len = 0;
	char ch;
	ch = getchar();
	while(len < 256 && ch != '\n')
	{
		buf[len++] = ch;
		ch = getchar();
	}
	if(len == 256)
	{
		printf("command is too long\n");
		exit(1);
	}
	buf[len] = '\n';
	buf[++len] = '\0';
}

//解析命令行
void explain_input(char *buf , int *cont , char list[100][256]) 
{
	int i=0;
	*cont = 0;
	int j;
	while(1)
	{
		if(buf[i] == ' ')
		{
			i++;
			continue;
		}
		if(buf[i] == '\n')
			break;
		else
		{
			j = 0;
			while(buf[i] != ' ' && buf[i] != '\n')
			{
				list[*cont][j] = buf[i];
				j++;
				i++;
			}	
			list[*cont][j] = '\0';
			*cont += 1;
		}
	}
}

//内置命令直接执行
void in_cd(char list[100][256],int *cont)
{
	if(strcmp(list[0],"cd") != 0)
		return;
	char buf[256];
	struct passwd *usrname;
	usrname = getpwuid(getuid());
	getcwd(cd.haha[cd.ha],sizeof(cd.haha[cd.ha]));
	if(cd.ha == 0)
		cd.ha++;
	else
		cd.ha--;
	if(strcmp(list[1],"~")==0)
	{
		if(strcmp(usrname->pw_name,"xzwb")==0)
			strcpy(buf,"/home/xzwb/");
		else if(strcmp(usrname->pw_name,"root")==0)
			strcpy(buf,"/root/");	
	}
	else if(strcmp(list[1],"-")==0)
		strcpy(buf,cd.haha[cd.ha]);
	else
		strcpy(buf,list[1]);
	chdir(buf);
}

//实现除了cd以外的外部命令
void in_command(char list[100][256] , int cont)
{
	int background = 0; //标识是否有后台运行符&
	int how = 0; //用于标识是否有><|
	pid_t pid;
	int i;
	int fd;
	char *arg[256];
	for(i=0;i<cont;i++)
		arg[i] = (char*)list[i];
	arg[cont] = NULL;

}

int main()
{
	cd.ha = 0;
	int cont;
	char list[100][256];
	char *buf = (char*)malloc(sizeof(256));
	while(1)
	{
	print_prompt();
	get_input(buf);
	explain_input(buf,&cont,list);
	in_cd(list,&cont);
	}
}
