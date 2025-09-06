/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_MODBUS_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_MODBUS_H_

#include <rtthread.h>
#include "rs485.h"
#include "uart.h"
#include <stdint.h>
#include <string.h>

void modbus_init(void);
void modbus_send(uint8_t slave_id, uint8_t function_code, uint16_t start_addr, uint16_t num_regs);
void modbus_receive();
void modbus_parse_response(uint8_t *response, uint16_t length);
uint16_t modbus_crc16(uint8_t *buffer, uint16_t length);
void modbus_startup();
//static void modbus_send_receive_thread(void *parameter);

#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_MODBUS_H_ */
