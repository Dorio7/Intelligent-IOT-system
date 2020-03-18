#ifndef __EXTI_H
#define	__EXTI_H

#include "stm32f4xx.h"

#define EXTI_INT_GPIO_PORT                GPIOI
#define EXTI_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOI
#define EXTI_INT_GPIO_PIN                 GPIO_Pin_1
#define EXTI_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOI
#define EXTI_INT_EXTI_PINSOURCE           EXTI_PinSource1
#define EXTI_INT_EXTI_LINE                EXTI_Line1
#define EXTI_INT_EXTI_IRQ                 EXTI1_IRQn

#define EXTI_IRQHandler                   EXTI1_IRQHandler

void EXTI_MPU_Config(void);
void EXTI_MPU_STOP(void);
#endif /* __EXTI_H */
