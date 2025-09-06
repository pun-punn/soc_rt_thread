/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-10-06     sippa       the first version
 */
#ifndef APPLICATIONS_COMMUNICATION_INCLUDE_ETHERNET_H_
#define APPLICATIONS_COMMUNICATION_INCLUDE_ETHERNET_H_


//Implement W5500 driver and socket

#include <rtthread.h>
#include "wiz.h"
#include "wiz_socket.h"
#include <netdb.h>
#include <netdev.h>
#include <string.h>

void ethernet_server(void *paramater);
//static void ethernet_client_startup(void *paramater);
void socket_init();
int socket_create_server();
int socket_bind();
int socket_listen();
void socket_accept();
void socket_connect();
void socket_send();
void socket_recev();


#endif /* APPLICATIONS_COMMUNICATION_INCLUDE_ETHERNET_H_ */
