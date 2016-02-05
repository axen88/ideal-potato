/*
* 使用gcc -Wall -g -o get_process_id get_process_id.c进行编译链接
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int GetProcessId(const char *v_pProcessName)
{
    FILE *f;
    char cmd[1024] ;
    char buf[100];
    int pid = 0;
    
    snprintf(cmd, 1024, "ps -ef|grep %s|grep -v \"grep\\|log\\|%d\"|awk {'print $2'}", v_pProcessName, getpid());
    
    printf("%s\n", cmd);
    
    if ((f = popen(cmd, "r")) == NULL)
    {
        printf("popen failed. [cmd: %s]\n", cmd);
        return -1;
    }
    
    if ((fgets(buf, 100, f) != NULL) && (strlen(buf) < 10))
    {
        pid = atoi(buf);
    }
    
    pclose(f);
    
    return pid;
}

char *GetProcessFile(char *v_pFileName, int v_size)
{
    char path[1024] ;
    int cnt;
    
    snprintf(path, 1024, "/proc/%d/exe", getpid());
    
    cnt = readlink(path, v_pFileName, v_size);
    
    if (cnt > 0)
    {
        if (cnt < v_size)
        {
            v_pFileName[cnt] = 0;
        }
        else
        {
            v_pFileName[cnt - 1] = 0;
        }
        
        return v_pFileName;
    }
    
    return NULL;
}

int main(int argc, char **argv)
{
    char fileName[1024];
    
    if (argc < 2)
    {
        printf("Usage: %s process_name\n", argv[0]);
        return -1;
    }
    
    printf("%d\n", GetProcessId(argv[1]));
    printf("%s\n", GetProcessFile(fileName, 1024));
    
    return 0;
}