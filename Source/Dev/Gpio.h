#ifndef __GPIO_H__
#define __GPIO_H__



void LedGpioInit(void);//Led�ܽų�ʼ��
void LedControl(void); //Led���ƴ���
void BeepGpioInit(void);
void BeepOn(void);
void BeepOff(void);
void LedToggle(GPIO_TypeDef *GPIOx, uint16 GPIO_Pin);//��תGPIO

#endif



