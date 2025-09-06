/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-08     sippakorn.s       the first version
 */

#include "j1939.h"

#define J1939_THREAD_STACK_SIZE 8192
#define J1939_THREAD_PRIORITY   10

j1939_50_01_data data_5001[PDU_NUM];
j1939_50_02_data data_5002[PDU_NUM];
j1939_50_03_data data_5003[PDU_NUM];
j1939_50_04_data data_5004[PDU_NUM];
j1939_50_06_data data_5006[PDU_NUM];
j1939_50_07_data data_5007[PDU_NUM];

j1939_E1_02_data data_E102[BMU_NUM];
j1939_E1_03_data data_E103[BMU_NUM];
j1939_E1_04_data data_E104[BMU_NUM];
j1939_E1_05_data data_E105[BMU_NUM];
j1939_E1_06_data data_E106[BMU_NUM];
j1939_E1_07_data data_E107[BMU_NUM];
j1939_E1_08_data data_E108[BMU_NUM];
j1939_E1_09_data data_E109[BMU_NUM];
j1939_E1_0A_data data_E10A[BMU_NUM];
j1939_E1_0B_data data_E10B[BMU_NUM];
j1939_E1_0C_data data_E10C[BMU_NUM];
j1939_E1_0D_data data_E10D[BMU_NUM];
j1939_E1_0E_data data_E10E[BMU_NUM];
j1939_E1_0F_data data_E10F[BMU_NUM];
j1939_E1_10_data data_E110[BMU_NUM];
j1939_E1_11_data data_E111[BMU_NUM];
j1939_E1_12_data data_E112[BMU_NUM];


can_receive_data_list list_dev_obj;
dbc_list list_can_info;
dictionary bms_cananly_info;


void j1939_init()
{
    j1939_init_dbc_list();
    can_receive_data_list_init(&list_dev_obj);
    j1939_init_bms_dic();

    j1939_startup();
}

void j1939_init_bms_dic()
{
    bms_cananly_info.count = 0;
    //dictionary_add_update(&bms_cananly_info, "BATTotalCurrent", 0.01);

}

