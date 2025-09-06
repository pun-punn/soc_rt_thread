/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-29     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_DICTIONARY_H_
#define APPLICATIONS_COMMON_INCLUDE_DICTIONARY_H_

#include "can_frame_struct.h"

#define MAX_ENTRIES 128

typedef struct
{
    char key[32];
    double value;
} dictionary_entry;

typedef struct {
    dictionary_entry entries[MAX_ENTRIES];
    int count;
} dictionary;

void dictionary_add_update(dictionary *info, const char *key, double value);
double dictionary_get_value(dictionary *info, const char *key);


#endif /* APPLICATIONS_COMMON_INCLUDE_DICTIONARY_H_ */
