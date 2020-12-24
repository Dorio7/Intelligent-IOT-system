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

	SystemInit();
  /*初始化USART 配置模式为 115200 8-N-1，中断接收*/
  USART_Config();
	USART2_Config();
	
	C_GPIO_Config();

	USART1_Sned_Char_Buff(SW1_ST,11);
  while(1)
	{	
//		C1_C(1);			  //触发继电器
		if(E1ST)
			C1_C(1);
		else
			C1_C(0);
	
		Delay(0X1FFFFf);

	}	
}

