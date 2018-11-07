/**
 * test unix file and dir  add by ls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

int file_dir_test(void)
{
	int ret = -1;
	ret = access("/home/linux-ls/workspace/study/UNIX/test/test.txt",F_OK);
	if(ret == 0)
	{
		printf("test.txt is exist!\r\n");
	}
	else
	{
		printf("test.txt is not exist!\r\n");
	}

	ret = chmod("/home/linux-ls/workspace/study/UNIX/test/test.txt",0777);
	if(ret < 0)
	{
		printf("chmod test.txt is failed!\r\n");
	}

	ret = chown("/home/linux-ls/workspace/study/UNIX/test/test.txt",1000,1000);
	if(ret < 0)
	{
		printf("chown test.txt is failed!\r\n");
	}

	ret = open("/home/linux-ls/workspace/study/UNIX/test/test.txt",O_RDWR | O_CREAT);
	if(ret < 0)
	{
		printf("open the test.txt failed!\r\n");
	}

	//unlink("/home/linux-ls/workspace/study/UNIX/test/test.txt");

	mkdir("/home/linux-ls/workspace/study/UNIX/test/test1",0777);
	//remove("/home/linux-ls/workspace/study/UNIX/test/test1");
	//remove("/home/linux-ls/workspace/study/UNIX/test/test.txt");
	rename("/home/linux-ls/workspace/study/UNIX/test/test1","/home/linux-ls/workspace/study/UNIX/test/test2");
	rename("/home/linux-ls/workspace/study/UNIX/test/test.txt","/home/linux-ls/workspace/study/UNIX/test/test.dat");

	return 0;
}
