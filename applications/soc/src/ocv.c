/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */

#include "ocv.h"

#define TABLE_SIZE_CH 62
#define TABLE_SIZE_DIS 61
#define CELL_NUM 21
#define MODULE_NUM (4 * CELL_NUM)
#define PACK_NUM (4 * MODULE_NUM)

double soc_table_ch[TABLE_SIZE_CH] = {
    0.00, 0.0006, 0.0037, 0.0069, 0.0102, 0.0137, 0.0175, 0.0215, 0.0258, 0.0307,
    0.0389, 0.0623, 0.0918, 0.1194, 0.1367, 0.1516, 0.1665, 0.1817, 0.1980, 0.2157,
    0.2363, 0.2562, 0.2755, 0.2968, 0.3247, 0.3568, 0.4005, 0.4492, 0.4935, 0.5324,
    0.5646, 0.5946, 0.6213, 0.6452, 0.6668, 0.6858, 0.6967, 0.7133, 0.7287, 0.7431,
    0.7568, 0.7692, 0.7812, 0.7927, 0.8036, 0.8143, 0.8249, 0.8359, 0.8472, 0.8582,
    0.8701, 0.8797, 0.8919, 0.9046, 0.9166, 0.9290, 0.9411, 0.9536, 0.9656, 0.9778,
    0.9897, 1.0000};

double soc_table_dis[TABLE_SIZE_DIS] = {
    0.00, 0.0117, 0.0235, 0.0355, 0.0475, 0.0579, 0.0719, 0.0841, 0.0963, 0.1084,
    0.1202, 0.1319, 0.1432, 0.1541, 0.1646, 0.1746, 0.1844, 0.1940, 0.2038, 0.2139,
    0.2247, 0.2363, 0.2487, 0.2620, 0.2763, 0.2917, 0.3083, 0.3264, 0.3458, 0.3672,
    0.3907, 0.4168, 0.4459, 0.4780, 0.5135, 0.5512, 0.5883, 0.6196, 0.6447, 0.6693,
    0.6928, 0.7145, 0.7366, 0.7598, 0.7809, 0.7993, 0.8160, 0.8326, 0.8495, 0.8652,
    0.8790, 0.8926, 0.9115, 0.9338, 0.9527, 0.9676, 0.9786, 0.9864, 0.9920, 0.9963,
    1.0000};

//1 cell = 3.40 - 4.00v charge
double voltage_table_cell_ch[TABLE_SIZE_CH] = {
    3.400, 3.40151, 3.41166, 3.42166, 3.43166, 3.44165, 3.45165, 3.46195, 3.47210, 3.48210,
    3.49210, 3.50240, 3.51240, 3.52408, 3.53408, 3.54408, 3.55438, 3.56438, 3.57453, 3.58468,
    3.59498, 3.60498, 3.61513, 3.62528, 3.63558, 3.64588, 3.65588, 3.66588, 3.67587, 3.68572,
    3.69494, 3.70494, 3.71494, 3.72509, 3.73508, 3.74508, 3.75092, 3.76092, 3.77092, 3.78107,
    3.79137, 3.80137, 3.81152, 3.82167, 3.83197, 3.84197, 3.85212, 3.86257, 3.87257, 3.88257,
    3.89272, 3.90056, 3.91056, 3.92071, 3.93070, 3.94070, 3.95070, 3.96100, 3.97100, 3.98115,
    3.99130, 4.00000};

//1 cell = 3.40 - 4.00v discharge
double voltage_table_cell_dis[TABLE_SIZE_DIS] = {
    4.000, 3.990, 3.980, 3.970, 3.960, 3.950, 3.940, 3.930, 3.920, 3.910,
    3.900, 3.890, 3.880, 3.870, 3.860, 3.850, 3.840, 3.830, 3.820, 3.810,
    3.800, 3.790, 3.780, 3.770, 3.760, 3.750, 3.740, 3.730, 3.720, 3.710,
    3.700, 3.690, 3.680, 3.670, 3.660, 3.650, 3.640, 3.630, 3.620, 3.610,
    3.600, 3.590, 3.580, 3.570, 3.560, 3.550, 3.540, 3.530, 3.520, 3.510,
    3.500, 3.490, 3.480, 3.470, 3.460, 3.450, 3.440, 3.430, 3.420, 3.410,
    3.400};

