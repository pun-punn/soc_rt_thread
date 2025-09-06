/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-04     sippakorn.s       the first version
 */

#include "at.h"




void at_send_at()
{
    char *str = "AT\r\n";
    size_t len = strlen(str);
    uint8_t u8_array[len + 1];

    for (size_t i = 0; i < len; i++) {
        u8_array[i] = (uint8_t)str[i];
    }
    rs485_send(u8_array, sizeof(u8_array));

    uint8_t rx_buffer[32];
    rs485_receive(rx_buffer, sizeof(rx_buffer));
    for (int i = 0; i < sizeof(rx_buffer); i++)
    {
        rt_kprintf("%02X ", rx_buffer[i]);
    }
    rt_kprintf("\n");
}
