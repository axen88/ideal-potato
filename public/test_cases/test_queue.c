
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  

#include "../os_adapter.h"
#include "../queue.h"


void test_queue_case0(void)
{
#define Q_SIZE 7
#define MEMB   90
        
    void *q = NULL;
    long push_msg = 0;
    void *pop_msg = NULL;

    CU_ASSERT((q = queue_create(Q_SIZE)) != NULL);

    push_msg = MEMB;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == -ERR_QUEUE_FULL);
    CU_ASSERT(queue_get_size(q) == Q_SIZE);

    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+1);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+2);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+3);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+4);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+5);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+6);
    CU_ASSERT(queue_pop(q, &pop_msg) == -ERR_QUEUE_EMPTY);
    CU_ASSERT(queue_get_size(q) == 0);
    
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    CU_ASSERT(queue_push(q, (void *)push_msg) == 0); push_msg++;
    
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+7);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+8);
    CU_ASSERT(queue_pop(q, &pop_msg) == 0); CU_ASSERT((long)pop_msg == MEMB+9);
    CU_ASSERT(queue_pop(q, &pop_msg) == -ERR_QUEUE_EMPTY);
    CU_ASSERT(queue_get_size(q) == 0);

    queue_destroy(q);

}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_queue_cases[]
= {  
    {to_str(test_queue_case0), test_queue_case0},  
    CU_TEST_INFO_NULL  
};  


