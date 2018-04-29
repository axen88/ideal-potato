

#include <CUnit/CUnit.h>  
#include <CUnit/TestDB.h>  

#include "../os_adapter.h"
#include "../radix_tree.h"

int WalkCallBack(char *str)
{
    if (NULL != str)
    {
        printf("%s\n", str);
    }
    
    return 0;
}

int ReleaseCallBack(char *str)
{
    return 0;
}




void test_radix_tree_case0(void)
{
    RADIX_NODE *root = calloc(sizeof(RADIX_NODE), 1);
    
    RadixInsert(root, "1001");
    RadixInsert(root, "101");
    RadixInsert(root, "001");
    RadixInsert(root, "0001");
    RadixInsert(root, "11001");
    RadixInsert(root, "10011");
    RadixInsert(root, "011001");
    RadixInsert(root, "111001");
    RadixInsert(root, "1111001");
    RadixInsert(root, "abcdef");

    RadixWalkAll(root, WalkCallBack);
    RadixRelease(root, ReleaseCallBack);
        
}



// 将多个测试用例打包成组，以便指定给一个Suite 
CU_TestInfo test_radix_tree_cases[]
= {  
    {to_str(test_radix_tree_case0), test_radix_tree_case0},  
    CU_TEST_INFO_NULL  
};  


