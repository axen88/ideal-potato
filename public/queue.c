/*
* 使用gcc -o avp_queue avp_queue.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
* 此模块只能适用于一个读线程和一个写线程会同时操作此队列的情况
*/

#include <stdlib.h>

#include "avp_queue.h"

//#ifndef ASSERT
#define ASSERT(x)
//#endif

typedef struct tagQUEUE_S
{
    void **pMemb;
    int head;
    int tail;
    int size;
} QUEUE_S; /* End of QUEUE_S */

void *QueueCreate(int v_size)
{
    QUEUE_S *pQ = NULL;

    pQ = (QUEUE_S *)malloc(sizeof(QUEUE_S));
    if (NULL == pQ)
    {
        return NULL;
    }

    pQ->pMemb = (void **)malloc(sizeof(void*) * v_size);
    if (NULL == pQ->pMemb)
    {
        free(pQ);
        return NULL;
    }

    pQ->head = 0;
    pQ->tail = 0;
    pQ->size = v_size;

    return pQ;
}

int QueuePush(void *v_pQ, void *v_pMemb)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    ASSERT(NULL != v_pQ);

    if (((pQ->tail + 1) % pQ->size) == pQ->head)
    {
        return -ERR_QUEUE_FULL;
    }

    pQ->pMemb[pQ->tail++] = v_pMemb;
    pQ->tail %= pQ->size;

    return 0;
}

int QueuePop(void *v_pQ, void **v_ppMemb)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    ASSERT(NULL != v_pQ);

    if (pQ->head == pQ->tail)
    {
        return -ERR_QUEUE_EMPTY;
    }

    *v_ppMemb = pQ->pMemb[pQ->head++];
    pQ->head %= pQ->size;

    return 0;
}

int QueueGetSize(void *v_pQ)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    ASSERT(NULL != v_pQ);

    return (pQ->tail - pQ->head + pQ->size) % pQ->size;
}

int QueueDestroy(void *v_pQ)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    free(pQ->pMemb);
    free(pQ);

    return 0;
}


