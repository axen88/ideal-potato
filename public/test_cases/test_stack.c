
#include <stdio.h>

#include "../os_adapter.h"
#include "../stack.h"

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = StackCreate(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (StackPush(p, (void *)msg) < 0)
        {
            printf("----------Stack is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, StackGetSize(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (StackPop(p, &pMsg) < 0)
        {
            printf("----------Stack is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, StackGetSize(p));
    }

    StackDestroy(p);
    system("pause");

    return 0;
}


