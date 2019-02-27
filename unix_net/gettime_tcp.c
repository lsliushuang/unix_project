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

#define MAX_LINE  15

void gettime_tcp_server(void)
{
	int listenfd,connfd;
	struct sockaddr_in serveraddr;
	struct sockaddr_in client_addr;
	char cli_ip[INET_ADDRSTRLEN] = "";	   // 用于保存客户端IP地址
	char buff[MAX_LINE];
	time_t ticks;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		printf("socket error\r\n");
	}
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;  //IPV4
	int ret = inet_pton(AF_INET,"192.168.0.106",&serveraddr.sin_addr.s_addr);//inet_addr("192.168.0.106");//htonl(INADDR_ANY);  //需要把IP字符串转化成Ｕ３２的二进制传入
	if(ret == 0)
	{
		printf("err\r\n");
	}
	serveraddr.sin_port = htons(8001);

	//绑定
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
	{
		printf("bind error\r\n");
	}

	if(listen(listenfd,10) < 0)
	{
		printf("listen error\r\n");
	}

	for( ;  ; )
	{
		connfd = accept(listenfd,(struct sockaddr*)&client_addr,sizeof(client_addr));
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("----------------------------------------------\n");
		printf("client ip=%s,port=%d\n", cli_ip,ntohs(client_addr.sin_port));
//		ticks = time(NULL);
//		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
//		write(connfd,buff,strlen(buff));
//		close(connfd);
	}
}


void judge_host_little_big(void)
{
	union{
		short s;
		char c[sizeof(short)];
	}un;

	un.s = 0x0102;

	if(un.c[0] == 0x01 && (un.c[1] == 0x02))
	{
		printf("host is big endian\r\n");
	}
	else if(un.c[0] == 0x02 && (un.c[1] == 0x01))
	{
		printf("host is little endian\r\n");
	}

}











