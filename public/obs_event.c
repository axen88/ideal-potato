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

            Copyright(C), 2018~2021, axen.hook@foxmail.com
********************************************************************************
File Name: OBS_EVENT.C
Author   : axen.hook
Version  : 1.00
Date     : 15/Jun/2018
Description: 
Function List: 
    1. ...: 
History: 
    Version: 1.00  Author: axen.hook  Date: 15/Jun/2018
--------------------------------------------------------------------------------
    1. Primary version
*******************************************************************************/

#include "os_adapter.h"

#include "obs_event.h"


void destroy_obs_event(obs_event_t *event)
{
    if (event == NULL)
    {
        return;
    }
    
    uint32_t event_id;
    for (event_id = 0; event_id < event->max_event_id; event_id++)
    {
        obs_observer_t *cur = event->observers[event_id];
        if (cur == NULL)
        {
            continue;
        }

        do {
            obs_observer_t *next = cur->next;
            OS_FREE(cur);
            cur = next;
        } while (cur != NULL);
    }

    OS_FREE(event);
}

int create_obs_event(uint32_t max_event_id, obs_event_t **obs_event)
{
    obs_event_t *tmp_event = OS_MALLOC(sizeof(obs_event_t));
    if (tmp_event == NULL)
    {
        return -ERR_EVENT_NO_MEMORY;
    }

    memset(tmp_event, 0, sizeof(obs_event_t));
    tmp_event->max_event_id = max_event_id;
    
    obs_observer_t **observers = OS_MALLOC(max_event_id * sizeof(obs_observer_t *));
    if (observers == NULL)
    {
        OS_FREE(tmp_event);
        return -ERR_EVENT_NO_MEMORY;
    }

    memset(observers, 0, max_event_id * sizeof(obs_observer_t *));
    tmp_event->observers = observers;

    *obs_event = tmp_event;
    return 0;
}

int register_obs_event(obs_event_t *obs_event, uint32_t event_id, observer_func_t func)
{
    if (obs_event == NULL)
    {
        return -ERR_INVALID_OBS_EVENT;
    }
    
    if (event_id >= obs_event->max_event_id)
    {
        return -ERR_EVENT_ID;
    }

    obs_observer_t *new_obs = OS_MALLOC(sizeof(obs_observer_t));
    if (new_obs == NULL)
    {
        return -ERR_EVENT_NO_MEMORY;
    }

    new_obs->func = func;
    new_obs->next = NULL;

    obs_observer_t *cur = obs_event->observers[event_id];
    if (cur == NULL)
    {
        obs_event->observers[event_id] = new_obs;
        return 0;
    }
    
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    
    cur->next = new_obs;
    return 0;
}

int notify_obs_event(obs_event_t *obs_event, uint32_t event_id, void *arg)
{
    if (obs_event == NULL)
    {
        return -ERR_INVALID_OBS_EVENT;
    }
    
    if (event_id >= obs_event->max_event_id)
    {
        return -ERR_EVENT_ID;
    }

    obs_observer_t *cur = obs_event->observers[event_id];
    if (cur != NULL)
    {
        do {
            cur->func(arg);
            cur = cur->next;
        } while (cur != NULL);
    }

    return 0;
}



#if DESC("单实例模式")

static obs_event_t *g_obs_event = NULL;

int create_obs_event_single(uint32_t max_event_id)
{
    if (g_obs_event != NULL)
    {
        if (g_obs_event->max_event_id != max_event_id)
        {

        }

        return 0;
    }

    return create_obs_event(max_event_id, &g_obs_event);
}

int register_obs_event_single(uint32_t event_id, observer_func_t func)
{
    return register_obs_event(g_obs_event, event_id, func);
}

int notify_obs_event_single(uint32_t event_id, void *arg)
{
    return notify_obs_event(g_obs_event, event_id, arg);
}

void destroy_obs_event_single(void)
{
    if (g_obs_event == NULL)
        return;
    
    destroy_obs_event(g_obs_event);
    g_obs_event = NULL;

}

#endif


