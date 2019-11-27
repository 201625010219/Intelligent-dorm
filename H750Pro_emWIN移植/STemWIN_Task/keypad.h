#ifndef _SOFTWAREKEY_H
#define _SOFTWAREKEY_H
#include "DIALOG.h"
#include "stm32h7xx.h"
//键盘类型，包括英文键盘，数字键盘，符号键盘，九宫格中文输入法键盘
typedef enum
{
	ENGLISH_KEYPAD,		//英文键盘
	NUMBER_KEYPAD,		//数字键盘
	SIGN_KEYPAD,		//符号键盘
	CHINESE_KEYPAD,	    //九宫格中文输入法键盘
	KEYPAD_NUM			//键盘数量
}keypad_type;

//keypad配置结构体
typedef struct 
{
	keypad_type padtype;				//键盘类型
	uint16_t xpos;							//软键盘X坐标		
	uint16_t ypos;							//软键盘Y坐标
	uint16_t width;							//软键盘宽度
	uint16_t height;							//软键盘高度
	WM_HWIN hKeypad;					//软键盘窗口句柄
	WM_HWIN EngButton_Handle[35];		//英文键盘上按钮句柄
	WM_HWIN NumButton_Handle[20];		//数字键盘上按钮句柄
	WM_HWIN SignButton_Handle[20];		//符号键盘上按钮句柄
    WM_HWIN ChineseButton_Handle[25];   //中文键盘上按钮句柄
    uint8_t      inputstr[7];                //输入字符串，最大6个字符+结束符
    uint8_t      inputlen;                   //输入长度
    uint8_t      cur_index;                  //当前拼音索引
    uint8_t      result_num;                 //拼音匹配数
    uint8_t      pypagenum;                  //拼音页数
    uint8_t      pynowpage;                  //当前选中的拼音页
    uint8_t      dispagenum;                 //汉字显示总页数
    uint8_t      dispagecur;                 //当前汉字显示页
    uint8_t      *disstr;                    //横向显示的字符串指针
    uint8_t      *cur_hz;                    //当前选中的汉字
    uint8_t      hznum;                      //选中的拼音对应的汉字数量
    uint8_t      disbuf[6][3];               //横向按钮缓存
    uint8_t      *butname[6];                //按钮名字
    uint8_t      sta;                        //记录输入法状态
                                        //bit7: 0,第一次进入中文输入法;1,前面已经进入了中文输入法
	uint8_t LowerCase;						//0 大写;1 小写
	uint8_t signpad_flag;					//符号键盘索引
	uint8_t signpad_num;						//符号键盘的数量
	
} KEYPADStructTypeDef;


#define COLOR_BACK0            0xFF3333
#define COLOR_BACK1            0x550000
#define COLOR_BORDER           0x444444
#define COLOR_KEYPAD0          0xAAAAAA
#define COLOR_KEYPAD1          0x555555
#define BUTTON_COLOR0          0xEEEEEE		
#define BUTTON_COLOR1          0xCCCCCC  	
#define BUTTON_COLOR2          0xCCCCCC
#define BUTTON_COLOR3          0xAAAAAA
#define BUTTON_SKINFLEX_RADIUS 4

#define ID_BUTTON              (GUI_ID_USER + 0)
#define ID_BUTTON_0            (GUI_ID_USER + 0x30)
//自定义消息	
#define CHANGE_ENGLISHPADTO_NUMPAD	   	(WM_USER+0)			




//数字键盘按键参数
#define NUM_BUTTON_WIDTH			90	//数字键盘按钮宽度
#define NUM_BUTTON_HEIGHT			45	//数字键盘按钮高度
#define NUM_BUTTON_XPAC				0	//数字键盘按钮X轴间距
#define NUM_BUTTON_XOFFSET			130	//数字键盘每行按钮X轴偏移

#define NUM_BUTTON_LINE1YPOS		7	//数字键盘第一行Y坐标
#define NUM_BUTTON_LINE2YPOS		47	//数字键盘第二行Y坐标
#define NUM_BUTTON_LINE3YPOS		87	//数字键盘第三行Y坐标
#define NUM_BUTTON_LINE4YPOS		127	//数字键盘第四行Y坐标

#define NUM_BUTTONSING_XINC			15	//数字键盘中符号的X轴长度增量
#define NUM_BUTTONBACKSPACE_XINC	15	//数字键盘中删除键的X轴长度增量
#define NUM_BUTTONSPACE_XINC		15	//数字键盘中空格键的X轴长度增量
#define NUM_BUTTONENTER_XINC		15  //数字键盘中回车键的X轴长度增量
#define NUM_BUTTONRETURN_XINC		15	//数字键盘中返回键的X轴长度增量







void keypad_demo(void);
#endif
