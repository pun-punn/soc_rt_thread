/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-11     sippakorn.s       the first version
 */
#include "app_run.h"

bms_Vmeasurements_t v_meas;
bms_Cmeasurements_t c_meas;
battery_status_t state;
battery_status_t state_prev;

battery_status_t states[5];

static rt_mutex_t state_mutex;

#define CELL_VOLT_DIFF_THD 0.120
#define CELL_VOLT_HIGH_CHG_THD 4.0
#define CELL_VOLT_LOW_CHG_THD 4.0
#define CELL_VOLT_HIGH_DIS_THD 4.0
#define CELL_VOLT_LOW_DIS_THD 4.0
#define TIME_THD 10000 // 10 sec
#define TIME_REST_THD 300000 //60000 = 1 min * 5 = 300000

void app_init(void *parameter)
{
   rt_kprintf("RT-Thread application thread starting...\n");

   bms_status_t s = BMS_NOT_READY;
   while(1)
   {
       s = app_init_soc();
       if(s == BMS_OK)
       {
           break;
       }
       rt_thread_mdelay(100);
   }

   app_loop();

}

bms_status_t app_init_soc(void)
{
    bms_status_t status = BMS_NOT_READY;
    bms_status_t status_v = BMS_NOT_READY;
    bms_status_t status_c = BMS_NOT_READY;

    status_v = app_get_voltage_sys(&v_meas);
    status_c = app_get_current(&c_meas);

    //rt_thread_delay(1000);
    if(status_v == BMS_OK && status_c == BMS_OK)
    {
        status = BMS_OK;
        state = BAT_INIT;
        state_prev = BAT_INIT;
        state_mutex = rt_mutex_create("state_mutex", RT_IPC_FLAG_FIFO);
        soc_init(v_meas,c_meas.current);
    }
    return status;
}

void app_loop()
{
    while(1)
    {
        app_get_voltage_sys(&v_meas);
        app_get_current(&c_meas); //the current convert using 2 complement

        if(c_meas.current >= 0  && c_meas.current <= 5)
        {
            //c_meas.current = 0;
        }

        app_recalibrate_soc();

        //soc will be calculate from soc = soc + (dt*iv/WH*3600)
        //dt is fix to 1 if there is the current soc will increase or decrease depend on current
        soc_get_value_ekf(c_meas.current, v_meas,PRE_MODE); //using ekf only predict part
        double soc_count = x_pack->entry[0][0] * 100; //re-calibrate when there is no load (current < 10A in 5 min)

        //ekf: predict and update
        soc_get_value_ekf(c_meas.current, v_meas,EKF_MODE);
        double soc_ekf = x_pack_ekf->entry[0][0] * 100;

        //soc from energy
        //double soc2 = soc_get_value_ener(c_meas.current, v_meas);

        //soc from voltage
        double soc_v =soc_get_value_volt(v_meas,c_meas.current);
        soc_v = soc_v*100;

        double soc_max = soc_get_value_volt_cell(v_meas, c_meas.current, 1);
        soc_max = soc_max*100;

        double soc_min = soc_get_value_volt_cell(v_meas, c_meas.current, 0);
        soc_min = soc_min*100;

        double soc_count_wo = soc_get_value_curr(c_meas.current, v_meas);
        soc_count_wo = soc_count_wo*100; //no re-calibrate

        printf("<Start> SumVolt: %.3f, SumCurr: %.3f, State: %d, SOCATESS: %.3f, SOCCount: %.3f, SOCCountWO: %.3f, SOCV: %.3f, SOCMAX: %.3f, SOCMIN: %.3f, SOCEKF: %.3f <End>\n",
                v_meas.vsum_sys, c_meas.current, state ,dictionary_get_value(&bms_cananly_info, "SOC"),soc_count,soc_count_wo,soc_v,soc_max,soc_min,soc_ekf);

        rt_thread_delay(1000);
    }

}

void app_get_battery_data(void *parameter)
{
    while(1)
    {
        rt_kprintf("Loop ........\n");
        //soc_get_value(c_meas.current, v_meas);
    }

}

