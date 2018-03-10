
#ifdef _EN_DEBUG_

#include <stdio.h>

#define TEST_BUF_SIZE 256
#define TEST_INI_FILE "config\\sample.ini"

int main(int argc, char *argv[])
{
    const char szSection[] = "student";
    const char szKeyName[] = "name";
    const char szKeyAge[] = "age";
    char szValue[TEST_BUF_SIZE]={0};
    int age;

    if (0 != IniWriteString(TEST_INI_FILE, szSection, szKeyName, "Tony"))
    {
        printf("IniWriteString failed. [%s, %s, %s]\n",
            szSection, szKeyName, "Tony");
    }
    else
    {
        printf("IniWriteString success. [%s, %s, %s]\n",
            szSection, szKeyName, "Tony");
    }

    if (0 != IniWriteString(TEST_INI_FILE, szSection, szKeyAge, "20"))
    {
        printf("IniWriteString failed. [%s, %s, %s]\n",
            szSection, szKeyAge, "20");
    }
    else
    {
        printf("IniWriteString Success. [%s, %s, %s]\n",
            szSection, szKeyAge, "20");
    }

    if (0 != IniReadString(TEST_INI_FILE, szSection, szKeyName,
        szValue, TEST_BUF_SIZE, ""))
    {
        printf("IniReadString failed. [%s, %s]\n",
            szSection, szKeyName);
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            szSection, szKeyName, szValue);
    }

    age = IniReadInt(TEST_INI_FILE, szSection, szKeyAge, 0);
    printf("IniReadInt here. [%s, %s, %d]\n",
    szSection, szKeyAge, age);

    /* 用例 */
    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case", szValue);
    }

    /* 用例 */
    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case1",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case1", szValue);
    }

    /* 用例 */
    if (0 != IniWriteString(TEST_INI_FILE, "Main", "test case1",
        "TEST CASE1"))
    {
        printf("IniWriteString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniWriteString success. [%s, %s]\n",
            "Main", "test case1");
    }

    if (0 != IniReadString(TEST_INI_FILE, "Main", "test case1",
        szValue, TEST_BUF_SIZE, NULL))
    {
        printf("IniReadString failed. [%s, %s]\n",
            "Main", "test case1");
    }
    else
    {
        printf("IniReadString success. [%s, %s, %s]\n",
            "Main", "test case1", szValue);
    }


    return 0;
}

#endif

