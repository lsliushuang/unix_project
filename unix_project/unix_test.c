/**
 * test unix  add by ls
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>
#include <sys/ioctl.h>

int main(int argc , char *argv[])
{

	int i =0;
	for(i = 0; i < argc; i++)
		printf("str = %s \r\n",argv[i]);
//	file_io_test();
//	file_dir_test();
//	std_io_test();
//	data_file_test();
	create_process_test();
	//get_env_test();
	while(1);
	return 0;

}