void j1939_init_dbc_list()
{
    //small dbc implement
    dbc_list_info_init(&list_can_info);

    //Main data to calculate soc
    dbc_info dbcs[6] = {
        { .id = 0x180150F1 },
        { .id = 0x180250F1 },
        { .id = 0x180350F1 },
        { .id = 0x180450F1 },
        { .id = 0x180650F1 },
        { .id = 0x180750F1 }
    };

    int count = sizeof(dbcs) / sizeof(dbcs[0]);
    for (int i = 0; i < count; i++)
    {
        dbc_list_data_init(&dbcs[i], 2);
    }

    //add data 0x180150F1
    dbc_data data1 = {my_strdup("CellVoltHigh"),0, 0, 16, 0.001, 0.0};
    dbc_data data2 = {my_strdup("CellVoltLow"),0, 16, 16, 0.001, 0.0};
    dbc_data data3 = {my_strdup("SOC"),0, 32, 8, 1.00, 0.0};
    dbc_data data4 = {my_strdup("SOH"),0, 40, 8, 1.00, 0.0};
    dbc_list_data_add(&dbcs[0], data1);
    dbc_list_data_add(&dbcs[0], data2);
    dbc_list_data_add(&dbcs[0], data3);
    dbc_list_data_add(&dbcs[0], data4);

    //add data 0x180250F1
    dbc_data data5 = {my_strdup("SumVolt"),0, 0, 16, 0.1, 0.0};
    dbc_data data6 = {my_strdup("SumCurr"),1, 16, 16, 0.1, 0.0};
    dbc_list_data_add(&dbcs[1], data5);
    dbc_list_data_add(&dbcs[1], data6);

    //add data 0x180350F1
    dbc_data data7 = {my_strdup("CellTempHigh"),0, 40, 8, 1.00, 0.0};
    dbc_list_data_add(&dbcs[2], data7);

    //add data 0x180450F1
    dbc_data data8 = {my_strdup("CellTempLow"),0, 40, 8, 1.00, 0.0};
    dbc_list_data_add(&dbcs[3], data8);

    //add data 0x180650F1
    dbc_data data9 = {my_strdup("BATStatus"),0, 0, 8, 1.00, 0.0};
    dbc_data data10 = {my_strdup("SYSStatus"),0, 8, 8, 1.00, 0.0};
    dbc_list_data_add(&dbcs[4], data9);
    dbc_list_data_add(&dbcs[4], data10);

    //add data 0x180750F1
    dbc_data data11 = {my_strdup("LevelII_1"),0, 0, 8, 1.00, 0.0};
    dbc_data data12 = {my_strdup("LevelII_2"),0, 8, 8, 1.00, 0.0};
    dbc_list_data_add(&dbcs[5], data11);
    dbc_list_data_add(&dbcs[5], data12);

    //add info to list
    for (int i = 0; i < count; i++)
    {
        dbc_list_info_add(&list_can_info, dbcs[i]);
    }

    /*
    for (int i = 0; i < count; i++)
    {
        rt_kprintf("DBC ID: 0x%X\n", dbcs[i].id);
        for (size_t j = 0; j < dbcs[i].data_count; j++)
        {
            printf("Data: %s, BegBit: %u, BitNumber: %u, Coeff: %.3f, Offset: %.3f\n",
                   dbcs[i].list_data[j].name, dbcs[i].list_data[j].begbit,
                   dbcs[i].list_data[j].bitnumber, dbcs[i].list_data[j].coeff, dbcs[i].list_data[j].offset);
        }
    }
   */


    //each PDU (1-18)
    for(int j=0; j<PDU_NUM; j++) //there are 0x12 sa = (0x01-0x12)
    {
        //add source address into sa for each pdu
        data_5001[j].sa = j+1;
        data_5002[j].sa = j+1;
        data_5003[j].sa = j+1;
        data_5004[j].sa = j+1;
        data_5006[j].sa = j+1;
        data_5007[j].sa = j+1;


        //init data 01
        data_5001[j].tmax_pack_num = 0;
        data_5001[j].tmax = 0;
        data_5001[j].tmin_pack_num = 0;
        data_5001[j].tmin = 0;
        data_5001[j].soc = 0;
        data_5001[j].soh = 0;

        //init data 02
        data_5002[j].vol = 0;
        data_5002[j].curr = 0;
        data_5002[j].vol_raw = 0;
        data_5002[j].curr_raw = 0;

        //init data 03
        data_5003[j].vmax_pack_num = 0;
        data_5003[j].vmax_cell_num = 0;
        data_5003[j].vmax = 0;
        data_5003[j].vmin_pack_num = 0;
        data_5003[j].vmin_cell_num = 0;
        data_5003[j].vmin = 0;

        //init data 04
        data_5004[j].unk1 = 0;
        data_5004[j].unk2 = 0;
        data_5004[j].unk3 = 0;
        data_5004[j].unk4 = 0;

        //init data 06
        data_5006[j].unk1 = 0;
        data_5006[j].unk2 = 0;
        data_5006[j].unk3 = 0;
        data_5006[j].unk4 = 0;

        //init data 07
        data_5007[j].unk1 = 0;
        data_5007[j].unk2 = 0;
        data_5007[j].unk3 = 0;
        data_5007[j].unk4 = 0;

    }

    j1939_dbc_verify();

}

void j1939_dbc_verify()
{
    size_t t = 2;
    dbc_info *dbc = dbc_list_get(&list_can_info, t);

    if (dbc != NULL)
    {
        rt_kprintf("DBC ID: 0x%X\n", dbc->id);
    } else
    {
        rt_kprintf("DBC not found.\n");
    }
}

void j1939_startup()
{
    rt_kprintf("RT-Thread J1939 thread starting...\n");

    rt_thread_t j1939_thread = rt_thread_create("j1939_thread",  // Thread name
                                                 j1939_analy, // Thread entry function
                                                 RT_NULL,     // Thread parameters
                                                 J1939_THREAD_STACK_SIZE, // Stack size
                                                 J1939_THREAD_PRIORITY,   // Thread priority
                                                 20);         // Time slice
    if (j1939_thread != RT_NULL)
    {
        rt_thread_startup(j1939_thread);  // Start the thread
    }

}

