/**
  *********************************************************************
  * @file    desktop.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   emXGUI 桌面
  *********************************************************************
  * 实验平台:野火  开发板
  * 官网    :www.embedfire.com
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 



#include "emXGUI.h"


/* 外部资源加载完成标志 */
BOOL Load_state = FALSE;

/*===================================================================================*/
extern void	GUI_Boot_Interface_Dialog(void *param);
extern void GUI_AppMain(void);


void	gui_app_thread(void *p)
{
    #if(GUI_TOUCHSCREEN_EN & GUI_TOUCHSCREEN_CALIBRATE)
    {
        int i=0;
        while(TouchPanel_IsPenDown())
    	{
    		GUI_msleep(100);
    		if(i++>10)
    		{
    			ShowCursor(FALSE);
    			TouchScreenCalibrate(NULL);
    			ShowCursor(TRUE);
    			break;
    		}
    	}
    }
    #endif
		
  /* 调用APP函数 */  
//	GUI_AppMain();
 //   GUI_UserAppStart();
//   	ShellWindowStartup();
    while(1)
    {
//      GUI_DEBUG("gui_app_thread");
      GUI_msleep(500);
    }
}

/*===================================================================================*/

/**
  * @brief  桌面背景绘制函数，触发背景绘制时会调用本函数，
            通过修改本函数的内容可更改桌面的背景
  * @param  hdc 绘图上下文
  * @param  lprc 要绘制的矩形区域，为NULL时会自动绘制hwnd的客户区
  * @param  hwnd 窗口对象句柄
  * @retval 无
  */
