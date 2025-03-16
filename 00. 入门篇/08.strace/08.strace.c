
/************************************************
*      Filename: strace.c
*        Author: litao.wang
*   Description: 
*        Create: 2018-10-10 12:20:20
* Last Modified: 2018-10-11 02:57:51
*************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
	int fd1, fd2;

	fd1 = open ("test.dat", O_RDONLY | O_CREAT);
	if (fd1 == -1)
	{
		printf ("open test.dat failed!\n");
		return -1;
	}

	fd2 = open ("hello.dat", O_WRONLY);
	if (fd2 == -1)
	{
		close (fd1);
		printf ("open hello.dat failed!\n");
		return -1;
	}
	close (fd1);
	close (fd2);
	return 0;
}
