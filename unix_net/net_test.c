/**
 * test unix net add by ls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/errno.h>

int main(int argc , char *argv[])
{
	//gettime_tcp_server();
	//judge_host_little_big();
	//echo_tcp_server();
	//echo_tcp_client("192.168.10.176");
	echo_udp_server();
	return 0;

}
