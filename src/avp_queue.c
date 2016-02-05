/*
* 使用gcc -o avp_queue avp_queue.c avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdlib.h>

#include "avp_mutex.h"
#include "avp_queue.h"

typedef struct tagQUEUE_S
{
    void **pMemb;
    int head;
    int tail;
    int num;
    int max;
    OSP_CS cs;
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
    pQ->num = 0;
    pQ->max = v_size;
    CSInit(&pQ->cs);

    return pQ;
}

int QueuePush(void *v_pQ, void *v_pMemb)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    CSEnter(&pQ->cs);
    if (pQ->num >= pQ->max)
    {
        CSLeave(&pQ->cs);
        return -ERR_QUEUE_FULL;
    }

    pQ->pMemb[pQ->tail++] = v_pMemb;
    if (pQ->tail >= pQ->max)
    {
        pQ->tail = 0;
    }

    pQ->num++;

    CSLeave(&pQ->cs);

    return 0;
}

int QueuePop(void *v_pQ, void **v_ppMemb)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    CSEnter(&pQ->cs);
    if (pQ->num <= 0)
    {
        CSLeave(&pQ->cs);
        return -ERR_QUEUE_EMPTY;
    }

    *v_ppMemb = pQ->pMemb[pQ->head++];
    if (pQ->head >= pQ->max)
    {
        pQ->head = 0;
    }

    pQ->num--;

    CSLeave(&pQ->cs);

    return 0;
}

int QueuePopPush(void *v_pQ, void *v_pMembPush, void **v_ppMembPop)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    int ret = 0;
    
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    CSEnter(&pQ->cs);
    if (pQ->num <= 0)
    {
        ret = ERR_QUEUE_EMPTY;
    }
    else
    {
        *v_ppMembPop = pQ->pMemb[pQ->head++];
        if (pQ->head >= pQ->max)
        {
            pQ->head = 0;
        }
        
        pQ->num--;
    }
    
    pQ->pMemb[pQ->tail++] = v_pMembPush;
    if (pQ->tail >= pQ->max)
    {
        pQ->tail = 0;
    }

    pQ->num++;

    CSLeave(&pQ->cs);

    return ret;
}

int QueueGetSize(void *v_pQ)
{
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    return ((QUEUE_S *)v_pQ)->num;
}

int QueueDestroy(void *v_pQ)
{
    QUEUE_S *pQ = (QUEUE_S *)v_pQ;
    
    if (NULL == v_pQ)
    {
        return -ERR_QUEUE_INVALID_PARA;
    }

    CSDestroy(&pQ->cs);
    free(pQ->pMemb);
    free(pQ);

    return 0;
}

#ifdef _EN_DEBUG_

#include <stdio.h>

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = QueueCreate(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (QueuePush(p, (void *)msg) < 0)
        {
            printf("----------Queue is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, QueueGetSize(p));
    }
    
    msg = 15;
    while (msg--)
    {
        if (QueuePopPush(p, (void *)msg, &pMsg) < 0)
        {
            printf("----------QueuePopPush failed-----------\n");
            break;
        }

        printf("PopPush ok. [msgPop: %ld, msgPush: %ld, size: %d]\n",
            (long)pMsg, msg, QueueGetSize(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (QueuePop(p, &pMsg) < 0)
        {
            printf("----------Queue is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, QueueGetSize(p));
    }

    QueueDestroy(p);
    system("pause");

    return 0;
}

#endif

