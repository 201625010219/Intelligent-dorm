/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   电容触摸屏-触摸画板
  ******************************************************************
  * @attention
  *
  * 实验平台:野火 STM32H750开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
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
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* STemWIN头文件 */
#include "GUI.h"
#include "DIALOG.h"

/*传感器头文件*/
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


DHT11_Data_TypeDef DHT11_Data;
char SDPath[4]; /* SD逻辑驱动器路径 */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */

/*GSM*/
char *  rebuff;
 
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
/* 创建任务句柄 */
static TaskHandle_t AppTaskCraete_Handle = NULL;
/* LED任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;
/* Touch任务句柄 */
static TaskHandle_t Touch_Task_Handle = NULL;
/* GUI任务句柄 */
static TaskHandle_t GUI_Task_Handle = NULL;
/*DHT11任务句柄 */
static TaskHandle_t DHT11_Task_Handle = NULL;
/*DHT11任务句柄 */
static TaskHandle_t Clock_Task_Handle = NULL;
/*光敏电阻LDR任务句柄 */
static TaskHandle_t LDR_Task_Handle = NULL;
/*烟雾传感器MQ任务句柄 */
static TaskHandle_t MQ_Task_Handle = NULL;
/*MP3任务句柄 */
static TaskHandle_t MP3_Task_Handle = NULL;
/*RC522任务句柄 */
static TaskHandle_t RC522_Task_Handle = NULL;
/*GSM任务句柄 */
static TaskHandle_t GSM_Task_Handle = NULL;

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
SemaphoreHandle_t ScreenShotSem_Handle = NULL;
 
/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */


/*
*************************************************************************
*                             函数声明
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

static void BSP_Init(void);
extern void MainTask(void);
extern void SpeedTest(void);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	/* 开发板硬件初始化 */
	BSP_Init();
	/* 初始化GUI */
	GUI_Init();
	/* 开启三缓冲 */
	WM_MULTIBUF_Enable(1);
     
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,/* 任务入口函数 */
											 (const char*    )"AppTaskCreate",/* 任务名称 */
											 (uint16_t       )512,					/* 任务栈大小 */
											 (void*          )NULL,					/* 任务入口函数参数 */
											 (UBaseType_t    )1,						/* 任务的优先级 */
											 (TaskHandle_t   )&AppTaskCraete_Handle);/* 任务控制块指针 */
	/* 启动任务调度 */
	if(pdPASS == xReturn)
		vTaskStartScheduler();/* 启动任务，开启调度 */
	else
		return -1;
	
	while(1);/* 正常不会执行到这里 */
}

