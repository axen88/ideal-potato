/*
* 使用g++ -o avp_queue avp_queue.cpp avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdlib.h>

#include "avp_queue.hpp"

CTQueue::CTQueue(unsigned int v_maxsize)
{
    m_maxsize = v_maxsize;
    CSInit(&m_cs);
}

int CTQueue::Push(void *v_pMemb)
{
    CSEnter(&m_cs);
    if (m_q.size() >= m_maxsize)
    {
        CSLeave(&m_cs);
        return -ERR_QUEUE_FULL;
    }

    m_q.push(v_pMemb);
    CSLeave(&m_cs);

    return 0;
}

int CTQueue::Pop(void **v_ppMemb)
{
    CSEnter(&m_cs);
    if (m_q.empty())
    {
        CSLeave(&m_cs);
        return -ERR_QUEUE_EMPTY;
    }

    *v_ppMemb = m_q.front();
    m_q.pop();
    CSLeave(&m_cs);

    return 0;
}

int CTQueue::PopPush(void *v_pMembPush, void **v_ppMembPop)
{
    int ret = 0;
    
    CSEnter(&m_cs);
    if (m_q.empty())
    {
        ret = ERR_QUEUE_EMPTY;
    }
    else
    {
        *v_ppMembPop = m_q.front();
        m_q.pop();
    }
    
    m_q.push(v_pMembPush);
    CSLeave(&m_cs);

    return ret;
}

unsigned int CTQueue::GetSize(void)
{
    unsigned int size = 0;
    
    CSEnter(&m_cs);
    size = m_q.size();
    CSLeave(&m_cs);

    return size;
}

CTQueue::~CTQueue()
{
    CSDestroy(&m_cs);
}

#ifdef _EN_DEBUG_

#include <stdio.h>

int main(int argc, char *argv[])
{
    CTQueue *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = new CTQueue(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (p->Push((void *)msg) < 0)
        {
            printf("----------Queue is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, p->GetSize());
    }
    
    msg = 15;
    while (msg--)
    {
        if (p->PopPush((void *)msg, &pMsg) < 0)
        {
            printf("----------QueuePopPush failed-----------\n");
            break;
        }

        printf("PopPush ok. [msgPop: %ld, msgPush: %ld, size: %d]\n",
            (long)pMsg, msg, p->GetSize());
    }
    
    msg = 60;
    while (msg--)
    {
        if (p->Pop(&pMsg) < 0)
        {
            printf("----------Queue is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, p->GetSize());
    }

    delete p;
    system("pause");

    return 0;
}

#endif

