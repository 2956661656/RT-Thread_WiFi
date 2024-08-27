#include "config.h"
#include "Button.h"


//-----------------------------------------------------------------------------------------------
// �����߳���ز����궨��
#define BUTTON_THREAD_STACK_SIZE 256
#define BUTTON_THREAD_PRIORITY   5


//-----------------------------------------------------------------------------------------------
// �����¼�����
static rt_mq_t button_mq = RT_NULL;//�����¼����ж���


//-----------------------------------------------------------------------------------------------
// ��������
static Button_t s_tBtnKey1;			
static Button_t s_tBtnKey2;		


//-------------------------------------------------------------------------------
// Key1 ��������
#define GPIO_CLK_Key1			RCC_APB2Periph_GPIOB
#define GPIO_Pin_Key1			GPIO_Pin_1                  
#define GPIO_Mode_Key1			GPIO_Mode_IPU  
#define GPIO_GPIOx1				GPIOB                     
#define Key1In					GPIO_ReadInputDataBit(GPIO_GPIOx1, GPIO_Pin_Key1)	
// Key2 ��������
#define GPIO_CLK_Key2			RCC_APB2Periph_GPIOB
#define GPIO_Pin_Key2			GPIO_Pin_11                  
#define GPIO_Mode_Key2			GPIO_Mode_IPU  
#define GPIO_GPIOx2				GPIOB                    
#define Key2In					GPIO_ReadInputDataBit(GPIO_GPIOx2, GPIO_Pin_Key2)



//-----------------------------------------------------------------------------------------------
// Key��������ʱ�ĵ�ƽ��=0,����ʱΪ�͵�ƽ;=1,����ʱΪ�ߵ�ƽ
#define KeyPressedLevel 0
//-----------------------------------------------------------------------------------------------
// ��ȡ�������º���	
static unsigned char IsKey1Down(void) {return (Key1In != KeyPressedLevel) ? 0 : 1;}
static unsigned char IsKey2Down(void) {return (Key2In != KeyPressedLevel) ? 0 : 1;}




