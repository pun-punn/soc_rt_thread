/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-30     sippakorn.s       the first version
 */
#include "data_analy.h"

double extract_data(uint8_t *data, uint8_t start_bit, uint8_t bit_length, float coeff, float offset,uint8_t issigned)
{
    double res = 0.00;

    uint32_t exresult = 0;
    int end_bit = start_bit + bit_length;
    for (int i = start_bit; i < end_bit; i++)
    {
        int byte_index = i / 8;
        int bit_index = 7 - (i % 8);

        exresult = (exresult << 1) | ((data[byte_index] >> bit_index) & 1);
    }

    //example
    //exresult = DF66 = 57190 = 1101111101100110 & 1000000000000000 (2^15)
    if (issigned == 1 && (exresult & (1 << (bit_length - 1)))) //check MSB is 1 by & 32768
    {
        int32_t _exresult = 0;
        _exresult = exresult - (1 << bit_length); //57190 - 65535 = -8345
        float _round_coeff = round(coeff*1000)/1000;
        res = (_exresult * _round_coeff) + offset;
        return res;
    }

    float round_coeff = round(coeff*1000)/1000;
    res = (exresult * round_coeff) + offset;

    return res;
}
