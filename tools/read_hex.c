/*
* 使用gcc -Wall -g -o read_hex read_hex.c进行编译链接
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

#define BUF_SIZE 256

void PrintMemory(unsigned char *v_mem, unsigned int v_len, unsigned long long v_pos)
{
    unsigned int num = 0;

    while (v_len--)
    {
        if (num == 0)
        {
            printf("%08llX:", v_pos);
        }

        printf(" %02X", *v_mem++);

        if (++num == 16)
        {
            v_pos += num;
            num = 0;
            printf("\n");
        }
    }
}

int main(int argc, char **argv)
{
    int fd;
    unsigned char dat[BUF_SIZE];
    char *fn;
    long long pos;
    int len;
    int size;
    int ret;
    
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

    while (len > 0)
    {
        size = (len > BUF_SIZE) ? BUF_SIZE : len;
        
        ret = read(fd, dat, size);
        if (ret < 0)
        {
            printf("Read file failed. [filename: %s, position: %lld]\n",
                fn, pos);
            break;
        }
        else if (ret == 0)
        {
            break;
        }
        else if (ret != size)
        {
            size = ret;
            len = ret;
        }

        PrintMemory(dat, size, pos);

        pos += size;
        len -= size;
    }

    printf("\n");

    close(fd);
	
	return 0;
}

