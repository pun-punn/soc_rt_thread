/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_BLINK_INCLUDE_BLINK_H_
#define APPLICATIONS_BLINK_INCLUDE_BLINK_H_

#include <rtthread.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"

void gpio_init(void);
void blink_led(void *parameter);
void blink_startup(void);

#endif /* APPLICATIONS_BLINK_INCLUDE_BLINK_H_ */
