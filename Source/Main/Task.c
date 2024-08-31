#include "config.h"
#include "Task.h"

//ALIGN(RT_ALIGN_SIZE)
//static char bright_thread_stack[1024];
//static struct rt_thread bright_thread_s;
//ALIGN(RT_ALIGN_SIZE)
//static char temper_thread_stack[1024];
//static struct rt_thread temper_thread_s;

rt_thread_t bright_thread;
rt_thread_t temper_thread;


static rt_thread_t dynamic_thread_t = RT_NULL;


void led_thread_entry(void *parameter);
void usart2_recv_thread_entry(void *parameter);
//void msg_process_thread_entry(void *parameter);
void temper_checkout_thread_entry(void *params);
void bright_checkout_thread_entry(void *params);

//�̼߳��й���
TaskStruct Tasks[] = {
	/*******�߳���**************��ں���**********************����*******�ռ�**����**ʱ��*/
	{"led", 				led_thread_entry, 				RT_NULL,	512,   10, 	10},
/*	{"msg_process_thread", 	msg_process_thread_entry, 		RT_NULL, 	512, 	5, 	10},*/
	{"bright_checkout", 	bright_checkout_thread_entry, 	RT_NULL, 	512, 	2, 	10},
	{"temper_checkout", 	temper_checkout_thread_entry, 	RT_NULL, 	512, 	3, 	10},
	{"usart2_recv_thread", 	usart2_recv_thread_entry, 		RT_NULL, 	512, 	1, 	10},
	
	
	{"", RT_NULL, RT_NULL, RT_NULL, RT_NULL, RT_NULL}		//dummy�߳̽ṹ��������־λ
};

void TaskInit(void)
{
	usart2_recv_sem = rt_sem_create("usart2_recv_sem", 0, RT_IPC_FLAG_FIFO);
	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\nusart2_recv_sem init successed\n");
	
//	msg_mq = rt_mq_create("msg_mq", 32, 10, RT_IPC_FLAG_FIFO);
//	if(usart2_recv_sem != RT_NULL)	rt_kprintf("\n��Ϣ����key_mq�����ɹ�\n");
	
//========================================================================================================
//	rt_thread_t rt_thread_create(const char *name,
//                             void (*entry)(void *parameter),
//                             void       *parameter,
//                             rt_uint32_t stack_size,
//                             rt_uint8_t  priority,
//                             rt_uint32_t tick);
	uint8 TaskIndex = 0;
	
	while(strcmp("",Tasks[TaskIndex].name) != 0)//�������ַ������
	{	
		dynamic_thread_t = rt_thread_create(Tasks[TaskIndex].name, 
											Tasks[TaskIndex].entry, 
											Tasks[TaskIndex].parameter, 
											Tasks[TaskIndex].stack_size, 
											Tasks[TaskIndex].priority, 
											Tasks[TaskIndex].tick);
		if(dynamic_thread_t != RT_NULL){
			rt_thread_startup(dynamic_thread_t);					//���̼߳����������
		}
		
		++TaskIndex;
	}
//	rt_err_t result = RT_EOK;
//	
//	result = rt_thread_init(&bright_thread_s,
//							"bright_thread",
//							bright_checkout_thread_entry, RT_NULL,
//							bright_thread_stack, sizeof(bright_thread_stack),
//							2, 10);
//	if(result == RT_EOK) rt_thread_startup(&bright_thread_s);
//	result = rt_thread_init(&temper_thread_s,
//							"temper_thread",
//							temper_checkout_thread_entry, RT_NULL,
//							temper_thread_stack, sizeof(temper_thread_stack),
//							3, 10);
//	if(result == RT_EOK) rt_thread_startup(&temper_thread_s);

	
}
INIT_APP_EXPORT(TaskInit);


void led_thread_entry(void *parameter)
{
	while(1){
		LedToggle(GPIOA, GPIO_Pin_1);
		rt_thread_mdelay(2000);		//GPIOA0ÿ�����뷴תһ��
		//���̱߳������ó�cpu��������|rt_thread_yield�������������ǰ�߳��Ǵ����ȼ������������ȼ��޷�ִ��
//		rt_kprintf("System Running Time:%d s\n ", rt_tick_get() / RT_TICK_PER_SECOND);
	}
}

void usart2_recv_thread_entry(void *parameter)
{
	rt_err_t result;
	while(1){
		result = rt_sem_take(usart2_recv_sem, RT_WAITING_FOREVER);
		if(result == RT_EOK){
			//���ڽ��յ�����
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
//	MSG_TYPE msgBuf = MSG_NULL;		//�̶���Ϣ
//	while(1){
//		rt_err_t result = rt_mq_recv(msg_mq, &msgBuf, sizeof(msgBuf), RT_WAITING_FOREVER);
//		
//		if(result == RT_EOK){
//			rt_kprintf("\n���յ���Ϣ��%s\n", msgBuf);
//		}else{
//			rt_kprintf("\n������Ϣ���󣬴�����룺0x%lx\n", result);
//		}
//	}
//}

void bright_checkout_thread_entry(void *params)		//���ȼ���
{
	bright_thread = rt_thread_self();
	uint16_t temper = 0;
	uint16_t brightness = 0;

	while(1){		//���������ⲿ�ж�-������1[PA11]����
		if(bright_thread->user_data == 0){
			//���� ��ȡbuffer[0]
			rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);
			brightness = ADCBuffer[0];
			rt_mutex_release(buffer_lock);
			//�ȴ���һ����������Ϣ
			rt_mq_recv(sensor_mq, &temper, sizeof(temper), RT_WAITING_FOREVER);
			if(temper == 0){		//ʧ��
				rt_kprintf("��ȡtemperatureʧ��\n\n");
			}else{
				//�ɹ������ӽ��
				rt_kprintf("\nbrightness: %d, temperature: %d\n\n", brightness, temper);
			}
			
			
	//		rt_kprintf("bright test\n");
//			bright_thread = rt_thread_self();
			rt_thread_mdelay(0xFFFFFF);
	//		rt_thread_suspend(bright_thread);
	//		rt_schedule();
		}
		else{
			rt_kprintf("\nbright_thread on stopping...\n\n");
			rt_thread_mdelay(0xFFFFFF);
		}
	}
}

void temper_checkout_thread_entry(void *params)		//���ȼ���
{
	temper_thread = rt_thread_self();
	uint16_t temper = 0;
	while(1){
		if(bright_thread->user_data == 0){
			//���� ��ȡbuffer[1]
			rt_mutex_take(buffer_lock, RT_WAITING_FOREVER);
			temper = ADCBuffer[1];
			rt_mutex_release(buffer_lock);
			//������Ϣ
			rt_mq_send(sensor_mq, &temper, sizeof(temper));
			
	//		rt_kprintf("temper test\n");
			
//			temper_thread = rt_thread_self();
			rt_thread_mdelay(0xFFFFFF);
	//		rt_thread_suspend(temper_thread);
	//		rt_schedule();		
		}
		else{
			rt_kprintf("\ntemper_thread stopping...\n\n");
			rt_thread_mdelay(0xFFFFFF);
		}
	}
}

