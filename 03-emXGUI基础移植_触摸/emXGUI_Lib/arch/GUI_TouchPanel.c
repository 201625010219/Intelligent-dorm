
#include <emXGUI.h>
#include    <string.h>

#include	"GUI_Drv.h"

#if 1/*(GUI_TOUCHSCREEN_EN > 0)*/

/*============================================================================*/

#define	TS_CAL_XOFFSET		(40)
#define	TS_CAL_YOFFSET		(40)

/*============================================================================*/

static	BOOL ts_xy_swap =FALSE;	     //�Ƿ�XY����
static	TS_CFG_DATA	ts_cal_data;	//У������
static	TS_CFG_DATA	ts_cfg_data;	//��������


/*============================================================================*/

static int	touch_screen_data_init(void);

void TouchPanel_TranslateInit(const TS_CFG_DATA *cfg)
{
	memcpy(&ts_cfg_data,cfg,sizeof(TS_CFG_DATA));
	touch_screen_data_init();
}

/*============================================================================*/
/**
  * @brief  ��Ҫ����ʱ���õĴ���������
  * @param  ��
  * @retval ��
  */
void	GUI_TouchHandler(void)
{
	int act;
	POINT pt;

  /* �жϴ���״̬ */
	act =TouchDev_GetAction();
	if(act==TS_ACT_DOWN)
	{
    /* ��ȡ�������� */
		if(TouchDev_GetPoint(&pt))
		{
			MouseInput(pt.x,pt.y,MK_LBUTTON);
		}
	}

	if(act==TS_ACT_UP)
	{
		GetCursorPos(&pt);
		MouseInput(pt.x,pt.y,0);
	}

}

/*============================================================================*/
/**
  * @brief  ����ת�������ڵ��败����
  * @param  width height ���������� �� ��
  * @param  pt[in out] ת�������������
  * @retval �Ƿ��������
  */
BOOL	TouchPanel_TranslatePoint(int width,int height,POINT *pt)
{
	s16 x,y,adx,ady;
	s16	adx_min,adx_max,ady_min,ady_max;

	if(pt != NULL)
	{
		adx	=pt->x;
		ady	=pt->y;

		adx_min	=ts_cfg_data.LUAdx;
		ady_min	=ts_cfg_data.LUAdy;

		adx_max	=ts_cfg_data.RDAdx;
		ady_max	=ts_cfg_data.RDAdy;

		if((adx_min==adx_max) || (ady_min==ady_max))
		{
			pt->x = -1;
			pt->y = -1;
			return	FALSE;
		}

		if(ts_xy_swap)
		{
			x = TS_CAL_XOFFSET+(width-(TS_CAL_XOFFSET*2))*(ady-ady_min) / (ady_max-ady_min);
			y = TS_CAL_YOFFSET+(height-(TS_CAL_YOFFSET*2))*(adx-adx_min) / (adx_max-adx_min);
		}
		else
		{
			x = TS_CAL_XOFFSET+(width-(TS_CAL_XOFFSET*2))*(adx-adx_min) / (adx_max-adx_min);
			y = TS_CAL_YOFFSET+(height-(TS_CAL_YOFFSET*2))*(ady-ady_min) / (ady_max-ady_min);
		}

		pt->x =x;
		pt->y =y;
		return	TRUE;
			
	}
	return	FALSE;
}

/*============================================================================*/
//����ÿ��У׼��Ĳ�������

int	TouchPanel_SetCalibrationSample(POINT *pt,int idx)
{
	switch(idx)
	{
		
		case	0:
				ts_cal_data.LUAdx	=pt->x;
				ts_cal_data.LUAdy	=pt->y;
				return	1;
				////
			
		case	1:
				ts_cal_data.RUAdx	=pt->x;
				ts_cal_data.RUAdy	=pt->y;
				return	1;
				////
				
		case	2:
				ts_cal_data.RDAdx	=pt->x;
				ts_cal_data.RDAdy	=pt->y;
				return	1;
				////
				
		case	3:
				ts_cal_data.LDAdx	=pt->x;
				ts_cal_data.LDAdy	=pt->y;
				return	1;
				////
		default:
				return	0;				
				
			
	}
}


/*============================================================================*/
//��ȡУ׼�����Ļ����

