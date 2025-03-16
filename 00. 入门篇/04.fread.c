#include <stdio.h>
#include <stdlib.h>

struct student
{
  char name[10];
  int age;
  float score;
} stu[2];


// Linux系统调用
// • 软中断：X86下int 0x80；ARM架构下 SWI软中断指令
// • 寄存器保存相关参数：参数、系统调用号
// • 进入内核态，执行内核特权指令代码函数
// • 返回值保存到寄存器
// • 返回到用户态、系统调用结束

// 既然有了系统调用API，为什么还要使用C标准库函数？
// 直接使用 系统调用 API（如 open()、read()、write()）时，我们需要自己管理文件描述符、缓冲区、错误处理等细节.
// 直接使用 write()，每次调用都要触发一次 系统调用，会有 性能损失。C 标准库提供 缓冲 I/O，减少 write() 调用次数，提高性能。
// 兼容性：open()、read()、write() 这些是 POSIX API，适用于 Unix/Linux，但 Windows 没有这些函数。
// C 标准库 (fopen(), fread(), fprintf()) 在 Windows、Linux、MacOS 都可以用，它们是 跨平台的。
// 大多数情况 推荐用 fopen()、fwrite()，性能更好，更易用; 需要 精确控制文件描述符、文件锁、无缓冲写入 时，才使用 open()、write()。

// 系统调用的API从哪里来，对应的头文件如unistd.h从哪里来的？
// 系统调用 API 从哪里来？	由 操作系统内核 提供，用户态程序通过 glibc 访问
// 头文件（如 unistd.h）从哪里来？	头文件是 C 标准库（glibc）的部分，一般位于 /usr/include/
// 系统调用的实现在哪里？	头文件只是声明，实际实现由 glibc 和 syscall 机制完成
// 如何直接调用 Linux 系统调用？	使用 syscall(SYS_xxx, ...)
// 你可以直接用 syscall() 调用系统调用
// ssize_t my_read(int fd, void *buf, size_t count) {
//     return syscall(SYS_read, fd, buf, count);
// }

// 文件读写 C标准库 API
int main (void)
{
  for (int i = 0; i < 2; i ++) 
  {
    printf("please input name age score:\n");
    scanf("%s %d %f", stu[i].name, &stu[i].age, &stu[i].score);
  }

  //FILE * 是 C 语言标准库中的 文件指针，用于管理 文件流。它指向一个 FILE 结构体，该结构体维护了：
  //文件描述符
  //缓冲区
  //当前读/写位置
  //状态（EOF、错误等）
  FILE *fp;
  if ((fp = fopen("hello.dat", "w+")) == NULL)
  {
    printf("fopen failed!\n");
    return -1;
  }

  fwrite(&stu, sizeof(struct student), 2, fp);
  // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
  // 用于向文件写入二进制数据，prt：指向要写入的数据的指针，size：每个元素的大小，nmemb：要写入的元素大小，stream：目标文件流指针
  if (ferror(fp) != 0)  // 检测文件流fp是否发生了错误
  {
    printf("fwrite failed!\n");
    clearerr(fp);
    return -1;
  }

  fflush(fp);
  rewind(fp); // 等价于 fseek(fp, 0, SEEK_SET)
  struct student *buf = (struct student*)malloc (2*sizeof(struct student));
  if (ferror(fp) != 0)
  {
    printf("fread failed!\n");
    clearerr(fp);
    return -1;
  }
  printf("姓名\t年龄\t分数\t\n");
  for (int i = 0; i < 2; i ++)
    printf("%s\t%d\t%f\n", buf[i].name, buf[i].age, buf[i].score);
  fclose(fp);
  free(buf);
  buf = NULL;
  return 0;
}