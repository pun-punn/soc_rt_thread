/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-28     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_QUEUE_H_
#define APPLICATIONS_COMMON_INCLUDE_QUEUE_H_

#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include "can_frame_struct.h"

#define QUEUE_SIZE 256

typedef struct
{
    can_receive_data data[QUEUE_SIZE];
    int front;
    int rear;
    int size;
    pthread_mutex_t lock;
    //rt_mutex_t lock;
} queue;

void init_queue(queue* q);
bool is_queue_empty(queue* q);
bool is_queue_full(queue* q);
int queue_count(queue* q);
bool enqueue(queue* q, can_receive_data item);
can_receive_data dequeue(queue* q);

#endif /* APPLICATIONS_COMMON_INCLUDE_QUEUE_H_ */
