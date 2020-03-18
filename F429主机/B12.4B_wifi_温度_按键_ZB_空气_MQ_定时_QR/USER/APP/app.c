/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用1.5.1版本库建的工程模板
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "includes.h"

/*
*********************************************************************************************************
*                                         宏定义 
*********************************************************************************************************
*/
#define ICONVIEW_Width      70  /* 控件ICONVIEW的宽                         */  
#define ICONVIEW_Height     70  /* 控件ICONVIEW的高，包括图标的Y方向间距    */  
#define ICONVIEW_XSpace     30  /* 控件ICONVIEW的X方向间距                  */ 
#define ICONVIEW_YSpace     40  /* 控件ICONVIEW的Y方向间距                  */ 
#define ICONVIEW_XPos       10  /* 控件ICONVIEW的X方向调整距离              */
#define BOTWIN_YSpace       60  /* 屏幕下方部分窗口Y方向间距                */
#define MIDWIN_yPos         40  /* 中间窗口Y方向起始位置即顶部文字可用高度  */
#define MIDWIN_xPos          0  /* 中间窗口Y方向起始位置                    */

/*
*********************************************************************************************************
*                                      变量
*********************************************************************************************************
*/ 
extern GUI_CONST_STORAGE GUI_BITMAP bmIdea;
//extern GUI_CONST_STORAGE GUI_BITMAP bmCalculator;
extern GUI_CONST_STORAGE GUI_BITMAP bmClock;
//extern GUI_CONST_STORAGE GUI_BITMAP bmCompass;
//extern GUI_CONST_STORAGE GUI_BITMAP bmMessages;
//extern GUI_CONST_STORAGE GUI_BITMAP bmMusic;
//extern GUI_CONST_STORAGE GUI_BITMAP bmPhone;
//extern GUI_CONST_STORAGE GUI_BITMAP bmPhotos;
//extern GUI_CONST_STORAGE GUI_BITMAP bmVideos;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSafari;
extern GUI_CONST_STORAGE GUI_BITMAP bmWeather;
//extern GUI_CONST_STORAGE GUI_BITMAP bmwifi;
//extern GUI_CONST_STORAGE GUI_BITMAP bmSiri;
//extern GUI_CONST_STORAGE GUI_BITMAP bmCamera;
//extern GUI_CONST_STORAGE GUI_BITMAP bmUSB;
extern GUI_CONST_STORAGE GUI_BITMAP bmHome;
extern GUI_CONST_STORAGE GUI_BITMAP bmQR;
extern GUI_CONST_STORAGE GUI_BITMAP bmLOCK;

uint8_t UserApp_Flag=0;
extern RTC_TIME rtc_time;

uint8_t Flag_ICON0   = 0;   /* ICONVIEW控件按下的标志，0表示未按下，1表示按下 */
uint8_t Flag_ICON1   = 0;
uint8_t Flag_ICON2   = 0;
uint8_t Flag_ICON3   = 0;
uint8_t Flag_ICON4   = 0;
uint8_t Flag_ICON5   = 0;
uint8_t Flag_ICON6   = 0;
uint8_t Flag_ICON7   = 0;
uint8_t Flag_ICON8   = 0;

/* 用于桌面ICONVIEW图标的创建 */
typedef struct {
  const GUI_BITMAP * pBitmap;
  const char       * pText;
} BITMAP_ITEM;

typedef struct WIN_PARA{			//窗口使用到的用户定义参数，方便在回调函数中使用
	int xSizeLCD, ySizeLCD;			//LCD屏幕尺寸
	int xPosWin,  yPosWin;			//窗口的起始位置
	int xSizeWin, ySizeWin;			//窗口尺寸	
	int xSizeBM,  ySizeBM;
	WM_HWIN hWinMid;				//主显示窗的句柄（中部）	
}WIN_PARA;

WIN_PARA WinPara;			//用户使用的窗口额外数据

/* 用于桌面ICONVIEW0图标的创建 */
static const BITMAP_ITEM _aBitmapItem[] = 
{
  {&bmIdea,              "RGBLED"},
	{&bmWeather,     	    "Weather"},   
	{&bmClock,              "Clock"},
	{&bmHome,                "Home"}, 
	{&bmQR,                    "QR"}, 
	{&bmLOCK,                "LOCK"}, 
};

