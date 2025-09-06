/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */

#include "EKF_init.h"
#include <math.h>

#define M_E 2.718281828459045

void ekf_init(float time_step,
              int C1,
              float R1,
              float WH_energy,
              float noise_std,
              Matrix_t A,
              Matrix_t B,
              float R,
              Matrix_t P,
              Matrix_t Q)
{

    float exp_coef = pow(M_E, -(time_step / (C1 * R1)));
    A.entry[0][0] = 1;
    A.entry[0][1] = 0;
    A.entry[1][0] = 0;
    A.entry[1][1] = exp_coef;

    B.entry[0][0] = time_step / (WH_energy * 60 * 60); // using flag to determine that positive and negative: -time_step / (WH_energy * 60 * 60)
    B.entry[1][0] =  R1 * (1 - exp_coef);

    float noise_variance = pow(noise_std, 2);
    R = noise_variance;

    P.entry[0][0] = R;
    P.entry[0][1] = 0;
    P.entry[1][0] = 0;
    P.entry[1][1] = R;

    Q.entry[0][0] = R/50;
    Q.entry[0][1] = 0;
    Q.entry[1][0] = 0;
    Q.entry[1][1] = R/50;

}
