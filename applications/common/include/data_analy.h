/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-30     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_DATA_ANALY_H_
#define APPLICATIONS_COMMON_INCLUDE_DATA_ANALY_H_

#include "can_frame_struct.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

double extract_data(uint8_t *data, uint8_t start_bit, uint8_t bit_length,float coeff, float offset,uint8_t issigned);
void temperature_max_calculate();
void temperature_min_calculate();
void temperature_avg_calculate();
void voltage_max_calculate();
void voltage_min_calculate();

#endif /* APPLICATIONS_COMMON_INCLUDE_DATA_ANALY_H_ */
