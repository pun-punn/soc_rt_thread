/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-28     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_CAN_FRAME_STRUCT_H_
#define APPLICATIONS_COMMON_INCLUDE_CAN_FRAME_STRUCT_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern struct rt_messagequeue can_mq;

typedef struct
{
    struct
    {
        uint32_t can_id;
        uint8_t data[8];
    } frame;
} can_receive_data;



#endif /* APPLICATIONS_COMMON_INCLUDE_CAN_FRAME_STRUCT_H_ */
