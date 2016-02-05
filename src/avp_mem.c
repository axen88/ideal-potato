/*
* 使用gcc -o avp_mem avp_mem.c avp_mutex.c -Wall -g -I./ -D_EN_DEBUG_ -lpthread进行编译链接
*/

#include <stdlib.h>

#include "avp_mutex.h"

static OSP_CS g_csMem;
static unsigned long long g_totalMem = 0;
static unsigned long long g_maxMem = 0;

void MemInit(void)
{
    g_totalMem = 0;
    g_maxMem = 0;
    CSInit(&g_csMem);
}

unsigned long long MemGetTotal(void)
{
    unsigned long long size = 0;

    CSEnter(&g_csMem);
    size = g_totalMem;
    CSLeave(&g_csMem);

    return size;
}


unsigned long long MemGetMax(void)
{
    unsigned long long size = 0;

    CSEnter(&g_csMem);
    size = g_maxMem;
    CSLeave(&g_csMem);

    return size;
}

unsigned long long MemExit(void)
{
    unsigned long long size = 0;

    CSEnter(&g_csMem);
    size = g_totalMem;
    CSLeave(&g_csMem);
    
    CSDestroy(&g_csMem);

    return size;
}

void *MemAlloc(unsigned int v_size)
{
    void *pMem = malloc(v_size);
    if (NULL != pMem)
    {
        CSEnter(&g_csMem);
        g_totalMem += v_size;
        if (g_maxMem < g_totalMem)
        {
            g_maxMem = g_totalMem;
        }
        
        CSLeave(&g_csMem);
    }

    return pMem;
}

void MemFree(void *v_pMem, unsigned int v_size)
{
    if (NULL == v_pMem)
    {
        return;
    }
    
    free(v_pMem);
    CSEnter(&g_csMem);
    g_totalMem -= v_size;
    CSLeave(&g_csMem);
}

void *MemAlignedAlloc(unsigned int v_size, unsigned int v_alignment)
{
    void *pMem = NULL;

#ifdef WIN32
    pMem = _aligned_malloc(v_size, v_alignment);
#else
    if (posix_memalign(&pMem, v_alignment, v_size) != 0)
    {
        return NULL;
    }
#endif

    if (NULL != pMem)
    {
        CSEnter(&g_csMem);
        g_totalMem += v_size;
        if (g_maxMem < g_totalMem)
        {
            g_maxMem = g_totalMem;
        }
        
        CSLeave(&g_csMem);
    }

    return pMem;
}

void MemAlignedFree(void *v_pMem, unsigned int v_size)
{
    if (NULL == v_pMem)
    {
        return;
    }
    
#ifdef WIN32
    _aligned_free(v_pMem);
#else
    free(v_pMem);
#endif

    CSEnter(&g_csMem);
    g_totalMem -= v_size;
    CSLeave(&g_csMem);
}

#ifdef _EN_DEBUG_

#include <stdio.h>

int main(int argc, char *argv[])
{
    void *pMem1 = NULL;
    void *pMem2 = NULL;

    MemInit();
    printf("total: %lld, max: %lld\n", MemGetTotal(), MemGetMax());

    pMem1 = MemAlloc(1024);
    printf("total: %lld, max: %lld\n", MemGetTotal(), MemGetMax());
    
    pMem2 = MemAlignedAlloc(2048, 4096);
    printf("total: %lld, max: %lld\n", MemGetTotal(), MemGetMax());

    printf("pMem1: %p, pMem2: %p\n", pMem1, pMem2);

    MemFree(pMem1, 1024);
    printf("total: %lld, max: %lld\n", MemGetTotal(), MemGetMax());

    MemAlignedFree(pMem2, 2048);
    printf("total: %lld, max: %lld\n", MemGetTotal(), MemGetMax());

    printf("remain: %lld\n", MemExit());

    system("pause");

    return 0;
}

#endif