/**
  * @brief 任务创建函数
  * @note 为了方便管理，所有的任务创建都放在这个函数里面
  * @param 无
  * @retval 无
  */
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	taskENTER_CRITICAL();//进入临界区

	xReturn = xTaskCreate((TaskFunction_t)LED_Task,/* 任务入口函数 */
											 (const char*    )"LED_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )5,         /* 任务的优先级 */
											 (TaskHandle_t   )&LED_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LED1_Task任务成功！\r\n");
    
    xReturn = xTaskCreate((TaskFunction_t)DHT11_Task,/* 任务入口函数 */
											 (const char*    )"DHT11_Task",/* 任务名称 */
											 (uint16_t       )256,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )6,         /* 任务的优先级 */
											 (TaskHandle_t   )&DHT11_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建DHT11_Task任务成功！\r\n");
    
      xReturn = xTaskCreate((TaskFunction_t)LDR_Task,/* 任务入口函数 */
											 (const char*    )"LDR_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )6,         /* 任务的优先级 */
											 (TaskHandle_t   )&LDR_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建LDR_Task任务成功！\r\n");
    
     xReturn = xTaskCreate((TaskFunction_t)MQ_Task,/* 任务入口函数 */
											 (const char*    )"MQ_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )6,         /* 任务的优先级 */
											 (TaskHandle_t   )&MQ_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建MQ_Task任务成功！\r\n");
        
//     xReturn = xTaskCreate((TaskFunction_t)GSM_Task,/* 任务入口函数 */
//											 (const char*    )"GSM_Task",/* 任务名称 */
//											 (uint16_t       )128,       /* 任务栈大小 */
//											 (void*          )NULL,      /* 任务入口函数参数 */
//											 (UBaseType_t    )6,         /* 任务的优先级 */
//											 (TaskHandle_t   )&GSM_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GSM_Task任务成功！\r\n");
    
//     xReturn = xTaskCreate((TaskFunction_t)MP3_Task,/* 任务入口函数 */
//											 (const char*    )"MP3_Task",/* 任务名称 */
//											 (uint16_t       )1024,       /* 任务栈大小 */
//											 (void*          )NULL,      /* 任务入口函数参数 */
//											 (UBaseType_t    )2,         /* 任务的优先级 */
//											 (TaskHandle_t   )&MP3_Task_Handle);/* 任务控制块指针 */
//	if(pdPASS == xReturn)
//		printf("创建MP3_Task任务成功！\r\n");
    
     xReturn = xTaskCreate((TaskFunction_t)Clock_Task,/* 任务入口函数 */
											 (const char*    )"Clock_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )6,         /* 任务的优先级 */
											 (TaskHandle_t   )&Clock_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Clock_Task任务成功！\r\n");
    
       xReturn = xTaskCreate((TaskFunction_t)RC522_Task,/* 任务入口函数 */
											 (const char*    )"RC522_Task",/* 任务名称 */
											 (uint16_t       )128,       /* 任务栈大小 */
											 (void*          )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t    )7,         /* 任务的优先级 */
											 (TaskHandle_t   )&RC522_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建RC522_Task任务成功！\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t)Touch_Task,/* 任务入口函数 */
											 (const char*      )"Touch_Task",/* 任务名称 */
											 (uint16_t         )512,     /* 任务栈大小 */
											 (void*            )NULL,    /* 任务入口函数参数 */
											 (UBaseType_t      )4,       /* 任务的优先级 */
											 (TaskHandle_t     )&Touch_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建Touch_Task任务成功！\r\n");
  
  xReturn = xTaskCreate((TaskFunction_t)GUI_Task,/* 任务入口函数 */
											 (const char*      )"GUI_Task",/* 任务名称 */
											 (uint16_t         )1024,      /* 任务栈大小 */
											 (void*            )NULL,      /* 任务入口函数参数 */
											 (UBaseType_t      )3,         /* 任务的优先级 */
											 (TaskHandle_t     )&GUI_Task_Handle);/* 任务控制块指针 */
	if(pdPASS == xReturn)
		printf("创建GUI_Task任务成功！\r\n");
	
	vTaskDelete(AppTaskCraete_Handle);//删除AppTaskCreate任务
	
	taskEXIT_CRITICAL();//退出临界区
}

/**
  * @brief LED任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief RC522任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void RC522_Task(void* parameter)
{
    char cStr [ 30 ];
    uint8_t ucArray_ID [ 4 ];    /*先后存放IC卡的类型和UID(IC卡序列号)*/                                                                                         
	uint8_t ucStatusReturn;      /*返回状态*/                                                                                          

  while ( 1 )
  { 
	taskENTER_CRITICAL();//进入临界区	
    /*寻卡*/
		if ( ( ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID ) ) != MI_OK )           
       /*若失败再次寻卡*/
	    ucStatusReturn = PcdRequest ( PICC_REQALL, ucArray_ID );		                                                
		if ( ucStatusReturn == MI_OK  )
		{
      /*防冲撞（当有多张卡进入读写器操作范围时，防冲突机制会从其中选择一张进行操作）*/
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
        taskEXIT_CRITICAL();//退出临界区        
		vTaskDelay(2000);        
  }	
}
/**
  * @brief GSM任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
//static void GSM_Task(void* paramete)
//{
//    char num[20]={0};
//    gsm_call("13926769552");                           //拨打112电话测试
//    rebuff = gsm_waitask(0);
//    if(strstr(rebuff,"ATD") != NULL)           //响应OK，表示模块正常接收到命令
//    {
//      printf("\r\n正在呼叫\r\n");
//      GSM_CLEAN_RX();                     //清除接收缓冲区
//      rebuff = gsm_waitask(0);            //重新等待消息
//     
//      if(strstr(rebuff,"NO CARRIER") != NULL) //响应NO CARRIER,通话结束
//       {
//         printf("\r\n通话结束\r\n");
//       }
//      else if(strstr(rebuff,"NO ANSWER") != NULL)   //响应NO ANSWER，无人接听
//      {
//        printf("\r\n你拨打的电话暂时无人接听，请稍后再拨\r\n");
//      }  
//    }
//	
//	GSM_CLEAN_RX();                     //清除接收缓冲区
//	
//    while(1)
//	{
//		
//        if(IsRing(num)== GSM_TRUE)
//        {
//            printf("Ringing:\nFrom:%s\n请按下KEY2接听或者按下KEY1挂断\n\r",num);	
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
  * @brief MQ任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief Clock任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief 光敏电阻LDR任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief DHT11任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
static void DHT11_Task(void* parameter)
{
    while(1)
    {
        taskENTER_CRITICAL();//进入临界区
    /*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
    if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
    {
        printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d ％RH ，温度为 %d.%d℃ \r\n",\
        DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
    }		
    else
    {
      printf("Read DHT11 ERROR!\r\n");
    }
    	taskEXIT_CRITICAL();//退出临界区
		/* 2秒读一次数据 */
        vTaskDelay(2000);
    }
}
/**
  * @brief ,P3任务主体
  * @note 无
  * @param 无
  * @retval 无
  */
