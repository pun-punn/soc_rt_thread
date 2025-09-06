/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "modbus.h"

#define SLAVE_ID 0x01
#define MODBUS_RX_BUFFER_SIZE 32
//static struct rt_semaphore rx_sem;

uint16_t modbus_crc16(uint8_t *buffer, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for (int pos = 0; pos < length; pos++)
    {
        crc ^= buffer[pos]; // XOR byte into least significant byte of CRC
        for (int i = 8; i != 0; i--)
        {
            if ((crc & 0x0001) != 0)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void modbus_send(uint8_t slave_id, uint8_t function_code, uint16_t start_addr, uint16_t num_regs)
{
    // Send:01 04 00 00 00 08 F1 CC
    uint8_t request[8];
    uint16_t crc;

    request[0] = slave_id;
    request[1] = function_code;
    request[2] = (start_addr >> 8) & 0xFF;
    request[3] = start_addr & 0xFF;
    request[4] = (num_regs >> 8) & 0xFF;
    request[5] = num_regs & 0xFF;

    crc = modbus_crc16(request, 6);
    request[6] = crc & 0xFF;
    request[7] = (crc >> 8) & 0xFF;


    // Print the Modbus request in hex format
    rt_kprintf("Send request: ");
    for (int i = 0; i < sizeof(request); i++)
    {
        rt_kprintf("%02X ", request[i]);  // Print each byte in hexadecimal format
    }
    rt_kprintf("\n");  // Add a newline at the end

    rs485_send(request, sizeof(request));
}

static void modbus_send_receive_thread(void *parameter)
{
    rt_kprintf("Thread modbus_send_receive_thread start....\n");
    uint8_t rx_buffer[MODBUS_RX_BUFFER_SIZE];
    //t_size_t length;

    for(int i=0; i<sizeof(rx_buffer);i++)
    {
        rx_buffer[i] = 0;
    }

    while(1)
    {
        // id=1 code=4, addrress=0, len=8
        modbus_send(SLAVE_ID, 0x04, 0x0000, 0x0008);

        // Wait for data reception (block until data is available)
        //rt_sem_take(&rx_sem, RT_WAITING_FOREVER);

        rt_thread_mdelay(1000);

        rs485_receive(rx_buffer, sizeof(rx_buffer));

        //check data length, crc

        //print some data for test
        //uint8_t byte_count = rx_buffer[2];
        for (int i = 0; i < sizeof(rx_buffer); i++)
        {
            rt_kprintf("%02X ", rx_buffer[i]);
        }
        rt_kprintf("\n");

        rt_thread_mdelay(2000);
    }
}

void modbus_startup()
{
    // Initialize semaphore for RX indication
    //rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    // Create Modbus send/receive thread
    rt_thread_t modbus_thread = rt_thread_create("modbus_thread", modbus_send_receive_thread, RT_NULL, 1024, 10, 10);
    if (modbus_thread != RT_NULL)
    {
        rt_thread_startup(modbus_thread);
    }
}
