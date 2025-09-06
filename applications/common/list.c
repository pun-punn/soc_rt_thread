/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-28     sippakorn.s       the first version
 */

#include "list.h"

void can_receive_data_list_init(can_receive_data_list *list)
{
    list->size = 0;
    list->capacity = 4;
    list->data = malloc(list->capacity * sizeof(can_receive_data));
}

void can_receive_data_list_add(can_receive_data_list *list, can_receive_data item)
{
    if (list->size >= list->capacity) {
        // Expand the capacity
        list->capacity *= 2;
        list->data = realloc(list->data, list->capacity * sizeof(can_receive_data));
    }
    list->data[list->size++] = item; // Add item and increment size
}

void can_receive_data_list_clear(can_receive_data_list *list)
{
    list->size = 0;
}

size_t can_receive_data_list_count(const can_receive_data_list *list)
{
    return list->size;
}

can_receive_data can_receive_data_list_get(const can_receive_data_list *list, size_t index)
{
    if (index < list->size) {
        return list->data[index];
    }
    can_receive_data empty_data = {0};
    return empty_data;
}

//////////////////////////////////////////////////////////

void dbc_list_info_init(dbc_list *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

void dbc_list_data_init(dbc_info *info, size_t initial_capacity)
{
    info->list_data = malloc(initial_capacity * sizeof(dbc_data));
    info->data_count = 0;
    info->data_capacity = initial_capacity;
}

void dbc_list_info_add(dbc_list *list,dbc_info item)
{
    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 2 : list->capacity * 2;
        list->items = realloc(list->items, new_capacity * sizeof(dbc_info));
        list->capacity = new_capacity;
    }
    list->items[list->count++] = item;
}

void dbc_list_data_add(dbc_info *info,dbc_data item)
{
    if (info->data_count == info->data_capacity)
    {
        size_t new_capacity = info->data_capacity == 0 ? 2 : info->data_capacity * 2;
        info->list_data = realloc(info->list_data, new_capacity * sizeof(dbc_data));
        info->data_capacity = new_capacity;
    }
    info->list_data[info->data_count++] = item;
}

void dbc_list_remove(dbc_list *list,size_t index)
{
    if (index >= list->count) return;
    for (size_t i = index; i < list->count - 1; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;
}

void dbc_list_clear(dbc_list *list)
{
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

size_t dbc_list_count(dbc_list *list)
{
    return list->count;
}

dbc_info* dbc_list_get(dbc_list *list,size_t index)
{
    if (index >= list->count) return NULL;
    return &list->items[index];
}
