/**************************
 * TCP 回射服务器程序
 * *****************/
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


#define SERV_PORT   8000
#define LISTENQ     10


/***************************
 * funtion name:
 * author name:
 * date:
 *
 * ************************/
void str_echo(int sockfd)
{
	ssize_t n;
	char buf[1024];

again:
	while((n = read(sockfd,buf,1024)) > 0)
		write(sockfd,buf,n);

	if(n < 0 && errno == EINTR)
	{
		goto again;
	}
	else if(n < 0)
	{
		printf("read sockfd err\r\n");
	}

}



void echo_tcp_server(void)
{
	int listenfd,connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);  //创建套接字
	if(listenfd < 0)
	{
		printf("socket err\r\n");
	}

	bzero(&servaddr,sizeof(servaddr));  //清空服务器地址结构体
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//绑定套接字和地址
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("bind err\r\n");
	}

	if(listen(listenfd,LISTENQ) < 0)
	{
		printf("listen err\r\n");
	}

	while(1)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd,(struct sockaddr*)&cliaddr,&clilen); //接受客户端的连接
		if((childpid == fork()) == 0)  //创建子进程来处理新的tcp连接
		{
			close(listenfd); //关闭监听套接字，因为监听套接字在父进程中执行
			str_echo(connfd);
		}
		close(connfd);//父进程中关闭新连接的套接字，因为新连接的套接字在子进程中执行
	}

}

void str_cli(FILE *fp, int sockfd)
{
	char sendline[1024],recvline[1024];
	while(fgets(sendline,1024,fp) != NULL)
	{
		write(sockfd,sendline,strlen(sendline));
		if(read(sockfd,recvline,1024) == 0)
		{
			printf("err\r\n");
		}
		fputs(recvline,stdout);
	}

}

void str_cli_select(FILE *fp, int sockfd)
{
	int maxfd;
	fd_set rset;
	char sendline[1024],recvline[1024];
	FD_ZERO(&rset);

	while(1)
	{
		FD_SET(fileno(fp),&rset);
		FD_SET(sockfd,&rset);
		maxfd = max(fileno(fp),sockfd) + 1;
		select(maxfd,&rset,NULL,NULL,NULL);

		if(FD_ISSET(sockfd,&rset))
		{
			if(read(sockfd,recvline,1024) == 0)
				printf("sock resad err\r\n");
			fputs(recvline,stdout);
		}

		if(FD_ISSET(fileno(fp),&rset))
		{
			if(fgets(sendline,1024,fp)== NULL)
			return;
			write(sockfd,sendline,strlen(sendline));
		}
	}

}

void echo_tcp_client(char* ip_str)
{
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		printf("socket err\r\n");
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = SERV_PORT;
	inet_pton(AF_INET,ip_str,&servaddr.sin_addr);
	if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("connect err\r\n");
	}
	str_cli(stdin,sockfd);
	exit(0);
}
