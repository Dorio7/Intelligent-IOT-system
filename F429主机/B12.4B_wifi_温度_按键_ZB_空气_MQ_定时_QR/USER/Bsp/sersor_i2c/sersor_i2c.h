/*********************************************************************
File    : i2c.h
Purpose : 
**********************************************************************/
#ifndef __I2C_H__
#define __I2C_H__
/****************************** Includes *****************************/
#include "./Bsp/bsp.h"
#include "./Bsp/usart/bsp_debug_usart.h"
/****************************** Defines *******************************/
#define SENSORS_I2C_FAIL 			  1
#define SENSORS_I2C_SUCCESS 	  0

#define SENSORS_I2C                      I2C1

#define I2C_SPEED                        400000
#define I2C_OWN_ADDRESS                  0x0A

#define I2C_Config()                     I2cMaster_Init();

#define mdelay(x)                        bsp_DelayMS(x)

 
#define I2Cx_FLAG_TIMEOUT                ((uint32_t) 900) //0x1100
#define I2Cx_LONG_TIMEOUT                ((uint32_t) (300 * I2Cx_FLAG_TIMEOUT)) //was300
 

#define SENSORS_I2C_SCL_GPIO_PORT         GPIOB
#define SENSORS_I2C_SCL_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SCL_GPIO_PIN          GPIO_Pin_6
#define SENSORS_I2C_SCL_GPIO_PINSOURCE    GPIO_PinSource6
 
#define SENSORS_I2C_SDA_GPIO_PORT         GPIOB
#define SENSORS_I2C_SDA_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define SENSORS_I2C_SDA_GPIO_PIN          GPIO_Pin_7
#define SENSORS_I2C_SDA_GPIO_PINSOURCE    GPIO_PinSource7

#define SENSORS_I2C_RCC_CLK               RCC_APB1Periph_I2C1
#define SENSORS_I2C_AF                    GPIO_AF_I2C1


#define WAIT_FOR_FLAG(flag, value, timeout, errorcode)  I2CTimeout = timeout;\
          while(I2C_GetFlagStatus(SENSORS_I2C, flag) != value) {\
            if((I2CTimeout--) == 0) return I2Cx_TIMEOUT_UserCallback(errorcode); \
          }
  
#define CLEAR_ADDR_BIT      I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR1);\
                            I2C_ReadRegister(SENSORS_I2C, I2C_Register_SR2);
          
/*ÐÅÏ¢Êä³ö*/
#define MPU_DEBUG_ON                   1
#define MPU_DEBUG_FUNC_ON              0

#define MPU_INFO(fmt,arg...)           printf("<<-MPU-INFO->> "fmt"\n",##arg)
#define MPU_ERROR(fmt,arg...)          printf("<<-MPU-ERROR->> "fmt"\n",##arg)
#define MPU_DEBUG(fmt,arg...)          do{\
                                          if(MPU_DEBUG_ON)\
                                          printf("<<-MPU-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define MPU_DEBUG_FUNC()               do{\
                                         if(MPU_DEBUG_FUNC_ON)\
                                         printf("<<-MPU-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


void I2cMaster_Init(void);
void Set_I2C_Retry(unsigned short ml_sec);
unsigned short Get_I2C_Retry(void);

int Sensors_I2C_ReadRegister(unsigned char Address, unsigned char RegisterAddr, 
                                          unsigned short RegisterLen, unsigned char *RegisterValue);
int Sensors_I2C_WriteRegister(unsigned char Address, unsigned char RegisterAddr, 
                                           unsigned short RegisterLen, const unsigned char *RegisterValue);


unsigned long ST_Sensors_I2C_WriteNoRegister(unsigned char Address, unsigned char RegisterAddr);
unsigned long ST_Sensors_I2C_ReadNoRegister(unsigned char Address, unsigned short RegisterLen, unsigned char *RegisterValue);


#endif // __I2C_H__


