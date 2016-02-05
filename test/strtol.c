/*
* 使用gcc -Wall -g -o strtol strtol.c进行编译链接
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

#define rdtscll(val)  __asm__ __volatile__("rdtsc":"=A"(val))

unsigned long long GetCycleCount(void)
{
    unsigned long long t = 0;
    
    rdtscll(t);
    
    return t;
}

unsigned long long GetMsCount(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (((unsigned long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

unsigned long long GetSecCount(void)
{
    return (time(NULL));
}

int StrToLong(const char *v_pStr, long *v_pVal, int v_base)
{
    char *pEnd;

    *v_pVal = strtol(v_pStr, &pEnd, v_base);

    /*
    * 有错误发生:
    *  1. EINVAL: v_base在[2, 36]范围之外
    *  2. ERANGE: 转换出来的数据越界
    */
    if (errno != 0)
    {
        return -1;
    }

    /*
    * 处理以下情况:
    *  1. 首个字符为非法字符，如QQ, Q8等，但是0xQQ处理不了
    *  2. 字符串为空
    */
    if (pEnd == v_pStr)
    {
        return -2;
    }
    
    /*
    * 处理以下情况:
    *  1. 字符串中含有非法字符，如QQ, Q8, 0xQQ, 0xABQQ, 0xQQAB等
    */
    if (pEnd != (v_pStr + strlen(v_pStr)))
    {
        return -3;
    }

    return 0;
}

int StrToLongLong(const char *v_pStr, long long *v_pVal, int v_base)
{
    char *pEnd;

    *v_pVal = strtoll(v_pStr, &pEnd, v_base);

    /*
    * 有错误发生:
    *  1. EINVAL: v_base在[2, 36]范围之外
    *  2. ERANGE: 转换出来的数据越界
    */
    if (errno != 0)
    {
        return -1;
    }

    /*
    * 处理以下情况:
    *  1. 首个字符为非法字符，如QQ, Q8等，但是0xQQ处理不了
    *  2. 字符串为空
    */
    if (pEnd == v_pStr)
    {
        return -2;
    }
    
    /*
    * 处理以下情况:
    *  1. 字符串中含有非法字符，如QQ, Q8, 0xQQ, 0xABQQ, 0xQQAB等
    */
    if (pEnd != (v_pStr + strlen(v_pStr)))
    {
        return -3;
    }

    return 0;
}

#include <unistd.h>

int main(int argc, char **argv)
{
    long val;
    long long val2;
    int ret;
    
    if (argc < 2)
    {
        printf("Usage: %s str [base]\n", argv[0]);
        return -1;
    }
    
    printf("GetCycleCount: %lld, GetMsCount: %lld, GetSecCount: %lld\n", GetCycleCount(), GetMsCount(), GetSecCount());
    printf("GetCycleCount: %lld, GetMsCount: %lld, GetSecCount: %lld\n", GetCycleCount(), GetMsCount(), GetSecCount());

    ret = StrToLong(argv[1], &val, (argc < 3) ? 0 : atoi(argv[2]));
    if (ret < 0)
    {
        printf("StrToLong failed. [ret: %d]\n", ret);
    }
    else
    {
        printf("%ld\n", val);
    }

    ret = StrToLongLong(argv[1], &val2, (argc < 3) ? 0 : atoi(argv[2]));
    if (ret < 0)
    {
        printf("StrToLongLong failed. [ret: %d]\n", ret);
    }
    else
    {
        printf("%lld\n", val2);
    }
    
    usleep(1000 * 1000);

    printf("GetCycleCount: %lld, GetMsCount: %lld, GetSecCount: %lld\n", GetCycleCount(), GetMsCount(), GetSecCount());
    printf("GetCycleCount: %lld, GetMsCount: %lld, GetSecCount: %lld\n", GetCycleCount(), GetMsCount(), GetSecCount());
    
    return 0;
        
}


