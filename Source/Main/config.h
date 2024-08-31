#ifndef  __CONFIG_H__
#define  __CONFIG_H__

//无符号类型的定义
#define uchar unsigned char
#define uint unsigned int 
	
#define uint8 unsigned char
#define uint16 unsigned short int
#define uint32 unsigned int
	
//----------------------------------------


//全局宏定义
#define CPU_INT_DISABLE()  {__set_PRIMASK(1);} //关闭中断
#define CPU_INT_ENABLE()   {__set_PRIMASK(0);}  //打开中断

//全局类型定义
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
#ifdef MAIN_CONFIG  //头文件被多个C调用时，避免变量冲突问题
	#define EXT
#else 
	#define EXT extern
#endif
	

//-------------------------------------
	
//RTOS头文件包含
#include <rthw.h>
#include <rtthread.h>
#include "Task.h"		//自定义文件，Task.c中存放线程、定时器、IPC等相关创建代码
	
//-------------------------------------
	
//常用头文件包含
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
	
//-------------------------------------
	
//外设驱动头文件包含
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
//全局变量定义,请使用EXT修饰
EXT unsigned char 	g_Var;	
EXT rt_sem_t 		usart2_recv_sem;		//串口2控制信号量控制块指针
//EXT rt_mq_t			msg_mq;					//按键消息队列控制块指针

//----------------------------------------
#endif

//通用函数声明
void weakup_thread_to_get_sensor(void);

/********************************************************************************************************
**                            End Of File
********************************************************************************************************/
