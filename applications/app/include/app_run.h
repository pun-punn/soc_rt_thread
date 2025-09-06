/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_APP_INCLUDE_APP_RUN_H_
#define APPLICATIONS_APP_INCLUDE_APP_RUN_H_

#include "common_type.h"
#include "cell.h"
#include "soc.h"
#include "j1939.h"
#include "can.h"

void app_init(void *parameter);
bms_status_t app_init_soc();
bms_status_t app_get_current(bms_Cmeasurements_t *data);
bms_status_t app_get_voltage_all_cells(bms_Vmeasurements_t *data);
bms_status_t app_get_voltage_pack(bms_Vmeasurements_t *data);
bms_status_t app_get_voltage_sys(bms_Vmeasurements_t *data);
battery_status_t app_get_bat_status();
battery_status_t app_get_bat_status_curr(float current);
void app_recalibrate_soc();
void app_get_battery_data(void *parameter);
void app_loop(void);

#endif /* APPLICATIONS_APP_INCLUDE_APP_RUN_H_ */
