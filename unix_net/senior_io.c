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
#include <sys/poll.h>

#define   MAXLINE    1024


//信号回调函数
static void connect_alarm(int signo)
{
	printf("catch a alarm signal!\r\n");
	return;
}


/******************
 * 带超时的连接函数（使用信号实现）
 * *****************/
int connect_timeo(int sockfd,struct sockaddr*addr,socklen_t len,int nsec)
{
	int n;
	signal(SIGALRM,connect_alarm);

	if(alarm(nsec) != 0)  //定时连接超时时间
		printf("alarm was already set!\r\n");

	if((n = connect(sockfd,addr,len)) < 0)
	{
		perror("connect is err:");
		close(sockfd);
	}
	if(errno == EINTR)   //连接被信号中断,连接已超时
	{
		errno = ETIMEDOUT;
		printf("connect timeout!\r\n");
	}


	alarm(0);  //关闭alarm
	signal(SIGALRM,connect_alarm);
	return(n);
}



/***********
 * 超时一段时间fd不可读，则超时返回（使用select实现）
 **********/
int read_timeo(int fd, int sec)
{
	fd_set rset;
	struct timeval tv;
	FD_ZERO(&rset);
	FD_SET(fd,&rset);

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	return (select(fd + 1,&rset,NULL,NULL,&tv));

}

//
//void str_cli_poll(FILE *fp, int sockfd)
//{
//	int stdineof;
//	char buf[MAXLINE];
//	int n,wfd;
//	struct pollfd pollfd[2];
//	struct dvpoll dopoll;
//
//
//}



int test()
{
	;
}




