#include "config.h"
#include "system.h"


void CpuInit(void)
{
	//CPU相关设置
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//选择第2组中断优先级，即2位主优先级，2位次优先级
	
}

void SysInit(void)
{
	
	CpuInit();//配置系统信息
	
	
	//外设驱动初始化
	SysTickInit();//系统滴答定时器初始化
	LedGpioInit();//LED初始化
	//BeepGpioInit();//Beep蜂鸣器初始化
	UsartInit();//串口初始化
	
	//Tim2Init(72, 1000);		//1MHz -- 1S //中断周期为1ms，用于按键扫描
	

	
}
