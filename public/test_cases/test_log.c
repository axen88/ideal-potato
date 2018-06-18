#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  


#include "../os_adapter.h"
#include "../log.h"

MODULE_ID(10);


void test_log_case0(void)
{
    int i;

    LOG_SYSTEM_INIT("./log", "test");
    
    i = 0;
    LOG_DEBUG("Test %d\n", i++);
    LOG_INFO("Test %d\n", i++);
    LOG_WARN("Test %d\n", i++);
    LOG_ERROR("Test %d\n", i++);
    LOG_EVENT("Test %d\n", i++);
    LOG_EMERG("Test %d\n", i++);
    
    LOG_EVENT("get log level %d\n", LOG_GET_LEVEL());

    LOG_SET_LEVEL(5);
    CU_ASSERT(LOG_GET_LEVEL() == 5);
    
    LOG_EVENT("set log level 5, real: %d\n", LOG_GET_LEVEL());
    
    LOG_DEBUG("Test %d\n", i++);
    LOG_INFO("Test %d\n", i++);
    LOG_WARN("Test %d\n", i++);
    LOG_ERROR("Test %d\n", i++);
    LOG_EVENT("Test %d\n", i++);
    LOG_EMERG("Test %d\n", i++);
    
    LOG_SET_LEVEL(3);
    CU_ASSERT(LOG_GET_LEVEL() == 3);

    LOG_SYSTEM_EXIT();

}


// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_log_cases[]
= {  
    {to_str(test_log_case0), test_log_case0},  
    CU_TEST_INFO_NULL  
};  

