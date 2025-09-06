/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */

#include "soc.h"

#define CELL_NUMBERS 42
#define WH_ENERGY 1828915 // 0.75% of WH_ENERGY_MAX
#define WH_ENERGY_MAX 2438553 // (4.2*21*4*4) * (96*18)
#define AH_CAPACITY_MAX 1728 //(96*18)
#define NOISE_STD 0.015

//parameter in thevenin for 1 order RC
/*
 * ----R0-----C1-------
 *         |      |
 * -----------R1-------
 */
#define C1 20000  //cell polarization capacitance (F)
#define R1 0.01   //cell polarization resistance  (Ohm)
#define R0 0.0009 //cell ohmic resistance = DCIR of battery cell (Ohm)

//module 21S2P = 21 resistance cell in serial and 2 parallel
#define MODULE_R0 (R0 * 21 * 0.5)
#define MODULE_R1 (R1 * 21 * 0.5)
#define MODULE_C1 ((C1*2)/21)

//pack 4 module = 4 resistance module in serial
#define PACK_R0 (MODULE_R0 * 4)
#define PACK_R1 (MODULE_R1 * 4)
#define PACK_C1 (MODULE_C1 * 0.25)

//string 4 pack = 4 resistance pack in serial
#define STRING_R0 (PACK_R0 * 4)
#define STRING_R1 (PACK_R1 * 4)
#define STRING_C1 (PACK_C1 * 0.25)

//system 18 string = 18 resistance string in parallel
#define SYS_R0 (STRING_R0/18)
#define SYS_R1 (STRING_R1/18)
#define SYS_C1 (STRING_C1*18)

float soc[2][14] = {};

Matrix_t *A;
Matrix_t *B;
Matrix_t *Q;
Matrix_t *_P;
Matrix_t *P_CELL;
Matrix_t *P_PACK;
Matrix_t *P_PACK_EKF;

Matrix_t *x_pack;
Matrix_t *x_pack_ekf;
Matrix_t *x_cell;

float R=0;

//energy
double energy;
double soc_g;

//static float current;
static uint32_t time_step = 1;
static bool init_lib = false;

bms_status_t soc_init(bms_Vmeasurements_t voltage,float current)
{
    bms_status_t status = BMS_OK;
    initialize_voltage_tables();

    //init global matrix size
    ulapack_init(&A, 2, 2);
    ulapack_init(&B, 2, 1);
    ulapack_init(&Q, 2, 2);

    ulapack_init(&P_PACK, 2, 2);
    ulapack_init(&P_PACK_EKF, 2, 2);

    ulapack_init(&x_pack, 2, 1);
    ulapack_init(&x_pack_ekf, 2, 1);

    double init_soc_pack = 0;
    ekf_init(time_step, SYS_C1, SYS_R1, WH_ENERGY, NOISE_STD, *A, *B, R, *P_PACK, *Q); // AH_CAPACITY_MAX, WH_ENERGY


    //Note: get init_soc by total voltage
    init_soc_pack = ocv_get_soc_by_voltage_sys(voltage.vsum_sys, current);
    //init_soc_pack = ocv_get_soc_by_voltage_cell(voltage.cell_volt_high, current);
    //init_soc_pack = ocv_get_soc_by_voltage_cell(voltage.cell_volt_low, current);

    x_pack->entry[0][0] = init_soc_pack;
    x_pack->entry[1][0] = 0;

    x_pack_ekf->entry[0][0] = init_soc_pack;
    x_pack_ekf->entry[1][0] = 0;

    //using init_soc_pack to get energy remaining
    energy = energy_get_ener_by_soc(init_soc_pack);

    //using counting
    soc_g = init_soc_pack;

    /*
    float init_soc_cell = 0;
    cell_id_t cell = CELL1;

    //init soc for 42 cell
    for(cell_id_t _cell = CELL1; _cell < CELL_ALL; _cell++)
    {
        //get soc by voltage polynomial
        init_soc_cell = ocv_get_soc_by_voltage(voltage.vcell_data[_cell]);
        cell++;
    }
    */

    init_lib = true;
    return status;
}

