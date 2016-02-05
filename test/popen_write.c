/*
* 使用gcc -Wall -g -o popen_write popen_write.c进行编译链接
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *f;
    int len = 0;
    
    if (argc < 3)
    {
        printf("usage: %s arg cmd\n", argv[0]);
        printf("example: %s / \"xargs ls\" \n", argv[0]);
        return -1;
    }
    
    if ((f = popen(argv[2], "w")) == NULL)
    {
        printf("popen failed. [cmd: %s]\n", argv[2]);
        return -1;
    }
    
    len = strlen(argv[1]);
    if (fwrite(argv[1], 1, len, f) != len)
    {
        printf("fwrite failed.\n");
    }
    
    pclose(f);
    
    return 0;
}