/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "./Bsp/usart/bsp_debug_usart.h"
#include "./Bsp/exti/bsp_exti.h"
#include  <os.h>


/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);
extern void gyro_data_ready_cb(void);
/* Private functions ---------------------------------------------------------*/

extern uint8_t ZIGBEE_R_TEMP[];//zigbee串口接收缓存
extern uint8_t R_flag;
extern uint8_t NOFINS_flag;

extern u8 SW1_ST[];
extern uint32_t IOT_DHT11_wendu;//温度
extern uint32_t valueDHT11_shidu;//湿度
extern uint8_t E1ST;
extern uint8_t E2ST;

extern uint32_t POW_H;
extern uint32_t POW_L;



extern uint32_t MQ_W;

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


void USART3_IRQHandler(void)                    //串口3中断服务程序
{
    uint8_t ucTemp;
		if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
		{
			ucTemp = USART_ReceiveData( USART3 );
//			USART_SendData(USART3,ucTemp);
			if (ucTemp==0xFE)
			{
				R_flag=0;
				ZIGBEE_R_TEMP[0]=ucTemp;
				R_flag++;
			}
			else if (ucTemp==0xFF)
			{
				ZIGBEE_R_TEMP[10]=0xFF;
				R_flag=0;
				NOFINS_flag=1;//有未处理的包标志
			}
			else
			{
				ZIGBEE_R_TEMP[R_flag]=ucTemp;
				R_flag++;
			}	
		}
		
//		USART_SendData(USART1,ucTemp);
		
		if(NOFINS_flag)//有数据包未处理
		{
			if(ZIGBEE_R_TEMP[2]==0xA1&ZIGBEE_R_TEMP[4]==0x22)
			{
				E1ST=ZIGBEE_R_TEMP[6];
			}
//			else if(ZIGBEE_R_TEMP[2]==0xA2&ZIGBEE_R_TEMP[4]==0x22)
//			{
//							
//				POW_H=ZIGBEE_R_TEMP[8];
//				POW_L=ZIGBEE_R_TEMP[9];
//				
//			}
		else if(ZIGBEE_R_TEMP[2]==0xA4&ZIGBEE_R_TEMP[4]==0x44)
			MQ_W=ZIGBEE_R_TEMP[6];
			
			
			
			NOFINS_flag=0;
		}

}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	printf("hardfault!!!\n");
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();  
	OSIntNestingCtr++;
	CPU_CRITICAL_EXIT();
  
  LTDC_ISR_Handler();
  
  OSIntExit();
}

/**
  * @brief  This function handles DMA2D global interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();  
	OSIntNestingCtr++;
	CPU_CRITICAL_EXIT();
  
  DMA2D_ISR_Handler();
  
  OSIntExit();
}

void EXTI_IRQHandler(void)
{
	CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();  
	OSIntNestingCtr++;
	CPU_CRITICAL_EXIT();
  
	if(EXTI_GetITStatus(EXTI_INT_EXTI_LINE) != RESET) //确保是否产生了EXTI Line中断
	{
		/* Handle new gyro*/
		gyro_data_ready_cb();
	
		EXTI_ClearITPendingBit(EXTI_INT_EXTI_LINE);     //清除中断标志位
	}  
  
  OSIntExit();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
