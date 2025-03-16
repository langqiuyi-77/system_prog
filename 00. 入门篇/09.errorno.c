#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

// 系统编程错误一般通过函数的返回值表示
// 执行成功，返回0或正确值
// 执行失败，返回-1，并把系统全局变量errno赋值，指示具体错误
// 一般返回值只知道是否执行成功，而更具体的错误由errno指出
// 使用perror可以输出具体错误原因
// 全局变量errno由操作系统维护：当系统调用或调动库函数出错时，会重置该值
int main (void)
{
  int fd;

  fd = open("hello.dat", O_WRONLY);
  if (fd == -1)
  {
    // 会输出 open hello.dat failed!: No such file or directoryopen hello.dat failed: 
    // 输出了用户定义的部分后，后面会加上erron执行的错误
    perror ("open hello.dat failed!");
    return -1;
  }
  close(fd);
  return 0;
}