int	TouchPanel_GetCalibrationPoints(POINT *pt)
{
	int xsize,ysize;

	if(pt!=NULL)
	{
		xsize =GUI_XSIZE;
		ysize =GUI_YSIZE;

		pt[0].x	=TS_CAL_XOFFSET;
		pt[0].y	=TS_CAL_YOFFSET;
		
		pt[1].x	=xsize-TS_CAL_XOFFSET;
		pt[1].y	=TS_CAL_YOFFSET;

		pt[2].x	=xsize-TS_CAL_XOFFSET;
		pt[2].y	=ysize-TS_CAL_YOFFSET;
		
		pt[3].x	=TS_CAL_XOFFSET;
		pt[3].y	=ysize-TS_CAL_YOFFSET;
				
	}
	
	return	4;
}

/*============================================================================*/
/*============================================================================*/
//������У����ʼ

int	TouchPanel_CalibrationStart(void)
{

	GUI_Printf("TouchPanelCalibrationStart.\r\n");

	return TRUE;//TouchPanel_HardInit();
}

/*============================================================================*/
//������У������

int	TouchPanel_CalibrationFinish(void)
{

	ts_cal_data.ChkSum	= 0;
	ts_cal_data.ChkSum	+= ts_cal_data.LUAdx+ts_cal_data.LUAdy;
	ts_cal_data.ChkSum	+= ts_cal_data.RUAdx+ts_cal_data.RUAdy;
	ts_cal_data.ChkSum	+= ts_cal_data.RDAdx+ts_cal_data.RDAdy;
	ts_cal_data.ChkSum	+= ts_cal_data.LDAdx+ts_cal_data.LDAdy;

	memcpy(&ts_cfg_data,&ts_cal_data,sizeof(TS_CFG_DATA));
	
	touch_screen_data_init();
	GUI_Printf("TouchPanelCalibrationFinish.\r\n");
	return TRUE;
}

/*============================================================================*/
//������У���ɹ�

int	TouchPanel_CalibrationSucceed(void)
{

	TouchDev_SaveCfg(&ts_cal_data);
	GUI_Printf("TouchPanelCalibrationSucceed.\r\n");
	return TRUE;

}

/*============================================================================*/
//������У��ʧ��

int	TouchPanel_CalibrationFailed(void)
{
	if(TouchDev_LoadCfg(&ts_cfg_data))
	{
		TouchPanel_TranslateInit(&ts_cfg_data);
	}
	else
	{
	}
	
	GUI_Printf("TouchPanelCalibrationFailed.\r\n");
	return	TRUE;
}




/*============================================================================*/

static	U32	x_abs(int a)
{
	return	a>0 ? a:0-a;
}

//��鴥����XY�Ƿ񽻻�
static BOOL	check_touch_screen_swap(const TS_CFG_DATA *pDat)
{
	int lu_x,ru_x;
	int	lu_y,ru_y;
	int	dx,dy;
	////
	
	lu_x = pDat->LUAdx;
	ru_x = pDat->RUAdx;
	
	lu_y = pDat->LUAdy;
	ru_y = pDat->RUAdy;
	
	dx	=x_abs(lu_x-ru_x);
	dy	=x_abs(lu_y-ru_y);
	
	if(dx > dy)
	{
		ts_xy_swap	=FALSE;		//XY������.
	}
	else
	{
		ts_xy_swap	=TRUE;		//XY����.
	}
	
	return ts_xy_swap;

}

/*============================================================================*/
//��ʼ������������

static int	touch_screen_data_init(void)
{
	
	check_touch_screen_swap(&ts_cfg_data);


#if TS_DBG

	GUI_Printf("TS_LUAddata =%d, %d\n",ts_cfg_data.LUAdx,ts_cfg_data.LUAdy);
	GUI_Printf("TS_RUAddata =%d, %d\n",ts_cfg_data.RUAdx,ts_cfg_data.RUAdy);
	GUI_Printf("TS_RDAddata =%d, %d\n",ts_cfg_data.RDAdx,ts_cfg_data.RDAdy);
	GUI_Printf("TS_LDAddata =%d, %d\n",ts_cfg_data.LDAdx,ts_cfg_data.LDAdy);
	GUI_Printf("TS_XYSwap   =%d\n",ts_xy_swap);
	GUI_Printf("ts_org  =%d\n",ts_org);

#endif

	return 1;
	
}


/*============================================================================*/


/*============================================================================*/

#endif /*(GUI_TOUCHSCREEN_EN)*/

