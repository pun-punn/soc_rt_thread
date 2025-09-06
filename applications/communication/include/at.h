/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-04     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_AT_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_AT_H_

#include <rtthread.h>
#include "rs485.h"
#include "uart.h"
#include <stdint.h>
#include <string.h>

//sim900a

void at_init();

void at_startup();

void at_loop();

void at_send_cmd();

void at_send_at();

void at_receive();

#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_AT_H_ */
