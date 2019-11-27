#include <stddef.h>
#include "DIALOG.h"
#include "keypad.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "WM.h"
/*GSMģ��ͷ�ļ�*/
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./gsm_gprs/bsp_gsm_usart.h"

/*FreeRTOSͷ�ļ�*/
#include "FreeRTOS.h"
#include "task.h"


WM_HWIN hMulti=0;       //�����ı�
WM_HWIN hButton=0;       //����
KEYPADStructTypeDef keypad_dev;		
static WM_CALLBACK* _pcbOldFrame;

static int _DrawSkinFlex_BUTTON(const WIDGET_ITEM_DRAW_INFO * pDrawItemInfo); 

	
typedef struct 
{
	int          xPos;              //��ťXλ��
	int          yPos;              //��ťYλ��
	int          xSize;             //��ť���
	int          ySize;             //��ť�߶�
	const char * acLabel;           //��ť��������ַ�
	void (* pfDraw)(WM_HWIN hWin);  //��ť�Ļ��ƺ���
    const char * ButString;         //��ť����ʾ���ַ�
} BUTTON_DATA;






//���ּ���λ��
static  BUTTON_DATA _aNumButtonData[] = {
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE1YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "+",0,"+" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "1" ,0,"1"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "2" ,0,"2"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "3" ,0,"3"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE1YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "\x08",0,"Delete"},	//ɾ����
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE2YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "-",0,"-" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "4" ,0,"4"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "5" ,0,"5"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "6" ,0,"6"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE2YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, " " ,0,"Space"},
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE3YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "/",0,"/" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "7" ,0,"7"},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "8" ,0,"8"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "9" ,0,"9"},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE3YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "\x0d",0,"Enter"},	//�س���
  
	{ (NUM_BUTTON_XPAC*1+NUM_BUTTON_WIDTH*0+NUM_BUTTON_XOFFSET),  NUM_BUTTON_LINE4YPOS,(NUM_BUTTON_WIDTH-NUM_BUTTONSING_XINC),NUM_BUTTON_HEIGHT, "@",0,"@" },
	{ (NUM_BUTTON_XPAC*2+NUM_BUTTON_WIDTH*1+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "," ,0,","},
	{ (NUM_BUTTON_XPAC*3+NUM_BUTTON_WIDTH*2+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "0" ,0,"0"},
	{ (NUM_BUTTON_XPAC*4+NUM_BUTTON_WIDTH*3+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "." ,0,"."},
	{ (NUM_BUTTON_XPAC*5+NUM_BUTTON_WIDTH*4+NUM_BUTTON_XOFFSET-NUM_BUTTONSING_XINC),  NUM_BUTTON_LINE4YPOS, NUM_BUTTON_WIDTH,NUM_BUTTON_HEIGHT, "",0,"Call" },//���ؼ�
};



//�������ּ���
void drawnumber_pad(WM_HWIN hWin)
{
	uint16_t i;
	for(i=0;i<GUI_COUNTOF(_aNumButtonData);i++)								//�������ּ���
	{
		//������ť
		BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); //����BUTTON��Ƥ��
		keypad_dev.NumButton_Handle[i] = BUTTON_CreateEx(_aNumButtonData[i].xPos, _aNumButtonData[i].yPos, _aNumButtonData[i].xSize, _aNumButtonData[i].ySize, 
                             hWin, WM_CF_SHOW | WM_CF_HASTRANS, 0, ID_BUTTON + i);
		BUTTON_SetFont(keypad_dev.NumButton_Handle[i],GUI_FONT_16B_ASCII);
		BUTTON_SetText(keypad_dev.NumButton_Handle[i], _aNumButtonData[i].ButString);
		BUTTON_SetFocussable(keypad_dev.NumButton_Handle[i], 0);
	}
}




char phone_buff[12];
//���ּ��̴�����
//buttondata:���ּ��̰�ť����
//Id:��ťID
//*pMsg:ָ����Ϣ
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
              vTaskSuspendAll();//�����ٽ���
              while(gsm_init()!= GSM_TRUE)
                {
                  printf("\r\nģ����Ӧ���Բ���������\r\n");
                  printf("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");
                        GSM_DELAY(500);

                }
    
    printf("\r\nͨ����ģ����Ӧ���ԣ�5���ʼ���Ų���...\r\n");
    
    /* ��ʱ3���ٷ������ģ�� */
    GSM_DELAY(5000);
    
            //����绰����
             MULTIEDIT_GetText(hMulti,phone_buff,sizeof(phone_buff));
             printf("%s\n",phone_buff); //���ܵ绰����
             gsm_call(phone_buff);                           //����绰
             rev_phone = gsm_waitask(0);
            if(strstr(rev_phone,"ATD") != NULL)           //��ӦOK����ʾģ���������յ�����
            {
              printf("\r\n���ں���\r\n");
              GSM_CLEAN_RX();                     //������ջ�����
              rev_phone = gsm_waitask(0);            //���µȴ���Ϣ
              printf("\r\n���ں���\r\n");
              printf("%s\n",rev_phone);
              if(strstr(rev_phone,"NO CARRIER") != NULL) //��ӦNO CARRIER,ͨ������
               {
                 printf("\r\nͨ������\r\n");
               }
              else if(strstr(rev_phone,"NO ANSWER") != NULL)   //��ӦNO ANSWER�����˽���
              {
                printf("\r\n�㲦��ĵ绰��ʱ���˽��������Ժ��ٲ�\r\n");
              }  
            }
            GSM_HANGOFF();
            GSM_CLEAN_RX();                     //������ջ�����
            xTaskResumeAll();//�˳��ٽ���
         }
        else
        {
            GUI_StoreKeyMsg(c, 1);				//����Ϣ������̻�����������״̬
			GUI_StoreKeyMsg(c, 0);				//����Ϣ������̻��������ɿ�״̬ 
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

//����BUTTONҪʹ�õ�Ƥ��
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
				//����һ��С�İ������ڣ����ǲ��ɼ�
				hWin = WM_CreateWindowAsChild(0, 0, 16, 16, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, NULL, 0);
				WM_SelectWindow(hWin);
				//Ϊbutton��ÿ����Ե�����洢�豸
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
					//�����Ŵ��豸
					hMem = GUI_MEMDEV_CreateFixed(0, 0, 16, 16, GUI_MEMDEV_NOTRANS, GUI_MEMDEV_APILIST_32, GUI_COLOR_CONV_8888);
					GUI_MEMDEV_Select(hMem);
					GUI_SetBkColor(GUI_TRANSPARENT);
					GUI_Clear();
					GUI_SetColor(COLOR_BORDER);
					GUI_DrawRoundedFrame(x, y, x + 31, y + 31, 16, 4);	//���ƻ�����İ�ť�Ľǵ�Բ��

					//������ָ���Ĵ�С
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
			//�ڰ�ť����ʾ�Զ������Ĭ�ϵ���ɫ�����ı�
			hWin = pDrawItemInfo->hWin;
			Id   = WM_GetId(hWin);
			i    = Id - ID_BUTTON;

             pfDraw = _aNumButtonData[i].pfDraw;	
			if (pfDraw) pfDraw(hWin);
			else BUTTON_DrawSkinFlex(pDrawItemInfo);
			break;
		case WIDGET_ITEM_DRAW_BACKGROUND:
			//���ư�ť�ı���
			IsPressed = BUTTON_IsPressed(pDrawItemInfo->hWin); 	//�ж��Ƿ�ť�Ƿ񱻰���
			xPos      = WM_GetWindowOrgX(pDrawItemInfo->hWin);	//��ȡ��ťС���ߵ�X����ԭ��λ��
			yPos      = WM_GetWindowOrgY(pDrawItemInfo->hWin);	//��ȡ��ťС���ߵ�Y����ԭ��λ��
			xSize     = WM_GetWindowSizeX(pDrawItemInfo->hWin);	//��ȡ��ťС���ߵ�X��С
			ySize     = WM_GetWindowSizeY(pDrawItemInfo->hWin);	//��ȡ��ťС���ߵ�Y��С
			if (IsPressed) 										//�����ť������
			{
				GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR2, BUTTON_COLOR3);//���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ���
			} 
			else 
			{
				GUI_DrawGradientRoundedV(0, 0, xSize - 1, ySize - 1, 4, BUTTON_COLOR0, BUTTON_COLOR1);//���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ���
			}
			GUI_SetColor(COLOR_BORDER);
			//���ư�ť�������߽�
			GUI_DrawHLine(        0, 4, xSize - 5);
			GUI_DrawHLine(ySize - 1, 4, xSize - 5);
			GUI_DrawVLine(        0, 4, ySize - 5);
			GUI_DrawVLine(xSize - 1, 4, ySize - 5);
			//�������洢�豸������д�뵱ǰѡ���洢�豸���ƶ�λ��	
            butheigh = 68;
			GUI_MEMDEV_WriteAt(ahMemEdges[0], xPos +  0, yPos +  0);			    //��ť���Ͻ�Բ��
			GUI_MEMDEV_WriteAt(ahMemEdges[1], xPos + xSize - 4, yPos +  0);		    //��ť���Ͻ�Բ��
			GUI_MEMDEV_WriteAt(ahMemEdges[2], xPos + xSize - 4, yPos + butheigh);   //��ť���½�Բ��
			GUI_MEMDEV_WriteAt(ahMemEdges[3], xPos +  0, yPos + butheigh);			//��ť���½�Բ��
			break;
		default:
			return BUTTON_DrawSkinFlex(pDrawItemInfo);//���������ʹ��Ĭ�ϵ�Ƥ��
	}
	return 0;
}




