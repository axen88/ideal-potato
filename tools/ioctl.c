/*
* 使用gcc -Wall -g -o ioctl ioctl.c进行编译链接
*/

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

int MyIoctl(const char *v_pFileName, int v_cmd, void *v_pArg)
{
    int fd;
    int ret;
    
    fd = open(v_pFileName, O_RDWR);
    if (fd < 0)
    {
        printf("Open file failed. [filename: %s]\n", v_pFileName);
        return -2;
    }
    
    ret = ioctl(fd, v_cmd, v_pArg);
    
    close(fd);
    
    return ((ret < 0) ? ret : 0);
}

int main(int argc, char **argv)
{
    int cmd;
    
	if (argc < 4)
	{
        printf("Usage: %s filename cmd arg\n", argv[0]);
        return -1;
    }
    
    cmd = strtol(argv[2], NULL, 0);
    
    printf("filename: %s, cmd: 0x%x, arg: %s\n", argv[1], cmd, argv[3]);
    
    if (MyIoctl(argv[1], cmd, argv[3]) == 0)
    {
        printf("ioctl success.\n");
    }
    else
    {
        printf("ioctl failed.\n");
    }

    return 0;
}