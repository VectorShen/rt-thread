#include <rtthread.h>
#include <board.h>

#ifdef RT_USING_RTC_DEVICE
#include "drivers/rtc.h"
#ifdef RT_USING_EXTERNAL_RTC_DEVICE_DS1302
#define DS1302_POWER_PIN						10	//PC2
#define DS1302_IO_PIN							2	//PC13
#define DS1302_CLK_PIN							3	//PC14
#define DS1302_RST_PIN							4	//PC15

#include "rtc/ds1302.h"
#endif /* RT_USING_EXTERNAL_RTC_DEVICE_DS1302 */
#endif /* RT_USING_RTC_DEVICE */

#if defined(RT_USING_SDIO) 
#ifdef RT_USING_SDIO_IN_DFS
#include "sdcard.h"
#endif
#endif

#ifdef RT_USING_SPI
#include "rt_stm32f10x_spi.h"

#ifdef RT_ZIGBEE_ZMESH_ZAP
#include "spi/spi_zigbee_zmesh_zap.h"
#define SPI_ZIGBEE_ZMESH_ZAP_PORT_RCC           RCC_AHB1Periph_GPIOB
#define SPI_ZIGBEE_ZMESH_ZAP_PORT               GPIOB
#define SPI_ZIGBEE_ZMESH_ZAP_PIN                GPIO_Pin_14
#endif

#ifdef RT_USING_SPI_FLASH
#ifdef RT_USING_SPI_FLASH_W25QXX
#include "spi/spi_flash_w25qxx.h"
#define SPI_FLASH_W25QXX_POWER_PIN				26	//PB0
#define SPI_FLASH_W25QXX_CS_PORT_RCC            RCC_APB2Periph_GPIOB
#define SPI_FLASH_W25QXX_CS_PORT                GPIOB  
#define SPI_FLASH_W25QXX_CS_PIN                 GPIO_Pin_1
#endif
#endif

#ifdef RT_USING_LWIP
#if RT_USING_LWIP_ETH_DRIVER == RT_USING_LWIP_ETH_DRIVER_ENC28J60
#include "enc28j60.h"
#if (defined STM32F10X_MD)
#define ENC28J60_POWER_PIN						14	//PA0
#define ENC28J60_RST_PIN						16	//PA2
#define ENC28J60_IRQ_PIN        				15  //PA8
#elif (defined STM32F10X_HD)
#define ENC28J60_POWER_PIN						34	//PA0
#define ENC28J60_IRQ_PIN        				35  //PA1
#define ENC28J60_RST_PIN						100	//PA8
#endif
#endif
#endif

#endif

#ifdef RT_USING_SPI
static void rt_hw_spi_init(void)
{
#ifdef RT_USING_SPI1
    static struct stm32_spi_bus stm32_spi1;
    static struct rt_spi_device spi_device1;
    static struct stm32_spi_cs  spi_cs1;
#endif
#ifdef RT_USING_SPI2
    static struct stm32_spi_bus stm32_spi2;
    static struct rt_spi_device spi_device2;
    static struct stm32_spi_cs  spi_cs2;    
#endif
#ifdef RT_USING_SPI3
    static struct stm32_spi_bus stm32_spi3;
    static struct rt_spi_device spi_device3;
    static struct stm32_spi_cs  spi_cs3;    
#endif
    
#ifdef RT_USING_SPI1
#if RT_USING_SPI1_REMAP
/*
 * SPI1_MOSI: PB5
 * SPI1_MISO: PB4
 * SPI1_SCK : PB3
 *
 * CS0: PB0  SD card.
*/
#else
/*
 * SPI1_MOSI: PA7
 * SPI1_MISO: PA6
 * SPI1_SCK : PA5
 *
 * CS0: PA4  SD card.
*/
#endif
#if RT_USING_SPI1_REMAP
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,
        ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        stm32_spi_register(SPI1, &stm32_spi1, "spi1");
    }

    /* attach cs */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

#ifdef RT_USING_SPI_FLASH
#ifdef RT_USING_SPI_FLASH_W25QXX
        /* spi11: PB0 */
        spi_cs1.GPIOx = SPI_FLASH_W25QXX_CS_PORT;
        spi_cs1.GPIO_Pin = SPI_FLASH_W25QXX_CS_PIN;
        RCC_APB2PeriphClockCmd(SPI_FLASH_W25QXX_CS_PORT_RCC, ENABLE);  
#endif
#else
        /* spi11: PB0 */
        spi_cs1.GPIOx = GPIOB;
        spi_cs1.GPIO_Pin = GPIO_Pin_0;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);       
#endif
           
        GPIO_InitStructure.GPIO_Pin = spi_cs1.GPIO_Pin;
        GPIO_SetBits(spi_cs1.GPIOx, spi_cs1.GPIO_Pin);
        GPIO_Init(spi_cs1.GPIOx, &GPIO_InitStructure);

        rt_spi_bus_attach_device(&spi_device1, "spi10", "spi1", (void*)&spi_cs1);
    }
