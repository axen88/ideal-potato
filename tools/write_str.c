/*
* 使用gcc -Wall -g -o write_str write_str.c进行编译链接
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

int main(int argc, char **argv)
{
    int fd;
    char *fn;
    char *str;
    long long pos;
    int len;
    
	if (argc < 4)
	{
        printf("Usage: %s filename position string\n", argv[0]);
        return -1;
    }

    fn = argv[1];
    pos = atoll(argv[2]);
    str = argv[3];
    len = strlen(str);

    fd = open(fn, O_CREAT | O_RDWR | O_LARGEFILE);
    if (fd < 0)
    {
        printf("Open file failed. [filename: %s]\n", fn);
        return -2;
    }

    if (lseek(fd, pos, SEEK_SET) != pos)
    {
        close(fd);
        printf("Set file position failed. [filename: %s, position: %lld]\n",
            fn, pos);
        return -3;
    }

    if (write(fd, str, len) != len)
    {
        close(fd);
        printf("Write file failed. [filename: %s, position: %lld, size: %d]\n",
            fn, pos, len);
        return -4;
    }

    close(fd);
	
	return 0;
}

