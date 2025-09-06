/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-28     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_LIST_H_
#define APPLICATIONS_COMMON_INCLUDE_LIST_H_

#include "can_frame_struct.h"
#include "dbc_struct.h"

typedef struct
{
    can_receive_data *data;
    size_t size;
    size_t capacity;
} can_receive_data_list;

typedef struct {
    dbc_info *items;
    size_t count;
    size_t capacity;
} dbc_list;

//CAN data list
void can_receive_data_list_init(can_receive_data_list *list);
void can_receive_data_list_add(can_receive_data_list *list, can_receive_data item);
void can_receive_data_list_clear(can_receive_data_list *list);
size_t can_receive_data_list_count(const can_receive_data_list *list);
can_receive_data can_receive_data_list_get(const can_receive_data_list *list, size_t index);

//DBC data list
void dbc_list_info_init(dbc_list *list);
void dbc_list_data_init(dbc_info *info, size_t initial_capacity);
void dbc_list_info_add(dbc_list *list,dbc_info item);
void dbc_list_data_add(dbc_info *info,dbc_data item);
void dbc_list_remove(dbc_list *list,size_t index);
void dbc_list_clear(dbc_list *list);
size_t dbc_list_count(dbc_list *list);
dbc_info* dbc_list_get(dbc_list *list,size_t index);
#endif /* APPLICATIONS_COMMON_INCLUDE_LIST_H_ */
