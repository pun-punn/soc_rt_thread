/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-07     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_MCU_SYSTEM_H_
#define APPLICATIONS_COMMON_MCU_SYSTEM_H_

#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "can.h"
#include "rs485.h"
#include "modbus.h"
#include "j1939.h"
#include "blink.h"
#include "at.h"

void mcu_system_init();
void mcu_system_start();

#endif /* APPLICATIONS_COMMON_MCU_SYSTEM_H_ */
