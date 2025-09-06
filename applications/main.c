/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     RT-Thread    first version
 */

#include <rtthread.h>
#include "mcu_system.h"
#include "worker.h"
#include "app_run.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <string.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static worker_t app_worker;
static rt_thread_t app_worker_thread;

static void app_reg(void)
{
    reg_worker(&app_worker, app_worker_thread, app_init,"app_thread" ,true, true);
}

//PA9 PA10 using for UART debug ?
int main(void)
{

    //mcu system init such as hal, ethernet, can, rs485 and uart
    mcu_system_init();

    //app register
    app_reg();

    //mcu system start such as j1939, modbus
    mcu_system_start();

    //start app thread
    start_workers();

    return 1;

}
