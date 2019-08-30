#ifndef __GUI_LCD_PORT_H
#define	__GUI_LCD_PORT_H

#include "stm32h7xx.h"
#include	"emXGUI.h"
#include	"gui_drv_cfg.h"


/* ��Ļ��ת���Ͷ��� */
#define	ROTATE_0	  0
#define	ROTATE_90	1
#define	ROTATE_180	2
#define	ROTATE_270	3

/*============================================================================*/


SURFACE* GUI_DisplayInit(void);
SURFACE*	GUI_CreateSurface(SURF_FORMAT Format,int Width,int Height,int LineBytes,void *bits);
void	GUI_DeleteSurface(const SURFACE *pSurf);



#endif /* __GUI_LCD_PORT_H */