void FUN_ICON1Clicked(void)  {printf("FUN_ICON1Clicked\n");}
//void FUN_ICON8Clicked(void)  {printf("FUN_ICON7Clicked\n");}

/*
*********************************************************************************************************
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口的回调函数 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void _cbBkWindow(WM_MESSAGE * pMsg) 
{
  uint8_t sec, min, hour;
  char text_buffer[20]={0};
	switch (pMsg->MsgId) 
	{
		/* 重绘消息*/
		case WM_PAINT:		
#if   LCD_NUM_LAYERS == 1 
				GUI_SetBkColor(DTCOLOR);
#elif LCD_NUM_LAYERS == 2
          GUI_SetBkColor(GUI_TRANSPARENT);
#endif
				GUI_Clear();	
        ICONVIEW_SetSel(WM_GetDialogItem(WinPara.hWinMid, GUI_ID_ICONVIEW0),-1);
		break;
    case WM_TIMER:
      if(UserApp_Flag==1)
      {
        WM_RestartTimer(pMsg->Data.v, 1000);
        break; 
      }      
      if(!(bsp_result&BSP_RTC))
      {
        //printf("app timer\n");
        RTC_GetTime(RTC_Format_BIN, &rtc_time.RTC_Time);
        sec    =  rtc_time.RTC_Time.RTC_Seconds;
        min    =  rtc_time.RTC_Time.RTC_Minutes;
        hour   =  rtc_time.RTC_Time.RTC_Hours;
        sprintf(text_buffer,"%02d:%02d:%02d",hour,min,sec);
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT1),text_buffer);
      }
      sprintf(text_buffer,"CPU:%02d%%",(uint8_t)(OSStatTaskCPUUsage/100));
      TEXT_SetText(WM_GetDialogItem(pMsg->hWin, GUI_ID_TEXT2),text_buffer);
      WM_RestartTimer(pMsg->Data.v, 1000);
      break;
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}
/*
*********************************************************************************************************
*	函 数 名: _cbMidWin
*	功能说明: 
*	形    参：WM_MESSAGE * pMsg
*	返 回 值: 无
*********************************************************************************************************
*/
void _cbMidWin(WM_MESSAGE * pMsg) 
{
	int NCode, Id;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);      /* Id of widget */
			NCode = pMsg->Data.v;                 /* Notification code */
			switch (Id) 
			{
				case GUI_ID_ICONVIEW0:
					switch (NCode) 
					{
						/* ICON控件点击消息 */
						case WM_NOTIFICATION_CLICKED:
              UserApp_Flag=1;
							break;
						
						/* ICON控件释放消息 */
						case WM_NOTIFICATION_RELEASED: 							
							/* 打开相应选项 */
							switch(ICONVIEW_GetSel(pMsg->hWinSrc))
							{
								
								/* RGBLED *******************************************************************/
								case 0:	
									Flag_ICON0 = 1;
									FUN_ICON0Clicked();			
									break;	
								
								/* Weather *******************************************************************/
								case 1:
									Flag_ICON3 = 1;
									FUN_ICON3Clicked();
									break;
								
								/* Clock ***********************************************************************/
								case 2:
									Flag_ICON5 = 1;
									FUN_ICON5Clicked();
									break;
								
								/* Home *******************************************************************/
								case 3:
									Flag_ICON6 = 1;
									FUN_ICON6Clicked();
									break;         
								/* Home *******************************************************************/
								case 4:
									Flag_ICON7 = 1;
									FUN_ICON7Clicked();
									break;   
								
								/* LOCK *******************************************************************/
								case 5:
									Flag_ICON8 = 1;
									FUN_ICON8Clicked();
									break;    								

								}
							 break;
						}
					break;
			}
			break;
			
		/* 重绘消息*/
		case WM_PAINT:
			{
#if   LCD_NUM_LAYERS == 1 
				GUI_SetBkColor(DTCOLOR);
#elif LCD_NUM_LAYERS == 2
        GUI_SetBkColor(GUI_TRANSPARENT);
#endif
				GUI_Clear();
			}			
		break;			
	 default:
		WM_DefaultProc(pMsg);
		break;
	}
}


/**
  * @brief  CreateTopWin，创建顶部的窗口
  * @param  none
  * @retval none
  */
