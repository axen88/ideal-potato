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
#ifndef __OBS_EVENT_H__
#define __OBS_EVENT_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ERR_EVENT_ID = 100,
    ERR_INVALID_OBS_EVENT,
    ERR_EVENT_NO_MEMORY,
} EVENT_ERROR_CODE_E;


typedef void (*observer_func_t)(void *arg);


// 观察者结构定义
typedef struct obs_observer
{  
    observer_func_t func; // 观察者信息
    struct obs_observer *next;
} obs_observer_t;   

// 事件结构定义
typedef struct obs_event
{
    uint32_t max_event_id;
    obs_observer_t **observers; // [max_event_id]观察者列表
} obs_event_t;


void destroy_obs_event(obs_event_t *event);

int create_obs_event(uint32_t max_event_id, obs_event_t **obs_event);

int register_obs_event(obs_event_t *obs_event, uint32_t event_id, observer_func_t func);

int notify_obs_event(obs_event_t *obs_event, uint32_t event_id, void *arg);


#if DESC("单实例模式")

int create_obs_event_single(uint32_t max_event_id);

int register_obs_event_single(uint32_t event_id, observer_func_t func);

int notify_obs_event_single(uint32_t event_id, void *arg);

void destroy_obs_event_single(void);

#endif


#ifdef __cplusplus
}
#endif

#endif


