#include "config.h"

rt_timer_t sensor_timer;

void sensor_start_convert_entry(void* params)
{
	//rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);//todo DMA与读取线程互斥访问buffer
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//rt_mutex_release(buffer_lock);
}

void SensorTimerInit(void)
{
	sensor_timer = rt_timer_create("ad_convert_timer",
									sensor_start_convert_entry,
									RT_NULL,
									2 * RT_TICK_PER_SECOND,
									RT_TIMER_FLAG_PERIODIC);
	rt_timer_start(sensor_timer);
	
}
INIT_ENV_EXPORT(SensorTimerInit);

