
#include <stdio.h>

#include <semaphore.h>

#include "pthread.h"



void *test_thread(void *para)
{
	printf("thread run ok, now walk up the sem\n");
    sem_t *sem= (sem_t *)para;
    sem_post(sem);
}



int main(int argc, char *argv[])
{
    sem_t sem;
	pthread_t thread_id;
	int ret;

    sem_init(&sem, 0, 0);    

	ret = pthread_create(&thread_id, NULL, test_thread, &sem);
	if (ret)
	{
		printf("create thread failed\n");
		return -1;
	}
		
    printf("waiting for wake up\n");
	
    sem_wait(&sem);

    printf("wake up success\n");
	
	pthread_join(thread_id, NULL);
    
    return 0;
}


