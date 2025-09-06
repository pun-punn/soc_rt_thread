/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "uart.h"

#define UART_DEVICE_NAME "uart4"
#define BUFFER_SIZE 64

UART_HandleTypeDef huart2;
static rt_device_t uart_device;
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

static volatile bool transmission_complete = false;
//static struct rt_semaphore tx_complete_sem;
static rt_sem_t tx_sem = RT_NULL;
static rt_sem_t rx_sem = RT_NULL;

void uart_init_hal(void)
{

    // Initialize USART2
    __HAL_RCC_USART2_CLK_ENABLE(); // Enable clock for USART2

    huart2.Instance = USART2; // Specify which UART instance to use
    huart2.Init.BaudRate = 115200; // Set baud rate
    huart2.Init.WordLength = UART_WORDLENGTH_8B; // 8 data bits
    huart2.Init.StopBits = UART_STOPBITS_1; // 1 stop bit
    huart2.Init.Parity = UART_PARITY_NONE; // No parity
    huart2.Init.Mode = UART_MODE_TX_RX; // Transmit and receive mode
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE; // No hardware flow control
    huart2.Init.OverSampling = UART_OVERSAMPLING_16; // Over-sampling by 16

    // Initialize the UART
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        rt_kprintf("UART initialization failed!\n");
    }

    // Enable the UART interrupt
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

}

bool uart_init_rt(void)
{
    //check device is exist
    uart_device = rt_device_find(UART_DEVICE_NAME);
    if (!uart_device)
    {
        rt_kprintf("Find %s failed!\n", UART_DEVICE_NAME);
        return false;
    }

    //configure uart
    config.baud_rate = 9600;
    config.data_bits = DATA_BITS_8;
    config.stop_bits = STOP_BITS_1;
    config.parity = PARITY_NONE;
    //config deive
    rt_err_t ret = rt_device_control(uart_device, RT_DEVICE_CTRL_CONFIG, &config);
    if (ret != RT_EOK)
    {
        rt_kprintf("Failed to configure %s! Error code: %d\n", UART_DEVICE_NAME, ret);
        rt_device_close(uart_device);
        return false;
    }

    //open device
    //FLAG
    //RT_DEVICE_FLAG_INT_RX  >> Interrupt receive
    //RT_DEVICE_FLAG_INT_TX  >> Interrupt send
    //RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STREAM
    ret = rt_device_open(uart_device, RT_DEVICE_FLAG_INT_RX);
    if(ret != RT_EOK)
    {
        rt_kprintf("Failed to open %s! Error code: %d\n", UART_DEVICE_NAME, ret);
        return false;
    }

    //There are 2 options to init semaphore
    //1.rt_sem_init >> rt_sem_init(&tx_complete_sem, "tx_done", 0, RT_IPC_FLAG_FIFO)
    //2.rt_sem_create >> tx_sem = rt_sem_create("tx_sem", 0, RT_IPC_FLAG_FIFO)

    //rx interrupt
    rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);
    if (rx_sem == RT_NULL) // Check if semaphore creation was successful
    {
        rt_kprintf("Failed to create rx_sem semaphore!\n");
        rt_device_close(uart_device);
        return false;
    }

    ret = rt_device_set_rx_indicate(uart_device, rx_done);
    if (ret != RT_EOK)
    {
        rt_kprintf("Failed to register rx_done callback for %s! Error code: %d\n", UART_DEVICE_NAME, ret);
        rt_device_close(uart_device);
        return false;
    }
    return true;

}

void uart_send_hal(uint8_t *pData, uint16_t Size)
{
    // Send data via HAL UART
    //for test
    uint8_t Data[] = "Hello, RT-Thread\n";
    rt_size_t len = sizeof(pData); // Size of the data buffer.
    if (HAL_UART_Transmit(&huart2, Data, len,100) != HAL_OK)
    {
        // Transmission Error
        //Error_Handler();
    }

    // Add delay to ensure the data has been sent before switching to receive mode
    HAL_Delay(100);
}

rt_err_t tx_done(rt_device_t dev, void *buffer)
{
    rt_kprintf("tx_done callback triggered for device: %s\n", dev->parent.name);
    rt_sem_release(tx_sem);
    rt_kprintf("Transmission complete for device: %s\n", dev->parent.name);
    return RT_EOK;
}

void uart_send_rt(uint8_t *pData, uint16_t Size)
{
    if(uart_device)
    {

        rt_err_t result = rt_device_write(uart_device, 0, pData, Size);

        // Wait for transmission to complete
        //if (rt_sem_take(tx_sem, RT_WAITING_FOREVER) != RT_EOK)
        //{
        //    rt_kprintf("Timeout waiting for transmission complete");
        //}

        // Check if the write was successful
        if (result != Size) // Check if one byte was sent
        {
            rt_kprintf("Failed to send result:  %d\n", result);
        }

        //rt_kprintf("Send data size:  %d\n", result);


        /*
        // Check for NULL data pointer or zero size
        if (pData == NULL || Size == 0)
        {
            rt_kprintf("Invalid data or size.\n");
            return;
        }

        for(uint16_t i = 0; i < Size; i++)
        {
            // Send one byte at a time
            rt_err_t result = rt_device_write(uart_device, 0, &pData[i], 1);

            // Wait for transmission to complete
            if (rt_sem_take(tx_sem, RT_WAITING_FOREVER) != RT_EOK)
            {
                rt_kprintf("Timeout waiting for transmission complete for byte %d!\n", i);
                break; // Break the loop on timeout
            }

            // Check if the write was successful
            if (result != 1) // Check if one byte was sent
            {
                rt_kprintf("Failed to send byte %d! Result: %d\n", i, result);
            }
        }
        */

    }

}

void uart_receive_hal(uint8_t *pData, uint16_t Size)
{
    // Receive data via HAL UART
    if (HAL_UART_Receive(&huart2, pData, Size, 1000) != HAL_OK)
    {
        // Reception Error
        //Error_Handler();
    }
}

void uart_receive_rt(uint8_t *pData, uint16_t Size)
{

        rt_size_t result = rt_device_read(uart_device, 0, pData, Size);

        if(rt_sem_take(rx_sem, RT_WAITING_FOREVER) == RT_EOK)
        {

        }

        // Check if the read operation was successful
        if (result < 0)
        {
            rt_kprintf("Failed to read from UART! Error code: %d\n", result);
        }
        else if (result < Size)
        {
            //rt_kprintf("Warning: Only %d bytes read from UART! Expected: %d\n", result, Size);
        }

        rt_kprintf("Data receive %d bytes \n", result);


}

rt_err_t rx_done(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(rx_sem);
    //rt_kprintf("Receive complete for device: %s\n", dev->parent.name);
    return RT_EOK;
}

bool is_transmission_complete(void)
{
    return transmission_complete;
}

void reset_transmission_complete(void)
{
    transmission_complete = false;  // Reset the flag for the next transmission
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    rt_kprintf("HAL_UART_TxCpltCallback: completed \n");
}


