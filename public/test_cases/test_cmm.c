
#include <stdio.h>

#include "../os_adapter.h"
#include "../cmm/cmm.h"

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


