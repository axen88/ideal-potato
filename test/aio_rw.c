/*
* 使用gcc -Wall -g -o aio_rw aio_rw.c -lrt进行编译链接
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

int AioWaitFinish(struct aiocb *v_pCb, long v_timeOut)
{
    int ret = 0;
    long timeStart = time(NULL);

    while (1)
    {
        ret = aio_error(v_pCb);
        if (EINPROGRESS != ret)
        {
            break;
        }

        if ((time(NULL) - timeStart) > v_timeOut)
        {
            return -ERR_AIO_WAIT_TIMEOUT;
        }

        sleep(1);
    }

    return 0;
}

int AioRead(struct aiocb *v_pCb)
{
    int ret = aio_read(v_pCb);
    if (0 > ret)
    {
        return -ERR_AIO_READ;
    }

    ret = AioWaitFinish(v_pCb, 100);
    if (0 > ret)
    {
        return ret;
    }

    return aio_return(v_pCb);
}

int AioWrite(struct aiocb *v_pCb)
{
    int ret = aio_write(v_pCb);
    if (0 > ret)
    {
        return -ERR_AIO_WRITE;
    }

    ret = AioWaitFinish(v_pCb, 100);
    if (0 > ret)
    {
        return ret;
    }

    return aio_return(v_pCb);
}

#define BUF_SIZE 4096

int AioCopyFile(const char *v_src, const char *v_dst)
{
    int fdSrc = 0;
    int fdDst = 0;
    struct aiocb cbSrc;
    struct aiocb cbDst;
    int ret = 0;
    void *pBuf = NULL;

    pBuf = malloc(BUF_SIZE);
    if (NULL == pBuf)
    {
        printf("malloc failed.\n");
        return -ERR_AIO_MALLOC;
    }

    fdSrc = open(v_src, O_RDONLY);
    if (0 > fdSrc)
    {
        free(pBuf);
        printf("open src file failed. [name: %s, fd: %d]\n", v_src, fdSrc);
        return -ERR_AIO_OPEN;
    }
    
    fdDst = open(v_dst, O_WRONLY | O_CREAT);
    if (0 > fdDst)
    {
        close(fdSrc);
        free(pBuf);
        printf("open dst file failed. [name: %s, fd: %d]\n", v_dst, fdDst);
        return -ERR_AIO_OPEN;
    }

    memset(&cbSrc, 0, sizeof(struct aiocb));
    memset(&cbDst, 0, sizeof(struct aiocb));

    cbSrc.aio_fildes = fdSrc;
    cbSrc.aio_buf = pBuf;
    cbSrc.aio_nbytes = BUF_SIZE;
    cbSrc.aio_offset = 0;
    
    cbDst.aio_fildes = fdDst;
    cbDst.aio_buf = pBuf;
    cbDst.aio_nbytes = BUF_SIZE;
    cbDst.aio_offset = 0;

    while (1)
    {
        ret = AioRead(&cbSrc);
        if (0 > ret)
        {
            printf("AioRead failed. [name: %s, fd: %d]\n", v_src, fdSrc);
            break;
        }
        else if (0 == ret)
        {
            break;
        }

        cbDst.aio_nbytes = ret;

        ret = AioWrite(&cbDst);
        if (cbDst.aio_nbytes != ret)
        {
            printf("AioWrite failed. [name: %s, fd: %d]\n", v_dst, fdDst);
            break;
        }

        cbSrc.aio_offset += ret;
        cbDst.aio_offset += ret;
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

    free(pBuf);

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

