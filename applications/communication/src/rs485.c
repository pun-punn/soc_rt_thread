/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */

#include "rs485.h"
#include "uart.h"
#define RS485_DE_PIN GPIO_PIN_4  // DE on PA4
#define RS485_RE_PIN GPIO_PIN_5  // RE on PA5
#define RS485_GPIO_PORT GPIOA       // Change this according to your GPIO port
#define RS485_RECEIVE_THREAD_STACK_SIZE 1024
#define RS485_RECEIVE_THREAD_PRIORITY   10

#define RS485_TRANS_MODE 0
#define RS485_RECV_MODE 1


//static struct rt_device *uart_device;

void rs485_gpio_control_init(void)
{
    // Enable GPIO clock for Port A
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    // Configure PA3 (RX), PA2 (TX) for UART and PA0/PA1 for DE/RE
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure DE/RE pin
    GPIO_InitStruct.Pin = RS485_DE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS485_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_RE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS485_GPIO_PORT, &GPIO_InitStruct);

    // Configure PA3 (RX) and PA2 (TX) as Alternate Function
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3; // PA2 (DI_TX) and PA3 (RO_RX)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Alternate Function Push-Pull
    GPIO_InitStruct.Pull = GPIO_PULLUP; // No pull-up or pull-down
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // Fast speed
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2; // Alternate function for USART2
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void rs485_init(void)
{
    //rs485_gpio_control_init();
    uart_init_rt(); //uart4 BSP_UART4_TX_PIN PC10 and BSP_UART4_RX_PIN PC11
    //uart_init_hal();
    //HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_DE_PIN, GPIO_PIN_RESET); // Set DE/RE low for receiving
    //HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
}

void rs485_mode(int mode)
{
    if(mode == 0) //send
    {
        HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_DE_PIN, GPIO_PIN_SET);

        HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_RE_PIN, GPIO_PIN_SET);
        rs485_de_state();
        rs485_re_state();
        rt_thread_mdelay(5);
    }
    else if(mode == 1) //recv
    {
        HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_GPIO_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
        rs485_de_state();
        rs485_re_state();
        rt_thread_mdelay(5);
    }
}

void rs485_de_state()
{
    GPIO_PinState deState = HAL_GPIO_ReadPin(RS485_GPIO_PORT, RS485_DE_PIN); // Read the state of DE pin

    if (deState == GPIO_PIN_SET)
    {
        rt_kprintf("DE pin is HIGH\n"); // DE is enabled (transmit mode)
    }
    else
    {
        rt_kprintf("DE pin is LOW\n"); // DE is disabled (receive mode)
    }
}

void rs485_re_state()
{
    GPIO_PinState reState = HAL_GPIO_ReadPin(RS485_GPIO_PORT, RS485_RE_PIN); // Read the state of RE pin

    if (reState == GPIO_PIN_SET)
    {
        rt_kprintf("RE pin is HIGH\n"); // DE is enabled (transmit mode)
    }
    else
    {
        rt_kprintf("RE pin is LOW\n"); // DE is disabled (receive mode)
    }
}

void rs485_receive_loop(void)
{

    rt_kprintf("RT-Thread RS485 starting...\n");

    rt_thread_t rs485_receive_thread = rt_thread_create("rs485_sendrecv",  // Thread name
                                                         rs485_send_receive, // Thread entry function
                                                         RT_NULL,     // Thread parameters
                                                         RS485_RECEIVE_THREAD_STACK_SIZE, // Stack size
                                                         RS485_RECEIVE_THREAD_PRIORITY,   // Thread priority
                                                         20);         // Time slice
    if (rs485_receive_thread != RT_NULL)
    {
        rt_thread_startup(rs485_receive_thread);  // Start the thread
    }
    else
    {
        rt_kprintf("Failed to create RS485 send receive thread\n");
    }
}

void rs485_send_receive(void *parameter)
{
    while(1)
    {

    }

}

void rs485_send(uint8_t *pData, uint16_t Size)
{
    // Set to transmit mode
    //rs485_mode(RS485_TRANS_MODE);

    uart_send_rt(pData, Size);
    //uart_send_hal(pData, Size);

    // Switch back to receive mode
    //rs485_mode(RS485_RECV_MODE);
}

void rs485_receive(uint8_t *pData, uint16_t Size)
{
    // Set to receive mode
    //rs485_mode(RS485_RECV_MODE);

    uart_receive_rt(pData, Size);
}
