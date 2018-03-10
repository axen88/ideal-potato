/*
* 使用g++ -o avp_queue avp_queue.cpp avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdlib.h>

#include "avp_stack.hpp"

CTStack::CTStack(unsigned int v_maxsize)
{
    m_maxsize = v_maxsize;
    CSInit(&m_cs);
}

int CTStack::Push(void *v_pMemb)
{
    CSEnter(&m_cs);
    if (m_s.size() >= m_maxsize)
    {
        CSLeave(&m_cs);
        return -ERR_STACK_FULL;
    }

    m_s.push(v_pMemb);
    CSLeave(&m_cs);

    return 0;
}

int CTStack::Pop(void **v_ppMemb)
{
    CSEnter(&m_cs);
    if (m_s.empty())
    {
        CSLeave(&m_cs);
        return -ERR_STACK_EMPTY;
    }

    *v_ppMemb = m_s.top();
    m_s.pop();
    CSLeave(&m_cs);

    return 0;
}

int CTStack::PopPush(void *v_pMembPush, void **v_ppMembPop)
{
    int ret = 0;
    
    CSEnter(&m_cs);
    if (m_s.empty())
    {
        ret = ERR_STACK_EMPTY;
    }
    else
    {
        *v_ppMembPop = m_s.top();
        m_s.pop();
    }
    
    m_s.push(v_pMembPush);
    CSLeave(&m_cs);

    return ret;
}

unsigned int CTStack::GetSize(void)
{
    unsigned int size = 0;
    
    CSEnter(&m_cs);
    size = m_s.size();
    CSLeave(&m_cs);

    return size;
}

CTStack::~CTStack()
{
    CSDestroy(&m_cs);
}

#ifdef _EN_DEBUG_

#include <stdio.h>

int main(int argc, char *argv[])
{
    CTStack *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = new CTStack(10);
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
            printf("----------Stack is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, p->GetSize());
    }
    
    msg = 15;
    while (msg--)
    {
        if (p->PopPush((void *)msg, &pMsg) < 0)
        {
            printf("----------StackPopPush failed-----------\n");
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
            printf("----------Stack is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, p->GetSize());
    }

    delete p;
    system("pause");

    return 0;
}

#endif

