#include "config.h"
#include "Exti.h"


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
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
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
	if(EXTI_GetFlagStatus(EXTI_Line11) == SET)
	{
	
		char * msg = "Key11 has been pushed. external interupt triggered.";
		rt_mq_send(msg_mq, msg, sizeof(msg));
		
		EXTI_ClearFlag(EXTI_Line11);
	}
}

void EXTI1_IRQHandler(void)
{
	MSG_TYPE msg = MSG_KEY1_PRESS;
	
	//char * msg = "Key1 has been pushed. external interupt triggered.";
	rt_mq_send(msg_mq, &msg, sizeof(msg));
	
	EXTI_ClearFlag(EXTI_Line1);
}

#if 0
void EXTI2_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET )
	{
		//用户代码
		
		rt_mq_send(key_mq,														// 写入（发送）队列的ID(句柄) 
							"Key2(PE.2) EXIT Occur \n", 				// 写入（发送）的数据
							sizeof("Key2(PE.2) EXIT Occur \n")	// 数据的长度 
							);
		
		//--------------------------------
		EXTI_ClearFlag(EXTI_Line2);
	}
}
#endif

#if 0
void EXTI3_IRQHandler (void)
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET )
	{
		//用户代码
		
	rt_mq_send(key_mq, 												// 写入（发送）队列的ID(句柄) 
		         "Key1(PE.3) is Pressed\n",					// 写入（发送）的数据 
          		sizeof("Key1(PE.3) is Pressed\n")	// 数据的长度 
						);
		
		//--------------------------------
		EXTI_ClearFlag(EXTI_Line3);
	}
}
#endif


#if 0
void EXTI9_5_IRQHandler(void)
{
		if(EXTI_GetITStatus(EXTI_Line5) == SET )
	{
		//用户代码
		printf("PC5 Press!\n");
		
		//--------------------------------
		
		EXTI_ClearFlag(EXTI_Line5);
	}
}
#endif
