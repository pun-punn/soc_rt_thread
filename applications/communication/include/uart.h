/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_UART_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_UART_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_uart.h"
#include <stdbool.h>

extern UART_HandleTypeDef huart1;

void uart_init_hal(void);
bool uart_init_rt(void);
void uart_send_hal(uint8_t *pData, uint16_t Size);
void uart_send_rt(uint8_t *pData, uint16_t Size);
void uart_receive_hal(uint8_t *pData, uint16_t Size);
void uart_receive_rt(uint8_t *pData, uint16_t Size);
bool is_transmission_complete(void);
void reset_transmission_complete(void);

//callback
rt_err_t tx_done(rt_device_t dev, void *buffer);
rt_err_t rx_done(rt_device_t dev, rt_size_t size);
#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_UART_H_ */
