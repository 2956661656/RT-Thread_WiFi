#include "config.h"
#include "SysTick.h"


void SysTickInit(void)
{
	
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	
}
