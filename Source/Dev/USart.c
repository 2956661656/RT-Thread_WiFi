#include "config.h"
#include "USart.h"

#if USART1_EN == 1
	uint8   g_USART1_RxBuf[USART1_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART1_RecPos = 0;//存放当前串口接收数据存放的位置
#endif 

#if USART2_EN == 1
	uint8   g_USART2_RxBuf[USART2_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART2_RecPos = 0;//存放当前串口接收数据存放的位置
#endif 

#if USART3_EN == 1
	uint8   g_USART3_RxBuf[USART3_RX_BUF_SIZE];//串口接收缓冲区
	uint16 	g_USART3_RecPos = 0;//存放当前串口接收数据存放的位置
#endif 

void UsartInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
#if USART1_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART1_BAUD;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity =  USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);//初始化串口1
#if 0	// USART1_INT_INIT
	//串口1中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
	USART_Cmd(USART1, ENABLE);//使能串口1
	
	USART_ClearFlag(USART1, USART_FLAG_TC  );//清发送完成标志位
#endif


#if USART2_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART2_BAUD;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity =  USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);//初始化串口2
	
	//串口2中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//使能接收缓存区非空中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);		//使能空闲中断

	USART_Cmd(USART2, ENABLE);//使能串口2
	
	USART_ClearFlag(USART2, USART_FLAG_TC  );//清发送完成标志位
#endif

#if USART3_EN == 1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 ;  //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 ;  //RX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = USART3_BAUD;  
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity =  USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStructure);//初始化串口3
	
	//串口3中断初始化
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART3, ENABLE);//使能串口3
	
  USART_ClearFlag(USART3, USART_FLAG_TC  );//清发送完成标志位
#endif

}


//为能够调用printf函数从选定的串口打印输出，重定义fputc函数，本例子使用串口1
int fputc(int ch,FILE *f)
{
	
	USART_SendData(USART1, (uint8)ch);
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)== RESET);
	
	return (ch);

}

static void UsartSendByte(USART_TypeDef* USARTx,uint8 ch)
{
	
	USART_SendData(USARTx, (uint8)ch);
	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)== RESET);
}


void UsartSendString(USART_TypeDef* USARTx,uint8 *str)
{
	uint32 pos = 0;
	while(*(str+pos)!='\0')
	{
		UsartSendByte(USARTx,*(str+pos));
		pos ++;
		
	}

}




//USART1_IRQHandler,串口1中断回调函数
void USART1_IRQHandler(void)
{
#if USART1_EN == 1 
	uint8 RecCh;
#endif
	if( USART_GetFlagStatus(USART1,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
#if USART1_EN == 1 
		
		
#endif
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART1, USART_FLAG_ORE);
	}
}




//USART2_IRQHandler,串口2中断回调函数
void USART2_IRQHandler(void)
{
	#if USART2_EN == 1 
		//用户代码
	#endif
	if( USART_GetFlagStatus(USART2,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
		#if USART2_EN == 1 
		 
		//用户代码
		uint8 RevChar = (uint8) USART_ReceiveData(USART2);
		g_USART2_RxBuf[g_USART2_RecPos++] = RevChar;//串口接收缓冲区
		 
		#endif
		USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	if( USART_GetFlagStatus(USART2,USART_FLAG_IDLE)==SET ) 				// 串口空闲中断
	{
		#if USART2_EN == 1 
		 
		//用户代码
		
		g_USART2_RxBuf[g_USART2_RecPos++] = '\0';//串口接收缓冲区
		rt_sem_release(usart2_recv_sem);
		 
		#endif
		USART_ReceiveData(USART2); //USART_ClearFlag(USART2, USART_FLAG_RXNE);
	}
}


//USART3_IRQHandler,串口3中断回调函数
void USART3_IRQHandler(void)
{
	#if USART3_EN == 1 
		//用户代码
	#endif

	if( USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET )				// 串口接收数据 
	{		
		#if USART3_EN == 1 
			//用户代码
		#endif
		
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
	}
	if( USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET ) 				// 串口溢出错误
	{
			USART_ClearFlag(USART3, USART_FLAG_ORE);
	}
}

void rt_hw_console_output(const char *str)
{
	rt_enter_critical();		//进入临界段，关中断，rt_scheduler_lock_nest>=1停止调度
	
	while(*str != 0)
	{
		if(*str == '\n'){
			USART_SendData(USART1, '\r');
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);				
		}
		USART_SendData(USART1, *str++);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
	
	rt_exit_critical();			//退出临界段
}


char rt_hw_console_getchar(void)
{
	int ch = -1;
	
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
		ch = (int)USART_ReceiveData(USART1);
		//USART_ClearFlag(USART1, USART_FLAG_RXNE);
	}else{
		if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET){
			USART_ClearFlag(USART1, USART_FLAG_ORE);
		}
		rt_thread_mdelay(10);
	}
	return ch;
}
