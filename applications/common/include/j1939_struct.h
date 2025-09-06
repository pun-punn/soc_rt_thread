/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-31     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMON_INCLUDE_J1939_STRUCT_H_
#define APPLICATIONS_COMMON_INCLUDE_J1939_STRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

//180150xx
typedef struct
{
    //if there are multiple position max/min it will return first index
    uint32_t sa;
    uint8_t tmax_pack_num;
    uint8_t tmax;
    uint8_t tmin_pack_num;
    uint8_t tmin;
    uint8_t soc;
    uint8_t soh;
    uint8_t tdiff;

} j1939_50_01_data;

//180250xx
typedef struct
{
    uint32_t sa;
    float vol;
    float curr;
    uint16_t vol_raw;
    uint16_t curr_raw;

} j1939_50_02_data;

//180350xx
typedef struct
{
    uint32_t sa;
    uint8_t vmax_pack_num;
    uint8_t vmax_cell_num;
    uint16_t vmax_raw;
    float vmax;
    uint8_t vmin_pack_num;
    uint8_t vmin_cell_num;
    uint16_t vmin_raw;
    float vmin;
    float vdiff;

} j1939_50_03_data;

//180450xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;


} j1939_50_04_data;

//180650xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_50_06_data;

//180750xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_50_07_data;

//0x18F15AF3
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_broadcast_F15A_data;

typedef struct
{
    uint32_t sa_t_max;
    uint32_t sa_t_min;
    float tmax_18;
    float tmin_18;
    float tavg_18;
    uint32_t sa_v_max;
    uint32_t sa_v_min;
    float vmax_18;
    float vmin_18;

} j1939_analy_data;


/// ---- BCU and BMU ----  ///
//1802E1xx
typedef struct
{
    uint32_t sa;
    uint16_t vsum_raw;
    uint16_t vmin_raw;
    uint16_t vmax_raw;
    uint8_t vmin_cell_num;
    uint8_t vmax_cell_num;

} j1939_E1_02_data;

//1803E1xx
typedef struct
{
    uint32_t sa;
    uint8_t tmin;
    uint8_t tmax;
    uint8_t tmin_cell_num;
    uint8_t tmax_cell_num;

} j1939_E1_03_data;

//1804E1xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_E1_04_data;

//1805E1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v1_raw;
    uint16_t cell_v2_raw;
    uint16_t cell_v3_raw;
    uint16_t cell_v4_raw;

} j1939_E1_05_data;

//1806E1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v5_raw;
    uint16_t cell_v6_raw;
    uint16_t cell_v7_raw;
    uint16_t cell_v8_raw;

} j1939_E1_06_data;

//1807E1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v9_raw;
    uint16_t cell_v10_raw;
    uint16_t cell_v11_raw;
    uint16_t cell_v12_raw;

} j1939_E1_07_data;

//1808E1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v13_raw;
    uint16_t cell_v14_raw;
    uint16_t cell_v15_raw;
    uint16_t cell_v16_raw;

} j1939_E1_08_data;

//1809E1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v17_raw;
    uint16_t cell_v18_raw;
    uint16_t cell_v19_raw;
    uint16_t cell_v20_raw;

} j1939_E1_09_data;

//180AE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v21_raw;
    uint16_t cell_v22_raw;
    uint16_t cell_v23_raw;
    uint16_t cell_v24_raw;

} j1939_E1_0A_data;

//180BE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v25_raw;
    uint16_t cell_v26_raw;
    uint16_t cell_v27_raw;
    uint16_t cell_v28_raw;

} j1939_E1_0B_data;

//180CE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v29_raw;
    uint16_t cell_v30_raw;
    uint16_t cell_v31_raw;
    uint16_t cell_v32_raw;

} j1939_E1_0C_data;

//180DE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v33_raw;
    uint16_t cell_v34_raw;
    uint16_t cell_v35_raw;
    uint16_t cell_v36_raw;

} j1939_E1_0D_data;

//180EE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v37_raw;
    uint16_t cell_v38_raw;
    uint16_t cell_v39_raw;
    uint16_t cell_v40_raw;

} j1939_E1_0E_data;

//180FE1xx
typedef struct
{
    uint32_t sa;
    uint16_t cell_v41_raw;
    uint16_t cell_v42_raw;

} j1939_E1_0F_data;

//1810E1xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_E1_10_data;

//1811E1xx
typedef struct
{
    uint32_t sa;
    uint8_t cell_t1;
    uint8_t cell_t2;
    uint8_t cell_t3;
    uint8_t cell_t4;
    uint8_t cell_t5;
    uint8_t cell_t6;
    uint8_t cell_t7;
    uint8_t cell_t8;

} j1939_E1_11_data;

//1812E1xx
typedef struct
{
    uint32_t sa;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t unk3;
    uint16_t unk4;

} j1939_E1_12_data;

#endif /* APPLICATIONS_COMMON_INCLUDE_J1939_STRUCT_H_ */