bms_status_t app_get_voltage_all_cells(bms_Vmeasurements_t *data)
{
    bms_status_t status = BMS_OK;
    //GET data from CAN or RS485

    data->vcell_data[CELL1] = 0;
    data->vcell_data[CELL2] = 0;
    data->vcell_data[CELL3] = 0;
    data->vcell_data[CELL4] = 0;
    data->vcell_data[CELL5] = 0;
    data->vcell_data[CELL6] = 0;
    data->vcell_data[CELL7] = 0;
    data->vcell_data[CELL8] = 0;
    data->vcell_data[CELL9] = 0;
    data->vcell_data[CELL10] = 0;
    data->vcell_data[CELL11] = 0;
    data->vcell_data[CELL12] = 0;
    data->vcell_data[CELL13] = 0;
    data->vcell_data[CELL14] = 0;
    data->vcell_data[CELL15] = 0;
    data->vcell_data[CELL16] = 0;
    data->vcell_data[CELL17] = 0;
    data->vcell_data[CELL18] = 0;
    data->vcell_data[CELL19] = 0;
    data->vcell_data[CELL20] = 0;
    data->vcell_data[CELL21] = 0;
    data->vcell_data[CELL22] = 0;
    data->vcell_data[CELL23] = 0;
    data->vcell_data[CELL24] = 0;
    data->vcell_data[CELL25] = 0;
    data->vcell_data[CELL26] = 0;
    data->vcell_data[CELL27] = 0;
    data->vcell_data[CELL28] = 0;
    data->vcell_data[CELL29] = 0;
    data->vcell_data[CELL30] = 0;
    data->vcell_data[CELL31] = 0;
    data->vcell_data[CELL32] = 0;
    data->vcell_data[CELL33] = 0;
    data->vcell_data[CELL34] = 0;
    data->vcell_data[CELL35] = 0;
    data->vcell_data[CELL36] = 0;
    data->vcell_data[CELL37] = 0;
    data->vcell_data[CELL38] = 0;
    data->vcell_data[CELL39] = 0;
    data->vcell_data[CELL40] = 0;
    data->vcell_data[CELL41] = 0;
    data->vcell_data[CEll42] = 0;

    return status;
}

bms_status_t app_get_voltage_sys(bms_Vmeasurements_t *data)
{
    bms_status_t status = BMS_OK;
    data->vsum_sys = dictionary_get_value(&bms_cananly_info, "SumVolt");
    data->cell_volt_high = dictionary_get_value(&bms_cananly_info, "CellVoltHigh");
    data->cell_volt_low = dictionary_get_value(&bms_cananly_info, "CellVoltLow");

    //test discharge/charge cell voltage
    data->cell_volt_high = 3.96; // 3.516  3.985   3.96
    data->cell_volt_low = 3.862;   // 3.45   3.889   3.862

    data->cell_volt_diff = data->cell_volt_high - data->cell_volt_low;

    if(data->vsum_sys == 0) //|| data->cell_volt_high == 0 || data->cell_volt_low == 0)
    {
        status = BMS_NOT_READY;
    }

    return status;
}

bms_status_t app_get_current(bms_Cmeasurements_t *data)
{
    bms_status_t status = BMS_OK;
    data->current = dictionary_get_value(&bms_cananly_info, "SumCurr");

    if(data->current == 0)
    {
        status = BMS_NOT_READY;
    }

    return status;
}

battery_status_t app_get_bat_status()
{
    battery_status_t status = BAT_INIT;
    int flag = dictionary_get_value(&bms_cananly_info, "BATStatus");
    if (rt_mutex_take(state_mutex, RT_WAITING_FOREVER) == RT_EOK)
    {
        if(flag != state)
        {
            state_prev = state;
        }
        rt_mutex_release(state_mutex);
    }

    switch(flag)
    {
        case 0: //waiting
            status = BAT_WAIT;
            break;
        case 1: //fault
            status = BAT_FAULT;
            break;
        case 2: //no charge
            status = BAT_NO_CHARGE;
            break;
        case 3: //no discharge
            status = BAT_NO_DISCHARGE;
            break;
        case 4: //charging
            status = BAT_CHARGING;
            break;
        case 5: //discharging
            status = BAT_DISCHARGING;
            break;
        default:
            break;
    }

    return status;

}

