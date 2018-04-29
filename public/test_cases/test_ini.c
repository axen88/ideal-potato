
#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  

#include "../os_adapter.h"
#include "../ini.h"

void test_ini_case0(void)
{
#define TEST_BUF_SIZE 256
#define TEST_INI_FILE "test_ini.ini"
    
    char value[TEST_BUF_SIZE]={0};

    // section student
    CU_ASSERT(IniWriteString(TEST_INI_FILE, "student", "name", "Tony") == 0);
    CU_ASSERT(IniWriteString(TEST_INI_FILE, "student", "age", "20") == 0);

    CU_ASSERT(IniReadString(TEST_INI_FILE, "student", "name", value, TEST_BUF_SIZE, NULL) == 0);
    CU_ASSERT(strcmp(value, "Tony") == 0);

    CU_ASSERT(IniReadInt(TEST_INI_FILE, "student", "age", 0) == 20);

    // section sucess inc
    CU_ASSERT(IniWriteString(TEST_INI_FILE, "success inc", "test name", "what's your name") == 0);
    CU_ASSERT(IniWriteString(TEST_INI_FILE, "success inc", "test num", "123456") == 0);

    CU_ASSERT(IniReadString(TEST_INI_FILE, "success inc", "test name", value, TEST_BUF_SIZE, NULL) == 0);
    CU_ASSERT(strcmp(value, "what's your name") == 0);

    CU_ASSERT(IniReadInt(TEST_INI_FILE, "success inc", "test num", 0) == 123456);
}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_ini_cases[]
= {  
    {to_str(test_ini_case0), test_ini_case0},  
    CU_TEST_INFO_NULL  
};  


