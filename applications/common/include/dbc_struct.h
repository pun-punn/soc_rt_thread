/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-29     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_DBC_STRUCT_H_
#define APPLICATIONS_COMMON_INCLUDE_DBC_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct
{
    char *name;
    u_int8_t issigned;
    u_int8_t begbit;
    u_int8_t bitnumber;
    float coeff;
    float offset;
} dbc_data;

typedef struct
{
    uint32_t id;
    dbc_data *list_data;
    size_t data_count;
    size_t data_capacity;
} dbc_info;


#endif /* APPLICATIONS_COMMON_INCLUDE_DBC_STRUCT_H_ */
