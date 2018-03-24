/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*******************************************************************************

            Copyright(C), 2016~2019, axen.hook@foxmail.com
********************************************************************************
File Name: OS_DLIST.H
Author   : axen.hook
Version  : 1.00
Date     : 02/Mar/2016
Description: 
Function List: 
    1. ...: 
History: 
    Version: 1.00  Author: axen.hook  Date: 02/Mar/2016
--------------------------------------------------------------------------------
    1. Primary version
*******************************************************************************/

#ifndef __OS_DLIST_H__
#define __OS_DLIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct list_head
{
    struct list_head *next;
    struct list_head *prev;
} list_head_t;

#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member))) 

static inline void list_init_head(list_head_t *node)
{
    ASSERT(node);

    node->next = node;
    node->prev = node;
}

static void add_node(list_head_t *node, list_head_t *prev, list_head_t *next)
{
    ASSERT(node);
    ASSERT(prev);
    ASSERT(next);

    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

static void remove_node(list_head_t *node, list_head_t *prev, list_head_t *next)
{
    ASSERT(prev);
    ASSERT(next);

    next->prev = prev;
    prev->next = next;
    list_init_head(node);
}

static inline void list_add_head(list_head_t *head, list_head_t *node)
{
    ASSERT(head);
    ASSERT(node);

    add_node(node, head, head->next);
}

static inline void list_add_tail(list_head_t *head, list_head_t *node)
{
    ASSERT(head);
    ASSERT(node);

	add_node(node, head->prev, head);
}

static inline void list_remove_node(list_head_t *node)
{
    ASSERT(node);

    remove_node(node, node->prev, node->next);
}

static inline list_head_t *list_get_node(list_head_t *head, uint32_t pos)
{
    uint32_t cnt = 0;
    list_head_t *next = NULL;

    ASSERT(head);

    next = head->next;
    ASSERT(next);

    while (next != head)
    {
        if (pos == cnt)
        {
            return next;
        }

        next = next->next;
        ASSERT(next);

        cnt++;
    }

    return NULL;
}

static inline int32_t list_remove_target_node(list_head_t *head, uint32_t position)
{
    list_head_t *node = NULL;

    ASSERT(head);

    node = list_get_node(head, position);

    if (node)
    {
        list_remove_node(node);
        return 0;
    }

    return -1;
}

typedef int32_t (*list_cb)(void *, list_head_t *);

static inline int32_t list_walk_all(list_head_t *head, list_cb cb, void *para)
{
    int32_t ret = 0;
    list_head_t *next = NULL;
    list_head_t *next_next = NULL;

    ASSERT(cb);
    ASSERT(head);

    next = head->next;
    ASSERT(next);

    next_next = next->next;
    ASSERT(next_next);

    while (next != head)
    {
        ret = cb(para, next);
        if (ret != 0)
        {
            return ret;
        }

        next = next_next;

        next_next = next->next;
        ASSERT(next_next);
    }

    return 0;
}

static inline bool_t list_is_empty(list_head_t *head)
{
    ASSERT(head);

    return (head->next == head) ? TRUE : FALSE;
}


#ifdef __cplusplus
}
#endif

#endif
