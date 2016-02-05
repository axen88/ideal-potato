	
#ifndef _DLIST_H_
#define _DLIST_H_

typedef struct tagDLIST_HEAD_S
{
	struct tagDLIST_HEAD_S *pNext;
	struct tagDLIST_HEAD_S *pPrev;
} DLIST_HEAD_S;

/**
* DListInit - Initialize list head
*/
extern
void
DListInit(
	DLIST_HEAD_S *v_pEntry
	);

/**
* DListAdd - add a new entry
*
* Insert a new entry after the specified head.
* This is good for implementing stacks.
*/
extern
void
DListAddHead(
	DLIST_HEAD_S *v_pHead,  // DList head to add it after
	DLIST_HEAD_S *v_pEntry // Entry to be added
	);

/**
* DListAddTail - add a new entry
*
* Insert a new entry before the specified head.
* This is useful for implementing queues.
*/
extern
void
DListAddTail(
	DLIST_HEAD_S *v_pHead,  // DList head to add it before
	DLIST_HEAD_S *v_pEntry // Entry to be added
	);

extern
DLIST_HEAD_S *
DListGetEntry(
    DLIST_HEAD_S *v_pHead, // The list to test
    int position
    );

/**
* DListDelete - deletes entry from list.
*/
extern
void
DListRemoveEntry(
	DLIST_HEAD_S *v_pEntry // The element to delete from the list.
	);
    
extern int DListRemoveTargetEntry(DLIST_HEAD_S *v_pHead, int v_cnt);

extern
int
DListWalkAll(
    DLIST_HEAD_S *v_pHead, // The list to test
    int (v_pFunc)(void *)
    );

/**
* DListEmpty - tests whether a list is empty
*/
extern
int
DListIsEmpty(
	DLIST_HEAD_S *v_pHead // The list to test
	);

extern
int
DListCount(
	DLIST_HEAD_S *v_pHead // The list to test
	);

#endif // End of _DLIST_H_ 


