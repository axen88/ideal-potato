
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  

#include "../os_adapter.h"
#include "../obs_event.h"


void test_obs1(void *arg)
{
    u64_t *obs = arg;

    (*obs) += 1;
}

void test_obs2(void *arg)
{
    u64_t *obs = arg;

    (*obs) *= 2;
}

void test_obs_event_case0(void)
{
    int ret;
    u64_t obs1 = 1;
    u64_t obs2 = 1;
    
    ret = create_obs_event_single(10);

    ret = register_obs_event_single(1, test_obs1);
    CU_ASSERT(ret == 0);
    ret = register_obs_event_single(1, test_obs1);
    CU_ASSERT(ret == 0);
    ret = register_obs_event_single(2, test_obs2);
    CU_ASSERT(ret == 0);

    ret = notify_obs_event_single(1, &obs1);
    CU_ASSERT(ret == 0);
    ret = notify_obs_event_single(2, &obs2);
    CU_ASSERT(ret == 0);
    CU_ASSERT(obs1 == 3);
    CU_ASSERT(obs2 == 2);

    obs1 = 1;
    obs2 = 1;
    ret = register_obs_event_single(1, test_obs2);
    CU_ASSERT(ret == 0);
    ret = register_obs_event_single(2, test_obs1);
    CU_ASSERT(ret == 0);
    
    ret = notify_obs_event_single(1, &obs1);
    CU_ASSERT(ret == 0);
    ret = notify_obs_event_single(2, &obs2);
    CU_ASSERT(ret == 0);
    CU_ASSERT(obs1 == 6);
    CU_ASSERT(obs2 == 3);

    destroy_obs_event_single();

}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_obs_event_cases[]
= {  
    {to_str(test_obs_event_case0), test_obs_event_case0},  
    CU_TEST_INFO_NULL  
};  


