
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>  
#include <CUnit/Console.h>  
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  
#include <CUnit/Automated.h>  

// 测试用例集合
extern CU_TestInfo test_queue_cases[];
extern CU_TestInfo test_stack_cases[];
extern CU_TestInfo test_ini_cases[];
extern CU_TestInfo test_cmm_cases[];
extern CU_TestInfo test_log_cases[];
extern CU_TestInfo test_radix_tree_cases[];
extern CU_TestInfo test_hashtab_cases[];

  
// suite初始化过程 
int suite_success_init(void)
{  
    return 0;  
      
}  
  
// suite清理过程，以便恢复原状，使结果不影响到下次运行 
int suite_success_clean(void)
{  
    return 0;  
}  
  
// 定义suite数组，包括多个suite，每个suite又会包括若干个测试方法。  
CU_SuiteInfo suites[]
= {  
    {"test_queue_suite", suite_success_init, suite_success_clean, test_queue_cases},  
    {"test_stack_suite", suite_success_init, suite_success_clean, test_stack_cases},  
    {"test_ini_suite", suite_success_init, suite_success_clean, test_ini_cases},  
    {"test_cmm_suite", suite_success_init, suite_success_clean, test_cmm_cases},  
    {"test_log_suite", suite_success_init, suite_success_clean, test_log_cases},  
    {"test_radix_tree_suite", suite_success_init, suite_success_clean, test_radix_tree_cases},  
    {"test_hashtab_suite", suite_success_init, suite_success_clean, test_hashtab_cases},  
    CU_SUITE_INFO_NULL  
};  
  
// 测试类的调用总接口 
void add_test_suites(void)
{  
    assert(NULL != CU_get_registry());  
    assert(!CU_is_test_running());  
  
    if (CUE_SUCCESS != CU_register_suites(suites))
    {  
        exit(EXIT_FAILURE);  
    }  
}  

// 运行测试入口 
int run_test(void)
{  
    if (CU_initialize_registry())
    {  
        fprintf(stderr, "Initialize Test Registry failed.");  
        exit(EXIT_FAILURE);  
    }

    add_test_suites();  
    
    /// 测试模式1: Automated Mode，自动模式，会生成报表，用浏览器查看测试结果
    //CU_set_output_filename("test_suite"); 
    //CU_list_tests_to_file(); 
    //CU_automated_run_tests(); 

    // 测试模式2: Basice Mode，基础模式，直接看测试结果
    CU_basic_set_mode(CU_BRM_VERBOSE); 
    CU_basic_run_tests(); 
    

    // 测试模式3: Console Mode，交互模式，在交互界面上运行测试或查看测试结果
    //CU_console_run_tests(); 


    CU_cleanup_registry();  

    return CU_get_error();  

}  

int main(int argc, char *argv[])
{
    run_test();
    
    return 0;
}

