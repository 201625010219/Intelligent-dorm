#include <stddef.h>
#include "DIALOG.h"
#include "keypad.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "WM.h"
/*GSM模块头文件*/
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./gsm_gprs/bsp_gsm_usart.h"

/*FreeRTOS头文件*/
#include "FreeRTOS.h"
#include "task.h"


WM_HWIN hMulti=0;       //多行文本
WM_HWIN hButton=0;       //按键
KEYPADStructTypeDef keypad_dev;		
static WM_CALLBACK* _pcbOldFrame;

static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo); 

	
typedef struct 
{
	int          xPos;              //按钮X位置
	int          yPos;              //按钮Y位置
	int          xSize;             //按钮宽度
	int          ySize;             //按钮高度
	const char * acLabel;           //按钮所代表的字符
	void (* pfDraw)(WM_HWIN hWin);  //按钮的绘制函数
    const char * ButString;         //按钮上显示的字符
} BUTTON_DATA;






//数字键盘位置
static  BUTTON_DATA _aNumButtonData[] = {
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE1YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "+",0,"+" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "1" ,0,"1"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "2" ,0,"2"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "3" ,0,"3"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "\x08",0,"Delete"},	//删除键
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE2YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "-",0,"-" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "4" ,0,"4"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "5" ,0,"5"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "6" ,0,"6"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, " " ,0,"Space"},
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE3YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "/",0,"/" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "7" ,0,"7"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "8" ,0,"8"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "9" ,0,"9"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "\x0d",0,"Enter"},	//回车键
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE4YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "@",0,"@" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "," ,0,","},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "0" ,0,"0"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "." ,0,"."},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "",0,"Call" },//返回键
};



//绘制数字键盘
void drawnumber_pad(WM_HWIN hWin)
{
	uint16_t i;
	for(i=0;i<GUI_COUNTOF(_aNumButtonData);i++)								//创建数字键盘
	{
		//创建按钮
		BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); //设置BUTTON的皮肤
		keypad_dev.NumButton_Handle[i] = BUTTON_CreateEx(_aNumButtonData[i].xPos, _aNumButtonData[i].yPos, _aNumButtonData[i].xSize, _aNumButtonData[i].ySize, 
                             hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
		BUTTON_SetFont(keypad_dev.NumButton_Handle[i],GUI_FONT_16B_ASCII);
		BUTTON_SetText(keypad_dev.NumButton_Handle[i], _aNumButtonData[i].ButString);
		BUTTON_SetFocussable(keypad_dev.NumButton_Handle[i], 0);
	}
}




char phone_buff[12];
//数字键盘处理函数
//buttondata:数字键盘按钮参数
//Id:按钮ID
//*pMsg:指向消息
void numkeypad_process(BUTTON_DATA *buttondata,int Id,WM_MESSAGE *pMsg)	
{
	char 		c;
    uint8_t          KeyID;
    char *rev_phone;
    KeyID=Id-ID_BUTTON;
	 if (buttondata[Id - ID_BUTTON].acLabel) 
	{
		c = buttondata[Id - ID_BUTTON].acLabel[0];

        if(KeyID == 19)                          
        {
              vTaskSuspendAll();//进入临界区
              while(gsm_init()!= GSM_TRUE)
                {
                  printf("\r\n模块响应测试不正常！！\r\n");
                  printf("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");
                        GSM_DELAY(500);

                }
    
    printf("\r\n通过了模块响应测试，5秒后开始拨号测试...\r\n");
    
    /* 延时3秒再发送命令到模块 */
    GSM_DELAY(5000);
    
            //拨打电话按键
             MULTIEDIT_GetText(hMulti,phone_buff,sizeof(phone_buff));
             printf("%s\n",phone_buff); //接受电话号码
             gsm_call(phone_buff);                           //拨打电话
             rev_phone = gsm_waitask(0);
            if(strstr(rev_phone,"ATD") != NULL)           //响应OK，表示模块正常接收到命令
            {
              printf("\r\n正在呼叫\r\n");
              GSM_CLEAN_RX();                     //清除接收缓冲区
              rev_phone = gsm_waitask(0);            //重新等待消息
              printf("\r\n正在呼叫\r\n");
              printf("%s\n",rev_phone);
              if(strstr(rev_phone,"NO CARRIER") != NULL) //响应NO CARRIER,通话结束
               {
                 printf("\r\n通话结束\r\n");
               }
              else if(strstr(rev_phone,"NO ANSWER") != NULL)   //响应NO ANSWER，无人接听
              {
                printf("\r\n你拨打的电话暂时无人接听，请稍后再拨\r\n");
              }  
            }
            GSM_HANGOFF();
            GSM_CLEAN_RX();                     //清除接收缓冲区
            xTaskResumeAll();//退出临界区
         }
        else
        {
            GUI_StoreKeyMsg(c, 1);				//把消息存进键盘缓冲器，按下状态
			GUI_StoreKeyMsg(c, 0);				//把消息存进键盘缓冲器，松开状态 
        }
	}
}