static	void	_EraseBackgnd(HDC hdc,const RECT *lprc,HWND hwnd)
{
	RECT rc;

	if(lprc==NULL)
	{
		GetClientRect(hwnd,&rc);
	}
	else
	{
		CopyRect(&rc,lprc);
	}

  /* 恢复默认字体 */
  SetFont(hdc, defaultFont);

	SetBrushColor(hdc,MapRGB(hdc,32,72,144));
	FillRect(hdc,&rc);
  	
  SetTextColor(hdc,MapRGB(hdc,250,250,250));


//#if (GUI_EXTERN_FONT_EN || GUI_INER_CN_FONT_EN)
#if (1)

  /* 居中显示结果 */
	DrawText(hdc,L"HELLO,Intelligent dorm!",-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);  
#else
  /* 居中显示结果 */
	DrawText(hdc,L"Hello Intelligent dorm!",-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#endif
  
	SetTextColor(hdc,MapRGB(hdc,250,250,250));
	TextOut(hdc,20,20,L"Intelligent STM32H750/H743 ",-1);

}

/* 使用专用的线程来处理输入 */
#if 0
static	int	gui_input_thread(void *p)
{
	while(1)
	{
		GUI_InputHandler(); //处理输入设备
		GUI_msleep(20);
	}
}
#endif
 #define ID_EXIT   0x1100
 #define ID_RB1    0x1101 
 #define ID_RB2    0x1102 
 #define ID_RB3    0x1103 
 #define ID_RB4    0x1104 
 #define ID_RB5    0x1105 
 #define ID_RB6    0x1106 
RECT rc;

/**
  * @brief  桌面回调函数
  * @param  hwnd 当前处理该消息的窗口对象句柄
  * @param  msg 消息类型值，用以标识和区分当前产生的消息
  * @param  wParam 消息参数值，根据msg消息代码值不同
  * @param  lParam 消息参数值，根据msg消息代码值不同
  * @retval 返回给SendMessage的值
  */
static 	 LRESULT  	desktop_proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
    /* 桌面创建时,会产生该消息,可以在这里做一些初始化工作. */
		case	WM_CREATE:	
             {
			   //创建1个20ms定时器，处理循环事件.
				// SetTimer(hwnd,1,20,TMR_START,NULL);
                  GetClientRect(hwnd,&rc);
                  CreateWindow(BUTTON,L"EXIT",WS_VISIBLE,rc.w-100,8,80,48,hwnd,ID_EXIT,NULL,NULL);
				//创建App线程						
			//	{
//#if (GUI_APP_BOOT_INTERFACE_EN )  
//          
//         GUI_Thread_Create(GUI_Boot_Interface_Dialog,  /* 任务入口函数 */
//                              "Boot_Interface",/* 任务名字 */
//                              8*1024,  /* 任务栈大小 */
//                              NULL, /* 任务入口函数参数 */
//                              5,    /* 任务的优先级 */
//                              10); /* 任务时间片，部分任务不支持 */
//#else
//          
//        GUI_Thread_Create(gui_app_thread,  /* 任务入口函数 */
//                            "GUI_APP",/* 任务名字 */
//                            2*1024,  /* 任务栈大小 */
//                            NULL, /* 任务入口函数参数 */
//                            5,    /* 任务的优先级 */
//                            10); /* 任务时间片，部分任务不支持 */    
//#endif
//				}
                    //创建第 1 组单选按钮(窗口 ID 的高 16 位为 GroupID 号，只有在同一组内的单选按钮，才会有"多选一"的效果) 
                                              
                 rc.x =20; 
                 rc.y =30; 
                 rc.w =120; 
                 rc.h =32; 
                 CreateWindow(BUTTON,L"Radiobox1",BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(1<<16)|ID_RB1,NULL,NULL); 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox2",WS_DISABLED|BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(1<<16)|ID_RB2,NULL,NULL); 

                 //创建第 2 组单选按钮(GroupID 号为 2,使用常规按钮风格(BS_PUSHLIKE)). 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox3",BS_RADIOBOX|BS_PUSHLIKE|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(2<<16)|ID_RB3,NULL,NULL); 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox4",BS_RADIOBOX|BS_PUSHLIKE|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(2<<16)|ID_RB4,NULL,NULL); 

                 //创建第 3 组单选按钮(GroupID 号为 3). 
                 rc.x =160; 
                 rc.y =30; 
                 rc.w =120; 
                 rc.h =32; 
                 CreateWindow(BUTTON,L"Radiobox5",BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(3<<16)|ID_RB5,NULL,NULL); 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox6",BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(3<<16)|ID_RB6,NULL,NULL); 
                 return TRUE; 
             }
				//break;

		/* 定时处理输入设备的信息 */
//		case	WM_TIMER:
//      #if(GUI_INPUT_DEV_EN)
//        {
//          uint16_t id;

//          id =LOWORD(wParam);
//          if(id==1)
//          {
//            GUI_InputHandler(); //处理输入设备
//          }
//        }
//      #endif
//		break;
        
         case  WM_NOTIFY: //WM_NOTIFY 消息:wParam 低 16 位为发送该消息的控件 ID,高 16 位为通知码;  lParam 指向了一个 NMHDR 结构体. 
        { 
            u16 code,id; 

            id  =LOWORD(wParam); 
            code=HIWORD(wParam); 
            if(id >= ID_RB1 && id<= ID_RB6) 
            { 
                  if(code == BN_CHECKED) 
                { //单选框选中. 
                  GUI_Printf("Radiobox Checked: ID:%04XH\r\n",id); 
                } 
            }       
            if(id == ID_EXIT && code == BN_CLICKED) 
            { 
              PostCloseMessage(hwnd);//产生 WM_CLOSE 消息关闭主窗口  
            }   
            break;   
        } 
        
        case    WM_CTLCOLOR:
        {
           uint16_t id;
           id =LOWORD(wParam); 
           if(id == ID_RB1 || id == ID_RB2)
           {
              CTLCOLOR *cr;
              cr = (CTLCOLOR*)lParam;
              if(SendMessage(GetDlgItem(hwnd,id),BM_GETSTATE,0,0)&BST_CHECKED)
              {
                cr->TextColor =RGB888(250,0,0);      //文字颜色（RGB32 颜色格式) 
                cr->BackColor =RGB888(220,200,200);    //背景颜色（RGB32 颜色格式) 
                cr->BorderColor =RGB888(30,30,230);  //边框颜色（RGB32 颜色格式) 
                cr->ForeColor =RGB888(100,250,100);  //前景颜色（RGB32 颜色格式) 
              } 
              else 
              { 
                cr->TextColor =RGB888(20,20,250); 
                cr->BackColor =RGB888(200,220,200); 
                cr->BorderColor =RGB888(50,50,50); 
                cr->ForeColor =RGB888(180,200,230); 
              } 
           }
              return TRUE; 
         } 
           
          case  WM_PAINT: //窗口需要重绘制时，会自动收到该消息. 
        {  
            PAINTSTRUCT ps;   
            HDC hdc; 
            hdc =BeginPaint(hwnd,&ps);            
            SetTextColor(hdc,MapRGB(hdc,0,0,255));  
            TextOut(hdc,8,4,L"Radiobox Test:",-1);   
            EndPaint(hwnd,&ps); 
            break;
        }    

//    /* 客户区背景需要被擦除 */
//		case	WM_ERASEBKGND:
//		{   
//            RECT rc = *(RECT*)lParam;
//			HDC hdc =(HDC)wParam; 
//      
//            /* 字体资源加载完成后才显示正常界面，刚开始时只显示纯色 */
//           if(Load_state == TRUE)
//           {
//             // _EraseBackgnd(hdc,NULL,hwnd);
//           }
//           else
//           {
//             // SetBrushColor(hdc, MapRGB(hdc, 0, 0, 0));
//              //FillRect(hdc, &rc);
//           }
//		}
//		return TRUE;  

    /* 用户不关心的信息，由系统处理 */
		default:
				return	DefDesktopProc(hwnd,msg,wParam,lParam);
	}

	return WM_NULL;

}

/**
  * @brief  桌面启动函数
  * @param  无
  * @retval 无
  */
void GUI_DesktopStartup(void)
{
	WNDCLASS	wcex;
	HWND hwnd;
	MSG msg;

	wcex.Tag 		    = WNDCLASS_TAG;
	wcex.Style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= desktop_proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;//hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);

//  GUI_DEBUG("Create desktop");
	//创建桌面窗口
	hwnd = GUI_CreateDesktop( WS_EX_LOCKPOS,
                              &wcex,
                              L"DESKTOP",
                               WS_CAPTION|WS_DLGFRAME|WS_BORDER|WS_CLIPCHILDREN,
                              0,0,GUI_XSIZE,GUI_YSIZE,
                              NULL,0,NULL,NULL);

//	GUI_DEBUG("HWND_Desktop=%08XH\r\n",	hwnd);

	//显示桌面窗口.
	ShowWindow(hwnd,SW_SHOW);

#if (GUI_SHOW_CURSOR_EN)
	//设置系统打开光标显示(可以按实际情况看是否需要).
	ShowCursor(TRUE);
#endif

	while(GetMessage(&msg,hwnd))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/********************************END OF FILE****************************/

