```bash
// -e 的作用就是指定显示，这里就是指定显示open和close的调用
// <0.000089> 就是 -T 的作用显示系统调用花费的时间
lqy@lqy:~/projects/system_prog/00. 入门篇/08.strace$ strace -T -e open,close ./a.out 
close(3)                                = 0 <0.000089>  
close(3)                                = 0 <0.000113>
close(3)                                = 0 <0.000086>
open hello.dat failed!
+++ exited with 255 +++


// -tt 就是在前面显示具体的时间戳 07:42:19.519597 这种
lqy@lqy:~/projects/system_prog/00. 入门篇/08.strace$ strace -tt -T -e open,close ./a.out 
07:42:19.519597 close(3)                = 0 <0.000137>
07:42:19.524255 close(3)                = 0 <0.000112>
07:42:19.528934 close(3)                = 0 <0.000452>
open hello.dat failed!
07:42:19.532642 +++ exited with 255 +++


// -o log 作用就是将输出写到 log 文件中
lqy@lqy:~/projects/system_prog/00. 入门篇/08.strace$ strace -tt -T -e open,close -o log ./a.out 
open hello.dat failed!
lqy@lqy:~/projects/system_prog/00. 入门篇/08.strace$ ls
08.strace.c  a.out  log  stract命令使用.md  test.dat


// 之前那个不显示open的系统调用是因为在现代 Linux 版本（glibc 2.26+）
//open() 可能被 openat() 取代，所以 strace -e open 可能不会捕获 openat()
lqy@lqy:~/projects/system_prog/00. 入门篇/08.strace$ strace -tt -T -e open,openat,close ./a.out 
07:49:51.845942 openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3 <0.000249>
07:49:51.849882 close(3)                = 0 <0.000096>
07:49:51.850700 openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3 <0.000136>
07:49:51.854783 close(3)                = 0 <0.000254>
07:49:51.858475 openat(AT_FDCWD, "test.dat", O_RDONLY|O_CREAT, 051010) = 3 <0.000129> 
// 这里open返回的 = 3 就是说这个打开后给的文件描述符是 3
// 打开失败就是返回的文件描述符是 -1
07:49:51.859134 openat(AT_FDCWD, "hello.dat", O_WRONLY) = -1 ENOENT (没有那个文件或目录) <0.000114>
07:49:51.860171 close(3)                = 0 <0.000116>
open hello.dat failed!
07:49:51.862608 +++ exited with 255 +++
```

