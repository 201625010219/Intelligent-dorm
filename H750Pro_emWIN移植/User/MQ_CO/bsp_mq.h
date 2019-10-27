#ifndef _BSP_MQ_H
#define _BSP_MQ_H


#include "stm32h7xx.h"

//���Ŷ���
/*******************************************************/
//MQ��������˿�
#define MQ_PIN                  GPIO_PIN_9                
#define MQ_GPIO_PORT            GPIOB                      
#define MQ_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

#define MQ_ON	1     // ������
#define MQ_OFF	0     // ������

uint8_t MQ_Test(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void MQ_GPIO_Config(void);

#endif
