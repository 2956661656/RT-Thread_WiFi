#ifndef __ADC_H__
#define __ADC_H__




#define ADC_EN   1


#if ADC_EN == 1

#define CountOfADCChannel 2
extern __IO uint16 ADCBuffer[CountOfADCChannel];		//buffer
void AdcInit(void);


#endif





#endif
