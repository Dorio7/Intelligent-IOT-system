#include  "app.h"

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0        (GUI_ID_USER + 0x00)

#define ID_BUTTON_0 (GUI_ID_USER + 0x01)
#define ID_BUTTON_1 (GUI_ID_USER + 0x02)
#define ID_BUTTON_2 (GUI_ID_USER + 0x05)
#define ID_TEXT_0 (GUI_ID_USER + 0x03)
#define ID_TEXT_1 (GUI_ID_USER + 0x04)
#define SWPOS_x 0 //开关x坐标

static WM_HTIMER hTimerTime;  //定时更新300ms


//按键
GUI_BITMAP buttonbmp_tab[2];
extern GUI_CONST_STORAGE GUI_BITMAP bmdd2;
//位图********************************************************************************************************************************
extern GUI_CONST_STORAGE GUI_BITMAP bmBUTTONOFF;
extern GUI_CONST_STORAGE GUI_BITMAP bmBUTTONON;
extern GUI_CONST_STORAGE GUI_BITMAP bmwendu;
extern GUI_CONST_STORAGE GUI_BITMAP bmSD7;//湿度
extern GUI_CONST_STORAGE GUI_BITMAP bmKQZL;//空气质量
extern GUI_CONST_STORAGE GUI_BITMAP bmGLDS;//功率定时
extern uint32_t IOT_Time_OnOff;
extern uint32_t IOT_Time_Off_Minute;


extern GUI_CONST_STORAGE GUI_BITMAP bmsdtx;//除湿器文本

extern uint32_t IOT_CSQ_FLAG;//除湿器判断

extern RTC_TIME rtc_time;//时间

//机智云IOT
extern uint32_t IOT_DHT11_wendu;//温度
extern uint32_t valueDHT11_shidu;//湿度
extern uint8_t E1ST;
extern uint8_t E2ST;
extern float POW_T;
extern int pow_x;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Home", ID_FRAMEWIN_0, 0, 0, 800, 480, 0, 0x0, 0 },
	
	//LED按钮
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, SWPOS_x+60, 122, 110, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, SWPOS_x+60, 251, 110, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, SWPOS_x, 132, 60, 20, 0, 0x64, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_1, SWPOS_x, 261, 60, 20, 0, 0x64, 0 },
	
	//时间
	{ TEXT_CreateIndirect,     "Clock",       GUI_ID_TEXT0,    300, 12,  200, 40,  0, 0x64, 0 },

	//湿度
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT1, 87, 375, 60, 20, 0, 0x64, 0 },
	
	//功率
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT2, 710, 115, 60, 20, 0, 0x64, 0 },
	
	//定时
	{ TEXT_CreateIndirect, "Text", GUI_ID_TEXT3, 710, 145, 60, 20, 0, 0x64, 0 },
	
	//除湿器
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 610, 251, 110, 40, 0, 0x0, 0 },
	
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