#else
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,
        ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        stm32_spi_register(SPI1, &stm32_spi1, "spi1");
    }

    /* attach cs */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

        /* spi21: PG10 */
        spi_cs1.GPIOx = GPIOA;
        spi_cs1.GPIO_Pin = GPIO_Pin_4;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

        GPIO_InitStructure.GPIO_Pin = spi_cs1.GPIO_Pin;
        GPIO_SetBits(spi_cs1.GPIOx, spi_cs1.GPIO_Pin);
        GPIO_Init(spi_cs1.GPIOx, &GPIO_InitStructure);

        rt_spi_bus_attach_device(&spi_device1, "spi10", "spi1", (void*)&spi_cs1);
    }
#endif /* RT_USING_SPI1_REMAP */
#endif /* RT_USING_SPI1 */

#ifdef RT_USING_SPI2
/*
 * SPI2_MOSI: PB15
 * SPI2_MISO: PB14
 * SPI2_SCK : PB13
 *
 * CS2: PB12  SD card.
*/
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,
        ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        stm32_spi_register(SPI2, &stm32_spi2, "spi2");
    }

    /* attach cs */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

        /* spi21: PG10 */
        spi_cs2.GPIOx = GPIOB;
        spi_cs2.GPIO_Pin = GPIO_Pin_12;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

        GPIO_InitStructure.GPIO_Pin = spi_cs2.GPIO_Pin;
        GPIO_SetBits(spi_cs2.GPIOx, spi_cs2.GPIO_Pin);
        GPIO_Init(spi_cs2.GPIOx, &GPIO_InitStructure);

        rt_spi_bus_attach_device(&spi_device2, "spi20", "spi2", (void*)&spi_cs2);
    }
#endif /* RT_USING_SPI2 */

#ifdef RT_USING_SPI3
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    /*
     * SPI1_MOSI: PB5
     * SPI1_MISO: PB4
     * SPI1_SCK : PB3
     *
     * CS0: PA15
    */
    /* register spi bus */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,
        ENABLE);

        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        stm32_spi_register(SPI3, &stm32_spi3, "spi3");
    }

    /* attach cs */
    {
        GPIO_InitTypeDef GPIO_InitStructure;

        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

        /* spi31: PA15 */
        spi_cs3.GPIOx = SPI_ZIGBEE_ZMESH_ZAP_PORT;
        spi_cs3.GPIO_Pin = SPI_FLASH_W25QXX_CS_PIN;
        RCC_APB2PeriphClockCmd(SPI_FLASH_W25QXX_CS_PORT_RCC, ENABLE);

        GPIO_InitStructure.GPIO_Pin = spi_cs3.GPIO_Pin;
        GPIO_SetBits(spi_cs3.GPIOx, spi_cs3.GPIO_Pin);
        GPIO_Init(spi_cs3.GPIOx, &GPIO_InitStructure);

        rt_spi_bus_attach_device(&spi_device3, "spi30", "spi3", (void*)&spi_cs3);
    }
#endif
}
#endif /* RT_USING_SPI */

void rt_platform_init(void)
{
#ifdef RT_USING_RTC_DEVICE
#ifdef RT_USING_EXTERNAL_RTC_DEVICE_DS1302
  	rt_hw_ds1302_init_pins(DS1302_POWER_PIN, DS1302_IO_PIN, \
	  	DS1302_CLK_PIN, DS1302_RST_PIN);
    rt_hw_ds1302_init();
#endif
#endif
	
#ifdef RT_USING_SPI
    rt_hw_spi_init();
    
#ifdef RT_USING_LWIP
#if RT_USING_LWIP_ETH_DRIVER == RT_USING_LWIP_ETH_DRIVER_ENC28J60
	enc28j60_attach_pins(ENC28J60_POWER_PIN,\
	  	ENC28J60_RST_PIN, ENC28J60_IRQ_PIN);
    enc28j60_attach("spi10");
    rt_kprintf("stm32 register spi bus device spi10\n");
#endif
#endif /* RT_USING_LWIP */
    
#ifdef RT_WIRELESS_ZIGBEE
#ifdef RT_ZIGBEE_ZMESH_ZAP
    zap_zigbee_attach("zap0", "spi20");
#endif
#endif

#ifdef RT_USING_SPI_FLASH
#ifdef RT_USING_SPI_FLASH_W25QXX
    w25qxx_attach_pins(SPI_FLASH_W25QXX_POWER_PIN);
#if (RT_USING_SPI_FLASH_W25QXX==RT_USING_SPI1_FLAG)
    w25qxx_init("flash0","spi10");    
#elif (RT_USING_SPI_FLASH_W25QXX==RT_USING_SPI2_FLAG)
    w25qxx_init("flash0","spi20");      
#elif (RT_USING_SPI_FLASH_W25QXX==RT_USING_SPI3_FLAG)
    w25qxx_init("flash0","spi30");      
#endif
    
#endif  //RT_USING_SPI_FLASH_W25QXX
#endif  //RT_USING_SPI_FLASH

#endif // RT_USING_SPI

#if defined(RT_USING_SDIO) 
#if (RT_USING_SDIO_APP==RT_USING_SDIO_FOR_SDCARD)
/*
½«sd¿¨¹ÒÔØµ½sd0
sdioÒý½Å£º
	PC.06		POWER
  	PC.08 		D0
  	PC.09 		D1
  	PC.10 		D2
  	PC.11 		D3
  	PC.12 		CLK
   	PD.02 		CMD  
*/
	rt_hw_sdcard_init();
#else
    
#endif
#endif
    
}
