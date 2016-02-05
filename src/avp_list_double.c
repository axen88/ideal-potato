/*
* 使用gcc -o avp_list_double avp_list_double.c -Wall -g -I./ -D_EN_DEBUG_进行编译链接
*/

#include "avp_list_double.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/**
* DListInit - Initialize list head
*/
void
DListInit(
    DLIST_HEAD_S *v_pEntry
    )
{
    v_pEntry->pNext = v_pEntry;
    v_pEntry->pPrev = v_pEntry;
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
    DLIST_HEAD_S *v_pEntry, // The entry want to add
    DLIST_HEAD_S *v_pPrev,
    DLIST_HEAD_S *v_pNext
    )
{
    v_pNext->pPrev = v_pEntry;
    v_pEntry->pNext = v_pNext;
    v_pEntry->pPrev = v_pPrev;
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
    DLIST_HEAD_S *v_pPrev,
    DLIST_HEAD_S *v_pNext
    )
{
    v_pNext->pPrev = v_pPrev;
    v_pPrev->pNext = v_pNext;
}

/**
* DListAdd - add a new entry
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
void
DListAddHead(
    DLIST_HEAD_S *v_pHead,  // DList head to add it after
    DLIST_HEAD_S *v_pEntry // Entry to be added
    )
{
    AddEntry(v_pEntry, v_pHead, v_pHead->pNext);
}

/**
* DListAddTail - add a new entry
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
void
DListAddTail(
    DLIST_HEAD_S *v_pHead,  // DList head to add it before
    DLIST_HEAD_S *v_pEntry // Entry to be added
    )
{
    AddEntry(v_pEntry, v_pHead->pPrev, v_pHead);
}

/**
* DListDelete - deletes entry from list.
*/
void
DListRemoveEntry(
    DLIST_HEAD_S *v_pEntry // The element to delete from the list.
    )
{
    RemoveEntry(v_pEntry->pPrev, v_pEntry->pNext);
}

DLIST_HEAD_S *
DListGetEntry(
    DLIST_HEAD_S *v_pHead, // The list to test
    int position
    )
{
    int count = 0;
    DLIST_HEAD_S *pNext = v_pHead->pNext;

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

int DListRemoveTargetEntry(DLIST_HEAD_S *v_pHead, int v_cnt)
{
    DLIST_HEAD_S *pEntry = DListGetEntry(v_pHead, v_cnt);

    if (NULL != pEntry)
    {
        DListRemoveEntry(pEntry);
        return 0;
    }

    return -1;
}

int
DListWalkAll(
    DLIST_HEAD_S *v_pHead, // The list to test
    int (v_pFunc)(void *)
    )
{
    int ret = 0;
    DLIST_HEAD_S *pNext = v_pHead->pNext;

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
* DListCount
*/
int
DListCount(
    DLIST_HEAD_S *v_pHead // The list to test
    )
{
    int count = 0;
    DLIST_HEAD_S *pNext = v_pHead->pNext;

    while (pNext != v_pHead)
    {
        pNext = pNext->pNext;
        count++;
    }

    return count;
}

/**
* DListEmpty - tests whether a list is empty
*/
int
DListIsEmpty(
    DLIST_HEAD_S *v_pHead // The list to test
    )
{
    return (v_pHead->pNext == v_pHead) ? 1 : 0;
}

#ifdef _EN_DEBUG_

#include <stdio.h>

typedef struct tagLIST_DATA_S
{
    DLIST_HEAD_S entry;
    int dat;    
} LIST_DATA_S; /* End of tagLIST_DATA_S */

DLIST_HEAD_S g_head;

int CallBack(void *v_pPara)
{
    printf("%d ", ((LIST_DATA_S *)v_pPara)->dat);
    return 0;
}

void PrintListData(DLIST_HEAD_S *v_pHead)
{
    DListWalkAll(v_pHead, CallBack);
    printf("\ncount: %d\n", DListCount(v_pHead));
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

    DListInit(&g_head);

    DListAddHead(&g_head, &listd[0].entry);
    DListAddHead(&g_head, &listd[1].entry);
    DListAddHead(&g_head, &listd[2].entry);
    DListAddHead(&g_head, &listd[3].entry);
    PrintListData(&g_head);

    DListAddTail(&g_head, &listd[4].entry);
    DListAddTail(&g_head, &listd[5].entry);
    DListAddTail(&g_head, &listd[6].entry);
    DListAddTail(&g_head, &listd[7].entry);
    PrintListData(&g_head);
    
    DListAddHead(&g_head, &listd[8].entry);
    DListAddTail(&g_head, &listd[9].entry);
    PrintListData(&g_head);

    DListRemoveTargetEntry(&g_head, 0);
    PrintListData(&g_head);
    
    DListRemoveTargetEntry(&g_head, 8);
    PrintListData(&g_head);
    
    DListRemoveTargetEntry(&g_head, 4);
    PrintListData(&g_head);
    
    DListRemoveTargetEntry(&g_head, 1);
    PrintListData(&g_head);
    
    return 0;
}


#endif

