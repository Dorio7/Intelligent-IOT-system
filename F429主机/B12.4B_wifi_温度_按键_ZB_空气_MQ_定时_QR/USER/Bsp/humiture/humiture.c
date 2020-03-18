/**
  ******************************************************************************
  * @file    bsp_beep.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   温湿度传感器应用函数接口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "./Bsp/bsp.h"
#include "./Bsp/humiture/humiture.h"


/*
 * 函数名：TEMP_HUM_GPIO_Config
 * 描述  ：配置TEMP_HUM用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void TEMP_HUM_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启DHT11_PORT的外设时钟*/
	RCC_AHB1PeriphClockCmd(TEMP_HUM_CLK, ENABLE); 

	/*选择要控制的DHT11_PORT引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  
  /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  /*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	/*设置引脚速率为50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_PORT*/
  GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure); 
}

/*
 * 函数名：TEMP_HUM_Mode_IPU
 * 描述  ：使TEMP_HUM-DATA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void TEMP_HUM_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;

	  	/*选择要控制的DHT11_PORT引脚*/	
	  GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;

	   /*设置引脚模式为浮空输入模式*/ 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN ; 
  
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  	/*设置引脚速率为50MHz */   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  
	  /*调用库函数，初始化DHT11_PORT*/
	  GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 
}

/*
 * 函数名：TEMP_HUM_Mode_Out_PP
 * 描述  ：使TEMP_HUM-DATA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void TEMP_HUM_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	/*选择要控制的DHT11_PORT引脚*/															   
  GPIO_InitStructure.GPIO_Pin = TEMP_HUM_PIN;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
  
  /*设置引脚的输出类型为推挽输出*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  /*设置引脚为上拉模式*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
	/*设置引脚速率为50MHz */   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化DHT11_PORT*/
  	GPIO_Init(TEMP_HUM_PORT, &GPIO_InitStructure);	 	 
}

/* 
 * 从DHT11读取一个字节，MSB先行
 */
