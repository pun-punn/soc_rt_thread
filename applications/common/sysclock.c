/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-10     sippa       the first version
 */
#include "sysclock.h"

uint32_t GetAPB1Clock()
{
    uint32_t sysclk, apb2_prescaler, apb2_clock;

    // Get the system clock frequency
    sysclk = HAL_RCC_GetSysClockFreq();

    // Read APB2 prescaler (PPRE2 bits in RCC_CFGR)
    apb2_prescaler = ((RCC->CFGR & RCC_CFGR_PPRE2) >> 13);

    // Decode the APB2 prescaler value
    if (apb2_prescaler < 4)
    {
        apb2_prescaler = 1; // No division
    }
    else
    {
        apb2_prescaler = 1 << (apb2_prescaler - 3); // 2, 4, 8, 16 division
    }

    // Calculate APB2 clock
    apb2_clock = sysclk / apb2_prescaler;

    return apb2_clock;
}
