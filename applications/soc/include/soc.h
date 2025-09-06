/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_SOC_INCLUDE_SOC_H_
#define APPLICATIONS_SOC_INCLUDE_SOC_H_

#include "common_type.h"
#include "cell.h"

#include "EKF_init.h"
#include "EKF.h"
#include "ocv.h"
#include "ulapack.h"
#include "matrice_operation.h"
#include "energy.h"
#include <math.h>
#include <stdint.h>

#define EKF_MODE 1
#define PRE_MODE 0

extern Matrix_t *x_pack;
extern Matrix_t *x_pack_ekf;
/**
* @brief    Disables the coulomb counting.
* @note     This API is meant for the first device of a stack unless there are other parallel
*           stacks of devices.
*
* @param[in] stack           pointer to the @p l9963_stack_t object
* @param[in] devnum          the selected L9963 instance
* @return                    void.
*
* @api
*/
bms_status_t soc_init(bms_Vmeasurements_t voltage,float current);

/**
* @brief    Disables the coulomb counting.
* @note     This API is meant for the first device of a stack unless there are other parallel
*           stacks of devices.
*
* @param[in] stack           pointer to the @p l9963_stack_t object
* @param[in] devnum          the selected L9963 instance
* @return                    void.
*
* @api
*/
bms_status_t soc_get_value_ekf(float current, bms_Vmeasurements_t voltage, int MODE);

double soc_get_value_ener(float current, bms_Vmeasurements_t voltage);

double soc_get_value_volt(bms_Vmeasurements_t voltage, float current);

double soc_get_value_volt_test(double vol, float current);

double soc_get_value_volt_cell(bms_Vmeasurements_t voltage, float current, int max);

double soc_get_value_curr(float current, bms_Vmeasurements_t voltage);

/**
* @brief    Disables the coulomb counting.
* @note     This API is meant for the first device of a stack unless there are other parallel
*           stacks of devices.
*
* @param[in] stack           pointer to the @p l9963_stack_t object
* @param[in] devnum          the selected L9963 instance
* @return                    void.
*
* @api
*/
bms_status_t soc_set_timestep(uint32_t timestep);
#endif /* APPLICATIONS_SOC_INCLUDE_SOC_H_ */
