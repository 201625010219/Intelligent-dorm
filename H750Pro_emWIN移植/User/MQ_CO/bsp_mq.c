#include "./MQ_CO/bsp_mq.h" 




/**
  * @brief  配置烟雾传感器用到的I/O口
  * @param  无
  * @retval 无
  */
void MQ_GPIO_Config(void)
{
	
/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*开启端口的时钟*/
	MQ_GPIO_CLK_ENABLE();
	
	//选择输入的引脚
	GPIO_InitStruct.Pin = MQ_PIN; 
	
  /*设置引脚的输出类型为推挽输出*/
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;  
  
 	/*设置引脚为上拉模式*/
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	
	/*设置引脚速率为高速 */   
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 

	/*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
	HAL_GPIO_Init(MQ_GPIO_PORT, &GPIO_InitStruct);	
    
    HAL_GPIO_WritePin(MQ_GPIO_PORT,MQ_PIN,1);

}



 /*
 * 函数名：烟雾传感器测试
 * 描述  ：检测烟雾传感器的状态
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：MQ_OFF：有烟雾  MQ_ON：无烟雾
 */
uint8_t MQ_Test(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测光敏输入状态 */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == 0 )  
	{	  
		return MQ_OFF;    // 有烟雾
	}
	else
		return MQ_ON;    // 无烟雾
}
/*********************************************END OF FILE**********************/
