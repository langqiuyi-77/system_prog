#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <linux/soundcard.h>

// ioctl系统调用：向设备发送控制参数，配置命令或者获取设备信息
// 就使用ioctl来设置声卡的参数：采样率，采样格式，声道设置

int main (void)
{
	int dst_fd;
	dst_fd = open ("/dev/dsp", O_WRONLY, 0666); /*open dsp*/
	if (dst_fd == -1)
	{
		perror ("open /dev/dsp error!");
		return -1;
	}

	int rate = 44100;
	ioctl (dst_fd, SNDCTL_DSP_SPEED, &rate);
	int channels = 2;
	ioctl (dst_fd, SNDCTL_DSP_CHANNELS, &channels);
	int format = 16;
	ioctl (dst_fd, SNDCTL_DSP_SETFMT, &format);

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
