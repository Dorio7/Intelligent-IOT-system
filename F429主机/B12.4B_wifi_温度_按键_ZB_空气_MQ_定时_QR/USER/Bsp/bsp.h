/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/
#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  "stm32f4xx.h"


extern uint16_t bsp_result;
/*
*********************************************************************************************************
*                                              DEFINE
*********************************************************************************************************
*/
#define   BSP_OK              (uint16_t)     0				/*  (00) Succeeded                              */
#define 	BSP_OS              (uint16_t)0x0001				/*  (01) The emWin drive cannot work            */ 
#define 	BSP_SDRAM           (uint16_t)0x0002				/*  (02) The sdram drive cannot work            */  
#define 	BSP_GTP             (uint16_t)0x0004				/*  (03) The gt9xx drive cannot work            */ 
#define 	BSP_TPAD            (uint16_t)0x0008				/*  (04) The touch pad drive cannot work        */ 
#define 	BSP_SD              (uint16_t)0x0010				/*  (05) The SD Card drive cannot work          */ 
#define 	BSP_SPIFLASH        (uint16_t)0x0020				/*  (06) The SPI FLASH drive cannot work        */ 
#define 	BSP_GUI             (uint16_t)0x0040				/*  (07) The emWin drive cannot work            */ 
#define 	BSP_WM8978          (uint16_t)0x0080				/*  (08) The WM8978 drive cannot work           */
#define 	BSP_ALLOC           (uint16_t)0x0100				/*  (09) The memory alloc fail                  */
#define 	BSP_XBF             (uint16_t)0x0200				/*  (10) The XBF Font cannot work               */
#define   BSP_RTC             (uint16_t)0x0400        /*  (11) The RTC cannot work                    */
#define   BSP_NETWORK         (uint16_t)0x0800        /*  (12) The Network cannot work                */
#define   BSP_CAMERA          (uint16_t)0x1000        /*  (13) The Camera cannot work                 */

/*------------------- BL8782_PDN   -------------------------*/
#define WIFI_PDN_PIN                  GPIO_Pin_9                 
#define WIFI_PDN_GPIO_PORT            GPIOG                      
#define WIFI_PDN_GPIO_CLK             RCC_AHB1Periph_GPIOG
#define WIFI_PDN_ENABLE()             GPIO_SetBits(WIFI_PDN_GPIO_PORT,WIFI_PDN_PIN)
#define WIFI_PDN_DISABLE()            GPIO_ResetBits(WIFI_PDN_GPIO_PORT,WIFI_PDN_PIN)  
/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void Bsp_Soft_Reset(void);

void BSP_Init(void);

void BSP_IntDisAll(void);

CPU_INT32U BSP_CPU_ClkFreq(void);

void BSP_Tick_Init(void);

#define bsp_DelayMS(x)   bsp_DelayUS(1000*x)

void bsp_DelayUS(uint32_t _ulDelayTime);

int get_tick_count(unsigned long *count);
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */

