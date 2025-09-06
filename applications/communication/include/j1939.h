/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_J1939_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_J1939_H_

#include "queue.h"
#include "list.h"
#include "dictionary.h"
#include "can.h"
#include "can_frame_struct.h"
#include "dbc_struct.h"
#include "my_string.h"
#include "data_analy.h"
#include <stdio.h>
#include "j1939_struct.h"
#include <rtthread.h>

#define PDU_NUM 0x12
#define BMU_NUM 8
#define PDU_ENABLED

extern can_receive_data_list list_dev_obj;
extern dbc_list list_can_info;
extern dictionary bms_cananly_info;

extern j1939_50_01_data data_5001[PDU_NUM];
extern j1939_50_02_data data_5002[PDU_NUM];
extern j1939_50_03_data data_5003[PDU_NUM];
extern j1939_50_04_data data_5004[PDU_NUM];
extern j1939_50_06_data data_5006[PDU_NUM];
extern j1939_50_07_data data_5007[PDU_NUM];

void j1939_init();
void j1939_init_bms_dic();
void j1939_init_dbc_list();
void j1939_dbc_verify();
void j1939_compile();
void j1939_decomplie(can_receive_data recv);
void j1939_analy(void *parameter);
void j1939_startup();

//BMS-BCU
void handle_pf_501(uint8_t source_address, can_receive_data recv);
void handle_pf_502(uint8_t source_address, can_receive_data recv);
void handle_pf_503(uint8_t source_address, can_receive_data recv);
void handle_pf_504(uint8_t source_address, can_receive_data recv);
void handle_pf_506(uint8_t source_address, can_receive_data recv);
void handle_pf_507(uint8_t source_address, can_receive_data recv);

//BCU-BMU
void handle_pf_e12(uint8_t source_address, can_receive_data recv);
void handle_pf_e13(uint8_t source_address, can_receive_data recv);
void handle_pf_e14(uint8_t source_address, can_receive_data recv);
void handle_pf_e16(uint8_t source_address, can_receive_data recv);
void handle_pf_e17(uint8_t source_address, can_receive_data recv);
void handle_pf_e18(uint8_t source_address, can_receive_data recv);
void handle_pf_e19(uint8_t source_address, can_receive_data recv);
void handle_pf_e1a(uint8_t source_address, can_receive_data recv);
void handle_pf_e1b(uint8_t source_address, can_receive_data recv);
void handle_pf_e1c(uint8_t source_address, can_receive_data recv);
void handle_pf_e1d(uint8_t source_address, can_receive_data recv);
void handle_pf_e1e(uint8_t source_address, can_receive_data recv);
void handle_pf_e1f(uint8_t source_address, can_receive_data recv);
void handle_pf_e110(uint8_t source_address, can_receive_data recv);
void handle_pf_e111(uint8_t source_address, can_receive_data recv);
void handle_pf_e112(uint8_t source_address, can_receive_data recv);

#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_J1939_H_ */
