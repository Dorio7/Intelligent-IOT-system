/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related       hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/

#include <stdio.h>
#include <string.h>
#include "hal_key.h"
#include "gizwits_product.h"
#include "common.h"




static uint32_t timerMsCount;
uint8_t aRxBuffer;

/** User area the current device state structure*/
dataPoint_t currentDataPoint;//设备状态结构体
extern keysTypedef_t keys;

extern u8 SW1_ST[];
extern uint32_t IOT_DHT11_wendu;//温度
extern uint32_t valueDHT11_shidu;//湿度
extern uint8_t E1ST;
extern uint8_t E2ST;
extern uint32_t MQ_W;
extern float POW_T;

extern uint32_t IOT_Time_OnOff;
extern uint32_t IOT_Time_Off_Minute;


/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_SW1_OnOff:
        currentDataPoint.valueSW1_OnOff = dataPointPtr->valueSW1_OnOff;
        GIZWITS_LOG("Evt: EVENT_SW1_OnOff %d \n", currentDataPoint.valueSW1_OnOff);
        if(0x01 == currentDataPoint.valueSW1_OnOff)
        {
          //user handle
					E1ST=0;
					
        }
        else
        {
          //user handle    
					E1ST=1;
					
        }
        break;
      case EVENT_Time_OnOff:
        currentDataPoint.valueTime_OnOff = dataPointPtr->valueTime_OnOff;
        GIZWITS_LOG("Evt: EVENT_Time_OnOff %d \n", currentDataPoint.valueTime_OnOff);
        if(0x01 == currentDataPoint.valueTime_OnOff)
        {
          //user handle
					IOT_Time_OnOff=0X01;
        }
        else
        {
          //user handle    
					IOT_Time_OnOff=0X00;
        }
        break;
      case EVENT_SW2_OnOff:
        currentDataPoint.valueSW2_OnOff = dataPointPtr->valueSW2_OnOff;
        GIZWITS_LOG("Evt: EVENT_SW2_OnOff %d \n", currentDataPoint.valueSW2_OnOff);
        if(0x01 == currentDataPoint.valueSW2_OnOff)
        {
          //user handle
					E2ST=0;
        }
        else
        {
          //user handle 
					E2ST=1;					
        }
        break;


      case EVENT_Time_Off_Minute:
        currentDataPoint.valueTime_Off_Minute = dataPointPtr->valueTime_Off_Minute;
        GIZWITS_LOG("Evt:EVENT_Time_Off_Minute %d\n",IOT_Time_Off_Minute);
        //user handle
				IOT_Time_Off_Minute=currentDataPoint.valueTime_Off_Minute;
			
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:
 
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/


void userHandle(void)
{
 /*
    currentDataPoint.valueLIGHT_BUTTON = ;//Add Sensor Data Collection
    currentDataPoint.valueDHT11_wendu = ;//Add Sensor Data Collection
    currentDataPoint.valueDHT11_shidu = ;//Add Sensor Data Collection
    currentDataPoint.valuePower1_Consumption = ;//Add Sensor Data Collection
    currentDataPoint.valuePower2_Consumption = ;//Add Sensor Data Collection
    currentDataPoint.valueWORRY_MQ = ;//Add Sensor Data Collection
    */
	
	currentDataPoint.valueDHT11_wendu=IOT_DHT11_wendu;
  currentDataPoint.valueDHT11_shidu=valueDHT11_shidu;
	currentDataPoint.valueSW1_OnOff=E1ST?0:1;
	currentDataPoint.valueSW2_OnOff=E2ST?0:1;
	currentDataPoint.valueWORRY_MQ =MQ_W;
	currentDataPoint.valuePower1_Consumption = POW_T;
	currentDataPoint.valueTime_OnOff=IOT_Time_OnOff;
	
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
    /*
      currentDataPoint.valueSW1_OnOff = ;
      currentDataPoint.valueTime_OnOff = ;
      currentDataPoint.valueSW2_OnOff = ;
      currentDataPoint.valueTime_Off_Minute = ;
      currentDataPoint.valueLIGHT_BUTTON = ;
      currentDataPoint.valueDHT11_wendu = ;
      currentDataPoint.valueDHT11_shidu = ;
      currentDataPoint.valuePower1_Consumption = ;
      currentDataPoint.valuePower2_Consumption = ;
      currentDataPoint.valueWORRY_MQ = ;
    */

}