void j1939_analy(void *parameter)
{
    while (1)
    {
        //clear data in list
        can_receive_data_list_clear(&list_dev_obj);

        //rt_err_t result;
        can_receive_data received_data;
        //rt_uint32_t message_count;

        //test mq
        //rt_mq_control(&can_mq, RT_IPC_CMD, &message_count);
        rt_mq_recv(&can_mq, &received_data, sizeof(received_data), RT_WAITING_FOREVER);
        j1939_decomplie(received_data);

        //if(message_count >= 0)
        //{
        //    rt_mq_recv(&can_mq, &received_data, sizeof(received_data), RT_WAITING_FOREVER);
        //}

        /*
        if(!is_queue_empty(q_rev_data_to_analy))
        {
            //rt_mutex_take(q_rev_data_to_analy->lock, RT_WAITING_FOREVER);
            pthread_mutex_lock(&q_rev_data_to_analy->lock);
            for (int t = 0; t < queue_count(q_rev_data_to_analy); t++)
            {
                can_receive_data_list_add(&list_dev_obj, dequeue(q_rev_data_to_analy));
            }
            pthread_mutex_unlock(&q_rev_data_to_analy->lock);
            //rt_mutex_release(q_rev_data_to_analy->lock);

            if(list_dev_obj.size > 0)
            {
                for(size_t k = 0; k < can_receive_data_list_count(&list_dev_obj); k++)
                {
                    //parser can id and data
                    j1939_decomplie(can_receive_data_list_get(&list_dev_obj,k));
                }
            }


        }
        else //when there is no data in q_rev_data_to_analy
        {
            //want to update bms_cananly_info for all key = 0
            for(int i=0; i<bms_cananly_info.count; i++)
            {
                //bms_cananly_info[i].entries.value = 0;
            }
        }
        */

    }

}

typedef void (*ps50_handlers_t)(uint8_t source_address, can_receive_data recv);
ps50_handlers_t ps50_handlers[] = {
    NULL,            // pf0 (not used)
    handle_pf_501,     // pf1 data_5001
    handle_pf_502,     // pf2 data_5002
    handle_pf_503,     // pf3 data_5003
    handle_pf_504,     // pf4 data_5004
    NULL,            // pf5 (not used)
    handle_pf_506,     // pf6 data_5006
    handle_pf_507      // pf7 data_5007
};

typedef void (*psE1_handlers_t)(uint8_t source_address, can_receive_data recv);
psE1_handlers_t psE1_handlers[] = {
    NULL,              // pf0 (not used)
    NULL,              // pf1 (not used)
    handle_pf_e12,     // pf2 data_E102
    handle_pf_e13,     // pf3 data_E103
    handle_pf_e14,     // pf4 data_E104
    handle_pf_e15,     // pf5 data_E105
    handle_pf_e16,     // pf6 data_E106
    handle_pf_e17,     // pf7 data_E107
    handle_pf_e18,     // pf8 data_E108
    handle_pf_e19,     // pf9 data_E109
    handle_pf_e1a,     // pfa data_E10A
    handle_pf_e1b,     // pfb data_E10B
    handle_pf_e1c,     // pfc data_E10C
    handle_pf_e1d,     // pfd data_E10D
    handle_pf_e1e,     // pfe data_E10E
    handle_pf_e1f,     // pff data_E10F
    handle_pf_e110,    // pf10 data_E110
    handle_pf_e111,    // pf11 data_E111
    handle_pf_e112,    // pf12 data_E112
};

