/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.30                          *
*        Compiled Jul  1 2015, 10:50:32                              *
*        (c) 2015 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "./led/bsp_led.h"
extern GUI_CONST_STORAGE GUI_BITMAP bmON;
extern GUI_CONST_STORAGE GUI_BITMAP bmOFF;
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x00)
#define ID_BUTTON_0    (GUI_ID_USER + 0x01)
static WM_CALLBACK* _pcbOldFrame;
static unsigned int Flag =0 ;
// USER START (Optionally insert additional defines)
// USER END

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
  { FRAMEWIN_CreateIndirect, "DOOR", ID_FRAMEWIN_0, 2, -1, 800, 480, 0, 0x64, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 327, 188, 110, 40, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code

*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
static void _cbONButton(WM_MESSAGE * pMsg) 
{
	switch (pMsg->MsgId) 
	{
		case WM_PAINT:
		//	if (BUTTON_IsPressed(hWin)) 
		//	{
				if(Flag == 1)
				{
			 	 GUI_SetBkColor(GUI_GREEN);
			  	 GUI_Clear();
			     GUI_SetAlpha(0xb0);
				 GUI_DrawBitmap(&bmON, 0, 0);
                 LED3_ON;
				}
				else
				{
	               GUI_SetBkColor(GUI_RED);
			  	    GUI_Clear();
			        GUI_SetAlpha(0xb0);
                  	GUI_DrawBitmap(&bmOFF, 0, 0);
                     LED3_OFF;                    
				}
		//	} 
		
			break;
			
		default:
			BUTTON_Callback(pMsg);
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
                     GUI_SetAlpha(0XFF);
                      //LED2_ON;
                 }
            
			break;
	}
    if(_pcbOldFrame)(*_pcbOldFrame)(pMsg);
}
// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;

  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'DOOR'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetFont(hItem, GUI_FONT_32B_ASCII);
    FRAMEWIN_SetText(hItem, "DOOR");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
	FRAMEWIN_AddCloseButton(hItem, FRAMEWIN_BUTTON_RIGHT, 0); 
    _pcbOldFrame = WM_SetCallback(hItem, _cbFram); 
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0); //灯
    WM_SetHasTrans(hItem);
    WM_SetCallback(hItem, _cbONButton); 
  
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;

    case WM_PAINT:
	GUI_DrawGradientV(  0,			   /* 左上角X 位置 */
							0,			   /* 左上角Y 位置 */                    //purple
						    800,	   	       /* 右下角X 位置 */
							480,  	   	   /* 右下角Y 位置 */
							GUI_WHITE,	   /* 矩形最左侧要绘制的颜色 */
							GUI_BLUE);  /* 矩形最右侧要绘制的颜色 */
	break;

  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
		  Flag = !Flag;
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateDOOR
*/
WM_HWIN CreateCurtain(void);
WM_HWIN CreateCurtain(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/
