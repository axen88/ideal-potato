/*
* 使用gcc -Wall -g -o popen_read popen_read.c进行编译链接
*/

#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *f;
    char buf[20];
    
    if (argc < 2)
    {
        printf("usage: %s cmd\n", argv[0]);
        printf("example: %s ls\n", argv[0]);
        return -1;
    }

    if ((f = popen(argv[1], "r")) == NULL)
    {
        printf("popen failed. [cmd: %s]\n", argv[1]);
        return -1;
    }
    
    while (fgets(buf, 20, f) != NULL)
    {
        printf("%s", buf);
    }
    
    pclose(f);
    
    return 0;
}