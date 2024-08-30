#include "config.h"

void weakup_thread_to_get_sensor(void)
{
	uint16 tick = 0;
	//应唤醒而不是启动
	if(bright_thread != RT_NULL){
		rt_timer_control(&(bright_thread->thread_timer), RT_TIMER_CTRL_SET_TIME, &tick);
	    rt_timer_start(&(bright_thread->thread_timer));
		rt_schedule();
	}else rt_kprintf("bright_thread = NULL!");
	if(temper_thread != RT_NULL){
		rt_timer_control(&(temper_thread->thread_timer), RT_TIMER_CTRL_SET_TIME, &tick);
	    rt_timer_start(&(temper_thread->thread_timer));
		rt_schedule();
	}else rt_kprintf("temper_thread = NULL!");

}
MSH_CMD_EXPORT(weakup_thread_to_get_sensor, RT_NULL);
