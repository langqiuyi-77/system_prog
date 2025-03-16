#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFERSIZE 4096

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    printf("Usage: copy src dest\n");
    return 1;
  }

  int srcfd = open(argv[1], O_RDONLY);
  if (srcfd == -1)
  {
    perror("open srcfd failed");
    return 1;
  }
  int dstfd = open(argv[2], O_WRONLY|O_CREAT, 0666);
  if (dstfd == -1) 
  {
    perror("open destfd failed");
    return 1;
  }

  int len = 0;
  char buffer[BUFFERSIZE] = {0};
  // 读写都会返回读写的长度，需要对长度进行检查
  while ((len = read(srcfd, buffer, BUFFERSIZE)) > 0)
  {
    if (write(dstfd, buffer, len) != len)
    {
      perror("Write error");
      return 2;
    }
  }
  // 读操作如果返回 len<0 就是出现了问题
  if (len < 0)
  {
    perror("read error");
    return 3;
  }
  close(srcfd);
  close(dstfd);
  return 0;
}