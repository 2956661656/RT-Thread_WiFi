#include "config.h"
#include "Button.h"


//-----------------------------------------------------------------------------------------------
// 按键线程相关参数宏定义
#define BUTTON_THREAD_STACK_SIZE 256
#define BUTTON_THREAD_PRIORITY   5


//-----------------------------------------------------------------------------------------------
// 按键事件队列
static rt_mq_t button_mq = RT_NULL;//按键事件队列定义


//-----------------------------------------------------------------------------------------------
// 按键定义
static Button_t s_tBtnKey1;			
static Button_t s_tBtnKey2;		


//-------------------------------------------------------------------------------
// Key1 按键引脚
#define GPIO_CLK_Key1			RCC_APB2Periph_GPIOB
#define GPIO_Pin_Key1			GPIO_Pin_1                  
#define GPIO_Mode_Key1			GPIO_Mode_IPU  
#define GPIO_GPIOx1				GPIOB                     
#define Key1In					GPIO_ReadInputDataBit(GPIO_GPIOx1, GPIO_Pin_Key1)	
// Key2 按键引脚
#define GPIO_CLK_Key2			RCC_APB2Periph_GPIOB
#define GPIO_Pin_Key2			GPIO_Pin_11                  
#define GPIO_Mode_Key2			GPIO_Mode_IPU  
#define GPIO_GPIOx2				GPIOB                    
#define Key2In					GPIO_ReadInputDataBit(GPIO_GPIOx2, GPIO_Pin_Key2)



//-----------------------------------------------------------------------------------------------
// Key按键按下时的电平，=0,按下时为低电平;=1,按下时为高电平
#define KeyPressedLevel 0
//-----------------------------------------------------------------------------------------------
// 获取按键按下函数	
static unsigned char IsKey1Down(void) {return (Key1In != KeyPressedLevel) ? 0 : 1;}
static unsigned char IsKey2Down(void) {return (Key2In != KeyPressedLevel) ? 0 : 1;}




/*******************************************************************************************************
** 函数: ButtonGpioInit,Button按键初始化函数
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
static void ButtonGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// 对应外设时钟与GPIO配置
	RCC_APB2PeriphClockCmd(GPIO_CLK_Key1 , ENABLE);					// 使能通道时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_Key1;					// IO口选择
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Key1;					// 配置模式
	GPIO_Init(GPIO_GPIOx1, &GPIO_InitStructure);					// 使能
	
	// 对应外设时钟与GPIO配置 
	RCC_APB2PeriphClockCmd(GPIO_CLK_Key2 , ENABLE);					// 使能通道时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_Key2;					// IO口选择
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Key2;					// 配置模式
	GPIO_Init(GPIO_GPIOx2, &GPIO_InitStructure);					// 使能					
	
}
/*******************************************************************************************************
** 函数: ButtonVarInit,初始化按键变量
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
static void ButtonVarInit(void)
{
	s_tBtnKey1.IsKeyDownFunc=IsKey1Down;							// 检测按键按下函数
	s_tBtnKey1.wFilterCount=0;										// 滤波器计数器 
	s_tBtnKey1.wFilterTime =BUTTON_FILTER_TIME;						// 滤波时间 
	s_tBtnKey1.wLongCount =0;										// 长按计数器 
	s_tBtnKey1.wLongTime=BUTTON_LONG_TIME;							// 长按时间 	
	s_tBtnKey1.byState=0;											// 按键当前状态（按下还是弹起）
	s_tBtnKey1.byKeyCodeUp=KEY1_UP;									// 按键弹起的键值代码, 0表示不检测按键弹起 
	s_tBtnKey1.byKeyCodeDown=KEY1_DOWN;								// 按键按下的键值代码, 0表示不检测按键按下
	s_tBtnKey1.byKeyCodeLong=KEY1_LONG;								// 按键长按的键值代码, 0表示不检测长按 
	
	s_tBtnKey2.IsKeyDownFunc=IsKey2Down;							// 检测按键按下函数
	s_tBtnKey2.wFilterCount=0;										// 滤波器计数器 
	s_tBtnKey2.wFilterTime =BUTTON_FILTER_TIME;						// 滤波时间 
	s_tBtnKey2.wLongCount =0;										// 长按计数器 
	s_tBtnKey2.wLongTime=BUTTON_LONG_TIME;							// 长按时间 	
	s_tBtnKey2.byState=0;											// 按键当前状态（按下还是弹起）
	s_tBtnKey2.byKeyCodeUp=KEY2_UP;									// 按键弹起的键值代码, 0表示不检测按键弹起 
	s_tBtnKey2.byKeyCodeDown=KEY2_DOWN;								// 按键按下的键值代码, 0表示不检测按键按下
	s_tBtnKey2.byKeyCodeLong=KEY2_LONG;								// 按键长按的键值代码, 0表示不检测长按 


}
/*******************************************************************************************************
** 函数: ButtonInit,Button初始化
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void ButtonInit(void)
{
	ButtonGpioInit();												// Button 按键GPIO初始化
	ButtonVarInit();												// Button 按键配置初始化
}
INIT_BOARD_EXPORT(ButtonInit);										//按键硬件接口初始化




/*******************************************************************************************************
** 函数: ButtonDetect,Button按键检测函数
**------------------------------------------------------------------------------------------------------
** 参数: ptBtn 按键结构体指针
** 返回: void
********************************************************************************************************/
static void ButtonDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// 检测按键函数是否存在，按键是否按下
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime){				// 滤波检测，滤波计数器到达滤波时间
			ptBtn->wFilterCount++;									// 计数器加一
			return;													// 退出检测函数
		}
		if(ptBtn->byState ==0 ){ 									// 检测是否是按键按下							
			ptBtn->byState = 1;
			rt_mq_send(button_mq,									// 写入（发送）队列的ID(句柄)
						&(ptBtn->byKeyCodeDown),  					// 写入（发送）的数据所对应地址 
						sizeof(ptBtn->byKeyCodeDown)				// 数据的长度 
						);
			return;
		}
		if( ptBtn->wLongCount++ == ptBtn->wLongTime){ 				// 检测是否是按键长按，长按计数器是否到达长按时间
			rt_mq_send(button_mq,									// 写入（发送）队列的ID(句柄)
						&(ptBtn->byKeyCodeLong),  					// 写入（发送）的数据所对应地址 
						sizeof(ptBtn->byKeyCodeLong)				// 数据的长度 
						);
			return;
		}			
	}else{		
		if(ptBtn->wFilterCount){ 									// 滤波检测，滤波计数器是否为0
			ptBtn->wFilterCount--;									// 计数器减一
			return;													// 退出检测函数
		}			
		if(ptBtn->byState ==1 ){									// 检测是否是按键弹起
			ptBtn->byState =0; 										
			rt_mq_send(button_mq,									// 写入（发送）队列的ID(句柄)
						&(ptBtn->byKeyCodeUp),  					// 写入（发送）的数据所对应地址 
						sizeof(ptBtn->byKeyCodeUp)					// 数据的长度 
						);
		}

		ptBtn->wLongCount = 0;										// 按键长按计数器清零
	}
	
}

