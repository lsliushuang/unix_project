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
	char buff[MAX_LINE];
	time_t ticks;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd < 0)
	{
		printf("socket error\r\n");
	}
	bzero(&serveraddr,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;  //IPV4
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(13);
	//绑定
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
	{
		printf("bind error\r\n");
	}

	if(listen(listenfd,1) < 0)
	{
		printf("listen error\r\n");
	}

	for( ;  ; )
	{
		connfd = accept(listenfd,NULL,NULL);
		ticks = time(NULL);
		snprintf(buff,sizeof(buff),"%.24s\r\n",ctime(&ticks));
		write(connfd,buff,strlen(buff));
		close(connfd);
	}
}











