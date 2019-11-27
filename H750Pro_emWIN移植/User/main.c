/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ���ݴ�����-��������
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32H750������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */  
#include "stm32h7xx.h"
#include "main.h"
#include "./led/bsp_led.h" 
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h" 
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_i2c_touch.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./delay/core_delay.h" 
#include "./mpu/bsp_mpu.h" 
#include "./wm8978/bsp_wm8978.h"
#include "./mp3Player/mp3Player.h"
#include <string.h>
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* STemWINͷ�ļ� */
#include "GUI.h"
#include "DIALOG.h"

/*������ͷ�ļ�*/
#include "./dht11/bsp_dht11.h"
#include "./RTC/bsp_rtc.h"
#include "./ldr/bsp_ldr.h" 
#include "./MQ_CO/bsp_mq.h" 

#include "xbffontcreate.h"
#include "./RC522/rc522_config.h"
#include "./RC522/rc522_function.h"
#include "./gsm_gprs/bsp_gsm_gprs.h"
#include "./gsm_gprs/bsp_gsm_usart.h"
#include "./key/bsp_key.h"

/* FatFs includes component */
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/*������ͷ�ļ�*/
#include "./git_usart/git_usart.h"
#include "gizwits_product.h"
#include "./tim/bsp_basic_tim.h"
 

 

DHT11_Data_TypeDef DHT11_Data;
char SDPath[4]; /* SD�߼�������·�� */
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */

/*GSM*/
char *  rebuff;
 
/*�����ƺ���*/
//Э���ʼ�� 
void Gizwits_Init(void) 
{ 
          TIM_Basic_Init(); //1MS ϵͳ��ʱ   
          Gitwit_USART_Config();//WIFI ��ʼ��   
          memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t)); 
//�豸״̬�ṹ���ʼ�� 
          gizwitsInit();//��������ʼ�� 
} 
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
/* ���������� */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* LED������ */
static TaskHandle_t LED_Task_Handle = NULL;
/* Touch������ */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI������ */
static TaskHandle_t GUI_Task_Handle = NULL;
/*DHT11������ */
static TaskHandle_t DHT11_Task_Handle = NULL;
/*DHT11������ */
static TaskHandle_t Clock_Task_Handle = NULL;
/*��������LDR������ */
static TaskHandle_t LDR_Task_Handle = NULL;
/*��������MQ������ */
static TaskHandle_t MQ_Task_Handle = NULL;
/*MP3������ */
static TaskHandle_t MP3_Task_Handle = NULL;
/*RC522������ */
static TaskHandle_t RC522_Task_Handle = NULL;
/*GSM������ */
static TaskHandle_t GSM_Task_Handle = NULL;
/*GitWit������ */
static TaskHandle_t GitWit_Task_Handle = NULL;

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
SemaphoreHandle_t ScreenShotSem_Handle = NULL;
 
/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);
static void LED_Task(void* parameter);
static void Touch_Task(void* parameter);
static void GUI_Task(void* parameter);
static void DHT11_Task(void* parameter);
static void Clock_Task(void* parameter);
static void LDR_Task(void* parameter);
static void MQ_Task(void* parameter);
static void MP3_Task(void* parameter);
static void RC522_Task(void* parameter);
static void GSM_Task(void* parameter);
static void GitWit_Task(void* parameter);

static void BSP_Init(void);
extern void MainTask(void);
extern void SpeedTest(void);
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	/* ������Ӳ����ʼ�� */
	BSP_Init();
	/* ��ʼ��GUI */
	GUI_Init();
	/* ���������� */
	WM_MULTIBUF_Enable(1);
     
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* ������ں��� */
											 (const char*    )"AppTaskCreate",/* �������� */
											 (uint16_t       )512,					/* ����ջ��С */
											 (void*          )NULL,					/* ������ں������� */
											 (UBaseType_t    )1,						/* ��������ȼ� */
											 (TaskHandle_t   )&AppTaskCraete_Handle);/* ������ƿ�ָ�� */
	/* ����������� */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* �������񣬿������� */
	else
		return -1;
	
	while(1);/* ��������ִ�е����� */
}

