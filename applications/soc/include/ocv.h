/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_SOC_INCLUDE_OCV_H_
#define APPLICATIONS_SOC_INCLUDE_OCV_H_

#include <math.h>


void initialize_voltage_tables(void);

/**
* @brief    Get Voltage value .
* @note     function based on polynomial description returns voltage based on current SoC and OCV characteristics
*
* @param[in] soc  current value of the state of charge
* @return voltage return the voltage based on OCV characteristics.
*
*/
double ocv_cell(float soc);

/**
* @brief    Get Voltage value .
* @note     function based on polynomial description returns voltage based on current SoC and OCV characteristics
*
* @param[in] soc  current value of the state of charge
* @return voltage return the voltage based on OCV characteristics.
*
*/
double ocv_pack(float soc);

/**
* @brief    Get Voltage value .
* @note     function based on polynomial description returns voltage based on current SoC and OCV characteristics
*
* @param[in] soc  current value of the state of charge
* @return voltage return the voltage based on OCV characteristics.
*
*/

double ocv_sys_poly(float soc);

double ocv_sys(float soc, float current);

double ocv_sys_ch(float soc);

double ocv_sys_dis(float soc);

/**
* @brief    Derivative function for OCV characteristics.
* @note     this function return voltage based on current state of charge value but return only voltage difference.
*
* @param[in] soc  current value of the state of charge
* @return voltage return the voltage based on OCV characteristics.
*
*/
double ocv_deriv(float soc);

double ocv_deriv_sys_poly(float soc);

double ocv_deriv_sys(float soc, float current);

double ocv_deriv_sys_ch(float soc);

double ocv_deriv_sys_dis(float soc);

void compute_derivative_table(const double soc_table[], const double voltage_table[], double derivative_table[], int size);

int find_soc_index(double soc, const double *soc_table, int size);
/**
* @brief    Get State of Charge value.
* @note     function based on polynomial description returns state of charge based on current voltage and OCV characteristics
*
* @param[in] voltage  current value of the voltage of the investigated cell
* @return soc the soc based on OCV characteristics.
*
*/

float ocv_get_soc_by_voltage_cell(double voltage, float current);

float ocv_get_soc_by_voltage_pack(double voltage, float current);

float ocv_get_soc_by_voltage_sys(double voltage, float current);

float ocv_get_soc_by_voltage_sys_poly(double voltage);

float ocv_get_soc_by_voltage_cell_ch(double voltage);

float ocv_get_soc_by_voltage_pack_ch(double voltage);

float ocv_get_soc_by_voltage_sys_ch(double voltage);

float ocv_get_soc_by_voltage_cell_dis(double voltage);

float ocv_get_soc_by_voltage_pack_dis(double voltage);

float ocv_get_soc_by_voltage_sys_dis(double voltage);
#endif /* APPLICATIONS_SOC_INCLUDE_OCV_H_ */
