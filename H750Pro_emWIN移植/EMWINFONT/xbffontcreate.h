#ifndef _XBFFONTCREATE_H
#define _XBFFONTCREATE_H
//#include "sys.h"
#include "GUI.h"
#include "stm32h7xx.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//STemwin XBF��ʽ������ʾ   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/3/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern GUI_FONT XBF12_Font;
extern GUI_FONT XBF16_Font;
extern GUI_FONT XBF24_Font;


uint8_t Create_XBF12(uint8_t *fxpath); 
uint8_t Create_XBF16(uint8_t *fxpath); 
uint8_t Create_XBF24(uint8_t *fxpath); 

#endif
