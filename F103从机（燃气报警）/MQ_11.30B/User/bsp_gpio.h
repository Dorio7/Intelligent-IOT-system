#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0


void C_GPIO_Config(void);

#endif /* __LED_H */
