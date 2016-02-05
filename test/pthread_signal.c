/*
* 使用gcc -Wall -g -o pthread_signal pthread_signal.c -lpthread进行编译链接
*/

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

int g_exitFlag = 0;

void StopTest(int v_para)
{
	g_exitFlag = 1;
}

void *ThreadTest(void *v_pPara)
{
	int cnt = 0;
	
	while (1)
	{
		if (g_exitFlag)
		{
			printf("The exit flag cached. [pthread_self(): %lu]\n", pthread_self());
			break;
		}
		
		printf("The thread is running. [para: %ld, cnt: %d, pthread_self(): %lu]\n",
			(unsigned long)v_pPara, cnt++, pthread_self());
			
		usleep(100 * 1000);
	}
	
	printf("The thread finished ok. [pthread_self(): %lu]\n", pthread_self());
	return NULL;
}

int main(int argc, char *argv[])
{
	#define THREADS_NUM 10
	
	pthread_t tids[THREADS_NUM];
	int tidsSize = 0;
	pthread_t tid;
	int i = 0;
	int ret = 0;
	
	g_exitFlag = 0;
	
	signal(SIGINT, StopTest);
	signal(SIGKILL, StopTest);
	signal(SIGTERM, StopTest);
	
	for (i = 0; i < THREADS_NUM; i++)
	{
		ret = pthread_create(&tid, NULL, ThreadTest, (void *)(long)i);
		if (ret == 0)
		{
			printf("Create thread success. [threadid: %lu, i: %d, pthread_self(): %lu]\n", tid, i, pthread_self());
			tids[tidsSize++] = tid;
		}
		else
		{
			printf("Create thread failed. [ret: %d, i: %d, pthread_self(): %lu]\n", ret, i, pthread_self());
			break;
		}
	}
	
	for (i = 0; i < tidsSize; i++)
	{
		printf("Waitting for thread finish. [threadid: %lu, i: %d, pthread_self(): %lu]\n", tids[i], i, pthread_self());
		ret = pthread_join(tids[i], NULL);
		printf("The thread finished. [threadid: %lu, i: %d, pthread_self(): %lu]\n", tids[i], i, pthread_self());
	}
	
	printf("All threads finished. [pthread_self(): %lu]\n", pthread_self());
	
	return 0;
}