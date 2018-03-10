

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
    * �д�����:
    *  1. EINVAL: v_base��[2, 36]��Χ֮��
    *  2. ERANGE: ת������������Խ��
    */
    if (errno != 0)
    {
        return -1;
    }

    /*
    * �����������:
    *  1. �׸��ַ�Ϊ�Ƿ��ַ�����QQ, Q8�ȣ�����0xQQ������
    *  2. �ַ���Ϊ��
    */
    if (pEnd == v_pStr)
    {
        return -2;
    }
    
    /*
    * �����������:
    *  1. �ַ����к��зǷ��ַ�����QQ, Q8, 0xQQ, 0xABQQ, 0xQQAB��
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
    * �д�����:
    *  1. EINVAL: v_base��[2, 36]��Χ֮��
    *  2. ERANGE: ת������������Խ��
    */
    if (errno != 0)
    {
        return -1;
    }

    /*
    * �����������:
    *  1. �׸��ַ�Ϊ�Ƿ��ַ�����QQ, Q8�ȣ�����0xQQ������
    *  2. �ַ���Ϊ��
    */
    if (pEnd == v_pStr)
    {
        return -2;
    }
    
    /*
    * �����������:
    *  1. �ַ����к��зǷ��ַ�����QQ, Q8, 0xQQ, 0xABQQ, 0xQQAB��
    */
    if (pEnd != (v_pStr + strlen(v_pStr)))
    {
        return -3;
    }

    return 0;
}

