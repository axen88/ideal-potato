/*
* 使用gcc -o avp_list_single avp_list_single.c -Wall -g -I./ -D_EN_DEBUG_进行编译链接
*/

#include "avp_list_single.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
* SListInit - Initialize list head
*/
void
SListInit(
    SLIST_HEAD_S *v_pEntry
    )
{
    v_pEntry->pNext = v_pEntry;
}

/**
* AddEntry - Insert a new entry between two known consecutive entries.
*
* This is only for internal list manipulation where we know the prev/next
* entries already!
*/
static
void
AddEntry(
    SLIST_HEAD_S *v_pEntry, // The entry want to add
    SLIST_HEAD_S *v_pPrev,
    SLIST_HEAD_S *v_pNext
    )
{
    v_pEntry->pNext = v_pNext;
    v_pPrev->pNext = v_pEntry;
}

/**
* RemoveEntry -delete entry
*
* Delete a list entry by making the prev/next entries point to each other.
*
* This is only for internal list manipulation where we know the prev/next
* entries already!
*/
static
void
RemoveEntry(
    SLIST_HEAD_S *v_pPrev,
    SLIST_HEAD_S *v_pEntry
    )
{
    v_pPrev->pNext = v_pEntry->pNext;
}

/**
* SListAdd - add a new entry
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
void
SListAddHead(
    SLIST_HEAD_S *v_pHead,  // SList head to add it after
    SLIST_HEAD_S *v_pEntry // Entry to be added
    )
{
    AddEntry(v_pEntry, v_pHead, v_pHead->pNext);
}

/**
* SListAddTail - add a new entry
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
void
SListAddTail(
    SLIST_HEAD_S *v_pHead,  // SList head to add it before
    SLIST_HEAD_S *v_pEntry // Entry to be added
    )
{
    SLIST_HEAD_S *pPrev = v_pHead;
    SLIST_HEAD_S *pEntry = v_pHead->pNext;

    while (pEntry != v_pHead)
    {
        pPrev = pEntry;
        pEntry = pEntry->pNext;
    }
    
    AddEntry(v_pEntry, pPrev, v_pHead);
}

SLIST_HEAD_S *
SListGetEntry(
    SLIST_HEAD_S *v_pHead, // The list to test
    int position
    )
{
    int count = 0;
    SLIST_HEAD_S *pNext = v_pHead->pNext;

    while (pNext != v_pHead)
    {
        if (position == count)
        {
            return pNext;
        }
        
        pNext = pNext->pNext;
        count++;
   }

    return NULL;
}

int SListRemoveTargetEntry(SLIST_HEAD_S *v_pHead, int v_cnt)
{
     int count = 0;
     SLIST_HEAD_S *pPrev = v_pHead;
     SLIST_HEAD_S *pEntry = v_pHead->pNext;
    
     while (pEntry != v_pHead)
     {
         if (v_cnt == count)
         {
             RemoveEntry(pPrev, pEntry);
             return 0;
         }
         
         pPrev = pEntry;
         pEntry = pEntry->pNext;
         count++;
    }

    return -1;
}

int
SListWalkAll(
    SLIST_HEAD_S *v_pHead, // The list to test
    int (v_pFunc)(void *)
    )
{
    int ret = 0;
    SLIST_HEAD_S *pNext = v_pHead->pNext;

    while (pNext != v_pHead)
    {
        ret = v_pFunc(pNext);
        if (0 > ret)
        {
            return ret;
        }
        
        pNext = pNext->pNext;
    }

    return 0;
}

/**
* SListCount
*/
int
SListCount(
    SLIST_HEAD_S *v_pHead // The list to test
    )
{
    int count = 0;
    SLIST_HEAD_S *pNext = v_pHead->pNext;

    while (pNext != v_pHead)
    {
        pNext = pNext->pNext;
        count++;
    }

    return count;
}

/**
* SListEmpty - tests whether a list is empty
*/
int
SListIsEmpty(
    SLIST_HEAD_S *v_pHead // The list to test
    )
{
    return (v_pHead->pNext == v_pHead) ? 1 : 0;
}

#if 1

#include <stdio.h>

typedef struct tagLIST_DATA_S
{
    SLIST_HEAD_S entry;
    int dat;    
} LIST_DATA_S; /* End of tagLIST_DATA_S */

SLIST_HEAD_S g_head;

int CallBack(void *v_pPara)
{
    printf("%d ", ((LIST_DATA_S *)v_pPara)->dat);
    return 0;
}

void PrintListData(SLIST_HEAD_S *v_pHead)
{
    SListWalkAll(v_pHead, CallBack);
    printf("\ncount: %d\n", SListCount(v_pHead));
}

void InitListData(LIST_DATA_S *v_pListd, int v_size)
{
    int i = 0;
    
    for (i = 0; i < v_size; i++)
    {
        v_pListd[i].dat = i;
    }
}


int main(int argc, char *argv[])
{
    LIST_DATA_S listd[10];

    InitListData(listd, 10);

    SListInit(&g_head);

    SListAddHead(&g_head, &listd[0].entry);
    SListAddHead(&g_head, &listd[1].entry);
    SListAddHead(&g_head, &listd[2].entry);
    SListAddHead(&g_head, &listd[3].entry);
    PrintListData(&g_head);

    SListAddTail(&g_head, &listd[4].entry);
    SListAddTail(&g_head, &listd[5].entry);
    SListAddTail(&g_head, &listd[6].entry);
    SListAddTail(&g_head, &listd[7].entry);
    PrintListData(&g_head);
    
    SListAddHead(&g_head, &listd[8].entry);
    SListAddTail(&g_head, &listd[9].entry);
    PrintListData(&g_head);

    SListRemoveTargetEntry(&g_head, 0);
    PrintListData(&g_head);
    
    SListRemoveTargetEntry(&g_head, 8);
    PrintListData(&g_head);
    
    SListRemoveTargetEntry(&g_head, 4);
    PrintListData(&g_head);
    
    SListRemoveTargetEntry(&g_head, 1);
    PrintListData(&g_head);
    
    return 0;
}


#endif

