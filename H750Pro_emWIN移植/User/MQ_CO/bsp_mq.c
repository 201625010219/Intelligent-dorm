#include "./MQ_CO/bsp_mq.h" 




/**
  * @brief  �������������õ���I/O��
  * @param  ��
  * @retval ��
  */
void MQ_GPIO_Config(void)
{
	
/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*�����˿ڵ�ʱ��*/
	MQ_GPIO_CLK_ENABLE();
	
	//ѡ�����������
	GPIO_InitStruct.Pin = MQ_PIN; 
	
  /*�������ŵ��������Ϊ�������*/
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  
  
 	/*��������Ϊ����ģʽ*/
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	
	/*������������Ϊ���� */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	HAL_GPIO_Init(MQ_GPIO_PORT, &GPIO_InitStruct);	
    
    HAL_GPIO_WritePin(MQ_GPIO_PORT,MQ_PIN,1);

}



 /*
 * ��������������������
 * ����  ���������������״̬
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��MQ_OFF��������  MQ_ON��������
 */
uint8_t MQ_Test(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����������״̬ */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == 0 )  
	{	  
		return MQ_OFF;    // ������
	}
	else
		return MQ_ON;    // ������
}
/*********************************************END OF FILE**********************/
