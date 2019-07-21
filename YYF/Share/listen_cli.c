#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>

int main(int argc , char *argv[])
{
        int sock_fd , ret;
        int haha;
        struct sockaddr_in seve;
        sock_fd = socket(AF_INET,SOCK_STREAM,0);
        seve.sin_family = AF_INET;
        seve.sin_port = htons(4555);
        inet_aton("10.101.14.250",&seve.sin_addr);
        if(connect(sock_fd,(struct sockaddr*)&seve,sizeof(struct sockaddr_in)) < 0)
                perror("connect");
	printf("连接成功\n");
	while(1)
	{}
}