/*******************************************************************************************************
** ����: ButtonGpioInit,Button������ʼ������
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
static void ButtonGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// ��Ӧ����ʱ����GPIO����
	RCC_APB2PeriphClockCmd(GPIO_CLK_Key1 , ENABLE);					// ʹ��ͨ��ʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_Key1;					// IO��ѡ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Key1;					// ����ģʽ
	GPIO_Init(GPIO_GPIOx1, &GPIO_InitStructure);					// ʹ��
	
	// ��Ӧ����ʱ����GPIO���� 
	RCC_APB2PeriphClockCmd(GPIO_CLK_Key2 , ENABLE);					// ʹ��ͨ��ʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_Key2;					// IO��ѡ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Key2;					// ����ģʽ
	GPIO_Init(GPIO_GPIOx2, &GPIO_InitStructure);					// ʹ��					
	
}
/*******************************************************************************************************
** ����: ButtonVarInit,��ʼ����������
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
static void ButtonVarInit(void)
{
	s_tBtnKey1.IsKeyDownFunc=IsKey1Down;							// ��ⰴ�����º���
	s_tBtnKey1.wFilterCount=0;										// �˲��������� 
	s_tBtnKey1.wFilterTime =BUTTON_FILTER_TIME;						// �˲�ʱ�� 
	s_tBtnKey1.wLongCount =0;										// ���������� 
	s_tBtnKey1.wLongTime=BUTTON_LONG_TIME;							// ����ʱ�� 	
	s_tBtnKey1.byState=0;											// ������ǰ״̬�����»��ǵ���
	s_tBtnKey1.byKeyCodeUp=KEY1_UP;									// ��������ļ�ֵ����, 0��ʾ����ⰴ������ 
	s_tBtnKey1.byKeyCodeDown=KEY1_DOWN;								// �������µļ�ֵ����, 0��ʾ����ⰴ������
	s_tBtnKey1.byKeyCodeLong=KEY1_LONG;								// ���������ļ�ֵ����, 0��ʾ����ⳤ�� 
	
	s_tBtnKey2.IsKeyDownFunc=IsKey2Down;							// ��ⰴ�����º���
	s_tBtnKey2.wFilterCount=0;										// �˲��������� 
	s_tBtnKey2.wFilterTime =BUTTON_FILTER_TIME;						// �˲�ʱ�� 
	s_tBtnKey2.wLongCount =0;										// ���������� 
	s_tBtnKey2.wLongTime=BUTTON_LONG_TIME;							// ����ʱ�� 	
	s_tBtnKey2.byState=0;											// ������ǰ״̬�����»��ǵ���
	s_tBtnKey2.byKeyCodeUp=KEY2_UP;									// ��������ļ�ֵ����, 0��ʾ����ⰴ������ 
	s_tBtnKey2.byKeyCodeDown=KEY2_DOWN;								// �������µļ�ֵ����, 0��ʾ����ⰴ������
	s_tBtnKey2.byKeyCodeLong=KEY2_LONG;								// ���������ļ�ֵ����, 0��ʾ����ⳤ�� 


}
/*******************************************************************************************************
** ����: ButtonInit,Button��ʼ��
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
void ButtonInit(void)
{
	ButtonGpioInit();												// Button ����GPIO��ʼ��
	ButtonVarInit();												// Button �������ó�ʼ��
}
INIT_BOARD_EXPORT(ButtonInit);										//����Ӳ���ӿڳ�ʼ��




/*******************************************************************************************************
** ����: ButtonDetect,Button������⺯��
**------------------------------------------------------------------------------------------------------
** ����: ptBtn �����ṹ��ָ��
** ����: void
********************************************************************************************************/
static void ButtonDetect(Button_t *ptBtn)
{
	if(ptBtn->IsKeyDownFunc && ptBtn->IsKeyDownFunc()) 				// ��ⰴ�������Ƿ���ڣ������Ƿ���
	{

		if(ptBtn->wFilterCount < ptBtn->wFilterTime){				// �˲���⣬�˲������������˲�ʱ��
			ptBtn->wFilterCount++;									// ��������һ
			return;													// �˳���⺯��
		}
		if(ptBtn->byState ==0 ){ 									// ����Ƿ��ǰ�������							
			ptBtn->byState = 1;
			rt_mq_send(button_mq,									// д�루���ͣ����е�ID(���)
						&(ptBtn->byKeyCodeDown),  					// д�루���ͣ�����������Ӧ��ַ 
						sizeof(ptBtn->byKeyCodeDown)				// ���ݵĳ��� 
						);
			return;
		}
		if( ptBtn->wLongCount++ == ptBtn->wLongTime){ 				// ����Ƿ��ǰ��������������������Ƿ񵽴ﳤ��ʱ��
			rt_mq_send(button_mq,									// д�루���ͣ����е�ID(���)
						&(ptBtn->byKeyCodeLong),  					// д�루���ͣ�����������Ӧ��ַ 
						sizeof(ptBtn->byKeyCodeLong)				// ���ݵĳ��� 
						);
			return;
		}			
	}else{		
		if(ptBtn->wFilterCount){ 									// �˲���⣬�˲��������Ƿ�Ϊ0
			ptBtn->wFilterCount--;									// ��������һ
			return;													// �˳���⺯��
		}			
		if(ptBtn->byState ==1 ){									// ����Ƿ��ǰ�������
			ptBtn->byState =0; 										
			rt_mq_send(button_mq,									// д�루���ͣ����е�ID(���)
						&(ptBtn->byKeyCodeUp),  					// д�루���ͣ�����������Ӧ��ַ 
						sizeof(ptBtn->byKeyCodeUp)					// ���ݵĳ��� 
						);
		}

		ptBtn->wLongCount = 0;										// ������������������
	}
	
}

/*******************************************************************************************************
** ����: ButtonProj,��������ɨ�裩����
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
void ButtonProcess(void)
{
	//�ú����ڶ�ʱ���жϻص������е��ã���ʱ�ж�����Ϊ1ms
	ButtonDetect(&s_tBtnKey1);										// ��� K1 �� 
	ButtonDetect(&s_tBtnKey2);										// ��� K2 ��
	
}




/*******************************************************************************************************
** ����: button_thread_entry,��ȡ�����¼������д���
**------------------------------------------------------------------------------------------------------
** ����: void
** ����: void
********************************************************************************************************/
void button_thread_entry(void *parameter)//�û���Ϣ������ں���
{
	rt_err_t uwRet = RT_EOK;
	uint8_t r_queue;//���ڽ���msg_mq��Ϣ������Ϣ
	
	button_mq = rt_mq_create("button_mq",							//��Ϣ��������
							1,  									//��Ϣ����󳤶�, bytes
							256,									//��Ϣ���е��������(����)
							RT_IPC_FLAG_FIFO						//����ģʽ FIFO
							);
	if(button_mq != RT_NULL) rt_kprintf("button_mq create success\n\n");
	
	while(1)
	{  //��ȡ������Ϣ
		 uwRet = rt_mq_recv(button_mq,
							&r_queue,
							sizeof(r_queue),
							RT_WAITING_FOREVER
							);
		 if(RT_EOK == uwRet){
			 switch(r_queue)//���ݽ��յ�����Ϣ���ݷֱ���д���
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
			 rt_kprintf("���ݽ��մ��󣬴�����룺0x%lx\n\n",uwRet);
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





