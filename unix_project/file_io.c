/**
 * test unix file io  add by ls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>

int file_io_test(void)
{
	int ret = -1;
	uint8_t *test_buf = "welcom to study unix!\r\n";
	int fd = open("/home/linux-ls/workspace/study/UNIX/test/test.txt",O_RDWR, 0x777);
	if(fd < 0)
	{
		printf("open or creat file failed!\r\n");
		return -1;
	}
	//lseek(fd,0,SEEK_CUR);
	ret = write(fd,test_buf,strlen(test_buf));
	if(ret < 0)
	{
		printf("write file failed!\r\n");
		return 0;
	}
	//fcntl();  //改变文件打开状态
	sync();
	fsync(fd);
	fdatasync(fd);
	close(fd);

	fd = open("/dev/fd/1",O_RDWR,0x777);
	if(fd < 0)
	{
		printf("open or creat file failed!\r\n");
		return -1;
	}

	ret = write(fd,test_buf,strlen(test_buf));
	if(ret < 0)
	{
		printf("write file failed!\r\n");
		return 0;
	}
	close(fd);
	return 0;


}
