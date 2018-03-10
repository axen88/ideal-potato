/*
* 使用gcc -Wall -g -o aio_callback aio_callback.c -lrt进行编译链接
*/

#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <aio.h>
#include <time.h>
#include <pthread.h>

typedef enum tagAIO_ERROR_CODE_E
{
    ERR_AIO_MALLOC = 100,
    ERR_AIO_INVALID_PARA,
    ERR_AIO_CANCEL_TIMEOUT,
    ERR_AIO_WAIT_TIMEOUT,
    ERR_AIO_READ,
    ERR_AIO_WRITE,
    ERR_AIO_ERROR,
    ERR_AIO_RETURN,
    ERR_AIO_OPEN,
    ERR_FILE_STAT,
} AIO_ERROR_CODE_E; /* End of tagAIO_ERROR_CODE_E */

int AioCancel(int v_fd, long v_timeOut)
{
    int ret = 0;
    long timeStart = time(NULL);

    while (1)
    {
        ret = aio_cancel(v_fd, NULL);
        if ((AIO_ALLDONE == ret) || (AIO_CANCELED == ret))
        {
            break;
        }

        if ((time(NULL) - timeStart) > v_timeOut)
        {
            return -ERR_AIO_CANCEL_TIMEOUT;
        }

        sleep(1);
    }

    return 0;
}

typedef struct tagAIO_PARA_S
{
    int fdWr;
    struct aiocb cb;
} AIO_PARA_S; /* End of tagAIO_PARA_S */

pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
int g_total = 0;
int g_success = 0;
int g_failed = 0;

void FreeResource(AIO_PARA_S *v_pAio, int v_failed)
{
    free((void *)v_pAio->cb.aio_buf);
    free(v_pAio);

    pthread_mutex_lock(&g_mtx);
    if (v_failed)
    {
        g_failed++;
    }
    else
    {
        g_success++;
    }
    
    pthread_mutex_unlock(&g_mtx);

    return;
}

void AioWriteCompleteHandle(sigval_t v_sigval)
{
    AIO_PARA_S *pAio = (AIO_PARA_S *)v_sigval.sival_ptr;
    struct aiocb *pCb = &pAio->cb;
    int ret = 0;

    ret = aio_error(pCb);
    if (0 != ret)
    {
        printf("aio_error failed. [ret: %d]\n", ret);
        FreeResource(pAio, 1);
        return;
    }
    
    ret = aio_return(pCb);
    if (pCb->aio_nbytes != ret)
    {
        printf("aio_return failed. [ret: %d, aio_nbytes: %d]\n",
            ret, pCb->aio_nbytes);
        FreeResource(pAio, 1);
        return;
    }

    FreeResource(pAio, 0);

    return;
}

void AioReadCompleteHandle(sigval_t v_sigval)
{
    AIO_PARA_S *pAio = (AIO_PARA_S *)v_sigval.sival_ptr;
    struct aiocb *pCb = &pAio->cb;
    int ret = 0;

    ret = aio_error(pCb);
    if (0 != ret)
    {
        printf("aio_error failed. [ret: %d]\n", ret);
        FreeResource(pAio, 1);
        return;
    }
    
    ret = aio_return(pCb);
    if (0 > ret)
    {
        printf("aio_return failed. [ret: %d]\n", ret);
        FreeResource(pAio, 1);
        return;
    }

    pCb->aio_nbytes = ret;
    pCb->aio_fildes = pAio->fdWr;
    pCb->aio_sigevent.sigev_notify_function = AioWriteCompleteHandle;
    
    ret = aio_write(pCb);
    if (0 > ret)
    {
        printf("aio_write failed. [ret: %d]\n", ret);
        FreeResource(pAio, 1);
    }

    return;
}

int AioSendReadCmd(int v_fdRd, long long v_offset, int v_len, int v_fdWr)
{
    int ret = 0;
    AIO_PARA_S *pAio = NULL;
    struct aiocb *pCb = NULL;
    char *pBuf = malloc(v_len);
    if (NULL == pBuf)
    {
        printf("malloc failed.\n");
        return -ERR_AIO_MALLOC;
    }

    pAio = malloc(sizeof(AIO_PARA_S));
    if (NULL == pBuf)
    {
        free(pBuf);
        printf("malloc failed.\n");
        return -ERR_AIO_MALLOC;
    }

    memset(pAio, 0, sizeof(AIO_PARA_S));
    pAio->fdWr = v_fdWr;
    
    pCb = &pAio->cb;

    pCb->aio_fildes = v_fdRd;
    pCb->aio_buf = pBuf;
    pCb->aio_nbytes = v_len;
    pCb->aio_offset = v_offset;
    pCb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    pCb->aio_sigevent.sigev_notify_function = AioReadCompleteHandle;
    pCb->aio_sigevent.sigev_notify_attributes = NULL;
    pCb->aio_sigevent.sigev_value.sival_ptr = pAio;

    ret = aio_read(pCb);
    if (0 > ret)
    {
        printf("aio_read failed. [ret: %d]\n", ret);
        return -ERR_AIO_READ;
    }

    pthread_mutex_lock(&g_mtx);
    g_total++;
    pthread_mutex_unlock(&g_mtx);

    return 0;
}

int GetFileSize(const char *v_name, long long *v_size)
{
    struct stat buf;

    if (stat(v_name, &buf) < 0)
    {
        return -ERR_FILE_STAT;
    }

    *v_size = buf.st_size;

    return 0;
}

#define BUF_SIZE 4096

int AioCopyFile(const char *v_src, const char *v_dst)
{
    int fdSrc = 0;
    int fdDst = 0;
    int ret = 0;
    long long size = 0;
    long long blocks = 0;
    long long offset = 0;

    ret = GetFileSize(v_src, &size);
    if (0 > ret)
    {
        printf("get file size failed. [name: %s]\n", v_src);
        return ret;
    }

    blocks = (size + BUF_SIZE - 1) / BUF_SIZE;

    fdSrc = open(v_src, O_RDONLY);
    if (0 > fdSrc)
    {
        printf("open src file failed. [name: %s, fd: %d]\n", v_src, fdSrc);
        return -ERR_AIO_OPEN;
    }
    
    fdDst = open(v_dst, O_WRONLY | O_CREAT);
    if (0 > fdDst)
    {
        close(fdSrc);
        printf("open dst file failed. [name: %s, fd: %d]\n", v_dst, fdDst);
        return -ERR_AIO_OPEN;
    }

    while (blocks--)
    {
        ret = AioSendReadCmd(fdSrc, offset, BUF_SIZE, fdDst);
        if (0 > ret)
        {
            printf("AioSendReadCmd failed. [name: %s, fd: %d]\n", v_src, fdSrc);
            break;
        }

        offset += BUF_SIZE;
    }

    while (g_total != (g_success + g_failed))
    {
        printf("g_total: %d, g_success: %d, g_failed: %d\n",
            g_total, g_success, g_failed);
        sleep(1);
    }

    ret = AioCancel(fdSrc, 100);
    if (0 > ret)
    {
        printf("AioCancel failed. [name: %s, fd: %d]\n", v_src, fdSrc);
    }
    
    ret = AioCancel(fdDst, 100);
    if (0 > ret)
    {
        printf("AioCancel failed. [name: %s, fd: %d]\n", v_dst, fdDst);
    }

    close(fdSrc);
    close(fdDst);

    return ret;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
        printf("Usage: %s src dst\n", argv[0]);
        return -1;
    }

    return AioCopyFile(argv[1], argv[2]);
}