/**
  * @brief ���񴴽�����
  * @note Ϊ�˷���������е����񴴽������������������
  * @param ��
  * @retval ��
  */
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	taskENTER_CRITICAL();//�����ٽ���

	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* ������ں��� */
											 (const char*    )"LED_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )5,         /* ��������ȼ� */
											 (TaskHandle_t   )&LED_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����LED1_Task����ɹ���\r\n");
    
    xReturn = xTaskCreate((TaskFunction_t)DHT11_Task,/* ������ں��� */
											 (const char*    )"DHT11_Task",/* �������� */
											 (uint16_t       )256,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )6,         /* ��������ȼ� */
											 (TaskHandle_t   )&DHT11_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����DHT11_Task����ɹ���\r\n");
    
      xReturn = xTaskCreate((TaskFunction_t)LDR_Task,/* ������ں��� */
											 (const char*    )"LDR_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )6,         /* ��������ȼ� */
											 (TaskHandle_t   )&LDR_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����LDR_Task����ɹ���\r\n");
    
     xReturn = xTaskCreate((TaskFunction_t)MQ_Task,/* ������ں��� */
											 (const char*    )"MQ_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )6,         /* ��������ȼ� */
											 (TaskHandle_t   )&MQ_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����MQ_Task����ɹ���\r\n");
        
//     xReturn = xTaskCreate((TaskFunction_t)GSM_Task,/* ������ں��� */
//											 (const char*    )"GSM_Task",/* �������� */
//											 (uint16_t       )128,       /* ����ջ��С */
//											 (void*          )NULL,      /* ������ں������� */
//											 (UBaseType_t    )6,         /* ��������ȼ� */
//											 (TaskHandle_t   )&GSM_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����GSM_Task����ɹ���\r\n");
    
//     xReturn = xTaskCreate((TaskFunction_t)MP3_Task,/* ������ں��� */
//											 (const char*    )"MP3_Task",/* �������� */
//											 (uint16_t       )1024,       /* ����ջ��С */
//											 (void*          )NULL,      /* ������ں������� */
//											 (UBaseType_t    )2,         /* ��������ȼ� */
//											 (TaskHandle_t   )&MP3_Task_Handle);/* ������ƿ�ָ�� */
//	if(pdPASS == xReturn)
//		printf("����MP3_Task����ɹ���\r\n");
    
     xReturn = xTaskCreate((TaskFunction_t)Clock_Task,/* ������ں��� */
											 (const char*    )"Clock_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )6,         /* ��������ȼ� */
											 (TaskHandle_t   )&Clock_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����Clock_Task����ɹ���\r\n");
    
       xReturn = xTaskCreate((TaskFunction_t)RC522_Task,/* ������ں��� */
											 (const char*    )"RC522_Task",/* �������� */
											 (uint16_t       )128,       /* ����ջ��С */
											 (void*          )NULL,      /* ������ں������� */
											 (UBaseType_t    )7,         /* ��������ȼ� */
											 (TaskHandle_t   )&RC522_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����RC522_Task����ɹ���\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* ������ں��� */
											 (const char*      )"Touch_Task",/* �������� */
											 (uint16_t         )512,     /* ����ջ��С */
											 (void*            )NULL,    /* ������ں������� */
											 (UBaseType_t      )5,       /* ��������ȼ� */
											 (TaskHandle_t     )&Touch_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����Touch_Task����ɹ���\r\n");
  
  xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* ������ں��� */
											 (const char*      )"GUI_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )4,         /* ��������ȼ� */
											 (TaskHandle_t     )&GUI_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����GUI_Task����ɹ���\r\n");
   xReturn = xTaskCreate((TaskFunction_t)GitWit_Task,/* ������ں��� */
											 (const char*      )"GitWit_Task",/* �������� */
											 (uint16_t         )1024,      /* ����ջ��С */
											 (void*            )NULL,      /* ������ں������� */
											 (UBaseType_t      )3,         /* ��������ȼ� */
											 (TaskHandle_t     )&GitWit_Task_Handle);/* ������ƿ�ָ�� */
	if(pdPASS == xReturn)
		printf("����GitWit_Task����ɹ���\r\n");
	
	vTaskDelete(AppTaskCraete_Handle);//ɾ��AppTaskCreate����
	
	taskEXIT_CRITICAL();//�˳��ٽ���
}

/**
  * @brief LED��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void LED_Task(void* parameter)
{
	while(1)
	{
//      printf("%d\r\n", (int)GUI_ALLOC_GetNumUsedBytes());
		//LED3_TOGGLE;
		vTaskDelay(1000);
	}
}
/**
  * @brief RC522��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void RC522_Task(void* parameter)
{
    char cStr [ 30 ];
    uint8_t ucArray_ID [ 4 ];    /*�Ⱥ���IC�������ͺ�UID(IC�����к�)*/                                                                                         
	uint8_t ucStatusReturn;      /*����״̬*/                                                                                          

  while ( 1 )
  { 
	taskENTER_CRITICAL();//�����ٽ���	
    /*Ѱ��*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )           
       /*��ʧ���ٴ�Ѱ��*/
	    ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                
		if ( ucStatusReturn == MI_OK  )
		{
      /*����ײ�����ж��ſ������д��������Χʱ������ͻ���ƻ������ѡ��һ�Ž��в�����*/
			if ( PcdAnticoll ( ucArray_ID ) == MI_OK )                                                                   
			{
				sprintf ( cStr, "The Card ID is: %02X%02X%02X%02X",
                  ucArray_ID [ 0 ],
                  ucArray_ID [ 1 ],
                  ucArray_ID [ 2 ],
                  ucArray_ID [ 3 ] );
        
        	     printf ( "%s\r\n",cStr ); 	
						
			}		
		}
        taskEXIT_CRITICAL();//�˳��ٽ���        
		vTaskDelay(2000);        
  }	
}
/**
  * @brief GSM��������
  * @note ��
  * @param ��
  * @retval ��
  */
//static void GSM_Task(void* paramete)
//{
//    char num[20]={0};
//    gsm_call("13926769552");                           //����112�绰����
//    rebuff = gsm_waitask(0);
//    if(strstr(rebuff,"ATD") != NULL)           //��ӦOK����ʾģ���������յ�����
//    {
//      printf("\r\n���ں���\r\n");
//      GSM_CLEAN_RX();                     //������ջ�����
//      rebuff = gsm_waitask(0);            //���µȴ���Ϣ
//     
//      if(strstr(rebuff,"NO CARRIER") != NULL) //��ӦNO CARRIER,ͨ������
//       {
//         printf("\r\nͨ������\r\n");
//       }
//      else if(strstr(rebuff,"NO ANSWER") != NULL)   //��ӦNO ANSWER�����˽���
//      {
//        printf("\r\n�㲦��ĵ绰��ʱ���˽��������Ժ��ٲ�\r\n");
//      }  
//    }
//	
//	GSM_CLEAN_RX();                     //������ջ�����
//	
//    while(1)
//	{
//		
//        if(IsRing(num)== GSM_TRUE)
//        {
//            printf("Ringing:\nFrom:%s\n�밴��KEY2�������߰���KEY1�Ҷ�\n\r",num);	
//        }
//			

//		if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  )
//		{
//			GSM_HANGON();
//		}
//		if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  )
//		{
//			GSM_HANGOFF();
//		}	
//	
//		GSM_DELAY(10);
//	}
	//GSM_DELAY(10);
//    	vTaskDelay(100);

//}
/**
  * @brief MQ��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void MQ_Task(void* parameter)
{
   uint8_t MQ_state; 
   while(1)
	{
       MQ_state =  MQ_Test(MQ_GPIO_PORT,MQ_PIN);
        if(MQ_state==0)
        {
          vTaskDelay(20);
          if(MQ_state==0)
          LED2_ON;
          ;    
        }            
        else
        {            
         LED2_OFF;
        }
		vTaskDelay(100);
	}   
}

/**
  * @brief Clock��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void Clock_Task(void* parameter)
{
    while(1)
	{
        RTC_TimeAndDate_Show();
		vTaskDelay(1000);
	}
}
/**
  * @brief ��������LDR��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void LDR_Task(void *parameter)
{
   uint8_t Ldr_state; 
   while(1)
	{
       Ldr_state =  LDR_Test(LDR_GPIO_PORT,LDR_PIN);
        if(Ldr_state)
        {
         LED1_OFF;
        }            
        else
        {            
         LED1_ON;
        }
		vTaskDelay(100);
	}   
}
/**
  * @brief GitWit��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void GitWit_Task(void* parameter)
{
	while(1)
	{
        userHandle();//�û��ɼ� 
        gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦�� 
		vTaskDelay(100);
	}
}
/**
  * @brief DHT11��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void DHT11_Task(void* parameter)
{
    while(1)
    {
        taskENTER_CRITICAL();//�����ٽ���
    /*����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
    if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
    {
        printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d ��RH ���¶�Ϊ %d.%d�� \r\n",\
        DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
    }		
    else
    {
      printf("Read DHT11 ERROR!\r\n");
    }
    	taskEXIT_CRITICAL();//�˳��ٽ���
		/* 2���һ������ */
        vTaskDelay(2000);
    }
}
/**
  * @brief ,P3��������
  * @note ��
  * @param ��
  * @retval ��
  */