/**
* @brief Millisecond timing maintenance function, milliseconds increment, overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief Read millisecond count

* @param none
* @return millisecond count
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief MCU reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
//    __set_FAULTMASK(1);
//    HAL_NVIC_SystemReset();
	GIZWITS_LOG("MCU restart \n");
}

/**@} */

//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//PUTCHAR_PROTOTYPE
//{
//  /* 发送一个字节数据到USART1 */
//		USART_SendData( ((USART_TypeDef *) USART1_BASE), (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus( ((USART_TypeDef *) USART1_BASE), USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}



/**
* @brief Timer TIM3 init function 定时器初始化

* @param none
* @return none
*/
void timerInit(void)
{
	  u16 arr = 7199;
    u16 psc = 9;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE); //Clock enable

    //Timer TIM3 init
    
    TIM_TimeBaseStructure.TIM_Period = 99; //Set the value of the auto-reload register cycle in the next update event
    
    TIM_TimeBaseStructure.TIM_Prescaler = 899; //Set the prescaler value used as the TIMx clock frequency
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Set the clock split:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM count up mode
    TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure); //Initializes the time base according to the specified parameter

    //Enable the specified TIM3 interrupt, allowing update interrupts
    

    //Set NVIC interrupt priority
    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;  //TIM3 interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //Excellent priority level 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //Slave priority level 2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ channel is enabled
    NVIC_Init(&NVIC_InitStructure);  //Initialize the NVIC register
		
		TIM_ITConfig(TIMER, TIM_IT_Update,ENABLE );
    TIM_Cmd(TIMER, ENABLE);  //TIMx enable
}





/**
  * @brief  This function handles USART IDLE interrupt.
  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef*UartHandle)  
//{  
//    if(UartHandle->Instance == USART2)  
//    {  
//				gizPutData((uint8_t *)&aRxBuffer, 1);

//        HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);//开启下一次接收中断  
//    }  
//}  
/**
* @brief USART2 interrupt handler

* Receive the serial protocol data from the WiFi module
* @param none
* @return none
*/
void USART2_IRQHandler(void)
{
//	GIZWITS_LOG("Gizwits1 \n");
    uint8_t value = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        value = USART_ReceiveData(USART2);

        gizPutData(&value, 1);
    }
}
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  /* 配置中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = RS232_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}
/**
* @brief USART init function

* Serial communication between WiFi modules and device MCU
* @param none
* @return none
*/
void uartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( RS232_USART_RX_GPIO_CLK|RS232_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(RS232_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(RS232_USART_RX_GPIO_PORT,RS232_USART_RX_SOURCE, RS232_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(RS232_USART_TX_GPIO_PORT,RS232_USART_TX_SOURCE,RS232_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS232_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS232_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS232_USART_RX_PIN;
  GPIO_Init(RS232_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串口RS232_USART 模式 */
  USART_InitStructure.USART_BaudRate = RS232_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(RS232_USART, &USART_InitStructure); 
	USART_Cmd(RS232_USART, ENABLE); 
	
	NVIC_Configuration();
	/*配置串口接收中断*/
	USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE);
	
  USART_ClearFlag(RS232_USART, USART_FLAG_TC);
	GIZWITS_LOG("Gizwits2 \n");
}






static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/**
* @brief Serial port write operation, send data to WiFi module
*
* @param buf      : buf address
* @param len      : buf length
*
* @return : Return effective data length;-1，return failure
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
//		uint8_t crc[1] = {0x55};
    uint32_t i = 0;
	
    if(NULL == buf)
    {
        return -1;
    }

    for(i=0; i<len; i++)
    {
         Usart_SendByte( USART2, *(buf + i) );
    }
//		GIZWITS_LOG("Gizwits3 \n");

#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);

        if(i >=2 && buf[i] == 0xFF)
        {
            GIZWITS_LOG("%02x ", 0x55);
        }
    }
    GIZWITS_LOG("\n");
#endif
		
		return len;
}  

extern int32_t testtime;
extern int32_t tests;
void TIM4_IRQHandler(void) //TIM4 中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查 TIM4 更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update ); //清除 TIMx 更新中断标志
		gizTimerMs();//系统毫秒定时
		
//		testtime++;
//		if(testtime==1000)
//		{
//			tests++;
//			GIZWITS_LOG("%d ",tests);
//			testtime=0;
//		}
		
		
	}
}
