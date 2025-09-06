/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-21     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_MATH_INCLUDE_MATRICE_OPERATION_H_
#define APPLICATIONS_MATH_INCLUDE_MATRICE_OPERATION_H_

#include "ulapack.h"
#include <math.h>

void conv2d_tiled(Matrix_t I, Matrix_t O, Matrix_t K, int input_size, int kernel_size, int stride, int tile_size);
void conv2d_tiled_unrolling_33(Matrix_t I, Matrix_t O, Matrix_t K, int input_size, int stride, int tile_size);
void conv2d_tiled_unrolling_55();
void conv2d_tiled_unrolling_77();

#endif /* APPLICATIONS_MATH_INCLUDE_MATRICE_OPERATION_H_ */