//static void MP3_Task(void* parameter)
//{
//    while(1)
//  {
//        taskENTER_CRITICAL();//进入临界区
//        mp3PlayerDemo("0:/mp3/张国荣-玻璃之情.mp3");
//		mp3PlayerDemo("0:/mp3/张国荣-全赖有你.mp3");
//    	taskEXIT_CRITICAL();//退出临界区      
//      // vTaskDelay(2000);
//  }  
//}
/**
  * @brief 触摸检测任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief GUI任务主体
  * @note 无
  * @param 无
  * @retval 无
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
  * @brief 板级外设初始化
  * @note 所有板子上的初始化均可放在这个函数里面
  * @param 无
  * @retval 无
  */
static void BSP_Init(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();
  Board_MPU_Config(0, MPU_Normal_WT, 0x20000000, MPU_REGION_SIZE_128KB);
  Board_MPU_Config(1, MPU_Normal_WT, 0x24000000, MPU_REGION_SIZE_512KB);
  Board_MPU_Config(2, MPU_Normal_WT, 0xD0000000, MPU_REGION_SIZE_32MB);
  
	/* CRC和emWin没有关系，只是他们为了库的保护而做的
   * 这样STemWin的库只能用在ST的芯片上面，别的芯片是无法使用的。
   */
  __HAL_RCC_CRC_CLK_ENABLE();
	
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用同一个优先级分组，千万不要再分组，切记。
	 */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	/* 系统时钟初始化成400MHz */
	SystemClock_Config();
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 配置串口1为：115200 8-N-1 */
	DEBUG_USART_Config();		
  /* 初始化触摸屏 */
   GTP_Init_Panel(); 
  /* 初始化SDRAM 用作LCD 显存*/
	//SDRAM_Init();
	/* LCD 端口初始化 */ 
	LCD_Init();
    /* DHT11初始化 */
	DHT11_GPIO_Config();
    /*RTC配置*/
    RTC_CLK_Config();
    /*光敏电阻配置*/
    LDR_GPIO_Config();
    /*MQ初始化*/
    MQ_GPIO_Config();
    /*按键初始化*/
    Key_GPIO_Config();
    /*RC522初始化*/
    RC522_Init();
    PcdReset ();
  /*设置工作方式*/   
	M500PcdConfigISOType ( 'A' );
	
//     //链接驱动器，创建盘符
    FATFS_LinkDriver(&SD_Driver, SDPath);
    //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
    res_sd = f_mount(&fs,"0:",1);  
  
    /*----------------------- 格式化测试 ---------------------------*/  
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if(res_sd == FR_NO_FILESYSTEM)
    {
      printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
      /* 格式化 */
      res_sd=f_mkfs("0:",0,0);							
      
      if(res_sd == FR_OK)
      {
        printf("》SD卡已成功格式化文件系统。\r\n");
        /* 格式化后，先取消挂载 */
        res_sd = f_mount(NULL,"0:",1);			
        /* 重新挂载	*/			
        res_sd = f_mount(&fs,"0:",1);
      }
      else
      {
        LED_RED;
        printf("《《格式化失败。》》\r\n");
        while(1);
      }
    }
    else if(res_sd!=FR_OK)
    {
      printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
      printf("！！可能原因：SD卡初始化不成功。\r\n");
      while(1);
    }
    else
    {
      printf("》文件系统挂载成功，可以进行读写测试\r\n");
    }
    printf("WM8978录音和回放功能\n");
  /* 检测WM8978芯片，此函数会自动配置CPU的GPIO */
    //Delay_ms(10);
  if (wm8978_Init()==0)
  {
    printf("检测不到WM8978芯片!!!\n");
    while (1);	/* 停机 */
  }
  printf("初始化WM8978成功\n");	
 //检测模块响应是否正常
//    while(gsm_init()!= GSM_TRUE)
//    {
//      printf("\r\n模块响应测试不正常！！\r\n");
//      printf("\r\n若模块响应测试一直不正常，请检查模块的连接或是否已开启电源开关\r\n");

//    }
//      

 
}

/**
  * @brief  System Clock 配置
  *         system Clock 配置如下: 
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
  
  /*使能供电配置更新 */
  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

  /* 当器件的时钟频率低于最大系统频率时，电压调节可以优化功耗，
		 关于系统频率的电压调节值的更新可以参考产品数据手册。  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
 
  /* 启用HSE振荡器并使用HSE作为源激活PLL */
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
  
	/* 选择PLL作为系统时钟源并配置总线时钟分频器 */
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
