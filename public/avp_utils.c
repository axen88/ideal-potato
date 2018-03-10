

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define rdtscll(val)  __asm__ __volatile__("rdtsc":"=A"(val))

unsigned long long GetCycleCount(void)
{
    unsigned long long t = 0;
    
#ifdef WIN32
#else
    rdtscll(t);
#endif
    
    return t;
}

unsigned long long GetMsCount(void)
{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (((unsigned long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
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

#ifdef WIN32
    *v_pVal = strtol(v_pStr, &pEnd, v_base);
#else
    *v_pVal = strtoll(v_pStr, &pEnd, v_base);
#endif

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

