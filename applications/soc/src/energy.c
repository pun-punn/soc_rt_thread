/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-11-13     sippakorn.s       the first version
 */
#include "energy.h"

#define TABLE_SIZE 21

double soce_table[TABLE_SIZE] = {
    0.00, 0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.45,
    0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90, 0.95,1.00};

//Emax (wh) = v*ah = (4.2*21*4*4)*(96*18) = 1411.2 * 1728 = 2438553.6
//Emax (wh) = v*ah = (4.0*21*4*4)*(96*18) = 1344   * 1728 = 2322432.0
//Enor (wh) = v*ah = (3.7*21*4*4)*(96*18) = 1243.2 * 1728 = 2148249.6
//Emin (wh) = v*ah = (3.4*21*4*4)*(96*18) = 1142.4 * 1728 = 1974067.2

//assume soc and energy is linear relation
//RD suggestion energy usage = 0.75*2,438,553.6 = 1828915.2 -> (10% top - 15% bot)
//1828915.2 is maximum energy (100% SOC)
double energy_table[TABLE_SIZE] = {
    0, 91445.76, 182891.52, 274337.28, 365783.04, 457228.8, 548674.56, 640120.32, 731566.08, 823011.84,
    914457.6, 1005903.36, 1097349.12, 1188794.88, 1280240.64, 1371686.4, 1463132.16, 1554577.92, 1646023.68, 1737469.44, 1828915.2};

double energy_get_ener_by_soc(double soc)
{
    //edge case max/min
    if(soc >= soce_table[TABLE_SIZE-1])
    {
        return energy_table[TABLE_SIZE-1]; //return Emax
    }
    else if(soc <= soce_table[0])
    {
        return energy_table[0]; //return Emin
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soce_table[i] && soc <= soce_table[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //ener = ener[i] + (ener[i+1]-ener[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double ener = energy_table[i] + (soc - soce_table[i]) *
                                    (energy_table[i + 1] - energy_table[i]) /
                                    (soce_table[i + 1] - soce_table[i]);
            return ener;
        }
    }

    //not in voltage range
    return -1;

}
