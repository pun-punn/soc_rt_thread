/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-28     sippakorn.s       the first version
 */

#include "queue.h"

void init_queue(queue* q)
{
    q->front = 0;
    q->rear = 0;
    q->size = 0;
    pthread_mutex_init(&q->lock, NULL);
    //q->lock = rt_mutex_create("q_lock", RT_IPC_FLAG_FIFO);
}

bool is_queue_empty(queue* q)
{
    return q->front == q->rear;
}

bool is_queue_full(queue* q)
{
    return (q->rear + 1) % QUEUE_SIZE == q->front;
}

int queue_count(queue* q)
{
    return q->size;
}

bool enqueue(queue* q, can_receive_data item)
{
    pthread_mutex_lock(&q->lock);
    //rt_mutex_take(q->lock, RT_WAITING_FOREVER);
    if (is_queue_full(q)) {
        pthread_mutex_unlock(&q->lock);
        //rt_mutex_release(q->lock);
        return false; // Queue is full
    }
    q->data[q->rear] = item;
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->size++;
    pthread_mutex_unlock(&q->lock);
    //rt_mutex_release(q->lock);
    return true;
}

can_receive_data dequeue(queue* q)
{
    pthread_mutex_lock(&q->lock);
    can_receive_data item = {0};
    //rt_mutex_take(q->lock, RT_WAITING_FOREVER);
    if (!is_queue_empty(q))
    {
        item = q->data[q->front];
        q->front = (q->front + 1) % QUEUE_SIZE;
        q->size--;
    }
    pthread_mutex_unlock(&q->lock);
    //rt_mutex_release(q->lock);
    return item;
}
