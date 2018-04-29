
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  


#include "../os_adapter.h"
#include "../stack.h"

void test_stack_case0(void)
{
#define S_SIZE 7
#define MEMB   90
        
    void *q = NULL;
    long push_msg = 0;
    void *pop_msg = NULL;

    CU_ASSERT((q = stack_create(S_SIZE)) != NULL);

    push_msg = MEMB;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == -ERR_STACK_FULL);
    CU_ASSERT(stack_get_size(q) == S_SIZE);

    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+6);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+5);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+4);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+3);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+2);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+1);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+0);
    CU_ASSERT(stack_pop(q, &pop_msg) == -ERR_STACK_EMPTY);
    CU_ASSERT(stack_get_size(q) == 0);
    
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(stack_push(q, (void *)push_msg) == 0); push_msg++;
    
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+9);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+8);
    CU_ASSERT(stack_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+7);
    CU_ASSERT(stack_pop(q, &pop_msg) == -ERR_STACK_EMPTY);
    CU_ASSERT(stack_get_size(q) == 0);

    stack_destroy(q);

}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_stack_cases[]
= {  
    {to_str(test_stack_case0), test_stack_case0},  
    CU_TEST_INFO_NULL  
};  


