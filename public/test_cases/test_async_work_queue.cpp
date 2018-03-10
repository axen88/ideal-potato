
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

int main(int argc, char *argv[])
{
    void *pThreadsPut = NULL;
    void *pThreadsGet = NULL;

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

    pThreadsPut = ThreadsGroupCreate(2, ThreadPutTask, NULL);
    if (NULL == pThreadsPut)
    {
        printf("Create threads put group failed.\n");
        delete g_pAWQ;
        g_pAWQ = NULL;
        return -1;
    }
    
    pThreadsGet = ThreadsGroupCreate(2, ThreadGetTask, NULL);
    if (NULL == pThreadsGet)
    {
        printf("Create threads get group failed.\n");
        delete g_pAWQ;
        g_pAWQ = NULL;
        return -1;
    }

    sleep(10);

    delete g_pAWQ;
    g_pAWQ = NULL;

    ThreadsGroupDestroy(pThreadsPut, 1, 0);
    ThreadsGroupDestroy(pThreadsGet, 1, 0);

    printf("Test finished.\n");
    
    return 0;
}


