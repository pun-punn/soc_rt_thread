/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-07     sippakorn.s       the first version
 */

#include "mcu_system.h"

void mcu_system_init()
{
    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the
     *   RTOS is active.
     */
    HAL_Init();

    //SystemClock_Config();
    //SystemClock = 180000000 = 180 MHz
    //APB1Clock =    45000000 = 45 MHz

    //blink_startup(); //start blink thread
    can_init(CAN_TEST_MODE);  //init CAN peripheral for 250k, CAN_RECEIVE_MODE
    //rs485_init();    //init RS485 using UART peripheral
}

void mcu_system_start()
{
    //start thread protocol ethernet, modbus, j1939, mqtt (at command)
    //modbus_startup();
    j1939_init();
    //at_send_at();
}

