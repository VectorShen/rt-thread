/**************************************************************************************************
 * Filename:       main.c
 * Description:    This file contains the main function of the Serial BootLoader tool.
 *
 *
 * Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef RTT_ZG_GATEWAY
#include <poll.h>
#else
#include <ipc/poll.h>
#endif
#include <time.h>
#include <unistd.h>
#ifndef RTT_ZG_GATEWAY
#include <sys/timerfd.h>
#endif
#include <termios.h>

#include "zbSocCmd.h"
#include "zbSocTransportUart.h"

#ifdef RTT_ZG_GATEWAY
#include <rtthread.h>
#include <dfs_poll.h>
#endif
/*********************************************************************
 * CONSTANTS
 */
#define VER_MAJOR   0
#define VER_MINOR   83
#define VER_WORD    ""

#undef printf
#define printf      rt_kprintf

/*********************************************************************
 * GLOBAL VARIABLES
 */
uint8_t uartDebugPrintsEnabled = 0;

bool usb_target = false;

struct termios old_tio;

//static size_t context = 0;

rt_uint8_t version_query_stack[ 2048 ];
struct rt_thread version_query_thread;
/*********************************************************************
 * FUNCTIONS
 */
//int version_query_main(int argc, char* argv[])
void version_query_main(void* arg)
{
#if 0
	char * selected_serial_port;

	if (argc < 2)
	{
		printf("Error, wrong number of argument. Please use the following syntax:\n");
		printf("  %s <serial_port>\n", argv[0]);
		printf("e.g.\n");
		printf("  %s /dev/ttyACM0\n", argv[0]);
		exit(-1);
	}

	selected_serial_port = argv[1];
#else
    char * selected_serial_port = "/dev/uart2";
#endif
	printf("Using serial port: %s\n", selected_serial_port);
	
	zbSocOpen( selected_serial_port );
	if( serialPortFd == -1 )
	{
		exit(-1);
	}

	zbSocForceRun();
	zbSocGetSystemVersion();

	while(finish_state == STATE_NOT_FINISHED)
	{          
		struct pollfd pollFds[1];

		pollFds[0].fd = serialPortFd;
		pollFds[0].events = POLLIN;

		poll(pollFds, 1, -1);

		if(pollFds[0].revents)
		{
			zbSocProcessRpc();
		}
        rt_thread_delay(10);
	}    
	
	zbSocClose();

	//return finish_state;
}