// USER START (Optionally insert additional static code)
static void _cbFram(WM_MESSAGE * pMsg) 
{
    int Id;
	switch (pMsg->MsgId) 
	{
		case WM_NOTIFY_PARENT:
                 Id = WM_GetId(pMsg->hWinSrc); 
                 if(Id == GUI_ID_CLOSE)
                 {
                     WM_DeleteWindow(keypad_dev.hKeypad);
                 }
            
			break;
	}
    if(_pcbOldFrame)(*_pcbOldFrame)(pMsg);
}

//设置BUTTON要使用的皮肤
static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo) 
{
	static GUI_MEMDEV_Handle ahMemEdges[4];
	GUI_MEMDEV_Handle        hMem;
	WM_HWIN                  hWin;
	unsigned                 i;
	int                      xPos;
	int                      yPos;
	int                      xSize;
	int                      ySize;
	int                      x;
	int                      y;
	int                      Id;
	int                      IsPressed;
    int                      butheigh;
	void (* pfDraw)(WM_HWIN hWin);

	x = 0;
	y = 0;
	switch (pDrawItemInfo->Cmd) 
	{
		case WIDGET_ITEM_CREATE:
			if (ahMemEdges[0] == 0) 
			{
				//创建一个小的帮助窗口，但是不可见
				hWin = WM_CreateWindowAsChild(0, 0, 16, 16, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, NULL, 0);
				WM_SelectWindow(hWin);
				//为button的每个边缘创建存储设备
				for (i = 0; i < GUI_COUNTOF(ahMemEdges); i++) 
				{
					switch (i) 
					{
						case 0:
							x = 0;
							y = 0;
							break;
						case 1:
							x = -16;
							y = 0;
							break;
						case 2:
							x = -16;
							y = -16;
							break;
						case 3:
							x = 0;
							y = -16;
							break;
					}
					//创建放大设备
					hMem = GUI_MEMDEV_CreateFixed(0, 0, 16, 16, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
					GUI_MEMDEV_Select(hMem);
					GUI_SetBkColor(GUI_TRANSPARENT);
					GUI_Clear();
					GUI_SetColor(COLOR_BORDER);
					GUI_DrawRoundedFrame(x, y, x + 31, y + 31, 16, 4);	//绘制换肤后的按钮四角的圆弧

					//收缩到指定的大小
					ahMemEdges[i] = GUI_MEMDEV_CreateFixed(0, 0, 4, 4, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
					GUI_MEMDEV_Select(ahMemEdges[i]);
					GUI_SetBkColor(GUI_TRANSPARENT);
					GUI_Clear();
					GUI_MEMDEV_RotateHQ(hMem, ahMemEdges[i], -6, -6, 0, 250);
					GUI_MEMDEV_Delete(hMem);
					GUI_MEMDEV_Select(0);
				}
				WM_DeleteWindow(hWin);
			}
			break;
		case WIDGET_ITEM_DRAW_TEXT:
			//在按钮上显示自定义或者默认的颜色或者文本
			hWin = pDrawItemInfo->hWin;
			Id   = WM_GetId(hWin);
			i    = Id - ID_BUTTON;

             pfDraw = _aNumButtonData[i].pfDraw;	
			if (pfDraw) pfDraw(hWin);
			else BUTTON_DrawSkinFlex(pDrawItemInfo);
			break;
		case WIDGET_ITEM_DRAW_BACKGROUND:
			//绘制按钮的背景
			IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin); 	//判断是否按钮是否被按下
			xPos      = WM_GetWindowOrgX(pDrawItemInfo->hWin);	//获取按钮小工具的X方向原点位置
			yPos      = WM_GetWindowOrgY(pDrawItemInfo->hWin);	//获取按钮小工具的Y方向原点位置
			xSize     = WM_GetWindowSizeX(pDrawItemInfo->hWin);	//获取按钮小工具的X大小
			ySize     = WM_GetWindowSizeY(pDrawItemInfo->hWin);	//获取按钮小工具的Y大小
			if (IsPressed) 										//如果按钮被按下
			{
				GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR2, BUTTON_COLOR3);//绘制垂直颜色梯度填充的圆角矩形
			} 
			else 
			{
				GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR0, BUTTON_COLOR1);//绘制垂直颜色梯度填充的圆角矩形
			}
			GUI_SetColor(COLOR_BORDER);
			//绘制按钮的四条边界
			GUI_DrawHLine(        0, 4, xSize - 5);
			GUI_DrawHLine(ySize - 1, 4, xSize - 5);
			GUI_DrawVLine(        0, 4, ySize - 5);
			GUI_DrawVLine(xSize - 1, 4, ySize - 5);
			//将给定存储设备的内容写入当前选定存储设备的制定位置	
            butheigh = 68;
			GUI_MEMDEV_WriteAt(ahMemEdges[0], xPos +  0, yPos +  0);			    //按钮左上角圆弧
			GUI_MEMDEV_WriteAt(ahMemEdges[1], xPos + xSize - 4, yPos +  0);		    //按钮右上角圆弧
			GUI_MEMDEV_WriteAt(ahMemEdges[2], xPos + xSize - 4, yPos + butheigh);   //按钮右下角圆弧
			GUI_MEMDEV_WriteAt(ahMemEdges[3], xPos +  0, yPos + butheigh);			//按钮左下角圆弧
			break;
		default:
			return BUTTON_DrawSkinFlex(pDrawItemInfo);//其他命令就使用默认的皮肤
	}
	return 0;
}




