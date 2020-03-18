/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_gpio.h"


uint32_t E1ST=0;
u8 SW1_ST[11]={0xFE, 0x08,	0xA1,	0xA1,	0x22,	0x22,	0x01,	0x00,	0x00,	0x00,	0xFF};


uint8_t ZIGBEE_R_TEMP[30]={0x00};//zigbee串口接收缓存
uint8_t R_flag=0;
uint8_t NOFINS_flag=0;//有未包处理标志



//延时
void Delay(__IO u32 nCount)
{
  for(; nCount != 0; nCount--);
} 

//继电器控制
void C1_C(u8 a)
{	
	if (a)	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
		else		
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}




/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
		u8 i;
	
	SystemInit();
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	USART2_Config();
	
	C_GPIO_Config();
	
//	/* 发送一个字符串 */
//	USART_SendData(USART2,0x01);	
//	USART_SendData(USART1,0x02);



	USART1_Sned_Char_Buff(SW1_ST,11);
  while(1)
	{	
//		C1_C(1);			  //触发继电器
		if(E1ST)
			C1_C(1);
		else
			C1_C(0);
		
		Delay(0X1FFFFf);
		
		
//		USART2_Sned_Char_Buff(SW1_ST,11);

//		for(i=0;i<11;i++)
//		{
//		USART_SendData(USART2,SW1_ST[i]);		
//		/* 等待发送数据寄存器为空 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
//		}

		
	}	
}

/*********************************************END OF FILE**********************/
