#include "config.h"

rt_mq_t sensor_mq;

void SensorMQInit(void)
{
	
	sensor_mq = rt_mq_create("sensor_mq",16, 2, RT_IPC_FLAG_FIFO);
	if(sensor_mq != RT_NULL)	rt_kprintf("\nsensor_mq init successed\n");
	
}
INIT_PREV_EXPORT(SensorMQInit);
