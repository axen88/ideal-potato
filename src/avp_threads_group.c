/*
* 使用gcc -o avp_threads_group avp_threads_group.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "avp_threads_group.h"

typedef struct tagTHREAD_ARRAY_S
{
    int realNum;
    pthread_t *pPids;
} THREAD_ARRAY_S; /* End of tagTHREAD_ARRAY_S */

void *ThreadsGroupCreate(int v_num, void *(*v_pFunc)(void *),
    void *v_pArg)
{
    THREAD_ARRAY_S *pThreadGroup = NULL;
    int i = 0;
    int ret = 0;
    pthread_t tid = 0;

    pThreadGroup = (THREAD_ARRAY_S *)malloc(sizeof(THREAD_ARRAY_S));
    if (NULL == pThreadGroup)
    {
        return NULL;
    }

    pThreadGroup->pPids = (pthread_t *)malloc(sizeof(pthread_t) * v_num);
    if (NULL == pThreadGroup->pPids)
    {
        free(pThreadGroup);
        return NULL;
    }

    for (i = 0; i < v_num; i++)
    {
        ret = pthread_create(&tid, NULL, v_pFunc, v_pArg);
        if (ret == 0)
        {
            pThreadGroup->pPids[i] = tid;
        }
        else
        {
            break;
        }
    }

    if (0 == i)
    {
        free(pThreadGroup->pPids);
        free(pThreadGroup);
        return NULL;
    }

    pThreadGroup->realNum = i;

    return pThreadGroup;
}

int ThreadsGroupGetRealNum(void *v_pThreadGroup)
{
    if (NULL == v_pThreadGroup)
    {
        return -ERR_THREADS_GROUP_INVALID_PARA;
    }
    
    return ((THREAD_ARRAY_S *)v_pThreadGroup)->realNum;
}

int ThreadsGroupDestroy(void *v_pThreadGroup, int v_force,
    unsigned long long v_msOverTime)
{
    int i = 0;
    THREAD_ARRAY_S *pThreadGroup = (THREAD_ARRAY_S *)v_pThreadGroup;

    if (NULL == pThreadGroup)
    {
        return -ERR_THREADS_GROUP_INVALID_PARA;
    }

    for (i = 0; i < pThreadGroup->realNum; i++)
    {
        if (v_force)
        {
            pthread_cancel(pThreadGroup->pPids[i]);
        }
        
        pthread_join(pThreadGroup->pPids[i], NULL);
    }

    free(pThreadGroup->pPids);
    free(pThreadGroup);

    return 0;
}

#ifdef _EN_DEBUG_

#include <stdio.h>
#include <unistd.h>

void *ThreadTest(void *v_pPara)
{
	int cnt = 0;
	
	while (1)
	{
		printf("Thread running. [pthread_self(): %lu, cnt: %d]\n",
            pthread_self(), cnt++);
		sleep(1);
	}
	
    printf("Thread finished. [pthread_self(): %lu, cnt: %d]\n",
        pthread_self(), cnt);
    
	return NULL;
}

int main(int argc, char *argv[])
{
    #define THREADS_NUM 10
    
    int realNum = 0;
    
    THREAD_ARRAY_S *pThreadGroup = ThreadsGroupCreate(THREADS_NUM, ThreadTest, NULL);
    if (NULL == pThreadGroup)
    {
        printf("Create threads group failed.\n");

        return -1;
    }

    realNum = ThreadsGroupGetRealNum(pThreadGroup);
    if (realNum != THREADS_NUM)
    {
        printf("Threads number not match. [THREADS_NUM: %d, realNum: %d]\n",
            THREADS_NUM, realNum);
    }

    sleep(10);

	ThreadsGroupDestroy(pThreadGroup, 1, 0);
	
	return 0;
}

#endif

