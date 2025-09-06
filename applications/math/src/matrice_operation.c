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

void conv2d_tiled(Matrix_t I, Matrix_t O, Matrix_t K, int input_size, int kernel_size, int stride, int tile_size)
{

    int output_size = (input_size - kernel_size)/stride +1;

    //loop over output matrix size
    for(int ii=0; ii<output_size; ii+=tile_size)
    {
        for(int jj=0; jj<output_size; jj+=tile_size)
        {
            //loop for each tile
            for(int i=ii; i<ii+tile_size && i<output_size; i++)
            {
                for(int j=jj; j<jj+tile_size && j<output_size; j++)
                {
                    float sum = 0.0;
                    //loop for kernel size
                    for(int ki=0; ki<kernel_size; ki++)
                    {
                        for(int kj=0; kj<kernel_size; kj++)
                        {
                            int input_row = i*stride + ki;
                            int input_col = j*stride + kj;
                            sum += I.entry[input_row][input_col] * K.entry[ki][kj];
                            //sum += I.entry[input_row*input_size + input_col] * K[ki*kernel_size + kj];
                        }
                    }

                    O.entry[i][j] = sum;
                }
            }
        }
    }

}

void conv2d_tiled_unrolling_33(Matrix_t I, Matrix_t O, Matrix_t K, int input_size, int stride, int tile_size)
{
    //check kernel size
    if(K.n_cols !=3 || K.n_rows != 3)
    {
        return;
    }

    int kernel_size = K.n_cols;
    int output_size = (input_size - kernel_size)/stride +1;

    //loop over output matrix size
    for(int ii=0; ii<output_size; ii+=tile_size)
    {
        for(int jj=0; jj<output_size; jj+=tile_size)
        {
            //loop for each tile
            for(int i=ii; i<ii+tile_size && i<output_size; i++)
            {
                for(int j=jj; j<jj+tile_size && j<output_size; j++)
                {
                    float sum = 0.0;

                    int input_row = i*stride;
                    int input_col = j*stride;
                    sum +=  I.entry[input_row+0][input_col+0] * K.entry[0][0];
                    sum +=  I.entry[input_row+0][input_col+1] * K.entry[0][1];
                    sum +=  I.entry[input_row+0][input_col+2] * K.entry[0][2];

                    sum +=  I.entry[input_row+1][input_col+0] * K.entry[1][0];
                    sum +=  I.entry[input_row+1][input_col+1] * K.entry[1][1];
                    sum +=  I.entry[input_row+1][input_col+2] * K.entry[1][2];

                    sum +=  I.entry[input_row+2][input_col+0] * K.entry[2][0];
                    sum +=  I.entry[input_row+2][input_col+1] * K.entry[2][1];
                    sum +=  I.entry[input_row+2][input_col+2] * K.entry[2][2];

                    O.entry[i][j] = sum;
                }
            }
        }
    }
}

void conv2d_tiled_unrolling_55()
{

}

void conv2d_tiled_unrolling_77()
{

}