//keypad���ڵ��õĻص�����
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
		case WM_CREATE:				//����Ϣ�ڴ��ڴ������������ͣ��Ǵ��ڿ��Գ�ʼ���������κ��Ӵ���
			drawnumber_pad(hWin);	//��������
			break;
		case WM_PAINT:
			//���Ʊ���
			xSize = WM_GetWindowSizeX(hWin);
			ySize = WM_GetWindowSizeY(hWin);
			GUI_SetColor(COLOR_BORDER);
			GUI_DrawRect(0, 0, xSize - 1, ySize - 1);
		//	GUI_DrawGradientV(1, 1, xSize - 2, ySize - 2, COLOR_KEYPAD0, COLOR_KEYPAD1);
            GUI_DrawGradientV(  0,			   /* ���Ͻ�X λ�� */
							0,			   /* ���Ͻ�Y λ�� */                    //purple
						    800,	   	       /* ���½�X λ�� */
							480,  	   	   /* ���½�Y λ�� */
							GUI_WHITE,	   /* ���������Ҫ���Ƶ���ɫ */
							GUI_BLUE);  /* �������Ҳ�Ҫ���Ƶ���ɫ */
			break;
		case WM_NOTIFY_PARENT:	//���߸����ڣ��Ӵ��ڷ����˸ı�
			//�뵱ǰ���㴰�ڷ��Ͱ�����Ϣ
			Id    = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;
			switch (NCode) 
			{
				case WM_NOTIFICATION_RELEASED:				//��ť���ͷ�
				
					//���ּ���
					{
						numkeypad_process(_aNumButtonData,Id,pMsg);		//�������ּ���
					}
                
					break;
			}
			break;	
	
	}
}


