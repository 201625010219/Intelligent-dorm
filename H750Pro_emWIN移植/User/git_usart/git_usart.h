#ifndef __Gitwit_USART_H
#define	__Gitwit_USART_H

#include "stm32h7xx.h"
#include <stdio.h>



//引脚定义
/*******************************************************/
#define Gitwit_USART                             USART3
#define Gitwit_USART_CLK_ENABLE()                __USART3_CLK_ENABLE();

#define Gitwit_USART_RX_GPIO_PORT                GPIOB
#define Gitwit_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define Gitwit_USART_RX_PIN                      GPIO_PIN_11
#define Gitwit_USART_RX_AF                       GPIO_AF7_USART3


#define Gitwit_USART_TX_GPIO_PORT                GPIOB
#define Gitwit_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define Gitwit_USART_TX_PIN                      GPIO_PIN_10
#define Gitwit_USART_TX_AF                       GPIO_AF7_USART3

#define Gitwit_USART_IRQHandler                  USART3_IRQHandler
#define Gitwit_USART_IRQ                 		USART3_IRQn
/************************************************************/


//串口波特率
#define Gitwit_USART_BAUDRATE                    9600

void Git_SendString(uint8_t *str);
void Gitwit_USART_Config(void);
//int fputc(int ch, FILE *f);
extern UART_HandleTypeDef Git_UartHandle;
#endif /* __USART1_H */
