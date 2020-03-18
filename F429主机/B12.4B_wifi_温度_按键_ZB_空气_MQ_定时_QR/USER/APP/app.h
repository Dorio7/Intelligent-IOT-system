#ifndef	_APP_H_
#define	_APP_H_

#include "includes.h"
#include "GUI.h"
#include "DIALOG.h"
#include "MESSAGEBOX.h"
#include  <os.h>

//
// Redefine number of layers for this configuration file. Must be equal or less than in GUIConf.h!
//
#undef  LCD_NUM_LAYERS
#define LCD_NUM_LAYERS    2               //设置显示层数

#if   LCD_NUM_LAYERS == 1                 //如果仅有一次使用WM_HBKWIN为主显示层
  #define HDTWIN      WM_HBKWIN
#elif LCD_NUM_LAYERS == 2
  #define HDTWIN      WM_GetDesktopWindowEx(1)
#else
  #error LCD_NUM_LAYERS must be set to 1 or 2.
#endif


#define DTCOLOR         0X200000
#define TEXTCOLOR       GUI_WHITE
#define APPBKCOLOR  		GUI_DARKGRAY
#define APPTEXTCOLOR    GUI_WHITE
/*
*********************************************************************************************************
*                                              Public data
*********************************************************************************************************
*/
extern uint8_t   tpad_flag;
extern uint8_t   UserApp_Flag;

extern FATFS     fs[2];
extern FIL       f_file;
extern FRESULT   f_result;
extern UINT      f_num;

extern GUI_FONT     XBF_XINSONGTI25;
extern GUI_FONT     XBF_XINSONGTI19;

/************ KeypadInput **************/
extern uint8_t  keypadflag;
extern WM_HWIN  hkeypad;
void   KeyPad_Interface(uint16_t hwin,uint8_t flag);
/************************************/

extern uint8_t Flag_ICON0;
extern void    FUN_ICON0Clicked(void);

//extern uint8_t Flag_ICON1;
//extern void    FUN_ICON1Clicked(void);

//extern uint8_t Flag_ICON2;
//extern void    FUN_ICON2Clicked(void);

extern uint8_t Flag_ICON3;
extern void    FUN_ICON3Clicked(void);

extern uint8_t Flag_ICON4;
extern void    FUN_ICON4Clicked(void);

extern uint8_t Flag_ICON5;
extern void    FUN_ICON5Clicked(void);

extern uint8_t Flag_ICON6;
extern void    FUN_ICON6Clicked(void);

extern uint8_t Flag_ICON7;
extern void    FUN_ICON7Clicked(void);

extern uint8_t Flag_ICON8;
extern void    FUN_ICON8Clicked(void);

//extern uint8_t Flag_ICON9;
//extern void    FUN_ICON9Clicked(void);

//extern uint8_t Flag_ICON10;
//extern void    FUN_ICON10Clicked(void);

//extern uint8_t Flag_ICON11;
//extern void    FUN_ICON11Clicked(void);

//extern uint8_t Flag_ICON12;
//extern void    FUN_ICON12Clicked(void);



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void emWinMainApp(void);
#endif	//_APP_H_

