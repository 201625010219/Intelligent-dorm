/**
  ******************************************************************************
  * @file    bsp_Gitwit_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ�ô���1���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 H750 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./git_usart/git_usart.h"

UART_HandleTypeDef Git_UartHandle;
extern uint8_t ucTemp;  
 /**
  * @brief  Gitwit_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */  
void Gitwit_USART_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
        
    Gitwit_USART_RX_GPIO_CLK_ENABLE();
    Gitwit_USART_TX_GPIO_CLK_ENABLE();
    
    /* ���ô���1ʱ��Դ*/
		RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
		RCC_PeriphClkInit.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
		HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
    /* ʹ�ܴ���1ʱ�� */
    Gitwit_USART_CLK_ENABLE();

    /* ����Tx����Ϊ���ù���  */
    GPIO_InitStruct.Pin = Gitwit_USART_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = Gitwit_USART_TX_AF;
    HAL_GPIO_Init(Gitwit_USART_TX_GPIO_PORT, &GPIO_InitStruct);
    
    /* ����Rx����Ϊ���ù��� */
    GPIO_InitStruct.Pin = Gitwit_USART_RX_PIN;
    GPIO_InitStruct.Alternate = Gitwit_USART_RX_AF;
    HAL_GPIO_Init(Gitwit_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
    
    /* ���ô�Gitwit_USART ģʽ */
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

    /*����1�жϳ�ʼ�� */
    HAL_NVIC_SetPriority(Gitwit_USART_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(Gitwit_USART_IRQ);
    /*���ô��ڽ����ж� */
    __HAL_UART_ENABLE_IT(&Git_UartHandle,UART_IT_RXNE);  
}


/*****************  �����ַ��� **********************/
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
