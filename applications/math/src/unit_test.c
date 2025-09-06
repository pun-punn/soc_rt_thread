/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-21     sippakorn.s       the first version
 */

#include "matrice_operation.h"
#include <math.h>
#include "ulapack.h"

Matrix_t *I;
Matrix_t *O;
Matrix_t *K;

void unit_conv2d_tiled();
void unit_conv2d_tiled_33();

void unit_test()
{
    unit_conv2d_tiled();
    unit_conv2d_tiled_33();
}

void unit_conv2d_tiled()
{
    const double data[5][5] = {{0, 1, 2, 3, 0},
                                {1, 2, 3, 0, 1},
                                {2, 3, 0, 1, 2},
                                {3, 0, 1, 2, 3},
                                {0, 1, 2, 3, 0}};

    ulapack_init(&I, 5, 5);
    ulapack_init(&O, 3, 3);
    ulapack_init(&K, 3, 3);

    for(int i=0;i<5;i++)
    {
       for(int j=0;j<5;j++)
       {
           I->entry[i][j] = data[i][j];
       }
    }
    ulapack_print(I, stdout);

    ulapack_eye(O);
    O->entry[0][0] = 0;
    O->entry[1][1] = 0;
    O->entry[2][2] = 0;
    ulapack_print(O, stdout);

    ulapack_eye(K);
    ulapack_scale(K, 3, K);
    ulapack_print(K, stdout);

    int input_size = 5;
    int kernel_size = 3; // Change this for different kernel sizes
    int stride = 1;
    int tile_size = 2; // Tile size for blocking

    conv2d_tiled(*I,*O,*K,input_size,kernel_size,stride,tile_size);
    ulapack_print(O, stdout);

}

void unit_conv2d_tiled_33()
{
    const double data[5][5] = {{0, 1, 2, 3, 0},
                                {1, 2, 3, 0, 1},
                                {2, 3, 0, 1, 2},
                                {3, 0, 1, 2, 3},
                                {0, 1, 2, 3, 0}};

    ulapack_init(&I, 5, 5);
    ulapack_init(&O, 3, 3);
    ulapack_init(&K, 3, 3);

    for(int i=0;i<5;i++)
    {
       for(int j=0;j<5;j++)
       {
           I->entry[i][j] = data[i][j];
       }
    }
    ulapack_print(I, stdout);

    ulapack_eye(O);
    O->entry[0][0] = 0;
    O->entry[1][1] = 0;
    O->entry[2][2] = 0;
    ulapack_print(O, stdout);

    ulapack_eye(K);
    ulapack_scale(K, 3, K);
    ulapack_print(K, stdout);

    int input_size = 5;
    int stride = 1;
    int tile_size = 2; // Tile size for blocking

    conv2d_tiled_unrolling_33(*I,*O,*K,input_size,stride,tile_size);
    ulapack_print(O, stdout);
}
