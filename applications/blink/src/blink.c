/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "blink.h"

#define LED_PIN GPIO_PIN_14
#define LED_PORT GPIOG

void gpio_init(void)
{
    // Initialize the HAL Library
    HAL_Init();

    // Enable GPIO clock
    __HAL_RCC_GPIOD_CLK_ENABLE();

    // Configure the GPIO pin
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void blink_led(void *parameter)
{
    while(1)
    {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
        rt_thread_mdelay(500);
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
        rt_thread_mdelay(500);
    }

}

void blink_startup(void)
{
    gpio_init();

    rt_thread_t blink_thread = rt_thread_create("blink_thread", blink_led, RT_NULL, 1024, 10, 10);

    if (blink_thread != RT_NULL)
    {
        rt_thread_startup(blink_thread);
    }
}
