#include "config.h"


void ExtiGpioInit(void);
void ExtiNvicInit(void);
void ExtiModeInit(void);

void ExtiInit(void)
{
	ExtiGpioInit();
	ExtiNvicInit();
	ExtiModeInit();
	
}
INIT_BOARD_EXPORT(ExtiInit);

void ExtiGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

rt_inline void ExtiNvicInit(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
rt_inline void ExtiModeInit(void)
{
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);		//AFIO选择引脚
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;					//配置EXTI
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

void EXTI15_10_IRQHandler(void)
{
	//todo 按键消抖
	
	if(EXTI_GetFlagStatus(EXTI_Line11) == SET)
	{
		if(sensor_timer->parent.flag & RT_TIMER_FLAG_ACTIVATED)
		{
			rt_timer_stop(sensor_timer);		//停止检测传感器
			bright_thread->user_data = 1;		//1表示 停止
		}else{
			rt_timer_start(sensor_timer);		//开始检测传感器
			bright_thread->user_data = 0;
			weakup_thread_to_get_sensor();
		}
		rt_kprintf("\nsensor switch to:%d\n\n", !bright_thread->user_data);
		
//		char * msg = "Key11 has been pushed. external interupt triggered.";
//		rt_mq_send(msg_mq, msg, sizeof(msg));
		
		EXTI_ClearFlag(EXTI_Line11);
	}
}
#if 0
void EXTI1_IRQHandler(void)
{
	MSG_TYPE msg = MSG_KEY1_PRESS;
	
	//char * msg = "Key1 has been pushed. external interupt triggered.";
	rt_mq_send(msg_mq, &msg, sizeof(msg));
	
	EXTI_ClearFlag(EXTI_Line1);
}
#endif
