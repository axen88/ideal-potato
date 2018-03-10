
#include <stdio.h>

#include "../os_adapter.h"
#include "../queue.h"

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = QueueCreate(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (QueuePush(p, (void *)msg) < 0)
        {
            printf("----------Queue is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, QueueGetSize(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (QueuePop(p, &pMsg) < 0)
        {
            printf("----------Queue is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, QueueGetSize(p));
    }

    QueueDestroy(p);
    system("pause");

    return 0;
}


