/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "can.h"

#define CAN_THREAD_STACK_SIZE 4096
#define CAN_THREAD_PRIORITY   10
#define CAN_MQ_SIZE 2048
#define CAN_MQ_MSG_SIZE sizeof(can_receive_data)

//#define CAN_SEND_MODE 0
//#define CAN_RECEIVE_MODE 1

CAN_HandleTypeDef hcan1;
CAN_FilterTypeDef sFilterConfig;

queue *q_rev_data_to_analy = NULL;
struct rt_messagequeue can_mq;
static uint8_t can_mq_pool[CAN_MQ_SIZE * CAN_MQ_MSG_SIZE];

//static rt_mutex_t can_mutex;
static rt_sem_t can_sem;
int MODE;


void can_init(int mode)
{

    //init queue
    can_queue_init();
    rt_mq_init(&can_mq, "cam_mq", can_mq_pool, CAN_MQ_MSG_SIZE, sizeof(can_mq_pool), RT_IPC_FLAG_FIFO);

    if(mode == CAN_TEST_MODE)
    {
        MODE = mode;
        can_startup();
        return;
    }

    //CAN1 GPIO Configuration
    //PA11     ------> CAN1_RX
    //PA12     ------> CAN1_TX

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable GPIOA clock for CAN

    //CAN1 RX Pin CAN1 TX Pin
    GPIO_InitStruct.Pin = GPIO_PIN_11 |GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //Enable CAN1 clock
    __HAL_RCC_CAN1_CLK_ENABLE();

    //Configure CAN
    //baud rate = apb1/[prescaler*(syncsec+timesec1+timesec2)] set to 250k
    hcan1.Instance = CAN1;
    hcan1.Init.Prescaler = 4; //16
    hcan1.Init.Mode = CAN_MODE_NORMAL;
    hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan1.Init.TimeSeg1 = CAN_BS1_13TQ; //1
    hcan1.Init.TimeSeg2 = CAN_BS2_2TQ; //2
    hcan1.Init.TimeTriggeredMode = DISABLE;
    hcan1.Init.AutoBusOff = DISABLE;
    hcan1.Init.AutoWakeUp = DISABLE;
    hcan1.Init.AutoRetransmission = ENABLE;
    hcan1.Init.ReceiveFifoLocked = DISABLE;
    hcan1.Init.TransmitFifoPriority = DISABLE;

    if(HAL_CAN_Init(&hcan1) != HAL_OK)
    {
        // Initialization Error
        rt_kprintf("CAN Initialization Failed!\n");
        uint32_t error = HAL_CAN_GetError(&hcan1);
        rt_kprintf("CAN Init Error Code: %lu\n", error);
        return;
    }

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterIdHigh = 0x0000;  // Shifted by 5 bits for the CAN ID
    sFilterConfig.FilterMaskIdHigh = 0x0000;  // Mask to match exactly ID 0x1
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;

    if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
    {
        rt_kprintf("CAN filter configuration failed!\n");
    }

    if(HAL_CAN_Start(&hcan1) != HAL_OK)
    {
        // Start Error
        //uint32_t error = HAL_CAN_GetError(&hcan1);
        rt_kprintf("CAN Start Failed!\n");
        return;
    }

    // Enable the CAN1 RX0 interrupt in the NVIC
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

    if(mode == CAN_SEND_MODE)
    {
        // Enable CAN interrupts
        if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        {
            rt_kprintf("CAN Notification Activation Failed!\n");
        }
        MODE=mode;
        //can_startup();
    }
    else if(mode == CAN_RECEIVE_MODE)
    {
        // Enable CAN interrupts
        if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
        {
            rt_kprintf("CAN Notification Activation Failed!\n");
        }
        MODE=mode;
        //can_mutex = rt_mutex_create("can_mutex", RT_IPC_FLAG_FIFO);
        can_sem = rt_sem_create("can_sem", 0, RT_IPC_FLAG_FIFO);
        can_startup();
    }

}

void can_queue_init()
{
    q_rev_data_to_analy = malloc(sizeof(queue));
    if (q_rev_data_to_analy != NULL)
    {
        init_queue(q_rev_data_to_analy);
    }
}