//�������ڻص�����
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
	
	WM_SetCallback(WM_HBKWIN, _cbBk);		        //��ָ�������ڻص�����
	
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
	
	//����keypad��ʹ�õİ�ť��Ƥ��
	BUTTON_SetDefaultSkin(_DrawSkinFlex_BUTTON); 	//����BUTTON��Ƥ��
	//����keypad
	keypad_dev.hKeypad = WM_CreateWindowAsChild(keypad_dev.xpos,keypad_dev.ypos,keypad_dev.width,keypad_dev.height, WM_HBKWIN, WM_CF_SHOW | WM_CF_STAYONTOP, _cbKeyPad, 0);
	
	//����notepad����
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	FRAMEWIN_SetDefaultTextAlign(GUI_TA_HCENTER);

	//����FRAME����
	hFrame = FRAMEWIN_CreateEx(0, 0, 800, 480, WM_HBKWIN, WM_CF_SHOW, 0, 0, "", 0);
    FRAMEWIN_SetText(hFrame, "Phone");
    FRAMEWIN_SetTitleHeight(hFrame, 40);
    FRAMEWIN_SetFont(hFrame, GUI_FONT_32B_ASCII);
    FRAMEWIN_SetTextAlign(hFrame, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_SetClientColor(hFrame, GUI_WHITE);
	FRAMEWIN_AddCloseButton(hFrame, FRAMEWIN_BUTTON_RIGHT, 0); 
    _pcbOldFrame = WM_SetCallback(hFrame, _cbFram); 
    
	//����һ��multi edit(�����ı�С����)С����
	hMulti = MULTIEDIT_CreateEx(0, 0, 0, 0, WM_GetClientWindow(hFrame), WM_CF_SHOW, 0, GUI_ID_MULTIEDIT0, 100, NULL);
	MULTIEDIT_EnableBlink(hMulti,500,1);			//�������,����500ms
    MULTIEDIT_SetInsertMode(hMulti,1);  //��������ģʽ
	MULTIEDIT_SetFont(hMulti,GUI_FONT_16B_ASCII);
	WM_SetFocus(hMulti);
	

}


