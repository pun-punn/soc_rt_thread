/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-07     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_COMMON_TYPE_H_
#define APPLICATIONS_COMMON_INCLUDE_COMMON_TYPE_H_

#include <stdbool.h>

//config parameters
#define NUM_CELL 42
#define NUM_PDU 18
#define NUM_PACK (NUM_PDU*4)
typedef enum
{
    BMS_ERROR = -3,
    BMS_NULLPTR,
    BMS_NOT_READY,
    BMS_OK,
} bms_status_t;

typedef enum
{
    BAT_INIT = -1,
    BAT_WAIT = 0,
    BAT_FAULT,
    BAT_NO_CHARGE,
    BAT_NO_DISCHARGE,
    BAT_CHARGING,
    BAT_DISCHARGING,
} battery_status_t;

//The number of voltage must be configured according to number of cells
typedef struct
{
  bool  vcell_ready[NUM_CELL];
  float vcell_data[NUM_CELL];
  float vsum_pack[NUM_PACK];
  double vsum_sys;
  double cell_volt_high;
  double cell_volt_low;
  double cell_volt_diff;
} bms_Vmeasurements_t;

typedef struct
{
  float current;
  float coul_count;
  float conv_current;
} bms_Cmeasurements_t;

#endif /* APPLICATIONS_COMMON_INCLUDE_COMMON_TYPE_H_ */
