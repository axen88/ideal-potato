#if 0
#include <new>
#include <cstddef>

static void NewHandler(void)throw()
{

}

void DisableNew(void)
{
    std::set_new_handler(NewHandler);
}
#else
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int StartService(int argc, char *argv[])
{
    printf("Start service ok.\n");
    
    return 0;
}

int StopService(void)
{
    printf("Stop service ok.\n");
    
    return 0;
}

static int g_exitFlag = 0;

void StopDaemon(int v_sig)
{
    printf("Catch signal(%d), exit.\n", v_sig);

    g_exitFlag = 1;

    return;
}

void MakeDaemon(void)
{
    int ret = 0;
    int pid = fork();

    if (pid < 0)
    {
        printf("fork failed.\n");
        exit(pid);
    }

    if (pid > 0)
    {
        exit(0);
    }

    /* 创建一个新的会话组 */
    ret = setsid();
    if (ret < 0)
    {
        printf("setsid failed. [ret: %d]\n", ret);
        exit(ret);
    }

    /* 重设文件创建掩模 */
    umask(0);

    return;
}

int GetProcessId(const char *v_pProcessName)
{
    FILE *f;
    char cmd[1024] ;
    char buf[100];
    int pid = 0;
    
    snprintf(cmd, 1024, "ps -ef|grep %s|grep -v \"grep\\|log\\|%d\"|awk {'print $2'}",
        v_pProcessName, getpid());
    
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

void SetSignals(void)
{
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN); /* 与终端连接断开 */
    signal(SIGABRT, SIG_IGN); /* 屏蔽退出信号 */
    signal(SIGTERM, StopDaemon); /* 由kill函数产生的信号 */
    signal(SIGKILL, StopDaemon); /* 由kill函数产生的信号 */
    signal(SIGINT, StopDaemon); /* 来自终端的ctrl+c和delete */

    return;
}

int main(int argc, char **argv)
{
    #define START 0x01
    #define STOP  0x02
    
    int pid = 0;
    int daemon = 0;
    
    if (argc > 1)
    {
        if (strcasecmp(argv[1], "stop") == 0)
        {
            daemon = STOP;
        }
        else if (strcasecmp(argv[1], "restart") == 0)
        {
            daemon = START | STOP;
        }
        else if (strcasecmp(argv[1], "start") == 0)
        {
            daemon = START;
        }
        else
        {
            printf("usage: %s [start|stop|restart]\n", argv[0]);
            return -1;
        }
    }
    
    if (daemon & STOP)
    {
        pid = GetProcessId(argv[0]);
        if ((pid > 0) && (kill(pid, SIGTERM) >= 0))
        {
            alarm(0);
            do
            {
                sleep(1);
            } while (kill(pid, 0) >= 0);
        }

        printf("%s stop ok.\n", argv[0]);

        if ((daemon & START) == 0)
        {
            return 0;
        }
    }

    pid = GetProcessId(argv[0]);
    if (pid > 0)
    {
        printf("%s already exist. [pid: %d]\n", argv[0], pid);
        return 1;
    }

    g_exitFlag = 0;
    SetSignals();
    //chdir("/home");
    if (daemon != 0)
    {
        MakeDaemon();
    }

    printf("%s initializing now...\n", argv[0]);

    if (StartService(argc, argv) != 0)
    {
        printf("%s initialize failed!\n", argv[0]);
        StopService();
        return -1;
    }
    
    printf("%s initialize finished!\n", argv[0]);

    while (g_exitFlag == 0)
    {
        sleep(1);
    }

    StopService();
    printf("%s stop finished.\n", argv[0]);
    
    return 0;
}



