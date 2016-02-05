/*
* 使用gcc -Wall -g -o read_str read_str.c进行编译链接
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
    char dat;
    char *fn;
    long long pos;
    int len;
    
	if (argc < 4)
	{
        printf("Usage: %s filename position length\n", argv[0]);
        return -1;
    }

    fn = argv[1];
    pos = atoll(argv[2]);
    len = atoi(argv[3]);

    fd = open(fn, O_RDONLY | O_LARGEFILE);
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

    while ((len > 0) && (read(fd, &dat, 1) > 0))
    {
        putchar(dat);
        len--;
    }

    printf("\n");

    close(fd);
	
	return 0;
}

