/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_CAN_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_CAN_H_

#include <rtthread.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"
#include "can_frame_struct.h"

#define CAN_SEND_MODE 0
#define CAN_RECEIVE_MODE 1
#define CAN_TEST_MODE 2

extern CAN_HandleTypeDef hcan1;
extern queue *q_rev_data_to_analy;

void can_init(int test_mode);
void can_queue_init();
void can_send(uint32_t id, uint8_t *data, uint8_t len);
void can_send_loop(void *parmater);
void can_receive(void);
void can_receive_loop(void *parameter);
void can_startup(void);
void can_test_loop(void *parameter);

#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_CAN_H_ */
