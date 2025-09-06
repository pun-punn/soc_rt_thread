/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */


#include "ethernet.h"

socklen_t sin_size;
int sock, connected, bytes_received;
struct sockaddr_in server_addr, client_addr;
struct netdev *net_dev = RT_NULL;


void socket_init()
{
    rt_kprintf("RT-Thread Ethernet thread starting...\n");

    rt_thread_t can_send_thread = rt_thread_create("ethernet_thread",  // Thread name
                                                        ethernet_server, // Thread entry function
                                                          RT_NULL,     // Thread parameters
                                                          4096, // Stack size
                                                          10,   // Thread priority
                                                          20);  // Time slice
    if (can_send_thread != RT_NULL)
    {
        rt_thread_startup(can_send_thread);  // Start the thread
    }
    else
    {
        rt_kprintf("Failed to create CAN thread\n");
    }
}



void ethernet_server(void *paramater)
{

}



int socket_create_server()
{

    //recv_data = rt_malloc(BUFSZ + 1);
    //if (recv_data == RT_NULL)
    //{
    //    rt_kprintf("No memory\n");
    //    return -RT_ERROR;
    //}


    net_dev = netdev_get_by_name("W5500");
    if (net_dev == RT_NULL)
    {
        rt_kprintf("get network interface device(%s) failed!\n", "W5500");
        return -RT_ERROR;
    }rt_kprintf("get network interface device(%s) success!\n", "W5500");
    netdev_set_up(net_dev);

    int count = 20;
    while(count-- && !(net_dev->ip_addr.addr))
    {
        rt_thread_mdelay(1000);
    }
    if(!count)
    {
        rt_kprintf("can not get ip from net_dev!\n");
        return -RT_ERROR;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0))== -1)
    {
        rt_kprintf("can not create socket\n");
        //rt_free(recv_data);
        return -RT_ERROR;
    }

    return RT_EOK;
}



int socket_bind()
{
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000); //port
    server_addr.sin_addr.s_addr = net_dev->ip_addr.addr; //ip
    rt_memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        rt_kprintf("unable to bind socket\n");
        //rt_free(recv_data);
        return -RT_ERROR;
    }
    return RT_EOK;
}



int socket_listen()
{
    if (listen(sock, 5) == -1)
    {
        rt_kprintf("unable to listen socket\n");
        //rt_free(recv_data);
        return -RT_ERROR;
    }

    return RT_EOK;
}



void socket_accept()
{
    rt_bool_t stop = RT_FALSE;

    while (stop != RT_TRUE)
    {
        sin_size = sizeof(struct sockaddr_in);
        connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);
        if (connected < 0)
        {
            rt_kprintf("unable to accept connection failed!\n");
            continue;
        }
        rt_kprintf("I got a connection from (%s , %d)\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*
        while (1)
        {
            ret = send(connected, send_data, strlen(send_data), 0);
            if (ret < 0)
            {
                closesocket(connected);
                rt_kprintf("\nsend error,close the socket.\r\n");
                break;
            }
            else if (ret == 0)
            {
                rt_kprintf("\n Send warning,send function return 0.\r\n");
            }
            bytes_received = recv(connected, recv_data, BUFSZ, 0);
            if (bytes_received < 0)
            {
                closesocket(connected);
                break;
            }
            else if (bytes_received == 0)
            {
                rt_kprintf("\nReceived warning,recv function return 0.\r\n");
                closesocket(connected);
                break;
            }
            recv_data[bytes_received] = '\0';
            if (strcmp(recv_data, "q") == 0 || strcmp(recv_data, "Q") == 0)
            {
                rt_kprintf("\n Client disconected \r\n");
                closesocket(connected);
                break;
            }
            else if (strcmp(recv_data, "exit") == 0)
            {
                rt_kprintf("\n Server is closed by client \r\n");
                closesocket(connected);
                stop = RT_TRUE;
                break;
            }
            else
            {
                rt_kprintf("RECEIVED DATA = %s \n", recv_data);
            }
        }
        */

    }
}

