#include "config.h"
#include "Task.h"


rt_thread_t bright_thread;
rt_thread_t temper_thread;


static rt_thread_t dynamic_thread_t = RT_NULL;


void led_thread_entry(void *parameter);
void usart2_recv_thread_entry(void *parameter);
//void msg_process_thread_entry(void *parameter);
void temper_checkout_thread_entry(void *params);
void bright_checkout_thread_entry(void *params);

//线程集中管理
TaskStruct Tasks[] = {
	/*******线程名**************入口函数**********************参数*******空间**优先**时间*/
	{"led", 				led_thread_entry, 				RT_NULL,	256,   11, 	10},
	{"usart2_recv_thread", 	usart2_recv_thread_entry, 		RT_NULL, 	512, 	1, 	10},
/*	{"msg_process_thread", 	msg_process_thread_entry, 		RT_NULL, 	512, 	5, 	10},*/
	{"bright_checkout", 	bright_checkout_thread_entry, 	RT_NULL, 	512, 	2, 	10},
	{"temper_checkout", 	temper_checkout_thread_entry, 	RT_NULL, 	512, 	3, 	10},
	
	
	{" ", RT_NULL, RT_NULL, RT_NULL, RT_NULL, RT_NULL}		//dummy线程结构体作作标志位
};

void TaskInit(void)
{
	usart2_recv_sem = rt_sem_create("usart2_recv_sem", 0, RT_IPC_FLAG_FIFO);
	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\nusart2_recv_sem init successed\n");
	
//	msg_mq = rt_mq_create("msg_mq", 32, 10, RT_IPC_FLAG_FIFO);
//	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\n消息队列key_mq创建成功\n");
	
//========================================================================================================
//	rt_thread_t rt_thread_create(const char *name,
//                             void (*entry)(void *parameter),
//                             void       *parameter,
//                             rt_uint32_t stack_size,
//                             rt_uint8_t  priority,
//                             rt_uint32_t tick);
	uint8 TaskIndex = 0;
	
	while(strcmp("",Tasks[TaskIndex].name) != 0)//等于零字符串相等
	{	
		dynamic_thread_t = rt_thread_create(Tasks[TaskIndex].name, 
											Tasks[TaskIndex].entry, 
											Tasks[TaskIndex].parameter, 
											Tasks[TaskIndex].stack_size, 
											Tasks[TaskIndex].priority, 
											Tasks[TaskIndex].tick);
		if(dynamic_thread_t != RT_NULL){
			rt_thread_startup(dynamic_thread_t);					//将线程加入就绪队列
		}
		
		++TaskIndex;
	}
	
//	bright_thread = rt_thread_create("bright_thread", 
//										bright_checkout_thread_entry, 
//										RT_NULL, 512, 2, 10);
//	if(bright_thread != RT_NULL){
//		rt_thread_startup(bright_thread);					//将线程加入就绪队列
//	}
//	temper_thread = rt_thread_create("temper_checkout", 
//										temper_checkout_thread_entry, 
//										RT_NULL, 512, 3, 10);
//	if(temper_thread != RT_NULL){
//		rt_thread_startup(temper_thread);					//将线程加入就绪队列
//	}
}
INIT_APP_EXPORT(TaskInit);


void led_thread_entry(void *parameter)
{
	while(1){
		LedToggle(GPIOA, GPIO_Pin_1);
		rt_thread_mdelay(2000);		//GPIOA0每隔两秒反转一次
		//↑线程必须有让出cpu的条件（|rt_thread_yield），否则如果当前线程是大优先级，其他低优先级无法执行
//		rt_kprintf("System Running Time:%d s\n ", rt_tick_get() / RT_TICK_PER_SECOND);
	}
}

void usart2_recv_thread_entry(void *parameter)
{
	while(1){
		rt_err_t result = rt_sem_take(usart2_recv_sem, RT_WAITING_FOREVER);
		if(result == RT_EOK){
			//串口接收到数据
			rt_kprintf("\nUsart2 has recevied data: %s.\n", g_USART1_RxBuf);
			if(strstr((char*)g_USART1_RxBuf, "BeepOn")){
				BeepOn();
			}
			if(strstr((char*)g_USART1_RxBuf, "BeepOff")){
				BeepOff();
			}
			memset(g_USART1_RxBuf, 0, USART2_RX_BUF_SIZE);
			g_USART1_RecPos = 0;
		}
	}
}

//void msg_process_thread_entry(void *parameter)
//{
//	MSG_TYPE msgBuf = MSG_NULL;		//固定消息
//	while(1){
//		rt_err_t result = rt_mq_recv(msg_mq, &msgBuf, sizeof(msgBuf), RT_WAITING_FOREVER);
//		
//		if(result == RT_EOK){
//			rt_kprintf("\n接收到消息：%s\n", msgBuf);
//		}else{
//			rt_kprintf("\n接收消息错误，错误代码：0x%lx\n", result);
//		}
//	}
//}

void bright_checkout_thread_entry(void *params)		//优先级高
{
	uint16_t temper = 0;
	uint16_t brightness = 0;

	while(1){
		//互斥 读取buffer[0]
		rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);
		brightness = ADCBuffer[0];
		rt_mutex_release(buffer_lock);
		//等待另一个函数的消息
		rt_mq_recv(sensor_mq, &temper, sizeof(temper), RT_WAITING_FOREVER);
		if(temper == 0){		//失败
			rt_kprintf("获取temperature失败\n");
		}else{
			//成功，连接结果
			rt_kprintf("\nbrightness: %d, temperature: %d\n", brightness, temper);
		}
		
		
		rt_thread_mdelay(0xFFFF);
//		rt_thread_suspend(bright_thread);
//		rt_schedule();
	}
}

void temper_checkout_thread_entry(void *params)		//优先级低
{
	uint16_t temper = 0;
	while(1){
		//互斥 读取buffer[1]
		rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);
		temper = ADCBuffer[1];
		rt_mutex_release(buffer_lock);
		//发送消息
		rt_mq_send(sensor_mq, &temper, sizeof(temper));
		
		
		
		//rt_thread_mdelay(1000);		//todo BUG 不允许该线程睡眠
//		rt_thread_suspend(temper_thread);
//		rt_schedule();		
	}//todo 应使两个线程睡眠
}

