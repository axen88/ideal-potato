/*
* 使用g++ -o avp_async_work_queue avp_async_work_queue.cpp avp_mutex.c
  avp_queue.c avp_threads_group.c -Wall -g -I./ -D_EN_ASYNC_ -lpthread进行编译链接
*/

#include <unistd.h>

#include "avp_queue.h"
#include "avp_threads_group.h"
#include "async_work_queue.hpp"

void *ThreadWork(void *v_pObj)
{
    CTAWQTask *pTaskPush = NULL;
    CTAWQTask *pTaskPop = NULL;
    CTAsyncWorkQueue *pAWQ = (CTAsyncWorkQueue *)v_pObj;
    int ret = 0;

    while (1)
    {
        if (NULL == pTaskPush)
        {
            ret = QueuePop(pAWQ->m_pRunQ, (void **)&pTaskPop);
        }
        else
        {
            ret = QueuePopPush(pAWQ->m_pRunQ, pTaskPush, (void **)&pTaskPop);
        }

        if (0 != ret)
        {
            usleep(100 * 1000);
            pTaskPush = NULL;
            continue;
        }

        ret = pTaskPop->OnRun();
        if (0 != ret)
        {
            pTaskPush = pTaskPop;
            continue;
        }

        pTaskPush = NULL;
        
        do
        {
            ret = QueuePush(pAWQ->m_pEndQ, pTaskPop);
        } while (0 > ret);
    }

    return NULL;
}

CTAsyncWorkQueue::CTAsyncWorkQueue()
{
    m_pRunQ = NULL;
    m_pEndQ = NULL;
    m_pThreadsGroup = NULL;
    m_stopped = 0;
}

CTAsyncWorkQueue::~CTAsyncWorkQueue()
{
    int size = 0;

    m_stopped = 1;

    if ((NULL != m_pEndQ) && (NULL != m_pRunQ))
    {
        while (1)
        {
            size = QueueGetSize(m_pEndQ) + QueueGetSize(m_pRunQ);
            if (0 == size)
            {
                break;
            }

            printf("QueueGetSize(m_pEndQ): %d, QueueGetSize(m_pRunQ): %d\n",
                QueueGetSize(m_pEndQ), QueueGetSize(m_pRunQ));
            usleep(100 * 1000);
        }
    }

    if (NULL == m_pThreadsGroup)
    {
        ThreadsGroupDestroy(m_pThreadsGroup, 1, 0);
        m_pThreadsGroup = NULL;
    }

    if (NULL == m_pRunQ)
    {
        QueueDestroy(m_pRunQ);
        m_pRunQ = NULL;
    }

    if (NULL == m_pEndQ)
    {
        QueueDestroy(m_pEndQ);
        m_pEndQ = NULL;
    }
}

int CTAsyncWorkQueue::Start(int v_threadsNum, int v_runQSize, int v_endQSize)
{
    m_pRunQ = QueueCreate(v_runQSize);
    if (NULL == m_pRunQ)
    {
        return -1;
    }

    m_pEndQ = QueueCreate(v_endQSize);
    if (NULL == m_pEndQ)
    {
        return -1;
    }

    
    m_pThreadsGroup = ThreadsGroupCreate(v_threadsNum, ThreadWork, this);
    if (NULL == m_pThreadsGroup)
    {
        return -1;
    }

    return 0;
}

int CTAsyncWorkQueue::TryPutTask(CTAWQTask *v_pTask)
{
    if (m_stopped)
    {
        return -1;
    }

    return QueuePush(m_pRunQ, v_pTask);
}

int CTAsyncWorkQueue::TryGetTask(CTAWQTask **v_ppTask)
{
    return QueuePop(m_pEndQ, (void **)v_ppTask);
}


