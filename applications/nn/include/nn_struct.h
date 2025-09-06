/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-09     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_NN_INCLUDE_NN_STRUCT_H_
#define APPLICATIONS_NN_INCLUDE_NN_STRUCT_H_

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct {
    double *w;
    double b;
    double out;
    double delta;
} n;

typedef struct {
    n *neurons;
    int num_neurons;
} layer;

typedef struct {
    layer *layers;
    int num_layers;
} nn;

#endif /* APPLICATIONS_NN_INCLUDE_NN_STRUCT_H_ */