/*********************************************************************
*
*       _cbDialog回调函数
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
	  uint8_t sec, min, hour;
	  char text_buffer[20]={0};
		char text1_buffer[2]={0};
  // USER END

switch (pMsg->MsgId) {
		case WM_DELETE:
		// 关闭
		OS_INFO("HomeDLG delete\n");
		Flag_ICON6 = 0;
		UserApp_Flag = 0;
    tpad_flag=0;  
		WM_DeleteTimer(hTimerTime);//关定时更新定时器

	  break;
  case WM_INIT_DIALOG://初始化
		
			// 创建关闭按键
			hItem = pMsg->hWin;
			FRAMEWIN_SetText(hItem,"HOME");
			FRAMEWIN_AddCloseButton(hItem,FRAMEWIN_BUTTON_RIGHT,0);

			//初始化BUTTON0
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetBitmapEx(hItem,0,&buttonbmp_tab[0],0,0);
			BUTTON_SetText(hItem, "");
			
			//初始化BUTTON1
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetBitmapEx(hItem,0,&buttonbmp_tab[0],0,0);
			BUTTON_SetText(hItem, "");
			
			//初始化TEXT0
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "SW1");
			
			//初始化TEXT1
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
			TEXT_SetText(hItem, "SW2");
			
			//时间显示文本
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0);
			TEXT_SetFont(hItem, GUI_FONT_24B_1);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetTextColor(hItem, GUI_CYAN);
			RTC_GetTime(RTC_Format_BIN, &rtc_time.RTC_Time);
        sec    =  rtc_time.RTC_Time.RTC_Seconds;
        min    =  rtc_time.RTC_Time.RTC_Minutes;
        hour   =  rtc_time.RTC_Time.RTC_Hours;
			sprintf(text_buffer,"%02d:%02d:%02d",hour,min,sec);
			TEXT_SetText(hItem, text_buffer);
			
			//初始化湿度文本
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
			TEXT_SetText(hItem, "50");
	
			//初始化功率文本
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII );
			TEXT_SetText(hItem, "0 W");
	
			//初始化定时文本
			hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3);
			TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
			TEXT_SetFont(hItem, GUI_FONT_24_ASCII );
			TEXT_SetText(hItem, "OFF");
			
			
			//初始化BUTTON2
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetBitmapEx(hItem,0,&buttonbmp_tab[0],0,0);
			BUTTON_SetText(hItem, "");
			
    break;
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    
		switch(Id) 
			{
				case ID_BUTTON_0: //BUTTON_0的通知代码，控制LED1
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED: //按钮被按下并释放
								LED1_TOGGLE;
								if(E1ST)
									E1ST=0;
								else
									E1ST=1;
								BUTTON_SetBitmapEx(hItem,0,E1ST?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);	
							break;	
					}
					break;
				case ID_BUTTON_1: //BUTTON_1的通知代码，控制BEEP
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
								LED1_TOGGLE;
						
								if(E2ST)
									E2ST=0;
								else
									E2ST=1;
								
								BUTTON_SetBitmapEx(hItem,0,E2ST?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);	
							break;
					}
					break;
					
					case ID_BUTTON_2: //BUTTON_2的通知代码，控制BEEP
					hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
					switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							
								LED1_TOGGLE;
						
								if(IOT_CSQ_FLAG)
									IOT_CSQ_FLAG=0;
								else
									IOT_CSQ_FLAG=1;
								
								BUTTON_SetBitmapEx(hItem,0,IOT_CSQ_FLAG?&buttonbmp_tab[0]:&buttonbmp_tab[1],0,0);	
							break;
					}
					break;
			}
			break;
		
		//定时刷新
	  case WM_TIMER:
			
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
			BUTTON_SetBitmapEx(hItem,0,E1ST?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);	
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
			BUTTON_SetBitmapEx(hItem,0,E2ST?&buttonbmp_tab[1]:&buttonbmp_tab[0],0,0);	
		
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
			BUTTON_SetBitmapEx(hItem,0,IOT_CSQ_FLAG?&buttonbmp_tab[0]:&buttonbmp_tab[1],0,0);	
		
//			printf("IOT %d\n",IOT_CSQ_FLAG);
//			printf("E %d\n",E2ST);
		
			RTC_GetTime(RTC_Format_BIN, &rtc_time.RTC_Time);
			sec    =  rtc_time.RTC_Time.RTC_Seconds;
			min    =  rtc_time.RTC_Time.RTC_Minutes;
			hour   =  rtc_time.RTC_Time.RTC_Hours;
			sprintf(text_buffer,"%02d:%02d:%02d",hour,min,sec);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT0),text_buffer);
		
		
			sprintf(text1_buffer,"%02d",valueDHT11_shidu);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),text1_buffer);
		
			if(E1ST)
				sprintf(text1_buffer,"0 W");
			else
				sprintf(text1_buffer,"1.%d W",pow_x);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),text1_buffer);
			
			if(IOT_Time_OnOff==1&&E1ST==0)
				sprintf(text1_buffer,"%d",IOT_Time_Off_Minute);
			else
				sprintf(text1_buffer,"OFF");
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT3),text1_buffer);
			
			
			WM_RestartTimer(hTimerTime,200);

    break;
		
		case WM_PAINT:			 
    GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		//上部渐变线
		GUI_DrawGradientH(400, 0, 800,3,0xF5F200, 0x000000);
		GUI_DrawGradientH(000, 0, 400,3 ,0x000000, 0xF5F200);
		//底部智慧物联系统标题
		GUI_DrawBitmap(&bmdd2,265,404);
		//温度底图
		GUI_DrawBitmap(&bmwendu,250,50);
		//湿度底图
		GUI_DrawBitmap(&bmSD7,55,330);
		//空气质量底图
		GUI_DrawBitmap(&bmKQZL,610,330);
		//功率定时
		GUI_DrawBitmap(&bmGLDS,550,100);
		//除湿器文本
		GUI_DrawBitmap(&bmsdtx,575,187);
		
		
		//温度指针
    GUI_SetColor(0xF5F200);  
		GUI_SetPenSize(4);
		GUI_DrawLine(398, /* X 起始位置 */
								 200, /* Y 起始位置 */
							 	 398+55*cos((125.0-(IOT_DHT11_wendu/40.0)*140.0)/180*3.1416), /* X 结束位置 */
							   200-55*sin((125.0-(IOT_DHT11_wendu/40.0)*140.0)/180*3.1416)); /* Y 结束位置 */
		GUI_SetPenSize(18);
		GUI_DrawPoint(398,200);
		
    GUI_SetFont(&XBF_XINSONGTI19);

		break;

  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


/*********************************************************************
*
*       CreateHome
*/
void FUN_ICON6Clicked(void){

  WM_HWIN hWin;
	OS_INFO("HomeDLG create\n");
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, HDTWIN, 0, 0);
	
	buttonbmp_tab[1]=bmBUTTONOFF;
	buttonbmp_tab[0]=bmBUTTONON;
	
  hTimerTime=WM_CreateTimer(WM_GetClientWindow(hWin), 0, 200, 0); //定时更新
	
	while(Flag_ICON6)
  {
    if(tpad_flag)WM_DeleteWindow(hWin);  
    GUI_Delay(5);
  }
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
