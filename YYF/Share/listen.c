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

int main()
{
	int sock_fd;
	struct sockaddr_in seve,cli;

	sock_fd = socket(AF_INET,SOCK_STREAM,0);
        
	memset(&seve,0,sizeof(struct sockaddr_in));
        seve.sin_family = AF_INET;
        seve.sin_port = htons(4555);
        seve.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sock_fd,(struct sockaddr*)&seve,sizeof(struct sockaddr_in));
	
	if(listen(sock_fd,5)<0)
		perror("listen");
	
	int len = sizeof(struct sockaddr_in);
	
	sleep(30);
	printf("连接一个\n");
	int newfd = accept(sock_fd,(struct sockaddr*)&cli,&len);

	sleep(30);
	printf("连接一个\n");
	newfd = accept(sock_fd,(struct sockaddr*)&cli,&len);

	sleep(30);
	printf("连接一个\n");
	newfd = accept(sock_fd,(struct sockaddr*)&cli,&len);

	while(1)
	{}

}
