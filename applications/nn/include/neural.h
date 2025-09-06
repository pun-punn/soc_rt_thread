/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-09     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_NN_INCLUDE_NEURAL_H_
#define APPLICATIONS_NN_INCLUDE_NEURAL_H_

#include "nn_struct.h";

double sigmoid(double x);
double sigmoid_derivative(double x);

void nn_crate_network();
void nn_forward_propagation();
void nn_backpropagation();

#endif /* APPLICATIONS_NN_INCLUDE_NEURAL_H_ */