///----------------------charge-------------------------///
//1 pack = 21S2P = 21 cells (3.40-4.00) * 4 = {285.6 - 336}
double voltage_table_pack_ch[TABLE_SIZE_CH];

//1 system = 4 pack (285.6 - 336) * 4 = {1142.4 - 1344}
double voltage_table_sys_ch[TABLE_SIZE_CH];

//system voltage derivative table
double voltage_deriv_table_sys_ch[TABLE_SIZE_CH];

///----------------------discharge----------------------///
//1 pack = 21S2P = 21 cells (3.40-4.00) * 4 = {285.6 - 336}
double voltage_table_pack_dis[TABLE_SIZE_DIS];

//1 system = 4 pack (285.6 - 336) * 4 = {1142.4 - 1344}
double voltage_table_sys_dis[TABLE_SIZE_DIS];

//system voltage derivative table
double voltage_deriv_table_sys_dis[TABLE_SIZE_DIS];

void initialize_voltage_tables(void)
{
    //charge table
    for (int i = 0; i < TABLE_SIZE_CH; i++)
    {
        voltage_table_pack_ch[i] = voltage_table_cell_ch[i] * MODULE_NUM;
        voltage_table_sys_ch[i] = voltage_table_cell_ch[i] * PACK_NUM;
    }

    //discharge table
    for(int j=0; j< TABLE_SIZE_DIS; j++)
    {
        voltage_table_pack_dis[j] = voltage_table_cell_dis[j] * MODULE_NUM;
        voltage_table_sys_dis[j] = voltage_table_cell_dis[j] * PACK_NUM;
    }

    ///----------derivative table-------------///

    //charge
    compute_derivative_table(soc_table_ch,voltage_table_sys_ch, voltage_deriv_table_sys_ch, TABLE_SIZE_CH);

    //discharge
    compute_derivative_table(soc_table_dis,voltage_table_sys_dis, voltage_deriv_table_sys_dis, TABLE_SIZE_DIS);


}

void compute_derivative_table(const double soc_table[], const double voltage_table[], double derivative_table[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i == 0)
        {
            derivative_table[i] = fabs((voltage_table[i + 1] - voltage_table[i])) /
                                  (soc_table[i + 1] - soc_table[i]);
        }
        else if (i == size - 1)
        {
            derivative_table[i] = fabs((voltage_table[i] - voltage_table[i - 1])) /
                                  (soc_table[i] - soc_table[i - 1]);
        }
        else
        {
            derivative_table[i] = fabs((voltage_table[i + 1] - voltage_table[i - 1])) /
                                  (soc_table[i + 1] - soc_table[i - 1]);
        }
    }
}

int find_soc_index(double soc, const double *soc_table, int size)
{
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (soc_table[mid] <= soc && soc_table[mid + 1] > soc) {
            return mid;
        } else if (soc_table[mid] < soc) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; // Not found
}