//keypad窗口调用的回调函数
static void _cbKeyPad(WM_MESSAGE * pMsg) 
{
	WM_HWIN    hWin;
	int        Id;
	int        NCode;
	int        xSize;
	int        ySize;


	hWin = pMsg->hWin;
	switch (pMsg->MsgId) 
	{
		case WM_CREATE:				//此消息在窗口创建后立即发送，是窗口可以初始化并创建任何子窗口
			drawnumber_pad(hWin);	//创建键盘
			break;
		case WM_PAINT:
			//绘制背景
			xSize = WM_GetWindowSizeX(hWin);
			ySize = WM_GetWindowSizeY(hWin);
			GUI_SetColor(COLOR_BORDER);
			GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
		//	GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
            GUI_DrawGradientV(  0,			   /* 左上角X 位置 */
							0,			   /* 左上角Y 位置 */                    //purple
						    800,	   	       /* 右下角X 位置 */
							480,  	   	   /* 右下角Y 位置 */
							GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							GUI_BLUE);  /* 矩形最右侧要绘制的颜色 */
			break;
		case WM_NOTIFY_PARENT:	//告诉父窗口，子窗口发生了改变
			//想当前焦点窗口发送按键消息
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_RELEASED:				//按钮被释放
				
					//数字键盘
					{
						numkeypad_process(_aNumButtonData,Id,pMsg);		//处理数字键盘
					}
                
					break;
			}
			break;	
	
	}
}


//背景窗口回调函数
static void _cbBk(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
			GUI_DrawGradientV(0, 0,480,480, COLOR_BACK0, COLOR_BACK1);
			break;
	}
}


void keypad_demo(void) 
{
	WM_HWIN hFrame;
	
	WM_SetCallback(WM_HBKWIN, _cbBk);		        //是指背景窗口回调函数
	
	keypad_dev.xpos=0;
	keypad_dev.ypos=(480*6)/10;
	keypad_dev.width=800;
	keypad_dev.height=480-((480*6)/10);
	keypad_dev.signpad_flag=0;	
	keypad_dev.signpad_num=2;
    keypad_dev.inputlen=0;
    keypad_dev.pynowpage=0;
    keypad_dev.cur_index=0;
    keypad_dev.sta=0;
	
	//设置keypad所使用的按钮的皮肤
	BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); 	//设置BUTTON的皮肤
	//创建keypad
	keypad_dev.hKeypad = WM_CreateWindowAsChild(keypad_dev.xpos,keypad_dev.ypos,keypad_dev.width,keypad_dev.height, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, _cbKeyPad, 0);
	
	//设置notepad属性
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);

	//创建FRAME窗口
	hFrame = FRAMEWIN_CreateEx(0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, 0, 0, "", 0);
    FRAMEWIN_SetText(hFrame, "Phone");
    FRAMEWIN_SetTitleHeight(hFrame, 40);
    FRAMEWIN_SetFont(hFrame, GUI_FONT_32B_ASCII);
    FRAMEWIN_SetTextAlign(hFrame, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetClientColor(hFrame, GUI_WHITE);
	FRAMEWIN_AddCloseButton(hFrame, FRAMEWIN_BUTTON_RIGHT, 0); 
    _pcbOldFrame = WM_SetCallback(hFrame, _cbFram); 
    
	//创建一个multi edit(多行文本小工具)小工具
	hMulti = MULTIEDIT_CreateEx(0, 0, 0, 0, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, GUI_ID_MULTIEDIT0, 100, NULL);
	MULTIEDIT_EnableBlink(hMulti,500,1);			//开启光标,周期500ms
    MULTIEDIT_SetInsertMode(hMulti,1);  //开启插入模式
	MULTIEDIT_SetFont(hMulti,GUI_FONT_16B_ASCII);
	WM_SetFocus(hMulti);
	

}