//static void MP3_Task(void* parameter)
//{
//    while(1)
//  {
//        taskENTER_CRITICAL();//�����ٽ���
//        mp3PlayerDemo("0:/mp3/�Ź���-����֮��.mp3");
//		mp3PlayerDemo("0:/mp3/�Ź���-ȫ������.mp3");
//    	taskEXIT_CRITICAL();//�˳��ٽ���      
//      // vTaskDelay(2000);
//  }  
//}
/**
  * @brief ���������������
  * @note ��
  * @param ��
  * @retval ��
  */
static void Touch_Task(void* parameter)
{
	while(1)
	{
		GT9xx_GetOnePiont();
		vTaskDelay(5);
	}
}

/**
  * @brief GUI��������
  * @note ��
  * @param ��
  * @retval ��
  */
static void GUI_Task(void* parameter)
{
	
   Create_XBF12((uint8_t*)"0:/SYSTEM/EMWINFONT/XBF/XBF12.xbf");
   Create_XBF16((uint8_t*)"0:/SYSTEM/EMWINFONT/XBF/XBF16.xbf");
   Create_XBF24((uint8_t*)"0:/SYSTEM/EMWINFONT/XBF/XBF24.xbf");
      
	while(1)
	{
       
      MainTask();
//        while(1)
//        {
         vTaskDelay(100);
//        }
	}
}