double ocv_cell(float soc)
{
    /*
    float voltage =
        1 * pow(soc, 9) - 1 * pow(soc, 8)
        + 1 * pow(soc, 7) - 1 * pow(soc, 6)
        + 1 * pow(soc, 5) - 1 * pow(soc, 4)
        + 1 * pow(soc, 3) - 1 * pow(soc, 2)
        + 1 * soc + 1;
    */

    //edge case max/min
    if(soc >= soc_table_ch[TABLE_SIZE_CH -1])
    {
        return voltage_table_cell_ch[TABLE_SIZE_CH -1]; //return Vmax (4.00 v)
    }
    else if(soc <= soc_table_ch[0])
    {
        return voltage_table_cell_ch[0]; //return Vmin (3.00 v)
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_ch[i] && soc <= soc_table_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_table_cell_ch[i] + (soc - soc_table_ch[i]) *
                                    (voltage_table_cell_ch[i + 1] - voltage_table_cell_ch[i]) /
                                    (soc_table_ch[i + 1] - soc_table_ch[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;

}

double ocv_pack(float soc)
{
    //edge case max/min
    if(soc >= soc_table_ch[TABLE_SIZE_CH-1])
    {
        return voltage_table_pack_ch[TABLE_SIZE_CH-1]; //return Vmax (4.00 v)
    }
    else if(soc <= soc_table_ch[0])
    {
        return voltage_table_pack_ch[0]; //return Vmin (3.00 v)
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_ch[i] && soc <= soc_table_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_table_pack_ch[i] + (soc - soc_table_ch[i]) *
                                    (voltage_table_pack_ch[i + 1] - voltage_table_pack_ch[i]) /
                                    (soc_table_ch[i + 1] - soc_table_ch[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;

}

double ocv_sys(float soc, float current)
{
    float ocv = 0.0;

    if(current >=0)
    {
        ocv = ocv_sys_ch(soc);
    }
    else if (current <0)
    {
        soc = 1 - soc;
        ocv = ocv_sys_dis(soc);
    }

    return ocv;
}

double ocv_sys_poly(float soc)
{
    double voltage =
        1800.941125871006 * pow(soc, 9) - 8360.276114127131 * pow(soc, 8)
        + 16383.409658550576 * pow(soc, 7) - 17625.665416989188 * pow(soc, 6)
        + 11336.89182380215 * pow(soc, 5) - 4455.711864932477 * pow(soc, 4)
        + 1054.2224654170263 * pow(soc, 3) - 143.90092381623583 * pow(soc, 2)
        + 11.26987017688682 * soc + 2.92452548;

    return voltage;
}

double ocv_sys_ch(float soc)
{
    //edge case max/min
    if(soc >= soc_table_ch[TABLE_SIZE_CH-1])
    {
        return voltage_table_sys_ch[TABLE_SIZE_CH-1]; //return Vmax
    }
    else if(soc <= soc_table_ch[0])
    {
        return voltage_table_sys_ch[0]; //return Vmin
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_ch[i] && soc <= soc_table_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_table_sys_ch[i] + (soc - soc_table_ch[i]) *
                                    (voltage_table_sys_ch[i + 1] - voltage_table_sys_ch[i]) /
                                    (soc_table_ch[i + 1] - soc_table_ch[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;

}

double ocv_sys_dis(float soc)
{

    //edge case max/min
    if(soc >= soc_table_dis[TABLE_SIZE_DIS-1])
    {
        return voltage_table_sys_dis[TABLE_SIZE_DIS-1]; //return Vmax (4.00 v)
    }
    else if(soc <= soc_table_dis[0])
    {
        return voltage_table_sys_dis[0]; //return Vmin (3.00 v)
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_DIS; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_dis[i] && soc <= soc_table_dis[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_table_sys_dis[i] + (soc - soc_table_dis[i]) *
                                    (voltage_table_sys_dis[i + 1] - voltage_table_sys_dis[i]) /
                                    (soc_table_dis[i + 1] - soc_table_dis[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;

}

double ocv_deriv_sys(float soc, float current)
{
    float ocv = 0.0;

    if(current >=0)
    {
        ocv = ocv_deriv_sys_ch(soc);
    }
    else if (current <0)
    {
        soc = 1 - soc;
        ocv = ocv_deriv_sys_dis(soc);
    }

    return ocv;
}

double ocv_deriv_sys_poly(float soc)
{
    double voltage =
        16208.470132839055 * pow(soc, 8) - 66882.20891301705 * pow(soc, 7)
        + 114683.86760985403 * pow(soc, 6) - 105753.99250193514 * pow(soc, 5)
        + 56684.45911901075 * pow(soc, 4) - 17822.847459729906 * pow(soc, 3)
        + 3162.667396251079 * pow(soc, 2) - 287.80184763247166 * pow(soc, 1)
        + 11.26987017688682;

    return voltage;
}

double ocv_deriv_sys_ch(float soc)
{
    //edge case max/min
    if(soc >= soc_table_ch[TABLE_SIZE_CH-1])
    {
        double slope = (voltage_deriv_table_sys_ch[TABLE_SIZE_CH-1] - voltage_deriv_table_sys_ch[TABLE_SIZE_CH-2]) /
                       (soc_table_ch[TABLE_SIZE_CH-1] - soc_table_ch[TABLE_SIZE_CH-2]);
        return voltage_deriv_table_sys_ch[TABLE_SIZE_CH-1] + slope * (soc - soc_table_ch[TABLE_SIZE_CH-1]);
    }
    else if(soc <= soc_table_ch[0])
    {
        double slope = (voltage_deriv_table_sys_ch[1] - voltage_deriv_table_sys_ch[0]) /
                               (soc_table_ch[1] - soc_table_ch[0]);
        return voltage_deriv_table_sys_ch[1] + slope * (soc - soc_table_ch[1]);
    }

    //new code
    int index = find_soc_index(soc, soc_table_ch, TABLE_SIZE_CH);
    if (index == -1) return -1;

    double ocv_new = voltage_deriv_table_sys_ch[index] + (soc - soc_table_ch[index]) *
                     (voltage_deriv_table_sys_ch[index+1] - voltage_deriv_table_sys_ch[index]) /
                     (soc_table_ch[index+1] - soc_table_ch[index]);

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_ch[i] && soc <= soc_table_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_deriv_table_sys_ch[i] + (soc - soc_table_ch[i]) *
                                    (voltage_deriv_table_sys_ch[i + 1] - voltage_deriv_table_sys_ch[i]) /
                                    (soc_table_ch[i + 1] - soc_table_ch[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;
}

double ocv_deriv_sys_dis(float soc)
{
    //edge case max/min
    if(soc >= soc_table_dis[TABLE_SIZE_DIS-1])
    {
        return voltage_deriv_table_sys_dis[TABLE_SIZE_DIS-1]; //return Vmax
    }
    else if(soc <= soc_table_dis[0])
    {
        return voltage_deriv_table_sys_dis[0]; //return Vmin
    }

    //interpolation by each range
    for(int i=0; i<TABLE_SIZE_DIS; i++)
    {
        //check voltage in range i to (i+1)
        if(soc >= soc_table_dis[i] && soc <= soc_table_dis[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //vol = vol[i] + (vol[i+1]-vol[i])/(soc[i+1]-soc[i])*(soc - soc[i])
            double vol = voltage_deriv_table_sys_dis[i] + (soc - soc_table_dis[i]) *
                                    (voltage_deriv_table_sys_dis[i + 1] - voltage_deriv_table_sys_dis[i]) /
                                    (soc_table_dis[i + 1] - soc_table_dis[i]);
            return vol;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_cell(double voltage, float current)
{
    float soc = 0.0;

    if(current >=0)
    {
        soc = ocv_get_soc_by_voltage_cell_ch(voltage);
    }
    else if (current <0)
    {
        soc = ocv_get_soc_by_voltage_cell_dis(voltage);
        soc = 1- soc;
    }

    return soc;
}

float ocv_get_soc_by_voltage_pack(double voltage, float current)
{
    float soc = 0.0;

    if(current >=0)
    {

    }
    else if (current <0)
    {

    }

    return soc;
}

float ocv_get_soc_by_voltage_sys(double voltage, float current)
{
    float soc = 0.0;

    if(current >=0)
    {
        soc = ocv_get_soc_by_voltage_sys_ch(voltage);
    }
    else if (current < 0)
    {
        soc = ocv_get_soc_by_voltage_sys_dis(voltage);
        soc = 1 - soc;
    }
    return soc;
}

float ocv_get_soc_by_voltage_sys_poly(double voltage)
{
    float soc =
        98.32364478334785 * pow(voltage, 9) - 3128.588525264524 * pow(voltage, 8)
        + 44130.64071982843 * pow(voltage, 7) - 362187.61486364005 * pow(voltage, 6)
        + 1906026.9443413012 * pow(voltage, 5) - 6669995.851822569 * pow(voltage, 4)
        + 15521362.806850897 * pow(voltage, 3) - 23160938.999846745 * pow(voltage, 2)
        + 20110232.580308657 * voltage - 7741501.00321622;

    return soc;
}

float ocv_get_soc_by_voltage_cell_ch(double voltage)
{
    /*
    double soc =
        - 503.965 * pow(voltage, 6)
        + 11360.6 * pow(voltage, 5) - 106597 * pow(voltage, 4)
        + 532890 * pow(voltage, 3) - 1496910 * pow(voltage, 2)
        + 2240230 * voltage - 1395460;
     */

    //edge case max/min
    if(voltage >= voltage_table_cell_ch[TABLE_SIZE_CH-1])
    {
        return soc_table_ch[TABLE_SIZE_CH-1]; //return 1.00 (100%)
    }
    else if(voltage <= voltage_table_cell_ch[0])
    {
        return soc_table_ch[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage >= voltage_table_cell_ch[i] && voltage <= voltage_table_cell_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_ch[i] + (voltage - voltage_table_cell_ch[i]) *
                                    (soc_table_ch[i + 1] - soc_table_ch[i]) /
                                    (voltage_table_cell_ch[i + 1] - voltage_table_cell_ch[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_pack_ch(double voltage)
{
    //edge case max/min
    if(voltage >= voltage_table_pack_ch[TABLE_SIZE_CH-1])
    {
        return soc_table_ch[TABLE_SIZE_CH-1]; //return 1.00 (100%)
    }
    else if(voltage <= voltage_table_pack_ch[0])
    {
        return soc_table_ch[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage >= voltage_table_pack_ch[i] && voltage <= voltage_table_pack_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_ch[i] + (voltage - voltage_table_pack_ch[i]) *
                                    (soc_table_ch[i + 1] - soc_table_ch[i]) /
                                    (voltage_table_pack_ch[i + 1] - voltage_table_pack_ch[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_sys_ch(double voltage)
{
    //edge case max/min
    if(voltage >= voltage_table_sys_ch[TABLE_SIZE_CH-1])
    {
        return soc_table_ch[TABLE_SIZE_CH-1]; //return 1.00 (100%)
    }
    else if(voltage <= voltage_table_sys_ch[0])
    {
        return soc_table_ch[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_CH; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage >= voltage_table_sys_ch[i] && voltage <= voltage_table_sys_ch[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_ch[i] + (voltage - voltage_table_sys_ch[i]) *
                                    (soc_table_ch[i + 1] - soc_table_ch[i]) /
                                    (voltage_table_sys_ch[i + 1] - voltage_table_sys_ch[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_cell_dis(double voltage)
{

    //edge case max/min
    if(voltage <= voltage_table_cell_dis[TABLE_SIZE_DIS-1])
    {
        return soc_table_dis[TABLE_SIZE_DIS-1]; //return 1.00 (100%)
    }
    else if(voltage >= voltage_table_cell_dis[0])
    {
        return soc_table_dis[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_DIS; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage <= voltage_table_cell_dis[i] && voltage >= voltage_table_cell_dis[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_dis[i] + (voltage - voltage_table_cell_dis[i]) *
                                    (soc_table_dis[i + 1] - soc_table_dis[i]) /
                                    (voltage_table_cell_dis[i + 1] - voltage_table_cell_dis[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_pack_dis(double voltage)
{
    //edge case max/min
    if(voltage >= voltage_table_pack_dis[TABLE_SIZE_DIS-1])
    {
        return soc_table_dis[TABLE_SIZE_DIS-1]; //return 1.00 (100%)
    }
    else if(voltage <= voltage_table_pack_dis[0])
    {
        return soc_table_dis[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_DIS; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage >= voltage_table_pack_dis[i] && voltage <= voltage_table_pack_dis[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_dis[i] + (voltage - voltage_table_pack_dis[i]) *
                                    (soc_table_dis[i + 1] - soc_table_dis[i]) /
                                    (voltage_table_pack_dis[i + 1] - voltage_table_pack_dis[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}

float ocv_get_soc_by_voltage_sys_dis(double voltage)
{
    //edge case max/min
    if(voltage <= voltage_table_sys_dis[TABLE_SIZE_DIS-1])
    {
        return soc_table_dis[TABLE_SIZE_DIS-1]; //return 1.00 (100%)
    }
    else if(voltage >= voltage_table_sys_dis[0])
    {
        return soc_table_dis[0]; //return 0.00 (0%)
    }

    //interpolation by each range 0,5,10,15, .. 85,90,95,100
    for(int i=0; i<TABLE_SIZE_DIS; i++)
    {
        //check voltage in range i to (i+1)
        if(voltage <= voltage_table_sys_dis[i] && voltage >= voltage_table_sys_dis[i+1])
        {
            //y = y0 + m(x-x0)
            //m = (yn-y0)/(xn-x0)
            //soc = soc[i] + (soc[i+1]-soc[i])/(vol[i+1]-vol[i])*(vol - vol[i])
            float soc = soc_table_dis[i] + (voltage - voltage_table_sys_dis[i]) *
                                    (soc_table_dis[i + 1] - soc_table_dis[i]) /
                                    (voltage_table_sys_dis[i + 1] - voltage_table_sys_dis[i]);
            return soc;
        }
    }

    //not in voltage range
    return -1;
}


