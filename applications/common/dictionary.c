/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-29     sippakorn.s       the first version
 */

#include "dictionary.h"

void dictionary_add_update(dictionary *info, const char *key, double value)
{
    // Check if the key already exists
    for (int i = 0; i < info->count; ++i)
    {
        if (strcmp(info->entries[i].key, key) == 0)
        {
            info->entries[i].value = value;
            return;
        }
    }

    // Add new entry if key does not exist
    if (info->count < MAX_ENTRIES)
    {
        strncpy(info->entries[info->count].key, key, 32);
        info->entries[info->count].value = value;
        info->count++;
    }
}
double dictionary_get_value(dictionary *info, const char *key)
{
    for (int i = 0; i < info->count; ++i)
    {
        if (strcmp(info->entries[i].key, key) == 0)
        {
            return info->entries[i].value;
        }
    }
    return 0.0; // Or a specific error value
}
