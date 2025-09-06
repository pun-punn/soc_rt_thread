/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_SOC_INCLUDE_EKF_INIT_H_
#define APPLICATIONS_SOC_INCLUDE_EKF_INIT_H_

#include "ulapack.h"

/**
* @brief    Performs battery State of Charge algorithm initialization.
* @note     initialization function that is called onlye once and this function describe initial state and
*           main configuration parameters of EKF-SOC algorithm
* @param[in] time_step  time in seconds.
* @param[in] C1  capacity for ECM model
* @param[in] R1  resistance for ECM model
* @param[in] Q_capacity  capacity of the investigated cell in Ah
* @param[in] noise_std  standard deviation of noise in measurement channels
* @param[in] R  return the measurement noise
* @param[out] A  return the array of state transition model 1
* @param[out] B  return the array of state transition model 2
* @param[out] R  return the measurement noise
* @param[out] P  return the state covariance matrix
* @param[out] Q  return the process noise covariance matrix
*
*/
void ekf_init(
        float time_step,
        int C1,
        float R1,
        float Q_capacity,
        float noise_std,
        Matrix_t A,
        Matrix_t B,
        float R,
        Matrix_t P,
        Matrix_t Q
);


#endif /* APPLICATIONS_SOC_INCLUDE_EKF_INIT_H_ */
