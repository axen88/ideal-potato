/*
* 使用gcc -Wall -g -o shmmutex shmmutex.c -lpthread进行编译链接
*/

#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#define GetKey() ftok("/home/test", 256)

int ShmMutexGet(void **v_ppShmMutex)
{
    int iShmId = 0;

    key_t key = GetKey();
    if (key < 0)
    {
        printf("GetKey failed.\n");
        return -1;
    }
    

    iShmId = shmget(key, 0, 0);
    if (iShmId >= 0)
    {
    		printf("old iShmId: %d\n", iShmId);
        *v_ppShmMutex = shmat(iShmId, NULL, 0);
        if (*v_ppShmMutex == NULL)
        {
            printf("shmat failed.\n");
            return -2;
        }
        else
        {
            return 0;
        }
    }

    iShmId = shmget(key, sizeof(pthread_rwlock_t), IPC_CREAT);
    if (iShmId < 0)
    {
        printf("shmget failed.\n");
        return -3;
    }
    
    printf("new iShmId: %d\n", iShmId);

    *v_ppShmMutex = shmat(iShmId, NULL, 0);
    if (*v_ppShmMutex == NULL)
    {
        printf("shmat failed.\n");
        shmctl(iShmId, IPC_RMID, 0);
        return -4;
    }

    if (pthread_rwlock_init(*v_ppShmMutex, NULL) != 0)
    {
        printf("pthread_rwlock_init failed.\n");
        shmdt(*v_ppShmMutex);
        shmctl(iShmId, IPC_RMID, 0);
        return -5;
    }

    return 0;
}

int ShmMutexPut(void *v_pShmMutex)
{
    return shmdt(v_pShmMutex);
}

int ShmMutexDestroy(void)
{
    void *pShmMutex = NULL;
    int iShmId = 0;

    key_t key = GetKey();
    if (key < 0)
    {
        printf("GetKey failed.\n");
        return -1;
    }

    iShmId = shmget(key, 0, 0);
    if (iShmId < 0)
    {
        printf("shmget failed.\n");
        return -2;
    }
    
    pShmMutex = shmat(iShmId, NULL, 0);
    if (pShmMutex == NULL)
    {
        printf("shmat failed.\n");
        shmctl(iShmId, IPC_RMID, 0);
        return -2;
    }

    pthread_rwlock_destroy(pShmMutex);
    shmdt(pShmMutex);
    
    return (shmctl(iShmId, IPC_RMID, 0));
}

int ShmMutexReadLock(void *v_pShmMutex)
{
    return (pthread_rwlock_rdlock(v_pShmMutex));
}

int ShmMutexWriteLock(void *v_pShmMutex)
{
    return (pthread_rwlock_wrlock(v_pShmMutex));
}

int ShmMutexUnlock(void *v_pShmMutex)
{
    return (pthread_rwlock_unlock(v_pShmMutex));
}

#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int iRet = 0;
    void *pShmMutex = NULL;
    
    if (argc < 2)
    {
        printf("Usage: %s R|W|X\n", argv[0]);
        return -1;
    }

    iRet = ShmMutexGet(&pShmMutex);
    if (iRet < 0)
    {
        printf("ShmMutexGet failed. [iRet: %d]\n", iRet);
        return -2;
    }

    printf("pShmMutex: %p\n", pShmMutex);
    
    if (strcasecmp(argv[1], "R") == 0)
    {
        printf("Waitting for R lock now...\n");
        ShmMutexReadLock(pShmMutex);
        printf("Get R lock ok\nDo someting now...\n");
        sleep(15);
        printf("Unlock now...\n");
        ShmMutexUnlock(pShmMutex);
        ShmMutexPut(pShmMutex);
        return 0;
    }
    else if (strcasecmp(argv[1], "W") == 0)
    {
        printf("Waitting for W lock now...\n");
        ShmMutexWriteLock(pShmMutex);
        printf("Get W lock ok\nDo someting now...\n");
        sleep(15);
        printf("Unlock now...\n");
        ShmMutexUnlock(pShmMutex);
        ShmMutexPut(pShmMutex);
        return 0;
    }

    ShmMutexPut(pShmMutex);
    ShmMutexDestroy();

    return 0;
}


