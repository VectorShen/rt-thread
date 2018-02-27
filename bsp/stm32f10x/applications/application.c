/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>

#ifdef RT_USING_DFS
/* dfs filesystem:ELM filesystem init */
#include <dfs_elm.h>
/* dfs Filesystem APIs */
#include <dfs_fs.h>
#endif

#ifdef RT_USING_RTGUI
#include <rtgui/rtgui.h>
#include <rtgui/rtgui_server.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/driver.h>
#include <rtgui/calibration.h>
#endif

#if 0
#include "led.h"

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_stack[ 512 ];
static struct rt_thread led_thread;
static void led_thread_entry(void* parameter)
{
    unsigned int count=0;

    rt_hw_led_init();

    while (1)
    {
        /* led1 on */
#ifndef RT_USING_FINSH
        rt_kprintf("led on, count : %d\r\n",count);
#endif
        count++;
        rt_hw_led_on(0);
        rt_thread_delay( RT_TICK_PER_SECOND/2 ); /* sleep 0.5 second and switch to other thread */

        /* led1 off */
#ifndef RT_USING_FINSH
        rt_kprintf("led off\r\n");
#endif
        rt_hw_led_off(0);
        rt_thread_delay( RT_TICK_PER_SECOND/2 );
    }
}
#endif

#ifdef RT_USING_RTGUI
rt_bool_t cali_setup(void)
{
    rt_kprintf("cali setup entered\n");
    return RT_FALSE;
}

void cali_store(struct calibration_data *data)
{
    rt_kprintf("cali finished (%d, %d), (%d, %d)\n",
               data->min_x,
               data->max_x,
               data->min_y,
               data->max_y);
}
#endif /* RT_USING_RTGUI */

#ifdef RT_USING_ZIGBEE_TI_GATEWAY
#if 0
extern rt_uint8_t npi_lnx_ipc_stack[ 3072 ];
extern struct rt_thread npi_lnx_ipc_thread;
extern void npi_lnx_ipc_main(void* arg);
#endif
#ifdef RT_USING_ZIGBEE_TI_GATEWAY_VERSION_QUERY
extern rt_uint8_t version_query_stack[ 2048 ];
extern struct rt_thread version_query_thread;
extern void version_query_main(void* arg);
#endif
#endif

#ifdef RT_USING_PLATFORM_INIT
extern void rt_platform_init(void);
#endif /* RT_USING_PLATFORM_INIT */

void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_PLATFORM_INIT
    rt_platform_init();
#endif

#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

    /* Filesystem Initialization */
#if defined(RT_USING_DFS) && defined(RT_USING_DFS_ELMFAT)
    /* mount sd card fat partition 1 as root directory */
    if (dfs_mount("sd0", "/", "elm", 0, 0) == 0)
    {
        rt_kprintf("File System initialized!\n");
    }
    else
        rt_kprintf("File System initialzation failed!\n");
#endif  /* RT_USING_DFS */

#ifdef RT_USING_RTGUI
    {
        extern void rt_hw_lcd_init();
        extern void rtgui_touch_hw_init(void);

        rt_device_t lcd;

        /* init lcd */
        rt_hw_lcd_init();

        /* init touch panel */
        rtgui_touch_hw_init();

        /* find lcd device */
        lcd = rt_device_find("lcd");

        /* set lcd device as rtgui graphic driver */
        rtgui_graphic_set_device(lcd);

#ifndef RT_USING_COMPONENTS_INIT
        /* init rtgui system server */
        rtgui_system_server_init();
#endif

        calibration_set_restore(cali_setup);
        calibration_set_after(cali_store);
        calibration_init();
    }
#endif /* #ifdef RT_USING_RTGUI */

#ifdef RT_USING_ZIGBEE_TI_GATEWAY
#if 0
    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&npi_lnx_ipc_thread,
                            "npi_lnx_ipc",
                            npi_lnx_ipc_main,
                            RT_NULL,
                            (rt_uint8_t*)&npi_lnx_ipc_stack[0],
                            sizeof(npi_lnx_ipc_stack),
                            9,
                            20);
    if (result == RT_EOK)
    {
        rt_thread_startup(&npi_lnx_ipc_thread);
    }
#endif
#if 0
    rt_thread_t zb_gateway_thread;
    zb_gateway_thread = rt_thread_create("zb_gateway",
                                   npi_lnx_ipc_main, RT_NULL,
                                   3072, 9, 20);

    if (zb_gateway_thread != RT_NULL)
        rt_thread_startup(zb_gateway_thread);
#endif
#ifdef RT_USING_ZIGBEE_TI_GATEWAY_VERSION_QUERY
    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&version_query_thread,
                            "version_query",
                            version_query_main,
                            RT_NULL,
                            (rt_uint8_t*)&version_query_stack[0],
                            sizeof(version_query_stack),
                            9,
                            20);
    if (result == RT_EOK)
    {
        rt_thread_startup(&version_query_thread);
    }
#endif
#endif
}

int rt_application_init(void)
{
    rt_thread_t init_thread;
#if 0
    rt_err_t result;

    /* init led thread */
    result = rt_thread_init(&led_thread,
                            "led",
                            led_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&led_stack[0],
                            sizeof(led_stack),
                            20,
                            5);
    if (result == RT_EOK)
    {
        rt_thread_startup(&led_thread);
    }
#endif

#if (RT_THREAD_PRIORITY_MAX == 32)
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 8, 20);
#else
    init_thread = rt_thread_create("init",
                                   rt_init_thread_entry, RT_NULL,
                                   2048, 80, 20);
#endif

    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);

    return 0;
}

/*@}*/
