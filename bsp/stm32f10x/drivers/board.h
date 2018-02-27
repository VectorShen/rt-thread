/*
 * File      : board.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-09-22     Bernard      add board.h to this bsp
 */

// <<< Use Configuration Wizard in Context Menu >>>
#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "stm32f10x.h"

/* board configuration */

/* whether use board external SRAM memory */
// <e>Use external SRAM memory on the board
// 	<i>Enable External SRAM memory
#define STM32_EXT_SRAM          0
//	<o>Begin Address of External SRAM
//		<i>Default: 0x68000000
#define STM32_EXT_SRAM_BEGIN    0x68000000 /* the begining address of external SRAM */
//	<o>End Address of External SRAM
//		<i>Default: 0x68080000
#define STM32_EXT_SRAM_END      0x68080000 /* the end address of external SRAM */
// </e>

// <o> Internal SRAM memory size[Kbytes] <8-64>
//	<i>Default: 64
#define STM32_SRAM_SIZE         64
#define STM32_SRAM_END          (0x20000000 + STM32_SRAM_SIZE * 1024)

// <<< Use Configuration Wizard in Context Menu >>>

/* USART driver select. */
#define RT_USING_UART1
#define RT_USING_UART2

//#define RT_USING_UART3
//#define USING_BXCAN1


// <o> Console on USART: <0=> no console <1=>USART 1 <2=>USART 2 <3=> USART 3
// 	<i>Default: 1
#define STM32_CONSOLE_USART		        1

#if STM32_CONSOLE_USART                 == 0
#define CONSOLE_DEVICE                  "no"
#elif STM32_CONSOLE_USART               == 1
#define CONSOLE_DEVICE                  "uart1"
#elif STM32_CONSOLE_USART               == 2
#define CONSOLE_DEVICE                  "uart2"
#elif STM32_CONSOLE_USART               == 3
#define CONSOLE_DEVICE                  "uart3"
#endif

#define FINSH_DEVICE_NAME               CONSOLE_DEVICE

#ifdef RT_USING_SPI
#define SPI_USING_DMA

#define RT_USING_SPI1_FLAG              0
#define RT_USING_SPI2_FLAG              1
#define RT_USING_SPI3_FLAG              2

#define RT_USING_SPI1					//ENC28J60
#ifdef RT_USING_SPI1
#define RT_USING_SPI1_REMAP             0   //1
#endif /* RT_USING_SPI1 */

#define RT_USING_SPI2					//CC2530

//#define RT_USING_SPI3					//Flash

/* SPI Applications */
//#define RT_USING_SPI_SDCARD             RT_USING_SPI1_FLAG
//#define RT_USING_SPI_SDCARD_IN_DFS

//#define RT_USING_SPI_FLASH
#ifdef RT_USING_SPI_FLASH
#define RT_USING_SPI_FLASH_W25QXX       RT_USING_SPI3_FLAG
#ifdef RT_USING_SPI_FLASH_W25QXX
#define RT_USING_SPI_FLASH_W25QXX_IN_DFS
#endif	/* RT_USING_SPI_FLASH_W25QXX */
#endif	/* RT_USING_SPI_FLASH */

#endif /* RT_USING_SPI */

#ifdef RT_USING_SDIO
#define RT_USING_SDIO_FOR_SDCARD        0
#define RT_USING_SDIO_FOR_SDIO_WIFI     1

#define RT_USING_SDIO_APP               RT_USING_SDIO_FOR_SDCARD
#endif /* RT_USING_SDIO */

void rt_hw_board_init(void);

#endif /* __BOARD_H__ */
