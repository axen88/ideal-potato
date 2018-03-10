/*
* 使用gcc -Wall -g -o fork fork.c进行编译链接
*/

#include <stdio.h>
#include <unistd.h>

int ForkProcesses(int v_cnt)
{
	#define FORK_STEPS 1
	int pid = 1;
	
	printf("Process start. [cnt: %d, getpid(): %d]\n", v_cnt, getpid());
	
	while (v_cnt > FORK_STEPS)
	{
		printf("Start fork. [cnt: %d, getpid(): %d]\n", v_cnt, getpid());
		
		pid = fork();
		if (pid < 0)
		{
			printf("error. [fork(): %d, getpid(): %d]\n", pid, getpid());
		}
		else if (pid == 0)
		{
			printf("child. [fork(): %d, getpid(): %d]\n", pid, getpid());
			break;
		}
		else
		{
			printf("parent. [fork(): %d, getpid(): %d]\n", pid, getpid());
			v_cnt -= FORK_STEPS;
		}
	}
		
	if (pid > 0)
	{
		sleep(2);
	}
	else if (pid == 0)
	{
		sleep(1);
	}
	
	printf("Process finished. [cnt: %d, getpid(): %d]\n", v_cnt, getpid());
	
	return pid;
}

#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("usage: %s fork_cnt\n", argv[0]);
		return -1;
	}
	
	return ForkProcesses(atoi(argv[1]));
}