/*******************************************************************************************************
** 函数: ButtonProj,按键处理（扫描）函数
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void ButtonProcess(void)
{
	//该函数在定时器中断回调函数中调用，定时中断周期为1ms
	ButtonDetect(&s_tBtnKey1);										// 检测 K1 键 
	ButtonDetect(&s_tBtnKey2);										// 检测 K2 键
	
}




/*******************************************************************************************************
** 函数: button_thread_entry,获取按键事件并进行处理
**------------------------------------------------------------------------------------------------------
** 参数: void
** 返回: void
********************************************************************************************************/
void button_thread_entry(void *parameter)//用户消息处理入口函数
{
	rt_err_t uwRet = RT_EOK;
	uint8_t r_queue;//用于接收msg_mq消息队列信息
	
	button_mq = rt_mq_create("button_mq",							//消息队列名字
							1,  									//消息的最大长度, bytes
							256,									//消息队列的最大容量(个数)
							RT_IPC_FLAG_FIFO						//队列模式 FIFO
							);
	if(button_mq != RT_NULL) rt_kprintf("button_mq create success\n\n");
	
	while(1)
	{  //获取队列信息
		 uwRet = rt_mq_recv(button_mq,
							&r_queue,
							sizeof(r_queue),
							RT_WAITING_FOREVER
							);
		 if(RT_EOK == uwRet){
			 switch(r_queue)//根据接收到的消息内容分别进行处理
			 {
				 case KEY1_DOWN: rt_kprintf("Receive message:KEY1(PE.3) Down\n\n");break;
				 case KEY1_UP:	 rt_kprintf("Receive message:KEY1(PE.3) Up\n\n");break;
				 case KEY1_LONG: rt_kprintf("Receive message:KEY1(PE.3) LongPressed Down\n\n");break;
				 case KEY2_DOWN: rt_kprintf("Receive message:KEY2(PE.2) Down\n\n");break;
				 case KEY2_UP:	 rt_kprintf("Receive message:KEY2(PE.2) Up\n\n");break;
				 case KEY2_LONG: rt_kprintf("Receive message:KEY2(PE.2) LongPressed Down\n\n");break;
				 
				 default: rt_kprintf("No button Message!\n\n");break;
			 }
		 }else{
			 rt_kprintf("数据接收错误，错误代码：0x%lx\n\n",uwRet);
		 }
	}	
	
}
int button_process_init(void)
{
    rt_thread_t tid;

    tid = rt_thread_create("button_process",
                           button_thread_entry, RT_NULL,
                           BUTTON_THREAD_STACK_SIZE, BUTTON_THREAD_PRIORITY, 10);


    if (tid != NULL) rt_thread_startup(tid);
    return 0;
}
INIT_APP_EXPORT(button_process_init);





