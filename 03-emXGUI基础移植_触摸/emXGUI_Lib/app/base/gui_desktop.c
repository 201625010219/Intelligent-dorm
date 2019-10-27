/**
  *********************************************************************
  * @file    desktop.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   emXGUI ����
  *********************************************************************
  * ʵ��ƽ̨:Ұ��  ������
  * ����    :www.embedfire.com
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 



#include "emXGUI.h"


/* �ⲿ��Դ������ɱ�־ */
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
		
  /* ����APP���� */  
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
  * @brief  ���汳�����ƺ�����������������ʱ����ñ�������
            ͨ���޸ı����������ݿɸ�������ı���
  * @param  hdc ��ͼ������
  * @param  lprc Ҫ���Ƶľ�������ΪNULLʱ���Զ�����hwnd�Ŀͻ���
  * @param  hwnd ���ڶ�����
  * @retval ��
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

  /* �ָ�Ĭ������ */
  SetFont(hdc, defaultFont);

	SetBrushColor(hdc,MapRGB(hdc,32,72,144));
	FillRect(hdc,&rc);
  	
  SetTextColor(hdc,MapRGB(hdc,250,250,250));


//#if (GUI_EXTERN_FONT_EN || GUI_INER_CN_FONT_EN)
#if (1)

  /* ������ʾ��� */
	DrawText(hdc,L"HELLO,Intelligent dorm!",-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);  
#else
  /* ������ʾ��� */
	DrawText(hdc,L"Hello Intelligent dorm!",-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
#endif
  
	SetTextColor(hdc,MapRGB(hdc,250,250,250));
	TextOut(hdc,20,20,L"Intelligent STM32H750/H743 ",-1);

}