static void CreateTopWin(void)
{
	WM_HWIN hText;
	/* 顶部的 "wildfire"文本 */
	hText = TEXT_CreateEx(0, 2, 200 , 30, HDTWIN, WM_CF_SHOW|WM_CF_MEMDEV, GUI_TA_LEFT|TEXT_CF_VCENTER, GUI_ID_TEXT0, "");
	TEXT_SetFont(hText,GUI_FONT_24B_ASCII);
	TEXT_SetText(hText,"IOT System");
	TEXT_SetTextColor(hText,GUI_WHITE);
	
	/* 状态栏的时间显示文本 */
	hText = TEXT_CreateEx(WinPara.xSizeLCD/2-50,2,100,30,HDTWIN,WM_CF_SHOW|WM_CF_MEMDEV,GUI_TA_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT1,"11:56:00");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_WHITE);
	TEXT_SetFont(hText,GUI_FONT_20B_ASCII);
  
  	/* 状态栏的cpu显示文本 */
	hText = TEXT_CreateEx(WinPara.xSizeLCD-200,2,200,30,HDTWIN,WM_CF_SHOW|WM_CF_MEMDEV,GUI_TA_RIGHT|TEXT_CF_VCENTER,GUI_ID_TEXT2,"CPU: 10%");
	//TEXT_SetBkColor(hText,GUI_INVALID_COLOR);
	TEXT_SetTextColor(hText,GUI_WHITE);
	TEXT_SetFont(hText,GUI_FONT_24B_ASCII);
}
/**
  * @brief  CreateMidWin，创建中间的窗口
  * @param  none
  * @retval none
  */
static void CreateMidWin(void)
{
	uint8_t i=0;
	WM_HWIN MIDWIN;
	WinPara.hWinMid= WM_CreateWindowAsChild(
											WinPara.xPosWin,											
											WinPara.yPosWin,
											WinPara.xSizeWin,
											WinPara.ySizeWin,	
											HDTWIN, 
											WM_CF_SHOW|WM_CF_MEMDEV,
											_cbMidWin, 
											sizeof(WIN_PARA *)
											);
/*-------------------------------------------------------------------------------------*/
					/*在指定位置创建指定尺寸的ICONVIEW1 小工具*/
					MIDWIN=ICONVIEW_CreateEx(
											 ICONVIEW_XPos, 					          /* 小工具的最左像素（在父坐标中）*/
											 0, 								                /* 小工具的最上像素（在父坐标中）*/
											 WinPara.xSizeLCD-ICONVIEW_XPos,	  /* 小工具的水平尺寸（单位：像素）*/
											 3*(WinPara.ySizeBM+35), 			      /* 小工具的垂直尺寸（单位：像素）*/
											 WinPara.hWinMid, 				          /* 父窗口的句柄。如果为0 ，则新小工具将成为桌面（顶级窗口）的子窗口 */
											 WM_CF_SHOW|WM_CF_HASTRANS,         /* 窗口创建标记。为使小工具立即可见，通常使用 WM_CF_SHOW */ 
											 0,//ICONVIEW_CF_AUTOSCROLLBAR_V, 	/* 默认是0，如果不够现实可设置增减垂直滚动条 */
											 GUI_ID_ICONVIEW0, 			            /* 小工具的窗口ID */
											 ICONVIEW_Width, 				            /* 图标的水平尺寸 */
											 ICONVIEW_Height+20);				        /* 图标的垂直尺寸 */
											 
		/* 向ICONVIEW 小工具添加新图标 */
		for (i = 0; i < GUI_COUNTOF(_aBitmapItem); i++) 
		{
			ICONVIEW_AddBitmapItem(MIDWIN, _aBitmapItem[i].pBitmap,_aBitmapItem[i].pText);
			ICONVIEW_SetTextColor(MIDWIN,i,GUI_WHITE);
		}	
		/* 设置小工具的背景色 32 位颜色值的前8 位可用于alpha混合处理效果*/
		ICONVIEW_SetBkColor(MIDWIN, ICONVIEW_CI_SEL, 0xffffc0 | 0x80000000);
		/* 设置字体 */
		ICONVIEW_SetFont(MIDWIN, GUI_FONT_16B_ASCII);
		/* 设置初始选择的图标为 -1 (表示尚未选择)*/
		ICONVIEW_SetSel(MIDWIN,-1);
		/* 设置图标在x 或y 方向上的间距。*/
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_X, ICONVIEW_XSpace);
		ICONVIEW_SetSpace(MIDWIN, GUI_COORD_Y, ICONVIEW_YSpace);
		/* 设置对齐方式 在5.22版本中最新加入的 */
		ICONVIEW_SetIconAlign(MIDWIN, ICONVIEW_IA_HCENTER|ICONVIEW_IA_TOP);
		
