#ifndef __HUMITURE_H_
#define __HUMITURE_H_

#include  "stm32f4xx.h"

#define HUMITURE_HIGH  1
#define HUMITURE_LOW   0


/*---------------------------------------*/
#define TEMP_HUM_CLK     RCC_AHB1Periph_GPIOE
#define TEMP_HUM_PIN     GPIO_Pin_2               
#define TEMP_HUM_PORT    GPIOE


#define TEMP_HUM_DATA_OUT(a)	if (a)	\
                                   GPIO_SetBits(TEMP_HUM_PORT,TEMP_HUM_PIN);\
                                   else		\
                                   GPIO_ResetBits(TEMP_HUM_PORT,TEMP_HUM_PIN)

#define  TEMP_HUM_DATA_IN()	  GPIO_ReadInputDataBit(TEMP_HUM_PORT,TEMP_HUM_PIN)

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
}DHT11_Data_TypeDef;

void TEMP_HUM_GPIO_Config(void);
uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);

#endif //__HUMITURE_H_
