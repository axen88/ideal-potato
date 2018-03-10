/*
* 使用gcc -Wall -g -o ftruncate ftruncate.c进行编译链接
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
    char dat = 0;
    long long pos;
    
	if (argc < 3)
	{
        printf("Usage: %s filename filesize\n", argv[0]);
        return -1;
    }

    fn = argv[1];
    pos = atoll(argv[2]);

    fd = open(fn, O_CREAT | O_RDWR | O_LARGEFILE);
    if (fd < 0)
    {
        printf("Open file failed. [filename: %s]\n", fn);
        return -2;
    }

#if 0
    if (lseek(fd, pos - 1, SEEK_SET) != pos - 1)
    {
        close(fd);
        printf("Set file position failed. [filename: %s, position: %lld]\n",
            fn, pos);
        return -3;
    }

    if (write(fd, &dat, 1) != 1)
    {
        close(fd);
        printf("Write file failed. [filename: %s, position: %lld, size: %d]\n",
            fn, pos, 1);
        return -4;
    }

#else
		if (ftruncate(fd, pos) < 0)
		{
        close(fd);
        printf("ftruncate failed. [filename: %s, position: %lld]\n",
            fn, pos);
        return -4;
		}
#endif

    close(fd);
	
	return 0;
}