/**
  * @brief �弶�����ʼ��
  * @note ���а����ϵĳ�ʼ�����ɷ��������������
  * @param ��
  * @retval ��
  */
static void BSP_Init(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();
  Board_MPU_Config(0, MPU_Normal_WT, 0x20000000, MPU_REGION_SIZE_128KB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_REGION_SIZE_512KB);
  Board_MPU_Config(2, MPU_Normal_WT, 0xD0000000, MPU_REGION_SIZE_32MB);
  
	/* CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı���������
   * ����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ�
   */
  __HAL_RCC_CRC_CLK_ENABLE();
	
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��ͬһ�����ȼ����飬ǧ��Ҫ�ٷ��飬�мǡ�
	 */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	/* ϵͳʱ�ӳ�ʼ����400MHz */
	SystemClock_Config();
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	/* ���ô���1Ϊ��115200 8-N-1 */
	DEBUG_USART_Config();		
  /* ��ʼ�������� */
   GTP_Init_Panel(); 
  /* ��ʼ��SDRAM ����LCD �Դ�*/
	//SDRAM_Init();
	/* LCD �˿ڳ�ʼ�� */ 
	LCD_Init();
    /* DHT11��ʼ�� */
	DHT11_GPIO_Config();
    /*RTC����*/
    RTC_CLK_Config();
    /*������������*/
    LDR_GPIO_Config();
    /*MQ��ʼ��*/
    MQ_GPIO_Config();
    /*������ʼ��*/
    Key_GPIO_Config();
    /*RC522��ʼ��*/
    RC522_Init();
    PcdReset ();
  /*���ù�����ʽ*/   
	M500PcdConfigISOType ( 'A' );
	/*�����Ƴ�ʼ��*/
    userInit();
    Gizwits_Init(); 
    gizwitsSetMode(WIFI_AIRLINK_MODE);
//	GIZWITS_LOG("MCU Init Success \n");     
//     //�����������������̷�
    FATFS_LinkDriver(&SD_Driver, SDPath);
    //���ⲿSD�������ļ�ϵͳ���ļ�ϵͳ����ʱ���SD����ʼ��
    res_sd = f_mount(&fs,"0:",1);  
  
    /*----------------------- ��ʽ������ ---------------------------*/  
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if(res_sd == FR_NO_FILESYSTEM)
    {
      printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
      /* ��ʽ�� */
      res_sd=f_mkfs("0:",0,0);							
      
      if(res_sd == FR_OK)
      {
        printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
        /* ��ʽ������ȡ������ */
        res_sd = f_mount(NULL,"0:",1);			
        /* ���¹���	*/			
        res_sd = f_mount(&fs,"0:",1);
      }
      else
      {
        LED_RED;
        printf("������ʽ��ʧ�ܡ�����\r\n");
        while(1);
      }
    }
    else if(res_sd!=FR_OK)
    {
      printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
      printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
      while(1);
    }
    else
    {
      printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
    }
    printf("WM8978¼���ͻطŹ���\n");
  /* ���WM8978оƬ���˺������Զ�����CPU��GPIO */
    //Delay_ms(10);
  if (wm8978_Init()==0)
  {
    printf("��ⲻ��WM8978оƬ!!!\n");
    while (1);	/* ͣ�� */
  }
  printf("��ʼ��WM8978�ɹ�\n");	
 //���ģ����Ӧ�Ƿ�����
//    while(gsm_init()!= GSM_TRUE)
//    {
//      printf("\r\nģ����Ӧ���Բ���������\r\n");
//      printf("\r\n��ģ����Ӧ����һֱ������������ģ������ӻ��Ƿ��ѿ�����Դ����\r\n");

//    }
//      

 
}

