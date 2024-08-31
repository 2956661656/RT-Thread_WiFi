#ifndef  __CONFIG_H__
#define  __CONFIG_H__

//�޷������͵Ķ���
#define uchar unsigned char
#define uint unsigned int 
	
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
	
//----------------------------------------


//ȫ�ֺ궨��
#define CPU_INT_DISABLE()  {__set_PRIMASK(1);} //�ر��ж�
#define CPU_INT_ENABLE()   {__set_PRIMASK(0);}  //���ж�

//ȫ�����Ͷ���
typedef enum{FALSE = 0, TRUE = !FALSE}BOOL;
typedef enum{
	MSG_NULL = 0,
	
	MSG_KEY1_PRESS,
	MSG_KEY11_PRESS,
	
	MSG_SECCESSED,
	MSG_FAILED,
	
	MSG_LEN
}MSG_TYPE;

//-------------------------------------
#ifdef MAIN_CONFIG  //ͷ�ļ������C����ʱ�����������ͻ����
	#define EXT
#else 
	#define EXT extern
#endif
	

//-------------------------------------
	
//RTOSͷ�ļ�����
#include <rthw.h>
#include <rtthread.h>
#include "Task.h"		//�Զ����ļ���Task.c�д���̡߳���ʱ����IPC����ش�������
	
//-------------------------------------
	
//����ͷ�ļ�����
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
	
//-------------------------------------
	
//��������ͷ�ļ�����
#include "system.h"
#include "SysTick.h"
#include "Gpio.h"
#include "USart.h"
#include "ChipInfo.h"
#include "Exti.h"
#include "ADCTimer.h"
//#include "Button.h"
#include "Sensor.h"
#include "Lock.h"
#include "ADC.h"
#include "SensorMQ.h"
		
//----------------------------------------
//ȫ�ֱ�������,��ʹ��EXT����
EXT unsigned char 	g_Var;	
EXT rt_sem_t 		usart2_recv_sem;		//����2�����ź������ƿ�ָ��
//EXT rt_mq_t			msg_mq;					//������Ϣ���п��ƿ�ָ��

//----------------------------------------
#endif

//ͨ�ú�������
void weakup_thread_to_get_sensor(void);

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
