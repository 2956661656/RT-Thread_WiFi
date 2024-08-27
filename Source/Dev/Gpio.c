#include "config.h"
#include "Gpio.h"

void LedGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1 ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
	
}
void LedControl(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
	//SysTickDelayMs(1000);
	rt_thread_mdelay(1000);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0|GPIO_Pin_1);
	//SysTickDelayMs(1000);
	rt_thread_mdelay(1000);
}

void LedToggle(GPIO_TypeDef *GPIOx, uint16 GPIO_Pin)
{
	GPIOx->ODR ^= GPIO_Pin;
}


void BeepGpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8 ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}
INIT_BOARD_EXPORT(BeepGpioInit);

void BeepOn(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
}
MSH_CMD_EXPORT(BeepOn, Turn on Beep);
void BeepOff(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
}
MSH_CMD_EXPORT(BeepOff, Turn off Beep);