/*-------------------------------------------------------------------------------------*/
}

/*
*********************************************************************************************************
*	函 数 名: emWinMainApp
*	功能说明: GUI主函数 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void emWinMainApp(void)
{
  FRAMEWIN_SKINFLEX_PROPS FRAMEWIN_pProps;
  RADIO_SKINFLEX_PROPS    RADIO_pProps;

#if LCD_NUM_LAYERS == 2
  /*  如果使用双层显示功能需要配置上层为通透的，不然下层无法显示 */
  GUI_SelectLayer(1);
  GUI_SetBkColor(GUI_TRANSPARENT);
  GUI_Clear();
#endif
  
	//准备建立3个窗口，以下是使用到的用户定义参数，方便在回调函数中使用
	WinPara.xSizeLCD = LCD_GetXSize();				            //LCD屏幕尺寸
	WinPara.ySizeLCD = LCD_GetYSize();				            //LCD屏幕尺寸
	WinPara.xSizeBM  = ICONVIEW_Width;				            //图标宽度
	WinPara.ySizeBM  = ICONVIEW_Height;				            //图标高度
	WinPara.xPosWin	 = MIDWIN_xPos;							          //窗口的起始位置
	WinPara.yPosWin  = MIDWIN_yPos;							          //窗口的起始位置
	WinPara.xSizeWin = WinPara.xSizeLCD;						      //窗口尺寸
	WinPara.ySizeWin = WinPara.ySizeLCD-WinPara.yPosWin;	//窗口尺寸
  
  PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
  
  /***************************定制皮肤*****************************/
  FRAMEWIN_GetSkinFlexProps(&FRAMEWIN_pProps,FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_pProps.aColorFrame[0]=GUI_BLACK;
  FRAMEWIN_pProps.aColorFrame[1]=GUI_BLACK;
  FRAMEWIN_pProps.aColorFrame[2]=GUI_BLACK;
  FRAMEWIN_pProps.Radius=0;
  FRAMEWIN_pProps.aColorTitle[0]=GUI_BLACK;
  FRAMEWIN_pProps.aColorTitle[1]=GUI_BLACK;
  FRAMEWIN_SetSkinFlexProps(&FRAMEWIN_pProps,FRAMEWIN_SKINFLEX_PI_ACTIVE);
  FRAMEWIN_SetDefaultClientColor(APPBKCOLOR);
  FRAMEWIN_SetDefaultFont(&XBF_XINSONGTI19);
  FRAMEWIN_SetDefaultTextColor(1,TEXTCOLOR);
  FRAMEWIN_SetDefaultTitleHeight(30);
  FRAMEWIN_SetDefaultTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);

  WINDOW_SetDefaultBkColor(APPBKCOLOR);
  
  EDIT_SetDefaultBkColor(EDIT_CI_ENABLED,APPBKCOLOR);
  EDIT_SetDefaultTextColor(EDIT_CI_ENABLED,TEXTCOLOR);
  
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_UNSEL,APPBKCOLOR);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SEL,GUI_BLUE);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SELFOCUS,GUI_BLUE);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_DISABLED,APPBKCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_UNSEL,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SEL,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SELFOCUS,TEXTCOLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_DISABLED,TEXTCOLOR);
  
  TEXT_SetDefaultTextColor(TEXTCOLOR);
  
  RADIO_SetDefaultTextColor(TEXTCOLOR);
  RADIO_GetSkinFlexProps(&RADIO_pProps,0);
  RADIO_pProps.aColorButton[3]=GUI_YELLOW;
  RADIO_SetSkinFlexProps(&RADIO_pProps,0);
  
  CHECKBOX_SetDefaultTextColor(TEXTCOLOR);
  
	/*************************************************************************/
	WM_SetCallback(HDTWIN, _cbBkWindow);
	WM_CreateTimer(HDTWIN, 0, 1000, 0);
	/* 创建三个窗口 状态栏、控制栏、主窗口*/
	CreateMidWin();
	CreateTopWin();
  GUI_Delay(20);
#if LCD_NUM_LAYERS == 1
  LCD_BL_ON();
#endif
	while(1)
	{
		GUI_Delay(5);
	}
}
/*************************** End of file ****************************/
