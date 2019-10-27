#include "xbffontcreate.h"
#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"
							  

#define SYSTEM_SUPPORT_OS 1
#if SYSTEM_SUPPORT_OS
//#include "includes.h"					//freertos ʹ��	  
#endif
//extern GUI_CONST_STORAGE GUI_FONT GUI_FontXBF12;
//��������
GUI_FONT XBF12_Font;
GUI_FONT XBF16_Font;
GUI_FONT XBF24_Font;


GUI_XBF_DATA	XBF12_Data;
GUI_XBF_DATA	XBF16_Data;
GUI_XBF_DATA	XBF24_Data;

FIL XBF16FontFile;
FIL XBF12FontFile;
FIL XBF24FontFile;


//�ص�������������ȡ��������
//������Off:		��XBF��ƫ��(λ��)
//		NumBytes:	Ҫ��ȡ���ֽ���
//		pVoid:	Ҫ��ȡ���ļ�
//		pBuff:	��ȡ�������ݵĻ�����
//����ֵ:0 �ɹ���1 ʧ��
static int _cbGetData(U32 Off, U16 NumBytes, void * pVoid, void * pBuffer) 
{
	int result;
	uint16_t bread; 
	FIL *hFile;

	#if SYSTEM_SUPPORT_OS
		//CPU_SR_ALLOC();
	#endif

	hFile = (FIL*)pVoid; 
	
	//�������ļ��е�ƫ��(λ��)
	result = f_lseek(hFile,Off);
	if(result != FR_OK)	return 1; //���ش���

	//��ȡ��������
	#if SYSTEM_SUPPORT_OS
	taskENTER_CRITICAL();	//�ٽ���
	#endif
		
	result = f_read(hFile,pBuffer,NumBytes,(UINT *)&bread); //��ȡ����
	
	#if SYSTEM_SUPPORT_OS
    taskEXIT_CRITICAL();//�˳��ٽ���	
	#endif
	
	if(result != FR_OK) return 1; //���ش���
	return 0; 
}

//����XBF12���壬��EMWINʹ��
//fxpath:XBF�����ļ�·��
//����ֵ:0���ɹ���1��ʧ��
uint8_t Create_XBF12(uint8_t *fxpath) 
{
	int result;
	result = f_open(&XBF12FontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
	
	if(result != FR_OK) return 1;
	//����XBF16����
	GUI_XBF_CreateFont(	&XBF12_Font,    //ָ��GUI_FONT�ṹ
						&XBF12_Data, 	//ָ��GUI_XBF_DATA�ṹ
						GUI_XBF_TYPE_PROP_AA2_EXT,//Ҫ��������������
						_cbGetData,   	//�ص�����
						&XBF12FontFile);  //������ص�����_cbGetData�Ĳ���
	return 0;
}

//����XBF16���壬��EMWINʹ��
//fxpath:XBF�����ļ�·��
//����ֵ:0���ɹ���1��ʧ��
uint8_t Create_XBF16(uint8_t *fxpath) 
{
	int result;
	result = f_open(&XBF16FontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
	
	if(result != FR_OK) return 1;
	//����XBF16����
	GUI_XBF_CreateFont(	&XBF16_Font,    //ָ��GUI_FONT�ṹ
						&XBF16_Data, 	//ָ��GUI_XBF_DATA�ṹ
						GUI_XBF_TYPE_PROP_AA2_EXT,//Ҫ��������������
						_cbGetData,   	//�ص�����
						&XBF16FontFile);  //������ص�����_cbGetData�Ĳ���
	return 0;
}

//����XBF24���壬��EMWINʹ��
//fxpath:XBF�����ļ�·��
//����ֵ:0���ɹ���1��ʧ��
uint8_t Create_XBF24(uint8_t *fxpath) 
{
	int result;
	result = f_open(&XBF24FontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
	if(result != FR_OK) return 1;
	//����XBF16����
	GUI_XBF_CreateFont(	&XBF24_Font,    //ָ��GUI_FONT�ṹ
						&XBF24_Data, 	//ָ��GUI_XBF_DATA�ṹ
						GUI_XBF_TYPE_PROP_AA2_EXT,//Ҫ��������������
						_cbGetData,   	//�ص�����
						&XBF24FontFile);  //������ص�����_cbGetData�Ĳ���
	return 0;
}

////����XBF36���壬��EMWINʹ��
////fxpath:XBF�����ļ�·��
////����ֵ:0���ɹ���1��ʧ��
//uint8_t Create_XBF36(uint8_t *fxpath) 
//{
//	int result;
//	result = f_open(&XBF36FontFile,(const TCHAR*)fxpath,FA_READ);	//���ֿ��ļ�
//	if(result != FR_OK) return 1;	
//	//����XBF16����
//	GUI_XBF_CreateFont(	&XBF36_Font,    //ָ��GUI_FONT�ṹ
//						&XBF36_Data, 	//ָ��GUI_XBF_DATA�ṹ
//						GUI_XBF_TYPE_PROP_AA2_EXT,//Ҫ��������������
//						_cbGetData,   	//�ص�����
//						&XBF36FontFile);  //������ص�����_cbGetData�Ĳ���
//	return 0;
//}
