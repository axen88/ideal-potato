/*
* 使用gcc -o avp_stack avp_stack.c avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdlib.h>

#include "avp_mutex.h"
#include "avp_stack.h"

typedef struct tagSTACK_S
{
    void **pMemb;
    int num;
    int max;
    OSP_CS cs;
} STACK_S; /* End of STACK_S */

void *StackCreate(int v_size)
{
    STACK_S *pS = NULL;

    pS = malloc(sizeof(STACK_S));
    if (NULL == pS)
    {
        return NULL;
    }

    pS->pMemb = malloc(sizeof(void*) * v_size);
    if (NULL == pS->pMemb)
    {
        free(pS);
        return NULL;
    }

    pS->num = 0;
    pS->max = v_size;
    CSInit(&pS->cs);

    return pS;
}

int StackPush(void *v_pS, void *v_pMemb)
{
    STACK_S *pS = v_pS;
    
    if (NULL == v_pS)
    {
        return -ERR_STACK_INVALID_PARA;
    }

    CSEnter(&pS->cs);
    if (pS->num >= pS->max)
    {
        CSLeave(&pS->cs);
        return -ERR_STACK_FULL;
    }

    pS->pMemb[pS->num++] = v_pMemb;

    CSLeave(&pS->cs);

    return 0;
}

int StackPop(void *v_pS, void **v_ppMemb)
{
    STACK_S *pS = v_pS;
    
    if (NULL == v_pS)
    {
        return -ERR_STACK_INVALID_PARA;
    }

    CSEnter(&pS->cs);
    if (pS->num <= 0)
    {
        CSLeave(&pS->cs);
        return -ERR_STACK_EMPTY;
    }

    *v_ppMemb = pS->pMemb[--pS->num];

    CSLeave(&pS->cs);

    return 0;
}

int StackPopPush(void *v_pS, void *v_pMembPush, void **v_ppMembPop)
{
    STACK_S *pS = v_pS;
    int ret = 0;
    
    if (NULL == v_pS)
    {
        return -ERR_STACK_INVALID_PARA;
    }

    CSEnter(&pS->cs);
    if (pS->num <= 0)
    {
        ret = ERR_STACK_EMPTY;
    }
    else
    {
        *v_ppMembPop = pS->pMemb[--pS->num];
    }
    
    pS->pMemb[pS->num++] = v_pMembPush;

    CSLeave(&pS->cs);

    return ret;
}

int StackGetSize(void *v_pS)
{
    if (NULL == v_pS)
    {
        return -ERR_STACK_INVALID_PARA;
    }

    return ((STACK_S *)v_pS)->num;
}

int StackDestroy(void *v_pS)
{
    STACK_S *pS = v_pS;
    
    if (NULL == v_pS)
    {
        return -ERR_STACK_INVALID_PARA;
    }

    CSDestroy(&pS->cs);
    free(pS->pMemb);
    free(pS);

    return 0;
}

#ifdef _EN_DEBUG_

#include <stdio.h>

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = StackCreate(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (StackPush(p, (void *)msg) < 0)
        {
            printf("----------Stack is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, StackGetSize(p));
    }
    
    msg = 15;
    while (msg--)
    {
        if (StackPopPush(p, (void *)msg, &pMsg) < 0)
        {
            printf("----------StackPopPush failed-----------\n");
            break;
        }

        printf("PopPush ok. [msgPop: %ld, msgPush: %ld, size: %d]\n",
            (long)pMsg, msg, StackGetSize(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (StackPop(p, &pMsg) < 0)
        {
            printf("----------Stack is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, StackGetSize(p));
    }

    StackDestroy(p);
    system("pause");

    return 0;
}

#endif

