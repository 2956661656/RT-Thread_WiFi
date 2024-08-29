#include "config.h"
#include "Adc.h"

#if ADC_EN == 1

__IO uint16 ADCBuffer[CountOfADCChannel] = {0};

void AdcGpioInit(void);
void AdcDMAInit(void);
void AdcModeInit(void);

void AdcInit(void)
{
	AdcGpioInit();
	AdcDMAInit();
	AdcModeInit();

}	
INIT_BOARD_EXPORT(AdcInit);

void AdcGpioInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Configure PC.0 (ADC Channel10)、PC.1 (ADC Channel11)、PC.2 (ADC Channel12)、PC.3 (ADC Channel13) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void AdcDMAInit(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;
	
	 /* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32)(&(ADC1->DR));
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBuffer;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	DMA_InitStructure.DMA_BufferSize = CountOfADCChannel;		//传输次数
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}
void AdcModeInit(void)
{
	/* Enable ADC1clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 	
	
	/* ADC1 regular configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);//IN5-PA5通道：光敏传感器
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_55Cycles5);//IN6-PA6通道：温度传感器
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//非双ADC
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						//多通道扫描
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//连续转换关闭
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = CountOfADCChannel;
	ADC_Init(ADC1, &ADC_InitStructure);


	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
  
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	//校准
	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
		
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
}














#endif
