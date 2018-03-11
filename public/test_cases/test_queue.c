
#include <stdio.h>

#include "../os_adapter.h"
#include "../queue.h"

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = queue_create(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (queue_push(p, (void *)msg) < 0)
        {
            printf("----------Queue is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, queue_get_size(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (queue_pop(p, &pMsg) < 0)
        {
            printf("----------Queue is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, queue_get_size(p));
    }

    queue_destroy(p);
    system("pause");

    return 0;
}