void j1939_decomplie(can_receive_data recv)
{
    // the recv id is the id in small dbc or not
    if(recv.frame.can_id == 0x180150F1 ||
       recv.frame.can_id == 0x180250F1 ||
       recv.frame.can_id == 0x180350F1 ||
       recv.frame.can_id == 0x180450F1 ||
       recv.frame.can_id == 0x180650F1 ||
       recv.frame.can_id == 0x180750F1)
    {

        // add data in bms_cananly_info using small dbc used for calculation
        if(dbc_list_count(&list_can_info) > 0)
        {
            for(int i=0; i<dbc_list_count(&list_can_info); i++)
            {
                if(recv.frame.can_id == dbc_list_get(&list_can_info, i)->id)
                {
                    for(int j=0; j<dbc_list_get(&list_can_info, i)->data_count; j++)
                    {

                        double ret_data = 0.00;
                        ret_data =   extract_data(recv.frame.data,
                                     dbc_list_get(&list_can_info, i)->list_data[j].begbit,
                                     dbc_list_get(&list_can_info, i)->list_data[j].bitnumber,
                                     dbc_list_get(&list_can_info, i)->list_data[j].coeff,
                                     dbc_list_get(&list_can_info, i)->list_data[j].offset,
                                     dbc_list_get(&list_can_info, i)->list_data[j].issigned);

                        //printf("CAN id: 0x%lX, Name: %s, Value: %f\n",recv.frame.can_id,dbc_list_get(&list_can_info, i)->list_data[j].name ,ret_data);

                        //add to BMS dictionary
                        //using bms_cananly_info to other function such as soc
                        dictionary_add_update(&bms_cananly_info, dbc_list_get(&list_can_info, i)->list_data[j].name, ret_data);

                    }
                }
            }
        }
    }
    else
    {

        #ifdef PDU_ENABLED
        //j1939 if pf <  240 the ps is destination address which is point to point communication
        //         pf >= 240 the ps is becomes a part of pgn which is broadcast message
        //               it mean that pf start at F0 for broadcast
        //line1 18 power distribution unit: extract j1939 for each pdu (pf [group] = (01,02,03,04,06,07), ps [destination] = 50, sa [source] = (01-12) 18 ids)
        //line2

        // Extract fields from the 29-bit CAN ID
        //uint8_t priority = (recv.frame.can_id >> 26) & 0x07;       // 3 bits for Priority
        //uint8_t reserved = (recv.frame.can_id >> 25) & 0x01;       // 1 bit for Reserved
        //uint8_t data_page = (recv.frame.can_id >> 24) & 0x01;      // 1 bit for Data Page (DP)
        uint8_t pdu_format = (recv.frame.can_id >> 16) & 0xFF;     // 8 bits for PDU Format (PF)
        uint8_t pdu_specific = (recv.frame.can_id >> 8) & 0xFF;    // 8 bits for PDU Specific (PS)
        uint8_t source_address = recv.frame.can_id & 0xFF;         // 8 bits for Source Address (SA)

        //uint32_t pgn;
        if (pdu_format >= 240)
        {
            // Broadcast message: PGN is PF only
        }
        else
        {
            // Point-to-point message: PGN is PF and PS
            if (pdu_specific == 0x50 && pdu_format < sizeof(ps50_handlers) / sizeof(ps50_handlers[0])) //ps=50 BMS <-> BCU
            {
                if (ps50_handlers[pdu_format])
                {
                    ps50_handlers[pdu_format](source_address, recv);
                }
            }
            else if(pdu_specific == 0xE1) //ps=E1 BCU <-> BMU
            {
                if(psE1_handlers[pdu_format])
                {
                    psE1_handlers[pdu_format](source_address, recv);
                }
            }
            else if(pdu_specific == 0xF1) //handler other ps
            {

            }

        }
        #endif

    }

}

/// ---- BMS <-> BCU ---- ///
void handle_pf_501(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        data_5001[source_address-1].tmax_pack_num = recv.frame.data[0];
        data_5001[source_address-1].tmax = recv.frame.data[1];
        data_5001[source_address-1].tmin_pack_num = recv.frame.data[2];
        data_5001[source_address-1].tmin = recv.frame.data[3];
        data_5001[source_address-1].soc = recv.frame.data[4];
        data_5001[source_address-1].soh = recv.frame.data[5];
        data_5001[source_address-1].tdiff = data_5001[source_address-1].tmax - data_5001[source_address-1].tmin;
    }
}

void handle_pf_502(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        data_5002[source_address-1].vol = ((recv.frame.data[0]<<8) + recv.frame.data[1])*0.1;
        data_5002[source_address-1].curr = (((recv.frame.data[2] << 8) + recv.frame.data[3]) & (1 << 15)) ?
                                           (((recv.frame.data[2] << 8) + recv.frame.data[3]) - (1 << 16))*0.1 :
                                           (((recv.frame.data[2] << 8) + recv.frame.data[3]))*0.1;
        data_5002[source_address-1].vol_raw =  ((recv.frame.data[0]<<8) + recv.frame.data[1]);
        data_5002[source_address-1].curr_raw = ((recv.frame.data[2] << 8) + recv.frame.data[3]);
    }
}

void handle_pf_503(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        data_5003[source_address-1].vmax_pack_num = recv.frame.data[0];
        data_5003[source_address-1].vmax_cell_num = recv.frame.data[1];
        data_5003[source_address-1].vmax = ((recv.frame.data[2]<<8) + recv.frame.data[3])*0.001;
        data_5003[source_address-1].vmin_pack_num = recv.frame.data[4];
        data_5003[source_address-1].vmin_cell_num = recv.frame.data[5];
        data_5003[source_address-1].vmin = ((recv.frame.data[6]<<8) + recv.frame.data[7])*0.001;
    }
}

void handle_pf_504(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        //unknown data
    }
}

void handle_pf_506(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        //unknown data
    }
}

void handle_pf_507(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < PDU_NUM)
    {
        //unknown data
    }
}

/// ---- BCU <-> BMU ---- ///
void handle_pf_e12(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e13(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e14(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e16(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e17(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e18(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e19(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1a(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1b(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1c(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1d(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1e(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e1f(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e110(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e111(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
void handle_pf_e112(uint8_t source_address, can_receive_data recv)
{
    if (source_address >= 1 && source_address < BMU_NUM)
    {

    }
}
