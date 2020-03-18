#ifndef _BSP_BH1750_H_
#define _BSP_BH1750_H_

#include "stm32f4xx.h"
 
#define BH1750_SLAVE_ADDRESS    0x23//0x46		/* I2C从机地址 */

/* 操作码 Opercode 定义 */
enum
{
	BHOP_POWER_DOWN = 0x00,		/* 进入掉电模式。芯片上电后缺省就是PowerDown模式 */
	BHOP_POWER_ON = 0x01,		  /* 上电，等待测量命令 */
	BHOP_RESET = 0x07,			  /* 清零数据寄存器 (Power Down 模式无效) */
	BHOP_CON_H_RES  = 0x10,		/* 连续高分辨率测量模式  （测量时间 120ms） （最大 180ms）*/
	BHOP_CON_H_RES2 = 0x11,		/* 连续高分辨率测量模式2 （测量时间 120ms）*/
	BHOP_CON_L_RES = 0x13,		/* 连续低分辨率测量模式 （测量时间 16ms）*/

	BHOP_ONE_H_RES  = 0x20,		/* 单次高分辨率测量模式 , 之后自动进入Power Down */
	BHOP_ONE_H_RES2 = 0x21,		/* 单次高分辨率测量模式2 , 之后自动进入Power Down  */
	BHOP_ONE_L_RES = 0x23,		/* 单次低分辨率测量模式 , 之后自动进入Power Down  */
};

uint8_t bsp_InitBH1750(void);
uint8_t bsp_DeinitBH1750(void);
void BH1750_AdjustSensitivity(uint8_t _ucMTReg);
void BH1750_ChageMode(uint8_t _ucMode);
float BH1750_GetLux(void);

#endif   //_BSP_BH1750_H_

/***************************** (END OF FILE) *********************************/
