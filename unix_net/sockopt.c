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


void call_back(void)
{
	printf("catch a sigio!\r\n");
}



void test_fcntl(void)
{
	int fd;
	int flag;

	signal(SIGIO, call_back); //注册一个IO信号
	if (fcntl(fd, F_SETOWN, getpid()) < 0)  //指定接收信号的宿主
	{
		printf("fcntl err : F_SETOWN\r\n");
	}

	flag = fcntl(fd, F_GETFL); //获取当前fd上的标志
	if (flag < 0)
	{
		printf("fcntl err : F_GETFL\r\n");
	}

	if (fcntl(fd, F_SETFL, flag | O_ASYNC) < 0)  //设置成信号驱动式IO
	{
		printf("fcntl er : F_SETFL\r\n");
	}

}
