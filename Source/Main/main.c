#define  MAIN_CONFIG


#include "config.h"
#include "stm32f10x.h"


int main(void)
{
	
	while(1)
	{
		LedToggle(GPIOA, GPIO_Pin_0);
		rt_thread_mdelay(1000);		//GPIOA0每隔一秒反转一次
		
	}
	
}

