#include "config.h"
#include "Task.h"

//static rt_thread_t led_thread;
//static rt_thread_t usart2_recv_thread = RT_NULL;
//static rt_thread_t msg_process_thread = RT_NULL;
static rt_thread_t dynamic_thread_t = RT_NULL;


void led_thread_entry(void *parameter);
void usart2_recv_thread_entry(void *parameter);
void msg_process_thread_entry(void *parameter);

//线程集中管理
TaskStruct Tasks[] = {
	/*******线程名**************入口函数******************参数*******空间**优先**时间*/
	{"led", 				led_thread_entry, 			RT_NULL,	256, 	5, 	10},
	{"usart2_recv_thread", 	usart2_recv_thread_entry, 	RT_NULL, 	512, 	2, 	10},
	{"msg_process_thread", 	msg_process_thread_entry, 	RT_NULL, 	512, 	2, 	10},
	
	
	{" ", RT_NULL, RT_NULL, RT_NULL, RT_NULL, RT_NULL}		//dummy线程结构体作作标志位
};

void TaskInit(void)
{
	usart2_recv_sem = rt_sem_create("usart2_recv_sem", 0, RT_IPC_FLAG_FIFO);
	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\n信号量usart2_recv_sem创建成功\n");
	
	msg_mq = rt_mq_create("msg_mq", 32, 10, RT_IPC_FLAG_FIFO);
	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\n消息队列key_mq创建成功\n");
	
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

//	led_thread = rt_thread_create("led", 
//									led_thread_entry, 
//									RT_NULL, 
//									256, 5, 10);		//动态创建线程
//	if(led_thread != RT_NULL){
//		rt_thread_startup(led_thread);					//将线程加入就绪队列
//	}
//	
//	usart2_recv_thread = rt_thread_create("usart2_recv_thread",
//											usart2_recv_thread_entry, RT_NULL,
//											512, 2, 10);//动态创建线程
//	if(usart2_recv_thread != RT_NULL){
//		rt_thread_startup(usart2_recv_thread);			//将线程加入就绪队列
//	}
//		
//	msg_process_thread = rt_thread_create("msg_process_thread",
//									msg_process_thread_entry, RT_NULL,
//									512, 2, 10);		//动态创建线程
//	if(msg_process_thread != RT_NULL){
//		rt_thread_startup(msg_process_thread);			//将线程加入就绪队列
//	}
}

void led_thread_entry(void *parameter)
{
	while(1){
		LedToggle(GPIOA, GPIO_Pin_1);
		rt_thread_mdelay(2000);		//GPIOA0每隔两秒反转一次
		//↑线程必须有让出cpu的条件（|rt_thread_yield），否则如果当前线程是大优先级，其他低优先级无法执行
		//? 加入高优先级执行两秒才翻转并让出执行权，那有个低优先级但1秒翻转一次的线程的时候，被迫只能3秒翻转一次吗====高优先级的线程在没有让出cpu使用权时，其他线程没有机会执行，就绪状态的线程不能翻转电平，所以要等3秒才反转电平吗？
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

void msg_process_thread_entry(void *parameter)
{
	MSG_TYPE msgBuf = MSG_NULL;		//固定消息
	while(1){
		rt_err_t result = rt_mq_recv(msg_mq, &msgBuf, sizeof(msgBuf), RT_WAITING_FOREVER);
		
		if(result == RT_EOK){
			rt_kprintf("\n接收到消息：%s\n", msgBuf);
		}else{
			rt_kprintf("\n接收消息错误，错误代码：0x%lx\n", result);
		}
	}
	
//	static uint8 msgBuf[32] = {0};
//	while(1){
//		rt_err_t result = rt_mq_recv(msg_mq, msgBuf, sizeof(msgBuf), RT_WAITING_FOREVER);
//		
//		if(result == RT_EOK){
//			rt_kprintf("\n接收到消息：%s\n", msgBuf);
//		}else{
//			rt_kprintf("\n接收消息错误，错误代码：0x%lx\n", result);
//		}
//	}
}

