#include "config.h"
//#include <board.h>

void rt_hw_us_delay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    /* �����ʱ������ tick �� */
    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    /* ��õ�ǰʱ�� */
    told = SysTick->VAL;
    while (1)
    {
        /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}


rt_timer_t sensor_timer;

void sensor_start_convert_entry(void* params)
{
	uint32 time = 0;
	rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);//DMA���ȡ�̻߳������buffer
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	//�ȴ�DMAͨ��1ת������ź�
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET){
		//��ʱ�˳�
		if(time++ == 0xFFFFFFFF) {
			rt_kprintf("ADC|DMA time out!");break;
		}
	}
	rt_mutex_release(buffer_lock);
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

