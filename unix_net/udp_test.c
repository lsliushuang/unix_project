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
void dg_echo(int sockfd, struct sockaddr* pcliaddr, socklen_t len)
{
	int n;
	socklen_t length;
	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	char msg[1024];
	while (1)
	{
		length = len;
		setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));  //设置接收超时时间
		n = recvfrom(sockfd, msg, 1024, 0, pcliaddr, &length);
		if(n < 0)
		{
			if(errno == EWOULDBLOCK)  //接收超时
			{
				printf("recvfrom timeout!\r\n");
			}
		}

		sendto(sockfd, msg, n, 0, pcliaddr, length);
		printf("msdg = %s \r\n", msg);
	}

}

/***********
 *
 *udp 回射服务器
 *
 * ***********/
void echo_udp_server(void)
{
	int sockfd;
	struct sockaddr_in cliaddr, servaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //创建UDP套接字
	if (sockfd < 0)
	{
		printf("socket err\r\n");
	}

	bzero(&servaddr, sizeof(servaddr));  //清空服务器地址结构体
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		printf("bind err\r\n");
	}

	dg_echo(sockfd, (struct sockaddr*) &cliaddr, sizeof(cliaddr));  //回射服务处理

}

//回射客户端处理函数
void dg_ecli(FILE*fp, int sockfd, struct sockaddr* servaddr, socklen_t len)
{
	int n;
	char cli_ip[15];
	char *ip = "192.168.30.93";
	struct sockaddr_in cliaddr;
	char sendline[1024], recvline[1025];
	while (fgets(sendline, 1024, fp) != NULL)  //读取到数据
	{
		sendto(sockfd, sendline, strlen(sendline), 0, servaddr, len);
		recvfrom(sockfd, recvline, 1025, 0, &cliaddr, &len);
		inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, cli_ip, 15);
		printf("server ip = %s \r\n", cli_ip);
		if (strcmp(cli_ip, ip) == 0)
		{
			printf("cli_ip equal ip!\r\n");
			recvline[1024] = '\0';
			fputs(recvline, stdout);
		}
	}

}

/***********
 *
 *udp 回射客户端
 *
 * ***********/
void echo_udp_client(char *ip)
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);  //创建UDP套接字
	if (sockfd < 0)
	{
		printf("socket err\r\n");
	}

	bzero(&servaddr, sizeof(servaddr));  //清空服务器地址结构体
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &servaddr.sin_addr.s_addr);
	servaddr.sin_port = htons(SERV_PORT);

	dg_ecli(stdin, sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); //回射客户端处理
	exit(0);
}