void can_startup()
{
    rt_kprintf("RT-Thread CAN thread starting...\n");

    if(MODE == CAN_SEND_MODE)
    {
        rt_thread_t can_send_thread = rt_thread_create("can_thread",  // Thread name
                                                              can_send_loop, // Thread entry function
                                                              RT_NULL,     // Thread parameters
                                                              CAN_THREAD_STACK_SIZE, // Stack size
                                                              CAN_THREAD_PRIORITY,   // Thread priority
                                                              20);         // Time slice
        if (can_send_thread != RT_NULL)
        {
            rt_thread_startup(can_send_thread);  // Start the thread
        }
        else
        {
            rt_kprintf("Failed to create CAN thread\n");
        }
    }
    else if (MODE == CAN_RECEIVE_MODE)
    {
        rt_thread_t can_receive_thread = rt_thread_create("can_thread",  // Thread name
                                                              can_receive_loop, // Thread entry function
                                                              RT_NULL,     // Thread parameters
                                                              CAN_THREAD_STACK_SIZE, // Stack size
                                                              CAN_THREAD_PRIORITY,   // Thread priority
                                                              20);         // Time slice
        if (can_receive_thread != RT_NULL)
        {
            rt_thread_startup(can_receive_thread);  // Start the thread
        }
        else
        {
            rt_kprintf("Failed to create CAN thread\n");
        }
    }
    else if (MODE == CAN_TEST_MODE)
    {
        rt_thread_t can_test_thread = rt_thread_create("can_thread",  // Thread name
                                                              can_test_loop, // Thread entry function
                                                              RT_NULL,     // Thread parameters
                                                              CAN_THREAD_STACK_SIZE, // Stack size
                                                              CAN_THREAD_PRIORITY,   // Thread priority
                                                              20);         // Time slice
        if (can_test_thread != RT_NULL)
        {
            rt_thread_startup(can_test_thread);  // Start the thread
        }
        else
        {
            rt_kprintf("Failed to create CAN thread\n");
        }
    }


    // Start the scheduler (RT-Thread main loop)
    //rt_system_scheduler_start();
}

void can_send_loop(void *parameter)
{
    //test function send data
    uint8_t data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    while (1)
    {
        // Send a CAN message every second
        can_send(0x123, data, 8);
        rt_thread_mdelay(1000);
    }
}

void can_send(uint32_t id, uint8_t *data, uint8_t len)
{
    CAN_TxHeaderTypeDef tx_header;
    uint32_t tx_mailbox;

    tx_header.StdId = id;
    tx_header.ExtId = 0x01;
    tx_header.IDE = CAN_ID_STD;
    tx_header.RTR = CAN_RTR_DATA;
    tx_header.DLC = len;
    tx_header.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, data, &tx_mailbox) != HAL_OK)
    {
        rt_kprintf("CAN transmit failed\n");
    }
    else
    {
        // Wait until the message has been transmitted
        //while (HAL_CAN_IsTxMessagePending(&hcan1, tx_mailbox))
        //{
        //    // Optional: Add a timeout mechanism to prevent infinite looping
        //    rt_thread_mdelay(10);
        //}

        rt_kprintf("CAN message sent, ID: 0x%X\n", id);
    }
}

void can_receive_loop(void *parameter)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t rxData[8];
    while(1)
    {
        if(hcan1.Instance == CAN1)
        {
            if (rt_sem_take(can_sem, RT_WAITING_FOREVER) == RT_EOK)
            {
                if (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, rxData) == HAL_OK)
                {
                    can_receive_data *dataItem = malloc(sizeof(can_receive_data));
                    if (dataItem == NULL)
                    {
                        rt_kprintf("Memory allocation failed\n");
                        rt_thread_mdelay(1000);
                        continue;
                    }
                    dataItem->frame.can_id = RxHeader.ExtId;
                    memcpy(dataItem->frame.data, rxData, sizeof(rxData));

                    //if (!enqueue(q_rev_data_to_analy, *dataItem))
                    //{
                    //    free(dataItem);
                    //}

                    //mq
                    if(rt_mq_send(&can_mq, dataItem, sizeof(can_receive_data)) == RT_EOK) //&can_mq, (void*)&dataItem, sizeof(can_receive_data))
                    {
                        free(dataItem);
                    }
                    else
                    {
                        free(dataItem);
                    }

                    //rt_kprintf("Received CAN Message: ID=0x%X, Data=%02X %02X %02X %02X %02X %02X %02X %02X\n",
                                //RxHeader.ExtId, rxData[0], rxData[1], rxData[2], rxData[3],
                                //rxData[4], rxData[5], rxData[6], rxData[7]);
                    rt_thread_delay(100);
                }
                else
                {
                    //rt_kprintf("Received CAN Message: Failed");
                }
                __HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
            }
        }

    }
}

void can_receive(void)
{


}

void CAN1_RX0_IRQHandler(void)
{
    //rt_sem_release(can_sem);
    HAL_CAN_IRQHandler(&hcan1);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    rt_sem_release(can_sem);
    __HAL_CAN_DISABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

    //rt_kprintf("Received CAN CallBack \n");
    //CAN_RxHeaderTypeDef RxHeader;
    //uint8_t rxData[8];


    /*
    if(hcan->Instance == CAN1)
    {
        if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, rxData) == HAL_OK)
        {
            can_receive_data *dataItem = malloc(sizeof(can_receive_data));
            if (dataItem == NULL)
            {
                rt_kprintf("Memory allocation failed\n");
                return;
            }

            dataItem->frame.can_id = RxHeader.ExtId;
            memcpy(dataItem->frame.data, rxData, sizeof(rxData));
            if (!enqueue(q_rev_data_to_analy, *dataItem))
            {
                rt_kprintf("Add to queue failed\n");
            }

            // Process the received CAN message
            //rt_kprintf("Received CAN Message: ID=0x%X, Data=%02X %02X %02X %02X %02X %02X %02X %02X\n",
                        //RxHeader.ExtId, rxData[0], rxData[1], rxData[2], rxData[3],
                        //rxData[4], rxData[5], rxData[6], rxData[7]);
        }
        else
        {
            rt_kprintf("Received CAN Message: Failed");
        }
    }

*/
}

