#include "config.h"
#include "system.h"


void CpuInit(void)
{
	//CPU�������
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ѡ���2���ж����ȼ�����2λ�����ȼ���2λ�����ȼ�
	
}

void SysInit(void)
{
	
	CpuInit();//����ϵͳ��Ϣ
	
	
	//����������ʼ��
	SysTickInit();//ϵͳ�δ�ʱ����ʼ��
	LedGpioInit();//LED��ʼ��
	//BeepGpioInit();//Beep��������ʼ��
	UsartInit();//���ڳ�ʼ��
	
	//Tim2Init(72, 1000);		//1MHz -- 1S //�ж�����Ϊ1ms�����ڰ���ɨ��
	

	
}