static uint8_t Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		bsp_DelayUS(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(TEMP_HUM_DATA_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(TEMP_HUM_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}
/*
 * 一次完整的数据传输为40bit，高位先出
 * 8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
 */
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
  uint16_t count;
	/*输出模式*/
	TEMP_HUM_Mode_Out_PP();
	/*主机拉低*/
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	/*延时18ms*/
	bsp_DelayUS(20000);

	/*总线拉高 主机延时30us*/
	TEMP_HUM_DATA_OUT(HUMITURE_HIGH); 

	bsp_DelayUS(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	TEMP_HUM_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(TEMP_HUM_DATA_IN()==Bit_RESET)     
	{
    count=0;
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(TEMP_HUM_DATA_IN()==Bit_RESET)
    {
      count++;
      if(count>1000)  return 0;
      bsp_DelayUS(10); 
    }    
    
    count=0;
		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(TEMP_HUM_DATA_IN()==Bit_SET)
    {
      count++;
      if(count>1000)  return 0;
      bsp_DelayUS(10); 
    }  
		/*开始接收数据*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();

		/*读取结束，引脚改为输出模式*/
		TEMP_HUM_Mode_Out_PP();
		/*主机拉高*/
		TEMP_HUM_DATA_OUT(HUMITURE_HIGH);

		/*检查读取的数据是否正确*/
		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;
	}
	else
	{		
		return 0;
	}   
}


/*********************  DS18B20 **************************/
/*
 *主机给从机发送复位脉冲
 */
static void DS18B20_Rst(void)
{
	/* 主机设置为推挽输出 */
	TEMP_HUM_Mode_Out_PP();
	
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	
	/* 主机至少产生480us的低电平复位信号 */
	bsp_DelayUS(750);

	/* 主机在产生复位信号后，需将总线拉高 */
	TEMP_HUM_DATA_OUT(HUMITURE_HIGH);
	
	/*从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲*/
	bsp_DelayUS(15);
}

/*
 * 检测从机给主机返回的存在脉冲
 * 0：成功
 * 1：失败
 */
static uint8_t DS18B20_Presence(void)
{
	uint8_t pulse_time = 0;
	
	/* 主机设置为上拉输入 */
	TEMP_HUM_Mode_IPU();
	
	/* 等待存在脉冲的到来，存在脉冲为一个60~240us的低电平信号 
	 * 如果存在脉冲没有来则做超时处理，从机接收到主机的复位信号后，会在15~60us后给主机发一个存在脉冲
	 */
	while( TEMP_HUM_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	/* 经过100us后，存在脉冲都还没有到来*/
	if( pulse_time >=100 )
		return 0;
	else
		pulse_time = 0;
	
	/* 存在脉冲到来，且存在的时间不能超过240us */
	while( !TEMP_HUM_DATA_IN() && pulse_time<240 )
	{
		pulse_time++;
		bsp_DelayUS(1);
	}	
	if( pulse_time >=240 )
		return 0;
	else
		return 1;
}

/*
 * 从DS18B20读取一个bit
 */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t dat;
	
	/* 读0和读1的时间至少要大于60us */	
	TEMP_HUM_Mode_Out_PP();
	/* 读时间的起始：必须由主机产生 >1us <15us 的低电平信号 */
	TEMP_HUM_DATA_OUT(HUMITURE_LOW);
	bsp_DelayUS(10);
	
	/* 设置成输入，释放总线，由外部上拉电阻将总线拉高 */
	TEMP_HUM_Mode_IPU();
	//Delay_us(2);
	
	if( TEMP_HUM_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* 这个延时参数请参考时序图 */
	bsp_DelayUS(45);
	
	return dat;
}

/*
 * 从DS18B20读一个字节，低位先行
 */
uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		
		dat = (dat) | (j<<i);
	}
	
	return dat;
}

/*
 * 写一个字节到DS18B20，低位先行
 */
void DS18B20_Write_Byte(uint8_t dat)
{
	uint8_t i, testb;
	TEMP_HUM_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* 写0和写1的时间至少要大于60us */
		if (testb)
		{			
			TEMP_HUM_DATA_OUT(HUMITURE_LOW);
			/* 1us < 这个延时 < 15us */
			bsp_DelayUS(8);
			
			TEMP_HUM_DATA_OUT(HUMITURE_HIGH);
			bsp_DelayUS(58);
		}		
		else
		{			
			TEMP_HUM_DATA_OUT(HUMITURE_LOW);
			/* 60us < Tx 0 < 120us */
			bsp_DelayUS(70);
			
			TEMP_HUM_DATA_OUT(HUMITURE_HIGH);			
			/* 1us < Trec(恢复时间) < 无穷大*/
			bsp_DelayUS(2);
		}
	}
}

void DS18B20_Start(void)
{
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);		/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);		/* 开始转换 */
}

uint8_t DS18B20_Init(void)
{
	TEMP_HUM_GPIO_Config();
	DS18B20_Rst();
	
	return DS18B20_Presence();
}
/*
 * 存储的温度是16 位的带符号扩展的二进制补码形式
 * 当工作在12位分辨率时，其中5个符号位，7个整数位，4个小数位
 *
 *         |---------整数----------|-----小数 分辨率 1/(2^4)=0.0625----|
 * 低字节  | 2^3 | 2^2 | 2^1 | 2^0 | 2^(-1) | 2^(-2) | 2^(-3) | 2^(-4) |
 *
 *
 *         |-----符号位：0->正  1->负-------|-----------整数-----------|
 * 高字节  |  s  |  s  |  s  |  s  |    s   |   2^6  |   2^5  |   2^4  |
 *
 * 
 * 温度 = 符号位 + 整数 + 小数*0.0625
 */
float DS18B20_Get_Temp(void)
{
	uint8_t tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0X44);				/* 开始转换 */
	
	DS18B20_Rst();
	DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* 跳过 ROM */
	DS18B20_Write_Byte(0XBE);				/* 读温度值 */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	
	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* 负温度 */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = s_tem * 0.0625;
	
	return f_tem; 	
}