/* ʹ��ר�õ��߳����������� */
#if 0
static	int	gui_input_thread(void *p)
{
	while(1)
	{
		GUI_InputHandler(); //���������豸
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
  * @brief  ����ص�����
  * @param  hwnd ��ǰ�������Ϣ�Ĵ��ڶ�����
  * @param  msg ��Ϣ����ֵ�����Ա�ʶ�����ֵ�ǰ��������Ϣ
  * @param  wParam ��Ϣ����ֵ������msg��Ϣ����ֵ��ͬ
  * @param  lParam ��Ϣ����ֵ������msg��Ϣ����ֵ��ͬ
  * @retval ���ظ�SendMessage��ֵ
  */
static 	 LRESULT  	desktop_proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
    /* ���洴��ʱ,���������Ϣ,������������һЩ��ʼ������. */
		case	WM_CREATE:	
             {
			   //����1��20ms��ʱ��������ѭ���¼�.
				// SetTimer(hwnd,1,20,TMR_START,NULL);
                  GetClientRect(hwnd,&rc);
                  CreateWindow(BUTTON,L"EXIT",WS_VISIBLE,rc.w-100,8,80,48,hwnd,ID_EXIT,NULL,NULL);
				//����App�߳�						
			//	{
//#if (GUI_APP_BOOT_INTERFACE_EN )  
//          
//         GUI_Thread_Create(GUI_Boot_Interface_Dialog,  /* ������ں��� */
//                              "Boot_Interface",/* �������� */
//                              8*1024,  /* ����ջ��С */
//                              NULL, /* ������ں������� */
//                              5,    /* ��������ȼ� */
//                              10); /* ����ʱ��Ƭ����������֧�� */
//#else
//          
//        GUI_Thread_Create(gui_app_thread,  /* ������ں��� */
//                            "GUI_APP",/* �������� */
//                            2*1024,  /* ����ջ��С */
//                            NULL, /* ������ں������� */
//                            5,    /* ��������ȼ� */
//                            10); /* ����ʱ��Ƭ����������֧�� */    
//#endif
//				}
                    //������ 1 �鵥ѡ��ť(���� ID �ĸ� 16 λΪ GroupID �ţ�ֻ����ͬһ���ڵĵ�ѡ��ť���Ż���"��ѡһ"��Ч��) 
                                              
                 rc.x =20; 
                 rc.y =30; 
                 rc.w =120; 
                 rc.h =32; 
                 CreateWindow(BUTTON,L"Radiobox1",BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(1<<16)|ID_RB1,NULL,NULL); 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox2",WS_DISABLED|BS_RADIOBOX|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(1<<16)|ID_RB2,NULL,NULL); 

                 //������ 2 �鵥ѡ��ť(GroupID ��Ϊ 2,ʹ�ó��水ť���(BS_PUSHLIKE)). 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox3",BS_RADIOBOX|BS_PUSHLIKE|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(2<<16)|ID_RB3,NULL,NULL); 
                 OffsetRect(&rc,0,rc.h+10); 
                 CreateWindow(BUTTON,L"Radiobox4",BS_RADIOBOX|BS_PUSHLIKE|WS_VISIBLE, 
                      rc.x,rc.y,rc.w,rc.h,hwnd,(2<<16)|ID_RB4,NULL,NULL); 

                 //������ 3 �鵥ѡ��ť(GroupID ��Ϊ 3). 
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

		/* ��ʱ���������豸����Ϣ */
//		case	WM_TIMER:
//      #if(GUI_INPUT_DEV_EN)
//        {
//          uint16_t id;

//          id =LOWORD(wParam);
//          if(id==1)
//          {
//            GUI_InputHandler(); //���������豸
//          }
//        }
//      #endif
//		break;
        
         case  WM_NOTIFY: //WM_NOTIFY ��Ϣ:wParam �� 16 λΪ���͸���Ϣ�Ŀؼ� ID,�� 16 λΪ֪ͨ��;  lParam ָ����һ�� NMHDR �ṹ��. 
        { 
            u16 code,id; 

            id  =LOWORD(wParam); 
            code=HIWORD(wParam); 
            if(id >= ID_RB1 && id<= ID_RB6) 
            { 
                  if(code == BN_CHECKED) 
                { //��ѡ��ѡ��. 
                  GUI_Printf("Radiobox Checked: ID:%04XH\r\n",id); 
                } 
            }       
            if(id == ID_EXIT && code == BN_CLICKED) 
            { 
              PostCloseMessage(hwnd);//���� WM_CLOSE ��Ϣ�ر�������  
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
                cr->TextColor =RGB888(250,0,0);      //������ɫ��RGB32 ��ɫ��ʽ) 
                cr->BackColor =RGB888(220,200,200);    //������ɫ��RGB32 ��ɫ��ʽ) 
                cr->BorderColor =RGB888(30,30,230);  //�߿���ɫ��RGB32 ��ɫ��ʽ) 
                cr->ForeColor =RGB888(100,250,100);  //ǰ����ɫ��RGB32 ��ɫ��ʽ) 
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
           
          case  WM_PAINT: //������Ҫ�ػ���ʱ�����Զ��յ�����Ϣ. 
        {  
            PAINTSTRUCT ps;   
            HDC hdc; 
            hdc =BeginPaint(hwnd,&ps);            
            SetTextColor(hdc,MapRGB(hdc,0,0,255));  
            TextOut(hdc,8,4,L"Radiobox Test:",-1);   
            EndPaint(hwnd,&ps); 
            break;
        }    

//    /* �ͻ���������Ҫ������ */
//		case	WM_ERASEBKGND:
//		{   
//            RECT rc = *(RECT*)lParam;
//			HDC hdc =(HDC)wParam; 
//      
//            /* ������Դ������ɺ����ʾ�������棬�տ�ʼʱֻ��ʾ��ɫ */
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

    /* �û������ĵ���Ϣ����ϵͳ���� */
		default:
				return	DefDesktopProc(hwnd,msg,wParam,lParam);
	}

	return WM_NULL;

}

/**
  * @brief  ������������
  * @param  ��
  * @retval ��
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
	//�������洰��
	hwnd = GUI_CreateDesktop( WS_EX_LOCKPOS,
                              &wcex,
                              L"DESKTOP",
                               WS_CAPTION|WS_DLGFRAME|WS_BORDER|WS_CLIPCHILDREN,
                              0,0,GUI_XSIZE,GUI_YSIZE,
                              NULL,0,NULL,NULL);

//	GUI_DEBUG("HWND_Desktop=%08XH\r\n",	hwnd);

	//��ʾ���洰��.
	ShowWindow(hwnd,SW_SHOW);

#if (GUI_SHOW_CURSOR_EN)
	//����ϵͳ�򿪹����ʾ(���԰�ʵ��������Ƿ���Ҫ).
	ShowCursor(TRUE);
#endif

	while(GetMessage(&msg,hwnd))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/********************************END OF FILE****************************/

