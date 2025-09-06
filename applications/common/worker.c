/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */

#include "worker.h"

static worker_t *workers = NULL;


void reg_worker(worker_t *wp,rt_thread_t thread,void (*func)(void *parameter),char *name,bool critical, bool autostart)
{
    wp->thread = thread;
    wp->func = func;
    wp->name = name;
    wp->critical = critical;
    wp->autostart = autostart;
    wp->next = workers;

    if(workers != NULL)
    {
        workers->prev = wp;
    }

    wp->prev = NULL;
    workers = wp;
}


void start_workers(void)
{

    for (worker_t *wp = workers; wp; wp = wp->next)
    {
        if (wp->autostart)
        {
            if (wp->thread == NULL)
            {
                wp->thread = rt_thread_create(wp->name, wp->func, RT_NULL,
                                              4096, 10, 30);
                if (wp->thread != NULL)
                {
                    rt_thread_startup(wp->thread);
                }
            }
        }
    }

    rt_system_scheduler_start();

}