battery_status_t app_get_bat_status_curr(float current)
{
    static rt_tick_t start_time_rest = 0;
    static rt_tick_t start_time_charge = 0;
    static rt_tick_t start_time_discharge = 0;
    battery_status_t status = state;

    if(current >=0 && current <=10)
    {
        if (state != BAT_WAIT)
        {
            if (start_time_rest == 0)
            {
                start_time_rest = rt_tick_get();
            }
            else if (abs(rt_tick_get() - start_time_rest) >= rt_tick_from_millisecond(TIME_REST_THD))
            {
                start_time_rest = 0;
                status = BAT_WAIT;
            }
        }
        else if (state == BAT_WAIT)
        {
            start_time_rest = 0;
            start_time_charge = 0;
            start_time_discharge = 0;
            status = BAT_WAIT;
        }
    }
    else if (current > 10)
    {
        if (state != BAT_CHARGING)
        {
            if (start_time_charge == 0)
            {
                start_time_charge = rt_tick_get();
            }
            else if (abs(rt_tick_get() - start_time_charge) >= rt_tick_from_millisecond(TIME_THD))
            {
                start_time_charge = 0;
                status = BAT_CHARGING;
            }
        }
        else if (state == BAT_CHARGING)
        {
            start_time_rest = 0;
            start_time_charge = 0;
            start_time_discharge = 0;
            status = BAT_CHARGING;
        }

    }
    else if (current < 0)
    {
        if (state != BAT_DISCHARGING)
        {
            if (start_time_discharge == 0)
            {
                start_time_discharge = rt_tick_get();
            }
            else if (abs(rt_tick_get() - start_time_discharge) >= rt_tick_from_millisecond(TIME_THD))
            {
                start_time_discharge = 0;
                status = BAT_DISCHARGING;
            }
        }
        else if (state == BAT_DISCHARGING)
        {
            start_time_rest = 0;
            start_time_charge = 0;
            start_time_discharge = 0;
            status = BAT_DISCHARGING;
        }
    }

    if (rt_mutex_take(state_mutex, RT_WAITING_FOREVER) == RT_EOK)
    {
        if (status != state)
        {
            start_time_rest = 0;
            start_time_charge = 0;
            start_time_discharge =0;
            state_prev = state;
        }
        rt_mutex_release(state_mutex);
    }

    return status;
}

void app_recalibrate_soc()
{
    //state = app_get_bat_status();
    state = app_get_bat_status_curr(c_meas.current);
    switch(state)
    {
        case 0: //waiting
            //counting method need to get re-calibrate SOC value
            //if cell voltage parameter > thresholding using max/min cell voltage to re-calibrate
            //check voltage difference first
            if(v_meas.cell_volt_diff >= CELL_VOLT_DIFF_THD)
            {
                if(state_prev == BAT_CHARGING)
                {
                    double soc_cal_max = soc_get_value_volt_cell(v_meas, c_meas.current, 1);
                    x_pack->entry[0][0] = soc_cal_max;
                }
                else if (state_prev == BAT_DISCHARGING)
                {
                    double soc_cal_min = soc_get_value_volt_cell(v_meas, c_meas.current, 0);
                    x_pack->entry[0][0] = soc_cal_min;
                }

            }
            else
            {
                if(state_prev == BAT_CHARGING)
                {
                    float ch_dummy = 50;
                    double soc_cal_ch = soc_get_value_volt(v_meas,ch_dummy); //force to use look-up in charge table
                    x_pack->entry[0][0] = soc_cal_ch;
                }
                else if (state_prev == BAT_DISCHARGING)
                {
                    float dis_dummy = -50;
                    double soc_cal_dis = soc_get_value_volt(v_meas,dis_dummy); //force to use look-up in discharge table
                    x_pack->entry[0][0] = soc_cal_dis;
                }
                else if (state_prev == BAT_INIT)
                {
                    double soc_cal_init = soc_get_value_volt(v_meas,c_meas.current);
                    x_pack->entry[0][0] = soc_cal_init;
                }
            }

            //break;
        case 1: //fault
            //status = BAT_FAULT;
            break;
        case 2: //no charge
            //status = BAT_NO_CHARGE;
            break;
        case 3: //no discharge
            //status = BAT_NO_DISCHARGE;
            break;
        case 4: //charging
            //status = BAT_CHARGING;
            break;
        case 5: //discharging
            //status = BAT_DISCHARGING;
            break;
        default:
            break;
    }
}
