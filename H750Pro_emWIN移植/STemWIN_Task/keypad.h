#ifndef _SOFTWAREKEY_H
#define _SOFTWAREKEY_H
#include "DIALOG.h"
#include "stm32h7xx.h"
//�������ͣ�����Ӣ�ļ��̣����ּ��̣����ż��̣��Ź����������뷨����
typedef enum
{
	ENGLISH_KEYPAD,		//Ӣ�ļ���
	NUMBER_KEYPAD,		//���ּ���
	SIGN_KEYPAD,		//���ż���
	CHINESE_KEYPAD,	    //�Ź����������뷨����
	KEYPAD_NUM			//��������
}keypad_type;

//keypad���ýṹ��
typedef struct 
{
	keypad_type padtype;				//��������
	uint16_t xpos;							//�����X����		
	uint16_t ypos;							//�����Y����
	uint16_t width;							//����̿��
	uint16_t height;							//����̸߶�
	WM_HWIN hKeypad;					//����̴��ھ��
	WM_HWIN EngButton_Handle[35];		//Ӣ�ļ����ϰ�ť���
	WM_HWIN NumButton_Handle[20];		//���ּ����ϰ�ť���
	WM_HWIN SignButton_Handle[20];		//���ż����ϰ�ť���
    WM_HWIN ChineseButton_Handle[25];   //���ļ����ϰ�ť���
    uint8_t      inputstr[7];                //�����ַ��������6���ַ�+������
    uint8_t      inputlen;                   //���볤��
    uint8_t      cur_index;                  //��ǰƴ������
    uint8_t      result_num;                 //ƴ��ƥ����
    uint8_t      pypagenum;                  //ƴ��ҳ��
    uint8_t      pynowpage;                  //��ǰѡ�е�ƴ��ҳ
    uint8_t      dispagenum;                 //������ʾ��ҳ��
    uint8_t      dispagecur;                 //��ǰ������ʾҳ
    uint8_t      *disstr;                    //������ʾ���ַ���ָ��
    uint8_t      *cur_hz;                    //��ǰѡ�еĺ���
    uint8_t      hznum;                      //ѡ�е�ƴ����Ӧ�ĺ�������
    uint8_t      disbuf[6][3];               //����ť����
    uint8_t      *butname[6];                //��ť����
    uint8_t      sta;                        //��¼���뷨״̬
                                        //bit7: 0,��һ�ν����������뷨;1,ǰ���Ѿ��������������뷨
	uint8_t LowerCase;						//0 ��д;1 Сд
	uint8_t signpad_flag;					//���ż�������
	uint8_t signpad_num;						//���ż��̵�����
	
} KEYPADStructTypeDef;


#define COLOR_BACK0            0xFF3333
#define COLOR_BACK1            0x550000
#define COLOR_BORDER           0x444444
#define COLOR_KEYPAD0          0xAAAAAA
#define COLOR_KEYPAD1          0x555555
#define BUTTON_COLOR0          0xEEEEEE		
#define BUTTON_COLOR1          0xCCCCCC  	
#define BUTTON_COLOR2          0xCCCCCC
#define BUTTON_COLOR3          0xAAAAAA
#define BUTTON_SKINFLEX_RADIUS 4

#define ID_BUTTON              (GUI_ID_USER + 0)
#define ID_BUTTON_0            (GUI_ID_USER + 0x30)
//�Զ�����Ϣ	
#define CHANGE_ENGLISHPADTO_NUMPAD	   	(WM_USER+0)			




//���ּ��̰�������
#define NUM_BUTTON_WIDTH			90	//���ּ��̰�ť���
#define NUM_BUTTON_HEIGHT			45	//���ּ��̰�ť�߶�
#define NUM_BUTTON_XPAC				0	//���ּ��̰�ťX����
#define NUM_BUTTON_XOFFSET			130	//���ּ���ÿ�а�ťX��ƫ��

#define NUM_BUTTON_LINE1YPOS		7	//���ּ��̵�һ��Y����
#define NUM_BUTTON_LINE2YPOS		47	//���ּ��̵ڶ���Y����
#define NUM_BUTTON_LINE3YPOS		87	//���ּ��̵�����Y����
#define NUM_BUTTON_LINE4YPOS		127	//���ּ��̵�����Y����

#define NUM_BUTTONSING_XINC			15	//���ּ����з��ŵ�X�᳤������
#define NUM_BUTTONBACKSPACE_XINC	15	//���ּ�����ɾ������X�᳤������
#define NUM_BUTTONSPACE_XINC		15	//���ּ����пո����X�᳤������
#define NUM_BUTTONENTER_XINC		15  //���ּ����лس�����X�᳤������
#define NUM_BUTTONRETURN_XINC		15	//���ּ����з��ؼ���X�᳤������







void keypad_demo(void);
#endif