/**
  * @brief  System Clock ����
  *         system Clock ��������: 
	*            System Clock source  = PLL (HSE)
	*            SYSCLK(Hz)           = 400000000 (CPU Clock)
	*            HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
	*            AHB Prescaler        = 2
	*            D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
	*            D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
	*            D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
	*            D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
	*            HSE Frequency(Hz)    = 25000000
	*            PLL_M                = 5
	*            PLL_N                = 160
	*            PLL_P                = 2
	*            PLL_Q                = 4
	*            PLL_R                = 2
	*            VDD(V)               = 3.3
	*            Flash Latency(WS)    = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;
  
  /*ʹ�ܹ������ø��� */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* ��������ʱ��Ƶ�ʵ������ϵͳƵ��ʱ����ѹ���ڿ����Ż����ģ�
		 ����ϵͳƵ�ʵĵ�ѹ����ֵ�ĸ��¿��Բο���Ʒ�����ֲᡣ  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
 
  /* ����HSE������ʹ��HSE��ΪԴ����PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
 
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
	/* ѡ��PLL��Ϊϵͳʱ��Դ����������ʱ�ӷ�Ƶ�� */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK  | \
																 RCC_CLOCKTYPE_HCLK    | \
																 RCC_CLOCKTYPE_D1PCLK1 | \
																 RCC_CLOCKTYPE_PCLK1   | \
                                 RCC_CLOCKTYPE_PCLK2   | \
																 RCC_CLOCKTYPE_D3PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;  
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2; 
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2; 
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2; 
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/****************************END OF FILE***************************/
