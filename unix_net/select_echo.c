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
#include <math.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <signal.h>

#define MAXLINE  1024
#define SERV_PORT   8000

/**********
 * 信号处理函数
 **********/
void sig_child()
{
	;
}

/**********
 *大小比较函数
 ************/
int max(int a, int b)
{
	return a > b ? a : b;
}


void echo_select_server(void)
{
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char msg[MAXLINE];
	pid_t child_pid;  //进程ID
	fd_set rset;       //select监测的fd集合
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, serveraddr;

	/*********TCP初始化********/
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("tcp socket err:");
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERV_PORT);
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));  //设置复用

	if (bind(listenfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("tcp bind err:");
	}

	if (listen(listenfd, 10) < 0)
	{
		perror("tcp listen err:");
	}

	/********UDP初始化*********/
	if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("udp socket err:");
	}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERV_PORT);
	if (bind(udpfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("tcp bind err:");
	}

	signal(SIGCHLD, sig_child);   //注册信号函数
	FD_ZERO(&rset);  //清空监视集合
	maxfdp1 = max(listenfd, udpfd) + 1;

	while (1)
	{
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0)
		{
			if (errno == EINTR)
				continue;
			else
				perror("select err:");
		}

		if (FD_ISSET(listenfd, &rset))  //TCP套接字可读
		{
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &len);

			if ((child_pid == fork()) == 0)  //子进程
			{
				close(listenfd);  //关闭监听套接字，处理连接套接字
				str_echo(connfd);
				exit(0);

			}

			close(connfd);  //父进程关闭连接套接字
		}

		if (FD_ISSET(udpfd, &rset))  //udp套接字可读
		{
			len = sizeof(cliaddr);
			recvfrom(udpfd, msg, MAXLINE, 0, (struct sockaddr*) &cliaddr, &len);
			sendto(udpfd, msg, MAXLINE, 0, (struct sockaddr*) &cliaddr, len);
		}

	}

}