void can_test_loop(void *parameter)
{

    int min = 1, max = 10;
    while(1)
    {
        for(int i=0; i<=0x12;i++)
        {
            for(int j=0; j<=7;j++)
            {
                can_receive_data *dataItem = rt_malloc(sizeof(can_receive_data));
                if (dataItem == NULL)
                {
                    rt_kprintf("Memory allocation failed\n");
                    rt_thread_mdelay(1000);
                    continue;
                }

                int rd_num = rand() % (max-min+1)+min;

                if(j == 0)
                {
                    //E6B4
                    //DF66
                    //test small dbc
                    dataItem->frame.can_id=0x180250F1;
                    dataItem->frame.data[0] = 51;
                    dataItem->frame.data[1] = 134;

                    //negative [2] = 223 [3] = 102
                    //positive [2] = 25  [3] = 200
                    //rest     [2] = 0   [3] = 17
                    dataItem->frame.data[2] = 223;
                    dataItem->frame.data[3] = 102 + rd_num;
                    dataItem->frame.data[4] = 0;
                    dataItem->frame.data[5] = 0;
                    dataItem->frame.data[6] = 0;

                }
                else if (j == 1)
                {
                    //test j1939 parser
                    dataItem->frame.can_id=0x18015001 + i;
                    dataItem->frame.data[0] = 0x1;
                    dataItem->frame.data[1] = 0x18;
                    dataItem->frame.data[2] = 0x3;
                    dataItem->frame.data[3] = 0x16;
                    dataItem->frame.data[4] = 0x63;
                    dataItem->frame.data[5] = 0x64;
                    dataItem->frame.data[6] = 0;
                    dataItem->frame.data[7] = 0;
                }
                else if (j == 2)
                {
                    //test j1939 parser
                    //DF66 current negative
                    dataItem->frame.can_id=0x18025001 + i;
                    dataItem->frame.data[0] = 0x34;
                    dataItem->frame.data[1] = 0x36;
                    dataItem->frame.data[2] = 0xDF;
                    dataItem->frame.data[3] = 0x66;
                    dataItem->frame.data[4] = 0;
                    dataItem->frame.data[5] = 0x46;
                    dataItem->frame.data[6] = 0;
                    dataItem->frame.data[7] = 0x36;
                }
                else if (j == 3)
                {
                    //test j1939 parser
                    dataItem->frame.can_id=0x18035001 +i;
                    dataItem->frame.data[0] = 6;
                    dataItem->frame.data[1] = 0x2A;
                    dataItem->frame.data[2] = 0xF;
                    dataItem->frame.data[3] = 93;
                    dataItem->frame.data[4] = 2;
                    dataItem->frame.data[5] = 24;
                    dataItem->frame.data[6] = 0xF;
                    dataItem->frame.data[7] = 77;
                }
                else if (j == 4)
                {
                    //test j1939 parser
                    dataItem->frame.can_id=0x18045001 +i;
                    dataItem->frame.data[0] = 3;
                    dataItem->frame.data[1] = 0xE8;
                    dataItem->frame.data[2] = 3;
                    dataItem->frame.data[3] = 0xE8;
                    dataItem->frame.data[4] = 3;
                    dataItem->frame.data[5] = 0xE7;
                    dataItem->frame.data[6] = 0;
                    dataItem->frame.data[7] = 14;
                }
                else if (j == 6)
                {
                    //test j1939 parser
                    dataItem->frame.can_id=0x18035001+i;
                    dataItem->frame.data[0] = 0;
                    dataItem->frame.data[1] = 1;
                    dataItem->frame.data[2] = 0;
                    dataItem->frame.data[3] = 0;
                    dataItem->frame.data[4] = 2;
                    dataItem->frame.data[5] = 0;
                    dataItem->frame.data[6] = 0x3C;
                    dataItem->frame.data[7] = 0xBB;
                }
                else if (j == 7)
                {
                    //test j1939 parser
                    dataItem->frame.can_id=0x18035001+i;
                    dataItem->frame.data[0] = 0;
                    dataItem->frame.data[1] = 0;
                    dataItem->frame.data[2] = 0;
                    dataItem->frame.data[3] = 0;
                    dataItem->frame.data[4] = 0;
                    dataItem->frame.data[5] = 0;
                    dataItem->frame.data[6] = 0;
                    dataItem->frame.data[7] = 0x1B;
                }

                if(rt_mq_send(&can_mq, dataItem, sizeof(can_receive_data)) == RT_EOK)
                {
                    rt_free(dataItem);
                }
                else
                {
                    rt_free(dataItem);
                }
                rt_thread_delay(100);
            }
        }

    }
}
