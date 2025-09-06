/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_WORKER_H_
#define APPLICATIONS_COMMON_INCLUDE_WORKER_H_

#include <rtthread.h>
#include <stdbool.h>

typedef struct worker
{
    rt_thread_t thread;
    bool critical;
    bool autostart;
    char *name;
    void (*func)(void *parameter);
    struct worker *next;
    struct worker *prev;

}worker_t;

void reg_worker(worker_t *wp,rt_thread_t thread, void (*func)(void *parameter),char *name ,bool critical, bool autostart);
void start_workers(void);
#endif /* APPLICATIONS_COMMON_INCLUDE_WORKER_H_ */