bms_status_t soc_get_value_ekf(float current, bms_Vmeasurements_t voltage, int MODE)
{
    if (false == init_lib)
    {
        return BMS_ERROR;
    }

    //case measure sum voltage from system 1142.4 - 1344 v
    if(voltage.vsum_sys > (1344+5)) //edge case upper soc = 100%
    {
        x_pack->entry[0][0] = 1;
        x_pack_ekf->entry[0][0] = 1;
    }
    else if (voltage.vsum_sys < (1142.4-5)) //edge case lower soc = 0%
    {
        x_pack->entry[0][0] = 0;
        x_pack_ekf->entry[0][0] = 0;
    }
    else
    {
        if(MODE == EKF_MODE)
        {
            //ekf_predict(A,B,Q,current,voltage.vsum_sys,x_pack,P_PACK);
            //ekf_update(voltage.vsum_sys + SYS_R0*current,0.015*0.015,WH_ENERGY,x_pack,P_PACK, current);

            ekf_predict(A,B,Q,current,voltage.vsum_sys,x_pack_ekf,P_PACK_EKF);
            ekf_update(voltage.vsum_sys + SYS_R0*current,0.015*0.015,WH_ENERGY,x_pack_ekf,P_PACK_EKF, current);
        }
        else if (MODE == PRE_MODE)
        {
            ekf_predict(A,B,Q,current,voltage.vsum_sys,x_pack,P_PACK);
            //ekf_predict(A,B,Q,current,voltage.vsum_sys,x_pack_ekf,P_PACK_EKF);
        }

    }

    //control law if soc calculation is upper/lower
    if(x_pack->entry[0][0] > 1)
    {
        x_pack->entry[0][0] = 1;
    }
    else if (x_pack->entry[0][0] < 0)
    {
        x_pack->entry[0][0] = 0;
    }

    return BMS_OK;
}

double soc_get_value_ener(float current, bms_Vmeasurements_t voltage)
{
    double soc = 0.0;
    double ratio = 0.0;
    energy += (current*voltage.vsum_sys*time_step)/(3600);

    ratio = energy/WH_ENERGY;

    if(ratio >=1)
    {
        ratio = 1;
    }
    else if (ratio <=0)
    {
        ratio = 0;
    }

    soc = 100*ratio;

    return soc;
}

double soc_get_value_volt(bms_Vmeasurements_t voltage, float current)
{
    double soc = 0.0;

    soc = ocv_get_soc_by_voltage_sys(voltage.vsum_sys, current);

    if(soc >=1)
    {
        soc = 1;
    }
    else if (soc<=0)
    {
        soc = 0;
    }

    return soc;
}

double soc_get_value_volt_cell(bms_Vmeasurements_t voltage, float current, int max)
{
    double soc = 0.0;

    if(max == 1)
    {
        soc = ocv_get_soc_by_voltage_cell(voltage.cell_volt_high, current);
    }
    else if (max ==0)
    {
        soc = ocv_get_soc_by_voltage_cell(voltage.cell_volt_low, current);
    }

    if(soc >=1)
    {
        soc = 1;
    }
    else if (soc<=0)
    {
        soc = 0;
    }

    return soc;
}

double soc_get_value_volt_test(double vol, float current)
{
    double soc = 0.0;
    soc = ocv_get_soc_by_voltage_sys(vol, current);

    if(soc >=1)
    {
        soc = 1;
    }
    else if (soc<=0)
    {
        soc = 0;
    }

    return soc;
}

double soc_get_value_curr(float current, bms_Vmeasurements_t voltage)
{
    double rate = 0.0;
    rate = (current*voltage.vsum_sys)/((double)WH_ENERGY*60.0*60.0);
    soc_g+=rate;

    if(soc_g >=1)
    {
        soc_g = 1;
    }
    else if (soc_g<=0)
    {
        soc_g = 0;
    }

    return soc_g;
}

bms_status_t soc_set_timestep(uint32_t timestep)
{
  time_step = timestep;

  return BMS_OK;
}

