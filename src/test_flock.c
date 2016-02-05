#include <stdio.h>
#include <sys/file.h>
#include <unistd.h>

#define LOCK_FILE "/tmp/test_lock"

int main(int argc, char **argv)
{
    int fd = 0;
    int ret = 0;
    int cnt = 0;
    
    fd = open(LOCK_FILE, O_WRONLY | O_CREAT);
    if (fd <= 0)
    {
        printf("open file failed. name: %s, fd: %d\n", LOCK_FILE, fd);
        return -1;
    }
    
    printf("open file success. name: %s, fd: %d\n", LOCK_FILE, fd);
    
    ret = flock(fd, LOCK_EX);
    if (ret != 0)
    {
        printf("flock file failed. fd: %d, ret: %d\n", fd, ret);
        close(fd);
        return ret;
    }
    
    printf("flock file success. fd: %d\n", fd);
    
    cnt = 0;
    while (cnt++ < 100)
    {
        printf("fd: %d, cnt: %d\n", fd, cnt);
        sleep(1);
    }
    
    close(fd);
    
    return 0;
}


