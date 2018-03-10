
#include "../os_adapter.h"
#include "../async_work_queue.hpp"

CTAsyncWorkQueue *g_pAWQ = NULL;
int g_taskID = 0;

void *ThreadPutTask(void *)
{
    while (1)
    {
        int taskID = g_taskID++;
        CTAWQTask *pTask = new CTAWQTask(taskID);
        if (NULL == pTask)
        {
            printf("new task failed. [taskID: %d]\n", taskID);
            sleep(1);
            continue;
        }

        while (g_pAWQ->TryPutTask(pTask) < 0)
        {
            printf("Put task failed. [taskID: %d]\n", taskID);
            sleep(1);
        }
        
        printf("Put task success. [taskID: %d]\n", taskID);
    }

    return NULL;
}

void *ThreadGetTask(void *)
{
    while (1)
    {
        CTAWQTask *pTask = NULL;
        while (g_pAWQ->TryGetTask(&pTask) < 0)
        {
            printf("Get task failed.\n");
            sleep(1);
        }
        
        printf("Get task success. [taskID: %d]\n", pTask->GetTaskId());
        delete pTask;
    }

    return NULL;
}

#define THREADS_NUM  2

void create_multi_threads(os_thread_t *tid, int tid_num,
    void *(*func)(void *), void *para, char *name)
{
    int i;

    for (i = 0; i < tid_num; i++)
    {
        tid[i] = INVALID_TID;
    }
    
    for (i = 0; i < tid_num; i++)
    {
        tid[i] = thread_create(func, para, name);
    }
}

void destroy_multi_threads(os_thread_t *tid, int tid_num)
{
    int i;
    
    for (i = 0; i < tid_num; i++)
    {
        if (tid[i] != INVALID_TID)
        {
            thread_destroy(tid[i], 0);
        }
    }
}

int main(int argc, char *argv[])
{
    os_thread_t thread1[THREADS_NUM];
    os_thread_t thread2[THREADS_NUM];

    g_pAWQ = new CTAsyncWorkQueue();
    if (NULL == g_pAWQ)
    {
        printf("new CTAsyncWorkQueue failed.\n");
        return -1;
    }

    if (g_pAWQ->Start(3, 10, 15) < 0)
    {
        printf("Start CTAsyncWorkQueue failed.\n");
        delete g_pAWQ;
        g_pAWQ = NULL;
        return -1;
    }

    create_multi_threads(thread1, THREADS_NUM, ThreadPutTask, NULL, NULL);
    create_multi_threads(thread2, THREADS_NUM, ThreadGetTask, NULL, NULL);

    sleep(10);

    delete g_pAWQ;
    g_pAWQ = NULL;

    destroy_multi_threads(thread1, THREADS_NUM);
    destroy_multi_threads(thread2, THREADS_NUM);

    printf("Test finished.\n");
    
    return 0;
}


