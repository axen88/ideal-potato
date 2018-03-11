
#include <stdio.h>

#include "../os_adapter.h"
#include "../stack.h"

int main(int argc, char *argv[])
{
    void *p = NULL;
    long msg = 0;
    void *pMsg = NULL;

    p = stack_create(10);
    if (NULL == p)
    {
        printf("Create queue failed.\n");
        return -1;
    }

    msg = 90;
    while (msg--)
    {
        if (stack_push(p, (void *)msg) < 0)
        {
            printf("----------Stack is full now-----------\n");
            break;
        }

        printf("Push ok. [msg: %ld, size: %d]\n", msg, stack_get_size(p));
    }
    
    msg = 60;
    while (msg--)
    {
        if (stack_pop(p, &pMsg) < 0)
        {
            printf("----------Stack is empty now-----------\n");
            break;
        }

        printf("Pop ok. [msg: %ld, size: %d]\n", (long)pMsg, stack_get_size(p));
    }

    stack_destroy(p);
    system("pause");

    return 0;
}


