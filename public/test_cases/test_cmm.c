
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  

#include "../os_adapter.h"
#include "../cmm/cmm.h"

void test_cmm_case0(void)
{
    void *pMem1 = NULL;
    void *pMem2 = NULL;

    MemInit();
    CU_ASSERT(MemGetTotal() == 0);
    CU_ASSERT(MemGetMax() == 0);

    pMem1 = MemAlloc(1024);
    CU_ASSERT(MemGetTotal() == 1024);
    CU_ASSERT(MemGetMax() == 1024);
    
    pMem2 = MemAlignedAlloc(2048, 4096);
    CU_ASSERT(MemGetTotal() == 1024+2048);
    CU_ASSERT(MemGetMax() == 1024+2048);

    MemFree(pMem1, 1024);
    CU_ASSERT(MemGetTotal() == 2048);
    CU_ASSERT(MemGetMax() == 1024+2048);

    MemAlignedFree(pMem2, 2048);
    CU_ASSERT(MemGetTotal() == 0);
    CU_ASSERT(MemGetMax() == 1024+2048);

    CU_ASSERT(MemExit() == 0);

}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_cmm_cases[]
= {  
    {to_str(test_cmm_case0), test_cmm_case0},  
    CU_TEST_INFO_NULL  
};  


