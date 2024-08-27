#include "config.h"
#include "BspTimer.h"



void Tim2ModeInit(int prescaler,int period)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_DeInit(TIM2);										//重新将Timer设置为缺省值
	TIM_InternalClockConfig(TIM2);							//采用内部时钟给TIM2提供时钟源
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(prescaler-1);//例如prescaler=7200;			//预分频系数为7200-1，这样计数器时钟为72MHz/7200 = 10kHz。注意：预分频系数取值范围为1-65535
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//设置计数器模式为向上计数模式
	TIM_TimeBaseStructure.TIM_Period = (period<2)?1:period-1;		//设置计数溢出大小，每计period个数就产生一个更新事件
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);			//将配置应用到TIM2中
	TIM_ARRPreloadConfig(TIM2, DISABLE);					//禁止ARR预装载缓冲器
}
void Tim2NvicInit(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	// 配置TIM中断	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      	// 选择TIM2的中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 抢占式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		// 响应式中断优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// 中断使能
	NVIC_Init(&NVIC_InitStructure);							// 配置中断
}

/*******************************************************************************************************
** 函数: Tim2Init, Tim定时器延时函数
**------------------------------------------------------------------------------------------------------
** 参数: prescaler 预分频系数
         period  计数周期
Example:
若：
	TIM_Period 即Auto Reload Register(ARR) = 1000  , TIM_Prescaler--71 
则：中断周期为 = 1/(72MHZ /72) * 1000 = 1ms

	TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> 中断 且TIMxCNT重置为0重新计数 
        
** 返回: void
********************************************************************************************************/
void Tim2Init(int prescaler,int period)
{
	Tim2ModeInit(prescaler,period);
	Tim2NvicInit();

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);					//清除溢出中断标志
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);				//开启TIM2的中断
	TIM_Cmd(TIM2,ENABLE);
	
}


void  TIM2_IRQHandler(void)
{


	if ( TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET ) 
	{	
		//----------------------------------------------------------
		//填充用户代码 Add User Code
		
		ButtonProcess();//按键定时扫描

			
		

		//---------------------------------------------------------
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}



