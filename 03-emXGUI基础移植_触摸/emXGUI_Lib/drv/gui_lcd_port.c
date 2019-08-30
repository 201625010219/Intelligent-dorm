/**
  *********************************************************************
  * @file    gui_lcd_port.c
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   Һ�������ӿ�
  *********************************************************************
  * @attention
  * ����    :www.emXGUI.com
  *
  **********************************************************************
  */ 
  
#include "board.h"
#include "gui_drv.h"
#include "gui_drv_cfg.h"
#include "gui_lcd_port.h"
/*============================================================================*/


extern void DMA2D_DrvInit(void);


/**
  * @brief  Һ������ʼ���ӿ�
  * @param  fb_addr Ҫʹ�õ��Դ��ַ
  */
void LCD_HardInit(u32 fb_addr)
{	
  /* ��ʼ��Һ���� */  
#if	(LCD_FORMAT == COLOR_FORMAT_RGB565)
  LCD_Init(fb_addr, 0, LTDC_PIXEL_FORMAT_RGB565);
#endif
  
#if	(LCD_FORMAT == COLOR_FORMAT_XRGB8888)
  LCD_Init(fb_addr, 0, LTDC_PIXEL_FORMAT_ARGB8888);
#endif

}

/**
  * @brief  Һ��������ƽӿ�
  * @param  on 1Ϊ��������ֵΪ��
  */
void LCD_BkLight(int on)
{
  LCD_BackLed_Control(on);
}


/**
  * @brief  ��ʼ����ʾ�豸�ӿڣ�������SURFACE����
  * @param  ��
  * @retval ��ʾ�豸Surface����ָ�룬�����õ��Ļ�ͼ����
  */
SURFACE* GUI_DisplayInit(void)
{
  /* ��ͼ���� */
	SURFACE *pSurf;

#if	(LCD_FORMAT == COLOR_FORMAT_RGB565)

  //������ͼ����
  /* ��̬����ķ�ʽ  */
// pSurf = GUI_CreateSurface(SURF_RGB565, 
//                              LCD_XSIZE,LCD_YSIZE,
//                              LCD_XSIZE*2,
//                              NULL);  
                              
  /* ֱ��ָ����ַ�ķ�ʽ�� �Դ��ַ��*/
 pSurf = GUI_CreateSurface(SURF_RGB565, 
                              LCD_XSIZE,LCD_YSIZE,
                              LCD_XSIZE*2,
                              (void*)LCD_FRAME_BUFFER);    

#endif

#if	(LCD_FORMAT == COLOR_FORMAT_XRGB8888)
	//��̬����ķ�ʽ����ʼ��LCD Surface�ṹ����(XRGB8888��ʽ)
	//lcd_buffer =(u8*)GUI_GRAM_Alloc(LCD_XSIZE,LCD_YSIZE*4);
  
  /* ֱ��ָ����ַ�ķ�ʽ�� �Դ��ַ��*/
  /* ��̬����ķ�ʽ  */
// pSurf = GUI_CreateSurface(SURF_XRGB8888, 
//                              LCD_XSIZE,LCD_YSIZE,
//                              LCD_XSIZE*4,
//                              NULL);  
                          
  pSurf = GUI_CreateSurface(SURF_XRGB8888, 
                              LCD_XSIZE,LCD_YSIZE,
                              LCD_XSIZE*4,
                              (void*)LCD_FRAME_BUFFER);                             

#endif
  
  if(pSurf == NULL)
  {
    GUI_Printf("#Error: GUI_CreateSurface Failed.\r\n");
  }

  //LCDӲ����ʼ��
 	LCD_HardInit((u32)pSurf->Bits); 
  
#if  DMA2D_EN 
  DMA2D_DrvInit();
#endif
  
#if FRAME_BUFFER_EN
  {
    const SURFACE *pSurf_FB;
    pSurf_FB = GUI_CreateSurface((SURF_FORMAT)pSurf->Format,pSurf->Width,pSurf->Height,0,NULL);
    GUI_SetFrameBufferSurface(pSurf_FB);  
    //����
	  pSurf_FB->GL->FillArea(pSurf_FB,0,0,LCD_XSIZE,LCD_YSIZE,pSurf_FB->CC->MapRGB(0,0,0)); 

  }
#endif  

  //����
	pSurf->GL->FillArea(pSurf,0,0,LCD_XSIZE,LCD_YSIZE,pSurf->CC->MapRGB(0,0,0)); 
	//�򿪱���
  LCD_BkLight(TRUE);

	while(0)
	{ //����

		pSurf->GL->FillArea(pSurf,20,30,128,80,pSurf->CC->MapRGB(255,255,255));
		GUI_msleep(200);
		pSurf->GL->FillArea(pSurf,20,30,128,80,pSurf->CC->MapRGB(255,0,0));
		GUI_msleep(200);
		pSurf->GL->FillArea(pSurf,20,30,128,80,pSurf->CC->MapRGB(0,255,0));
		GUI_msleep(200);
		pSurf->GL->FillArea(pSurf,20,30,128,80,pSurf->CC->MapRGB(0,0,255));
		GUI_msleep(200);
		pSurf->GL->FillArea(pSurf,20,30,128,80,pSurf->CC->MapRGB(0,0,0));
		GUI_msleep(200);
		break;
	}

	return pSurf;
}


/********************************END OF FILE****************************/

