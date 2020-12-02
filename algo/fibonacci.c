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

unsigned long long get_cycle_count(void)
{
    unsigned long long t = 0;
    
    rdtscll(t);
    
    return t;
}

unsigned long long get_us_count(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (((unsigned long long)tv.tv_sec * 1000 * 1000) + (tv.tv_usec));
}

unsigned long long get_ms_count(void)
{
    struct timeval tv;
    
    gettimeofday(&tv, NULL);
    
    return (((unsigned long long)tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

unsigned long long get_second_count(void)
{
    return (time(NULL));
}

// 用递归方式解决斐波拉契数列
unsigned long long fib_recursive(unsigned long long n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;
	if (n == 2 )
		return 2;
	
	return fib_recursive(n - 1) + fib_recursive(n - 2);
}

// 用动态规划方法解决斐波拉契数列
unsigned long long fib_dp(unsigned long long n)
{
	
}

#include <unistd.h>

void test_fib(unsigned long long n)
{
	unsigned long long us;
	unsigned long long result;
	
	us = get_us_count();
	result = fib_recursive(n);
	us = get_us_count() - us;
	printf("fib_recursive(%ull), result(%ull), time(%lld us)", n, result, us);
	
	us = get_us_count();
	result = fib_dp(n);
	us = get_us_count() - us;
	printf("fib_dp(%ull), result(%ull), time(%lld us)", n, result, us);
	
	
}



int main(int argc, char **argv)
{
	if (argc < 2)
		return -1;
	
	test_fib(atoi(argv[1]));
    
    return 0;
        
}


