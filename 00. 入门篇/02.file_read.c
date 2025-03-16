#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

// 文件读写系统调用
int main (void) 
{
  int fd;
  fd = open("hello", O_RDWR|O_CREAT, 0666); 
  // 0666 (oct) = 110 110 110 (binary) = rw-rw-rw- (文件权限) 
  //所有者（User）| 同组 Group | 其他人 Other的权限：可读 (r)，可写 (w)，不可执行 (-)
  // open是系统调用

  if (fd == -1)
  {
    printf("open file failed!\n");
    return -1;
  }

  char string[20] = "hello world!\n";
  write (fd, string, 14);
  fsync(fd);  // 同步刷新到磁盘中

  char *buf = (char*)malloc (20);
  memset(buf, 0, 20);
  lseek(fd, 0, SEEK_SET); 
  // SEEK_SET：文件开始
  // SEEK_CUR：文件当前位置
  // SEEK_END：文件末尾
  read(fd, buf, 14);
  printf("%s", buf);

  free(buf);
  close(fd);
  return 0;
}