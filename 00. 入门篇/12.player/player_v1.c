#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <linux/soundcard.h>

// 整个程序的逻辑和03.copy是相似的，只是修改了src和dst的位置
// .wav文件是那种没有经过压缩的，不用解码然后播放，mps就是有压缩所以要先解码

// 普通文件可以通过文件名与实际的存储数据的 address 进行关联
// 设备文件通过 设备节点/设备号 与具体的物理设备进行关联
// 设备号：主设备号 + 次设备号组成（使用次设备号是为了支持多个，比如说双网卡这种）
// 设备文件存在于/dev目录下
// 设备节点可以自动创建、也可以手工创建 
// 好像是说如果设备只是给操作系统用不用创建设备节点,创建设备节点是为了给应用层面用的
// 设备节点（Device Node）是用户态程序访问设备驱动的入口，但所有操作最终都会进入内核

// 这个以下代码在ubuntu22上是不能的，因为22使用了新的内核版本
// 这个内核使用了snd新的声卡框架，不再支持dsp框架
// 使用这个可以驱动声卡发出声音，但是是噪音，因为还要根据对应的特性设置合适的工作模式，左声道...

int main (void)
{
	int dst_fd;
	dst_fd = open ("/dev/dsp", O_WRONLY, 0666); /*open dsp*/
	if (dst_fd == -1)
	{
		perror ("open /dev/dsp error!");
		return -1;
	}

	int src_fd;
	src_fd  = open ("./fruit.wav", O_RDONLY, 0666);
	if (src_fd == -1)
	{
		perror ("open wav file failed!");
		close (dst_fd);
		return -2;
	}
	int read_count;
	char buf[1024];
	while ((read_count = read(src_fd, buf, 1024)) > 0)
	{
		if (write(dst_fd, buf, read_count) != read_count)
		{
			perror("write /dev/dsp error");
			return -3;
		}
	}
	if (read_count < 0)
	{
		perror("read wav file error");
		return -4;
	}
	close(src_fd);
	close(dst_fd);
	return 0;
}
