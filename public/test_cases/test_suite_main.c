
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <CUnit/Basic.h>  
#include <CUnit/Console.h>  
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  
#include <CUnit/Automated.h>  

// ������������
extern CU_TestInfo test_queue_cases[];
extern CU_TestInfo test_stack_cases[];
extern CU_TestInfo test_ini_cases[];
extern CU_TestInfo test_cmm_cases[];
extern CU_TestInfo test_log_cases[];
extern CU_TestInfo test_radix_tree_cases[];
extern CU_TestInfo test_hashtab_cases[];
extern CU_TestInfo test_obs_event_cases[];

  
// suite��ʼ������ 
int suite_success_init(void)
{  
    return 0;  
      
}  
  
// suite������̣��Ա�ָ�ԭ״��ʹ�����Ӱ�쵽�´����� 
int suite_success_clean(void)
{  
    return 0;  
}  
  
// ����suite���飬�������suite��ÿ��suite�ֻ�������ɸ����Է�����  
CU_SuiteInfo suites[]
= {  
    {"test_queue_suite", suite_success_init, suite_success_clean, test_queue_cases},  
    {"test_stack_suite", suite_success_init, suite_success_clean, test_stack_cases},  
    {"test_ini_suite", suite_success_init, suite_success_clean, test_ini_cases},  
    {"test_cmm_suite", suite_success_init, suite_success_clean, test_cmm_cases},  
    {"test_log_suite", suite_success_init, suite_success_clean, test_log_cases},  
    {"test_radix_tree_suite", suite_success_init, suite_success_clean, test_radix_tree_cases},  
    {"test_hashtab_suite", suite_success_init, suite_success_clean, test_hashtab_cases},  
    {"test_obs_event_cases", suite_success_init, suite_success_clean, test_obs_event_cases},  
    CU_SUITE_INFO_NULL  
};  
  
// ������ĵ����ܽӿ� 
void add_test_suites(void)
{  
    assert(NULL != CU_get_registry());  
    assert(!CU_is_test_running());  
  
    if (CUE_SUCCESS != CU_register_suites(suites))
    {  
        exit(EXIT_FAILURE);  
    }  
}  

// ���в������ 
int run_test(void)
{  
    if (CU_initialize_registry())
    {  
        fprintf(stderr, "Initialize Test Registry failed.");  
        exit(EXIT_FAILURE);  
    }

    add_test_suites();  
    
    /// ����ģʽ1: Automated Mode���Զ�ģʽ�������ɱ�����������鿴���Խ��
    //CU_set_output_filename("test_suite"); 
    //CU_list_tests_to_file(); 
    //CU_automated_run_tests(); 

    // ����ģʽ2: Basice Mode������ģʽ��ֱ�ӿ����Խ��
    CU_basic_set_mode(CU_BRM_VERBOSE); 
    CU_basic_run_tests(); 
    

    // ����ģʽ3: Console Mode������ģʽ���ڽ������������в��Ի�鿴���Խ��
    //CU_console_run_tests(); 


    CU_cleanup_registry();  

    return CU_get_error();  

}  

int main(int argc, char *argv[])
{
    run_test();
    
    return 0;
}

