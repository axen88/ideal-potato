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
File Name: OBS_EVENT.H
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

typedef enum
{
    ERR_EVENT_ID = 100,
    ERR_INVALID_OBS_EVENT,
    ERR_EVENT_NO_MEMORY,
} EVENT_ERROR_CODE_E;

typedef enum
{
    EVENT_EXAMPLE = 0,

    
    EVENT_NUM = 100,
} EVENT_ID_E;

typedef void (*observer_func_t)(void *arg);



void destroy_obs_event(obs_event_t *event);

int create_obs_event(uint32_t max_event_id, obs_event_t **obs_event);

int register_obs_event(obs_event_t *obs_event, EVENT_ID_E event_id, observer_func_t func);

void notify_obs_event(obs_event_t *obs_event, EVENT_ID_E event_id, void *arg);


#if DESC("单实例模式")

int create_obs_event_single(uint32_t max_event_id);

int register_obs_event_single(EVENT_ID_E event_id, observer_func_t func);

void notify_obs_event_single(obs_event_t *obs_event, EVENT_ID_E event_id, void *arg);

#endif


