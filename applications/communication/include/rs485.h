/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_RS485_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_RS485_H_

#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

void rs485_init(void);
void rs485_gpio_control_init(void);
void rs485_mode(int mode);
void rs485_send(uint8_t *pData, uint16_t Size);
void rs485_receive_loop(void);
void rs485_receive(uint8_t *pData, uint16_t Size);
void rs485_send_receive(void *parameter);
void rs485_startup();
void rs485_de_state(void);
void rs485_re_state(void);
#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_RS485_H_ */
