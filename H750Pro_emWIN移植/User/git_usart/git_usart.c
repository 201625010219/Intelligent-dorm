/**
  ******************************************************************************
  * @file    bsp_Gitwit_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 H750 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./git_usart/git_usart.h"

UART_HandleTypeDef Git_UartHandle;
extern uint8_t ucTemp;  
 /**
  * @brief  Gitwit_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void Gitwit_USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
        
    Gitwit_USART_RX_GPIO_CLK_ENABLE();
    Gitwit_USART_TX_GPIO_CLK_ENABLE();
    
    /* 配置串口1时钟源*/
		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* 使能串口1时钟 */
    Gitwit_USART_CLK_ENABLE();

    /* 配置Tx引脚为复用功能  */
    GPIO_InitStruct.Pin = Gitwit_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = Gitwit_USART_TX_AF;
    HAL_GPIO_Init(Gitwit_USART_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* 配置Rx引脚为复用功能 */
    GPIO_InitStruct.Pin = Gitwit_USART_RX_PIN;
    GPIO_InitStruct.Alternate = Gitwit_USART_RX_AF;
    HAL_GPIO_Init(Gitwit_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
    
    /* 配置串Gitwit_USART 模式 */
    Git_UartHandle.Instance = Gitwit_USART;
    Git_UartHandle.Init.BaudRate = 9600;
    Git_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    Git_UartHandle.Init.StopBits = UART_STOPBITS_1;
    Git_UartHandle.Init.Parity = UART_PARITY_NONE;
    Git_UartHandle.Init.Mode = UART_MODE_TX_RX;
    Git_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    Git_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    Git_UartHandle.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
    Git_UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    HAL_UART_Init(&Git_UartHandle);

    /*串口1中断初始化 */
    HAL_NVIC_SetPriority(Gitwit_USART_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(Gitwit_USART_IRQ);
    /*配置串口接收中断 */
    __HAL_UART_ENABLE_IT(&Git_UartHandle,UART_IT_RXNE);  
}


/*****************  发送字符串 **********************/
void Git_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit( &Git_UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

/*********************************************END OF FILE**********************/
