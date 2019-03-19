#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#define   SERV_PORT   8000


//回射服务器处理函数
void dg_echo(int sockfd, struct sockaddr* pcliaddr,socklen_t len)
{
	int n;
	socklen_t length;
	char msg[1024];
	while(1)
	{
		length = len;
		n = recvfrom(sockfd,msg,1024,0,pcliaddr,&length);
		sendto(sockfd,msg,n,0,pcliaddr,length);
		printf("msdg = %s \r\n",msg);
	}

}

/***********
 *
 *
 * ***********/
void echo_udp_server(void)
{
	int sockfd;
	struct sockaddr_in cliaddr,servaddr;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);  //创建UDP套接字
	if(sockfd < 0)
	{
		printf("socket err\r\n");
	}

	bzero(&servaddr,sizeof(servaddr));  //清空服务器地址结构体
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("bind err\r\n");
	}

	dg_echo(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));  //回射服务处理

}
